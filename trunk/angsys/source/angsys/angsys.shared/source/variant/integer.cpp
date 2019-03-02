#include "pch.h"
#include <angsys.h>

using namespace ang;

#define MY_TYPE ang::variable<int>
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::variable<long>
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::variable<long64>
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

namespace ang
{
	static char hexl[18] = { "0123456789abcdefx" };
	static char hexu[18] = { "0123456789ABCDEFX" };
}


wstring variable<int>::to_string(value<int> val,text::text_format_t f)
{
	const byte max_char = 120;
	text::text_format_flags_t format;
	format.value = f.format_flags();

	switch (format.target)
	{
	case text::text_format::text_: format.value = text::default_text_format<int>::format().format_flags(); break;
	case text::text_format::signed_: break;
	case text::text_format::unsigned_: return variable<uint>::to_string((uint)val.get(), f);
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
		if (neg)
			out[c--] = '-';
		else if (format.sign)
			out[c--] = '+';
	}
	return cstr_t(&out[c + 1u], max_char - c);
}

variable<int>::variable()
	: value<int>(0)
{

}

variable<int>::variable(int const& val)
	: value<int>(val)
{

}

variable<int>::variable(value<int> const& val) 
	: value<int>(val)
{

}

variable<int>::variable(variable const* val)
	: value<int>(val ? val->get() : false)
{

}

variable<int>::~variable()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::integer, object, ivariant);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::integer);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::integer, object, ivariant);

comparision_result_t variable<int>::compare(object const* obj)const
{
	ivariant* var = interface_cast<ivariant>(const_cast<object*>(obj));
	if (var == null)
		return ang::comparision_result::diferent;
	int val;
	if (var->get_value(val))
		return val < get() ? ang::comparision_result::mayor
		: val > get() ? ang::comparision_result::minor
		: ang::comparision_result::same;
	else
		return ang::comparision_result::diferent;
}

wstring variable<int>::to_string()const
{
	return ang::move(to_string(*this, text::default_text_format<int>::format()));
}

wstring variable<int>::to_string(text::text_format_t f)const
{
	return ang::move(to_string(*this, f));
}

rtti_t const& variable<int>::value_type()const
{
	return type_of<int>();
}

bool variable<int>::set_value(rtti_t const& id, unknown_t val)
{
	if (val == null)
		return false;
	if (id.type_id() == type_of<bool>().type_id())
		set(*static_cast<bool*>(val) ? 1 : 0);
	else if (id.type_id() == type_of<boolean>().type_id())
		set(static_cast<boolean*>(val)->get() ? 1 : 0);

	else if (id.type_id() == type_of<int>().type_id())
		set((int)*static_cast<int*>(val));
	else if (id.type_id() == type_of<integer>().type_id())
		set((int)static_cast<integer*>(val)->get());
	else if (id.type_id() == type_of<uint>().type_id())
		set((int)*static_cast<uint*>(val));
	else if (id.type_id() == type_of<uinteger>().type_id())
		set((int)static_cast<uinteger*>(val)->get());

	else if (id.type_id() == type_of<long>().type_id())
		set((int)*static_cast<long*>(val));
	else if (id.type_id() == type_of<variable<long>>().type_id())
		set((int)static_cast<variable<long>*>(val)->get());
	else if (id.type_id() == type_of<ulong>().type_id())
		set((int)*static_cast<ulong*>(val));
	else if (id.type_id() == type_of<variable<ulong>>().type_id())
		set((int)static_cast<variable<ulong>*>(val)->get());

	else if (id.type_id() == type_of<long64>().type_id())
		set((int)*static_cast<long64*>(val));
	else if (id.type_id() == type_of<integer64>().type_id())
		set((int)static_cast<integer64*>(val)->get());
	else if (id.type_id() == type_of<ulong64>().type_id())
		set((int)*static_cast<ulong64*>(val));
	else if (id.type_id() == type_of<uinteger64>().type_id())
		set((int)static_cast<uinteger64*>(val)->get());

	else if (id.type_id() == type_of<float>().type_id())
		set((int)*static_cast<float*>(val));
	else if (id.type_id() == type_of<floating>().type_id())
		set((int)static_cast<floating*>(val)->get());
	else if (id.type_id() == type_of<double>().type_id())
		set((int)*static_cast<double*>(val));
	else if (id.type_id() == type_of<floating64>().type_id())
		set((int)static_cast<floating64*>(val)->get());
	else return false;

	return true;
}

bool variable<int>::get_value(rtti_t const& id, unknown_t val)const
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

variant variable<int>::clone()const
{
	return new variable<int>(get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////

wstring variable<long>::to_string(value<long> val, text::text_format_t f)
{
	const byte max_char = 120;
	text::text_format_flags_t format;
	format.value = f.format_flags();

	switch (format.target)
	{
	case text::text_format::text_: format.value = text::default_text_format<long>::format().format_flags(); break;
	case text::text_format::signed_: break;
	case text::text_format::unsigned_: return variable<ulong>::to_string((ulong)val.get(), f);
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
		if (neg)
			out[c--] = '-';
		else if (format.sign)
			out[c--] = '+';
	}
	return cstr_t(&out[c + 1u], max_char - c);
}

variable<long>::variable()
	: value<long>(0)
{

}

variable<long>::variable(long const& val)
	: value<long>(val)
{

}

variable<long>::variable(value<long> const& val)
	: value<long>(val)
{

}

variable<long>::variable(variable const* val)
	: value<long>(val ? val->get() : false)
{

}

variable<long>::~variable()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::linteger, object, ivariant);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::linteger);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::linteger, object, ivariant);

comparision_result_t variable<long>::compare(object const* obj)const
{
	ivariant* var = interface_cast<ivariant>(const_cast<object*>(obj));
	if (var == null)
		return ang::comparision_result::diferent;
	long val;
	if (var->get_value(val))
		return val < get() ? ang::comparision_result::mayor
		: val > get() ? ang::comparision_result::minor
		: ang::comparision_result::same;
	else
		return ang::comparision_result::diferent;
}

wstring variable<long>::to_string()const
{
	return ang::move(to_string(*this, text::default_text_format<long>::format()));
}

wstring variable<long>::to_string(text::text_format_t f)const
{
	return ang::move(to_string(*this, f));
}

rtti_t const& variable<long>::value_type()const
{
	return type_of<long>();
}

bool variable<long>::set_value(rtti_t const& id, unknown_t val)
{
	if (val == null)
		return false;
	if (id.type_id() == type_of<bool>().type_id())
		set(*static_cast<bool*>(val) ? 1 : 0);
	else if (id.type_id() == type_of<boolean>().type_id())
		set(static_cast<boolean*>(val)->get() ? 1 : 0);

	else if (id.type_id() == type_of<int>().type_id())
		set((long)*static_cast<int*>(val));
	else if (id.type_id() == type_of<integer>().type_id())
		set((long)static_cast<integer*>(val)->get());
	else if (id.type_id() == type_of<uint>().type_id())
		set((long)*static_cast<uint*>(val));
	else if (id.type_id() == type_of<uinteger>().type_id())
		set((long)static_cast<uinteger*>(val)->get());


	else if (id.type_id() == type_of<long>().type_id())
		set((long)*static_cast<long*>(val));
	else if (id.type_id() == type_of<variable<long>>().type_id())
		set((long)static_cast<variable<long>*>(val)->get());
	else if (id.type_id() == type_of<ulong>().type_id())
		set((long)*static_cast<ulong*>(val));
	else if (id.type_id() == type_of<variable<ulong>>().type_id())
		set((long)static_cast<variable<ulong>*>(val)->get());

	else if (id.type_id() == type_of<long64>().type_id())
		set((long)*static_cast<long64*>(val));
	else if (id.type_id() == type_of<integer64>().type_id())
		set((long)static_cast<integer64*>(val)->get());
	else if (id.type_id() == type_of<ulong64>().type_id())
		set((long)*static_cast<ulong64*>(val));
	else if (id.type_id() == type_of<uinteger64>().type_id())
		set((long)static_cast<uinteger64*>(val)->get());

	else if (id.type_id() == type_of<float>().type_id())
		set((long)*static_cast<float*>(val));
	else if (id.type_id() == type_of<floating>().type_id())
		set((long)static_cast<floating*>(val)->get());
	else if (id.type_id() == type_of<double>().type_id())
		set((long)*static_cast<double*>(val));
	else if (id.type_id() == type_of<floating64>().type_id())
		set((long)static_cast<floating64*>(val)->get());
	else return false;

	return true;
}

bool variable<long>::get_value(rtti_t const& id, unknown_t val)const
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

variant variable<long>::clone()const
{
	return new variable<long>(get());
}


////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////

wstring variable<long64>::to_string(value<long64> val, text::text_format_t f)
{
	const byte max_char = 120;
	text::text_format_flags_t format;
	format.value = f.format_flags();

	switch (format.target)
	{
	case text::text_format::text_: format.value = text::default_text_format<ulong64>::format().format_flags(); break;
	case text::text_format::signed_: break;
	case text::text_format::unsigned_: return variable<ulong64>::to_string((ulong64)val.get(), f);
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
		if (neg)
			out[c--] = '-';
		else if (format.sign)
			out[c--] = '+';
	}
	return cstr_t(&out[c + 1u], max_char - c);
}

variable<long64>::variable()
	: value<long64>(0)
{

}

variable<long64>::variable(long64 const& val)
	: value<long64>(val)
{

}

variable<long64>::variable(value<long64> const& val)
	: value<long64>(val)
{

}

variable<long64>::variable(variable const* val)
	: value<long64>(val ? val->get() : false)
{

}

variable<long64>::~variable()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::integer64, object, ivariant);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::integer64);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::integer64, object, ivariant);

comparision_result_t variable<long64>::compare(object const* obj)const
{
	ivariant* var = interface_cast<ivariant>(const_cast<object*>(obj));
	if (var == null)
		return ang::comparision_result::diferent;
	long64 val;
	if (var->get_value(val))
		return val < get() ? ang::comparision_result::mayor
		: val > get() ? ang::comparision_result::minor
		: ang::comparision_result::same;
	else
		return ang::comparision_result::diferent;
}

wstring variable<long64>::to_string()const
{
	return ang::move(to_string(*this, text::default_text_format<long64>::format()));
}

wstring variable<long64>::to_string(text::text_format_t f)const
{
	return ang::move(to_string(*this, f));
}

rtti_t const& variable<long64>::value_type()const
{
	return type_of<long64>();
}

bool variable<long64>::set_value(rtti_t const& id, unknown_t val)
{
	if (val == null)
		return false;
	if (id.type_id() == type_of<bool>().type_id())
		set(*static_cast<bool*>(val) ? 1 : 0);
	else if (id.type_id() == type_of<boolean>().type_id())
		set(static_cast<boolean*>(val)->get() ? 1 : 0);

	else if (id.type_id() == type_of<int>().type_id())
		set((long64)*static_cast<int*>(val));
	else if (id.type_id() == type_of<integer>().type_id())
		set((long64)static_cast<integer*>(val)->get());
	else if (id.type_id() == type_of<uint>().type_id())
		set((long64)*static_cast<uint*>(val));
	else if (id.type_id() == type_of<uinteger>().type_id())
		set((long64)static_cast<uinteger*>(val)->get());


	else if (id.type_id() == type_of<long>().type_id())
		set((long64)*static_cast<long*>(val));
	else if (id.type_id() == type_of<variable<long>>().type_id())
		set((long64)static_cast<variable<long>*>(val)->get());
	else if (id.type_id() == type_of<ulong>().type_id())
		set((long64)*static_cast<ulong*>(val));
	else if (id.type_id() == type_of<variable<ulong>>().type_id())
		set((long64)static_cast<variable<ulong>*>(val)->get());

	else if (id.type_id() == type_of<long64>().type_id())
		set((long64)*static_cast<long64*>(val));
	else if (id.type_id() == type_of<integer64>().type_id())
		set((long64)static_cast<integer64*>(val)->get());
	else if (id.type_id() == type_of<ulong64>().type_id())
		set((long64)*static_cast<ulong64*>(val));
	else if (id.type_id() == type_of<uinteger64>().type_id())
		set((long64)static_cast<uinteger64*>(val)->get());

	else if (id.type_id() == type_of<float>().type_id())
		set((long64)*static_cast<float*>(val));
	else if (id.type_id() == type_of<floating>().type_id())
		set((long64)static_cast<floating*>(val)->get());
	else if (id.type_id() == type_of<double>().type_id())
		set((long64)*static_cast<double*>(val));
	else if (id.type_id() == type_of<floating64>().type_id())
		set((long64)static_cast<floating64*>(val)->get());
	else return false;

	return true;
}

bool variable<long64>::get_value(rtti_t const& id, unknown_t val)const
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

variant variable<long64>::clone()const
{
	return new variable<long64>(get());
}