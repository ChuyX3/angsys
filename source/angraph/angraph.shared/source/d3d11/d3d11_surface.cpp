#include "pch.h"
#include "d3d11/driver.h"

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

d3d11_surface::d3d11_surface(d3d11_driver* driver)
	: _parent_driver(driver)
{
	
}

d3d11_surface::~d3d11_surface()
{
	close();
}

ANG_IMPLEMENT_CLASSNAME(ang::graphics::d3d11::d3d11_surface);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::d3d11::d3d11_surface);

bool d3d11_surface::is_child_of(type_name_t name)
{
	return name == type_name<d3d11_surface>()
		|| object::is_child_of(name)
		|| isurface::is_child_of(name);
}

bool d3d11_surface::is_kind_of(type_name_t name)const
{
	return name == type_name<d3d11_surface>()
		|| object::is_kind_of(name)
		|| isurface::is_kind_of(name);
}

bool d3d11_surface::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<d3d11_surface>())
	{
		*out = static_cast<d3d11_surface*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (isurface::query_object(name, out))
	{
		return true;
	}
	return false;
}

bool d3d11_surface::create(platform::icore_view_t view)
{
	HRESULT hr = S_OK;
	d3d11_driver_t driver = _parent_driver.lock<d3d11_driver>();
	if (driver.is_empty())
		return false;

	foundation::size<float> size = view->get_core_view_size();
	_current_size = size;

	view->get_listener()->listen_to(new ang::platform::events::display_size_change_event(this, &d3d11_surface::on_display_size_changed_event));

	DXGI_SWAP_CHAIN_DESC1 sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Width = (uint)size.width;
	sd.Height = (uint)size.height;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;

	hr = driver->DXGIFactory()->CreateSwapChainForHwnd
	(
		driver->D3D11Device(),
		(HWND)view->get_core_view_handle(),
		&sd,
		nullptr,
		nullptr,
		&dxgi_swap_chain
	);

	platform::icore_context_t context = view->get_core_context();
	context->bind_graphic_native_surface(dxgi_swap_chain.get());

	driver->DXGIFactory()->MakeWindowAssociation((HWND)view->get_core_view_handle(), DXGI_MWA_NO_ALT_ENTER);

	d3d_frame_buffer = new d3d11_frame_buffer(driver.get());
	d3d_frame_buffer->create(this);

	return true;
}

bool d3d11_surface::update(platform::icore_view_t view, foundation::size<float> size, foundation::size<float> scale)
{
	HRESULT hr = S_OK;
	bool bind = false;

	d3d11_driver_t driver = _parent_driver.lock<d3d11_driver>();
	if (driver.is_empty())
		return false;

	return driver->execute_on_thread_safe([&]() 
	{
		size.height = max(size.height, 10.0f);
		size.width = max(size.width, 10.0f);

		if (dxgi_swap_chain.get())
		{
			if (driver->current_frame_buffer().get() == d3d_frame_buffer.get())
			{
				driver->bind_frame_buffer(nullptr);
				bind = true;
			}
		}

		d3d_frame_buffer->close();

		if (FAILED(dxgi_swap_chain->ResizeBuffers(
			2, // Double-buffered swap chain.
			(uint)maths::round(size.width),
			(uint)maths::round(size.height),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0
		)))
			return false;

		_current_size = size;
		d3d_frame_buffer->create(this);
		if (bind) driver->bind_frame_buffer(d3d_frame_buffer.get());

		return true;
	});
}

bool d3d11_surface::close()
{
	dxgi_swap_chain = null;
	d3d_frame_buffer = null;
	return true;
}

void d3d11_surface::on_display_size_changed_event(objptr sender, ang::platform::events::idisplay_info_event_args_t args)
{
	auto info = args->display_info();
	update(args->core_view(), info.display_resolution, info.display_scale_factor);

}



void d3d11_surface::swap_buffers()
{
	dxgi_swap_chain->Present(1, 0);
}

iframe_buffer_t d3d11_surface::frame_buffer()const
{
	return d3d_frame_buffer.get();
}