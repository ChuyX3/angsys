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
#include "ang/collections/vector.h"

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::strings;

string_buffer::string_buffer()
{
	memset(&_data, 0, sizeof(_data));
	allocator = memory::allocator_manager::get_allocator(memory::allocator_manager::buffer_allocator);
}

string_buffer::string_buffer(uint reserv)
	: string_buffer()
{
	realloc(reserv, false);
}

string_buffer::string_buffer(cstr_t cstr)
	: string_buffer()
{
	copy(cstr);
}

string_buffer::string_buffer(cwstr_t cstr)
	: string_buffer()
{
	copy(cstr);
}

string_buffer::string_buffer(cmstr_t cstr)
	: string_buffer()
{
	copy(cstr);
}

string_buffer::string_buffer(const string_buffer& cstr)
	: string_buffer()
{
	copy(&cstr);
}

string_buffer::string_buffer(const string_buffer* cstr)
	: string_buffer()
{
	copy(cstr);
}

string_buffer::string_buffer(const wstring_buffer* cstr)
	: string_buffer()
{
	copy(cstr);
}

string_buffer::string_buffer(const mstring_buffer* cstr)
	: string_buffer()
{
	copy(cstr);
}

string_buffer::~string_buffer() {
	clean();
}

type_name_t string_buffer::class_name() { return "ang::string"; }
type_name_t string_buffer::object_name()const { return class_name(); }
ANG_IMPLEMENT_ISKINDOF_BASE(ang::strings::string_buffer, ang::object);
ANG_IMPLEMENT_QUERYOBJECT_BASE(ang::strings::string_buffer, ang::object);
ANG_IMPLEMENT_DYNAMICTYPE(ang::strings::string_buffer);

string string_buffer::to_string()const
{
	return cstr();
}

bool string_buffer::is_local_data()const
{
	return  bool(_data._flag_is_local_data != 0XFFFF);
}

bool string_buffer::realloc(uint new_size, bool save)
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
	char* new_buffer = allocator->object_alloc<char>(size);
	if (new_buffer == null)
		return false;
	uint len = 0U;
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

bool string_buffer::realloc(memory::iallocator* alloc)
{
	if (!_data._flag_is_local_data)
	{
		char* new_buffer = alloc->object_alloc<char>(_data._buffer_capacity);
		if (new_buffer == null) //out of memory??
			return false;
		new_buffer[0] = 0;
		string_copy(new_buffer, cstr());
		allocator->memory_release(_data._buffer_ptr);
		_data._buffer_ptr = new_buffer;
	}
	return true;
}

void string_buffer::length(uint len)
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

bool string_buffer::is_empty()const
{
	return (_data._local_size == 0 && _data._buffer_size_used == 0) ? true : false;
}

str_t string_buffer::str()
{
	return is_local_data() ? str_t(_data._local_buffer, 30) : str_t(_data._buffer_ptr, _data._buffer_size_used);
}

cstr_t string_buffer::cstr() const
{
	return is_local_data() ? cstr_t(_data._local_buffer, _data._local_size) : cstr_t(_data._buffer_ptr, _data._buffer_size_used);
}

uint string_buffer::length() const
{
	return is_local_data() ? _data._local_size : _data._buffer_size_used;
}

uint string_buffer::capacity() const
{
	return is_local_data() ? 29U : _data._buffer_capacity;
}

void string_buffer::clean()
{
	if (!is_local_data())
		allocator->memory_release(_data._buffer_ptr);
	memset(&_data, 0, sizeof(_data));
}

uint string_buffer::copy(cstr_t cstr)
{
	uint len = cstr.size();
	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr, min<uint>(len + 1, 30U));

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr, min<uint>(len + 1, _data._buffer_capacity + 1 ));
	}
	return length();
}

uint string_buffer::copy(cwstr_t cstr)
{
	uint len = cstr.size();;
	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr, min<uint>(len + 1, 30U));

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr, min<uint>(len + 1, _data._buffer_capacity + 1));
	}
	return length();
}

uint string_buffer::copy(cmstr_t cstr)
{
	uint len = string_length(cstr);

	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr, min<uint>(len + 1, 30U));

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr, min<uint>(len + 1, _data._buffer_capacity + 1));
	}
	return length();
}

uint string_buffer::copy(string_buffer const* cstr)
{
	if (cstr == null)
		return 0;
	uint len = cstr->length();

	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr->cstr(), min<uint>(len + 1, 30U));

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr->cstr(), min<uint>(len + 1, _data._buffer_capacity + 1));
	}
	return length();
}

uint string_buffer::copy(wstring_buffer const* cstr)
{
	if (cstr == null)
		return 0;
	uint len = cstr->length();

	if (len == 0U)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr->cstr(), min<uint>(len + 1, 30U));

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr->cstr(), min<uint>(len + 1, _data._buffer_capacity + 1));
	}
	return length();
}

uint string_buffer::copy(mstring_buffer const* cstr)
{
	if (cstr == null)
		return 0;
	uint len = cstr->length();
	if (len == 0)
		return 0U;
	if (len <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, cstr->cstr(), min<uint>(len + 1, 30U));

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, cstr->cstr(), min<uint>(len + 1, _data._buffer_capacity + 1));
	}
	return length();
}

uint string_buffer::concat(cstr_t cstr)
{
	uint my_len = length();
	uint len = cstr.size();

	if (len == 0)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr, min<uint>(len + 1, 30U - my_len)));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr, min<uint>(len + 1, _data._buffer_capacity - my_len + 1)));
	}
	return length() - my_len;
}

uint string_buffer::concat(cwstr_t cstr)
{
	uint my_len = length();
	uint len = cstr.size();

	if (len == 0)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr, min<uint>(len + 1, 30U - my_len)));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr, min<uint>(len + 1, _data._buffer_capacity - my_len + 1)));
	}
	return length() - my_len;
}

uint string_buffer::concat(cmstr_t cstr)
{
	uint my_len = length();
	uint len = string_length(cstr);

	if (len == 0U)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr, min<uint>(len + 1, 30U - my_len)));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr, min<uint>(len + 1, _data._buffer_capacity - my_len + 1)));
	}
	return length() - my_len;
}

uint string_buffer::concat(string_buffer const* cstr)
{
	if (cstr == null)
		return 0;

	uint my_len = length();
	uint len = cstr->length();

	if (len == 0)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr->cstr(), min<uint>(len + 1, 30U - my_len)));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr->cstr(), min<uint>(len + 1, _data._buffer_capacity - my_len + 1)));
	}
	return length() - my_len;
}

uint string_buffer::concat(wstring_buffer const* cstr)
{
	if (cstr == null)
		return 0;
	uint my_len = length();
	uint len = cstr->length();

	if (len == 0)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr->cstr(), min<uint>(len + 1, 30U - my_len)));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr->cstr(), min<uint>(len + 1, _data._buffer_capacity - my_len + 1)));
	}
	return length() - my_len;
}

uint string_buffer::concat(mstring_buffer const* cstr)
{
	if (cstr == null)
		return 0;
	uint my_len = length();
	uint len = cstr->length();

	if (len == 0)
		return 0U;

	if ((my_len + len) < 29U)
	{
		_data._local_size = (ushort)(my_len + string_copy(&_data._local_buffer[my_len], cstr->cstr(), min<uint>(len + 1, 30U - my_len)));
	}
	else
	{
		realloc(len + my_len);
		_data._buffer_size_used = (my_len + string_copy(&_data._buffer_ptr[my_len], cstr->cstr(), min<uint>(len + 1, _data._buffer_capacity - my_len + 1)));
	}
	return length() - my_len;
}

void string_buffer::move(string_buffer & str)
{
	memcpy(&_data, &str._data, sizeof(_data));
	memset(&str._data, 0, sizeof(str._data));
}

void string_buffer::move(string_buffer * str)
{
	memcpy(&_data, &str->_data, sizeof(_data));
	memset(&str->_data, 0, sizeof(str->_data));
}


int string_buffer::compare(cstr_t cstr)const
{
	return string_compare(this->cstr(), cstr);
}

int string_buffer::compare(cwstr_t cstr)const
{
	return string_compare(this->cstr(), cstr);
}

int string_buffer::compare(cmstr_t cstr)const
{
	return string_compare(this->cstr(), cstr);
}

/////////////////////////////////////////////////////////////

void string_buffer::set_allocator(memory::iallocator* alloc)
{
	if (allocator == alloc)
		return;

	if (!alloc->can_delete_from(allocator))
		realloc(alloc);
	allocator = alloc;
}

memory::iallocator* string_buffer::get_allocator()const
{
	return allocator;
}

pointer string_buffer::buffer_ptr()const
{
	return pointer(cstr());
}

wsize string_buffer::buffer_size()const
{
	return capacity();
}

wsize string_buffer::mem_copy(wsize _size, pointer _ptr)
{
	if (_size == 0U)
		return 0U;
	if (_size <= 29U)
	{
		clean();
		_data._local_size = (ushort)string_copy(_data._local_buffer, (char*)_ptr, min<uint>(_size, 30U));
	}
	else
	{
		realloc(_size, false);
		_data._buffer_size_used = string_copy(_data._buffer_ptr, (char*)_ptr, min<uint>(_size, _data._buffer_capacity + 1));
	}
	return length();
}

pointer string_buffer::map_buffer(windex start, wsize size)
{
	if ((start + size) > capacity())
		return null;
	return (byte*)(pointer)str() + start;
}

void string_buffer::unmap_buffer(pointer ptr, wsize used)
{
	auto this_ptr = ((byte*)(pointer)str() + length());
	if (ptr == this_ptr
		&& (length() + used) <= capacity())
	{
		length(length() + used);
		str()[length()] = 0;
	}
}

bool string_buffer::can_realloc_buffer()const { return true; };

bool string_buffer::realloc_buffer(wsize size) { return realloc(size, true); };

text::encoding_t string_buffer::encoding()const
{
	return text::encoding::ascii;
}

char& string_buffer::at(const collections::iterator<char>& it)const
{
	if (!it.is_valid() || it.current() != (pointer)cstr())
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= length())
		throw(exception_t(except_code::array_overflow));
	return ((char*)(it.current()))[it.offset()];
}

collections::iterator<char> string_buffer::begin()const
{
	return collections::iterator<char>(const_cast<string_buffer*>(this), (collections::position_t)cstr(), 0);
}

collections::iterator<char> string_buffer::end()const
{
	return collections::iterator<char>(const_cast<string_buffer*>(this), (collections::position_t)cstr(), length() - 1);
}

bool string_buffer::next(collections::iterator<char>& it)const
{

	if (it.parent() != this || it.current() != (pointer)cstr())
		throw(exception_t(except_code::invalid_param));
	it.offset(it.offset() + 1);
	if (it.offset() == length()) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

bool string_buffer::next(collections::iterator<char>& it, int val)const
{
	if (it.parent() != this || it.current() != (pointer)cstr())
		throw(exception_t(except_code::invalid_param));
	it.offset(it.offset() + val);
	if (it.offset() == length()) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

bool string_buffer::prev(collections::iterator<char>& it)const
{
	if (it.parent() != this || it.current() != (pointer)cstr())
		throw(exception_t(except_code::invalid_param));
	it.offset(it.offset() - 1);
	if (it.offset() > length()) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

bool string_buffer::prev(collections::iterator<char>& it, int val)const
{
	if (it.parent() != this || it.current() != (pointer)cstr())
		throw(exception_t(except_code::invalid_param));
	it.offset(it.offset() - val);
	if (it.offset() > length()) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

uint string_buffer::counter()const
{
	return length();
}



collections::iterator<char> string_buffer::at(index idx)const
{
	if (idx >= length())
		return collections::iterator<char>(const_cast<string_buffer*>(this));
	return collections::iterator<char>(const_cast<string_buffer*>(this), (collections::position_t)cstr(), idx);
}

uint string_buffer::insert(cstr_t cstr, collections::iterator<char> _pos)
{
	if (is_empty())
		return copy(cstr);

	index pos = _pos.offset();
	uint l1 = length();
	uint l2 = cstr.size();
	if (l2 == 0)
		return false;
	if (pos >= l1)
		return concat(cstr);

	if ((capacity() <= (l1 + l2)) && !realloc((l1 + l2), true))
		return 0;

	auto data = str();
	l2 = string_expand(data, l1, pos, pos + l2, capacity() + 1);
	string_copy(&data[pos], cstr, l2);

	if (is_local_data())
		_data._local_size = ushort(l1 + l2);
	else
		_data._buffer_size_used = l1 + l2;
	return l2;
}

uint string_buffer::insert(char c, collections::iterator<char> _pos)
{
	const char cstr[] = { c, 0 };

	if (is_empty())
		return copy(cstr);

	index pos = _pos.offset();
	uint l1 = length();
	uint l2 = 1;
	if (pos >= l1)
		return concat(cstr);

	if ((capacity() <= (l1 + l2)) && !realloc((l1 + l2), true))
		return 0;

	auto data = str();
	l2 = string_expand(data, l1, pos, pos + l2, capacity() + 1);
	string_copy(&data[pos], cstr, l2);

	if (is_local_data())
		_data._local_size = ushort(l1 + l2);
	else
		_data._buffer_size_used = l1 + l2;
	return l2;
}

uint string_buffer::insert(cwstr_t cstr, collections::iterator<char> _pos)
{
	if (is_empty())
		return copy(cstr);

	index pos = _pos.offset();
	uint l1 = length();
	uint l2 = cstr.size();
	if (l2 == 0)
		return false;
	if (pos >= l1)
		return concat(cstr);

	if ((capacity() <= (l1 + l2)) && !realloc((l1 + l2), true))
		return 0;

	auto data = str();
	l2 = string_expand(data, l1, pos, pos + l2, capacity() + 1);
	string_copy(&data[pos], cstr, l2);

	if (is_local_data())
		_data._local_size = ushort(l1 + l2);
	else
		_data._buffer_size_used = l1 + l2;
	return l2;
}

uint string_buffer::insert(wchar c, collections::iterator<char> _pos)
{
	const wchar cstr[] = { c, 0 };

	if (is_empty())
		return copy(cstr);

	index pos = _pos.offset();
	uint l1 = length();
	uint l2 = 1;
	if (pos >= l1)
		return concat(cstr);

	if ((capacity() <= (l1 + l2)) && !realloc((l1 + l2), true))
		return 0;

	auto data = str();
	l2 = string_expand(data, l1, pos, pos + l2, capacity() + 1);
	string_copy(&data[pos], cstr, l2);

	if (is_local_data())
		_data._local_size = ushort(l1 + l2);
	else
		_data._buffer_size_used = l1 + l2;
	return l2;
}

uint string_buffer::replace(cstr_t cstr, collections::iterator<char> beg, collections::iterator<char> end)
{
	if (is_empty() || cstr == null)
		return 0;

	if (beg.offset() >= end.offset())
		return insert(cstr, beg);

	uint l1 = length();
	uint l2 = cstr.size();

	if (end.offset() > l1)
		end.offset(l1);

	if (l2 == (end.offset() - beg.offset()))
	{
		str_t buff = str();
		for (index i = 0; i < l2; i++)
			buff[i + beg.offset()] = (char)cstr[i];
	}
	else if (capacity() <= (l1 + l2 + beg.offset() - end.offset()))
	{
		uint size = 32, new_size = l1 + l2 + beg.offset() - end.offset();
		while (size <= new_size)
			size *= 2U;
		char* new_buffer = allocator->object_alloc<char>(size);
		new_size = string_copy(new_buffer, this->cstr(), beg.offset());
		new_size += string_copy(&new_buffer[new_size], cstr, l2);
		new_size += string_copy(&new_buffer[new_size], &this->cstr()[end.offset()]);

		clean();
		_data._buffer_size_used = new_size;
		_data._buffer_capacity = size - 1;
		_data._buffer_ptr = new_buffer;
		_data._flag_is_local_data = 0XFFFF;
	}
	else
	{
		uint new_size = l1 + l2 + beg.offset() - end.offset();
		if (l1 < new_size)
		{
			string_expand(str(), l1, beg.offset(), 2 * beg.offset() + l2 - end.offset(), capacity() + 1);
			string_copy(&str()[beg.offset()], cstr, l2);
		}
		else
		{
			string_contract(str(), l1, beg.offset(), end.offset() - l2);
			string_copy(&str()[beg.offset()], cstr, l2);
		}
		if (is_local_data())
			_data._local_size = (ushort)new_size;
		else
			_data._buffer_size_used = new_size;
	}
	return l2;
}

uint string_buffer::replace(char c, collections::iterator<char> beg, collections::iterator<char> end)
{
	if (is_empty())
		return 0;

	if (beg.offset() >= end.offset())
		return insert(c, beg);

	const char cstr[] = { c, 0 };
	uint l1 = length();
	uint l2 = 1;

	if (end.offset() > l1)
		end.offset(l1);

	if (l2 == (end.offset() - beg.offset()))
	{
		str_t buff = str();
		for (uint i = 0; i < l2; i++)
			buff[i + beg.offset()] = (char)cstr[i];
	}
	else if (capacity() <= (l1 + l2 + beg.offset() - end.offset()))
	{
		uint size = 32, new_size = l1 + l2 + beg.offset() - end.offset();
		while (size <= new_size)
			size *= 2U;
		char* new_buffer = allocator->object_alloc<char>(size);
		new_size = string_copy(new_buffer, this->cstr(), beg.offset());
		new_size += string_copy(&new_buffer[new_size], cstr, l2);
		new_size += string_copy(&new_buffer[new_size], &this->cstr()[end.offset()]);

		clean();
		_data._buffer_size_used = new_size;
		_data._buffer_capacity = size - 1;
		_data._buffer_ptr = new_buffer;
		_data._flag_is_local_data = 0XFFFF;
	}
	else
	{
		uint new_size = l1 + l2 + beg.offset() - end.offset();
		if (l1 < new_size)
		{
			string_expand(str(), l1, beg.offset(), 2 * beg.offset() + l2 - end.offset(), capacity() + 1);
			string_copy(&str()[beg.offset()], cstr, l2);
		}
		else
		{
			string_contract(str(), l1, beg.offset(), end.offset() - l2);
			string_copy(&str()[beg.offset()], cstr, l2);
		}
		if (is_local_data())
			_data._local_size = (ushort)new_size;
		else
			_data._buffer_size_used = new_size;
	}
	return l2;
}

uint string_buffer::replace(cwstr_t cstr, collections::iterator<char> beg, collections::iterator<char> end)
{
	if (is_empty() || cstr == null)
		return 0;

	if (beg.offset() >= end.offset())
		return insert(cstr, beg);

	uint l1 = length();
	uint l2 = cstr.size();

	if (end.offset() > l1)
		end.offset(l1);

	if (l2 == (end.offset() - beg.offset()))
	{
		str_t buff = str();
		for (index i = 0; i < l2; i++)
			buff[i + beg.offset()] = (char)(byte)cstr[i];
	}
	else if (capacity() <= (l1 + l2 + beg.offset() - end.offset()))
	{
		uint size = 32, new_size = l1 + l2 + beg.offset() - end.offset();
		while (size <= new_size)
			size *= 2U;
		char* new_buffer = allocator->object_alloc<char>(size);
		new_size = string_copy(new_buffer, this->cstr(), beg.offset());
		new_size += string_copy(&new_buffer[new_size], cstr, l2);
		new_size += string_copy(&new_buffer[new_size], &this->cstr()[end.offset()]);

		clean();
		_data._buffer_size_used = new_size;
		_data._buffer_capacity = size - 1;
		_data._buffer_ptr = new_buffer;
		_data._flag_is_local_data = 0XFFFF;
	}
	else
	{
		uint new_size = l1 + l2 + beg.offset() - end.offset();
		if (l1 < new_size)
		{
			string_expand(str(), l1, beg.offset(), 2 * beg.offset() + l2 - end.offset(), capacity() + 1);
			string_copy(&str()[beg.offset()], cstr, l2);
		}
		else
		{
			string_contract(str(), l1, beg.offset(), end.offset() - l2);
			string_copy(&str()[beg.offset()], cstr, l2);
		}
		if (is_local_data())
			_data._local_size = (ushort)new_size;
		else
			_data._buffer_size_used = new_size;
	}
	return l2;
}

uint string_buffer::replace(wchar c, collections::iterator<char> beg, collections::iterator<char> end)
{
	if (is_empty())
		return 0;

	if (beg.offset() >= end.offset())
		return insert(c, beg);

	const wchar cstr[] = { c, 0 };
	uint l1 = length();
	uint l2 = 1;

	if (end.offset() > l1)
		end.offset(l1);

	if (l2 == (end.offset() - beg.offset()))
	{
		str_t buff = str();
		for (uint i = 0; i < l2; i++)
			buff[i + beg.offset()] = (char)(byte)cstr[i];
	}
	else if (capacity() <= (l1 + l2 + beg.offset() - end.offset()))
	{
		uint size = 32, new_size = l1 + l2 + beg.offset() - end.offset();
		while (size <= new_size)
			size *= 2U;
		char* new_buffer = allocator->object_alloc<char>(size);
		new_size = string_copy(new_buffer, this->cstr(), beg.offset());
		new_size += string_copy(&new_buffer[new_size], cstr, l2);
		new_size += string_copy(&new_buffer[new_size], &this->cstr()[end.offset()]);

		clean();
		_data._buffer_size_used = new_size;
		_data._buffer_capacity = size - 1;
		_data._buffer_ptr = new_buffer;
		_data._flag_is_local_data = 0XFFFF;
	}
	else
	{
		uint new_size = l1 + l2 + beg.offset() - end.offset();
		if (l1 < new_size)
		{
			string_expand(str(), l1, beg.offset(), 2 * beg.offset() + l2 - end.offset(), capacity() + 1);
			string_copy(&str()[beg.offset()], cstr, l2);
		}
		else
		{
			string_contract(str(), l1, beg.offset(), end.offset() - l2);
			string_copy(&str()[beg.offset()], cstr, l2);
		}
		if (is_local_data())
			_data._local_size = (ushort)new_size;
		else
			_data._buffer_size_used = new_size;
	}
	return l2;
}

collections::iterator<char> string_buffer::find(cstr_t _cstr, collections::iterator<char> _start, bool _rev)const
{
	return _start.is_valid() ? at(ang::string_find<char, char>(cstr(), length(), _cstr, _cstr.size(), _start.offset(), _rev)) : _start;
}

collections::iterator<char> string_buffer::find(cstr_t _cstr, collections::iterator<char> _start, collections::iterator<char> _end, bool _rev)const
{
	return _start.is_valid() ? at(ang::string_find<char, char>(cstr(), min<uint>(length(), _end.offset()), _cstr, _cstr.size(), _start.offset(), _rev)) : _start;
}

collections::iterator<char> string_buffer::find(cwstr_t _cstr, collections::iterator<char> _start, bool _rev)const
{
	return _start.is_valid() ? at(ang::string_find<char, wchar>(cstr(), length(), _cstr, _cstr.size(), _start.offset(), _rev)) : _start;
}

collections::iterator<char> string_buffer::find(cwstr_t _cstr, collections::iterator<char> _start, collections::iterator<char> _end, bool _rev)const
{
	return _start.is_valid() ? at(ang::string_find<char, wchar>(cstr(), min<uint>(length(), _end.offset()), _cstr, _cstr.size(), _start.offset(), _rev)) : _start;
}

uint string_buffer::sub_string(string& out, collections::iterator<char> start, uint count)const
{
	if (is_empty() || start.parent() != this)
		return 0;

	if (out.is_empty())
		out = new string_buffer();

	auto l1 = length();
	if (count > l1 || (count + start.offset()) > l1)
		count = l1 - start.offset();

	if (!out->realloc(count, false))
		return 0;

	string_buffer* buffer = out.get();

	if (buffer->is_local_data())
		return (buffer->_data._local_size = (ushort)string_substr<char, char>(cstr(), l1, buffer->str(), start.offset(), count));
	else
		return (buffer->_data._buffer_size_used = string_substr<char, char>(cstr(), l1, buffer->str(), start.offset(), count));
}

uint string_buffer::sub_string(str_t out, collections::iterator<char> start, uint count)const
{
	if (is_empty() || start.parent() != this)
		return 0;
	return string_substr<char, char>(cstr(), length(), out, start.offset(), count);
}

uint string_buffer::sub_string(string& out, collections::iterator<char> start, collections::iterator<char> end)const
{
	auto l1 = length();
	if (is_empty() || end.offset() <= start.offset() || start.offset() >= l1)
		return 0;

	if (out.is_empty())
		out = new string_buffer();

	if (end.offset() > l1)
		end.offset(l1);

	if (!out->realloc(end.offset() - start.offset(), false))
		return 0;

	string_buffer* buffer = out.get();

	if (buffer->is_local_data())
		return (buffer->_data._local_size = (ushort)string_substr<char, char>(cstr(), l1, buffer->str(), start.offset(), end.offset() - start.offset()));
	else
		return (buffer->_data._buffer_size_used = string_substr<char, char>(cstr(), l1, buffer->str(), start.offset(), end.offset() - start.offset()));
}

uint string_buffer::sub_string(str_t out, collections::iterator<char> start, collections::iterator<char> end)const
{
	auto l1 = length();
	if (is_empty() || end.offset() <= start.offset() || start.offset() >= l1)
		return 0;
	return string_substr<char, char>(cstr(), l1, out, start.offset(), end.offset() - start.offset());
}

uint string_buffer::sub_string(wstring& out, collections::iterator<char> start, uint count)const
{
	if (is_empty() || start.parent() != this)
		return 0;

	if (out.is_empty())
		out = new wstring_buffer();

	if (!out->realloc(count, false))
		return 0;

	wstring_buffer* buffer = out.get();

	if (buffer->is_local_data())
		return (buffer->_data._local_size = (ushort)string_substr<char, wchar>(cstr(), length(), buffer->str(), start.offset(), count));
	else
		return (buffer->_data._buffer_size_used = string_substr<char, wchar>(cstr(), length(), buffer->str(), start.offset(), count));
}

uint string_buffer::sub_string(wstr_t out, collections::iterator<char> start, uint count)const
{
	if (is_empty() || start.parent() != this)
		return 0;
	return string_substr<char, wchar>(cstr(), length(), out, start.offset(), count);
}

uint string_buffer::sub_string(wstring& out, collections::iterator<char> start, collections::iterator<char> end)const
{
	if (is_empty() || !end.is_valid() || !start.is_valid() || end.offset() <= start.offset())
		return 0;

	if (out.is_empty())
		out = new wstring_buffer();

	if (!out->realloc(end.offset() - start.offset(), false))
		return 0;

	wstring_buffer* buffer = out.get();

	if (buffer->is_local_data())
		return (buffer->_data._local_size = (ushort)string_substr<char, wchar>(cstr(), length(), buffer->str(), start.offset(), end.offset() - start.offset()));
	else
		return (buffer->_data._buffer_size_used = string_substr<char, wchar>(cstr(), length(), buffer->str(), start.offset(), end.offset() - start.offset()));
}

uint string_buffer::sub_string(wstr_t out, collections::iterator<char> start, collections::iterator<char> end)const
{
	if (is_empty() || !end.is_valid() || !start.is_valid() || end.offset() <= start.offset())
		return 0;
	return string_substr<char, wchar>(cstr(), length(), out, start.offset(), end.offset() - start.offset());
}

void string_buffer::format(cstr_t format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	uint size = _vscprintf(format, arg_list);
	if (size <= 0U)
		return clean();

	realloc(size, false);
	if (is_local_data())
		_data._local_size = vsprintf_s(_data._local_buffer, 30, format, arg_list);
	else
		_data._buffer_size_used = vsprintf_s(_data._buffer_ptr, _data._buffer_capacity + 1, format, arg_list);
	va_end(arg_list);
}

void string_buffer::concat_format(cstr_t format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	uint size = _vscprintf(format, arg_list);
	if (size <= 0U)
		return clean();

	realloc(size + length(), true);
	if (is_local_data())
		_data._local_size += vsprintf_s(&_data._local_buffer[_data._local_size], 30 - _data._local_size, format, arg_list);
	else
		_data._buffer_size_used += vsprintf_s(&_data._buffer_ptr[_data._buffer_size_used], _data._buffer_capacity - _data._buffer_size_used + 1, format, arg_list);
	va_end(arg_list);
}

void string_buffer::invert()
{
	if (is_empty())
		return;

	char* beg = str();
	char* end = beg + length();
	char temp;
	while (beg < end)
	{
		temp = *beg;
		*beg = *end;
		*end = temp;
		end--;
		beg++;
	}
}

void string_buffer::invert(collections::iterator<char> b, collections::iterator<char> e)
{
	if (is_empty() || !e.is_valid() || !b.is_valid()
		|| (length() <= e.offset()) || (e.offset() <= b.offset()))
		return;

	char* beg = &(char&)b;
	char* end = &(char&)e;
	char temp;
	while (beg < end)
	{
		temp = *beg;
		*beg = *end;
		*end = temp;
		end--;
		beg++;
	}
}

void string_buffer::uppercase()
{
	if (is_empty())
		return;
	uint l = length();
	char* buff = str();
	for (uint i = 0; i < l; i++)
	{
		if ((buff[i] >= 'a') && (buff[i] <= 'z'))
			buff[i] = char('A' - 'a' + buff[i]);
	}
}

void string_buffer::lowercase()
{
	if (is_empty())
		return;
	uint l = length();
	str_t buff = str();
	for (uint i = 0; i < l; i++)
	{
		if ((buff[i] >= 'A') && (buff[i] <= 'Z'))
			buff[i] = char('a' - 'A' + buff[i]);
	}
}

array<string> string_buffer::split(char val)const
{
	collections::vector<string > list;

	const char str_val[] = { val, 0 };

	
	index beg = 0, end = 0;
	string _word;
	cstr_t data = cstr();
	uint l = length();

	end = string_find<char>(data, l, str_val, 1, 0, false);
	if (end == invalid_index)
		return array<string>({cstr()});
	
	do {
		cstr().sub_string(_word, beg, end - beg);
		if (_word->length() > 0)
		{
			list += null;// ->append(_word.get(), true);
			list[list->size() -1] = _word.get();
			_word = null;
		}
		beg = end + 1;
		end = string_find<char>(data, l, str_val, 1, beg, false);//Find(strVal, beg);
	} while (end != invalid_index);

	if (_word.is_empty()) _word = new string_buffer();
	_word->realloc(l - beg, false);
	string_substr<char, char>(cstr(), l, _word->str(), beg, l - beg);
	if (_word->length() > 0)
	{
		list += null;// ->append(_word.get(), true);
		list[list->size() - 1] = _word.get();
		_word = null;
	}

	array<string> arr = new collections::array_buffer<string>(list->size());
	index i = 0;
	foreach(list, [&](string& str) { arr[i++] = ang::move(str); });
	return arr.get();
}

array<string> string_buffer::split(array<char> arr)const
{
	return array<string>({ cstr() }); //todo
}

array<string> string_buffer::split(cstr_t val)const
{
	collections::vector<string > list;

	index beg = 0, end = 0;
	string _word;
	cstr_t data = cstr();
	uint l = length();

	end = string_find<char>(data, l, val.cstr(), val.size(), 0, false);
	if (end == invalid_index)
		return array<string>({ cstr() });

	do {
		if (end - beg > 0)
		{
			cstr().sub_string(_word, beg, end - beg);
			//_word->length(end - beg);
			list += _word.get();
			_word = null;
		}
		beg = end + val.size();
		end = string_find<char>(data, l, val.cstr(), val.size(), beg, false);//Find(strVal, beg);
	} while (end != invalid_index);

	
	if (l - beg > 0)
	{
		_word = new string_buffer();
		_word->realloc(l - beg, false);
		string_substr<char, char>(cstr(), l, _word->str(), beg, l - beg);
		_word->length(l - beg);
		list += _word.get();
		_word = null;
	}

	array<string> arr = new collections::array_buffer<string>(list->size());
	index i = 0;
	foreach(list, [&](string& str) { arr[i++] = ang::move(str); });
	return arr.get();
}