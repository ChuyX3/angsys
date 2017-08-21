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

#ifndef __ANG_TYPES_H__
#define __ANG_TYPES_H__

#include<angplatform.h>

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

#if defined WINDOWS_PLATFORM
#define STDCALL	__stdcall
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
#define __stdcall
#define STDCALL __stdcall
#elif defined MICRO8_PLATFORM
#define __stdcall
#define STDCALL __stdcall
#endif

#ifdef _DEVELOPPER
#define ANG_DEVELOPPER //for developper
#endif//ANG_DEVELOPPER

#if defined _DEBUG || defined _DEVELOPPER
#define DEBUG_SAFE_CODE
#endif//_DEBUG

#ifdef __cplusplus
#define ANG_EXTERN extern "C"
#else
#define ANG_EXTERN extern
#endif//__cplusplus

typedef char				ang_char_t;
typedef unsigned char		ang_mchar_t;
typedef wchar_t				ang_wchar_t;

typedef char				ang_int8_t;
typedef unsigned char		ang_uint8_t;

typedef ang_uint8_t 		ang_bool_t;
#define ang_false ((ang_bool_t)0)
#define ang_true ((ang_bool_t)1)

#ifndef MICRO8_PLATFORM
typedef short				ang_int16_t;
typedef unsigned short		ang_uint16_t;
typedef int					ang_int32_t;
typedef unsigned int		ang_uint32_t;
typedef long long			ang_int64_t;
typedef unsigned long long	ang_uint64_t;
typedef float				ang_float32_t;
typedef double				ang_float64_t;
#else
typedef short int				ang_int16_t;
typedef unsigned short int		ang_uint16_t;
typedef short long int			ang_int24_t;
typedef unsigned short long int	ang_uint24_t;
typedef long int				ang_int32_t;
typedef unsigned long int		ang_uint32_t;
typedef double					ang_float32_t;
#endif//MICRO8_PLATFORM

#if defined WINDOWS_PLATFORM
#if defined _WIN64
typedef unsigned __int64	ang_size_t;
typedef unsigned __int64	ang_index_t;
typedef unsigned __int64	ang_int_ptr_t;
typedef __int64				ang_long_t;
typedef unsigned __int64	ang_ulong_t;
typedef void*				ang_handle_t;
#elif defined _ARM_
typedef unsigned int	ang_size_t;
typedef unsigned int	ang_index_t;
typedef unsigned int	ang_int_ptr_t;
typedef unsigned long	ang_long_t;
typedef long			ang_ulong_t;
typedef void*			ang_handle_t;
#elif defined _WIN32 
typedef __w64 unsigned int	ang_size_t;
typedef __w64 unsigned int	ang_index_t;
typedef __w64 unsigned int	ang_int_ptr_t;
typedef __w64 int			ang_long_t;
typedef __w64 unsigned int	ang_ulong_t;
typedef void*				ang_handle_t;
#endif
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
typedef unsigned int		ang_size_t;
typedef unsigned int		ang_index_t;
typedef unsigned int*		ang_int_ptr_t;
typedef unsigned long		ang_long_t;
typedef long				ang_ulong_t;
typedef long				ang_handle_t;
#elif defined MICRO8_PLATFORM
typedef unsigned int		ang_size_t;
typedef unsigned int		ang_index_t;
typedef unsigned int*		ang_int_ptr_t;
typedef unsigned int* far	ang_int_lptr_t;

#endif
typedef void*				ang_void_ptr_t;
typedef void*				ang_object_ptr_t;
typedef void*				ang_buffer_t;
typedef char*				ang_str_t;
typedef wchar_t*			ang_wstr_t;
typedef unsigned char*		ang_mstr_t;
typedef char const*			ang_cstr_t;
typedef wchar_t const*		ang_cwstr_t;
typedef unsigned char const* ang_cmstr_t;
typedef char const*			ang_type_name_t;

typedef unsigned char		ang_byte_t;
typedef unsigned short		ang_word_t;
typedef unsigned long		ang_dword_t;

typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned char mchar;
typedef wchar_t wchar;
typedef unsigned short word;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long dword;
typedef unsigned long ulong;
typedef long long long64;
typedef unsigned long long qword;
typedef unsigned long long ulong64;
typedef void * pointer;
#ifdef  index
#undef  index
#endif//index
typedef uint index;
typedef size_t windex;
typedef size_t wsize;

typedef union
{
	ang_byte_t value;
	struct {
		ang_bool_t bit0 : 1;
		ang_bool_t bit1 : 1;
		ang_bool_t bit2 : 1;
		ang_bool_t bit3 : 1;
		ang_bool_t bit4 : 1;
		ang_bool_t bit5 : 1;
		ang_bool_t bit6 : 1;
		ang_bool_t bit7 : 1;
	}bits;
}ang_byte_bits_t;

typedef union
{
	ang_word_t value;
	struct {
		ang_byte_bits_t lo;
		ang_byte_bits_t hi;
	}bytes;
}ang_word_bits_t;

typedef union
{
	ang_dword_t value;
	struct {
		ang_word_bits_t lo;
		ang_word_bits_t hi;
	}words;
}ang_dword_bits_t;

ANG_EXTERN ang_void_ptr_t LINK ang_alloc_unmanaged_memory(ang_size_t);
ANG_EXTERN void LINK ang_free_unmanaged_memory(ang_void_ptr_t);

#ifndef MICRO8_PLATFORM

//#ifdef __cplusplus
//ANG_EXTERN {
//#endif

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
typedef dword(STDCALL*ang_thread_start_routine)(pointer);
typedef dword(STDCALL*ang_thread_then_routine)(pointer, dword);
#endif

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
	inline bool signal()const { return _signal(const_cast<ang_core_cond_ptr_t>(this)) ? true : false; }
	inline bool wait(ang_core_mutex_ptr_t mutex)const { return _wait(const_cast<ang_core_cond_ptr_t>(this), mutex) ? true : false; }
	inline bool wait(ang_core_mutex_ptr_t mutex, dword ms)const { return _timed_wait(const_cast<ang_core_cond_ptr_t>(this), mutex, ms) ? true : false; }
#else
	ang_bool_t (*signal)(ang_core_cond_ptr_t);
	ang_bool_t (*wait)(ang_core_cond_ptr_t, ang_core_mutex_ptr_t);
	ang_bool_t (*timed_wait)(ang_core_cond_ptr_t, ang_core_mutex_ptr_t, dword);
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

ANG_EXTERN ang_core_thread_ptr_t ang_core_thread_create_thread_suspended(uint flags, wsize sz, pointer tle, ang_bool_t alloc);
ANG_EXTERN ang_core_thread_ptr_t ang_core_thread_create_thread(ang_core_thread_start_routine_t routine, pointer args, uint flags, wsize sz, pointer tle, ang_bool_t alloc);
ANG_EXTERN void ang_core_thread_destroy(ang_core_thread_ptr_t);
ANG_EXTERN  ang_core_thread_ptr_t ang_core_thread_main_thread();
ANG_EXTERN  ang_core_thread_ptr_t ang_core_thread_this_thread();
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
	pointer(*_user_args)(ang_core_thread_ptr_t);
	dword(*_thread_id)(ang_core_thread_ptr_t);
	void(*_sleep)(ang_core_thread_ptr_t, dword);
	ang_bool_t(*_start)(ang_core_thread_ptr_t, ang_core_thread_start_routine_t, pointer);
	ang_bool_t(*_then)(ang_core_thread_ptr_t, ang_core_thread_then_routine_t, pointer);
	ang_bool_t(*_wait)(ang_core_thread_ptr_t);
	ang_bool_t(*_timed_wait)(ang_core_thread_ptr_t, dword);
	ang_bool_t(*_cancel)(ang_core_thread_ptr_t);
	ang_bool_t(*_join)(ang_core_thread_ptr_t);

public:
	inline bool is_main_thread()const { return _is_main_thread(const_cast<ang_core_thread_ptr_t>(this)) ? true : false; }
	inline bool is_current_thread()const { return _is_current_thread(const_cast<ang_core_thread_ptr_t>(this)) ? true : false; }
	inline wsize tle_size()const { return _tle_size(const_cast<ang_core_thread_ptr_t>(this)); }
	inline pointer tle_buffer()const { return _tle_buffer(const_cast<ang_core_thread_ptr_t>(this)); }
	template<typename T> T* tle_data()const { return tle_size() == sizeof(T) ? (T*)tle_buffer() : NULL; }
	inline void set_tle_data(pointer ptr, wsize sz, bool alloc = false) { _set_tle_data(this, ptr, sz, (ang_bool_t)alloc); }
	inline pointer user_args()const { return _user_args(const_cast<ang_core_thread_ptr_t>(this)); }
	inline dword thread_id()const { return _thread_id(const_cast<ang_core_thread_ptr_t>(this)); }
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
	pointer(*user_args)(ang_core_thread_ptr_t);
	dword(*thread_id)(ang_core_thread_ptr_t);
	void(*sleep)(ang_core_thread_ptr_t, dword);
	ang_bool_t(*start)(ang_core_thread_ptr_t, ang_core_thread_start_routine_t, pointer);
	ang_bool_t(*then)(ang_core_thread_ptr_t, ang_core_thread_then_routine_t, pointer);
	ang_bool_t(*wait)(ang_core_thread_ptr_t);
	ang_bool_t(*timed_wait)(ang_core_thread_ptr_t, dword);
	ang_bool_t(*cancel)(ang_core_thread_ptr_t);
	ang_bool_t(*join)(ang_core_thread_ptr_t);
#endif
};


//#ifdef __cplusplus
//}
//#endif//__cplusplus
#endif // !MICRO8_PLATFORM

#ifdef LINK
#undef LINK
#endif//LINK

#endif//__ANG_TYPES_H__