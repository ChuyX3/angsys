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

string_buffer::string_buffer()
{
	memset(&_data, 0, sizeof(_data));
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
	copy(cstr.cstr());
}

string_buffer::string_buffer(const string_buffer* cstr)
	: string_buffer()
{
	cstr? copy(cstr->cstr()) : 0;
}

string_buffer::string_buffer(const wstring_buffer* cstr)
	: string_buffer()
{
	cstr ? copy(cstr->cstr()) : 0;
}

string_buffer::string_buffer(const mstring_buffer* cstr)
	: string_buffer()
{
	cstr ? copy(cstr->cstr()) : 0;
}

string_buffer::~string_buffer() {
	clean();
}

type_name_t string_buffer::class_name() { return "ang::string"_s; }
type_name_t string_buffer::object_name()const { return class_name(); }
ANG_IMPLEMENT_ISKINDOF_BASE(ang::strings::string_buffer, ang::object);
ANG_IMPLEMENT_QUERYOBJECT_BASE(ang::strings::string_buffer, ang::object);
ANG_IMPLEMENT_DYNAMICTYPE(ang::strings::string_buffer);

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
	char* new_buffer = ANG_ALLOCATOR_NEW(char, size);
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
		ANG_ALLOCATOR_RELEASE(_data._buffer_ptr);
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
		_data._local_size = (ushort)algorithms::string_copy(_data._local_buffer, cstr, min<uint>(len + 1, 30U));

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = algorithms::string_copy(_data._buffer_ptr, cstr, min<uint>(len + 1, _data._buffer_capacity + 1));
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
		_data._local_size = (ushort)algorithms::string_copy(_data._local_buffer, cstr, min<uint>(len + 1, 30U));

	}
	else
	{
		realloc(len, false);
		_data._buffer_size_used = algorithms::string_copy(_data._buffer_ptr, cstr, min<uint>(len + 1, _data._buffer_capacity + 1));
	}
	return length();
}

uint string_buffer::copy(cmstr_t cstr)
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

uint string_buffer::concat(cstr_t cstr)
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

uint string_buffer::concat(cwstr_t cstr)
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

uint string_buffer::concat(cmstr_t cstr)
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
	return algorithms::string_compare(this->cstr(), cstr);
}

int string_buffer::compare(cwstr_t cstr)const
{
	return algorithms::string_compare(this->cstr(), cstr);
}

int string_buffer::compare(cmstr_t cstr)const
{
	return algorithms::string_compare(this->cstr(), cstr);
}

comparision_result_t string_buffer::compare(object const& obj)const
{
	if (obj.is_kind_of(type_of<string_buffer>()))
		return (comparision_result)compare(static_cast<string_buffer const&>(obj).cstr());
	return comparision_result::diferent;
}

/////////////////////////////////////////////////////////////

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
		_data._local_size = (ushort)algorithms::string_copy(_data._local_buffer, (char*)_ptr, min<uint>(_size, 30U));
	}
	else
	{
		realloc(_size, false);
		_data._buffer_size_used = algorithms::string_copy(_data._buffer_ptr, (char*)_ptr, min<uint>(_size, _data._buffer_capacity + 1));
	}
	return length();
}

pointer string_buffer::map_buffer(windex start, wsize size)
{
	if ((start + size) > capacity())
		return null;
	//length(start);//new length??
	return (byte*)(pointer)str() + start;
}

void string_buffer::unmap_buffer(pointer ptr, wsize used)
{
	auto this_ptr = (byte*)(pointer)str();
	if (ptr >= this_ptr && ptr < (this_ptr + capacity()))
	{
		length(min(length() + used, capacity()));
		str()[length()] = 0;
	}
}

bool string_buffer::can_realloc_buffer()const { return true; };

bool string_buffer::realloc_buffer(wsize size) { return realloc(size, true); };

text::encoding_t string_buffer::encoding()const
{
	return text::encoding::ascii;
}

char& string_buffer::at(index idx)
{
	if (idx >= length())
		throw exception(except_code::array_overflow);
	return str()[idx];
}

str_t string_buffer::begin()
{
	return  str();
}

str_t string_buffer::end()
{
	return  &str()[length()];
}


char const& string_buffer::at(index idx)const
{
	if (idx >= length())
		throw exception(except_code::array_overflow);

	return cstr()[idx];
}

cstr_t string_buffer::begin()const
{
	return  cstr();
}

cstr_t string_buffer::end()const
{
	return  &cstr()[length()];
}

uint string_buffer::insert(cstr_t cstr, index pos)
{
	if (is_empty())
		return copy(cstr);

	uint l1 = length();
	uint l2 = cstr.size();
	if (l2 == 0)
		return false;
	if (pos >= l1)
		return concat(cstr);

	if ((capacity() <= (l1 + l2)) && !realloc((l1 + l2), true))
		return 0;

	auto data = str();
	l2 = algorithms::string_expand(data, l1, pos, pos + l2, capacity() + 1);
	algorithms::string_copy(&data[pos], cstr, l2);

	if (is_local_data())
		_data._local_size = ushort(l1 + l2);
	else
		_data._buffer_size_used = l1 + l2;
	return l2;
}

uint string_buffer::insert(char c, index pos)
{
	const char cstr[] = { c, 0 };

	if (is_empty())
		return copy(cstr);

	uint l1 = length();
	uint l2 = 1;
	if (pos >= l1)
		return concat(cstr);

	if ((capacity() <= (l1 + l2)) && !realloc((l1 + l2), true))
		return 0;

	auto data = str();
	l2 = algorithms::string_expand(data, l1, pos, pos + l2, capacity() + 1);
	algorithms::string_copy(&data[pos], cstr, l2);

	if (is_local_data())
		_data._local_size = ushort(l1 + l2);
	else
		_data._buffer_size_used = l1 + l2;
	return l2;
}

uint string_buffer::insert(cwstr_t cstr, index pos)
{
	if (is_empty())
		return copy(cstr);

	uint l1 = length();
	uint l2 = cstr.size();
	if (l2 == 0)
		return false;
	if (pos >= l1)
		return concat(cstr);

	if ((capacity() <= (l1 + l2)) && !realloc((l1 + l2), true))
		return 0;

	auto data = str();
	l2 = algorithms::string_expand(data, l1, pos, pos + l2, capacity() + 1);
	algorithms::string_copy(&data[pos], cstr, l2);

	if (is_local_data())
		_data._local_size = ushort(l1 + l2);
	else
		_data._buffer_size_used = l1 + l2;
	return l2;
}

uint string_buffer::insert(wchar c, index pos)
{
	const wchar cstr[] = { c, 0 };

	if (is_empty())
		return copy(cstr);

	uint l1 = length();
	uint l2 = 1;
	if (pos >= l1)
		return concat(cstr);

	if ((capacity() <= (l1 + l2)) && !realloc((l1 + l2), true))
		return 0;

	auto data = str();
	l2 = algorithms::string_expand(data, l1, pos, pos + l2, capacity() + 1);
	algorithms::string_copy(&data[pos], cstr, l2);

	if (is_local_data())
		_data._local_size = ushort(l1 + l2);
	else
		_data._buffer_size_used = l1 + l2;
	return l2;
}

uint string_buffer::replace(cstr_t cstr, index beg, index end)
{
	if (is_empty() || cstr == null)
		return 0;

	if (beg >= end)
		return insert(cstr, beg);

	uint l1 = length();
	uint l2 = cstr.size();

	if (end > l1)
		end = l1;

	if (l2 == (end - beg))
	{
		str_t buff = str();
		for (index i = 0; i < l2; i++)
			buff[i + beg] = (char)cstr[i];
	}
	else if (capacity() <= (l1 + l2 + beg - end))
	{
		uint size = 32, new_size = l1 + l2 + beg - end;
		while (size <= new_size)
			size *= 2U;
		char* new_buffer = ANG_ALLOCATOR_NEW(char, size);
		new_size = algorithms::string_copy(new_buffer, this->cstr(), beg);
		new_size += algorithms::string_copy(&new_buffer[new_size], cstr, l2);
		new_size += algorithms::string_copy(&new_buffer[new_size], &this->cstr()[end]);

		clean();
		_data._buffer_size_used = new_size;
		_data._buffer_capacity = size - 1;
		_data._buffer_ptr = new_buffer;
		_data._flag_is_local_data = 0XFFFF;
	}
	else
	{
		uint new_size = l1 + l2 + beg - end;
		if (l1 < new_size)
		{
			algorithms::string_expand(str(), l1, beg, 2 * beg + l2 - end, capacity() + 1);
			algorithms::string_copy(&str()[beg], cstr, l2);
		}
		else
		{
			algorithms::string_contract(str(), l1, beg, end - l2);
			algorithms::string_copy(&str()[beg], cstr, l2);
		}
		if (is_local_data())
			_data._local_size = (ushort)new_size;
		else
			_data._buffer_size_used = new_size;
	}
	return l2;
}

uint string_buffer::replace(char c, index beg, index end)
{
	if (is_empty())
		return 0;

	if (beg >= end)
		return insert(c, beg);

	const char cstr[] = { c, 0 };
	uint l1 = length();
	uint l2 = 1;

	if (end > l1)
		end = l1;

	if (l2 == (end - beg))
	{
		str_t buff = str();
		for (uint i = 0; i < l2; i++)
			buff[i + beg] = (char)cstr[i];
	}
	else if (capacity() <= (l1 + l2 + beg - end))
	{
		uint size = 32, new_size = l1 + l2 + beg - end;
		while (size <= new_size)
			size *= 2U;
		char* new_buffer = ANG_ALLOCATOR_NEW(char, size);
		new_size = algorithms::string_copy(new_buffer, this->cstr(), beg);
		new_size += algorithms::string_copy(&new_buffer[new_size], cstr, l2);
		new_size += algorithms::string_copy(&new_buffer[new_size], &this->cstr()[end]);

		clean();
		_data._buffer_size_used = new_size;
		_data._buffer_capacity = size - 1;
		_data._buffer_ptr = new_buffer;
		_data._flag_is_local_data = 0XFFFF;
	}
	else
	{
		uint new_size = l1 + l2 + beg - end;
		if (l1 < new_size)
		{
			algorithms::string_expand(str(), l1, beg, 2 * beg + l2 - end, capacity() + 1);
			algorithms::string_copy(&str()[beg], cstr, l2);
		}
		else
		{
			algorithms::string_contract(str(), l1, beg, end - l2);
			algorithms::string_copy(&str()[beg], cstr, l2);
		}
		if (is_local_data())
			_data._local_size = (ushort)new_size;
		else
			_data._buffer_size_used = new_size;
	}
	return l2;
}

uint string_buffer::replace(cwstr_t cstr, index beg, index end)
{
	if (is_empty() || cstr == null)
		return 0;

	if (beg >= end)
		return insert(cstr, beg);

	uint l1 = length();
	uint l2 = cstr.size();

	if (end > l1)
		end = l1;

	if (l2 == (end - beg))
	{
		str_t buff = str();
		for (index i = 0; i < l2; i++)
			buff[i + beg] = (char)(byte)cstr[i];
	}
	else if (capacity() <= (l1 + l2 + beg - end))
	{
		uint size = 32, new_size = l1 + l2 + beg - end;
		while (size <= new_size)
			size *= 2U;
		char* new_buffer = ANG_ALLOCATOR_NEW(char, size);
		new_size = algorithms::string_copy(new_buffer, this->cstr(), beg);
		new_size += algorithms::string_copy(&new_buffer[new_size], cstr, l2);
		new_size += algorithms::string_copy(&new_buffer[new_size], &this->cstr()[end]);

		clean();
		_data._buffer_size_used = new_size;
		_data._buffer_capacity = size - 1;
		_data._buffer_ptr = new_buffer;
		_data._flag_is_local_data = 0XFFFF;
	}
	else
	{
		uint new_size = l1 + l2 + beg - end;
		if (l1 < new_size)
		{
			algorithms::string_expand(str(), l1, beg, 2 * beg + l2 - end, capacity() + 1);
			algorithms::string_copy(&str()[beg], cstr, l2);
		}
		else
		{
			algorithms::string_contract(str(), l1, beg, end - l2);
			algorithms::string_copy(&str()[beg], cstr, l2);
		}
		if (is_local_data())
			_data._local_size = (ushort)new_size;
		else
			_data._buffer_size_used = new_size;
	}
	return l2;
}

uint string_buffer::replace(wchar c, index beg, index end)
{
	if (is_empty())
		return 0;

	if (beg >= end)
		return insert(c, beg);

	const wchar cstr[] = { c, 0 };
	uint l1 = length();
	uint l2 = 1;

	if (end > l1)
		end = l1;

	if (l2 == (end - beg))
	{
		str_t buff = str();
		for (uint i = 0; i < l2; i++)
			buff[i + beg] = (char)(byte)cstr[i];
	}
	else if (capacity() <= (l1 + l2 + beg - end))
	{
		uint size = 32, new_size = l1 + l2 + beg - end;
		while (size <= new_size)
			size *= 2U;
		char* new_buffer = ANG_ALLOCATOR_NEW(char, size);
		new_size = algorithms::string_copy(new_buffer, this->cstr(), beg);
		new_size += algorithms::string_copy(&new_buffer[new_size], cstr, l2);
		new_size += algorithms::string_copy(&new_buffer[new_size], &this->cstr()[end]);

		clean();
		_data._buffer_size_used = new_size;
		_data._buffer_capacity = size - 1;
		_data._buffer_ptr = new_buffer;
		_data._flag_is_local_data = 0XFFFF;
	}
	else
	{
		uint new_size = l1 + l2 + beg - end;
		if (l1 < new_size)
		{
			algorithms::string_expand(str(), l1, beg, 2 * beg + l2 - end, capacity() + 1);
			algorithms::string_copy(&str()[beg], cstr, l2);
		}
		else
		{
			algorithms::string_contract(str(), l1, beg, end - l2);
			algorithms::string_copy(&str()[beg], cstr, l2);
		}
		if (is_local_data())
			_data._local_size = (ushort)new_size;
		else
			_data._buffer_size_used = new_size;
	}
	return l2;
}

index string_buffer::find(cstr_t _cstr, index _start, bool _rev)const
{
	return _start < length() ? ang::string_find<char, char>(cstr(), length(), _cstr, _cstr.size(), _start, _rev) : invalid_index;
}

index string_buffer::find(cstr_t _cstr, index _start, index _end, bool _rev)const
{
	return _start < length() ? ang::string_find<char, char>(cstr(), min<uint>(length(), _end), _cstr, _cstr.size(), _start, _rev) : invalid_index;
}

index string_buffer::find(cwstr_t _cstr, index _start, bool _rev)const
{
	return _start < length() ? ang::string_find<char, wchar>(cstr(), length(), _cstr, _cstr.size(), _start, _rev) : invalid_index;
}

index string_buffer::find(cwstr_t _cstr, index _start, index _end, bool _rev)const
{
	return _start < length() ? ang::string_find<char, wchar>(cstr(), min<uint>(length(), _end), _cstr, _cstr.size(), _start, _rev) : invalid_index;
}

uint string_buffer::sub_string(string& out, index start, uint count)const
{
	if (is_empty() )
		return 0;

	if (out.is_empty())
		out = new string_buffer();

	auto l1 = length();
	if (count > l1 || (count + start) > l1)
		count = l1 - start;

	if (!out->realloc(count, false))
		return 0;

	string_buffer* buffer = out.get();

	if (buffer->is_local_data())
		return (buffer->_data._local_size = (ushort)string_substr<char, char>(cstr(), l1, buffer->str(), start, count));
	else
		return (buffer->_data._buffer_size_used = string_substr<char, char>(cstr(), l1, buffer->str(), start, count));
}

uint string_buffer::sub_string(str_t out, index start, uint count)const
{
	if (is_empty())
		return 0;
	return string_substr<char, char>(cstr(), length(), out, start, count);
}

uint string_buffer::sub_string(wstring& out, index start, uint count)const
{
	if (is_empty())
		return 0;

	if (out.is_empty())
		out = new wstring_buffer();

	if (!out->realloc(count, false))
		return 0;

	wstring_buffer* buffer = out.get();

	if (buffer->is_local_data())
		return (buffer->_data._local_size = (ushort)string_substr<char, wchar>(cstr(), length(), buffer->str(), start, count));
	else
		return (buffer->_data._buffer_size_used = string_substr<char, wchar>(cstr(), length(), buffer->str(), start, count));
}

uint string_buffer::sub_string(wstr_t out, index start, uint count)const
{
	if (is_empty())
		return 0;
	return string_substr<char, wchar>(cstr(), length(), out, start, count);
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

	char* _beg = begin();
	char* _end = end();
	char temp;
	while (_beg < _end)
	{
		temp = *_beg;
		*_beg = *_end;
		*_end = temp;
		_end--;
		_beg++;
	}
}

void string_buffer::invert(index b, index e)
{
	if (is_empty() || length() <= e || e <= b)
		return;

	char* _beg = &(char&)b;
	char* _end = &(char&)e;
	char temp;
	while (_beg < _end)
	{
		temp = *_beg;
		*_beg = *_end;
		*_end = temp;
		_end--;
		_beg++;
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
		if ((buff[i] >= 97) && (buff[i] <= 122))
			buff[i] = char((long)buff[i] + 65 - 97 );
	}
}

void string_buffer::lowercase()
{
	if (is_empty())
		return;
	uint l = length();
	char* buff = str();
	for (uint i = 0; i < l; i++)
	{
		if ((buff[i] >= 65) && (buff[i] <= 90))
			buff[i] = char((long)buff[i] + 97 - 65);
	}
}

