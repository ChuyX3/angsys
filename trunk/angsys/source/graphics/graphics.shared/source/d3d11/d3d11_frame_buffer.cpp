#include "pch.h"
#include "d3d11/d3d11_driver.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

//#define MY_TYPE ang::graphics::d3d11::d3d11_frame_buffer
//#include <ang/inline/object_wrapper_specialization.inl>
//#undef MY_TYPE

d3d11_frame_buffer::d3d11_frame_buffer(d3d11_driver* driver)
	: m_parent_driver(driver)
{
	
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::d3d11_frame_buffer);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::d3d11_frame_buffer, object, iframe_buffer, resources::iresource);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::d3d11_frame_buffer, object, iframe_buffer, resources::iresource);

void d3d11_frame_buffer::dispose()
{
	close();
}

resources::iresource_t d3d11_frame_buffer::resource()const { return const_cast<d3d11_frame_buffer*>(this); }
resources::resource_type_t d3d11_frame_buffer::resource_type()const { return resources::resource_type::frame_buffer; }
string d3d11_frame_buffer::resource_sid()const { return (cstr_t)m_resource_sid; }
void d3d11_frame_buffer::resource_sid(cstr_t sid) { m_resource_sid = sid; }
effects::ieffect_t d3d11_frame_buffer::to_effect() { return null; }
effects::ishaders_t d3d11_frame_buffer::to_shaders() { return null; }
textures::itexture_t d3d11_frame_buffer::to_texture() { return null; }
iframe_buffer_t d3d11_frame_buffer::to_frame_buffer() { return const_cast<d3d11_frame_buffer*>(this); }
buffers::iindex_buffer_t d3d11_frame_buffer::to_index_buffer() { return null; }
buffers::ivertex_buffer_t d3d11_frame_buffer::to_vertex_buffer() { return null; }

bool d3d11_frame_buffer::create(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<uint> dimentions, string sid)
{
	if (color_format.size() == 0)
		return false;

	d3d11_driver_t driver = m_parent_driver.lock();

	if (driver.is_empty())
		return false;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Height = dimentions.height;
	desc.Width = dimentions.width;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc;
	ZeroMemory(&srvdesc, sizeof(srvdesc));
	srvdesc.Format = DXGI_FORMAT_UNKNOWN;
	srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvdesc.Texture2D.MipLevels = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	srvdesc.Texture2D.MostDetailedMip = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvdesc;
	rtvdesc.Format = desc.Format;
	rtvdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvdesc.Texture2D.MipSlice = 0;

	HRESULT hr;
	com_wrapper<ID3D11Texture2D> texture = null;
	com_wrapper<ID3D11ShaderResourceView> view = null;
	com_wrapper<ID3D11RenderTargetView> render_target = null;
	float clear_color[4] = { 0,0,0,0 };

	for (index i = 0, c = min((uint)color_format.size(), 4U); i < c; ++i)
	{
		view = null;
		texture = null;
		render_target = null;
		desc.Format = d3d11::traslate_format_dx(color_format[i]);
		srvdesc.Format = desc.Format;
		rtvdesc.Format = desc.Format;
		hr = driver->D3D11Device()->CreateTexture2D(&desc, null, &texture);
		if (FAILED(hr))
		{
			close();
			return false;
		}
		hr = driver->D3D11Device()->CreateShaderResourceView(texture.get(), &srvdesc, &view);
		if (FAILED(hr))
		{
			close();
			return false;
		}

		hr = driver->D3D11Device()->CreateRenderTargetView(texture.get(), &rtvdesc, &render_target);
		if (FAILED(hr))
		{
			close();
			return false;
		}

		driver->D3D11Context()->ClearRenderTargetView(render_target, clear_color);
		m_d3d_render_targets += render_target;

		d3d11_texture_t _texture = new d3d11_texture();
		_texture->attach(view);
		m_d3d_color_textures += _texture;

	}

	m_resource_sid = sid;

	if (depth_stencil_format == textures::tex_format::null)
		return true;
	
	com_wrapper<ID3D11Texture2D> backBuffer = nullptr;
	desc.Format = d3d11::traslate_format_dx(depth_stencil_format);
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;

	hr = driver->D3D11Device()->CreateTexture2D(&desc, null, &backBuffer);
	if (FAILED(hr))
	{
		close();
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC dsdesc;
	ZeroMemory(&dsdesc, sizeof(dsdesc));
	dsdesc.DepthEnable = true;
	dsdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsdesc.DepthFunc = D3D11_COMPARISON_LESS;

	dsdesc.StencilEnable = true;
	dsdesc.StencilReadMask = 0xFF;
	dsdesc.StencilWriteMask = 0xFF;

	dsdesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsdesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dsdesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsdesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsdesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = driver->D3D11Device()->CreateDepthStencilState(&dsdesc, &m_d3d_depth_stencil_state);
	if (FAILED(hr))
	{
		close();
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvdesc;
	ZeroMemory(&dsvdesc, sizeof(dsvdesc));
	dsvdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvdesc.Texture2D.MipSlice = 0;

	hr = driver->D3D11Device()->CreateDepthStencilView(backBuffer, &dsvdesc, &m_d3d_depth_stencil);
	
	if (FAILED(hr))
	{
		close();
		return false;
	}

	return true;
}

bool d3d11_frame_buffer::create(d3d11_surface_t surface)
{
	if (surface.get() == null)
		return false;

	d3d11_driver_t driver = m_parent_driver.lock();
	if (driver.is_empty())
		return false;
	
	close();
	m_dimentions = surface->surface_size();
	HRESULT hr;

	com_wrapper<ID3D11Texture2D> pBackBuffer = nullptr;
	hr = surface->DXGISwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>((ID3D11Texture2D**)&pBackBuffer));
	if (FAILED(hr))
		return false;

	com_wrapper<ID3D11RenderTargetView> render_target;
	D3D11_TEXTURE2D_DESC desc;
	m_d3d_color_textures = null;
	hr = driver->D3D11Device()->CreateRenderTargetView(pBackBuffer, nullptr, &render_target);
	pBackBuffer->GetDesc(&desc);
	if (FAILED(hr))
		return false;

	m_dimentions.width = desc.Width;
	m_dimentions.height = desc.Height;

	m_d3d_render_targets += render_target;
	d3d11_texture_t tex = new d3d11_texture();
	if (tex->attach(pBackBuffer.get(), driver))
		m_d3d_color_textures += tex;
	
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = desc.Width;
	depthBufferDesc.Height = desc.Height;
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

	hr = driver->D3D11Device()->CreateDepthStencilState(&depthStencilDesc, &m_d3d_depth_stencil_state);
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

	hr = driver->D3D11Device()->CreateDepthStencilView(pBackBuffer, &depthStencilViewDesc, &m_d3d_depth_stencil);
	m_d3d_depth_stencil_texture = new d3d11_texture();
	m_d3d_depth_stencil_texture->attach(pBackBuffer.get(), driver);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		close();
		return false;
	}
	m_resource_sid = "render_target_default"_r;
	return true;
}

bool d3d11_frame_buffer::create(array_view<d3d11_texture_t> textures, d3d11_texture_t depth_stencil, string sid)
{
	if (textures.size() == 0)
		return false;

	d3d11_driver_t driver = m_parent_driver.lock();
	if (driver.is_empty())
		return false;

	collections::vector<com_wrapper<ID3D11RenderTargetView>> render_targets;
	collections::vector<d3d11_texture_t> color_textures;

	HRESULT hr;
	size<uint> prev_dim;

	for (d3d11_texture_t resource : textures)
	{
		if (resource->tex_type() != textures::tex_type::tex2D
			|| !resource->buffer_bind_flag().is_active<buffers::buffer_bind_flag::render_target>())
			continue;

		com_wrapper<ID3D11Texture2D> pBackBuffer = resource->D3D11Texture2D();

		com_wrapper<ID3D11RenderTargetView> render_target;
		D3D11_TEXTURE2D_DESC desc;
		hr = driver->D3D11Device()->CreateRenderTargetView(pBackBuffer, nullptr, &render_target);
		pBackBuffer->GetDesc(&desc);
		pBackBuffer = null;
		if (FAILED(hr))
			continue;

		render_targets += render_target;
		color_textures += resource;

		if (prev_dim.width == 0 && prev_dim.height == 0)
			prev_dim = { desc.Width, desc.Height };
		else if (prev_dim.width != desc.Width || prev_dim.height != desc.Height)
		{
			throw_exception((dword)except_code::invalid_param, "d3d11_frame_buffer::create : textures passed as argument must have same dimentions"_r);
			return false;
		}
	}

	m_d3d_render_targets = ang::move(render_targets);
	m_d3d_color_textures = ang::move(color_textures);
	m_dimentions = { prev_dim.width, prev_dim.height };

	m_d3d_depth_stencil = null;
	m_d3d_depth_stencil_state = null;
	m_d3d_depth_stencil_texture = null;

	m_resource_sid = sid;

	if (depth_stencil.is_empty() || depth_stencil->tex_type() != textures::tex_type::tex2D
		|| !depth_stencil->buffer_bind_flag().is_active<buffers::buffer_bind_flag::depth_stencil>())
		return true;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	com_wrapper<ID3D11Texture2D> pBackBuffer = depth_stencil->D3D11Texture2D();
	pBackBuffer->GetDesc(&depthBufferDesc);

	if (prev_dim.width != depthBufferDesc.Width || prev_dim.height != depthBufferDesc.Height)
		return true;

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

	hr = driver->D3D11Device()->CreateDepthStencilState(&depthStencilDesc, &m_d3d_depth_stencil_state);
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

	hr = driver->D3D11Device()->CreateDepthStencilView(pBackBuffer, &depthStencilViewDesc, &m_d3d_depth_stencil);

	m_d3d_depth_stencil_texture = new d3d11_texture();
	m_d3d_depth_stencil_texture->attach(pBackBuffer.get(), driver);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		close();
		return false;
	}
	return true;
}

bool d3d11_frame_buffer::update(d3d11_texture_t resource)
{
	if (resource.get() == null)
		return false;
	if (!m_d3d_color_textures.is_empty() &&
		resource->D3D11Resource() == m_d3d_color_textures[0]->D3D11Resource())
		return true;

	d3d11_driver_t driver = m_parent_driver.lock();
	if (driver.is_empty())
		return false;

	if (resource->tex_type() != textures::tex_type::tex2D
		|| !resource->buffer_bind_flag().is_active<buffers::buffer_bind_flag::render_target>())
		return false;


	HRESULT hr;

	com_wrapper<ID3D11Texture2D> pBackBuffer = resource->D3D11Texture2D();

	com_wrapper<ID3D11RenderTargetView> render_target;
	D3D11_TEXTURE2D_DESC desc;
	hr = driver->D3D11Device()->CreateRenderTargetView(pBackBuffer, nullptr, &render_target);
	pBackBuffer->GetDesc(&desc);
	pBackBuffer = null;
	if (FAILED(hr))
		return false;

	bool update_depth_stencil = false;
	if(m_dimentions.width != (float)desc.Width || m_dimentions.height != (float)desc.Height)
		update_depth_stencil = true;

	m_dimentions.width = (float)desc.Width;
	m_dimentions.height = (float)desc.Height;

	m_d3d_render_targets.reset();
	m_d3d_render_targets += render_target;
	m_d3d_color_textures.reset();
	m_d3d_color_textures += resource;

	if (update_depth_stencil)
	{
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		depthBufferDesc.Width = desc.Width;
		depthBufferDesc.Height = desc.Height;
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

		hr = driver->D3D11Device()->CreateDepthStencilState(&depthStencilDesc, m_d3d_depth_stencil_state.addres_for_init());
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

		hr = driver->D3D11Device()->CreateDepthStencilView(pBackBuffer, &depthStencilViewDesc, m_d3d_depth_stencil.addres_for_init());
		m_d3d_depth_stencil_texture = new d3d11_texture();
		m_d3d_depth_stencil_texture->attach(pBackBuffer.get(), driver);
		pBackBuffer->Release();
		if (FAILED(hr))
		{
			close();
			return false;
		}
	}
	return true;
}

bool  d3d11_frame_buffer::try_resize(size<uint> size)
{
	return false;
}

bool d3d11_frame_buffer::close()
{
	m_d3d_render_targets = null;
	m_d3d_depth_stencil = null;
	m_d3d_depth_stencil_state = null;
	m_d3d_color_textures = null;
	m_resource_sid = null;
	return true;
}



graphics::size<uint> d3d11_frame_buffer::dimentions()const
{
	return m_dimentions;
}

uint d3d11_frame_buffer::color_buffer_count()const
{
	return m_d3d_render_targets.is_empty() ? 0 : (uint)m_d3d_render_targets->counter();
}

textures::itexture_t d3d11_frame_buffer::color_buffer(index idx)const
{
	if (m_d3d_color_textures.is_empty() || m_d3d_color_textures->size() <= idx)
		return null;
	return m_d3d_color_textures[idx].get();
}

textures::itexture_t d3d11_frame_buffer::depth_stencil_buffer()const
{
	return m_d3d_depth_stencil_texture.get();
}

#endif
