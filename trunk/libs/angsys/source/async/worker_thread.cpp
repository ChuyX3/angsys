#include "pch.h"
#include <coffe/core/async.h>
#include "thread_manager.h"

using namespace coffe;
using namespace coffe::core;
using namespace coffe::core::async;


thread_local bool m_auto_release = false;

worker_thread::worker_thread()
	: m_was_joined(false)
	, m_state(async_action_status::initializing)
{
}

worker_thread::~worker_thread()
{
}

//COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::core::async::worker_thread);
//COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::core::async::worker_thread);
//COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::core::async::worker_thread, core_thread);

void worker_thread::dispose()
{
	core_thread::dispose();
}

bool worker_thread::auto_release()
{
	if (!is_this_thread())
		return bean::auto_release();
	else if(is_this_thread() && m_state == async_action_status::completed)
		return bean::auto_release();
	m_auto_release = true;
	join();
	return false;
}

bool worker_thread::start()
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

bool worker_thread::attach()
{
	thread_manager::instance()->regist_thread(this);
	return true;
}

bool worker_thread::detach()
{
	thread_manager::instance()->unregist_thread(this);
	return true;
}

int worker_thread::dispatch()
{
	thread_task_t task;
	m_mutex.lock();
	m_state = async_action_status::wait_for_start; //wait for start routine
	m_cond.signal();
	m_mutex.unlock();
	sleep(1);
	do
	{
		m_mutex.lock();
		m_cond.waitfor(m_mutex, [&]()->bool
		{
			return m_task.is_empty() && !m_was_joined && !(m_state & async_action_status::finished);
		});

		if (m_state & async_action_status::finished)//cancel
		{
			if (m_task)
				m_task->cancel();

			m_state = async_action_status::completed;
			m_cond.signal();
			m_mutex.unlock();
			break;
		}

		task = m_task;
		m_task = null;
		m_state = async_action_status::running; //running worker thread
		m_cond.signal();
		m_mutex.unlock();

		try {
			task->run();
		}
		catch (...) {}
	
		task = null;
		m_mutex.lock();	

		if (m_state & async_action_status::finished || m_was_joined)//cancel
		{
			if (m_task)
				m_task->cancel();
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

thread_task_t worker_thread::post_task(thread_task_t task)
{
	scope_locker<mutex_t> lock = m_mutex;
	if (m_was_joined || (async_action_status::finished & m_state))
		return null;
	task->mutex().lock();
	task->m_thread = this;
	task->m_status = async_action_status::wait_for_start;
	task->cond().signal();
	task->mutex().unlock();
	m_task = task;// .push(task);

	if (!is_this_thread())
		m_cond.signal();
	return task.get();
}

thread_task_t worker_thread::post_task(function<void(iasync<void>)> action)
{
	scope_locker<mutex_t> lock = m_mutex;
	if (m_was_joined || (async_action_status::finished & m_state))
		return null;

	thread_task_t task = new thread_task(this);
	task->action = action;
	
	task->m_status = async_action_status::wait_for_start;
	m_task = task;// .push(task);

	if (!is_this_thread())
		m_cond.signal();
	return task.get();
}

async_action_status_t worker_thread::status()const
{
	return m_state;
}

void worker_thread::exit()
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

void worker_thread::join()
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

