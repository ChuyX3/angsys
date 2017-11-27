/*********************************************************************************************************************/
/*   File Name: exception.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"
//#include "ang/xml.h"

using namespace ang;

static collections::pair<except_code_enum, cstr_t> def_except[] = {
	{ except_code::unknown, "exception: unkown exception"_s },
	{ except_code::unsupported, "exception: the current platform do not support this feature..."_s },
	{ except_code::invalid_memory, "exception: there has been an invalid memory accessing"_s },
	{ except_code::invalid_access, "exception: the operation is not allowed"_s },
	{ except_code::expired_object, "exception: trying to use a expired object"_s },
	{ except_code::array_overflow, "exception: there has been an array_buffer overflow"_s },
	{ except_code::invalid_param, "exception: one or more params are incorrect"_s },
	{ except_code::two_singleton, "exception: more than one instance of a specialized singleton class has been created"_s },
	{ except_code::already_created, "exception: has tried to create a handle more than once"_s },
	{ except_code::bad_allocation, "exception: can not alloc memory"_s },
	{ except_code::operation_canceled, "exception: the operation was canceled"_s },
};

///////////////////////////////////////////

ANG_IMPLEMENT_ENUM(ang, except_code, uint, except_code::unknown);


cstr_t except_code_t::to_string()const {
	wsize idx = collections::algorithms::binary_search<except_code>(_value, to_array(def_except));
	if (idx > array_size(def_except))
		return  "exception: unkown exception"_s;
	else
		return def_except[idx].value;
}

exception::exception()throw()
	: object()
	, code(except_code::unknown)
	, message()
{
	message = code.to_string();
}

exception::exception(except_code_t type)throw()
	: object()
	, code(type)
	, message(null)
{
	message = type.to_string();
}

exception::exception(uint errorCode, string msg)throw()
	: object()
	, code((except_code)errorCode)
	, message(null)
{
	if (!msg.is_empty())
	{
		message = msg.get();
	}
	else
	{
		message = code.to_string();
	}
}

exception::exception(string msg)throw()
	: object()
	, code(except_code::custom)
	, message(null)
{
	if (!msg.is_empty())
	{
		message = msg.get();
	}
	else
	{
		code = except_code::unknown;
		message = "unknown"_s;
		message = code.to_string();
	}
}

exception::exception(const exception& e)throw()
	: object()
	, code(e.code)
	, message(null)
{
	if (e.message.get()) {
		message = e.message;
	}
}

exception::~exception()throw()
{
	code = except_code::unknown;
	message = null;
}

string exception::what()const throw()
{
	return message;
}

dword exception::result()const throw()
{
	return (dword)code.get();
}

type_name_t exception::class_name()throw()
{
	return "ang::exception_t"_s;
}

type_name_t exception::object_name()const throw()
{
	return "ang::exception_t"_s;
}

bool exception::is_inherited_of(type_name_t type) throw()
{
	return (type == type_of<exception>())
		|| object::is_inherited_of(type);
}

bool exception::is_kind_of(type_name_t type)const throw()
{
	return (type == type_of<exception>())
		|| object::is_kind_of(type);
}


bool exception::query_object(type_name_t type, unknown_ptr_t out) throw()
{
	if (out == null)
		return false;
	if (type == type_of<exception>())
	{
		*out = this;
		return true;
	}
	return object::query_object(type, out);
}

/////////////////////////////////////////////////////////////////

object_wrapper<exception>::object_wrapper()
	: _ptr(null)
{
	set(new exception());
}

object_wrapper<exception>::object_wrapper(except_code_t type)
	: _ptr(null)
{
	set(new exception(type));
}

object_wrapper<exception>::object_wrapper(dword errorCode, string msg)
	: _ptr(null)
{
	set(new exception(errorCode, msg));
}

object_wrapper<exception>::object_wrapper(string msg)
	: _ptr(null)
{
	set(new exception(msg));
}

object_wrapper<exception>::object_wrapper(object_wrapper && other)
	: _ptr(null)
{
	exception * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

object_wrapper<exception>::object_wrapper(object_wrapper const& other)
	: _ptr(null)
{
	set(other._ptr);
}

object_wrapper<exception>::object_wrapper(ang::nullptr_t const&)
	: _ptr(null)
{
}

void object_wrapper<exception>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

bool object_wrapper<exception>::is_empty()const
{
	return _ptr == null;
}

exception* object_wrapper<exception>::get(void)const
{
	return _ptr;
}

void object_wrapper<exception>::set(exception* ptr)
{
	exception * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}


object_wrapper<exception>& object_wrapper<exception>::operator = (object_wrapper<exception> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

object_wrapper<exception>& object_wrapper<exception>::operator = (object_wrapper<exception> const& other)
{
	set(other._ptr);
	return*this;
}


exception *object_wrapper<exception>:: operator -> (void)
{
	return get();
}
exception const* object_wrapper<exception>::operator -> (void)const
{
	return get();
}




