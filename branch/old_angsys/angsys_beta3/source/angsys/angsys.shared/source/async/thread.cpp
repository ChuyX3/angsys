#include "pch.h"
#include <ang/core/async.h>
#include "thread_manager.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

/////////////////////////////////////////////////////////////////////////////////////////////////////

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::idispatcher, intf);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::ithread, idispatcher);

/////////////////////////////////////////////////////////////////////////////////////////////////////

thread_t thread::create_thread()
{
	worker_thread_t th = new worker_thread();
	th->start();
	return th.get();
}

listen_token<void(void)> thread::add_idle_event(base_event* prop, function<void(void)> e)
{
	return field_to_parent(&thread::idle_event, prop)->add_idle_event(e);
}

bool thread::remove_idle_event(base_event* prop, listen_token<void(void)> token)
{
	return field_to_parent(&thread::idle_event, prop)->remove_idle_event(token);
}

void thread::sleep(dword ms)
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	usleep(ms * 1000);
#elif defined WINDOWS_PLATFORM
	::Sleep(ms);
#endif
}

thread_t thread::main_thread()
{
	return thread_manager::instance()->main_thread().get();
}

thread_t thread::this_thread()
{
	return thread_manager::instance()->this_thread().get();
}

dword thread::this_thread_id()
{
	return thread_manager::this_thread_id();
}

thread::thread()
{

}

thread::~thread()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::async::thread);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::async::thread, object, ithread);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::async::thread, object, ithread, idispatcher);

/////////////////////////////////////////////////////////////////////////////////////////////////////

int worker_thread::entry_point(worker_thread* thread)
{
	//worker_thread_t	thread = thread_;
	thread_manager::instance()->regist_thread(thread);
	int code = thread->dispatch();
	thread_manager::instance()->unregist_thread(thread);
	return code;
}

thread_local bool m_auto_release = false;

worker_thread::worker_thread()
	: m_exit_code(0)
	, m_is_main_thread(false)
	//, m_auto_release(false)
	, m_state(async_action_status::initializing)
	, m_thread(NULL)
#if defined WINDOWS_PLATFORM
	, m_id(0)
#endif
{

}

worker_thread::~worker_thread()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::async::worker_thread);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::async::worker_thread, thread);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::async::worker_thread, thread);

void worker_thread::dispose()
{
	m_idle_event.clear();

	exit();
#ifdef WINDOWS_PLATFORM
	if (m_thread)
		CloseHandle(m_thread);
	m_thread = null;
#else
#endif
}

bool worker_thread::auto_release()
{
	if (!has_thread_access() || m_state == async_action_status::attached)
		return object::auto_release();
	else if(has_thread_access() && m_state == async_action_status::completed)
		return object::auto_release();
	m_auto_release = true;
	exit();
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
	pthread_attr_setdetachstate(&attr, flags);
	pthread_create(&this->_thread, &attr, [](pointer args)->pointer {
		return return (pointer)worker_thread::entry_point((worker_thread*)args);
	}, this);
	pthread_attr_destroy(&attr);
	if (this->_thread == 0)
		throw_exception(except_code::invalid_memory);
#elif defined WINDOWS_PLATFORM
	m_thread = ::CreateThread(NULL, 0
		, [](pointer args)->dword { return (dword)worker_thread::entry_point((worker_thread*)args); }
	, this, CREATE_SUSPENDED, &m_id);

	//SetThreadPriority(thread, (int)flags);
	if (m_thread == null)
		throw_exception(except_code::invalid_memory);
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
	if (m_thread) return false;

	m_state = async_action_status::attached;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	thread = pthread_self();
#elif defined WINDOWS_PLATFORM
	DuplicateHandle(GetCurrentProcess()
		, GetCurrentThread()
		, GetCurrentProcess()
		, &m_thread
		, 0, FALSE, DUPLICATE_SAME_ACCESS);
	m_id = GetCurrentThreadId();
#endif
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
		while (m_tasks->is_empty() && !(m_state & async_action_status::finished)) {
			if(m_idle_event.is_empty())
				m_cond.wait(m_mutex);
			else
				m_idle_event();
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

		m_tasks->pop(task, false);
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

	int error = m_exit_code;

	if (m_auto_release) 
		object::auto_release();
	return error;
}

thread_task_t worker_thread::post_task(thread_task_t task)
{
	scope_locker<mutex_t> lock = m_mutex;
	task->mutex().lock();
	task->m_thread = this;
	task->m_status = async_action_status::wait_for_start;
	task->cond().signal();
	task->mutex().unlock();
	m_tasks->push(task, true);

	if (!has_thread_access())
		m_cond.signal();
	return task.get();
}

thread_task_t worker_thread::post_task(core::delegates::function<void(iasync<void>)> action)
{
	thread_task_t task = new thread_task(this);
	task->action += action;
	scope_locker<mutex_t> lock = m_mutex;
	task->m_status = async_action_status::wait_for_start;
	m_tasks->push(task, true);

	if (!has_thread_access())
		m_cond.signal();
	return task.get();
}

iasync<void> worker_thread::run_async(core::delegates::function<void(iasync<void>)> action)
{
	if ((async_action_status::finished | async_action_status::attached) & m_state)
		return null;
	return post_task(action).get();
}

listen_token<void(void)> worker_thread::add_idle_event(function<void(void)> func)
{
	scope_locker<mutex_t> lock = m_mutex;
	auto token = m_idle_event += move(func);
	if (!has_thread_access())
		m_cond.signal();
	return token;
}

bool worker_thread::remove_idle_event(listen_token<void(void)> token)
{
	scope_locker<mutex_t> lock = m_mutex;
	return m_idle_event -= token;
}

bool worker_thread::is_main_thread()const
{
	return m_is_main_thread;
}

bool worker_thread::has_thread_access()const
{
	return thread::this_thread_id() == thread_id();
}

dword worker_thread::thread_id()const
{
#ifdef WINDOWS_PLATFORM
	return m_id;
#else
	return (dword)thread;
#endif
}

async_action_status_t worker_thread::status()const
{
	return m_state;
}

void worker_thread::exit(int code)const
{
	if (((async_action_status::finished | async_action_status::attached) & m_state))
		return;

	scope_locker<mutex_t> lock = m_mutex;
	m_state = async_action_status::canceled;
	m_exit_code = code;
	if (!has_thread_access()) {
		m_cond.signal();
		m_cond.waitfor(m_mutex, [this]() {
			return m_state != async_action_status::completed;
		});
	}
}

void worker_thread::wait()const
{
	if (((async_action_status::finished | async_action_status::attached) & m_state))
		return;

	scope_locker<mutex_t> lock = m_mutex;
	if (!has_thread_access()) {
		m_cond.waitfor(m_mutex, [this]() {
			return m_state != async_action_status::completed;
		});
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////


