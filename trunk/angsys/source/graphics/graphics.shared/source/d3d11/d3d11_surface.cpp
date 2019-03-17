#include "pch.h"
#include "d3d11/d3d11_driver.h"

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
	, m_need_update(false)
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
	graphics::size<float> scale = view->core_view_scale_factor();
	m_current_size = { uint(size.width * scale.width), uint(size.height * scale.height) };

	DXGI_SWAP_CHAIN_DESC1 sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Width = (uint)m_current_size.width;
	sd.Height = (uint)m_current_size.height;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 3;

#if WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsd;
	ZeroMemory(&fsd, sizeof(fsd));
	fsd.RefreshRate = { 60,1 };
	fsd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	fsd.Scaling = DXGI_MODE_SCALING_CENTERED;
	fsd.Windowed = TRUE;

	HWND wnd = (HWND)view->core_view_handle();
	if (IsWindow(wnd))
	{
		sd.Scaling = DXGI_SCALING_NONE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		hr = driver->DXGIFactory()->CreateSwapChainForHwnd
		(
			driver->D3D11Device(),
			wnd,
			&sd,
			&fsd,
			nullptr,
			&m_dxgi_swap_chain
		);
	}	
#elif WINDOWS_PLATFORM == WINDOWS_STORE_PLATFORM
	IUnknown* cwnd = reinterpret_cast<IUnknown*>(view->core_view_handle());

	if (cwnd != null)
	{
		sd.Scaling = DXGI_SCALING_STRETCH;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		hr = driver->DXGIFactory()->CreateSwapChainForCoreWindow
		(
			driver->D3D11Device(),
			cwnd,
			&sd,
			nullptr,
			&m_dxgi_swap_chain
		);
	}
#endif
	else
	{
		sd.Scaling = DXGI_SCALING_STRETCH;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		hr = driver->DXGIFactory()->CreateSwapChainForComposition
		(
			driver->D3D11Device(),
			&sd,
			nullptr,
			&m_dxgi_swap_chain
		);
	}

	graphics::icore_context_t context = view->core_context();
	if(!context.is_empty())
		context->bind_graphic_native_surface(m_dxgi_swap_chain.get());

	m_d3d_frame_buffer = new d3d11_frame_buffer(driver.get());
	m_d3d_frame_buffer->create(this);

	m_view = view;
	view->dispatcher()->listen_to(new ang::platform::events::display_size_change_event(this, &d3d11_surface::on_display_size_changed_event));

	return true;
}

bool d3d11_surface::update(platform::icore_view_t view, graphics::size<uint> size)
{
	HRESULT hr = S_OK;
	bool bind = false;

	d3d11_driver_t driver = m_parent_driver.lock();
	if (driver.is_empty())
		return false;

	return driver->execute_on_thread_safe([&]() 
	{
		m_current_size = size;

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

void d3d11_surface::on_display_size_changed_event(objptr, platform::events::idisplay_info_event_args_t args)
{
	m_need_update = true;
	auto const& display = args->display_info();
	m_pending_size.width = max(display.display_resolution.width * display.display_scale_factor.width, 10.0f);
	m_pending_size.height = max(display.display_resolution.height * display.display_scale_factor.height, 10.0f);
}

void d3d11_surface::update()
{
	if (m_need_update)
	{
		m_need_update = !update(m_view, m_pending_size);
	}	
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
