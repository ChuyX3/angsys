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
#include "ang_inlines.h"
#include "exception_code_map.h"
//#include "ang/xml.h"

using namespace ang;


ANG_IMPLEMENT_BASIC_INTERFACE(except_code_map, object);

static cstr_t def_except[] = {
	"exception: unkown exception",
	"exception: the current platform do not support this feature...",
	"exception: there has been an invalid memory accessing",
	"exception: the operation is not allowed",
	"exception: trying to use a expired object",
	"exception: there has been an array_buffer overflow",
	"exception: one or more params are incorrect",
	"exception: more than one instance of a specialized singleton class has been created",
	"exception: has tried to create a handle more than once",
	"exception: can not alloc memory"
	"exception: the operation was canceled"
};

except_code_map::except_code_map()
{
	//instance_manager::regist_instance(class_name(), this);
	_code_map.insert(except_code::unknown, def_except[except_code::unknown]);
	_code_map.insert(except_code::unsupported, def_except[except_code::unsupported]);
	_code_map.insert(except_code::invalid_memory, def_except[except_code::invalid_memory]);
	_code_map.insert(except_code::invalid_access, def_except[except_code::invalid_access]);
	_code_map.insert(except_code::expired_object, def_except[except_code::expired_object]);
	_code_map.insert(except_code::array_overflow, def_except[except_code::array_overflow]);
	_code_map.insert(except_code::invalid_param, def_except[except_code::invalid_param]);
	_code_map.insert(except_code::two_singleton, def_except[except_code::two_singleton]);
	_code_map.insert(except_code::already_created, def_except[except_code::already_created]);
	_code_map.insert(except_code::bad_allocation, def_except[except_code::bad_allocation]);
	_code_map.insert(except_code::operation_canceled, def_except[except_code::operation_canceled]);

	//_code_map.insert(xml::xml_exception_code::not_implemented, "exception: the operation is not implemented");
	//_code_map.insert(xml::xml_exception_code::parsing_error, "exception: an error has occured while parsing the document");
	//_code_map.insert(xml::xml_exception_code::unexpected_error, "exception: unexpected error has occured");
}

except_code_map::~except_code_map()
{
}

extern "C" int
#ifdef WINDOWS_PLATFORM
__declspec(dllexport)
#endif
 ang_exceptions_regist_code(unsigned int error_code, const char* error_message)
{
	return except_code_map::instance()->code_map().insert(error_code,  cstr_t(error_message, -1)) ? 0 : -1;
}

///////////////////////////////////////////

ANG_IMPLEMENT_ENUM(ang, except_code, uint, except_code::unknown);

cstr_t except_code_t::to_string()const
{
	cstr_t code;
	if (except_code_map::instance()->code_map().find(_value, code))
		return code;
	return "exception: unkown exception";
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
		message = new strings::string_buffer(*e.message.get());
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

uint exception::result()const throw()
{
	return (uint_t)code;
}

type_name_t exception::class_name()throw()
{
	return "ang::exception_t";
}

type_name_t exception::object_name()const throw()
{
	return "ang::exception_t";
}

bool exception::is_child_of(type_name_t type) throw()
{
	return (type == type_name<exception>())
		|| object::is_child_of(type);
}

bool exception::is_kind_of(type_name_t type)const throw()
{
	return (type == type_name<exception>())
		|| object::is_kind_of(type);
}


bool exception::query_object(type_name_t type, unknown_ptr_t out) throw()
{
	if (out == null)
		return false;
	if (type == type_name<exception>())
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

object_wrapper<exception>::object_wrapper(uint_t errorCode, string msg)
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




