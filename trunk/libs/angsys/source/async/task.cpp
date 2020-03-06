#include "pch.h"
#include <ang/core/async.h>
#include "thread_manager.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

ANG_EXTERN ulong64 get_performance_time_us(void);

////////////////////////////////////////////////////////////////////////////////////////


ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<void>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<char>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<mchar>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<wchar>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<char16>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<char32>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<short>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<ushort>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<int>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<uint>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<long>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<ulong>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<long64>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<ulong64>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<float>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<double>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<object_t>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<variant>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<string>);


ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<void>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<char>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<mchar>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<wchar>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<char16>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<char32>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<short>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<ushort>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<int>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<uint>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<long>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<ulong>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<long64>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<ulong64>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<float>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<double>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<objptr>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<variant>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<string>);

////////////////////////////////////////////////////////////////////////////////////////

template<> iasync<void> task::run_async(function<void(iasync<void>)> callback)
{
	return thread::create_worker_thread(ang::forward<function<void(iasync<void>)>>(callback));
}

////////////////////////////////////////////////////////////////////////////////////////

task::task()
{

}

task::~task()
{

}

////////////////////////////////////////////////////////////////////////////////////////

thread_task::thread_task(core_thread_t th)
	: m_was_canceled(false)
	, m_status(async_action_status::initializing)
	, m_parent_task(null)
	, m_child_task(null)
	, m_thread(th)
{
}

thread_task::~thread_task()
{
}

void thread_task::dispose()
{
	task::dispose();
	action.reset();
}

void thread_task::run()
{
	core_thread_t thread = m_thread;
	m_mutex.lock();
	if (m_status == async_action_status::canceled)
	{
		m_status = async_action_status::completed;
		action.reset();
		m_cond.signal();
		m_mutex.unlock();
		return;
	}

	m_status = async_action_status::running;
	m_mutex.unlock();
	action(this);	
	m_mutex.lock();

	if (m_status == async_action_status::canceled)
	{
		m_status = async_action_status::completed;
		if (!m_child_task.is_empty())
		{
			m_child_task->cancel();
			m_child_task = null;
		}
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
			m_status = async_action_status::completed;
			m_thread->post_task(m_child_task);
			m_child_task->mutex().unlock();
			m_child_task = null;
			m_thread = null; //unreference thread
		}
	}
	
	action.reset();
	m_cond.signal();
	m_mutex.unlock();
}

iasync<void> thread_task::then(function<void(iasync<void>)> func)
{
	scope_locker<async::mutex_t> lock = m_mutex;
	if (async_action_status::finished & m_status)
		return null;

	if (m_child_task.is_empty() || m_child_task->m_status == async_action_status::canceled)
		m_child_task = new thread_task(m_thread);

	thread_task_t task = m_child_task;
	m_child_task->action = func;

	if (m_status == async_action_status::wait_for_then)
	{
		m_thread->post_task(m_child_task);
		m_child_task = null;
		m_thread = null; //unreference thread
		action.reset();
		m_status = async_action_status::completed;
		m_cond.signal();
	}

	return task.get();
}

bool thread_task::wait(async_action_status_t st)const
{
	if (!m_thread.is_empty() && m_thread->is_this_thread())
		return false;
	scope_locker<async::mutex_t> lock = m_mutex;
	if (m_status > st)return false;
	m_cond.waitfor(m_mutex, [&]() { return !(st & m_status); });
	return true;
}

bool thread_task::wait(async_action_status_t st, dword ms)const
{
	if (m_thread.is_empty() || m_thread->is_this_thread())
		return false;
	dword last_time = (dword)(get_performance_time_us() / 1000.0);
	dword current = 0;

	scope_locker<async::mutex_t> lock = m_mutex;
	if (m_status > st)return false;
	while (!(st & m_status))
	{
		m_cond.wait(m_mutex, ms);
		thread::sleep(1);
		current = (dword)(get_performance_time_us() / 1000);
		if (ms <= (current - last_time))
			break;
		else ms -= (current - last_time);
		last_time = current;
	}
	return st&m_status;
}

async_action_status_t thread_task::status()const
{
	return m_status;
}

bool thread_task::cancel()
{
	scope_locker<async::mutex_t> lock = m_mutex;
	if (m_thread.is_empty() || m_status & (async_action_status_t(async_action_status::finished) | async_action_status::attached))
		return false;

	if (m_status & async_action_status::initializing && !m_child_task.is_empty())
		m_child_task->cancel();

	m_was_canceled = true;
	m_status = async_action_status::canceled;
	action.reset();
	if (!m_thread->is_this_thread())
		m_cond.signal();
	m_thread = null;
	return true;
}

void thread_task::result()const
{
	scope_locker<async::mutex_t> lock = m_mutex;
	//	if (m_thread.is_empty())
	//		return;

	if (!m_thread.is_empty() && !m_thread->is_this_thread()) m_cond.waitfor(m_mutex, [&]() ->bool
	{
		return m_status <= async_action_status::running;//  !(async_action_status::finished | async_action_status::wait_for_then).is_active(state);
	});

	if (m_was_canceled)
		//throw_exception(error_code::operation_canceled);
		__debugbreak();

	if (m_status == async_action_status::completed)
		return;

	m_status = async_action_status::completed;
	if (!m_thread->is_this_thread())
		m_cond.signal();
	const_cast<thread_task*>(this)->m_thread->done(const_cast<thread_task*>(this));
	const_cast<thread_task*>(this)->m_thread = null;
	const_cast<thread_task*>(this)->action.reset();
}


