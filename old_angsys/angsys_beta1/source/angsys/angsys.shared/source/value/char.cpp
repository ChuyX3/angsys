/*********************************************************************************************************************/
/*   File Name: char.cpp                                                                                             */
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

value<char>::value() : _value() {}
value<char>::value(type val) : _value(val) {}
value<char>::value(value<char> const& other) : _value(other._value) {}
value<char>::value(value<char> && other) : _value(ang::move(other._value)) {}
char & value<char>::get() { return _value; }
char const& value<char>::get()const { return _value; }
void value<char>::set(type val) { _value = ang::move(val); }
value<char>& value<char>::operator = (type val) { set(ang::move(val)); return*this; }
value<char>& value<char>::operator = (value<char> const& val) { set(val._value); return*this; }
value<char>& value<char>::operator = (value<char> && val) { set(ang::move(val._value)); return*this; }

value<char const>::value(char val) : _value(val) {}
value<char const>::value(value<char const> const& other) : _value(other._value) {}
char const& value<char const>::get()const { return _value; }

value<char&>::value(char& val) : _value(val) {}
value<char&>::value(value<char&>& other) : _value(other._value) {}
value<char&>::value(value<char>& other) : _value(other.get()) {}
char & value<char&>::get() { return _value; }
char const& value<char&>::get()const { return _value; }
void value<char&>::set(type val) { _value = ang::move(val); }
value<char>& value<char&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<char>*>(&get()); }
value<char>& value<char&>::operator = (value<char&> const& val) { set(val._value); return*reinterpret_cast<value<char>*>(&get()); }
value<char>& value<char&>::operator = (value<char> val) { set(ang::move(val.get())); return*reinterpret_cast<value<char>*>(&get()); }

value<char const&>::value(char const& val) : _value(val) {}
value<char const&>::value(value<char const&> const& other) : _value(other._value) {}
char const& value<char const&>::get()const { return _value; }

value<char*>::value() : _value(null) {}
value<char*>::value(char* val, uint size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<char*>::value(value<char>* val, uint size) : _value(&val->get()), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length((char*)val);
}
value<char*>::value(value const& other) : _value(other._value), _size(other._size) {}
value<char*>::value(value && other) : _value(ang::move(other._value)), _size(other._size) {}
value<char*>::value(ang::nullptr_t) : _value(null), _size(0) {}
char* & value<char*>::get() { return _value; }
char* const& value<char*>::get()const { return _value; }
void value<char*>::set(char* val, uint size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<char*>& value<char*>::operator = (char* val) { set(ang::move(val), -1); return*this; }
value<char*>& value<char*>::operator = (value<char*> const& val) { set(val._value, val._size); return*this; }
value<char*>& value<char*>::operator = (value<char*> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

value<char const*>::value() : _value(null), _size(0) {}
value<char const*>::value(type val, uint size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<char const*>::value(ang::nullptr_t) : _value(null), _size(0) {}
value<char const*>::value(value<char> const* val, uint size) : _value(&val->get()), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length((char*)val);
}
value<char const*>::value(value const& other) : _value(other._value), _size(other._size) {}
value<char const*>::value(value && other) : _value(ang::move(other._value)), _size(other._size) {}
char const* & value<char const*>::get() { return _value; }
char const* const& value<char const*>::get()const { return _value; }
void value<char const*>::set(type val, uint size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<char const*>& value<char const*>::operator = (type val) { set(ang::move(val), -1); return*this; }
value<char const*>& value<char const*>::operator = (value<char const*> const& val) { set(val._value, val._size); return*this; }
value<char const*>& value<char const*>::operator = (value<char const*> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


value<wchar_t>::value() : _value() {}
value<wchar_t>::value(type val) : _value(val) {}
value<wchar_t>::value(value<wchar_t> const& other) : _value(other._value) {}
value<wchar_t>::value(value<wchar_t> && other) : _value(ang::move(other._value)) {}
wchar_t & value<wchar_t>::get() { return _value; }
wchar_t const& value<wchar_t>::get()const { return _value; }
void value<wchar_t>::set(type val) { _value = ang::move(val); }
value<wchar_t>& value<wchar_t>::operator = (type val) { set(ang::move(val)); return*this; }
value<wchar_t>& value<wchar_t>::operator = (value<wchar_t> const& val) { set(val._value); return*this; }
value<wchar_t>& value<wchar_t>::operator = (value<wchar_t> && val) { set(ang::move(val._value)); return*this; }

value<wchar_t const>::value(wchar_t val) : _value(val) {}
value<wchar_t const>::value(value<wchar_t const> const& other) : _value(other._value) {}
wchar_t const& value<wchar_t const>::get()const { return _value; }

value<wchar_t&>::value(wchar_t& val) : _value(val) {}
value<wchar_t&>::value(value<wchar_t&>& other) : _value(other._value) {}
value<wchar_t&>::value(value<wchar_t>& other) : _value(other.get()) {}
wchar_t & value<wchar_t&>::get() { return _value; }
wchar_t const& value<wchar_t&>::get()const { return _value; }
void value<wchar_t&>::set(type val) { _value = ang::move(val); }
value<wchar_t>& value<wchar_t&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<wchar_t>*>(&get()); }
value<wchar_t>& value<wchar_t&>::operator = (value<wchar_t&> const& val) { set(val._value); return*reinterpret_cast<value<wchar_t>*>(&get()); }
value<wchar_t>& value<wchar_t&>::operator = (value<wchar_t> val) { set(ang::move(val.get())); return*reinterpret_cast<value<wchar_t>*>(&get()); }

value<wchar_t const&>::value(wchar_t const& val) : _value(val) {}
value<wchar_t const&>::value(value<wchar_t const&> const& other) : _value(other._value) {}
wchar_t const& value<wchar_t const&>::get()const { return _value; }

value<wchar_t*>::value() : _value(null) {}
value<wchar_t*>::value(wchar_t* val, uint size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<wchar_t*>::value(value<wchar_t>* val, uint size) : _value(&val->get()), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length((wchar_t*)val);
}
value<wchar_t*>::value(value const& other) : _value(other._value), _size(other._size) {}
value<wchar_t*>::value(value && other) : _value(ang::move(other._value)), _size(other._size) {}
value<wchar_t*>::value(ang::nullptr_t) : _value(null), _size(0) {}
wchar_t* & value<wchar_t*>::get() { return _value; }
wchar_t* const& value<wchar_t*>::get()const { return _value; }
void value<wchar_t*>::set(wchar_t* val, uint size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<wchar_t*>& value<wchar_t*>::operator = (wchar_t* val) { set(ang::move(val), -1); return*this; }
value<wchar_t*>& value<wchar_t*>::operator = (value<wchar_t*> const& val) { set(val._value, val._size); return*this; }
value<wchar_t*>& value<wchar_t*>::operator = (value<wchar_t*> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

value<wchar_t const*>::value() : _value(null), _size(0) {}
value<wchar_t const*>::value(type val, uint size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<wchar_t const*>::value(ang::nullptr_t) : _value(null), _size(0) {}
value<wchar_t const*>::value(value<wchar_t> const* val, uint size) : _value(&val->get()), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length((wchar_t*)val);
}
value<wchar_t const*>::value(value const& other) : _value(other._value), _size(other._size) {}
value<wchar_t const*>::value(value && other) : _value(ang::move(other._value)), _size(other._size) {}
wchar_t const* & value<wchar_t const*>::get() { return _value; }
wchar_t const* const& value<wchar_t const*>::get()const { return _value; }
void value<wchar_t const*>::set(type val, uint size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<wchar_t const*>& value<wchar_t const*>::operator = (type val) { set(ang::move(val), -1); return*this; }
value<wchar_t const*>& value<wchar_t const*>::operator = (value<wchar_t const*> const& val) { set(val._value, val._size); return*this; }
value<wchar_t const*>& value<wchar_t const*>::operator = (value<wchar_t const*> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


value<unsigned char>::value() : _value() {}
value<unsigned char>::value(type val) : _value(val) {}
value<unsigned char>::value(value<unsigned char> const& other) : _value(other._value) {}
value<unsigned char>::value(value<unsigned char> && other) : _value(ang::move(other._value)) {}
unsigned char & value<unsigned char>::get() { return _value; }
unsigned char const& value<unsigned char>::get()const { return _value; }
void value<unsigned char>::set(type val) { _value = ang::move(val); }
value<unsigned char>& value<unsigned char>::operator = (type val) { set(ang::move(val)); return*this; }
value<unsigned char>& value<unsigned char>::operator = (value<unsigned char> const& val) { set(val._value); return*this; }
value<unsigned char>& value<unsigned char>::operator = (value<unsigned char> && val) { set(ang::move(val._value)); return*this; }

value<unsigned char const>::value(unsigned char val) : _value(val) {}
value<unsigned char const>::value(value<unsigned char const> const& other) : _value(other._value) {}
unsigned char const& value<unsigned char const>::get()const { return _value; }

value<unsigned char&>::value(unsigned char& val) : _value(val) {}
value<unsigned char&>::value(value<unsigned char&>& other) : _value(other._value) {}
value<unsigned char&>::value(value<unsigned char>& other) : _value(other.get()) {}
unsigned char & value<unsigned char&>::get() { return _value; }
unsigned char const& value<unsigned char&>::get()const { return _value; }
void value<unsigned char&>::set(type val) { _value = ang::move(val); }
value<unsigned char>& value<unsigned char&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<unsigned char>*>(&get()); }
value<unsigned char>& value<unsigned char&>::operator = (value<unsigned char&> const& val) { set(val._value); return*reinterpret_cast<value<unsigned char>*>(&get()); }
value<unsigned char>& value<unsigned char&>::operator = (value<unsigned char> val) { set(ang::move(val.get())); return*reinterpret_cast<value<unsigned char>*>(&get()); }

value<unsigned char const&>::value(unsigned char const& val) : _value(val) {}
value<unsigned char const&>::value(value<unsigned char const&> const& other) : _value(other._value) {}
unsigned char const& value<unsigned char const&>::get()const { return _value; }

value<unsigned char*>::value() : _value(null) {}
value<unsigned char*>::value(unsigned char* val, uint size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<unsigned char*>::value(value<unsigned char>* val, uint size) : _value(&val->get()), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length((unsigned char*)val);
}
value<unsigned char*>::value(value const& other) : _value(other._value), _size(other._size) {}
value<unsigned char*>::value(value && other) : _value(ang::move(other._value)), _size(other._size) {}
value<unsigned char*>::value(ang::nullptr_t) : _value(null), _size(0) {}
unsigned char* & value<unsigned char*>::get() { return _value; }
unsigned char* const& value<unsigned char*>::get()const { return _value; }
void value<unsigned char*>::set(unsigned char* val, uint size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<unsigned char*>& value<unsigned char*>::operator = (unsigned char* val) { set(ang::move(val), -1); return*this; }
value<unsigned char*>& value<unsigned char*>::operator = (value<unsigned char*> const& val) { set(val._value, val._size); return*this; }
value<unsigned char*>& value<unsigned char*>::operator = (value<unsigned char*> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

value<unsigned char const*>::value() : _value(null), _size(0) {}
value<unsigned char const*>::value(type val, uint size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<unsigned char const*>::value(ang::nullptr_t) : _value(null), _size(0) {}
value<unsigned char const*>::value(value<unsigned char> const* val, uint size) : _value(&val->get()), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length((unsigned char*)val);
}
value<unsigned char const*>::value(value const& other) : _value(other._value), _size(other._size) {}
value<unsigned char const*>::value(value && other) : _value(ang::move(other._value)), _size(other._size) {}
unsigned char const* & value<unsigned char const*>::get() { return _value; }
unsigned char const* const& value<unsigned char const*>::get()const { return _value; }
void value<unsigned char const*>::set(type val, uint size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<unsigned char const*>& value<unsigned char const*>::operator = (type val) { set(ang::move(val), -1); return*this; }
value<unsigned char const*>& value<unsigned char const*>::operator = (value<unsigned char const*> const& val) { set(val._value, val._size); return*this; }
value<unsigned char const*>& value<unsigned char const*>::operator = (value<unsigned char const*> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }


static_array<char>::static_array() : _value(null), _size(0) {}
static_array<char>::static_array(type val, uint size) : _value(val), _size(size == -1? strings::algorithms::string_length(val) : size) {}
static_array<char>::static_array(ang::nullptr_t, uint) : _value(null), _size(0) {}
static_array<char>::static_array(value<char*> val, uint size) : _value(val.get()), _size(size == -1 ? strings::algorithms::string_length(val.get()) : size) {}
static_array<char>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
static_array<char>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
static_array<char>::~static_array() {}
char* & static_array<char>::get() { return _value; }
char* const& static_array<char>::get()const { return _value; }
void static_array<char>::set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
char* static_array<char>::data()const { return _value; }
uint static_array<char>::size()const { return _size; }
static_array<char>& static_array<char>::operator = (type val) { set(ang::move(val), strings::algorithms::string_length(val)); return*this; }
static_array<char>& static_array<char>::operator = (value<char*> const& val) { set(val.get(), strings::algorithms::string_length(val.get())); return*this; }
static_array<char>& static_array<char>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
static_array<char>& static_array<char>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

static_const_array<char>::static_const_array() : _value(null), _size(0) {}
static_const_array<char>::static_const_array(type val, uint size) : _value(val), _size(size) {}
static_const_array<char>::static_const_array(ang::nullptr_t, uint size) : _value(null), _size(0) {}
static_const_array<char>::static_const_array(value<char const*> val, uint size) : _value(val.get()), _size(size) {}
static_const_array<char>::static_const_array(static_const_array<char> const* val, uint size) : _value(val->get()), _size(size) {}
static_const_array<char>::static_const_array(static_const_array const& other) : _value(other._value), _size(other._size) {}
static_const_array<char>::static_const_array(static_const_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
static_const_array<char>::~static_const_array() {}
char const* & static_const_array<char>::get() { return _value; }
char const* const& static_const_array<char>::get()const { return _value; }
void static_const_array<char>::set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
char const* static_const_array<char>::data()const { return _value; }
uint static_const_array<char>::size()const { return _size; }
static_const_array<char>& static_const_array<char>::operator = (type val) { set(ang::move(val), 1); return*this; }
static_const_array<char>& static_const_array<char>::operator = (value<char const*> const& val) { set(val.get(), 1); return*this; }
static_const_array<char>& static_const_array<char>::operator = (static_const_array const& val) { set(val._value, val._size); return*this; }
static_const_array<char>& static_const_array<char>::operator = (static_const_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

static_array<byte>::static_array() : _value(null), _size(0) {}
static_array<byte>::static_array(type val, uint size) : _value(val), _size(size == -1 ? strings::algorithms::string_length(val) : size) {}
static_array<byte>::static_array(ang::nullptr_t, uint) : _value(null), _size(0) {}
static_array<byte>::static_array(value<byte*> val, uint size) : _value(val.get()), _size(size == -1 ? strings::algorithms::string_length(val.get()) : size) {}
static_array<byte>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
static_array<byte>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
static_array<byte>::~static_array() {}
byte* & static_array<byte>::get() { return _value; }
byte* const& static_array<byte>::get()const { return _value; }
void static_array<byte>::set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
byte* static_array<byte>::data()const { return _value; }
uint static_array<byte>::size()const { return _size; }
static_array<byte>& static_array<byte>::operator = (type val) { set(ang::move(val), strings::algorithms::string_length(val)); return*this; }
static_array<byte>& static_array<byte>::operator = (value<byte*> const& val) { set(val.get(), strings::algorithms::string_length(val.get())); return*this; }
static_array<byte>& static_array<byte>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
static_array<byte>& static_array<byte>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

static_const_array<byte>::static_const_array() : _value(null), _size(0) {}
static_const_array<byte>::static_const_array(type val, uint size) : _value(val), _size(size) {}
static_const_array<byte>::static_const_array(ang::nullptr_t, uint size) : _value(null), _size(0) {}
static_const_array<byte>::static_const_array(value<byte const*> val, uint size) : _value(val.get()), _size(size) {}
static_const_array<byte>::static_const_array(static_const_array<byte> const* val, uint size) : _value(val->get()), _size(size) {}
static_const_array<byte>::static_const_array(static_const_array const& other) : _value(other._value), _size(other._size) {}
static_const_array<byte>::static_const_array(static_const_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
static_const_array<byte>::~static_const_array() {}
byte const* & static_const_array<byte>::get() { return _value; }
byte const* const& static_const_array<byte>::get()const { return _value; }
void static_const_array<byte>::set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
byte const* static_const_array<byte>::data()const { return _value; }
uint static_const_array<byte>::size()const { return _size; }
static_const_array<byte>& static_const_array<byte>::operator = (type val) { set(ang::move(val), 1); return*this; }
static_const_array<byte>& static_const_array<byte>::operator = (value<byte const*> const& val) { set(val.get(), 1); return*this; }
static_const_array<byte>& static_const_array<byte>::operator = (static_const_array const& val) { set(val._value, val._size); return*this; }
static_const_array<byte>& static_const_array<byte>::operator = (static_const_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }


static_array<wchar>::static_array() : _value(null), _size(0) {}
static_array<wchar>::static_array(type val, uint size) : _value(val), _size(size == -1 ? strings::algorithms::string_length(val) : size) {}
static_array<wchar>::static_array(ang::nullptr_t, uint) : _value(null), _size(0) {}
static_array<wchar>::static_array(value<wchar*> val, uint size) : _value(val.get()), _size(size == -1 ? strings::algorithms::string_length(val.get()) : size) {}
static_array<wchar>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
static_array<wchar>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
static_array<wchar>::~static_array() {}
wchar* & static_array<wchar>::get() { return _value; }
wchar* const& static_array<wchar>::get()const { return _value; }
void static_array<wchar>::set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
wchar* static_array<wchar>::data()const { return _value; }
uint static_array<wchar>::size()const { return _size; }
static_array<wchar>& static_array<wchar>::operator = (type val) { set(ang::move(val), strings::algorithms::string_length(val)); return*this; }
static_array<wchar>& static_array<wchar>::operator = (value<wchar*> const& val) { set(val.get(), strings::algorithms::string_length(val.get())); return*this; }
static_array<wchar>& static_array<wchar>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
static_array<wchar>& static_array<wchar>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

static_const_array<wchar>::static_const_array() : _value(null), _size(0) {}
static_const_array<wchar>::static_const_array(type val, uint size) : _value(val), _size(size) {}
static_const_array<wchar>::static_const_array(ang::nullptr_t, uint size) : _value(null), _size(0) {}
static_const_array<wchar>::static_const_array(value<wchar const*> val, uint size) : _value(val.get()), _size(size) {}
static_const_array<wchar>::static_const_array(static_const_array<wchar> const* val, uint size) : _value(val->get()), _size(size) {}
static_const_array<wchar>::static_const_array(static_const_array const& other) : _value(other._value), _size(other._size) {}
static_const_array<wchar>::static_const_array(static_const_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
static_const_array<wchar>::~static_const_array() {}
wchar const* & static_const_array<wchar>::get() { return _value; }
wchar const* const& static_const_array<wchar>::get()const { return _value; }
void static_const_array<wchar>::set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
wchar const* static_const_array<wchar>::data()const { return _value; }
uint static_const_array<wchar>::size()const { return _size; }
static_const_array<wchar>& static_const_array<wchar>::operator = (type val) { set(ang::move(val), 1); return*this; }
static_const_array<wchar>& static_const_array<wchar>::operator = (value<wchar const*> const& val) { set(val.get(), 1); return*this; }
static_const_array<wchar>& static_const_array<wchar>::operator = (static_const_array const& val) { set(val._value, val._size); return*this; }
static_const_array<wchar>& static_const_array<wchar>::operator = (static_const_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }


