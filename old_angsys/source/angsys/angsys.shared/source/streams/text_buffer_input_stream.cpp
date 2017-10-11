/*********************************************************************************************************************/
/*   File Name: text_buffer_input_stream.cpp                                                                         */
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

text_buffer_input_stream::text_buffer_input_stream()
	: _buffer(null)
	, _cursor(0)
	, _format(text::encoding::unknown)
{
}

text_buffer_input_stream::text_buffer_input_stream(text_buffer_input_stream* stream)
	: text_buffer_input_stream()
{
	if (stream)
	{
		attach(stream->_buffer, stream->_format);
	}
}

text_buffer_input_stream::text_buffer_input_stream(text::itext_buffer* buff)
	: text_buffer_input_stream()
{
	attach(buff);
}

text_buffer_input_stream::text_buffer_input_stream(ibuffer_t buff, text::encoding_t f)
	: text_buffer_input_stream()
{
	attach(buff, buff ? f.get() : text::encoding::unknown);
}

text_buffer_input_stream::~text_buffer_input_stream()
{
	_buffer = null;
	_cursor = 0;
	_format = text::encoding::unknown;
}

ANG_IMPLEMENT_CLASSNAME(ang::streams::text_buffer_input_stream);
ANG_IMPLEMENT_OBJECTNAME(ang::streams::text_buffer_input_stream);

bool text_buffer_input_stream::is_kind_of(type_name_t type)const
{
	return (type == type_name<text_buffer_input_stream>())
		|| object::is_kind_of(type)
		|| (type == type_name<itext_input_stream>());
}

bool text_buffer_input_stream::is_child_of(type_name_t type)
{
	return (type == type_name<text_buffer_input_stream>())
		|| object::is_child_of(type)
		|| (type == type_name<itext_input_stream>());
}

bool text_buffer_input_stream::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_name<text_buffer_input_stream>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_name<itext_input_stream>())
	{
		*out = static_cast<itext_input_stream*>(this);
		return true;
	}
	else if (type == type_name<ibuffer>())
	{
		*out = buffer();
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
	return _buffer.get() ? (_format.get() == text::encoding::unicode)? _buffer->buffer_size() *2: _buffer->buffer_size() : 0U;
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


text::encoding_t text_buffer_input_stream::text_encoding()const
{
	return _format;
}

ibuffer* text_buffer_input_stream::buffer()const
{
	return _buffer.get();
}

bool text_buffer_input_stream::attach(text::itext_buffer* buff)
{
	_buffer = buff;
	_format = buff ? buff->encoding().get() : text::encoding::unknown;
	_cursor = 0;

	if (buff)
	{
		uint bom;
		switch (buff->encoding().get())
		{
		case text::encoding::utf_8:
			bom = 0X00FFFFFF& *(uint*)buff->buffer_ptr();
			if (bom == mbyte::mbom || bom == mbyte::inv_mbom)
				_cursor += 3;
			break;
		case text::encoding::utf_16:
		case text::encoding::unicode:
			bom = 0X0000FFFF & *(uint*)buff->buffer_ptr();
			if (bom == mbyte::ubom || bom == mbyte::inv_ubom)
				_cursor += 3;
			break;
		}
	}

	return true;
}

bool text_buffer_input_stream::attach(ibuffer* buff, text::encoding_t format)
{
	_buffer = buff;
	_format = buff ? format.get() : text::encoding::unknown;
	_cursor = 0;
	if (buff)
	{
		uint bom;
		switch (format.get())
		{
		case text::encoding::utf_8:
			bom = 0X00FFFFFF & *(uint*)buff->buffer_ptr();
			if (bom == mbyte::mbom || bom == mbyte::inv_mbom)
				_cursor += 3;
			break;
		case text::encoding::utf_16:
		case text::encoding::unicode:
			bom = 0X0000FFFF & *(uint*)buff->buffer_ptr();
			if (bom == mbyte::ubom || bom == mbyte::inv_ubom)
				_cursor += 2;
			break;
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

bool text_buffer_input_stream::read(wchar& value)
{
	if (!is_valid() || (position()>= stream_size()))
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		value = (wchar)(byte)text[0];
		forward(sizeof(char));
	}break;
	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		value = text[0];
		forward(sizeof(wchar));
	}break;
	case text::encoding::utf_8: {
		mbyte mb;
		mb.value = *reinterpret_cast<uint*>(buffer);
		value = mb.convert();
		forward(mb.size());
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(char& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		value = text[0];
		forward(sizeof(char));
	}break;
	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		value = (char)(byte)text[0];
		forward(sizeof(wchar));
	}break;
	case text::encoding::utf_8: {
		mbyte mb;
		mb.value = *reinterpret_cast<uint*>(buffer);
		value = (char)(byte)mb.convert();
		forward(mb.size());
	}break;
	default:return false;
	}
	return true;
}

bool text_buffer_input_stream::read(byte& value)
{
	if (position() >= stream_size())
		return false;
	pointer buffer = pointer(wsize(_buffer->buffer_ptr()) + position());

	switch (_format)
	{
	case text::encoding::ascii: {
		auto text = reinterpret_cast<char*>(buffer);
		value = text[0];
		forward(sizeof(char));
	}break;
	case text::encoding::unicode: {
		auto text = reinterpret_cast<wchar*>(buffer);
		value = (byte)text[0];
		forward(sizeof(wchar));
	}break;
	case text::encoding::utf_8: {
		mbyte mb;
		mb.value = *reinterpret_cast<uint*>(buffer);
		value = (byte)mb.convert();
		forward(mb.size());
	}break;
	default:return false;
	}
	return true;
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
