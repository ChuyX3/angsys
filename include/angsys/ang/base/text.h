/*********************************************************************************************************************/
/*   File Name: ang/base/text.h                                                                                     */
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
	struct endian_proxy { enum type : word; };
	typedef enum endian_proxy::type : word {
		little = 0,
		big = 1,
#ifdef _WIN32
		native = endian_proxy::little
#else 
		native = endian_proxy::little
#endif
	}endian;

	constexpr bool LITTLE_ENDIAN_PLATFORM = yes_expression<endian::little == endian::native>::value;

	namespace text
	{
		struct LINK encoding_proxy{ enum type : word; protected: type _value; };
		typedef enum encoding_proxy::type : word {
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
			auto_detect = 0XFFFF
		} encoding;

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
		template<> struct char_type_by_encoding<encoding::ascii> {
			typedef char char_t;
			typedef char* str_t;
			typedef const char* cstr_t;
		};
		template<> struct char_type_by_encoding<encoding::unicode> {
			typedef wchar_t char_t;
			typedef wchar_t* str_t;
			typedef const wchar_t* cstr_t;
		};
		template<> struct char_type_by_encoding<encoding::utf8> {
			typedef mchar char_t;
			typedef mchar* str_t;
			typedef const mchar* cstr_t;
		};
		template<> struct char_type_by_encoding<encoding::utf16> {
			typedef char16_t char_t;
			typedef char16_t* str_t;
			typedef const char16_t* cstr_t;
		};
		template<> struct char_type_by_encoding<encoding::utf16_se> : char_type_by_encoding<encoding::utf16> { };
		template<> struct char_type_by_encoding<encoding::utf16_le> : char_type_by_encoding<encoding::utf16> { };
		template<> struct char_type_by_encoding<encoding::utf16_be> : char_type_by_encoding<encoding::utf16> { };
		template<> struct char_type_by_encoding<encoding::utf32> {
			typedef char32_t char_t;
			typedef char32_t* str_t;
			typedef const char32_t* cstr_t;
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
	}

	template<typename T, text::encoding ENCODING_ = text::encoding_by_char_type<T>::value> struct str_view;
	template<typename T, text::encoding ENCODING_ = text::encoding_by_char_type<T>::value> using cstr_view = str_view<const T, ENCODING_>;

	namespace text
	{
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

		template<encoding ENCODING> struct is_endian_swapped : false_type {};
		template<> struct is_endian_swapped<encoding::utf16_se> : true_type {};
		template<> struct is_endian_swapped<encoding::utf16_be> : yes_expression<LITTLE_ENDIAN_PLATFORM> {};
		template<> struct is_endian_swapped<encoding::utf16_le> : not_expression<LITTLE_ENDIAN_PLATFORM> {};
		template<> struct is_endian_swapped<encoding::utf32_se> : true_type {};
		template<> struct is_endian_swapped<encoding::utf32_be> : yes_expression<LITTLE_ENDIAN_PLATFORM> {};
		template<> struct is_endian_swapped<encoding::utf32_le> : not_expression<LITTLE_ENDIAN_PLATFORM> {};

		template<encoding ENCODING_>
		struct encoder
		{
			static constexpr text::encoding ENCODING = ENCODING_;

			typedef typename char_type_by_encoding<ENCODING>::char_t char_t;
			typedef typename char_type_by_encoding<ENCODING>::str_t raw_str_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t raw_cstr_t;
			typedef str_view<char_t, ENCODING> str_t;
			typedef str_view<const char_t, ENCODING> cstr_t;

			static wsize lenght(raw_cstr_t cstr);//base character count
			static wsize count(raw_cstr_t cstr);//character count
			template <typename cstr2_t> static wsize size(cstr2_t cstr);//character needed for convertion
			template<typename cstr2_t> static int compare(raw_cstr_t first, cstr2_t second);
			template<typename cstr2_t> static windex compare_until(raw_cstr_t first, cstr2_t second);
			template<typename cstr2_t> static raw_str_t convert(raw_str_t str, cstr2_t cstr, bool eos = true, wsize max = -1);
			template<typename cstr2_t> static raw_str_t convert(raw_str_t str, wsize& i, cstr2_t cstr, wsize& j, bool eos = true, wsize max = -1);
		};

		typedef encoder<encoding::ascii>	ascii_t,	ascii;
		typedef encoder<encoding::unicode>	unicode_t,	unicode;
		typedef encoder<encoding::utf8>		utf8_t,		utf8;
		typedef encoder<encoding::utf16>	utf16_t,	utf16;
		typedef encoder<encoding::utf16_se> utf16_se_t, utf16_se;
		typedef encoder<encoding::utf16_le> utf16_le_t, utf16_le;
		typedef encoder<encoding::utf16_be> utf16_be_t, utf16_be;
		typedef encoder<encoding::utf32>	utf32_t,	utf32;
		typedef encoder<encoding::utf32_se> utf32_se_t, utf32_se;
		typedef encoder<encoding::utf32_le> utf32_le_t, utf32_le;
		typedef encoder<encoding::utf32_be> utf32_be_t, utf32_be;
	}


	template<typename T, text::encoding ENCODING_>
	struct str_view {
		static_assert(is_same_type<
			typename text::char_type_by_type<T>::char_t,
			typename text::char_type_by_encoding<ENCODING_>::char_t
		>::value, "T is not a valid type");

		static constexpr text::encoding ENCODING = ENCODING_;
		typedef str_view<T, ENCODING_> sefl_t;
		typedef typename text::char_type_by_encoding<ENCODING>::char_t char_t;
		typedef typename text::char_type_by_encoding<ENCODING>::str_t str_t;
		typedef typename text::char_type_by_encoding<ENCODING>::cstr_t cstr_t;


		str_view() : size_(0), view_(nullptr) {
		}
		str_view(str_t str, wsize sz = -1) : size_(0), view_(nullptr) {
			set(str, sz);
		}
		template<wsize SIZE> str_view(char_t(&str)[SIZE]) : size_(0), view_(nullptr) {
			set(str, SIZE - 1);
		}
		wsize size()const {
			return size_;
		};
		str_t str() {
			return view_;
		};
		cstr_t cstr()const {
			return view_;
		};
		void set(str_t str, wsize sz = -1) {
			if (str) {
				view_ = str;
				size_ = (sz == wsize(-1)) ? size_ = text::encoder<ENCODING>::lenght(str) : sz;
			}
			else {
				view_ = nullptr;
				size_ = 0;
			}
		}
		sefl_t& operator = (sefl_t const& str) {
			set(str.view_, str.size_);
			return*this;
		}
		sefl_t& operator = (str_t str) {
			set(str);
			return*this;
		}
		template<wsize SIZE> sefl_t& operator = (char_t(&str)[SIZE]) {
			set(str, SIZE - 1);
			return*this;
		}
		char_t& operator*() {
			return *str();
		}
		char_t const& operator*()const {
			return *cstr();
		}
		operator str_t () {
			return str();
		}
		operator cstr_t () {
			return cstr();
		}
		template<typename index_t> char_t& operator[](index_t const& i) {
			return str()[i];
		}
		template<typename index_t> char_t const& operator[](index_t const& i)const {
			return cstr()[i];
		}
		template<typename index_t> sefl_t& operator += (index_t i) {
			set(str() + (wsize)i, size() - (wsize)i);
			return this;
		}
		sefl_t& operator ++ () {
			set(str() + 1, size() - 1);
			return this;
		}
		sefl_t operator ++ (int) {
			sefl_t ret = *this;
			set(str() + 1, size() - 1);
			return std::move(ret);
		}
		friend sefl_t operator + (sefl_t const& str, wsize i) {
			return sefl_t(str.view_ + i, str.size() - i);
		}

	private:
		wsize size_;
		str_t view_;
	};

	template<typename T, text::encoding ENCODING_>
	struct str_view<const T, ENCODING_> {
		static constexpr text::encoding ENCODING = ENCODING_;
		typedef str_view<const T, ENCODING_> sefl_t;
		typedef typename text::char_type_by_encoding<ENCODING>::char_t char_t;
		typedef typename text::char_type_by_encoding<ENCODING>::cstr_t cstr_t;

		str_view() : size_(0), view_(nullptr) {
		}
		str_view(cstr_t str, wsize sz = -1) : size_(0), view_(nullptr) {
			set(str, sz);
		}
		template<wsize SIZE> str_view(const char_t(&str)[SIZE]) : size_(0), view_(nullptr) {
			set(str, SIZE - 1);
		}
		wsize size()const {
			return size_;
		};
		cstr_t cstr()const {
			return view_;
		};
		void set(cstr_t str, wsize sz = -1) {
			if (str) {
				view_ = str;
				size_ = (sz == wsize(-1)) ? size_ = text::encoder<ENCODING>::lenght(str) : sz;
			}
			else {
				view_ = nullptr;
				size_ = 0;
			}
		}
		sefl_t& operator = (sefl_t const& str) {
			set(str.view_, str.size_);
			return*this;
		}
		sefl_t& operator = (cstr_t str) {
			set(str);
			return*this;
		}
		template<wsize SIZE> sefl_t& operator = (const char_t(&str)[SIZE]) {
			set(str, SIZE - 1);
			return*this;
		}
		char_t const& operator*()const {
			return *cstr();
		}
		operator cstr_t () {
			return cstr();
		}
		template<typename index_t> char_t const& operator[](index_t const& i)const {
			return cstr()[i];
		}
		template<typename index_t> sefl_t& operator += (index_t i) {
			set(cstr() + (wsize)i, size() - (wsize)i);
			return this;
		}
		sefl_t& operator ++ () {
			set(cstr() + 1, size() - 1);
			return this;
		}
		sefl_t operator ++ (int) {
			sefl_t ret = *this;
			set(cstr() + 1, size() - 1);
			return std::move(ret);
		}
		friend sefl_t operator + (sefl_t const& str, wsize i) {
			return sefl_t(str.cstr() + (wsize)i, str.size() - (wsize)i);
		}

	private:
		wsize size_;
		cstr_t view_;
	};
}

#include <ang/base/inlines/text.inl>

#endif//__ANG_BASE_TEXT_H__