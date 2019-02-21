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

safe_flags_implement(ang::platform::input, key_modifiers);

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::input::ikeyboard, ang::interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::imessage_listener, ang::interface);
//ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::icore_msg_dispatcher, imessage_reciever);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::icore_context, ang::interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::icore_view, ang::interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::icore_app, ang::interface);

