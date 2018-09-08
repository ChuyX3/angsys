#include "pch.h"
#include <angsys.h>
#include "format_parser.h"
using namespace ang;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::ivariant, ang::interface);

#define MY_TYPE ang::ivariant
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE



intf_wrapper<ivariant>::intf_wrapper(bool value)
	: intf_wrapper()
{
	set(new variable<bool>(value));
}

intf_wrapper<ivariant>::intf_wrapper(int value)
	: intf_wrapper()
{
	set(new variable<int>(value));
}

intf_wrapper<ivariant>::intf_wrapper(uint value)
	: intf_wrapper()
{
	set(new variable<uint>(value));
}

intf_wrapper<ivariant>::intf_wrapper(long value)
	: intf_wrapper()
{
	set(new variable<int>(value));
}

intf_wrapper<ivariant>::intf_wrapper(ulong value)
	: intf_wrapper()
{
	set(new variable<uint>(value));
}

intf_wrapper<ivariant>::intf_wrapper(long64 value)
	: intf_wrapper()
{
	set(new variable<long64>(value));
}

intf_wrapper<ivariant>::intf_wrapper(ulong64 value)
	: intf_wrapper()
{
	set(new variable<ulong64>(value));
}

intf_wrapper<ivariant>::intf_wrapper(float value)
	: intf_wrapper()
{
	set(new variable<float>(value));
}

intf_wrapper<ivariant>::intf_wrapper(double value)
	: intf_wrapper()
{
	set(new variable<double>(value));
}


intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator=(bool value)
{
	if (is_empty() || !_ptr->set_value(value))
		set(new variable<bool>(value));
	return*this;
}

intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator=(int value)
{
	if (is_empty() || !_ptr->set_value(value))
		set(new variable<int>(value));
	return*this;
}

intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator=(uint value)
{
	if (is_empty() || !_ptr->set_value(value))
		set(new variable<uint>(value));
	return*this;
}

intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator=(long value)
{
	if (is_empty() || !_ptr->set_value(value))
		set(new variable<int>(value));
	return*this;
}

intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator=(ulong value)
{
	if (is_empty() || !_ptr->set_value(value))
		set(new variable<uint>(value));
	return*this;
}

intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator=(long64 value)
{
	if (is_empty() || !_ptr->set_value(value))
		set(new variable<long64>(value));
	return*this;
}

intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator=(ulong64 value)
{
	if (is_empty() || !_ptr->set_value(value))
		set(new variable<ulong64>(value));
	return*this;
}

intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator=(float value)
{
	if (is_empty() || !_ptr->set_value(value))
		set(new variable<float>(value));
	return*this;
}

intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator=(double value)
{
	if (is_empty() || !_ptr->set_value(value))
		set(new variable<double>(value));
	return*this;
}
