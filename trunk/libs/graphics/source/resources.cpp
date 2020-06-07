#include "pch.h"
#include <ang/graphics/graphics.h>

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif


using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::resources;

//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::resources::iresource, lattelib)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::resources::imodel_loader, intf);
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::resources::ilibrary, lattelib);

//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::meshes::imaterial, lattelib)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::meshes::igeometry, lattelib)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::meshes::imesh, lattelib)
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::meshes::imesh_loader, lattelib)

ANG_ENUM_IMPLEMENT(ang::graphics::resources, resource_type)
ANG_ENUM_IMPLEMENT(ang::graphics::resources, light_type)

