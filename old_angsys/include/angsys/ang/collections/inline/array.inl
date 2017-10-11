/*********************************************************************************************************************/
/*   File Name: ang/collections/inline/array.hpp                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_ARRAY_H__
#error Can't include array_buffer.hpp, please include angsys.h inside
#elif !defined __ANG_ARRAY_HPP__
#define __ANG_ARRAY_HPP__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// array_buffer<T> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::collections::array_buffer<T>::array_buffer()
	: object()
	, _size(0)
	, _data(null)
{
	allocator = memory::allocator_manager::get_allocator(0);
}

template<typename T>
inline ang::collections::array_buffer<T>::array_buffer(std::initializer_list<T> list)
	: array_buffer()
{
	uint c = 0;
	size((uint)list.size());
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		_data[c++] = *it;
	}
}

template<typename T>
inline ang::collections::array_buffer<T>::array_buffer(const std::nullptr_t&)
	: array_buffer()
{

}

template<typename T>
inline ang::collections::array_buffer<T>::array_buffer(ang::collections::array_buffer<T>&& ar)
	: array_buffer()
{
	_size = ar._size;
	_data = ar._data;
	allocator = ar.allocator;
	ar._size = 0;
	ar._data = null;
}

template<typename T>
inline ang::collections::array_buffer<T>::array_buffer(const ang::collections::array_buffer<T>& ar)
	: array_buffer()
{
	copy(&static_cast<const ienum_t&>(ar));
}

template<typename T>
inline ang::collections::array_buffer<T>::array_buffer(const ang::collections::array_buffer<T>* ar)
	: array_buffer()
{
	if (ar) copy(static_cast<const ienum_t*>(ar));
}

template<typename T>
inline ang::collections::array_buffer<T>::array_buffer(const ang::collections::ienum<T>* store)
	: array_buffer()
{
	copy(store);
}


template<typename T>
inline ang::collections::array_buffer<T>::array_buffer(uint sz, T const* ar)
	: array_buffer()
{
	if (ar != null)
		copy(sz, ar);
	else
		size(sz);
}

template<typename T>
inline ang::collections::array_buffer<T>::~array_buffer()
{
	clean();
}

template<typename T>
inline bool ang::collections::array_buffer<T>::is_empty()const
{
	return null == _data;
}

template<typename T>
inline T* ang::collections::array_buffer<T>::data()const
{
	if (_data)
		return _data;
	return null;
}

template<typename T>
inline uint ang::collections::array_buffer<T>::size()const
{
	return _size;
}

template<typename T>
inline void ang::collections::array_buffer<T>::size(uint size)
{
	if (size == _size)
		return;
	clean();
	if (size > 0U)
		realloc(size);
}

template<typename T>
inline bool ang::collections::array_buffer<T>::copy(uint s, T const* ar)
{
	size(s);
	uint l = size();
	for (index i = 0U; i < l; i++)
		_data[i] = ar[i];
	return true;
}

template<typename T>
inline void ang::collections::array_buffer<T>::set_allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == allocator)
		return;

	if (!alloc->can_delete_from(allocator))
		realloc(alloc);

	allocator = alloc;
}

template<typename T>
inline ang::memory::iallocator* ang::collections::array_buffer<T>::get_allocator()const
{
	return allocator;
}

template<typename T>
inline pointer ang::collections::array_buffer<T>::buffer_ptr()const
{
	return (void_ptr_t)_data;
}

template<typename T>
inline wsize ang::collections::array_buffer<T>::buffer_size()const
{
	return _size * (wsize)sizeof(T);
}

template<typename T>
inline wsize ang::collections::array_buffer<T>::mem_copy(wsize _s, pointer _p)
{
	throw(exception_t(except_code::unsupported));
	return 0;
}

template<typename T>
inline pointer ang::collections::array_buffer<T>::map_buffer(windex start, wsize size)
{
	throw(exception_t(except_code::unsupported));
	return null;
}

template<typename T>
void ang::collections::array_buffer<T>::unmap_buffer(pointer ptr, wsize used)
{
}

template<typename T>
inline bool ang::collections::array_buffer<T>::can_realloc_buffer()const { return false; };

template<typename T>
inline bool ang::collections::array_buffer<T>::realloc_buffer(wsize) { return false; };


template<typename T>
inline bool ang::collections::array_buffer<T>::has_items()const
{
	return bool(size() != 0);
}

template<typename T>
inline bool ang::collections::array_buffer<T>::move(ang::collections::array_buffer<T>& ar)
{
	if (&ar == this)
		return false;
	clean();
	_size = ar._size;
	_data = ar._data;
	allocator = ar.allocator;
	ar._data = null;
	ar._size = 0;
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::copy(const ang::collections::ienum<T>* _items)
{
	if (_items == null)
		return false;
	size(_items->counter());
	index_t i = 0;
	for (auto it = _items->begin(); it.is_valid(); ++it)
	{
		_data[i++] = const_cast<T&>(*it);
	}
	return true;
}

template<typename T>
inline T& ang::collections::array_buffer<T>::at(const ang::collections::iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw(exception_t(except_code::invalid_memory));
	if (_data != it.current())
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	return _data[it.offset()];
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::array_buffer<T>::find_index(index idx)const
{
	if (idx >= _size)
		return  iterator_t(const_cast<self_t*>(this), null, 0);
	return iterator_t(const_cast<self_t*>(this), (void_ptr_t)_data, idx);
}

template<typename T>
inline index ang::collections::array_buffer<T>::index_of(ang::collections::iterator<T> it)const
{
	if (!it.is_valid() || it.parent() != this)
		return invalid_index;
	return it.offset();
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::array_buffer<T>::find(T const& datum, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (auto i = size(); i > 0 ; --i)
		{
			if (_data[i - 1] == datum)
				return ang::collections::iterator<T>(const_cast<array_buffer<T>*>(this), (void*)_data, i - 1);
		}
		else for (auto i = 0U; i < size(); ++i)
		{
			if (_data[i] == datum)
				return ang::collections::iterator<T>(const_cast<array_buffer<T>*>(this), (void*)_data, i);
		}
	}
	return iterator_t(const_cast<self_t*>(this), 0);
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::array_buffer<T>::find(T const& datum, ang::collections::iterator<T> nextTo, bool invert)const
{
	if (!is_empty() && nextTo.parent() == this)
	{
		if (invert) for (auto i = nextTo.offset() + 1; i > 0; --i)
		{
			if (_data[i - 1] == datum)
				return ang::collections::iterator<T>(const_cast<array_buffer<T>*>(this), (void*)_data, i - 1);
		}
		else for (auto i = nextTo.offset(); i < size(); ++i)
		{
			if (_data[i] == datum)
				return ang::collections::iterator<T>(const_cast<array_buffer<T>*>(this), (void*)_data, i);
		}
	}
	return iterator_t(const_cast<array_buffer*>(this), 0);
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::array_buffer<T>::begin()const
{
	return iterator_t(const_cast<array_buffer*>(this), position_t((is_empty()) ? null : _data));
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::array_buffer<T>::end()const
{
	return iterator_t(const_cast<array_buffer*>(this), position_t(_data), _size - 1);
}

template<typename T>
inline bool ang::collections::array_buffer<T>::next(ang::collections::iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + 1);
	if (it.offset() == _size) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::next(ang::collections::iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + val);
	if (it.offset() == _size) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::prev(ang::collections::iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() - 1);
	if (it.offset() == uint(-1)) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::prev(ang::collections::iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset((uint(val) > it.offset())? uint (-1) : it.offset() - val);
	if (it.offset() == uint(-1)) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

template<typename T>
inline uint ang::collections::array_buffer<T>::counter()const
{
	return _size;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<array_buffer<T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::icollection<T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T>
inline ang::type_name_t ang::collections::array_buffer<T>::class_name()
{
	static ang::string className = ang::string("ang::collections::array<"_s) + type_name<T>() + ">"_s;
	return className->cstr();
}

template<typename T>
inline ang::type_name_t ang::collections::array_buffer<T>::object_name()const
{
	return ang::collections::array_buffer<T>::class_name();
}

template<typename T>
inline bool ang::collections::array_buffer<T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (className == type_name<array_buffer<T>>())
	{
		*out = static_cast<ang::collections::array_buffer<T>*>(this);
		return true;
	}
	else if (ang::object::query_object(className, out))
	{
		return true;
	}
	//else if (ang::collections::ienum<T>::query_object(className, out))
	//{
	//	return true;
	//}
	else if (ang::collections::icollection<T>::query_object(className, out))
	{
		return true;
	}
	return false;
}

template<typename T>
inline void ang::collections::array_buffer<T>::clean()
{
	if (_data != null)
	{
		for (uint i = 0; i < _size; ++i)
			allocator->destruct<T>(&_data[i]);
		_size = 0U;
		allocator->memory_release(_data);
	}
	_data = null;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::operator == (const ang::collections::array_buffer<T>& ar)
{
	if (size() != ar.size())
		return false;
	uint l = size();
	for (uint i = 0; i < l; i++)
		if (_data[i] != ar._data[i])
			return false;
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::operator != (const ang::collections::array_buffer<T>& ar)
{
	return !operator == (ar);
}

template<typename T>
inline ang::collections::array_buffer<T>::operator T*()const
{
	return _data;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::realloc(uint size)
{
	clean();
	_data = allocator->object_alloc<T>(size);
	for (uint i = 0; i < size; ++i)
		allocator->construct<T>(&_data[i]);
	if (is_empty())
		return false;
	_size = size;
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::realloc(ang::memory::iallocator* alloc)
{
	uint sz = size();
	auto new_data = alloc->object_alloc<T>(sz);
	for (uint i = 0; i < sz; ++i)
		alloc->construct<T>(&new_data[i], ang::move(_data[i]));
	clean(); _data = new_data;
	if (is_empty()) return false;
	_size = sz;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// array_buffer<object_wrapper<T>> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>::array_buffer()
	: object()
	, _size(0)
	, _data(null)
{
	allocator = memory::allocator_manager::get_allocator(0);
}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>::array_buffer(uint sz)
	: array_buffer()
{
	size(sz);
}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>::array_buffer(std::initializer_list<ang::object_wrapper<T>> list)
	: array_buffer()
{
	uint c = 0;
	size((uint)list.size());
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		get(c++) = *it;
	}
}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>::array_buffer(const ang::nullptr_t&)
	: array_buffer()
{

}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>::array_buffer(ang::collections::array_buffer<ang::object_wrapper<T>>&& ar)
	: array_buffer()
{
	_size = ar._size;
	_data = ar._data;
	allocator = ar.allocator;
	ar._size = 0;
	ar._data = null;
}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>::array_buffer(const ang::collections::array_buffer<ang::object_wrapper<T>>& ar)
	: array_buffer()
{
	copy(&static_cast<const ienum_t&>(ar));
}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>::array_buffer(const ang::collections::array_buffer<ang::object_wrapper<T>>* ar)
	: array_buffer()
{
	if (ar) copy(static_cast<const ienum_t*>(ar));
}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>::array_buffer(const ang::collections::ienum<ang::object_wrapper<T>>* store)
	: array_buffer()
{
	copy(store);
}


template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>::~array_buffer()
{
	clean();
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::is_empty()const
{
	return null == _data;
}

template<typename T>
inline ang::object_wrapper<T>* ang::collections::array_buffer<ang::object_wrapper<T>>::data()const
{
	return _data;
}

template<typename T>
inline uint ang::collections::array_buffer<ang::object_wrapper<T>>::size()const
{
	return _size;
}

template<typename T>
inline void ang::collections::array_buffer<ang::object_wrapper<T>>::size(uint size)
{
	if (size == _size)
		return;
	clean();
	if (size > 0U)
		realloc(size);
}

template<typename T>
inline ang::object_wrapper<T>& ang::collections::array_buffer<ang::object_wrapper<T>>::get(index idx)
{
	return _data[idx];
}

template<typename T>
inline ang::object_wrapper<T> const& ang::collections::array_buffer<ang::object_wrapper<T>>::get(index idx)const
{
	return _data[idx];
}

template<typename T>
inline void ang::collections::array_buffer<ang::object_wrapper<T>>::set_allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == allocator)
		return;

	if (!alloc->can_delete_from(allocator))
		realloc(alloc);

	allocator = alloc;
}

template<typename T>
inline ang::memory::iallocator* ang::collections::array_buffer<ang::object_wrapper<T>>::get_allocator()const
{
	return allocator;
}

template<typename T>
inline pointer ang::collections::array_buffer<ang::object_wrapper<T>>::buffer_ptr()const
{
	return (void_ptr_t)_data;
}

template<typename T>
inline wsize ang::collections::array_buffer<ang::object_wrapper<T>>::buffer_size()const
{
	return _size * (uint)sizeof(T);
}

template<typename T>
inline wsize ang::collections::array_buffer<ang::object_wrapper<T>>::mem_copy(wsize _s, pointer _p)
{
	throw(exception_t(except_code::unsupported));
	return 0;
}

template<typename T>
inline pointer ang::collections::array_buffer<ang::object_wrapper<T>>::map_buffer(windex start, wsize size)
{
	throw(exception_t(except_code::unsupported));
	return null;
}

template<typename T>
void ang::collections::array_buffer<ang::object_wrapper<T>>::unmap_buffer(pointer ptr, wsize used)
{
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::can_realloc_buffer()const { return false; };

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::realloc_buffer(wsize) { return false; };

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::has_items()const
{
	return bool(size() != 0);
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::move(ang::collections::array_buffer<ang::object_wrapper<T>>& ar)
{
	if (&ar == this)
		return false;
	clean();
	_size = ar._size;
	_data = ar._data;
	allocator = ar.allocator;
	ar._data = null;
	ar._size = 0;
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::copy(const ang::collections::ienum<ang::object_wrapper<T>>* _items)
{
	if (_items == null)
		return false;
	size(_items->counter());
	index_t i = 0;
	for (auto it = _items->begin(); it.is_valid(); ++it)
	{
		get(i) = const_cast<object_wrapper<T>&>(*it);
		i++;
	}
	return true;
}

template<typename T>
inline ang::object_wrapper<T>& ang::collections::array_buffer<ang::object_wrapper<T>>::at(const ang::collections::iterator<ang::object_wrapper<T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw(exception_t(except_code::invalid_memory));
	if (_data != it.current())
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	return const_cast<ang::object_wrapper<T>&>(get(it.offset()));
}

template<typename T>
inline ang::collections::iterator<ang::object_wrapper<T>> ang::collections::array_buffer<ang::object_wrapper<T>>::find_index(index idx)const
{
	if (idx >= _size)
		return  iterator_t(const_cast<self_t*>(this), null, 0);
	return iterator_t(const_cast<self_t*>(this), (void_ptr_t)_data, idx);
}

template<typename T>
inline index ang::collections::array_buffer<ang::object_wrapper<T>>::index_of(ang::collections::iterator<ang::object_wrapper<T>> it)const
{
	if (!it.is_valid() || it.parent() != this)
		return invalid_index;
	return it.offset();
}

template<typename T>
inline ang::collections::iterator<ang::object_wrapper<T>> ang::collections::array_buffer<ang::object_wrapper<T>>::find(ang::object_wrapper<T> const& datum, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (auto i = size(); i > 0; --i)
		{
			if (_data[i - 1].get() == datum.get())
				return ang::collections::iterator<ang::object_wrapper<T>>(const_cast<array_buffer<ang::object_wrapper<T>>*>(this), (void*)_data, i - 1);
		}
		else for (auto i = 0U; i < size(); ++i)
		{
			if (_data[i].get() == datum.get())
				return ang::collections::iterator<ang::object_wrapper<T>>(const_cast<array_buffer<ang::object_wrapper<T>>*>(this), (void*)_data, i);
		}
	}
	return iterator_t(const_cast<self_t*>(this), 0);
}

template<typename T>
inline ang::collections::iterator<ang::object_wrapper<T>> ang::collections::array_buffer<ang::object_wrapper<T>>::find(ang::object_wrapper<T> const& datum, ang::collections::iterator<ang::object_wrapper<T>> nextTo, bool invert)const
{
	if (!is_empty() && nextTo.parent() == this)
	{
		if (invert) for (auto i = nextTo.offset() + 1; i > 0; --i)
		{
			if (_data[i - 1].get() == datum.get())
				return ang::collections::iterator<ang::object_wrapper<T>>(const_cast<ang::collections::array_buffer<ang::object_wrapper<T>>*>(this), (void*)_data, i - 1);
		}
		else for (auto i = nextTo.offset(); i < size(); ++i)
		{
			if (_data[i].get() == datum.get())
				return ang::collections::iterator<ang::object_wrapper<T>>(const_cast<ang::collections::array_buffer<ang::object_wrapper<T>>*>(this), (void*)_data, i);
		}
	}
	return iterator_t(const_cast<array_buffer*>(this), 0);
}

template<typename T>
inline ang::collections::iterator<ang::object_wrapper<T>> ang::collections::array_buffer<ang::object_wrapper<T>>::begin()const
{
	return iterator_t(const_cast<array_buffer*>(this), position_t((is_empty()) ? null : _data));
}

template<typename T>
inline ang::collections::iterator<ang::object_wrapper<T>> ang::collections::array_buffer<ang::object_wrapper<T>>::end()const
{
	return iterator_t(const_cast<array_buffer*>(this), position_t(_data), _size - 1);
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::next(ang::collections::iterator<ang::object_wrapper<T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + 1);
	if (it.offset() == _size) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::next(ang::collections::iterator<ang::object_wrapper<T>>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + val);
	if (it.offset() == _size) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::prev(ang::collections::iterator<ang::object_wrapper<T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() - 1);
	if (it.offset() == uint(-1)) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::prev(ang::collections::iterator<ang::object_wrapper<T>>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset((uint(val) > it.offset()) ? uint(-1) : it.offset() - val);
	if (it.offset() == uint(-1)) {
		it.current(null);
		it.offset(0);
	}
	return true;
}

template<typename T>
inline uint ang::collections::array_buffer<ang::object_wrapper<T>>::counter()const
{
	return _size;
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<array_buffer<ang::object_wrapper<T>>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::icollection<object_wrapper<T>>::is_kind_of(name))
		return true;
	return false;
}

template<typename T>
inline ang::type_name_t ang::collections::array_buffer<ang::object_wrapper<T>>::class_name()
{
	static ang::string className = ang::string("ang::collections::array<"_s) + type_name<T>() + ">"_s;
	return className->cstr();
}

template<typename T>
inline ang::type_name_t ang::collections::array_buffer<ang::object_wrapper<T>>::object_name()const
{
	return ang::collections::array_buffer<ang::object_wrapper<T>>::class_name();
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (className == type_name<array_buffer<ang::object_wrapper<T>>>())
	{
		*out = static_cast<ang::collections::array_buffer<ang::object_wrapper<T>>*>(this);
		return true;
	}
	else if (ang::object::query_object(className, out))
	{
		return true;
	}
	//else if (ang::collections::ienum<object_wrapper<T>>::query_object(className, out))
	//{
	//	return true;
	//}
	else if (ang::collections::icollection<object_wrapper<T>>::query_object(className, out))
	{
		return true;
	}
	return false;
}

template<typename T>
inline void ang::collections::array_buffer<ang::object_wrapper<T>>::clean()
{
	if (_data != null)
	{
		for (uint i = 0; i < _size; ++i)
			allocator->destruct<object_wrapper<T>>(&get(i));
		_size = 0U;
		allocator->memory_release(_data);
	}
	_data = null;
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::operator == (const ang::collections::array_buffer<ang::object_wrapper<T>>& ar)
{
	if (size() != ar.size())
		return false;
	uint l = size();
	for (uint i = 0; i < l; i++)
		if (get(i) != ar.get(i))
			return false;
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::operator != (const ang::collections::array_buffer<ang::object_wrapper<T>>& ar)
{
	return !operator == (ar);
}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>::operator T**()const
{
	return _data;
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::realloc(uint size)
{
	clean();
	_data = allocator->object_alloc<object_wrapper<T>>(size);
	for (uint i = 0; i < size; ++i)
		allocator->construct<object_wrapper<T>>(&get(i));
	if (is_empty())
		return false;
	_size = size;
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<ang::object_wrapper<T>>::realloc(ang::memory::iallocator* alloc)
{
	uint sz = size();
	clean();
	_data = alloc->object_alloc<ang::object_wrapper<T>>(sz);
	for (uint i = 0; i < sz; ++i)
		alloc->construct<object_wrapper<T>>(&get(i));
	if (is_empty())
		return false;
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::object_wrapper(ang::collections::array_buffer<T>* ptr)
	: object_wrapper<ang::collections::array_buffer<T>>()
{
	set(ptr);
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper<ang::collections::array_buffer<T>>()
{
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::object_wrapper(std::initializer_list<data_type> list)
	: object_wrapper<ang::collections::array_buffer<T>>()
{
	set(new collections::array_buffer<T>(ang::move(list)));
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: object_wrapper<ang::collections::array_buffer<T>>()
{
	set(new collections::array_buffer<T>(store));
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::object_wrapper(uint size, data_type const* ar)
	: object_wrapper<ang::collections::array_buffer<T>>()
{
	set(new collections::array_buffer<T>(size, ar));
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<T>> && other)
	: object_wrapper<collections::array_buffer<T>>()
{
	collections::array_buffer<T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<T>> const& other)
	: object_wrapper<collections::array_buffer<T>>()
{
	set(other.get());
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::~object_wrapper()
{
	clean();
}

template<typename T>
inline void ang::object_wrapper<ang::collections::array_buffer<T>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::array_buffer<T>>::clean_unsafe()
{
	_ptr = null;
}

template<typename T>
inline bool ang::object_wrapper<ang::collections::array_buffer<T>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::collections::array_buffer<T>* ang::object_wrapper<ang::collections::array_buffer<T>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::array_buffer<T>>::set(ang::collections::array_buffer<T>* ptr)
{
	ang::collections::array_buffer<T> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T>
inline ang::collections::array_buffer<T>** ang::object_wrapper<ang::collections::array_buffer<T>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>& ang::object_wrapper<ang::collections::array_buffer<T>>::operator = (ang::collections::array_buffer<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>& ang::object_wrapper<ang::collections::array_buffer<T>>::operator = (const std::nullptr_t&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>& ang::object_wrapper<ang::collections::array_buffer<T>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (_ptr == null)
		set(new collections::array_buffer<T>(items));
	else
		_ptr->copy(items);
	return *this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>& ang::object_wrapper<ang::collections::array_buffer<T>>::operator = (ang::object_wrapper<ang::collections::array_buffer<T>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>& ang::object_wrapper<ang::collections::array_buffer<T>>::operator = (ang::object_wrapper<ang::collections::array_buffer<T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper_ptr<ang::collections::array_buffer<T>> ang::object_wrapper<ang::collections::array_buffer<T>>::operator & (void)
{
	return this;
}

template<typename T>
inline ang::collections::array_buffer<T> * ang::object_wrapper<ang::collections::array_buffer<T>>::operator -> (void)
{
	return get();
}

template<typename T>
inline ang::collections::array_buffer<T> const* ang::object_wrapper<ang::collections::array_buffer<T>>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::operator ang::collections::array_buffer<T> * (void)
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::operator ang::collections::array_buffer<T> const* (void)const
{
	return get();
}

template<typename T>
inline typename ang::collections::array_buffer<T>::type const& ang::object_wrapper<ang::collections::array_buffer<T>>::operator[](index idx)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= _ptr->size()) || (idx < 0U)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

template<typename T>
inline typename ang::collections::array_buffer<T>::type & ang::object_wrapper<ang::collections::array_buffer<T>>::operator[](index idx)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= _ptr->size()) || (idx < 0U)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::object_wrapper(uint sz)
	: _ptr(null)
{
	set(new collections::array_buffer<ang::object_wrapper<T>>(sz));
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::object_wrapper(ang::collections::array_buffer<ang::object_wrapper<T>>* ptr)
	: object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>()
{
	set(ptr);
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::object_wrapper(std::initializer_list<ang::object_wrapper<T>> list)
	: object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>()
{
	set(new collections::array_buffer<ang::object_wrapper<T>>(ang::move(list)));
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::object_wrapper(const ang::collections::ienum<ang::object_wrapper<T>>* store)
	: object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>()
{
	set(new collections::array_buffer<ang::object_wrapper<T>>(store));
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>> && other)
	: object_wrapper<collections::array_buffer<ang::object_wrapper<T>>>()
{
	object_t * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>> const& other)
	: object_wrapper<collections::array_buffer<ang::object_wrapper<T>>>()
{
	set(other.get());
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::~object_wrapper()
{
	clean();
}

template<typename T>
inline void ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::clean_unsafe()
{
	_ptr = null;
}

template<typename T>
inline bool ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>* ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::set(ang::collections::array_buffer<ang::object_wrapper<T>>* ptr)
{
	ang::collections::array_buffer<ang::object_wrapper<T>> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>>** ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>& ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator = (ang::collections::array_buffer<ang::object_wrapper<T>>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>& ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator = (const std::nullptr_t&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>& ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator = (ang::collections::ienum<ang::object_wrapper<T>> const* items)
{
	if (_ptr == null)
		set(new collections::array_buffer<ang::object_wrapper<T>>(items));
	else
		_ptr->copy(items);
	return *this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>& ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator = (ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>& ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator = (ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper_ptr<ang::collections::array_buffer<ang::object_wrapper<T>>> ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator & (void)
{
	return this;
}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>> * ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator -> (void)
{
	return get();
}

template<typename T>
inline ang::collections::array_buffer<ang::object_wrapper<T>> const* ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator ang::collections::array_buffer<ang::object_wrapper<T>> * (void)
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator ang::collections::array_buffer<ang::object_wrapper<T>> const* (void)const
{
	return get();
}

template<typename T>
inline ang::object_wrapper<T> const& ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator[](index idx)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= _ptr->size()) || (idx < 0U)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

template<typename T>
inline ang::object_wrapper<T> & ang::object_wrapper<ang::collections::array_buffer<ang::object_wrapper<T>>>::operator[](index idx)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= _ptr->size()) || (idx < 0U)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


#endif//__ARRAY_H__

