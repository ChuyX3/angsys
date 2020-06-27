#include "pch.h"
#include <ang/platform/platform.h>

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::display;
using namespace ang::platform::input;


ANG_ENUM_IMPLEMENT(ang::platform, activate_status);
ANG_ENUM_IMPLEMENT(ang::platform, display_invalidate_reason);
ANG_ENUM_IMPLEMENT(ang::platform, display::orientation);

ANG_FLAGS_IMPLEMENT(ang::platform::input, key_modifiers);
ANG_ENUM_IMPLEMENT(ang::platform::input, key_state);
ANG_ENUM_IMPLEMENT(ang::platform::input, pointer_hardware_type);
ANG_ENUM_IMPLEMENT(ang::platform::input, keyboard_type);
ANG_ENUM_IMPLEMENT(ang::platform::input, virtual_key);

ANG_ENUM_IMPLEMENT(ang::platform::input, controller_status);
ANG_ENUM_FLAGS_IMPLEMENT(ang::platform::input, controller_button, controller_buttons_state);

//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::input::ikeyboard);
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::input::icontroller);
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::input::icontroller_manager);

//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::imessage_listener);
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::imessage_dispatcher);
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::icore_context);
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::icore_view);
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::icore_app);

namespace ang::platform {
	extern icore_app* s_current_app = null;
}

input::icontroller_manager_t s_controller_manager = null;

icore_app_t icore_app::current_app()
{
	return s_current_app;
}

input::icontroller_manager_t input::icontroller_manager::instance()
{
	return s_controller_manager.get();
}

