#include "pch.h"
#include "d3d11/driver.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

//#define MY_TYPE ang::graphics::d3d11::d3d11_surface
//#include <ang/inline/object_wrapper_specialization.inl>
//#undef MY_TYPE

d3d11_surface::d3d11_surface(d3d11_driver* driver)
	: m_parent_driver(driver)
{
	
}

d3d11_surface::~d3d11_surface()
{
	close();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::d3d11_surface);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::d3d11_surface, object, isurface);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::d3d11_surface, object, isurface);

bool d3d11_surface::create(platform::icore_view_t view)
{
	HRESULT hr = S_OK;
	d3d11_driver_t driver = m_parent_driver.lock();
	if (driver.is_empty())
		return false;

	graphics::size<float> size = view->core_view_size();
	m_current_size = size;

	DXGI_SWAP_CHAIN_DESC1 sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Width = (uint)size.width;
	sd.Height = (uint)size.height;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsd;
	ZeroMemory(&fsd, sizeof(fsd));
	fsd.RefreshRate = { 60,1 };
	fsd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	fsd.Scaling = DXGI_MODE_SCALING_CENTERED;
	fsd.Windowed = TRUE;
	hr = driver->DXGIFactory()->CreateSwapChainForHwnd
	(
		driver->D3D11Device(),
		(HWND)view->core_view_handle(),
		&sd,
		&fsd,
		nullptr,
		&m_dxgi_swap_chain
	);

	platform::icore_context_t context = view->core_context();
	context->bind_graphic_native_surface(m_dxgi_swap_chain.get());

//	driver->DXGIFactory()->MakeWindowAssociation((HWND)view->get_core_view_handle(), DXGI_MWA_NO_ALT_ENTER);

	m_d3d_frame_buffer = new d3d11_frame_buffer(driver.get());
	m_d3d_frame_buffer->create(this);

	view->dispatcher()->listen_to(new ang::platform::events::display_size_change_event(this, &d3d11_surface::on_display_size_changed_event));

	return true;
}

bool d3d11_surface::update(platform::icore_view_t view, graphics::size<float> size, graphics::size<float> scale)
{
	HRESULT hr = S_OK;
	bool bind = false;

	d3d11_driver_t driver = m_parent_driver.lock();
	if (driver.is_empty())
		return false;

	return driver->execute_on_thread_safe([&]() 
	{
		size.height = max(size.height, 10.0f);
		size.width = max(size.width, 10.0f);

		if (m_dxgi_swap_chain.get())
		{
			if (driver->current_frame_buffer().get() == m_d3d_frame_buffer.get())
			{
				driver->bind_frame_buffer(nullptr);
				bind = true;
			}
		}

		m_d3d_frame_buffer->close();

		if (FAILED(m_dxgi_swap_chain->ResizeBuffers(
			2, // Double-buffered swap chain.
			(uint)maths::round(size.width),
			(uint)maths::round(size.height),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0
		)))
			return false;

		m_current_size = size;
		m_d3d_frame_buffer->create(this);
		if (bind) driver->bind_frame_buffer(m_d3d_frame_buffer.get());

		return true;
	});
}

bool d3d11_surface::close()
{
	m_dxgi_swap_chain = null;
	m_d3d_frame_buffer = null;
	return true;
}

void d3d11_surface::on_display_size_changed_event(objptr sender, ang::platform::events::idisplay_info_event_args_t args)
{
	auto info = args->display_info();
	update(args->core_view(), info.display_resolution, info.display_scale_factor);
}



void d3d11_surface::swap_buffers(bool syncronize)
{
	m_dxgi_swap_chain->Present(syncronize ? 1 : 0, 0);
}

iframe_buffer_t d3d11_surface::frame_buffer()const
{
	return m_d3d_frame_buffer.get();
}

#endif
