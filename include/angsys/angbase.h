/*********************************************************************************************************************/
/*   File Name: angbase.h                                                                                            */
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

#include <angtypes.h>

#ifdef __cplusplus

//#include <ang/interface.h>
//#include <ang/utils.h>

#endif//__cplusplus

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

ANG_EXTERN LINK ang_void_ptr_t ang_alloc_unmanaged_memory(ang_size_t);
ANG_EXTERN LINK void ang_free_unmanaged_memory(ang_void_ptr_t);

typedef enum _ang_memory_hint {
	ang_default_memory,
	ang_object_memory,
	ang_buffer_memory,
}ang_memory_hint, ang_memory_hint_t;

ANG_EXTERN LINK ang_void_ptr_t ang_alloc_managed_memory(ang_size_t, ang_memory_hint_t);
ANG_EXTERN LINK void ang_free_managed_memory(ang_void_ptr_t, ang_memory_hint_t);

ANG_EXTERN LINK ang_void_ptr_t ang_alloc_aligned_memory(ang_size_t, wsize ALIGMENT);
ANG_EXTERN LINK void ang_free_aligned_memory(ang_void_ptr_t);

#ifdef _DEBUG
ANG_EXTERN LINK ang_void_ptr_t ang_alloc_unmanaged_memory_debug(ang_size_t, const char* file, int line);
ANG_EXTERN LINK ang_void_ptr_t ang_alloc_managed_memory_debug(ang_size_t, ang_memory_hint_t, const char* file, int line);
ANG_EXTERN LINK ang_void_ptr_t ang_alloc_aligned_memory_debug(ang_size_t, wsize ALIGMENT, const char* file, int line);
#endif

ANG_EXTERN LINK void ang_throw_exception(dword code, const char* message);

ANG_EXTERN LINK ang_bool_t ang_is_little_endian_platform();

#if _DEBUG
#define ANG_ASSERT(EXPRESION, MESSAGE) if(!(EXPRESION)) ang_throw_exception(-1, MESSAGE)
#else
#define ANG_ASSERT(EXPRESION, MESSAGE) (void)0
#endif // _DEBUG

#define ANG_STATIC_ASSERT(EXPRESION, MESSAGE) static_assert(EXPRESION, MESSAGE)


#ifdef __cplusplus

namespace ang
{
	class object;
	template<typename T> class object_wrapper;
	template<typename T> class object_wrapper_ptr;
	template<typename T> class intf_wrapper;
	template<typename T> class intf_wrapper_ptr;

	typedef object_wrapper<object> object_t, objptr;

	template<typename T> struct safe_str;

	typedef decltype(nullptr) nullptr_t;
	template<typename T> using initializer_list_t = std::initializer_list<T>;

	constexpr windex invalid_index = -1;
	nullptr_t const null = {};

	typedef pointer			unknown_t;
	typedef pointer*		unknown_ptr_t;

	typedef char					ascii_char_t;
	typedef wchar					unicode_char_t;
	typedef byte					utf8_char_t;
	typedef char16_t				utf16_char_t;
	typedef char32_t 				utf32_char_t;

	typedef safe_str<char>			str_t, ascii_str_t;
	typedef safe_str<wchar>			wstr_t, unicode_str_t;
	typedef safe_str<byte>			mstr_t, utf8_str_t;
	typedef safe_str<char16_t>		utf16_str_t;
	typedef safe_str<char32_t>		utf32_str_t;
	typedef safe_str<char const>	cstr_t, ascii_cstr_t;
	typedef safe_str<wchar const>	cwstr_t, unicode_cstr_t;
	typedef safe_str<byte const>	cmstr_t, utf8_cstr_t;
	typedef safe_str<char16_t const> utf16_cstr_t;
	typedef safe_str<char32_t const> utf32_cstr_t;
	typedef safe_str<char const>	type_name_t;

	typedef char*				unsafe_ascii_str_t;
	typedef const char*			unsafe_ascii_cstr_t;
	typedef wchar*				unsafe_unicode_str_t;
	typedef const wchar*		unsafe_unicode_cstr_t;
	typedef byte*				unsafe_utf8_str_t;
	typedef const byte*			unsafe_utf8_cstr_t;
	typedef char16_t*			unsafe_utf16_str_t;
	typedef const char16_t*		unsafe_utf16_cstr_t;
	typedef char32_t*			unsafe_utf32_str_t;
	typedef const char32_t*		unsafe_utf32_cstr_t;

	namespace memory
	{
		template<typename T> struct unmanaged_allocator;
		template<typename T, ang_memory_hint_t HINT> struct managed_allocator;
		template<typename T, wsize ALIGNMENT> struct aligned_allocator;
	
		template<typename T> using default_allocator = managed_allocator<T, ang_memory_hint::ang_default_memory>;
		template<typename T> using object_allocator = managed_allocator<T, ang_memory_hint::ang_object_memory>;
		template<typename T> using buffer_allocator = managed_allocator<T, ang_memory_hint::ang_buffer_memory>;

		template<typename T> using aligned1_allocator = aligned_allocator<T, 1U>;
		template<typename T> using aligned4_allocator = aligned_allocator<T, 4U>;
		template<typename T> using aligned8_allocator = aligned_allocator<T, 8U>;
		template<typename T> using aligned16_allocator = aligned_allocator<T, 16U>;
	}

	namespace collections
	{
		template<typename T> struct array_view;
		template<typename T, template <typename> class allocator = memory::default_allocator> struct scope_array;
		template<typename T, wsize _SIZE> struct stack_array;
	}

	using collections::array_view;
	using collections::stack_array;
	using collections::scope_array;
}

using ang::null;

#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
inline pointer operator new(wsize, pointer ptr) throw() { return ptr; }
inline void operator delete(pointer, pointer) throw() { return; }
#endif

#include <ang/base/utils.hpp>
#include <ang/base/enum.hpp>
#include <ang/base/interface.hpp>
#include <ang/base/allocator.hpp>
#include <ang/base/array.hpp>
#include <ang/base/text.hpp>
#include <ang/base/string.hpp>
//#include <ang/base/string.hpp>

#endif//__cplusplus

#ifdef LINK
#undef LINK
#endif//LINK

#endif//__ANG_BASE_H__