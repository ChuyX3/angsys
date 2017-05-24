/*********************************************************************************************************************/
/*   File Name: ushort.cpp                                                                                           */
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

value<unsigned short>::value() : _value() {}
value<unsigned short>::value(type val) : _value(val) {}
value<unsigned short>::value(value<unsigned short> const& other) : _value(other._value) {}
value<unsigned short>::value(value<unsigned short> && other) : _value(ang::move(other._value)) {}
unsigned short & value<unsigned short>::get() { return _value; }
unsigned short const& value<unsigned short>::get()const { return _value; }
void value<unsigned short>::set(type val) { _value = ang::move(val); }
value<unsigned short>& value<unsigned short>::operator = (type val) { set(ang::move(val)); return*this; }
value<unsigned short>& value<unsigned short>::operator = (value<unsigned short> const& val) { set(val._value); return*this; }
value<unsigned short>& value<unsigned short>::operator = (value<unsigned short> && val) { set(ang::move(val._value)); return*this; }
value<unsigned short>& value<unsigned short>::operator += (type val) { _value += val; return*this; }
value<unsigned short>& value<unsigned short>::operator += (value<unsigned short> const& val) { _value += val._value; return*this; }
value<unsigned short>& value<unsigned short>::operator -= (type val) { _value -= val; return*this; }
value<unsigned short>& value<unsigned short>::operator -= (value<unsigned short> const& val) { _value -= val._value; return*this; }
value<unsigned short>& value<unsigned short>::operator *= (type val) { _value *= val; return*this; }
value<unsigned short>& value<unsigned short>::operator *= (value<unsigned short> const& val) { _value *= val._value; return*this; }
value<unsigned short>& value<unsigned short>::operator /= (type val) { _value /= val; return*this; }
value<unsigned short>& value<unsigned short>::operator /= (value<unsigned short> const& val) { _value /= val._value; return*this; }


value<unsigned short const>::value(unsigned short val) : _value(val) {}
value<unsigned short const>::value(value<unsigned short const> const& other) : _value(other._value) {}
unsigned short const& value<unsigned short const>::get()const { return _value; }

value<unsigned short&>::value(unsigned short& val) : _value(val) {}
value<unsigned short&>::value(value<unsigned short&>& other) : _value(other._value) {}
value<unsigned short&>::value(value<unsigned short>& other) : _value(other.get()) {}
unsigned short & value<unsigned short&>::get() { return _value; }
unsigned short const& value<unsigned short&>::get()const { return _value; }
void value<unsigned short&>::set(type val) { _value = ang::move(val); }
value<unsigned short>& value<unsigned short&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short>& value<unsigned short&>::operator = (value<unsigned short&> const& val) { set(val._value); return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short>& value<unsigned short&>::operator = (value<unsigned short> val) { set(ang::move(val.get())); return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short>& value<unsigned short&>::operator += (type val) { _value += val; return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short>& value<unsigned short&>::operator += (value const& val) { _value += val._value; return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short>& value<unsigned short&>::operator -= (type val) { _value -= val; return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short>& value<unsigned short&>::operator -= (value const& val) { _value -= val._value; return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short>& value<unsigned short&>::operator *= (type val) { _value *= val; return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short>& value<unsigned short&>::operator *= (value const& val) { _value *= val._value; return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short>& value<unsigned short&>::operator /= (type val) { _value /= val; return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short>& value<unsigned short&>::operator /= (value const& val) { _value /= val._value; return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short>& value<unsigned short&>::operator ++() { ++_value; return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short> value<unsigned short&>::operator ++(int) { auto ret = _value++; return ret; }
value<unsigned short>& value<unsigned short&>::operator --() { --_value; return*reinterpret_cast<value<unsigned short>*>(&_value); }
value<unsigned short> value<unsigned short&>::operator --(int) { auto ret = _value--; return ret; }

value<unsigned short const&>::value(unsigned short const& val) : _value(val) {}
value<unsigned short const&>::value(value<unsigned short const&> const& other) : _value(other._value) {}
unsigned short const& value<unsigned short const&>::get()const { return _value; }

value<unsigned short*>::value() : _value(null) {}
value<unsigned short*>::value(unsigned short* val) : _value(val) {}
value<unsigned short*>::value(ang::nullptr_t) : _value(null) {}
value<unsigned short*>::value(value<unsigned short>* val) : _value(&val->get()) {}
value<unsigned short*>::value(value const& other) : _value(other._value) {}
value<unsigned short*>::value(value && other) : _value(ang::move(other._value)) {}
unsigned short* & value<unsigned short*>::get() { return _value; }
unsigned short* const& value<unsigned short*>::get()const { return _value; }
void value<unsigned short*>::set(unsigned short* val) { _value = ang::move(val); }
value<unsigned short*>& value<unsigned short*>::operator = (unsigned short* val) { set(ang::move(val)); return*this; }
value<unsigned short*>& value<unsigned short*>::operator = (value<unsigned short*> const& val) { set(val._value); return*this; }
value<unsigned short*>& value<unsigned short*>::operator = (value<unsigned short*> && val) { set(ang::move(val._value)); return*this; }


value<unsigned short const*>::value() : _value(null) {}
value<unsigned short const*>::value(type val) : _value(val) {}
value<unsigned short const*>::value(ang::nullptr_t) : _value(null) {}
value<unsigned short const*>::value(value<unsigned short> const* val) : _value(&val->get()) {}
value<unsigned short const*>::value(value const& other) : _value(other._value) {}
value<unsigned short const*>::value(value && other) : _value(ang::move(other._value)) {}
unsigned short const* & value<unsigned short const*>::get() { return _value; }
unsigned short const* const& value<unsigned short const*>::get()const { return _value; }
void value<unsigned short const*>::set(type val) { _value = ang::move(val); }
value<unsigned short const*>& value<unsigned short const*>::operator = (type val) { set(ang::move(val)); return*this; }
value<unsigned short const*>& value<unsigned short const*>::operator = (value<unsigned short const*> const& val) { set(val._value); return*this; }
value<unsigned short const*>& value<unsigned short const*>::operator = (value<unsigned short const*> && val) { set(ang::move(val._value)); return*this; }
