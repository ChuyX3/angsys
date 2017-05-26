#include "pch.h"

#include <ang/graphics/angraph.h>

#include "d3d11/driver.h"

using namespace ang;
using namespace ang::graphics;

ANG_IMPLEMENT_INTERFACE(ang::graphics, idriver)
ANG_IMPLEMENT_INTERFACE(ang::graphics, isurface)
ANG_IMPLEMENT_INTERFACE(ang::graphics, iframe_buffer)

ANG_IMPLEMENT_INTERFACE(ang::graphics::effects, ishaders)
ANG_IMPLEMENT_INTERFACE(ang::graphics::effects, ieffect)
ANG_IMPLEMENT_INTERFACE(ang::graphics::effects, ieffect_library)



idriver_t graphics::create_graphic_driver(graph_driver_type_t type)
{
	return new d3d11::d3d11_driver();
}