
#include "pch.h"
#include "vulkan/driver.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::vulkan;

vulkan_driver::vulkan_driver()
{
	main_mutex = new core::async::mutex();
	_cull_mode = graphics::cull_mode::back;
	_front_face = graphics::front_face::def;
	_blend_mode = graphics::blend_mode::disable;

	_device = new vulkan::VKInternalDevice();
	_vk_instance = VulkanSDK::Instance();
}

vulkan_driver::vulkan_driver(platform::icore_view_t view, vulkan_surface_ptr_t out)
{
	main_mutex = new core::async::mutex();
	_cull_mode = graphics::cull_mode::back;
	_front_face = graphics::front_face::def;
	_blend_mode = graphics::blend_mode::disable;
	VkSurfaceKHR vk_surface;
	_vk_instance = VulkanSDK::Initialize();

	auto app = platform::icore_app::get_core_app();
#ifdef WINDOWS_PLATFORM
	VkWin32SurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  // VkStructureType                  sType
		nullptr,                                          // const void                      *pNext
		0,                                                // VkWin32SurfaceCreateFlagsKHR     flags
		(HMODULE)app->get_core_app_handle(),              // HINSTANCE                        hinstance
		(HWND)view->get_core_view_handle()                // HWND                             hwnd
	};

	if (VulkanSDK::vkCreateWin32SurfaceKHR(_vk_instance, &surface_create_info, nullptr, &vk_surface) == VK_SUCCESS) {
		_device = new vulkan::VKInternalDevice();
		VulkanSDK::Release();
		return;
	}

#elif defined(VK_USE_PLATFORM_XCB_KHR)
	VkXcbSurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,    // VkStructureType                  sType
		nullptr,                                          // const void                      *pNext
		0,                                                // VkXcbSurfaceCreateFlagsKHR       flags
		Window.Connection,                                // xcb_connection_t*                connection
		Window.Handle                                     // xcb_window_t                     window
	};

	if (vkCreateXcbSurfaceKHR(driver->vk_instance(), &surface_create_info, nullptr, &vk_surface) == VK_SUCCESS) {
		_device = new vulkan::VKInternalDevice();
		VulkanSDK::Release();
		return;
	}

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	VkXlibSurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,   // VkStructureType                sType
		nullptr,                                          // const void                    *pNext
		0,                                                // VkXlibSurfaceCreateFlagsKHR    flags
		Window.DisplayPtr,                                // Display                       *dpy
		Window.Handle                                     // Window                         window
	};
	if (vkCreateXlibSurfaceKHR(driver->vk_instance(), &surface_create_info, nullptr, &vk_surface) == VK_SUCCESS) {
		_device = new vulkan::VKInternalDevice();
		VulkanSDK::Release();
		return;
	}

#endif

	_device = new vulkan::VKInternalDevice(vk_surface);

	if ((vulkan_surface**)out == null)
	{
		VulkanSDK::vkDestroySurfaceKHR(_vk_instance, vk_surface, null);
	}
	else
	{
		vulkan_surface_t surface = new vulkan_surface(const_cast<vulkan_driver*>(this));
		if (!surface->create(view, vk_surface))
		{
			*out = null;
			VulkanSDK::vkDestroySurfaceKHR(_vk_instance, vk_surface, null);
		}
		else
		{
			*out = surface;
		}
	}

	VulkanSDK::Release();
}

vulkan_driver::~vulkan_driver()
{
	_device = null;
}



ANG_IMPLEMENT_CLASSNAME(ang::graphics::vulkan::vulkan_driver);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::vulkan::vulkan_driver);

bool vulkan_driver::is_child_of(type_name_t name)
{
	return name == type_name<vulkan_driver>()
		|| object::is_child_of(name)
		|| idriver::is_child_of(name);
}

bool vulkan_driver::is_kind_of(type_name_t name)const
{
	return name == type_name<vulkan_driver>()
		|| object::is_kind_of(name)
		|| idriver::is_kind_of(name);
}

bool vulkan_driver::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<vulkan_driver>())
	{
		*out = static_cast<vulkan_driver*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (idriver::query_object(name, out))
	{
		return true;
	}
	return false;
}

bool vulkan_driver::init_driver()
{
	return false;
}

void vulkan_driver::close_driver()
{
}



isurface_t vulkan_driver::create_surface(platform::icore_view_t view)const
{
	VkSurfaceKHR vk_surface;
	auto app = platform::icore_app::get_core_app();
#ifdef WINDOWS_PLATFORM
	VkWin32SurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  // VkStructureType                  sType
		nullptr,                                          // const void                      *pNext
		0,                                                // VkWin32SurfaceCreateFlagsKHR     flags
		(HMODULE)app->get_core_app_handle(),              // HINSTANCE                        hinstance
		(HWND)view->get_core_view_handle()                // HWND                             hwnd
	};

	if (VulkanSDK::vkCreateWin32SurfaceKHR(_vk_instance, &surface_create_info, nullptr, &vk_surface) == VK_SUCCESS) {
		return null;
	}

#elif defined(VK_USE_PLATFORM_XCB_KHR)
	VkXcbSurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,    // VkStructureType                  sType
		nullptr,                                          // const void                      *pNext
		0,                                                // VkXcbSurfaceCreateFlagsKHR       flags
		Window.Connection,                                // xcb_connection_t*                connection
		Window.Handle                                     // xcb_window_t                     window
	};

	if (vkCreateXcbSurfaceKHR(driver->vk_instance(), &surface_create_info, nullptr, &vk_surface) == VK_SUCCESS) {
		return null;
	}

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	VkXlibSurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,   // VkStructureType                sType
		nullptr,                                          // const void                    *pNext
		0,                                                // VkXlibSurfaceCreateFlagsKHR    flags
		Window.DisplayPtr,                                // Display                       *dpy
		Window.Handle                                     // Window                         window
	};
	if (vkCreateXlibSurfaceKHR(driver->vk_instance(), &surface_create_info, nullptr, &vk_surface) == VK_SUCCESS) {
		return null;
	}

#endif
	if (!_device->IsPresentSupported())
	{
		VKInternalDevice_t device = new VKInternalDevice(vk_surface);
		if (device.is_empty())
		{
			VulkanSDK::vkDestroySurfaceKHR(_vk_instance, vk_surface, null);
			return null;
		}
			
		const_cast<vulkan_driver*>(this)->_device = device;
	}
	vulkan_surface_t surface = new vulkan_surface(const_cast<vulkan_driver*>(this));
	if (!surface->create(view, vk_surface))
	{
		VulkanSDK::vkDestroySurfaceKHR(_vk_instance, vk_surface, null);
		return null;
	}

	return surface;
}

effects::ieffect_library_t vulkan_driver::create_effect_library()const
{
	return null;
}

textures::itexture_loader_t vulkan_driver::create_texture_loader()const
{
	return null;
}

buffers::ivertex_buffer_t vulkan_driver::create_vertex_buffer(buffers::buffer_usage_t usage, static_array<reflect::attribute_desc> vertex_desc, wsize vertex_count, static_array<byte> init_data)const
{
	return null;
}

buffers::iindex_buffer_t vulkan_driver::create_index_buffer(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, static_array<byte> init_data)const
{
	return null;
}

void vulkan_driver::cull_mode(cull_mode_t)
{
}

cull_mode_t vulkan_driver::cull_mode()const
{
	return _cull_mode;
}

void vulkan_driver::front_face(front_face_t)
{
}

front_face_t vulkan_driver::front_face()const
{
	return _front_face;
}

void vulkan_driver::blend_mode(blend_mode_t)
{
}

blend_mode_t vulkan_driver::blend_mode()const
{
	return _blend_mode;
}

void vulkan_driver::bind_frame_buffer(iframe_buffer_t) {}
void vulkan_driver::bind_shaders(effects::ishaders_t) {}
void vulkan_driver::bind_texture(textures::itexture_t, index) {}
void vulkan_driver::bind_index_buffer(buffers::iindex_buffer_t) {}
void vulkan_driver::bind_vertex_buffer(buffers::ivertex_buffer_t) {}

void vulkan_driver::clear(color_t) {}
void vulkan_driver::clear(maths::float4 const&) {}
void vulkan_driver::draw(uint count, primitive_t) {}
void vulkan_driver::draw_indexed(uint count, primitive_t) {}

core::async::mutex_t vulkan_driver::driver_guard()const
{
	return main_mutex;
}