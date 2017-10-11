#ifndef __ANG_BASE_MBYTE_HPP__
#error ...
#elif !defined __ANG_BASE_MBYTE_INL__
#define __ANG_BASE_MBYTE_INL__

inline ang::utf32_char_t ang::utf8_char::to_utf32(const byte* bytes, int& c) {
	utf32_char_t out;
	switch (bytes[0] & 0XF0)
	{
	case 0XD0:
	case 0XC0:
		out = (char)((bytes[0] & 0X1F) << 6);
		out |= (char)(bytes[1] & 0X3F);
		c = 2;
		break;
	case 0XE0:
		out = (wchar)((bytes[0] & 0X0F) << 12);
		out |= (wchar)((bytes[1] & 0X3F) << 6);
		out |= (wchar)(bytes[2] & 0X3F);
		c = 3;
		break;
	case 0XF0:
		out = (wchar)((bytes[0] & 0X0F) << 18);
		out |= (wchar)((bytes[1] & 0X3F) << 12);
		out = (wchar)((bytes[2] & 0X3F) << 6);
		out |= (wchar)(bytes[3] & 0X3F);
		c = 4;
		break;
	default: out = (char)bytes[0]; c = 1; break;
	}
	return out;
}

inline ang::utf8_char::utf8_char()
{
	from(U'\0');
}

inline ang::utf8_char::utf8_char(const byte* arr)
{
	from(arr);
}

inline ang::utf8_char::utf8_char(ang::utf16_char_t n)
{
	from(n);
}

inline ang::utf8_char::utf8_char(ang::utf32_char_t n)
{
	from(n);
}

inline ang::utf8_char::utf8_char(ang::utf8_char&& b)
{
	value = ang::move(b.value);
}

inline ang::utf8_char::utf8_char(const ang::utf8_char& b)
{
	value = b.value;
}

inline void ang::utf8_char::from(char c) { from((utf32_char_t)c); }

inline void ang::utf8_char::from(wchar c) { from((utf32_char_t)c); }

inline void ang::utf8_char::from(ang::utf16_char_t c)
{
	value = 0;
	if ((c.words[0] & 0XDC00) == 0XDC00) {
		byte temp = ((c.words[0] >> 6) & 0X0F) + 1; //110110ww wwzzzzyy (uuuuu = wwww + 1)
		bytes[0] = 0XF0 | (temp >> 2); //11110uuu
		bytes[1] = 0X80 | (0X30 & (temp << 4)) | (0X0F & (c.words[0] >> 2));
		bytes[2] = 0X80 | (0X30 & (c.words[0] << 4)) | (0X0F & (c.words[1] >> 6));
		bytes[3] = 0X80 | (0X3F & c.words[1]);
	}
	else {
		if (c.words[0] <= 0X7F) {
			bytes[0] = byte(c.words[0]);
		}
		else if (c.words[0] <= 0X7FF) {
			bytes[0] = 0XC0 | (0X1F & (c.words[0] >> 6));
			bytes[1] = 0X80 | (0X3F & c.words[0]);	
		}
		else {
			bytes[0] = 0XE0 | (0X0F & (c.words[0] >> 12));
			bytes[1] = 0X80 | (0X3F & (c.words[0] >> 6));
			bytes[2] = 0X80 | (0X3F & c.words[0]);
		}
	}
}

inline void ang::utf8_char::from(ang::utf32_char_t c)
{
	value = 0;
	if (c <= 0X7F) {
		bytes[0] = (byte)c;
	}
	else if (c <= 0X7FF) {
		bytes[0] = 0XC0 | (0X1F & (c >> 6));
		bytes[1] = 0X80 | (0X3F & c);
	}
	else if (c <= 0XFFFF) {
		bytes[0] = 0XE0 | (0X0F & (c >> 12));
		bytes[1] = 0X80 | (0X3F & (c >> 6));
		bytes[2] = 0X80 | (0X3F & c);
	}
	else {
		bytes[0] = 0XF0 | (0X07 & (c >> 18));
		bytes[1] = 0X80 | (0X3F & (c >> 12));
		bytes[2] = 0X80 | (0X3F & (c >> 6));
		bytes[3] = 0X80 | (0X3F & c);
	}
}

inline void ang::utf8_char::from(const byte* c)
{
	value = 0;
	switch (c[0] & 0XF0)
	{
	case 0XD0:
	case 0XC0:
		bytes[0] = c[0];
		bytes[1] = c[1];
		break;
	case 0XE0:
		bytes[0] = c[0];
		bytes[1] = c[1];
		bytes[2] = c[2];
		break;
	case 0XF0:
		bytes[0] = c[0];
		bytes[1] = c[1];
		bytes[2] = c[2];
		bytes[3] = c[3];
		break;
	default: bytes[0] = c[0]; break;
	}
}

inline ang::utf32_char_t ang::utf8_char::to_utf32()const
{
	utf32_char_t out;
	switch (bytes[0] & 0XF0)
	{
	case 0XD0:
	case 0XC0:
		out = (char)((bytes[0] & 0X1F) << 6);
		out |= (char)(bytes[1] & 0X3F);
		break;
	case 0XE0:
		out = (wchar)((bytes[0] & 0X0F) << 12);
		out |= (wchar)((bytes[1] & 0X3F) << 6);
		out |= (wchar)(bytes[2] & 0X3F);
		break;
	case 0XF0:
		out = (wchar)((bytes[0] & 0X0F) << 18);
		out |= (wchar)((bytes[1] & 0X3F) << 12);
		out = (wchar)((bytes[2] & 0X3F) << 6);
		out |= (wchar)(bytes[3] & 0X3F);
		break;
	default: out = (char)bytes[0]; break;
	}
	return out;
}

inline int ang::utf8_char::size()const
{
	if ((bytes[0] | 0X7F) == 0X7F)//0X7F = 01111111
		return 1;
	else if ((bytes[0] | 0X1F) == 0XDF)//0X7F = 11011111
		return 2;
	else if ((bytes[0] | 0X0F) == 0XEF)//0XEF = 11101111
		return 3;
	else//if((bytes[0]|0X07)==0XF7)
		return 4;
}

inline ang::utf8_char& ang::utf8_char::operator = (ang::utf8_char&& byte)
{
	value = ang::move(byte.value);
	return *this;
}

inline ang::utf8_char& ang::utf8_char::operator = (const ang::utf8_char& byte)
{
	value = byte.value;
	return *this;
}

inline ang::utf8_char& ang::utf8_char::operator = (ang::utf16_char_t val)
{
	from(val);
	return *this;
}

inline ang::utf8_char& ang::utf8_char::operator = (ang::utf32_char_t val)
{
	from(val);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////

inline ang::utf32_char_t ang::utf16_char::to_utf32(const word* words, int& c, bool swap) {
	utf32_char_t out;
	if (swap)
	{
		if ((swap_endian(words[0]) & 0XDC00) == 0XDC00) {
			out = (dword)(swap_endian(words[0]) & 0X03FF);
			out |= (dword)((swap_endian(words[1]) & 0X03FF) << 10);
			out += 0X10000;
		}
		else {
			out = (dword)swap_endian(words[0]);
		}
	}
	else
	{
		if ((words[0] & 0XDC00) == 0XDC00) {
			out = (dword)(words[0] & 0X03FF);
			out |= (dword)((words[1] & 0X03FF) << 10);
			out += 0X10000;
		}
		else {
			out = (dword)(words[0]);
		}
	}
	return out;
}

inline ang::utf16_char::utf16_char()
{
	from(0UL);
}

inline ang::utf16_char::utf16_char(ang::utf8_char_t n)
{
	from(n);
}

inline ang::utf16_char::utf16_char(ang::utf32_char_t n)
{
	from(n);
}

inline ang::utf16_char::utf16_char(ang::utf16_char&& b)
{
	value = ang::move(b.value);
}

inline ang::utf16_char::utf16_char(const ang::utf16_char& b)
{
	value = b.value;
}

inline void ang::utf16_char::from(ang::utf8_char_t c)
{
	value = 0;
	word temp;
	switch (c.bytes[0] & 0XF0)
	{
	case 0XD0://0X110x -> 0X1101
	case 0XC0://0X110x -> 0X1100
		words[0] = (word)(int)((c.bytes[0] & 0X1F) << 6);
		words[0] |= (word)(int)(c.bytes[1] & 0X3F);
		break;
	case 0XE0:
		words[0] = (word)(int)((c.bytes[0] & 0X0F) << 12);
		words[0] |= (word)(int)((c.bytes[1] & 0X3F) << 6);
		words[0] |= (word)(int)(c.bytes[2] & 0X3F);
		break;
	case 0XF0:
		words[0] = 0XDC00 | (word)(int)(c.bytes[3] & 0X3F);
		words[1] |= (word)(int)((c.bytes[2] & 0X0F) << 6);

		words[1] = 0XD800 | (word)(int)((c.bytes[2] & 0X30) >> 4);
		words[1] |= (word)(int)((c.bytes[1] & 0X0F) << 2);
		temp = (word)(int)((c.bytes[1] & 0X30) >> 4);
		temp |= (word)(int)((c.bytes[0] & 0X07) << 2);
		words[1] |= (temp - 1) << 6;

		break;
	default:
		words[0] = (word)(int)c.bytes[0];
		break;
	}
}

inline void ang::utf16_char::from(ang::utf32_char_t c)
{
	value = 0;
	if (c > 0XFFFF) {
		words[0] = 0XDC00 | (c & 0X02FF);
		words[1] = 0XD800 | (((c - 0X10000) >> 10) & 0X02FF);
	}
	else
		words[c] = (word)c;
}

inline ang::utf32_char_t ang::utf16_char::to_utf32()const
{
	utf32_char_t out;
	if ((words[0] & 0XDC00) == 0XDC00) {
		out = (dword)(words[0] & 0X03FF);
		out |= (dword)((words[1] & 0X03FF) << 10);
		out += 0X10000;
	}
	else {
		out = (dword)(int)(words[0]);
	}
	return out;
}

inline int ang::utf16_char::size()const
{
	return ((words[0] & 0XDC00) == 0XDC00) ? 2 : 1;
}

inline ang::utf16_char& ang::utf16_char::operator = (ang::utf16_char&& byte)
{
	value = ang::move(byte.value);
	return *this;
}

inline ang::utf16_char& ang::utf16_char::operator = (const ang::utf16_char& byte)
{
	value = byte.value;
	return *this;
}

inline ang::utf16_char& ang::utf16_char::operator = (ang::utf8_char_t val)
{
	from(val);
	return *this;
}

inline ang::utf16_char& ang::utf16_char::operator = (ang::utf32_char_t val)
{
	from(val);
	return *this;
}


#endif//__ANG_BASE_MBYTE_INL__
