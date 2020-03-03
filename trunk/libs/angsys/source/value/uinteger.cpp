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

value<uint> variable<uint>::parse(cstr_t str, windex i, int base) {
	return (uint)text::parser::to_unsigned(str, i, base);
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

string variable<uint>::to_string()const
{
	return ang::to_string<uint>(get());
}

string variable<uint>::to_string(text::text_format_t f)const
{
	return ang::to_string<uint>(get(), f);
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

value<ulong> variable<ulong>::parse(cstr_t str, windex i, int base) {
	return (ulong)text::parser::to_unsigned(str, i, base);
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

string variable<ulong>::to_string()const
{
	return ang::to_string<ulong>(get());
}

string variable<ulong>::to_string(text::text_format_t f)const
{
	return ang::to_string<ulong>(get(), f);
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

value<ulong64> variable<ulong64>::parse(cstr_t str, windex i, int base) {
	return text::parser::to_unsigned(str, i, base);
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

string variable<ulong64>::to_string()const
{
	return ang::to_string<ulong64>(get());
}

string variable<ulong64>::to_string(text::text_format_t f)const
{
	return ang::to_string<ulong64>(get(), f);
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