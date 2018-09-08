#include "pch.h"
#include <ang/core/async.h>
#include "thread_manager.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

/////////////////////////////////////////////////////////////////////////////////////////////////////

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::ithread, interface);

/////////////////////////////////////////////////////////////////////////////////////////////////////

thread_t thread::create_thread()
{
	worker_thread_t th = new worker_thread();
	th->start();
	return th;
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
	return thread_manager::instance()->main_thread();
}

thread_t thread::this_thread()
{
	return thread_manager::instance()->this_thread();
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
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::async::thread, object, ithread);

/////////////////////////////////////////////////////////////////////////////////////////////////////

void worker_thread::entry_point(worker_thread* thread)
{
	thread_manager::instance()->regist_thread(thread);
	thread->dispatch();
	thread_manager::instance()->unregist_thread(thread);
}

worker_thread::worker_thread()
	: _is_main_thread(false)
	, _state(async_action_status::initializing)
	, _thread(NULL)
#if defined WINDOWS_PLATFORM
	, _id(0)
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
	if (_thread)
		CloseHandle(_thread);
	_thread = null;
#else
#endif
}

thread_local bool autoRelease = false;

bool worker_thread::auto_release()
{
	if (!is_current_thread() || _state == async_action_status::attached)
		return object::auto_release();
	autoRelease = true;
	join();
	return false;
}

bool worker_thread::start()
{
	async::mutex_t& mutex = thread_manager::instance()->main_mutex();
	async::cond_t& cond = thread_manager::instance()->main_cond();
	scope_locker<async::mutex_t> lock = mutex;

	if (_thread)
		return false;

	_is_main_thread = false;
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
	_thread = ::CreateThread(NULL, 0
		, [](pointer args)->dword { worker_thread::entry_point((worker_thread*)args); return 0; }
	, this, CREATE_SUSPENDED, &_id);

	//SetThreadPriority(thread, (int)flags);
	if (_thread == null)
		throw_exception(except_code::invalid_memory);
	ResumeThread(_thread);
#endif	
	cond.waitfor(mutex, [this]()->bool
	{
		return _state < async_action_status::wait_for_start;
	});

	return true;
}

bool worker_thread::attach()
{
	if (_thread) return false;

	_state = async_action_status::attached;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	thread = pthread_self();
#elif defined WINDOWS_PLATFORM
	DuplicateHandle(GetCurrentProcess()
		, GetCurrentThread()
		, GetCurrentProcess()
		, &_thread
		, 0, FALSE, DUPLICATE_SAME_ACCESS);
	_id = GetCurrentThreadId();
#endif
	return true;
}

void worker_thread::dispatch()
{
	auto manager = thread_manager::instance();
	thread_task_t task;

	manager->main_mutex().lock();
	_state = async_action_status::wait_for_start; //wait for start routine
	manager->main_cond().signal();
	manager->main_mutex().unlock();
	sleep(1);
	do
	{
		manager->main_mutex().lock();
		while ((_tasks.is_empty() || _tasks->is_empty()) && !(_state & async_action_status::finished))
			manager->main_cond().wait(manager->main_mutex());

		if (_state & async_action_status::finished)//cancel
		{
			for (thread_task_t task : _tasks)
				task->cancel();
			_state = async_action_status::completed;
			manager->main_cond().signal();
			manager->main_mutex().unlock();
			return;
		}

		_tasks->pop(task, false);
		_state = async_action_status::running; //running worker thread
		manager->main_cond().signal();
		manager->main_mutex().unlock();

		try {
			task->run();
		}
		catch (...) {}

		manager->main_mutex().lock();
		if (_state & async_action_status::finished)//cancel
		{
			for (thread_task_t task : _tasks)
				task->cancel();
			_state = async_action_status::completed;
			manager->main_cond().signal();
			manager->main_mutex().unlock();
			return;
		}
		else
		{
			_state = async_action_status::wait_for_then;
			manager->main_cond().signal();
			manager->main_mutex().unlock();
			Sleep(1);
		}
	} while (true);

	if (autoRelease) object::auto_release();
}

thread_task_t worker_thread::post_task(thread_task_t task)
{
	scope_locker<mutex_t> lock = thread_manager::instance()->main_mutex();
	task->mutex().lock();
	task->_thread = this;
	task->_state = async_action_status::wait_for_start;
	task->cond().signal();
	task->mutex().unlock();
	_tasks += task;

	if (!is_current_thread())
		thread_manager::instance()->main_cond().signal();
	return task.get();
}

thread_task_t worker_thread::post_task(core::delegates::function<void(iasync<void>)> action)
{
	thread_task_t task = new thread_task(this);
	task->action += action;
	scope_locker<mutex_t> lock = thread_manager::instance()->main_mutex();
	task->_state = async_action_status::wait_for_start;
	_tasks += task;

	if (!is_current_thread())
		thread_manager::instance()->main_cond().signal();
	return task.get();
}

iasync<void> worker_thread::run(core::delegates::function<void(iasync<void>)> action)
{
	if ((async_action_status::finished | async_action_status::attached) & _state)
		return null;
	return post_task(action);
}

iasync<void> worker_thread::run(core::delegates::function<void(iasync<void>, var_args_t)> action, var_args_t args)
{
	if ((async_action_status::finished | async_action_status::attached) & _state)
		return null;
	return post_task([=](iasync<void> async)
	{
		action.get()->invoke(async.get(), args.get());
	});
}

bool worker_thread::is_main_thread()const
{
	return _is_main_thread;
}

bool worker_thread::is_current_thread()const
{
	return thread::this_thread_id() == thread_id();
}

dword worker_thread::thread_id()const
{
#ifdef WINDOWS_PLATFORM
	return _id;
#else
	return (dword)thread;
#endif
}

void worker_thread::join()const
{
	if (((async_action_status::finished | async_action_status::attached) & _state))
		return;

	cond_t& cond = thread_manager::instance()->main_cond();
	mutex_t& mutex = thread_manager::instance()->main_mutex();
	scope_locker<mutex_t> lock = mutex;
	_state = async_action_status::canceled;
	if (!is_current_thread()) {
		cond.signal();
		cond.waitfor(mutex, [this]() {
			return _state != async_action_status::completed;
		});
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////


