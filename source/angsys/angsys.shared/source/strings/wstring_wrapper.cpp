/*********************************************************************************************************************/
/*   File Name: wstring_wrapper.cpp                                                                                  */
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


object_wrapper<strings::wstring_buffer>::object_wrapper()
	: _ptr(null)
{

}

object_wrapper<strings::wstring_buffer>::object_wrapper(strings::wstring_buffer* ptr)
	: _ptr(null)
{
	set(ptr);
}

object_wrapper<strings::wstring_buffer>::object_wrapper(ang::nullptr_t const&)
	: _ptr(null)
{
}

object_wrapper<strings::wstring_buffer>::object_wrapper(cstr_t cs)
	: _ptr(null)
{
	set(new strings::wstring_buffer(cs));
}

object_wrapper<strings::wstring_buffer>::object_wrapper(cwstr_t cs)
	: _ptr(null)
{
	set(new strings::wstring_buffer(cs));
}

object_wrapper<strings::wstring_buffer>::object_wrapper(cmstr_t cs)
	: _ptr(null)
{
	set(new strings::wstring_buffer(cs));
}

object_wrapper<strings::wstring_buffer>::object_wrapper(object_wrapper<strings::wstring_buffer> && other)
	: _ptr(null)
{
	strings::wstring_buffer * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

object_wrapper<strings::wstring_buffer>::object_wrapper(object_wrapper<strings::wstring_buffer> const& other)
	: _ptr(null)
{
	set(new strings::wstring_buffer(other.get()));
}

object_wrapper<strings::wstring_buffer>::object_wrapper(object_wrapper<strings::string_buffer> const& other)
	: _ptr(null)
{
	set(new strings::wstring_buffer(other.get()));
}

object_wrapper<strings::wstring_buffer>::object_wrapper(object_wrapper<strings::mstring_buffer> const& other)
	: _ptr(null)
{
	set(new strings::wstring_buffer(other.get()));
}

object_wrapper<strings::wstring_buffer>::~object_wrapper() { clean(); }

void object_wrapper<strings::wstring_buffer>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void object_wrapper<strings::wstring_buffer>::clean_unsafe()
{
	_ptr = null;
}

bool object_wrapper<strings::wstring_buffer>::is_empty()const
{
	return _ptr == null;
}

strings::wstring_buffer* object_wrapper<strings::wstring_buffer>::get(void)const
{
	return _ptr;
}

void object_wrapper<strings::wstring_buffer>::set(strings::wstring_buffer* ptr )
{
	wstring_buffer * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

strings::wstring_buffer ** object_wrapper<strings::wstring_buffer>::addres_of(void)
{
	return &_ptr;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator = (object_wrapper<strings::wstring_buffer> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator = (object_wrapper<strings::wstring_buffer> const& other)
{
	if (_ptr)
		_ptr->copy(other.get());
	else
		set(new strings::wstring_buffer(other.get()));	
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator = (object_wrapper<strings::string_buffer> const& other)
{
	if (_ptr)
		_ptr->copy(other.get());
	else
		set(new strings::wstring_buffer(other.get()));
	
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator = (object_wrapper<strings::mstring_buffer> const& other)
{
	if (_ptr)
		_ptr->copy(other.get());
	else
		set(new strings::wstring_buffer(other.get()));
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator = (strings::wstring_buffer* ptr)
{
	set(ptr);
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator = (ang::nullptr_t const&)
{
	set(null);
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator = (cstr_t cs)
{
	if (_ptr)
		_ptr->copy(cs);
	else
		set(new strings::wstring_buffer(cs));
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator = (cwstr_t cs)
{
	if (_ptr)
		_ptr->copy(cs);
	else
		set(new strings::wstring_buffer(cs));
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator = (cmstr_t cs)
{
	if (_ptr)
		_ptr->copy(cs);
	else
		set(new strings::wstring_buffer(cs));
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator += (object_wrapper<strings::wstring_buffer> const& other)
{
	if (_ptr)
		_ptr->concat(other.get());
	else
		set(new strings::wstring_buffer(other.get()));	
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator += (cstr_t cs)
{
	if (_ptr)
		_ptr->concat(cs);
	else
		set(new strings::wstring_buffer(cs));
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator += (cwstr_t cs)
{
	if (_ptr)
		_ptr->concat(cs);
	else
		set(new strings::wstring_buffer(cs));
	return*this;
}

object_wrapper<strings::wstring_buffer>& object_wrapper<strings::wstring_buffer>::operator += (cmstr_t cs)
{
	if (_ptr)
		_ptr->concat(cs);
	else
		set(new strings::wstring_buffer(cs));
	return*this;
}

object_wrapper_ptr<strings::wstring_buffer> object_wrapper<strings::wstring_buffer>::operator & (void)
{
	return this;
}

strings::wstring_buffer * object_wrapper<strings::wstring_buffer>::operator -> (void)
{
	return get();
}

strings::wstring_buffer const* object_wrapper<strings::wstring_buffer>::operator -> (void)const
{
	return get();
}

object_wrapper<strings::wstring_buffer>::operator strings::wstring_buffer * (void)
{
	return get();
}

object_wrapper<strings::wstring_buffer>::operator strings::wstring_buffer const* (void)const
{
	return get();
}

object_wrapper<strings::wstring_buffer>::operator wstr_t (void)
{
	if (_ptr)
		return wstr_t(_ptr->str(), _ptr->length());
	return null;
}

object_wrapper<strings::wstring_buffer>::operator cwstr_t (void)const
{
	if (_ptr)
		return cwstr_t(_ptr->cstr(), _ptr->length());
	return null;
}

wchar& object_wrapper<strings::wstring_buffer>::operator [] (int idx)
{
	static wchar dummy = 0;
	if(_ptr && _ptr->length() > (uint)idx)
		return _ptr->str()[idx];
	return dummy;
}

wchar object_wrapper<strings::wstring_buffer>::operator [] (int idx)const
{
	if (_ptr && _ptr->length() > (uint)idx)
		return _ptr->str()[idx];
	return 0;
}


////////////////////////////////////////////////////////////////////////////////

wstring& ang::operator << (wstring& stream, wstring const& cstr)
{ 
	if (stream.is_empty())
		stream = cstr.is_empty() ? null : new wstring_buffer(cstr.get());
	else if(!cstr.is_empty())
		stream->concat(cstr.get());
	return stream;
}

wstring& ang::operator << (wstring& stream, string const& cstr)
{
	if (stream.is_empty())
		stream = cstr.is_empty() ? null : new wstring_buffer(cstr.get());
	else if (!cstr.is_empty())
		stream->concat(cstr.get());
	return stream;
}

wstring& ang::operator << (wstring& stream, cstr_t cstr)
{
	if (stream.is_empty())
		stream = new wstring_buffer(cstr);
	else
		stream->concat(cstr);
	return stream;
}

wstring& ang::operator << (wstring& stream, cwstr_t cstr)
{
	if (stream.is_empty())
		stream = new wstring_buffer(cstr);
	else
		stream->concat(cstr);
	return stream;
}

wstring& ang::operator << (wstring& stream, char c)
{
	char cstr[2] = { c, 0 };
	if (stream.is_empty())
		stream = new wstring_buffer(cstr);
	else
		stream->concat(cstr);
	return stream;
}

wstring& ang::operator << (wstring& stream, wchar c)
{
	wchar cstr[2] = { c, 0 };
	if (stream.is_empty())
		stream = new wstring_buffer(cstr);
	else
		stream->concat(cstr);
	return stream;
}

bool ang::operator == (wstring const& first, wstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cwstr_t)second) == 0); }
bool ang::operator != (wstring const& first, wstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cwstr_t)second) != 0); }
bool ang::operator >= (wstring const& first, wstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cwstr_t)second) >= 0); }
bool ang::operator <= (wstring const& first, wstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cwstr_t)second) <= 0); }
bool ang::operator > (wstring const& first, wstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cwstr_t)second) > 0); }
bool ang::operator < (wstring const& first, wstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cwstr_t)second) < 0); }

bool ang::operator == (wstring const& first, string const& second) { return (algorithms::string_compare((cwstr_t)first, (cstr_t)second) == 0); }
bool ang::operator != (wstring const& first, string const& second) { return (algorithms::string_compare((cwstr_t)first, (cstr_t)second) != 0); }
bool ang::operator >= (wstring const& first, string const& second) { return (algorithms::string_compare((cwstr_t)first, (cstr_t)second) >= 0); }
bool ang::operator <= (wstring const& first, string const& second) { return (algorithms::string_compare((cwstr_t)first, (cstr_t)second) <= 0); }
bool ang::operator > (wstring const& first, string const& second) { return (algorithms::string_compare((cwstr_t)first, (cstr_t)second) > 0); }
bool ang::operator < (wstring const& first, string const& second) { return (algorithms::string_compare((cwstr_t)first, (cstr_t)second) < 0); }

bool ang::operator == (wstring const& first, mstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cmstr_t)second) == 0); }
bool ang::operator != (wstring const& first, mstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cmstr_t)second) != 0); }
bool ang::operator >= (wstring const& first, mstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cmstr_t)second) >= 0); }
bool ang::operator <= (wstring const& first, mstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cmstr_t)second) <= 0); }
bool ang::operator > (wstring const& first, mstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cmstr_t)second) > 0); }
bool ang::operator < (wstring const& first, mstring const& second) { return (algorithms::string_compare((cwstr_t)first, (cmstr_t)second) < 0); }

bool ang::operator == (wstring const& first, cwstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) == 0); }
bool ang::operator != (wstring const& first, cwstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) != 0); }
bool ang::operator >= (wstring const& first, cwstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) >= 0); }
bool ang::operator <= (wstring const& first, cwstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) <= 0); }
bool ang::operator > (wstring const& first, cwstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) > 0); }
bool ang::operator < (wstring const& first, cwstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) < 0); }

bool ang::operator == (cwstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) == 0); }
bool ang::operator != (cwstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) != 0); }
bool ang::operator >= (cwstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) >= 0); }
bool ang::operator <= (cwstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) <= 0); }
bool ang::operator > (cwstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) > 0); }
bool ang::operator < (cwstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) < 0); }

bool ang::operator == (wstring const& first, cstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) == 0); }
bool ang::operator != (wstring const& first, cstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) != 0); }
bool ang::operator >= (wstring const& first, cstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) >= 0); }
bool ang::operator <= (wstring const& first, cstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) <= 0); }
bool ang::operator > (wstring const& first, cstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) > 0); }
bool ang::operator < (wstring const& first, cstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) < 0); }

bool ang::operator == (cstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) == 0); }
bool ang::operator != (cstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) != 0); }
bool ang::operator >= (cstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) >= 0); }
bool ang::operator <= (cstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) <= 0); }
bool ang::operator > (cstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) > 0); }
bool ang::operator < (cstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) < 0); }

bool ang::operator == (wstring const& first, cmstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) == 0); }
bool ang::operator != (wstring const& first, cmstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) != 0); }
bool ang::operator >= (wstring const& first, cmstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) >= 0); }
bool ang::operator <= (wstring const& first, cmstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) <= 0); }
bool ang::operator > (wstring const& first, cmstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) > 0); }
bool ang::operator < (wstring const& first, cmstr_t second) { return (algorithms::string_compare((cwstr_t)first, second) < 0); }

bool ang::operator == (cmstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) == 0); }
bool ang::operator != (cmstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) != 0); }
bool ang::operator >= (cmstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) >= 0); }
bool ang::operator <= (cmstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) <= 0); }
bool ang::operator > (cmstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) > 0); }
bool ang::operator < (cmstr_t first, wstring const& second) { return (algorithms::string_compare(first, (cwstr_t)second) < 0); }

