/*********************************************************************************************************************/
/*   File Name: text_buffer_output_stream.cpp                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/streams.hpp"
#include "string_helper.hpp"
#include "format_parser.h"

using namespace ang;
using namespace ang::streams;



/////////////////////////////////////////////////////////////////////

text_buffer_output_stream::text_buffer_output_stream()
	: _buffer(null)
	, _cursor(0)
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
	}
}

text_buffer_output_stream::text_buffer_output_stream(text::itext_buffer_t buff)
	: text_buffer_output_stream()
{
	_buffer = buff;

}

text_buffer_output_stream::~text_buffer_output_stream()
{
	_buffer = null;
	_cursor = 0;
}

ANG_IMPLEMENT_CLASSNAME(ang::streams::text_buffer_output_stream);
ANG_IMPLEMENT_OBJECTNAME(ang::streams::text_buffer_output_stream);

bool text_buffer_output_stream::is_kind_of(type_name_t type)const
{
	return (type == type_of<text_buffer_output_stream>())
		|| object::is_kind_of(type)
		|| itext_output_stream::is_kind_of(type);
}

bool text_buffer_output_stream::is_inherited_of(type_name_t type)
{
	return (type == type_of<text_buffer_output_stream>())
		|| object::is_inherited_of(type)
		|| itext_output_stream::is_inherited_of(type);
}

bool text_buffer_output_stream::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<text_buffer_output_stream>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (text_buffer_output_stream::query_object(type, out))
	{
		return true;
	}
	else if (type == type_of<ibuffer>())
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

void text_buffer_output_stream::enable_text_formating(bool value)
{
	_is_formating_enabled = value;
}

bool text_buffer_output_stream::is_text_formating_enabled()const
{
	return _is_formating_enabled;
}

void text_buffer_output_stream::text_formating(cstr_t format)
{
	text_formating(text::text_format(format));
}

void text_buffer_output_stream::text_formating(text::text_format_t format)
{
	switch (format.format_target())
	{
	case text::text_format::character:
		char_format = format;
		break;
	case text::text_format::text:
		string_format = format;
		break;
	case text::text_format::signed_integer:
		signed_format = format;
		break;
	case text::text_format::usigned_integer:
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

bool text_buffer_output_stream::forward(ibuffer_view_t view, stream_index_t size)
{
	stream_index_t maxPos = stream_size();

	if ((_cursor + size) > maxPos)
			_cursor = maxPos;
	else
		_cursor += size;

	_buffer->unmap_buffer(view, (wsize)size);

	return true;
}

bool text_buffer_output_stream::backward(ibuffer_view_t view, stream_index_t size)
{
	auto curSize = stream_size();
	stream_index_t maxPos = curSize - 1L;

	if ((_cursor - size) <= 0)
		_cursor = 0;
	else
		_cursor -= size;

	return true;
}


text::encoding_t text_buffer_output_stream::format()const
{
	return _buffer.is_empty() ? text::encoding::none : _buffer->encoding().get();
}

text::itext_buffer* text_buffer_output_stream::buffer()const
{
	return _buffer.get();
}

bool text_buffer_output_stream::attach(text::itext_buffer_t buff)
{
	_buffer = buff;
	_cursor = 0;

	if (buff)
	{
		text::encoder_interface::initialize_interface(&encoder, buff->encoding());
		switch (buff->encoding().get())
		{
		case text::encoding::utf8: _cursor += load_bom< text::encoding::utf8>(buff->buffer_ptr()); break;
		case text::encoding::utf32: _cursor += load_bom< text::encoding::utf32>(buff->buffer_ptr()); break;
		case text::encoding::utf32_se: _cursor += load_bom< text::encoding::utf32_se>(buff->buffer_ptr()); break;
		case text::encoding::utf32_le: _cursor += load_bom< text::encoding::utf32_le>(buff->buffer_ptr()); break;
		case text::encoding::utf32_be: _cursor += load_bom< text::encoding::utf32_be>(buff->buffer_ptr()); break;
		case text::encoding::utf16: _cursor += load_bom< text::encoding::utf16>(buff->buffer_ptr()); break;
		case text::encoding::utf16_se: _cursor += load_bom< text::encoding::utf16_se>(buff->buffer_ptr()); break;
		case text::encoding::utf16_le: _cursor += load_bom< text::encoding::utf16_le>(buff->buffer_ptr()); break;
		case text::encoding::utf16_be: _cursor += load_bom< text::encoding::utf16_be>(buff->buffer_ptr()); break;
		case text::encoding::unicode: _cursor += load_bom< text::encoding::unicode>(buff->buffer_ptr()); break;
		}
	}

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

wsize text_buffer_output_stream::write(pointer ptr, wsize sz, text::text_format_t format)
{
	if (!is_valid() || (position() >= stream_size()))
		return false;

	alignas(4) stack_array<char, 480> _buff;
	array_view<char32_t> _char(120, (char32_t*)_buff.get());

	raw_str_t dest(_buff.get(), _buff.size(), encoder._format());
	wsize i = 0, cs = dest.char_size();
	
	ibuffer_view_t view = null;

	switch (format.format_target())
	{
	case text::text_format::character:
		switch (sz) {
		case 1: _char[0] = text::ASCII().to_utf32((char const*)ptr, i); break;//ascii - utf8
		case 2: _char[0] = text::UTF16().to_utf32((char16_t const*)ptr, i); break; //utf16
		case 4: _char[0] = text::UTF32().to_utf32((char32_t const*)ptr, i); break; //utf32
		default: return 0;
		}
		_char[1] = 0;
		i = encoder._size(_char.get(), text::encoding::utf32);
		if (!can_forward(i * cs))
			return false;
		view = _buffer->map_buffer((windex)position(), i * cs);
		i = encoder._convert_string(view->buffer_ptr(), view->buffer_size(), _char, i, text::encoding::utf32, false) * cs;
		forward(view, i);
		break;

	case text::text_format::signed_integer: 
		if (is_text_formating_enabled()) format = signed_format;
		switch (sz) {
		case 1: dest = encoder._integer_to_string(*(char*)ptr, dest.ptr(), dest.size() / cs, format); break;
		case 2: dest = encoder._integer_to_string(*(short*)ptr, dest.ptr(), dest.size() / cs, format); break;
		case 4: dest = encoder._integer_to_string(*(int*)ptr, dest.ptr(), dest.size() / cs, format); break;
		case 8: dest = encoder._integer_to_string(*(long64*)ptr, dest.ptr(), dest.size() / cs, format); break;
		}

		if (!can_forward(dest.size()))
			return false;

		view = _buffer->map_buffer((windex)position(), i * cs);
		memcpy(view->buffer_ptr(), dest.ptr(), dest.size());	
		forward(view, dest.size());
		break;

	case text::text_format::usigned_integer:
		if (is_text_formating_enabled()) format = unsigned_format;
		switch (sz) {
		case 1: dest = encoder._uinteger_to_string(*(uchar*)ptr, dest.ptr(), dest.size() / cs, format); break;
		case 2: dest = encoder._uinteger_to_string(*(ushort*)ptr, dest.ptr(), dest.size() / cs, format); break;
		case 4: dest = encoder._uinteger_to_string(*(uint*)ptr, dest.ptr(), dest.size() / cs, format); break;
		case 8: dest = encoder._uinteger_to_string(*(ulong64*)ptr, dest.ptr(), dest.size() / cs, format); break;
		}

		if (!can_forward(dest.size()))
			return false;

		view = _buffer->map_buffer((windex)position(), i * cs);
		memcpy(view->buffer_ptr(), dest.ptr(), dest.size());
		forward(view, dest.size());
		break;

	case text::text_format::floating: 
		if (is_text_formating_enabled()) format = floating_format;
		switch (sz) {
		case 4: dest = encoder._floating_to_string(*(float*)ptr, dest.ptr(), dest.size() / cs, format); break;
		case 8: dest = encoder._floating_to_string(*(double*)ptr, dest.ptr(), dest.size() / cs, format); break;
		}

		if (!can_forward(dest.size()))
			return false;

		view = _buffer->map_buffer((windex)position(), i * cs);
		memcpy(view->buffer_ptr(), dest.ptr(), dest.size());
		forward(view, dest.size());
		break;
	}

	return i;
}


wsize text_buffer_output_stream::write(raw_str_t value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	wsize _size = encoder._size(value.ptr(), value.encoding())
		, cs = raw_str(0, 0, encoder._format()).char_size();

	if (!can_forward(_size * cs))
		return false;
	ibuffer_view_t view = _buffer->map_buffer((windex)position(), _size * cs);
	_size = encoder._convert_string(view->buffer_ptr(), _size, value.ptr(), _size, value.encoding(), false);
	forward(view, _size * cs);
	return _size * cs;
}

wsize text_buffer_output_stream::write_line(raw_str_t value)
{
	if (!is_valid() || (position() >= stream_size() && !_buffer->can_realloc_buffer()))
		return false;
	char _endl[2] = "\n";
	wsize _size = encoder._size(value.ptr(), value.encoding()) + 2
		, cs = raw_str(0, 0, encoder._format()).char_size(), i = 0;

	if (!can_forward(_size * cs))
		return false;
	ibuffer_view_t view = _buffer->map_buffer((windex)position(), _size * cs);
	i = encoder._convert_string(view->buffer_ptr(), _size, value.ptr(), i, value.encoding(), false);
	encoder._convert_string((byte*)view->buffer_ptr() + (i * cs), _size - i, _endl, i, text::encoding::ascii, false);
	forward(view, i * cs);
	return i * cs;
}


