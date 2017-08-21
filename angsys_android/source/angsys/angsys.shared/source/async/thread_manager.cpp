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

ANG_EXTERN ang_bool_t ang_core_thread_manager_initialize()
{
	thread_manager::instance();
	return ang_true;
}

ANG_EXTERN ang_bool_t ang_core_thread_manager_terminate()
{
	thread_manager::release_instance();
	return ang_true;
}

ANG_EXTERN ang_core_thread_ptr_t ang_core_thread_main_thread()
{
	return thread_manager::instance()->main_thread();
}

ANG_EXTERN ang_core_cond_ptr_t ang_core_thread_main_cond()
{
	return thread_manager::instance()->main_cond();
}

ANG_EXTERN ang_core_mutex_ptr_t ang_core_thread_main_mutex()
{
	return thread_manager::instance()->main_mutex();
}

ANG_EXTERN dword ang_core_thread_this_thread_id()
{
	return thread_manager::instance()->this_thread_id();
}

ANG_EXTERN ang_core_thread_ptr_t ang_core_thread_this_thread()
{
	return thread_manager::instance()->this_thread();
}

ANG_EXTERN ang_core_thread_ptr_t ang_core_thread_regist_thread(ang_core_thread_ptr_t thread)
{
	return thread_manager::instance()->regist_thread(thread);
}

ANG_EXTERN ang_core_thread_ptr_t ang_core_thread_unregist_thread(ang_core_thread_ptr_t thread)
{
	return thread_manager::instance()->unregist_thread(thread);
}

ANG_EXTERN ang_bool_t ang_lock_main_mutex()
{
	return thread_manager::instance()->main_mutex()->lock();
}

ANG_EXTERN ang_bool_t ang_unlock_main_mutex()
{
	return thread_manager::instance()->main_mutex()->unlock();
}

ANG_EXTERN ang_bool_t ang_singal_main_cond()
{
	return thread_manager::instance()->main_cond()->signal();
}

ANG_EXTERN ang_bool_t ang_wait_main_cond()
{
	auto mgr = thread_manager::instance();
	return mgr->main_cond()->wait(mgr->main_mutex());
}

ANG_EXTERN ang_bool_t ang_timed_wait_main_cond(dword ms)
{
	auto mgr = thread_manager::instance();
	return mgr->main_cond()->wait(mgr->main_mutex(), ms);
}

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

thread_manager::thread_manager()
{
	_main_cond = ang_core_cond_create();
	_main_mutex = ang_core_mutex_create();
	_main_thread = null;
}

thread_manager::~thread_manager()
{
	_thread_map.clean([&](ang_core_thread_ptr_t thread) 
	{
		ang_core_thread_create_destroy(thread);
	});

	ang_core_cond_destroy(_main_cond);
	ang_core_mutex_destroy(_main_mutex);
}

ang_core_thread_ptr_t thread_manager::main_thread()const
{
	return _main_thread;
}

ang_core_cond_ptr_t thread_manager::main_cond()const 
{
	return _main_cond; 
}

ang_core_mutex_ptr_t thread_manager::main_mutex()const
{
	return _main_mutex;
}

dword thread_manager::this_thread_id() const
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return (dword)pthread_self();
#elif defined WINDOWS_PLATFORM
	return GetCurrentThreadId();
#endif
}

ang_core_thread_ptr_t thread_manager::this_thread()const
{
	ang_core_thread_ptr_t thread = NULL;
	ang_scope_locker lock = main_mutex();
	if (_thread_map.find(this_thread_id(), thread))
		return thread;
	return null;
}

//virtual ang_thread_ptr_t regist_thread(ang_thread_ptr_t) override;
//virtual ang_thread_ptr_t unregist_thread(ang_thread_ptr_t) override;

ang_thread_ptr_t thread_manager::regist_thread(ang_thread_ptr_t _thread)
{
	ang_scope_locker lock = main_mutex();
	return _thread_map.insert(_thread->thread_id(), _thread)? _thread : null;
}

ang_thread_ptr_t thread_manager::unregist_thread(ang_thread_ptr_t _thread)
{
	ang_scope_locker lock = main_mutex();
	if (!_thread_map.remove(_thread->thread_id(), _thread))
		return null;
	return _thread;
}

ang_thread_ptr_t thread_manager::create_thread(uint flags, wsize sz, pointer tle, bool alloc)
{
	return ang_allocator<ang_thread>::construct(ang_allocator<ang_thread>::alloc(1), flags, sz, tle, alloc);
}

ang_thread_ptr_t thread_manager::create_thread(ang_thread_start_routine routine, pointer args, uint flags, wsize sz, pointer tle, bool alloc)
{
	ang_thread_ptr_t _thread = ang_allocator<ang_thread>::construct(ang_allocator<ang_thread>::alloc(1), flags, sz, tle, alloc);
	_thread->start(routine, args);
	return _thread;
}

