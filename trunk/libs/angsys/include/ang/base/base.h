/*********************************************************************************************************************/
/*   File Name: ang/base/base.h                                                                                      */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#define __ANG_BASE_H__

#include <ang/base/types.h>

#ifdef LINK
#undef LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANG_DYNAMIC_LIBRARY

#ifdef ANG_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_EXPORTS
#else//#elif defined ANG_STATIC_LIBRARY
#define LINK
#endif//ANG_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

//#define ANG_DYNAMIC_INITIALIZE
#ifdef ANG_DYNAMIC_INITIALIZE
ANG_EXTERN LINK void ang_library_start();
ANG_EXTERN LINK void ang_library_shutdown();
#endif

typedef enum _ang_memory_hint {
	ang_default_memory = 0,
	ang_object_memory,
	ang_buffer_memory,
	ang_aligned_memory,
}ang_memory_hint, ang_memory_hint_t;

ANG_EXTERN LINK pointer ang_alloc_unmanaged_memory(wsize);
ANG_EXTERN LINK void ang_free_unmanaged_memory(pointer);

ANG_EXTERN LINK pointer ang_alloc_managed_memory(wsize, ang_memory_hint_t);
ANG_EXTERN LINK void ang_free_managed_memory(pointer, ang_memory_hint_t);
ANG_EXTERN LINK pointer ang_alloc_aligned_memory(wsize, wsize ALIGMENT);
ANG_EXTERN LINK void ang_free_aligned_memory(pointer);

ANG_EXTERN LINK void* ang_alloc_object_memory(wsize sz, ang_memory_hint_t hint, ushort ALIGMENT);

#ifdef _DEBUG
ANG_EXTERN LINK pointer ang_alloc_unmanaged_memory_debug(wsize, const char* file, int line);
ANG_EXTERN LINK pointer ang_alloc_managed_memory_debug(wsize, ang_memory_hint_t, const char* file, int line);
ANG_EXTERN LINK pointer ang_alloc_aligned_memory_debug(wsize, wsize ALIGMENT, const char* file, int line);
ANG_EXTERN LINK void* ang_alloc_object_memory_debug(wsize sz, const char* file, int line, ang_memory_hint_t hint, ushort ALIGMENT);
#endif

#ifdef __cplusplus

#include <ang/base/defines.h>
#include <ang/base/utils.h>
#include <ang/base/memory.h>

namespace ang
{
	namespace collections
	{
		template<typename T> struct array_view;
		template<typename T, wsize SIZE> struct stack_array;
		template<typename T, template<typename> class A = memory::auto_detect_buffer_allocator> struct array;
		template<typename T, template<typename> class A = memory::auto_detect_buffer_allocator>	struct vector;
	}

	using collections::array_view;
	using collections::stack_array;
	using collections::array;
}


#include <ang/base/text.h>
#include <ang/base/packs.h>
#include <ang/base/tuple.h>
#include <ang/base/algorithms.h>
#include <ang/base/array.h>
#include <ang/base/vector.h>
#include <ang/base/encoder.h>
#include <ang/base/str_view.h>
#include <ang/base/string.h>
#include <ang/base/runtime.h>
#include <ang/base/value.h>
#include <ang/base/list.h>
#include <ang/base/hash.h>
#include <ang/base/binary_tree.h>
#include <ang/base/errors.h>
#include <ang/base/interface.h>
#include <ang/base/formater.h>
#include <ang/base/anyone.h>
#include <ang/base/async.h>
#include <ang/maths/maths.h>
#endif//__cplusplus

#ifdef LINK
#undef LINK
#endif//LINK

#ifdef __cplusplus
#include <ang/base/inline/text.inl>
#include <ang/base/inline/formater.inl>
#include <ang/base/inline/string.inl>
#include <ang/base/inline/runtime.inl>
#include <ang/base/inline/array.inl>
#include <ang/base/inline/vector.inl>
#endif//__cplusplus

#endif//__ANG_BASE_H__
