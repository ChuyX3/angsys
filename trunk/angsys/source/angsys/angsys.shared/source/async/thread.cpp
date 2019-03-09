#include "pch.h"
#include <ang/core/async.h>
#include "thread_manager.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

/////////////////////////////////////////////////////////////////////////////////////////////////////

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::idispatcher, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::ithread, idispatcher);

/////////////////////////////////////////////////////////////////////////////////////////////////////

thread_t thread::create_thread()
{
	worker_thread_t th = new worker_thread();
	th->start();
	return th.get();
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

void worker_thread::entry_point(worker_thread* thread)
{
	//worker_thread_t	thread = thread_;
	thread_manager::instance()->regist_thread(thread);
	thread->dispatch();
	thread_manager::instance()->unregist_thread(thread);
}

thread_local bool m_auto_release = false;

worker_thread::worker_thread()
	: m_is_main_thread(false)
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

void worker_thread::clear()
{
	join();
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
	join();
	return false;
}

bool worker_thread::start()
{
	async::mutex_t& mutex = thread_manager::instance()->main_mutex();
	async::cond_t& cond = thread_manager::instance()->main_cond();
	scope_locker<async::mutex_t> lock = mutex;

	if (m_thread)
		return false;

	m_is_main_thread = false;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, flags);
	pthread_create(&this->_thread, &attr, [](pointer args)->pointer {
		auto result = (pointer)core_thread::core_thread_start_routine(args);
		core_thread_manager::instance()->unregist_thread((core_thread*)args);
		return result;
	}, this);
	pthread_attr_destroy(&attr);
	if (this->_thread == 0)
		throw_exception(except_code::invalid_memory);
#elif defined WINDOWS_PLATFORM
	m_thread = ::CreateThread(NULL, 0
		, [](pointer args)->dword { worker_thread::entry_point((worker_thread*)args); return 0; }
	, this, CREATE_SUSPENDED, &m_id);

	//SetThreadPriority(thread, (int)flags);
	if (m_thread == null)
		throw_exception(except_code::invalid_memory);
	ResumeThread(m_thread);
#endif	
	cond.waitfor(mutex, [this]()->bool
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

void worker_thread::dispatch()
{
	auto manager = thread_manager::instance();
	thread_task_t task;

	manager->main_mutex().lock();
	m_state = async_action_status::wait_for_start; //wait for start routine
	manager->main_cond().signal();
	manager->main_mutex().unlock();
	sleep(1);
	do
	{
		manager->main_mutex().lock();
		while ((m_tasks.is_empty() || m_tasks->is_empty()) && !(m_state & async_action_status::finished))
			manager->main_cond().wait(manager->main_mutex());

		if (m_state & async_action_status::finished)//cancel
		{
			for (thread_task_t task : m_tasks)
				task->cancel();
			m_state = async_action_status::completed;
			manager->main_cond().signal();
			manager->main_mutex().unlock();
			break;
		}

		m_tasks->pop(task, false);
		m_state = async_action_status::running; //running worker thread
		manager->main_cond().signal();
		manager->main_mutex().unlock();

		try {
			task->run();
		}
		catch (...) {}
		task = null;
		manager->main_mutex().lock();
		if (m_state & async_action_status::finished)//cancel
		{
			for (thread_task_t task : m_tasks)
				task->cancel();
			m_state = async_action_status::completed;
			manager->main_cond().signal();
			manager->main_mutex().unlock();
			break;
		}
		else
		{
			m_state = async_action_status::wait_for_then;
			manager->main_cond().signal();
			manager->main_mutex().unlock();
			Sleep(1);
		}
	} while (true);

	if (m_auto_release) 
		object::auto_release();
}

thread_task_t worker_thread::post_task(thread_task_t task)
{
	scope_locker<mutex_t> lock = thread_manager::instance()->main_mutex();
	task->mutex().lock();
	task->m_thread = this;
	task->m_status = async_action_status::wait_for_start;
	task->cond().signal();
	task->mutex().unlock();
	m_tasks->push(task, true);

	if (!has_thread_access())
		thread_manager::instance()->main_cond().signal();
	return task.get();
}

thread_task_t worker_thread::post_task(core::delegates::function<void(iasync<void>)> action)
{
	thread_task_t task = new thread_task(this);
	task->action += action;
	scope_locker<mutex_t> lock = thread_manager::instance()->main_mutex();
	task->m_status = async_action_status::wait_for_start;
	m_tasks->push(task, true);

	if (!has_thread_access())
		thread_manager::instance()->main_cond().signal();
	return task.get();
}

iasync<void> worker_thread::run_async(core::delegates::function<void(iasync<void>)> action)
{
	if ((async_action_status::finished | async_action_status::attached) & m_state)
		return null;
	return post_task(action).get();
}

//iasync<void> worker_thread::run_async(core::delegates::function<void(iasync<void>, var_args_t)> action, var_args_t args)
//{
//	if ((async_action_status::finished | async_action_status::attached) & _state)
//		return null;
//	return post_task([=](iasync<void> async)
//	{
//		action.get()->invoke(async.get(), args.get());
//	}).get();
//}

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

void worker_thread::join()const
{
	if (((async_action_status::finished | async_action_status::attached) & m_state))
		return;

	cond_t& cond = thread_manager::instance()->main_cond();
	mutex_t& mutex = thread_manager::instance()->main_mutex();
	scope_locker<mutex_t> lock = mutex;
	m_state = async_action_status::canceled;
	if (!has_thread_access()) {
		cond.signal();
		cond.waitfor(mutex, [this]() {
			return m_state != async_action_status::completed;
		});
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////


