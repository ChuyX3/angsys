#include "pch.h"
#include <ang/graphics/graphics.h>

#if DIRECTX11_SUPPORT
#include "d3d11/d3d11_driver.h"
#include "d3d11/drawing/d3d11_drawer.h"
#endif

#if OPENGL_FAMILY_SUPPORT
#include "GL/gl_driver.h"
#endif


//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::idriver)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::isurface)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::iframe_buffer)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::ifactory)

//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::scenes::inode)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::scenes::itransform)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::scenes::icamera)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::scenes::imodel)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::scenes::iscene)

//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::effects::ishaders)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::effects::ipass)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::effects::ieffect)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::effects::ieffect_library);

//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::drawing::ibrush)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::drawing::idrawer)

ANG_ENUM_IMPLEMENT(ang::graphics, blend_mode)
ANG_ENUM_IMPLEMENT(ang::graphics, cull_mode)
ANG_ENUM_IMPLEMENT(ang::graphics, front_face)

ANG_ENUM_IMPLEMENT(ang::graphics::scenes, node_type)

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

drawing::idrawer_t ang::graphics::drawing::create_drawing_context(idriver_t driver)
{
	switch (driver->graph_driver_type())
	{
#if DIRECTX11_SUPPORT
	case graph_driver_type::DirectX11:
	{
		d3d11::d3d11_drawer_t context;
		d3d11::d3d11_driver_t d3d_driver = interface_cast<d3d11::d3d11_driver>(driver.get());
		if (d3d_driver.is_empty())
			return null;
		context = new d3d11::d3d11_drawer(d3d_driver);
		return context.get();
	}
#if DIRECTX11_VRX_SUPPORT
	case graph_driver_type::DirectX11_VRX:
	{
		d3d11::d3d11_drawer_t context;
		d3d11::d3d11_driver_t d3d_driver = interface_cast<d3d11::d3d11_driver>(driver.get());
		if (d3d_driver.is_empty())
			return null;
		context = new d3d11::d3d11_drawer(d3d_driver);
		return context.get();
	}
#endif
#endif//DIRECTX_SUPPORT

#if VULKAN_SUPPORT
	case graph_driver_type::Vulkan:
	{
		vulkan::vulkan_drawer_t context;
		vulkan::vulkan_driver_t vk_driver = interface_cast<vulkan::vulkan_driver>(driver.get());
		if (vk_driver.is_empty())
			return null;
		context = new vulkan::vulkan_drawer(vk_driver);
		return context.get();
	}
#endif//VULKAN_SUPPORT

#if OPENGL_FAMILY_SUPPORT
	case graph_driver_type::OpenGL:
	case graph_driver_type::OpenGLES:
	{
		gl::gl_drawer_t context;
		gl::gl_driver_t gl_driver = interface_cast<gl::gl_driver>(driver.get());
		if (gl_driver.is_empty())
			return null;
		context = new gl::gl_drawer(gl_driver);
		return context.get();
	}
#endif//OPENGL_FAMILY_SUPPORT

	default:
		return null;
	}
}

core::async::scope_locker<graphics::idriver_t>::scope_locker(graphics::idriver_t driver)
{
	m_driver = driver;
	if (!m_driver.is_empty())
		m_driver->lock();
}

core::async::scope_locker<graphics::idriver_t>::~scope_locker() {
	if (!m_driver.is_empty())
		m_driver->unlock();
}





