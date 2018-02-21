#include "pch.h"
#include "ang/platform/angwin/angwin.h"
#include "async_msg_task.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;
using namespace ang::platform::windows;

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

async_msg_task::async_msg_task(imessage_reciever* parent, core::async::cond_t cond, core::async::mutex_t mut, events::message_t msg)
	: _msg(new events::message(msg))
	, _parent(parent)
	, _status(core::async::async_action_status::stopped)
	, _result(0)
	, _cond(cond)
	, _mutex(mut)
	, _handled(false)
	, _then(null)
	, _then_task(null)
{
}

async_msg_task::~async_msg_task()
{
	if (_then_task != null)
		_then_task->release();
	_then_task = null;
}

ANG_IMPLEMENT_CLASSNAME(ang::platform::windows::async_msg_task);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::windows::async_msg_task);

bool async_msg_task::is_child_of(type_name_t className)
{
	if (className == type_name<async_msg_task>()
		|| object::is_child_of(className)
		|| className == type_name<ang::core::async::iasync<dword>>())
		return true;
	return false;
}

bool async_msg_task::is_kind_of(type_name_t className)const
{
	if (className == type_name<async_msg_task>()
		|| object::is_kind_of(className)
		|| className == type_name<ang::core::async::iasync<dword>>())
		return true;
	return false;
}

bool async_msg_task::query_object(type_name_t className, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == type_name<async_msg_task>())
	{
		*out = static_cast<async_msg_task*>(this);
		return true;
	}
	else if (object::query_object(className, out))
	{
		return true;
	}
	else if (className == type_name<ang::core::async::iasync<dword>>())
	{
		*out = static_cast<ang::core::async::iasync<dword>*>(this);
		return true;
	}
	return false;
}

bool async_msg_task::wait(core::async::async_action_status_t st, dword ms)const
{
	async_msg_task_t prevent_destroy = const_cast<async_msg_task*>(this);
	if (_parent == null)
		return false;

	if (app::current_app()->is_worker_thread())
		return false; //Error: Wait for the same thread

	if (ms == dword(-1))
	{
		core::async::scope_locker locker(_mutex);
		_cond->waitfor(_mutex, [&](void)->bool
		{
			return !st.is_active(_status);
		});
		return true;
	}
	else
	{
		core::async::scope_locker locker(_mutex);
		if (!st.is_active(_status))
			_cond->wait(_mutex, ms);
		return st.is_active(_status);
	}
}

core::async::async_action_status_t async_msg_task::status()const
{
	if (_parent == null)
		return core::async::async_action_status_t::stopped;
	return _status;
}

bool async_msg_task::cancel()
{
	core::async::scope_locker lock(_mutex);
	if (_status == core::async::async_action_status_t::starting)
	{
		_status = core::async::async_action_status_t::canceled;
		return true;
	}
	return false;
}

bool async_msg_task::suspend()
{
	return false;
}

bool async_msg_task::resume()
{
	return false;
}

dword async_msg_task::result()const
{
	object_t prevent_destruction = const_cast<async_msg_task*>(this);
	if (app::current_app()->is_worker_thread())
		return _result;
	wait(core::async::async_action_status::finished, -1);
	return _result;
}

void async_msg_task::then(core::delegates::function<void(ang::core::async::iasync<dword>*)> func)
{
	_mutex->lock();
	if (_handled) {
		_mutex->unlock();
		throw(ang::exception_t(ang::except_code::expired_object));
	}
	core::async::async_action_status_t status = core::async::async_action_status::finished;
	_then = ang::move(func);

	if (status.is_active(_status))
	{
		throw(ang::exception_t(ang::except_code::expired_object));

	//	_handled = true;
	//	_then(this);
		//this->add_ref();
		//_async = core::async::async_task<void>::run_async([this](core::async::iasync<void>*, var_args_t)
		//{
		//	this->_then(this->_result);
		//	this->release();
		//});
	}
	_mutex->unlock();
}


void async_msg_task::complete()
{
	_result = _msg->result();
	_cond->signal();
	if (_then.get() != null)
	{
	//	this->add_ref();
	//	core::async::async_task<void>::run_async(this->_then_task, [this](iasync<void>*, var_args_t)
	//	{
			_then(this);
	//		this->release();
	//	});
	}
}

//////////////////////////////////////////
