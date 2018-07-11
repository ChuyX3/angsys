/*********************************************************************************************************************/
/*   File Name: binary_buffer_output_stream.cpp                                                                      */
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

binary_buffer_output_stream::binary_buffer_output_stream()
	: _buffer(null)
	, _cursor(0)
{

}

binary_buffer_output_stream::binary_buffer_output_stream(binary_buffer_output_stream* stream)
	: binary_buffer_output_stream()
{
	if (stream)
		_buffer = stream->_buffer;
}

binary_buffer_output_stream::binary_buffer_output_stream(ibuffer_t buff)
	: binary_buffer_output_stream()
{
	_buffer = buff;
}

binary_buffer_output_stream::~binary_buffer_output_stream()
{
	_buffer = null;
	_cursor = 0;
}

ANG_IMPLEMENT_CLASSNAME(ang::streams::binary_buffer_output_stream);
ANG_IMPLEMENT_OBJECTNAME(ang::streams::binary_buffer_output_stream);

bool binary_buffer_output_stream::is_kind_of(type_name_t type)const
{
	return (type == type_of<binary_buffer_output_stream>())
		|| object::is_kind_of(type)
		|| (type == type_of<ibinary_output_stream>());
}

bool binary_buffer_output_stream::is_inherited_of(type_name_t type)
{
	return (type == type_of<binary_buffer_output_stream>())
		|| object::is_inherited_of(type)
		|| (type == type_of<ibinary_output_stream>());
}

bool binary_buffer_output_stream::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<binary_buffer_output_stream>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_of<ibinary_output_stream>())
	{
		*out = static_cast<ibinary_output_stream*>(this);
		return true;
	}
	else if (type == type_of<ibuffer>())
	{
		*out = buffer();
		return true;
	}
	return false;
}

text::encoding_t binary_buffer_output_stream::format()const
{
	return text::encoding::binary;
}

bool binary_buffer_output_stream::is_valid()const
{
	return _buffer.get() != null;
}

stream_index_t binary_buffer_output_stream::position()const
{
	return _cursor;
}

stream_size_t binary_buffer_output_stream::stream_size()const
{
	return _buffer.get() ? _buffer->buffer_size() : 0U;
}

bool binary_buffer_output_stream::move_to(stream_index_t size, stream_reference_t ref)
{
	if (!is_valid())
		return false;
	auto curSize = stream_size();
	if (curSize == 0)
		return false;

	stream_index_t maxPos = curSize;

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


bool binary_buffer_output_stream::forward(stream_index_t size)
{
	auto curSize = stream_size();
	stream_index_t maxPos = curSize;

	if ((_cursor + size) > maxPos)
		_cursor = maxPos;
	else
		_cursor += size;

	return true;
}

bool binary_buffer_output_stream::backward(stream_index_t size)
{
	auto curSize = stream_size();
	stream_index_t maxPos = curSize;

	if ((_cursor - size) <= 0)
		_cursor = 0;
	else
		_cursor -= size;

	return true;
}


ibuffer* binary_buffer_output_stream::buffer()const
{
	return _buffer.get();
}

bool binary_buffer_output_stream::attach(ibuffer* buff)
{
	_buffer = buff;
	_cursor = 0;
	return true;
}

bool binary_buffer_output_stream::can_move_to(stream_index_t size, stream_reference_t ref)
{
	if (!is_valid())
		return false;
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

bool binary_buffer_output_stream::can_forward(stream_index_t size)
{
	stream_index_t maxPos = stream_size();
	if ((_cursor + size) > maxPos)
		return _buffer->realloc_buffer((wsize)(_cursor + size));
	else
		return true;
	return false;
}

bool binary_buffer_output_stream::can_backward(stream_index_t size)
{
	stream_index_t maxPos = stream_size();
	if ((_cursor - size) <= 0)
		return false;
	else
		return true;
}

pointer binary_buffer_output_stream::pointer_at(stream_index_t idx)
{
	if (!can_move_to(idx, stream_reference::begin))
		return null;

	if (_buffer.get())
		return pointer(wsize(_buffer->buffer_ptr()) + idx);

	return null;
}

wsize binary_buffer_output_stream::write(pointer ptr, wsize sz)
{
	if (!can_forward(sz))
		return 0;
	memcpy(pointer_at(position()), ptr, sz);
	forward(sz);
	return sz;
}

wsize binary_buffer_output_stream::write(pointer ptr, wsize sz, text::text_format_t)
{
	if (!can_forward(sz))
		return 0;
	memcpy(pointer_at(position()), ptr, sz);
	forward(sz);
	return sz;
}