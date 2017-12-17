/*********************************************************************************************************************/
/*   File Name: text_format.cpp                                                                                      */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.hpp"
#include "format_parser.h"

#if defined _DEBUG && defined MEMORY_DEBUGGING
#define NEW ANG_DEBUG_NEW()
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

using namespace ang;
using namespace ang::text;

////////////////////////////////////////////////////////////////////////

text_format::text_format()
	: flags(0)
{
}

text_format::text_format(cstr_t format)
	: flags(0)
{
	flags = format_parser::parse(ang::move(format));
}

text_format::text_format(const text_format& format)
	: flags(format.flags)
{

}

text_format::~text_format()
{
}

text_format::target text_format::format_target()const
{
	return GET_FLAG_TARGET(flags);
}

void text_format::format(cstr_t format)
{
	flags = format_parser::parse(ang::move(format));
}

//string text_format::format()const
//{
//	string out;
//	if (format_parser::format(flags, out))
//		return out;
//	return ""_s;
//}

qword text_format::format_flags()const
{
	return flags;
}

text_format& text_format::operator = (const text_format& format)
{
	flags = format.flags;
	return *this;
}

bool format_parser::format(qword flags, string& out)
{
	bool b = false;
	out.clean();
	switch (GET_FLAG_TARGET(flags))
	{
	case text_format::character:
		out += "{char:"_s;
		if (flags&FLAG_CHAR_LOWERCASE)
		{
			out += "l"_s;
		}
		else if (flags&FLAG_CHAR_UPPERCASE)
		{
			out += "u"_s;
		}
		out += "}"_s;
		break;
	case text_format::text:
		out += "{text:"_s;
		if (flags&FLAG_CHAR_LOWERCASE)
		{
			out += "l"_s;
		}
		else if (flags&FLAG_CHAR_UPPERCASE)
		{
			out += "u"_s;
		}
		out += "}"_s;
		break;
	case text_format::signed_integer:
		out += "{signed:"_s;
		if ((flags&FLAG_SIGNED_BIN) == FLAG_SIGNED_BIN)
		{
			out += "b"_s;
			b = true;
		}
		else if ((flags&FLAG_SIGNED_HEX_L) == FLAG_SIGNED_HEX_L)
		{
			out += "x"_s;
			b = true;
		}
		else if ((flags&FLAG_SIGNED_HEX_U) == FLAG_SIGNED_HEX_U)
		{
			out += "X"_s;
			b = true;
		}
		if (flags&FLAG_SIGNED_SIG)
		{
			if (b)
			{
				out += ",s"_s;
			}
			else
			{
				out += "s"_s;
			}
			b = true;
		}
		if (GET_FLAG_FILL(flags) != 0)
		{
			if (b)
			{
				const char str[4] = { ',', 'f', GET_FLAG_FILL(flags), 0 };
				out += cstr_t(str);
			}
			else
			{
				const char str[3] = { 'f', GET_FLAG_FILL(flags), 0 };
				out += cstr_t(str);
			}
			b = true;
		}
		if (GET_FLAG_MAX(flags) != 0)
		{
			if (b)
			{
				out->concat_format(",n%d"_s, GET_FLAG_MAX(flags));
			}
			else
			{
				out->concat_format("n%d"_s, GET_FLAG_MAX(flags));
			}
		}
		break;
	case text_format::usigned_integer:
		out += "{unsigned:"_s;
		if (flags&FLAG_SIGNED_BIN)
		{
			out += "b"_s;
			b = true;
		}
		else if (flags&FLAG_UNSIGNED_HEX_L)
		{
			out += "x"_s;
			b = true;
		}
		else if (flags&FLAG_UNSIGNED_HEX_U)
		{
			out += "X"_s;
			b = true;
		}
		if (GET_FLAG_FILL(flags) != 0)
		{
			if (b)
			{
				const char str[4] = { ',', 'f', GET_FLAG_FILL(flags), 0 };
				out += cstr_t(str);
			}
			else
			{
				const char str[3] = { 'f', GET_FLAG_FILL(flags), 0 };
				out += cstr_t(str);
			}
			b = true;
		}
		if (GET_FLAG_MAX(flags) != 0)
		{
			if (b)
			{
				out->concat_format(",n%d"_s, GET_FLAG_MAX(flags));
			}
			else
			{
				out->concat_format("n%d"_s, GET_FLAG_MAX(flags));
			}
		}
		break;
	case text_format::floating:
		out += "{floating:"_s;
		if (flags&FLAG_FLOATING_BIN)
		{
			out += "b"_s;
			b = true;
		}
		else if (flags&FLAG_FLOATING_HEX_L)
		{
			out += "x"_s;
			b = true;
		}
		else if (flags&FLAG_FLOATING_HEX_U)
		{
			out += "X"_s;
			b = true;
		}
		if (flags&FLAG_FLOATING_SIG)
		{
			if (b)
			{
				out += ",s"_s;
			}
			else
			{
				out += "s"_s;
			}
			b = true;
		}
		if (flags&FLAG_FLOATING_EXP)
		{
			if (b)
			{
				out += ",e"_s;
			}
			else
			{
				out += "e"_s;
			}
			b = true;
		}
		if (GET_FLAG_FILL(flags) != 0)
		{
			if (b)
			{
				const char str[4] = { ',', 'f', GET_FLAG_FILL(flags), 0 };
				out += cstr_t(str);
			}
			else
			{
				const char str[3] = { 'f', GET_FLAG_FILL(flags), 0 };
				out += cstr_t(str);
			}
			b = true;
		}
		if (GET_FLAG_MAX(flags) != 0)
		{
			if (b)
			{
				out->concat_format(",n%d"_s, GET_FLAG_MAX(flags));
			}
			else
			{
				out->concat_format("n%d"_s, GET_FLAG_MAX(flags));
			}
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}

bool format_parser::parse_char(cstr_t format, wsize& beg, qword& flag)
{
	flag = (qword)text_format::target::character;

	wsize end = format.size();

	while (beg < end)
	{
		char _c = format[beg];
		if (_c == '\t' || _c == ' ')
		{
			beg++;
			continue;
		}
		else if (_c == 'N' || _c == 'n')
		{
			beg++;
			break;
		}
		else if (_c == 'L' || _c == 'l')
		{
			beg++;
			flag |= FLAG_CHAR_LOWERCASE;
			break;
		}
		else if (_c == 'U' || _c == 'u')
		{
			beg++;
			flag |= FLAG_CHAR_UPPERCASE;
			break;
		}
		else
			return false;
	}
	while (beg < end)
	{
		char _c = format[beg];
		if (_c == '\t' || _c == ' ')
		{
			beg++;
			continue;
		}
		else if (_c == '}')
		{
			beg++;
			return true;
		}
	}
	return false;
}

bool format_parser::parse_string(cstr_t format, wsize& beg, qword& flag)
{
	flag = (qword)text_format::target::text;

	wsize end = format.size();

	while (beg < end)
	{
		char _c = format[beg];
		if (_c == '\t' || _c == ' ')
		{
			beg++;
			continue;
		}
		else if (_c == 'N' || _c == 'n')
		{
			beg++;
			break;
		}
		else if (_c == 'L' || _c == 'l')
		{
			beg++;
			flag |= FLAG_CHAR_LOWERCASE;
			break;
		}
		else if (_c == 'U' || _c == 'u')
		{
			beg++;
			flag |= FLAG_CHAR_UPPERCASE;
			break;
		}
		else
			return false;
	}
	while (beg < end)
	{
		char _c = format[beg];
		if (_c == '\t' || _c == ' ')
		{
			beg++;
			continue;
		}
		else if (_c == '}')
		{
			beg++;
			return true;
		}
	}
	return false;
}

bool format_parser::parse_signed(cstr_t format, wsize& beg, qword& flag)
{
	flag = (qword)text_format::target::signed_integer;

	wsize end = format.size();

	bool waitForEnd = false;
	bool notation = false;
	bool fill = false;
	bool max = false;
	bool sign = false;

	while (beg < end)
	{
		char _c = format[beg];

		if (waitForEnd)
		{
			waitForEnd = false;
			while (beg < end)
			{
				char _c = format[beg++];
				if (_c == '\t' || _c == ' ')
					continue;
				else if (_c == '}')
					return true;
				if (_c == ',')
					break;
				else
					return false;
			}
			continue;
		}
		else if (_c == '\t' || _c == ' ')
		{
			beg++;
			continue;
		}
		else if (_c == '}')
		{
			beg++;
			return true;
		}
		else if (_c == 'N' || _c == 'n')
		{
			if (max)
				return false;
			max = true;
			byte nMax = 0;
			char strMax[3] = { 0 };
			beg++;
			uint i = 0;
			while (beg < end)
			{
				char n = format[beg];
				if (n >= '0' && n <= '9')
					strMax[i++] = n;
				else if (i == 0)
					return false;
				else
					break;
				if (i >= 3)
					break;
				beg++;
			}
			nMax = (byte)atoi(strMax);
			flag |= FLAG_SIGNED_MAX(nMax);
			waitForEnd = true;
			continue;
		}
		else if (_c == 'F' || _c == 'f')
		{
			if (fill)
				return false;
			fill = true;
			if ((beg + 1) >= end)
				return false;
			char cFill = format[++beg];
			flag |= FLAG_SIGNED_FILL(cFill);
			beg++;
			waitForEnd = true;
			continue;
		}
		else if (_c == 'X')
		{
			if (notation)
				return false;
			notation = true;
			flag |= FLAG_SIGNED_HEX_U;
			waitForEnd = true;
			beg++;
			continue;
		}
		else if (_c == 'x')
		{
			if (notation)
				return false;
			notation = true;
			flag |= FLAG_SIGNED_HEX_L;
			waitForEnd = true;
			beg++;
			continue;
		}
		else if (_c == 'B' || _c == 'b')
		{
			if (notation)
				return false;
			notation = true;
			flag |= FLAG_SIGNED_BIN;
			waitForEnd = true;
			beg++;
			continue;
		}
		else if (_c == 'S' || _c == 's')
		{
			if (sign)
				return false;
			sign = true;
			flag |= FLAG_SIGNED_SIG;
			waitForEnd = true;
			beg++;
			continue;
		}
		else
			return false;
	}

	return false;
}

bool format_parser::parse_unsigned(cstr_t format, wsize& beg, qword& flag)
{
	flag = (qword)text_format::target::usigned_integer;

	wsize end = format.size();

	bool waitForEnd = false;
	bool notation = false;
	bool fill = false;
	bool max = false;

	while (beg < end)
	{
		char _c = format[beg];

		if (waitForEnd)
		{
			waitForEnd = false;
			while (beg < end)
			{
				char _c = format[beg++];
				if (_c == '\t' || _c == ' ')
					continue;
				else if (_c == '}')
					return true;
				if (_c == ',')
					break;
				else
					return false;
			}
			continue;
		}
		else if (_c == '\t' || _c == ' ')
		{
			beg++;
			continue;
		}
		else if (_c == '}')
		{
			beg++;
			return true;
		}
		else if (_c == 'N' || _c == 'n')
		{
			if (max)
				return false;
			max = true;
			byte nMax = 0;
			char strMax[3] = { 0 };
			beg++;
			uint i = 0;
			while (beg < end)
			{
				char n = format[beg];
				if (n >= '0' && n <= '9')
					strMax[i++] = n;
				else if (i == 0)
					return false;
				else
					break;
				if (i >= 3)
					break;
				beg++;
			}
			nMax = (byte)atoi(strMax);
			flag |= FLAG_UNSIGNED_MAX(nMax);
			waitForEnd = true;
			continue;
		}
		else if (_c == 'F' || _c == 'f')
		{
			if (fill)
				return false;
			fill = true;
			if ((beg + 1) >= end)
				return false;
			flag |= FLAG_UNSIGNED_FILL(format[++beg]);
			beg++;
			waitForEnd = true;
			continue;
		}
		else if (_c == 'X')
		{
			if (notation)
				return false;
			notation = true;
			flag |= FLAG_UNSIGNED_HEX_U;
			waitForEnd = true;
			beg++;
			continue;
		}
		else if (_c == 'x')
		{
			if (notation)
				return false;
			notation = true;
			flag |= FLAG_UNSIGNED_HEX_L;
			waitForEnd = true;
			beg++;
			continue;
		}
		else if (_c == 'B' || _c == 'b')
		{
			if (notation)
				return false;
			notation = true;
			flag |= FLAG_UNSIGNED_BIN;
			waitForEnd = true;
			beg++;
			continue;
		}
		else
			return false;
	}
	return false;
}

bool format_parser::parse_floating(cstr_t format, wsize& beg, qword& flag)
{
	flag = (qword)text_format::target::floating;

	wsize end = format.size();

	bool waitForEnd = false;
	bool notation = false;
	bool fill = false;
	bool max = false;
	bool sign = false;
	bool exp = false;

	while (beg < end)
	{
		char _c = format[beg];

		if (waitForEnd)
		{
			waitForEnd = false;
			while (beg < end)
			{
				char _c = format[beg++];
				if (_c == '\t' || _c == ' ')
					continue;
				else if (_c == '}')
					return true;
				if (_c == ',')
					break;
				else
					return false;
			}
			continue;
		}
		else if (_c == '\t' || _c == ' ')
		{
			beg++;
			continue;
		}
		else if (_c == '}')
		{
			beg++;
			return true;
		}
		else if (_c == 'N' || _c == 'n')
		{
			if (max)
				return false;
			max = true;
			bool dec = false;
			byte nMax = 0;
			char strMax[3] = { 0 };
			beg++;
			uint i = 0;
			while (beg < end)
			{
				char n = format[beg];
				if (n >= '0' && n <= '9')
					strMax[i++] = n;
				else if (n == '.') {
					dec = true;
					nMax |= (byte)min(atoi(strMax),0XF) << 4;
					i = 0;
				}
				else if (i == 0)
					return false;
				else
					break;
				if (i >= 3)
				{
					if (dec)
						break;
					else if (i > 3)
						break;
				}
				beg++;
			}
			if (dec)
				nMax |= (byte)min(atoi(strMax), 0XF);
			else
				nMax |= (byte)min(atoi(strMax), 0XF) << 4;
			flag |= FLAG_FLOATING_MAX(nMax);
			waitForEnd = true;
			continue;
		}
		else if (_c == 'F' || _c == 'f')
		{
			if (fill)
				return false;
			fill = true;
			if ((beg + 1) >= end)
				return false;
			flag |= FLAG_FLOATING_FILL(format[++beg]);
			beg++;
			waitForEnd = true;
			continue;
		}
		else if (_c == 'X')
		{
			if (notation)
				return false;
			notation = true;
			flag |= FLAG_FLOATING_HEX_U;
			waitForEnd = true;
			beg++;
			continue;
		}
		else if (_c == 'x')
		{
			if (notation)
				return false;
			notation = true;
			flag |= FLAG_FLOATING_HEX_L;
			waitForEnd = true;
			beg++;
			continue;
		}
		else if (_c == 'B' || _c == 'b')
		{
			if (notation)
				return false;
			notation = true;
			flag |= FLAG_FLOATING_BIN;
			waitForEnd = true;
			beg++;
			continue;
		}
		else if (_c == 'S' || _c == 's')
		{
			if (sign)
				return false;
			sign = true;
			flag |= FLAG_FLOATING_SIG;
			waitForEnd = true;
			beg++;
			continue;
		}
		else if (_c == 'E' || _c == 'e')
		{
			if (exp)
				return false;
			exp = true;
			flag |= FLAG_FLOATING_EXP;
			waitForEnd = true;
			beg++;
			continue;
		}
		else
			return false;
	}

	return false;
}

qword format_parser::parse(cstr_t format)
{
	qword flag = 0;
	if (!format.cstr())
		return 0;

	//format->lowercase();

	wsize beg = 0, pos = 0, end = format.size();

	bool isOpened = false;

	cstr_t _char = "char";
	cstr_t _string = "string";
	cstr_t _signed = "signed";
	cstr_t _unsigned = "unsigned";
	cstr_t _floating = "floating";

	ang_byte_bits_t check_fail;
	check_fail.value = 0X1F;//{ true, true, true, true, true };

	while (beg < end)
	{
		//removing spaces
		if (format[beg] == ' ' || format[beg] == '\t')
		{
			beg++;
			continue;
		}

		//finding open character "{"
		if (format[beg] == '{')
		{
			//Only one open character "{"
			if (isOpened)
				return FLAG_INVALID_FORMAT;

			isOpened = true;
			beg++;
			continue;
		}

		if (isOpened)
		{
			pos = 0;
			do
			{
				char _c1 = 0;
				char _c2 = format[beg + pos];

				if (check_fail.bits.bit0)
				{
					_c1 = _char[pos];
					if (_c1 == 0)
					{
						if ((_c2 == ' ') || (_c2 == '\t'))
						{
							pos++;
							continue;
						}
						else if (_c2 == ':')
						{
							beg += pos + 1;
							break;
						}
						else
							check_fail.bits.bit0 = false;
					}
					else if (_c1 != _c2)
						check_fail.bits.bit0 = false;
				}

				if (check_fail.bits.bit1)
				{
					_c1 = _string[pos];
					if (_c1 == 0)
					{
						if ((_c2 == ' ') || (_c2 == '\t'))
						{
							pos++;
							continue;
						}
						else if (_c2 == ':')
						{
							beg += pos + 1;
							break;
						}
						else
							check_fail.bits.bit1 = false;
					}
					else if (_c1 != _c2)
						check_fail.bits.bit1 = false;
				}
				if (check_fail.bits.bit2)
				{
					_c1 = _signed[pos];
					if (_c1 == 0)
					{
						if ((_c2 == ' ') || (_c2 == '\t'))
						{
							pos++;
							continue;
						}
						else if (_c2 == ':')
						{
							beg += pos + 1;
							break;
						}
						else
							check_fail.bits.bit2 = false;
					}
					else if (_c1 != _c2)
						check_fail.bits.bit2 = false;
				}
				if (check_fail.bits.bit3)
				{
					_c1 = _unsigned[pos];
					if (_c1 == 0)
					{
						if ((_c2 == ' ') || (_c2 == '\t'))
						{
							pos++;
							continue;
						}
						else if (_c2 == ':')
						{
							beg += pos + 1;
							break;
						}
						else
							check_fail.bits.bit3 = false;
					}
					else if (_c1 != _c2)
						check_fail.bits.bit3 = false;
				}
				if (check_fail.bits.bit4)
				{
					_c1 = _floating[pos];
					if (_c1 == 0)
					{
						if ((_c2 == ' ') || (_c2 == '\t'))
						{
							pos++;
							continue;
						}
						else if (_c2 == ':')
						{
							beg += pos + 1;
							break;
						}
						else
							check_fail.bits.bit4 = false;
					}
					else if (_c1 != _c2)
						check_fail.bits.bit4 = false;
				}

				if (((byte)check_fail.value) == 0)
				{
					return FLAG_INVALID_FORMAT;
				}
				pos++;
			} while ((beg + pos) < end);
			break;
		}
		else
		{
			return FLAG_INVALID_FORMAT;
		}
	}//while

	switch ((byte)check_fail.value)
	{
	case 0X01:
		if (!parse_char(format, beg, flag))
			return FLAG_INVALID_FORMAT;
		break;
	case 0X02:
		if (!parse_string(format, beg, flag))
			return FLAG_INVALID_FORMAT;
		break;
	case 0X04:
		if (!parse_signed(format, beg, flag))
			return FLAG_INVALID_FORMAT;
		break;
	case 0X08:
		if (!parse_unsigned(format, beg, flag))
			return FLAG_INVALID_FORMAT;
		break;
	case 0X10:
		if (!parse_floating(format, beg, flag))
			return FLAG_INVALID_FORMAT;
		break;
	default:
		return FLAG_INVALID_FORMAT;
	}

	return flag;
}

