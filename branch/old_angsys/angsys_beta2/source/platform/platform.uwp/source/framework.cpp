#include "pch.h"
#include <ang/platform/uwp/angwin.hpp>
#include <ang/core/files.hpp>
#include "FrameworkView.hpp"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::windows;

framework::framework()
{

}

framework::~framework()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::platform::windows::framework);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::windows::framework);

bool framework::is_inherited_of(type_name_t type)
{
	return type == type_of<framework>()
		|| object::is_inherited_of(type)
		|| icore_app::is_inherited_of(type)
		|| imessage_reciever::is_inherited_of(type);
}

bool framework::is_kind_of(type_name_t type)const
{
	return type == type_of<framework>()
		|| object::is_kind_of(type)
		|| icore_app::is_kind_of(type)
		|| imessage_reciever::is_kind_of(type);
}

bool framework::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)return false;
	if (type == type_of<framework>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (icore_app::query_object(type, out))
	{
		return true;
	}
	else if (imessage_reciever::query_object(type, out))
	{
		return true;
	}
	return false;
}


void framework::send_msg(events::message_t msg)
{

}

core::async::ioperation_t<dword> framework::post_msg(events::message_t msg)
{

}

bool framework::listen_to(events::event_t e)
{

}

pointer framework::get_core_app_handle()const
{

}

icore_view_t framework::get_main_core_view()
{

}

input::ikeyboard_t framework::get_keyboard()
{

}