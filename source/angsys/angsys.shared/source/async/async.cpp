#include "pch.h"
#include "ang/core/async.hpp"

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

ANG_IMPLEMENT_INTERFACE(ang::core::async, iasync_task);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::async::iasync<void>, iasync_task)

ANG_IMPLEMENT_FLAGS(async, async_action_status, uint);


extern "C" ulong64 ang_get_performance_time()
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	struct timespec ts;
	ulong64 theTick = 0;
	clock_gettime(CLOCK_REALTIME, &ts);
	theTick = (ulong64)ts.tv_nsec / 1000.0;
	theTick += (ulong64)ts.tv_sec * 1000000.0;
	return theTick;
#else
	static struct PerformanceFrequency {
		ulong64 QuadPart;
		PerformanceFrequency() {
			LARGE_INTEGER _frec;
			QueryPerformanceFrequency(&_frec);
			QuadPart = (ulong64)_frec.QuadPart;
		}
	}frec;

	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return count.QuadPart / frec.QuadPart;
#endif
}



////////////////////////////////////////////////////////////////

mutex::mutex()
	: _handle(NULL)
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	_handle = memory::default_allocator<pthread_mutex_t>::alloc(1);
	pthread_mutex_init((pthread_mutex_t*)_handle, &attr);
	pthread_mutexattr_destroy(&attr);
#elif defined WINDOWS_PLATFORM
	this->_handle = CreateMutexExW(NULL, NULL, 0, SYNCHRONIZE);
#endif
}

mutex::mutex(bool _lock)
	: _handle(NULL)
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	_handle = memory::default_allocator<pthread_mutex_t>::alloc(1);
	pthread_mutex_init((pthread_mutex_t*)_handle, &attr);
	pthread_mutexattr_destroy(&attr);
	if (_lock)pthread_mutex_lock((pthread_mutex_t*)_handle);
#elif defined WINDOWS_PLATFORM
	this->_handle = CreateMutexExW(NULL, NULL, 0, SYNCHRONIZE);
	if (_lock)WaitForSingleObjectEx(this->_handle, INFINITE, FALSE);
#endif
	
}

mutex::~mutex()
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_mutex_destroy((pthread_mutex_t*)_handle);
	memory::default_allocator<pthread_mutex_t>::free((pthread_mutex_t*)_handle);
#elif defined WINDOWS_PLATFORM
	::CloseHandle(this->_handle);
#endif
}

bool mutex::lock()const
{
	if (_handle != NULL)
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		return pthread_mutex_lock((pthread_mutex_t*)_handle) == 0 ? ang_true : ang_false;
#elif defined WINDOWS_PLATFORM
		return WaitForSingleObjectEx(this->_handle, INFINITE, FALSE) == WAIT_OBJECT_0 ? true : false;
#endif
	return false;
}

bool mutex::trylock()const
{
	if (_handle != NULL)
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		return pthread_mutex_trylock((pthread_mutex_t*)_handle) == 0 ? ang_true : ang_false;
#elif defined WINDOWS_PLATFORM
		return WaitForSingleObjectEx(this->_handle, 0, FALSE) == WAIT_OBJECT_0 ? true : false;
#endif
	return false;
}

bool mutex::unlock()const
{
	if (_handle != NULL)
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		return pthread_mutex_unlock((pthread_mutex_t*)_handle) == 0 ? ang_true : ang_false;
#elif defined WINDOWS_PLATFORM
		return ReleaseMutex(this->_handle) ? true : false;
#endif
	return false;
}


////////////////////////////////////////////////////////////////////////////////

cond::cond()
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	_handle = memory::default_allocator<pthread_cond_t>::alloc(1);
	pthread_cond_init((pthread_cond_t*)_handle, NULL);
#elif defined WINDOWS_PLATFORM
	this->_handle = CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
#endif
}

cond::~cond()
{
	if (_handle != NULL)
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	{
		pthread_cond_destroy((pthread_cond_t*)_handle);
		memory::default_allocator<pthread_cond_t>::free((pthread_cond_t*)_handle);
	}
#elif defined WINDOWS_PLATFORM
		CloseHandle(this->_handle);
#endif
	_handle = NULL;
}

bool cond::wait(mutex& mutex)const
{
	if (_handle == NULL)
		return false;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return pthread_cond_wait((pthread_cond_t*)_handle, (pthread_mutex_t*)mutex._handle) == 0 ? true : false;
#elif defined WINDOWS_PLATFORM
	mutex.unlock();
	auto res = WaitForSingleObjectEx(this->_handle, INFINITE, FALSE) == WAIT_OBJECT_0 ? true : false;
	mutex.lock();
	return res;
#endif
}

bool cond::wait(mutex& mutex, dword ms)const
{
	if (_handle == NULL)
		return false;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	timespec time;
	time.tv_sec = (long)ms / 1000;
	time.tv_nsec = ((long)ms - time.tv_sec * 1000) * 1000;
	return pthread_cond_timedwait((pthread_cond_t*)_handle, (pthread_mutex_t*)mutex._handle, &time) == 0 ? ang_true : ang_false;
#elif defined WINDOWS_PLATFORM
	return scope_locker::lock(mutex, [&]() {
		return WaitForSingleObjectEx(this->_handle, ms, FALSE) == WAIT_OBJECT_0 ? true : false;
	});
#endif
}

bool cond::signal()const
{
	if (_handle != null)
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		return pthread_cond_broadcast((pthread_cond_t*)_handle) == 0 ? true : false;
#elif defined WINDOWS_PLATFORM
	{
		bool _res = SetEvent(_handle) ? true : false;
		ResetEvent(_handle);
		return _res;
	}
#endif
	return false;
}

