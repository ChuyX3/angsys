/*********************************************************************************************************************/
/*   File Name: short.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/
#include "pch.h"
#include "angsys.h"

using namespace ang;

value<short>::value() : _value() {}
value<short>::value(type val) : _value(val) {}
value<short>::value(value<short> const& other) : _value(other._value) {}
value<short>::value(value<short> && other) : _value(ang::move(other._value)) {}
short & value<short>::get() { return _value; }
short const& value<short>::get()const { return _value; }
void value<short>::set(type val) { _value = ang::move(val); }
value<short>& value<short>::operator = (type val) { set(ang::move(val)); return*this; }
value<short>& value<short>::operator = (value<short> const& val) { set(val._value); return*this; }
value<short>& value<short>::operator = (value<short> && val) { set(ang::move(val._value)); return*this; }
value<short>& value<short>::operator += (type val) { _value += val; return*this; }
value<short>& value<short>::operator += (value<short> const& val) { _value += val._value; return*this; }
value<short>& value<short>::operator -= (type val) { _value -= val; return*this; }
value<short>& value<short>::operator -= (value<short> const& val) { _value -= val._value; return*this; }
value<short>& value<short>::operator *= (type val) { _value *= val; return*this; }
value<short>& value<short>::operator *= (value<short> const& val) { _value *= val._value; return*this; }
value<short>& value<short>::operator /= (type val) { _value /= val; return*this; }
value<short>& value<short>::operator /= (value<short> const& val) { _value /= val._value; return*this; }


value<short const>::value(short val) : _value(val) {}
value<short const>::value(value<short const> const& other) : _value(other._value) {}
short const& value<short const>::get()const { return _value; }

value<short&>::value(short& val) : _value(val) {}
value<short&>::value(value<short&>& other) : _value(other._value) {}
value<short&>::value(value<short>& other) : _value(other.get()) {}
short & value<short&>::get() { return _value; }
short const& value<short&>::get()const { return _value; }
void value<short&>::set(type val) { _value = ang::move(val); }
value<short>& value<short&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<short>*>(&_value); }
value<short>& value<short&>::operator = (value<short&> const& val) { set(val._value); return*reinterpret_cast<value<short>*>(&_value); }
value<short>& value<short&>::operator = (value<short> val) { set(ang::move(val.get())); return*reinterpret_cast<value<short>*>(&_value); }
value<short>& value<short&>::operator += (type val) { _value += val; return*reinterpret_cast<value<short>*>(&_value); }
value<short>& value<short&>::operator += (value const& val) { _value += val._value; return*reinterpret_cast<value<short>*>(&_value); }
value<short>& value<short&>::operator -= (type val) { _value -= val; return*reinterpret_cast<value<short>*>(&_value); }
value<short>& value<short&>::operator -= (value const& val) { _value -= val._value; return*reinterpret_cast<value<short>*>(&_value); }
value<short>& value<short&>::operator *= (type val) { _value *= val; return*reinterpret_cast<value<short>*>(&_value); }
value<short>& value<short&>::operator *= (value const& val) { _value *= val._value; return*reinterpret_cast<value<short>*>(&_value); }
value<short>& value<short&>::operator /= (type val) { _value /= val; return*reinterpret_cast<value<short>*>(&_value); }
value<short>& value<short&>::operator /= (value const& val) { _value /= val._value; return*reinterpret_cast<value<short>*>(&_value); }
value<short>& value<short&>::operator ++() { ++_value; return*reinterpret_cast<value<short>*>(&_value); }
value<short> value<short&>::operator ++(int) { auto ret = _value++; return ret; }
value<short>& value<short&>::operator --() { --_value; return*reinterpret_cast<value<short>*>(&_value); }
value<short> value<short&>::operator --(int) { auto ret = _value--; return ret; }

value<short const&>::value(short const& val) : _value(val) {}
value<short const&>::value(value<short const&> const& other) : _value(other._value) {}
short const& value<short const&>::get()const { return _value; }

value<short*>::value() : _value(null) {}
value<short*>::value(short* val) : _value(val) {}
value<short*>::value(ang::nullptr_t) : _value(null) {}
value<short*>::value(value<short>* val) : _value(&val->get()) {}
value<short*>::value(value const& other) : _value(other._value) {}
value<short*>::value(value && other) : _value(ang::move(other._value)) {}
short* & value<short*>::get() { return _value; }
short* const& value<short*>::get()const { return _value; }
void value<short*>::set(short* val) { _value = ang::move(val); }
value<short*>& value<short*>::operator = (short* val) { set(ang::move(val)); return*this; }
value<short*>& value<short*>::operator = (value<short*> const& val) { set(val._value); return*this; }
value<short*>& value<short*>::operator = (value<short*> && val) { set(ang::move(val._value)); return*this; }


value<short const*>::value() : _value(null) {}
value<short const*>::value(type val) : _value(val) {}
value<short const*>::value(ang::nullptr_t) : _value(null) {}
value<short const*>::value(value<short> const* val) : _value(&val->get()) {}
value<short const*>::value(value const& other) : _value(other._value) {}
value<short const*>::value(value && other) : _value(ang::move(other._value)) {}
short const* & value<short const*>::get() { return _value; }
short const* const& value<short const*>::get()const { return _value; }
void value<short const*>::set(type val) { _value = ang::move(val); }
value<short const*>& value<short const*>::operator = (type val) { set(ang::move(val)); return*this; }
value<short const*>& value<short const*>::operator = (value<short const*> const& val) { set(val._value); return*this; }
value<short const*>& value<short const*>::operator = (value<short const*> && val) { set(ang::move(val._value)); return*this; }
