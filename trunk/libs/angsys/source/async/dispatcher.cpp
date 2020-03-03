#include "pch.h"
#include <coffe/core/async.h>
#include "thread_manager.h"

using namespace coffe;
using namespace coffe::core;
using namespace coffe::core::async;

extern thread_local bool m_auto_release;

dispatcher::dispatcher()
	: m_was_joined(false)
	, m_state(async_action_status::initializing)
{
}

dispatcher::~dispatcher()
{
}

//COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::core::async::dispatcher);
//COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::core::async::dispatcher);
//COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::core::async::dispatcher, core_thread, idispatcher);

void dispatcher::dispose()
{
	m_idle_event.clear();
	core_thread::dispose();
}

bool dispatcher::auto_release()
{
	if (!is_this_thread())
		return bean::auto_release();
	else if (is_this_thread() && m_state == async_action_status::completed)
		return bean::auto_release();
	m_auto_release = true;
	join();
	return false;
}

bool dispatcher::attach()
{
	//async::this_thread.m_thread_object = this;
	thread_manager::instance()->regist_thread(this);
	return true;
}

bool dispatcher::detach()
{
	thread_manager::instance()->unregist_thread(this);
	//if (async::this_thread.m_thread_object == this)
	//	async::this_thread.m_thread_object = null;
	return true;
}

bool dispatcher::start()
{
	scope_locker<async::mutex_t> lock = m_mutex;

	if (m_thread)
		return false;

	m_is_main_thread = false;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	//pthread_attr_setdetachstate(&attr, flags);
	pthread_create(&this->m_thread, &attr, [](pointer args)->pointer {
		return (pointer)worker_thread::entry_point((worker_thread*)args);
	}, this);
	pthread_attr_destroy(&attr);
	if (this->m_thread == 0)
		throw_exception(error_code::invalid_memory);
#elif defined WINDOWS_PLATFORM
	m_thread = ::CreateThread(NULL, 0
		, [](pointer args)->dword { return (dword)entry_point((core_thread*)args); }
	, this, CREATE_SUSPENDED, &m_id);

	//SetThreadPriority(thread, (int)flags);
	if (m_thread == null)
		throw_exception(error_code::invalid_memory);

	ResumeThread(m_thread);
#endif	
	m_cond.waitfor(m_mutex, [this]()->bool
	{
		return m_state < async_action_status::wait_for_start;
	});

	return true;
}


int dispatcher::dispatch()
{
	thread_task_t task;
	m_timer.reset();
	m_mutex.lock();
	m_state = async_action_status::wait_for_start; //wait for start routine
	m_cond.signal();
	m_mutex.unlock();
	sleep(1);
	do
	{
		m_mutex.lock();
		while (m_tasks.is_empty() && !(m_state & async_action_status::finished)) {
			if(m_idle_event.is_empty())
				m_cond.wait(m_mutex);
			else
				m_timer.tick([this]() { m_idle_event(m_timer); });
		}

		if (m_state & async_action_status::finished)//cancel
		{
			for (thread_task_t task : m_tasks)
				task->cancel();
			m_state = async_action_status::completed;
			m_cond.signal();
			m_mutex.unlock();
			break;
		}

		m_tasks.pop(task);
		m_state = async_action_status::running; //running worker thread
		m_cond.signal();
		m_mutex.unlock();

		try {
			task->run();
		}
		catch (...) {}
		task = null;
		m_mutex.lock();
		if (m_state & async_action_status::finished)//cancel
		{
			for (thread_task_t task : m_tasks)
				task->cancel();
			m_state = async_action_status::completed;
			m_cond.signal();
			m_mutex.unlock();
			break;
		}
		else
		{
			m_state = async_action_status::wait_for_then;
			m_cond.signal();
			m_mutex.unlock();
			sleep(1);
		}
	} while (true);

	long err = m_was_joined ? 0 : -1;
	if (m_auto_release) 
		bean::auto_release();
	return err;
}


thread_task_t dispatcher::post_task(thread_task_t task)
{
	scope_locker<mutex_t> lock = m_mutex;
	if (m_was_joined || (async_action_status::finished & m_state))
		return null;
	task->mutex().lock();
	task->m_thread = this;
	task->m_status = async_action_status::wait_for_start;
	task->cond().signal();
	task->mutex().unlock();
	m_tasks.push(task);

	if (!is_this_thread())
		m_cond.signal();
	return task.get();
}

thread_task_t dispatcher::post_task(function<void(iasync<void>)> action)
{
	scope_locker<mutex_t> lock = m_mutex;
	if (m_was_joined || (async_action_status::finished & m_state))
		return null;

	thread_task_t task = new thread_task(this);
	task->action = action;

	task->m_status = async_action_status::wait_for_start;
	m_tasks.push(task);

	if (!is_this_thread())
		m_cond.signal();
	return task.get();
}

iasync<void> dispatcher::run_async(function<void(iasync<void>)> action)
{
	if ((async_action_status_t(async_action_status::finished) | async_action_status::attached) & m_state)
		return null;
	return post_task(action).get();
}

listen_token<void(time::step_timer const&)> dispatcher::add_idle_event(function<void(time::step_timer const&)> func)
{
	scope_locker<mutex_t> lock = m_mutex;
	auto token = m_idle_event += move(func);
	if (!has_thread_access())
		m_cond.signal();
	return token;
}

bool dispatcher::remove_idle_event(listen_token<void(time::step_timer const&)> token)
{
	scope_locker<mutex_t> lock = m_mutex;
	return m_idle_event -= token;
}

bool dispatcher::has_thread_access()const
{
	return is_this_thread();
}

async_action_status_t dispatcher::status()const
{
	return m_state;
}

void dispatcher::exit()
{
	if (async_action_status::finished & m_state)
		return;

#ifdef WINDOWS_PLATFORM
	dword thread_aborted = 0;
#endif

	scope_locker<mutex_t> lock = m_mutex;
	m_state = async_action_status::canceled;

	if (!is_this_thread()) 
	{
		m_cond.signal();
		m_cond.waitfor(m_mutex, 1, [&]() {
#ifdef WINDOWS_PLATFORM
			thread_aborted = WaitForSingleObject(m_thread, 0);
			return m_state != async_action_status::completed && !(thread_aborted == WAIT_OBJECT_0);
#else
			return m_state != async_action_status::completed;
#endif // WINDOWS_PLATFORM
		});
	}

#ifdef WINDOWS_PLATFORM
	m_mutex.sync([&]()
	{
		if (thread_aborted == WAIT_OBJECT_0 && m_state != async_action_status::completed)
		{
			m_state = async_action_status::completed;
			m_cond.signal();
			detach();
		}
	});
#endif // WINDOWS_PLATFORM
}

void dispatcher::join()
{
	if (async_action_status::finished & m_state)
		return;

#ifdef WINDOWS_PLATFORM
	dword thread_aborted = 0;
#endif

	scope_locker<mutex_t> lock = m_mutex;
	m_was_joined = true;
	m_state = async_action_status::canceled;

	if (!is_this_thread()) {
		m_cond.signal();
		m_cond.waitfor(m_mutex, [&]() {
#ifdef WINDOWS_PLATFORM
			thread_aborted = WaitForSingleObject(m_thread, 0);
			return m_state != async_action_status::completed && !(thread_aborted == WAIT_OBJECT_0);
#else
			return m_state != async_action_status::completed;
#endif // WINDOWS_PLATFORM
		});
	}
#ifdef WINDOWS_PLATFORM
	m_mutex.sync([&]()
	{
		if (thread_aborted == WAIT_OBJECT_0 && m_state != async_action_status::completed)
		{
			m_state = async_action_status::completed;
			m_cond.signal();
			detach();
		}
	});
#endif // WINDOWS_PLATFORM
}
