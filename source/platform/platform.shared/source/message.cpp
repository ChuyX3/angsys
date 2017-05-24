#include "pch.h"
#include "ang/platform/platform.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;

message::message(const message& m)
	: _msg(m._msg)
	, _arg1(m._arg1)
	, _arg2(m._arg2)
	, _result(m._result)
{

}

message::message(const message* m)
	: _msg(0)
	, _arg1(0)
	, _arg2(0)
	, _result(0)
{
	if (m != null)
	{
		_msg = m->_msg;
		_arg1 = m->_arg1;
		_arg2 = m->_arg2;
		_result = m->_result;
	}
}

message::message(core_msg_t m, pointer a1, pointer a2)
	: _msg(m)
	, _arg1(a1)
	, _arg2(a2)
	, _result(0)
{

}

message::~message()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::message, object);

core_msg_t message::msg()const { return _msg; }
pointer message::arg1()const { return _arg1; }
pointer message::arg2()const { return _arg2; }
dword message::result()const { return _result; }
void message::result(dword res)const { _result = res; }

//////////////////////////////////////////////////////////////////////////////////////

ANG_IMPLEMENT_INTERFACE(ang::platform::events, imsg_event_args);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::icreated_event_args, imsg_event_args);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::ivisibility_change_event_args, imsg_event_args);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::idisplay_info_event_args, imsg_event_args);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::iactivate_event_args, imsg_event_args);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::idraw_event_args, imsg_event_args);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::ikey_event_args, imsg_event_args);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::ipointer_event_args, imsg_event_args);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::iapp_status_event_args, imsg_event_args);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::iupdate_event_args, imsg_event_args);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::itext_change_event_args, imsg_event_args);

