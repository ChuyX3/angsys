#include "pch.h"
#include "angsys.h"
#include "inline/parser_bridge.hpp"

using namespace ang;
using namespace ang::text;


text_format::text_format_flags::text_format_flags()
{
	value = 0;
}

text_format::text_format_flags::text_format_flags(qword val)
{
	value = val;
}

text_format::text_format_flags::text_format_flags(ang::text::text_format::target t)
{
	value = 0;
	target = t;
	switch (t)
	{
	case bool_:
		case_ = 3;
		break;
	case float_:
		pres = 4;
		break;
	}
}

text_format::text_format()
{
	flags.value = 0;
}

text_format::text_format(castr_t format)
{
	flags.value = text_parser<encoding::ascii>::parse(format);
}

text_format::text_format(qword format)
{
	flags.value = format;
}

text_format::text_format(const text_format& val)
{
	flags.value = val.flags.value;
}

text_format::text_format(const text_format_flags& val)
{
	flags.value = val.value;
}

text_format::~text_format()
{
	flags.value = 0;
}

text_format::target text_format::format_target()const
{
	return flags.target;
}

void text_format::format(castr_t format)
{
	flags.value = text_parser<encoding::ascii>::parse(format);
}

qword text_format::format_flags()const
{
	return flags.value;
}

text_format& text_format::operator = (const text_format& val)
{
	flags.value = val.flags.value;
	return*this;
}

/////////////////////////////////////////////////////////////////////////////////////////////



text_format_t text_parser<encoding::auto_detect>::default_format(text_format_target_t f)
{
	static text_format_t s_array[] = {
		0x0000000000000301,
		0x0000000000000002,
		0x0000000000000003,
		0x0000000000000004,
		0x0000000000000005,
		0x0000000040000006
	};

	return (f < text_format_target::bool_ || f >text_format_target::float_ )? text_format_t() : s_array[(int)f];
}


cstr_t text_parser<encoding::auto_detect>::seek(cstr_t cstr1, windex& i, cstr_t cstr2)
{
	return parser_bridge<>::seek(cstr1, i, cstr2);
}

long64 text_parser<encoding::auto_detect>::to_signed(cstr_t cstr, windex& i, bool increment, int base)
{
	return parser_bridge<>::to_signed(cstr, i, increment, base);
}

ulong64 text_parser<encoding::auto_detect>::to_unsigned(cstr_t cstr, windex& i, bool increment, int base)
{
	return parser_bridge<>::to_unsigned(cstr, i, increment, base);
}

double text_parser<encoding::auto_detect>::to_floating(cstr_t cstr, windex& i, bool increment, bool ex)
{
	return parser_bridge<>::to_floating(cstr, i, increment, ex);
}

cstr_t text_parser<encoding::auto_detect>::format(cstr_t cstr, var_args_t args, encoding_t e, istring_ptr_t out)
{
	return parser_bridge<>::format(cstr, ang::forward<var_args_t>(args), e, out);
}

text_format_t text_parser<encoding::auto_detect>::parse(cstr_t cstr)
{
	return parser_bridge<>::parse(cstr);
}

text_format_t text_parser<encoding::auto_detect>::parse(cstr_t cstr, wsize& beg, int& arg)
{
	return parser_bridge<>::parse(cstr, beg, arg);
}

