/*********************************************************************************************************************/
/*   File Name: interget.cpp                                                                                         */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"
#include "format_parser.h"
using namespace ang;

namespace ang
{
	static char hexl[18] = { "0123456789abcdefx" };
	static char hexu[18] = { "0123456789ABCDEFX" };
}

value<int> integer::parse(cstr_t cstr, int base)
{
	if (cstr == null)
		return 0;
	char* end;
	return (int)strtol(cstr, &end, (int)base);
}

value<int> integer::parse(cwstr_t cstr, int base)
{
	if (cstr == null)
		return 0;
	wchar* end;
	return (int)wcstol(cstr, &end, (int)base);
}

string integer::to_string(value<int> value)
{
	return ang::move(to_string(value, text::default_text_format<int>::format()));
}

wstring integer::to_wstring(value<int> value)
{
	return ang::move(to_wstring(value, text::default_text_format<int>::format()));
}

string integer::to_string(value<int> value, text::text_format_t f)
{
	const uint max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_integer)
		return "invalid format"_s;
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	bool neg = false;
	uint c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	if ((FLAG_SIGNED_BIN & format) == FLAG_SIGNED_BIN)
	{
		if (value == 0)
		{
			out[c--] = '0';
		}
		else do
		{
			if (value != 0)
				out[c--] = (0X0001 & value) ? '1' : '0';
			else
				out[c--] = fill;
			value = value >> 1;
			max--;
		} while (value != 0 || max > 0);
	}
	else
	{
		if (value < 0)
		{
			neg = true;
			value *= -1;
		}
		if ((FLAG_SIGNED_HEX_U & format) == FLAG_SIGNED_HEX_U)
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = hexu[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		else if ((FLAG_SIGNED_HEX_L & format) == FLAG_SIGNED_HEX_L)
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = hexl[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		else
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = '0' + value % 10;
					value /= 10;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		if (neg)
			out[c--] = '-';
		else if ((FLAG_SIGNED_SIG & format) == FLAG_SIGNED_SIG)
			out[c--] = '+';
	}
	return cstr_t(&out[c + 1u], max_char - c);
}

wstring integer::to_wstring(value<int> value, text::text_format_t f)
{
	const uint max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_integer)
		return "invalid format"_s;
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	bool neg = false;
	uint c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	if ((FLAG_SIGNED_BIN & format) == FLAG_SIGNED_BIN)
	{
		if (value == 0)
		{
			out[c--] = '0';
		}
		else do
		{
			if (value != 0)
				out[c--] = (0X0001 & value) ? '1' : '0';
			else
				out[c--] = fill;
			value = value >> 1;
			max--;
		} while (value != 0 || max > 0);
	}
	else
	{
		if (value < 0)
		{
			neg = true;
			value *= -1;
		}
		if ((FLAG_SIGNED_HEX_U & format) == FLAG_SIGNED_HEX_U)
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = hexu[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		else if ((FLAG_SIGNED_HEX_L & format) == FLAG_SIGNED_HEX_L)
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = hexl[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		else
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = '0' + value % 10;
					value /= 10;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		if (neg)
			out[c--] = '-';
		else if ((FLAG_SIGNED_SIG & format) == FLAG_SIGNED_SIG)
			out[c--] = '+';
	}
	return cstr_t(&out[c + 1u], max_char - c);
}


integer::value_wrapper()
	: value<int>()
{

}

integer::value_wrapper(int val)
	: value<int>(val)
{

}

integer::value_wrapper(integer const& other)
	: value<int>(other)
{

}

integer::value_wrapper(value<int> const& other)
	: value<int>(other)
{

}

integer::~value_wrapper()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::integer);
ANG_IMPLEMENT_OBJECTNAME(ang::integer);

bool integer::is_kind_of(type_name_t type)const
{
	return (type == type_of<integer>())
		|| object::is_kind_of(type)
		|| (type == type_of<value<int>>());
}

bool integer::is_inherited_of(type_name_t type)
{
	return (type == type_of<integer>())
		|| object::is_inherited_of(type)
		|| (type == type_of<value<int>>());
}

bool integer::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<integer>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_of<value<int>>())
	{
		*out = static_cast<value<int>*>(this);
		return true;
	}
	return false;
}

string integer::to_string()const
{
	return ang::move(to_string(*this));
}

string integer::to_string(text::text_format_t format)const
{
	return ang::move(to_string(*this, format));
}

///////////////////////////////////////////////////////////////////////////


value<uint> uinteger::parse(cstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	char* end;
	return (uint)strtoul(cstr, &end, (int)base);
}

value<uint> uinteger::parse(cwstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	wchar* end;
	return (uint)wcstoul(cstr, &end, (int)base);
}

string uinteger::to_string(value<uint> value)
{
	static text::text_format_t format("{unsigned:}");
	return ang::move(to_string(value, format));
}

wstring uinteger::to_wstring(value<uint> value)
{
	static text::text_format_t format("{unsigned:}");
	return ang::move(to_wstring(value, format));
}

string uinteger::to_string(value<uint> value, text::text_format_t f)
{
	const uint max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_integer)
		return "invalid format"_s;
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	uint c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	if ((FLAG_UNSIGNED_BIN & format) == FLAG_UNSIGNED_BIN)
	{
		if (value == 0U)
		{
			out[c--] = '0';
		}
		else do
		{
			if (value != 0)
				out[c--] = (0X0001 & value) ? '1' : '0';
			else
				out[c--] = fill;
			value = value >> 1;
			max--;
		} while (value != 0U || max > 0);
	}
	else
	{
		if ((FLAG_UNSIGNED_HEX_U & format) == FLAG_UNSIGNED_HEX_U)
		{
			if (value == 0U)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = hexu[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0U);
		}
		else if ((FLAG_UNSIGNED_HEX_L & format) == FLAG_UNSIGNED_HEX_L)
		{
			if (value == 0U)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = hexl[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0);
		}
		else
		{
			if (value == 0U)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = '0' + value % 10;
					value /= 10U;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0);
		}
	}
	return cstr_t(&out[c + 1], max_char - c);
}

wstring uinteger::to_wstring(value<uint> value, text::text_format_t f)
{
	const uint max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_integer)
		return "invalid format"_s;
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	uint c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	if ((FLAG_UNSIGNED_BIN & format) == FLAG_UNSIGNED_BIN)
	{
		if (value == 0U)
		{
			out[c--] = '0';
		}
		else do
		{
			if (value != 0U)
				out[c--] = (0X0001 & value) ? '1' : '0';
			else
				out[c--] = fill;
			value = value >> 1;
			max--;
		} while (value != 0U || max > 0);
	}
	else
	{
		if ((FLAG_UNSIGNED_HEX_U & format) == FLAG_UNSIGNED_HEX_U)
		{
			if (value == 0U)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = hexu[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0);
		}
		else if ((FLAG_UNSIGNED_HEX_L & format) == FLAG_UNSIGNED_HEX_L)
		{
			if (value == 0U)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = hexl[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0);
		}
		else
		{
			if (value == 0U)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = '0' + value % 10;
					value /= 10U;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0);
		}
	}
	return  cstr_t(&out[c + 1], max_char - c);
}


uinteger::value_wrapper()
	: value<uint>()
{

}

uinteger::value_wrapper(uint val)
	: value<uint>(val)
{

}

uinteger::value_wrapper(uinteger const& other)
	: value<uint>(other)
{

}

uinteger::value_wrapper(value<uint> const& other)
	: value<uint>(other)
{

}

uinteger::~value_wrapper()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::uinteger);
ANG_IMPLEMENT_OBJECTNAME(ang::uinteger);

bool uinteger::is_kind_of(type_name_t type)const
{
	return (type == type_of<uinteger>())
		|| object::is_kind_of(type)
		|| (type == type_of<value<uint>>());
}

bool uinteger::is_inherited_of(type_name_t type)
{
	return (type == type_of<uinteger>())
		|| object::is_inherited_of(type)
		|| (type == type_of<value<uint>>());
}

bool uinteger::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<uinteger>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_of<value<uint>>())
	{
		*out = static_cast<value<uint>*>(this);
		return true;
	}
	return false;
}

string uinteger::to_string()const
{
	return ang::move(to_string(*this));
}

string uinteger::to_string(text::text_format_t format)const
{
	return ang::move(to_string(*this, format));
}


/////////////////////////////////////////////////////////////////


value<long64> integer64::parse(cstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	char* end;
	return strtoll(cstr, &end, (int)base);
}

value<long64> integer64::parse(cwstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	wchar* end;
	return wcstoll(cstr, &end, (int)base);
}

string integer64::to_string(value<long64> value)
{
	static text::text_format_t format("{signed:}");
	return ang::move(to_string(value, format));
}

wstring integer64::to_wstring(value<long64> value)
{
	static text::text_format_t format("{signed:}");
	return ang::move(to_wstring(value, format));
}

string integer64::to_string(value<long64> value, text::text_format_t f)
{
	const uint max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_integer)
		return "invalid format"_s;
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	bool neg = false;
	uint c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	if ((FLAG_SIGNED_BIN & format) == FLAG_SIGNED_BIN)
	{
		if (value == 0)
		{
			out[c--] = '0';
		}
		else do
		{
			if (value != 0)
				out[c--] = (0X0001 & value) ? '1' : '0';
			else
				out[c--] = fill;
			value = value >> 1;
			max--;
		} while (value != 0 || max > 0);
	}
	else
	{
		if (value < 0)
		{
			neg = true;
			value *= -1LL;
		}
		if ((FLAG_SIGNED_HEX_U & format) == FLAG_SIGNED_HEX_U)
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = hexu[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		else if ((FLAG_SIGNED_HEX_L & format) == FLAG_SIGNED_HEX_L)
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = hexl[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		else
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = '0' + value % 10;
					value /= 10LL;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		if (neg)
			out[c--] = '-';
		else if ((FLAG_SIGNED_SIG & format) == FLAG_SIGNED_SIG)
			out[c--] = '+';
	}
	return cstr_t(&out[c + 1], max_char -c);
}

wstring integer64::to_wstring(value<long64> value, text::text_format_t f)
{
	const uint max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_integer)
		return "invalid format"_s;
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	bool neg = false;
	uint c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	if ((FLAG_SIGNED_BIN & format) == FLAG_SIGNED_BIN)
	{
		if (value == 0)
		{
			out[c--] = '0';
		}
		else do
		{
			if (value != 0)
				out[c--] = (0X0001 & value) ? '1' : '0';
			else
				out[c--] = fill;
			value = value >> 1;
			max--;
		} while (value != 0 || max > 0);
	}
	else
	{
		if (value < 0)
		{
			neg = true;
			value *= -1LL;
		}
		if ((FLAG_SIGNED_HEX_U & format) == FLAG_SIGNED_HEX_U)
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = hexu[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		else if ((FLAG_SIGNED_HEX_L & format) == FLAG_SIGNED_HEX_L)
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = hexl[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		else
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = '0' + value % 10;
					value /= 10LL;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		if (neg)
			out[c--] = '-';
		else if ((FLAG_SIGNED_SIG & format) == FLAG_SIGNED_SIG)
			out[c--] = '+';
	}
	return cstr_t(&out[c + 1], max_char -c);
}


integer64::value_wrapper()
	: value<long64>()
{

}

integer64::value_wrapper(long64 val)
	: value<long64>(val)
{

}

integer64::value_wrapper(integer64 const& other)
	: value<long64>(other)
{

}

integer64::value_wrapper(value<long64> const& other)
	: value<long64>(other)
{

}

integer64::~value_wrapper()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::integer64);
ANG_IMPLEMENT_OBJECTNAME(ang::integer64);

bool integer64::is_kind_of(type_name_t type)const
{
	return (type == type_of<integer64>())
		|| object::is_kind_of(type)
		|| (type == type_of<value<long64>>());
}

bool integer64::is_inherited_of(type_name_t type)
{
	return (type == type_of<integer64>())
		|| object::is_inherited_of(type)
		|| (type == type_of<value<long64>>());
}

bool integer64::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<integer64>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_of<value<long64>>())
	{
		*out = static_cast<value<long64>*>(this);
		return true;
	}
	return false;
}

string integer64::to_string()const
{
	return ang::move(to_string(*this));
}

string integer64::to_string(text::text_format_t format)const
{
	return ang::move(to_string(*this, format));
}

///////////////////////////////////////////////////////////////////////////




value<ulong64> uinteger64::parse(cstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0ULL;
	char* end;
	return strtoull(cstr, &end, (int)base);
}

value<ulong64> uinteger64::parse(cwstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0ULL;
	wchar* end;
	return wcstoull(cstr, &end, (int)base);
}

string uinteger64::to_string(value<ulong64> value)
{
	static text::text_format_t format("{unsigned:}");
	return ang::move(to_string(value, format));
}

wstring uinteger64::to_wstring(value<ulong64> value)
{
	static text::text_format_t format("{unsigned:}");
	return ang::move(to_wstring(value, format));
}

string uinteger64::to_string(value<ulong64> value, text::text_format_t f)
{
	const uint max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_integer)
		return "invalid format"_s;
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	uint c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	if ((FLAG_UNSIGNED_BIN & format) == FLAG_UNSIGNED_BIN)
	{
		if (value == 0U)
		{
			out[c--] = '0';
		}
		else do
		{
			if (value != 0U)
				out[c--] = (0X0001 & value) ? '1' : '0';
			else
				out[c--] = fill;
			value = value >> 1;
			max--;
		} while (value != 0U || max > 0);
	}
	else
	{
		if ((FLAG_UNSIGNED_HEX_U & format) == FLAG_UNSIGNED_HEX_U)
		{
			if (value == 0U)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = hexu[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0);
		}
		else if ((FLAG_UNSIGNED_HEX_L & format) == FLAG_UNSIGNED_HEX_L)
		{
			if (value == 0U)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = hexl[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0);
		}
		else
		{
			if (value == 0U)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = '0' + value % 10;
					value /= 10ULL;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0);
		}
	}
	return cstr_t(&out[c + 1], max_char -c);
}

wstring uinteger64::to_wstring(value<ulong64> value, text::text_format_t f)
{
	const uint max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_integer)
		return "invalid format"_s;

	char out[max_char] = { 0 };
	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	uint c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	if ((FLAG_UNSIGNED_BIN & format) == FLAG_UNSIGNED_BIN)
	{
		if (value == 0)
		{
			out[c--] = '0';
		}
		else do
		{
			if (value != 0)
				out[c--] = (0X0001 & value) ? '1' : '0';
			else
				out[c--] = fill;
			value = value >> 1;
			max--;
		} while (value != 0 || max > 0);
	}
	else
	{
		if ((FLAG_UNSIGNED_HEX_U & format) == FLAG_UNSIGNED_HEX_U)
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = hexu[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		else if ((FLAG_UNSIGNED_HEX_L & format) == FLAG_UNSIGNED_HEX_L)
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = hexl[(byte)0X000F & value];
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		else
		{
			if (value == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = '0' + value % 10;
					value /= 10ULL;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
	}
	return cstr_t(&out[c + 1], max_char -c);
}


uinteger64::value_wrapper()
	: value<ulong64>()
{

}

uinteger64::value_wrapper(ulong64 val)
	: value<ulong64>(val)
{

}

uinteger64::value_wrapper(uinteger64 const& other)
	: value<ulong64>(other)
{

}

uinteger64::value_wrapper(value<ulong64> const& other)
	: value<ulong64>(other)
{

}

uinteger64::~value_wrapper()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::uinteger64);
ANG_IMPLEMENT_OBJECTNAME(ang::uinteger64);

bool uinteger64::is_kind_of(type_name_t type)const
{
	return (type == type_of<uinteger64>())
		|| object::is_kind_of(type)
		|| (type == type_of<value<ulong64>>());
}

bool uinteger64::is_inherited_of(type_name_t type)
{
	return (type == type_of<uinteger64>())
		|| object::is_inherited_of(type)
		|| (type == type_of<value<ulong64>>());
}

bool uinteger64::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<uinteger64>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_of<value<ulong64>>())
	{
		*out = static_cast<value<ulong64>*>(this);
		return true;
	}
	return false;
}

string uinteger64::to_string()const
{
	return ang::move(to_string(*this));
}

string uinteger64::to_string(text::text_format_t format)const
{
	return ang::move(to_string(*this, format));
}


