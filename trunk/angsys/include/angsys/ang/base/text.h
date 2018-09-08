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

		template<encoding E>
		struct native_encoding : integer_constant<encoding, E> { };

		template<> 
		struct native_encoding<encoding::utf16>
			: value_selector<encoding, encoding::utf16_le, encoding::utf16_be, LITTLE_ENDIAN_PLATFORM> {
		};

		template<> 
		struct native_encoding<encoding::utf32>
			: value_selector<encoding, encoding::utf32_le, encoding::utf32_be, LITTLE_ENDIAN_PLATFORM > {
		};

		template<>
		struct native_encoding<encoding::unicode>
			: value_selector<encoding, native_encoding<encoding::utf16>::value, native_encoding<encoding::utf32>::value, sizeof(wchar) == 2>{
		};

		template<encoding E>
		struct native_inverse_encoding : integer_constant<encoding, E> {
		};

		template<>
		struct native_inverse_encoding<encoding::utf16>
			: value_selector<encoding, encoding::utf16_be, encoding::utf16_le, LITTLE_ENDIAN_PLATFORM > { 
		};

		template<>
		struct native_inverse_encoding<encoding::utf32>
			: value_selector<encoding, encoding::utf32_be, encoding::utf32_le, LITTLE_ENDIAN_PLATFORM >{ 
		};

		template<>
		struct native_inverse_encoding<encoding::unicode>
			: value_selector<encoding, native_inverse_encoding<encoding::utf16>::value, native_inverse_encoding<encoding::utf32>::value, sizeof(wchar) == 2> {
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
			template <typename cstr2_t> static wsize size(cstr2_t cstr, windex start = 0, windex end = -1);//character needed for convertion
			template<typename cstr2_t> static int compare(raw_cstr_t first, cstr2_t second);
			template<typename cstr2_t> static windex compare_until(raw_cstr_t first, cstr2_t second);
			template<typename cstr2_t> static windex find(raw_cstr_t first, wsize sz1, cstr2_t second, wsize sz2, windex start);
			template<typename cstr2_t> static windex find_reverse(raw_cstr_t first, wsize sz1, cstr2_t second, wsize sz2, windex start);
			template<typename cstr2_t> static raw_str_t convert(raw_str_t str, cstr2_t cstr, bool eos = true, wsize max_out = -1, wsize max_in = -1);
			template<typename cstr2_t> static raw_str_t convert(raw_str_t str, wsize& i, cstr2_t cstr, wsize& j, bool eos = true, wsize max_out = -1, wsize max_in = -1);
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


		template<> struct encoder<encoding::auto_detect> {

			inline static wsize char_size_by_encoding(text::encoding encoding) {
				switch (encoding)
				{
				case text::encoding::unicode:
					return sizeof(wchar_t);
				case text::encoding::utf16:
				case text::encoding::utf16_be:
				case text::encoding::utf16_le:
				case text::encoding::utf16_se:
					return sizeof(char16_t);
				case text::encoding::utf32:
				case text::encoding::utf32_be:
				case text::encoding::utf32_le:
				case text::encoding::utf32_se:
					return sizeof(char32_t);
				default:
					//case text::encoding::ascii:
					//case text::encoding::utf8:
					return sizeof(char);
				}
			}

			template<typename T>
			inline static wsize length(T cstr) {
				return encoder<encoding_by_char_type<T>::value>::length((typename char_type_by_type<T>::cstr_t)cstr);
			}

			template<typename T1, typename T2>
			inline static wsize size(T2 cstr) {
				return encoder<encoding_by_char_type<T1>::value>::size((typename char_type_by_type<T2>::cstr_t)cstr);
			}

			template<typename T1, typename T2>
			inline static int compare(T1 cstr1, T2 cstr2) {
				return encoder<encoding_by_char_type<T1>::value>::compare((typename char_type_by_type<T1>::cstr_t)cstr1, (typename char_type_by_type<T2>::cstr_t)cstr2);
			}

			template<typename T1, typename T2>
			inline static wsize compare_until(T1 cstr1, T2 cstr2) {
				return encoder<encoding_by_char_type<T1>::value>::compare_until((typename char_type_by_type<T1>::cstr_t)cstr1, (typename char_type_by_type<T2>::cstr_t)cstr2);
			}

			template<typename T1, typename T2>
			inline static wsize convert(T1 dest, wsize maxsize, T2 src, bool end_of_string = true) {
				return encoder<encoding_by_char_type<T1>::value>::convert((typename char_type_by_type<T1>::str_t)dest, maxsize, (typename char_type_by_type<T2>::cstr_t)src, end_of_string);
			}
		};
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
		sefl_t begin()const {
			return sefl_t(view_, size_);
		}
		sefl_t end()const {
			return sefl_t(view_ + size_, 0);
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
		char_t** operator & () { 
			return &view_;
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
			return ang::move(ret);
		}
		friend sefl_t operator + (sefl_t const& str, wsize i) {
			return sefl_t(str.view_ + i, str.size() - i);
		}

	private:
		str_t view_;
		wsize size_;
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
		sefl_t begin()const {
			return sefl_t(view_, size_);
		}
		sefl_t end()const {
			return sefl_t(view_ + size_, 0);
		}
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
		char_t const** operator & () { 
			return &view_;
		}
		operator cstr_t () {
			return cstr();
		}
		template<typename index_t> char_t const& operator[](index_t const& i)const {
			return cstr()[i];
		}
		template<typename index_t> sefl_t& operator += (index_t i) {
			set(cstr() + (wsize)i, size() - (wsize)i);
			return *this;
		}
		sefl_t& operator ++ () {
			set(cstr() + 1, size() - 1);
			return *this;
		}
		sefl_t operator ++ (int) {
			sefl_t ret = *this;
			set(cstr() + 1, size() - 1);
			return ang::move(ret);
		}
		friend sefl_t operator + (sefl_t const& str, wsize i) {
			return sefl_t(str.cstr() + (wsize)i, str.size() - (wsize)i);
		}

	private:
		cstr_t view_;
		wsize size_;
	};


	template<typename T1, text::encoding E1, typename T2, text::encoding E2>
	bool operator == (const str_view<T1, E1>& value1, const str_view<T2, E2>& value2) {
		return text::encoder<E1>::template compare<T2 const*>(value1.cstr(), value2.cstr()) == 0; 
	}

	template<typename T1, text::encoding E1, typename T2, text::encoding E2>
	bool operator != (const str_view<T1, E1>& value1, const str_view<T2, E2>& value2) {
		return text::encoder<E1>::template compare<T2 const*>(value1.cstr(), value2.cstr()) != 0;
	}

	template<typename T1, text::encoding E1, typename T2, text::encoding E2>
	bool operator >= (const str_view<T1, E1>& value1, const str_view<T2, E2>& value2) {
		return text::encoder<E1>::template compare<T2 const*>(value1.cstr(), value2.cstr()) >= 0;
	}

	template<typename T1, text::encoding E1, typename T2, text::encoding E2>
	bool operator <= (const str_view<T1, E1>& value1, const str_view<T2, E2>& value2) {
		return text::encoder<E1>::template compare<T2 const*>(value1.cstr(), value2.cstr()) <= 0;
	}

	template<typename T1, text::encoding E1, typename T2, text::encoding E2>
	bool operator > (const str_view<T1, E1>& value1, const str_view<T2, E2>& value2) {
		return text::encoder<E1>::template compare<T2 const*>(value1.cstr(), value2.cstr()) > 0;
	}

	template<typename T1, text::encoding E1, typename T2, text::encoding E2>
	bool operator < (const str_view<T1, E1>& value1, const str_view<T2, E2>& value2) {
		return text::encoder<E1>::template compare<T2 const*>(value1.cstr(), value2.cstr()) < 0;
	}


	inline str_view<const char> operator "" _s(const char* str, wsize sz) { return str_view<const char>(str, sz); }
	inline str_view<const mchar> operator "" _sm(const char* str, wsize sz) { return str_view<const mchar>((mchar const*)str, sz); }
	inline str_view<const wchar_t> operator "" _s(const wchar_t* str, wsize sz) { return str_view<const wchar_t>(str, sz); }
	inline str_view<const char16_t> operator "" _s(const char16_t* str, wsize sz) { return str_view<const char16_t>(str, sz); }
	inline str_view<const char32_t> operator "" _s(const char32_t* str, wsize sz) { return str_view<const char32_t>(str, sz); }

	typedef str_view<char> str_t;
	typedef str_view<mchar> mstr_t;
	typedef str_view<wchar> wstr_t;
	typedef str_view<char16_t> str16_t;
	typedef str_view<char32_t> str32_t;

	typedef str_view<const char> cstr_t;
	typedef str_view<const mchar> cmstr_t;
	typedef str_view<const wchar> cwstr_t;
	typedef str_view<const char16_t> cstr16_t;
	typedef str_view<const char32_t> cstr32_t;


	namespace text
	{
		////text_format class////
		// {b:[u|U]|[l|L]|[c|C],[t|T]}
		//	u = uppercase (TRUE/FALSE)
		//	l = lowercase (true/false)
		//	c = camelcase (True/False)
		//  t = (type) 0 == [true/false](default), 1 == [yes/no], 2 == [hight/low]
		// {c:[u|U]|[l|L][N|n]}
		//	u = uppercase
		//	l = lowercase
		//  n = default, optional
		// {s:[u|U]|[l|L]|[c|C]}
		//	u = uppercase (ABCD)
		//	l = lowercase (abcd)
		//	c = camelcase (Abcd)
		//  n = default, optional
		// {i:[n|N]xx,[t|T]c,(F|f)c,[[x|X]|[b|B]],[S|s]}
		//	N = Max Number of digits (xx = number)
		//	T = Thousand separator character (c = character)
		//	F = Fill character (c)
		//	X = Uppercase Hexadecimal format
		//	x = Lowercase Hexadecimal format
		//	B = Binary
		//	S = +|-
		// {u:[n|N]xx,[t|T]c,(F|f)c,[[x|X]|[b|B]]}
		//	N = Max Number of digits (xx = number)
		//	T = Thousand separator character (c = character)
		//	X = Hexadecimal format
		//	F = Fill character (c)
		//	B = Binary
		// {f:[n|N]xx,[t|T]c,(F|f)c,[[x|X]|[b|B]],[E],[S|s]}
		//	N = Max Number of decimals (xx = number)
		//	T = Thousand separator character (c = character)
		//	X = Hexadecimal format
		//	F = Fill character (c)
		//	B = Binary
		//	S = +|-
		//	E = Cientific Notation
		class LINK text_format
		{
		public:
			enum target : byte
			{
				none = 0,
				bool_,
				char_,
				text_,
				signed_,
				unsigned_,
				float_,
			};

			struct text_format_flags
			{
				union
				{
					qword value;
					struct
					{
						ang::text::text_format::target target;
						union
						{
							byte flags;
							union
							{
								struct
								{
									bool lower_case : 1;
									bool upper_case : 1;
									byte type : 2; //(boolean) : 0 - [true/false], 1 - [yes/no], 2 - [hight/low]
								};
							};
							struct
							{
								byte case_ : 2; // 1 - lower case, 2 - upper case, 3 - camel case
								byte base : 2; //(number) : 1-x, 2-X, 3-b
								bool sign : 1;
								bool exponent : 1;
							};
						};
						char fill;
						union
						{
							byte max_pres;
							struct {
								byte max : 4;
								byte pres : 4;
							};
						};
						char thousand;
					};
				};
				text_format_flags() {
					value = 0;
				}
				text_format_flags(qword val) {
					value = val;
				}
				text_format_flags(ang::text::text_format::target t) {
					value = 0;
					target = t;
					switch (t)
					{
					case bool_:
						case_ = 3;
						break;
					case float_:
						pres = 4;
						break;
					}
				}
			};
		public:
			text_format();//default format-> bad format
			text_format(cstr_t format);
			text_format(const text_format&);
			text_format(const text_format_flags&);
			virtual~text_format();

		public:
			target format_target()const;
			void format(cstr_t format);
			//string format()const;
			qword format_flags()const;
			text_format& operator = (const text_format&);

		protected:
			text_format_flags flags;

		};

		typedef text_format text_format_t;
		typedef text_format::text_format_flags text_format_flags, text_format_flags_t;


		template<typename T>
		struct default_text_format {
			static text_format format() {
				return text_format();
			}
		};


		template<typename T>
		struct default_text_format<const T> : default_text_format<T> { };

		template<typename T>
		struct default_text_format<T&> : default_text_format<T> { };

		template<typename T>
		struct default_text_format<const T&> : default_text_format<T> { };

		template<> struct default_text_format<bool> {
			static text_format format() { text_format _format = cstr_t("{b:}"); return _format; }
		};

		template<> struct default_text_format<char> {
			static text_format format() { text_format _format = cstr_t("{c:}"); return _format; }
		};

		template<> struct default_text_format<mchar> :public default_text_format<char> {};
		template<> struct default_text_format<wchar> :public default_text_format<char> {};
		template<> struct default_text_format<char16_t> :public default_text_format<char> {};
		template<> struct default_text_format<char32_t> :public default_text_format<char> {};

		template<> struct default_text_format<int> {
			static text_format format() { text_format _format = cstr_t("{s:}"); return _format; }
		};

		template<> struct default_text_format<short> :public default_text_format<int> {};
		template<> struct default_text_format<long> :public default_text_format<int> {};
		template<> struct default_text_format<long64> :public default_text_format<int> {};

		template<> struct default_text_format<uint> {
			static text_format format() { text_format _format = cstr_t("{u:}"); return _format; }
		};

		template<> struct default_text_format<ushort> :public default_text_format<uint> {};
		template<> struct default_text_format<ulong> :public default_text_format<uint> {};
		template<> struct default_text_format<ulong64> :public default_text_format<uint> {};

		template<> struct default_text_format<float> {
			static text_format format() { text_format _format = cstr_t("{f:N.4}"); return _format; }
		};

		template<> struct default_text_format<double> :public default_text_format<float> {};



		template<text::encoding ENCODING> wsize load_bom(pointer ptr) { return 0; }

		template<> LINK wsize load_bom<text::encoding::utf8>(pointer ptr);

		template<> LINK wsize load_bom<text::encoding::utf16_le>(pointer ptr);

		template<> LINK wsize load_bom<text::encoding::utf16_be>(pointer ptr);

		template<> LINK wsize load_bom<text::encoding::utf32_le>(pointer ptr);

		template<> LINK wsize load_bom<text::encoding::utf32_be>(pointer ptr);

		template<> LINK wsize load_bom<text::encoding::utf16>(pointer ptr);

		template<> LINK wsize load_bom<text::encoding::utf16_se>(pointer ptr);

		template<> LINK wsize load_bom<text::encoding::utf32>(pointer ptr);

		template<> LINK wsize load_bom<text::encoding::utf32_se>(pointer ptr);

		template<> LINK wsize load_bom<text::encoding::unicode>(pointer ptr);
	}


	template<typename T, T VALUE>
	struct to_string
	{
		static const str_view<const char> value;
	};

#define TO_STRING_TEMPLATE(_LINK, _ENUM, _VALUE) namespace ang { template<> struct _LINK to_string<_ENUM, _ENUM::_VALUE>	{ static const str_view<const char> value; }; }
#define TO_STRING_TEMPLATE_IMPLEMENT(_ENUM, _VALUE) const ang::str_view<const char>  ang::to_string<_ENUM, _ENUM::_VALUE>::value = ANG_UTILS_TO_STRING_OBJ(_VALUE);

}

#include <ang/base/inline/text.inl>

TO_STRING_TEMPLATE(LINK, ang::text::encoding, ascii);
TO_STRING_TEMPLATE(LINK, ang::text::encoding, unicode);
TO_STRING_TEMPLATE(LINK, ang::text::encoding, utf8);
TO_STRING_TEMPLATE(LINK, ang::text::encoding, utf16);
TO_STRING_TEMPLATE(LINK, ang::text::encoding, utf16_se);
TO_STRING_TEMPLATE(LINK, ang::text::encoding, utf16_le);
TO_STRING_TEMPLATE(LINK, ang::text::encoding, utf16_be);
TO_STRING_TEMPLATE(LINK, ang::text::encoding, utf32);
TO_STRING_TEMPLATE(LINK, ang::text::encoding, utf32_se);
TO_STRING_TEMPLATE(LINK, ang::text::encoding, utf32_le);
TO_STRING_TEMPLATE(LINK, ang::text::encoding, utf32_be);

#endif//__ANG_BASE_TEXT_H__
