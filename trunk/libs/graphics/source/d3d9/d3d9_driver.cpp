#include "pch.h"
#include "d3d9/d3d9_driver.h"

#if DIRECTX9_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d9;

d3d9_driver::d3d9_driver()
{

}

d3d9_driver::~d3d9_driver()
{

}

void d3d9_driver::dispose()
{
	close_driver();
}

bool d3d9_driver::init_driver(platform::icore_view_t wnd, long64 adapter_id)
{
	HWND hwnd = NULL;
	if (wnd.is_empty()) {
		//crerate dummy window
		hwnd = CreateWindowA("static", "d3d9_driver", WS_OVERLAPPED, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
		if (hwnd == NULL)
			return false;
	}
	else {
		hwnd = (HWND)wnd->core_view_handle();
	}

	Direct3DCreate9Ex(D3D_SDK_VERSION, &m_d3d9);


	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
	d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.hDeviceWindow = hwnd;    // set the window to be used by Direct3D

	//D3DDISPLAYMODEEX d3ddm;
	//ZeroMemory(&d3ddm, sizeof(d3ddm));
	//d3ddm.Size = sizeof(D3DDISPLAYMODEEX);

	// create a device class using this information and information from the d3dpp stuct
	m_d3d9->CreateDeviceEx(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		NULL,
		&m_d3d9_device);


	if (wnd.is_empty()) {
		CloseWindow(hwnd);
	}

	return true;
}

void d3d9_driver::close_driver()
{
	m_d3d9_device = null;
	m_d3d9 = null;
}

graph_driver_type_t d3d9_driver::graph_driver_type()const
{
	return graph_driver_type::DirectX9;
}

bool d3d9_driver::matrix_transpose_needed()const
{
	return true;
}

bool d3d9_driver::vr_extension_support()const
{
	return false;
}

ulong d3d9_driver::min_version()const
{
	return m_min_version;
}

ulong d3d9_driver::max_version()const
{
	return m_max_version;
}

isurface_t d3d9_driver::create_surface(platform::icore_view_t)const
{
	return null;
}

ifactory_t d3d9_driver::get_factory()const
{
	return const_cast<d3d9_driver*>(this);
}

void d3d9_driver::set_file_system(core::files::ifile_system_t fs)
{
	m_fs = fs;
}

void d3d9_driver::viewport(box<float> vp)
{
	//TODO:
}

box<float> d3d9_driver::viewport()const
{
	return m_viewport;
}

void d3d9_driver::cull_mode(cull_mode_t)
{
	//TODO:
}

cull_mode_t d3d9_driver::cull_mode()const
{
	return m_cull_mode;
}

void d3d9_driver::front_face(front_face_t)
{
	//TODO:
}

front_face_t d3d9_driver::front_face()const
{
	return m_front_face;
}

void d3d9_driver::blend_mode(blend_mode_t)
{
	//TODO:
}

blend_mode_t d3d9_driver::blend_mode()const
{
	return m_blend_mode;
}








void d3d9_driver::bind_frame_buffer(iframe_buffer_t)
{

}

void d3d9_driver::bind_shaders(effects::ishaders_t)
{

}

void d3d9_driver::bind_texture(textures::itexture_t, windex idx)
{

}

void d3d9_driver::bind_index_buffer(buffers::iindex_buffer_t)
{

}

void d3d9_driver::bind_vertex_buffer(buffers::ivertex_buffer_t)
{

}

void d3d9_driver::clear(maths::float4 const&)
{

}

void d3d9_driver::clear(color_t)
{

}

void d3d9_driver::draw(uint count, primitive_t)
{

}

void d3d9_driver::draw_indexed(uint count, primitive_t)
{

}

core::async::idispatcher_t d3d9_driver::dispatcher()const
{
	return m_async_worker.get();
}

bool d3d9_driver::lock()const
{
	if (!m_mutex.lock())
		return false;
	return SUCCEEDED(m_d3d9_device->BeginScene()) ? true : false;;
}

bool d3d9_driver::unlock()const
{
	m_d3d9_device->EndScene();
	return m_mutex.unlock();
}

#endif/*DIRECTX_SUPPORT*/
