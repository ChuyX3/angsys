/*********************************************************************************************************************/
/*   File Name: array.cpp                                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include <angsys.h>

#include <ang/core/files.h>

using namespace ang;

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

ang::object_wrapper<ang::collections::array_buffer<object_t>>::object_wrapper()
	: _ptr(null)
{

}

ang::object_wrapper<ang::collections::array_buffer<object_t>>::object_wrapper(uint sz)
	: _ptr(null)
{
	set(new collections::array_buffer<object_t>(sz));
}


ang::object_wrapper<ang::collections::array_buffer<object_t>>::object_wrapper(ang::collections::array_buffer<object_t>* ptr)
	: object_wrapper<ang::collections::array_buffer<object_t>>()
{
	set(ptr);
}


ang::object_wrapper<ang::collections::array_buffer<object_t>>::object_wrapper(ang::initializer_list_t<object_t> list)
	: object_wrapper<ang::collections::array_buffer<object_t>>()
{
	set(new collections::array_buffer<object_t>(ang::move(list)));
}

ang::object_wrapper<ang::collections::array_buffer<object_t>>::object_wrapper(const ang::collections::ienum<object_t>* store)
	: object_wrapper<ang::collections::array_buffer<object_t>>()
{
	set(new collections::array_buffer<object_t>(store));
}


ang::object_wrapper<ang::collections::array_buffer<object_t>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<object_t>> && other)
	: object_wrapper<collections::array_buffer<object_t>>()
{
	collections::array_buffer<object_t> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}


ang::object_wrapper<ang::collections::array_buffer<object_t>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<object_t>> const& other)
	: object_wrapper<collections::array_buffer<object_t>>()
{
	set(other.get());
}

ang::object_wrapper<ang::collections::array_buffer<object_t>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper<collections::array_buffer<object_t>>() { }


ang::object_wrapper<ang::collections::array_buffer<object_t>>::~object_wrapper()
{
	clean();
}


void ang::object_wrapper<ang::collections::array_buffer<object_t>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}


void ang::object_wrapper<ang::collections::array_buffer<object_t>>::clean_unsafe()
{
	_ptr = null;
}


bool ang::object_wrapper<ang::collections::array_buffer<object_t>>::is_empty()const
{
	return _ptr == null;
}


ang::collections::array_buffer<object_t>* ang::object_wrapper<ang::collections::array_buffer<object_t>>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<ang::collections::array_buffer<object_t>>::set(ang::collections::array_buffer<object_t>* ptr)
{
	ang::collections::array_buffer<object_t> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}


ang::collections::array_buffer<object_t>** ang::object_wrapper<ang::collections::array_buffer<object_t>>::addres_of(void)
{
	return &_ptr;
}


ang::object_wrapper<ang::collections::array_buffer<object_t>>& ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator = (ang::collections::array_buffer<object_t>* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<object_t>>& ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator = (const ang::nullptr_t&)
{
	clean();
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<object_t>>& ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator = (ang::collections::ienum<object_t> const* items)
{
	if (_ptr == null)
		set(new collections::array_buffer<object_t>(items));
	else
		_ptr->copy(items);
	return *this;
}


ang::object_wrapper<ang::collections::array_buffer<object_t>>& ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator = (ang::object_wrapper<ang::collections::array_buffer<object_t>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<object_t>>& ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator = (ang::object_wrapper<ang::collections::array_buffer<object_t>> const& other)
{
	set(other._ptr);
	return*this;
}


ang::object_wrapper_ptr<ang::collections::array_buffer<object_t>> ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator & (void)
{
	return this;
}


ang::collections::array_buffer<object_t> * ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator -> (void)
{
	return get();
}


ang::collections::array_buffer<object_t> const* ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator -> (void)const
{
	return get();
}


ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator ang::collections::array_buffer<object_t> * (void)
{
	return get();
}


ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator ang::collections::array_buffer<object_t> const* (void)const
{
	return get();
}


object_t const& ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator[](int idx)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


object_t & ang::object_wrapper<ang::collections::array_buffer<object_t>>::operator[](int idx)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



ang::object_wrapper<ang::collections::array_buffer<string>>::object_wrapper()
	: _ptr(null)
{

}


ang::object_wrapper<ang::collections::array_buffer<string>>::object_wrapper(uint sz)
	: _ptr(null)
{
	set(new collections::array_buffer<string>(sz));
}


ang::object_wrapper<ang::collections::array_buffer<string>>::object_wrapper(ang::collections::array_buffer<string>* ptr)
	: object_wrapper<ang::collections::array_buffer<string>>()
{
	set(ptr);
}


ang::object_wrapper<ang::collections::array_buffer<string>>::object_wrapper(ang::initializer_list_t<string> list)
	: object_wrapper<ang::collections::array_buffer<string>>()
{
	set(new collections::array_buffer<string>(ang::move(list)));
}

ang::object_wrapper<ang::collections::array_buffer<string>>::object_wrapper(ang::initializer_list_t<cstr_t> list)
	: object_wrapper<ang::collections::array_buffer<string>>()
{
	set(new collections::array_buffer<string>((uint)list.size()));
	auto data = get()->data();
	int i = 0;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		data[i++] = *it;
	}
	
}

ang::object_wrapper<ang::collections::array_buffer<string>>::object_wrapper(ang::initializer_list_t<cwstr_t> list)
	: object_wrapper<ang::collections::array_buffer<string>>()
{
	set(new collections::array_buffer<string>((uint)list.size()));
	auto data = get()->data();
	int i = 0;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		data[i++] = *it;
	}
}


ang::object_wrapper<ang::collections::array_buffer<string>>::object_wrapper(const ang::collections::ienum<string>* store)
	: object_wrapper<ang::collections::array_buffer<string>>()
{
	set(new collections::array_buffer<string>(store));
}


ang::object_wrapper<ang::collections::array_buffer<string>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<string>> && other)
	: object_wrapper<collections::array_buffer<string>>()
{
	collections::array_buffer<string> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}


ang::object_wrapper<ang::collections::array_buffer<string>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<string>> const& other)
	: object_wrapper<collections::array_buffer<string>>()
{
	set(other.get());
}

ang::object_wrapper<ang::collections::array_buffer<string>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper<collections::array_buffer<string>>() { }


ang::object_wrapper<ang::collections::array_buffer<string>>::~object_wrapper()
{
	clean();
}


void ang::object_wrapper<ang::collections::array_buffer<string>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}


void ang::object_wrapper<ang::collections::array_buffer<string>>::clean_unsafe()
{
	_ptr = null;
}


bool ang::object_wrapper<ang::collections::array_buffer<string>>::is_empty()const
{
	return _ptr == null;
}


ang::collections::array_buffer<string>* ang::object_wrapper<ang::collections::array_buffer<string>>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<ang::collections::array_buffer<string>>::set(ang::collections::array_buffer<string>* ptr)
{
	ang::collections::array_buffer<string> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}


ang::collections::array_buffer<string>** ang::object_wrapper<ang::collections::array_buffer<string>>::addres_of(void)
{
	return &_ptr;
}


ang::object_wrapper<ang::collections::array_buffer<string>>& ang::object_wrapper<ang::collections::array_buffer<string>>::operator = (ang::collections::array_buffer<string>* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<string>>& ang::object_wrapper<ang::collections::array_buffer<string>>::operator = (const ang::nullptr_t&)
{
	clean();
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<string>>& ang::object_wrapper<ang::collections::array_buffer<string>>::operator = (ang::collections::ienum<string> const* items)
{
	if (_ptr == null)
		set(new collections::array_buffer<string>(items));
	else
		_ptr->copy(items);
	return *this;
}


ang::object_wrapper<ang::collections::array_buffer<string>>& ang::object_wrapper<ang::collections::array_buffer<string>>::operator = (ang::object_wrapper<ang::collections::array_buffer<string>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<string>>& ang::object_wrapper<ang::collections::array_buffer<string>>::operator = (ang::object_wrapper<ang::collections::array_buffer<string>> const& other)
{
	set(other._ptr);
	return*this;
}


ang::object_wrapper_ptr<ang::collections::array_buffer<string>> ang::object_wrapper<ang::collections::array_buffer<string>>::operator & (void)
{
	return this;
}


ang::collections::array_buffer<string> * ang::object_wrapper<ang::collections::array_buffer<string>>::operator -> (void)
{
	return get();
}


ang::collections::array_buffer<string> const* ang::object_wrapper<ang::collections::array_buffer<string>>::operator -> (void)const
{
	return get();
}


ang::object_wrapper<ang::collections::array_buffer<string>>::operator ang::collections::array_buffer<string> * (void)
{
	return get();
}


ang::object_wrapper<ang::collections::array_buffer<string>>::operator ang::collections::array_buffer<string> const* (void)const
{
	return get();
}


string const& ang::object_wrapper<ang::collections::array_buffer<string>>::operator[](int idx)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


string & ang::object_wrapper<ang::collections::array_buffer<string>>::operator[](int idx)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


ang::object_wrapper<ang::collections::array_buffer<wstring>>::object_wrapper()
	: _ptr(null)
{

}


ang::object_wrapper<ang::collections::array_buffer<wstring>>::object_wrapper(uint sz)
	: _ptr(null)
{
	set(new collections::array_buffer<wstring>(sz));
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>::object_wrapper(ang::collections::array_buffer<wstring>* ptr)
	: object_wrapper<ang::collections::array_buffer<wstring>>()
{
	set(ptr);
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>::object_wrapper(ang::initializer_list_t<wstring> list)
	: object_wrapper<ang::collections::array_buffer<wstring>>()
{
	set(new collections::array_buffer<wstring>(ang::move(list)));
}

ang::object_wrapper<ang::collections::array_buffer<wstring>>::object_wrapper(ang::initializer_list_t<cstr_t> list)
	: object_wrapper<ang::collections::array_buffer<wstring>>()
{
	set(new collections::array_buffer<wstring>((uint)list.size()));
	auto data = get()->data();
	int i = 0;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		data[i++] = *it;
	}

}

ang::object_wrapper<ang::collections::array_buffer<wstring>>::object_wrapper(ang::initializer_list_t<cwstr_t> list)
	: object_wrapper<ang::collections::array_buffer<wstring>>()
{
	set(new collections::array_buffer<wstring>((uint)list.size()));
	auto data = get()->data();
	int i = 0;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		data[i++] = *it;
	}
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>::object_wrapper(const ang::collections::ienum<wstring>* store)
	: object_wrapper<ang::collections::array_buffer<wstring>>()
{
	set(new collections::array_buffer<wstring>(store));
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<wstring>> && other)
	: object_wrapper<collections::array_buffer<wstring>>()
{
	collections::array_buffer<wstring> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<wstring>> const& other)
	: object_wrapper<collections::array_buffer<wstring>>()
{
	set(other.get());
}

ang::object_wrapper<ang::collections::array_buffer<wstring>>::object_wrapper(ang::nullptr_t const& other)
	: object_wrapper<collections::array_buffer<wstring>>() { }

ang::object_wrapper<ang::collections::array_buffer<wstring>>::~object_wrapper()
{
	clean();
}


void ang::object_wrapper<ang::collections::array_buffer<wstring>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}


void ang::object_wrapper<ang::collections::array_buffer<wstring>>::clean_unsafe()
{
	_ptr = null;
}


bool ang::object_wrapper<ang::collections::array_buffer<wstring>>::is_empty()const
{
	return _ptr == null;
}


ang::collections::array_buffer<wstring>* ang::object_wrapper<ang::collections::array_buffer<wstring>>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<ang::collections::array_buffer<wstring>>::set(ang::collections::array_buffer<wstring>* ptr)
{
	ang::collections::array_buffer<wstring> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}


ang::collections::array_buffer<wstring>** ang::object_wrapper<ang::collections::array_buffer<wstring>>::addres_of(void)
{
	return &_ptr;
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>& ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator = (ang::collections::array_buffer<wstring>* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>& ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator = (const ang::nullptr_t&)
{
	clean();
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>& ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator = (ang::collections::ienum<wstring> const* items)
{
	if (_ptr == null)
		set(new collections::array_buffer<wstring>(items));
	else
		_ptr->copy(items);
	return *this;
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>& ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator = (ang::object_wrapper<ang::collections::array_buffer<wstring>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>& ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator = (ang::object_wrapper<ang::collections::array_buffer<wstring>> const& other)
{
	set(other._ptr);
	return*this;
}


ang::object_wrapper_ptr<ang::collections::array_buffer<wstring>> ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator & (void)
{
	return this;
}


ang::collections::array_buffer<wstring> * ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator -> (void)
{
	return get();
}


ang::collections::array_buffer<wstring> const* ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator -> (void)const
{
	return get();
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator ang::collections::array_buffer<wstring> * (void)
{
	return get();
}


ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator ang::collections::array_buffer<wstring> const* (void)const
{
	return get();
}


wstring const& ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator[](int idx)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


wstring & ang::object_wrapper<ang::collections::array_buffer<wstring>>::operator[](int idx)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define _TYPE int

#define ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(_TYPE)	\
ang::collections::array_buffer<_TYPE>::array_buffer() : object() , _size(0) , _data(null) { \
	allocator = memory::allocator_manager::get_allocator(0); \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(ang::initializer_list_t<_TYPE> list) : array_buffer() { \
	uint c = 0; \
	size((uint)list.size()); \
	for (auto it = list.begin(); it != list.end(); ++it) { _data[c++] = *it; } \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(const ang::nullptr_t&) : array_buffer() { } \
ang::collections::array_buffer<_TYPE>::array_buffer(ang::collections::array_buffer<_TYPE>&& ar) : array_buffer() { \
	_size = ar._size; \
	_data = ar._data; \
	allocator = ar.allocator; \
	ar._size = 0; \
	ar._data = null; \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(const ang::collections::array_buffer<_TYPE>& ar) : array_buffer() { \
	copy(&static_cast<const ienum_t&>(ar)); \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(const ang::collections::array_buffer<_TYPE>* ar) : array_buffer() { \
	if (ar) copy(static_cast<const ienum_t*>(ar)); \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(const ang::collections::ienum<_TYPE>* store) : array_buffer() { \
	copy(store); \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(uint sz, _TYPE const* ar) : array_buffer() { \
	if (ar != null) copy(sz, ar); \
	else size(sz); \
} \
ang::collections::array_buffer<_TYPE>::~array_buffer() { clean(); } \
bool ang::collections::array_buffer<_TYPE>::is_empty()const { return null == _data; } \
_TYPE* ang::collections::array_buffer<_TYPE>::data()const { return _data; } \
uint ang::collections::array_buffer<_TYPE>::size()const { return _size; } \
void ang::collections::array_buffer<_TYPE>::size(uint size) { \
	if (size == _size) return; \
	clean(); \
	if (size > 0U) realloc(size); \
} \
bool ang::collections::array_buffer<_TYPE>::copy(uint s, _TYPE const* ar) { \
	size(s); \
	for (uint i = 0U; i < s; i++) _data[i] = ar[i]; \
	return true; \
} \
void ang::collections::array_buffer<_TYPE>::set_allocator(memory::iallocator* alloc) { \
	if (alloc == null && alloc == allocator) return; \
	if (!alloc->can_delete_from(allocator)) realloc(alloc); \
	allocator = alloc; \
} \
ang::memory::iallocator* ang::collections::array_buffer<_TYPE>::get_allocator()const { return allocator; } \
pointer ang::collections::array_buffer<_TYPE>::buffer_ptr()const { return (void_ptr_t)_data; } \
wsize ang::collections::array_buffer<_TYPE>::buffer_size()const { return _size * (wsize)sizeof(_TYPE); } \
wsize ang::collections::array_buffer<_TYPE>::mem_copy(wsize _s, pointer _p) {  \
	auto s = min((_s / sizeof(_TYPE)) * sizeof(_TYPE), _size * sizeof(_TYPE)); \
	memcpy(_data, _p, s); \
	return s; \
} \
pointer ang::collections::array_buffer<_TYPE>::map_buffer(windex start, wsize sz) { \
	if ((start + sz) >= size() * sizeof(_TYPE)) return null; \
	return (byte_t*)data() + start; \
} \
void ang::collections::array_buffer<_TYPE>::unmap_buffer(pointer ptr, wsize used) { \
} \
bool ang::collections::array_buffer<_TYPE>::can_realloc_buffer()const { return false; }; \
bool ang::collections::array_buffer<_TYPE>::realloc_buffer(wsize) { return false; }; \
bool ang::collections::array_buffer<_TYPE>::has_items()const { return bool(size() != 0); } \
bool ang::collections::array_buffer<_TYPE>::move(ang::collections::array_buffer<_TYPE>& ar) { \
	if (&ar == this) return false; \
	clean(); \
	_size = ar._size; \
	_data = ar._data; \
	allocator = ar.allocator; \
	ar._data = null; \
	ar._size = 0; \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::copy(const ang::collections::ienum<_TYPE>* _items) { \
	if (_items == null) return false; \
	size(_items->counter()); \
	index_t i = 0; \
	for (auto it = _items->begin(); it.is_valid(); ++it) _data[i++] = const_cast<_TYPE&>(*it); \
	return true; \
} \
_TYPE& ang::collections::array_buffer<_TYPE>::at(const ang::collections::iterator<_TYPE>& it)const { 	return _data[it.offset()]; } \
ang::collections::iterator<_TYPE> ang::collections::array_buffer<_TYPE>::find_index(index idx)const { \
	if (idx >= _size)	return  iterator_t(const_cast<self_t*>(this), null, 0); \
	return iterator_t(const_cast<self_t*>(this), (void_ptr_t)_data, idx); \
} \
index ang::collections::array_buffer<_TYPE>::index_of(ang::collections::iterator<_TYPE> it)const { \
	if (!it.is_valid() || it.parent() != this) return invalid_index; \
	return it.offset(); \
} \
ang::collections::iterator<_TYPE> ang::collections::array_buffer<_TYPE>::find(_TYPE const& datum, bool invert)const { \
	if (!is_empty()) { \
		if (invert) for (auto i = size(); i > 0; --i) { \
			if (_data[i - 1] == datum) return ang::collections::iterator<_TYPE>(const_cast<array_buffer<_TYPE>*>(this), (void*)_data, i - 1); \
		} \
		else for (auto i = 0U; i < size(); ++i) { \
			if (_data[i] == datum) return ang::collections::iterator<_TYPE>(const_cast<array_buffer<_TYPE>*>(this), (void*)_data, i); \
		} \
	} \
	return iterator_t(const_cast<self_t*>(this), 0); \
} \
ang::collections::iterator<_TYPE> ang::collections::array_buffer<_TYPE>::find(_TYPE const& datum, ang::collections::iterator<_TYPE> nextTo, bool invert)const { \
	if (!is_empty() && nextTo.parent() == this) { \
		if (invert) for (auto i = nextTo.offset() + 1; i > 0; --i) { \
			if (_data[i - 1] == datum) return ang::collections::iterator<_TYPE>(const_cast<array_buffer<_TYPE>*>(this), (void*)_data, i - 1);\
		} \
		else for (auto i = nextTo.offset(); i < size(); ++i) { \
			if (_data[i] == datum) return ang::collections::iterator<_TYPE>(const_cast<array_buffer<_TYPE>*>(this), (void*)_data, i); \
		} \
	} \
	return iterator_t(const_cast<array_buffer*>(this), 0); \
} \
ang::collections::iterator<_TYPE> ang::collections::array_buffer<_TYPE>::begin()const { return iterator_t(const_cast<array_buffer*>(this), position_t((is_empty()) ? null : _data)); } \
ang::collections::iterator<_TYPE> ang::collections::array_buffer<_TYPE>::end()const { return iterator_t(const_cast<array_buffer*>(this), position_t(_data), _size - 1); } \
bool ang::collections::array_buffer<_TYPE>::next(ang::collections::iterator<_TYPE>& it)const { \
	it.offset(it.offset() + 1); \
	if (it.offset() == _size) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::next(ang::collections::iterator<_TYPE>& it, int val)const { \
	it.offset(it.offset() + val); \
	if (it.offset() == _size) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::prev(ang::collections::iterator<_TYPE>& it)const { \
	it.offset(it.offset() - 1); \
	if (it.offset() == uint(-1)) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::prev(ang::collections::iterator<_TYPE>& it, int val)const { \
	it.offset((uint(val) > it.offset()) ? uint(-1) : it.offset() - val); \
	if (it.offset() == uint(-1)) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
uint ang::collections::array_buffer<_TYPE>::counter()const { return _size; } \
bool ang::collections::array_buffer<_TYPE>::is_kind_of(ang::type_name_t name)const { \
	return (name == type_name<array_buffer<_TYPE>>() || ang::object::is_kind_of(name) || ang::collections::icollection<_TYPE>::is_kind_of(name)); \
} \
ang::type_name_t ang::collections::array_buffer<_TYPE>::class_name() {return "ang::collections::array<"#_TYPE">"; } \
ang::type_name_t ang::collections::array_buffer<_TYPE>::object_name()const { return ang::collections::array_buffer<_TYPE>::class_name(); } \
bool ang::collections::array_buffer<_TYPE>::query_object(ang::type_name_t className, ang::unknown_ptr_t out) { \
	if (out == null) return false; \
	if (className == type_name<array_buffer<_TYPE>>()) { \
		*out = static_cast<ang::collections::array_buffer<_TYPE>*>(this); \
		return true; \
	} \
	else if (ang::object::query_object(className, out)) { return true; } \
	else if (ang::collections::icollection<_TYPE>::query_object(className, out)) { return true; } \
	return false; \
} \
void ang::collections::array_buffer<_TYPE>::clean() { \
	if (_data != null) allocator->memory_release(_data); \
	_data = null; \
} \
bool ang::collections::array_buffer<_TYPE>::operator == (const ang::collections::array_buffer<_TYPE>& ar) { \
	if (size() != ar.size()) return false; \
	uint l = size(); \
	for (uint i = 0; i < l; i++) \
		if (_data[i] != ar._data[i]) \
			return false; \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::operator != (const ang::collections::array_buffer<_TYPE>& ar) { return !operator == (ar); } \
ang::collections::array_buffer<_TYPE>::operator _TYPE*()const { return _data; } \
bool ang::collections::array_buffer<_TYPE>::realloc(uint size) { \
	clean(); \
	_data = allocator->object_alloc<_TYPE>(size); \
	if (is_empty()) return false; \
	_size = size; \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::realloc(ang::memory::iallocator* alloc) { \
	uint sz = size(); \
	auto new_data = alloc->object_alloc<_TYPE>(sz); \
	memcpy(new_data, _data, sz); \
	clean(); _data = new_data; \
	if (is_empty()) return false; \
	_size = sz; \
	return true; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper() \
	: _ptr(null) { } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(ang::collections::array_buffer<_TYPE>* ptr) \
	: object_wrapper<ang::collections::array_buffer<_TYPE>>() { set(ptr); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(ang::initializer_list_t<data_type> list) \
	: object_wrapper<ang::collections::array_buffer<_TYPE>>() { set(new collections::array_buffer<_TYPE>(ang::move(list))); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(const ang::collections::ienum<data_type>* store) \
	: object_wrapper<ang::collections::array_buffer<_TYPE>>() { set(new collections::array_buffer<_TYPE>(store)); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(uint size, data_type const* ar) \
	: object_wrapper<ang::collections::array_buffer<_TYPE>>() { set(new collections::array_buffer<_TYPE>(size, ar)); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<_TYPE>> && other) \
	: object_wrapper<collections::array_buffer<_TYPE>>() {  \
	collections::array_buffer<_TYPE> * temp = other._ptr; \
	other._ptr = null; \
	_ptr = temp; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<_TYPE>> const& other)  \
	: object_wrapper<collections::array_buffer<_TYPE>>() { set(other.get()); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::~object_wrapper() { clean(); } \
void ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::clean() { \
	if (_ptr)_ptr->release(); \
	_ptr = null; \
} \
void ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::clean_unsafe() { _ptr = null; } \
bool ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::is_empty()const { return _ptr == null; } \
ang::collections::array_buffer<_TYPE>* ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::get(void)const { return _ptr; } \
void ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::set(ang::collections::array_buffer<_TYPE>* ptr) { \
	ang::collections::array_buffer<_TYPE> * temp = _ptr; \
	if (ptr == _ptr) return; \
	_ptr = ptr; \
	if (_ptr)_ptr->add_ref(); \
	if (temp)temp->release(); \
} \
ang::collections::array_buffer<_TYPE>** ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::addres_of(void) { return &_ptr; } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator = (ang::collections::array_buffer<_TYPE>* ptr) { \
	set(ptr); return*this; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator = (const ang::nullptr_t&) { \
	clean(); \
	return*this; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator = (ang::collections::ienum<data_type> const* items) { \
	if (_ptr == null) set(new collections::array_buffer<_TYPE>(items)); \
	else _ptr->copy(items); \
	return *this; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator = (ang::object_wrapper<ang::collections::array_buffer<_TYPE>> && other) { \
	if (this == &other) return *this;\
	clean(); \
	_ptr = other._ptr; \
	other._ptr = null; \
	return*this; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator = (ang::object_wrapper<ang::collections::array_buffer<_TYPE>> const& other) { \
	set(other._ptr); \
	return*this; \
} \
ang::object_wrapper_ptr<ang::collections::array_buffer<_TYPE>> ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator & (void) { \
	return this; \
} \
ang::collections::array_buffer<_TYPE> * ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator -> (void) { return get(); } \
ang::collections::array_buffer<_TYPE> const* ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator -> (void)const { return get(); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator ang::collections::array_buffer<_TYPE> * (void) { return get(); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator ang::collections::array_buffer<_TYPE> const* (void)const { return get(); } \
typename ang::collections::array_buffer<_TYPE>::type const& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator[](int idx)const { return _ptr->data()[idx]; } \
typename ang::collections::array_buffer<_TYPE>::type & ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator[](int idx) { return _ptr->data()[idx]; }


#define ANG_IMPLEMENT_ARRAY_DATA_OBJECT_SPECIALIZATION(_TYPE)	\
ang::collections::array_buffer<_TYPE>::array_buffer() : object() , _size(0) , _data(null) { \
	allocator = memory::allocator_manager::get_allocator(0); \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(ang::initializer_list_t<_TYPE> list) : array_buffer() { \
	uint c = 0; \
	size((uint)list.size()); \
	for (auto it = list.begin(); it != list.end(); ++it) { _data[c++] = *it; } \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(const ang::nullptr_t&) : array_buffer() { } \
ang::collections::array_buffer<_TYPE>::array_buffer(ang::collections::array_buffer<_TYPE>&& ar) : array_buffer() { \
	_size = ar._size; \
	_data = ar._data; \
	allocator = ar.allocator; \
	ar._size = 0; \
	ar._data = null; \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(const ang::collections::array_buffer<_TYPE>& ar) : array_buffer() { \
	copy(&static_cast<const ienum_t&>(ar)); \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(const ang::collections::array_buffer<_TYPE>* ar) : array_buffer() { \
	if (ar) copy(static_cast<const ienum_t*>(ar)); \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(const ang::collections::ienum<_TYPE>* store) : array_buffer() { \
	copy(store); \
} \
ang::collections::array_buffer<_TYPE>::array_buffer(uint sz, _TYPE const* ar) : array_buffer() { \
	if (ar != null) copy(sz, ar); \
	else size(sz); \
} \
ang::collections::array_buffer<_TYPE>::~array_buffer() { clean(); } \
bool ang::collections::array_buffer<_TYPE>::is_empty()const { return null == _data; } \
_TYPE* ang::collections::array_buffer<_TYPE>::data()const { return _data; } \
uint ang::collections::array_buffer<_TYPE>::size()const { return _size; } \
void ang::collections::array_buffer<_TYPE>::size(uint size) { \
	if (size == _size) return; \
	clean(); \
	if (size > 0U) realloc(size); \
} \
bool ang::collections::array_buffer<_TYPE>::copy(uint s, _TYPE const* ar) { \
	size(s); \
	uint l = size(); \
	for (uint i = 0U; i < l; i++) _data[i] = ar[i]; \
	return true; \
} \
void ang::collections::array_buffer<_TYPE>::set_allocator(memory::iallocator* alloc) { \
	if (alloc == null && alloc == allocator) return; \
	if (!alloc->can_delete_from(allocator)) realloc(alloc); \
	allocator = alloc; \
} \
ang::memory::iallocator* ang::collections::array_buffer<_TYPE>::get_allocator()const { return allocator; } \
pointer ang::collections::array_buffer<_TYPE>::buffer_ptr()const { return (void_ptr_t)_data; } \
wsize ang::collections::array_buffer<_TYPE>::buffer_size()const { return _size * (wsize)sizeof(_TYPE); } \
wsize ang::collections::array_buffer<_TYPE>::mem_copy(wsize _s, pointer _p) {  \
	auto s = min((_s / sizeof(_TYPE)) * sizeof(_TYPE), _size * sizeof(_TYPE)); \
	memcpy(_data, _p, s); \
	return s; \
} \
pointer ang::collections::array_buffer<_TYPE>::map_buffer(windex start, wsize sz) { \
	if ((start + sz) >= size() * sizeof(_TYPE)) return null; \
	return (byte_t*)data() + start; \
} \
void ang::collections::array_buffer<_TYPE>::unmap_buffer(pointer ptr, wsize used) { \
} \
bool ang::collections::array_buffer<_TYPE>::can_realloc_buffer()const { return false; }; \
bool ang::collections::array_buffer<_TYPE>::realloc_buffer(wsize) { return false; }; \
bool ang::collections::array_buffer<_TYPE>::has_items()const { return bool(size() != 0); } \
bool ang::collections::array_buffer<_TYPE>::move(ang::collections::array_buffer<_TYPE>& ar) { \
	if (&ar == this) return false; \
	clean(); \
	_size = ar._size; \
	_data = ar._data; \
	allocator = ar.allocator; \
	ar._data = null; \
	ar._size = 0; \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::copy(const ang::collections::ienum<_TYPE>* _items) { \
	if (_items == null) return false; \
	size(_items->counter()); \
	index_t i = 0; \
	for (auto it = _items->begin(); it.is_valid(); ++it) _data[i++] = const_cast<_TYPE&>(*it); \
	return true; \
} \
_TYPE& ang::collections::array_buffer<_TYPE>::at(const ang::collections::iterator<_TYPE>& it)const { 	return _data[it.offset()]; } \
ang::collections::iterator<_TYPE> ang::collections::array_buffer<_TYPE>::find_index(index idx)const { \
	if (idx >= _size)	return  iterator_t(const_cast<self_t*>(this), null, 0); \
	return iterator_t(const_cast<self_t*>(this), (void_ptr_t)_data, idx); \
} \
index ang::collections::array_buffer<_TYPE>::index_of(ang::collections::iterator<_TYPE> it)const { \
	if (!it.is_valid() || it.parent() != this) return invalid_index; \
	return it.offset(); \
} \
ang::collections::iterator<_TYPE> ang::collections::array_buffer<_TYPE>::find(_TYPE const& datum, bool invert)const { \
	if (!is_empty()) { \
		if (invert) for (auto i = size(); i > 0; --i) { \
			if (_data[i - 1] == datum) return ang::collections::iterator<_TYPE>(const_cast<array_buffer<_TYPE>*>(this), (void*)_data, i - 1); \
		} \
		else for (auto i = 0U; i < size(); ++i) { \
			if (_data[i] == datum) return ang::collections::iterator<_TYPE>(const_cast<array_buffer<_TYPE>*>(this), (void*)_data, i); \
		} \
	} \
	return iterator_t(const_cast<self_t*>(this), 0); \
} \
ang::collections::iterator<_TYPE> ang::collections::array_buffer<_TYPE>::find(_TYPE const& datum, ang::collections::iterator<_TYPE> nextTo, bool invert)const { \
	if (!is_empty() && nextTo.parent() == this) { \
		if (invert) for (auto i = nextTo.offset() + 1; i > 0; --i) { \
			if (_data[i - 1] == datum) return ang::collections::iterator<_TYPE>(const_cast<array_buffer<_TYPE>*>(this), (void*)_data, i - 1);\
		} \
		else for (auto i = nextTo.offset(); i < size(); ++i) { \
			if (_data[i] == datum) return ang::collections::iterator<_TYPE>(const_cast<array_buffer<_TYPE>*>(this), (void*)_data, i); \
		} \
	} \
	return iterator_t(const_cast<array_buffer*>(this), 0); \
} \
ang::collections::iterator<_TYPE> ang::collections::array_buffer<_TYPE>::begin()const { return iterator_t(const_cast<array_buffer*>(this), position_t((is_empty()) ? null : _data)); } \
ang::collections::iterator<_TYPE> ang::collections::array_buffer<_TYPE>::end()const { return iterator_t(const_cast<array_buffer*>(this), position_t(_data), _size - 1); } \
bool ang::collections::array_buffer<_TYPE>::next(ang::collections::iterator<_TYPE>& it)const { \
	it.offset(it.offset() + 1); \
	if (it.offset() == _size) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::next(ang::collections::iterator<_TYPE>& it, int val)const { \
	it.offset(it.offset() + val); \
	if (it.offset() == _size) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::prev(ang::collections::iterator<_TYPE>& it)const { \
	it.offset(it.offset() - 1); \
	if (it.offset() == uint(-1)) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::prev(ang::collections::iterator<_TYPE>& it, int val)const { \
	it.offset((uint(val) > it.offset()) ? uint(-1) : it.offset() - val); \
	if (it.offset() == uint(-1)) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
uint ang::collections::array_buffer<_TYPE>::counter()const { return _size; } \
bool ang::collections::array_buffer<_TYPE>::is_kind_of(ang::type_name_t name)const { \
	return (name == type_name<array_buffer<_TYPE>>() || ang::object::is_kind_of(name) || ang::collections::icollection<_TYPE>::is_kind_of(name)); \
} \
ang::type_name_t ang::collections::array_buffer<_TYPE>::class_name() {return "ang::collections::array<"#_TYPE">"; } \
ang::type_name_t ang::collections::array_buffer<_TYPE>::object_name()const { return ang::collections::array_buffer<_TYPE>::class_name(); } \
bool ang::collections::array_buffer<_TYPE>::query_object(ang::type_name_t className, ang::unknown_ptr_t out) { \
	if (out == null) return false; \
	if (className == type_name<array_buffer<_TYPE>>()) { \
		*out = static_cast<ang::collections::array_buffer<_TYPE>*>(this); \
		return true; \
	} \
	else if (ang::object::query_object(className, out)) { return true; } \
	else if (ang::collections::icollection<_TYPE>::query_object(className, out)) { return true; } \
	return false; \
} \
void ang::collections::array_buffer<_TYPE>::clean() { \
	if (_data != null) allocator->memory_release(_data); \
	_data = null; \
} \
bool ang::collections::array_buffer<_TYPE>::operator == (const ang::collections::array_buffer<_TYPE>& ar) { \
	if (size() != ar.size()) return false; \
	uint l = size(); \
	for (uint i = 0; i < l; i++) \
		if (_data[i] != ar._data[i]) \
			return false; \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::operator != (const ang::collections::array_buffer<_TYPE>& ar) { return !operator == (ar); } \
ang::collections::array_buffer<_TYPE>::operator _TYPE*()const { return _data; } \
bool ang::collections::array_buffer<_TYPE>::realloc(uint size) { \
	clean(); \
	_data = allocator->object_alloc<_TYPE>(size); \
	for(index i = 0; i < size; i++) { allocator->construct<_TYPE>(&_data[i]); } \
	if (is_empty()) return false; \
	_size = size; \
	return true; \
} \
bool ang::collections::array_buffer<_TYPE>::realloc(ang::memory::iallocator* alloc) { \
	uint sz = size(); \
	auto new_data = alloc->object_alloc<_TYPE>(sz); \
	for(index i = 0; i < sz; i++) { alloc->construct<_TYPE, _TYPE&&>(&new_data[i], ang::move(_data[i])); } \
	clean(); _data = new_data; \
	if (is_empty()) return false; \
	_size = sz; \
	return true; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper() \
	: _ptr(null) { } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(ang::collections::array_buffer<_TYPE>* ptr) \
	: object_wrapper<ang::collections::array_buffer<_TYPE>>() { set(ptr); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(ang::initializer_list_t<data_type> list) \
	: object_wrapper<ang::collections::array_buffer<_TYPE>>() { set(new collections::array_buffer<_TYPE>(ang::move(list))); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(const ang::collections::ienum<data_type>* store) \
	: object_wrapper<ang::collections::array_buffer<_TYPE>>() { set(new collections::array_buffer<_TYPE>(store)); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(uint size, data_type const* ar) \
	: object_wrapper<ang::collections::array_buffer<_TYPE>>() { set(new collections::array_buffer<_TYPE>(size, ar)); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<_TYPE>> && other) \
	: object_wrapper<collections::array_buffer<_TYPE>>() {  \
	collections::array_buffer<_TYPE> * temp = other._ptr; \
	other._ptr = null; \
	_ptr = temp; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<_TYPE>> const& other)  \
	: object_wrapper<collections::array_buffer<_TYPE>>() { set(other.get()); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::~object_wrapper() { clean(); } \
void ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::clean() { \
	if (_ptr)_ptr->release(); \
	_ptr = null; \
} \
void ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::clean_unsafe() { _ptr = null; } \
bool ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::is_empty()const { return _ptr == null; } \
ang::collections::array_buffer<_TYPE>* ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::get(void)const { return _ptr; } \
void ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::set(ang::collections::array_buffer<_TYPE>* ptr) { \
	ang::collections::array_buffer<_TYPE> * temp = _ptr; \
	if (ptr == _ptr) return; \
	_ptr = ptr; \
	if (_ptr)_ptr->add_ref(); \
	if (temp)temp->release(); \
} \
ang::collections::array_buffer<_TYPE>** ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::addres_of(void) { return &_ptr; } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator = (ang::collections::array_buffer<_TYPE>* ptr) { \
	set(ptr); return*this; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator = (const ang::nullptr_t&) { \
	clean(); \
	return*this; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator = (ang::collections::ienum<data_type> const* items) { \
	if (_ptr == null) set(new collections::array_buffer<_TYPE>(items)); \
	else _ptr->copy(items); \
	return *this; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator = (ang::object_wrapper<ang::collections::array_buffer<_TYPE>> && other) { \
	if (this == &other) return *this;\
	clean(); \
	_ptr = other._ptr; \
	other._ptr = null; \
	return*this; \
} \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator = (ang::object_wrapper<ang::collections::array_buffer<_TYPE>> const& other) { \
	set(other._ptr); \
	return*this; \
} \
ang::object_wrapper_ptr<ang::collections::array_buffer<_TYPE>> ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator & (void) { \
	return this; \
} \
ang::collections::array_buffer<_TYPE> * ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator -> (void) { return get(); } \
ang::collections::array_buffer<_TYPE> const* ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator -> (void)const { return get(); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator ang::collections::array_buffer<_TYPE> * (void) { return get(); } \
ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator ang::collections::array_buffer<_TYPE> const* (void)const { return get(); } \
typename ang::collections::array_buffer<_TYPE>::type const& ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator[](int idx)const { return _ptr->data()[idx]; } \
typename ang::collections::array_buffer<_TYPE>::type & ang::object_wrapper<ang::collections::array_buffer<_TYPE>>::operator[](int idx) { return _ptr->data()[idx]; }



ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(char)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(mchar)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(wchar)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(short)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(ushort)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(int)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(uint)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(long)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(ulong)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(long64)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(ulong64)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(float)
ANG_IMPLEMENT_ARRAY_DATA_SPECIALIZATION(double)

//ANG_IMPLEMENT_ARRAY_DATA_OBJECT_SPECIALIZATION(ang::core::files::pack_file_info)