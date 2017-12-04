/*********************************************************************************************************************/
/*   File Name: text_buffer_input_stream.cpp                                                                         */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/streams.hpp"
#include "string_helper.hpp"

using namespace ang;
using namespace ang::streams;

text_buffer_input_stream::text_buffer_input_stream()
	: _buffer(null)
	, _cursor(0)
{
}

text_buffer_input_stream::text_buffer_input_stream(text_buffer_input_stream const* stream)
	: text_buffer_input_stream()
{
	if (stream)
	{
		attach(stream->_buffer.get());
	}
}

text_buffer_input_stream::text_buffer_input_stream(text::itext_buffer_t buff)
	: text_buffer_input_stream()
{
	attach(buff);
}

text_buffer_input_stream::~text_buffer_input_stream()
{
	_buffer = null;
	_cursor = 0;
}

ANG_IMPLEMENT_CLASSNAME(ang::streams::text_buffer_input_stream);
ANG_IMPLEMENT_OBJECTNAME(ang::streams::text_buffer_input_stream);

bool text_buffer_input_stream::is_kind_of(type_name_t type)const
{
	return (type == type_of<text_buffer_input_stream>())
		|| object::is_kind_of(type)
		|| (type == type_of<itext_input_stream>());
}

bool text_buffer_input_stream::is_inherited_of(type_name_t type)
{
	return type == type_of<text_buffer_input_stream>()
		|| object::is_inherited_of(type)
		|| itext_input_stream::is_inherited_of(type);
}

bool text_buffer_input_stream::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<text_buffer_input_stream>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (itext_input_stream::query_object(type, out))
	{
		return true;
	}
	else if (!_buffer.is_empty() && _buffer->query_object(type, out))
	{
		return true;
	}
	return false;
}

bool text_buffer_input_stream::is_valid()const
{
	return _buffer.get() != null;
}

stream_index_t text_buffer_input_stream::position()const
{
	return _cursor;
}

stream_size_t text_buffer_input_stream::stream_size()const
{
	return _buffer.get() ? _buffer->buffer_size() : 0U;
}

bool text_buffer_input_stream::end_of_stream()const
{
	return _buffer.get() ? _cursor >= (stream_index_t)_buffer->buffer_size() : true;
}

bool text_buffer_input_stream::move_to(stream_index_t size, stream_reference_t ref)
{
	auto curSize = stream_size();
	if (curSize == 0)
		return false;

	stream_index_t maxPos = curSize - 1;

	switch (ref)
	{
	default:break;
	case ang::streams::stream_reference::begin:
		_cursor = min(max(size, 0LL), maxPos);
		break;
	case ang::streams::stream_reference::current:
		_cursor = min(max(_cursor + size, 0LL), maxPos);
		break;
	case ang::streams::stream_reference::end:
		_cursor = min(max(maxPos + size, 0LL), maxPos);
		break;
	}

	return true;
}

bool text_buffer_input_stream::forward(stream_index_t size)
{
	auto curSize = stream_size();
	stream_index_t maxPos = curSize - 1L;

	_cursor = min(_cursor + size, maxPos);

	return true;
}

bool text_buffer_input_stream::backward(stream_index_t size)
{
	auto curSize = stream_size();
	stream_index_t maxPos = curSize - 1L;

	if ((_cursor - size) <= 0)
		_cursor = 0;
	else
		_cursor -= size;

	return true;
}


text::encoding_t text_buffer_input_stream::format()const
{
	return !_buffer.is_empty() ? _buffer->encoding().get() : text::encoding::none;
}

ibuffer* text_buffer_input_stream::buffer()const
{
	return _buffer.get();
}

//static xml_encoding_t  xml_detect_encoding(raw_str_t text, windex& idx)
//{
//	if (text.size() < 4) return xml_encoding::none;
//
//	alignas(4) static byte utf8_bom[4] = { 0xef, 0xbb, 0xbf, 0x0 };
//	alignas(4) static byte utf16_le_bom[4] = { 0xff, 0xfe, 0x0, 0x0 };
//	alignas(4) static byte utf16_be_bom[4] = { 0xfe, 0xff, 0x0, 0x0 };
//	alignas(4) static byte utf32_le_bom[8] = { 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
//	alignas(4) static byte utf32_be_bom[8] = { 0x0, 0x0, 0xfe, 0xff, 0x0, 0x0, 0x0, 0x0 };
//
//	switch (text.encoding())
//	{
//	case text::encoding::auto_detect:
//		if (text::UTF8().compare_until((utf8_char_t const*)text.ptr(), (utf8_char_t const*)utf8_bom) == 3)
//		{
//			idx += 3;
//			return text::encoding::utf8;
//		}
//		else if (text::UTF32().compare_until((utf32_char_t const*)text.ptr(), (utf32_char_t const*)utf32_le_bom) == 1)
//		{
//			idx += 1;
//			return text::native_encoding<text::encoding::utf32_le>();
//		}
//		else if (text::UTF32().compare_until((utf32_char_t const*)text.ptr(), (utf32_char_t const*)utf32_be_bom) == 1)
//		{
//			idx += 1;
//			return text::native_encoding<text::encoding::utf16_be>();
//		}
//		else if (text::UTF16().compare_until((utf16_char_t const*)text.ptr(), (utf16_char_t const*)utf16_le_bom) == 1)
//		{
//			idx += 1;
//			return text::native_encoding<text::encoding::utf16_le>();
//		}
//		else if (text::UTF16().compare_until((utf16_char_t const*)text.ptr(), (utf16_char_t const*)utf16_be_bom) == 1)
//		{
//			idx += 1;
//			return text::native_encoding<text::encoding::utf16_be>();
//		}
//
//		else if (text::UTF8().compare_until((utf8_char_t const*)text.ptr(), u8"<?xml"_sm.cstr()) == 5)
//			return text::encoding::utf8;
//		else if (text::UTF32().compare_until((utf32_char_t const*)text.ptr(), U"<?xml") == 5)
//			return text::encoding::utf32;
//		else if (text::UTF32_SE().compare_until((utf32_char_t const*)text.ptr(), U"<?xml") == 5)
//			return text::encoding::utf32_se;
//		else if (text::UTF16().compare_until((utf16_char_t const*)text.ptr(), u"<?xml") == 5)
//			return text::encoding::utf16;
//		else if (text::UTF16_SE().compare_until((utf16_char_t const*)text.ptr(), u"<?xml") == 5)
//			return text::encoding::utf16_se;
//
//		return text::encoding::utf8;
//
//	case text::encoding::utf16_le: return text::native_encoding<text::encoding::utf16_le>();
//	case text::encoding::utf16_be: return text::native_encoding<text::encoding::utf16_be>();
//	case text::encoding::utf32_le: return text::native_encoding<text::encoding::utf32_le>();
//	case text::encoding::utf32_be: return text::native_encoding<text::encoding::utf32_be>();
//	default:
//		return text.encoding();
//	}
//}


bool text_buffer_input_stream::attach(text::itext_buffer_t buff)
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

bool text_buffer_input_stream::can_move_to(stream_index_t size, stream_reference_t ref)
{
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
				return false;
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
				return false;
			else
				return true;
		}
	case ang::streams::stream_reference::end:
		if (size > 0)
			return false;
		else
		{
			if (-size >= maxPos)
				return false;
			else
				return true;
		}
	}
	return false;
}

bool text_buffer_input_stream::can_forward(stream_index_t size)
{
	stream_index_t maxPos = stream_size() - 1;
	if ((_cursor + size) > maxPos)
		return false;
	else
		return true;
	return false;
}

bool text_buffer_input_stream::can_backward(stream_index_t size)
{
	stream_index_t maxPos = stream_size() - 1;
	if ((_cursor - size) <= 0)
		return false;
	else
		return true;
}


pointer text_buffer_input_stream::pointer_at(stream_index_t idx)
{
	return pointer(wsize(_buffer->buffer_ptr()) + idx);
}

wsize text_buffer_input_stream::read(pointer ptr, wsize sz)
{
	return 0;
}

wsize text_buffer_input_stream::read(pointer ptr, wsize sz, text::text_format_t format)
{
	if (!is_valid() || (position() >= stream_size()))
		return false;
	stream_index_t last_pos = position();
	raw_str buffer = raw_str(pointer(wsize(_buffer->buffer_ptr()) + last_pos), stream_size() - last_pos, this->format());
	windex i = 0, j= 0;

	switch (format.format_target())
	{
	case text::text_format::character:
		switch (sz) {
		case 1: forward(text::ASCII().from_utf32(encoder._to_utf32(buffer.ptr(), i), (char*)ptr, j)); break;//ascii - utf8
		case 2: forward(text::UTF16().from_utf32(encoder._to_utf32(buffer.ptr(), i), (char16_t*)ptr, j)); break; //utf16
		case 4: forward(text::UTF32().from_utf32(encoder._to_utf32(buffer.ptr(), i), (char32_t*)ptr, j)); break; //utf32
		default: return 0;
		}

	case text::text_format::signed_integer: {
		long64 value = str_to_integer(buffer, i);
		switch (sz) {
		case 1: *(char*)ptr = (char)value; break;
		case 2: *(short*)ptr = (short)value; break;
		case 4: *(int*)ptr = (int)value; break;
		case 8: *(long64*)ptr = value; break;
		}
		forward(i * buffer.char_size());
	}	break;
		
	case text::text_format::usigned_integer: {
		ulong64 value = str_to_uinteger(buffer, i);
		switch (sz) {
		case 1: *(uchar*)ptr = (uchar)value; break;
		case 2: *(ushort*)ptr = (ushort)value; break;
		case 4: *(uint*)ptr = (uint)value; break;
		case 8: *(ulong64*)ptr = value; break;
		}
		forward(i * buffer.char_size());
	}	break;

	case text::text_format::floating: {
		double value = str_to_float(buffer, i);
		switch (sz) {
		case 4: *(float*)ptr = (float)value; break;
		case 8: *(double*)ptr = value; break;
		}
		forward(i * buffer.char_size());
	}	break;

	}

	return position() - last_pos;
}

wsize text_buffer_input_stream::seek(raw_str_t cstr)
{
	if (position() >= stream_size())
		return false;
	stream_index_t last_pos = position();
	raw_str buffer = raw_str(pointer(wsize(_buffer->buffer_ptr()) + last_pos), stream_size() - last_pos, this->format());
	forward(encoder._compare_string_until(buffer.ptr(), cstr.ptr(), cstr.encoding()) * buffer.char_size());
	return position() - last_pos;
}

wsize text_buffer_input_stream::read(ibuffer_view_t out, text::encoding_t encoding, wsize max)
{
	if (position() >= stream_size())
		return false;
	windex i = 0, c = 0; char32_t val, val2; bool b = false;
	array_view<const char32_t> end = U" \n\r\t";
	stream_index_t last_pos = position();
	raw_str buffer = raw_str(pointer(wsize(_buffer->buffer_ptr()) + last_pos), stream_size() - last_pos, this->format());

	text::encoder_interface _encoder;
	text::encoder_interface::initialize_interface(&_encoder, encoding);

LOOP:
	if (i >= max) goto END;
	val = encoder._to_utf32(buffer.ptr(), i);
	c = 0;
LOOP2:
	if (c >= end.size()) goto LOOP;
	if (val == end[c]) goto END;
	c++;
	goto LOOP2;
END:
	_encoder._convert_string(out->buffer_ptr(), max, buffer.ptr(), i, buffer.encoding(), true);
	return i * buffer.char_size();
} 

wsize text_buffer_input_stream::read_line(ibuffer_view_t out, text::encoding_t encoding, wsize max, array_view<const char32_t> end)
{
	if (position() >= stream_size())
		return false;
	windex i = 0, c = 0; char32_t val, val2; bool b = false;
	if(end.size() == 0)
		end = U"\n\r";
	stream_index_t last_pos = position();
	raw_str buffer = raw_str(pointer(wsize(_buffer->buffer_ptr()) + last_pos), stream_size() - last_pos, this->format());

	text::encoder_interface _encoder;
	text::encoder_interface::initialize_interface(&_encoder, encoding);

LOOP:
	if (i >= max) goto END;
	val = encoder._to_utf32(buffer.ptr(), i);
	c = 0;
LOOP2:
	if (c >= end.size()) goto LOOP;
	if (val == end[c]) goto END;
	c++;
	goto LOOP2;
END:
	_encoder._convert_string(out->buffer_ptr(), max, buffer.ptr(), i, buffer.encoding(), true);
	return i * buffer.char_size();
}
