/*********************************************************************************************************************/
/*   File Name: floating.cpp                                                                                         */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/
#include "pch.h"
#include "angsys.hpp"
#include "format_parser.h"
using namespace ang;

namespace ang
{
	static char hexl[18] = { "0123456789abcdefx" };
	static char hexu[18] = { "0123456789ABCDEFX" };
}

value<float> floating::parse(cstr_t cstr)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	char* end;
	return strtof(cstr, &end);
}

value<float> floating::parse(cwstr_t cstr)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	wchar* end;
	return (float)wcstod(cstr, &end);
}

string floating::to_string(value<float> value)
{
	static text::text_format_t format("{floating:n.4}");
	return ang::move(to_string(value, format));
}

wstring floating::to_wstring(value<float> value)
{
	static text::text_format_t format("{floating:n.4}");
	return ang::move(to_wstring(value, format));
}

string floating::to_string(value<float> value, text::text_format_t f)
{
	const byte max_char = 120;
	
	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::floating)
		return "invalid format"_s;

	char out[max_char] = { 0 };
	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format) >> 4;
	int pres = 0X0F & GET_FLAG_MAX(format);
	bool exp = (format & FLAG_FLOATING_EXP) == FLAG_FLOATING_EXP;
	word c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	union
	{
		uint int_value;
		float float_value;
	}union_value;
	union_value.float_value = value;
	if ((FLAG_FLOATING_BIN & format) == FLAG_FLOATING_BIN)
	{
		do
		{
			if (union_value.int_value != 0)
				out[c--] = (0X0001 & union_value.int_value) ? '1' : '0';
			else
				out[c--] = fill;
			union_value.int_value = union_value.int_value >> 1;
			max--;
		} while (union_value.int_value != 0 || max > 0);
	}
	if ((FLAG_FLOATING_HEX_U & format) == FLAG_FLOATING_HEX_U)
	{
		do
		{
			if (union_value.int_value != 0)
			{
				out[c--] = hexu[(byte)(0X000F & union_value.int_value)];
				union_value.int_value = union_value.int_value >> 4;
			}
			else
				out[c--] = fill;
			max--;
		} while (union_value.int_value > 0 || max > 0);
	}
	else if ((FLAG_SIGNED_HEX_L & format) == FLAG_SIGNED_HEX_L)
	{
		do
		{
			if (union_value.int_value != 0)
			{
				out[c--] = hexl[(byte)(0X000F & union_value.int_value)];
				union_value.int_value = union_value.int_value >> 4;
			}
			else
				out[c--] = fill;
			max--;
		} while (union_value.int_value > 0 || max > 0);
	}
	else
	{
		bool neg = false;
		int int_value;
		int iexp = 0;
		if (value < 0.0f)
		{
			neg = true;
			value *= -1.0f;
		}
		if (exp)
		{
			bool bexp = false;
			if (value > 10.0f)
			{
				bexp = false;
				do {
					iexp++;
					value /= 10.0f;
				} while (value > 10.0f);
			}
			else if (value < 1.0f)
			{
				bexp = true;
				do {
					iexp++;
					value *= 10.0f;
				} while (value < 1.0f);
			}
			do
			{
				out[c--] = '0' + iexp % 10;
				iexp /= 10;
			} while (iexp > 0);
			if (bexp)out[c--] = '-';
			out[c--] = 'e';
		}

		int_value = (int)value;
		value = value - float(int_value);

		if (pres > 0)
		{
			c -= pres;
			for (word i = 0; i < pres; i++)
			{
				out[i + c + 1] = '0' + byte(value * 10.0f);
				value *= 10.0f;
				value -= float(int(value));
			}
			out[c--] = '.';
		}

		do
		{
			if (int_value != 0)
			{
				out[c--] = '0' + int_value % 10;
				int_value /= 10;
			}
			else
				out[c--] = fill;
			max--;
		} while (int_value > 0 || max > 0);

		if (neg)
			out[c--] = '-';
		else if ((FLAG_SIGNED_SIG & format) == FLAG_SIGNED_SIG)
			out[c--] = '+';
	}

	return cstr_t(&out[c + 1], max_char - c);

}

wstring floating::to_wstring(value<float> value, text::text_format_t f)
{
	const byte max_char = 120;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::floating)
		return "invalid format"_s;

	char out[max_char] = { 0 };
	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format) >> 4;
	int pres = 0X0F & GET_FLAG_MAX(format);
	bool exp = (format & FLAG_FLOATING_EXP) == FLAG_FLOATING_EXP;
	word c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	union
	{
		uint int_value;
		float float_value;
	}union_value;
	union_value.float_value = value;
	if ((FLAG_FLOATING_BIN & format) == FLAG_FLOATING_BIN)
	{
		do
		{
			if (union_value.int_value != 0)
				out[c--] = (0X0001 & union_value.int_value) ? '1' : '0';
			else
				out[c--] = fill;
			union_value.int_value = union_value.int_value >> 1;
			max--;
		} while (union_value.int_value != 0 || max > 0);
	}
	if ((FLAG_FLOATING_HEX_U & format) == FLAG_FLOATING_HEX_U)
	{
		do
		{
			if (union_value.int_value != 0)
			{
				out[c--] = hexu[(byte)(0X000F & union_value.int_value)];
				union_value.int_value = union_value.int_value >> 4;
			}
			else
				out[c--] = fill;
			max--;
		} while (union_value.int_value > 0 || max > 0);
	}
	else if ((FLAG_SIGNED_HEX_L & format) == FLAG_SIGNED_HEX_L)
	{
		do
		{
			if (union_value.int_value != 0)
			{
				out[c--] = hexl[(byte)(0X000F & union_value.int_value)];
				union_value.int_value = union_value.int_value >> 4;
			}
			else
				out[c--] = fill;
			max--;
		} while (union_value.int_value > 0 || max > 0);
	}
	else
	{
		bool neg = false;
		int int_value;
		int iexp = 0;
		if (value < 0.0f)
		{
			neg = true;
			value *= -1.0f;
		}
		if (exp)
		{
			bool bexp = false;
			if (value > 10.0f)
			{
				bexp = false;
				do {
					iexp++;
					value /= 10.0f;
				} while (value > 10.0f);
			}
			else if (value < 1.0f)
			{
				bexp = true;
				do {
					iexp++;
					value *= 10.0f;
				} while (value < 1.0f);
			}
			do
			{
				out[c--] = '0' + iexp % 10;
				iexp /= 10;
			} while (iexp > 0);
			if (bexp)out[c--] = '-';
			out[c--] = 'e';
		}

		int_value = (int)value;
		value = value - float(int_value);

		if (pres > 0)
		{
			c -= pres;
			for (word i = 0; i < pres; i++)
			{
				out[i + c + 1] = '0' + byte(value * 10.0f);
				value *= 10.0f;
				value -= float(int(value));
			}
			out[c--] = '.';
		}

		do
		{
			if (int_value != 0)
			{
				out[c--] = '0' + int_value % 10;
				int_value /= 10;
			}
			else
				out[c--] = fill;
			max--;
		} while (int_value > 0 || max > 0);

		if (neg)
			out[c--] = '-';
		else if ((FLAG_SIGNED_SIG & format) == FLAG_SIGNED_SIG)
			out[c--] = '+';
	}

	return cstr_t(&out[c + 1], max_char - c);
}


floating::value_wrapper()
	: value<float>()
{

}

floating::value_wrapper(float val)
	: value<float>(val)
{

}

floating::value_wrapper(floating const& other)
	: value<float>(other)
{

}

floating::value_wrapper(value<float> const& other)
	: value<float>(other)
{

}

floating::~value_wrapper()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::floating);
ANG_IMPLEMENT_OBJECTNAME(ang::floating);

bool floating::is_kind_of(type_name_t type)const
{
	return (type == type_of<floating>())
		|| object::is_kind_of(type)
		|| (type == type_of<value<float>>());
}

bool floating::is_inherited_of(type_name_t type)
{
	return (type == type_of<floating>())
		|| object::is_inherited_of(type)
		|| (type == type_of<value<float>>());
}

bool floating::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<floating>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_of<value<float>>())
	{
		*out = static_cast<value<float>*>(this);
		return true;
	}
	return false;
}

string floating::to_string()const
{
	return ang::move(to_string(*this));
}

string floating::to_string(text::text_format_t format)const
{
	return ang::move(to_string(*this, format));
}


/////////////////////////////////////////////////////////////////

value<double> floating64::parse(cstr_t cstr)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	char* end;
	return strtod(cstr, &end);
}

value<double> floating64::parse(cwstr_t cstr)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	wchar* end;
	return wcstod(cstr, &end);
}

string floating64::to_string(value<double> value)
{
	static text::text_format_t format("{floating:n.4}");
	return ang::move(to_string(value, format));
}

wstring floating64::to_wstring(value<double> value)
{
	static text::text_format_t format("{floating:n.4}");
	return ang::move(to_wstring(value, format));
}


string floating64::to_string(value<double> value, text::text_format_t f)
{
	const byte max_char = 120;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::floating)
		return "invalid format"_s;

	char out[max_char] = { 0 };
	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format) >> 4;
	int pres = 0X0F & GET_FLAG_MAX(format);
	bool exp = (format & FLAG_FLOATING_EXP) == FLAG_FLOATING_EXP;
	word c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	union
	{
		qword int_value;
		double float_value;
	}union_value;
	union_value.float_value = value;
	if ((FLAG_FLOATING_BIN & format) == FLAG_FLOATING_BIN)
	{
		do
		{
			if (union_value.int_value != 0)
				out[c--] = (0X0001 & union_value.int_value) ? '1' : '0';
			else
				out[c--] = fill;
			union_value.int_value = union_value.int_value >> 1;
			max--;
		} while (union_value.int_value != 0 || max > 0);
	}
	if ((FLAG_FLOATING_HEX_U & format) == FLAG_FLOATING_HEX_U)
	{
		do
		{
			if (union_value.int_value != 0)
			{
				out[c--] = hexu[(byte)(0X000F & union_value.int_value)];
				union_value.int_value = union_value.int_value >> 4;
			}
			else
				out[c--] = fill;
			max--;
		} while (union_value.int_value > 0 || max > 0);
	}
	else if ((FLAG_SIGNED_HEX_L & format) == FLAG_SIGNED_HEX_L)
	{
		do
		{
			if (union_value.int_value != 0)
			{
				out[c--] = hexl[(byte)(0X000F & union_value.int_value)];
				union_value.int_value = union_value.int_value >> 4;
			}
			else
				out[c--] = fill;
			max--;
		} while (union_value.int_value > 0 || max > 0);
	}
	else
	{
		bool neg = false;
		long64 int_value;
		int iexp = 0;
		if (value < 0.0f)
		{
			neg = true;
			value *= -1.0;
		}
		if (exp)
		{
			bool bexp = false;
			if (value > 10.0)
			{
				bexp = false;
				do {
					iexp++;
					value /= 10.0;
				} while (value > 10.0);
			}
			else if (value < 1.0f)
			{
				bexp = true;
				do {
					iexp++;
					value *= 10.0;
				} while (value < 1.0);
			}
			do
			{
				out[c--] = '0' + iexp % 10;
				iexp /= 10;
			} while (iexp > 0);
			if (bexp)out[c--] = '-';
			out[c--] = 'e';
		}

		int_value = (long64)value;
		value = value - double(int_value);

		if (pres > 0)
		{
			c -= pres;
			for (word i = 0; i < pres; i++)
			{
				out[i + c + 1] = '0' + byte(value * 10.0);
				value *= 10.0;
				value -= double(long64(value));
			}
			out[c--] = '.';
		}

		do
		{
			if (int_value != 0)
			{
				out[c--] = '0' + int_value % 10;
				int_value /= 10;
			}
			else
				out[c--] = fill;
			max--;
		} while (int_value > 0 || max > 0);

		if (neg)
			out[c--] = '-';
		else if ((FLAG_SIGNED_SIG & format) == FLAG_SIGNED_SIG)
			out[c--] = '+';
	}

	return cstr_t(&out[c + 1], max_char - c);

}

wstring floating64::to_wstring(value<double> value, text::text_format_t f)
{
	const byte max_char = 120;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::floating)
		return "invalid format"_s;

	char out[max_char] = { 0 };
	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format) >> 4;
	int pres = 0X0F & GET_FLAG_MAX(format);
	bool exp = (format & FLAG_FLOATING_EXP) == FLAG_FLOATING_EXP;
	word c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	union
	{
		qword int_value;
		double float_value;
	}union_value;
	union_value.float_value = value;
	if ((FLAG_FLOATING_BIN & format) == FLAG_FLOATING_BIN)
	{
		do
		{
			if (union_value.int_value != 0)
				out[c--] = (0X0001 & union_value.int_value) ? '1' : '0';
			else
				out[c--] = fill;
			union_value.int_value = union_value.int_value >> 1;
			max--;
		} while (union_value.int_value != 0 || max > 0);
	}
	if ((FLAG_FLOATING_HEX_U & format) == FLAG_FLOATING_HEX_U)
	{
		do
		{
			if (union_value.int_value != 0)
			{
				out[c--] = hexu[(byte)(0X000F & union_value.int_value)];
				union_value.int_value = union_value.int_value >> 4;
			}
			else
				out[c--] = fill;
			max--;
		} while (union_value.int_value > 0 || max > 0);
	}
	else if ((FLAG_SIGNED_HEX_L & format) == FLAG_SIGNED_HEX_L)
	{
		do
		{
			if (union_value.int_value != 0)
			{
				out[c--] = hexl[(byte)(0X000F & union_value.int_value)];
				union_value.int_value = union_value.int_value >> 4;
			}
			else
				out[c--] = fill;
			max--;
		} while (union_value.int_value > 0 || max > 0);
	}
	else
	{
		bool neg = false;
		long64 int_value;
		int iexp = 0;
		if (value < 0.0f)
		{
			neg = true;
			value *= -1.0;
		}
		if (exp)
		{
			bool bexp = false;
			if (value > 10.0)
			{
				bexp = false;
				do {
					iexp++;
					value /= 10.0;
				} while (value > 10.0);
			}
			else if (value < 1.0f)
			{
				bexp = true;
				do {
					iexp++;
					value *= 10.0;
				} while (value < 1.0);
			}
			do
			{
				out[c--] = '0' + iexp % 10;
				iexp /= 10;
			} while (iexp > 0);
			if (bexp)out[c--] = '-';
			out[c--] = 'e';
		}

		int_value = (long64)value;
		value = value - double(int_value);

		if (pres > 0)
		{
			c -= pres;
			for (word i = 0; i < pres; i++)
			{
				out[i + c + 1] = '0' + byte(value * 10.0);
				value *= 10.0;
				value -= double(long64(value));
			}
			out[c--] = '.';
		}

		do
		{
			if (int_value != 0)
			{
				out[c--] = '0' + int_value % 10;
				int_value /= 10;
			}
			else
				out[c--] = fill;
			max--;
		} while (int_value > 0 || max > 0);

		if (neg)
			out[c--] = '-';
		else if ((FLAG_SIGNED_SIG & format) == FLAG_SIGNED_SIG)
			out[c--] = '+';
	}

	return cstr_t(&out[c + 1], max_char - c);

}


floating64::value_wrapper()
	: value<double>()
{

}

floating64::value_wrapper(double val)
	: value<double>(val)
{

}

floating64::value_wrapper(floating64 const& other)
	: value<double>(other)
{

}

floating64::value_wrapper(value<double> const& other)
	: value<double>(other)
{

}

floating64::~value_wrapper()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::floating64);
ANG_IMPLEMENT_OBJECTNAME(ang::floating64);

bool floating64::is_kind_of(type_name_t type)const
{
	return (type == type_of<floating64>())
		|| object::is_kind_of(type)
		|| (type == type_of<value<double>>());
}

bool floating64::is_inherited_of(type_name_t type)
{
	return (type == type_of<floating64>())
		|| object::is_inherited_of(type)
		|| (type == type_of<value<double>>());
}

bool floating64::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<floating64>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_of<value<double>>())
	{
		*out = static_cast<value<double>*>(this);
		return true;
	}
	return false;
}

string floating64::to_string()const
{
	return ang::move(to_string(*this));
}

string floating64::to_string(text::text_format_t format)const
{
	return ang::move(to_string(*this, format));
}

