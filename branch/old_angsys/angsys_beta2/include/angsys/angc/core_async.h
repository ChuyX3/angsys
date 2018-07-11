/*********************************************************************************************************************/
/*   File Name: angtypes.h                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types for multiplatform use                                  */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGC_CORE_ASYNC_H__
#define __ANGC_CORE_ASYNC_H__

#include <angtypes.h>

#ifdef LINK
#undef LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

#ifndef MICRO8_PLATFORM

typedef enum ang_thread_state_enum
{
	ang_thread_state_initializing = 0,
	ang_thread_state_wait_for_start,
	ang_thread_state_running,
	ang_thread_state_wait_for_finish,
	ang_thread_state_finishing,
	ang_thread_state_finished,
	ang_thread_state_canceled = ang_thread_state_finished,
	ang_thread_state_attached,
	ang_thread_state_max_size = 0XFFFFFFFF,
}ang_thread_state, ang_thread_state_t;

typedef struct ang_core_cond ang_core_cond_t, *ang_core_cond_ptr_t;
typedef struct ang_core_mutex ang_core_mutex_t, *ang_core_mutex_ptr_t;
typedef struct ang_core_thread ang_core_thread_t, *ang_core_thread_ptr_t;
typedef struct ang_core_thread_mgr ang_core_thread_mgr_t, *ang_core_thread_mgr_ptr_t;

#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
typedef pointer(*ang_core_thread_start_routine_t)(pointer);
typedef pointer(*ang_core_thread_then_routine_t)(pointer, pointer);
#elif defined WINDOWS_PLATFORM
typedef dword(STDCALL*ang_core_thread_start_routine_t)(pointer);
typedef dword(STDCALL*ang_core_thread_then_routine_t)(pointer, dword);
#endif
typedef void(*ang_core_thread_tle_deleting_notify_callback_t)(ang_core_thread_ptr_t, wsize, pointer);

ANG_EXTERN ang_core_cond_ptr_t LINK ang_core_cond_create();
ANG_EXTERN void LINK ang_core_cond_destroy(ang_core_cond_ptr_t);

struct ang_core_cond
{
#ifdef __cplusplus
private:
	ang_bool_t(*_signal)(ang_core_cond_ptr_t);
	ang_bool_t(*_wait)(ang_core_cond_ptr_t, ang_core_mutex_ptr_t);
	ang_bool_t(*_timed_wait)(ang_core_cond_ptr_t, ang_core_mutex_ptr_t, dword);

public:
	static inline ang_core_cond_ptr_t create() { return ang_core_cond_create(); }
	static inline void destroy(ang_core_cond_ptr_t cond) { ang_core_cond_destroy(cond); }

	inline bool signal()const { return _signal(const_cast<ang_core_cond_ptr_t>(this)) ? true : false; }
	inline bool wait(ang_core_mutex_ptr_t mutex)const { return _wait(const_cast<ang_core_cond_ptr_t>(this), mutex) ? true : false; }
	inline bool wait(ang_core_mutex_ptr_t mutex, dword ms)const { return _timed_wait(const_cast<ang_core_cond_ptr_t>(this), mutex, ms) ? true : false; }
#else
	ang_bool_t(*signal)(ang_core_cond_ptr_t);
	ang_bool_t(*wait)(ang_core_cond_ptr_t, ang_core_mutex_ptr_t);
	ang_bool_t(*timed_wait)(ang_core_cond_ptr_t, ang_core_mutex_ptr_t, dword);
#endif
};

ANG_EXTERN ang_core_mutex_ptr_t LINK ang_core_mutex_create();
ANG_EXTERN void LINK ang_core_mutex_destroy(ang_core_mutex_ptr_t);

struct ang_core_mutex
{
#ifdef __cplusplus
private:
	ang_bool_t(*_lock)(ang_core_mutex_ptr_t);
	ang_bool_t(*_trylock)(ang_core_mutex_ptr_t);
	ang_bool_t(*_unlock)(ang_core_mutex_ptr_t);

public:
	static inline ang_core_mutex_ptr_t create() { return ang_core_mutex_create(); }
	static inline void destroy(ang_core_mutex_ptr_t cond) { ang_core_mutex_destroy(cond); }

	inline bool lock()const { return _lock(const_cast<ang_core_mutex_ptr_t>(this)) ? true : false; }
	inline bool trylock()const { return _trylock(const_cast<ang_core_mutex_ptr_t>(this)) ? true : false; }
	inline bool unlock()const { return _unlock(const_cast<ang_core_mutex_ptr_t>(this)) ? true : false; }
#else
	ang_bool_t(*lock)(ang_core_mutex_ptr_t);
	ang_bool_t(*trylock)(ang_core_mutex_ptr_t);
	ang_bool_t(*unlock)(ang_core_mutex_ptr_t);
#endif
};

#ifdef __cplusplus
template<typename T> struct ang_scope_locker
{
	T _mutex;
	ang_scope_locker(T m) : _mutex(m) { _mutex->lock(); }
	~ang_scope_locker() { _mutex->unlock(); }
};
#endif

ANG_EXTERN ang_core_thread_ptr_t ang_core_thread_create_suspended(uint flags, wsize sz, pointer tle, ang_bool_t alloc);
ANG_EXTERN ang_core_thread_ptr_t ang_core_thread_create(ang_core_thread_start_routine_t routine, pointer args, uint flags, wsize sz, pointer tle, ang_bool_t alloc);
ANG_EXTERN void ang_core_thread_destroy(ang_core_thread_ptr_t);
ANG_EXTERN ang_core_thread_ptr_t ang_core_thread_main_thread();
ANG_EXTERN ang_core_thread_ptr_t ang_core_thread_this_thread();
ANG_EXTERN dword ang_core_thread_this_thread_id();

struct ang_core_thread
{
#ifdef __cplusplus
private:
	ang_bool_t(*_is_main_thread)(ang_core_thread_ptr_t);
	ang_bool_t(*_is_current_thread)(ang_core_thread_ptr_t);
	wsize(*_tle_size)(ang_core_thread_ptr_t);
	pointer(*_tle_buffer)(ang_core_thread_ptr_t);
	void(*_set_tle_data)(ang_core_thread_ptr_t, pointer, wsize, ang_bool_t);
	void(*_set_tle_notify)(ang_core_thread_ptr_t, ang_core_thread_tle_deleting_notify_callback_t);
	pointer(*_user_args)(ang_core_thread_ptr_t);
	dword(*_thread_id)(ang_core_thread_ptr_t);
	uint(*_thread_state)(ang_core_thread_ptr_t);
	void(*_sleep)(ang_core_thread_ptr_t, dword);
	ang_bool_t(*_start)(ang_core_thread_ptr_t, ang_core_thread_start_routine_t, pointer);
	ang_bool_t(*_then)(ang_core_thread_ptr_t, ang_core_thread_then_routine_t, pointer);
	ang_bool_t(*_wait)(ang_core_thread_ptr_t);
	ang_bool_t(*_timed_wait)(ang_core_thread_ptr_t, dword);
	ang_bool_t(*_cancel)(ang_core_thread_ptr_t);
	ang_bool_t(*_join)(ang_core_thread_ptr_t);

public:
	static inline ang_core_thread_ptr_t create(uint flags, wsize sz, pointer tle, bool alloc) {
		return ang_core_thread_create_suspended(flags, sz, tle, (ang_bool_t)alloc);
	}
	static inline ang_core_thread_ptr_t create(ang_core_thread_start_routine_t routine, pointer args, uint flags, wsize sz, pointer tle, bool alloc) {
		return ang_core_thread_create(routine, args, flags, sz, tle, (ang_bool_t)alloc);
	}
	static inline void destroy(ang_core_thread_ptr_t thread) { ang_core_thread_destroy(thread); }
	static inline ang_core_thread_ptr_t main_thread() { return ang_core_thread_main_thread(); }
	static inline ang_core_thread_ptr_t this_thread() { return ang_core_thread_this_thread(); }
	static inline dword this_thread_id() { return ang_core_thread_this_thread_id(); }

	inline bool is_main_thread()const { return _is_main_thread(const_cast<ang_core_thread_ptr_t>(this)) ? true : false; }
	inline bool is_current_thread()const { return _is_current_thread(const_cast<ang_core_thread_ptr_t>(this)) ? true : false; }
	inline wsize tle_size()const { return _tle_size(const_cast<ang_core_thread_ptr_t>(this)); }
	inline pointer tle_buffer()const { return _tle_buffer(const_cast<ang_core_thread_ptr_t>(this)); }
	template<typename T> T* tle_data()const { return tle_size() == sizeof(T) ? (T*)tle_buffer() : NULL; }
	inline void set_tle_data(pointer ptr, wsize sz, bool alloc = false) { _set_tle_data(this, ptr, sz, (ang_bool_t)alloc); }
	inline void set_tle_notify(ang_core_thread_tle_deleting_notify_callback_t callback) { _set_tle_notify(this, callback); }
	inline pointer user_args()const { return _user_args(const_cast<ang_core_thread_ptr_t>(this)); }
	inline dword thread_id()const { return _thread_id(const_cast<ang_core_thread_ptr_t>(this)); }
	inline uint thread_state()const { return _thread_state(const_cast<ang_core_thread_ptr_t>(this)); }
	inline void sleep(dword ms) { _sleep(const_cast<ang_core_thread_ptr_t>(this), ms); }
	inline bool start(ang_core_thread_start_routine_t callback, pointer args) { return _start(this, callback, args) ? true : false; }
	inline bool then(ang_core_thread_then_routine_t callback, pointer args) { return _then(this, callback, args) ? true : false; }
	inline bool wait() { return _wait(this) ? true : false; }
	inline bool wait(dword ms) { return _timed_wait(this, ms) ? true : false; }
	inline bool cancel() { return _cancel(this) ? true : false; }
	inline bool join() { return _join(this) ? true : false; }

#else
	ang_bool_t(*is_main_thread)(ang_core_thread_ptr_t);
	ang_bool_t(*is_current_thread)(ang_core_thread_ptr_t);
	wsize(*tle_size)(ang_core_thread_ptr_t);
	pointer(*tle_buffer)(ang_core_thread_ptr_t);
	void(*set_tle_data)(ang_core_thread_ptr_t, pointer, wsize, ang_bool_t);
	void(*set_tle_notify)(ang_core_thread_ptr_t, ang_core_thread_tle_deleting_notify_callback_t);
	pointer(*user_args)(ang_core_thread_ptr_t);
	dword(*thread_id)(ang_core_thread_ptr_t);
	uint(*thread_state)(ang_core_thread_ptr_t);
	void(*sleep)(ang_core_thread_ptr_t, dword);
	ang_bool_t(*start)(ang_core_thread_ptr_t, ang_core_thread_start_routine_t, pointer);
	ang_bool_t(*then)(ang_core_thread_ptr_t, ang_core_thread_then_routine_t, pointer);
	ang_bool_t(*wait)(ang_core_thread_ptr_t);
	ang_bool_t(*timed_wait)(ang_core_thread_ptr_t, dword);
	ang_bool_t(*cancel)(ang_core_thread_ptr_t);
	ang_bool_t(*join)(ang_core_thread_ptr_t);
#endif
};

#endif // !MICRO8_PLATFORM

#ifdef LINK
#undef LINK
#endif//LINK

#endif//__ANGC_CORE_ASYNC_H__