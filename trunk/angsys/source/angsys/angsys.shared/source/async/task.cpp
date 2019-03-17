#include "pch.h"
#include <ang/core/async.h>
#include "thread_manager.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////

//#if defined _DEBUG || defined _DEVELOPPER
//#define new new(__FILE__, __LINE__)
//#define new_args(...) new(__VAR_ARGS__, __FILE__, __LINE__)
//#else
//#define new_args(...) new(__VAR_ARGS__)
//#endif // MEMORY_

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

ANG_EXTERN ulong64 get_performance_time_us(void);

////////////////////////////////////////////////////////////////////////////////////////


template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<void>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<char>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<mchar>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<wchar>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<char16>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<char32>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<short>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<ushort>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<int>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<uint>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<long>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<ulong>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<long64>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<ulong64>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<float>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<double>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<object>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<variant>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<astring>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<mstring>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<wstring>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<string16>, intf);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<string32>, intf);


template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<void>, iaction<void>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<char>, iaction<char>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<mchar>, iaction<mchar>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<wchar>, iaction<wchar>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<char16>, iaction<char16>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<char32>, iaction<char32>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<short>, iaction<short>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<ushort>, iaction<ushort>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<int>, iaction<int>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<uint>, iaction<uint>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<long>, iaction<long>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<ulong>, iaction<ulong>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<long64>, iaction<long64>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<ulong64>, iaction<ulong64>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<float>, iaction<float>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<double>, iaction<double>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<object>, iaction<object>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<variant>, iaction<variant>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<astring>, iaction<astring>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<mstring>, iaction<mstring>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<wstring>, iaction<wstring>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<string16>, iaction<string16>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<string32>, iaction<string32>);

////////////////////////////////////////////////////////////////////////////////////////

template<> iasync<void> task::run_async(core::delegates::function<void(iasync<void>)> callback)
{
	auto thread = thread::create_thread();
	return thread->run_async(callback);
}

////////////////////////////////////////////////////////////////////////////////////////

task::task()
{

}

task::~task()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::async::task, object, itask<void>);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::async::task);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::async::task, object, itask<void>, iaction<void>);

////////////////////////////////////////////////////////////////////////////////////////

static ulong s_task_count = 0;

thread_task::thread_task(worker_thread_t th)
	: m_was_canceled(false)
	, m_status(async_action_status::initializing)
	, m_parent_task(null)
	, m_child_task(null)
	, m_thread(th)
{
	s_task_count++;
}

thread_task::~thread_task()
{
	s_task_count--;
}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::async::thread_task, task);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::async::thread_task);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::async::thread_task, task);

void thread_task::clear()
{
	task::clear();
	action.empty();
}

void thread_task::run()
{
	worker_thread_t thread = m_thread;
	m_mutex.lock();
	if (m_status == async_action_status::canceled)
	{
		m_status = async_action_status::completed;
		action.empty();
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
			m_thread->post_task(m_child_task);
			m_child_task->mutex().unlock();
			m_child_task = null;
			m_thread = null; //unreference thread
			m_status = async_action_status::completed;
		}
	}
	
	action.empty();
	m_cond.signal();
	m_mutex.unlock();
}

iasync<void> thread_task::then(core::delegates::function<void(iasync<void>)> func)
{
	scope_locker<async::mutex_t> lock = m_mutex;
	if (async_action_status::finished & m_status)
		return null;

	if (m_child_task.is_empty() || m_child_task->m_status == async_action_status::canceled)
		m_child_task = new thread_task(m_thread);

	thread_task_t task = m_child_task;
	m_child_task->action += func;

	if (m_status == async_action_status::wait_for_then)
	{
		m_thread->post_task(m_child_task);
		m_child_task = null;
		m_thread = null; //unreference thread
		action.empty();
		m_status = async_action_status::completed;
		m_cond.signal();
	}

	return task.get();
}

bool thread_task::wait(async_action_status_t st)const
{
	if (!m_thread.is_empty() && m_thread->has_thread_access())
		return false;
	scope_locker<async::mutex_t> lock = m_mutex;
	if (m_status > st)return false;
	m_cond.waitfor(m_mutex, [&]() { return !(st & m_status); });
	return true;
}

bool thread_task::wait(async_action_status_t st, dword ms)const
{
	if (m_thread.is_empty() || m_thread->has_thread_access())
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
	if (m_thread.is_empty() || m_status & (async_action_status::finished | async_action_status::attached))
		return false;

	if (m_status & async_action_status::initializing && !m_child_task.is_empty())
		m_child_task->cancel();

	m_was_canceled = true;
	m_status = async_action_status::canceled;
	action.empty();
	if (!m_thread->has_thread_access())
		m_cond.signal();
	m_thread = null;
	return true;
}

void thread_task::result()const
{
	scope_locker<async::mutex_t> lock = m_mutex;
	//	if (m_thread.is_empty())
	//		return;

	if (!m_thread.is_empty() && !m_thread->has_thread_access()) m_cond.waitfor(m_mutex, [&]() ->bool
	{
		return m_status <= async_action_status::running;//  !(async_action_status::finished | async_action_status::wait_for_then).is_active(state);
	});

	if (m_was_canceled)
		throw_exception(except_code::operation_canceled);

	if (m_status == async_action_status::completed)
		return;

	m_status = async_action_status::completed;
	if (!m_thread->has_thread_access())
		m_cond.signal();
	const_cast<thread_task*>(this)->m_thread = null;
	const_cast<thread_task*>(this)->action.empty();
}


