/*********************************************************************************************************************/
/*   File Name: ang/strings/string.h                                                                                 */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#elif !defined __ANG_STRING_H__
#define __ANG_STRING_H__


#define MY_LINKAGE LINK
#define MY_ALLOCATOR ang::memory::buffer_allocator

#define MY_ENCODING ang::text::encoding::ascii
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::unicode
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf8
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_le
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_be
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_se
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_le
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_be
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_se
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#undef MY_ALLOCATOR
#undef MY_LINKAGE


namespace ang
{
	namespace text
	{
		template<typename T>
		auto to_string(const object_wrapper<T>& val) {
			return to_string(val.get());
		}

		template<typename T>
		auto to_string(const object_wrapper<T>& val, text::text_format_t const& f) {
			return to_string(val.get(), f);
		}

		template<typename T>
		auto to_string(const intf_wrapper<T>& val) {
			return to_string(val.get());
		}

		template<typename T>
		auto to_string(const intf_wrapper<T>& val, text::text_format_t const& f) {
			return to_string(val.get(), f);
		}
	}

	using text::to_string;

	/*template<typename T, text::encoding E>
	inline intf_wrapper<text::istring>::intf_wrapper(str_view<T, E> const& cstr)
		: m_ptr(null) {
		set(new text::basic_string_buffer<E>(cstr));
	}

	template<text::encoding E, template<typename> class A> 
	inline intf_wrapper<text::istring>::intf_wrapper(text::fast_string<E, A>&& str)
		: m_ptr(null) {
		set(new text::basic_string_buffer<E>(ang::forward<text::fast_string<E, A>>(str)));
	}*/

	inline string& operator << (string& stream, string const& cstr) {
		stream->append(cstr);
		return stream;
	}

	inline string& operator << (string& stream, cstr_t const& cstr) {
		stream->append(cstr);
		return stream;
	}

	template<typename T> inline string& operator << (string& stream, T const& value) {
		stream << to_string(value);
		return stream;
	}

	template<typename T, text::encoding E> inline string& operator << (string& stream, str_view<T, E> const& cstr) {
		stream->append((cstr_t)cstr);
		return stream;
	}


	inline string operator "" _s(const char* str, wsize sz) { return new text::basic_string_buffer<text::encoding::ascii>(str_view<const char>(str, sz)); }
	inline string operator "" _sm(const char* str, wsize sz) { return new text::basic_string_buffer<text::encoding::utf8>(str_view<const mchar>((mchar const*)str, sz)); }
	inline string operator "" _s(const wchar_t* str, wsize sz) { return new text::basic_string_buffer<text::encoding::unicode>(str_view<const wchar_t>(str, sz)); }
	inline string operator "" _s(const char16_t* str, wsize sz) { return new text::basic_string_buffer<text::encoding::utf16>(str_view<const char16_t>(str, sz)); }
	inline string operator "" _s(const char32_t* str, wsize sz) { return new text::basic_string_buffer<text::encoding::utf8>(str_view<const char32_t>(str, sz)); }

}

#endif//__ANG_STRING_H__