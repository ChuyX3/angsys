/*********************************************************************************************************************/
/*   File Name: double.cpp                                                                                           */
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

value<double>::value() : _value() {}
value<double>::value(type val) : _value(val) {}
value<double>::value(value<double> const& other) : _value(other._value) {}
value<double>::value(value<double> && other) : _value(ang::move(other._value)) {}
double & value<double>::get() { return _value; }
double const& value<double>::get()const { return _value; }
void value<double>::set(type val) { _value = ang::move(val); }
value<double>& value<double>::operator = (type val) { set(ang::move(val)); return*this; }
value<double>& value<double>::operator = (value<double> const& val) { set(val._value); return*this; }
value<double>& value<double>::operator = (value<double> && val) { set(ang::move(val._value)); return*this; }
value<double>& value<double>::operator += (type val) { _value += val; return*this; }
value<double>& value<double>::operator += (value<double> const& val) { _value += val._value; return*this; }
value<double>& value<double>::operator -= (type val) { _value -= val; return*this; }
value<double>& value<double>::operator -= (value<double> const& val) { _value -= val._value; return*this; }
value<double>& value<double>::operator *= (type val) { _value *= val; return*this; }
value<double>& value<double>::operator *= (value<double> const& val) { _value *= val._value; return*this; }
value<double>& value<double>::operator /= (type val) { _value /= val; return*this; }
value<double>& value<double>::operator /= (value<double> const& val) { _value /= val._value; return*this; }


value<double const>::value(double val) : _value(val) {}
value<double const>::value(value<double const> const& other) : _value(other._value) {}
double const& value<double const>::get()const { return _value; }

value<double&>::value(double& val) : _value(val) {}
value<double&>::value(value<double&>& other) : _value(other._value) {}
value<double&>::value(value<double>& other) : _value(other.get()) {}
double & value<double&>::get() { return _value; }
double const& value<double&>::get()const { return _value; }
void value<double&>::set(type val) { _value = ang::move(val); }
value<double>& value<double&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<double>*>(&_value); }
value<double>& value<double&>::operator = (value<double&> const& val) { set(val._value); return*reinterpret_cast<value<double>*>(&_value); }
value<double>& value<double&>::operator = (value<double> val) { set(ang::move(val.get())); return*reinterpret_cast<value<double>*>(&_value); }
value<double>& value<double&>::operator += (type val) { _value += val; return*reinterpret_cast<value<double>*>(&_value); }
value<double>& value<double&>::operator += (value const& val) { _value += val._value; return*reinterpret_cast<value<double>*>(&_value); }
value<double>& value<double&>::operator -= (type val) { _value -= val; return*reinterpret_cast<value<double>*>(&_value); }
value<double>& value<double&>::operator -= (value const& val) { _value -= val._value; return*reinterpret_cast<value<double>*>(&_value); }
value<double>& value<double&>::operator *= (type val) { _value *= val; return*reinterpret_cast<value<double>*>(&_value); }
value<double>& value<double&>::operator *= (value const& val) { _value *= val._value; return*reinterpret_cast<value<double>*>(&_value); }
value<double>& value<double&>::operator /= (type val) { _value /= val; return*reinterpret_cast<value<double>*>(&_value); }
value<double>& value<double&>::operator /= (value const& val) { _value /= val._value; return*reinterpret_cast<value<double>*>(&_value); }
value<double>& value<double&>::operator ++() { ++_value; return*reinterpret_cast<value<double>*>(&_value); }
value<double> value<double&>::operator ++(int) { auto ret = _value++; return ret; }
value<double>& value<double&>::operator --() { --_value; return*reinterpret_cast<value<double>*>(&_value); }
value<double> value<double&>::operator --(int) { auto ret = _value--; return ret; }

value<double const&>::value(double const& val) : _value(val) {}
value<double const&>::value(value<double const&> const& other) : _value(other._value) {}
double const& value<double const&>::get()const { return _value; }

value<double*>::value() : _value(null) {}
value<double*>::value(double* val) : _value(val) {}
value<double*>::value(ang::nullptr_t) : _value(null) {}
value<double*>::value(value<double>* val) : _value(&val->get()) {}
value<double*>::value(value const& other) : _value(other._value) {}
value<double*>::value(value && other) : _value(ang::move(other._value)) {}
double* & value<double*>::get() { return _value; }
double* const& value<double*>::get()const { return _value; }
void value<double*>::set(double* val) { _value = ang::move(val); }
value<double*>& value<double*>::operator = (double* val) { set(ang::move(val)); return*this; }
value<double*>& value<double*>::operator = (value<double*> const& val) { set(val._value); return*this; }
value<double*>& value<double*>::operator = (value<double*> && val) { set(ang::move(val._value)); return*this; }


value<double const*>::value() : _value(null) {}
value<double const*>::value(type val) : _value(val) {}
value<double const*>::value(ang::nullptr_t) : _value(null) {}
value<double const*>::value(value<double> const* val) : _value(&val->get()) {}
value<double const*>::value(value const& other) : _value(other._value) {}
value<double const*>::value(value && other) : _value(ang::move(other._value)) {}
double const* & value<double const*>::get() { return _value; }
double const* const& value<double const*>::get()const { return _value; }
void value<double const*>::set(type val) { _value = ang::move(val); }
value<double const*>& value<double const*>::operator = (type val) { set(ang::move(val)); return*this; }
value<double const*>& value<double const*>::operator = (value<double const*> const& val) { set(val._value); return*this; }
value<double const*>& value<double const*>::operator = (value<double const*> && val) { set(ang::move(val._value)); return*this; }
