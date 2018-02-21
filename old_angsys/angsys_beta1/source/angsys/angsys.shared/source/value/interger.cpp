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

value<int> interger::parse(cstr_t cstr, int base)
{
	if (cstr == null)
		return 0;
	char* end;
	return (int)strtol(cstr, &end, (int)base);
}

value<int> interger::parse(cwstr_t cstr, int base)
{
	if (cstr == null)
		return 0;
	wchar* end;
	return (int)wcstol(cstr, &end, (int)base);
}

string interger::to_string(value<int> value)
{
	static text::text_format_t format("{signed:}");
	return ang::move(to_string(value, format));
}

wstring interger::to_wstring(value<int> value)
{
	static text::text_format_t format("{signed:}");
	return ang::move(to_wstring(value, format));
}

string interger::to_string(value<int> value, text::text_format_t f)
{
	const byte max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_interger)
		return "invalid format";
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	bool neg = false;
	word c = max_char - 2;

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

wstring interger::to_wstring(value<int> value, text::text_format_t f)
{
	const byte max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_interger)
		return "invalid format";
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	bool neg = false;
	word c = max_char - 2;

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


interger::value_wrapper()
	: value<int>()
{

}

interger::value_wrapper(int val)
	: value<int>(val)
{

}

interger::value_wrapper(interger const& other)
	: value<int>(other)
{

}

interger::value_wrapper(value<int> const& other)
	: value<int>(other)
{

}

interger::~value_wrapper()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::interger);
ANG_IMPLEMENT_OBJECTNAME(ang::interger);

bool interger::is_kind_of(type_name_t type)const
{
	return (type == type_name<interger>())
		|| object::is_kind_of(type)
		|| (type == type_name<value<int>>());
}

bool interger::is_child_of(type_name_t type)
{
	return (type == type_name<interger>())
		|| object::is_child_of(type)
		|| (type == type_name<value<int>>());
}

bool interger::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_name<interger>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_name<value<int>>())
	{
		*out = static_cast<value<int>*>(this);
		return true;
	}
	return false;
}

string interger::to_string()const
{
	return ang::move(to_string(*this));
}

string interger::to_string(text::text_format_t format)const
{
	return ang::move(to_string(*this, format));
}

///////////////////////////////////////////////////////////////////////////


value<uint> uinterger::parse(cstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	char* end;
	return (uint)strtoul(cstr, &end, (int)base);
}

value<uint> uinterger::parse(cwstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	wchar* end;
	return (uint)wcstoul(cstr, &end, (int)base);
}

string uinterger::to_string(value<uint> value)
{
	static text::text_format_t format("{unsigned:}");
	return ang::move(to_string(value, format));
}

wstring uinterger::to_wstring(value<uint> value)
{
	static text::text_format_t format("{unsigned:}");
	return ang::move(to_wstring(value, format));
}

string uinterger::to_string(value<uint> value, text::text_format_t f)
{
	const byte max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_interger)
		return "invalid format";
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	word c = max_char - 2;

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

wstring uinterger::to_wstring(value<uint> value, text::text_format_t f)
{
	const byte max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_interger)
		return "invalid format";
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	word c = max_char - 2;

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


uinterger::value_wrapper()
	: value<uint>()
{

}

uinterger::value_wrapper(uint val)
	: value<uint>(val)
{

}

uinterger::value_wrapper(uinterger const& other)
	: value<uint>(other)
{

}

uinterger::value_wrapper(value<uint> const& other)
	: value<uint>(other)
{

}

uinterger::~value_wrapper()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::uinterger);
ANG_IMPLEMENT_OBJECTNAME(ang::uinterger);

bool uinterger::is_kind_of(type_name_t type)const
{
	return (type == type_name<uinterger>())
		|| object::is_kind_of(type)
		|| (type == type_name<value<uint>>());
}

bool uinterger::is_child_of(type_name_t type)
{
	return (type == type_name<uinterger>())
		|| object::is_child_of(type)
		|| (type == type_name<value<uint>>());
}

bool uinterger::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_name<uinterger>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_name<value<uint>>())
	{
		*out = static_cast<value<uint>*>(this);
		return true;
	}
	return false;
}

string uinterger::to_string()const
{
	return ang::move(to_string(*this));
}

string uinterger::to_string(text::text_format_t format)const
{
	return ang::move(to_string(*this, format));
}


/////////////////////////////////////////////////////////////////


value<long64> interger64::parse(cstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	char* end;
	return strtoll(cstr, &end, (int)base);
}

value<long64> interger64::parse(cwstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0;
	wchar* end;
	return wcstoll(cstr, &end, (int)base);
}

string interger64::to_string(value<long64> value)
{
	static text::text_format_t format("{signed:}");
	return ang::move(to_string(value, format));
}

wstring interger64::to_wstring(value<long64> value)
{
	static text::text_format_t format("{signed:}");
	return ang::move(to_wstring(value, format));
}

string interger64::to_string(value<long64> value, text::text_format_t f)
{
	const byte max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_interger)
		return "invalid format";
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	bool neg = false;
	word c = max_char - 2;

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

wstring interger64::to_wstring(value<long64> value, text::text_format_t f)
{
	const byte max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_interger)
		return "invalid format";
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	bool neg = false;
	word c = max_char - 2;

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


interger64::value_wrapper()
	: value<long64>()
{

}

interger64::value_wrapper(long64 val)
	: value<long64>(val)
{

}

interger64::value_wrapper(interger64 const& other)
	: value<long64>(other)
{

}

interger64::value_wrapper(value<long64> const& other)
	: value<long64>(other)
{

}

interger64::~value_wrapper()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::interger64);
ANG_IMPLEMENT_OBJECTNAME(ang::interger64);

bool interger64::is_kind_of(type_name_t type)const
{
	return (type == type_name<interger64>())
		|| object::is_kind_of(type)
		|| (type == type_name<value<long64>>());
}

bool interger64::is_child_of(type_name_t type)
{
	return (type == type_name<interger64>())
		|| object::is_child_of(type)
		|| (type == type_name<value<long64>>());
}

bool interger64::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_name<interger64>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_name<value<long64>>())
	{
		*out = static_cast<value<long64>*>(this);
		return true;
	}
	return false;
}

string interger64::to_string()const
{
	return ang::move(to_string(*this));
}

string interger64::to_string(text::text_format_t format)const
{
	return ang::move(to_string(*this, format));
}

///////////////////////////////////////////////////////////////////////////




value<ulong64> uinterger64::parse(cstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0ULL;
	char* end;
	return strtoull(cstr, &end, (int)base);
}

value<ulong64> uinterger64::parse(cwstr_t cstr, int base)
{
	if (cstr.cstr() == null || cstr.size() == 0)
		return 0ULL;
	wchar* end;
	return wcstoull(cstr, &end, (int)base);
}

string uinterger64::to_string(value<ulong64> value)
{
	static text::text_format_t format("{unsigned:}");
	return ang::move(to_string(value, format));
}

wstring uinterger64::to_wstring(value<ulong64> value)
{
	static text::text_format_t format("{unsigned:}");
	return ang::move(to_wstring(value, format));
}

string uinterger64::to_string(value<ulong64> value, text::text_format_t f)
{
	const byte max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_interger)
		return "invalid format";
	char out[max_char] = { 0 };

	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	word c = max_char - 2;

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

wstring uinterger64::to_wstring(value<ulong64> value, text::text_format_t f)
{
	const byte max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::signed_interger)
		return "invalid format";

	char out[max_char] = { 0 };
	char fill = GET_FLAG_FILL(format);
	int max = GET_FLAG_MAX(format);
	word c = max_char - 2;

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


uinterger64::value_wrapper()
	: value<ulong64>()
{

}

uinterger64::value_wrapper(ulong64 val)
	: value<ulong64>(val)
{

}

uinterger64::value_wrapper(uinterger64 const& other)
	: value<ulong64>(other)
{

}

uinterger64::value_wrapper(value<ulong64> const& other)
	: value<ulong64>(other)
{

}

uinterger64::~value_wrapper()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::uinterger64);
ANG_IMPLEMENT_OBJECTNAME(ang::uinterger64);

bool uinterger64::is_kind_of(type_name_t type)const
{
	return (type == type_name<uinterger64>())
		|| object::is_kind_of(type)
		|| (type == type_name<value<ulong64>>());
}

bool uinterger64::is_child_of(type_name_t type)
{
	return (type == type_name<uinterger64>())
		|| object::is_child_of(type)
		|| (type == type_name<value<ulong64>>());
}

bool uinterger64::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_name<uinterger64>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_name<value<ulong64>>())
	{
		*out = static_cast<value<ulong64>*>(this);
		return true;
	}
	return false;
}

string uinterger64::to_string()const
{
	return ang::move(to_string(*this));
}

string uinterger64::to_string(text::text_format_t format)const
{
	return ang::move(to_string(*this, format));
}


