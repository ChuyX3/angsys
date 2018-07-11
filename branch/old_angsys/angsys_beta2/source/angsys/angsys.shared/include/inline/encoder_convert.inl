#pragma once


template<ang::text::encoding_enum ENCODING, ang::text::encoding_enum OTHER_ENCODING>
struct ang_text_encoder_convert
{
	template<bool SWAP1, bool SWAP2>
	static wsize convert(typename ang::text::char_type_by_encoding<ENCODING>::str_t dest, wsize sz, typename ang::text::char_type_by_encoding<OTHER_ENCODING>::cstr_t src, wsize& j, bool eos)
	{
		wsize i = 0;// j = 0;
		if (src == null)
			goto END;

	LOOP:
		if (src[j] == 0 || i >= sz) goto END;
		ang::text::converter<typename ang::text::char_type_by_encoding<ENCODING>::char_t, typename ang::text::char_type_by_encoding<OTHER_ENCODING>::char_t>::template
			convert<SWAP1, SWAP2>(dest, i, src, j);
		goto LOOP;
	END:
		if (eos)dest[i] = 0;
		return i;
	}
};


template<ang::text::encoding_enum ENCODING>
wsize ang::text::ang_text_encoder<ENCODING>::convert(typename ang::text::char_type_by_encoding<ENCODING>::str_t dest, wsize sz, pointer src, wsize& idx, ang::text::encoding_t format, bool eos)
{
	switch (format)
	{
	case encoding::ascii:
		return ang_text_encoder_convert<ENCODING, encoding::ascii>::template convert<is_endian_swapped(),false>(dest, sz, (unsafe_ascii_cstr_t)src, idx, eos);
	case encoding::unicode:
		return ang_text_encoder_convert<ENCODING, encoding::unicode>::template convert<is_endian_swapped(), false>(dest, sz, (unsafe_unicode_cstr_t)src, idx, eos);
	case encoding::utf8:
		return ang_text_encoder_convert<ENCODING, encoding::utf8>::template convert<is_endian_swapped(), false>(dest, sz, (unsafe_utf8_cstr_t)src, idx, eos);
	case encoding::utf16:
		return ang_text_encoder_convert<ENCODING, encoding::utf16>::template convert<is_endian_swapped(), false>(dest, sz, (unsafe_utf16_cstr_t)src, idx, eos);
	case encoding::utf16_se:
		return ang_text_encoder_convert<ENCODING, encoding::utf16>::template convert<is_endian_swapped(), true>(dest, sz, (unsafe_utf16_cstr_t)src, idx, eos);
	case encoding::utf16_le:
		return ang::text::is_endian_swapped<encoding::utf16_le>()? ang_text_encoder_convert<ENCODING, encoding::utf16>::template convert<is_endian_swapped(), true>(dest, sz, (unsafe_utf16_cstr_t)src, idx, eos)
			: ang_text_encoder_convert<ENCODING, encoding::utf16>::template convert<is_endian_swapped(), false>(dest, sz, (unsafe_utf16_cstr_t)src, idx, eos);
	case encoding::utf16_be:
		return ang::text::is_endian_swapped<encoding::utf16_be>() ? ang_text_encoder_convert<ENCODING, encoding::utf16>::template convert<is_endian_swapped(), true>(dest, sz, (unsafe_utf16_cstr_t)src, idx, eos)
			: ang_text_encoder_convert<ENCODING, encoding::utf16>::template convert<is_endian_swapped(), false>(dest, sz, (unsafe_utf16_cstr_t)src, idx, eos);
	case encoding::utf32:
		return ang_text_encoder_convert<ENCODING, encoding::utf32>::template convert<is_endian_swapped(), false>(dest, sz, (unsafe_utf32_cstr_t)src, idx, eos);
	case encoding::utf32_se:
		return ang_text_encoder_convert<ENCODING, encoding::utf32>::template convert<is_endian_swapped(), true>(dest, sz, (unsafe_utf32_cstr_t)src, idx, eos);
	case encoding::utf32_le:
		return ang::text::is_endian_swapped<encoding::utf32_le>() ? ang_text_encoder_convert<ENCODING, encoding::utf32>::template convert<is_endian_swapped(), true>(dest, sz, (unsafe_utf32_cstr_t)src, idx, eos)
			: ang_text_encoder_convert<ENCODING, encoding::utf32>::template convert<is_endian_swapped(), false>(dest, sz, (unsafe_utf32_cstr_t)src, idx, eos);
	case encoding::utf32_be:
		return ang::text::is_endian_swapped<encoding::utf32_be>() ? ang_text_encoder_convert<ENCODING, encoding::utf32>::template convert<is_endian_swapped(), true>(dest, sz, (unsafe_utf32_cstr_t)src, idx, eos)
			: ang_text_encoder_convert<ENCODING, encoding::utf32>::template convert<is_endian_swapped(), false>(dest, sz, (unsafe_utf32_cstr_t)src, idx, eos);
	default:break;
	}

	return 0;
}