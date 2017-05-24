/*********************************************************************************************************************/
/*   File Name: uint.cpp                                                                                           */
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

value<unsigned int>::value() : _value() {}
value<unsigned int>::value(type val) : _value(val) {}
value<unsigned int>::value(value<unsigned int> const& other) : _value(other._value) {}
value<unsigned int>::value(value<unsigned int> && other) : _value(ang::move(other._value)) {}
unsigned int & value<unsigned int>::get() { return _value; }
unsigned int const& value<unsigned int>::get()const { return _value; }
void value<unsigned int>::set(type val) { _value = ang::move(val); }
value<unsigned int>& value<unsigned int>::operator = (type val) { set(ang::move(val)); return*this; }
value<unsigned int>& value<unsigned int>::operator = (value<unsigned int> const& val) { set(val._value); return*this; }
value<unsigned int>& value<unsigned int>::operator = (value<unsigned int> && val) { set(ang::move(val._value)); return*this; }
value<unsigned int>& value<unsigned int>::operator += (type val) { _value += val; return*this; }
value<unsigned int>& value<unsigned int>::operator += (value<unsigned int> const& val) { _value += val._value; return*this; }
value<unsigned int>& value<unsigned int>::operator -= (type val) { _value -= val; return*this; }
value<unsigned int>& value<unsigned int>::operator -= (value<unsigned int> const& val) { _value -= val._value; return*this; }
value<unsigned int>& value<unsigned int>::operator *= (type val) { _value *= val; return*this; }
value<unsigned int>& value<unsigned int>::operator *= (value<unsigned int> const& val) { _value *= val._value; return*this; }
value<unsigned int>& value<unsigned int>::operator /= (type val) { _value /= val; return*this; }
value<unsigned int>& value<unsigned int>::operator /= (value<unsigned int> const& val) { _value /= val._value; return*this; }
value<unsigned int>& value<unsigned int>::operator ++() { ++_value; return*this; }
value<unsigned int> value<unsigned int>::operator ++(int) { auto ret = _value++; return ret; }
value<unsigned int>& value<unsigned int>::operator --() { --_value; return*this; }
value<unsigned int> value<unsigned int>::operator --(int) { auto ret = _value--; return ret; }


value<unsigned int const>::value(unsigned int val) : _value(val) {}
value<unsigned int const>::value(value<unsigned int const> const& other) : _value(other._value) {}
unsigned int const& value<unsigned int const>::get()const { return _value; }

value<unsigned int&>::value(unsigned int& val) : _value(val) {}
value<unsigned int&>::value(value<unsigned int&>& other) : _value(other._value) {}
value<unsigned int&>::value(value<unsigned int>& other) : _value(other.get()) {}
unsigned int & value<unsigned int&>::get() { return _value; }
unsigned int const& value<unsigned int&>::get()const { return _value; }
void value<unsigned int&>::set(type val) { _value = ang::move(val); }
value<unsigned int>& value<unsigned int&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int>& value<unsigned int&>::operator = (value<unsigned int&> const& val) { set(val._value); return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int>& value<unsigned int&>::operator = (value<unsigned int> val) { set(ang::move(val.get())); return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int>& value<unsigned int&>::operator += (type val) { _value += val; return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int>& value<unsigned int&>::operator += (value const& val) { _value += val._value; return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int>& value<unsigned int&>::operator -= (type val) { _value -= val; return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int>& value<unsigned int&>::operator -= (value const& val) { _value -= val._value; return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int>& value<unsigned int&>::operator *= (type val) { _value *= val; return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int>& value<unsigned int&>::operator *= (value const& val) { _value *= val._value; return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int>& value<unsigned int&>::operator /= (type val) { _value /= val; return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int>& value<unsigned int&>::operator /= (value const& val) { _value /= val._value; return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int>& value<unsigned int&>::operator ++() { ++_value; return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int> value<unsigned int&>::operator ++(int) { auto ret = _value++; return ret; }
value<unsigned int>& value<unsigned int&>::operator --() { --_value; return*reinterpret_cast<value<unsigned int>*>(&_value); }
value<unsigned int> value<unsigned int&>::operator --(int) { auto ret = _value--; return ret; }

value<unsigned int const&>::value(unsigned int const& val) : _value(val) {}
value<unsigned int const&>::value(value<unsigned int const&> const& other) : _value(other._value) {}
unsigned int const& value<unsigned int const&>::get()const { return _value; }

value<unsigned int*>::value() : _value(null) {}
value<unsigned int*>::value(unsigned int* val) : _value(val) {}
value<unsigned int*>::value(ang::nullptr_t) : _value(null) {}
value<unsigned int*>::value(value<unsigned int>* val) : _value(&val->get()) {}
value<unsigned int*>::value(value const& other) : _value(other._value) {}
value<unsigned int*>::value(value && other) : _value(ang::move(other._value)) {}
unsigned int* & value<unsigned int*>::get() { return _value; }
unsigned int* const& value<unsigned int*>::get()const { return _value; }
void value<unsigned int*>::set(unsigned int* val) { _value = ang::move(val); }
value<unsigned int*>& value<unsigned int*>::operator = (unsigned int* val) { set(ang::move(val)); return*this; }
value<unsigned int*>& value<unsigned int*>::operator = (value<unsigned int*> const& val) { set(val._value); return*this; }
value<unsigned int*>& value<unsigned int*>::operator = (value<unsigned int*> && val) { set(ang::move(val._value)); return*this; }


value<unsigned int const*>::value() : _value(null) {}
value<unsigned int const*>::value(type val) : _value(val) {}
value<unsigned int const*>::value(ang::nullptr_t) : _value(null) {}
value<unsigned int const*>::value(value<unsigned int> const* val) : _value(&val->get()) {}
value<unsigned int const*>::value(value const& other) : _value(other._value) {}
value<unsigned int const*>::value(value && other) : _value(ang::move(other._value)) {}
unsigned int const* & value<unsigned int const*>::get() { return _value; }
unsigned int const* const& value<unsigned int const*>::get()const { return _value; }
void value<unsigned int const*>::set(type val) { _value = ang::move(val); }
value<unsigned int const*>& value<unsigned int const*>::operator = (type val) { set(ang::move(val)); return*this; }
value<unsigned int const*>& value<unsigned int const*>::operator = (value<unsigned int const*> const& val) { set(val._value); return*this; }
value<unsigned int const*>& value<unsigned int const*>::operator = (value<unsigned int const*> && val) { set(ang::move(val._value)); return*this; }
