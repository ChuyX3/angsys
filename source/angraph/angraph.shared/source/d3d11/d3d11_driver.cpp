#include "pch.h"
#include "d3d11/driver.h"

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

d3d11_driver::d3d11_driver()
{
	main_mutex = new core::async::mutex();
	_cull_mode = graphics::cull_mode::back;
	_front_face = graphics::front_face::def;
	_blend_mode = graphics::blend_mode::disable;
	init_driver();
}

d3d11_driver::~d3d11_driver()
{
	close_driver();
}

ANG_IMPLEMENT_CLASSNAME(ang::graphics::d3d11::d3d11_driver);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::d3d11::d3d11_driver);

bool d3d11_driver::is_child_of(type_name_t name)
{
	return name == type_name<d3d11_driver>()
		|| object::is_child_of(name)
		|| idriver::is_child_of(name);
}

bool d3d11_driver::is_kind_of(type_name_t name)const
{
	return name == type_name<d3d11_driver>()
		|| object::is_kind_of(name)
		|| idriver::is_kind_of(name);
}

bool d3d11_driver::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<d3d11_driver>())
	{
		*out = static_cast<d3d11_driver*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (idriver::query_object(name, out))
	{
		return true;
	}
	return false;
}

inline bool SdkLayersAvailable()
{
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
		0,
		D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
		nullptr,                    // Any feature level will do.
		0,
		D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
		nullptr,                    // No need to keep the D3D device reference.
		nullptr,                    // No need to know the feature level.
		nullptr                     // No need to keep the D3D device context reference.
	);
	return SUCCEEDED(hr);
}

bool d3d11_driver::init_driver()
{
	HRESULT hr = S_OK;

	uint createDeviceFlags = 0;
#ifdef _DEBUG
	if (SdkLayersAvailable())
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	uint numFeatureLevels = ARRAYSIZE(featureLevels);

	ID3D11Device * device;
	ID3D11DeviceContext * context;

	hr = D3D11CreateDevice(null, D3D_DRIVER_TYPE_HARDWARE, null, createDeviceFlags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &device, &_feature_level, &context);

	if (FAILED(hr)) {
		return false;
	}

	device->QueryInterface((ID3D11Device2**)&d3d_device);
	context->QueryInterface((ID3D11DeviceContext2**)&d3d_context);
	device->Release();
	context->Release();

	if (D3D11Device() == null || D3D11Context() == null)
	{
		return false;
	}

	{
		IDXGIDevice1* dxgiDevice = nullptr;
		hr = d3d_device->QueryInterface(__uuidof(IDXGIDevice1), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(dxgi_factory.addres_of()));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
	{
		close_driver();
		return false;
	}

	D3D11_RASTERIZER_DESC rs;
	ZeroMemory(&rs, sizeof(rs));
	rs.FillMode = D3D11_FILL_SOLID;
	rs.CullMode = D3D11_CULL_BACK;
	rs.FrontCounterClockwise = TRUE;
	ID3D11RasterizerState* rasterizerState;
	D3D11Device()->CreateRasterizerState(&rs, &rasterizerState);
	D3D11Context()->RSSetState(rasterizerState);
	rasterizerState->Release();


	D3D11_BLEND_DESC bl;

	ZeroMemory(&bl, sizeof(bl));

	bl.AlphaToCoverageEnable = TRUE;
	bl.IndependentBlendEnable = TRUE;

	bl.RenderTarget[0].BlendEnable = TRUE;
	bl.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bl.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bl.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bl.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bl.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bl.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bl.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	d3d_device->CreateBlendState(&bl, &d3d_blend_state);
	//d3d_context->OMSetBlendState(d3d_blend_state, NULL, -1);

	return true;
}

void d3d11_driver::close_driver()
{
	_current_frame_buffer = null;
	d3d_context = null;
	d3d_device = null;
	dxgi_factory = null;
}


isurface_t d3d11_driver::create_surface(platform::icore_view_t view)const
{
	d3d11_surface_t surface = new d3d11_surface(const_cast<d3d11_driver*>(this));
	if (!surface->create(view))
		return null;
	return surface.get();
}

effects::ieffect_library_t d3d11_driver::create_effect_library()const
{
	return new d3d11_effect_library(const_cast<d3d11_driver*>(this));
}

textures::itexture_loader_t d3d11_driver::create_texture_loader()const
{
	return new d3d11_texture_loader(const_cast<d3d11_driver*>(this));
}

buffers::ivertex_buffer_t d3d11_driver::create_vertex_buffer(buffers::buffer_usage_t usage, static_array<reflect::attribute_desc> vertex_desc, wsize vertex_count, static_array<byte> init_data)const
{
	d3d11_vertex_buffer_t buffer = new d3d11_vertex_buffer();
	if (!buffer->create(
		const_cast<d3d11_driver*>(this),
		usage,
		vertex_desc,
		vertex_count,
		init_data
		))
		return null;
	return buffer.get();
}

buffers::iindex_buffer_t d3d11_driver::create_index_buffer(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, static_array<byte> init_data)const
{
	d3d11_index_buffer_t buffer = new d3d11_index_buffer();
	if (!buffer->create(
		const_cast<d3d11_driver*>(this),
		usage,
		index_type,
		index_count,
		init_data
	))
		return null;
	return buffer.get();
}


void d3d11_driver::cull_mode(cull_mode_t value)
{
	if (_cull_mode != value)
	{
		_cull_mode = value;
		D3D11_RASTERIZER_DESC rs;
		ZeroMemory(&rs, sizeof(rs));
		rs.FillMode = D3D11_FILL_SOLID;
		rs.CullMode = D3D11_CULL_MODE(_cull_mode.get() + 1);
		rs.FrontCounterClockwise = _front_face == front_face::counter_clockwise;
		ID3D11RasterizerState* rasterizerState;
		D3D11Device()->CreateRasterizerState(&rs, &rasterizerState);
		D3D11Context()->RSSetState(rasterizerState);
		rasterizerState->Release();
	}
}

cull_mode_t d3d11_driver::cull_mode()const
{
	return _cull_mode;
}

void d3d11_driver::front_face(front_face_t value)
{
	if (_front_face != value)
	{
		_front_face = value;
		D3D11_RASTERIZER_DESC rs;
		ZeroMemory(&rs, sizeof(rs));
		rs.FillMode = D3D11_FILL_SOLID;
		rs.CullMode = D3D11_CULL_MODE(_cull_mode.get() + 1);
		rs.FrontCounterClockwise = _front_face == front_face::counter_clockwise;
		ID3D11RasterizerState* rasterizerState;
		D3D11Device()->CreateRasterizerState(&rs, &rasterizerState);
		D3D11Context()->RSSetState(rasterizerState);
		rasterizerState->Release();
	}
}

front_face_t d3d11_driver::front_face()const
{
	return _front_face;
}

void d3d11_driver::blend_mode(blend_mode_t value)
{
	_blend_mode = value;
	if (value == blend_mode::enable)	
		d3d_context->OMSetBlendState(d3d_blend_state, NULL, -1);
	else
		d3d_context->OMSetBlendState(NULL, NULL, -1);
}

blend_mode_t d3d11_driver::blend_mode()const
{
	return _blend_mode;
}


void d3d11_driver::clear(color_t color)
{
	float _color[] = {
		(float)color.components.red / 255.0f,
		(float)color.components.green / 255.0f ,
		(float)color.components.blue / 255.0f ,
		(float)color.components.alpha / 255.0f
	};

	core::async::scope_locker::lock(main_mutex, [&]() {
		if (!_current_frame_buffer.is_empty())
		{
			for (index i = 0, c = _current_frame_buffer->color_buffer_count(); i < c; ++i)
				D3D11Context()->ClearRenderTargetView(_current_frame_buffer->D3DRenderTargetView(i), _color);
			if (_current_frame_buffer->has_depth_stencil_buffer())
				D3D11Context()->ClearDepthStencilView(_current_frame_buffer->D3DDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
	});
}

void d3d11_driver::clear(maths::float4 const& color)
{
	core::async::scope_locker::lock(main_mutex, [&]() {
		if (!_current_frame_buffer.is_empty())
		{
			for (index i = 0, c = _current_frame_buffer->color_buffer_count(); i < c; ++i)
				D3D11Context()->ClearRenderTargetView(_current_frame_buffer->D3DRenderTargetView(i), (float*)&color);
			if (_current_frame_buffer->has_depth_stencil_buffer())
				D3D11Context()->ClearDepthStencilView(_current_frame_buffer->D3DDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
	});
}

void d3d11_driver::draw(uint count, primitive_t prim)
{
	if (_current_frame_buffer.is_empty())
		return;
	core::async::scope_locker::lock(main_mutex, [&]() {
		if (prim != _primitive)
		{
			_primitive = prim;
			D3D11Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		D3D11Context()->Draw(count, 0);
	});
}

void d3d11_driver::draw_indexed(uint count, primitive_t prim)
{
	if (_current_frame_buffer.is_empty())
		return;
	core::async::scope_locker::lock(main_mutex, [&]() {
		if (prim != _primitive)
		{
			_primitive = prim;
			D3D11Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		D3D11Context()->DrawIndexed(count, 0, 0);
	});
}

void d3d11_driver::bind_frame_buffer(iframe_buffer_t ifb)
{
	d3d11_frame_buffer_t fb = interface_cast<d3d11_frame_buffer>(ifb.get());

	if (fb.get() == _current_frame_buffer.get())
		return;

	_current_frame_buffer = fb;

	if (_current_frame_buffer.get() == null)
	{
		core::async::scope_locker::lock(main_mutex, [&]() {
			D3D11Context()->OMSetRenderTargets(0, NULL, NULL);
		});
	}
	else
	{
		core::async::scope_locker::lock(main_mutex, [&]() {
			auto render_targets = _current_frame_buffer->D3DRenderTargetView(0);
			D3D11Context()->OMSetDepthStencilState(_current_frame_buffer->D3DDepthStencilState(), 1);
			D3D11Context()->OMSetRenderTargets(1, &render_targets, _current_frame_buffer->D3DDepthStencilView());

			auto size = _current_frame_buffer->dimentions();
			// Setup the viewport
			D3D11_VIEWPORT vp;
			vp.Width = size.width;
			vp.Height = size.height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			D3D11Context()->RSSetViewports(1, &vp);
		});	
	}
}

void d3d11_driver::bind_shaders(effects::ishaders_t sh)
{
	d3d11_shaders_t shaders = interface_cast<d3d11_shaders>(sh.get());

	if (shaders.get() == _current_shaders.get())
		return;
	_current_shaders = shaders;
	if (_current_shaders.get() == null)
	{
		core::async::scope_locker::lock(main_mutex, [&]() {
			D3D11Context()->IASetInputLayout(NULL);
			D3D11Context()->VSSetShader(NULL, NULL, 0);
			D3D11Context()->PSSetShader(NULL, NULL, 0);
			D3D11Context()->VSSetConstantBuffers(0, 0, NULL);
			D3D11Context()->PSSetConstantBuffers(0, 0, NULL);
			D3D11Context()->PSSetSamplers(0, 0, NULL);
		});

	}
	else
	{
		core::async::scope_locker::lock(main_mutex, [&]() {
			shaders->use_shaders(this);
		});
	}
}

void d3d11_driver::bind_texture(textures::itexture_t _tex, index idx)
{
	d3d11_texture_t tex = interface_cast<d3d11_texture>(_tex.get());
	if (tex.is_empty())
	{
		core::async::scope_locker::lock(main_mutex, [&]() {
			D3D11Context()->PSSetShaderResources(idx, 0, NULL);
		});		
	}
	else
	{
		core::async::scope_locker::lock(main_mutex, [&]() {
			tex->use_texture(this, idx);
		});	
	}
}

void d3d11_driver::bind_index_buffer(buffers::iindex_buffer_t buff)
{
	d3d11_index_buffer_t buffer = interface_cast<d3d11_index_buffer>(buff.get());
	if (buffer.is_empty())
	{
		core::async::scope_locker::lock(main_mutex, [&]() {
			D3D11Context()->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);
		});
		
	}
	else
	{
		core::async::scope_locker::lock(main_mutex, [&]() {
			buffer->use_buffer(this);
		});
	}
}

void d3d11_driver::bind_vertex_buffer(buffers::ivertex_buffer_t buff)
{
	d3d11_vertex_buffer_t buffer = interface_cast<d3d11_vertex_buffer>(buff.get());
	if (buffer.is_empty())
	{
		core::async::scope_locker::lock(main_mutex, [&]() {
			D3D11Context()->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
		});
	}
	else
	{
		core::async::scope_locker::lock(main_mutex, [&]() {
			buffer->use_buffer(this);
		});
	}
}

core::async::mutex_t d3d11_driver::driver_guard()const
{
	return main_mutex;
}