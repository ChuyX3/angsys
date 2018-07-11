#pragma once

template<ang::text::encoding_enum ENCODING
	, ang::text::encoding_enum OTHER_ENCODING
	, bool MULTIBYTE = ang::xor_operator<ang::text::ang_text_encoder<ENCODING>::is_multibyte(), ang::text::ang_text_encoder<OTHER_ENCODING>::is_multibyte()>()>
struct ang_text_encoder_size {
	static wsize size(typename ang::text::char_type_by_encoding<OTHER_ENCODING>::cstr_t cstr) {
		wsize i = 0, j = 0;
		if (cstr == null)
			goto END;
	LOOP:
		if (cstr[j] == 0) goto END;
		i +=
		ang::text::converter<typename ang::text::char_type_by_encoding<ENCODING>::char_t, typename ang::text::char_type_by_encoding<OTHER_ENCODING>::char_t>::template
			size<ang::text::ang_text_encoder<OTHER_ENCODING>::is_endian_swapped()>(cstr, j);
		goto LOOP;
	END:
		return i;
	}
};

//template<ang::text::encoding_enum ENCODING>
//struct ang_text_encoder_size<ENCODING, ang::text::encoding::utf8> {
//	static wsize size(typename ang::text::char_type_by_encoding<ang::text::encoding_enum::utf8>::cstr_t cstr) {
//		wsize c = 0;
//		while (cstr[0] != 0 && (cstr[0] != 0XC0 || cstr[1] != 0X80)) {
//			switch (cstr[0] & 0XF0)
//			{
//			case 0XC0: cstr += 2; break;
//			case 0XE0: cstr += 3; break;
//			case 0XF0: cstr += 4; break;
//			default: cstr++; break;
//			}	
//			c++;
//		}
//		return c;
//	}
//};
//
//template<> struct ang_text_encoder_size<ang::text::encoding::utf8, ang::text::encoding::utf8> {
//	static wsize size(typename ang::text::char_type_by_encoding<ang::text::encoding_enum::utf8>::cstr_t cstr) {
//		wsize c = 0;
//		while (cstr[c] != 0)c++;
//		return c;
//	}
//};
//
//template<> struct ang_text_encoder_size<ang::text::encoding::utf16, ang::text::encoding::utf8> {
//	static wsize size(typename ang::text::char_type_by_encoding<ang::text::encoding_enum::utf8>::cstr_t cstr) {
//		wsize c = 0;
//		while (cstr[0] != 0 && (cstr[0] != 0XC0 || cstr[1] != 0X80)) {
//			switch (cstr[0] & 0XF0)
//			{
//			default: cstr++; c++; break;
//			case 0XC0: cstr += 2; c++; break;
//			case 0XE0: cstr += 3; c++; break;
//			case 0XF0: cstr += 4; c += 2; break;
//			}
//		}
//		return c;
//	}
//};
//
//template<ang::text::encoding_enum ENCODING>
//struct ang_text_encoder_size<ang::text::encoding::utf16, ang::text::encoding::utf16> {
//	static wsize size(typename ang::text::char_type_by_encoding<ang::text::encoding_enum::utf16>::cstr_t cstr) {
//		wsize c = 0;
//		while (cstr[0] != 0) {
//			if ((cstr[0] & 0XDE00) != 0XDE00) cstr++;
//			else cstr += 2;
//			c++;
//		}
//		return c;
//	}
//};
//
//
//template<ang::text::encoding_enum ENCODING>
//struct ang_text_encoder_size<ENCODING, ang::text::encoding::utf16> {
//	static wsize size(typename ang::text::char_type_by_encoding<ang::text::encoding_enum::utf16>::cstr_t cstr) {
//		wsize c = 0;
//		while (cstr[0] != 0) {
//			if ((cstr[0] & 0XDE00) != 0XDE00) cstr++;
//			else cstr += 2;
//			c++;
//		}
//		return c;
//	}
//};
//
//template<ang::text::encoding_enum ENCODING>
//struct ang_text_encoder_size<ENCODING, ang::text::encoding::utf16_se> {
//	static wsize size(typename ang::text::char_type_by_encoding<ang::text::encoding::utf16>::cstr_t cstr) {
//		wsize c = 0;
//		while (cstr[0] != 0) {
//			if ((cstr[0] & 0X00DE) != 0X00DE) cstr++;
//			else cstr += 2;
//			c++;
//		}
//		return c;
//	}
//};
//
//template<ang::text::encoding_enum OTHER_ENCODING>
//struct ang_text_encoder_size<ang::text::encoding::utf8, OTHER_ENCODING> {
//	static wsize size(typename ang::text::char_type_by_encoding<OTHER_ENCODING>::cstr_t cstr) {
//		wsize c = 0;
//		while (cstr[0] != 0) {
//			if ((dword)cstr[0] > (dword)0XFFFF)
//				c += 4;
//			else if ((dword)cstr[0] > (dword)0X7FF)
//				c += 3;
//			else if ((dword)cstr[0] > (dword)0X7F)
//				c += 2;
//			else c++;
//			cstr++;
//		}
//		return c;
//	}
//};
//
//template<ang::text::encoding_enum OTHER_ENCODING>
//struct ang_text_encoder_size<ang::text::encoding::utf16, OTHER_ENCODING> {
//	static wsize size(typename ang::text::char_type_by_encoding<OTHER_ENCODING>::cstr_t cstr) {
//		wsize c = 0;
//		while (cstr[0] != 0) {
//			if ((dword)cstr[0] > (dword)0XFFFF)
//				c += 2;
//			else c++;
//			cstr++;
//		}
//		return c;
//	}
//};
//
//
//template<> struct ang_text_encoder_size<ang::text::encoding::utf8, ang::text::encoding::utf16> {
//	static wsize size(typename ang::text::char_type_by_encoding<ang::text::encoding_enum::utf16>::cstr_t cstr) {
//		wsize c = 0;
//		while (cstr[0] != 0) {
//			if ((cstr[0] & 0XDE00) != 0XDE00) {
//				c += 4;
//				cstr += 2;
//				continue;
//			}
//			else if (cstr[0] > 0X7FF)
//				c += 3;
//			else if (cstr[0] > 0X7F)
//				c += 2;
//			else c++;
//			cstr++;
//		}
//		return c;
//	}
//};
//
//template<> struct ang_text_encoder_size<ang::text::encoding::utf8, ang::text::encoding::utf16_se> {
//	static wsize size(typename ang::text::char_type_by_encoding<ang::text::encoding_enum::utf16>::cstr_t cstr) {
//		wsize c = 0;
//		while (cstr[0] != 0) {
//			if ((cstr[0] & 0X00DE) != 0X00DE) {
//				c += 4;
//				cstr += 2;
//				continue;
//			}
//			else if (cstr[0] > 0X7FF)
//				c += 3;
//			else if (cstr[0] > 0X7F)
//				c += 2;
//			else c++;
//			cstr++;
//		}
//		return c;
//	}
//};

template<ang::text::encoding_enum ENCODING>
wsize ang::text::ang_text_encoder<ENCODING>::size(pointer raw, ang::text::encoding_t format) {

	switch (format.get())
	{
	case encoding::ascii:
		return ang_text_encoder_size<ENCODING, encoding::ascii>::size((typename char_type_by_encoding<encoding::ascii>::cstr_t)raw);
	case encoding::unicode:
		return ang_text_encoder_size<ENCODING, encoding::unicode>::size((typename char_type_by_encoding<encoding::unicode>::cstr_t)raw);
	case encoding::utf8:
		return ang_text_encoder_size<ENCODING, encoding::utf8>::size((typename char_type_by_encoding<encoding::utf8>::cstr_t)raw);
	case encoding::utf16:
		return ang_text_encoder_size<ENCODING, encoding::utf16>::size((typename char_type_by_encoding<encoding::utf16>::cstr_t)raw);
	case encoding::utf16_le:
		return (ang_is_little_endian_platform()) ? ang_text_encoder_size<ENCODING, encoding::utf16>::size((typename char_type_by_encoding<encoding::utf16>::cstr_t)raw)
			: ang_text_encoder_size<ENCODING, encoding::utf16_se>::size((typename char_type_by_encoding<encoding::utf16>::cstr_t)raw);
	case encoding::utf16_be:
		return (!ang_is_little_endian_platform()) ? ang_text_encoder_size<ENCODING, encoding::utf16>::size((typename char_type_by_encoding<encoding::utf16>::cstr_t)raw)
			: ang_text_encoder_size<ENCODING, encoding::utf16_se>::size((typename char_type_by_encoding<encoding::utf16>::cstr_t)raw);
	case encoding::utf32:
	case encoding::utf32_le:
	case encoding::utf32_be:
		return ang_text_encoder_size<ENCODING, encoding::utf32>::size((typename char_type_by_encoding<encoding::utf32>::cstr_t)raw);
	default:return 0;
	}
}