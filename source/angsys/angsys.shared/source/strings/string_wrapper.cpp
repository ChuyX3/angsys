/*********************************************************************************************************************/
/*   File Name: string_wrapper.cpp                                                                                   */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"

using namespace ang;
using namespace ang::strings;

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif


object_wrapper<strings::string_buffer>::object_wrapper()
	: _ptr(null)
{

}

object_wrapper<strings::string_buffer>::object_wrapper(strings::string_buffer* ptr)
	: _ptr(null)
{
	set(ptr);
}

object_wrapper<strings::string_buffer>::object_wrapper(ang::nullptr_t const&)
	: _ptr(null)
{
}

object_wrapper<strings::string_buffer>::object_wrapper(object_wrapper<strings::string_buffer> && other)
	: _ptr(null)
{
	strings::string_buffer * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

object_wrapper<strings::string_buffer>::object_wrapper(object_wrapper<strings::wstring_buffer> const& other)
	: _ptr(null)
{
	set(new strings::string_buffer(other.get()));
}

object_wrapper<strings::string_buffer>::object_wrapper(object_wrapper<strings::string_buffer> const& other)
	: _ptr(null)
{
	set(new strings::string_buffer(other.get()));
}

object_wrapper<strings::string_buffer>::object_wrapper(object_wrapper<strings::mstring_buffer> const& other)
	: _ptr(null)
{
	set(new strings::string_buffer(other.get()));
}

object_wrapper<strings::string_buffer>::object_wrapper(cstr_t cs)
	: _ptr(null)
{
	set(new strings::string_buffer(cs));
}

object_wrapper<strings::string_buffer>::object_wrapper(cwstr_t cs)
	: _ptr(null)
{
	set(new strings::string_buffer(cs));
}

object_wrapper<strings::string_buffer>::object_wrapper(cmstr_t cs)
	: _ptr(null)
{
	set(new strings::string_buffer(cs));
}

object_wrapper<strings::string_buffer>::~object_wrapper() { clean(); }

void object_wrapper<strings::string_buffer>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void object_wrapper<strings::string_buffer>::clean_unsafe()
{
	_ptr = null;
}

bool object_wrapper<strings::string_buffer>::is_empty()const
{
	return _ptr == null;
}

strings::string_buffer* object_wrapper<strings::string_buffer>::get(void)const
{
	return _ptr;
}

void object_wrapper<strings::string_buffer>::set(strings::string_buffer* ptr )
{
	string_buffer * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

strings::mstring_buffer ** object_wrapper<strings::mstring_buffer>::addres_of(void)
{
	return &_ptr;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator = (object_wrapper<strings::string_buffer> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator = (object_wrapper<strings::string_buffer> const& other)
{
	if (_ptr)
		_ptr->copy(other.get());
	else
		set(new strings::string_buffer(other.get()));	
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator = (object_wrapper<strings::wstring_buffer> const& other)
{
	if (_ptr)
		_ptr->copy(other.get());
	else
		set(new strings::string_buffer(other.get()));
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator = (object_wrapper<strings::mstring_buffer> const& other)
{
	if (_ptr)
		_ptr->copy(other.get());
	else
		set(new strings::string_buffer(other.get()));
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator = (strings::string_buffer* ptr)
{
	set(ptr);
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator = (ang::nullptr_t const&)
{
	set(null);
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator = (cstr_t cs)
{
	if (_ptr)
		_ptr->copy(cs);
	else
		set(new strings::string_buffer(cs));
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator = (cwstr_t cs)
{
	if (_ptr)
		_ptr->copy(cs);
	else
		set(new strings::string_buffer(cs));
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator = (cmstr_t cs)
{
	if (_ptr)
		_ptr->copy(cs);
	else
		set(new strings::string_buffer(cs));
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator += (object_wrapper<strings::string_buffer> const& other)
{
	if (_ptr)
		_ptr->concat(other.get());
	else
		set(new strings::string_buffer(other.get()));	
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator += (cstr_t cs)
{
	if (_ptr)
		_ptr->concat(cs);
	else
		set(new strings::string_buffer(cs));
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator += (cwstr_t cs)
{
	if (_ptr)
		_ptr->concat(cs);
	else
		set(new strings::string_buffer(cs));
	return*this;
}

object_wrapper<strings::string_buffer>& object_wrapper<strings::string_buffer>::operator += (cmstr_t cs)
{
	if (_ptr)
		_ptr->concat(cs);
	else
		set(new strings::string_buffer(cs));
	return*this;
}

object_wrapper_ptr<strings::string_buffer> object_wrapper<strings::string_buffer>::operator & (void)
{
	return this;
}

strings::string_buffer * object_wrapper<strings::string_buffer>::operator -> (void)
{
	return get();
}

strings::string_buffer const* object_wrapper<strings::string_buffer>::operator -> (void)const
{
	return get();
}

object_wrapper<strings::string_buffer>::operator strings::string_buffer * (void)
{
	return get();
}

object_wrapper<strings::string_buffer>::operator strings::string_buffer const* (void)const
{
	return get();
}

object_wrapper<strings::string_buffer>::operator str_t (void)
{
	if (_ptr)return _ptr->str();
	return null;
}

object_wrapper<strings::string_buffer>::operator cstr_t (void)const
{
	if (_ptr)return _ptr->cstr();
	return null;
}

char& object_wrapper<strings::string_buffer>::operator [] (int idx)
{
	static char dummy = 0;
	if(_ptr && _ptr->length() > (uint)idx)
		return _ptr->str()[idx];
	return dummy;
}

char object_wrapper<strings::string_buffer>::operator [] (int idx)const
{
	if (_ptr && _ptr->length() > (uint)idx)
		return _ptr->str()[idx];
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////

string& ang::operator << (string& stream, wstring const& cstr)
{
	if (stream.is_empty())
		stream = new string_buffer(cstr.get());
	else
		stream->concat(cstr.get());
	return stream;
}

string& ang::operator << (string& stream, string const& cstr)
{
	if (stream.is_empty())
		stream = new string_buffer(cstr.get());
	else
		stream->concat(cstr.get());
	return stream;
}

string& ang::operator << (string& stream, cstr_t cstr)
{
	if (stream.is_empty())
		stream = new string_buffer(cstr);
	else
		stream->concat(cstr);
	return stream;
}

string& ang::operator << (string& stream, cwstr_t cstr)
{
	if (stream.is_empty())
		stream = new string_buffer(cstr);
	else
		stream->concat(cstr);
	return stream;
}

string& ang::operator << (string& stream, char c)
{
	char_t cstr[2] = { c, 0 };
	if (stream.is_empty())
		stream = new string_buffer(cstr_t(cstr, 2));
	else
		stream->concat(cstr_t(cstr, 2));
	return stream;
}

string& ang::operator << (string& stream, wchar c)
{
	wchar cstr[2] = { c, 0 };
	if (stream.is_empty())
		stream = new string_buffer(cstr);
	else
		stream->concat(cstr);
	return stream;
}

bool ang::operator == (string const& first, string const& second) { return (algorithms::string_compare((cstr_t)first, (cstr_t)second) == 0); }
bool ang::operator != (string const& first, string const& second) { return (algorithms::string_compare((cstr_t)first, (cstr_t)second) != 0); }
bool ang::operator >= (string const& first, string const& second) { return (algorithms::string_compare((cstr_t)first, (cstr_t)second) >= 0); }
bool ang::operator <= (string const& first, string const& second) { return (algorithms::string_compare((cstr_t)first, (cstr_t)second) <= 0); }
bool ang::operator > (string const& first, string const& second) { return (algorithms::string_compare((cstr_t)first, (cstr_t)second) > 0); }
bool ang::operator < (string const& first, string const& second) { return (algorithms::string_compare((cstr_t)first, (cstr_t)second) < 0); }

bool ang::operator == (string const& first, wstring const& second) { return (algorithms::string_compare((cstr_t)first, (cwstr_t)second) == 0); }
bool ang::operator != (string const& first, wstring const& second) { return (algorithms::string_compare((cstr_t)first, (cwstr_t)second) != 0); }
bool ang::operator >= (string const& first, wstring const& second) { return (algorithms::string_compare((cstr_t)first, (cwstr_t)second) >= 0); }
bool ang::operator <= (string const& first, wstring const& second) { return (algorithms::string_compare((cstr_t)first, (cwstr_t)second) <= 0); }
bool ang::operator > (string const& first, wstring const& second) { return (algorithms::string_compare((cstr_t)first, (cwstr_t)second) > 0); }
bool ang::operator < (string const& first, wstring const& second) { return (algorithms::string_compare((cstr_t)first, (cwstr_t)second) < 0); }

bool ang::operator == (string const& first, mstring const& second) { return (algorithms::string_compare((cstr_t)first, (cmstr_t)second) == 0); }
bool ang::operator != (string const& first, mstring const& second) { return (algorithms::string_compare((cstr_t)first, (cmstr_t)second) != 0); }
bool ang::operator >= (string const& first, mstring const& second) { return (algorithms::string_compare((cstr_t)first, (cmstr_t)second) >= 0); }
bool ang::operator <= (string const& first, mstring const& second) { return (algorithms::string_compare((cstr_t)first, (cmstr_t)second) <= 0); }
bool ang::operator > (string const& first, mstring const& second) { return (algorithms::string_compare((cstr_t)first, (cmstr_t)second) > 0); }
bool ang::operator < (string const& first, mstring const& second) { return (algorithms::string_compare((cstr_t)first, (cmstr_t)second) < 0); }

bool ang::operator == (string const& first, cstr_t second) { return (algorithms::string_compare((cstr_t)first, second) == 0); }
bool ang::operator != (string const& first, cstr_t second) { return (algorithms::string_compare((cstr_t)first, second) != 0); }
bool ang::operator >= (string const& first, cstr_t second) { return (algorithms::string_compare((cstr_t)first, second) >= 0); }
bool ang::operator <= (string const& first, cstr_t second) { return (algorithms::string_compare((cstr_t)first, second) <= 0); }
bool ang::operator > (string const& first, cstr_t second) { return (algorithms::string_compare((cstr_t)first, second) > 0); }
bool ang::operator < (string const& first, cstr_t second) { return (algorithms::string_compare((cstr_t)first, second) < 0); }

bool ang::operator == (cstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) == 0); }
bool ang::operator != (cstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) != 0); }
bool ang::operator >= (cstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) >= 0); }
bool ang::operator <= (cstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) <= 0); }
bool ang::operator > (cstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) > 0); }
bool ang::operator < (cstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) < 0); }

bool ang::operator == (string const& first, cwstr_t second) { return (algorithms::string_compare((cstr_t)first, second) == 0); }
bool ang::operator != (string const& first, cwstr_t second) { return (algorithms::string_compare((cstr_t)first, second) != 0); }
bool ang::operator >= (string const& first, cwstr_t second) { return (algorithms::string_compare((cstr_t)first, second) >= 0); }
bool ang::operator <= (string const& first, cwstr_t second) { return (algorithms::string_compare((cstr_t)first, second) <= 0); }
bool ang::operator > (string const& first, cwstr_t second) { return (algorithms::string_compare((cstr_t)first, second) > 0); }
bool ang::operator < (string const& first, cwstr_t second) { return (algorithms::string_compare((cstr_t)first, second) < 0); }

bool ang::operator == (cwstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) == 0); }
bool ang::operator != (cwstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) != 0); }
bool ang::operator >= (cwstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) >= 0); }
bool ang::operator <= (cwstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) <= 0); }
bool ang::operator > (cwstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) > 0); }
bool ang::operator < (cwstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) < 0); }

bool ang::operator == (string const& first, cmstr_t second) { return (algorithms::string_compare((cstr_t)first, second) == 0); }
bool ang::operator != (string const& first, cmstr_t second) { return (algorithms::string_compare((cstr_t)first, second) != 0); }
bool ang::operator >= (string const& first, cmstr_t second) { return (algorithms::string_compare((cstr_t)first, second) >= 0); }
bool ang::operator <= (string const& first, cmstr_t second) { return (algorithms::string_compare((cstr_t)first, second) <= 0); }
bool ang::operator > (string const& first, cmstr_t second) { return (algorithms::string_compare((cstr_t)first, second) > 0); }
bool ang::operator < (string const& first, cmstr_t second) { return (algorithms::string_compare((cstr_t)first, second) < 0); }

bool ang::operator == (cmstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) == 0); }
bool ang::operator != (cmstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) != 0); }
bool ang::operator >= (cmstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) >= 0); }
bool ang::operator <= (cmstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) <= 0); }
bool ang::operator > (cmstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) > 0); }
bool ang::operator < (cmstr_t first, string const& second) { return (algorithms::string_compare(first, (cstr_t)second) < 0); }
