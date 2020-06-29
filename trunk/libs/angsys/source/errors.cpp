#include "pch.h"
#include "angsys.h"

using namespace ang;


static collections::pair<error_code, castr_t> def_except[] = {
	{ error_code::success, "success"_sv },
	{ error_code::failed, "operation failed"_sv },
	{ error_code::unknown, "unkown error"_sv },
	{ error_code::unimplemented, "this feature is not implemented yet..."_sv },
	{ error_code::unsupported, "the current platform do not support this feature..."_sv },
	{ error_code::invalid_memory, "there has been an invalid memory accessing"_sv },
	{ error_code::invalid_access, "the operation is not allowed"_sv },
	{ error_code::expired_object, "trying to use a expired object"_sv },
	{ error_code::array_overflow, "there has been an array_buffer overflow"_sv },
	{ error_code::invalid_param, "one or more params are incorrect"_sv },
	{ error_code::two_singleton, "more than one instance of a specialized singleton class has been created"_sv },
	{ error_code::already_created, "has tried to create a handle more than once"_sv },
	{ error_code::bad_allocation, "can not alloc memory"_sv },
	{ error_code::operation_canceled, "the operation was canceled"_sv },
	{ error_code::item_not_found, "the system can't find the specified item"_sv },
	{ error_code::file_not_found, "the system can't find the specified file"_sv },
};

///////////////////////////////////////////

ANG_ENUM_IMPLEMENT(ang, error_code)
ANG_ENUM_TO_STRING_IMPLEMENT(error_code, def_except, "unkown error"_sv);

error::error()
	: m_code(error_code::success)
	, m_info(0)
	, m_message()
{
	m_message = error_code_t((error_code)m_code).to_string();
}

error::error(error const& e)
	: m_code(e.m_code)
	, m_info(e.m_info)
	, m_message(e.m_message)
{
}

error::error(error_code_t const& type)
	: m_code(type.get())
	, m_info(0)
	, m_message(null)
{
	m_message = type.to_string();
}

error::error(dword errorCode, castr_t msg, error_code_t err)
	: m_code(err)
	, m_info(errorCode)
	, m_message(null)
{
	if (!msg.is_empty())
	{
		m_message = msg;
	}
	else
	{
		m_message = error_code_t((error_code)m_code).to_string();
	}
}

error::error(castr_t msg)
	: m_code(error_code::custom)
	, m_info(0)
	, m_message(null)
{
	if (!msg.is_empty())
	{
		m_message = msg;
	}
	else
	{
		m_code = error_code::unknown;
		m_message = "unknown"_sv;
		//m_message = error_code_t((error_code)m_code).to_string();
	}
}

error::~error()
{
	m_code = error_code::success;
	m_info = 0;
	m_message = null;
}

castr_t error::message()const
{
	return m_message;
}

error_code_t error::code()const
{
	return m_code;
}

dword error::info()const
{
	return m_info;
}

error& error::operator = (error const& err)
{
	if (&err != this) {
		m_info = err.m_info;
		m_code = err.m_code;
		m_message = err.m_message;
	}
	return*this;
}

error& error::operator = (error_code_t const& err)
{
	m_code = err;
	m_info = 0;
	m_message = err.to_string();
	return*this;
}

/////////////////////////////////////////////////////////////////

exception::exception()throw()
	: error()
{
#ifdef _DEBUG
	__debugbreak();
#endif // _DEBUG
}

exception::exception(error_code_t const& type)throw()
	: error(type)
{
#ifdef _DEBUG
//	__debugbreak();
#endif // _DEBUG
}

exception::exception(dword errorCode, castr_t msg)throw()
	: error(errorCode, msg)
{
#ifdef _DEBUG
	__debugbreak();
#endif // _DEBUG
}

exception::exception(castr_t msg)throw()
	: error(msg)
{
#ifdef _DEBUG
	__debugbreak();
#endif // _DEBUG
}

exception::exception(const exception& e)throw()
	: error(static_cast<error const&>(e))
{
#ifdef _DEBUG
	__debugbreak();
#endif // _DEBUG
}

exception::exception(error const& e)throw()
	: error(e)
{
#ifdef _DEBUG
	__debugbreak();
#endif // _DEBUG
}

exception::~exception()throw()
{
}

castr_t exception::what()const throw()
{
	return message();
}

dword exception::result()const throw()
{
	return code() == error_code::custom ? info() : (dword)code().get();
}

