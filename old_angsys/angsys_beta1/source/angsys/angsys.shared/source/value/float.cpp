/*********************************************************************************************************************/
/*   File Name: float.cpp                                                                                            */
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

value<float>::value() : _value() {}
value<float>::value(type val) : _value(val) {}
value<float>::value(value<float> const& other) : _value(other._value) {}
value<float>::value(value<float> && other) : _value(ang::move(other._value)) {}
float & value<float>::get() { return _value; }
float const& value<float>::get()const { return _value; }
void value<float>::set(type val) { _value = ang::move(val); }
value<float>& value<float>::operator = (type val) { set(ang::move(val)); return*this; }
value<float>& value<float>::operator = (value<float> const& val) { set(val._value); return*this; }
value<float>& value<float>::operator = (value<float> && val) { set(ang::move(val._value)); return*this; }
value<float>& value<float>::operator += (type val) { _value += val; return*this; }
value<float>& value<float>::operator += (value<float> const& val) { _value += val._value; return*this; }
value<float>& value<float>::operator -= (type val) { _value -= val; return*this; }
value<float>& value<float>::operator -= (value<float> const& val) { _value -= val._value; return*this; }
value<float>& value<float>::operator *= (type val) { _value *= val; return*this; }
value<float>& value<float>::operator *= (value<float> const& val) { _value *= val._value; return*this; }
value<float>& value<float>::operator /= (type val) { _value /= val; return*this; }
value<float>& value<float>::operator /= (value<float> const& val) { _value /= val._value; return*this; }


value<float const>::value(float val) : _value(val) {}
value<float const>::value(value<float const> const& other) : _value(other._value) {}
float const& value<float const>::get()const { return _value; }

value<float&>::value(float& val) : _value(val) {}
value<float&>::value(value<float&>& other) : _value(other._value) {}
value<float&>::value(value<float>& other) : _value(other.get()) {}
float & value<float&>::get() { return _value; }
float const& value<float&>::get()const { return _value; }
void value<float&>::set(type val) { _value = ang::move(val); }
value<float>& value<float&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<float>*>(&_value); }
value<float>& value<float&>::operator = (value<float&> const& val) { set(val._value); return*reinterpret_cast<value<float>*>(&_value); }
value<float>& value<float&>::operator = (value<float> val) { set(ang::move(val.get())); return*reinterpret_cast<value<float>*>(&_value); }
value<float>& value<float&>::operator += (type val) { _value += val; return*reinterpret_cast<value<float>*>(&_value); }
value<float>& value<float&>::operator += (value const& val) { _value += val._value; return*reinterpret_cast<value<float>*>(&_value); }
value<float>& value<float&>::operator -= (type val) { _value -= val; return*reinterpret_cast<value<float>*>(&_value); }
value<float>& value<float&>::operator -= (value const& val) { _value -= val._value; return*reinterpret_cast<value<float>*>(&_value); }
value<float>& value<float&>::operator *= (type val) { _value *= val; return*reinterpret_cast<value<float>*>(&_value); }
value<float>& value<float&>::operator *= (value const& val) { _value *= val._value; return*reinterpret_cast<value<float>*>(&_value); }
value<float>& value<float&>::operator /= (type val) { _value /= val; return*reinterpret_cast<value<float>*>(&_value); }
value<float>& value<float&>::operator /= (value const& val) { _value /= val._value; return*reinterpret_cast<value<float>*>(&_value); }
value<float>& value<float&>::operator ++() { ++_value; return*reinterpret_cast<value<float>*>(&_value); }
value<float> value<float&>::operator ++(int) { auto ret = _value++; return ret; }
value<float>& value<float&>::operator --() { --_value; return*reinterpret_cast<value<float>*>(&_value); }
value<float> value<float&>::operator --(int) { auto ret = _value--; return ret; }

value<float const&>::value(float const& val) : _value(val) {}
value<float const&>::value(value<float const&> const& other) : _value(other._value) {}
float const& value<float const&>::get()const { return _value; }

value<float*>::value() : _value(null) {}
value<float*>::value(float* val) : _value(val) {}
value<float*>::value(ang::nullptr_t) : _value(null) {}
value<float*>::value(value<float>* val) : _value(&val->get()) {}
value<float*>::value(value const& other) : _value(other._value) {}
value<float*>::value(value && other) : _value(ang::move(other._value)) {}
float* & value<float*>::get() { return _value; }
float* const& value<float*>::get()const { return _value; }
void value<float*>::set(float* val) { _value = ang::move(val); }
value<float*>& value<float*>::operator = (float* val) { set(ang::move(val)); return*this; }
value<float*>& value<float*>::operator = (value<float*> const& val) { set(val._value); return*this; }
value<float*>& value<float*>::operator = (value<float*> && val) { set(ang::move(val._value)); return*this; }


value<float const*>::value() : _value(null) {}
value<float const*>::value(type val) : _value(val) {}
value<float const*>::value(ang::nullptr_t) : _value(null) {}
value<float const*>::value(value<float> const* val) : _value(&val->get()) {}
value<float const*>::value(value const& other) : _value(other._value) {}
value<float const*>::value(value && other) : _value(ang::move(other._value)) {}
float const* & value<float const*>::get() { return _value; }
float const* const& value<float const*>::get()const { return _value; }
void value<float const*>::set(type val) { _value = ang::move(val); }
value<float const*>& value<float const*>::operator = (type val) { set(ang::move(val)); return*this; }
value<float const*>& value<float const*>::operator = (value<float const*> const& val) { set(val._value); return*this; }
value<float const*>& value<float const*>::operator = (value<float const*> && val) { set(ang::move(val._value)); return*this; }
