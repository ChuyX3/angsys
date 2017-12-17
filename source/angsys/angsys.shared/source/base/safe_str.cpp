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
#include "angbase.hpp"

using namespace ang;

safe_str<char>::safe_str() : _value(null) {}
safe_str<char>::safe_str(char* val, wsize size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<char>::safe_str(safe_str const& other) : _value(other._value), _size(other._size) {}
safe_str<char>::safe_str(safe_str && other) : _value(ang::move(other._value)), _size(other._size) {}
safe_str<char>::safe_str(ang::nullptr_t) : _value(null), _size(0) {}
char* & safe_str<char>::get() { return _value; }
char* const& safe_str<char>::get()const { return _value; }
void safe_str<char>::set(char* val, wsize size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<char>& safe_str<char>::operator = (char* val) { set(ang::move(val), -1); return*this; }
safe_str<char>& safe_str<char>::operator = (safe_str<char> const& val) { set(val._value, val._size); return*this; }
safe_str<char>& safe_str<char>::operator = (safe_str<char> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

safe_str<char>& safe_str<char>::operator ++ (void)
{
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return*this;
}

safe_str<char> safe_str<char>::operator ++ (int)
{
	safe_str<char> save = *this;
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return save;
}

safe_str<char>& safe_str<char>::operator -- (void)
{
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return*this;
}

safe_str<char> safe_str<char>::operator -- (int)
{
	safe_str<char> save = *this;
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return save;
}


safe_str<const char>::safe_str() : _value(null), _size(0) {}
safe_str<const char>::safe_str(type val, wsize size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<const char>::safe_str(ang::nullptr_t) : _value(null), _size(0) {}
safe_str<const char>::safe_str(safe_str const& other) : _value(other._value), _size(other._size) {}
safe_str<const char>::safe_str(safe_str && other) : _value(ang::move(other._value)), _size(other._size) {}
char const* & safe_str<const char>::get() { return _value; }
char const* const& safe_str<const char>::get()const { return _value; }
void safe_str<const char>::set(type val, wsize size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<const char>& safe_str<const char>::operator = (type val) { set(ang::move(val), -1); return*this; }
safe_str<const char>& safe_str<const char>::operator = (safe_str<const char> const& val) { set(val._value, val._size); return*this; }
safe_str<const char>& safe_str<const char>::operator = (safe_str<const char> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
safe_str<const char>& safe_str<const char>::operator ++ (void) {
	if (_size > 0) {
		_value++;
		_size--;
	}
	return*this;
}
safe_str<const char> safe_str<const char>::operator ++ (int) {
	safe_str<const char> save = *this;
	if (_size > 0) {
		_value++;
		_size--;
	}
	return save;
}
safe_str<const char>& safe_str<const char>::operator -- (void) {
	if (_value != null) {
		_value--;
		_size++;
	}
	return*this;
}
safe_str<const char> safe_str<const char>::operator -- (int) {
	safe_str<const char> save = *this;
	if (_value != null) {
		_value--;
		_size++;
	}
	return save;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////

safe_str<wchar>::safe_str() : _value(null) {}
safe_str<wchar>::safe_str(wchar* val, wsize size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<wchar>::safe_str(safe_str const& other) : _value(other._value), _size(other._size) {}
safe_str<wchar>::safe_str(safe_str && other) : _value(ang::move(other._value)), _size(other._size) {}
safe_str<wchar>::safe_str(ang::nullptr_t) : _value(null), _size(0) {}
wchar* & safe_str<wchar>::get() { return _value; }
wchar* const& safe_str<wchar>::get()const { return _value; }
void safe_str<wchar>::set(wchar* val, wsize size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<wchar>& safe_str<wchar>::operator = (wchar* val) { set(ang::move(val), -1); return*this; }
safe_str<wchar>& safe_str<wchar>::operator = (safe_str<wchar> const& val) { set(val._value, val._size); return*this; }
safe_str<wchar>& safe_str<wchar>::operator = (safe_str<wchar> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

safe_str<wchar>& safe_str<wchar>::operator ++ (void)
{
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return*this;
}

safe_str<wchar> safe_str<wchar>::operator ++ (int)
{
	safe_str<wchar> save = *this;
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return save;
}

safe_str<wchar>& safe_str<wchar>::operator -- (void)
{
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return*this;
}

safe_str<wchar> safe_str<wchar>::operator -- (int)
{
	safe_str<wchar> save = *this;
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return save;
}


safe_str<const wchar>::safe_str() : _value(null), _size(0) {}
safe_str<const wchar>::safe_str(type val, wsize size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<const wchar>::safe_str(ang::nullptr_t) : _value(null), _size(0) {}
safe_str<const wchar>::safe_str(safe_str const& other) : _value(other._value), _size(other._size) {}
safe_str<const wchar>::safe_str(safe_str && other) : _value(ang::move(other._value)), _size(other._size) {}
wchar const* & safe_str<const wchar>::get() { return _value; }
wchar const* const& safe_str<const wchar>::get()const { return _value; }
void safe_str<const wchar>::set(type val, wsize size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<const wchar>& safe_str<const wchar>::operator = (type val) { set(ang::move(val), -1); return*this; }
safe_str<const wchar>& safe_str<const wchar>::operator = (safe_str<const wchar> const& val) { set(val._value, val._size); return*this; }
safe_str<const wchar>& safe_str<const wchar>::operator = (safe_str<const wchar> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
safe_str<const wchar>& safe_str<const wchar>::operator ++ (void) {
	if (_size > 0) {
		_value++;
		_size--;
	}
	return*this;
}
safe_str<const wchar> safe_str<const wchar>::operator ++ (int) {
	safe_str<const wchar> save = *this;
	if (_size > 0) {
		_value++;
		_size--;
	}
	return save;
}
safe_str<const wchar>& safe_str<const wchar>::operator -- (void) {
	if (_value != null) {
		_value--;
		_size++;
	}
	return*this;
}
safe_str<const wchar> safe_str<const wchar>::operator -- (int) {
	safe_str<const wchar> save = *this;
	if (_value != null) {
		_value--;
		_size++;
	}
	return save;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////

safe_str<byte>::safe_str() : _value(null) {}
safe_str<byte>::safe_str(byte* val, wsize size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<byte>::safe_str(safe_str const& other) : _value(other._value), _size(other._size) {}
safe_str<byte>::safe_str(safe_str && other) : _value(ang::move(other._value)), _size(other._size) {}
safe_str<byte>::safe_str(ang::nullptr_t) : _value(null), _size(0) {}
byte* & safe_str<byte>::get() { return _value; }
byte* const& safe_str<byte>::get()const { return _value; }
void safe_str<byte>::set(byte* val, wsize size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<byte>& safe_str<byte>::operator = (byte* val) { set(ang::move(val), -1); return*this; }
safe_str<byte>& safe_str<byte>::operator = (safe_str<byte> const& val) { set(val._value, val._size); return*this; }
safe_str<byte>& safe_str<byte>::operator = (safe_str<byte> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

safe_str<byte>& safe_str<byte>::operator ++ (void)
{
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return*this;
}

safe_str<byte> safe_str<byte>::operator ++ (int)
{
	safe_str<byte> save = *this;
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return save;
}

safe_str<byte>& safe_str<byte>::operator -- (void)
{
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return*this;
}

safe_str<byte> safe_str<byte>::operator -- (int)
{
	safe_str<byte> save = *this;
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return save;
}


safe_str<const byte>::safe_str() : _value(null), _size(0) {}
safe_str<const byte>::safe_str(type val, wsize size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<const byte>::safe_str(ang::nullptr_t) : _value(null), _size(0) {}
safe_str<const byte>::safe_str(safe_str const& other) : _value(other._value), _size(other._size) {}
safe_str<const byte>::safe_str(safe_str && other) : _value(ang::move(other._value)), _size(other._size) {}
byte const* & safe_str<const byte>::get() { return _value; }
byte const* const& safe_str<const byte>::get()const { return _value; }
void safe_str<const byte>::set(type val, wsize size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<const byte>& safe_str<const byte>::operator = (type val) { set(ang::move(val), -1); return*this; }
safe_str<const byte>& safe_str<const byte>::operator = (safe_str<const byte> const& val) { set(val._value, val._size); return*this; }
safe_str<const byte>& safe_str<const byte>::operator = (safe_str<const byte> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
safe_str<const byte>& safe_str<const byte>::operator ++ (void) {
	if (_size > 0) {
		_value++;
		_size--;
	}
	return*this;
}
safe_str<const byte> safe_str<const byte>::operator ++ (int) {
	safe_str<const byte> save = *this;
	if (_size > 0) {
		_value++;
		_size--;
	}
	return save;
}
safe_str<const byte>& safe_str<const byte>::operator -- (void) {
	if (_value != null) {
		_value--;
		_size++;
	}
	return*this;
}
safe_str<const byte> safe_str<const byte>::operator -- (int) {
	safe_str<const byte> save = *this;
	if (_value != null) {
		_value--;
		_size++;
	}
	return save;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

safe_str<char16_t>::safe_str() : _value(null) {}
safe_str<char16_t>::safe_str(char16_t* val, wsize size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<char16_t>::safe_str(safe_str const& other) : _value(other._value), _size(other._size) {}
safe_str<char16_t>::safe_str(safe_str && other) : _value(ang::move(other._value)), _size(other._size) {}
safe_str<char16_t>::safe_str(ang::nullptr_t) : _value(null), _size(0) {}
char16_t* & safe_str<char16_t>::get() { return _value; }
char16_t* const& safe_str<char16_t>::get()const { return _value; }
void safe_str<char16_t>::set(char16_t* val, wsize size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<char16_t>& safe_str<char16_t>::operator = (char16_t* val) { set(ang::move(val), -1); return*this; }
safe_str<char16_t>& safe_str<char16_t>::operator = (safe_str<char16_t> const& val) { set(val._value, val._size); return*this; }
safe_str<char16_t>& safe_str<char16_t>::operator = (safe_str<char16_t> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

safe_str<char16_t>& safe_str<char16_t>::operator ++ (void)
{
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return*this;
}

safe_str<char16_t> safe_str<char16_t>::operator ++ (int)
{
	safe_str<char16_t> save = *this;
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return save;
}

safe_str<char16_t>& safe_str<char16_t>::operator -- (void)
{
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return*this;
}

safe_str<char16_t> safe_str<char16_t>::operator -- (int)
{
	safe_str<char16_t> save = *this;
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return save;
}


safe_str<const char16_t>::safe_str() : _value(null), _size(0) {}
safe_str<const char16_t>::safe_str(type val, wsize size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<const char16_t>::safe_str(ang::nullptr_t) : _value(null), _size(0) {}
safe_str<const char16_t>::safe_str(safe_str const& other) : _value(other._value), _size(other._size) {}
safe_str<const char16_t>::safe_str(safe_str && other) : _value(ang::move(other._value)), _size(other._size) {}
char16_t const* & safe_str<const char16_t>::get() { return _value; }
char16_t const* const& safe_str<const char16_t>::get()const { return _value; }
void safe_str<const char16_t>::set(type val, wsize size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<const char16_t>& safe_str<const char16_t>::operator = (type val) { set(ang::move(val), -1); return*this; }
safe_str<const char16_t>& safe_str<const char16_t>::operator = (safe_str<const char16_t> const& val) { set(val._value, val._size); return*this; }
safe_str<const char16_t>& safe_str<const char16_t>::operator = (safe_str<const char16_t> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
safe_str<const char16_t>& safe_str<const char16_t>::operator ++ (void) {
	if (_size > 0) {
		_value++;
		_size--;
	}
	return*this;
}
safe_str<const char16_t> safe_str<const char16_t>::operator ++ (int) {
	safe_str<const char16_t> save = *this;
	if (_size > 0) {
		_value++;
		_size--;
	}
	return save;
}
safe_str<const char16_t>& safe_str<const char16_t>::operator -- (void) {
	if (_value != null) {
		_value--;
		_size++;
	}
	return*this;
}
safe_str<const char16_t> safe_str<const char16_t>::operator -- (int) {
	safe_str<const char16_t> save = *this;
	if (_value != null) {
		_value--;
		_size++;
	}
	return save;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////

safe_str<char32_t>::safe_str() : _value(null) {}
safe_str<char32_t>::safe_str(char32_t* val, wsize size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<char32_t>::safe_str(safe_str const& other) : _value(other._value), _size(other._size) {}
safe_str<char32_t>::safe_str(safe_str && other) : _value(ang::move(other._value)), _size(other._size) {}
safe_str<char32_t>::safe_str(ang::nullptr_t) : _value(null), _size(0) {}
char32_t* & safe_str<char32_t>::get() { return _value; }
char32_t* const& safe_str<char32_t>::get()const { return _value; }
void safe_str<char32_t>::set(char32_t* val, wsize size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<char32_t>& safe_str<char32_t>::operator = (char32_t* val) { set(ang::move(val), -1); return*this; }
safe_str<char32_t>& safe_str<char32_t>::operator = (safe_str<char32_t> const& val) { set(val._value, val._size); return*this; }
safe_str<char32_t>& safe_str<char32_t>::operator = (safe_str<char32_t> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }

safe_str<char32_t>& safe_str<char32_t>::operator ++ (void)
{
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return*this;
}

safe_str<char32_t> safe_str<char32_t>::operator ++ (int)
{
	safe_str<char32_t> save = *this;
	if (_size > 0)
	{
		_value++;
		_size--;
	}
	return save;
}

safe_str<char32_t>& safe_str<char32_t>::operator -- (void)
{
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return*this;
}

safe_str<char32_t> safe_str<char32_t>::operator -- (int)
{
	safe_str<char32_t> save = *this;
	if (_value != null)
	{
		_value--;
		_size++;
	}
	return save;
}


safe_str<const char32_t>::safe_str() : _value(null), _size(0) {}
safe_str<const char32_t>::safe_str(type val, wsize size) : _value(val), _size(0) {
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<const char32_t>::safe_str(ang::nullptr_t) : _value(null), _size(0) {}
safe_str<const char32_t>::safe_str(safe_str const& other) : _value(other._value), _size(other._size) {}
safe_str<const char32_t>::safe_str(safe_str && other) : _value(ang::move(other._value)), _size(other._size) {}
char32_t const* & safe_str<const char32_t>::get() { return _value; }
char32_t const* const& safe_str<const char32_t>::get()const { return _value; }
void safe_str<const char32_t>::set(type val, wsize size) {
	_value = val;
	if (val == null)
		_size = 0;
	else if (size != -1)
		_size = size;
	else
		_size = strings::algorithms::string_length(val);
}
safe_str<const char32_t>& safe_str<const char32_t>::operator = (type val) { set(ang::move(val), -1); return*this; }
safe_str<const char32_t>& safe_str<const char32_t>::operator = (safe_str<const char32_t> const& val) { set(val._value, val._size); return*this; }
safe_str<const char32_t>& safe_str<const char32_t>::operator = (safe_str<const char32_t> && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
safe_str<const char32_t>& safe_str<const char32_t>::operator ++ (void) {
	if (_size > 0) {
		_value++;
		_size--;
	}
	return*this;
}
safe_str<const char32_t> safe_str<const char32_t>::operator ++ (int) {
	safe_str<const char32_t> save = *this;
	if (_size > 0) {
		_value++;
		_size--;
	}
	return save;
}
safe_str<const char32_t>& safe_str<const char32_t>::operator -- (void) {
	if (_value != null) {
		_value--;
		_size++;
	}
	return*this;
}
safe_str<const char32_t> safe_str<const char32_t>::operator -- (int) {
	safe_str<const char32_t> save = *this;
	if (_value != null) {
		_value--;
		_size++;
	}
	return save;
}




//
//
//static_array<char>::static_array() : _value(null), _size(0) {}
//static_array<char>::static_array(type val, wsize size) : _value(val), _size(size == -1 ? strings::algorithms::string_length(val) : size) {}
//static_array<char>::static_array(ang::nullptr_t, wsize) : _value(null), _size(0) {}
//static_array<char>::static_array(str_t val) : _value(val.get()), _size(val.size()) {}
//static_array<char>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
//static_array<char>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//static_array<char>::~static_array() {}
//char* & static_array<char>::get() { return _value; }
//char* const& static_array<char>::get()const { return _value; }
//void static_array<char>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//char* static_array<char>::data()const { return _value; }
//wsize static_array<char>::size()const { return _size; }
//static_array<char>& static_array<char>::operator = (type val) { set(ang::move(val), strings::algorithms::string_length(val)); return*this; }
//static_array<char>& static_array<char>::operator = (safe_str<char> const& val) { set(val.get(), strings::algorithms::string_length(val.get())); return*this; }
//static_array<char>& static_array<char>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
//static_array<char>& static_array<char>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
//
//static_array<const char>::static_array() : _value(null), _size(0) {}
//static_array<const char>::static_array(type val, wsize size) : _value(val), _size(size) {}
//static_array<const char>::static_array(ang::nullptr_t, wsize size) : _value(null), _size(0) {}
//static_array<const char>::static_array(cstr_t val) : _value(val.get()), _size(val.size()) {}
//static_array<const char>::static_array(static_array<char> const& val) : _value(val.get()), _size(val.size()) {}
//static_array<const char>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
//static_array<const char>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//static_array<const char>::~static_array() {}
//char const* & static_array<const char>::get() { return _value; }
//char const* const& static_array<const char>::get()const { return _value; }
//void static_array<const char>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//char const* static_array<const char>::data()const { return _value; }
//wsize static_array<const char>::size()const { return _size; }
//static_array<const char>& static_array<const char>::operator = (type val) { set(ang::move(val), 1); return*this; }
//static_array<const char>& static_array<const char>::operator = (safe_str<char const> const& val) { set(val.get(), 1); return*this; }
//static_array<const char>& static_array<const char>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
//static_array<const char>& static_array<const char>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
//
//static_array<byte>::static_array() : _value(null), _size(0) {}
//static_array<byte>::static_array(type val, wsize size) : _value(val), _size(size == -1 ? strings::algorithms::string_length(val) : size) {}
//static_array<byte>::static_array(ang::nullptr_t, wsize) : _value(null), _size(0) {}
//static_array<byte>::static_array(str_t val) : _value((byte*)val.get()), _size(val.size()) {}
//static_array<byte>::static_array(mstr_t val) : _value(val.get()), _size(val.size()) {}
//static_array<byte>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
//static_array<byte>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//static_array<byte>::~static_array() {}
//byte* & static_array<byte>::get() { return _value; }
//byte* const& static_array<byte>::get()const { return _value; }
//void static_array<byte>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//byte* static_array<byte>::data()const { return _value; }
//wsize static_array<byte>::size()const { return _size; }
//static_array<byte>& static_array<byte>::operator = (type val) { set(ang::move(val), strings::algorithms::string_length(val)); return*this; }
//static_array<byte>& static_array<byte>::operator = (safe_str<byte> const& val) { set(val.get(), strings::algorithms::string_length(val.get())); return*this; }
//static_array<byte>& static_array<byte>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
//static_array<byte>& static_array<byte>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
//
//static_array<const byte>::static_array() : _value(null), _size(0) {}
//static_array<const byte>::static_array(type val, wsize size) : _value(val), _size(size) {}
//static_array<const byte>::static_array(ang::nullptr_t, wsize size) : _value(null), _size(0) {}
//static_array<const byte>::static_array(cstr_t val) : _value((byte const*)val.get()), _size(val.size()) {}
//static_array<const byte>::static_array(cmstr_t val) : _value(val.get()), _size(val.size()) {}
//static_array<const byte>::static_array(static_array<byte> const& val) : _value(val.get()), _size(val.size()) {}
//static_array<const byte>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
//static_array<const byte>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//static_array<const byte>::~static_array() {}
//byte const* & static_array<const byte>::get() { return _value; }
//byte const* const& static_array<const byte>::get()const { return _value; }
//void static_array<const byte>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//byte const* static_array<const byte>::data()const { return _value; }
//wsize static_array<const byte>::size()const { return _size; }
//static_array<const byte>& static_array<const byte>::operator = (type val) { set(ang::move(val), 1); return*this; }
//static_array<const byte>& static_array<const byte>::operator = (safe_str<byte const> const& val) { set(val.get(), 1); return*this; }
//static_array<const byte>& static_array<const byte>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
//static_array<const byte>& static_array<const byte>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
//
//
//static_array<wchar>::static_array() : _value(null), _size(0) {}
//static_array<wchar>::static_array(type val, wsize size) : _value(val), _size(size == -1 ? strings::algorithms::string_length(val) : size) {}
//static_array<wchar>::static_array(ang::nullptr_t, wsize) : _value(null), _size(0) {}
//static_array<wchar>::static_array(wstr_t val) : _value(val.get()), _size(val.size()) {}
//static_array<wchar>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
//static_array<wchar>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//static_array<wchar>::~static_array() {}
//wchar* & static_array<wchar>::get() { return _value; }
//wchar* const& static_array<wchar>::get()const { return _value; }
//void static_array<wchar>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//wchar* static_array<wchar>::data()const { return _value; }
//wsize static_array<wchar>::size()const { return _size; }
//static_array<wchar>& static_array<wchar>::operator = (type val) { set(ang::move(val), strings::algorithms::string_length(val)); return*this; }
//static_array<wchar>& static_array<wchar>::operator = (safe_str<wchar> const& val) { set(val.get(), strings::algorithms::string_length(val.get())); return*this; }
//static_array<wchar>& static_array<wchar>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
//static_array<wchar>& static_array<wchar>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
//
//static_array<const wchar>::static_array() : _value(null), _size(0) {}
//static_array<const wchar>::static_array(type val, wsize size) : _value(val), _size(size) {}
//static_array<const wchar>::static_array(ang::nullptr_t, wsize size) : _value(null), _size(0) {}
//static_array<const wchar>::static_array(cwstr_t val) : _value(val.get()), _size(val.size()) {}
//static_array<const wchar>::static_array(static_array<wchar> const& val) : _value(val.get()), _size(val.size()) {}
//static_array<const wchar>::static_array(static_array const& other) : _value(other._value), _size(other._size) {}
//static_array<const wchar>::static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//static_array<const wchar>::~static_array() {}
//wchar const* & static_array<const wchar>::get() { return _value; }
//wchar const* const& static_array<const wchar>::get()const { return _value; }
//void static_array<const wchar>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//wchar const* static_array<const wchar>::data()const { return _value; }
//wsize static_array<const wchar>::size()const { return _size; }
//static_array<const wchar>& static_array<const wchar>::operator = (type val) { set(ang::move(val), 1); return*this; }
//static_array<const wchar>& static_array<const wchar>::operator = (safe_str<wchar const> const& val) { set(val.get(), 1); return*this; }
//static_array<const wchar>& static_array<const wchar>::operator = (static_array const& val) { set(val._value, val._size); return*this; }
//static_array<const wchar>& static_array<const wchar>::operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }


