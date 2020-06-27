#include "pch.h"
#include <ang/core/async.h>

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

ANG_FLAGS_IMPLEMENT(ang::core::async, async_action_status);

////////////////////////////////////////////////////////////////

mutex::mutex()
	: m_handle(NULL)
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	m_handle = ang_alloc_unmanaged_memory(sizeof(pthread_mutex_t));
	pthread_mutex_init((pthread_mutex_t*)m_handle, &attr);
	pthread_mutexattr_destroy(&attr);
#elif defined WINDOWS_PLATFORM
	m_handle = ang_alloc_unmanaged_memory(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection((LPCRITICAL_SECTION)m_handle);
	//this->m_handle = CreateMutexExW(NULL, NULL, 0, SYNCHRONIZE);
#endif
}

mutex::mutex(bool _lock)
	: m_handle(NULL)
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	m_handle = ang_alloc_unmanaged_memory(sizeof(pthread_mutex_t));
	pthread_mutex_init((pthread_mutex_t*)m_handle, &attr);
	pthread_mutexattr_destroy(&attr);
	if (_lock)pthread_mutex_lock((pthread_mutex_t*)m_handle);
#elif defined WINDOWS_PLATFORM
	//this->m_handle = CreateMutexExW(NULL, NULL, 0, SYNCHRONIZE);
	m_handle = ang_alloc_unmanaged_memory(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection((LPCRITICAL_SECTION)m_handle);
	if (_lock)EnterCriticalSection(LPCRITICAL_SECTION(m_handle));
#endif

}

mutex::~mutex()
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_mutex_destroy((pthread_mutex_t*)m_handle);
#elif defined WINDOWS_PLATFORM
	//::CloseHandle(this->m_handle);
	auto handle = (LPCRITICAL_SECTION)m_handle;
	EnterCriticalSection(handle);
	m_handle = null;
	DeleteCriticalSection(handle);	
#endif
	ang_free_unmanaged_memory(m_handle);
}

bool mutex::lock()const
{
	if (m_handle != NULL) {
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		return pthread_mutex_lock((pthread_mutex_t*)m_handle) == 0 ? true : false;
#elif defined WINDOWS_PLATFORM
		//return WaitForSingleObjectEx(this->m_handle, INFINITE, FALSE) == WAIT_OBJECT_0 ? true : false;
		EnterCriticalSection(LPCRITICAL_SECTION(m_handle));
		return true;
#endif
	}
	return false;
}

bool mutex::trylock()const
{
	if (m_handle != NULL)
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		return pthread_mutex_trylock((pthread_mutex_t*)m_handle) == 0 ? true : false;
#elif defined WINDOWS_PLATFORM
		//return WaitForSingleObjectEx(this->m_handle, 0, FALSE) == WAIT_OBJECT_0 ? true : false;
		return TryEnterCriticalSection(LPCRITICAL_SECTION(m_handle)) ? true : false;
#endif
	return false;
}

bool mutex::unlock()const
{
	if (m_handle != NULL)
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		return pthread_mutex_unlock((pthread_mutex_t*)m_handle) == 0 ? ang_true : ang_false;
#elif defined WINDOWS_PLATFORM
		LeaveCriticalSection(LPCRITICAL_SECTION(m_handle));
	return true;
	//return ReleaseMutex(this->m_handle) ? true : false;
#endif
	return false;
}


////////////////////////////////////////////////////////////////////////////////

cond::cond()
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	m_handle = ang_alloc_unmanaged_memory(sizeof(pthread_cond_t));
	pthread_cond_init((pthread_cond_t*)m_handle, NULL);
#elif defined WINDOWS_PLATFORM
	//this->m_handle = CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
	m_handle = ang_alloc_unmanaged_memory(sizeof(CONDITION_VARIABLE));
	InitializeConditionVariable(PCONDITION_VARIABLE(m_handle));
#endif
}

cond::~cond()
{
	if (m_handle != NULL)
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	{
		pthread_cond_destroy((pthread_cond_t*)m_handle);
	}
#elif defined WINDOWS_PLATFORM
		//CloseHandle(this->m_handle);
#endif
		ang_free_unmanaged_memory(m_handle);
	m_handle = NULL;
}

bool cond::wait(mutex& mutex)const
{
	if (m_handle == NULL)
		return false;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return pthread_cond_wait((pthread_cond_t*)m_handle, (pthread_mutex_t*)mutex.m_handle) == 0 ? true : false;
#elif defined WINDOWS_PLATFORM
	//mutex.unlock();
	//auto res = WaitForSingleObjectEx(this->m_handle, INFINITE, FALSE) == WAIT_OBJECT_0 ? true : false;
	//mutex.lock();
	//return res;
	return SleepConditionVariableCS(PCONDITION_VARIABLE(m_handle), PCRITICAL_SECTION(mutex.m_handle), INFINITE) ? true : false;
#endif
}

bool cond::wait(mutex& mutex, dword ms)const
{
	if (m_handle == NULL)
		return false;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	timespec time;
	time.tv_sec = (long)ms / 1000;
	time.tv_nsec = ((long)ms - time.tv_sec * 1000) * 1000;
	return pthread_cond_timedwait((pthread_cond_t*)m_handle, (pthread_mutex_t*)mutex.m_handle, &time) == 0 ? true : false;
#elif defined WINDOWS_PLATFORM
	//mutex.unlock();
	//auto res = WaitForSingleObjectEx(this->m_handle, ms, FALSE) == WAIT_OBJECT_0 ? true : false;
	//mutex.lock();
	//return res;
	return SleepConditionVariableCS(PCONDITION_VARIABLE(m_handle), PCRITICAL_SECTION(mutex.m_handle), ms) ? true : false;
#endif
}

bool cond::signal()const
{
	if (m_handle != null)
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		return pthread_cond_broadcast((pthread_cond_t*)m_handle) == 0 ? true : false;
#elif defined WINDOWS_PLATFORM	
	{
		//bool _res = SetEvent(m_handle) ? true : false;
		//ResetEvent(m_handle);
		//return _res;
		WakeAllConditionVariable(PCONDITION_VARIABLE(m_handle));
		return true;
	}
#endif
	return false;
}

