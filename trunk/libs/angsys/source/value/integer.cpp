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

value<int> variable<int>::parse(cstr_t str, windex i, int base) {
	return (int)text::parser::to_signed(str, i, base);
}

variable<int>::variable()
{

}

variable<int>::variable(int const& val)
{
	set(val);
}

variable<int>::variable(value<int> const& val) 
{
	set(val);
}

variable<int>::variable(variable const* val)
{
	set(val ? val->get() : 0);
}

variable<int>::~variable()
{

}

string variable<int>::to_string()const
{
	return ang::to_string<int>(get());
}

string variable<int>::to_string(text::text_format_t f)const
{
	return ang::to_string<int>(get(), f);
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

value<long> variable<long>::parse(cstr_t str, windex i, int base) {
	return (long)text::parser::to_signed(str, i, base);
}

variable<long>::variable()
{

}

variable<long>::variable(long const& val)
{
	set(val);
}

variable<long>::variable(value<long> const& val)
{
	set(val);
}

variable<long>::variable(variable const* val)
{
	set(val ? val->get() : 0);
}

variable<long>::~variable()
{

}

string variable<long>::to_string()const
{
	return ang::to_string<long>(get());
}

string variable<long>::to_string(text::text_format_t f)const
{
	return ang::to_string<long>(get(), f);
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

value<long64> variable<long64>::parse(cstr_t str, windex i, int base) {
	return text::parser::to_signed(str, i, base);
}

variable<long64>::variable()
{

}

variable<long64>::variable(long64 const& val)
{
	set(val);
}

variable<long64>::variable(value<long64> const& val)
{
	set(val);
}

variable<long64>::variable(variable const* val)
{
	set(val ? val->get() : 0);
}

variable<long64>::~variable()
{

}

string variable<long64>::to_string()const
{
	return ang::to_string<long64>(get());
}

string variable<long64>::to_string(text::text_format_t f)const
{
	return ang::to_string<long64>(get(), f);
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