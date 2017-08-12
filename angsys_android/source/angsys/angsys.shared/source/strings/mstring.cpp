/*********************************************************************************************************************/
/*   File Name: string.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"
#include "string_utils_inline.h"


#define ANG_ALLOCATOR_ALLOC(_SIZE) malloc(_SIZE)
#define ANG_ALLOCATOR_ALLOC_DEBUG(_SIZE, _FILE, _LINE) malloc(_SIZE)
#define ANG_ALLOCATOR_RELEASE(_PTR) free(_PTR)
#define ANG_ALIGNED_ALLOCATOR_ALLOC(_SIZE, _ALIGMENT) _mm_malloc(_SIZE, _ALIGMENT)
#define ANG_ALIGNED_ALLOCATOR_RELEASE(_PTR) _mm_free(_PTR)


#define ANG_ALLOCATOR_NEW(_TYPE, _COUNT)  (_TYPE*)ANG_ALLOCATOR_ALLOC(sizeof(_TYPE) * _COUNT)
#define ANG_ALLOCATOR_NEW_DEBUG(_TYPE, _COUNT, _FILE, _LINE)  (_TYPE*)ANG_ALLOCATOR_ALLOC_DEBUG(sizeof(_TYPE) * _COUNT, _FILE, _LINE)

using namespace ang;
using namespace ang::strings;

mstring_buffer::mstring_buffer()
{
	memset(&_data, 0, sizeof(_data));
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
	copy(cstr.cstr());
}

mstring_buffer::mstring_buffer(const mstring_buffer* cstr)
	: mstring_buffer()
{
	cstr ? copy(cstr->cstr()) : 0;
}

mstring_buffer::mstring_buffer(const wstring_buffer* cstr)
	: mstring_buffer()
{
	cstr ? copy(cstr->cstr()) : 0;
}

mstring_buffer::mstring_buffer(const string_buffer* cstr)
	: mstring_buffer()
{
	cstr ? copy(cstr->cstr()) : 0;
}

mstring_buffer::~mstring_buffer() {
	clean();
}

type_name_t mstring_buffer::class_name() { return "ang::mstring"_s; }
type_name_t mstring_buffer::object_name()const { return class_name(); }
ANG_IMPLEMENT_ISKINDOF_BASE(ang::strings::mstring_buffer, ang::object);
ANG_IMPLEMENT_QUERYOBJECT_BASE(ang::strings::mstring_buffer, ang::object);
ANG_IMPLEMENT_DYNAMICTYPE(ang::strings::mstring_buffer);

bool mstring_buffer::is_local_data()const
{
	return  bool(_data._flag_is_local_data != 0XFFFF);
}

bool mstring_buffer::realloc(uint new_size, bool save)
{
	if (capacity() > new_size)
	{
		//if (new_size < 29U && !is_local_data())
		//{
		//	delete[] _data._buffer_ptr;
		//	memset(&_data, 0, sizeof(_data));
		//}
		return true;
	}
	uint size = 32U;
	while (size <= new_size)
		size *= 2U;
	mchar* new_buffer = ANG_ALLOCATOR_NEW(mchar, size);
	if (new_buffer == null)
		return false;
	uint len = 0U;
	new_buffer[0] = 0;
	if (save)
		len = algorithms::string_copy(new_buffer, cstr());

	clean();
	_data._buffer_size_used = len;
	_data._buffer_capacity = size - 1;
	_data._buffer_ptr = new_buffer;
	_data._flag_is_local_data = 0XFFFF;
	return true;
}


void mstring_buffer::length(uint len)
{
	if (is_local_data()) {
		len = min(len, 29U);
		_data._local_size = (ushort)len;
		_data._local_buffer[len] = 0;
	}
	else {
		len = min(len, _data._buffer_capacity);
		_data._buffer_size_used = len;
		_data._buffer_ptr[len] = 0;
	}
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
	return is_local_data() ? cmstr_t(_data._local_buffer, _data._local_size) : cmstr_t(_data._buffer_ptr, _data._buffer_size_used);
}

uint mstring_buffer::length() const
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
		ANG_ALLOCATOR_RELEASE(_data._buffer_ptr);
	memset(&_data, 0, sizeof(_data));
}

uint mstring_buffer::copy(cstr_t cstr)
{
	uint len = cstr.size();
	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)algorithms::string_copy(_data._local_buffer, cstr, min<uint>(len + 1, 30U));

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = algorithms::string_copy(_data._buffer_ptr, cstr, min<uint>(len + 1, _data._buffer_capacity + 1));
	}
	return length();
}

uint mstring_buffer::copy(cwstr_t cstr)
{
	uint len = cstr.size();;
	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)algorithms::string_copy(_data._local_buffer, cstr, min<uint>(len + 1, 30U));

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = algorithms::string_copy(_data._buffer_ptr, cstr, min<uint>(len + 1, _data._buffer_capacity + 1));
	}
	return length();
}

uint mstring_buffer::copy(cmstr_t cstr)
{
	uint len = algorithms::string_length(cstr);

	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)algorithms::string_copy(_data._local_buffer, cstr, min<uint>(len + 1, 30U));

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = algorithms::string_copy(_data._buffer_ptr, cstr, min<uint>(len + 1, _data._buffer_capacity + 1));
	}
	return length();
}

uint mstring_buffer::concat(cstr_t cstr)
{
	uint my_len = length();
	uint len = cstr.size();

	if (len == 0)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + algorithms::string_copy(&_data._local_buffer[my_len], cstr, min<uint>(len + 1, 30U - my_len)));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + algorithms::string_copy(&_data._buffer_ptr[my_len], cstr, min<uint>(len + 1, _data._buffer_capacity - my_len + 1)));
	}
	return length() - my_len;
}

uint mstring_buffer::concat(cwstr_t cstr)
{
	uint my_len = length();
	uint len = cstr.size();

	if (len == 0)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + algorithms::string_copy(&_data._local_buffer[my_len], cstr, min<uint>(len + 1, 30U - my_len)));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + algorithms::string_copy(&_data._buffer_ptr[my_len], cstr, min<uint>(len + 1, _data._buffer_capacity - my_len + 1)));
	}
	return length() - my_len;
}

uint mstring_buffer::concat(cmstr_t cstr)
{
	uint my_len = length();
	uint len = algorithms::string_length(cstr);

	if (len == 0U)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + algorithms::string_copy(&_data._local_buffer[my_len], cstr, min<uint>(len + 1, 30U - my_len)));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + algorithms::string_copy(&_data._buffer_ptr[my_len], cstr, min<uint>(len + 1, _data._buffer_capacity - my_len + 1)));
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
	return algorithms::string_compare(this->cstr(), cstr);
}

int mstring_buffer::compare(cwstr_t cstr)const
{
	return algorithms::string_compare(this->cstr(), cstr);
}

int mstring_buffer::compare(cmstr_t cstr)const
{
	return algorithms::string_compare(this->cstr(), cstr);
}

comparision_result_t mstring_buffer::compare(object const& obj)const
{
	if (obj.is_kind_of(type_of<mstring_buffer>()))
		return (comparision_result)compare(static_cast<mstring_buffer const&>(obj).cstr());
	return comparision_result::diferent;
}

/////////////////////////////////////////////////////////////

pointer mstring_buffer::buffer_ptr()const
{
	return pointer(cstr());
}

wsize mstring_buffer::buffer_size()const
{
	return capacity();
}

wsize mstring_buffer::mem_copy(wsize _size, pointer _ptr)
{
	if (_size == 0U)
		return 0U;
	if (_size <= 29U)
	{
		clean();
		_data._local_size = (ushort)algorithms::string_copy(_data._local_buffer, (mchar*)_ptr, min<uint>(_size, 30U));
	}
	else
	{
		realloc(_size, false);
		_data._buffer_size_used = algorithms::string_copy(_data._buffer_ptr, (mchar*)_ptr, min<uint>(_size, _data._buffer_capacity + 1));
	}
	return length();
}

pointer mstring_buffer::map_buffer(windex start, wsize size)
{
	if ((start + size) > capacity())
		return null;
	return (byte*)(pointer)str() + start;
}

void mstring_buffer::unmap_buffer(pointer ptr, wsize used)
{
	auto this_ptr = ((byte*)(pointer)str() + length());
	if (ptr == this_ptr
		&& (length() + used) <= capacity())
	{
		length(length() + used);
		str()[length()] = 0;
	}
}

bool mstring_buffer::can_realloc_buffer()const { return true; };

bool mstring_buffer::realloc_buffer(wsize size) { return realloc(size, true); };

text::encoding_t mstring_buffer::encoding()const
{
	return text::encoding::utf_8;
}

mchar& mstring_buffer::at(index idx)
{
	if (idx >= length())
		throw exception(except_code::array_overflow);
	return str()[idx];
}

mstr_t mstring_buffer::begin()
{
	return  str();
}

mstr_t mstring_buffer::end()
{
	return  &str()[length()];
}

mchar const& mstring_buffer::at(index idx)const
{
	if (idx >= length())
		throw exception(except_code::array_overflow);

	return cstr()[idx];
}

cmstr_t mstring_buffer::begin()const
{
	return  cstr();
}

cmstr_t mstring_buffer::end()const
{
	return  &cstr()[length()];
}

