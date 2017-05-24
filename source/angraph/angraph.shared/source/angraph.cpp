#include "pch.h"

#include <ang/graphics/angraph.h>

using namespace ang;
using namespace ang::graphics;

ANG_IMPLEMENT_INTERFACE(ang::graphics, idriver)
ANG_IMPLEMENT_INTERFACE(ang::graphics, isurface)
ANG_IMPLEMENT_INTERFACE(ang::graphics, iframe_buffer)

ANG_IMPLEMENT_INTERFACE(ang::graphics::effects, ishaders)
ANG_IMPLEMENT_INTERFACE(ang::graphics::effects, ieffect)
ANG_IMPLEMENT_INTERFACE(ang::graphics::effects, ieffect_library)


