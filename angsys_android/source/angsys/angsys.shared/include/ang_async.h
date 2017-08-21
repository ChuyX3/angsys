//#ifndef __ANG_INLINES_ASYNC_H__
//#define __ANG_INLINES_ASYNC_H__
//
//#include "ang_inlines.h"
//
//ulong64 ang_get_performance_time()
//{
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//	struct timespec ts;
//	ulong64 theTick = 0;
//	clock_gettime(CLOCK_REALTIME, &ts);
//	theTick = (ulong64)ts.tv_nsec / 1000.0;
//	theTick += (ulong64)ts.tv_sec * 1000000.0;
//	return theTick;
//#else
//	static struct PerformanceFrequency {
//		ulong64 QuadPart;
//		PerformanceFrequency() {
//			LARGE_INTEGER _frec;
//			QueryPerformanceFrequency(&_frec);
//			QuadPart = (ulong64)_frec.QuadPart;
//		}
//	}frec;
//
//	LARGE_INTEGER count;
//	QueryPerformanceCounter(&count);
//	return count.QuadPart / frec.QuadPart;
//#endif
//}
//
//
//typedef struct ang_mutex
//{
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//	mutable pthread_mutexattr_t attr;
//	mutable pthread_mutex_t _mutex;
//	inline ang_mutex() {
//		pthread_mutexattr_init(&attr);
//		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
//		pthread_mutex_init(&_mutex, &attr);
//	}
//	inline ~ang_mutex() {
//		pthread_mutex_destroy(&_mutex);
//		pthread_mutexattr_destroy(&attr);
//	}
//	inline bool lock()const { pthread_mutex_lock(&_mutex) == 0; }
//	inline bool try_lock()const { pthread_mutex_trylock(&_mutex) == 0; }
//	inline bool unlock()const { pthread_mutex_unlock(&_mutex) == 0; }
//#elif defined WINDOWS_PLATFORM
//	HANDLE _hmutex;
//	inline ang_mutex() {
//		_hmutex = CreateMutexExW(NULL, NULL, 0, SYNCHRONIZE);
//	}
//	inline ~ang_mutex() {
//		::CloseHandle(_hmutex);
//	}
//	inline bool lock()const { return WaitForSingleObjectEx(_hmutex, INFINITE, FALSE) == WAIT_OBJECT_0; }
//	inline bool try_lock()const { return WaitForSingleObjectEx(_hmutex, 0, FALSE) == WAIT_OBJECT_0; }
//	inline bool unlock()const { return ReleaseMutex(_hmutex) ? true : false; }
//#endif
//}ang_mutex_t;
//
//struct ang_scope_locker
//{
//	ang_mutex const& _mutex;
//	ang_scope_locker(ang_mutex const& m) : _mutex(m) {
//		_mutex.lock();
//	}
//	~ang_scope_locker() {
//		_mutex.unlock();
//	}
//};
//
//typedef struct ang_cond
//{
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//	mutable pthread_cond_t _cond;
//	inline ang_cond() {
//		pthread_cond_init(&_cond, NULL);
//	}
//	inline ~ang_cond() {
//		pthread_cond_destroy(&_cond);
//	}
//	inline bool wait(ang_mutex_t const& mutex)const { return pthread_cond_wait(&_cond, &mutex._mutex) == 0; }
//	inline bool wait(ang_mutex_t const& mutex, dword ms)const {
//		timespec time = { (long)ms / 1000, ((long)ms - time.tv_sec * 1000) * 1000 };
//		return pthread_cond_timedwait(&_cond, &mutex._mutex, &time) == 0;
//	}
//	inline bool signal()const { pthread_cond_broadcast(&_cond) == 0; }
//#elif defined WINDOWS_PLATFORM
//	HANDLE _hcond;
//	inline ang_cond() {
//		_hcond = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
//	}
//	inline ~ang_cond() {
//		::CloseHandle(_hcond);
//	}
//	inline bool wait(ang_mutex_t const& mutex)const {
//		mutex.unlock();
//		bool res = WaitForSingleObjectEx(_hcond, INFINITE, FALSE) == WAIT_OBJECT_0;
//		mutex.lock();
//		return res;
//	}
//	inline bool wait(ang_mutex_t const& mutex, dword ms)const {
//		mutex.unlock();
//		bool res = WaitForSingleObjectEx(_hcond, ms, FALSE) == WAIT_OBJECT_0;
//		mutex.lock();
//		return res;
//	}
//	inline bool signal()const { return SetEvent(_hcond) ? true : false; }
//#endif
//}ang_cond_t;
//
//
//typedef struct ang_thread ang_thread_t, *ang_thread_ptr_t;
//typedef struct ang_thread_manager ang_thread_manager_t, *ang_thread_manager_ptr_t;
//
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//typedef pointer(*ang_thread_start_routine)(pointer);
//typedef pointer(*ang_thread_then_routine)(pointer, pointer);
//#elif defined WINDOWS_PLATFORM
//typedef dword(STDCALL*ang_thread_start_routine)(pointer);
//typedef dword(STDCALL*ang_thread_then_routine)(pointer, dword);
//#endif
//
//struct ang_thread_manager
//{
//	static ang_thread_manager_ptr_t get_thread_manager();
//
//	virtual ang_thread_ptr_t main_thread()const = 0;
//	virtual ang_cond_t const& main_cond()const = 0;
//	virtual ang_mutex_t const& main_mutex()const = 0;
//	virtual dword this_thread_id() const = 0;
//	virtual ang_thread_ptr_t this_thread()const = 0;
//	virtual ang_thread_ptr_t regist_thread(ang_thread_ptr_t) = 0;
//	virtual ang_thread_ptr_t unregist_thread(ang_thread_ptr_t) = 0;
//	virtual ang_thread_ptr_t create_thread(uint flags = 0, wsize sz = 0, pointer tle = NULL, bool alloc = true) = 0;
//	virtual ang_thread_ptr_t create_thread(ang_thread_start_routine routine, pointer args, uint flags = 0, wsize sz = 0, pointer tle = NULL, bool alloc = true) = 0;
//
//};
//
//enum class ang_thread_state : uint
//{
//	initializing = 0,
//	wait_for_start,
//	running,
//	wait_for_finish,
//	finishing,
//	finished,
//	canceled = finished,
//	attached,
//};
//
//struct ang_thread
//{
//
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//	static pointer start_routine(pointer args);
//#elif defined WINDOWS_PLATFORM
//	static dword start_routine(pointer args);
//#endif
//
//	bool _is_main;
//	bool _del_tle;
//	wsize _tle_size;
//	pointer _tle_data;
//	ang_thread_state _state;
//
//	pointer _user_args;
//	ang_thread_start_routine _start_routine;
//	pointer _then_args;
//	ang_thread_then_routine _then_routine;
//
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//	pthread_t _thread;
//#elif defined WINDOWS_PLATFORM
//	DWORD _id;
//	HANDLE _hthread;
//#endif
//
//	inline ang_thread(wsize sz, pointer tle, bool alloc, bool is_main)
//	{
//		_state = ang_thread_state::attached; //to attach current thread
//		_is_main = is_main;
//		if (alloc && sz > 0) {
//			_del_tle = true;
//			_tle_size = sz;
//			_tle_data = ang_alloc_unmanaged_memory(sz);
//			if (tle) memcpy(_tle_data, tle, sz);
//		}
//		else {
//			_del_tle = false;
//			_tle_size = sz;
//			_tle_data = tle;
//		}
//	}
//
//	inline ang_thread(uint flags, wsize sz, pointer tle, bool alloc)
//	{
//		_state = ang_thread_state::initializing; //initializing
//		_is_main = false;
//		if (alloc && sz > 0) {
//			_del_tle = true;
//			_tle_size = sz;
//			_tle_data = ang_alloc_unmanaged_memory(sz);
//			if (tle) memcpy(_tle_data, tle, sz);
//		}
//		else {
//			_del_tle = false;
//			_tle_size = sz;
//			_tle_data = tle;
//		}
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//		pthread_attr_t attr;
//		pthread_attr_init(&attr);
//		pthread_attr_setdetachstate(&attr, flags);
//		pthread_create(&_thread, &attr, &start_routine, this);
//		pthread_attr_destroy(&attr);
//#elif defined WINDOWS_PLATFORM
//		_hthread = CreateThread(NULL, 0
//			, &start_routine
//			, this, CREATE_SUSPENDED, &_id);		
//		SetThreadPriority(_hthread, flags);
//		ResumeThread(_hthread);
//#endif
//	}
//
//	inline ~ang_thread()
//	{
//		if (_del_tle && _tle_data != NULL)
//			ang_free_unmanaged_memory(_tle_data);		
//		_tle_data = NULL;
//		_tle_size = 0;
//		_del_tle = false;
//		join();
//#if defined WINDOWS_PLATFORM
//		CloseHandle(_hthread);
//#endif
//	}
//
//	inline bool is_main_thread()const { return _is_main; }
//
//	inline bool is_current_thread()const { return thread_id() == ang_thread_manager::get_thread_manager()->this_thread_id(); }
//	
//	inline wsize tle_size()const { return _tle_size; }
//	
//	inline pointer tle_buffer()const { return _tle_data; }
//
//	template<typename T> T* tle_data()const {
//		return (_tle_data && (_tle_size == sizeof(T))) ? (T*)_tle_data : null;
//	}
//	
//	inline void set_tle_data(pointer data, wsize sz, bool alloc = true) {
//		if (_del_tle && _tle_data != NULL)
//			ang_free_unmanaged_memory(_tle_data);
//		if (alloc && sz > 0) {
//			_del_tle = true;
//			_tle_size = sz;
//			_tle_data = ang_alloc_unmanaged_memory(sz);
//			if (data) memcpy(_tle_data, data, sz);
//		} 
//		else {
//			_del_tle = false;
//			_tle_size = sz;
//			_tle_data = data;
//		}
//	}
//	
//	inline pointer user_args()const { return _user_args; }
//	
//	inline dword thread_id()const {
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//		return (dword)_thread;
//#elif defined WINDOWS_PLATFORM
//		return _id;
//#endif
//	}
//	
//	inline void sleep(dword ms) {
//		if(is_current_thread())
//#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//		::sleep(ms);
//#elif defined WINDOWS_PLATFORM
//		 ::Sleep(ms);
//#endif
//	}
//
//	inline bool start(ang_thread_start_routine routine, pointer args) {
//		if (is_current_thread())return false;
//		ang_scope_locker lock = ang_thread_manager::get_thread_manager()->main_mutex();
//		while (_state < ang_thread_state::wait_for_start)
//			ang_thread_manager::get_thread_manager()->main_cond().wait(ang_thread_manager::get_thread_manager()->main_mutex());
//		if (_state > ang_thread_state::wait_for_start) return false;
//		_start_routine = routine;
//		_user_args = args;
//		ang_thread_manager::get_thread_manager()->main_cond().signal();
//		return _start_routine != NULL;
//	}
//
//	inline bool then(ang_thread_then_routine routine, pointer args) {
//		ang_scope_locker lock = ang_thread_manager::get_thread_manager()->main_mutex();
//		if (_state > ang_thread_state::wait_for_finish)
//			return false;
//		_then_routine = routine;
//		_then_args = args;
//		if (!is_current_thread())
//			ang_thread_manager::get_thread_manager()->main_cond().signal();
//		return _then_routine != NULL;
//	}
//
//	inline bool wait() {
//		if (is_current_thread())return false;
//		ang_scope_locker lock = ang_thread_manager::get_thread_manager()->main_mutex();
//		while (_state == ang_thread_state::running || _state == ang_thread_state::finishing)
//			ang_thread_manager::get_thread_manager()->main_cond().wait(ang_thread_manager::get_thread_manager()->main_mutex());
//		return true;
//	}
//
//	inline bool wait(dword ms) {
//		if (is_current_thread())return false;
//		ang_scope_locker lock = ang_thread_manager::get_thread_manager()->main_mutex();
//		if (_start_routine == NULL && _then_routine == NULL) return true;
//
//		dword last_time = dword(ang_get_performance_time() / 1000);
//		dword current = 0;
//		while ((_state == ang_thread_state::running || _state == ang_thread_state::finishing) && ms != 0) {
//			ang_thread_manager::get_thread_manager()->main_cond().wait(ang_thread_manager::get_thread_manager()->main_mutex(), ms);
//			sleep(1);
//			current = dword(ang_get_performance_time() / 1000);
//			if (ms <= (current - last_time))
//				break;
//			else ms -= (current - last_time);
//			last_time = current;
//		}
//		return (_state != ang_thread_state::running && _state != ang_thread_state::finishing);
//	}
//
//	inline bool cancel() {
//		ang_scope_locker lock = ang_thread_manager::get_thread_manager()->main_mutex();
//		if (_state >= ang_thread_state::finished)return false;
//		_state = ang_thread_state::canceled;
//		if (!is_current_thread())
//			ang_thread_manager::get_thread_manager()->main_cond().signal();
//		return true;
//	}
//
//	inline bool join() {
//		if (is_current_thread())return false;
//		ang_scope_locker lock = ang_thread_manager::get_thread_manager()->main_mutex();
//		while (_state == ang_thread_state::running || _state == ang_thread_state::finishing)
//			ang_thread_manager::get_thread_manager()->main_cond().wait(ang_thread_manager::get_thread_manager()->main_mutex());
//		_state = ang_thread_state::canceled;
//		ang_thread_manager::get_thread_manager()->main_cond().signal();
//		return true;
//	}
//};
//
//#endif//__ANG_INLINES_ASYNC_H__