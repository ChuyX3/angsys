/*********************************************************************************************************************/
/*   File Name: mstring.cpp                                                                                          */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"

using namespace ang;
using namespace ang::strings;

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

mstring_buffer::mstring_buffer()
{
	memset(&_data, 0, sizeof(_data));
	allocator = memory::allocator_manager::get_allocator(memory::allocator_manager::buffer_allocator);
}

mstring_buffer::mstring_buffer(uint reserv)
	: mstring_buffer()
{
	realloc(reserv, false);
}

mstring_buffer::mstring_buffer(cstr_t cstr)
	: mstring_buffer()
{
	copy(cstr);
}

mstring_buffer::mstring_buffer(cwstr_t cstr)
	: mstring_buffer()
{
	copy(cstr);
}

mstring_buffer::mstring_buffer(cmstr_t cstr)
	: mstring_buffer()
{
	copy(cstr);
}

mstring_buffer::mstring_buffer(const mstring_buffer& cstr)
	: mstring_buffer()
{
	copy(&cstr);
}

mstring_buffer::mstring_buffer(const mstring_buffer* cstr)
	: mstring_buffer()
{
	copy(cstr);
}

mstring_buffer::mstring_buffer(const wstring_buffer* cstr)
	: mstring_buffer()
{
	copy(cstr);
}

mstring_buffer::mstring_buffer(const string_buffer* cstr)
	: mstring_buffer()
{
	copy(cstr);
}

mstring_buffer::~mstring_buffer() {
	clean();
}

type_name_t mstring_buffer::class_name() { return "ang::mstring"; }
type_name_t mstring_buffer::object_name()const { return class_name(); }
ANG_IMPLEMENT_ISKINDOF_BASE(ang::strings::mstring_buffer, ang::object);
ANG_IMPLEMENT_QUERYOBJECT_BASE(ang::strings::mstring_buffer, ang::object);

string_t mstring_buffer::to_string()const
{
	return cstr();
}

bool mstring_buffer::is_local_data()const
{
	return  bool(_data._flag_is_local_data != 0XFFFF);
}

bool mstring_buffer::realloc(uint new_size, bool save)
{
	if (capacity() > new_size)
		return true;
	
	uint size = 32;
	while (size <= new_size)
		size *= 2U;
	mchar* new_buffer = allocator->object_alloc<mchar>(size);
	if (new_buffer == null)
		return false;
	uint len = 0;
	new_buffer[0] = 0;
	if (save)
		len = string_copy(new_buffer, cstr());

	clean();
	_data._buffer_size_used = len;
	_data._buffer_capacity = size - 1;
	_data._buffer_ptr = new_buffer; 
	_data._flag_is_local_data = 0XFFFF;
	return true;
}

bool mstring_buffer::realloc(memory::iallocator* alloc)
{
	if (!is_local_data())
	{
		mstr_t new_buffer = mstr_t(alloc->object_alloc<mchar>(_data._buffer_capacity), _data._buffer_capacity);
		if (new_buffer == null) //out of memory??
			return false;
		new_buffer[0] = 0;
		string_copy(new_buffer, cstr());
		allocator->memory_release(_data._buffer_ptr);
		_data._buffer_ptr = new_buffer;
	}
	return true;
}

void mstring_buffer::length(uint len) {
	if (is_local_data())
		_data._local_size = (ushort)len;
	else
		_data._buffer_size_used = len;
}

bool mstring_buffer::is_empty()const
{
	return (_data._local_size == 0 && _data._buffer_size_used == 0) ? true : false;
}

mstr_t mstring_buffer::str()
{
	return is_local_data() ? mstr_t(_data._local_buffer, 30) : mstr_t(_data._buffer_ptr, _data._buffer_size_used);
}

cmstr_t mstring_buffer::cstr() const
{
	return is_local_data() ? cmstr_t(_data._local_buffer, 30) : cmstr_t(_data._buffer_ptr, _data._buffer_size_used);
}

uint mstring_buffer::length() const
{
	return string_length(cstr());
}

uint mstring_buffer::size() const
{
	return is_local_data() ? _data._local_size : _data._buffer_size_used;
}

uint mstring_buffer::capacity() const
{
	return is_local_data() ? 29U : _data._buffer_capacity;
}

void mstring_buffer::clean()
{
	if (!is_local_data())
		delete[] _data._buffer_ptr;
	memset(&_data, 0, sizeof(_data));
}

uint mstring_buffer::copy(cstr_t cstr)
{
	uint len = string_size(cstr, text::encoding_t::utf_8);

	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr, 30);

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr, _data._buffer_capacity + 1);
	}
	return length();
}

uint mstring_buffer::copy(cwstr_t cstr)
{
	uint len = string_size(cstr, text::encoding_t::utf_8);

	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr, 30);

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr, _data._buffer_capacity + 1);
	}
	return length();
}

uint mstring_buffer::copy(cmstr_t cstr)
{
	uint len = string_size(cstr, text::encoding_t::utf_8);

	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr, 30);

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr, _data._buffer_capacity + 1);
	}
	return length();
}

uint mstring_buffer::copy(mstring_buffer const* cstr)
{
	if (cstr == null)
		return 0;
	uint len = cstr->size();

	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr->cstr(), 30);

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr->cstr(), _data._buffer_capacity + 1);
	}
	return length();
}

uint mstring_buffer::copy(wstring_buffer const* cstr)
{
	if (cstr == null)
		return 0;
	uint len = string_size(cstr->cstr(), text::encoding_t::utf_8);

	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr->cstr(), 30);

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr->cstr(), _data._buffer_capacity + 1);
	}
	return length();
}

uint mstring_buffer::copy(string_buffer const* cstr)
{
	if (cstr == null)
		return 0;
	uint len = string_size(cstr->cstr(), text::encoding_t::utf_8);

	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr->cstr(), 30);

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr->cstr(), _data._buffer_capacity + 1);
	}
	return length();
}

uint mstring_buffer::concat(cstr_t cstr)
{
	uint my_len = size();
	uint len = string_size(cstr, text::encoding_t::utf_8);

	if (len == 0U)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr, 30 - my_len));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr, _data._buffer_capacity - my_len + 1));
	}
	return length() - my_len;
}

uint mstring_buffer::concat(cwstr_t cstr)
{
	uint my_len = size();
	uint len = string_size(cstr, text::encoding_t::utf_8);

	if (len == 0U)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr, 30 - my_len));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr, _data._buffer_capacity - my_len + 1));
	}
	return length() - my_len;
}

uint mstring_buffer::concat(cmstr_t cstr)
{
	uint my_len = size();
	uint len = string_size(cstr, text::encoding_t::utf_8);

	if (len == 0U)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr, 30 - my_len));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr, _data._buffer_capacity - my_len + 1));
	}
	return length() - my_len;
}

uint mstring_buffer::concat(mstring_buffer const* cstr)
{
	if (cstr == null)
		return 0;
	uint my_len = size();
	uint len = cstr->size();

	if (len == 0U)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr->cstr(), 30 - my_len));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr->cstr(), _data._buffer_capacity - my_len + 1));
	}
	return length() - my_len;
}

uint mstring_buffer::concat(wstring_buffer const* cstr)
{
	if (cstr == null)
		return 0;
	uint my_len = size();
	uint len = string_size(cstr->cstr(), text::encoding_t::utf_8);

	if (len == 0U)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr->cstr(), 30 - my_len));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr->cstr(), _data._buffer_capacity - my_len + 1));
	}
	return length() - my_len;
}

uint mstring_buffer::concat(string_buffer const* cstr)
{
	if (cstr == null)
		return 0;
	uint my_len = size();
	uint len = string_size(cstr->cstr(), text::encoding_t::utf_8);

	if (len == 0U)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr->cstr(), 30 - my_len));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr->cstr(), _data._buffer_capacity - my_len + 1));
	}
	return length() - my_len;
}

void mstring_buffer::move(mstring_buffer & str)
{
	memcpy(&_data, &str._data, sizeof(_data));
	memset(&str._data, 0, sizeof(str._data));
}

void mstring_buffer::move(mstring_buffer * str)
{
	memcpy(&_data, &str->_data, sizeof(_data));
	memset(&str->_data, 0, sizeof(str->_data));
}


int mstring_buffer::compare(cstr_t cstr)const
{
	return string_compare(this->cstr(), cstr);
}

int mstring_buffer::compare(cwstr_t cstr)const
{
	return string_compare(this->cstr(), cstr);
}

int mstring_buffer::compare(cmstr_t cstr)const
{
	return string_compare(this->cstr(), cstr);
}

/////////////////////////////////////////////////////////////

void mstring_buffer::set_allocator(memory::iallocator* alloc)
{
	if (allocator == alloc)
		return;

	if (!alloc->can_delete_from(allocator))
		realloc(alloc);
	allocator = alloc;
}

memory::iallocator* mstring_buffer::get_allocator()const
{
	return allocator;
}

pointer mstring_buffer::buffer_ptr()const
{
	return (void*)(cstr());
}

wsize mstring_buffer::buffer_size()const
{
	return capacity();
}

wsize mstring_buffer::mem_copy(wsize _size, pointer _ptr)
{
	if (_size == 0)
		return 0U;
	if (_size <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, (mchar*)(pointer)_ptr, min<uint, uint>(_size, 30U));
	}
	else
	{
		realloc(_size, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, (mchar*)(pointer)_ptr, min<uint, uint>(_size, _data._buffer_capacity + 1));
	}
	return length();
}

pointer mstring_buffer::map_buffer(windex start, wsize size)
{
	if ((start + size) > capacity())
		return null;
	return (mchar*)(pointer)str() + start;
}

void mstring_buffer::unmap_buffer(pointer ptr, wsize used)
{
	if (ptr == ((mchar*)(pointer)str() + size())
		&& (size() + used) <= capacity())
	{
		length(size() + used);
		str()[size()] = 0;
	}
}

bool mstring_buffer::can_realloc_buffer()const { return true; };

bool mstring_buffer::realloc_buffer(wsize size) { return realloc(size, true); };

text::encoding_t mstring_buffer::encoding()const
{
	return text::encoding::utf_8;
}
