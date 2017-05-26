/*********************************************************************************************************************/
/*   File Name: binary_buffer_input_stream.cpp                                                                       */
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

binary_buffer_input_stream::binary_buffer_input_stream()
	: _buffer(null)
	, _cursor(0)
{

}

binary_buffer_input_stream::binary_buffer_input_stream(binary_buffer_input_stream* stream)
	: binary_buffer_input_stream()
{
	if (stream)
		_buffer = stream->_buffer;
}

binary_buffer_input_stream::binary_buffer_input_stream(ibuffer_t buff)
	: binary_buffer_input_stream()
{
	_buffer = buff;
}

binary_buffer_input_stream::~binary_buffer_input_stream()
{
	_buffer = null;
	_cursor = 0;
}

ANG_IMPLEMENT_CLASSNAME(ang::streams::binary_buffer_input_stream);
ANG_IMPLEMENT_OBJECTNAME(ang::streams::binary_buffer_input_stream);

bool binary_buffer_input_stream::is_kind_of(type_name_t type)const
{
	return (type == type_name<binary_buffer_input_stream>())
		|| object::is_kind_of(type)
		|| (type == type_name<ibinary_input_stream>());
}

bool binary_buffer_input_stream::is_child_of(type_name_t type)
{
	return (type == type_name<binary_buffer_input_stream>())
		|| object::is_child_of(type)
		|| (type == type_name<ibinary_input_stream>());
}

bool binary_buffer_input_stream::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_name<binary_buffer_input_stream>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_name<ibinary_input_stream>())
	{
		*out = static_cast<ibinary_input_stream*>(this);
		return true;
	}
	else if (type == type_name<ibuffer>())
	{
		*out = buffer();
		return true;
	}
	return false;
}

bool binary_buffer_input_stream::is_valid()const
{
	return _buffer.get() != null;
}

stream_index_t binary_buffer_input_stream::position()const
{
	return _cursor;
}

stream_size_t binary_buffer_input_stream::stream_size()const
{
	return _buffer.get() ? _buffer->buffer_size() : 0U;
}

bool binary_buffer_input_stream::end_of_stream()const
{
	return _buffer.get() ? _cursor >= (stream_index_t)_buffer->buffer_size() : true;
}

bool binary_buffer_input_stream::move_to(stream_index_t size, stream_reference_t ref)
{
	auto curSize = stream_size();
	if (curSize == 0)
		return false;

	stream_index_t maxPos = curSize;

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


bool binary_buffer_input_stream::forward(stream_index_t size)
{
	auto curSize = stream_size();
	stream_index_t maxPos = curSize;

	_cursor = min(_cursor + size, maxPos);
	return true;
}

bool binary_buffer_input_stream::backward(stream_index_t size)
{
	auto curSize = stream_size();
	stream_index_t maxPos = curSize;

	if ((_cursor - size) <= 0)
		_cursor = 0;
	else
		_cursor -= size;

	return true;
}


ibuffer* binary_buffer_input_stream::buffer()const
{
	return _buffer.get();
}

bool binary_buffer_input_stream::attach(ibuffer* buff)
{
	_buffer = buff;
	_cursor = 0;
	return true;
}

bool binary_buffer_input_stream::can_move_to(stream_index_t size, stream_reference_t ref)
{
	auto curSize = stream_size();
	if (curSize == 0)
		return false;
	stream_index_t maxPos = curSize;

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

bool binary_buffer_input_stream::can_forward(stream_index_t size)
{
	stream_index_t maxPos = stream_size();
	if ((_cursor + size) > maxPos)
		return false;
	else
		return true;
	return false;
}

bool binary_buffer_input_stream::can_backward(stream_index_t size)
{
	stream_index_t maxPos = stream_size();
	if ((_cursor - size) <= 0)
		return false;
	else
		return true;
}

pointer binary_buffer_input_stream::pointer_at(stream_index_t idx)
{
	return pointer(wsize(_buffer->buffer_ptr()) + idx);
}

wsize binary_buffer_input_stream::read(pointer ptr, wsize sz)
{
	if (!can_forward(sz))
		return 0;
	memcpy(ptr, pointer_at(position()), sz);
	forward(sz);
	return sz;
}

bool binary_buffer_input_stream::read(wchar& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(char& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(byte& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(short& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(ushort& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(int& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(uint& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(long& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(ulong& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(long64& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(ulong64& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(float& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(double& value) { return read(&value, sizeof(value)) > 0; }

bool binary_buffer_input_stream::read(string& out)
{
	struct
	{
		uint encoding;
		uint length;
	}info;
	if (!can_forward(sizeof(info)))
		return false;
	auto ptr = (byte*)pointer_at(position());
	memcpy(&info, ptr, sizeof(info));

	switch (info.encoding)
	{
	case text::encoding::ascii:
	{
		auto max_size = min(stream_size() - position() - sizeof(info), info.length);
		if (max_size <= 0)
			return false;

		if (out.is_empty())
			out = new strings::string_buffer();

		out->realloc((uint)max_size, false);
		auto dest = out->map_buffer(0, (wsize)max_size);
		memcpy(dest, &ptr[sizeof(info)], (uint)max_size);
		out->unmap_buffer(dest, (wsize)max_size);
		forward(max_size + sizeof(info));
		return true;
	}
	case text::encoding::unicode:
	{
		auto max_size = min((stream_size() - position() - sizeof(info)) / sizeof(wchar), info.length);
		if (max_size <= 0)
			return false;

		if (out.is_empty())
			out = new strings::string_buffer();

		out->realloc((uint)max_size, false);
		auto dest = (char*)out->map_buffer(0, (wsize)max_size);
		strings::algorithms::string_copy(dest, (wchar*)&ptr[sizeof(info)], (uint)max_size);
		out->unmap_buffer(dest, (wsize)max_size);
		forward(max_size * sizeof(wchar) + sizeof(info));
		return true;
	}
	default:
		return false;
	}

}


bool binary_buffer_input_stream::read(wstring& out)
{
	struct
	{
		uint encoding;
		uint length;
	}info;
	if (!can_forward(sizeof(info)))
		return false;
	auto ptr = (byte*)pointer_at(position());
	memcpy(&info, ptr, sizeof(info));

	switch (info.encoding)
	{
	case text::encoding::unicode:
	{
		auto max_size = min((stream_size() - position() - sizeof(info)) / sizeof(wchar), info.length);
		if (max_size <= 0)
			return false;

		if (out.is_empty())
			out = new strings::wstring_buffer();

		out->realloc((uint)max_size, false);
		auto dest = out->map_buffer(0, (wsize)max_size);
		memcpy(dest, &ptr[sizeof(info)], (uint)max_size * sizeof(wchar));
		out->unmap_buffer(dest, (wsize)max_size);
		forward(max_size * sizeof(wchar) + sizeof(info));
		return true;
	}
	case text::encoding::ascii:
	{
		auto max_size = min(stream_size() - position() - sizeof(info), info.length);
		if (max_size <= 0)
			return false;

		if (out.is_empty())
			out = new strings::wstring_buffer();

		out->realloc((uint)max_size, false);
		auto dest = (wchar*)out->map_buffer(0, (wsize)max_size);
		strings::algorithms::string_copy(dest, (char*)&ptr[sizeof(info)], (uint)max_size);
		out->unmap_buffer(dest, (wsize)max_size);
		forward(max_size + sizeof(info));
		return true;
	}
	default:
		return false;
	}
}



//bool binary_buffer_output_stream::write(string value)
//{
//	cstr_t cstr = value;
//	auto sz = cstr.size() + 8;
//	if (!can_forward(sizeof(sz)))
//		return 0;
//
//	uint temp = text::encoding::ascii;
//	const_array<byte> ptr = const_array<byte>((byte*)pointer_at(position()), cstr.size() + 8);
//
//	memcpy(&ptr[0], &temp, 4);
//	temp = cstr.size();
//	memcpy(&ptr[4], &temp, 4);
//	memcpy(&ptr[8], cstr.data(), temp);
//
//	forward(sz);
//	return sz;
//}
//
//bool binary_buffer_output_stream::write(wstring value)
//{
//	cwstr_t cstr = value;
//	auto sz = cstr.size() * sizeof(wchar) + 8;
//	if (!can_forward(sizeof(sz)))
//		return 0;
//
//	uint temp = text::encoding::unicode;
//	const_array<byte> ptr = const_array<byte>((byte*)pointer_at(position()), cstr.size() + 8);
//
//	memcpy(&ptr[0], &temp, 4);
//	temp = cstr.size();
//	memcpy(&ptr[4], &temp, 4);
//	memcpy(&ptr[8], cstr.data(), temp * sizeof(wchar));
//
//	forward(sz);
//	return sz;
//}