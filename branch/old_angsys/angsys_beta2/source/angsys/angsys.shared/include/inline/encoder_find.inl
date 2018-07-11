#pragma once

template<ang::text::encoding_enum ENCODING, ang::text::encoding_enum OTHER_ENCODING>
struct ang_text_encoder_find
{
	template<bool SWAP1, bool SWAP2>
	static windex find(typename ang::text::char_type_by_encoding<ENCODING>::cstr_t first, wsize s1, typename ang::text::char_type_by_encoding<OTHER_ENCODING>::cstr_t second, wsize s2, windex start)
	{
		typedef typename ang::text::char_type_by_encoding<ENCODING>::char_t char1_t;
		typedef typename ang::text::char_type_by_encoding<OTHER_ENCODING>::char_t char2_t;

		if (first == null || second == null) return ang::invalid_index;
		if (s2 == 0 || start >= s1) return ang::invalid_index;
		if (s2 > (s1 - start)) return ang::invalid_index;

		char1_t const* beg = first + start;
		char1_t const* end = first + s1 - s2 + 1;

		char32_t c1, c2;
		windex i = start, j, k, l, t;
		do {
			j = 0;
			k = 0;
			l = 0;
			while (k < s2)
			{
				c1 = ang::strings::converter<char32_t, char1_t>::template convert<SWAP1>(beg, j);
				c2 = ang::strings::converter<char32_t, char2_t>::template convert<SWAP1>(second, k);
				if (c1 != c2)
					break;
				l = k;
			}
			if (l == s2) return i;	
			t = 0;
			ang::strings::converter<char32_t, char1_t>::template size<SWAP1>(beg, t);
			i += t;
			beg += t;
		} while (beg < end);
		return ang::invalid_index;
	}

	template<bool SWAP1, bool SWAP2>
	static wsize find_revert(typename ang::text::char_type_by_encoding<ENCODING>::cstr_t first, wsize s1, typename ang::text::char_type_by_encoding<OTHER_ENCODING>::cstr_t second, wsize s2, windex start)
	{
		typedef typename ang::text::char_type_by_encoding<ENCODING>::char_t char1_t;
		typedef typename ang::text::char_type_by_encoding<OTHER_ENCODING>::char_t char2_t;

		if (first == null || second == null) return ang::invalid_index;
		if (s2 == 0 || start > s1) return ang::invalid_index;
		if (s2 > start)	return ang::invalid_index;

		char1_t const* beg = first;
		char1_t const* end = first + (start - s2);
		windex i = start - s2, j, k;
		do {
			j = 0;
			k = 0;
			while (k < s2 && ang::strings::converter<char32_t, char1_t>::template convert<SWAP1>(end, j) == ang::strings::converter<char32_t, char2_t>::template convert<SWAP1>(second, k));
			if (k == s2) return i;
			i--;
		} while (beg <= --end);
		return ang::invalid_index;
	}
};


template<ang::text::encoding_enum ENCODING>
windex ang::text::ang_text_encoder<ENCODING>::find(typename ang::text::char_type_by_encoding<ENCODING>::cstr_t first, wsize s1, pointer second, wsize s2, encoding_t format, windex start)
{
	switch (format)
	{
	case encoding::ascii:
		return ang_text_encoder_find<ENCODING, encoding::ascii>::template find<is_endian_swapped(), false>(first, s1,(unsafe_ascii_cstr_t)second, s2, start);
	case encoding::unicode:
		return ang_text_encoder_find<ENCODING, encoding::unicode>::template find<is_endian_swapped(), false>(first, s1, (unsafe_unicode_cstr_t)second, s2, start);
	case encoding::utf8:
		return ang_text_encoder_find<ENCODING, encoding::utf8>::template find<is_endian_swapped(), false>(first, s1, (unsafe_utf8_cstr_t)second, s2, start);
	case encoding::utf16:
		return ang_text_encoder_find<ENCODING, encoding::utf16>::template find<is_endian_swapped(), false>(first, s1, (unsafe_utf16_cstr_t)second, s2, start);
	case encoding::utf16_se:
		return ang_text_encoder_find<ENCODING, encoding::utf16>::template find<is_endian_swapped(), true>(first, s1, (unsafe_utf16_cstr_t)second, s2, start);
	case encoding::utf16_le:
		return ang::text::is_endian_swapped<encoding::utf16_le>() ? ang_text_encoder_find<ENCODING, encoding::utf16>::template find<is_endian_swapped(), true>(first, s1, (unsafe_utf16_cstr_t)second, s2, start)
			: ang_text_encoder_find<ENCODING, encoding::utf16>::template find<is_endian_swapped(), false>(first, s1, (unsafe_utf16_cstr_t)second, s2, start);
	case encoding::utf16_be:
		return ang::text::is_endian_swapped<encoding::utf16_be>() ? ang_text_encoder_find<ENCODING, encoding::utf16>::template find<is_endian_swapped(), true>(first, s1, (unsafe_utf16_cstr_t)second, s2, start)
			: ang_text_encoder_find<ENCODING, encoding::utf16>::template find<is_endian_swapped(), false>(first, s1, (unsafe_utf16_cstr_t)second, s2, start);
	case encoding::utf32:
		return ang_text_encoder_find<ENCODING, encoding::utf32>::template find<is_endian_swapped(), false>(first, s1, (unsafe_utf32_cstr_t)second, s2, start);
	case encoding::utf32_se:
		return ang_text_encoder_find<ENCODING, encoding::utf32>::template find<is_endian_swapped(), true>(first, s1, (unsafe_utf32_cstr_t)second, s2, start);
	case encoding::utf32_le:
		return ang::text::is_endian_swapped<encoding::utf32_le>() ? ang_text_encoder_find<ENCODING, encoding::utf32>::template find<is_endian_swapped(), false>(first, s1, (unsafe_utf32_cstr_t)second, s2, start)
			: ang_text_encoder_find<ENCODING, encoding::utf32>::template find<is_endian_swapped(), true>(first, s1, (unsafe_utf32_cstr_t)second, s2, start);
	case encoding::utf32_be:
		return ang::text::is_endian_swapped<encoding::utf32_be>() ? ang_text_encoder_find<ENCODING, encoding::utf32>::template find<is_endian_swapped(), false>(first, s1, (unsafe_utf32_cstr_t)second, s2, start)
			: ang_text_encoder_find<ENCODING, encoding::utf32>::template find<is_endian_swapped(), true>(first, s1, (unsafe_utf32_cstr_t)second, s2, start);
	default:return 0;
	}

}



template<ang::text::encoding_enum ENCODING>
windex ang::text::ang_text_encoder<ENCODING>::find_revert(typename ang::text::char_type_by_encoding<ENCODING>::cstr_t first, wsize s1, pointer second, wsize s2, encoding_t format, windex start)
{
	switch (format)
	{
	case encoding::ascii:
		return ang_text_encoder_find<ENCODING, encoding::ascii>::template find_revert<is_endian_swapped(), false>(first, s1, (unsafe_ascii_cstr_t)second, s2, start);
	case encoding::unicode:
		return ang_text_encoder_find<ENCODING, encoding::unicode>::template find_revert<is_endian_swapped(), false>(first, s1, (unsafe_unicode_cstr_t)second, s2, start);
	case encoding::utf8:
		return ang_text_encoder_find<ENCODING, encoding::utf8>::template find_revert<is_endian_swapped(), false>(first, s1, (unsafe_utf8_cstr_t)second, s2, start);
	case encoding::utf16:
		return ang_text_encoder_find<ENCODING, encoding::utf16>::template find_revert<is_endian_swapped(), false>(first, s1, (unsafe_utf16_cstr_t)second, s2, start);
	case encoding::utf16_se:
		return ang_text_encoder_find<ENCODING, encoding::utf16>::template find_revert<is_endian_swapped(), true>(first, s1, (unsafe_utf16_cstr_t)second, s2, start);
	case encoding::utf16_le:
		return ang::text::is_endian_swapped<encoding::utf16_le>() ? ang_text_encoder_find<ENCODING, encoding::utf16>::template find_revert<is_endian_swapped(), true>(first, s1, (unsafe_utf16_cstr_t)second, s2, start)
			: ang_text_encoder_find<ENCODING, encoding::utf16>::template find_revert<is_endian_swapped(), false>(first, s1, (unsafe_utf16_cstr_t)second, s2, start);
	case encoding::utf16_be:
		return ang::text::is_endian_swapped<encoding::utf16_be>() ? ang_text_encoder_find<ENCODING, encoding::utf16>::template find_revert<is_endian_swapped(), true>(first, s1, (unsafe_utf16_cstr_t)second, s2, start)
			: ang_text_encoder_find<ENCODING, encoding::utf16>::template find_revert<is_endian_swapped(), false>(first, s1, (unsafe_utf16_cstr_t)second, s2, start);
	case encoding::utf32:
		return ang_text_encoder_find<ENCODING, encoding::utf32>::template find_revert<is_endian_swapped(), false>(first, s1, (unsafe_utf32_cstr_t)second, s2, start);
	case encoding::utf32_se:
		return ang_text_encoder_find<ENCODING, encoding::utf32>::template find_revert<is_endian_swapped(), true>(first, s1, (unsafe_utf32_cstr_t)second, s2, start);
	case encoding::utf32_le:
		return ang::text::is_endian_swapped<encoding::utf32_le>() ? ang_text_encoder_find<ENCODING, encoding::utf32>::template find_revert<is_endian_swapped(), false>(first, s1, (unsafe_utf32_cstr_t)second, s2, start)
			: ang_text_encoder_find<ENCODING, encoding::utf32>::template find_revert<is_endian_swapped(), true>(first, s1, (unsafe_utf32_cstr_t)second, s2, start);
	case encoding::utf32_be:
		return ang::text::is_endian_swapped<encoding::utf32_be>() ? ang_text_encoder_find<ENCODING, encoding::utf32>::template find_revert<is_endian_swapped(), false>(first, s1, (unsafe_utf32_cstr_t)second, s2, start)
			: ang_text_encoder_find<ENCODING, encoding::utf32>::template find_revert<is_endian_swapped(), true>(first, s1, (unsafe_utf32_cstr_t)second, s2, start);
	default:return 0;
	}

}

