/*********************************************************************************************************************/
/*   File Name: text_buffer_output_stream.cpp                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/streams.h"

using namespace ang;
using namespace ang::streams;

text_buffer_output_stream::text_buffer_output_stream()
	: _buffer(null)
	, _cursor(0)
	, _format(text::encoding::unknown)
	, _is_formating_enabled(false)
	, char_format("{char:}")
	, string_format("{string:n}")
	, signed_format("{signed:}")
	, unsigned_format("{unsigned:}")
	, floating_format("{floating:n.4}")
{
}

text_buffer_output_stream::text_buffer_output_stream(text_buffer_output_stream* stream)
	: text_buffer_output_stream()
{
	if (stream)
	{
		_buffer = stream->_buffer;
		_format = stream->_format;
	}
}

text_buffer_output_stream::text_buffer_output_stream(text::itext_buffer* buff)
	: text_buffer_output_stream()
{
	_buffer = buff;
	_format = buff ? buff->encoding().get() : text::encoding::unknown;
}

text_buffer_output_stream::text_buffer_output_stream(ibuffer_t buff, text::encoding_t f)
	: text_buffer_output_stream()
{
	_buffer = buff;
	_format = buff ? f.get() : text::encoding::unknown;
}

text_buffer_output_stream::~text_buffer_output_stream()
{
	_buffer = null;
	_cursor = 0;
	_format = text::encoding::unknown;
}

ANG_IMPLEMENT_CLASSNAME(ang::streams::text_buffer_output_stream);
ANG_IMPLEMENT_OBJECTNAME(ang::streams::text_buffer_output_stream);

bool text_buffer_output_stream::is_kind_of(type_name_t type)const
{
	return (type == type_name<text_buffer_output_stream>())
		|| object::is_kind_of(type)
		|| (type == type_name<itext_output_stream>());
}

bool text_buffer_output_stream::is_child_of(type_name_t type)
{
	return (type == type_name<text_buffer_output_stream>())
		|| object::is_child_of(type)
		|| (type == type_name<itext_output_stream>());
}

bool text_buffer_output_stream::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_name<text_buffer_output_stream>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_name<itext_output_stream>())
	{
		*out = static_cast<itext_output_stream*>(this);
		return true;
	}
	else if (type == type_name<ibuffer>())
	{
		*out = buffer();
		return true;
	}
	return false;
}

bool text_buffer_output_stream::is_valid()const
{
	return _buffer.get() != null;
}

stream_index_t text_buffer_output_stream::position()const
{
	return _cursor;
}

stream_size_t text_buffer_output_stream::stream_size()const
{
	return _buffer.get() ? _buffer->buffer_size() : 0U;
}


bool text_buffer_output_stream::command(special_command_t command)
{
	switch (command)
	{
	case special_command::end_line:
		write('\n');
		break;
	case special_command::space:
		write(' ');
		break;
	case special_command::tab:
		write('\t');
		break;
	case special_command::back_space:
	case special_command::del:
		if (position() > 0)
			move_to(position() - 1, stream_reference::current);
		break;
	case special_command::home:
		move_to(0, stream_reference::begin);
		break;
	case special_command::end:
		move_to(0, stream_reference::end);
		break;
	case special_command::insert:
	default:return false;
	}
	return true;
}

void text_buffer_output_stream::enable_text_format(bool value)
{
	_is_formating_enabled = value;
}

bool text_buffer_output_stream::is_text_format_enabled()const
{
	return _is_formating_enabled;
}

void text_buffer_output_stream::text_format(cstr_t format)
{
	text_format(text::text_format(format));
}

void text_buffer_output_stream::text_format(text::text_format_t format)
{
	switch (format.format_target())
	{
	case text::text_format::character:
		char_format = format;
		break;
	case text::text_format::text:
		string_format = format;
		break;
	case text::text_format::signed_interger:
		signed_format = format;
		break;
	case text::text_format::usigned_interger:
		unsigned_format = format;
		break;
	case text::text_format::floating:
		floating_format = format;
		break;
	default:break;
	}
}

bool text_buffer_output_stream::move_to(stream_index_t size, stream_reference_t ref)
{
	if (!is_valid())
		return false;
	auto curSize = stream_size();
	if (curSize == 0)
		return false;

	stream_index_t maxPos = curSize - 1;

	switch (ref)
	{
	default:break;
	case ang::streams::stream_reference::begin:
		if (size < 0)
			_cursor = 0;
		else
		{
			if (size > maxPos)
				_cursor = maxPos;
			else
				_cursor = size;
		}
		break;
	case ang::streams::stream_reference::current:
		if (size < 0)
		{
			if ((_cursor - size) <= 0)
				_cursor = 0;
			else
				_cursor -= size;
		}
		else
		{
			if ((_cursor + size) > maxPos)
				_cursor = maxPos;
			else
				_cursor += size;
		}
		break;
	case ang::streams::stream_reference::end:
		if (size > 0)
			_cursor = maxPos;
		else
		{
			if (size >= maxPos)
				_cursor = 0;
			else
				_cursor = maxPos - size;
		}
		break;
	}

	return true;
}

bool text_buffer_output_stream::forward(pointer ptr, stream_index_t size)
{
	stream_index_t maxPos = stream_size();

	if ((_cursor + size) > maxPos)
			_cursor = maxPos;
	else
		_cursor += size;

	_buffer->unmap_buffer(ptr, (uint)size);

	return true;
}

bool text_buffer_output_stream::backward(pointer ptr, stream_index_t size)
{
	auto curSize = stream_size();
	stream_index_t maxPos = curSize - 1L;

	if ((_cursor - size) <= 0)
		_cursor = 0;
	else
		_cursor -= size;

	return true;
}


text::encoding_t text_buffer_output_stream::text_encoding()const
{
	return _format;
}

ibuffer* text_buffer_output_stream::buffer()const
{
	return _buffer.get();
}

bool text_buffer_output_stream::attach(text::itext_buffer* buff)
{
	_buffer = buff;
	_format = buff ? buff->encoding().get() : text::encoding::unknown;
	_cursor = 0;
	return true;
}

bool text_buffer_output_stream::attach(ibuffer* buff, text::encoding_t format)
{
	_buffer = buff;
	_format = buff ? format.get() : text::encoding::unknown;
	_cursor = 0;
	return true;
}

bool text_buffer_output_stream::can_move_to(stream_index_t size, stream_reference_t ref)
{
	if (!is_valid())
		return false;
	auto curSize = stream_size();
	if (curSize == 0)
		return false;
	stream_index_t maxPos = curSize - 1;

	switch (ref)
	{
	default:break;
	case ang::streams::stream_reference::begin:
		if (size < 0)
			return false;
		else
		{
			if (size > maxPos)
				return _buffer->realloc_buffer((wsize)size);
			else
				return true;
		}
	case ang::streams::stream_reference::current:
		if (size < 0)
		{
			if ((_cursor - size) <= 0)
				return false;
			else
				return true;
		}
		else
		{
			if ((_cursor + size) > maxPos)
				return _buffer->realloc_buffer((wsize)(_cursor + size));
			else
				return true;
		}
	case ang::streams::stream_reference::end:
		if (size > 0)
			return _buffer->realloc_buffer((wsize)(maxPos + size));
		else
		{
			if ((maxPos + size) < 0)
				return false;
			else
				return true;
		}
	}
	return false;
}

bool text_buffer_output_stream::can_forward(stream_index_t size)
{
	stream_index_t maxPos = stream_size();
	if ((_cursor + size) > maxPos)
		return _buffer->realloc_buffer((uint)(_cursor + size));
	else
		return true;
	return false;
}

bool text_buffer_output_stream::can_backward(stream_index_t size)
{
	stream_index_t maxPos = stream_size();
	if ((_cursor - size) <= 0)
		return false;
	else
		return true;
}


pointer text_buffer_output_stream::pointer_at(stream_index_t idx)
{
	return pointer(wsize(_buffer->buffer_ptr()) + idx);
}

wsize text_buffer_output_stream::write(pointer ptr, wsize sz)
{
	return 0;
}

bool text_buffer_output_stream::write(wchar value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;

	switch (_format)
	{
	case text::encoding::ascii: {
		if (!can_forward(sizeof(char)))
			return false;
		
		pointer buffer = _buffer->map_buffer((uint)position(), sizeof(char));
		auto text = reinterpret_cast<char*>(buffer);
		text[0] = (char)(byte)value;
		forward(buffer, sizeof(char));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		text[0] = value;
		forward(buffer, sizeof(wchar));
	}break;
	case text::encoding::utf_8: {
		if (!can_forward(4))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), sizeof(uint));
		mbyte& mb = *reinterpret_cast<mbyte*>(buffer);
		mb = value;
		forward(buffer, mb.size());
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(char value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;

	switch (_format)
	{
	case text::encoding::ascii: {
		if (!can_forward(sizeof(char)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), sizeof(char));
		auto text = reinterpret_cast<char*>(buffer);
		text[0] = value;
		forward(buffer, sizeof(char));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		text[0] = (wchar)(byte)value;
		forward(buffer, sizeof(wchar));
	}break;
	case text::encoding::utf_8: {
		if (!can_forward(2))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), sizeof(wchar));
		mbyte& mb = *reinterpret_cast<mbyte*>(buffer);
		mb = value;
		forward(buffer, mb.size());
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(byte value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;

	switch (_format)
	{
	case text::encoding::ascii: {
		if (!can_forward(sizeof(char)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), sizeof(char));
		auto text = reinterpret_cast<char*>(buffer);
		text[0] = value;
		forward(buffer, sizeof(char));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		text[0] = value;
		forward(buffer, sizeof(wchar));
	}break;
	case text::encoding::utf_8: {
		if (!can_forward(2))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), sizeof(wchar));
		mbyte& mb = *reinterpret_cast<mbyte*>(buffer);
		mb = (wchar)value;
		forward(buffer, mb.size());
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(short value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	string str = ang::move(interger::to_string(value, signed_format));
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		if (!can_forward(str->length()))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length());
		auto text = reinterpret_cast<char*>(buffer);	
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(str->length() * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length() * sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(ushort value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	string str = ang::move(uinterger::to_string(value, unsigned_format));
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		if (!can_forward(str->length()))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length());
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(str->length() * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length() * sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(int value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	string str = ang::move(interger::to_string(value, signed_format));
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		if (!can_forward(str->length()))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length());
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(str->length() * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length() * sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(uint value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	string str = ang::move(uinterger::to_string(value, unsigned_format));
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		if (!can_forward(str->length()))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length());
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(str->length() * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length() * sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(long value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	string str = ang::move(interger::to_string(value, signed_format));
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		if (!can_forward(str->length()))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length());
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(str->length() * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length() * sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(ulong value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	string str = ang::move(uinterger::to_string(value, unsigned_format));
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		if (!can_forward(str->length()))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length());
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(str->length() * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length() * sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(long64 value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	string str = ang::move(interger64::to_string(value, signed_format));
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		if (!can_forward(str->length()))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length());
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(str->length() * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length() * sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(ulong64 value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	string str = ang::move(uinterger64::to_string(value, unsigned_format));
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		if (!can_forward(str->length()))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length());
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(str->length() * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length() * sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(float value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	string str = ang::move(floating::to_string(value, floating_format));
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		if (!can_forward(str->length()))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length());
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(str->length() * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length() * sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(double value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	string str = ang::move(floating64::to_string(value, floating_format));
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		if (!can_forward(str->length()))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length());
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	case text::encoding::unicode: {
		if (!can_forward(str->length() * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), str->length());
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, (cstr_t)str));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(cwstr_t value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	switch (_format)
	{
	case text::encoding::utf_8:
	{
		uint _size = strings::algorithms::string_size(value, text::encoding::utf_8);
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<mchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, value));
	}break;
	case text::encoding::ascii: {
		uint _size = value.size();
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, value));
	}break;

	case text::encoding::unicode: {
		uint _size = value.size() * sizeof(wchar);
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, value));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(cstr_t value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	switch (_format)
	{
	case text::encoding::utf_8:
	{
		uint _size = strings::algorithms::string_size(value, text::encoding::utf_8);
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<mchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, value));
	}break;
	case text::encoding::ascii: {
		uint _size = value.size();
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, value));
	}break;

	case text::encoding::unicode: {
		uint _size = value.size() * sizeof(wchar);
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, value));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write(cmstr_t value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	switch (_format)
	{
	case text::encoding::utf_8:
	{
		uint _size = value.size();
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<mchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, value));
	}break;
	case text::encoding::ascii: {
		uint _size = strings::algorithms::string_size(value, text::encoding::ascii);
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<char*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, value));
	}break;

	case text::encoding::unicode: {
		uint _size = strings::algorithms::string_size(value, text::encoding::unicode);
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<wchar*>(buffer);
		forward(buffer, strings::algorithms::string_copy(text, value));
	}break;
	default:return false;
	}
	return true;
}


bool text_buffer_output_stream::write_line(cwstr_t value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;

	switch (_format)
	{
	case text::encoding::utf_8:
	{
		uint _size = strings::algorithms::string_size(value, text::encoding::utf_8) + 1;
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<mchar*>(buffer);
		auto s = strings::algorithms::string_copy(text, value);
		text = &text[s];
		forward(buffer, strings::algorithms::string_copy(text, "\n") + s);
	}break;
	case text::encoding::ascii: {
		if (!can_forward(value.size() + 1))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), value.size() + 1);
		auto text = reinterpret_cast<char*>(buffer);
		auto s = strings::algorithms::string_copy(text, value);
		text = &text[s];
		forward(buffer, strings::algorithms::string_copy(text, "\n") + s);
	}break;

	case text::encoding::unicode: {
		if (!can_forward((value.size() + 1) * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), (value.size() + 1) * sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		auto s = strings::algorithms::string_copy(text, value);
		text = &text[s];
		forward(buffer, strings::algorithms::string_copy(text, "\n") + s);
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write_line(cstr_t value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	switch (_format)
	{
	case text::encoding::utf_8:
	{
		uint _size = strings::algorithms::string_size(value, text::encoding::utf_8) + 1;
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<mchar*>(buffer);
		auto s = strings::algorithms::string_copy(text, value);
		text = &text[s];
		forward(buffer, strings::algorithms::string_copy(text, "\n") + s);
	}break;
	case text::encoding::ascii: {
		if (!can_forward(value.size() + 1))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), value.size() + 1);
		auto text = reinterpret_cast<char*>(buffer);
		auto s = strings::algorithms::string_copy(text, value);
		text = &text[s];
		forward(buffer, strings::algorithms::string_copy(text, "\n") + s);
	}break;

	case text::encoding::unicode: {
		if (!can_forward((value.size() + 1) * sizeof(wchar)))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), (value.size() + 1) * sizeof(wchar));
		auto text = reinterpret_cast<wchar*>(buffer);
		auto s = strings::algorithms::string_copy(text, value);
		text = &text[s];
		forward(buffer, strings::algorithms::string_copy(text, "\n") + s);
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_output_stream::write_line(cmstr_t value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	switch (_format)
	{
	case text::encoding::utf_8:
	{
		if (!can_forward(value.size() + 1))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), value.size() + 1);
		auto text = reinterpret_cast<mchar*>(buffer);
		auto s = strings::algorithms::string_copy(text, value);
		text = &text[s];
		forward(buffer, strings::algorithms::string_copy(text, "\n") + s);
	}break;
	case text::encoding::ascii: {
		uint _size = strings::algorithms::string_size(value, text::encoding::ascii) + 1;
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<char*>(buffer);
		auto s = strings::algorithms::string_copy(text, value);
		text = &text[s];
		forward(buffer, strings::algorithms::string_copy(text, "\n") + s);
	}break;

	case text::encoding::unicode: {
		uint _size = strings::algorithms::string_size(value, text::encoding::unicode) + 2;
		if (!can_forward(_size))
			return false;
		pointer buffer = _buffer->map_buffer((uint)position(), _size);
		auto text = reinterpret_cast<wchar*>(buffer);
		auto s = strings::algorithms::string_copy(text, value);
		text = &text[s];
		forward(buffer, strings::algorithms::string_copy(text, "\n") + s);
	}break;
	default:return false;
	}
	return true;
}

