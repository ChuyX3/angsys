#include "pch.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif


#include <ang/graphics/angraph.hpp>
#if DIRECTX_SUPPORT
#include "d3d11/driver.hpp"
#include "d3d11/drawing/draw_context.hpp"
#endif

#if VULKAN_SUPPORT

#endif

#include "ang/graphics/drawing.hpp"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::drawing;

ANG_IMPLEMENT_INTERFACE(ang::graphics, idriver)
ANG_IMPLEMENT_INTERFACE(ang::graphics, isurface)
ANG_IMPLEMENT_INTERFACE(ang::graphics, iframe_buffer)

ANG_IMPLEMENT_INTERFACE(ang::graphics::effects, ishaders)
ANG_IMPLEMENT_INTERFACE(ang::graphics::effects, ieffect)
ANG_IMPLEMENT_INTERFACE(ang::graphics::effects, ieffect_library)

ANG_IMPLEMENT_ENUM(ang::graphics, cull_mode, uint, cull_mode::none);
ANG_IMPLEMENT_ENUM(ang::graphics, front_face, uint, front_face::def);
ANG_IMPLEMENT_ENUM(ang::graphics, blend_mode, uint, blend_mode::disable);


ANG_IMPLEMENT_INTERFACE(ang::graphics::drawing, idraw_context)
ANG_IMPLEMENT_INTERFACE(ang::graphics::drawing, ibrush)


idriver_t graphics::create_graphic_driver(graph_driver_type_t type, platform::icore_view_t view, isurface_ptr_t surface)
{
	idriver_t driver = null;
	switch (type.get())
	{
#if DIRECTX_SUPPORT
	case graph_driver_type::DirectX11:
	{
		driver = new d3d11::d3d11_driver();
		if (!view.is_empty() && (isurface**)surface != null)
			*surface = driver->create_surface(view);
		return driver;
	}
#endif//DIRECTX_SUPPORT

#if VULKAN_SUPPORT
	case graph_driver_type::Vulkan:
	{
		driver = new d3d11::d3d11_driver();
		if (!view.is_empty() && (isurface**)surface != null)
			*surface = driver->create_surface(view);
		return driver;
	}
#endif//VULKAN_SUPPORT

	default:
		return null;
	}
}

drawing::idraw_context_t graphics::drawing::create_drawing_context(idriver_t driver, core::files::ifile_system_t fs)
{
	if (driver.is_empty())
		return null;
	switch (driver->graph_driver_type().get())
	{
#if DIRECTX_SUPPORT
	case graph_driver_type::DirectX11:	
		return new d3d11::d3d11_draw_context(static_cast<d3d11::d3d11_driver*>(driver.get()), fs);
#endif//DIRECTX_SUPPORT

#if VULKAN_SUPPORT
	case graph_driver_type::Vulkan:
		return null;
#endif//VULKAN_SUPPORT

	default:
		return null;
	}
}