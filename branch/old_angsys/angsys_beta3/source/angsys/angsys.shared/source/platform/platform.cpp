#include "pch.h"
#include <ang/platform/platform.h>

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::display;
using namespace ang::platform::input;


safe_enum_rrti2(ang::platform, activate_status);
safe_enum_rrti2(ang::platform, display_invalidate_reason);
safe_enum_rrti2(ang::platform, display::orientation);

safe_enum_rrti2(ang::platform::input, key_modifiers);
safe_enum_rrti2(ang::platform::input, pointer_hardware_type);
safe_enum_rrti2(ang::platform::input, keyboard_type);
safe_enum_rrti2(ang::platform::input, virtual_key);


safe_enum_rrti2(ang::platform::input, controller_button);
safe_enum_rrti2(ang::platform::input, controller_status);
safe_enum_rrti2(ang::platform::input, controller_buttons_state);

safe_flags_implement(ang::platform::input, key_modifiers);
safe_flags_implement(ang::platform::input, controller_buttons_state);


ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::input::ikeyboard, ang::intf);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::input::icontroller, ang::intf);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::input::icontroller_manager, ang::intf);

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::imessage_listener, ang::intf);
//ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::icore_msg_dispatcher, imessage_reciever);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::icore_context, ang::intf);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::icore_view, ang::intf);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::icore_app, ang::intf);

static icore_app* s_current_app = null;
static input::icontroller_manager_t s_controller_manager = null;

icore_app_t icore_app::current_app()
{
	return s_current_app;
}

input::icontroller_manager_t input::icontroller_manager::get_instance()
{
	return s_controller_manager.get();
}

#if defined WINDOWS_PLATFORM && defined ANGSYS_DYNAMIC_LIBRARY
__declspec(dllexport) void ang_platform_core_app_set_instance(icore_app* instance)
{
	s_current_app = instance;
}

__declspec(dllexport) void ang_platform_core_app_reset_instance()
{
	s_current_app = null;
}
__declspec(dllexport) void ang_platform_input_controller_manager_set_instance(input::icontroller_manager* instance)
{
	s_controller_manager = instance;
}

__declspec(dllexport) void ang_platform_input_controller_manager_reset_instance()
{
	s_controller_manager = null;
}
#else
void ang_platform_core_app_set_instance(icore_app* instance)
{
	s_current_app = instance;
}

void ang_platform_core_app_reset_instance()
{
	s_current_app = null;
}
void ang_platform_input_controller_manager_set_instance(input::icontroller_manager* instance)
{
	s_controller_manager = instance;
}

void ang_platform_input_controller_manager_reset_instance()
{
	s_controller_manager = null;
}
#endif