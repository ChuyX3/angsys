#include "pch.h"
#include <angsys.h>

using namespace ang;

#define MY_TYPE ang::variable<bool>
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

static collections::pair<castr_t, bool> s_boolean_parsing_map[] =
{
	{ "FALSE"_sv, false },
	{ "False"_sv, false },
	{ "HIGH"_sv, true },
	{ "High"_sv, true },
	{ "LOW"_sv, false },
	{ "Low"_sv, false },
	{ "NO"_sv, false },
	{ "No"_sv, false },
	{ "TRUE"_sv, true },
	{ "True"_sv, true },
	{ "YES"_sv, true },
	{ "Yes"_sv, true },
	{ "false"_sv, false },
	{ "high"_sv, true },
	{ "low"_sv, false },
	{ "no"_sv, false },
	{ "true"_sv, true },
	{ "yes"_sv, true },
};

value<bool> variable<bool>::parse(cstr_t cstr)
{
	auto idx = algorithms::binary_search(cstr, collections::to_array(s_boolean_parsing_map));
	if (idx >= algorithms::array_size(s_boolean_parsing_map))
		return false;
	else
		return s_boolean_parsing_map[idx].value;
}

string variable<bool>::to_string(value<bool> val, text::text_format_t f_)
{
	text::text_format_flags_t f;
	f.value = f_.format_flags();
	switch (f.target)
	{
	case text::text_format::bool_:
		switch (f.case_)
		{
		case 2:
			switch (f.type)
			{
			case 1: return val.get() ? "YES"_r : "NO"_r;
			case 2: return val.get() ? "HIGH"_r : "LOW"_r;
			default: return val.get() ? "TRUE"_r : "FALSE"_r;
			}
		case 3:
			switch (f.type)
			{
			case 1: return val.get() ? "Yes"_r : "No"_r;
			case 2: return val.get() ? "High"_r : "Low"_r;
			default: return val.get() ? "True"_r : "False"_r;
			}
		default:
			switch (f.type)
			{
			case 1: return val.get() ? "yes"_r : "no"_r;
			case 2: return val.get() ? "high"_r : "low"_r;
			default: return val.get() ? "true"_r : "false"_r;
			}
		}
	case text::text_format::signed_:
		return variable<int>::to_string(val.get() ? 1 : 0, f_);
	case text::text_format::unsigned_:
		return variable<uint>::to_string(val.get() ? 1 : 0, f_);
	case text::text_format::float_:
		return variable<float>::to_string(val.get() ? 1.0f : 0.0f, f_);
	default:  return val.get() ? "true"_r : "false"_r;
	}
}

variable<bool>::variable()
{
	set(false);
}

variable<bool>::variable(bool const& val)
{
	set(val);
}

variable<bool>::variable(value<bool> const& val) 
{
	set(val);
}

variable<bool>::variable(variable const* val)
{
	set(val ? val->get() : false);
}

variable<bool>::~variable()
{

}

string variable<bool>::to_string()const
{
	return get() ? "true"_r : "false"_r;
}

string variable<bool>::to_string(text::text_format_t f)const
{
	return ang::move(to_string(*this, f));
}

rtti_t const& variable<bool>::value_type()const
{
	return type_of<bool>();
}

bool variable<bool>::set_value(rtti_t const& id, unknown_t val)
{
	return false;
}

bool variable<bool>::get_value(rtti_t const& id, unknown_t val)const
{
	return false;
}

variant variable<bool>::clone()const
{
	return (ivariable*)new variable<bool>(get());
}

