#include "pch.h"
#include <ang/platform/platform.h>

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::display;
using namespace ang::platform::input;


safe_enum_rrti(ang::platform, activate_status_t, value<activate_status_proxy>);
safe_enum_rrti(ang::platform, display_invalidate_reason_t, value<activate_status_proxy>);
safe_enum_rrti(ang::platform, display::orientation_t, value<orientation_proxy>);

safe_enum_rrti(ang::platform::input, key_modifiers_t, value<key_modifiers_proxy>);
safe_enum_rrti(ang::platform::input, pointer_hardware_type_t, value<pointer_hardware_type_proxy>);
safe_enum_rrti(ang::platform::input, keyboard_type_t, value<keyboard_type_proxy>);
safe_enum_rrti(ang::platform::input, virtual_key_t, value<virtual_key_proxy>);

safe_flags_implement(ang::platform::input, key_modifiers, uint);

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::input::ikeyboard, ang::interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::imessage_reciever, ang::interface);
//ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::icore_msg_dispatcher, imessage_reciever);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::icore_context, ang::interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::icore_view, ang::interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::icore_app, ang::interface);

