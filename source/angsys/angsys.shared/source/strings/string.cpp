#include "pch.h"
#include <angsys.h>
#include <ang/streams.hpp>

using namespace ang;
using namespace ang::strings;

ANG_IMPLEMENT_BASIC_INTERFACE(ang::text::itext_buffer, ang::ibuffer);

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::ascii
#include "string_impl.inl"
#undef CURRENT_ENCODING

void string_buffer<encoding::ascii>::format(cstr_t format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	wsize size = _vscprintf(format, arg_list);
	if (size <= 0U)
		return clean();

	realloc(size, false);
	if (is_local_data()) {
		vsprintf_s(_data._stack_buffer, LOCAL_CAPACITY - 1, format, arg_list);
		_data._stack_length = size;
	}
	else
	{
		vsprintf_s(_data._allocated_buffer, _data._allocated_capacity - 1, format, arg_list);
		_data._allocated_length = size;
	}
	va_end(arg_list);
}

void string_buffer<encoding::ascii>::concat_format(cstr_t format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	wsize size = _vscprintf(format, arg_list);
	if (size <= 0U)
		return clean();

	realloc(size + length(), true);
	if (is_local_data())
	{
		vsprintf_s(&_data._stack_buffer[_data._stack_length], LOCAL_CAPACITY - _data._stack_length - 1, format, arg_list);
		_data._stack_length += size;
	}
	else
	{
		vsprintf_s(&_data._allocated_buffer[_data._allocated_length], _data._allocated_capacity - _data._allocated_length - 1, format, arg_list);
		_data._allocated_length += size;
	}
	va_end(arg_list);
}

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::unicode
#include "string_impl.inl"
#undef CURRENT_ENCODING

void string_buffer<encoding::unicode>::format(cstr_t format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	wsize size = _vscwprintf(format, arg_list);
	if (size <= 0U)
		return clean();

	realloc(size, false);
	if (is_local_data()) {
		vswprintf_s(_data._stack_buffer, LOCAL_CAPACITY - 1, format, arg_list);
		_data._stack_length = size;
	}
	else
	{
		vswprintf_s(_data._allocated_buffer, _data._allocated_capacity - 1, format, arg_list);
		_data._allocated_length = size;
	}
	va_end(arg_list);
}

void string_buffer<encoding::unicode>::concat_format(cstr_t format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	wsize size = _vscwprintf(format, arg_list);
	if (size <= 0U)
		return clean();

	realloc(size + length(), true);
	if (is_local_data())
	{
		vswprintf_s(&_data._stack_buffer[_data._stack_length], LOCAL_CAPACITY - _data._stack_length - 1, format, arg_list);
		_data._stack_length += size;
	}
	else
	{
		vswprintf_s(&_data._allocated_buffer[_data._allocated_length], _data._allocated_capacity - _data._allocated_length - 1, format, arg_list);
		_data._allocated_length += size;
	}
	va_end(arg_list);
}

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf8
#include "string_impl.inl"
#undef CURRENT_ENCODING

void string_buffer<encoding::utf8>::format(cstr_t format, ...)
{
}

void string_buffer<encoding::utf8>::concat_format(cstr_t format, ...)
{
}

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf16
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf16_le
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf16_be
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf32
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf32_le
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf32_be
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

