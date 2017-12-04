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

	template<bool SWAP, typename T>
	long64 str_to_integer(safe_str<const T> str, windex& i, int base = 10) {
		long64 res = 0; int sig = 1; i = 0; char32_t space = 0;

		space = text::swap_endian<SWAP>(str[i]);
		while (space == ' ' || space == '\t' || space == '\n' || space == '\r')
			space = text::swap_endian<SWAP>(str[i++]);

		if (text::swap_endian<SWAP>(str[i]) == '-') {
			sig = -1; i++;
		} else if (text::swap_endian<SWAP>(str[i]) == '+') {
			i++;
		}
		
		switch (base)
		{
		case 2:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '1')
				res = (res << 1) | (text::swap_endian<SWAP>(str[i++]) - '0');
			if (text::swap_endian<SWAP>(str[i]) == 'b')i++;
			break;
		case 8:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '7')
				res = (res << 3) | (text::swap_endian<SWAP>(str[i++]) - '0'); 
			if (text::swap_endian<SWAP>(str[i]) == 'o')i++;
			break;
		case 10:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '9')
				res = (res * 10) + (text::swap_endian<SWAP>(str[i++]) - '0');
			if (text::swap_endian<SWAP>(str[i]) == 'd')i++;
			break;
		case 16:
			LOOP:
				if(text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '7')
					res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - '0');
				else if(text::swap_endian<SWAP>(str[i]) >= 'a' && text::swap_endian<SWAP>(str[i]) <= 'f')
					res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - 'a' + 10);
				else if (text::swap_endian<SWAP>(str[i]) >= 'A' && text::swap_endian<SWAP>(str[i]) <= 'F')
					res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - 'A' + 10);
				else goto END;
				goto LOOP;
			END:
			if (text::swap_endian<SWAP>(str[i]) == 'h')i++;
			break;
		default:return 0;
		}

		return res*sig;
	}

	template<bool SWAP, typename T>
	ulong64 str_to_uinteger(safe_str<const T> str, windex& i, int base = 10) {
		ulong64 res = 0; char32_t space = 0;
		i = 0;

		space = text::swap_endian<SWAP>(str[i]);
		while (space == ' ' || space == '\t' || space == '\n' || space == '\r')
			space = text::swap_endian<SWAP>(str[i++]);

		switch (base)
		{
		case 2:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '1') {
				res = (res << 1) | (text::swap_endian<SWAP>(str[i++]) - '0');
			}break;
		case 8:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '7') {
				res = (res << 3) | (text::swap_endian<SWAP>(str[i++]) - '0');
			}break;
		case 10:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '9') {
				res = (res * 10) + (text::swap_endian<SWAP>(str[i++]) - '0');
			}break;
		case 16:
		LOOP:
			if (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '7')
				res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - '0');
			else if (text::swap_endian<SWAP>(str[i]) >= 'a' && text::swap_endian<SWAP>(str[i]) <= 'f')
				res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - 'a' + 10);
			else if (text::swap_endian<SWAP>(str[i]) >= 'A' && text::swap_endian<SWAP>(str[i]) <= 'F')
				res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - 'A' + 10);
			else goto END;
			goto LOOP;
		END:
			break;
		default:return 0;
		}

		return res;
	}

	template<bool SWAP, typename T>
	double str_to_floating(safe_str<const T> str, windex& i, bool ex = false) {
		double res = 0, dec = 0.1; int sig = 1; i = 0; char32_t space;

		space = text::swap_endian<SWAP>(str[i]);
		while (space == ' ' || space == '\t' || space == '\n' || space == '\r')
			space = text::swap_endian<SWAP>(str[i++]);

		if (text::swap_endian<SWAP>(str[i]) == '-') {
			sig = -1; i++;
		} else if (text::swap_endian<SWAP>(str[i]) == '+') {
			i++;
		}

		while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '9') {
			res = (res * 10) + (text::swap_endian<SWAP>(str[i]) - '0');
			i++;
		}

		if (text::swap_endian<SWAP>(str[i]) != '.')
			return res*sig;
		i++;

		while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '9') {
			res = res + (text::swap_endian<SWAP>(str[i]) - '0') * dec;
			dec /= 10.0;
			i++;
		}
		
		return res*sig;
	}

	long64 str_to_integer(raw_str_t str, windex& i, int base = 10) {
		switch (str.encoding())
		{
		case text::encoding::ascii: return str_to_integer<false>(str.to_cstr<char>(), i, base);
		case text::encoding::utf8: return str_to_integer<false>(str.to_cstr<mchar>(), i, base);
		case text::encoding::utf16_le: return text::is_little_endian()? str_to_integer<false>(str.to_cstr<char16_t>(), i, base) : str_to_integer<true>(str.to_cstr<char16_t>(), i, base);
		case text::encoding::utf16_be:return text::is_little_endian() ? str_to_integer<true>(str.to_cstr<char16_t>(), i, base) : str_to_integer<false>(str.to_cstr<char16_t>(), i, base);
		case text::encoding::utf16_se:return str_to_integer<true>(str.to_cstr<char16_t>(), i, base);
		case text::encoding::utf16: return str_to_integer<false>(str.to_cstr<char16_t>(), i, base);
		case text::encoding::utf32_le: return text::is_little_endian() ? str_to_integer<false>(str.to_cstr<char32_t>(), i, base) : str_to_integer<true>(str.to_cstr<char32_t>(), i, base);
		case text::encoding::utf32_be:return text::is_little_endian() ? str_to_integer<true>(str.to_cstr<char32_t>(), i, base) : str_to_integer<false>(str.to_cstr<char32_t>(), i, base);
		case text::encoding::utf32_se:return str_to_integer<true>(str.to_cstr<char32_t>(), i, base);
		case text::encoding::utf32: return str_to_integer<false>(str.to_cstr<char32_t>(), i, base);
		default: return 0;
		}
	}

	ulong64 str_to_uinteger(raw_str_t str, windex& i, int base = 10) {
		switch (str.encoding())
		{
		case text::encoding::ascii: return str_to_uinteger<false>(str.to_cstr<char>(), i, base);
		case text::encoding::utf8: return str_to_uinteger<false>(str.to_cstr<mchar>(), i, base);
		case text::encoding::utf16_le: return text::is_little_endian() ? str_to_uinteger<false>(str.to_cstr<char16_t>(), i, base) : str_to_uinteger<true>(str.to_cstr<char16_t>(), i, base);
		case text::encoding::utf16_be:return text::is_little_endian() ? str_to_uinteger<true>(str.to_cstr<char16_t>(), i, base) : str_to_uinteger<false>(str.to_cstr<char16_t>(), i, base);
		case text::encoding::utf16_se:return str_to_uinteger<true>(str.to_cstr<char16_t>(), i, base);
		case text::encoding::utf16: return str_to_uinteger<false>(str.to_cstr<char16_t>(), i, base);
		case text::encoding::utf32_le: return text::is_little_endian() ? str_to_uinteger<false>(str.to_cstr<char32_t>(), i, base) : str_to_uinteger<true>(str.to_cstr<char32_t>(), i, base);
		case text::encoding::utf32_be:return text::is_little_endian() ? str_to_uinteger<true>(str.to_cstr<char32_t>(), i, base) : str_to_uinteger<false>(str.to_cstr<char32_t>(), i, base);
		case text::encoding::utf32_se:return str_to_uinteger<true>(str.to_cstr<char32_t>(), i, base);
		case text::encoding::utf32: return str_to_uinteger<false>(str.to_cstr<char32_t>(), i, base);
		default: return 0;
		}
	}

	double str_to_float(raw_str_t str, windex& i, bool exp = false) {
		switch (str.encoding())
		{
		case text::encoding::ascii: return str_to_floating<false>(str.to_cstr<char>(), i, exp);
		case text::encoding::utf8: return str_to_floating<false>(str.to_cstr<mchar>(), i, exp);
		case text::encoding::utf16_le: return text::is_little_endian() ? str_to_floating<false>(str.to_cstr<char16_t>(), i, exp) : str_to_floating<true>(str.to_cstr<char16_t>(), i, exp);
		case text::encoding::utf16_be:return text::is_little_endian() ? str_to_floating<true>(str.to_cstr<char16_t>(), i, exp) : str_to_floating<false>(str.to_cstr<char16_t>(), i, exp);
		case text::encoding::utf16_se:return str_to_floating<true>(str.to_cstr<char16_t>(), i, exp);
		case text::encoding::utf16: return str_to_floating<false>(str.to_cstr<char16_t>(), i, exp);
		case text::encoding::utf32_le: return text::is_little_endian() ? str_to_floating<false>(str.to_cstr<char32_t>(), i, exp) : str_to_floating<true>(str.to_cstr<char32_t>(), i, exp);
		case text::encoding::utf32_be:return text::is_little_endian() ? str_to_floating<true>(str.to_cstr<char32_t>(), i, exp) : str_to_floating<false>(str.to_cstr<char32_t>(), i, exp);
		case text::encoding::utf32_se:return str_to_floating<true>(str.to_cstr<char32_t>(), i, exp);
		case text::encoding::utf32: return str_to_floating<false>(str.to_cstr<char32_t>(), i, exp);
		default: return 0;
		}
	}
}
