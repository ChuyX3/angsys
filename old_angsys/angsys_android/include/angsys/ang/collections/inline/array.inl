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
	_allocator = memory::allocator_manager::get_allocator(0);
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
	_allocator = ar._allocator;
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
inline ang::collections::array_buffer<T>::array_buffer(ang::static_array<T> ar)
	: array_buffer()
{
	copy(ar);
}

template<typename T>
inline ang::collections::array_buffer<T>::array_buffer(ang::static_const_array<T> ar)
	: array_buffer()
{
	copy(ar);
}

template<typename T>
inline ang::collections::array_buffer<T>::~array_buffer()
{
	clean();
}

template<typename T>
inline ang::type_name_t ang::collections::array_buffer<T>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move("ang::collections::array<"_o + type_of<T>() + ">"_s));
	return name;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::is_child_of(ang::type_name_t name)
{
	if (name == type_of<array_buffer<T>>()
		|| ang::object::is_child_of(name)
		|| ang::collections::iarray<T>::is_child_of(name))
		return true;
	return false;
}

template<typename T>
inline ang::type_name_t ang::collections::array_buffer<T>::object_name()const
{
	return ang::collections::array_buffer<T>::class_name();
}

template<typename T>
inline bool ang::collections::array_buffer<T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_of<array_buffer<T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::iarray<T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (className == type_of<array_buffer<T>>())
	{
		*out = static_cast<ang::collections::array_buffer<T>*>(this);
		return true;
	}
	else if (ang::object::query_object(className, out))
	{
		return true;
	}
	else if (ang::collections::iarray<T>::query_object(className, out))
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
			_allocator->destruct<T>(&_data[i]);
		_size = 0U;
		_allocator->free(_data);
	}
	_data = null;
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
inline void ang::collections::array_buffer<T>::copy(ang::static_array<T> ar)
{
	size(ar.size());
	uint l = size();
	for (index i = 0U; i < l; i++)
		_data[i] = ar[i];
}

template<typename T>
inline void ang::collections::array_buffer<T>::copy(ang::static_const_array<T> ar)
{
	size(ar.size());
	uint l = size();
	for (index i = 0U; i < l; i++)
		_data[i] = ar[i];
}

template<typename T>
inline void ang::collections::array_buffer<T>::allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == _allocator)
		return;

	if (!alloc->can_delete_from(_allocator))
		realloc(alloc);

	_allocator = alloc;
}

template<typename T>
inline ang::memory::iallocator* ang::collections::array_buffer<T>::allocator()const
{
	return _allocator;
}

template<typename T>
inline pointer ang::collections::array_buffer<T>::buffer_ptr()const
{
	return (pointer)_data;
}

template<typename T>
inline wsize ang::collections::array_buffer<T>::buffer_size()const
{
	return _size * (wsize)sizeof(T);
}

template<typename T>
inline wsize ang::collections::array_buffer<T>::mem_copy(wsize _s, pointer _p)
{
//	throw(exception_t(except_code::unsupported));
	return 0;
}

template<typename T>
inline pointer ang::collections::array_buffer<T>::map_buffer(windex start, wsize size)
{
//	throw(exception_t(except_code::unsupported));
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
inline bool ang::collections::array_buffer<T>::move(ang::collections::array_buffer<T>& ar)
{
	if (&ar == this)
		return false;
	clean();
	_size = ar._size;
	_data = ar._data;
	_allocator = ar._allocator;
	ar._data = null;
	ar._size = 0;
	return true;
}

template<typename T>
inline void ang::collections::array_buffer<T>::copy(const ang::collections::ienum<T>* _items)
{
	if (_items == null)
		return;
	size(_items->counter());
	index i = 0;
	for (T const& value : *_items)
		_data[i++] = value;
}

template<typename T>
inline uint ang::collections::array_buffer<T>::counter()const
{
	return _size;
}

template<typename T>
inline T& ang::collections::array_buffer<T>::at(ang::collections::base_iterator<T>const& it)
{
#ifdef DEBUG_SAFE_CODE
	//if (is_empty())
	//	throw(exception_t(except_code::invalid_memory));
	//if (_data != it.current())
	//	throw(exception_t(except_code::invalid_param));
	//if (it.offset() >= _size)
	//	throw(exception_t(except_code::array_overflow));
#endif
	return _data[it.offset()];
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::array_buffer<T>::at(index idx)
{
	if (idx >= _size)
		return  iterator_t(const_cast<self_t*>(this), null, 0);
	return iterator_t(const_cast<self_t*>(this), (pointer)_data, idx);
}

template<typename T>
inline ang::collections::iterator<const T> ang::collections::array_buffer<T>::at(index idx)const
{
	if (idx >= _size)
		return  const_iterator_t(const_cast<self_t*>(this), null, 0);
	return const_iterator_t(const_cast<self_t*>(this), (pointer)_data, idx);
}

template<typename T>
inline index ang::collections::array_buffer<T>::find(T const& datum, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (auto i = size(); i > 0 ; --i)
		{
			if (_data[i - 1] == datum)
				return i - 1;
		}
		else for (auto i = 0U; i < size(); ++i)
		{
			if (_data[i] == datum)
				return i;
		}
	}
	return invalid_index;
}

template<typename T>
inline index ang::collections::array_buffer<T>::find(T const& datum, ang::collections::base_iterator<T> next_to, bool invert)const
{
	if (!is_empty() && next_to.parent() == this)
	{
		if (invert) for (auto i = next_to.offset() + 1; i > 0; --i)
		{
			if (_data[i - 1] == datum)
				return i - 1;
		}
		else for (auto i = next_to.offset(); i < size(); ++i)
		{
			if (_data[i] == datum)
				return i;
		}
	}
	return invalid_index;
}

template<typename T>
inline ang::collections::forward_iterator<T> ang::collections::array_buffer<T>::begin()
{
	return forward_iterator_t(const_cast<array_buffer*>(this), position_t(_data));
}

template<typename T>
inline ang::collections::forward_iterator<T> ang::collections::array_buffer<T>::end()
{
	return forward_iterator_t(const_cast<array_buffer*>(this), position_t(_data), _size);
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::array_buffer<T>::begin()const
{
	return const_forward_iterator_t(const_cast<array_buffer*>(this), position_t(_data));
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::array_buffer<T>::end()const
{
	return const_forward_iterator_t(const_cast<array_buffer*>(this), position_t(_data), _size);
}


template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::array_buffer<T>::rbegin()
{
	return backward_iterator_t(const_cast<array_buffer*>(this), position_t(_data), _size - 1);
}

template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::array_buffer<T>::rend()
{
	return backward_iterator_t(const_cast<array_buffer*>(this), position_t(_data), invalid_index);
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::array_buffer<T>::rbegin()const
{
	return const_backward_iterator_t(const_cast<array_buffer*>(this), position_t(_data), _size - 1);
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::array_buffer<T>::rend()const
{
	return const_backward_iterator_t(const_cast<array_buffer*>(this), position_t(_data), invalid_index);
}

template<typename T>
inline bool ang::collections::array_buffer<T>::increase(ang::collections::base_iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	//if (it.parent() != this || it.current() != _data)
	//	throw(exception_t(except_code::invalid_param));
	//if (it.offset() >= _size)
	//	throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + 1);
	if (it.offset() > _size) it.offset(_size);
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::increase(ang::collections::base_iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	//if (it.parent() != this || it.current() != _data)
	//	throw(exception_t(except_code::invalid_param));
	//if (it.offset() >= _size)
	//	throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + val);
	if (it.offset() >= _size) 
		it.offset(_size);
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::decrease(ang::collections::base_iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	//if (it.parent() != this || it.current() != _data)
	//	throw(exception_t(except_code::invalid_param));
	//if (it.offset() >= _size)
	//	throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() - 1);
	if ((int)it.offset() < -1 )
		it.offset(invalid_index);
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::decrease(ang::collections::base_iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	//if (it.parent() != this || it.current() != _data)
	//	throw(exception_t(except_code::invalid_param));
	//if (it.offset() >= _size)
	//	throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() - val);
	if ((int)it.offset() < -1)
		it.offset(invalid_index);
	return true;
}

template<typename T>
inline ang::comparision_result_t ang::collections::array_buffer<T>::compare(const ang::object& obj)const
{
	if (obj.is_kind_of(class_name()))
	{
		array_buffer<T>const& other = static_cast<array_buffer<T>const&>(obj);
		if(counter() > other.counter())
			return comparision_result::mayor;
		else if (counter() < other.counter())
			return comparision_result::minor;
		else for (index i = 0, c = counter(); i < c; ++i)
			if(_data[i] != other._data[i])
				return comparision_result::diferent;
		return comparision_result::same;
	}
	return comparision_result::diferent;
}


template<typename T>
inline bool ang::collections::array_buffer<T>::realloc(uint size)
{
	clean();
	_data = _allocator->alloc_object<T>(size, __FILE__, __LINE__);
	for (uint i = 0; i < size; ++i)
		_allocator->construct<T>(&_data[i]);
	if (is_empty())
		return false;
	_size = size;
	return true;
}

template<typename T>
inline bool ang::collections::array_buffer<T>::realloc(ang::memory::iallocator* alloc)
{
	uint sz = size();
	auto new_data = alloc->alloc_object<T>(sz, __FILE__, __LINE__);
	for (uint i = 0; i < sz; ++i)
		alloc->construct<T>(&new_data[i], ang::move(_data[i]));
	clean(); _data = new_data;
	if (is_empty()) return false;
	_size = sz;
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
inline ang::object_wrapper<ang::collections::array_buffer<T>>::object_wrapper(ang::static_array<T> arr)
	: object_wrapper<ang::collections::array_buffer<T>>()
{
	set(new collections::array_buffer<T>(arr));
}

template<typename T>
inline ang::object_wrapper<ang::collections::array_buffer<T>>::object_wrapper(ang::static_const_array<T> arr)
	: object_wrapper<ang::collections::array_buffer<T>>()
{
	set(new collections::array_buffer<T>(arr));
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
inline T const& ang::object_wrapper<ang::collections::array_buffer<T>>::operator[](int idx)const
{
#ifdef DEBUG_SAFE_CODE
//	if (is_empty()) throw(exception_t(except_code::invalid_memory));
//	if ((idx >= _ptr->size()) || (idx < 0U)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

template<typename T>
inline T & ang::object_wrapper<ang::collections::array_buffer<T>>::operator[](int idx)
{
#ifdef DEBUG_SAFE_CODE
//	if (is_empty()) throw(exception_t(except_code::invalid_memory));
//	if ((idx >= _ptr->size()) || (idx < 0U)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


#endif//__ARRAY_H__

