
#include "pch.h"
#include "vulkan/driver.hpp"

#if VULKAN_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::vulkan;

vulkan_surface::vulkan_surface(vulkan_driver* _parent)
	: _parent_driver(_parent)
	, _current_size(1, 1)
	, vk_surface(0)
{

}

vulkan_surface::~vulkan_surface()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::graphics::vulkan::vulkan_surface);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::vulkan::vulkan_surface);

bool vulkan_surface::is_inherited_of(type_name_t name)
{
	return name == type_of<vulkan_surface>()
		|| object::is_inherited_of(name)
		|| isurface::is_inherited_of(name);
}

bool vulkan_surface::is_kind_of(type_name_t name)const
{
	return name == type_of<vulkan_surface>()
		|| object::is_kind_of(name)
		|| isurface::is_kind_of(name);
}

bool vulkan_surface::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<vulkan_surface>())
	{
		*out = static_cast<vulkan_surface*>(this);
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


bool vulkan_surface::create(platform::icore_view_t view, VkSurfaceKHR surface)
{
	vulkan_driver_t driver = _parent_driver.lock<vulkan_driver>();
	if (driver == null)
		return false;
	auto app = platform::icore_app::get_core_app();




	return false;
}

bool vulkan_surface::update(platform::icore_view_t, foundation::size<float> size, foundation::size<float> scale)
{
	return true;
}

bool vulkan_surface::close()
{
	if (vk_surface != 0)
	{
		VulkanSDK::vkDestroySurfaceKHR(VulkanSDK::Instance(), vk_surface, null);
	}
	return true;
}

void vulkan_surface::swap_buffers()
{

}

iframe_buffer_t vulkan_surface::frame_buffer()const
{
	return null;
}


void vulkan_surface::on_display_size_changed_event(objptr, ang::platform::events::idisplay_info_event_args_t)
{

}

#endif
