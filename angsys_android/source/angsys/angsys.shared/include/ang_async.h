#ifndef __ANG_INLINES_ASYNC_H__
#define __ANG_INLINES_ASYNC_H__

#include "ang_inlines.h"

ulong64 ang_get_performance_time()
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


typedef struct ang_mutex
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	mutable pthread_mutexattr_t attr;
	mutable pthread_mutex_t _mutex;
	inline ang_mutex() {
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&_mutex, &attr);
	}
	inline ~ang_mutex() {
		pthread_mutex_destroy(&_mutex);
		pthread_mutexattr_destroy(&attr);
	}
	inline bool lock()const { pthread_mutex_lock(&_mutex) == 0; }
	inline bool try_lock()const { pthread_mutex_trylock(&_mutex) == 0; }
	inline bool unlock()const { pthread_mutex_unlock(&_mutex) == 0; }
#elif defined WINDOWS_PLATFORM
	HANDLE _hmutex;
	inline ang_mutex() {
		_hmutex = CreateMutexExW(NULL, NULL, 0, SYNCHRONIZE);
	}
	inline ~ang_mutex() {
		::CloseHandle(_hmutex);
	}
	inline bool lock()const { return WaitForSingleObjectEx(_hmutex, INFINITE, FALSE) == WAIT_OBJECT_0; }
	inline bool try_lock()const { return WaitForSingleObjectEx(_hmutex, 0, FALSE) == WAIT_OBJECT_0; }
	inline bool unlock()const { return ReleaseMutex(_hmutex) ? true : false; }
#endif
}ang_mutex_t;

struct ang_scope_locker
{
	ang_mutex const& _mutex;
	ang_scope_locker(ang_mutex const& m) : _mutex(m) {
		_mutex.lock();
	}
	~ang_scope_locker() {
		_mutex.unlock();
	}
};

typedef struct ang_cond
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	mutable pthread_cond_t _cond;
	inline ang_cond() {
		pthread_cond_init(&_cond, NULL);
	}
	inline ~ang_cond() {
		pthread_cond_destroy(&_cond);
	}
	inline bool wait(ang_mutex_t const& mutex)const { return pthread_cond_wait(&_cond, &mutex._mutex) == 0; }
	inline bool wait(ang_mutex_t const& mutex, dword ms)const {
		timespec time = { ms / 1000, (ms - time.tv_sec * 1000) * 1000 };
		return pthread_cond_timedwait(&_cond, &mutex._mutex, &time) == 0;
	}
	inline bool signal()const { pthread_cond_broadcast(&_cond) == 0; }
#elif defined WINDOWS_PLATFORM
	HANDLE _hcond;
	inline ang_cond() {
		_hcond = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
	}
	inline ~ang_cond() {
		::CloseHandle(_hcond);
	}
	inline bool wait(ang_mutex_t const& mutex)const {
		mutex.unlock();
		bool res = WaitForSingleObjectEx(_hcond, INFINITE, FALSE) == WAIT_OBJECT_0;
		mutex.lock();
		return res;
	}
	inline bool wait(ang_mutex_t const& mutex, dword ms)const {
		mutex.unlock();
		bool res = WaitForSingleObjectEx(_hcond, ms, FALSE) == WAIT_OBJECT_0;
		mutex.lock();
		return res;
	}
	inline bool signal()const { return SetEvent(_hcond) ? true : false; }
#endif
}ang_cond_t;


typedef struct ang_thread ang_thread_t, *ang_thread_ptr_t;

static ang_hash_table<dword, ang_thread_ptr_t>& ang_thread_get_map() {
	static struct _thread_map_class
	{
		ang_hash_table<dword, ang_thread_ptr_t> map;
		~_thread_map_class() {
			map.clean([&](ang_thread_ptr_t thread) { ang_allocator<ang_thread>::destruct_and_free(thread); });
		}
	}_thread_map;
	return _thread_map.map;
}


#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
typedef pointer(*ang_thread_start_routine)(pointer);
#elif defined WINDOWS_PLATFORM
typedef dword(STDCALL*ang_thread_start_routine)(pointer);
#endif

struct ang_thread
{
	static ang_cond_t const& main_cond() { static ang_cond_t _cond; return _cond; }
	static ang_mutex_t const& main_mutex() { static ang_mutex_t _mutex; return _mutex; }
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	inline static dword this_thread_id() { return (dword)pthread_self(); }
	inline static void sleep(dword ms) { ::sleep(ms); }
#elif defined WINDOWS_PLATFORM
	inline static dword this_thread_id() { return GetCurrentThreadId(); }
	inline static void sleep(dword ms) { ::Sleep(ms); }
#endif
	static ang_thread_ptr_t get_this_thread(ang_thread_ptr_t __init = NULL) {
		ang_thread_ptr_t thread = NULL;
		if (ang_thread_get_map().find(this_thread_id(), thread))
			return thread;
		else if(__init)
		{
			ang_thread_get_map().insert(this_thread_id(), __init);
			return __init;
		}
	}

#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	inline static pointer start_routine(pointer args) 
#elif defined WINDOWS_PLATFORM
	inline static dword start_routine(pointer args)
#endif
	{
		ang_thread_ptr_t _this_thread = reinterpret_cast<ang_thread_ptr_t>(args);
		main_mutex().lock();
		get_this_thread(_this_thread);
		while (_this_thread->_start_routine == NULL)
			main_cond().wait(main_mutex());
		ang_thread_start_routine routine = _this_thread->_start_routine;
		pointer user_args = _this_thread->_user_args;
		main_mutex().unlock();

		auto result = routine(user_args);

		main_mutex().lock();
		_this_thread->_start_routine = NULL;
		_this_thread->_user_args =  NULL;
		main_cond().signal();
		main_mutex().unlock();
		return result;
	}

	bool _del_tle;
	wsize _tle_size;
	pointer _tle_data;
	pointer _user_args;
	ang_thread_start_routine _start_routine;

#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	
	pthread_t _thread;
#elif defined WINDOWS_PLATFORM
	DWORD _id;
	HANDLE _hthread;
#endif

	ang_thread(uint flags, wsize sz, pointer tle, bool alloc)
	{
		if (alloc && sz > 0) {
			_del_tle = true;
			_tle_size = sz;
			_tle_data = ang_alloc_unmanaged_memory(sz);
			if (tle) memcpy(_tle_data, tle, sz);
		}
		else {
			_del_tle = false;
			_tle_size = sz;
			_tle_data = tle;
		}
		//ang_thread_get_map().insert()
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, flags);
		pthread_create(&_thread, &attr, &start_routine, this);
		pthread_attr_destroy(&attr);
#elif defined WINDOWS_PLATFORM
		_hthread = CreateThread(NULL, 0
			, &start_routine
			, this, CREATE_SUSPENDED, &_id);		
		SetThreadPriority(_hthread, flags);
		ResumeThread(_hthread);
#endif
	}

	~ang_thread()
	{
		if (_del_tle && _tle_data != NULL) {
			ang_free_unmanaged_memory(_tle_data);
		}
		_tle_data = NULL;
		_tle_size = 0;
		_del_tle = false;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM

#elif defined WINDOWS_PLATFORM

#endif
	}

	bool start(ang_thread_start_routine routine, pointer args) {
		ang_scope_locker lock = main_mutex();
		if (_start_routine != NULL)
			return false;
		_start_routine = routine;
		_user_args = args;
		main_cond().signal();
		return _start_routine != NULL;
	}

	bool wait() {
		ang_scope_locker lock = main_mutex();
		while (_start_routine != NULL)
			main_cond().wait(main_mutex());
		return true;
	}

	bool wait(dword ms) {
		ang_scope_locker lock = main_mutex();
		if (_start_routine == NULL) return true;

		dword last_time = dword(ang_get_performance_time() / 1000);
		dword current = 0;
		while (_start_routine != NULL && ms != 0) {
			main_cond().wait(main_mutex(), ms);
			sleep(1);
			current = dword(ang_get_performance_time() / 1000);
			if (ms <= (current - last_time))
				break;
			else ms -= (current - last_time);
			last_time = current;
		}
		return _start_routine == NULL;
	}
};

ang_thread_ptr_t ang_create_thread(uint flags = 0, wsize sz = 0, pointer tle = NULL, bool alloc = true)
{
	return ang_allocator<ang_thread>::construct(ang_allocator<ang_thread>::alloc(1), flags, sz, tle, alloc);
}

ang_thread_ptr_t ang_create_thread(ang_thread_start_routine routine, pointer args, uint flags = 0, wsize sz = 0, pointer tle = NULL, bool alloc = true)
{
	ang_thread_ptr_t thread = ang_allocator<ang_thread>::construct(ang_allocator<ang_thread>::alloc(1), flags, sz, tle, alloc);
	thread->start(routine, args);
	return thread;
}

//
//typedef struct ang_thread
//{
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//	typedef pointer(STDCALL*ang_thread_start_routine)(pointer);
//	inline static dword this_thread_id() { return (dword)pthread_self(); }
//#elif defined WINDOWS_PLATFORM
//	typedef dword(STDCALL*ang_thread_start_routine)(pointer);
//	inline static dword this_thread_id() { return GetCurrentThreadId(); }
//#endif
//	inline static ang_thread const& this_thread(wsize sz = 0, pointer tle = NULL) {
//		thread_local ang_thread _this_thread;
//		if (sz > 0 && tle != NULL) {
//			_this_thread._del_tle = false;
//			_this_thread._tle_size = sz;
//			_this_thread._tle_pointer = tle;
//		}
//		return _this_thread;
//	}
//
//	bool _del_tle;
//	wsize _tle_size;
//	pointer _tle_pointer;
//	pointer _user_args;
//	ang_thread_start_routine _start_routine;
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//	pthread_t _thread;
//#elif defined WINDOWS_PLATFORM
//	DWORD _id;
//	HANDLE _hthread;
//#endif
//
//	ang_thread(ang_thread_start_routine routine, pointer args, uint flags = 0, wsize sz = 0, pointer tle = NULL, bool alloc = true) {
//		_user_args = args;
//		_start_routine = routine;
//		if (alloc && sz > 0) {
//			_del_tle = true;
//			_tle_size = sz;
//			_tle_pointer = ang_alloc_unmanaged_memory(sz);
//			if (tle) memcpy(_tle_pointer, tle, sz);
//		}
//		else {
//			_del_tle = false;
//			_tle_size = sz;
//			_tle_pointer = _tle_pointer;
//		}
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//		pthread_attr_t attr;
//		pthread_attr_init(&attr);
//		pthread_attr_setdetachstate(&attr, flags);
//		pthread_create(&_thread, &attr,
//			[](pointer args)->pointer {
//				auto thread = reinterpret_cast<ang_thread*>(args);
//				this_thread(0, NULL, thread);
//				return thread->_start_routine(thread->_user_args);
//			}
//			, args);
//		pthread_attr_destroy(&attr);
//#elif defined WINDOWS_PLATFORM
//		_hthread = CreateThread(NULL, 0
//			, (LPTHREAD_START_ROUTINE)routine
//			, args, CREATE_SUSPENDED, &_id);
//
//		SetThreadPriority(_hthread, flags);
//		ResumeThread(_hthread);
//#endif
//	}
//
//	ang_thread(ang_thread* other = NULL)
//	{
//		_del_tle = false;
//		_tle_size = other->_tle_size;
//		_tle_pointer = other->_tle_pointer;
//		_tl
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//		_thread = pthread_self();
//#elif defined WINDOWS_PLATFORM
//		_id = GetCurrentThreadId();
//		_hthread = GetCurrentThread();
//#endif
//
//	~ang_thread()
//	{
//		if (_del_tle && _tle_pointer != NULL) {
//			ang_free_unmanaged_memory(_tle_pointer);
//		}
//		_tle_pointer = NULL;
//		_tle_size = 0;
//		_del_tle = false;
//	}
//
//}ang_thread_t;
//

#endif//__ANG_INLINES_ASYNC_H__