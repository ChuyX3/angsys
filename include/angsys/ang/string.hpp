/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_STRINGS_HPP__
#define __ANG_STRINGS_HPP__

namespace ang
{
	namespace text
	{

		////text_format class////
		// {char:[u|U]|[l|L][N|n]}
		//	u = uppercase
		//	l = lowercase
		//  n = default, optional
		// {text:[u|U]|[l|L]}
		//	u = uppercase
		//	l = lowercase
		//  n = default, optional
		// {signed:[n|N]xx,(F|f)c,[[x|X]|[b|B]],[S|s]}
		//	N = Max Number of digits (xx = number),
		//	F = Fill character (c)
		//	X = Uppercase Hexadecimal format
		//	x = Lowercase Hexadecimal format
		//	B = Binary
		//	S = +|-
		// {unsigned:[n|N]xx,(F|f)c,[[x|X]|[b|B]]}
		//	N = Max Number of digits (xx = number),
		//	X = Hexadecimal format
		//	F = Fill character (c)
		//	B = Binary
		// {floating:[n|N]xx,(F|f)c,[[x|X]|[b|B]],[E],[S|s]}
		//	N = Max Number of decimals (xx = number)
		//	X = Hexadecimal format
		//	F = Fill character (c)
		//	B = Binary
		//	S = +|-
		//	E = Cientific Notation
		class LINK text_format
		{
		protected:
			qword flags;

		public:
			enum target : byte
			{
				none = 0,
				character,
				text,
				signed_integer,
				usigned_integer,
				floating,
			};

		public:
			text_format(cstr_t format);
			text_format(const text_format&);
			virtual~text_format();

		public:
			target format_target()const;
			void format(ang::cstr_t format);
			string format()const;
			qword format_flags()const;
			text_format& operator = (const text_format&);
		};

		typedef text_format text_format_t;

		ANG_INTERFACE(itext_buffer); //template<encoding_enum ENCODING> struct itext_buffer;
		//template<encoding_enum ENCODING> using itext_buffer_t = intf_wrapper<itext_buffer<ENCODING>>;
		//template<encoding_enum ENCODING> using itext_buffer_ptr_t = intf_wrapper_ptr<itext_buffer<ENCODING>>;

	

		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, itext_buffer, public ibuffer)
			visible vcall raw_str_t text_buffer() pure;
		ANG_END_INTERFACE();


		template<text::encoding_enum ENCODING>
		class text_buffer_wrapper final
			: public object
			, public itext_buffer
		{
		private:
			ibuffer_t _buffer;

		public:
			inline text_buffer_wrapper(ibuffer_t buffer) : _buffer(buffer) {}

			ANG_DECLARE_INLINE_INTERFACE();

			inline encoding_t encoding()const override { return ENCODING; }
			raw_str_t text_buffer() override {
				return _buffer.is_empty() ? raw_str_t{ null, 0, encoding::none } 
				: raw_str_t{ _buffer->buffer_ptr(), _buffer->buffer_size(), ENCODING };
			}

		private:
			inline~text_buffer_wrapper() {}
		};
	}

	namespace strings
	{
		template<encoding_enum _ENCODING>
		struct str_data
		{
			typedef typename char_type_by_encoding<_ENCODING>::char_t char_t;
			typedef typename char_type_by_encoding<_ENCODING>::str_t  str_t;
			typedef typename char_type_by_encoding<_ENCODING>::cstr_t cstr_t;

			static constexpr encoding_enum ENCODING = _ENCODING;
			static constexpr wsize CAPACITY = 128u / sizeof(char_t);

			union {
				wsize _storage_type;
				struct {
					wsize _unused;
					wsize _allocated_length;
					wsize _allocated_capacity;
					str_t _allocated_buffer;
				};
				struct {
					wsize _stack_length;
					char_t _stack_buffer[CAPACITY];
				};
			};
		};
	}

}

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::ascii
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::unicode
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf8
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf16
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf16_se
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf16_le
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf16_be
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf32
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf32_se
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf32_le
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf32_be
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

namespace ang
{
	namespace text
	{
		template<encoding_enum ENCODING>
		inline windex string_util::sub_string(strings::string_base<ENCODING>& str, windex start, windex end)const {
			if (str.is_empty())
				str = new strings::string_buffer<ENCODING>();
			str->realloc(min(end, _data.size() / _char_size) - start, false);
			auto ptr = str->str();
			str->length(get_encoder<ENCODING>().convert(str->str(), min(_data.size() / _char_size, end) - start, (byte*)_data.ptr() + start * _char_size, _data.encoding(), true));
			return str->length();
		}
	}

	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator == (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator != (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator >= (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator <= (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator > (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator < (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);

	template<text::encoding_enum ENCODING1, typename T>
	bool operator == (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);
	template<text::encoding_enum ENCODING1, typename T>
	bool operator != (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);
	template<text::encoding_enum ENCODING1, typename T>
	bool operator >= (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);
	template<text::encoding_enum ENCODING1, typename T>
	bool operator <= (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);
	template<text::encoding_enum ENCODING1, typename T>
	bool operator > (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);
	template<text::encoding_enum ENCODING1, typename T>
	bool operator < (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);

	template<typename T, text::encoding_enum ENCODING2>
	bool operator == (safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	template<typename T, text::encoding_enum ENCODING2>
	bool operator != (safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	template<typename T, text::encoding_enum ENCODING2>
	bool operator >= (safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	template<typename T, text::encoding_enum ENCODING2>
	bool operator <= (safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	template<typename T, text::encoding_enum ENCODING2>
	bool operator > (safe_str<T>const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	template<typename T, text::encoding_enum ENCODING2>
	bool operator < (safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);


	inline string operator "" _o(const char* str, wsize sz) { return new strings::string_buffer < text::encoding::ascii>(cstr_t(str, sz)); }
	inline wstring operator "" _o(const wchar_t* str, wsize sz) { return new strings::string_buffer < text::encoding::unicode>(cwstr_t(str, sz)); }
}
#endif//__ANG_STRINGS_HPP__