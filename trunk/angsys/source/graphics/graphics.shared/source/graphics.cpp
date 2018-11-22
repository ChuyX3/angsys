#include "pch.h"
#include <ang/graphics/graphics.h>

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif


ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::idriver, interface)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::isurface, interface)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::iframe_buffer, interface)

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::effects::ishaders, interface)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::effects::ieffect, interface)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::effects::ieffect_library, interface)

//ANG_IMPLEMENT_ENUM(ang::graphics, cull_mode, uint, cull_mode::none);
//ANG_IMPLEMENT_ENUM(ang::graphics, front_face, uint, front_face::def);
//ANG_IMPLEMENT_ENUM(ang::graphics, blend_mode, uint, blend_mode::disable);


using namespace ang;
using namespace ang::graphics;



using namespace ang::graphics;
idriver_t ang::graphics::create_graphic_driver(graph_driver_type_t type, platform::icore_view_t view, isurface_ptr_t surface)
{
	return null;
}