#include "pch.h"
#include <ang/core/timer.h>
#include "ang/platform/angwin/angwin.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;
using namespace ang::platform::windows;

ANG_IMPLEMENT_ENUM(ang::platform::events, win_msg_enum, core_msg_t, win_msg_enum::none);



msg_event_args::msg_event_args(message_t msg)
	: msg(msg)
{

}

msg_event_args::~msg_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::msg_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::msg_event_args);
bool msg_event_args::is_inherited_of(type_name_t name)
{
	return name == type_of<msg_event_args>()
		|| object::is_inherited_of(name)
		|| imsg_event_args::is_inherited_of(name);
}

bool msg_event_args::is_kind_of(type_name_t name)const
{
	return name == type_of<msg_event_args>()
		|| object::is_kind_of(name)
		|| imsg_event_args::is_kind_of(name);
}

bool msg_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<msg_event_args>())
	{
		*out = static_cast<msg_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (imsg_event_args::query_object(name, out))
	{
		return true;
	}
	return false;
}

const message_t& msg_event_args::msg_info()const
{
	return msg;
}

void msg_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool msg_event_args::handled()const
{
	return msg->result() != -1;
}

////////////////////////////////////////////////////////////////////

app_status_event_args::app_status_event_args(message_t msg)
	: msg(msg)
{

}

app_status_event_args::~app_status_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::app_status_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::app_status_event_args);

bool app_status_event_args::is_inherited_of(type_name_t name)
{
	return name == type_of<app_status_event_args>()
		|| object::is_inherited_of(name)
		|| iapp_status_event_args::is_inherited_of(name);
}

bool app_status_event_args::is_kind_of(type_name_t name)const
{
	return name == type_of<app_status_event_args>()
		|| object::is_kind_of(name)
		|| iapp_status_event_args::is_kind_of(name);
}

bool app_status_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<app_status_event_args>())
	{
		*out = static_cast<app_status_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (iapp_status_event_args::query_object(name, out))
	{
		return true;
	}
	return false;
}


const message_t& app_status_event_args::msg_info()const
{
	return msg;
}

icore_app_t app_status_event_args::core_app()const
{
	return msg->arg(0).as<app>().get();
}

//core::Files::IFileSystem* app_status_event_args::FileSystem()const
//{
//	return reinterpret_cast<core::Files::IFileSystem*>(msg.arg2());
//}

void app_status_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool app_status_event_args::handled()const
{
	return msg->result() != -1;
}

////////////////////////////////////////////////////////////////////

created_event_args::created_event_args(message_t msg)
	: msg(msg)
{

}

created_event_args::~created_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::created_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::created_event_args);

bool created_event_args::is_inherited_of(type_name_t name)
{
	return name == type_of<created_event_args>()
		|| object::is_inherited_of(name)
		|| icreated_event_args::is_inherited_of(name);
}

bool created_event_args::is_kind_of(type_name_t name)const
{
	return name == type_of<created_event_args>()
		|| object::is_kind_of(name)
		|| icreated_event_args::is_kind_of(name);
}

bool created_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<created_event_args>())
	{
		*out = static_cast<created_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (icreated_event_args::query_object(name, out))
	{
		return true;
	}
	return false;
}

const message_t& created_event_args::msg_info()const
{
	return msg;
}

icore_view_t created_event_args::core_view()const
{
	return msg->arg(0).as<window>().get();
}

icore_app_t created_event_args::core_app()const
{
	return static_cast<icore_app*>(app::current_app());
}

var_args_t created_event_args::args_list()const
{
	return msg->arg(1).as<var_args>().get();
}

void created_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool created_event_args::handled()const
{
	return msg->result() != -1;
}
////////////////////////////////////////////////////////////////////


update_event_args::update_event_args(message_t msg)
	: msg(msg)
{

}

update_event_args::~update_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::update_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::update_event_args);

bool update_event_args::is_inherited_of(type_name_t name)
{
	return name == type_of<update_event_args>()
		|| object::is_inherited_of(name)
		|| iupdate_event_args::is_inherited_of(name);
}

bool update_event_args::is_kind_of(type_name_t name)const
{
	return name == type_of<update_event_args>()
		|| object::is_kind_of(name)
		|| iupdate_event_args::is_kind_of(name);
}

bool update_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<update_event_args>())
	{
		*out = static_cast<update_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (iupdate_event_args::query_object(name, out))
	{
		return true;
	}
	return false;
}


const message_t& update_event_args::msg_info()const
{
	return msg;
}

shared_ptr<core::time::step_timer> update_event_args::step_timer()const
{
	return reinterpret_cast<value_wrapper<core::time::step_timer>*>(msg->lparam());
}

void update_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool update_event_args::handled()const
{
	return msg->result() != -1;
}

////////////////////////////////////////////////////////////////////

draw_event_args::draw_event_args(message_t msg)
	: msg(msg)
{

}

draw_event_args::~draw_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::draw_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::draw_event_args);

bool draw_event_args::is_inherited_of(type_name_t name)
{
	return name == type_of<draw_event_args>()
		|| object::is_inherited_of(name)
		|| idraw_event_args::is_inherited_of(name);
}

bool draw_event_args::is_kind_of(type_name_t name)const
{
	return name == type_of<draw_event_args>()
		|| object::is_kind_of(name)
		|| idraw_event_args::is_kind_of(name);
}

bool draw_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<draw_event_args>())
	{
		*out = static_cast<draw_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (idraw_event_args::query_object(name, out))
	{
		return true;
	}
	return false;
}

const message_t& draw_event_args::msg_info()const
{
	return msg;
}

icore_view_t draw_event_args::core_view()const
{
	return msg->arg(0).as<window>().get();
}

icore_context_t draw_event_args::core_context()const
{
	return msg->arg(1).as<graphics::device_context>().get();
}

graphics::size<float> draw_event_args::canvas_size()const
{
	return core_view()->get_core_view_size();
}

void draw_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool draw_event_args::handled()const
{
	return msg->result() != -1;
}

////////////////////////////////////////////////////////////////////

display_info_event_args::display_info_event_args(message_t msg)
	: msg(msg)
{

}

display_info_event_args::~display_info_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::display_info_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::display_info_event_args);

bool display_info_event_args::is_inherited_of(type_name_t name)
{
	return name == type_of<display_info_event_args>()
		|| object::is_inherited_of(name)
		|| idisplay_info_event_args::is_inherited_of(name);
}

bool display_info_event_args::is_kind_of(type_name_t name)const
{
	return name == type_of<display_info_event_args>()
		|| object::is_kind_of(name)
		|| idisplay_info_event_args::is_kind_of(name);
}

bool display_info_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<display_info_event_args>())
	{
		*out = static_cast<display_info_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (idisplay_info_event_args::query_object(name, out))
	{
		return true;
	}
	return false;
}

const message_t& display_info_event_args::msg_info()const
{
	return msg;
}

icore_view_t display_info_event_args::core_view()const
{
	return msg->arg(0).as<window>().get();
}

struct display_info_event_args2 {
	display_invalidate_reason_t state;
	display::display_info info;
};

ANG_REGISTER_RUNTIME_TYPENAME(display_info_event_args2);

display_invalidate_reason_t display_info_event_args::invalidate_reason()const
{
	return msg->arg(1).as<display_info_event_args2>()->state;
}

display::display_info_t display_info_event_args::display_info()const
{
	return msg->arg(1).as<display_info_event_args2>()->info;
}

void display_info_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool display_info_event_args::handled()const
{
	return msg->result() != -1;
}

//////////////////////////////////////////////////////////////////////

visibility_change_event_args::visibility_change_event_args(message_t)
	: msg(msg)
{

}

visibility_change_event_args::~visibility_change_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::visibility_change_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::visibility_change_event_args);

bool visibility_change_event_args::is_inherited_of(type_name_t name)
{
	return name == type_of<visibility_change_event_args>()
		|| object::is_inherited_of(name)
		|| ivisibility_change_event_args::is_inherited_of(name);
}

bool visibility_change_event_args::is_kind_of(type_name_t name)const
{
	return name == type_of<visibility_change_event_args>()
		|| object::is_kind_of(name)
		|| ivisibility_change_event_args::is_kind_of(name);
}

bool visibility_change_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<visibility_change_event_args>())
	{
		*out = static_cast<visibility_change_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (ivisibility_change_event_args::query_object(name, out))
	{
		return true;
	}
	return false;
}


const message_t& visibility_change_event_args::visibility_change_event_args::msg_info()const
{
	return msg;
}

icore_view_t visibility_change_event_args::visibility_change_event_args::core_view()const
{
	return msg->arg(0).as<window>().get();
}

bool visibility_change_event_args::visibility_change_event_args::is_visible()const
{
	objptr args;
	return (args = msg->arg(1)).is_empty() ? false : (bool)args.as<bool>();
}

void visibility_change_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool visibility_change_event_args::handled()const
{
	return msg->result() != -1;
}

////////////////////////////////////////////////////////////////////

activate_event_args::activate_event_args(message_t msg)
	: msg(msg)
{

}

activate_event_args::~activate_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::activate_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::activate_event_args);

bool activate_event_args::is_inherited_of(type_name_t name)
{
	return name == type_of<activate_event_args>()
		|| object::is_inherited_of(name)
		|| iactivate_event_args::is_inherited_of(name);
}

bool activate_event_args::is_kind_of(type_name_t name)const
{
	return name == type_of<activate_event_args>()
		|| object::is_kind_of(name)
		|| iactivate_event_args::is_kind_of(name);
}

bool activate_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<activate_event_args>())
	{
		*out = static_cast<activate_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (iactivate_event_args::query_object(name, out))
	{
		return true;
	}
	return false;
}

const message_t& activate_event_args::msg_info()const
{
	return msg;
}

activate_status_t activate_event_args::status()const
{
	return (activate_status)(uint)msg->arg(0).as<uint>();
}

void activate_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool activate_event_args::handled()const
{
	return msg->result() != -1;
}

////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////

//TODO:
//TOCHECK:
//TOFIX

pointer_event_args::pointer_event_args(message_t msg)
	: msg(msg)
{

}

pointer_event_args::~pointer_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::pointer_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::pointer_event_args);

bool pointer_event_args::is_inherited_of(type_name_t name)
{
	return name == type_of<pointer_event_args>()
		|| object::is_inherited_of(name)
		|| ipointer_event_args::is_inherited_of(name);
}

bool pointer_event_args::is_kind_of(type_name_t name)const
{
	return name == type_of<pointer_event_args>()
		|| object::is_kind_of(name)
		|| ipointer_event_args::is_kind_of(name);
}

bool pointer_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<pointer_event_args>())
	{
		*out = static_cast<pointer_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (ipointer_event_args::query_object(name, out))
	{
		return true;
	}
	return false;
}

const message_t& pointer_event_args::msg_info()const
{
	return msg;
}

graphics::point<float> pointer_event_args::position()const
{
	return msg->arg(0).as<input::poiner_info>()->point;
}

input::key_modifiers_t pointer_event_args::modifiers()const
{
	return msg->arg(0).as<input::poiner_info>()->modifiers;
}

input::poiner_info_t pointer_event_args::info()const
{
	return (input::poiner_info)msg->arg(0).as<input::poiner_info>();
}

void pointer_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool pointer_event_args::handled()const
{
	return msg->result() != -1;
}

////////////////////////////////////////////////////////////////////

key_event_args::key_event_args(message_t msg)
	: msg(msg)
{

}

key_event_args::~key_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::key_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::key_event_args);

bool key_event_args::is_inherited_of(type_name_t name)
{
	return name == type_of<key_event_args>()
		|| object::is_inherited_of(name)
		|| ikey_event_args::is_inherited_of(name);
}

bool key_event_args::is_kind_of(type_name_t name)const
{
	return name == type_of<key_event_args>()
		|| object::is_kind_of(name)
		|| ikey_event_args::is_kind_of(name);
}

bool key_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<key_event_args>())
	{
		*out = static_cast<key_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (ikey_event_args::query_object(name, out))
	{
		return true;
	}
	return false;
}

const message_t& key_event_args::msg_info()const
{
	return msg;
}

uint key_event_args::key()const
{
	return (uint)msg->arg(0).as<uint>();
}

input::key_modifiers_t key_event_args::modifiers()const
{
	word modifiers = 0;
	if (GetKeyState(VK_CONTROL) && 0x8000)
		modifiers |= (word)input::key_modifiers::control;
	if (GetKeyState(VK_SHIFT) && 0x8000)
		modifiers |= (word)input::key_modifiers::shift;
	if (GetKeyState(VK_MENU) && 0x8000)
		modifiers |= (word)input::key_modifiers::alt;
	if (GetKeyState(VK_CAPITAL) && 0x0001)
		modifiers |= (word)input::key_modifiers::caps_lock;
	if (GetKeyState(VK_NUMLOCK) && 0x0001)
		modifiers |= (word)input::key_modifiers::num_lock;
	return input::key_modifiers(modifiers);
}

word key_event_args::flags()const
{
	uint value = (uint)msg->arg(1).as<uint>();
	return word((value & 0XFFFF0000) >> 16);
}

bool key_event_args::is_pressed()const
{
	uint value = (uint)msg->arg(1).as<uint>();
	return (value & 0X0000FFFF) != 0;
}

void key_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool key_event_args::handled()const
{
	return msg->result() != -1;
}

////////////////////////////////////////////////////////////////////

text_change_event_args::text_change_event_args(message_t msg)
	: msg(msg)
{

}

text_change_event_args::~text_change_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::text_change_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::text_change_event_args);

bool text_change_event_args::is_inherited_of(type_name_t name)
{
	return name == type_of<text_change_event_args>()
		|| object::is_inherited_of(name)
		|| itext_change_event_args::is_inherited_of(name);
}

bool text_change_event_args::is_kind_of(type_name_t name)const
{
	return name == type_of<text_change_event_args>()
		|| object::is_kind_of(name)
		|| itext_change_event_args::is_kind_of(name);
}

bool text_change_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<text_change_event_args>())
	{
		*out = static_cast<text_change_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (itext_change_event_args::query_object(name, out))
	{
		return true;
	}
	return false;
}

const message_t& text_change_event_args::msg_info()const
{
	return msg;
}

mstring text_change_event_args::text()const
{
	return msg->arg(0).as<mstring>();
}

input::ikeyboard_t text_change_event_args::keyboard()const
{
	return msg->arg(1).as<input::ikeyboard>();
}

void text_change_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool text_change_event_args::handled()const
{
	return msg->result() != -1;
}


