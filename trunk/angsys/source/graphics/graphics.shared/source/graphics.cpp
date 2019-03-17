#include "pch.h"
#include <ang/graphics/graphics.h>

#if DIRECTX11_SUPPORT
#include "d3d11/d3d11_driver.h"
//#include "d3d11/drawing/d3d11_drawer.h"
#endif

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::idriver, intf)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::scenes::icamera, intf)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::isurface, intf)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::iframe_buffer, intf)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::ifactory, intf)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::resources::iresource, intf)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::effects::ishaders, intf)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::effects::ipass, intf)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::effects::ieffect, intf)

#define MY_TYPE ang::graphics::idriver
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::graphics::isurface
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::graphics::iframe_buffer
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::graphics::effects::ishaders
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

//ANG_IMPLEMENT_ENUM(ang::graphics, cull_mode, uint, cull_mode::none);
//ANG_IMPLEMENT_ENUM(ang::graphics, front_face, uint, front_face::def);
//ANG_IMPLEMENT_ENUM(ang::graphics, blend_mode, uint, blend_mode::disable);


using namespace ang;
using namespace ang::graphics;

idriver_t ang::graphics::create_graphic_driver(graph_driver_type_t type, long64 adapter_id, platform::icore_view_t view, isurface_ptr_t surface)
{
	idriver_t driver = null;
	switch (type.get())
	{
#if DIRECTX11_SUPPORT
	case graph_driver_type::DirectX11:
	{
		d3d11::d3d11_driver_t d3d_driver = new d3d11::d3d11_driver();
		if (!d3d_driver->init_driver(view, adapter_id))
			return null;
		driver = d3d_driver.get();
		if (!view.is_empty() && (isurface**)surface != null)
			*surface = driver->create_surface(view);
		return driver;
	}
#if DIRECTX11_VRX_SUPPORT
	case graph_driver_type::DirectX11_VRX:
	{
		d3d11::d3d11_driver_t d3d_driver = new d3d11::holographic_driver();
		if (!d3d_driver->init_driver(view, adapter_id))
			return null;
		driver = d3d_driver.get();
		if (!view.is_empty() && (isurface**)surface != null)
			*surface = driver->create_surface(view);
		return driver;
	}
#endif
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