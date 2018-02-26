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

namespace ang
{
	class object;
	typedef ang::object_wrapper<object> object_t, objptr;
	typedef ang::object_wrapper_ptr<object> object_ptr_t;

	namespace strings
	{
		using namespace text;

		template<encoding ENCODING> class basic_string_buffer;
		template<encoding ENCODING> using basic_string = object_wrapper<basic_string_buffer<ENCODING>>;
		typedef basic_string_buffer<encoding::ascii> ascii_string_buffer, string_buffer;
		typedef basic_string_buffer<encoding::unicode> unicode_string_buffer, wstring_buffer;
		typedef basic_string_buffer<encoding::utf8> utf8_string_buffer, mstring_buffer;

		typedef object_wrapper<ascii_string_buffer> ascii_string, string;
		typedef object_wrapper<unicode_string_buffer> unicode_string, wstring;
		typedef object_wrapper<utf8_string_buffer> utf8_string, mstring;
	}

	using strings::string;
	using strings::wstring;
	using strings::mstring;
}

#include <ang/smart_ptr.h>
#include <ang/object.h>
#include <ang/buffers.h>
#include <ang/string.h>


#endif //__ANGSYS_H__