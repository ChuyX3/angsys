#include "pch.h"
#include <ang/core/async.h>
#include "thread_manager.h"



//#if defined _DEBUG || defined _DEVELOPPER
//#define new new(__FILE__, __LINE__)
//#define new_args(...) new(__VAR_ARGS__, __FILE__, __LINE__)
//#else
//#define new_args(...) new(__VAR_ARGS__)
//#endif // MEMORY_

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

//////////////////////////////////////////////////////////////////////

thread_manager::thread_manager()
	: _thread_map()
	, _main_cond()
	, _main_mutex()
	, _main_thread(null)
{
	_main_thread = attach_this_thread(new worker_thread(), true);
	_attached_threads += _main_thread;
	//_main_thread->AddRef();
	_thread_map.insert(worker_thread::this_thread_id(), _main_thread);
}

thread_manager::~thread_manager()
{
	_main_mutex.lock();
	_thread_map.clear();
	_main_thread = null;
	_attached_threads.reset();
	_main_mutex.unlock();
}

worker_thread_t thread_manager::attach_this_thread(worker_thread_t thread, bool isMain)
{
	if (thread.is_empty()) thread = new worker_thread();
	thread->_is_main_thread = isMain;
	thread->attach();
	return thread;
}

dword thread_manager::this_thread_id()
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return (dword)pthread_self();
#elif defined WINDOWS_PLATFORM
	return GetCurrentThreadId();
#endif
}

worker_thread_t thread_manager::main_thread()const
{
	return _main_thread;
}

worker_thread_t thread_manager::this_thread()const
{
	worker_thread_t thread = null;
	_main_mutex.lock();
	if (!_thread_map.find(this_thread_id(), &thread))
	{
		thread = const_cast<thread_manager*>(this)->attach_this_thread(null, false);
		const_cast<thread_manager*>(this)->_thread_map.insert(this_thread_id(), thread);
		const_cast<thread_manager*>(this)->_attached_threads += thread;
	}
	_main_mutex.unlock();
	return thread;
}

worker_thread* thread_manager::regist_thread(worker_thread* thread)
{
	worker_thread* result = null;
	_main_mutex.lock();
	if (_thread_map.insert(thread->thread_id(), thread))
		result = thread;
	_main_mutex.unlock();
	return result;
}

worker_thread* thread_manager::unregist_thread(worker_thread* thread)
{
	worker_thread* result = null;
	_main_mutex.lock();
	if (!_thread_map.remove(thread->this_thread_id(), &result))
		result = null;
	_main_mutex.unlock();
	return result;
}

/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
/////////////