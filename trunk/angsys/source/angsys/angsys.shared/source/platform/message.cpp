#include "pch.h"
#include <ang/platform/platform.h>

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;

message::message(const message& m)
	: m_msg(m.m_msg)
	, m_wparam(m.m_wparam)
	, m_lparam(m.m_lparam)
	, m_result(m.m_result)
{

}

message::message(const message* m)
	: m_msg()
	, m_wparam(0)
	, m_lparam(0)
	, m_result(0)
{
	if (m != null)
	{
		m_msg = m->m_msg;
		m_wparam = m->m_wparam;
		m_lparam = m->m_lparam;
		m_result = m->m_result;
	}
}

message::message(core_msg_t m, ulong64 a1, long64 a2)
	: m_msg(m)
	, m_wparam(a1)
	, m_lparam(a2)
	, m_result(0)
{
}

message::~message()
{
}

core_msg_t message::msg()const { return m_msg; }
ulong64 message::wparam()const { return m_wparam; }
long64 message::lparam()const { return m_lparam; }
dword message::result()const { return m_result; }
void message::result(dword res)const { m_result = res; }

//////////////////////////////////////////////////////////////////////////////////////

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::imsg_event_args, ang::interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::icreated_event_args, imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::ivisibility_change_event_args, imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::idisplay_info_event_args, imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::iactivate_event_args, imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::idraw_event_args, imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::ikey_event_args, imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::ipointer_event_args, imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::iapp_status_event_args, imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::itext_change_event_args, imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::icontroller_status_args, imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::icontroller_digital_input_args, imsg_event_args);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::icontroller_analog_input_args, imsg_event_args);

