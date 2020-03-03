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
#elif !defined __ANG_BASE_ENCODER_H__
#define __ANG_BASE_ENCODER_H__

namespace ang //constants
{
	namespace text
	{
	
		template<encoding E> struct text_encoder;
		using encoder = text_encoder<encoding::auto_detect>;
		typedef text_encoder<encoding::ascii>	ascii_t, ascii;
		typedef text_encoder<encoding::unicode>	unicode_t, unicode;
		typedef text_encoder<encoding::utf8>		utf8_t, utf8;
		typedef text_encoder<encoding::utf16>	utf16_t, utf16;
		typedef text_encoder<encoding::utf16_se> utf16_se_t, utf16_se;
		typedef text_encoder<encoding::utf16_le> utf16_le_t, utf16_le;
		typedef text_encoder<encoding::utf16_be> utf16_be_t, utf16_be;
		typedef text_encoder<encoding::utf32>	utf32_t, utf32;
		typedef text_encoder<encoding::utf32_se> utf32_se_t, utf32_se;
		typedef text_encoder<encoding::utf32_le> utf32_le_t, utf32_le;
		typedef text_encoder<encoding::utf32_be> utf32_be_t, utf32_be;

		template<encoding E>
		struct text_encoder
		{
			static constexpr text::encoding ENCODING = E;
			static constexpr text::encoding NATIVE_ENCODING = native_encoding<ENCODING>::value;

			typedef typename char_type_by_encoding<ENCODING>::char_type char_t;
			typedef typename char_type_by_encoding<ENCODING>::str_type raw_str_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_type raw_cstr_t;
			typedef str_view<char_t, ENCODING> str_t;
			typedef str_view<const char_t, ENCODING> cstr_t;

			typedef typename char_type_by_encoding<NATIVE_ENCODING>::char_type native_char_t;
			typedef typename char_type_by_encoding<NATIVE_ENCODING>::str_type native_raw_str_t;
			typedef typename char_type_by_encoding<NATIVE_ENCODING>::cstr_type native_raw_cstr_t;
			typedef str_view<native_char_t, NATIVE_ENCODING> native_str_t;
			typedef str_view<const native_char_t, NATIVE_ENCODING> native_cstr_t;

			static wsize lenght(raw_cstr_t cstr);//base character count
			static wsize count(raw_cstr_t cstr);//character count
			template <typename cstr2_t> static wsize size(cstr2_t cstr, windex start = 0, windex end = -1);//character needed for convertion
			template<typename cstr2_t> static int compare(raw_cstr_t first, cstr2_t second);
			template<typename cstr2_t> static windex compare_until(raw_cstr_t first, cstr2_t second);
			template<typename cstr2_t> static windex find(raw_cstr_t first, wsize sz1, cstr2_t second, wsize sz2, windex start);
			template<typename cstr2_t> static windex find_reverse(raw_cstr_t first, wsize sz1, cstr2_t second, wsize sz2, windex start);
			static windex find_any(raw_cstr_t first, wsize sz1, windex start, array_view<const char32> chars);
			template<typename cstr2_t> static raw_str_t convert(raw_str_t str, cstr2_t cstr, bool eos = true, wsize max_out = -1, wsize max_in = -1);
			template<typename cstr2_t> static raw_str_t convert(raw_str_t str, wsize& i, cstr2_t cstr, wsize& j, bool eos = true, wsize max_out = -1, wsize max_in = -1);
		};

		template<> struct LINK text_encoder<encoding::auto_detect> {

			inline static wsize char_size_by_encoding(text::encoding encoding) {
				static const wsize sizes[] = {
					1, //this is 1 to prevent division by 0
					size_of<typename text::char_type_by_encoding<encoding::ascii>::char_type>(),
					size_of<typename text::char_type_by_encoding<encoding::unicode>::char_type>(),
					size_of<typename text::char_type_by_encoding<encoding::utf8>::char_type>(),
					size_of<typename text::char_type_by_encoding<encoding::utf16>::char_type>(),
					size_of<typename text::char_type_by_encoding<encoding::utf16_se>::char_type>(),
					size_of<typename text::char_type_by_encoding<encoding::utf16_le>::char_type>(),
					size_of<typename text::char_type_by_encoding<encoding::utf16_be>::char_type>(),
					size_of<typename text::char_type_by_encoding<encoding::utf32>::char_type>(),
					size_of<typename text::char_type_by_encoding<encoding::utf32_se>::char_type>(),
					size_of<typename text::char_type_by_encoding<encoding::utf32_le>::char_type>(),
					size_of<typename text::char_type_by_encoding<encoding::utf32_be>::char_type>()
				};
				return encoding < text::encoding::encoding_count ? sizes[(uint)encoding] : 1;
			}

			static wsize lenght(cstr_t const& cstr);//base character count
			static wsize count(cstr_t const& cstr);//character count
			static wsize size(cstr_t const& from, encoding to, windex start = 0, windex end = -1);//character needed for convertion
			static int compare(cstr_t const& first, cstr_t const& second);
			static windex compare_until(cstr_t const& first, cstr_t const& second);
			static windex find(cstr_t const& first, cstr_t const& second, windex start);
			static windex find_reverse(cstr_t const& first, cstr_t const& second, windex start);
			static windex find_any(cstr_t const& first, windex start, array_view<const char32> chars);
			static char32_t to_char32(cstr_t const& cstr, windex& at, bool increase = true);
			static void set_eos(str_t str, windex at);
			static cstr_t convert(str_t str, cstr_t const& cstr, bool eos = true, wsize max_out = -1, wsize max_in = -1);
			static cstr_t convert(str_t str, wsize& i, cstr_t const& cstr, wsize& j, bool eos = true, wsize max_out = -1, wsize max_in = -1);
		};

		template<typename T> inline dword char_to_dword(T);
		template<typename T> inline char32 char_to_char32(T);
		template <bool SWAP, typename T> inline constexpr T swap_endian(T);
		template<typename T> inline bool is_end_of_string(const T*);
		template<typename T> inline bool is_end_of_word(const T*);
		template<typename T> inline bool is_end_of_line(const T*);
		template<bool SWAP1, bool SWAP2, typename T> char32_t to_char32(T const*, wsize&);
		template<bool SWAP1, bool SWAP2, typename T> char32_t to_char32(T const*&);
		template<bool SWAP1, bool SWAP2, typename T, text::encoding E> char32_t to_char32(str_view< const T, E> const&, wsize&);
		template<bool SWAP1, bool SWAP2, typename T, text::encoding E> char32_t to_char32(str_view< const T, E>&);

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
}


#endif//__ANG_BASE_TEXT_H__
