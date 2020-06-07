#include "pch.h"
#include <ang/platform/platform.h>
#include "core_app.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::windows;

dword core_app::send_msg(events::message m)
{
	return SendMessageW(m_hwnd, (uint)m.msg().get(), m.wparam(), m.lparam());
}

core::async::iasync<dword> core_app::post_msg(events::message m)
{
	core::async::task_handler_ptr<dword> handler = new core::async::task_handler<dword>();
	handler->attach(run_async([this, handler, m](core::async::iasync<void> task)
	{
		handler->done(send_msg(m));
	}));
	return handler.get();
}

events::event_token_t core_app::notify(events::event_t e)
{
	if (e.is_empty())
		return events::event_token_t();
	auto it = m_event_listeners.find(e->msg_type());
	if (it.is_valid())
		return it->value->push(e);
	return (m_event_listeners[e->msg_type()] = new events::core_listener(this))->push(e);
}

bool core_app::notify(events::core_msg_t m, events::event_token_t token)
{
	auto it = m_event_listeners.find(m);
	if (!it.is_valid())
		return false;
	return it->value->pop(token);
}

bool core_app::has_thread_access()const
{
	return m_thread.is_empty() ? false : m_thread->is_this_thread();
}

core::async::async_action_status_t core_app::status()const
{
	return core::async::async_action_status::attached;
}

void core_app::exit()
{
	throw_exception(error_code::unsupported);
}

void core_app::join()
{
	throw_exception(error_code::unsupported);
}

listen_token<void(core::time::step_timer const&)> core_app::add_idle_event(function<void(core::time::step_timer const&)> action)
{
	return m_idle_event += action;
}

bool core_app::remove_idle_event(listen_token<void(core::time::step_timer const&)> token)
{
	return m_idle_event -= token;
}

core::async::iasync<void> core_app::run_async(function<void(core::async::iasync<void>)> action)
{
	async_task_t task = new async_task(this, action);
	return post_task(task.get());
}

core::async::iasync<void> core_app::post_task(async_task_t task)
{
	if (task == null)
		return null;

	task->mutex().lock();
	task->m_status = core::async::async_action_status::wait_for_start;
	task->cond().signal();
	task->mutex().unlock();

	task->add_ref();
	if (PostMessageW(m_hwnd, (uint)events::core_msg::system_reserved_event, 0, reinterpret_cast<LPARAM>(task.get())) == 0)
	{
		task->release();
		return null;
	}
	return task.get();
}


//////////////////////////////////////////

using namespace ang::core;
using namespace ang::core::async;

async_task::async_task(core_app* parent, function<void(core::async::iasync<void>)> action)
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
	else if (m_child_task.is_empty())
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
			core_app_t dispatcher = m_parent.lock();
			dispatcher->post_task(m_child_task);
			m_child_task->mutex().unlock();
			m_child_task = null;
			m_status = async_action_status::completed;
		}
	}

	m_cond.signal();
	m_mutex.unlock();
}

core::async::iasync<void> async_task::then(function<void(core::async::iasync<void>)> func)
{
	scope_locker<async::mutex_t> lock = m_mutex;
	if (async_action_status::finished & m_status)
		return null;

	core_app_t dispatcher = m_parent.lock();

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
	core_app_t dispatcher = m_parent.lock();
	if (!dispatcher.is_empty() && dispatcher->has_thread_access())
		return false;
	scope_locker<async::mutex_t> lock = m_mutex;
	if (m_status > state)return false;
	m_cond.waitfor(m_mutex, [&]() { return !(state & m_status); });
	return true;
}

bool async_task::wait(core::async::async_action_status_t state, dword ms)const
{
	core_app_t dispatcher = m_parent.lock();
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
	core_app_t dispatcher = m_parent.lock();
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
	core_app_t dispatcher = m_parent.lock();
	scope_locker<async::mutex_t> lock = m_mutex;

	if (!dispatcher.is_empty() && !dispatcher->has_thread_access()) m_cond.waitfor(m_mutex, [&]() ->bool
	{
		return m_status <= async_action_status::running;//  !(async_action_status::finished | async_action_status::wait_for_then).is_active(state);
	});

	if (m_was_canceled)
		throw_exception(error_code::operation_canceled);

	if (m_status == async_action_status::completed)
		return;

	m_status = async_action_status::completed;
	if (!dispatcher->has_thread_access())
		m_cond.signal();
}


