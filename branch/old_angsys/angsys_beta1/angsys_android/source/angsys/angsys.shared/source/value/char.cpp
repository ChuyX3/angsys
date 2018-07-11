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
#include "angbase.h"

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

value<char*>& value<char*>::operator ++ (void)
{
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return*this;
}

value<char*> value<char*>::operator ++ (int)
{
	value<char*> save = *this;
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return save;
}

value<char*>& value<char*>::operator -- (void)
{
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return*this;
}

value<char*> value<char*>::operator -- (int)
{
	value<char*> save = *this;
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return save;
}


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
value<char const*>& value<char const*>::operator ++ (void) {
	if (_size > 0) {
		_value++;
		_size--;
	}
	return*this;
}
value<char const*> value<char const*>::operator ++ (int) {
	value<char const*> save = *this;
	if (_size > 0) {
		_value++;
		_size--;
	}
	return save;
}
value<char const*>& value<char const*>::operator -- (void) {
	if (_value != null) {
		_value--;
		_size++;
	}
	return*this;
}
value<char const*> value<char const*>::operator -- (int) {
	value<char const*> save = *this;
	if (_value != null) {
		_value--;
		_size++;
	}
	return save;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


value<wchar>::value() : _value() {}
value<wchar>::value(type val) : _value(val) {}
value<wchar>::value(value<wchar> const& other) : _value(other._value) {}
value<wchar>::value(value<wchar> && other) : _value(ang::move(other._value)) {}
wchar & value<wchar>::get() { return _value; }
wchar const& value<wchar>::get()const { return _value; }
void value<wchar>::set(type val) { _value = ang::move(val); }
value<wchar>& value<wchar>::operator = (type val) { set(ang::move(val)); return*this; }
value<wchar>& value<wchar>::operator = (value<wchar> const& val) { set(val._value); return*this; }
value<wchar>& value<wchar>::operator = (value<wchar> && val) { set(ang::move(val._value)); return*this; }

value<wchar const>::value(wchar val) : _value(val) {}
value<wchar const>::value(value<wchar const> const& other) : _value(other._value) {}
wchar const& value<wchar const>::get()const { return _value; }

value<wchar&>::value(wchar& val) : _value(val) {}
value<wchar&>::value(value<wchar&>& other) : _value(other._value) {}
value<wchar&>::value(value<wchar>& other) : _value(other.get()) {}
wchar & value<wchar&>::get() { return _value; }
wchar const& value<wchar&>::get()const { return _value; }
void value<wchar&>::set(type val) { _value = ang::move(val); }
value<wchar>& value<wchar&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<wchar>*>(&get()); }
value<wchar>& value<wchar&>::operator = (value<wchar&> const& val) { set(val._value); return*reinterpret_cast<value<wchar>*>(&get()); }
value<wchar>& value<wchar&>::operator = (value<wchar> val) { set(ang::move(val.get())); return*reinterpret_cast<value<wchar>*>(&get()); }

value<wchar const&>::value(wchar const& val) : _value(val) {}
value<wchar const&>::value(value<wchar const&> const& other) : _value(other._value) {}
wchar const& value<wchar const&>::get()const { return _value; }

value<wchar*>::value() : _value(null) {}
value<wchar*>::value(wchar* val, uint size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<wchar*>::value(value<wchar>* val, uint size) : _value(&val->get()), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length((wchar*)val);
}
value<wchar*>::value(value const& other) : _value(other._value), _size(other._size) {}
value<wchar*>::value(value && other) : _value(ang::move(other._value)), _size(other._size) {}
value<wchar*>::value(ang::nullptr_t) : _value(null), _size(0) {}
wchar* & value<wchar*>::get() { return _value; }
wchar* const& value<wchar*>::get()const { return _value; }
void value<wchar*>::set(wchar* val, uint size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<wchar*>& value<wchar*>::operator = (wchar* val) { set(ang::move(val), -1); return*this; }
value<wchar*>& value<wchar*>::operator = (value<wchar*> const& val) { set(val._value, val._size); return*this; }
value<wchar*>& value<wchar*>::operator = (value<wchar*> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
value<wchar*>& value<wchar*>::operator ++ (void) {
	if (_size > 0) {
		_value++;
		_size--;
	}
	return*this;
}
value<wchar*> value<wchar*>::operator ++ (int) {
	value<wchar*> save = *this;
	if (_size > 0) {
		_value++;
		_size--;
	}
	return save;
}
value<wchar*>& value<wchar*>::operator -- (void) {
	if (_value != null) {
		_value--;
		_size++;
	}
	return*this;
}
value<wchar*> value<wchar*>::operator -- (int) {
	value<wchar*> save = *this;
	if (_value != null) {
		_value--;
		_size++;
	}
	return save;
}

value<wchar const*>::value() : _value(null), _size(0) {}
value<wchar const*>::value(type val, uint size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<wchar const*>::value(ang::nullptr_t) : _value(null), _size(0) {}
value<wchar const*>::value(value<wchar> const* val, uint size) : _value(&val->get()), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length((wchar*)val);
}
value<wchar const*>::value(value const& other) : _value(other._value), _size(other._size) {}
value<wchar const*>::value(value && other) : _value(ang::move(other._value)), _size(other._size) {}
wchar const* & value<wchar const*>::get() { return _value; }
wchar const* const& value<wchar const*>::get()const { return _value; }
void value<wchar const*>::set(type val, uint size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<wchar const*>& value<wchar const*>::operator = (type val) { set(ang::move(val), -1); return*this; }
value<wchar const*>& value<wchar const*>::operator = (value<wchar const*> const& val) { set(val._value, val._size); return*this; }
value<wchar const*>& value<wchar const*>::operator = (value<wchar const*> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
value<wchar const*>& value<wchar const*>::operator ++ (void) {
	if (_size > 0) {
		_value++;
		_size--;
	}
	return*this;
}
value<wchar const*> value<wchar const*>::operator ++ (int) {
	value<wchar const*> save = *this;
	if (_size > 0) {
		_value++;
		_size--;
	}
	return save;
}
value<wchar const*>& value<wchar const*>::operator -- (void) {
	if (_value != null) {
		_value--;
		_size++;
	}
	return*this;
}
value<wchar const*> value<wchar const*>::operator -- (int) {
	value<wchar const*> save = *this;
	if (_value != null) {
		_value--;
		_size++;
	}
	return save;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


value<mchar>::value() : _value() {}
value<mchar>::value(type val) : _value(val) {}
value<mchar>::value(value<mchar> const& other) : _value(other._value) {}
value<mchar>::value(value<mchar> && other) : _value(ang::move(other._value)) {}
mchar & value<mchar>::get() { return _value; }
mchar const& value<mchar>::get()const { return _value; }
void value<mchar>::set(type val) { _value = ang::move(val); }
value<mchar>& value<mchar>::operator = (type val) { set(ang::move(val)); return*this; }
value<mchar>& value<mchar>::operator = (value<mchar> const& val) { set(val._value); return*this; }
value<mchar>& value<mchar>::operator = (value<mchar> && val) { set(ang::move(val._value)); return*this; }

value<mchar const>::value(mchar val) : _value(val) {}
value<mchar const>::value(value<mchar const> const& other) : _value(other._value) {}
mchar const& value<mchar const>::get()const { return _value; }

value<mchar&>::value(mchar& val) : _value(val) {}
value<mchar&>::value(value<mchar&>& other) : _value(other._value) {}
value<mchar&>::value(value<mchar>& other) : _value(other.get()) {}
mchar & value<mchar&>::get() { return _value; }
mchar const& value<mchar&>::get()const { return _value; }
void value<mchar&>::set(type val) { _value = ang::move(val); }
value<mchar>& value<mchar&>::operator = (type val) { set(ang::move(val)); return*reinterpret_cast<value<mchar>*>(&get()); }
value<mchar>& value<mchar&>::operator = (value<mchar&> const& val) { set(val._value); return*reinterpret_cast<value<mchar>*>(&get()); }
value<mchar>& value<mchar&>::operator = (value<mchar> val) { set(ang::move(val.get())); return*reinterpret_cast<value<mchar>*>(&get()); }

value<mchar const&>::value(mchar const& val) : _value(val) {}
value<mchar const&>::value(value<mchar const&> const& other) : _value(other._value) {}
mchar const& value<mchar const&>::get()const { return _value; }

value<mchar*>::value() : _value(null) {}
value<mchar*>::value(mchar* val, uint size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<mchar*>::value(value<mchar>* val, uint size) : _value(&val->get()), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length((mchar*)val);
}
value<mchar*>::value(value const& other) : _value(other._value), _size(other._size) {}
value<mchar*>::value(value && other) : _value(ang::move(other._value)), _size(other._size) {}
value<mchar*>::value(ang::nullptr_t) : _value(null), _size(0) {}
mchar* & value<mchar*>::get() { return _value; }
mchar* const& value<mchar*>::get()const { return _value; }
void value<mchar*>::set(mchar* val, uint size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<mchar*>& value<mchar*>::operator = (mchar* val) { set(ang::move(val), -1); return*this; }
value<mchar*>& value<mchar*>::operator = (value<mchar*> const& val) { set(val._value, val._size); return*this; }
value<mchar*>& value<mchar*>::operator = (value<mchar*> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
value<mchar*>& value<mchar*>::operator ++ (void) {
	if (_size > 0) {
		_value++;
		_size--;
	}
	return*this;
}
value<mchar*> value<mchar*>::operator ++ (int) {
	value<mchar*> save = *this;
	if (_size > 0) {
		_value++;
		_size--;
	}
	return save;
}
value<mchar*>& value<mchar*>::operator -- (void) {
	if (_value != null) {
		_value--;
		_size++;
	}
	return*this;
}
value<mchar*> value<mchar*>::operator -- (int) {
	value<mchar*> save = *this;
	if (_value != null) {
		_value--;
		_size++;
	}
	return save;
}

value<mchar const*>::value() : _value(null), _size(0) {}
value<mchar const*>::value(type val, uint size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<mchar const*>::value(ang::nullptr_t) : _value(null), _size(0) {}
value<mchar const*>::value(value<mchar> const* val, uint size) : _value(&val->get()), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length((mchar*)val);
}
value<mchar const*>::value(value const& other) : _value(other._value), _size(other._size) {}
value<mchar const*>::value(value && other) : _value(ang::move(other._value)), _size(other._size) {}
mchar const* & value<mchar const*>::get() { return _value; }
mchar const* const& value<mchar const*>::get()const { return _value; }
void value<mchar const*>::set(type val, uint size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
value<mchar const*>& value<mchar const*>::operator = (type val) { set(ang::move(val), -1); return*this; }
value<mchar const*>& value<mchar const*>::operator = (value<mchar const*> const& val) { set(val._value, val._size); return*this; }
value<mchar const*>& value<mchar const*>::operator = (value<mchar const*> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
value<mchar const*>& value<mchar const*>::operator ++ (void) {
	if (_size > 0) {
		_value++;
		_size--;
	}
	return*this;
}
value<mchar const*> value<mchar const*>::operator ++ (int) {
	value<mchar const*> save = *this;
	if (_size > 0) {
		_value++;
		_size--;
	}
	return save;
}
value<mchar const*>& value<mchar const*>::operator -- (void) {
	if (_value != null) {
		_value--;
		_size++;
	}
	return*this;
}
value<mchar const*> value<mchar const*>::operator -- (int) {
	value<mchar const*> save = *this;
	if (_value != null) {
		_value--;
		_size++;
	}
	return save;
}


static_array<char>::static_array() : _value(null), _size(0) {}
static_array<char>::static_array(type val, uint size) : _value(val), _size(size == -1 ? strings::algorithms::string_length(val) : size) {}
static_array<char>::static_array(ang::nullptr_t, uint) : _value(null), _size(0) {}
static_array<char>::static_array(str_t val) : _value(val.get()), _size(val.size()) {}
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

static_array<const char>::static_array() : _value(null), _size(0) {}
static_array<const char>::static_array(type val, uint size) : _value(val), _size(size) {}
static_array<const char>::static_array(ang::nullptr_t, uint size) : _value(null), _size(0) {}
static_array<const char>::static_array(cstr_t val) : _value(val.get()), _size(val.size()) {}
static_array<const char>::static_array(static_array<char> const& val) : _value(val.get()), _size(val.size()) {}
static_array<const char>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
static_array<const char>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
static_array<const char>::~static_array() {}
char const* & static_array<const char>::get() { return _value; }
char const* const& static_array<const char>::get()const { return _value; }
void static_array<const char>::set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
char const* static_array<const char>::data()const { return _value; }
uint static_array<const char>::size()const { return _size; }
static_array<const char>& static_array<const char>::operator = (type val) { set(ang::move(val), 1); return*this; }
static_array<const char>& static_array<const char>::operator = (value<char const*> const& val) { set(val.get(), 1); return*this; }
static_array<const char>& static_array<const char>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
static_array<const char>& static_array<const char>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

static_array<byte>::static_array() : _value(null), _size(0) {}
static_array<byte>::static_array(type val, uint size) : _value(val), _size(size == -1 ? strings::algorithms::string_length(val) : size) {}
static_array<byte>::static_array(ang::nullptr_t, uint) : _value(null), _size(0) {}
static_array<byte>::static_array(str_t val) : _value((byte*)val.get()), _size(val.size()) {}
static_array<byte>::static_array(mstr_t val) : _value(val.get()), _size(val.size()) {}
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

static_array<const byte>::static_array() : _value(null), _size(0) {}
static_array<const byte>::static_array(type val, uint size) : _value(val), _size(size) {}
static_array<const byte>::static_array(ang::nullptr_t, uint size) : _value(null), _size(0) {}
static_array<const byte>::static_array(cstr_t val) : _value((byte const*)val.get()), _size(val.size()) {}
static_array<const byte>::static_array(cmstr_t val) : _value(val.get()), _size(val.size()) {}
static_array<const byte>::static_array(static_array<byte> const& val) : _value(val.get()), _size(val.size()) {}
static_array<const byte>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
static_array<const byte>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
static_array<const byte>::~static_array() {}
byte const* & static_array<const byte>::get() { return _value; }
byte const* const& static_array<const byte>::get()const { return _value; }
void static_array<const byte>::set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
byte const* static_array<const byte>::data()const { return _value; }
uint static_array<const byte>::size()const { return _size; }
static_array<const byte>& static_array<const byte>::operator = (type val) { set(ang::move(val), 1); return*this; }
static_array<const byte>& static_array<const byte>::operator = (value<byte const*> const& val) { set(val.get(), 1); return*this; }
static_array<const byte>& static_array<const byte>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
static_array<const byte>& static_array<const byte>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }


static_array<wchar>::static_array() : _value(null), _size(0) {}
static_array<wchar>::static_array(type val, uint size) : _value(val), _size(size == -1 ? strings::algorithms::string_length(val) : size) {}
static_array<wchar>::static_array(ang::nullptr_t, uint) : _value(null), _size(0) {}
static_array<wchar>::static_array(wstr_t val) : _value(val.get()), _size(val.size()) {}
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

static_array<const wchar>::static_array() : _value(null), _size(0) {}
static_array<const wchar>::static_array(type val, uint size) : _value(val), _size(size) {}
static_array<const wchar>::static_array(ang::nullptr_t, uint size) : _value(null), _size(0) {}
static_array<const wchar>::static_array(cwstr_t val) : _value(val.get()), _size(val.size()) {}
static_array<const wchar>::static_array(static_array<wchar> const& val) : _value(val.get()), _size(val.size()) {}
static_array<const wchar>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
static_array<const wchar>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
static_array<const wchar>::~static_array() {}
wchar const* & static_array<const wchar>::get() { return _value; }
wchar const* const& static_array<const wchar>::get()const { return _value; }
void static_array<const wchar>::set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
wchar const* static_array<const wchar>::data()const { return _value; }
uint static_array<const wchar>::size()const { return _size; }
static_array<const wchar>& static_array<const wchar>::operator = (type val) { set(ang::move(val), 1); return*this; }
static_array<const wchar>& static_array<const wchar>::operator = (value<wchar const*> const& val) { set(val.get(), 1); return*this; }
static_array<const wchar>& static_array<const wchar>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
static_array<const wchar>& static_array<const wchar>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }


