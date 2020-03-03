/*********************************************************************************************************************/
/*   File Name: ang/base/text.h                                                                                      */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_TEXT_H__
#define __ANG_BASE_TEXT_H__

namespace ang //constants
{
	typedef enum class endian : word {
		little = 0,
		big = 1,
#ifdef _WIN32
		native = little
#elif defined ANDROID_PLATFORM
		native = big
#else 
		native = little
#endif
	}endian;

#ifdef _WIN32
#define LITTLE_ENDIAN_PLATFORM (1)
#elif defined ANDROID_PLATFORM
#define BIG_ENDIAN_PLATFORM (1)
#else 
#define LITTLE_ENDIAN_PLATFORM (1)
#endif
	constexpr bool IS_LITTLE_ENDIAN_PLATFORM = yes_expression<endian::little == endian::native>::value;

	namespace text
	{
		enum class encoding : word {
			none = 0,
			binary = 0,
			ascii,
			unicode,
			utf8,
			utf16,
			utf16_se,
			utf16_le,
			utf16_be,
			utf32,
			utf32_se,
			utf32_le,
			utf32_be,
			encoding_count,
			auto_detect = 0XFFFF
		};

		template<encoding ENCODING> struct is_endian_swapped : false_type {};
		template<> struct is_endian_swapped<encoding::utf16_se> : true_type {};
		template<> struct is_endian_swapped<encoding::utf16_be> : yes_expression<IS_LITTLE_ENDIAN_PLATFORM> {};
		template<> struct is_endian_swapped<encoding::utf16_le> : not_expression<IS_LITTLE_ENDIAN_PLATFORM> {};
		template<> struct is_endian_swapped<encoding::utf32_se> : true_type {};
		template<> struct is_endian_swapped<encoding::utf32_be> : yes_expression<IS_LITTLE_ENDIAN_PLATFORM> {};
		template<> struct is_endian_swapped<encoding::utf32_le> : not_expression<IS_LITTLE_ENDIAN_PLATFORM> {};

		template<encoding E>
		struct native_encoding : integer_constant<encoding, E> { };

		template<>
		struct native_encoding<encoding::utf16>
			: value_selector<encoding, encoding::utf16_le, encoding::utf16_be, IS_LITTLE_ENDIAN_PLATFORM> {
		};

		template<>
		struct native_encoding<encoding::utf16_se>
			: value_selector<encoding, encoding::utf16_be, encoding::utf16_le, IS_LITTLE_ENDIAN_PLATFORM> {
		};

		template<>
		struct native_encoding<encoding::utf32>
			: value_selector<encoding, encoding::utf32_le, encoding::utf32_be, IS_LITTLE_ENDIAN_PLATFORM > {
		};

		template<>
		struct native_encoding<encoding::utf32_se>
			: value_selector<encoding, encoding::utf32_be, encoding::utf32_le, IS_LITTLE_ENDIAN_PLATFORM > {
		};

		template<>
		struct native_encoding<encoding::unicode>
			: value_selector<encoding, native_encoding<encoding::utf16>::value, native_encoding<encoding::utf32>::value, sizeof(wchar) == 2> {
		};

		template<encoding E>
		struct native_inverse_encoding : integer_constant<encoding, E> {
		};

		template<>
		struct native_inverse_encoding<encoding::utf16>
			: value_selector<encoding, encoding::utf16_be, encoding::utf16_le, IS_LITTLE_ENDIAN_PLATFORM > {
		};

		template<>
		struct native_inverse_encoding<encoding::utf16_se>
			: value_selector<encoding, encoding::utf16_le, encoding::utf16_be, IS_LITTLE_ENDIAN_PLATFORM > {
		};

		template<>
		struct native_inverse_encoding<encoding::utf32>
			: value_selector<encoding, encoding::utf32_be, encoding::utf32_le, IS_LITTLE_ENDIAN_PLATFORM > {
		};

		template<>
		struct native_inverse_encoding<encoding::utf32_se>
			: value_selector<encoding, encoding::utf32_le, encoding::utf32_be, IS_LITTLE_ENDIAN_PLATFORM > {
		};

		template<>
		struct native_inverse_encoding<encoding::unicode>
			: value_selector<encoding, native_inverse_encoding<encoding::utf16>::value, native_inverse_encoding<encoding::utf32>::value, size_of<wchar>() == size_of<short>()> {
		};


		template<typename T> struct is_char_type : false_type { };

		template<> struct is_char_type<char> : true_type { };
		template<> struct is_char_type<unsigned char> : true_type { };
		template<> struct is_char_type<wchar_t> : true_type { };
		template<> struct is_char_type<char16_t> : true_type { };
		template<> struct is_char_type<char32_t> : true_type { };

		template <typename T> struct is_char_type<const T> : is_char_type<T> {};
		template <typename T> struct is_char_type<T&> : is_char_type<T> {};
		template <typename T> struct is_char_type<T&&> : is_char_type<T> {};
		template <typename T> struct is_char_type<const T&> : is_char_type<T> {};

		template<encoding ENCODING> struct char_type_by_encoding;
		template<> struct char_type_by_encoding<encoding::auto_detect> {
			typedef void char_type;
			typedef void* str_type;
			typedef const void* cstr_type;
		};
		template<> struct char_type_by_encoding<encoding::ascii> {
			typedef char char_type;
			typedef char* str_type;
			typedef const char* cstr_type;
		};
		template<> struct char_type_by_encoding<encoding::unicode> {
			typedef wchar_t char_type;
			typedef wchar_t* str_type;
			typedef const wchar_t* cstr_type;
		};
		template<> struct char_type_by_encoding<encoding::utf8> {
			typedef mchar char_type;
			typedef mchar* str_type;
			typedef const mchar* cstr_type;
		};
		template<> struct char_type_by_encoding<encoding::utf16> {
			typedef char16_t char_type;
			typedef char16_t* str_type;
			typedef const char16_t* cstr_type;
		};
		template<> struct char_type_by_encoding<encoding::utf16_se> : char_type_by_encoding<encoding::utf16> { };
		template<> struct char_type_by_encoding<encoding::utf16_le> : char_type_by_encoding<encoding::utf16> { };
		template<> struct char_type_by_encoding<encoding::utf16_be> : char_type_by_encoding<encoding::utf16> { };
		template<> struct char_type_by_encoding<encoding::utf32> {
			typedef char32_t char_type;
			typedef char32_t* str_type;
			typedef const char32_t* cstr_type;
		};
		template<> struct char_type_by_encoding<encoding::utf32_se> : char_type_by_encoding<encoding::utf32> { };
		template<> struct char_type_by_encoding<encoding::utf32_le> : char_type_by_encoding<encoding::utf32> { };
		template<> struct char_type_by_encoding<encoding::utf32_be> : char_type_by_encoding<encoding::utf32> { };

		template<wsize SIZE> struct encoding_by_size;
		template<> struct encoding_by_size<1u> : char_type_by_encoding<encoding::utf8> {
			static constexpr encoding value = encoding::utf8;
		};
		template<> struct encoding_by_size<2u> : char_type_by_encoding<encoding::utf16> {
			static constexpr encoding value = encoding::utf16;
		};
		template<> struct encoding_by_size<4u> : char_type_by_encoding<encoding::utf32> {
			static constexpr encoding value = encoding::utf32;
		};

		template<typename T> struct encoding_by_char_type;
		template<> struct encoding_by_char_type<void> : char_type_by_encoding<encoding::auto_detect> {
			static constexpr encoding value = encoding::auto_detect;
		};
		template<> struct encoding_by_char_type<char> : char_type_by_encoding<encoding::ascii> {
			static constexpr encoding value = encoding::ascii;
		};
		template<> struct encoding_by_char_type<mchar> : char_type_by_encoding<encoding::utf8> {
			static constexpr encoding value = encoding::utf8;
		};
		template<> struct encoding_by_char_type<wchar> : char_type_by_encoding<encoding::unicode> {
			static constexpr encoding value = encoding::unicode;
		};
		template<> struct encoding_by_char_type<char16_t> : char_type_by_encoding<encoding::utf16> {
			static constexpr encoding value = encoding::utf16;
		};
		template<> struct encoding_by_char_type<char32_t> : char_type_by_encoding<encoding::utf32> {
			static constexpr encoding value = encoding::utf32;
		};
		template<typename T> struct encoding_by_char_type<const T> : encoding_by_char_type<T> { };
		template<typename T> struct encoding_by_char_type<T&> : encoding_by_char_type<T> { };
		template<typename T> struct encoding_by_char_type<T&&> : encoding_by_char_type<T> { };
		template<typename T> struct encoding_by_char_type<T const&> : encoding_by_char_type<T> { };
		template<typename T> struct encoding_by_char_type<T*> : encoding_by_char_type<T> { };
		template<typename T> struct encoding_by_char_type<T const*> : encoding_by_char_type<T> { };

		template<typename T> struct char_type_by_type;
		template<> struct char_type_by_type<char> : char_type_by_encoding<encoding::ascii> {
			static constexpr text::encoding encoding = text::encoding::ascii;
		};
		template<> struct char_type_by_type<wchar> : char_type_by_encoding<encoding::unicode> {
			static constexpr text::encoding encoding = text::encoding::unicode;
		};
		template<> struct char_type_by_type<mchar> : char_type_by_encoding<encoding::utf8> {
			static constexpr text::encoding encoding = text::encoding::utf8;
		};
		template<> struct char_type_by_type<char16_t> : char_type_by_encoding<encoding::utf16> {
			static constexpr text::encoding encoding = text::encoding::utf16;
		};
		template<> struct char_type_by_type<char32_t> : char_type_by_encoding<encoding::utf32> {
			static constexpr text::encoding encoding = text::encoding::utf32;
		};

		

		template<typename T> struct char_type_by_type<const T> : char_type_by_type<T> { };
		template<typename T> struct char_type_by_type<T&> : char_type_by_type<T> { };
		template<typename T> struct char_type_by_type<T&&> : char_type_by_type<T> { };
		template<typename T> struct char_type_by_type<T const&> : char_type_by_type<T> { };
		template<typename T> struct char_type_by_type<T*> : char_type_by_type<T> { };
		template<typename T> struct char_type_by_type<T const*> : char_type_by_type<T> { };
	
		template<typename T, encoding E = encoding_by_char_type<T>::value> struct str_view;
		template<typename T, encoding E = encoding_by_char_type<T>::value> using cstr_view = str_view<const T, E>;

		template<encoding E, template<typename> class A = memory::buffer_allocator> class fast_string;

		template<typename T, encoding E> struct encoding_by_char_type<str_view<T, E>> : char_type_by_encoding<E> {
			static constexpr encoding value = E;
		};
		template<typename T, encoding E> struct encoding_by_char_type<str_view<const T, E>> : char_type_by_encoding<E> {
			static constexpr encoding value = E;
		};
		template<typename T, encoding E> struct char_type_by_type<str_view<T, E>> : char_type_by_encoding<E> {
			static constexpr text::encoding encoding = E;
		};
		template<typename T, encoding E> struct char_type_by_type<str_view<const T, E>> : char_type_by_encoding<E> {
			static constexpr text::encoding encoding = E;
		};
	}

	using text::str_view;
	using text::cstr_view;
	using text::fast_string;

	typedef str_view<void, text::encoding::auto_detect> raw_str, raw_str_t, str_t;
	typedef str_view<const void, text::encoding::auto_detect> raw_cstr, raw_cstr_t, cstr_t;

	typedef str_view<char> astr_t;
	typedef str_view<mchar> mstr_t;
	typedef str_view<wchar> wstr_t;
	typedef str_view<char16_t> str16_t;
	typedef str_view<char32_t> str32_t;

	typedef str_view<const char> castr_t;
	typedef str_view<const mchar> cmstr_t;
	typedef str_view<const wchar> cwstr_t;
	typedef str_view<const char16_t> cstr16_t;
	typedef str_view<const char32_t> cstr32_t;

	typedef fast_string<text::encoding::ascii> astring, fast_astring_t;
	typedef fast_string<text::encoding::unicode> wstring, fast_wstring_t;
	typedef fast_string<text::encoding::utf8> mstring, fast_mstring_t;
	typedef fast_string<text::encoding::utf16> string16, fast_string16_t;
	typedef fast_string<text::encoding::utf32> string32, fast_string32_t;

}

#endif//__ANG_BASE_TEXT_H__
