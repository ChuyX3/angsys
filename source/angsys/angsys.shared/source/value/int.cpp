/*********************************************************************************************************************/
/*   File Name: int.cpp                                                                                              */
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

value<int>::value() : _value() {}
value<int>::value(type val) : _value(val) {}
value<int>::value(value<int> const& other) : _value(other._value) {}
value<int>::value(value<int> && other) : _value(ang::move(other._value)) {}
int & value<int>::get() { return _value; }
int const& value<int>::get()const { return _value; }
void value<int>::set(type val) { _value = ang::move(val); }
value<int>& value<int>::operator = (type val) { set(ang::move(val)); return*this; }
value<int>& value<int>::operator = (value<int> const& val) { set(val._value); return*this; }
value<int>& value<int>::operator = (value<int> && val) { set(ang::move(val._value)); return*this; }
value<int>& value<int>::operator += (type val) { _value += val; return*this; }
value<int>& value<int>::operator += (value<int> const& val) { _value += val._value; return*this; }
value<int>& value<int>::operator -= (type val) { _value -= val; return*this; }
value<int>& value<int>::operator -= (value<int> const& val) { _value -= val._value; return*this; }
value<int>& value<int>::operator *= (type val) { _value *= val; return*this; }
value<int>& value<int>::operator *= (value<int> const& val) { _value *= val._value; return*this; }
value<int>& value<int>::operator /= (type val) { _value /= val; return*this; }
value<int>& value<int>::operator /= (value<int> const& val) { _value /= val._value; return*this; }


value<int const>::value(int val) : _value(val) {}
value<int const>::value(value<int const> const& other) : _value(other._value) {}
int const& value<int const>::get()const { return _value; }

value<int&>::value(int& val) : _value(val) {}
value<int&>::value(value<int&>& other) : _value(other._value) {}
value<int&>::value(value<int>& other) : _value(other.get()) {}
int & value<int&>::get() { return _value; }
int const& value<int&>::get()const { return _value; }
void value<int&>::set(type val) { _value = ang::move(val); }
value<int>& value<int&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<int>*>(&_value); }
value<int>& value<int&>::operator = (value<int&> const& val) { set(val._value); return*reinterpret_cast<value<int>*>(&_value); }
value<int>& value<int&>::operator = (value<int> val) { set(ang::move(val.get())); return*reinterpret_cast<value<int>*>(&_value); }
value<int>& value<int&>::operator += (type val) { _value += val; return*reinterpret_cast<value<int>*>(&_value); }
value<int>& value<int&>::operator += (value const& val) { _value += val._value; return*reinterpret_cast<value<int>*>(&_value); }
value<int>& value<int&>::operator -= (type val) { _value -= val; return*reinterpret_cast<value<int>*>(&_value); }
value<int>& value<int&>::operator -= (value const& val) { _value -= val._value; return*reinterpret_cast<value<int>*>(&_value); }
value<int>& value<int&>::operator *= (type val) { _value *= val; return*reinterpret_cast<value<int>*>(&_value); }
value<int>& value<int&>::operator *= (value const& val) { _value *= val._value; return*reinterpret_cast<value<int>*>(&_value); }
value<int>& value<int&>::operator /= (type val) { _value /= val; return*reinterpret_cast<value<int>*>(&_value); }
value<int>& value<int&>::operator /= (value const& val) { _value /= val._value; return*reinterpret_cast<value<int>*>(&_value); }
value<int>& value<int&>::operator ++() { ++_value; return*reinterpret_cast<value<int>*>(&_value); }
value<int> value<int&>::operator ++(int) { auto ret = _value++; return ret; }
value<int>& value<int&>::operator --() { --_value; return*reinterpret_cast<value<int>*>(&_value); }
value<int> value<int&>::operator --(int) { auto ret = _value--; return ret; }

value<int const&>::value(int const& val) : _value(val) {}
value<int const&>::value(value<int const&> const& other) : _value(other._value) {}
int const& value<int const&>::get()const { return _value; }

value<int*>::value() : _value(null) {}
value<int*>::value(int* val) : _value(val) {}
value<int*>::value(ang::nullptr_t) : _value(null) {}
value<int*>::value(value<int>* val) : _value(&val->get()) {}
value<int*>::value(value const& other) : _value(other._value) {}
value<int*>::value(value && other) : _value(ang::move(other._value)) {}
int* & value<int*>::get() { return _value; }
int* const& value<int*>::get()const { return _value; }
void value<int*>::set(int* val) { _value = ang::move(val); }
value<int*>& value<int*>::operator = (int* val) { set(ang::move(val)); return*this; }
value<int*>& value<int*>::operator = (value<int*> const& val) { set(val._value); return*this; }
value<int*>& value<int*>::operator = (value<int*> && val) { set(ang::move(val._value)); return*this; }


value<int const*>::value() : _value(null) {}
value<int const*>::value(type val) : _value(val) {}
value<int const*>::value(ang::nullptr_t) : _value(null) {}
value<int const*>::value(value<int> const* val) : _value(&val->get()) {}
value<int const*>::value(value const& other) : _value(other._value) {}
value<int const*>::value(value && other) : _value(ang::move(other._value)) {}
int const* & value<int const*>::get() { return _value; }
int const* const& value<int const*>::get()const { return _value; }
void value<int const*>::set(type val) { _value = ang::move(val); }
value<int const*>& value<int const*>::operator = (type val) { set(ang::move(val)); return*this; }
value<int const*>& value<int const*>::operator = (value<int const*> const& val) { set(val._value); return*this; }
value<int const*>& value<int const*>::operator = (value<int const*> && val) { set(ang::move(val._value)); return*this; }
