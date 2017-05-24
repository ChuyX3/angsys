/*********************************************************************************************************************/
/*   File Name: void_ptr.cpp                                                                                           */
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

value<pointer>::value() : _value() {}
value<pointer>::value(type val, uint size) : _value(val), _size(size) {}
value<pointer>::value(value<pointer> const& other) : _value(other._value), _size(other._size) {}
value<pointer>::value(value<pointer> && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
pointer & value<pointer>::get() { return _value; }
pointer const& value<pointer>::get()const { return _value; }
uint value<pointer>::size()const { return _size; }
void value<pointer>::set(type val, uint size) { _value = ang::move(val);  _size = ang::move(size); }
value<pointer>& value<pointer>::operator = (type val) { set(ang::move(val), -1); return*this; }
value<pointer>& value<pointer>::operator = (value<pointer> const& val) { set(val._value, val._size); return*this; }
value<pointer>& value<pointer>::operator = (value<pointer> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
//value<pointer>& value<pointer>::operator += (type val) { (ang_size_t&)_value += (ang_size_t)val; return*this; }
//value<pointer>& value<pointer>::operator += (value<pointer> const& val) { (ang_size_t&)_value += (ang_size_t)val._value; return*this; }
//value<pointer>& value<pointer>::operator -= (type val) { (ang_size_t&)_value -= (ang_size_t)val; return*this; }
//value<pointer>& value<pointer>::operator -= (value<pointer> const& val) { (ang_size_t&)_value -= (ang_size_t)val._value; return*this; }

value<pointer const>::value(pointer val, uint size) : _value(val), _size(size) {}
value<pointer const>::value(value<pointer const> const& other) : _value(other._value), _size(other._size) {}
pointer const& value<pointer const>::get()const { return _value; }
uint value<pointer const>::size()const { return _size; }

value<pointer&>::value(pointer& val) : _value(val) {}
value<pointer&>::value(value<pointer&>& other) : _value(other._value) {}
value<pointer&>::value(value<pointer>& other) : _value(other.get()) {}
pointer & value<pointer&>::get() { return _value; }
pointer const& value<pointer&>::get()const { return _value; }
void value<pointer&>::set(type val) { _value = ang::move(val); }
value<pointer>& value<pointer&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<pointer>*>(&_value); }
value<pointer>& value<pointer&>::operator = (value<pointer&> const& val) { set(val._value); return*reinterpret_cast<value<pointer>*>(&_value); }
value<pointer>& value<pointer&>::operator = (value<pointer> val) { set(ang::move(val.get())); return*reinterpret_cast<value<pointer>*>(&_value); }
value<pointer>& value<pointer&>::operator += (type val) { (ang_size_t&)_value += (ang_size_t)val; return*reinterpret_cast<value<pointer>*>(&_value); }
value<pointer>& value<pointer&>::operator += (value const& val) { (ang_size_t&)_value += (ang_size_t)val._value; return*reinterpret_cast<value<pointer>*>(&_value); }
value<pointer>& value<pointer&>::operator -= (type val) { (ang_size_t&)_value -= (ang_size_t)val; return*reinterpret_cast<value<pointer>*>(&_value); }
value<pointer>& value<pointer&>::operator -= (value const& val) { (ang_size_t&)_value -= (ang_size_t)val._value; return*reinterpret_cast<value<pointer>*>(&_value); }

value<pointer const&>::value(pointer const& val) : _value(val) {}
value<pointer const&>::value(value<pointer const&> const& other) : _value(other._value) {}
pointer const& value<pointer const&>::get()const { return _value; }

value<pointer*>::value() : _value(null) {}
value<pointer*>::value(pointer* val) : _value(val) {}
value<pointer*>::value(ang::nullptr_t) : _value(null) {}
value<pointer*>::value(value<pointer>* val) : _value(&val->get()) {}
value<pointer*>::value(value const& other) : _value(other._value) {}
value<pointer*>::value(value && other) : _value(ang::move(other._value)) {}
pointer* & value<pointer*>::get() { return _value; }
pointer* const& value<pointer*>::get()const { return _value; }
void value<pointer*>::set(pointer* val) { _value = ang::move(val); }
value<pointer*>& value<pointer*>::operator = (pointer* val) { set(ang::move(val)); return*this; }
value<pointer*>& value<pointer*>::operator = (value<pointer*> const& val) { set(val._value); return*this; }
value<pointer*>& value<pointer*>::operator = (value<pointer*> && val) { set(ang::move(val._value)); return*this; }


value<pointer const*>::value() : _value(null) {}
value<pointer const*>::value(type val) : _value(val) {}
value<pointer const*>::value(ang::nullptr_t) : _value(null) {}
value<pointer const*>::value(value<pointer> const* val) : _value(&val->get()) {}
value<pointer const*>::value(value const& other) : _value(other._value) {}
value<pointer const*>::value(value && other) : _value(ang::move(other._value)) {}
pointer const* & value<pointer const*>::get() { return _value; }
pointer const* const& value<pointer const*>::get()const { return _value; }
void value<pointer const*>::set(type val) { _value = ang::move(val); }
value<pointer const*>& value<pointer const*>::operator = (type val) { set(ang::move(val)); return*this; }
value<pointer const*>& value<pointer const*>::operator = (value<pointer const*> const& val) { set(val._value); return*this; }
value<pointer const*>& value<pointer const*>::operator = (value<pointer const*> && val) { set(ang::move(val._value)); return*this; }
