
#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_BASE_TEXT_HPP__
#define __ANG_BASE_TEXT_HPP__


namespace ang
{
	namespace text
	{
		class text_format;

		template<wsize SIZE = sizeof(wchar)>struct native_encoding_selection;

		/******************************************************************/
		/* enum ang::text::encoding :                                     */
		/*  -> represents the text encoding                               */
		/******************************************************************/
		ANG_BEGIN_ENUM(LINK, encoding, uint)
			none = 0,
			binary = none,
			ascii,
			unicode,
			utf8,
			utf16_le,
			utf16_be,
			utf16,
			utf16_se,
			utf32_le,
			utf32_be,
			utf32,
			utf32_se,
			auto_detect = 0XFFFFFFFF,
		ANG_END_ENUM(encoding);

		template<encoding_enum ENCODING> struct char_type_by_encoding;

		template<typename T> struct encoding_by_type {
			static constexpr encoding_enum encoding() { return encoding::auto_detect; }
		};

		template<> 	struct char_type_by_encoding<encoding::ascii> {
			typedef ascii_char_t char_t;
			typedef unsafe_ascii_str_t str_t;
			typedef unsafe_ascii_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::unicode> {
			typedef unicode_char_t char_t;
			typedef unsafe_unicode_str_t str_t;
			typedef unsafe_unicode_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf8> {
			typedef utf8_char_t char_t;
			typedef unsafe_utf8_str_t str_t;
			typedef unsafe_utf8_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf16> {
			typedef utf16_char_t char_t;
			typedef unsafe_utf16_str_t str_t;
			typedef unsafe_utf16_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf16_le> {
			typedef utf16_char_t char_t;
			typedef unsafe_utf16_str_t str_t;
			typedef unsafe_utf16_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf16_be> {
			typedef utf16_char_t char_t;
			typedef unsafe_utf16_str_t str_t;
			typedef unsafe_utf16_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf16_se> {
			typedef utf16_char_t char_t;
			typedef unsafe_utf16_str_t str_t;
			typedef unsafe_utf16_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf32> {
			typedef utf32_char_t char_t;
			typedef unsafe_utf32_str_t str_t;
			typedef unsafe_utf32_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf32_le> {
			typedef utf32_char_t char_t;
			typedef unsafe_utf32_str_t str_t;
			typedef unsafe_utf32_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf32_be> {
			typedef utf32_char_t char_t;
			typedef unsafe_utf32_str_t str_t;
			typedef unsafe_utf32_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf32_se> {
			typedef utf32_char_t char_t;
			typedef unsafe_utf32_str_t str_t;
			typedef unsafe_utf32_cstr_t cstr_t;
		};

		template<> struct encoding_by_type<ascii_char_t> {
			static constexpr encoding_enum encoding() { return encoding::ascii; }
		};

		template<> struct encoding_by_type<unicode_char_t> {
			static constexpr encoding_enum encoding() { return encoding::unicode; }
		};

		template<> struct encoding_by_type<utf8_char_t> {
			static constexpr encoding_enum encoding() { return encoding::utf8; }
		};

		template<> struct encoding_by_type<utf16_char_t> {
			static constexpr encoding_enum encoding() { return encoding::utf16; }
		};

		template<> struct encoding_by_type<utf32_char_t> {
			static constexpr encoding_enum encoding() { return encoding::utf32; }
		};

		template<typename T> struct encoding_by_type<T*> {
			static constexpr encoding_enum encoding() { return encoding_by_type<T>::encoding(); }
		};

		template<typename T> struct encoding_by_type<T const*> {
			static constexpr encoding_enum encoding() { return encoding_by_type<T>::encoding(); }
		};

		template<typename T> struct encoding_by_type<safe_str<T>> {
			static constexpr encoding_enum encoding() { return encoding_by_type<T>::encoding(); }
		};

		template<typename T> struct encoding_by_type<safe_str<const T>> {
			static constexpr encoding_enum encoding() { return encoding_by_type<T>::encoding(); }
		};

		template<typename T> struct char_type_by_type {
			typedef T char_t;
			typedef T* str_t;
			typedef T const* cstr_t;
		};

		template<typename T> struct char_type_by_type<const T> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<T&> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<T&&> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<T const&> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<T*> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<T const*> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<safe_str<T>> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<safe_str<const T>> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		struct encoder_interface
		{
			static LINK void initialize_interface(encoder_interface*, encoding_t);
			encoding_t(*_format)();
			wsize(*_length)(pointer);
			wsize(*_size)(pointer, encoding_t);
			int(*_compare_string)(pointer, pointer, encoding_t);
			wsize(*_compare_string_until)(pointer, pointer, encoding_t);
			wsize(*_convert_string)(pointer, wsize, pointer, encoding_t, bool);
			windex(*_find)(pointer, wsize, pointer, wsize, encoding_t, windex);
			windex(*_find_revert)(pointer, wsize, pointer, wsize, encoding_t, windex);
		};


		template<encoding_enum ENCODING>
		class encoder
			: private encoder_interface
			, rule<(ENCODING <= encoding::utf32_se)>
		{
		public:
			typedef typename char_type_by_encoding<ENCODING>::char_t char_t;
			typedef typename char_type_by_encoding<ENCODING>::str_t str_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t cstr_t;

			encoder() { initialize_interface(this, ENCODING); }

			inline text::encoding_t format()const { return _format(); }
			inline wsize length(cstr_t str)const { return _length((pointer)str); }

			template<typename T>
			inline wsize size(T const* str)const { return _size((pointer)str, encoding_by_type<T>::encoding()); }
			inline wsize size(pointer str, text::encoding_t format)const { return _size((pointer)str, format); }

			template<typename T>
			inline int compare(cstr_t cstr1, T const* cstr2)const { return _compare_string((pointer)cstr1, (pointer)cstr2, encoding_by_type<T>::encoding()); }
			inline int compare(cstr_t cstr1, pointer cstr2, text::encoding_t format)const { return _compare_string((pointer)cstr1, cstr2, format); }

			template<typename T>
			inline wsize compare_until(cstr_t cstr1, T const* cstr2)const { return _compare_string_until((pointer)cstr1, (pointer)cstr2, encoding_by_type<T>::encoding()); }
			inline wsize compare_until(cstr_t cstr1, pointer cstr2, text::encoding_t format)const { return _compare_string_until((pointer)cstr1, cstr2, format); }

			template<typename T>
			inline wsize convert(str_t dest, wsize maxsize, T const* src, bool end_of_string = true)const { return _convert_string((pointer)dest, maxsize, (pointer)src, encoding_by_type<T>::encoding(), end_of_string); }
			inline wsize convert(str_t dest, wsize maxsize, pointer src, text::encoding_t format, bool end_of_string = true)const { return _convert_string((pointer)dest, maxsize, src, format, end_of_string); }

			template<typename T>
			windex find(cstr_t first, wsize s1, T const* second, wsize s2, windex start)const { return _find((pointer)first, s1, (pointer)second, s2, encoding_by_type<T>::encoding(), start); }
			windex find(cstr_t first, wsize s1, pointer second, wsize s2, text::encoding_t format, windex start)const { return _find((pointer)first, s1, second, s2, format, start); }

			template<typename T>
			windex find_revert(cstr_t first, wsize s1, T const* second, wsize s2, windex start)const { return _find_revert((pointer)first, s1, (pointer)second, s2, encoding_by_type<T>::encoding(), min(s1, start)); }
			windex find_revert(cstr_t first, wsize s1, pointer second, wsize s2, text::encoding_t format, windex start)const { return _find_revert((pointer)first, s1, second, s2, format, min(s1,start)); }
		};


		typedef encoder<encoding::ascii> ASCII_t;
		typedef encoder<encoding::unicode> UNICODE_t;
		typedef encoder<encoding::utf8> UTF8_t;
		typedef encoder<encoding::utf16> UTF16_t;
		typedef encoder<encoding::utf16_se> UTF16_SE_t;
		typedef encoder<encoding::utf16_le> UTF16_LE_t;
		typedef encoder<encoding::utf16_be> UTF16_BE_t;
		typedef encoder<encoding::utf32> UTF32_t;
		typedef encoder<encoding::utf32_se> UTF32_SE_t;
		typedef encoder<encoding::utf32_le> UTF32_LE_t;
		typedef encoder<encoding::utf32_be> UTF32_BE_t;

		template<encoding_enum ENCODING>
		inline encoder<ENCODING> const& get_encoder() {
			static encoder<ENCODING> _encoder; return _encoder;
		}

		template<typename T>
		inline encoder<encoding_by_type<T>::encoding()> const& get_encoder_by_type() {
			return get_encoder<encoding_by_type<T>::encoding()>();
		}

		inline const ASCII_t& ASCII() { return get_encoder<encoding::ascii>(); }
		inline const UNICODE_t& UNICODE() { return get_encoder<encoding::unicode>(); }
		inline const UTF8_t& UTF8() { return get_encoder<encoding::utf8>(); }
		inline const UTF16_t& UTF16() { return get_encoder<encoding::utf16>(); }
		inline const UTF32_t& UTF32() { return get_encoder<encoding::utf32>(); }

		template<>
		class encoder<encoding::auto_detect>
		{
		public:
			inline text::encoding_t format()const { return encoding::auto_detect; }
			template<typename T>
			inline wsize length(T cstr)const { 
				return get_encoder_by_type<T>().length((typename char_type_by_type<T>::cstr_t)cstr);
			}

			template<typename T1, typename T2>
			inline wsize size(T2 cstr)const { 
				return get_encoder_by_type<T1>().size((typename char_type_by_type<T2>::cstr_t)cstr);
			}

			template<typename T1, typename T2>
			inline int compare(T1 cstr1, T2 cstr2)const { 
				return get_encoder_by_type<T1>().compare((typename char_type_by_type<T1>::cstr_t)cstr1, (typename char_type_by_type<T2>::cstr_t)cstr2);
			}

			template<typename T1, typename T2>
			inline wsize compare_until(T1 cstr1, T2 cstr2)const {
				return get_encoder_by_type<T1>().compare_until((typename char_type_by_type<T1>::cstr_t)cstr1, (typename char_type_by_type<T2>::cstr_t)cstr2);
			}

			template<typename T1, typename T2>
			inline wsize convert(T1 dest, wsize maxsize, T2 src, bool end_of_string = true)const { 
				return get_encoder_by_type<T1>().convert((typename char_type_by_type<T1>::str_t)dest, maxsize, (typename char_type_by_type<T2>::cstr_t)src, end_of_string);
			}
		};

		typedef encoder<encoding::auto_detect> encoder_t;

		template< bool SWAP, typename T> inline T swap_endian(T value) { return value; }
		template<> inline char16_t  swap_endian<true, char16_t>(char16_t value) {
			return char16_t((word(value) & 0XFF00) >> 8) | ((word(value) & 0X00FF) << 8);
		}
		template<> inline char32_t swap_endian<true, char32_t>(char32_t value) {
			return char32_t((dword(value) & 0XFF000000) >> 24) | ((dword(value) & 0X00FF0000) >> 8) | ((dword(value) & 0X0000FF00) << 8) | ((dword(value) & 0X000000FF) << 24);
		}

		LINK bool is_little_endian();

		template<encoding_enum ENCODING>
		inline constexpr bool is_endian_swapped() { return false; }

		template<> inline bool is_endian_swapped<encoding::utf16_le>() { return !is_little_endian(); }
		template<> inline bool is_endian_swapped<encoding::utf16_be>() { return is_little_endian(); }
		template<> inline bool is_endian_swapped<encoding::utf32_le>() { return !is_little_endian(); }
		template<> inline bool is_endian_swapped<encoding::utf32_be>() { return is_little_endian(); }
	}
}

#endif//__ANG_BASE_TEXT_HPP__
