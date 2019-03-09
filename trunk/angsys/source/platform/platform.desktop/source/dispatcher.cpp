#include "pch.h"
#include "ang/platform/win32/windows.h"

#include "dispatcher.h"

#include <ang/core/timer.h>

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;
using namespace ang::platform::windows;

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

dispatcher::dispatcher(function<events::event_token_t(events::event_t)> listen_callback, function<dword(events::message)> send_msg_callback, function<core::async::iasync<void>(core::async::iasync<void>)> run_async_callback)
	: m_listen_callback(ang::move(listen_callback))
	, m_dispatch_message(ang::move(send_msg_callback))
	, m_run_async_callback(ang::move(run_async_callback))
{
	m_thread = core::async::thread::this_thread();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::dispatcher);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::dispatcher, object, core::async::idispatcher, platform::imessage_listener);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::dispatcher, object, core::async::idispatcher, platform::imessage_listener);

dispatcher::~dispatcher()
{
	m_listen_callback = null;
	m_dispatch_message = null;
	m_run_async_callback = null;
}

core::async::thread_t dispatcher::worker_thread()const
{
	return m_thread;
}

void dispatcher::worker_thread(core::async::thread_t thread)
{
	m_thread = thread;
}

bool dispatcher::has_thread_access()const
{
	return m_thread->has_thread_access();
}

core::async::iasync<void> dispatcher::post_task(async_task_t task)
{
	task->mutex().lock();
	task->m_status = core::async::async_action_status::wait_for_start;
	task->cond().signal();
	task->mutex().unlock();
	return m_run_async_callback(task.get());
}

core::async::iasync<void> dispatcher::run_async(core::delegates::function<void(core::async::iasync<void>)> action)
{
	async_task_t task = new async_task(this, action);
	return post_task(task.get());
}

dword dispatcher::send_msg(events::message msg)
{
	return m_dispatch_message(msg);
}

core::async::iasync<dword> dispatcher::post_msg(events::message msg)
{
	core::async::task_handler_ptr<dword> handler = new core::async::task_handler<dword>();
	handler->attach(run_async([this, handler,msg](core::async::iasync<void> task)
	{
		handler.get()->done(m_dispatch_message(msg));
	}));
	return handler.get();
}

events::event_token_t dispatcher::listen_to(events::event_t event)
{
	return m_listen_callback(event);
}

//////////////////////////////////////////

using namespace ang::core;
using namespace ang::core::async;

async_task::async_task(
	dispatcher* parent,
	function<void(core::async::iasync<void>)> action
	)
	: m_was_canceled(false)
	, m_status(core::async::async_action_status::initializing)
	, m_parent(parent)
	, m_action(action)
	, m_parent_task(null)
	, m_child_task(null)
{
}

async_task::~async_task()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::async_task);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::async_task, object, core::async::itask<void>);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::async_task, object, itask<void>, iaction<void>);

void async_task::execute(void)
{
	m_mutex.lock();
	if (m_status == async_action_status::canceled)
	{
		m_status = async_action_status::completed;
		m_cond.signal();
		m_mutex.unlock();
		return;
	}

	m_status = async_action_status::running;
	m_mutex.unlock();
	m_action(this);
	m_mutex.lock();

	if (m_status == async_action_status::canceled)
	{
		m_status = async_action_status::completed;
	}
	else if(m_child_task.is_empty())
	{
		m_status = async_action_status::wait_for_then;
	}
	else
	{
		m_child_task->mutex().lock();
		if (m_child_task->m_status == async_action_status::canceled)
		{
			m_child_task->mutex().unlock();
			m_child_task = null;
			m_status = async_action_status::wait_for_then;
		}
		else
		{
			dispatcher_t dispatcher = m_parent.lock();
			dispatcher->post_task(m_child_task);
			m_child_task->mutex().unlock();
			m_child_task = null;
			m_status = async_action_status::completed;
		}
	}

	m_cond.signal();
	m_mutex.unlock();
}

core::async::iasync<void> async_task::then(core::delegates::function<void(core::async::iasync<void>)> func)
{
	scope_locker<async::mutex_t> lock = m_mutex;
	if (async_action_status::finished & m_status)
		return null;

	dispatcher_t dispatcher = m_parent.lock();

	if (m_child_task.is_empty() || m_child_task->m_status == async_action_status::canceled)
		m_child_task = new async_task(dispatcher, func);

	async_task_t task = m_child_task;
	if (m_status == async_action_status::wait_for_then)
	{
		dispatcher->post_task(m_child_task);
		m_child_task = null;
		dispatcher = null; //unreference thread
		m_status = async_action_status::completed;
		m_cond.signal();
	}

	return task.get();
}

bool async_task::wait(core::async::async_action_status_t state)const
{
	dispatcher_t dispatcher = m_parent.lock();
	if (!dispatcher.is_empty() && dispatcher->has_thread_access())
		return false;
	scope_locker<async::mutex_t> lock = m_mutex;
	if (m_status > state)return false;
	m_cond.waitfor(m_mutex, [&]() { return !(state & m_status); });
	return true;
}


bool async_task::wait(core::async::async_action_status_t state, dword ms)const
{
	dispatcher_t dispatcher = m_parent.lock();
	if (dispatcher.is_empty() || dispatcher->has_thread_access())
		return false;
	dword last_time = (dword)(core::time::get_performance_time_us() / 1000);
	dword current = 0;

	scope_locker<async::mutex_t> lock = m_mutex;
	if (m_status > state)return false;
	while (!(state & m_status))
	{
		m_cond.wait(m_mutex, ms);
		thread::sleep(1);
		current = (dword)(core::time::get_performance_time_us() / 1000);
		if (ms <= (current - last_time))
			break;
		else ms -= (current - last_time);
		last_time = current;
	}
	return state & m_status;
}

core::async::async_action_status_t async_task::status()const
{
	return m_status;
}

bool async_task::cancel()
{
	dispatcher_t dispatcher = m_parent.lock();
	scope_locker<async::mutex_t> lock = m_mutex;
	if (dispatcher.is_empty() || m_status & (async_action_status::finished | async_action_status::attached))
		return false;

	m_was_canceled = true;
	m_status = async_action_status::canceled;

	if (!dispatcher->has_thread_access())
		m_cond.signal();
	return true;
}

void async_task::result()const
{
	dispatcher_t dispatcher = m_parent.lock();
	scope_locker<async::mutex_t> lock = m_mutex;

	if (!dispatcher.is_empty() && !dispatcher->has_thread_access()) m_cond.waitfor(m_mutex, [&]() ->bool
	{
		return m_status <= async_action_status::running;//  !(async_action_status::finished | async_action_status::wait_for_then).is_active(state);
	});

	if (m_was_canceled)
		throw_exception(except_code::operation_canceled);

	if (m_status == async_action_status::completed)
		return;

	m_status = async_action_status::completed;
	if (!dispatcher->has_thread_access())
		m_cond.signal();
}


