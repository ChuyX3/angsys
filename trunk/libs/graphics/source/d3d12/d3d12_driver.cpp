#include "pch.h"
#include "d3d12/d3d12_driver.h"

#if DIRECTX12_SUPPORT

using namespace coffe;
using namespace coffe::graphics;
using namespace coffe::graphics::d3d12;


d3d12_driver::d3d12_driver()
{
	m_cull_mode = graphics::cull_mode::back;
	m_front_face = graphics::front_face::def;
	m_blend_mode = graphics::blend_mode::disable;
	m_async_worker = core::async::thread::create_thread();
}

//COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::d3d12::d3d12_driver);
//COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::d3d12::d3d12_driver);
//COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::d3d12::d3d12_driver, system_object, idriver, ifactory);

void d3d12_driver::dispose()
{
	close_driver();
	m_async_worker->exit();
	m_async_worker = null;
}

void d3d12_driver::set_file_system(core::files::ifile_system_t fs)
{
	m_mutex.lock();
	m_fs = fs;
	m_mutex.unlock();
}

bool d3d12_driver::init_driver(platform::icore_view_t, long64 adapter_id)
{
	com_wrapper<IDXGIFactory4> dxgiFactory;
	UINT createFactoryFlags = 0;
	HRESULT hr;
#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	hr = CreateDXGIFactory2(createFactoryFlags, __uuidof(IDXGIFactory4),  &dxgiFactory);
	if (FAILED(hr))
	{
		close_driver();
		return false;
	}

	m_dxgi_factory = move(dxgiFactory);

	com_wrapper<IDXGIAdapter1> dxgiAdapter1;
	com_wrapper<IDXGIAdapter4> dxgiAdapter4;

	wsize maxDedicatedVideoMemory = 0;
	for (uint i = 0; m_dxgi_factory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
		dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

		if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
			SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.get(),
				D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
			dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
		{
			dxgiAdapter4 = null;
			maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
			hr = dxgiAdapter1.as(&dxgiAdapter4);
			if (SUCCEEDED(hr))
				m_dxgi_adapter = move(dxgiAdapter4);
		}
	}

	if (m_dxgi_adapter.is_empty())
	{
		close_driver();
		return false;
	}

	//com_wrapper<ID3D12Device2> d3d12Device2;
	hr = D3D12CreateDevice(m_dxgi_adapter.get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device2), &m_d3d_device);
	if (FAILED(hr))
	{
		close_driver();
		return false;
	}
	
#if defined(_DEBUG)
	com_wrapper<ID3D12InfoQueue> pInfoQueue;
	if (SUCCEEDED(m_d3d_device.as(&pInfoQueue)))
	{
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
		// Suppress whole categories of messages
		   //D3D12_MESSAGE_CATEGORY Categories[] = {};

		   // Suppress messages based on their severity level
		D3D12_MESSAGE_SEVERITY Severities[] =
		{
			D3D12_MESSAGE_SEVERITY_INFO
		};

		// Suppress individual messages by their ID
		D3D12_MESSAGE_ID DenyIds[] = {
			D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
			D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
			D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
		};

		D3D12_INFO_QUEUE_FILTER NewFilter = {};
		//NewFilter.DenyList.NumCategories = _countof(Categories);
		//NewFilter.DenyList.pCategoryList = Categories;
		NewFilter.DenyList.NumSeverities = _countof(Severities);
		NewFilter.DenyList.pSeverityList = Severities;
		NewFilter.DenyList.NumIDs = _countof(DenyIds);
		NewFilter.DenyList.pIDList = DenyIds;

		hr = pInfoQueue->PushStorageFilter(&NewFilter);
	}
#endif

	D3D12_FEATURE_DATA_FEATURE_LEVELS fl;
	D3D12Device()->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &fl, size_of(fl));
	m_feature_level = fl.MaxSupportedFeatureLevel;

	switch (m_feature_level)
	{
	case D3D_FEATURE_LEVEL_12_1:
		m_max_version = 12;
		m_min_version = 1;
		break;
	case D3D_FEATURE_LEVEL_12_0:
		m_max_version = 12;
		m_min_version = 0;
		break;
	case D3D_FEATURE_LEVEL_11_1:
		m_max_version = 11;
		m_min_version = 1;
		break;
	case D3D_FEATURE_LEVEL_11_0:
		m_max_version = 11;
		m_min_version = 0;
		break;
	case D3D_FEATURE_LEVEL_10_1:
		m_max_version = 10;
		m_min_version = 1;
		break;
	case D3D_FEATURE_LEVEL_10_0:
		m_max_version = 10;
		m_min_version = 0;
		break;
	case D3D_FEATURE_LEVEL_9_3:
		m_max_version = 9;
		m_min_version = 3;
		break;
	case D3D_FEATURE_LEVEL_9_2:
		m_max_version = 9;
		m_min_version = 2;
		break;
	case D3D_FEATURE_LEVEL_9_1:
		m_max_version = 9;
		m_min_version = 1;
		break;
	}

	
	D3D12_COMMAND_QUEUE_DESC cqDesc = {};
	cqDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	cqDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cqDesc.NodeMask = 0;

	hr = D3D12Device()->CreateCommandQueue(&cqDesc, __uuidof(ID3D12CommandQueue), &m_d3d_queue);

	if (FAILED(hr))
	{
		close_driver();
		return false;
	}

	m_d3d_desc_heaps[0];

	D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};
	dhDesc.NumDescriptors = 10;
	dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	hr = D3D12Device()->CreateDescriptorHeap(&dhDesc, __uuidof(ID3D12DescriptorHeap), &m_d3d_desc_heaps[0]);
	if (FAILED(hr))
	{
		close_driver();
		return false;
	}

	return true;
}




void d3d12_driver::close_driver()
{
	//m_d3d_blend_state = null;
	//m_current_frame_buffer = null;
	//m_current_technique = null;

	//m_d2d_context = null;
	//m_d2d_device = null;
	//m_d2d_factory = null;

	//m_current_frame_buffer = null;
	//m_d3d_context = null;
	for (auto& heap : m_d3d_desc_heaps)
		heap = null;
	m_d3d_device = null;
	m_dxgi_adapter = null;
	m_dxgi_factory = null;
}

graph_driver_type_t d3d12_driver::graph_driver_type()const { return graphics::graph_driver_type::DirectX11; }

bool d3d12_driver::matrix_transpose_needed()const { return true; }

bool d3d12_driver::vr_extension_support()const { return false; }

ulong d3d12_driver::min_version()const
{
	return m_min_version;
}

ulong d3d12_driver::max_version()const
{
	return m_max_version;
}

isurface_t d3d12_driver::create_surface(platform::icore_view_t view)const
{
	/*d3d12_surface_t surface = new d3d12_surface(const_cast<d3d12_driver*>(this));
	if (!surface->create(view))
		return null;
	return surface.get();*/
	return null;
}

ifactory_t d3d12_driver::get_factory()const
{ 
	return const_cast<d3d12_driver*>(this);
}

optional<buffers::ivertex_buffer> d3d12_driver::create_vertex_buffer(buffers::buffer_usage_t usage, vector<reflect::attribute_desc> vertex_desc, wsize vertex_count, ibuffer_t buff, string sid)const
{
	/*array_view<byte> init_data = buff.is_empty() ? to_array((byte*)null, 0) : to_array((byte*)buff->buffer_ptr(), buff->buffer_size());

	d3d12_vertex_buffer_t buffer = new d3d12_vertex_buffer();
	if (!buffer->create(
		const_cast<d3d12_driver*>(this),
		usage,
		vertex_desc,
		vertex_count,
		init_data,
		sid
		))
		return new error(error_code::unknown);
	return buffer.get();*/
	return new error(error_code::unsupported);
}

optional<buffers::iindex_buffer> d3d12_driver::create_index_buffer(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t buff, string sid)const
{
	/*array_view<byte> init_data = buff.is_empty() ? to_array((byte*)null, 0) : to_array((byte*)buff->buffer_ptr(), buff->buffer_size());
	d3d12_index_buffer_t buffer = new d3d12_index_buffer();
	if (!buffer->create(
		const_cast<d3d12_driver*>(this),
		usage,
		index_type,
		index_count,
		init_data,
		sid
	))
		return new error(error_code::unknown);
	return buffer.get();*/
	return new error(error_code::unsupported);
}

optional<textures::itexture> d3d12_driver::create_texture(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, graphics::size3d<uint> dimentions, string sid)const
{
	/*d3d12_texture_t text = new d3d12_texture();
	if (!text->create(
		const_cast<d3d12_driver*>(this),
		type,
		color_format,
		usage,
		flags,
		dimentions,
		sid
	))
		return new error(error_code::unknown);
	return text.get();*/
	return new error(error_code::unsupported);
}

optional<textures::itexture> d3d12_driver::create_texture(unknown_t tex_handle, string sid)const
{
	/*if (tex_handle == null)
		return new error(error_code::invalid_param);

	com_wrapper<ID3D11Resource> resource;
	IUnknown* unk = reinterpret_cast<IUnknown*>(tex_handle);

	unk->QueryInterface(resource.addres_for_init());
	d3d12_texture_t text = new d3d12_texture();
	if (!text->attach(resource, const_cast<d3d12_driver*>(this),sid))
		return new error(error_code::unknown);
	return text.get();*/
	return new error(error_code::unsupported);
}

optional<textures::itexture> d3d12_driver::load_texture(text::string file, textures::tex_type_t type, string sid)const
{
	/*d3d12_texture_t tex = new d3d12_texture();
	if (!tex->load(const_cast<d3d12_driver*>(this), file, type, sid))
		return new error(error_code::unknown);
	return tex.get();*/
	return new error(error_code::unsupported);
}

optional<textures::itexture> d3d12_driver::load_texture(array_view<text::string> files, textures::tex_type_t type, string sid)const
{
	return new error(error_code::unsupported);
}

optional<iframe_buffer> d3d12_driver::create_frame_buffer(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<uint> dimentions, bool is_stereo, string sid)const
{ 
	/*d3d12_frame_buffer_t buffer = new d3d12_frame_buffer(const_cast<d3d12_driver*>(this));
	if (!buffer->create(
		color_format, 
		depth_stencil_format,
		dimentions,
		is_stereo,
		sid
	))
		return new error(error_code::unknown);
	return buffer.get();*/
	return new error(error_code::unsupported);
}

optional<iframe_buffer> d3d12_driver::create_frame_buffer(array_view<textures::itexture_t> color_tex, textures::itexture_t depth_stencil, string sid)const
{
	/*d3d12_frame_buffer_t buffer = new d3d12_frame_buffer(const_cast<d3d12_driver*>(this));
	vector<d3d12_texture_t> textures;
	for (textures::itexture_t tex : color_tex)
		textures += interface_cast<d3d12_texture>(color_tex.get());
	if (!buffer->create(
		(array_view<d3d12_texture_t>)textures,
		interface_cast<d3d12_texture>(depth_stencil.get()),
		sid
	))
		return new error(error_code::unknown);
	return buffer.get();*/
	return new error(error_code::unsupported);
}

optional<effects::ishaders> d3d12_driver::compile_shaders(wstring vertex_shader, wstring pixel_shader, string sid)const
{
	/*string log;
	d3d12_shaders_t shaders = new d3d12_shaders();
	if(!shaders->load(
		const_cast<d3d12_driver*>(this),
		vertex_shader.get(),
		pixel_shader.get(),
		sid,
		&log))
		return new error(log);
	
	return shaders.get();*/
	return new error(error_code::unsupported);
}

optional<effects::ishaders> d3d12_driver::compile_shaders(effects::shader_info_t const& vertex_shader, effects::shader_info_t const& pixel_shader, string sid)const
{
	/*string log;
	d3d12_shaders_t shaders = new d3d12_shaders();
	if (!shaders->load(
		const_cast<d3d12_driver*>(this),
		vertex_shader,
		pixel_shader,
		sid,
		&log))
		return new error(log);
	return shaders.get();*/
	return new error(error_code::unsupported);
}

core::async::iasync_optional<buffers::ivertex_buffer> d3d12_driver::create_vertex_buffer_async(
	buffers::buffer_usage_t usage,
	vector<reflect::attribute_desc> vertex_desc,
	wsize vertex_count,
	ibuffer_t init_data,
	string sid)const
{
	return create_task<optional<buffers::ivertex_buffer>>([=](core::async::iasync_optional<buffers::ivertex_buffer>, d3d12_driver_t driver)
	{
		return driver->create_vertex_buffer(usage, vertex_desc, vertex_count, init_data, sid);
	});
}

core::async::iasync_optional<buffers::iindex_buffer> d3d12_driver::create_index_buffer_async(
	buffers::buffer_usage_t usage,
	reflect::var_type_t index_type, 
	wsize index_count,
	ibuffer_t init_data,
	string sid)const
{
	return create_task<optional<buffers::iindex_buffer>>([=](core::async::iasync_optional<buffers::iindex_buffer>, d3d12_driver_t driver)
	{
		return driver->create_index_buffer(usage, index_type, index_count, init_data, sid);
	});
}

core::async::iasync_optional<textures::itexture> d3d12_driver::create_texture_async(
	textures::tex_type_t type, 
	textures::tex_format_t color_format,
	buffers::buffer_usage_t usage, 
	buffers::buffer_bind_flag_t flags, 
	size3d<uint> dimentions, 
	string sid)const
{
	return create_task<optional<textures::itexture>>([=](core::async::iasync_optional<textures::itexture>, d3d12_driver_t driver)
	{
		return driver->create_texture(type, color_format, usage, flags, dimentions, sid);
	});
}

core::async::iasync_optional<textures::itexture> d3d12_driver::create_texture_async(
	unknown_t tex_handle, 
	string sid)const
{
	return create_task<optional<textures::itexture>>([=](core::async::iasync_optional<textures::itexture>, d3d12_driver_t driver)
	{
		return driver->create_texture(tex_handle, sid);
	});
}

core::async::iasync_optional<textures::itexture> d3d12_driver::load_texture_async(
	text::string file, 
	textures::tex_type_t type,
	string sid)const
{
	return create_task<optional<textures::itexture>>([=](core::async::iasync_optional<textures::itexture>, d3d12_driver_t driver)
	{
		return driver->load_texture(file, type, sid);
	});
}

core::async::iasync_optional<textures::itexture> d3d12_driver::load_texture_async(
	array_view<text::string> files_,
	textures::tex_type_t type,
	string sid)const
{
	array<string> files = files_;
	return create_task<optional<textures::itexture>>([=](core::async::iasync_optional<textures::itexture>, d3d12_driver_t driver)
	{
		return driver->load_texture(files, type, sid);
	});
}

core::async::iasync_optional<iframe_buffer> d3d12_driver::create_frame_buffer_async(
	array_view<textures::tex_format_t> color_format_,
	textures::tex_format_t depth_stencil_format,
	size<uint> dimentions,
	bool is_stereo,
	string sid)const
{
	array<textures::tex_format_t> color_format = color_format_;
	return create_task<optional<iframe_buffer>>([=](core::async::iasync_optional<iframe_buffer>, d3d12_driver_t driver)
	{
		return driver->create_frame_buffer(color_format, depth_stencil_format, dimentions, is_stereo, sid);
	});
}

core::async::iasync_optional<iframe_buffer> d3d12_driver::create_frame_buffer_async(
	array_view<textures::itexture_t> color_tex_,
	textures::itexture_t depth_stencil_format,
	string sid)const
{
	array<textures::itexture_t> color_tex = color_tex_;
	return create_task<optional<iframe_buffer>>([=](core::async::iasync_optional<iframe_buffer>, d3d12_driver_t driver)
	{
		return driver->create_frame_buffer(color_tex, depth_stencil_format, sid);
	});
}

core::async::iasync_optional<effects::ishaders> d3d12_driver::compile_shaders_async(
	wstring vertex_shader,
	wstring pixel_shader,
	string sid)const
{
	return create_task<optional<effects::ishaders>>([=](core::async::iasync_optional<effects::ishaders>, d3d12_driver_t driver)
	{
		return driver->compile_shaders(vertex_shader, pixel_shader, sid);
	});
}

core::async::iasync_optional<effects::ishaders> d3d12_driver::compile_shaders_async(
	effects::shader_info_t const& vertex_shader,
	effects::shader_info_t const& pixel_shader,
	string sid)const
{
	return create_task<optional<effects::ishaders>>([=](core::async::iasync_optional<effects::ishaders>, d3d12_driver_t driver)
	{
		return driver->compile_shaders(vertex_shader, pixel_shader, sid);
	});
}


void d3d12_driver::viewport(box<float> _vp)
{
	/*core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	D3D11_VIEWPORT vp;
	vp.Width = _vp.width;
	vp.Height = _vp.height;
	vp.MinDepth = _vp.front;
	vp.MaxDepth = _vp.back;
	vp.TopLeftX = _vp.left;
	vp.TopLeftY = _vp.top;
	D3D11Context()->RSSetViewports(1, &vp);*/
}

box<float> d3d12_driver::viewport()const
{
	return m_viewport;
}

void d3d12_driver::cull_mode(cull_mode_t value)
{
	if (m_cull_mode != value)
	{
		/*core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
		m_cull_mode = value;
		D3D11_RASTERIZER_DESC rs;
		ZeroMemory(&rs, sizeof(rs));
		rs.FillMode = D3D11_FILL_SOLID;
		rs.CullMode = D3D11_CULL_MODE((int)m_cull_mode.get() + 1);
		rs.FrontCounterClockwise = m_front_face == front_face::counter_clockwise;
		ID3D11RasterizerState* rasterizerState;
		D3D11Device()->CreateRasterizerState(&rs, &rasterizerState);
		D3D11Context()->RSSetState(rasterizerState);		
		rasterizerState->Release();*/
	}
}

cull_mode_t d3d12_driver::cull_mode()const
{
	return m_cull_mode;
}

void d3d12_driver::front_face(front_face_t value)
{
	if (m_front_face != value)
	{
		/*core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
		m_front_face = value;
		D3D11_RASTERIZER_DESC rs;
		ZeroMemory(&rs, sizeof(rs));
		rs.FillMode = D3D11_FILL_SOLID;
		rs.CullMode = D3D11_CULL_MODE((int)m_cull_mode.get() + 1);
		rs.FrontCounterClockwise = m_front_face == front_face::counter_clockwise;
		ID3D11RasterizerState* rasterizerState;
		D3D11Device()->CreateRasterizerState(&rs, &rasterizerState);
		D3D11Context()->RSSetState(rasterizerState);
		rasterizerState->Release();*/
	}
}

front_face_t d3d12_driver::front_face()const
{
	return m_front_face;
}

void d3d12_driver::blend_mode(blend_mode_t value)
{
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	m_blend_mode = value;
	/*if (value == blend_mode::enable)	
		D3D11Context()->OMSetBlendState(m_d3d_blend_state, NULL, -1);
	else
		D3D11Context()->OMSetBlendState(NULL, NULL, -1);*/
}

blend_mode_t d3d12_driver::blend_mode()const
{
	return m_blend_mode;
}


void d3d12_driver::clear(color_t color)
{
	float _color[] = {
		(float)color.components.red / 255.0f,
		(float)color.components.green / 255.0f ,
		(float)color.components.blue / 255.0f ,
		(float)color.components.alpha / 255.0f
	};

	core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
		/*if (!m_current_frame_buffer.is_empty())
		{
			for (windex i = 0, c = m_current_frame_buffer->color_buffer_count(); i < c; ++i)
				D3D11Context()->ClearRenderTargetView(m_current_frame_buffer->D3DRenderTargetView(i), _color);
			if (m_current_frame_buffer->D3DDepthStencilView() != null)
				D3D11Context()->ClearDepthStencilView(m_current_frame_buffer->D3DDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}*/
	});
}

void d3d12_driver::clear(maths::float4 const& color)
{
	core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
		/*if (!m_current_frame_buffer.is_empty())
		{
			for (windex i = 0, c = m_current_frame_buffer->color_buffer_count(); i < c; ++i)
				D3D11Context()->ClearRenderTargetView(m_current_frame_buffer->D3DRenderTargetView(i), (float*)&color);
			if (m_current_frame_buffer->D3DDepthStencilView() != null)
				D3D11Context()->ClearDepthStencilView(m_current_frame_buffer->D3DDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		}*/
	});
}

void d3d12_driver::draw(uint count, primitive_t prim)
{
	/*if (m_current_frame_buffer.is_empty())
		return;
	core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
		if (prim != m_primitive)
		{
			m_primitive = prim;
			D3D11Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		D3D11Context()->Draw(count, 0);
	});*/
}

void d3d12_driver::draw_indexed(uint count, primitive_t prim)
{
	/*if (m_current_frame_buffer.is_empty())
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
	});*/
}

void d3d12_driver::bind_frame_buffer(iframe_buffer_t ifb)
{
	//d3d12_frame_buffer_t fb = interface_cast<d3d12_frame_buffer>(ifb.get());
	//
	//if (fb.get() == m_current_frame_buffer.get())
	//	return;
	//core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	//m_current_frame_buffer = fb;
	//if (m_current_frame_buffer.get() == null)
	//{
	//	D3D11Context()->OMSetRenderTargets(0, NULL, NULL);	
	//}
	//else
	//{
	//	auto render_targets = m_current_frame_buffer->D3DRenderTargetView(0);
	//	D3D11Context()->OMSetDepthStencilState(m_current_frame_buffer->D3DDepthStencilState(), 1);
	//	D3D11Context()->OMSetRenderTargets(1, &render_targets, m_current_frame_buffer->D3DDepthStencilView());

	//	/*auto size = m_current_frame_buffer->dimentions();
	//	box<float> vp;
	//	vp.left = 0;
	//	vp.top = 0;
	//	vp.front = 0;
	//	vp.back = 1.0f;
	//	vp.width = (float)size.width;
	//	vp.height = (float)size.height;
	//	viewport(vp);*/
	//}
}

void d3d12_driver::bind_shaders(effects::ishaders_t sh)
{
	/*d3d12_shaders_t shaders = interface_cast<d3d12_shaders>(sh.get());

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
	}*/
}

void d3d12_driver::bind_texture(textures::itexture_t _tex, windex idx)
{
	/*d3d12_texture_t tex = interface_cast<d3d12_texture>(_tex.get());
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
	}*/
}

void d3d12_driver::bind_index_buffer(buffers::iindex_buffer_t buff)
{
	/*d3d12_index_buffer_t buffer = interface_cast<d3d12_index_buffer>(buff.get());
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
	}*/
}

void d3d12_driver::bind_vertex_buffer(buffers::ivertex_buffer_t buff)
{
	/*d3d12_vertex_buffer_t buffer = interface_cast<d3d12_vertex_buffer>(buff.get());
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
	}*/
}

core::async::idispatcher_t d3d12_driver::dispatcher()const
{
	return m_async_worker.get();
}

bool d3d12_driver::lock()const
{
	return m_mutex.lock();
}

bool d3d12_driver::unlock()const 
{ 
	return m_mutex.unlock();
}


#endif
