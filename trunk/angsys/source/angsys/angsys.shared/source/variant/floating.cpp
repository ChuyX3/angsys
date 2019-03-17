#include "pch.h"
#include <angsys.h>

using namespace ang;

#define MY_TYPE ang::variable<float>
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::variable<double>
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

namespace ang
{
	static char hexl[18] = { "0123456789abcdefx" };
	static char hexu[18] = { "0123456789ABCDEFX" };
}

value<float> variable<float>::parse(cstr_t cstr, windex i, bool exp)
{
	text::iparser_t parser = text::iparser::get_parser(cstr.encoding());
	return parser->to_floating(cstr, i, true, exp);
}

wstring variable<float>::to_string(value<float> val, text::text_format_t f)
{

	const byte max_char = 120;
	text::text_format_flags_t format;
	format.value = f.format_flags();

	switch (format.target)
	{
	case text::text_format::text_: format.value = text::default_text_format<float>::format().format_flags(); break;
	case text::text_format::float_: break;
	case text::text_format::signed_: return variable<int>::to_string((int)val.get(), f);
	case text::text_format::unsigned_: return variable<uint>::to_string((uint)val.get(), f);
	default: return "invalid format"_s;
	}

	char out[max_char] = { 0 };
	char fill = format.fill;
	int max = format.max;
	int pres = format.pres;
	bool exp = format.exponent;
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
	union_value.float_value = val;
	if (format.base == 3)
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
	if (format.base == 2)
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
	else if (format.base == 1)
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
		if (val < 0.0f)
		{
			neg = true;
			val *= -1.0f;
		}
		if (exp)
		{
			bool bexp = false;
			if (val > 10.0f)
			{
				bexp = false;
				do {
					iexp++;
					val /= 10.0f;
				} while (val > 10.0f);
			}
			else if (val < 1.0f)
			{
				bexp = true;
				do {
					iexp++;
					val *= 10.0f;
				} while (val < 1.0f);
			}
			do
			{
				out[c--] = '0' + iexp % 10;
				iexp /= 10;
			} while (iexp > 0);
			if (bexp)out[c--] = '-';
			out[c--] = 'e';
		}

		int_value = (int)val;
		val = val - float(int_value);

		if (pres > 0)
		{
			c -= pres;
			for (word i = 0; i < pres; i++)
			{
				out[i + c + 1] = '0' + byte(val * 10.0f);
				val *= 10.0f;
				val -= float(int(val));
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
		else if (format.sign)
			out[c--] = '+';
	}

	return castr_t(&out[c + 1], max_char - c);
}

variable<float>::variable()
{

}

variable<float>::variable(float const& val)
{
	set(val);
}

variable<float>::variable(value<float> const& val) 
{
	set(val);
}

variable<float>::variable(variable const* val)
{
	set(val ? val->get() : 0);
}

variable<float>::~variable()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::floating, object, ivariant);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::floating);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::floating, object, ivariant);

comparision_result_t variable<float>::compare(object const* obj)const
{
	ivariant* var = interface_cast<ivariant>(const_cast<object*>(obj));
	if (var == null)
		return ang::comparision_result::diferent;
	float val;
	if (var->get_value(val))
		return val < get() ? ang::comparision_result::mayor 
		: val > get() ? ang::comparision_result::minor 
		: ang::comparision_result::same;
	else
		return ang::comparision_result::diferent;
}

wstring variable<float>::to_string()const
{
	return ang::move(to_string(*this, text::default_text_format<float>::format()));
}

wstring variable<float>::to_string(text::text_format_t f)const
{
	return ang::move(to_string(*this, f));
}

rtti_t const& variable<float>::value_type()const
{
	return type_of<float>();
}

bool variable<float>::set_value(rtti_t const& id, unknown_t val)
{
	if (val == null)
		return false;

	if (id.type_id() == type_of<bool>().type_id())
		set(*static_cast<bool*>(val) ? true : false);
	else if (id.type_id() == type_of<boolean>().type_id())
		set(static_cast<boolean*>(val)->get() ? true : false);

	else if (id.type_id() == type_of<int>().type_id())
		set((float)*static_cast<int*>(val));
	else if (id.type_id() == type_of<integer>().type_id())
		set((float)static_cast<integer*>(val)->get());
	else if (id.type_id() == type_of<uint>().type_id())
		set((float)*static_cast<uint*>(val));
	else if (id.type_id() == type_of<uinteger>().type_id())
		set((float)static_cast<uinteger*>(val)->get());

	else if (id.type_id() == type_of<long>().type_id())
		set((float)*static_cast<long*>(val));
	else if (id.type_id() == type_of<variable<long>>().type_id())
		set((float)static_cast<variable<long>*>(val)->get());
	else if (id.type_id() == type_of<ulong>().type_id())
		set((float)*static_cast<ulong*>(val));
	else if (id.type_id() == type_of<variable<ulong>>().type_id())
		set((float)static_cast<variable<ulong>*>(val)->get());

	else if (id.type_id() == type_of<long64>().type_id())
		set((float)*static_cast<long64*>(val));
	else if (id.type_id() == type_of<integer64>().type_id())
		set((float)static_cast<integer64*>(val)->get());
	else if (id.type_id() == type_of<ulong64>().type_id())
		set((float)*static_cast<ulong64*>(val));
	else if (id.type_id() == type_of<uinteger64>().type_id())
		set((float)static_cast<uinteger64*>(val)->get());

	else if (id.type_id() == type_of<float>().type_id())
		set((float)*static_cast<float*>(val));
	else if (id.type_id() == type_of<floating>().type_id())
		set((float)static_cast<floating*>(val)->get());
	else if (id.type_id() == type_of<double>().type_id())
		set((float)*static_cast<double*>(val));
	else if (id.type_id() == type_of<floating64>().type_id())
		set((float)static_cast<floating64*>(val)->get());

	else return false;
	
	return true;
}

bool variable<float>::get_value(rtti_t const& id, unknown_t val)const
{
	if (val == null)
		return false;

	if (id.type_id() == type_of<bool>().type_id())
		*static_cast<bool*>(val) = get() ? true : false;
	else if (id.type_id() == type_of<boolean>().type_id())
		static_cast<boolean*>(val)->set(get() ? true : false);

	else if (id.type_id() == type_of<int>().type_id())
		*static_cast<int*>(val) = (int)get();
	else if (id.type_id() == type_of<integer>().type_id())
		static_cast<integer*>(val)->set((int)get());
	else if (id.type_id() == type_of<uint>().type_id())
		*static_cast<uint*>(val) = (uint)get();
	else if (id.type_id() == type_of<uinteger>().type_id())
		static_cast<uinteger*>(val)->set((uint)get());

	else if (id.type_id() == type_of<long>().type_id())
		*static_cast<long*>(val) = (long)get();
	else if (id.type_id() == type_of<variable<long>>().type_id())
		static_cast<variable<long>*>(val)->set((long)get());
	else if (id.type_id() == type_of<ulong>().type_id())
		*static_cast<ulong*>(val) = (ulong)get();
	else if (id.type_id() == type_of<variable<ulong>>().type_id())
		static_cast<variable<ulong>*>(val)->set((ulong)get());

	else if (id.type_id() == type_of<long64>().type_id())
		*static_cast<long64*>(val) = (long64)get();
	else if (id.type_id() == type_of<integer64>().type_id())
		static_cast<integer64*>(val)->set((long64)get());
	else if (id.type_id() == type_of<ulong64>().type_id())
		*static_cast<ulong64*>(val) = (ulong64)get();
	else if (id.type_id() == type_of<uinteger64>().type_id())
		static_cast<uinteger64*>(val)->set((ulong64)get());

	else if (id.type_id() == type_of<float>().type_id())
		*static_cast<float*>(val) = (float)get();
	else if (id.type_id() == type_of<floating>().type_id())
		static_cast<floating*>(val)->set((float)get());
	else if (id.type_id() == type_of<double>().type_id())
		*static_cast<double*>(val) = (double)get();
	else if (id.type_id() == type_of<floating64>().type_id())
		static_cast<floating64*>(val)->set((double)get());

	else return false;
	
	return true;
}

variant variable<float>::clone()const
{
	return new variable<float>(get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////

value<double> variable<double>::parse(cstr_t cstr, windex i, bool exp)
{
	text::iparser_t parser = text::iparser::get_parser(cstr.encoding());
	return parser->to_floating(cstr, i, true, exp);
}

wstring variable<double>::to_string(value<double> val, text::text_format_t f)
{

	const byte max_char = 120;
	text::text_format_flags_t format;
	format.value = f.format_flags();

	switch (format.target)
	{
	case text::text_format::text_: format.value = text::default_text_format<double>::format().format_flags(); break;
	case text::text_format::float_: break;
	case text::text_format::signed_: return variable<long64>::to_string((long64)val.get(), f);
	case text::text_format::unsigned_: return variable<ulong64>::to_string((ulong64)val.get(), f);
	default: return "invalid format"_s;
	}

	char out[max_char] = { 0 };
	char fill = format.fill;
	int max = format.max;
	int pres = format.pres;
	bool exp = format.exponent;
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
		ulong64 int_value;
		double float_value;
	}union_value;
	union_value.float_value = val;
	if (format.base == 3)
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
	if (format.base == 2)
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
	else if (format.base == 1)
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
		if (val < 0.0f)
		{
			neg = true;
			val *= -1.0f;
		}
		if (exp)
		{
			bool bexp = false;
			if (val > 10.0f)
			{
				bexp = false;
				do {
					iexp++;
					val /= 10.0f;
				} while (val > 10.0f);
			}
			else if (val < 1.0f)
			{
				bexp = true;
				do {
					iexp++;
					val *= 10.0f;
				} while (val < 1.0f);
			}
			do
			{
				out[c--] = '0' + iexp % 10;
				iexp /= 10;
			} while (iexp > 0);
			if (bexp)out[c--] = '-';
			out[c--] = 'e';
		}

		int_value = (long64)val;
		val = val - double(int_value);

		if (pres > 0)
		{
			c -= pres;
			for (word i = 0; i < pres; i++)
			{
				out[i + c + 1] = '0' + byte(val * 10.0f);
				val *= 10.0f;
				val -= double(int(val));
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
		else if (format.sign)
			out[c--] = '+';
	}

	return castr_t(&out[c + 1], max_char - c);
}

variable<double>::variable()
{
}

variable<double>::variable(double const& val)
{
	set(val);
}

variable<double>::variable(value<double> const& val)
{
	set(val);
}

variable<double>::variable(variable const* val)
{
	set(val ? val->get() : 0);
}

variable<double>::~variable()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::floating64, object, ivariant);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::floating64);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::floating64, object, ivariant);

comparision_result_t variable<double>::compare(object const* obj)const
{
	ivariant* var = interface_cast<ivariant>(const_cast<object*>(obj));
	if (var == null)
		return ang::comparision_result::diferent;
	double val;
	if (var->get_value(val))
		return val < get() ? ang::comparision_result::mayor
		: val > get() ? ang::comparision_result::minor
		: ang::comparision_result::same;
	else
		return ang::comparision_result::diferent;
}

wstring variable<double>::to_string()const
{
	return ang::move(to_string(*this, text::default_text_format<double>::format()));
}

wstring variable<double>::to_string(text::text_format_t f)const
{
	return ang::move(to_string(*this, f));
}

rtti_t const& variable<double>::value_type()const
{
	return type_of<double>();
}

bool variable<double>::set_value(rtti_t const& id, unknown_t val)
{
	if (val == null)
		return false;
	if (id.type_id() == type_of<bool>().type_id())
		set(*static_cast<bool*>(val) ? 1 : 0);
	else if (id.type_id() == type_of<boolean>().type_id())
		set(static_cast<boolean*>(val)->get() ? 1 : 0);

	else if (id.type_id() == type_of<int>().type_id())
		set((double)*static_cast<int*>(val));
	else if (id.type_id() == type_of<integer>().type_id())
		set((double)static_cast<integer*>(val)->get());
	else if (id.type_id() == type_of<uint>().type_id())
		set((double)*static_cast<uint*>(val));
	else if (id.type_id() == type_of<uinteger>().type_id())
		set((double)static_cast<uinteger*>(val)->get());

	else if (id.type_id() == type_of<long>().type_id())
		set((double)*static_cast<long*>(val));
	else if (id.type_id() == type_of<variable<long>>().type_id())
		set((double)static_cast<variable<long>*>(val)->get());
	else if (id.type_id() == type_of<ulong>().type_id())
		set((double)*static_cast<ulong*>(val));
	else if (id.type_id() == type_of<variable<ulong>>().type_id())
		set((double)static_cast<variable<ulong>*>(val)->get());

	else if (id.type_id() == type_of<long64>().type_id())
		set((double)*static_cast<long64*>(val));
	else if (id.type_id() == type_of<integer64>().type_id())
		set((double)static_cast<integer64*>(val)->get());
	else if (id.type_id() == type_of<ulong64>().type_id())
		set((double)*static_cast<ulong64*>(val));
	else if (id.type_id() == type_of<uinteger64>().type_id())
		set((double)static_cast<uinteger64*>(val)->get());

	else if (id.type_id() == type_of<float>().type_id())
		set((double)*static_cast<float*>(val));
	else if (id.type_id() == type_of<floating>().type_id())
		set((double)static_cast<floating*>(val)->get());
	else if (id.type_id() == type_of<double>().type_id())
		set((double)*static_cast<double*>(val));
	else if (id.type_id() == type_of<floating64>().type_id())
		set((double)static_cast<floating64*>(val)->get());

	else return false;
	
	return true;
}

bool variable<double>::get_value(rtti_t const& id, unknown_t val)const
{
	if (val == null)
		return false;

	if (id.type_id() == type_of<bool>().type_id())
		*static_cast<bool*>(val) = get() ? true : false;
	else if (id.type_id() == type_of<boolean>().type_id())
		static_cast<boolean*>(val)->set(get() ? true : false);

	else if (id.type_id() == type_of<int>().type_id())
		*static_cast<int*>(val) = (int)get();
	else if (id.type_id() == type_of<integer>().type_id())
		static_cast<integer*>(val)->set((int)get());
	else if (id.type_id() == type_of<uint>().type_id())
		*static_cast<uint*>(val) = (uint)get();
	else if (id.type_id() == type_of<uinteger>().type_id())
		static_cast<uinteger*>(val)->set((uint)get());

	else if (id.type_id() == type_of<long>().type_id())
		*static_cast<long*>(val) = (long)get();
	else if (id.type_id() == type_of<variable<long>>().type_id())
		static_cast<variable<long>*>(val)->set((long)get());
	else if (id.type_id() == type_of<ulong>().type_id())
		*static_cast<ulong*>(val) = (ulong)get();
	else if (id.type_id() == type_of<variable<ulong>>().type_id())
		static_cast<variable<ulong>*>(val)->set((ulong)get());

	else if (id.type_id() == type_of<long64>().type_id())
		*static_cast<long64*>(val) = (long64)get();
	else if (id.type_id() == type_of<integer64>().type_id())
		static_cast<integer64*>(val)->set((long64)get());
	else if (id.type_id() == type_of<ulong64>().type_id())
		*static_cast<ulong64*>(val) = (ulong64)get();
	else if (id.type_id() == type_of<uinteger64>().type_id())
		static_cast<uinteger64*>(val)->set((ulong64)get());

	else if (id.type_id() == type_of<float>().type_id())
		*static_cast<float*>(val) = (float)get();
	else if (id.type_id() == type_of<floating>().type_id())
		static_cast<floating*>(val)->set((float)get());
	else if (id.type_id() == type_of<double>().type_id())
		*static_cast<double*>(val) = (double)get();
	else if (id.type_id() == type_of<floating64>().type_id())
		static_cast<floating64*>(val)->set((double)get());

	else return false;
	
	return true;
}

variant variable<double>::clone()const
{
	return new variable<double>(get());
}
