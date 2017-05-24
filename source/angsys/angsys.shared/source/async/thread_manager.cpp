/*********************************************************************************************************************/
/*   File Name: thread_manager.cpp                                                                                   */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"
#include "ang/core/async.h"
#include "thread_manager.h"

//#if defined _DEBUG && defined MEMORY_DEBUGGING
//#define NEW ANG_DEBUG_NEW()
//#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
//#else
//#define NEW new
//#define NEW_ARGS(...) new(__VA_ARGS__)
//#endif

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

unsigned long STDCALL thread_manager::thread_callback_procedure(pointer args)
{
	if (args == null)
		return -1;

	auto manager = thread_manager::instance();
	auto _thread = reinterpret_cast<thread_handler_t>(args);

	manager->register_thread(_thread);
	{
		scope_locker lock = manager->main_mutex;
		_thread->_status = async_action_status::running;
		manager->main_cond->signal();
	}
	///////////////////////////////////////////////////////////////

	dword result = _thread->_callback(_thread->_args);

	///////////////////////////////////////////////////////////////
	{
		scope_locker lock = manager->main_mutex;
		_thread->_status = async_action_status::completed;
		manager->main_cond->signal();
	}
	manager->unregister_thread(_thread);
	return result;
}

thread_handler_t thread_manager::create_thread(thread_t thread, thread_callback_t callback, pointer args, thread_priority_t priority, detach_state_t ds)
{
	if (callback.is_empty())
		return null;

	auto handle = new _thread_handler();
	handle->_callback = callback;
	handle->_args = args;
	handle->_id = -1;
	handle->_handle = null;
	handle->_status = async_action_status::starting;
	handle->_is_main_thread = false;
	handle->_thread = thread.get();

#if defined WINDOWS_PLATFORM
	handle->_handle = CreateThread(NULL, 0
		, (LPTHREAD_START_ROUTINE)&thread_manager::thread_callback_procedure
		, handle, CREATE_SUSPENDED, (LPDWORD)&handle->_id);

	if (handle->_handle == null)
	{
		delete handle;
		return null;
	}
	
	SetThreadPriority(handle->_handle
		, priority == thread_priority::low ? THREAD_PRIORITY_BELOW_NORMAL
		: priority == thread_priority::high ? THREAD_PRIORITY_NORMAL
		: THREAD_PRIORITY_ABOVE_NORMAL);

	ResumeThread(handle->_handle);

#elif defined(LINUX_PLATFORM) || defined (ANDROID_PLATFORM)
	pthread_attr_t attr;
	pthread_t _thread;
	//HThread thread = new _HThread();
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,
		(ds.get() == detach_state::detached) ?
		PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);

	int_t result = pthread_create(
		&_thread,
		&attr,
		(thread_start_routine_t)&thread_manager::thread_callback_procedure,
		handle);
	pthread_attr_destroy(&attr);
	if (0 != result)
	{
		delete handle;
		return null;
	}
	handle->_handle = (void*)_thread;
	handle->_id = (dword_t)_thread;

#endif
	return handle;
}

thread_t thread::current_thread(thread_t _thread)
{
	auto id = thread_manager::get_current_thread_id();
	thread_handler_t handle = thread_manager::instance()->find_thread(id);
	if (handle == null)
	{
		auto handle = new _thread_handler();
		handle->_thread = (_thread.get() && !_thread->_handle) ? _thread.get() : new thread();
		handle->_callback = null;
		handle->_args = null;
		handle->_status = async_action_status::running;
		handle->_is_main_thread = false;
#if defined WINAPI_FAMILY
		DuplicateHandle(GetCurrentProcess()
			, GetCurrentThread()
			, GetCurrentProcess()
			, &handle->_handle
			, 0, FALSE, DUPLICATE_SAME_ACCESS);
		handle->_id = GetCurrentThreadId();
#elif defined __ANDROID__
		handle->_id = (dword_t)pthread_self();
		handle->_handle = (void*)pthread_self();
#endif
		thread_manager::instance()->register_thread(handle);
		handle->_thread->_handle = handle;
		return handle->_thread;
	}
	else
	{
		return handle->_thread;
	}
}

thread_manager::thread_manager()
	: main_mutex(null)
	, _thread_map()
{
	main_cond = new cond();
	main_mutex = new mutex();
	main_thread = thread::current_thread(new dispatcher_thread());
	thread_handler_t(main_thread->handle())->_is_main_thread = true;
	//register_thread(thread_handler_t(main_thread->handle()));
}

thread_manager::~thread_manager()
{
	main_mutex->lock();
	_thread_map.clean([](thread_handler_t handle) {
		handle->_thread = null;
	});

	if (main_thread->handle()) {
		auto handle = thread_handler_t(main_thread->handle());
		handle->_thread = null;
	}
		
	main_thread = null;
	main_cond = null;
	main_mutex->unlock();
	main_mutex = null;
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::async::thread_manager, ang::object);

bool thread_manager::register_thread(thread_handler_t h)
{
	if (h == null)
		return false;
	scope_locker lock = main_mutex;
	return _thread_map.insert({ h->_id, h }) ? true : false;
}

bool thread_manager::unregister_thread(thread_handler_t h)
{
	if (h == null)
		return false;
	scope_locker lock = main_mutex;
	thread_handler_t handle;
	if (!_thread_map.remove(h->_id, handle))
		return false;
	handle->_thread = null;
	return true;
}

thread_handler_t thread_manager::find_thread(dword id)const
{
	thread_handler_t _thread = null;
	scope_locker lock = main_mutex;

	if (_thread_map.find(id, _thread))
		return _thread;
	return null;
}

thread_t thread_manager::get_main_thread()
{
	return instance()->main_thread;
}

#if defined WINDOWS_PLATFORM
dword thread_manager::get_current_thread_id()
{
	return ::GetCurrentThreadId();
}

//dword_t thread_manager::get_thread_id(handle_t h)
//{
//#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
//	return ::GetThreadId(h);
//#else
//	auto  thead = Instance()->FindThread(h);
//	if (thead == null)
//		return 0;
//	return thead->nThreadID;
//#endif
//}
#elif defined __ANDROID__
dword thread_manager::get_current_thread_id()
{
	return (dword)pthread_self();
}

//dword_t thread_manager::GetThreadId(Handle h)
//{
//	HThread thread = reinterpret_cast<HThread>(h);
//	DWord threadId = 0;
//	memcpy(&threadId, &thread->thread, min(sizeof(threadId), sizeof(thread->thread)));
//	return threadId;
//}
#endif



//ThreadHandler _ThreadManager::MainThread()const
//{
//	ThreadHandler thread = null;
//	mutex.Lock();
//	Storage::Iteration::For(listThreads, [&](ThreadHandler handle)
//	{
//		if (handle->bIsMainThread)
//		{
//			thread = handle;
//			return Storage::Iteration::Break;
//		}
//		return Storage::Iteration::Continue;
//	});
//	mutex.Unlock();
//	return thread;
//}