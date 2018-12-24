/*********************************************************************************************************************/
/*   File Name: ang/system.h                                                                                         */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is the base for all library, it exposes the "object" class which is the class       */
/*   base for many other clases.                                                                                     */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#define __ANGSYS_H__

#include <ang/base/base.h>

#ifdef  LINK
#undef  LINK
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

#ifdef __cplusplus

namespace ang
{
	class object;
	typedef ang::object_wrapper<object> object_t, objptr;
	typedef ang::object_wrapper_ptr<object> object_ptr_t;

	typedef struct raw_str raw_str_t;
	typedef struct raw_cstr raw_cstr_t;

	ang_interface(ivariant);

	typedef ivariant_t variant, var;
	template<typename T> class weak_ptr;
	template<typename T> class variable;
	template<typename T> using variable_t = object_wrapper<variable<T>>;
	template<typename T> using shared_ptr = variable_t<T>;
	template<typename T, typename...Ts> shared_ptr<T> make_shared(Ts... args);

	typedef variable<bool> boolean;
	typedef variable<int> integer;
	typedef variable<uint> uinteger;
	typedef variable<long> linteger;
	typedef variable<ulong> ulinteger;
	typedef variable<long64> integer64;
	typedef variable<ulong64> uinteger64;
	typedef variable<float> floating;
	typedef variable<double> floating64;

	typedef shared_ptr<bool> boolean_t;
	typedef shared_ptr<int> integer_t;
	typedef shared_ptr<uint> uinteger_t;
	typedef shared_ptr<long64> integer64_t;
	typedef shared_ptr<ulong64> uinteger64_t;
	typedef shared_ptr<float> floating_t;
	typedef shared_ptr<double> floating64_t;

	namespace strings
	{
		using namespace text;

		template<encoding ENCODING> class basic_const_string_buffer;
		template<encoding ENCODING> using basic_const_string = object_wrapper<basic_const_string_buffer<ENCODING>>;
		typedef basic_const_string_buffer<encoding::ascii> ascii_const_string_buffer, const_string_buffer;
		typedef basic_const_string_buffer<encoding::unicode> unicode_const_string_buffer, const_wstring_buffer;
		typedef basic_const_string_buffer<encoding::utf8> utf8_const_string_buffer, const_mstring_buffer;
		typedef object_wrapper<ascii_const_string_buffer> ascii_const_string, const_string;
		typedef object_wrapper<unicode_const_string_buffer> unicode_const_string, const_wstring;
		typedef object_wrapper<utf8_const_string_buffer> utf8_const_string, const_mstring;

		template<encoding ENCODING, template<typename> class allocator = memory::buffer_allocator> class basic_string_buffer;
		template<encoding ENCODING, template<typename> class allocator = memory::buffer_allocator> using basic_string = object_wrapper<basic_string_buffer<ENCODING, allocator>>;
		typedef basic_string_buffer<encoding::ascii> ascii_string_buffer, string_buffer;
		typedef basic_string_buffer<encoding::unicode> unicode_string_buffer, wstring_buffer;
		typedef basic_string_buffer<encoding::utf8> utf8_string_buffer, mstring_buffer;
		typedef basic_string_buffer<encoding::utf16> utf16_string_buffer, string_buffer16;
		typedef basic_string_buffer<encoding::utf32> utf32_string_buffer, string_buffer32;

		typedef object_wrapper<ascii_string_buffer> ascii_string, string;
		typedef object_wrapper<unicode_string_buffer> unicode_string, wstring;
		typedef object_wrapper<utf8_string_buffer> utf8_string, mstring;
		typedef object_wrapper<utf16_string_buffer> utf16_string, string16;
		typedef object_wrapper<utf32_string_buffer> utf32_string, string32;
	}

	namespace collections
	{
		template<typename T, template<typename> class allocator = memory::object_allocator> class array_buffer;
		template<typename T, template<typename> class allocator = memory::object_allocator> class vector_buffer;
		template<typename T, template<typename> class allocator = memory::object_allocator> using array = object_wrapper<array_buffer<T, allocator>>;
		template<typename T, template<typename> class allocator = memory::object_allocator> using vector = object_wrapper<vector_buffer<T, allocator>>;
	}

	namespace core
	{
		namespace delegates
		{
			template<typename T> class listener;
			template<typename T> class function_object;
			template<typename T> using function = object_wrapper<function_object<T>>;
			template<typename T> struct ifunction;
			template<typename T> using ifunction_t = intf_wrapper<ifunction<T>>;
			template<typename T> using method = intf_wrapper<ifunction<T>>;

			
		}
	}

	using strings::string;
	using strings::wstring;
	using strings::mstring;
	using strings::string16;
	using strings::string32;

	using collections::array;
	using collections::vector;

	using args_t = vector<objptr>;
	using args = collections::vector_buffer<var>;

	using vars_t = vector<var>;
	using vars = collections::vector_buffer<var>;

	using var_args_t = vector<var>;
	using var_args = collections::vector_buffer<var>;


	ang_object(exception);
}

#include <ang/smart_ptr.h>
#include <ang/object.h>
#include <ang/variable.h>
#include <ang/singleton.h>
#include <ang/collections.h>
#include <ang/buffers.h>
#include <ang/core/function.h>
#include <ang/collections/array.h>
#include <ang/collections/vector.h>
#include <ang/string.h>
#include <ang/exception.h>
#include <ang/core/listener.h>

#include <ang/inline/object.inl>
#include <ang/base/inline/runtime.inl>
#include <ang/core/inline/function.inl>
#include <ang/collections/inline/collections.inl>
#include <ang/collections/inline/array.inl>
#include <ang/collections/inline/vector.inl>
#include <ang/core/inline/listener.inl>
#endif//__cplusplus

#ifdef MICRO8_PLATFORM

#include <stdio.h>

#include <ang/c18/digital_ports.h>
#include <ang/c18/interrupt.h>
#include <ang/c18/adc.h>
#include <ang/c18/timer0.h>
#include <ang/c18/timer1.h>
#include <ang/c18/timer2.h>
#include <ang/c18/delays.h>
#include <ang/c18/lcd.h>
#include <ang/c18/keyboard.h>

#endif//MICRO8_PLATFORM

#endif //__ANGSYS_H__