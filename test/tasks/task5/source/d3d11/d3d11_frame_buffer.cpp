#include "pch.h"
#include "d3d11/driver.h"

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

d3d11_frame_buffer::d3d11_frame_buffer(d3d11_driver* driver)
	: _parent_driver(driver)
{
	
}

d3d11_frame_buffer::~d3d11_frame_buffer()
{
	
}

ANG_IMPLEMENT_CLASSNAME(ang::graphics::d3d11::d3d11_frame_buffer);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::d3d11::d3d11_frame_buffer);

bool d3d11_frame_buffer::is_child_of(type_name_t name)
{
	return name == type_name<d3d11_frame_buffer>()
		|| object::is_child_of(name)
		|| iframe_buffer::is_child_of(name);
}

bool d3d11_frame_buffer::is_kind_of(type_name_t name)const
{
	return name == type_name<d3d11_frame_buffer>()
		|| object::is_kind_of(name)
		|| iframe_buffer::is_kind_of(name);
}

bool d3d11_frame_buffer::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<d3d11_frame_buffer>())
	{
		*out = static_cast<d3d11_frame_buffer*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (iframe_buffer::query_object(name, out))
	{
		return true;
	}
	return false;
}

bool d3d11_frame_buffer::create(d3d11_surface_t surface)
{
	if (surface.get() == null)
		return false;

	d3d11_driver_t driver = _parent_driver.lock<d3d11_driver>();
	if (driver.is_empty())
		return false;
	
	close();
	_dimentions = surface->surface_size();
	HRESULT hr;

	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = surface->DXGISwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return false;

	com_wrapper<ID3D11RenderTargetView> render_target;

	hr = driver->D3D11Device()->CreateRenderTargetView(pBackBuffer, nullptr, &render_target);
	pBackBuffer->Release();
	if (FAILED(hr))
		return false;

	d3d_render_target += render_target;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = (uint)_dimentions.width;
	depthBufferDesc.Height = (uint)_dimentions.height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	hr = driver->D3D11Device()->CreateTexture2D(&depthBufferDesc, NULL, &pBackBuffer);
	if (FAILED(hr))
	{
		close();
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = driver->D3D11Device()->CreateDepthStencilState(&depthStencilDesc, &d3d_depth_stencil_state);
	if (FAILED(hr))
	{
		close();
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = driver->D3D11Device()->CreateDepthStencilView(pBackBuffer, &depthStencilViewDesc, &d3d_depth_stencil);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		close();
		return false;
	}

	return true;
}

bool d3d11_frame_buffer::close()
{
	d3d_render_target = null;
	d3d_depth_stencil = null;
	d3d_depth_stencil_state = null;
	d3d_texture2D = null;
	return true;
}



foundation::size<float> d3d11_frame_buffer::dimentions()const
{
	return _dimentions;
}

uint d3d11_frame_buffer::color_buffer_count()const
{
	return d3d_render_target.is_empty() ? 0 : d3d_render_target->counter();
}

bool d3d11_frame_buffer::has_depth_stencil_buffer()const
{
	return !d3d_depth_stencil.is_empty();
}