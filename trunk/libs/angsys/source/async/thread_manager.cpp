#include "pch.h"
#include <coffe/core/async.h>
#include "thread_manager.h"

using namespace coffe;
using namespace coffe::core;
using namespace coffe::core::async;

//////////////////////////////////////////////////////////////////////

thread_manager::thread_manager()
	: m_thread_map()
	, m_main_cond()
	, m_main_mutex()
	, m_main_thread(null)
{
	m_main_thread = attach_this_thread(new core_thread(), true);
	m_attached_threads += m_main_thread;
	m_thread_map.insert(core_thread::this_thread_id(), m_main_thread);
}

thread_manager::~thread_manager()
{
	m_main_mutex.lock();
	m_thread_map.clear();
	m_main_thread = null;
	m_attached_threads.clear();
	m_main_mutex.unlock();
}

core_thread_t thread_manager::attach_this_thread(core_thread_t thread, bool isMain)
{
	if (thread.is_empty()) thread = new core_thread();
	thread->m_is_main_thread = isMain;
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

core_thread_t thread_manager::main_thread()const
{
	return m_main_thread;
}

core_thread_t thread_manager::this_thread()const
{
	return m_main_mutex.sync([this]()->core_thread_t
	{
		core_thread_t thread = null;
		if (auto it = m_thread_map.find(this_thread_id()))
		{
			thread = it->value;
		}
		else
		{
			thread = const_cast<thread_manager*>(this)->attach_this_thread(null, false);
			const_cast<thread_manager*>(this)->m_thread_map.insert(this_thread_id(), thread);
			const_cast<thread_manager*>(this)->m_attached_threads += thread;
		}
		return thread;
	});
}

core_thread* thread_manager::regist_thread(core_thread* thread)
{
	core_thread* result = null;
	m_main_mutex.lock();
	if (m_thread_map.insert(thread->thread_id(), thread))
		result = thread;
	m_main_mutex.unlock();
	return result;
}

core_thread* thread_manager::unregist_thread(core_thread* thread)
{
	core_thread* result = null;
	m_main_mutex.lock();
	if (!m_thread_map.remove(thread->this_thread_id(), result))
		result = null;
	m_main_mutex.unlock();
	return result;
}

/////////////////////////////////////////////////////////////////////////
