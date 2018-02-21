#include "pch.h"
#include "ang/platform/angwin/angwin.hpp"
#include "async_msg_task.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;
using namespace ang::platform::windows;

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

async_msg_task::async_msg_task(imessage_reciever* parent, core::async::cond_ptr_t cond, core::async::mutex_ptr_t mut, events::message_t msg)
	: msg_(new events::message(msg))
	, parent_(parent)
	, status_(core::async::async_action_status::initializing)
	, result_(0)
	, cond_(cond)
	, mutex_(mut)
	, handled_(false)
{
}

async_msg_task::~async_msg_task()
{
	result();
	//if (_then_task != null)
	//	_then_task->release();
	//_then_task = null;
}

ANG_IMPLEMENT_CLASSNAME(ang::platform::windows::async_msg_task);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::windows::async_msg_task);

bool async_msg_task::is_inherited_of(type_name_t name)
{
	return name == type_of<async_msg_task>()
		|| object::is_inherited_of(name)
		|| core::async::iasync<dword>::is_inherited_of(name);
}

bool async_msg_task::is_kind_of(type_name_t name)const
{
	return name == type_of<async_msg_task>()
		|| object::is_kind_of(name)
		|| core::async::ioperation<dword>::is_kind_of(name);
}

bool async_msg_task::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<async_msg_task>())
	{
		*out = static_cast<async_msg_task*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (core::async::ioperation<dword>::query_object(name, out))
	{
		return true;
	}
	return false;
}

bool async_msg_task::wait(core::async::async_action_status_t st)const
{
	async_msg_task_t prevent_destroy = const_cast<async_msg_task*>(this);
	if (parent_ == null)
		return false;

	if (app::current_app()->is_worker_thread())
		return false; //Error: Wait for the same thread

	core::async::scope_locker<core::async::mutex_ptr_t> locker(mutex_);
	cond_->waitfor(mutex_, [&](void)->bool
	{
		return !st.is_active(status_);
	});
	return true;
}

bool async_msg_task::wait(core::async::async_action_status_t st, dword ms)const
{
	async_msg_task_t prevent_destroy = const_cast<async_msg_task*>(this);
	if (parent_ == null)
		return false;

	if (app::current_app()->is_worker_thread())
		return false; //Error: Wait for the same thread

	if (ms == dword(-1))
	{

		core::async::scope_locker<core::async::mutex_ptr_t> locker(mutex_);
		cond_->waitfor(mutex_, [&](void)->bool
		{
			return !st.is_active(status_);
		});
		return true;
	}
	else
	{
		core::async::scope_locker<core::async::mutex_ptr_t> locker(mutex_);
		if (!st.is_active(status_))
			cond_->wait(mutex_, ms);
		return st.is_active(status_);
	}
}

core::async::async_action_status_t async_msg_task::status()const
{
	if (parent_ == null)
		return core::async::async_action_status_t::stopped;
	return status_;
}

bool async_msg_task::cancel()
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock(mutex_);
	if (status_ <= core::async::async_action_status_t::wait_for_start)
	{
		status_ = core::async::async_action_status_t::canceled;
		return true;
	}
	return false;
}


dword async_msg_task::result()const
{
	object_t prevent_destruction = const_cast<async_msg_task*>(this);
	if (app::current_app()->is_worker_thread())
		return result_;
	wait(core::async::async_action_status::finished, -1);
	return result_;
}

void async_msg_task::complete(dword value)
{
	result_ = value;// _msg->result();
	cond_->signal();
}

//////////////////////////////////////////
