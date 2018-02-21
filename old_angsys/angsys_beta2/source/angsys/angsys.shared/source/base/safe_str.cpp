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
//array_view<char>::array_view() : _value(null), _size(0) {}
//array_view<char>::array_view(type val, wsize size) : _value(val), _size(size == -1 ? strings::algorithms::string_length(val) : size) {}
//array_view<char>::array_view(ang::nullptr_t, wsize) : _value(null), _size(0) {}
//array_view<char>::array_view(str_t val) : _value(val.get()), _size(val.size()) {}
//array_view<char>::array_view(array_view const& other) : _value(other._value), _size(other._size) {}
//array_view<char>::array_view(array_view && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//array_view<char>::~array_view() {}
//char* & array_view<char>::get() { return _value; }
//char* const& array_view<char>::get()const { return _value; }
//void array_view<char>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//char* array_view<char>::data()const { return _value; }
//wsize array_view<char>::size()const { return _size; }
//array_view<char>& array_view<char>::operator = (type val) { set(ang::move(val), strings::algorithms::string_length(val)); return*this; }
//array_view<char>& array_view<char>::operator = (safe_str<char> const& val) { set(val.get(), strings::algorithms::string_length(val.get())); return*this; }
//array_view<char>& array_view<char>::operator = (array_view const& val) { set(val._value, val._size); return*this; }
//array_view<char>& array_view<char>::operator = (array_view && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
//
//array_view<const char>::array_view() : _value(null), _size(0) {}
//array_view<const char>::array_view(type val, wsize size) : _value(val), _size(size) {}
//array_view<const char>::array_view(ang::nullptr_t, wsize size) : _value(null), _size(0) {}
//array_view<const char>::array_view(cstr_t val) : _value(val.get()), _size(val.size()) {}
//array_view<const char>::array_view(array_view<char> const& val) : _value(val.get()), _size(val.size()) {}
//array_view<const char>::array_view(array_view const& other) : _value(other._value), _size(other._size) {}
//array_view<const char>::array_view(array_view && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//array_view<const char>::~array_view() {}
//char const* & array_view<const char>::get() { return _value; }
//char const* const& array_view<const char>::get()const { return _value; }
//void array_view<const char>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//char const* array_view<const char>::data()const { return _value; }
//wsize array_view<const char>::size()const { return _size; }
//array_view<const char>& array_view<const char>::operator = (type val) { set(ang::move(val), 1); return*this; }
//array_view<const char>& array_view<const char>::operator = (safe_str<char const> const& val) { set(val.get(), 1); return*this; }
//array_view<const char>& array_view<const char>::operator = (array_view const& val) { set(val._value, val._size); return*this; }
//array_view<const char>& array_view<const char>::operator = (array_view && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
//
//array_view<byte>::array_view() : _value(null), _size(0) {}
//array_view<byte>::array_view(type val, wsize size) : _value(val), _size(size == -1 ? strings::algorithms::string_length(val) : size) {}
//array_view<byte>::array_view(ang::nullptr_t, wsize) : _value(null), _size(0) {}
//array_view<byte>::array_view(str_t val) : _value((byte*)val.get()), _size(val.size()) {}
//array_view<byte>::array_view(mstr_t val) : _value(val.get()), _size(val.size()) {}
//array_view<byte>::array_view(array_view const& other) : _value(other._value), _size(other._size) {}
//array_view<byte>::array_view(array_view && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//array_view<byte>::~array_view() {}
//byte* & array_view<byte>::get() { return _value; }
//byte* const& array_view<byte>::get()const { return _value; }
//void array_view<byte>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//byte* array_view<byte>::data()const { return _value; }
//wsize array_view<byte>::size()const { return _size; }
//array_view<byte>& array_view<byte>::operator = (type val) { set(ang::move(val), strings::algorithms::string_length(val)); return*this; }
//array_view<byte>& array_view<byte>::operator = (safe_str<byte> const& val) { set(val.get(), strings::algorithms::string_length(val.get())); return*this; }
//array_view<byte>& array_view<byte>::operator = (array_view const& val) { set(val._value, val._size); return*this; }
//array_view<byte>& array_view<byte>::operator = (array_view && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
//
//array_view<const byte>::array_view() : _value(null), _size(0) {}
//array_view<const byte>::array_view(type val, wsize size) : _value(val), _size(size) {}
//array_view<const byte>::array_view(ang::nullptr_t, wsize size) : _value(null), _size(0) {}
//array_view<const byte>::array_view(cstr_t val) : _value((byte const*)val.get()), _size(val.size()) {}
//array_view<const byte>::array_view(cmstr_t val) : _value(val.get()), _size(val.size()) {}
//array_view<const byte>::array_view(array_view<byte> const& val) : _value(val.get()), _size(val.size()) {}
//array_view<const byte>::array_view(array_view const& other) : _value(other._value), _size(other._size) {}
//array_view<const byte>::array_view(array_view && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//array_view<const byte>::~array_view() {}
//byte const* & array_view<const byte>::get() { return _value; }
//byte const* const& array_view<const byte>::get()const { return _value; }
//void array_view<const byte>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//byte const* array_view<const byte>::data()const { return _value; }
//wsize array_view<const byte>::size()const { return _size; }
//array_view<const byte>& array_view<const byte>::operator = (type val) { set(ang::move(val), 1); return*this; }
//array_view<const byte>& array_view<const byte>::operator = (safe_str<byte const> const& val) { set(val.get(), 1); return*this; }
//array_view<const byte>& array_view<const byte>::operator = (array_view const& val) { set(val._value, val._size); return*this; }
//array_view<const byte>& array_view<const byte>::operator = (array_view && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
//
//
//array_view<wchar>::array_view() : _value(null), _size(0) {}
//array_view<wchar>::array_view(type val, wsize size) : _value(val), _size(size == -1 ? strings::algorithms::string_length(val) : size) {}
//array_view<wchar>::array_view(ang::nullptr_t, wsize) : _value(null), _size(0) {}
//array_view<wchar>::array_view(wstr_t val) : _value(val.get()), _size(val.size()) {}
//array_view<wchar>::array_view(array_view const& other) : _value(other._value), _size(other._size) {}
//array_view<wchar>::array_view(array_view && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//array_view<wchar>::~array_view() {}
//wchar* & array_view<wchar>::get() { return _value; }
//wchar* const& array_view<wchar>::get()const { return _value; }
//void array_view<wchar>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//wchar* array_view<wchar>::data()const { return _value; }
//wsize array_view<wchar>::size()const { return _size; }
//array_view<wchar>& array_view<wchar>::operator = (type val) { set(ang::move(val), strings::algorithms::string_length(val)); return*this; }
//array_view<wchar>& array_view<wchar>::operator = (safe_str<wchar> const& val) { set(val.get(), strings::algorithms::string_length(val.get())); return*this; }
//array_view<wchar>& array_view<wchar>::operator = (array_view const& val) { set(val._value, val._size); return*this; }
//array_view<wchar>& array_view<wchar>::operator = (array_view && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
//
//array_view<const wchar>::array_view() : _value(null), _size(0) {}
//array_view<const wchar>::array_view(type val, wsize size) : _value(val), _size(size) {}
//array_view<const wchar>::array_view(ang::nullptr_t, wsize size) : _value(null), _size(0) {}
//array_view<const wchar>::array_view(cwstr_t val) : _value(val.get()), _size(val.size()) {}
//array_view<const wchar>::array_view(array_view<wchar> const& val) : _value(val.get()), _size(val.size()) {}
//array_view<const wchar>::array_view(array_view const& other) : _value(other._value), _size(other._size) {}
//array_view<const wchar>::array_view(array_view && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
//array_view<const wchar>::~array_view() {}
//wchar const* & array_view<const wchar>::get() { return _value; }
//wchar const* const& array_view<const wchar>::get()const { return _value; }
//void array_view<const wchar>::set(type val, wsize size) { _value = ang::move(val); _size = ang::move(size); }
//wchar const* array_view<const wchar>::data()const { return _value; }
//wsize array_view<const wchar>::size()const { return _size; }
//array_view<const wchar>& array_view<const wchar>::operator = (type val) { set(ang::move(val), 1); return*this; }
//array_view<const wchar>& array_view<const wchar>::operator = (safe_str<wchar const> const& val) { set(val.get(), 1); return*this; }
//array_view<const wchar>& array_view<const wchar>::operator = (array_view const& val) { set(val._value, val._size); return*this; }
//array_view<const wchar>& array_view<const wchar>::operator = (array_view && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }


