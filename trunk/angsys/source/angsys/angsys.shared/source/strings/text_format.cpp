#include "pch.h"
#include "angsys.h"
#include "format_parser.h"

using namespace ang;
using namespace ang::text;

text_format::text_format()
{
	flags.value = 0;
}

text_format::text_format(cstr_t format)
{
	flags.value = format_parser<encoding::ascii>::parse(format);
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

void text_format::format(cstr_t format)
{
	flags.value = format_parser<encoding::ascii>::parse(format);
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

