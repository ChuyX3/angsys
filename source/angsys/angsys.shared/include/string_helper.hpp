#pragma once
#include <angsys.h>

namespace ang
{

	template<text::encoding_enum ENCODING> wsize load_bom(pointer ptr) { return 0; }

	template<> wsize load_bom<text::encoding::utf8>(pointer ptr) {
		alignas(4) static byte utf8_bom[4] = { 0xef, 0xbb, 0xbf, 0x0 };
		return (text::UTF8().compare_until((utf8_char_t const*)ptr, (utf8_char_t const*)utf8_bom) == 3) ? 3 : 0;
	}

	template<> wsize load_bom<text::encoding::utf16_le>(pointer ptr) {
		alignas(4) static byte utf16_le_bom[4] = { 0xff, 0xfe, 0x0, 0x0 };
		return (text::UTF16_LE().compare_until((utf16_char_t const*)ptr, (utf16_char_t const*)utf16_le_bom) == 1) ? 2 : 0;
	}

	template<> wsize load_bom<text::encoding::utf16_be>(pointer ptr) {
		alignas(4) static byte utf16_be_bom[4] = { 0xfe, 0xff, 0x0, 0x0 };
		return (text::UTF16_BE().compare_until((utf16_char_t const*)ptr, (utf16_char_t const*)utf16_be_bom) == 1) ? 2 : 0;
	}

	template<> wsize load_bom<text::encoding::utf32_le>(pointer ptr) {
		alignas(4) static byte utf32_le_bom[8] = { 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
		return (text::UTF32_LE().compare_until((utf32_char_t const*)ptr, (utf32_char_t const*)utf32_le_bom) == 1) ? 4 : 0;
	}

	template<> wsize load_bom<text::encoding::utf32_be>(pointer ptr) {
		alignas(4) static byte utf32_be_bom[8] = { 0x0, 0x0, 0xfe, 0xff, 0x0, 0x0, 0x0, 0x0 };
		return (text::UTF32_BE().compare_until((utf32_char_t const*)ptr, (utf32_char_t const*)utf32_be_bom) == 1) ? 4 : 0;
	}

	template<> wsize load_bom<text::encoding::utf16>(pointer ptr) {
		return text::is_little_endian() ? load_bom<text::encoding::utf16_le>(ptr) : load_bom<text::encoding::utf16_be>(ptr);
	}

	template<> wsize load_bom<text::encoding::utf16_se>(pointer ptr) {
		return text::is_little_endian() ? load_bom<text::encoding::utf16_be>(ptr) : load_bom<text::encoding::utf16_le>(ptr);
	}

	template<> wsize load_bom<text::encoding::utf32>(pointer ptr) {
		return text::is_little_endian() ? load_bom<text::encoding::utf32_le>(ptr) : load_bom<text::encoding::utf32_be>(ptr);
	}

	template<> wsize load_bom<text::encoding::utf32_se>(pointer ptr) {
		return text::is_little_endian() ? load_bom<text::encoding::utf32_be>(ptr) : load_bom<text::encoding::utf32_le>(ptr);
	}

	template<> wsize load_bom<text::encoding::unicode>(pointer ptr) {
		return load_bom<text::native_encoding<text::encoding::unicode>()>(ptr);
	}

	template<typename T>
	long64 str_to_integer(safe_str<T> str, windex& i, int base = 10) {
		;
		long64 res = 0;
		int sig = 1;
		i = 0;
		if (str[0] == '-') {
			sig = -1; i++;
		} else if (str[0] == '+') {
			i++;
		}
		
		switch (base)
		{
		case 2:
			while (str[i] >= '0' && str[i] <= '1') {
				res = (res << 1) | (str[i] - '0'); i++;
			}
		case 8:
			while (str[i] >= '0' && str[i] <= '7') {
				res = (res << 3) | (str[i] - '0'); i++;
			}
		case 10:
		case 16:
			while ((str[i] >= '0' && str[i] <= '7') || (str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F') {
				res = (res << 4) | (str[i] - '0'); i++;
			}
		default:return 0;
		}

		return res*sig;
	}

}
