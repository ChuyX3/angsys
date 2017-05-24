/*********************************************************************************************************************/
/*   File Name: long.cpp                                                                                           */
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


value<long>::value() : _value() {}
value<long>::value(type val) : _value(val) {}
value<long>::value(value<long> const& other) : _value(other._value) {}
value<long>::value(value<long> && other) : _value(ang::move(other._value)) {}
long & value<long>::get() { return _value; }
long const& value<long>::get()const { return _value; }
void value<long>::set(type val) { _value = ang::move(val); }
value<long>& value<long>::operator = (type val) { set(ang::move(val)); return*this; }
value<long>& value<long>::operator = (value<long> const& val) { set(val._value); return*this; }
value<long>& value<long>::operator = (value<long> && val) { set(ang::move(val._value)); return*this; }
value<long>& value<long>::operator += (type val) { _value += val; return*this; }
value<long>& value<long>::operator += (value<long> const& val) { _value += val._value; return*this; }
value<long>& value<long>::operator -= (type val) { _value -= val; return*this; }
value<long>& value<long>::operator -= (value<long> const& val) { _value -= val._value; return*this; }
value<long>& value<long>::operator *= (type val) { _value *= val; return*this; }
value<long>& value<long>::operator *= (value<long> const& val) { _value *= val._value; return*this; }
value<long>& value<long>::operator /= (type val) { _value /= val; return*this; }
value<long>& value<long>::operator /= (value<long> const& val) { _value /= val._value; return*this; }


value<long const>::value(long val) : _value(val) {}
value<long const>::value(value<long const> const& other) : _value(other._value) {}
long const& value<long const>::get()const { return _value; }

value<long&>::value(long& val) : _value(val) {}
value<long&>::value(value<long&>& other) : _value(other._value) {}
value<long&>::value(value<long>& other) : _value(other.get()) {}
long & value<long&>::get() { return _value; }
long const& value<long&>::get()const { return _value; }
void value<long&>::set(type val) { _value = ang::move(val); }
value<long>& value<long&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<long>*>(&_value); }
value<long>& value<long&>::operator = (value<long&> const& val) { set(val._value); return*reinterpret_cast<value<long>*>(&_value); }
value<long>& value<long&>::operator = (value<long> val) { set(ang::move(val.get())); return*reinterpret_cast<value<long>*>(&_value); }
value<long>& value<long&>::operator += (type val) { _value += val; return*reinterpret_cast<value<long>*>(&_value); }
value<long>& value<long&>::operator += (value const& val) { _value += val._value; return*reinterpret_cast<value<long>*>(&_value); }
value<long>& value<long&>::operator -= (type val) { _value -= val; return*reinterpret_cast<value<long>*>(&_value); }
value<long>& value<long&>::operator -= (value const& val) { _value -= val._value; return*reinterpret_cast<value<long>*>(&_value); }
value<long>& value<long&>::operator *= (type val) { _value *= val; return*reinterpret_cast<value<long>*>(&_value); }
value<long>& value<long&>::operator *= (value const& val) { _value *= val._value; return*reinterpret_cast<value<long>*>(&_value); }
value<long>& value<long&>::operator /= (type val) { _value /= val; return*reinterpret_cast<value<long>*>(&_value); }
value<long>& value<long&>::operator /= (value const& val) { _value /= val._value; return*reinterpret_cast<value<long>*>(&_value); }
value<long>& value<long&>::operator ++() { ++_value; return*reinterpret_cast<value<long>*>(&_value); }
value<long> value<long&>::operator ++(int) { auto ret = _value++; return ret; }
value<long>& value<long&>::operator --() { --_value; return*reinterpret_cast<value<long>*>(&_value); }
value<long> value<long&>::operator --(int) { auto ret = _value--; return ret; }

value<long const&>::value(long const& val) : _value(val) {}
value<long const&>::value(value<long const&> const& other) : _value(other._value) {}
long const& value<long const&>::get()const { return _value; }

value<long*>::value() : _value(null) {}
value<long*>::value(long* val) : _value(val) {}
value<long*>::value(ang::nullptr_t) : _value(null) {}
value<long*>::value(value<long>* val) : _value(&val->get()) {}
value<long*>::value(value const& other) : _value(other._value) {}
value<long*>::value(value && other) : _value(ang::move(other._value)) {}
long* & value<long*>::get() { return _value; }
long* const& value<long*>::get()const { return _value; }
void value<long*>::set(long* val) { _value = ang::move(val); }
value<long*>& value<long*>::operator = (long* val) { set(ang::move(val)); return*this; }
value<long*>& value<long*>::operator = (value<long*> const& val) { set(val._value); return*this; }
value<long*>& value<long*>::operator = (value<long*> && val) { set(ang::move(val._value)); return*this; }


value<long const*>::value() : _value(null) {}
value<long const*>::value(type val) : _value(val) {}
value<long const*>::value(ang::nullptr_t) : _value(null) {}
value<long const*>::value(value<long> const* val) : _value(&val->get()) {}
value<long const*>::value(value const& other) : _value(other._value) {}
value<long const*>::value(value && other) : _value(ang::move(other._value)) {}
long const* & value<long const*>::get() { return _value; }
long const* const& value<long const*>::get()const { return _value; }
void value<long const*>::set(type val) { _value = ang::move(val); }
value<long const*>& value<long const*>::operator = (type val) { set(ang::move(val)); return*this; }
value<long const*>& value<long const*>::operator = (value<long const*> const& val) { set(val._value); return*this; }
value<long const*>& value<long const*>::operator = (value<long const*> && val) { set(ang::move(val._value)); return*this; }



value<long64>::value() : _value() {}
value<long64>::value(type val) : _value(val) {}
value<long64>::value(value<long64> const& other) : _value(other._value) {}
value<long64>::value(value<long64> && other) : _value(ang::move(other._value)) {}
long64 & value<long64>::get() { return _value; }
long64 const& value<long64>::get()const { return _value; }
void value<long64>::set(type val) { _value = ang::move(val); }
value<long64>& value<long64>::operator = (type val) { set(ang::move(val)); return*this; }
value<long64>& value<long64>::operator = (value<long64> const& val) { set(val._value); return*this; }
value<long64>& value<long64>::operator = (value<long64> && val) { set(ang::move(val._value)); return*this; }
value<long64>& value<long64>::operator += (type val) { _value += val; return*this; }
value<long64>& value<long64>::operator += (value<long64> const& val) { _value += val._value; return*this; }
value<long64>& value<long64>::operator -= (type val) { _value -= val; return*this; }
value<long64>& value<long64>::operator -= (value<long64> const& val) { _value -= val._value; return*this; }
value<long64>& value<long64>::operator *= (type val) { _value *= val; return*this; }
value<long64>& value<long64>::operator *= (value<long64> const& val) { _value *= val._value; return*this; }
value<long64>& value<long64>::operator /= (type val) { _value /= val; return*this; }
value<long64>& value<long64>::operator /= (value<long64> const& val) { _value /= val._value; return*this; }


value<long64 const>::value(long64 val) : _value(val) {}
value<long64 const>::value(value<long64 const> const& other) : _value(other._value) {}
long64 const& value<long64 const>::get()const { return _value; }

value<long64&>::value(long64& val) : _value(val) {}
value<long64&>::value(value<long64&>& other) : _value(other._value) {}
value<long64&>::value(value<long64>& other) : _value(other.get()) {}
long64 & value<long64&>::get() { return _value; }
long64 const& value<long64&>::get()const { return _value; }
void value<long64&>::set(type val) { _value = ang::move(val); }
value<long64>& value<long64&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<long64>*>(&_value); }
value<long64>& value<long64&>::operator = (value<long64&> const& val) { set(val._value); return*reinterpret_cast<value<long64>*>(&_value); }
value<long64>& value<long64&>::operator = (value<long64> val) { set(ang::move(val.get())); return*reinterpret_cast<value<long64>*>(&_value); }
value<long64>& value<long64&>::operator += (type val) { _value += val; return*reinterpret_cast<value<long64>*>(&_value); }
value<long64>& value<long64&>::operator += (value const& val) { _value += val._value; return*reinterpret_cast<value<long64>*>(&_value); }
value<long64>& value<long64&>::operator -= (type val) { _value -= val; return*reinterpret_cast<value<long64>*>(&_value); }
value<long64>& value<long64&>::operator -= (value const& val) { _value -= val._value; return*reinterpret_cast<value<long64>*>(&_value); }
value<long64>& value<long64&>::operator *= (type val) { _value *= val; return*reinterpret_cast<value<long64>*>(&_value); }
value<long64>& value<long64&>::operator *= (value const& val) { _value *= val._value; return*reinterpret_cast<value<long64>*>(&_value); }
value<long64>& value<long64&>::operator /= (type val) { _value /= val; return*reinterpret_cast<value<long64>*>(&_value); }
value<long64>& value<long64&>::operator /= (value const& val) { _value /= val._value; return*reinterpret_cast<value<long64>*>(&_value); }
value<long64>& value<long64&>::operator ++() { ++_value; return*reinterpret_cast<value<long64>*>(&_value); }
value<long64> value<long64&>::operator ++(int) { auto ret = _value++; return ret; }
value<long64>& value<long64&>::operator --() { --_value; return*reinterpret_cast<value<long64>*>(&_value); }
value<long64> value<long64&>::operator --(int) { auto ret = _value--; return ret; }

value<long64 const&>::value(long64 const& val) : _value(val) {}
value<long64 const&>::value(value<long64 const&> const& other) : _value(other._value) {}
long64 const& value<long64 const&>::get()const { return _value; }

value<long64*>::value() : _value(null) {}
value<long64*>::value(long64* val) : _value(val) {}
value<long64*>::value(ang::nullptr_t) : _value(null) {}
value<long64*>::value(value<long64>* val) : _value(&val->get()) {}
value<long64*>::value(value const& other) : _value(other._value) {}
value<long64*>::value(value && other) : _value(ang::move(other._value)) {}
long64* & value<long64*>::get() { return _value; }
long64* const& value<long64*>::get()const { return _value; }
void value<long64*>::set(long64* val) { _value = ang::move(val); }
value<long64*>& value<long64*>::operator = (long64* val) { set(ang::move(val)); return*this; }
value<long64*>& value<long64*>::operator = (value<long64*> const& val) { set(val._value); return*this; }
value<long64*>& value<long64*>::operator = (value<long64*> && val) { set(ang::move(val._value)); return*this; }


value<long64 const*>::value() : _value(null) {}
value<long64 const*>::value(type val) : _value(val) {}
value<long64 const*>::value(ang::nullptr_t) : _value(null) {}
value<long64 const*>::value(value<long64> const* val) : _value(&val->get()) {}
value<long64 const*>::value(value const& other) : _value(other._value) {}
value<long64 const*>::value(value && other) : _value(ang::move(other._value)) {}
long64 const* & value<long64 const*>::get() { return _value; }
long64 const* const& value<long64 const*>::get()const { return _value; }
void value<long64 const*>::set(type val) { _value = ang::move(val); }
value<long64 const*>& value<long64 const*>::operator = (type val) { set(ang::move(val)); return*this; }
value<long64 const*>& value<long64 const*>::operator = (value<long64 const*> const& val) { set(val._value); return*this; }
value<long64 const*>& value<long64 const*>::operator = (value<long64 const*> && val) { set(ang::move(val._value)); return*this; }
