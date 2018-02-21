#pragma once

template<ang::text::encoding_enum ENCODING, ang::text::encoding_enum OTHER_ENCODING>
struct ang_text_encoder_compare
{
	template<bool SWAP1, bool SWAP2>
	static int compare(typename ang::text::char_type_by_encoding<ENCODING>::cstr_t first, typename ang::text::char_type_by_encoding<OTHER_ENCODING>::cstr_t second)
	{
		if ((pointer)first == (pointer)second)
			return 0;
		if (first == null)
			return 1;
		else if (second == null)
			return -1;
		windex i = 0, j = 0;
		int r;
		ang::utf32_char_t c1, c2;
	LOOP:
		c1 = ang::text::converter<ang::utf32_char_t, typename ang::text::char_type_by_encoding<ENCODING>::char_t>::template convert<SWAP1>(first, i);
		c2 = ang::text::converter<ang::utf32_char_t, typename ang::text::char_type_by_encoding<OTHER_ENCODING>::char_t>::template convert<SWAP2>(second, j);
		r = c1 - c2;
		if (r) return r > 0 ? 1 : -1;
		else if (!c1) return 0;
		goto LOOP;
	}

	template<bool SWAP1, bool SWAP2>
	static wsize compare_until(typename ang::text::char_type_by_encoding<ENCODING>::cstr_t first, typename ang::text::char_type_by_encoding<OTHER_ENCODING>::cstr_t second)
	{
		if ((pointer)first == (pointer)second)
			return ang::text::ang_text_encoder<ENCODING>::length(first);

		if (first == null || second == null)
			return 0;

		windex i = 0, j = 0, k = 0;
		ang::utf32_char_t c1, c2;
	LOOP:
		c1 = ang::text::converter<ang::utf32_char_t, typename ang::text::char_type_by_encoding<ENCODING>::char_t>::template convert<SWAP1>(first, i);
		c2 = ang::text::converter<ang::utf32_char_t, typename ang::text::char_type_by_encoding<OTHER_ENCODING>::char_t>::template convert<SWAP2>(second, j);
		if (c1 != c2 || c1 == 0) return k;
		k = i;
		goto LOOP;
	}
};


template<ang::text::encoding_enum ENCODING>
int ang::text::ang_text_encoder<ENCODING>::compare(typename char_type_by_encoding<ENCODING>::cstr_t cstr1, pointer cstr2, encoding_t format)
{
	switch (format)
	{
	case encoding::ascii:
		return ang_text_encoder_compare<ENCODING, encoding::ascii>::template compare<is_endian_swapped(), false>(cstr1, (unsafe_ascii_cstr_t)cstr2);
	case encoding::unicode:
		return ang_text_encoder_compare<ENCODING, encoding::unicode>::template compare<is_endian_swapped(), false>(cstr1, (unsafe_unicode_cstr_t)cstr2);
	case encoding::utf8:
		return ang_text_encoder_compare<ENCODING, encoding::utf8>::template compare<is_endian_swapped(), false>(cstr1, (unsafe_utf8_cstr_t)cstr2);
	case encoding::utf16:
		return ang_text_encoder_compare<ENCODING, encoding::utf16>::template compare<is_endian_swapped(), false>(cstr1, (unsafe_utf16_cstr_t)cstr2);
	case encoding::utf16_se:
		return ang_text_encoder_compare<ENCODING, encoding::utf16>::template compare<is_endian_swapped(), true>(cstr1, (unsafe_utf16_cstr_t)cstr2);
	case encoding::utf16_le:
		return ang::text::is_endian_swapped<encoding::utf16_le>() ? ang_text_encoder_compare<ENCODING, encoding::utf16>::template compare<is_endian_swapped(), true>(cstr1, (unsafe_utf16_cstr_t)cstr2)
			: ang_text_encoder_compare<ENCODING, encoding::utf16>::template compare<is_endian_swapped(), false>(cstr1, (unsafe_utf16_cstr_t)cstr2);
	case encoding::utf16_be:
		return ang::text::is_endian_swapped<encoding::utf16_be>() ? ang_text_encoder_compare<ENCODING, encoding::utf16>::template compare<is_endian_swapped(), true>(cstr1, (unsafe_utf16_cstr_t)cstr2)
			: ang_text_encoder_compare<ENCODING, encoding::utf16>::template compare<is_endian_swapped(), false>(cstr1, (unsafe_utf16_cstr_t)cstr2);
	case encoding::utf32:
		return ang_text_encoder_compare<ENCODING, encoding::utf32>::template compare<is_endian_swapped(), false>(cstr1, (unsafe_utf32_cstr_t)cstr2);
	case encoding::utf32_se:
		return ang_text_encoder_compare<ENCODING, encoding::utf32>::template compare<is_endian_swapped(), true>(cstr1, (unsafe_utf32_cstr_t)cstr2);
	case encoding::utf32_le:
		return ang::text::is_endian_swapped<encoding::utf32_le>() ? ang_text_encoder_compare<ENCODING, encoding::utf32>::template compare<is_endian_swapped(), false>(cstr1, (unsafe_utf32_cstr_t)cstr2)
			: ang_text_encoder_compare<ENCODING, encoding::utf32>::template compare<is_endian_swapped(), true>(cstr1, (unsafe_utf32_cstr_t)cstr2);
	case encoding::utf32_be:
		return ang::text::is_endian_swapped<encoding::utf32_be>() ? ang_text_encoder_compare<ENCODING, encoding::utf32>::template compare<is_endian_swapped(), false>(cstr1, (unsafe_utf32_cstr_t)cstr2)
			: ang_text_encoder_compare<ENCODING, encoding::utf32>::template compare<is_endian_swapped(), true>(cstr1, (unsafe_utf32_cstr_t)cstr2);
	default:return 0;
	}

}

template<ang::text::encoding_enum ENCODING>
wsize ang::text::ang_text_encoder<ENCODING>::compare_until(typename char_type_by_encoding<ENCODING>::cstr_t cstr1, pointer cstr2, encoding_t format)
{
	switch (format)
	{
	case encoding::ascii:
		return ang_text_encoder_compare<ENCODING, encoding::ascii>::template compare_until<is_endian_swapped(), false>(cstr1, (unsafe_ascii_cstr_t)cstr2);
	case encoding::unicode:
		return ang_text_encoder_compare<ENCODING, encoding::unicode>::template compare_until<is_endian_swapped(), false>(cstr1, (unsafe_unicode_cstr_t)cstr2);
	case encoding::utf8:
		return ang_text_encoder_compare<ENCODING, encoding::utf8>::template compare_until<is_endian_swapped(), false>(cstr1, (unsafe_utf8_cstr_t)cstr2);
	case encoding::utf16:
		return ang_text_encoder_compare<ENCODING, encoding::utf16>::template compare_until<is_endian_swapped(), false>(cstr1, (unsafe_utf16_cstr_t)cstr2);
	case encoding::utf16_le:
		return ang::text::is_endian_swapped<encoding::utf16_le>() ? ang_text_encoder_compare<ENCODING, encoding::utf16>::template compare_until<is_endian_swapped(), true>(cstr1, (unsafe_utf16_cstr_t)cstr2)
			: ang_text_encoder_compare<ENCODING, encoding::utf16>::template compare_until<is_endian_swapped(), false>(cstr1, (unsafe_utf16_cstr_t)cstr2);
	case encoding::utf16_be:
		return ang::text::is_endian_swapped<encoding::utf16_be>() ? ang_text_encoder_compare<ENCODING, encoding::utf16>::template compare_until<is_endian_swapped(), true>(cstr1, (unsafe_utf16_cstr_t)cstr2)
			: ang_text_encoder_compare<ENCODING, encoding::utf16>::template compare_until<is_endian_swapped(), false>(cstr1, (unsafe_utf16_cstr_t)cstr2);
	case encoding::utf32:
		return ang_text_encoder_compare<ENCODING, encoding::utf32>::template compare_until<is_endian_swapped(), false>(cstr1, (unsafe_utf32_cstr_t)cstr2);
	case encoding::utf32_le:
		return ang::text::is_endian_swapped<encoding::utf32_le>() ? ang_text_encoder_compare<ENCODING, encoding::utf32>::template compare_until<is_endian_swapped(), false>(cstr1, (unsafe_utf32_cstr_t)cstr2)
			: ang_text_encoder_compare<ENCODING, encoding::utf32>::template compare_until<is_endian_swapped(), true>(cstr1, (unsafe_utf32_cstr_t)cstr2);
	case encoding::utf32_be:
		return ang::text::is_endian_swapped<encoding::utf32_be>() ? ang_text_encoder_compare<ENCODING, encoding::utf32>::template compare_until<is_endian_swapped(), false>(cstr1, (unsafe_utf32_cstr_t)cstr2)
			: ang_text_encoder_compare<ENCODING, encoding::utf32>::template compare_until<is_endian_swapped(), true>(cstr1, (unsafe_utf32_cstr_t)cstr2);
	default:return 0;
	}
}
