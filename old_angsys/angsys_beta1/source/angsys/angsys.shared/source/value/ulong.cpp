/*********************************************************************************************************************/
/*   File Name: ulong.cpp                                                                                           */
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

value<ulong>::value() : _value() {}
value<ulong>::value(type val) : _value(val) {}
value<ulong>::value(value<ulong> const& other) : _value(other._value) {}
value<ulong>::value(value<ulong> && other) : _value(ang::move(other._value)) {}
ulong & value<ulong>::get() { return _value; }
ulong const& value<ulong>::get()const { return _value; }
void value<ulong>::set(type val) { _value = ang::move(val); }
value<ulong>& value<ulong>::operator = (type val) { set(ang::move(val)); return*this; }
value<ulong>& value<ulong>::operator = (value<ulong> const& val) { set(val._value); return*this; }
value<ulong>& value<ulong>::operator = (value<ulong> && val) { set(ang::move(val._value)); return*this; }
value<ulong>& value<ulong>::operator += (type val) { _value += val; return*this; }
value<ulong>& value<ulong>::operator += (value<ulong> const& val) { _value += val._value; return*this; }
value<ulong>& value<ulong>::operator -= (type val) { _value -= val; return*this; }
value<ulong>& value<ulong>::operator -= (value<ulong> const& val) { _value -= val._value; return*this; }
value<ulong>& value<ulong>::operator *= (type val) { _value *= val; return*this; }
value<ulong>& value<ulong>::operator *= (value<ulong> const& val) { _value *= val._value; return*this; }
value<ulong>& value<ulong>::operator /= (type val) { _value /= val; return*this; }
value<ulong>& value<ulong>::operator /= (value<ulong> const& val) { _value /= val._value; return*this; }


value<ulong const>::value(ulong val) : _value(val) {}
value<ulong const>::value(value<ulong const> const& other) : _value(other._value) {}
ulong const& value<ulong const>::get()const { return _value; }

value<ulong&>::value(ulong& val) : _value(val) {}
value<ulong&>::value(value<ulong&>& other) : _value(other._value) {}
value<ulong&>::value(value<ulong>& other) : _value(other.get()) {}
ulong & value<ulong&>::get() { return _value; }
ulong const& value<ulong&>::get()const { return _value; }
void value<ulong&>::set(type val) { _value = ang::move(val); }
value<ulong>& value<ulong&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong>& value<ulong&>::operator = (value<ulong&> const& val) { set(val._value); return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong>& value<ulong&>::operator = (value<ulong> val) { set(ang::move(val.get())); return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong>& value<ulong&>::operator += (type val) { _value += val; return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong>& value<ulong&>::operator += (value const& val) { _value += val._value; return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong>& value<ulong&>::operator -= (type val) { _value -= val; return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong>& value<ulong&>::operator -= (value const& val) { _value -= val._value; return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong>& value<ulong&>::operator *= (type val) { _value *= val; return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong>& value<ulong&>::operator *= (value const& val) { _value *= val._value; return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong>& value<ulong&>::operator /= (type val) { _value /= val; return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong>& value<ulong&>::operator /= (value const& val) { _value /= val._value; return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong>& value<ulong&>::operator ++() { ++_value; return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong> value<ulong&>::operator ++(int) { auto ret = _value++; return ret; }
value<ulong>& value<ulong&>::operator --() { --_value; return*reinterpret_cast<value<ulong>*>(&_value); }
value<ulong> value<ulong&>::operator --(int) { auto ret = _value--; return ret; }

value<ulong const&>::value(ulong const& val) : _value(val) {}
value<ulong const&>::value(value<ulong const&> const& other) : _value(other._value) {}
ulong const& value<ulong const&>::get()const { return _value; }

value<ulong*>::value() : _value(null) {}
value<ulong*>::value(ulong* val) : _value(val) {}
value<ulong*>::value(ang::nullptr_t) : _value(null) {}
value<ulong*>::value(value<ulong>* val) : _value(&val->get()) {}
value<ulong*>::value(value const& other) : _value(other._value) {}
value<ulong*>::value(value && other) : _value(ang::move(other._value)) {}
ulong* & value<ulong*>::get() { return _value; }
ulong* const& value<ulong*>::get()const { return _value; }
void value<ulong*>::set(ulong* val) { _value = ang::move(val); }
value<ulong*>& value<ulong*>::operator = (ulong* val) { set(ang::move(val)); return*this; }
value<ulong*>& value<ulong*>::operator = (value<ulong*> const& val) { set(val._value); return*this; }
value<ulong*>& value<ulong*>::operator = (value<ulong*> && val) { set(ang::move(val._value)); return*this; }


value<ulong const*>::value() : _value(null) {}
value<ulong const*>::value(type val) : _value(val) {}
value<ulong const*>::value(ang::nullptr_t) : _value(null) {}
value<ulong const*>::value(value<ulong> const* val) : _value(&val->get()) {}
value<ulong const*>::value(value const& other) : _value(other._value) {}
value<ulong const*>::value(value && other) : _value(ang::move(other._value)) {}
ulong const* & value<ulong const*>::get() { return _value; }
ulong const* const& value<ulong const*>::get()const { return _value; }
void value<ulong const*>::set(type val) { _value = ang::move(val); }
value<ulong const*>& value<ulong const*>::operator = (type val) { set(ang::move(val)); return*this; }
value<ulong const*>& value<ulong const*>::operator = (value<ulong const*> const& val) { set(val._value); return*this; }
value<ulong const*>& value<ulong const*>::operator = (value<ulong const*> && val) { set(ang::move(val._value)); return*this; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

value<ulong64>::value() : _value() {}
value<ulong64>::value(type val) : _value(val) {}
value<ulong64>::value(value<ulong64> const& other) : _value(other._value) {}
value<ulong64>::value(value<ulong64> && other) : _value(ang::move(other._value)) {}
ulong64 & value<ulong64>::get() { return _value; }
ulong64 const& value<ulong64>::get()const { return _value; }
void value<ulong64>::set(type val) { _value = ang::move(val); }
value<ulong64>& value<ulong64>::operator = (type val) { set(ang::move(val)); return*this; }
value<ulong64>& value<ulong64>::operator = (value<ulong64> const& val) { set(val._value); return*this; }
value<ulong64>& value<ulong64>::operator = (value<ulong64> && val) { set(ang::move(val._value)); return*this; }
value<ulong64>& value<ulong64>::operator += (type val) { _value += val; return*this; }
value<ulong64>& value<ulong64>::operator += (value<ulong64> const& val) { _value += val._value; return*this; }
value<ulong64>& value<ulong64>::operator -= (type val) { _value -= val; return*this; }
value<ulong64>& value<ulong64>::operator -= (value<ulong64> const& val) { _value -= val._value; return*this; }
value<ulong64>& value<ulong64>::operator *= (type val) { _value *= val; return*this; }
value<ulong64>& value<ulong64>::operator *= (value<ulong64> const& val) { _value *= val._value; return*this; }
value<ulong64>& value<ulong64>::operator /= (type val) { _value /= val; return*this; }
value<ulong64>& value<ulong64>::operator /= (value<ulong64> const& val) { _value /= val._value; return*this; }


value<ulong64 const>::value(ulong64 val) : _value(val) {}
value<ulong64 const>::value(value<ulong64 const> const& other) : _value(other._value) {}
ulong64 const& value<ulong64 const>::get()const { return _value; }

value<ulong64&>::value(ulong64& val) : _value(val) {}
value<ulong64&>::value(value<ulong64&>& other) : _value(other._value) {}
value<ulong64&>::value(value<ulong64>& other) : _value(other.get()) {}
ulong64 & value<ulong64&>::get() { return _value; }
ulong64 const& value<ulong64&>::get()const { return _value; }
void value<ulong64&>::set(type val) { _value = ang::move(val); }
value<ulong64>& value<ulong64&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64>& value<ulong64&>::operator = (value<ulong64&> const& val) { set(val._value); return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64>& value<ulong64&>::operator = (value<ulong64> val) { set(ang::move(val.get())); return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64>& value<ulong64&>::operator += (type val) { _value += val; return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64>& value<ulong64&>::operator += (value const& val) { _value += val._value; return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64>& value<ulong64&>::operator -= (type val) { _value -= val; return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64>& value<ulong64&>::operator -= (value const& val) { _value -= val._value; return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64>& value<ulong64&>::operator *= (type val) { _value *= val; return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64>& value<ulong64&>::operator *= (value const& val) { _value *= val._value; return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64>& value<ulong64&>::operator /= (type val) { _value /= val; return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64>& value<ulong64&>::operator /= (value const& val) { _value /= val._value; return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64>& value<ulong64&>::operator ++() { ++_value; return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64> value<ulong64&>::operator ++(int) { auto ret = _value++; return ret; }
value<ulong64>& value<ulong64&>::operator --() { --_value; return*reinterpret_cast<value<ulong64>*>(&_value); }
value<ulong64> value<ulong64&>::operator --(int) { auto ret = _value--; return ret; }

value<ulong64 const&>::value(ulong64 const& val) : _value(val) {}
value<ulong64 const&>::value(value<ulong64 const&> const& other) : _value(other._value) {}
ulong64 const& value<ulong64 const&>::get()const { return _value; }

value<ulong64*>::value() : _value(null) {}
value<ulong64*>::value(ulong64* val) : _value(val) {}
value<ulong64*>::value(ang::nullptr_t) : _value(null) {}
value<ulong64*>::value(value<ulong64>* val) : _value(&val->get()) {}
value<ulong64*>::value(value const& other) : _value(other._value) {}
value<ulong64*>::value(value && other) : _value(ang::move(other._value)) {}
ulong64* & value<ulong64*>::get() { return _value; }
ulong64* const& value<ulong64*>::get()const { return _value; }
void value<ulong64*>::set(ulong64* val) { _value = ang::move(val); }
value<ulong64*>& value<ulong64*>::operator = (ulong64* val) { set(ang::move(val)); return*this; }
value<ulong64*>& value<ulong64*>::operator = (value<ulong64*> const& val) { set(val._value); return*this; }
value<ulong64*>& value<ulong64*>::operator = (value<ulong64*> && val) { set(ang::move(val._value)); return*this; }


value<ulong64 const*>::value() : _value(null) {}
value<ulong64 const*>::value(type val) : _value(val) {}
value<ulong64 const*>::value(ang::nullptr_t) : _value(null) {}
value<ulong64 const*>::value(value<ulong64> const* val) : _value(&val->get()) {}
value<ulong64 const*>::value(value const& other) : _value(other._value) {}
value<ulong64 const*>::value(value && other) : _value(ang::move(other._value)) {}
ulong64 const* & value<ulong64 const*>::get() { return _value; }
ulong64 const* const& value<ulong64 const*>::get()const { return _value; }
void value<ulong64 const*>::set(type val) { _value = ang::move(val); }
value<ulong64 const*>& value<ulong64 const*>::operator = (type val) { set(ang::move(val)); return*this; }
value<ulong64 const*>& value<ulong64 const*>::operator = (value<ulong64 const*> const& val) { set(val._value); return*this; }
value<ulong64 const*>& value<ulong64 const*>::operator = (value<ulong64 const*> && val) { set(ang::move(val._value)); return*this; }
