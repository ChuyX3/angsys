#include <angtypes.h>
#include "ang_core_async.h"
#include "ang_core_hash_table.h"

#define new(_TYPE) (_TYPE*)ang_alloc_unmanaged_memory(sizeof(_TYPE))
#define delete(_PTR) ang_free_unmanaged_memory(_PTR)


#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
#elif defined WINDOWS_PLATFORM
#endif

static ulong64 ang_get_performance_time()
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


ang_core_cond_ptr_t ang_core_cond_create()
{
	ang_core_cond_impl_ptr_t this = new(ang_core_cond_impl_t);
	this->signal = &ang_core_cond_signal;
	this->wait = &ang_core_cond_wait;
	this->timed_wait = &ang_core_cond_timed_wait;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_cond_init(&this->_cond, NULL);
#elif defined WINDOWS_PLATFORM
	this->_hevent = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
#endif
	return (ang_core_cond_ptr_t)this;
}

void ang_core_cond_destroy(ang_core_cond_ptr_t ptr)
{
	ang_core_cond_impl_ptr_t this = (ang_core_cond_impl_ptr_t)ptr;
	if (this == NULL)return;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_cond_destroy(&this->_cond);
#elif defined WINDOWS_PLATFORM
	CloseHandle(this->_hevent);
#endif
	delete(ptr);
}

ang_bool_t ang_core_cond_signal(ang_core_cond_ptr_t ptr)
{
	ang_core_cond_impl_ptr_t this = (ang_core_cond_impl_ptr_t)ptr;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return pthread_cond_broadcast(&this->_cond) == 0 ? ang_true : ang_false;
#elif defined WINDOWS_PLATFORM
	return SetEvent(this->_hevent) ? ang_true : ang_false;
#endif
}

ang_bool_t ang_core_cond_wait(ang_core_cond_ptr_t  ptr, ang_core_mutex_ptr_t m)
{
	ang_core_cond_impl_ptr_t this = (ang_core_cond_impl_ptr_t)ptr;
	ang_core_mutex_impl_ptr_t mutex = (ang_core_mutex_impl_ptr_t)m;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return pthread_cond_wait(&this->_cond, &mutex->_mutex) == 0 ? ang_true : ang_false;
#elif defined WINDOWS_PLATFORM
	ang_bool_t res;
	WaitForSingleObjectEx(mutex->_hmutex, INFINITE, FALSE);
	res = WaitForSingleObjectEx(this->_hevent, INFINITE, FALSE) == WAIT_OBJECT_0 ? ang_true : ang_false;
	ReleaseMutex(mutex->_hmutex);
	return res;
#endif
}

ang_bool_t ang_core_cond_timed_wait(ang_core_cond_ptr_t ptr, ang_core_mutex_ptr_t m, dword ms)
{
	ang_core_cond_impl_ptr_t this = (ang_core_cond_impl_ptr_t)ptr;
	ang_core_mutex_impl_ptr_t mutex = (ang_core_mutex_impl_ptr_t)m;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	struct timespec time;
	time.tv_sec = (long)ms / 1000;
	time.tv_nsec = ((long)ms - time.tv_sec * 1000) * 1000;
	return pthread_cond_timedwait(&this->_cond, &mutex->_mutex, &time) == 0 ? ang_true : ang_false;
#elif defined WINDOWS_PLATFORM
	ang_bool_t res;
	WaitForSingleObjectEx(mutex->_hmutex, INFINITE, FALSE);
	res = WaitForSingleObjectEx(this->_hevent, ms, FALSE) == WAIT_OBJECT_0 ? ang_true : ang_false;
	ReleaseMutex(mutex->_hmutex);
	return res;
#endif
}

//////////////////////////////////////////////////////////////////////////////////////

ang_core_mutex_ptr_t ang_core_mutex_create()
{
	ang_core_mutex_impl_ptr_t this = new(ang_core_mutex_impl_t);
	this->lock = &ang_core_mutex_lock;
	this->trylock = &ang_core_mutex_trylock;
	this->unlock = &ang_core_mutex_unlock;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&this->_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
#elif defined WINDOWS_PLATFORM
	this->_hmutex = CreateMutexExW(NULL, NULL, 0, SYNCHRONIZE);
#endif
	return (ang_core_mutex_ptr_t)this;
}

void ang_core_mutex_destroy(ang_core_mutex_ptr_t ptr)
{
	ang_core_mutex_impl_ptr_t this = (ang_core_mutex_impl_ptr_t)ptr;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_mutex_destroy(&this->_mutex);
#elif defined WINDOWS_PLATFORM
	::CloseHandle(this->_hmutex);
#endif
	delete(ptr);
}

ang_bool_t ang_core_mutex_lock(ang_core_mutex_ptr_t ptr)
{
	ang_core_mutex_impl_ptr_t this = (ang_core_mutex_impl_ptr_t)ptr;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return pthread_mutex_lock(&this->_mutex) == 0 ? ang_true : ang_false;
#elif defined WINDOWS_PLATFORM
	return WaitForSingleObjectEx(this->_hmutex, INFINITE, FALSE) == WAIT_OBJECT_0 ? ang_true : ang_false;
#endif
}

ang_bool_t ang_core_mutex_trylock(ang_core_mutex_ptr_t ptr)
{
	ang_core_mutex_impl_ptr_t this = (ang_core_mutex_impl_ptr_t)ptr;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return pthread_mutex_trylock(&this->_mutex) == 0 ? ang_true : ang_false;
#elif defined WINDOWS_PLATFORM
	return WaitForSingleObjectEx(this->_hmutex, 0, FALSE) == WAIT_OBJECT_0 ? ang_true : ang_false;
#endif
}

ang_bool_t ang_core_mutex_unlock(ang_core_mutex_ptr_t ptr)
{
	ang_core_mutex_impl_ptr_t this = (ang_core_mutex_impl_ptr_t)ptr;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return pthread_mutex_unlock(&this->_mutex) == 0 ? ang_true : ang_false;
#elif defined WINDOWS_PLATFORM
	return ReleaseMutex(this->_hmutex)? ang_true : ang_false;
#endif
}

//////////////////////////////////////////////////////////////////////////////////////


#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
static pointer ang_core_thread_start_routine(pointer args)
#elif defined WINDOWS_PLATFORM
static dword ang_core_thread_start_routine(pointer args)
#endif
{
	ang_core_thread_ptr_t thread = (ang_core_thread_ptr_t)(args);
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t) (args);
	ang_core_thread_start_routine_t routine;
	ang_core_thread_then_routine_t then_routine;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pointer result;
#elif defined WINDOWS_PLATFORM
	dword result;
#endif

	ang_core_thread_regist_thread(thread);
	ang_lock_main_mutex();
	this->_state = ang_thread_state_wait_for_start; //wait for start routine
	ang_singal_main_cond();
	ang_unlock_main_mutex();

	ang_lock_main_mutex();
	while (this->_start_routine == NULL && this->_state != ang_thread_state_canceled)
		ang_wait_main_cond();

	if (this->_state == ang_thread_state_canceled)//cancel
	{
		ang_singal_main_cond();
		ang_unlock_main_mutex();
		return NULL;
	}

	routine = this->_start_routine;
	pointer user_args = this->_user_args;
	this->_state = ang_thread_state_running; //running worker thread
	ang_singal_main_cond();
	ang_unlock_main_mutex();

	result = routine(user_args);

	ang_lock_main_mutex();
	this->_start_routine = NULL;
	this->_user_args = NULL;
	this->_state = ang_thread_state_wait_for_finish;//wait for then routing
	ang_singal_main_cond();
	ang_unlock_main_mutex();
	this->sleep(thread, 1);

	ang_lock_main_mutex();
	while (this->_then_routine == NULL && this->_state != ang_thread_state_canceled)
		ang_wait_main_cond();

	if (this->_state == ang_thread_state_canceled)//cancel
	{
		ang_singal_main_cond();
		ang_unlock_main_mutex();
		return result;
	}

	then_routine = this->_then_routine;
	user_args = this->_then_args;
	this->_state = ang_thread_state_finishing; //running then routing
	ang_singal_main_cond();
	ang_unlock_main_mutex();

	result = then_routine(user_args, result);

	ang_lock_main_mutex();
	this->_then_routine = NULL;
	this->_then_args = NULL;
	this->_state = ang_thread_state_finished; //finished worker thread
	ang_singal_main_cond();
	ang_unlock_main_mutex();

	ang_core_thread_unregist_thread(thread);
	ang_core_thread_destroy(thread);
	return result;
}

ang_core_thread_ptr_t ang_core_thread_attach_this_thread(ang_bool_t is_main, wsize sz, pointer tle, ang_bool_t alloc)
{
	ang_core_thread_impl_ptr_t this = new (ang_core_thread_impl_t);
	memset(this, 0, sizeof(ang_core_thread_impl_t));

	this->is_main_thread = &ang_core_thread_impl_is_main_thread;
	this->is_current_thread = &ang_core_thread_impl_is_current_thread;
	this->tle_size = &ang_core_thread_impl_tle_size;
	this->tle_buffer = &ang_core_thread_impl_tle_buffer;
	this->set_tle_data = &ang_core_thread_impl_set_tle_data;
	this->set_tle_notify = &ang_core_thread_impl_set_tle_notify;
	this->user_args = &ang_core_thread_impl_user_args;
	this->thread_id = &ang_core_thread_impl_thread_id;
	this->thread_state = &ang_core_thread_impl_thread_state;
	this->sleep = &ang_core_thread_impl_sleep;
	this->start = &ang_core_thread_impl_start;
	this->then = &ang_core_thread_impl_then;
	this->wait = &ang_core_thread_impl_wait;
	this->timed_wait = &ang_core_thread_impl_timed_wait;
	this->cancel = &ang_core_thread_impl_cancel;
	this->join = &ang_core_thread_impl_join;

	this->_state = ang_thread_state_attached; //initializing
	this->_is_main = is_main;
	if (alloc && sz > 0) {
		this->_del_tle = ang_true;
		this->_tle_size = sz;
		this->_tle_data = ang_alloc_unmanaged_memory(sz);
		if (tle) memcpy(this->_tle_data, tle, sz);
	}
	else {
		this->_del_tle = ang_false;
		this->_tle_size = sz;
		this->_tle_data = tle;
	}
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	this->_thread = pthread_self();
#elif defined WINDOWS_PLATFORM
	DuplicateHandle(GetCurrentProcess()
		, GetCurrentThread()
		, GetCurrentProcess()
		, &this->_hthread
		, 0, FALSE, DUPLICATE_SAME_ACCESS);
	handle->_id = GetCurrentThreadId();
#endif
	return (ang_core_thread_ptr_t)this;
}


ang_core_thread_ptr_t ang_core_thread_create_suspended(uint flags, wsize sz, pointer tle, ang_bool_t alloc)
{
	ang_core_thread_impl_ptr_t this = new (ang_core_thread_impl_t);
	memset(this, 0, sizeof(ang_core_thread_impl_t));

	this->is_main_thread = &ang_core_thread_impl_is_main_thread;
	this->is_current_thread = &ang_core_thread_impl_is_current_thread;
	this->tle_size = &ang_core_thread_impl_tle_size;
	this->tle_buffer = &ang_core_thread_impl_tle_buffer;
	this->set_tle_data = &ang_core_thread_impl_set_tle_data;
	this->user_args = &ang_core_thread_impl_user_args;
	this->thread_id = &ang_core_thread_impl_thread_id;
	this->sleep = &ang_core_thread_impl_sleep;
	this->start = &ang_core_thread_impl_start;
	this->then = &ang_core_thread_impl_then;
	this->wait = &ang_core_thread_impl_wait;
	this->timed_wait = &ang_core_thread_impl_timed_wait;
	this->cancel = &ang_core_thread_impl_cancel;
	this->join = &ang_core_thread_impl_join;

	this->_state = ang_thread_state_initializing; //initializing
	this->_is_main = ang_false;
	if (alloc && sz > 0) {
		this->_del_tle = ang_true;
		this->_tle_size = sz;
		this->_tle_data = ang_alloc_unmanaged_memory(sz);
		if (tle) memcpy(this->_tle_data, tle, sz);
	}
	else {
		this->_del_tle = ang_false;
		this->_tle_size = sz;
		this->_tle_data = tle;
	}
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, flags);
	pthread_create(&this->_thread, &attr, &ang_core_thread_start_routine, this);
	pthread_attr_destroy(&attr);
#elif defined WINDOWS_PLATFORM
	this->_hthread = CreateThread(NULL, 0
		, &ang_core_thread_start_routine
		, this, CREATE_SUSPENDED, &this->_id);
	SetThreadPriority(this->_hthread, flags);
	ResumeThread(_hthread);
#endif
	return (ang_core_thread_ptr_t)this;
}

ang_core_thread_ptr_t ang_core_thread_create(ang_core_thread_start_routine_t routine, pointer args, uint flags, wsize sz, pointer tle, ang_bool_t alloc)
{
	ang_core_thread_ptr_t this = ang_core_thread_create_suspended(flags, sz, tle, alloc);
	this->start(this, routine, args);
	return this;
}

void ang_core_thread_destroy(ang_core_thread_ptr_t _this)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
	if (this == NULL)return;
	this->join(_this);
	if (this->tle_notify_callback) this->tle_notify_callback(_this, this->_tle_size, this->_tle_data);
	if (this->_del_tle && this->_tle_data != NULL)
		ang_free_unmanaged_memory(this->_tle_data);
	this->_tle_data = NULL;
	this->_tle_size = 0;
	this->_del_tle = ang_false;

#if defined WINDOWS_PLATFORM
	CloseHandle(this->_hthread);
#endif
	delete(this);
}

ang_bool_t ang_core_thread_impl_is_main_thread(ang_core_thread_ptr_t _this)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
	return this->_is_main;
}

ang_bool_t ang_core_thread_impl_is_current_thread(ang_core_thread_ptr_t _this)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return this->_thread == pthread_self();
#elif defined WINDOWS_PLATFORM
	return this->_id == GetCurrentThreadId();
#endif
}

wsize ang_core_thread_impl_tle_size(ang_core_thread_ptr_t _this)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
	return this->_tle_size;
}

pointer ang_core_thread_impl_tle_buffer(ang_core_thread_ptr_t _this)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
	return this->_tle_data;
}

void ang_core_thread_impl_set_tle_data(ang_core_thread_ptr_t _this, pointer data, wsize sz, ang_bool_t alloc)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
	if (this->tle_notify_callback) this->tle_notify_callback(_this, this->_tle_size, this->_tle_data);
	if (this->_del_tle && this->_tle_data != NULL)
		ang_free_unmanaged_memory(this->_tle_data);
	if (alloc && sz > 0) {
		this->_del_tle = ang_true;
		this->_tle_size = sz;
		this->_tle_data = ang_alloc_unmanaged_memory(sz);
		if (data) memcpy(this->_tle_data, data, sz);
	}
	else {
		this->_del_tle = ang_false;
		this->_tle_size = sz;
		this->_tle_data = data;
	}
}

void ang_core_thread_impl_set_tle_notify(ang_core_thread_ptr_t _this, ang_core_thread_tle_deleting_notify_callback_t callback)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
	this->tle_notify_callback = callback;
}

pointer ang_core_thread_impl_user_args(ang_core_thread_ptr_t _this)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
	return this->_user_args;
}

dword ang_core_thread_impl_thread_id(ang_core_thread_ptr_t _this)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return this->_thread;
#elif defined WINDOWS_PLATFORM
	return this->_id;
#endif
}

uint ang_core_thread_impl_thread_state(ang_core_thread_ptr_t _this)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
	return this->_state;
}

void ang_core_thread_impl_sleep(ang_core_thread_ptr_t _this, dword ms)
{
	if(ang_core_thread_impl_is_current_thread(_this))
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		usleep(ms * 1000);
#elif defined WINDOWS_PLATFORM
		Sleep(ms);
#endif
}

ang_bool_t ang_core_thread_impl_start(ang_core_thread_ptr_t _this, ang_core_thread_start_routine_t routine, pointer args)
{
	ang_bool_t res = ang_false;
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;

	if (this->is_current_thread(_this))
		return ang_false;

	ang_lock_main_mutex();
	while (this->_state < ang_thread_state_wait_for_start)
		ang_wait_main_cond();
	if (this->_state > ang_thread_state_wait_for_start)
		goto END;
	this->_start_routine = routine;
	this->_user_args = args;
	ang_singal_main_cond();
	res = this->_start_routine != NULL;

END:
	ang_unlock_main_mutex();
	return res;
}

ang_bool_t ang_core_thread_impl_then(ang_core_thread_ptr_t _this, ang_core_thread_then_routine_t routine, pointer args)
{
	ang_bool_t res = ang_false;
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;

	ang_lock_main_mutex();
	if (this->_state > ang_thread_state_wait_for_finish)
		goto END;
	this->_then_routine = routine;
	this->_then_args = args;
	if (!this->is_current_thread(_this))
		ang_singal_main_cond();
	res = this->_then_routine != NULL;
END:
	ang_unlock_main_mutex();
	return res;
}

ang_bool_t ang_core_thread_impl_wait(ang_core_thread_ptr_t _this)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
	if (this->is_current_thread(_this))
		return ang_false;
	ang_lock_main_mutex();
	while (this->_state == ang_thread_state_running || this->_state == ang_thread_state_finishing)
		ang_wait_main_cond();
	ang_unlock_main_mutex();
	return ang_true;
}

ang_bool_t ang_core_thread_impl_timed_wait(ang_core_thread_ptr_t _this, dword ms)
{
	ang_bool_t res = ang_false;
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
	dword last_time = (dword)(ang_get_performance_time() / 1000);
	dword current = 0;

	if (this->is_current_thread(_this))
		return ang_false;

	ang_lock_main_mutex();
	if (this->_start_routine == NULL && this->_then_routine == NULL)
		goto END;

	while ((this->_state == ang_thread_state_running || this->_state == ang_thread_state_finishing) && ms != 0) {
		ang_timed_wait_main_cond(ms);
		this->sleep(_this, 1);
		current = (dword)(ang_get_performance_time() / 1000);
		if (ms <= (current - last_time))
			break;
		else ms -= (current - last_time);
		last_time = current;
	}
	res = (this->_state != ang_thread_state_running && this->_state != ang_thread_state_finishing);
END:
	ang_unlock_main_mutex();
	return res;
}

ang_bool_t ang_core_thread_impl_cancel(ang_core_thread_ptr_t _this)
{
	ang_bool_t res = ang_false;
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;
	ang_lock_main_mutex();
	if (this->_state >= ang_thread_state_finished)
		goto END;
	this->_state = ang_thread_state_canceled;
	if (!this->is_current_thread(_this))
		ang_singal_main_cond();
END:
	ang_unlock_main_mutex();
	return res;
}

ang_bool_t ang_core_thread_impl_join(ang_core_thread_ptr_t _this)
{
	ang_core_thread_impl_ptr_t this = (ang_core_thread_impl_ptr_t)_this;

	if (this->is_current_thread(_this))
		return ang_false;

	ang_lock_main_mutex();
	while (this->_state == ang_thread_state_running || this->_state == ang_thread_state_finishing)
		ang_wait_main_cond();
	this->_state = ang_thread_state_canceled;
	ang_singal_main_cond();
	ang_unlock_main_mutex();
	return ang_true;
}

//////////////////////////////////////////////////////////////////////////////////////

typedef struct ang_core_thread_manager
{
	ang_hash_table_ptr_t _thread_map;
	ang_core_cond_ptr_t _main_cond;
	ang_core_mutex_ptr_t _main_mutex;
	ang_core_thread_ptr_t _main_thread;
}ang_core_thread_manager_t, *ang_core_thread_manager_ptr_t;



ang_core_thread_manager_ptr_t ang_core_thread_manager_instance(ang_core_thread_manager_ptr_t ptr, ang_bool_t write, ang_bool_t del)
{
	static ang_core_thread_manager_ptr_t _instance = NULL;
	if (write) {
		if (_instance && del) delete(_instance);
		_instance = ptr;
	}
	return _instance;
}

ang_core_thread_manager_ptr_t ang_core_thread_manager_get_instance()
{
	return ang_core_thread_manager_instance(NULL, ang_false, ang_false);
}

static void ang_core_thread_manager_hash_table_delete(ang_hash_table_pair_t data)
{
	ang_core_thread_ptr_t thread = (ang_core_thread_ptr_t)(void*)data.value;
	if (thread != NULL)
		ang_core_thread_destroy(thread);
}

static ulong64 ang_core_thread_manager_hash_table_create_hash(ulong64 key, ulong64 size)
{
	return (ulong64)((2475825 + key + 1) % size);
}

ang_bool_t ang_core_thread_manager_initialize()
{
	ang_core_thread_manager_ptr_t manager;
	if (ang_core_thread_manager_get_instance() != NULL)
		return ang_false;
	manager = new(ang_core_thread_manager_t);
	memset(manager, 0, sizeof(ang_core_thread_manager_t));
	manager->_main_cond = ang_core_cond_create();
	manager->_main_mutex = ang_core_mutex_create();
	manager->_thread_map = ang_hash_table_create(
		&ang_core_thread_manager_hash_table_delete,
		&ang_core_thread_manager_hash_table_create_hash
	);

	ang_core_thread_manager_instance(manager, ang_true, ang_false);

	manager->_main_thread = ang_core_thread_attach_this_thread(ang_true, 0, NULL, ang_false);
	manager->_thread_map->insert(manager->_thread_map, ang_core_thread_this_thread_id(), (ulong64)manager->_main_thread);
	return ang_true;
}

ang_bool_t ang_core_thread_manager_terminate()
{
	ang_core_mutex_ptr_t mutex;
	ang_core_thread_manager_ptr_t manager = ang_core_thread_manager_get_instance();
	if (manager == NULL)
		return ang_false;
	mutex = manager->_main_mutex;
	mutex->lock(mutex);
	manager->_thread_map->clean(manager->_thread_map);
	ang_hash_table_destroy(manager->_thread_map);
	ang_core_cond_destroy(manager->_main_cond);
	ang_core_thread_manager_instance(NULL, ang_true, ang_true);
	mutex->unlock(mutex);
	ang_core_mutex_destroy(mutex);
	return ang_true;
}

ang_core_thread_ptr_t ang_core_thread_main_thread()
{
	return ang_core_thread_manager_get_instance()->_main_thread;
}

ang_core_cond_ptr_t ang_core_thread_main_cond()
{
	return ang_core_thread_manager_get_instance()->_main_cond;
}

ang_core_mutex_ptr_t ang_core_thread_main_mutex()
{
	return ang_core_thread_manager_get_instance()->_main_mutex;
}

dword ang_core_thread_this_thread_id()
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return (dword)pthread_self();
#elif defined WINDOWS_PLATFORM
	return GetCurrentThreadId();
#endif
}

ang_core_thread_ptr_t ang_core_thread_this_thread()
{
	ulong64 value = 0;
	ang_hash_table_ptr_t thread_map;
	ang_core_thread_ptr_t thread = NULL;
	ang_lock_main_mutex();
	thread_map = ang_core_thread_manager_get_instance()->_thread_map;
	if (!thread_map->find(thread_map, ang_core_thread_this_thread_id(), &value))
	{
		thread = ang_core_thread_attach_this_thread(ang_false, 0, NULL, ang_false);
		thread_map->insert(thread_map, ang_core_thread_this_thread_id(), (ulong64)thread);
	}
	else thread = (ang_core_thread_ptr_t)(void*)value;
	ang_unlock_main_mutex();
	return thread;
}

ang_core_thread_ptr_t ang_core_thread_regist_thread(ang_core_thread_ptr_t thread)
{
	ang_hash_table_ptr_t thread_map;
	ang_core_thread_ptr_t result = NULL;
	ang_lock_main_mutex();
	thread_map = ang_core_thread_manager_get_instance()->_thread_map;
	if (thread_map->insert(thread_map, thread->thread_id(thread), (ulong64)thread))
		result = thread;
	ang_unlock_main_mutex();
	return result;
}

ang_core_thread_ptr_t ang_core_thread_unregist_thread(ang_core_thread_ptr_t thread)
{
	ulong64 value = 0;
	ang_hash_table_ptr_t thread_map;
	ang_core_thread_ptr_t result = NULL;
	ang_lock_main_mutex();
	thread_map = ang_core_thread_manager_get_instance()->_thread_map;
	if (thread_map->remove(thread_map, thread->thread_id(thread), &value))
		result = (ang_core_thread_ptr_t)(void*)value;
	ang_unlock_main_mutex();
	return result;
}

ang_bool_t ang_lock_main_mutex()
{
	ang_core_mutex_ptr_t mutex = ang_core_thread_manager_get_instance()->_main_mutex;
	return mutex->lock(mutex);
}

ang_bool_t ang_unlock_main_mutex()
{
	ang_core_mutex_ptr_t mutex = ang_core_thread_manager_get_instance()->_main_mutex;
	return mutex->unlock(mutex);
}

ang_bool_t ang_singal_main_cond()
{
	ang_core_cond_ptr_t cond = ang_core_thread_manager_get_instance()->_main_cond;
	return cond->signal(cond);
}

ang_bool_t ang_wait_main_cond()
{
	ang_core_thread_manager_ptr_t manager = ang_core_thread_manager_get_instance();
	return manager->_main_cond->wait(manager->_main_cond, manager->_main_mutex);
}

ang_bool_t ang_timed_wait_main_cond(dword ms)
{
	ang_core_thread_manager_ptr_t manager = ang_core_thread_manager_get_instance();
	return manager->_main_cond->timed_wait(manager->_main_cond, manager->_main_mutex, ms);
}
