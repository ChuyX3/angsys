#pragma once

template<ang::text::encoding_enum ENCODING>
wsize ang::text::ang_text_encoder<ENCODING>::length(typename ang::text::char_type_by_encoding<ENCODING>::cstr_t cstr)
{
	wsize c = 0;
	if (cstr == null)return 0;
	while (cstr[c] != 0)c++;
	return c;
}

template<>
wsize ang::text::ang_text_encoder<ang::text::encoding_enum::utf8>::length(typename ang::text::char_type_by_encoding<ang::text::encoding_enum::utf8>::cstr_t cstr)
{
	wsize c = 0;
	if (cstr == null)return 0;
	while (cstr[c] != 0 && (cstr[c] != 0XC0 || cstr[c + 1] != 0X80)) {
		switch (cstr[0] & 0XF0)
		{
		case 0XC0: c += 2; break;
		case 0XE0: c += 3; break;
		case 0XF0: c += 4; break;
		default: c++; break;
		}
	}
	return c;
}

template<> 
wsize ang::text::ang_text_encoder<ang::text::encoding_enum::utf16>::length(typename ang::text::char_type_by_encoding<ang::text::encoding_enum::utf16>::cstr_t cstr)
{
	wsize c = 0;
	if (cstr == null)return 0;
	while (cstr[c] != 0) {
		if ((cstr[c] & 0XFC00) == 0XDC00)
			c += 2;
		else c++;
	}
	return c;
}

template<>
wsize ang::text::ang_text_encoder<ang::text::encoding_enum::utf16_le>::length(typename ang::text::char_type_by_encoding<ang::text::encoding_enum::utf16>::cstr_t cstr)
{
	wsize c = 0;
	static const word temp1 = is_endian_swapped() ? 0X00FC : 0XFC00, temp2 = is_endian_swapped() ? 0X00DC : 0XDC00;
	if (cstr == null)return 0;
	while (cstr[c] != 0) {
		if ((cstr[c] & temp1) == temp2)
			c += 2;
		else c++;
	}
	return c;
}

template<>
wsize ang::text::ang_text_encoder<ang::text::encoding_enum::utf16_be>::length(typename ang::text::char_type_by_encoding<ang::text::encoding_enum::utf16>::cstr_t cstr)
{
	wsize c = 0;
	static const word temp1 = is_endian_swapped() ? 0X00FC : 0XFC00, temp2 = is_endian_swapped() ? 0X00DC : 0XDC00;
	if (cstr == null)return 0;
	while (cstr[c] != 0) {
		if ((cstr[c] & temp1) == temp2)
			c += 2;
		else c++;
	}
	return c;
}