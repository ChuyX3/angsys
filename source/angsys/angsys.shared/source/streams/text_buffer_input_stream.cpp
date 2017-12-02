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
	return _buffer.is_empty() ? _buffer->encoding().get() : text::encoding::none;
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
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());
	windex i = 0, j= 0;

	switch (format.format_target())
	{
	case text::text_format::character:
		switch (sz)
		{
		case 1: //ascii - utf8
			forward(text::ASCII().from_utf32(encoder._to_utf32(buffer, i), (char*)ptr, j));
		case 2: //utf16
			forward(text::UTF16().from_utf32(encoder._to_utf32(buffer, i), (char16_t*)ptr, j));
		case 4: //utf32
			forward(text::UTF32().from_utf32(encoder._to_utf32(buffer, i), (char32_t*)ptr, j));
		default: return 0;
		}

	case text::text_format::signed_integer:
		integer::parse();
		switch (sz)
		{
		case 1: //ascii - utf8
			forward(text::ASCII().from_utf32(encoder._to_utf32(buffer, i), (char*)ptr, j));
		case 2: //utf16
			forward(text::UTF16().from_utf32(encoder._to_utf32(buffer, i), (char16_t*)ptr, j));
		case 4: //utf32
			forward(text::UTF32().from_utf32(encoder._to_utf32(buffer, i), (char32_t*)ptr, j));
		default: return 0;
		}
	case text::text_format::usigned_integer:
	case text::text_format::floating:

		default:
			return 0;
	}

	return 0;
}

bool text_buffer_input_stream::read(short& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		char* end;
		value = (short)strtol(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		wchar* end;
		value = (short)wcstol(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(ushort& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		char* end;
		value = (ushort)strtoul(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		wchar* end;
		value = (ushort)wcstoul(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(int& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		char* end;
		value = (int)strtol(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		wchar* end;
		value = (int)wcstol(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(uint& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		char* end;
		value = (uint)strtoul(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		wchar* end;
		value = (uint)wcstoul(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(long& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		char* end;
		value = strtol(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		wchar* end;
		value = wcstol(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(ulong& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		char* end;
		value = strtoul(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		wchar* end;
		value = wcstoul(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(long64& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		char* end;
		value = strtoll(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		wchar* end;
		value = wcstoll(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(ulong64& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		char* end;
		value = strtoull(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		wchar* end;
		value = wcstoull(text, &end, 10);
		forward(wsize(end) - wsize(text));
	}break;
	default:return false;
	}
	return true;
}


bool text_buffer_input_stream::read(float& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		char* end;
		value = strtof(text, &end);
		forward(wsize(end) - wsize(text));
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		wchar* end;
		value = (float)wcstod(text, &end);
		forward(wsize(end) - wsize(text));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(double& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		char* end;
		value = strtod(text, &end);
		forward(wsize(end) - wsize(text));
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		wchar* end;
		value = wcstod(text, &end);
		forward(wsize(end) - wsize(text));
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(cstr_t cstr)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());
	int c = 0;
	switch (_format)
	{

	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		streams::stream_size_t max = stream_size() - position();		
		while ((c < max) && (text[c] != 0) && (cstr[c] != 0) && ((byte)text[c] == (byte)cstr[c]))
			c++;
		forward(c);
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		streams::stream_size_t max = stream_size() - position();
		while ((c < max) && (text[c] != 0) && (cstr[c] != 0) && (text[c] == cstr[c]))
			c++;
		forward(c * 2);
	}break;

	case text::encoding::utf_8: {
		//auto text = reinterpret_cast<MStr>(buffer);

	}break;
	default:return false;
	}
	return c;
}

bool text_buffer_input_stream::read(cwstr_t cstr)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());
	int c = 0;
	switch (_format)
	{

	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		streams::stream_size_t max = stream_size() - position();
		while ((c < max) && (text[c] != 0) && (cstr[c] != 0) && (text[c] == cstr[c]))
			c++;
		forward(c);
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		streams::stream_size_t max = stream_size() - position();
		while ((c < max) && (text[c] != 0) && (cstr[c] != 0) && ((byte)text[c] == (byte)cstr[c]))
			c++;
		forward(c * 2);
	}break;

	case text::encoding::utf_8: {
		//auto text = reinterpret_cast<MStr>(buffer);

	}break;
	default:return false;
	}
	return c;
}

bool text_buffer_input_stream::read(string& out, wsize _max)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	if (out.is_empty())
		out = new strings::string_buffer();
	static cstr_t stop = " \t\n\r";
	switch (_format)
	{
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, stream_size() - position());
		index c = 0;
		while ((c < max) && (text[c] != 0) && stop.find(text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, (stream_size() - position()) / sizeof(wchar));
		index c = 0;
		while ((c < max) && (text[c] != 0) && stop.find((char)(byte)text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::utf_8: {
		auto text = reinterpret_cast<mchar*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, stream_size() - position());
		index c = 0;
		while ((c < max) && (text[c] != 0) && (*((ushort*)&text[c]) != mbyte::inv_null) && stop.find(text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);

	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(wstring& out, wsize _max)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	if (out.is_empty())
		out = new strings::wstring_buffer();

	static cwstr_t stop = L" \t\n\r";

	switch (_format)
	{
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, stream_size() - position());
		index c = 0;
		while ((c < max) && (text[c] != 0) && stop.find(text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, (stream_size() - position()) / sizeof(wchar));
		index c = 0;
		while ((c < max) && (text[c] != 0) && stop.find((char)(byte)text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::utf_8: {
		auto text = reinterpret_cast<mchar*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, stream_size() - position());
		index c = 0;
		while ((c < max) && (text[c] != 0) && (*((ushort*)&text[c]) != mbyte::inv_null) && stop.find(text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);

	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(mstring& out, wsize _max)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	if (out.is_empty())
		out = new strings::mstring_buffer();

	static cstr_t stop = " \t\n\r";

	switch (_format)
	{
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, stream_size() - position());
		index c = 0;
		while ((c < max) && (text[c] != 0) && stop.find(text[c], 0) == invalid_index)
			c += mbyte(text[c]).size();
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, (stream_size() - position()) / sizeof(wchar));
		index c = 0;
		while ((c < max) && (text[c] != 0) && stop.find((char)(byte)text[c], 0) == invalid_index)
			c += mbyte(text[c]).size();
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::utf_8: {
		auto text = reinterpret_cast<mchar*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, stream_size() - position());
		index c = 0;
		while ((c < max) && (text[c] != 0) && (*((ushort*)&text[c]) != mbyte::inv_null) && stop.find(text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);

	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read_line(string& out, wsize _max, array<char> _stop)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	if (out.is_empty())
		out = new strings::string_buffer();
	auto stop = _stop.is_empty() ? cstr_t("\n\r") : cstr_t(_stop->data(), _stop->size());
	switch (_format)
	{
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, stream_size() - position());
		index c = 0;
		while ((c < max) && (text[c] != 0) && stop.find(text[c], 0) == invalid_index)
			c++;		
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		streams::stream_size_t max = min<ulong_t>(_max, (stream_size() - position()) / sizeof(wchar));
		streams::stream_size_t c = 0;
		while ((c < max) && (text[c] != 0) && stop.find((char)(byte)text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::utf_8: {
		auto text = reinterpret_cast<mchar*>(buffer);
		streams::stream_size_t max = min<ulong_t>(_max, stream_size() - position());
		streams::stream_size_t c = 0;
		while ((c < max) && (text[c] != 0) && (*((ushort*)&text[c]) != mbyte::inv_null) && stop.find(text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);

	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read_line(wstring& out, wsize _max, array<wchar> _stop)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	if (out.is_empty())
		out = new strings::wstring_buffer();

	auto stop = _stop.is_empty() ? cwstr_t(L"\n\r") : cwstr_t(_stop->data(), _stop->size());

	switch (_format)
	{
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, stream_size() - position());
		index c = 0;
		while ((c < max) && (text[c] != 0) && stop.find(text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, (stream_size() - position()) / sizeof(wchar));
		index c = 0;
		while ((c < max) && (text[c] != 0) && stop.find((char)(byte)text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::utf_8: {
		auto text = reinterpret_cast<mchar*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, stream_size() - position());
		index c = 0;
		while ((c < max) && (text[c] != 0) && (*((ushort*)&text[c]) != mbyte::inv_null) && stop.find(text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);

	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read_line(mstring& out, wsize _max, array<wchar> _stop)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	if (out.is_empty())
		out = new strings::mstring_buffer();

	auto stop = _stop.is_empty() ? cwstr_t(L"\n\r") : cwstr_t(_stop->data(), _stop->size());

	switch (_format)
	{
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, stream_size() - position());
		index c = 0;
		while ((c < max) && (text[c] != 0) && stop.find(text[c], 0) == invalid_index)
			c += mbyte(text[c]).size();
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, (stream_size() - position()) / sizeof(wchar));
		index c = 0;
		while ((c < max) && (text[c] != 0) && stop.find((char)(byte)text[c], 0) == invalid_index)
			c += mbyte(text[c]).size();
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);
	}break;

	case text::encoding::utf_8: {
		auto text = reinterpret_cast<mchar*>(buffer);
		streams::stream_size_t max = min<ulong>(_max, stream_size() - position());
		index c = 0;
		while ((c < max) && (text[c] != 0) && (*((ushort*)&text[c]) != mbyte::inv_null) && stop.find(text[c], 0) == invalid_index)
			c++;
		out->realloc((uint)c, false);
		strings::algorithms::string_copy(out->str(), text, (uint)c);
		out->length((uint)c);
		forward(c);

	}break;
	default:return false;
	}
	return true;
}
