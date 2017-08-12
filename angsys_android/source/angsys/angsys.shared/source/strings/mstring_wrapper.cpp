/*********************************************************************************************************************/
/*   File Name: mstring_wrapper.cpp                                                                                  */
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


object_wrapper<strings::mstring_buffer>::object_wrapper()
	: _ptr(null)
{

}

object_wrapper<strings::mstring_buffer>::object_wrapper(strings::mstring_buffer* ptr)
	: _ptr(null)
{
	set(ptr);
}

object_wrapper<strings::mstring_buffer>::object_wrapper(ang::nullptr_t const&)
	: _ptr(null)
{
}

object_wrapper<strings::mstring_buffer>::object_wrapper(cstr_t cs)
	: _ptr(null)
{
	set(new strings::mstring_buffer(cs));
}

object_wrapper<strings::mstring_buffer>::object_wrapper(cwstr_t cs)
	: _ptr(null)
{
	set(new strings::mstring_buffer(cs));
}

object_wrapper<strings::mstring_buffer>::object_wrapper(cmstr_t cs)
	: _ptr(null)
{
	set(new strings::mstring_buffer(cs));
}

object_wrapper<strings::mstring_buffer>::object_wrapper(object_wrapper<strings::mstring_buffer> && other)
	: _ptr(null)
{
	strings::mstring_buffer * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

object_wrapper<strings::mstring_buffer>::object_wrapper(object_wrapper<strings::wstring_buffer> const& other)
	: _ptr(null)
{
	set(new strings::mstring_buffer(other.get()));
}

object_wrapper<strings::mstring_buffer>::object_wrapper(object_wrapper<strings::string_buffer> const& other)
	: _ptr(null)
{
	set(new strings::mstring_buffer(other.get()));
}

object_wrapper<strings::mstring_buffer>::object_wrapper(object_wrapper<strings::mstring_buffer> const& other)
	: _ptr(null)
{
	set(new strings::mstring_buffer(other.get()));
}

object_wrapper<strings::mstring_buffer>::~object_wrapper() { clean(); }

void object_wrapper<strings::mstring_buffer>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void object_wrapper<strings::mstring_buffer>::clean_unsafe()
{
	_ptr = null;
}

bool object_wrapper<strings::mstring_buffer>::is_empty()const
{
	return _ptr == null;
}

strings::mstring_buffer* object_wrapper<strings::mstring_buffer>::get(void)const
{
	return _ptr;
}

void object_wrapper<strings::mstring_buffer>::set(strings::mstring_buffer* ptr)
{
	mstring_buffer * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator = (object_wrapper<strings::mstring_buffer> && other)
{
	set(other.get());
	other.set(null);
	return *this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator = (object_wrapper<strings::wstring_buffer> const& other)
{
	if (_ptr)
		_ptr->copy(other);
	else
		set(new strings::mstring_buffer(other));
	return*this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator = (object_wrapper<strings::string_buffer> const& other)
{
	if (_ptr)
		_ptr->copy(other);
	else
		set(new strings::mstring_buffer(other));
	return*this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator = (object_wrapper<strings::mstring_buffer> const& other)
{
	if (_ptr)
		_ptr->copy(other);
	else
		set(new strings::mstring_buffer(other));
	return*this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator = (strings::mstring_buffer* ptr)
{
	set(ptr);
	return*this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator = (ang::nullptr_t const&)
{
	set(null);
	return*this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator = (cstr_t cs)
{
	if (_ptr)
		_ptr->copy(cs);
	else
		set(new strings::mstring_buffer(cs));
	return*this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator = (cwstr_t cs)
{
	if (_ptr)
		_ptr->copy(cs);
	else
		set(new strings::mstring_buffer(cs));
	return*this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator = (cmstr_t cs)
{
	if (_ptr)
		_ptr->copy(cs);
	else
		set(new strings::mstring_buffer(cs));
	return*this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator += (object_wrapper<strings::mstring_buffer> const& other)
{
	if (_ptr)
		_ptr->concat(other);
	else
		set(new strings::mstring_buffer(other));
	return*this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator += (cstr_t cs)
{
	if (_ptr)
		_ptr->concat(cs);
	else
		set(new strings::mstring_buffer(cs));
	return*this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator += (cwstr_t cs)
{
	if (_ptr)
		_ptr->concat(cs);
	else
		set(new strings::mstring_buffer(cs));
	return*this;
}

object_wrapper<strings::mstring_buffer>& object_wrapper<strings::mstring_buffer>::operator += (cmstr_t cs)
{
	if (_ptr)
		_ptr->concat(cs);
	else
		set(new strings::mstring_buffer(cs));
	return*this;
}

object_wrapper_ptr<strings::mstring_buffer> object_wrapper<strings::mstring_buffer>::operator & (void)
{
	return this;
}

strings::mstring_buffer * object_wrapper<strings::mstring_buffer>::operator -> (void)
{
	return get();
}

strings::mstring_buffer const* object_wrapper<strings::mstring_buffer>::operator -> (void)const
{
	return get();
}

object_wrapper<strings::mstring_buffer>::operator strings::mstring_buffer * (void)
{
	return get();
}

object_wrapper<strings::mstring_buffer>::operator strings::mstring_buffer const* (void)const
{
	return get();
}

object_wrapper<strings::mstring_buffer>::operator mstr_t (void)
{
	if (_ptr)return _ptr->str();
	return null;
}

object_wrapper<strings::mstring_buffer>::operator cmstr_t (void)const
{
	if (_ptr)return _ptr->cstr();
	return null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ang::operator == (mstring const& first, mstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cmstr_t)second) == 0); }
bool ang::operator != (mstring const& first, mstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cmstr_t)second) != 0); }
bool ang::operator >= (mstring const& first, mstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cmstr_t)second) >= 0); }
bool ang::operator <= (mstring const& first, mstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cmstr_t)second) <= 0); }
bool ang::operator > (mstring const& first, mstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cmstr_t)second) > 0); }
bool ang::operator < (mstring const& first, mstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cmstr_t)second) < 0); }

bool ang::operator == (mstring const& first, wstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cwstr_t)second) == 0); }
bool ang::operator != (mstring const& first, wstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cwstr_t)second) != 0); }
bool ang::operator >= (mstring const& first, wstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cwstr_t)second) >= 0); }
bool ang::operator <= (mstring const& first, wstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cwstr_t)second) <= 0); }
bool ang::operator > (mstring const& first, wstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cwstr_t)second) > 0); }
bool ang::operator < (mstring const& first, wstring const& second) { return (algorithms::string_compare((cmstr_t)first, (cwstr_t)second) < 0); }

bool ang::operator == (mstring const& first, string const& second) { return (algorithms::string_compare((cmstr_t)first, (cstr_t)second) == 0); }
bool ang::operator != (mstring const& first, string const& second) { return (algorithms::string_compare((cmstr_t)first, (cstr_t)second) != 0); }
bool ang::operator >= (mstring const& first, string const& second) { return (algorithms::string_compare((cmstr_t)first, (cstr_t)second) >= 0); }
bool ang::operator <= (mstring const& first, string const& second) { return (algorithms::string_compare((cmstr_t)first, (cstr_t)second) <= 0); }
bool ang::operator > (mstring const& first, string const& second) { return (algorithms::string_compare((cmstr_t)first, (cstr_t)second) > 0); }
bool ang::operator < (mstring const& first, string const& second) { return (algorithms::string_compare((cmstr_t)first, (cstr_t)second) < 0); }

bool ang::operator == (mstring const& first, cstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) == 0); }
bool ang::operator != (mstring const& first, cstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) != 0); }
bool ang::operator >= (mstring const& first, cstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) >= 0); }
bool ang::operator <= (mstring const& first, cstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) <= 0); }
bool ang::operator > (mstring const& first, cstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) > 0); }
bool ang::operator < (mstring const& first, cstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) < 0); }

bool ang::operator == (cstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) == 0); }
bool ang::operator != (cstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) != 0); }
bool ang::operator >= (cstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) >= 0); }
bool ang::operator <= (cstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) <= 0); }
bool ang::operator > (cstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) > 0); }
bool ang::operator < (cstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) < 0); }

bool ang::operator == (mstring const& first, cwstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) == 0); }
bool ang::operator != (mstring const& first, cwstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) != 0); }
bool ang::operator >= (mstring const& first, cwstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) >= 0); }
bool ang::operator <= (mstring const& first, cwstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) <= 0); }
bool ang::operator > (mstring const& first, cwstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) > 0); }
bool ang::operator < (mstring const& first, cwstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) < 0); }

bool ang::operator == (cwstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) == 0); }
bool ang::operator != (cwstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) != 0); }
bool ang::operator >= (cwstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) >= 0); }
bool ang::operator <= (cwstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) <= 0); }
bool ang::operator > (cwstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) > 0); }
bool ang::operator < (cwstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) < 0); }

bool ang::operator == (mstring const& first, cmstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) == 0); }
bool ang::operator != (mstring const& first, cmstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) != 0); }
bool ang::operator >= (mstring const& first, cmstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) >= 0); }
bool ang::operator <= (mstring const& first, cmstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) <= 0); }
bool ang::operator > (mstring const& first, cmstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) > 0); }
bool ang::operator < (mstring const& first, cmstr_t second) { return (algorithms::string_compare((cmstr_t)first, second) < 0); }

bool ang::operator == (cmstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) == 0); }
bool ang::operator != (cmstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) != 0); }
bool ang::operator >= (cmstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) >= 0); }
bool ang::operator <= (cmstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) <= 0); }
bool ang::operator > (cmstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) > 0); }
bool ang::operator < (cmstr_t first, mstring const& second) { return (algorithms::string_compare(first, (cmstr_t)second) < 0); }
