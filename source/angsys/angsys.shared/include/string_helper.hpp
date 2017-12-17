#pragma once
#include <angsys.hpp>

namespace ang
{

	template<text::encoding_enum ENCODING> wsize load_bom(pointer ptr) { return 0; }

	template<> wsize load_bom<text::encoding::utf8>(pointer ptr);

	template<> wsize load_bom<text::encoding::utf16_le>(pointer ptr);

	template<> wsize load_bom<text::encoding::utf16_be>(pointer ptr);

	template<> wsize load_bom<text::encoding::utf32_le>(pointer ptr);

	template<> wsize load_bom<text::encoding::utf32_be>(pointer ptr);

	template<> wsize load_bom<text::encoding::utf16>(pointer ptr);

	template<> wsize load_bom<text::encoding::utf16_se>(pointer ptr);

	template<> wsize load_bom<text::encoding::utf32>(pointer ptr);

	template<> wsize load_bom<text::encoding::utf32_se>(pointer ptr);

	template<> wsize load_bom<text::encoding::unicode>(pointer ptr);



	template<bool SWAP, typename T>
	inline long64 str_to_integer(safe_str<const T> str, windex& i, int base = 10) {
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
	inline ulong64 str_to_uinteger(safe_str<const T> str, windex& i, int base = 10) {
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
	inline double str_to_floating(safe_str<const T> str, windex& i, bool ex = false) {
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

	long64 str_to_integer(raw_str_t str, windex& i, int base = 10);

	ulong64 str_to_uinteger(raw_str_t str, windex& i, int base = 10);

	double str_to_float(raw_str_t str, windex& i, bool exp = false);

	char32_t char_format(char32_t c, text::text_format_t f);
}
