#include "pch.h"
#include "d3d12/d3d12_driver.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX12_SUPPORT

using namespace coffe;
using namespace coffe::graphics;
using namespace coffe::graphics::d3d12;

//#define MY_TYPE coffe::graphics::d3d12::d3d12_surface
//#include <coffe/inline/object_wrapper_specialization.inl>
//#undef MY_TYPE

d3d12_surface::d3d12_surface(d3d12_driver* driver)
	: m_parent_driver(driver)
	, m_need_update(false)
{
	
}

//COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::d3d12::d3d12_surface);
//COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::d3d12::d3d12_surface);
//COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::d3d12::d3d12_surface, system_object, isurface);

void d3d12_surface::dispose()
{
	close();
}

bool d3d12_surface::create(platform::icore_view_t view)
{
	HRESULT hr = S_OK;
	d3d12_driver_t driver = m_parent_driver.lock();
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
	sd.Flags = driver->AllowTearing() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

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
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		hr = driver->DXGIFactory()->CreateSwapChainForHwnd
		(
			driver->D3D12Device(),
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
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		hr = driver->DXGIFactory()->CreateSwapChainForCoreWindow
		(
			driver->D3D12Device(),
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
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		hr = driver->DXGIFactory()->CreateSwapChainForComposition
		(
			driver->D3D12Device(),
			&sd,
			nullptr,
			&m_dxgi_swap_chain
		);
	}

	graphics::icore_context_t context = view->core_context();
	if(!context.is_empty())
		context->bind_graphic_native_surface(m_dxgi_swap_chain.get());

	auto rtvDescriptorSize = driver->D3D12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = driver->D3D12DescriptorHeap(0)->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < 3; ++i)
	{
		com_wrapper<ID3D12Resource> backBuffer;
		DXGISwapChain()->GetBuffer(i, __uuidof(ID3D12Resource), &backBuffer);
		driver->D3D12Device()->CreateRenderTargetView(backBuffer.get(), nullptr, rtvHandle);

		m_d3d_buffers[i] = backBuffer;

		rtvHandle.ptr += rtvDescriptorSize;
		//rtvHandle.Offset(rtvDescriptorSize);
	}


	//m_d3d_frame_buffer = new d3d12_frame_buffer(driver.get());
	//m_d3d_frame_buffer->create(this);

	m_view = view;
	view->dispatcher()->listen_to(new coffe::platform::events::display_size_change_event(this, &d3d12_surface::on_display_size_changed_event));

	return true;
}

bool d3d12_surface::update(platform::icore_view_t view, graphics::size<uint> size)
{
	HRESULT hr = S_OK;
	bool bind = false;

	d3d12_driver_t driver = m_parent_driver.lock();
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

bool d3d12_surface::close()
{
	m_dxgi_swap_chain = null;
	m_d3d_frame_buffer = null;
	m_view = null;
	m_parent_driver = null;
	return true;
}

void d3d12_surface::on_display_size_changed_event(objptr, platform::events::idisplay_info_event_args_t args)
{
	m_need_update = true;
	auto const& display = args->display_info();
	m_pending_size.width = (uint)max(display.display_resolution.width * display.display_scale_factor.width, 10.0f);
	m_pending_size.height = (uint)max(display.display_resolution.height * display.display_scale_factor.height, 10.0f);
}

collections::ienum_ptr<collections::tuple<scenes::itransform_t, iframe_buffer_t>> d3d12_surface::update()
{
	if (m_need_update)
	{
		m_need_update = !update(m_view, m_pending_size);
	}
	return null;
}

bool d3d12_surface::swap_buffers(bool syncronize)
{
	HRESULT hr = m_dxgi_swap_chain->Present(syncronize ? 1 : 0, 0);
	return SUCCEEDED(hr) ? true : false;
}

iframe_buffer_t d3d12_surface::frame_buffer()const
{
	return m_d3d_frame_buffer.get();
}


#endif
