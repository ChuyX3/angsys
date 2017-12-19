#include "pch.h"
#include "ang/platform/platform.hpp"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;

message::message(const message& m)
	: _msg(m._msg)
	, _args(m.args())
	, _result(m._result)
{

}

message::message(const message* m)
	: _msg()
	, _args()
	, _result(0)
{
	if (m != null)
	{
		_msg = m->_msg;
		_args = m->args();
		_result = m->_result;
	}
}

message::message(core_msg_t m, ulong64 a1, long64 a2)
	: _msg(m)
	, _args()
	, _result(0)
{
	_args += uinteger64_t(a1);
	_args += integer64_t(a2);
}

message::~message()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::events::message, object);

core_msg_t message::msg()const { return _msg; }
array_view<objptr> message::args()const { return _args; }
index message::push_arg(objptr obj) { _args += obj; return _args->counter(); }
objptr message::arg(index i)const { return _args[i + 2]; }
ulong64 message::wparam()const { return (ulong64)_args[0]; }
long64 message::lparam()const { return (long64)_args[1]; }
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

