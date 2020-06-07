#include "pch.h"
#include "d3d11/d3d11_driver.h"

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;


namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			inline bool sdk_layers_available()
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
		}
	}	
}

d3d11_driver::d3d11_driver()
{
	m_cull_mode = graphics::cull_mode::back;
	m_front_face = graphics::front_face::def;
	m_blend_mode = graphics::blend_mode::disable;
	m_async_worker = core::async::thread::create_dispatcher_thread();
}

void d3d11_driver::dispose()
{
	close_driver();
	m_async_worker->exit();
	m_async_worker = null;
}

void d3d11_driver::set_file_system(core::files::ifile_system_t fs)
{
	m_mutex.lock();
	m_fs = fs;
	m_mutex.unlock();
}

bool d3d11_driver::init_driver(platform::icore_view_t, long64 adapter_id)
{
	HRESULT hr = S_OK;
	// This flag adds support for surfaces with a different color channel ordering
	// than the API default. It is required for compatibility with Direct2D.
	uint createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	UINT createFlags = 0;
#ifdef _DEBUG
	if (sdk_layers_available())
	{
		createFlags |= DXGI_CREATE_FACTORY_DEBUG;
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif

	LUID id = reinterpret_cast<LUID&>(adapter_id);
	if ((id.HighPart != 0) || (id.LowPart != 0))
	{	
		com_wrapper<IDXGIFactory1> dxgiFactory;
		hr = CreateDXGIFactory2(createFlags, IID_PPV_ARGS((IDXGIFactory1**)&dxgiFactory));
	
		if (FAILED(hr))
			return false;

		com_wrapper<IDXGIFactory4> dxgiFactory4;
		hr = dxgiFactory.as(&dxgiFactory4);
		dxgiFactory4->EnumAdapterByLuid(id, IID_PPV_ARGS((IDXGIAdapter3**)&m_dxgi_adapter));
	}
	else
	{
		m_dxgi_adapter.reset();
	}


	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	uint numFeatureLevels = ARRAYSIZE(featureLevels);

	ID3D11Device * device;
	ID3D11DeviceContext * context;

	if (m_dxgi_adapter != null)
	{
		hr = D3D11CreateDevice(m_dxgi_adapter.get(), D3D_DRIVER_TYPE_UNKNOWN, null, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &device, &m_feature_level, &context);
	}
	else
	{
		hr = D3D11CreateDevice(m_dxgi_adapter.get(), D3D_DRIVER_TYPE_HARDWARE, null, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &device, &m_feature_level, &context);
	}

	if (FAILED(hr)) {
		hr = D3D11CreateDevice(null, D3D_DRIVER_TYPE_WARP, null, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &device, &m_feature_level, &context);

		if (FAILED(hr)) {
			return false;
		}
	}

	switch (m_feature_level)
	{
	case D3D_FEATURE_LEVEL_12_1:
		m_max_version = 12;
		m_min_version = 1;
		m_vs_model = "vs_5_0";
		m_ps_model = "ps_5_0";
		break;
	case D3D_FEATURE_LEVEL_12_0:
		m_max_version = 12;
		m_min_version = 0;
		m_vs_model = "vs_5_0";
		m_ps_model = "ps_5_0";
		break;
	case D3D_FEATURE_LEVEL_11_1:
		m_max_version = 11;
		m_min_version = 1;
		m_vs_model = "vs_5_0";
		m_ps_model = "ps_5_0";
		break;
	case D3D_FEATURE_LEVEL_11_0:
		m_max_version = 11;
		m_min_version = 0;
		m_vs_model = "vs_5_0";
		m_ps_model = "ps_5_0";
		break;
	case D3D_FEATURE_LEVEL_10_1:
		m_max_version = 10;
		m_min_version = 1;
		m_vs_model = "vs_4_1";
		m_ps_model = "ps_4_1";
		break;
	case D3D_FEATURE_LEVEL_10_0:
		m_max_version = 10;
		m_min_version = 0;
		m_vs_model = "vs_4_0";
		m_ps_model = "ps_4_0";
		break;
	case D3D_FEATURE_LEVEL_9_3:
		m_max_version = 9;
		m_min_version = 3;
		m_vs_model = "vs_4_0_level_9_3";
		m_ps_model = "ps_4_0_level_9_3";
		break;
	case D3D_FEATURE_LEVEL_9_2:
		m_max_version = 9;
		m_min_version = 2;
		m_vs_model = "vs_4_0_level_9_1";
		m_ps_model = "ps_4_0_level_9_1";
		break;
	case D3D_FEATURE_LEVEL_9_1:
		m_max_version = 9;
		m_min_version = 1;
		m_vs_model = "vs_4_0_level_9_1";
		m_ps_model = "ps_4_0_level_9_1";
		break;
	}

	device->QueryInterface((ID3D11Device2**)&m_d3d_device);
	context->QueryInterface((ID3D11DeviceContext2**)&m_d3d_context);
	device->Release();
	context->Release();

	com_wrapper<IDXGIDevice3> dxgiDevice = nullptr;

	if (D3D11Device() == null || D3D11Context() == null)
	{
		return false;
	}
	
	hr = m_d3d_device->QueryInterface(__uuidof(IDXGIDevice3), reinterpret_cast<void**>(dxgiDevice.addres_of()));
	if (SUCCEEDED(hr))
	{
		IDXGIAdapter* adapter = nullptr;
		hr = dxgiDevice->GetAdapter(&adapter);
		if (SUCCEEDED(hr))
		{
			m_dxgi_adapter.reset();
			adapter->QueryInterface((IDXGIAdapter3**)&m_dxgi_adapter);
			hr = adapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(m_dxgi_factory.addres_of()));
			adapter->Release();
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
	bl.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_d3d_device->CreateBlendState(&bl, &m_d3d_blend_state);
	//m_d3d_context->OMSetBlendState(m_d3d_blend_state, NULL, -1);

	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
#if defined(_DEBUG)
	// If the project is in a debug build, enable Direct2D debugging via SDK Layers.
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	// Initialize the Direct2D Factory.
	/*
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory2),
		&options,
		(void**)m_d2d_factory.addres_of()
	);

	if (FAILED(hr))
	{
		close_driver();
		return false;
	}

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory2),
		(IUnknown**)m_dwrite_factory.addres_of()
	);

	if (FAILED(hr))
	{
		close_driver();
		return false;
	}

	hr = m_d2d_factory->CreateDevice(dxgiDevice, &m_d2d_device);
	if (FAILED(hr))
	{
		close_driver();
		return false;
	}
	hr = m_d2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2d_context);
	if (FAILED(hr))
	{
		close_driver();
		return false;
	}
	*/
	return true;
}

void d3d11_driver::close_driver()
{
	m_d3d_blend_state = null;
	m_current_frame_buffer = null;
	m_current_technique = null;

	//m_d2d_context = null;
	//m_d2d_device = null;
	//m_d2d_factory = null;

	m_current_frame_buffer = null;
	m_d3d_context = null;
	m_d3d_device = null;
	m_dxgi_factory = null;
}

graph_driver_type_t d3d11_driver::graph_driver_type()const { return graphics::graph_driver_type::DirectX11; }

bool d3d11_driver::matrix_transpose_needed()const { return true; }

bool d3d11_driver::vr_extension_support()const { return false; }

ulong d3d11_driver::min_version()const
{
	return m_min_version;
}

ulong d3d11_driver::max_version()const
{
	return m_max_version;
}

isurface_t d3d11_driver::create_surface(platform::icore_view_t view)const
{
	d3d11_surface_t surface = new d3d11_surface(const_cast<d3d11_driver*>(this));
	if (!surface->create(view))
		return null;
	return surface.get();
}

ifactory_t d3d11_driver::get_factory()const
{ 
	return const_cast<d3d11_driver*>(this);
}

optional<buffers::ivertex_buffer> d3d11_driver::create_vertex_buffer(buffers::buffer_usage_t usage, vector<reflect::attribute_desc> vertex_desc, wsize vertex_count, ibuffer_t buff, string sid)const
{
	array_view<byte> init_data = buff.is_empty() ? to_array((byte*)null, (byte*)null) : to_array((byte*)buff->buffer_ptr(), (byte*)buff->buffer_ptr() + buff->buffer_size());

	d3d11_vertex_buffer_t buffer = new d3d11_vertex_buffer();
	if (!buffer->create(
		const_cast<d3d11_driver*>(this),
		usage,
		vertex_desc,
		vertex_count,
		init_data,
		sid
		))
		return error(error_code::unknown);
	return buffer.get();
}

optional<buffers::iindex_buffer> d3d11_driver::create_index_buffer(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t buff, string sid)const
{
	array_view<byte> init_data = buff.is_empty() ? to_array((byte*)null, (byte*)null) : to_array((byte*)buff->buffer_ptr(), (byte*)buff->buffer_ptr() + buff->buffer_size());
	d3d11_index_buffer_t buffer = new d3d11_index_buffer();
	if (!buffer->create(
		const_cast<d3d11_driver*>(this),
		usage,
		index_type,
		index_count,
		init_data,
		sid
	))
		return error(error_code::unknown);
	return buffer.get();
}

optional<textures::itexture> d3d11_driver::create_texture(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, graphics::size3d<uint> dimentions, string sid)const
{
	d3d11_texture_t text = new d3d11_texture();
	if (!text->create(
		const_cast<d3d11_driver*>(this),
		type,
		color_format,
		usage,
		flags,
		dimentions,
		sid
	))
		return error(error_code::unknown);
	return text.get();
}

optional<textures::itexture> d3d11_driver::create_texture(unknown_t tex_handle, string sid)const
{
	if (tex_handle == null)
		return error(error_code::invalid_param);

	com_wrapper<ID3D11Resource> resource;
	IUnknown* unk = reinterpret_cast<IUnknown*>(tex_handle);

	unk->QueryInterface(resource.addres_for_init());
	d3d11_texture_t text = new d3d11_texture();
	if (!text->attach(resource, const_cast<d3d11_driver*>(this),sid))
		return error(error_code::unknown);
	return text.get();
}

optional<textures::itexture> d3d11_driver::load_texture(text::string file, textures::tex_type_t type, string sid)const
{
	d3d11_texture_t tex = new d3d11_texture();
	if (!tex->load(const_cast<d3d11_driver*>(this), file, type, sid))
		return error(error_code::unknown);
	return tex.get();
}

optional<textures::itexture> d3d11_driver::load_texture(array_view<text::string> files, textures::tex_type_t type, string sid)const
{
	return error(error_code::unsupported);
}

optional<iframe_buffer> d3d11_driver::create_frame_buffer(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<uint> dimentions, bool is_stereo, string sid)const
{ 
	d3d11_frame_buffer_t buffer = new d3d11_frame_buffer(const_cast<d3d11_driver*>(this));
	if (!buffer->create(
		color_format, 
		depth_stencil_format,
		dimentions,
		is_stereo,
		sid
	))
		return error(error_code::unknown);
	return buffer.get();
}

optional<iframe_buffer> d3d11_driver::create_frame_buffer(array_view<textures::itexture_t> color_tex, textures::itexture_t depth_stencil, string sid)const
{
	d3d11_frame_buffer_t buffer = new d3d11_frame_buffer(const_cast<d3d11_driver*>(this));
	vector<d3d11_texture_t> textures;
	for (textures::itexture_t tex : color_tex)
		textures += interface_cast<d3d11_texture>(color_tex.get());
	if (!buffer->create(
		(array_view<d3d11_texture_t>)textures,
		interface_cast<d3d11_texture>(depth_stencil.get()),
		sid
	))
		return error(error_code::unknown);
	return buffer.get();
}

optional<effects::ishaders> d3d11_driver::compile_shaders(string vertex_shader, string pixel_shader, string sid)const
{
	astring log;
	d3d11_shaders_t shaders = new d3d11_shaders();
	if(!shaders->load(
		const_cast<d3d11_driver*>(this),
		vertex_shader,
		pixel_shader,
		sid,
		&log))
		return error(log);
	
	return shaders.get();
}

optional<effects::ishaders> d3d11_driver::compile_shaders(effects::shader_info_t const& vertex_shader, effects::shader_info_t const& pixel_shader, string sid)const
{
	astring log;
	d3d11_shaders_t shaders = new d3d11_shaders();
	if (!shaders->load(
		const_cast<d3d11_driver*>(this),
		vertex_shader,
		pixel_shader,
		sid,
		&log))
		return error(log);
	return shaders.get();
}

core::async::iasync_op<buffers::ivertex_buffer> d3d11_driver::create_vertex_buffer_async(
	buffers::buffer_usage_t usage,
	vector<reflect::attribute_desc> vertex_desc,
	wsize vertex_count,
	ibuffer_t init_data,
	string sid)const
{
	return create_task<optional<buffers::ivertex_buffer>>([=](core::async::iasync_op<buffers::ivertex_buffer>, d3d11_driver_t driver)
	{
		return driver->create_vertex_buffer(usage, vertex_desc, vertex_count, init_data, sid);
	});
}

core::async::iasync_op<buffers::iindex_buffer> d3d11_driver::create_index_buffer_async(
	buffers::buffer_usage_t usage,
	reflect::var_type_t index_type, 
	wsize index_count,
	ibuffer_t init_data,
	string sid)const
{
	return create_task<optional<buffers::iindex_buffer>>([=](core::async::iasync_op<buffers::iindex_buffer>, d3d11_driver_t driver)
	{
		return driver->create_index_buffer(usage, index_type, index_count, init_data, sid);
	});
}

core::async::iasync_op<textures::itexture> d3d11_driver::create_texture_async(
	textures::tex_type_t type, 
	textures::tex_format_t color_format,
	buffers::buffer_usage_t usage, 
	buffers::buffer_bind_flag_t flags, 
	size3d<uint> dimentions, 
	string sid)const
{
	return create_task<optional<textures::itexture>>([=](core::async::iasync_op<textures::itexture>, d3d11_driver_t driver)
	{
		return driver->create_texture(type, color_format, usage, flags, dimentions, sid);
	});
}

core::async::iasync_op<textures::itexture> d3d11_driver::create_texture_async(
	unknown_t tex_handle, 
	string sid)const
{
	return create_task<optional<textures::itexture>>([=](core::async::iasync_op<textures::itexture>, d3d11_driver_t driver)
	{
		return driver->create_texture(tex_handle, sid);
	});
}

core::async::iasync_op<textures::itexture> d3d11_driver::load_texture_async(
	text::string file, 
	textures::tex_type_t type,
	string sid)const
{
	return create_task<optional<textures::itexture>>([=](core::async::iasync_op<textures::itexture>, d3d11_driver_t driver)
	{
		return driver->load_texture(file, type, sid);
	});
}

core::async::iasync_op<textures::itexture> d3d11_driver::load_texture_async(
	array_view<text::string> files_,
	textures::tex_type_t type,
	string sid)const
{
	array<string> files = files_;
	return create_task<optional<textures::itexture>>([=](core::async::iasync_op<textures::itexture>, d3d11_driver_t driver)
	{
		return driver->load_texture(files, type, sid);
	});
}

core::async::iasync_op<iframe_buffer> d3d11_driver::create_frame_buffer_async(
	array_view<textures::tex_format_t> color_format_,
	textures::tex_format_t depth_stencil_format,
	size<uint> dimentions,
	bool is_stereo,
	string sid)const
{
	array<textures::tex_format_t> color_format = color_format_;
	return create_task<optional<iframe_buffer>>([=](core::async::iasync_op<iframe_buffer>, d3d11_driver_t driver)
	{
		return driver->create_frame_buffer(color_format, depth_stencil_format, dimentions, is_stereo, sid);
	});
}

core::async::iasync_op<iframe_buffer> d3d11_driver::create_frame_buffer_async(
	array_view<textures::itexture_t> color_tex_,
	textures::itexture_t depth_stencil_format,
	string sid)const
{
	array<textures::itexture_t> color_tex = color_tex_;
	return create_task<optional<iframe_buffer>>([=](core::async::iasync_op<iframe_buffer>, d3d11_driver_t driver)
	{
		return driver->create_frame_buffer(color_tex, depth_stencil_format, sid);
	});
}

core::async::iasync_op<effects::ishaders> d3d11_driver::compile_shaders_async(
	string vertex_shader,
	string pixel_shader,
	string sid)const
{
	return create_task<optional<effects::ishaders>>([=](core::async::iasync_op<effects::ishaders>, d3d11_driver_t driver)
	{
		return driver->compile_shaders(vertex_shader, pixel_shader, sid);
	});
}

core::async::iasync_op<effects::ishaders> d3d11_driver::compile_shaders_async(
	effects::shader_info_t const& vertex_shader,
	effects::shader_info_t const& pixel_shader,
	string sid)const
{
	return create_task<optional<effects::ishaders>>([=](core::async::iasync_op<effects::ishaders>, d3d11_driver_t driver)
	{
		return driver->compile_shaders(vertex_shader, pixel_shader, sid);
	});
}


void d3d11_driver::viewport(box<float> value)
{
	D3D11_VIEWPORT vp;
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	m_viewport = value;
	vp.Width = m_viewport.width();
	vp.Height = m_viewport.height();
	vp.MinDepth = m_viewport.front;
	vp.MaxDepth = m_viewport.back;
	vp.TopLeftX = m_viewport.left;
	vp.TopLeftY = m_viewport.top;
	D3D11Context()->RSSetViewports(1, &vp);
}

box<float> d3d11_driver::viewport()const
{
	return m_viewport;
}

void d3d11_driver::cull_mode(cull_mode_t value)
{
	if (m_cull_mode != value)
	{
		core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
		m_cull_mode = value;
		D3D11_RASTERIZER_DESC rs;
		ZeroMemory(&rs, sizeof(rs));
		rs.FillMode = D3D11_FILL_SOLID;
		rs.CullMode = D3D11_CULL_MODE((int)m_cull_mode.get() + 1);
		rs.FrontCounterClockwise = m_front_face == front_face::counter_clockwise;
		ID3D11RasterizerState* rasterizerState;
		D3D11Device()->CreateRasterizerState(&rs, &rasterizerState);
		D3D11Context()->RSSetState(rasterizerState);		
		rasterizerState->Release();
	}
}

cull_mode_t d3d11_driver::cull_mode()const
{
	return m_cull_mode;
}

void d3d11_driver::front_face(front_face_t value)
{
	if (m_front_face != value)
	{
		core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
		m_front_face = value;
		D3D11_RASTERIZER_DESC rs;
		ZeroMemory(&rs, sizeof(rs));
		rs.FillMode = D3D11_FILL_SOLID;
		rs.CullMode = D3D11_CULL_MODE((int)m_cull_mode.get() + 1);
		rs.FrontCounterClockwise = m_front_face == front_face::counter_clockwise;
		ID3D11RasterizerState* rasterizerState;
		D3D11Device()->CreateRasterizerState(&rs, &rasterizerState);
		D3D11Context()->RSSetState(rasterizerState);
		rasterizerState->Release();
	}
}

front_face_t d3d11_driver::front_face()const
{
	return m_front_face;
}

void d3d11_driver::blend_mode(blend_mode_t value)
{
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	m_blend_mode = value;
	if (value == blend_mode::enable)	
		D3D11Context()->OMSetBlendState(m_d3d_blend_state, NULL, -1);
	else
		D3D11Context()->OMSetBlendState(NULL, NULL, -1);
}

blend_mode_t d3d11_driver::blend_mode()const
{
	return m_blend_mode;
}


void d3d11_driver::clear(color_t color)
{
	float _color[] = {
		(float)color.components.red / 255.0f,
		(float)color.components.green / 255.0f ,
		(float)color.components.blue / 255.0f ,
		(float)color.components.alpha / 255.0f
	};

	core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
		if (!m_current_frame_buffer.is_empty())
		{
			for (windex i = 0, c = m_current_frame_buffer->color_buffer_count(); i < c; ++i)
				D3D11Context()->ClearRenderTargetView(m_current_frame_buffer->D3DRenderTargetView(i), _color);
			if (m_current_frame_buffer->D3DDepthStencilView() != null)
				D3D11Context()->ClearDepthStencilView(m_current_frame_buffer->D3DDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
	});
}

void d3d11_driver::clear(maths::float4 const& color)
{
	core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
		if (!m_current_frame_buffer.is_empty())
		{
			for (windex i = 0, c = m_current_frame_buffer->color_buffer_count(); i < c; ++i)
				D3D11Context()->ClearRenderTargetView(m_current_frame_buffer->D3DRenderTargetView(i), (float*)&color);
			if (m_current_frame_buffer->D3DDepthStencilView() != null)
				D3D11Context()->ClearDepthStencilView(m_current_frame_buffer->D3DDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
	});
}

void d3d11_driver::draw(uint count, primitive_t prim)
{
	if (m_current_frame_buffer.is_empty())
		return;
	core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
		if (prim != m_primitive)
		{
			m_primitive = prim;
			D3D11Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		D3D11Context()->Draw(count, 0);
	});
}

void d3d11_driver::draw_indexed(uint count, primitive_t prim)
{
	if (m_current_frame_buffer.is_empty())
		return;
	core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
		if (prim != m_primitive)
		{
			m_primitive = prim;
			D3D11Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		if(vr_extension_support())
			D3D11Context()->DrawIndexedInstanced(count, 2, 0, 0, 0);
		else
			D3D11Context()->DrawIndexed(count, 0, 0);
	});
}

void d3d11_driver::bind_frame_buffer(iframe_buffer_t ifb)
{
	d3d11_frame_buffer_t fb = interface_cast<d3d11_frame_buffer>(ifb.get());

	//if (fb.get() == m_current_frame_buffer.get())
	//	return;
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	m_current_frame_buffer = fb;
	if (m_current_frame_buffer.get() == null)
	{
		D3D11Context()->OMSetRenderTargets(0, NULL, NULL);	
	}
	else
	{
		auto render_targets = m_current_frame_buffer->D3DRenderTargetView(0);
		D3D11Context()->OMSetDepthStencilState(m_current_frame_buffer->D3DDepthStencilState(), 1);
		D3D11Context()->OMSetRenderTargets(1, &render_targets, m_current_frame_buffer->D3DDepthStencilView());

		/*auto size = m_current_frame_buffer->dimentions();
		box<float> vp;
		vp.left = 0;
		vp.top = 0;
		vp.front = 0;
		vp.back = 1.0f;
		vp.width = (float)size.width;
		vp.height = (float)size.height;
		viewport(vp);*/
	}
}

void d3d11_driver::bind_shaders(effects::ishaders_t sh)
{
	d3d11_shaders_t shaders = interface_cast<d3d11_shaders>(sh.get());

	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	if (shaders.get() == m_current_technique.get())
		return;
	m_current_technique = shaders;
	if (m_current_technique.get() == null)
	{
			D3D11Context()->IASetInputLayout(NULL);
			D3D11Context()->VSSetShader(NULL, NULL, 0);
			D3D11Context()->PSSetShader(NULL, NULL, 0);
			D3D11Context()->VSSetConstantBuffers(0, 0, NULL);
			D3D11Context()->PSSetConstantBuffers(0, 0, NULL);
			D3D11Context()->PSSetSamplers(0, 0, NULL);
	}
	else
	{
		shaders->use_shaders(this);
	}
}

void d3d11_driver::bind_texture(textures::itexture_t _tex, windex idx)
{
	d3d11_texture_t tex = interface_cast<d3d11_texture>(_tex.get());
	if (tex.is_empty())
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			D3D11Context()->PSSetShaderResources(idx, 0, NULL);
		});		
	}
	else
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			tex->use_texture(this, idx);
		});	
	}
}

void d3d11_driver::bind_index_buffer(buffers::iindex_buffer_t buff)
{
	d3d11_index_buffer_t buffer = interface_cast<d3d11_index_buffer>(buff.get());
	if (buffer.is_empty())
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			D3D11Context()->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);
		});
	}
	else
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			buffer->use_buffer(this);
		});
	}
}

void d3d11_driver::bind_vertex_buffer(buffers::ivertex_buffer_t buff)
{
	d3d11_vertex_buffer_t buffer = interface_cast<d3d11_vertex_buffer>(buff.get());
	if (buffer.is_empty())
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			D3D11Context()->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
		});
	}
	else
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			buffer->use_buffer(this);
		});
	}
}

core::async::idispatcher_t d3d11_driver::dispatcher()const
{
	return m_async_worker.get();
}

bool d3d11_driver::lock()const
{
	return m_mutex.lock();
}

bool d3d11_driver::unlock()const 
{ 
	return m_mutex.unlock();
}


#endif
