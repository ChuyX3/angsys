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

value<float> variable<float>::parse(cstr_t str, windex i, bool exp)
{
	return (float)text::parser::to_floating(str, i, exp);
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

string variable<float>::to_string()const
{
	return ang::to_string<float>(get());
}

string variable<float>::to_string(text::text_format_t f)const
{
	return ang::to_string<float>(get(), f);
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

value<double> variable<double>::parse(cstr_t str, windex i, bool exp)
{
	return text::parser::to_floating(str, i, exp);
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

string variable<double>::to_string()const
{
	return ang::to_string<double>(get());
}

string variable<double>::to_string(text::text_format_t f)const
{
	return ang::to_string<double>(get(), f);
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
