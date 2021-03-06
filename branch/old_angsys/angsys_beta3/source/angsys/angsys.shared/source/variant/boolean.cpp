#include "pch.h"
#include <angsys.h>

using namespace ang;

#define MY_TYPE ang::variable<bool>
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

static collections::pair<castr_t, bool> s_boolean_parsing_map[] =
{
	{ "FALSE"_s, false },
	{ "False"_s, false },
	{ "HIGH"_s, true },
	{ "High"_s, true },
	{ "LOW"_s, false },
	{ "Low"_s, false },
	{ "NO"_s, false },
	{ "No"_s, false },
	{ "TRUE"_s, true },
	{ "True"_s, true },
	{ "YES"_s, true },
	{ "Yes"_s, true },
	{ "false"_s, false },
	{ "high"_s, true },
	{ "low"_s, false },
	{ "no"_s, false },
	{ "true"_s, true },
	{ "yes"_s, true },
};

value<bool> variable<bool>::parse(cstr_t cstr)
{
	auto idx = algorithms::binary_search(cstr, collections::to_array(s_boolean_parsing_map));
	if (idx >= algorithms::array_size(s_boolean_parsing_map))
		return false;
	else
		return s_boolean_parsing_map[idx].value;
}

string variable<bool>::to_string(value<bool> val,text::text_format_t f_)
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
			case 1: return val.get() ? "YES"_s : "NO"_s;
			case 2: return val.get() ? "HIGH"_s : "LOW"_s;
			default: return val.get() ? "TRUE"_s : "FALSE"_s;
			}
		case 3:
			switch (f.type)
			{
			case 1: return val.get() ? "Yes"_s : "No"_s;
			case 2: return val.get() ? "High"_s : "Low"_s;
			default: return val.get() ? "True"_s : "False"_s;
			}
		default:
			switch (f.type)
			{
			case 1: return val.get() ? "yes"_s : "no"_s;
			case 2: return val.get() ? "high"_s : "low"_s;
			default: return val.get() ? "true"_s : "false"_s;
			}
		}
	case text::text_format::signed_:
		return variable<int>::to_string(val.get() ? 1 : 0, f_);
	case text::text_format::unsigned_:
		return variable<uint>::to_string(val.get() ? 1 : 0, f_);
	case text::text_format::float_:
		return variable<float>::to_string(val.get() ? 1.0f : 0.0f, f_);
	default:  return val.get() ? "true"_s : "false"_s;
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

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::boolean, object, ivariant);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::boolean);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::boolean, object, ivariant);

comparision_result_t variable<bool>::compare(object const* obj)const
{
	ivariant* var = interface_cast<ivariant>(const_cast<object*>(obj));
	if (var == null)
		return ang::comparision_result::diferent;
	bool val;
	if (var->get_value(val))
		return val == get() ? ang::comparision_result::same : ang::comparision_result::diferent;
	else
		return ang::comparision_result::diferent;
}

string variable<bool>::to_string()const
{
	return get() ? "true"_s : "false"_s;
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
	return new variable<bool>(get());
}

