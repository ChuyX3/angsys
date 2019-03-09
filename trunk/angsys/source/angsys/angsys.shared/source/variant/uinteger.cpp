#include "pch.h"
#include <angsys.h>

using namespace ang;

#define MY_TYPE ang::variable<uint>
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::variable<ulong>
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::variable<ulong64>
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

namespace ang
{
	static char hexl[18] = { "0123456789abcdefx" };
	static char hexu[18] = { "0123456789ABCDEFX" };
}

value<uint> variable<uint>::parse(text::raw_cstr_t str, windex i, int base) {
	text::iparser_t parser = text::iparser::get_parser(str.encoding());
	return parser->to_unsigned(str, i, true, base);
}

wstring variable<uint>::to_string(value<uint> val, text::text_format_t f)
{
	const byte max_char = 120;
	text::text_format_flags_t format;
	format.value = f.format_flags();

	switch (format.target)
	{
	case text::text_format::text_: format.value = text::default_text_format<uint>::format().format_flags(); break;
	case text::text_format::unsigned_: break;
	case text::text_format::signed_: return variable<int>::to_string((int)val.get(), f);
	case text::text_format::float_: return variable<float>::to_string((float)val.get(), f);
	default: return "invalid format"_s;
	}

	char out[max_char] = { 0 };

	int th = -1;
	char fill = format.fill;
	int max = format.max;
	bool neg = false;
	uint c = max_char - 2;
	//int value = val.Get();

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	if (format.thousand != 0)
		th = 0;

	if (format.base == 3)
	{
		if (val == 0)
		{
			out[c--] = '0';
		}
		else do
		{
			if (th >= 0) {
				if (th == 4) {
					out[c--] = format.thousand;
					th = 0;
				}
				th++;
			}
			if (val != 0)
				out[c--] = (0X0001 & val) ? '1' : '0';
			else
				out[c--] = fill;
			val = val >> 1;
			max--;
		} while (val != 0 || max > 0);
	}
	else
	{
		if (val < 0)
		{
			neg = true;
			val *= -1;
		}
		if (format.base == 2)
		{
			if (val == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (th >= 0) {
					if (th == 4) {
						out[c--] = format.thousand;
						th = 0;
					}
					th++;
				}
				if (val != 0)
				{
					out[c--] = hexu[(byte)0X000F & val];
					val = val >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (val > 0 || max > 0);
		}
		else if (format.base == 1)
		{
			if (val == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (th >= 0) {
					if (th == 4) {
						out[c--] = format.thousand;
						th = 0;
					}
					th++;
				}
				if (val != 0)
				{
					out[c--] = hexl[(byte)0X000F & val];
					val = val >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (val > 0 || max > 0);
		}
		else
		{
			if (val == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (th >= 0) {
					if (th == 3) {
						out[c--] = format.thousand;
						th = 0;
					}
					th++;
				}

				if (val != 0)
				{
					out[c--] = '0' + val % 10;
					val /= 10;
				}
				else
					out[c--] = fill;
				max--;
			} while (val > 0 || max > 0);
		}

	}
	return cstr_t(&out[c + 1u], max_char - c);
}

variable<uint>::variable()
{

}

variable<uint>::variable(uint const& val)
{
	set(val);
}

variable<uint>::variable(value<uint> const& val)
{
	set(val);
}

variable<uint>::variable(variable const* val)
{
	set(val ? val->get() : 0);
}

variable<uint>::~variable()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::uinteger, object, ivariant);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::uinteger);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::uinteger, object, ivariant);

comparision_result_t variable<uint>::compare(object const* obj)const
{
	ivariant* var = interface_cast<ivariant>(const_cast<object*>(obj));
	if (var == null)
		return ang::comparision_result::diferent;
	uint val;
	if (var->get_value(val))
		return val < get() ? ang::comparision_result::mayor
		: val > get() ? ang::comparision_result::minor
		: ang::comparision_result::same;
	else
		return ang::comparision_result::diferent;
}

wstring variable<uint>::to_string()const
{
	return ang::move(to_string(*this, text::default_text_format<uint>::format()));
}

wstring variable<uint>::to_string(text::text_format_t f)const
{
	return ang::move(to_string(*this, f));
}

rtti_t const& variable<uint>::value_type()const
{
	return type_of<uint>();
}

bool variable<uint>::set_value(rtti_t const& id, unknown_t val)
{
	if (val == null)
		return false;

	if (id.type_id() == type_of<bool>().type_id())
		set(*static_cast<bool*>(val) ? 1 : 0);
	else if (id.type_id() == type_of<boolean>().type_id())
		set(static_cast<boolean*>(val)->get() ? 1 : 0);

	else if (id.type_id() == type_of<int>().type_id())
		set((uint)*static_cast<int*>(val));
	else if (id.type_id() == type_of<integer>().type_id())
		set((uint)static_cast<integer*>(val)->get());
	else if (id.type_id() == type_of<uint>().type_id())
		set((uint)*static_cast<uint*>(val));
	else if (id.type_id() == type_of<uinteger>().type_id())
		set((uint)static_cast<uinteger*>(val)->get());

	else if (id.type_id() == type_of<long>().type_id())
		set((uint)*static_cast<long*>(val));
	else if (id.type_id() == type_of<variable<long>>().type_id())
		set((uint)static_cast<variable<long>*>(val)->get());
	else if (id.type_id() == type_of<ulong>().type_id())
		set((uint)*static_cast<ulong*>(val));
	else if (id.type_id() == type_of<variable<ulong>>().type_id())
		set((uint)static_cast<variable<ulong>*>(val)->get());

	else if (id.type_id() == type_of<long64>().type_id())
		set((uint)*static_cast<long64*>(val));
	else if (id.type_id() == type_of<integer64>().type_id())
		set((uint)static_cast<integer64*>(val)->get());
	else if (id.type_id() == type_of<ulong64>().type_id())
		set((uint)*static_cast<ulong64*>(val));
	else if (id.type_id() == type_of<uinteger64>().type_id())
		set((uint)static_cast<uinteger64*>(val)->get());

	else if (id.type_id() == type_of<float>().type_id())
		set((uint)*static_cast<float*>(val));
	else if (id.type_id() == type_of<floating>().type_id())
		set((uint)static_cast<floating*>(val)->get());
	else if (id.type_id() == type_of<double>().type_id())
		set((uint)*static_cast<double*>(val));
	else if (id.type_id() == type_of<floating64>().type_id())
		set((uint)static_cast<floating64*>(val)->get());
	else return false;

	return true;
}

bool variable<uint>::get_value(rtti_t const& id, unknown_t val)const
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

variant variable<uint>::clone()const
{
	return new variable<uint>(get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////

value<ulong> variable<ulong>::parse(text::raw_cstr_t str, windex i, int base) {
	text::iparser_t parser = text::iparser::get_parser(str.encoding());
	return parser->to_unsigned(str, i, true, base);
}

wstring variable<ulong>::to_string(value<ulong> val, text::text_format_t f)
{
	const byte max_char = 120;
	text::text_format_flags_t format;
	format.value = f.format_flags();

	switch (format.target)
	{
	case text::text_format::text_: format.value = text::default_text_format<ulong>::format().format_flags(); break;
	case text::text_format::unsigned_: break;
	case text::text_format::signed_: return variable<long>::to_string((long)val.get(), f);
	case text::text_format::float_: return variable<float>::to_string((float)val.get(), f);
	default: return "invalid format"_s;
	}

	char out[max_char] = { 0 };

	int th = -1;
	char fill = format.fill;
	int max = format.max;
	bool neg = false;
	uint c = max_char - 2;
	//int value = val.Get();

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	if (format.thousand != 0)
		th = 0;

	if (format.base == 3)
	{
		if (val == 0)
		{
			out[c--] = '0';
		}
		else do
		{
			if (th >= 0) {
				if (th == 4) {
					out[c--] = format.thousand;
					th = 0;
				}
				th++;
			}
			if (val != 0)
				out[c--] = (0X0001 & val) ? '1' : '0';
			else
				out[c--] = fill;
			val = val >> 1;
			max--;
		} while (val != 0 || max > 0);
	}
	else
	{
		if (val < 0)
		{
			neg = true;
			val *= -1;
		}
		if (format.base == 2)
		{
			if (val == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (th >= 0) {
					if (th == 4) {
						out[c--] = format.thousand;
						th = 0;
					}
					th++;
				}
				if (val != 0)
				{
					out[c--] = hexu[(byte)0X000F & val];
					val = val >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (val > 0 || max > 0);
		}
		else if (format.base == 1)
		{
			if (val == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (th >= 0) {
					if (th == 4) {
						out[c--] = format.thousand;
						th = 0;
					}
					th++;
				}
				if (val != 0)
				{
					out[c--] = hexl[(byte)0X000F & val];
					val = val >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (val > 0 || max > 0);
		}
		else
		{
			if (val == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (th >= 0) {
					if (th == 3) {
						out[c--] = format.thousand;
						th = 0;
					}
					th++;
				}

				if (val != 0)
				{
					out[c--] = '0' + val % 10;
					val /= 10;
				}
				else
					out[c--] = fill;
				max--;
			} while (val > 0 || max > 0);
		}
	}
	return cstr_t(&out[c + 1u], max_char - c);
}

variable<ulong>::variable()
{
}

variable<ulong>::variable(ulong const& val)
{
	set(val);
}

variable<ulong>::variable(value<ulong> const& val)
{
	set(val);
}

variable<ulong>::variable(variable const* val)
{
	set(val ? val->get() : 0);
}

variable<ulong>::~variable()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::ulinteger, object, ivariant);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::ulinteger);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::ulinteger, object, ivariant);

comparision_result_t variable<ulong>::compare(object const* obj)const
{
	ivariant* var = interface_cast<ivariant>(const_cast<object*>(obj));
	if (var == null)
		return ang::comparision_result::diferent;
	ulong val;
	if (var->get_value(val))
		return val < get() ? ang::comparision_result::mayor
		: val > get() ? ang::comparision_result::minor
		: ang::comparision_result::same;
	else
		return ang::comparision_result::diferent;
}

wstring variable<ulong>::to_string()const
{
	return ang::move(to_string(*this, text::default_text_format<ulong>::format()));
}

wstring variable<ulong>::to_string(text::text_format_t f)const
{
	return ang::move(to_string(*this, f));
}

rtti_t const& variable<ulong>::value_type()const
{
	return type_of<ulong>();
}

bool variable<ulong>::set_value(rtti_t const& id, unknown_t val)
{
	if (val == null)
		return false;
	if (id.type_id() == type_of<bool>().type_id())
		set(*static_cast<bool*>(val) ? 1 : 0);
	else if (id.type_id() == type_of<boolean>().type_id())
		set(static_cast<boolean*>(val)->get() ? 1 : 0);

	else if (id.type_id() == type_of<int>().type_id())
		set((ulong)*static_cast<int*>(val));
	else if (id.type_id() == type_of<integer>().type_id())
		set((ulong)static_cast<integer*>(val)->get());
	else if (id.type_id() == type_of<uint>().type_id())
		set((ulong)*static_cast<uint*>(val));
	else if (id.type_id() == type_of<uinteger>().type_id())
		set((ulong)static_cast<uinteger*>(val)->get());


	else if (id.type_id() == type_of<ulong>().type_id())
		set((ulong)*static_cast<long*>(val));
	else if (id.type_id() == type_of<variable<long>>().type_id())
		set((ulong)static_cast<variable<long>*>(val)->get());
	else if (id.type_id() == type_of<ulong>().type_id())
		set((ulong)*static_cast<ulong*>(val));
	else if (id.type_id() == type_of<variable<ulong>>().type_id())
		set((ulong)static_cast<variable<ulong>*>(val)->get());

	else if (id.type_id() == type_of<long64>().type_id())
		set((ulong)*static_cast<long64*>(val));
	else if (id.type_id() == type_of<integer64>().type_id())
		set((ulong)static_cast<integer64*>(val)->get());
	else if (id.type_id() == type_of<ulong64>().type_id())
		set((ulong)*static_cast<ulong64*>(val));
	else if (id.type_id() == type_of<uinteger64>().type_id())
		set((ulong)static_cast<uinteger64*>(val)->get());

	else if (id.type_id() == type_of<float>().type_id())
		set((ulong)*static_cast<float*>(val));
	else if (id.type_id() == type_of<floating>().type_id())
		set((ulong)static_cast<floating*>(val)->get());
	else if (id.type_id() == type_of<double>().type_id())
		set((ulong)*static_cast<double*>(val));
	else if (id.type_id() == type_of<floating64>().type_id())
		set((ulong)static_cast<floating64*>(val)->get());
	else return false;

	return true;
}

bool variable<ulong>::get_value(rtti_t const& id, unknown_t val)const
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

variant variable<ulong>::clone()const
{
	return new variable<ulong>(get());
}


////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////

value<ulong64> variable<ulong64>::parse(text::raw_cstr_t str, windex i, int base) {
	text::iparser_t parser = text::iparser::get_parser(str.encoding());
	return parser->to_unsigned(str, i, true, base);
}

wstring variable<ulong64>::to_string(value<ulong64> val, text::text_format_t f)
{
	const byte max_char = 120;
	text::text_format_flags_t format;
	format.value = f.format_flags();

	switch (format.target)
	{
	case text::text_format::text_: format.value = text::default_text_format<ulong64>::format().format_flags(); break;
	case text::text_format::unsigned_: break;
	case text::text_format::signed_: return variable<long64>::to_string((long64)val.get(), f);
	case text::text_format::float_: return variable<double>::to_string((double)val.get(), f);
	default: return "invalid format"_s;
	}

	char out[max_char] = { 0 };

	int th = -1;
	char fill = format.fill;
	int max = format.max;
	bool neg = false;
	uint c = max_char - 2;
	//int value = val.Get();

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = '0';
	}

	if (format.thousand != 0)
		th = 0;

	if (format.base == 3)
	{
		if (val == 0)
		{
			out[c--] = '0';
		}
		else do
		{
			if (th >= 0) {
				if (th == 4) {
					out[c--] = format.thousand;
					th = 0;
				}
				th++;
			}
			if (val != 0)
				out[c--] = (0X0001 & val) ? '1' : '0';
			else
				out[c--] = fill;
			val = val >> 1;
			max--;
		} while (val != 0 || max > 0);
	}
	else
	{
		if (val < 0)
		{
			neg = true;
			val *= -1;
		}
		if (format.base == 2)
		{
			if (val == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (th >= 0) {
					if (th == 4) {
						out[c--] = format.thousand;
						th = 0;
					}
					th++;
				}
				if (val != 0)
				{
					out[c--] = hexu[(byte)0X000F & val];
					val = val >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (val > 0 || max > 0);
		}
		else if (format.base == 1)
		{
			if (val == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (th >= 0) {
					if (th == 4) {
						out[c--] = format.thousand;
						th = 0;
					}
					th++;
				}
				if (val != 0)
				{
					out[c--] = hexl[(byte)0X000F & val];
					val = val >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (val > 0 || max > 0);
		}
		else
		{
			if (val == 0)
			{
				out[c--] = '0';
			}
			else do
			{
				if (th >= 0) {
					if (th == 3) {
						out[c--] = format.thousand;
						th = 0;
					}
					th++;
				}

				if (val != 0)
				{
					out[c--] = '0' + val % 10;
					val /= 10;
				}
				else
					out[c--] = fill;
				max--;
			} while (val > 0 || max > 0);
		}
	}
	return cstr_t(&out[c + 1u], max_char - c);
}

variable<ulong64>::variable()
{
}

variable<ulong64>::variable(ulong64 const& val)
{
	set(val);
}

variable<ulong64>::variable(value<ulong64> const& val)
{
	set(val);
}

variable<ulong64>::variable(variable const* val)
{
	set(val ? val->get() : 0);
}

variable<ulong64>::~variable()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::uinteger64, object, ivariant);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::uinteger64);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::uinteger64, object, ivariant);


comparision_result_t variable<ulong64>::compare(object const* obj)const
{
	ivariant* var = interface_cast<ivariant>(const_cast<object*>(obj));
	if (var == null)
		return ang::comparision_result::diferent;
	ulong64 val;
	if (var->get_value(val))
		return val < get() ? ang::comparision_result::mayor
		: val > get() ? ang::comparision_result::minor
		: ang::comparision_result::same;
	else
		return ang::comparision_result::diferent;
}

wstring variable<ulong64>::to_string()const
{
	return ang::move(to_string(*this, text::default_text_format<ulong64>::format()));
}

wstring variable<ulong64>::to_string(text::text_format_t f)const
{
	return ang::move(to_string(*this, f));
}

rtti_t const& variable<ulong64>::value_type()const
{
	return type_of<ulong64>();
}

bool variable<ulong64>::set_value(rtti_t const& id, unknown_t val)
{
	if (val == null)
		return false;
	if (id.type_id() == type_of<bool>().type_id())
		set(*static_cast<bool*>(val) ? 1 : 0);
	else if (id.type_id() == type_of<boolean>().type_id())
		set(static_cast<boolean*>(val)->get() ? 1 : 0);

	else if (id.type_id() == type_of<int>().type_id())
		set((ulong64)*static_cast<int*>(val));
	else if (id.type_id() == type_of<integer>().type_id())
		set((ulong64)static_cast<integer*>(val)->get());
	else if (id.type_id() == type_of<uint>().type_id())
		set((ulong64)*static_cast<uint*>(val));
	else if (id.type_id() == type_of<uinteger>().type_id())
		set((ulong64)static_cast<uinteger*>(val)->get());


	else if (id.type_id() == type_of<long>().type_id())
		set((ulong64)*static_cast<long*>(val));
	else if (id.type_id() == type_of<variable<long>>().type_id())
		set((ulong64)static_cast<variable<long>*>(val)->get());
	else if (id.type_id() == type_of<ulong>().type_id())
		set((ulong64)*static_cast<ulong*>(val));
	else if (id.type_id() == type_of<variable<ulong>>().type_id())
		set((ulong64)static_cast<variable<ulong>*>(val)->get());

	else if (id.type_id() == type_of<long64>().type_id())
		set((ulong64)*static_cast<long64*>(val));
	else if (id.type_id() == type_of<integer64>().type_id())
		set((ulong64)static_cast<integer64*>(val)->get());
	else if (id.type_id() == type_of<ulong64>().type_id())
		set((ulong64)*static_cast<ulong64*>(val));
	else if (id.type_id() == type_of<uinteger64>().type_id())
		set((ulong64)static_cast<uinteger64*>(val)->get());

	else if (id.type_id() == type_of<float>().type_id())
		set((ulong64)*static_cast<float*>(val));
	else if (id.type_id() == type_of<floating>().type_id())
		set((ulong64)static_cast<floating*>(val)->get());
	else if (id.type_id() == type_of<double>().type_id())
		set((ulong64)*static_cast<double*>(val));
	else if (id.type_id() == type_of<floating64>().type_id())
		set((ulong64)static_cast<floating64*>(val)->get());
	else return false;

	return true;
}

bool variable<ulong64>::get_value(rtti_t const& id, unknown_t val)const
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

variant variable<ulong64>::clone()const
{
	return new variable<ulong64>(get());
}