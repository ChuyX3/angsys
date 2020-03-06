#include "pch.h"
#include <ang/core/async.h>
#include "thread_manager.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

/////////////////////////////////////////////////////////////////////////////////////////////////////

//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::idispatcher);
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::ithread);

/////////////////////////////////////////////////////////////////////////////////////////////////////

iasync<void> thread::create_worker_thread(function<void(iasync<void>)>&& callback)
{
	worker_thread_t thread = new worker_thread();
	thread->start();
	return thread->post_task(ang::forward<function<void(iasync<void>)>>(callback));
}

idispatcher_t thread::create_dispatcher_thread()
{
	dispatcher_t thread = new dispatcher();
	thread->start();
	return thread.get();
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

/////////////////////////////////////////////////////////////////////////////////////////////////////

int core_thread::entry_point(core_thread* thread_)
{
	core_thread_t thread = thread_;
	thread->attach();
	int code = thread->dispatch();
	thread->detach();
	return code;
}

core_thread::core_thread()
	: m_is_main_thread(false)
	, m_thread(NULL)
#if defined WINDOWS_PLATFORM
	, m_id(0)
#endif
{

}

core_thread::~core_thread()
{

}

void core_thread::dispose()
{
	exit();
	detach();
}

bool core_thread::attach()
{
	if (m_thread) return false;

#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	m_thread = pthread_self();
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

bool core_thread::detach()
{
#ifdef WINDOWS_PLATFORM
	if (m_thread)
		CloseHandle(m_thread);
	m_thread = null;
#elif defined ANDROID_PLATFORM
	m_thread = NULL;
#endif
	return true;
}

bool core_thread::is_main_thread()const
{
	return m_is_main_thread;
}

bool core_thread::is_this_thread()const
{
	return thread::this_thread_id() == thread_id();
}

dword core_thread::thread_id()const
{
#ifdef WINDOWS_PLATFORM
	return m_id;
#else
	return (dword)m_thread;
#endif
}

async_action_status_t core_thread::status()const
{
	return async_action_status::attached;
}

void core_thread::exit()
{
}

void core_thread::join()
{
}
