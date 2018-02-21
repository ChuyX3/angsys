#include "pch.h"
#include <ang/core/time.h>
#include "ang/platform/android/android_platform.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;
using namespace ang::platform::android;

ANG_IMPLEMENT_ENUM(ang::platform::events, android_msg_enum, core_msg_t, android_msg_enum::none);



msg_event_args::msg_event_args(message_t msg)
	: msg(msg)
{

}

msg_event_args::~msg_event_args()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::msg_event_args);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::msg_event_args);
bool msg_event_args::is_child_of(type_name_t name)
{
	if (name == type_name<msg_event_args>()
		|| object::is_child_of(name)
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool msg_event_args::is_kind_of(type_name_t name)const
{
	if (name == type_name<msg_event_args>()
		|| object::is_kind_of(name)
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool msg_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<msg_event_args>())
	{
		*out = static_cast<msg_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<imsg_event_args>())
	{
		*out = static_cast<imsg_event_args*>(this);
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

bool app_status_event_args::is_child_of(type_name_t name)
{
	if (name == type_name<app_status_event_args>()
		|| object::is_child_of(name)
		|| name == type_name<iapp_status_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool app_status_event_args::is_kind_of(type_name_t name)const
{
	if (name == type_name<app_status_event_args>()
		|| object::is_kind_of(name)
		|| name == type_name<iapp_status_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool app_status_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<app_status_event_args>())
	{
		*out = static_cast<app_status_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<iapp_status_event_args>())
	{
		*out = static_cast<iapp_status_event_args*>(this);
		return true;
	}
	else if (name == type_name<imsg_event_args>())
	{
		*out = static_cast<imsg_event_args*>(this);
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
	return reinterpret_cast<activity*>(msg->arg1());
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

bool created_event_args::is_child_of(type_name_t name)
{
	if (name == type_name<created_event_args>()
		|| object::is_child_of(name)
		|| name == type_name<icreated_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool created_event_args::is_kind_of(type_name_t name)const
{
	if (name == type_name<created_event_args>()
		|| object::is_kind_of(name)
		|| name == type_name<icreated_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool created_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<created_event_args>())
	{
		*out = static_cast<created_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<icreated_event_args>())
	{
		*out = static_cast<icreated_event_args*>(this);
		return true;
	}
	else if (name == type_name<imsg_event_args>())
	{
		*out = static_cast<imsg_event_args*>(this);
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
	return reinterpret_cast<android::core_view*>(msg->arg1());
}

icore_app_t created_event_args::core_app()const
{
	return static_cast<icore_app*>(msg->arg2());
}

var_args_t created_event_args::args_list()const
{
	return null;// reinterpret_cast<var_args*>(msg->arg2());
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

bool update_event_args::is_child_of(type_name_t name)
{
	if (name == type_name<update_event_args>()
		|| object::is_child_of(name)
		|| name == type_name<iupdate_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool update_event_args::is_kind_of(type_name_t name)const
{
	if (name == type_name<update_event_args>()
		|| object::is_kind_of(name)
		|| name == type_name<iupdate_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool update_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<update_event_args>())
	{
		*out = static_cast<update_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<iupdate_event_args>())
	{
		*out = static_cast<iupdate_event_args*>(this);
		return true;
	}
	else if (name == type_name<imsg_event_args>())
	{
		*out = static_cast<imsg_event_args*>(this);
		return true;
	}
	return false;
}


const message_t& update_event_args::msg_info()const
{
	return msg;
}

double update_event_args::delta_time()const
{
	return reinterpret_cast<value_wrapper<core::time::timer>*>(msg->arg1())->get().delta();
}

double update_event_args::total_time()const
{
	return reinterpret_cast<value_wrapper<core::time::timer>*>(msg->arg1())->get().total();
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

bool draw_event_args::is_child_of(type_name_t name)
{
	if (name == type_name<draw_event_args>()
		|| object::is_child_of(name)
		|| name == type_name<idraw_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool draw_event_args::is_kind_of(type_name_t name)const
{
	if (name == type_name<draw_event_args>()
		|| object::is_kind_of(name)
		|| name == type_name<idraw_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool draw_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<draw_event_args>())
	{
		*out = static_cast<draw_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<idraw_event_args>())
	{
		*out = static_cast<idraw_event_args*>(this);
		return true;
	}
	else if (name == type_name<imsg_event_args>())
	{
		*out = static_cast<imsg_event_args*>(this);
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
	return reinterpret_cast<android::core_view*>(msg->arg1());
}

icore_context_t draw_event_args::core_context()const
{
	return core_view()->get_core_context();
}

foundation::size<float> draw_event_args::canvas_size()const
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

bool display_info_event_args::is_child_of(type_name_t name)
{
	if (name == type_name<display_info_event_args>()
		|| object::is_child_of(name)
		|| name == type_name<idisplay_info_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool display_info_event_args::is_kind_of(type_name_t name)const
{
	if (name == type_name<display_info_event_args>()
		|| object::is_kind_of(name)
		|| name == type_name<idisplay_info_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool display_info_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<display_info_event_args>())
	{
		*out = static_cast<display_info_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<idisplay_info_event_args>())
	{
		*out = static_cast<idisplay_info_event_args*>(this);
		return true;
	}
	else if (name == type_name<imsg_event_args>())
	{
		*out = static_cast<imsg_event_args*>(this);
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
	return reinterpret_cast<android::core_view*>(msg->arg1());
}


display_invalidate_reason_t display_info_event_args::invalidate_reason()const
{
	struct display_info_event_args2 {
		display_invalidate_reason_t state;
		display::display_info info;
	};
	return reinterpret_cast<display_info_event_args2*>(msg->arg2())->state;
}

display::display_info_t display_info_event_args::display_info()const
{
	struct display_info_event_args2 {
		display_invalidate_reason_t state;
		display::display_info info;
	};
	return reinterpret_cast<display_info_event_args2*>(msg->arg2())->info;
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

bool visibility_change_event_args::is_kind_of(type_name_t name)const
{
	if (name == type_name<visibility_change_event_args>()
		|| object::is_kind_of(name)
		|| name == type_name<ivisibility_change_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool visibility_change_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<visibility_change_event_args>())
	{
		*out = static_cast<visibility_change_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<ivisibility_change_event_args>())
	{
		*out = static_cast<ivisibility_change_event_args*>(this);
		return true;
	}
	else if (name == type_name<imsg_event_args>())
	{
		*out = static_cast<imsg_event_args*>(this);
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
	return reinterpret_cast<android::core_view*>(msg->arg1());
}

bool visibility_change_event_args::visibility_change_event_args::is_visible()const
{
	return msg->arg2() != null;
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

bool activate_event_args::is_kind_of(type_name_t name)const
{
	if (name == type_name<activate_event_args>()
		|| object::is_kind_of(name)
		|| name == type_name<iactivate_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool activate_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<activate_event_args>())
	{
		*out = static_cast<activate_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<iactivate_event_args>())
	{
		*out = static_cast<iactivate_event_args*>(this);
		return true;
	}
	else if (name == type_name<imsg_event_args>())
	{
		*out = static_cast<imsg_event_args*>(this);
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
	return activate_status(reinterpret_cast<wsize>(msg->arg1()));
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

bool pointer_event_args::is_child_of(type_name_t name)
{
	if (name == type_name<pointer_event_args>()
		|| object::is_child_of(name)
		|| name == type_name<ipointer_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool pointer_event_args::is_kind_of(type_name_t name)const
{
	if (name == type_name<pointer_event_args>()
		|| object::is_kind_of(name)
		|| name == type_name<ipointer_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool pointer_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<pointer_event_args>())
	{
		*out = static_cast<pointer_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<ipointer_event_args>())
	{
		*out = static_cast<ipointer_event_args*>(this);
		return true;
	}
	else if (name == type_name<imsg_event_args>())
	{
		*out = static_cast<imsg_event_args*>(this);
		return true;
	}
	return false;
}

const message_t& pointer_event_args::msg_info()const
{
	return msg;
}

foundation::point<float> pointer_event_args::position()const
{
	return reinterpret_cast<input::poiner_info*>(msg->arg1())->point;
}

input::key_modifiers_t pointer_event_args::modifiers()const
{
	return reinterpret_cast<input::poiner_info*>(msg->arg1())->modifiers;
}

input::poiner_info_t pointer_event_args::info()const
{
	return *reinterpret_cast<input::poiner_info*>(msg->arg1());
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

bool key_event_args::is_kind_of(type_name_t name)const
{
	if (name == type_name<key_event_args>()
		|| object::is_kind_of(name)
		|| name == type_name<ikey_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool key_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<key_event_args>())
	{
		*out = static_cast<key_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<ikey_event_args>())
	{
		*out = static_cast<ikey_event_args*>(this);
		return true;
	}
	else if (name == type_name<imsg_event_args>())
	{
		*out = static_cast<imsg_event_args*>(this);
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
	return (uint)reinterpret_cast<wsize>(msg->arg1());
}

input::key_modifiers_t key_event_args::modifiers()const
{
	word modifiers = 0;
	//if (GetKeyState(VK_CONTROL) && 0x8000)
	//	modifiers |= (word_t)input::key_modifiers::control;
	//if (GetKeyState(VK_SHIFT) && 0x8000)
	//	modifiers |= (word_t)input::key_modifiers::shift;
	//if (GetKeyState(VK_MENU) && 0x8000)
	//	modifiers |= (word_t)input::key_modifiers::alt;
	//if (GetKeyState(VK_CAPITAL) && 0x0001)
	//	modifiers |= (word_t)input::key_modifiers::caps_lock;
	//if (GetKeyState(VK_NUMLOCK) && 0x0001)
	//	modifiers |= (word_t)input::key_modifiers::num_lock;
	return input::key_modifiers(modifiers);
}

word key_event_args::flags()const
{
	uint_t value = (uint_t)(wsize_t)msg->arg2();
	return word_t((value & 0XFFFF0000) >> 16);
}

bool key_event_args::is_pressed()const
{
	uint_t value = (uint_t)(wsize_t)msg->arg2();
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

bool text_change_event_args::is_kind_of(type_name_t name)const
{
	if (name == type_name<text_change_event_args>()
		|| object::is_kind_of(name)
		|| name == type_name<itext_change_event_args>()
		|| name == type_name<imsg_event_args>())
		return true;
	return false;
}

bool text_change_event_args::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<text_change_event_args>())
	{
		*out = static_cast<text_change_event_args*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<itext_change_event_args>())
	{
		*out = static_cast<itext_change_event_args*>(this);
		return true;
	}
	else if (name == type_name<imsg_event_args>())
	{
		*out = static_cast<imsg_event_args*>(this);
		return true;
	}
	return false;
}

const message_t& text_change_event_args::msg_info()const
{
	return msg;
}

wstring text_change_event_args::text()const
{
	return reinterpret_cast<strings::wstring_buffer*>(msg->arg1());
}

input::ikeyboard_t text_change_event_args::keyboard()const
{
	return reinterpret_cast<input::ikeyboard*>(msg->arg2());
}

void text_change_event_args::handled(bool value)
{
	msg->result(value ? 0 : -1);
}

bool text_change_event_args::handled()const
{
	return msg->result() != -1;
}


