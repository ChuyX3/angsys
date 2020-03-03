#include "pch.h"
#include <ang/core/timer.h>
#include "ang/platform/uwp/windows.h"
#include "event_args.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;
using namespace ang::platform::windows;


msg_event_args::msg_event_args(message msg)
	: m_msg(msg)
{
}

msg_event_args::~msg_event_args()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::msg_event_args)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::msg_event_args, object, imsg_event_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::msg_event_args, object, imsg_event_args);

const message& msg_event_args::msg()const
{
	return m_msg;
}

void msg_event_args::handled(bool value)
{
	m_msg.result(value ? 0 : -1);
}

bool msg_event_args::handled()const
{
	return m_msg.result() != -1;
}

////////////////////////////////////////////////////////////////////

app_status_event_args::app_status_event_args(message msg, windows::appptr app)
	: m_msg(msg)
	, m_app(ang::move(app))
{
}

app_status_event_args::~app_status_event_args()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::app_status_event_args)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::app_status_event_args, object, iapp_status_event_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::app_status_event_args, object, iapp_status_event_args, imsg_event_args);

const message& app_status_event_args::msg()const
{
	return m_msg;
}

icore_app_t app_status_event_args::core_app()const
{
	return m_app.get();
}

void app_status_event_args::handled(bool value)
{
	m_msg.result(value ? 0 : -1);
}

bool app_status_event_args::handled()const
{
	return m_msg.result() != -1;
}

////////////////////////////////////////////////////////////////////

created_event_args::created_event_args(message msg, wndptr wnd, appptr app, var_args_t args)
	: m_msg(msg)
	, m_view(ang::move(wnd))
	, m_app(ang::move(app))
	, m_args(ang::move(args))
{
}

created_event_args::~created_event_args()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::created_event_args)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::created_event_args, object, icreated_event_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::created_event_args, object, icreated_event_args, imsg_event_args);

const message& created_event_args::msg()const
{
	return m_msg;
}

icore_view_t created_event_args::core_view()const
{
	return m_view.get();
}

icore_app_t created_event_args::core_app()const
{
	return m_app.get();
}

var_args_t created_event_args::args_list()const
{
	return m_args.get();
}

void created_event_args::handled(bool value)
{
	m_msg.result(value ? 0 : -1);
}

bool created_event_args::handled()const
{
	return m_msg.result() != -1;
}

////////////////////////////////////////////////////////////////////

display_info_event_args::display_info_event_args(message msg, wndptr wnd, display_invalidate_reason_t reason, display::display_info_t info)
	: m_msg(msg)
	, m_view(ang::move(wnd))
	, m_reason(reason)
	, m_info(info)
{
}

display_info_event_args::~display_info_event_args()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::display_info_event_args)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::display_info_event_args, object, idisplay_info_event_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::display_info_event_args, object, idisplay_info_event_args, imsg_event_args);

const message& display_info_event_args::msg()const
{
	return m_msg;
}

icore_view_t display_info_event_args::core_view()const
{
	return m_view.get();
}


display_invalidate_reason_t const& display_info_event_args::invalidate_reason()const
{
	return m_reason;
}

display::display_info_t const& display_info_event_args::display_info()const
{
	return m_info;
}

void display_info_event_args::handled(bool value)
{
	m_msg.result(value ? 0 : -1);
}

bool display_info_event_args::handled()const
{
	return m_msg.result() != -1;
}


//////////////////////////////////////////////////////////////////////

visibility_change_event_args::visibility_change_event_args(message msg, windows::wndptr wnd, bool vis)
	: m_msg(msg)
	, m_view(ang::move(wnd))
	, m_visible(vis)
{
}

visibility_change_event_args::~visibility_change_event_args()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::visibility_change_event_args)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::visibility_change_event_args, object, ivisibility_change_event_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::visibility_change_event_args, object, ivisibility_change_event_args, imsg_event_args);

const message& visibility_change_event_args::msg()const
{
	return m_msg;
}

icore_view_t visibility_change_event_args::core_view()const
{
	return m_view.get();
}

bool visibility_change_event_args::is_visible()const
{
	return m_visible;
}

void visibility_change_event_args::handled(bool value)
{
	m_msg.result(value ? 0 : -1);
}

bool visibility_change_event_args::handled()const
{
	return m_msg.result() != -1;
}

////////////////////////////////////////////////////////////////////

activate_event_args::activate_event_args(message msg, activate_status_t status)
	: m_msg(msg)
	, m_status(status)
{
}

activate_event_args::~activate_event_args()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::activate_event_args)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::activate_event_args, object, iactivate_event_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::activate_event_args, object, iactivate_event_args, imsg_event_args);

const message& activate_event_args::msg()const
{
	return m_msg;
}

activate_status_t const& activate_event_args::status()const
{
	return m_status;
}

void activate_event_args::handled(bool value)
{
	m_msg.result(value ? 0 : -1);
}

bool activate_event_args::handled()const
{
	return m_msg.result() != -1;
}

////////////////////////////////////////////////////////////////////

draw_event_args::draw_event_args(message msg, wndptr wnd, graphics::icore_context_t dc, graphics::size<float> size)
	: m_msg(msg)
	, m_view(ang::move(wnd))
	, m_dc(ang::move(dc))
	, m_size(size)
{
}

draw_event_args::~draw_event_args()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::draw_event_args)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::draw_event_args, object, idraw_event_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::draw_event_args, object, idraw_event_args, imsg_event_args);


const message& draw_event_args::msg()const
{
	return m_msg;
}

icore_view_t draw_event_args::core_view()const
{
	return m_view.get();
}

graphics::icore_context_t draw_event_args::core_context()const
{
	return m_dc.get();
}

graphics::size<float> const& draw_event_args::canvas_size()const
{
	return m_size;
}

void draw_event_args::handled(bool value)
{
	m_msg.result(value ? 0 : -1);
}

bool draw_event_args::handled()const
{
	return m_msg.result() != -1;
}

////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////

//TODO:
//TOCHECK:
//TOFIX

pointer_event_args::pointer_event_args(message msg, input::poiner_info_t info)
	: m_msg(msg)
	, m_info(info)
{
}

pointer_event_args::~pointer_event_args()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::pointer_event_args)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::pointer_event_args, object, ipointer_event_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::pointer_event_args, object, ipointer_event_args, imsg_event_args);

const message& pointer_event_args::msg()const
{
	return m_msg;
}

graphics::point<float> const& pointer_event_args::position()const
{
	return m_info.point;
}

input::key_modifiers_t const& pointer_event_args::modifiers()const
{
	return m_info.modifiers;
}

input::poiner_info_t const& pointer_event_args::info()const
{
	return m_info;
}

void pointer_event_args::handled(bool value)
{
	m_msg.result(value ? 0 : -1);
}

bool pointer_event_args::handled()const
{
	return m_msg.result() != -1;
}

////////////////////////////////////////////////////////////////////

key_event_args::key_event_args(message msg, input::key_info_t info)
	: m_msg(msg)
	, m_info(info)
{
}

key_event_args::~key_event_args()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::key_event_args)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::key_event_args, object, ikey_event_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::key_event_args, object, ikey_event_args, imsg_event_args);

const message& key_event_args::msg()const
{
	return m_msg;
}

char32_t key_event_args::key()const
{
	return m_info.char_code;
}

input::key_info_t const&  key_event_args::info()const
{
	return m_info;
}

void key_event_args::handled(bool value)
{
	m_msg.result(value ? 0 : -1);
}

bool key_event_args::handled()const
{
	return m_msg.result() != -1;
}

////////////////////////////////////////////////////////////////////

text_change_event_args::text_change_event_args(message msg, wstring txt, input::ikeyboard_t keyboard)
	: m_msg(msg)
	, m_text(ang::move(txt))
	, m_keyboard(ang::move(keyboard))
{
}

text_change_event_args::~text_change_event_args()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::text_change_event_args)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::text_change_event_args, object, itext_change_event_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::text_change_event_args, object, itext_change_event_args, imsg_event_args);

const message& text_change_event_args::msg()const
{
	return m_msg;
}

text::istring_t text_change_event_args::text()const
{
	return m_text.get();
}

input::ikeyboard_t text_change_event_args::keyboard()const
{
	return m_keyboard;
}

void text_change_event_args::handled(bool value)
{
	m_msg.result(value ? 0 : -1);
}

bool text_change_event_args::handled()const
{
	return m_msg.result() != -1;
}

//////////////////////////////////////////////////////////////////////////

controller_status_args::controller_status_args(message msg, input::icontroller_t controller, input::controller_status_t status)
	: m_msg(msg)
	, m_controller(controller)
	, m_status(status)
{

}

controller_status_args::~controller_status_args()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::controller_status_args)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::controller_status_args, object, icontroller_status_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::controller_status_args, object, icontroller_status_args, imsg_event_args);

const message& controller_status_args::msg()const
{
	return m_msg;
}

void controller_status_args::handled(bool value)
{
	m_msg.result(value ? 0 : -1);
}

bool controller_status_args::handled()const
{
	return m_msg.result() != -1;
}

uint controller_status_args::controller_id()const
{
	return m_controller->get_controller_id();
}

input::icontroller_t controller_status_args::controller()const
{
	return m_controller.get();
}

input::controller_status_t controller_status_args::status()const
{
	return m_status;
}


