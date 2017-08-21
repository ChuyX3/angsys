#ifndef __ANG_CORE_BASE_H__
#define __ANG_CORE_BASE_H__

#include <angtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

	
ang_bool_t ang_core_cond_signal(ang_core_cond_ptr_t);
ang_bool_t ang_core_cond_wait(ang_core_cond_ptr_t, ang_core_mutex_ptr_t);
ang_bool_t ang_core_cond_timed_wait(ang_core_cond_ptr_t, ang_core_mutex_ptr_t, dword);

ang_bool_t ang_core_mutex_lock(ang_core_mutex_ptr_t ptr);
ang_bool_t ang_core_mutex_trylock(ang_core_mutex_ptr_t ptr);
ang_bool_t ang_core_mutex_unlock(ang_core_mutex_ptr_t ptr);

ang_bool_t ang_core_thread_impl_is_main_thread(ang_core_thread_ptr_t _this);
ang_bool_t ang_core_thread_impl_is_current_thread(ang_core_thread_ptr_t _this);
wsize ang_core_thread_impl_tle_size(ang_core_thread_ptr_t _this);
pointer ang_core_thread_impl_tle_buffer(ang_core_thread_ptr_t _this);
void ang_core_thread_impl_set_tle_data(ang_core_thread_ptr_t _this, pointer data, wsize sz, ang_bool_t alloc);
pointer ang_core_thread_impl_user_args(ang_core_thread_ptr_t _this);
dword ang_core_thread_impl_thread_id(ang_core_thread_ptr_t _this);
void ang_core_thread_impl_sleep(ang_core_thread_ptr_t _this, dword ms);
ang_bool_t ang_core_thread_impl_start(ang_core_thread_ptr_t _this, ang_core_thread_start_routine_t routine, pointer args);
ang_bool_t ang_core_thread_impl_then(ang_core_thread_ptr_t _this, ang_core_thread_then_routine_t routine, pointer args);
ang_bool_t ang_core_thread_impl_wait(ang_core_thread_ptr_t _this);
ang_bool_t ang_core_thread_impl_timed_wait(ang_core_thread_ptr_t _this, dword ms);
ang_bool_t ang_core_thread_impl_cancel(ang_core_thread_ptr_t _this);
ang_bool_t ang_core_thread_impl_join(ang_core_thread_ptr_t _this);

typedef struct ang_core_cond_impl
{
	ang_bool_t (*signal)(ang_core_cond_ptr_t);
	ang_bool_t (*wait)(ang_core_cond_ptr_t, ang_core_mutex_ptr_t);
	ang_bool_t (*timed_wait)(ang_core_cond_ptr_t, ang_core_mutex_ptr_t, dword);

#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_cond_t _cond;
#elif defined WINDOWS_PLATFORM
	HANDLE _hevent;
#endif
}ang_core_cond_impl_t, *ang_core_cond_impl_ptr_t;

typedef struct ang_core_mutex_impl
{
	ang_bool_t(*lock)(ang_core_mutex_ptr_t);
	ang_bool_t(*trylock)(ang_core_mutex_ptr_t);
	ang_bool_t(*unlock)(ang_core_mutex_ptr_t);

#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_mutex_t _mutex;
#elif defined WINDOWS_PLATFORM
	HANDLE _hmutex;
#endif
}ang_core_mutex_impl_t, *ang_core_mutex_impl_ptr_t;


typedef struct ang_core_thread_impl
{
	ang_bool_t(*is_main_thread)(ang_core_thread_ptr_t);
	ang_bool_t(*is_current_thread)(ang_core_thread_ptr_t);
	wsize(*tle_size)(ang_core_thread_ptr_t);
	pointer(*tle_buffer)(ang_core_thread_ptr_t);
	void(*set_tle_data)(ang_core_thread_ptr_t, pointer, wsize, ang_bool_t);
	pointer(*user_args)(ang_core_thread_ptr_t);
	dword(*thread_id)(ang_core_thread_ptr_t);
	void(*sleep)(ang_core_thread_ptr_t, dword);
	ang_bool_t(*start)(ang_core_thread_ptr_t, ang_core_thread_start_routine_t, pointer);
	ang_bool_t(*then)(ang_core_thread_ptr_t, ang_core_thread_then_routine_t, pointer);
	ang_bool_t(*wait)(ang_core_thread_ptr_t);
	ang_bool_t(*timed_wait)(ang_core_thread_ptr_t, dword);
	ang_bool_t(*cancel)(ang_core_thread_ptr_t);
	ang_bool_t(*join)(ang_core_thread_ptr_t);


	ang_bool_t _is_main;
	ang_bool_t _del_tle;
	wsize _tle_size;
	pointer _tle_data;
	ang_thread_state _state;
	pointer _user_args;
	ang_core_thread_start_routine_t _start_routine;
	pointer _then_args;
	ang_core_thread_then_routine_t _then_routine;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_t _thread;
#elif defined WINDOWS_PLATFORM
	DWORD _id;
	HANDLE _hthread;
#endif

}ang_core_thread_impl_t, *ang_core_thread_impl_ptr_t;


ang_bool_t ang_core_thread_manager_initialize();
ang_bool_t ang_core_thread_manager_terminate();

ang_core_cond_ptr_t ang_core_thread_main_cond();
ang_core_mutex_ptr_t ang_core_thread_main_mutex();

ang_core_thread_ptr_t ang_core_thread_regist_thread(ang_core_thread_ptr_t);
ang_core_thread_ptr_t ang_core_thread_unregist_thread(ang_core_thread_ptr_t);

ang_bool_t ang_lock_main_mutex();
ang_bool_t ang_unlock_main_mutex();
ang_bool_t ang_singal_main_cond();
ang_bool_t ang_wait_main_cond();
ang_bool_t ang_timed_wait_main_cond(dword ms);

#ifdef __cplusplus
}
#endif

#endif//__ANG_CORE_BASE_H__