#include "pch.h"
#include <ang/graphics/graphics.h>

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif


using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::resources;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::resources::iresource, intf)
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::resources::imodel_loader, intf);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::resources::ilibrary, intf);

safe_enum_rrti2(ang::graphics::resources, light_type);