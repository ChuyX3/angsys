#ifndef __ANG_VECTOR_H__
#error Can't include vector.hpp, please include vector.h inside
#elif !defined __ANG_VECTOR_HPP__
#define __ANG_VECTOR_HPP__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// vector_buffer<T> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::collections::vector_buffer<T>::vector_buffer()
	: object()
	, _size(0)
	, _capacity(0)
	, _data(null)
{
	_allocator = memory::allocator_manager::get_allocator(0);
}

template<typename T>
inline ang::collections::vector_buffer<T>::vector_buffer(ang::initializer_list_t<T> list)
	: vector_buffer()
{
	capacity((uint)list.size());
	for (auto it = list.begin(); it != list.end(); ++it)
		push(*it);
}

template<typename T>
inline ang::collections::vector_buffer<T>::vector_buffer(const ang::nullptr_t&)
	: vector_buffer()
{

}

template<typename T>
inline ang::collections::vector_buffer<T>::vector_buffer(ang::collections::vector_buffer<T>&& ar)
	: vector_buffer()
{
	_size = ar._size;
	_data = ar._data;
	_capacity = ar._capacity;
	_allocator = ar._allocator;
	ar._size = 0;
	ar._capacity = 0;
	ar._data = null;
}

template<typename T>
inline ang::collections::vector_buffer<T>::vector_buffer(const ang::collections::vector_buffer<T>& ar)
	: vector_buffer()
{
	copy(static_cast<const ienum_t&>(ar));
}

template<typename T>
inline ang::collections::vector_buffer<T>::vector_buffer(const ang::collections::vector_buffer<T>* ar)
	: vector_buffer()
{
	if (ar) copy(static_cast<const ienum_t*>(ar));
}

template<typename T>
inline ang::collections::vector_buffer<T>::vector_buffer(const ang::collections::ienum<T>* store)
	: vector_buffer()
{
	copy(store);
}


template<typename T>
inline ang::collections::vector_buffer<T>::vector_buffer(ang::static_array<T> arr)
	: vector_buffer()
{
	copy(arr);
}

template<typename T>
inline ang::collections::vector_buffer<T>::vector_buffer(ang::static_const_array<T> arr)
	: vector_buffer()
{
	copy(arr);
}

template<typename T>
inline ang::collections::vector_buffer<T>::~vector_buffer()
{
	clean();
}

template<typename T>
inline ang::type_name_t ang::collections::vector_buffer<T>::class_name()
{
	static type_name_t  name = runtime_data_base::regist_typename(ang::move("ang::collections::vector<"_o + type_of<T>() + ">"_s));
	return name;
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::is_child_of(ang::type_name_t name)
{
	if (name == type_of<vector_buffer<T>>()
		|| ang::object::is_child_of(name)
		|| ang::collections::ilist<T>::is_child_of(name))
		return true;
	return false;
}

template<typename T>
inline ang::type_name_t ang::collections::vector_buffer<T>::object_name()const
{
	return ang::collections::vector_buffer<T>::class_name();
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_of<vector_buffer<T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::ilist<T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (className == type_of<vector_buffer<T>>())
	{
		*out = static_cast<ang::collections::vector_buffer<T>*>(this);
		return true;
	}
	else if (ang::object::query_object(className, out))
	{
		return true;
	}
	else if (ang::collections::ilist<T>::query_object(className, out))
	{
		return true;
	}
	return false;
}


template<typename T>
inline bool ang::collections::vector_buffer<T>::is_empty()const
{
	return size() == 0;
}

template<typename T>
inline T* ang::collections::vector_buffer<T>::data()const
{
	if (_data)
		return _data;
	return null;
}

template<typename T>
inline uint ang::collections::vector_buffer<T>::size()const
{
	return _size;
}

template<typename T>
inline uint ang::collections::vector_buffer<T>::capacity()const
{
	return _capacity;
}

template<typename T>
inline void ang::collections::vector_buffer<T>::capacity(uint size, bool save)
{
	realloc(size, save);
}

template<typename T>
inline void ang::collections::vector_buffer<T>::copy(ang::static_array<T> ar)
{
	capacity(ar.size());
	uint l = ar.size();
	for (uint i = 0U; i < l; i++)
		push(ar[i]);
}

template<typename T>
inline void ang::collections::vector_buffer<T>::copy(ang::static_const_array<T> ar)
{
	capacity(ar.size());
	uint l = ar.size();
	for (uint i = 0U; i < l; i++)
		push(ar[i]);
}

template<typename T>
inline void ang::collections::vector_buffer<T>::extend(ang::static_array<T> ar)
{
	capacity(ar.size() + size(), true);
	uint l = ar.size();
	for (uint i = 0U; i < l; i++)
		push(ar[i]);
}

template<typename T>
inline void ang::collections::vector_buffer<T>::extend(ang::static_const_array<T> ar)
{
	capacity(ar.size() + size(), true);
	uint l = ar.size();
	for (uint i = 0U; i < l; i++)
		push(ar[i]);
}


template<typename T>
inline void ang::collections::vector_buffer<T>::allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == _allocator)
		return;

	if (!alloc->can_delete_from(_allocator))
		realloc(alloc);

	_allocator = alloc;
}

template<typename T>
inline ang::memory::iallocator* ang::collections::vector_buffer<T>::allocator()const
{
	return _allocator;
}

template<typename T>
inline pointer ang::collections::vector_buffer<T>::buffer_ptr()const
{
	return (pointer)_data;
}

template<typename T>
inline wsize ang::collections::vector_buffer<T>::buffer_size()const
{
	return _capacity * sizeof(T);
}

template<typename T>
inline wsize ang::collections::vector_buffer<T>::mem_copy(wsize _s, pointer _p)
{
	//throw(exception_t(except_code::unsupported));
	return 0;
}

template<typename T>
inline pointer ang::collections::vector_buffer<T>::map_buffer(windex start, wsize size)
{
	//throw(exception_t(except_code::unsupported));
	return null;
}

template<typename T>
void ang::collections::vector_buffer<T>::unmap_buffer(pointer ptr, wsize used)
{
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::can_realloc_buffer()const { return true; };

template<typename T>
inline bool ang::collections::vector_buffer<T>::realloc_buffer(wsize size) { return realloc(size, true); };

template<typename T>
inline bool ang::collections::vector_buffer<T>::move(ang::collections::vector_buffer<T>& ar)
{
	if (&ar == this)
		return false;
	clean();
	_size = ar._size;
	_data = ar._data;
	_capacity = ar._capacity;
	_allocator = ar._allocator;
	ar._data = null;
	ar._size = 0;
	ar._capacity = 0;
	return true;
}

template<typename T>
inline void ang::collections::vector_buffer<T>::copy(const ang::collections::ienum<T>* _items)
{
	if (!_items)
		return;
	capacity(_items->counter(), false);
	for (T const& value : *_items)
		push(value);	
}


template<typename T>
inline uint ang::collections::vector_buffer<T>::counter()const
{
	return _size;
}

template<typename T>
inline T& ang::collections::vector_buffer<T>::at(ang::collections::base_iterator<T> const& it)
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
inline ang::collections::iterator<T> ang::collections::vector_buffer<T>::at(index idx)
{
	if (idx >= _size)
		return  iterator_t(const_cast<self_t*>(this), null, 0);
	return iterator_t(const_cast<self_t*>(this), (pointer)_data, idx);
}

template<typename T>
inline ang::collections::iterator<const T> ang::collections::vector_buffer<T>::at(index idx)const
{
	if (idx >= _size)
		return  const_iterator_t(const_cast<self_t*>(this), null, 0);
	return const_iterator_t(const_cast<self_t*>(this), (pointer)_data, idx);
}

template<typename T>
inline index ang::collections::vector_buffer<T>::find(T const& datum, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (auto i = size(); i > 0; --i)
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
inline index ang::collections::vector_buffer<T>::find(T const& datum, ang::collections::base_iterator<T> next_to, bool invert)const
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
inline ang::collections::forward_iterator<T> ang::collections::vector_buffer<T>::begin()
{
	return forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data));
}

template<typename T>
inline ang::collections::forward_iterator<T> ang::collections::vector_buffer<T>::end()
{
	return forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size);
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::vector_buffer<T>::begin()const
{
	return const_forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data));
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::vector_buffer<T>::end()const
{
	return const_forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size);
}

template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::vector_buffer<T>::rbegin()
{
	return backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1);
}

template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::vector_buffer<T>::rend()
{
	return backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), invalid_index);
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::vector_buffer<T>::rbegin()const
{
	return const_backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1);
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::vector_buffer<T>::rend()const
{
	return const_backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), invalid_index);
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::increase(ang::collections::base_iterator<T>& it)const
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
inline bool ang::collections::vector_buffer<T>::increase(ang::collections::base_iterator<T>& it, int val)const
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
inline bool ang::collections::vector_buffer<T>::decrease(ang::collections::base_iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	//if (it.parent() != this || it.current() != _data)
	//	throw(exception_t(except_code::invalid_param));
	//if (it.offset() >= _size)
	//	throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() - 1);
	if ((int)it.offset() < -1)
		it.offset(invalid_index);
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::decrease(ang::collections::base_iterator<T>& it, int val)const
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
inline ang::comparision_result_t ang::collections::vector_buffer<T>::compare(const ang::object& obj)const
{
	if (obj.is_kind_of(class_name()))
	{
		vector_buffer<T>const& other = static_cast<vector_buffer<T>const&>(obj);
		if (counter() > other.counter())
			return comparision_result::mayor;
		else if (counter() < other.counter())
			return comparision_result::minor;
		else for (index i = 0, c = counter(); i < c; ++i)
			if (_data[i] != other._data[i])
				return comparision_result::diferent;
		return comparision_result::same;
	}
	return comparision_result::diferent;
}


template<typename T>
inline void ang::collections::vector_buffer<T>::extend(const ang::collections::ienum<T>* items)
{
	if (!items) return;
	capacity(counter() + items->counter());
	for (T const& value : *items)
		push(value);
}

template<typename T>
inline void ang::collections::vector_buffer<T>::push(T const& value, bool last)
{
	if ((capacity() - counter()) <= 1U) //TODO: optimization
		capacity(capacity() + 1, true);

	if (last)
	{	
		_allocator->template construct<T, T const&>(&_data[_size++], value);
	}
	else
	{
		for (auto i = _size; i > 0U; --i)
		{
			_allocator->construct<T, T&&>(&_data[i], ang::move(_data[i - 1U]));
			_allocator->destruct<T>(&_data[i - 1U]);
		}
		_allocator->construct<T, T const&>(&_data[0], value);
		_size++;
	}
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::insert(index idx, T const& value)
{
	if (idx == 0U)
	{
		push(value, false);
		return true;
	}
	else if (idx >= _size)
	{
		push(value, true);
		return true;
	}

	if ((capacity() - counter()) <= 1U) //TODO: optimization
		capacity(capacity() + 1, true);

	for (auto i = _size; i > idx; --i)
	{
		_allocator->construct<T, T&&>(&_data[i], ang::move(_data[i - 1U]));
		_allocator->destruct<T>(&_data[i - 1U]);
	}
	_allocator->construct<T, T const&>(&_data[idx], value);
	_size++;
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::insert(ang::collections::base_iterator<T> it, T const& value)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return insert(it.offset(), value);
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::pop(bool last)
{
	if (_size == 0)
		return false;
	if (!last) for (index i = 1U; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);
	_allocator->destruct((T*)&(_data[--_size]));
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::pop(T& value, bool last)
{
	if (_size == 0)
		return false;
	if (!last)
	{
		value = ang::move(_data[0]);
		for (index i = 1U; i < _size; ++i)
			_data[i - 1] = ang::move(_data[i]);
	}
	else
	{
		value = ang::move(_data[_size - 1U]);
	}
	_allocator->destruct((T*)&(_data[--_size]));
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::pop_at(index idx)
{
	if (_size == 0U)
		return false;
	if (idx == 0U)
		return pop(false);
	for (index i = idx; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);
	_allocator->destruct((T*)&(_data[--_size]));
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::pop_at(ang::collections::base_iterator<T> it)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return pop_at(it.offset());
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::pop_at(index idx, T& value)
{
	if (_size == 0U)
		return false;
	if (idx == 0U)
		return pop(value, false);
	value = ang::move(_data[0]);
	for (index i = idx; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);
	_allocator->destruct((T*)&(_data[--_size]));
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::pop_at(ang::collections::base_iterator<T> it, T& value)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return pop_at(it.offset(), value);
}

template<typename T>
inline void ang::collections::vector_buffer<T>::clean()
{
	if (_data != null)
	{
		for (uint i = 0; i < _size; ++i)
			_allocator->destruct<T>(&_data[i]);
		_size = 0U;
		_capacity = 0U;
		_allocator->free(_data);
	}
	_data = null;
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::realloc(uint new_size, bool save)
{
	new_size++;
	if (capacity() >= new_size)
		return true;

	uint sz = 8U, temp = 0U;
	while (sz <= new_size)
		sz *= 2U;
	T* new_buffer = _allocator->alloc_object<T>(sz, __FILE__, __LINE__);

	if (save)
	{
		temp = _size;
		for (index i = 0U; i < _size; ++i)
		{
			_allocator->template construct<T, T&&>(&new_buffer[i], ang::move(_data[i]));
			_allocator->template destruct<T>(&_data[i]);
		}
		_allocator->free(_data);
	}
	else
	{
		clean();
	}
	_data = new_buffer;
	_size = temp;
	_capacity = sz;
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<T>::realloc(ang::memory::iallocator* alloc)
{
	if (capacity() == 0U)
		return true;

	T* new_buffer = alloc->alloc_object<T>(_capacity, __FILE__, __LINE__);
	for (index i = 0U; i < _size; ++i)
	{
		alloc->template construct<T, T&&>(&new_buffer[i], ang::move(_data[i]));
		_allocator->destruct(&_data[i]);
	}
	_allocator->free(_data);
	_data = new_buffer;
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::object_wrapper(ang::nullptr_t const&)
	: _ptr(null)
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::object_wrapper(ang::collections::vector_buffer<T>* ptr)
	: object_wrapper<ang::collections::vector_buffer<T>>()
{
	set(ptr);
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::object_wrapper(ang::initializer_list_t<T> list)
	: object_wrapper<ang::collections::vector_buffer<T>>()
{
	set(new collections::vector_buffer<T>(ang::move(list)));
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::object_wrapper(const ang::collections::ienum<T>* store)
	: object_wrapper<ang::collections::vector_buffer<T>>()
{
	set(new collections::vector_buffer<T>(store));
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::object_wrapper(ang::static_array<T> arr)
	: object_wrapper<ang::collections::vector_buffer<T>>()
{
	set(new collections::vector_buffer<T>(arr));
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::object_wrapper(ang::static_const_array<T> arr)
	: object_wrapper<ang::collections::vector_buffer<T>>()
{
	set(new collections::vector_buffer<T>(arr));
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<T>> && other)
	: object_wrapper<collections::vector_buffer<T>>()
{
	collections::vector_buffer<T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<T>> const& other)
	: object_wrapper<collections::vector_buffer<T>>()
{
	set(other.get());
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::~object_wrapper()
{
	clean();
}

template<typename T>
inline void ang::object_wrapper<ang::collections::vector_buffer<T>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::vector_buffer<T>>::clean_unsafe()
{
	_ptr = null;
}

template<typename T>
inline bool ang::object_wrapper<ang::collections::vector_buffer<T>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::collections::vector_buffer<T>* ang::object_wrapper<ang::collections::vector_buffer<T>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::vector_buffer<T>>::set(ang::collections::vector_buffer<T>* ptr)
{
	ang::collections::vector_buffer<T> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T>
inline ang::collections::vector_buffer<T>** ang::object_wrapper<ang::collections::vector_buffer<T>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>& ang::object_wrapper<ang::collections::vector_buffer<T>>::operator = (ang::collections::vector_buffer<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>& ang::object_wrapper<ang::collections::vector_buffer<T>>::operator = (const ang::nullptr_t&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>& ang::object_wrapper<ang::collections::vector_buffer<T>>::operator = (ang::collections::ienum<T> const* items)
{
	if (_ptr == null)
		set(new collections::vector_buffer<T>(items));
	else
		_ptr->copy(items);
	return *this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>& ang::object_wrapper<ang::collections::vector_buffer<T>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<T>> && other)
{
	collections::vector_buffer<T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>& ang::object_wrapper<ang::collections::vector_buffer<T>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>& ang::object_wrapper<ang::collections::vector_buffer<T>>::operator += (T const& value)
{
	if (is_empty())
		set(new ang::collections::vector_buffer<T>());
	get()->push(value);
	return*this;
}

template<typename T>
inline ang::object_wrapper_ptr<ang::collections::vector_buffer<T>> ang::object_wrapper<ang::collections::vector_buffer<T>>::operator & (void)
{
	return this;
}

template<typename T>
inline ang::collections::vector_buffer<T> * ang::object_wrapper<ang::collections::vector_buffer<T>>::operator -> (void)
{
	return get();
}

template<typename T>
inline ang::collections::vector_buffer<T> const* ang::object_wrapper<ang::collections::vector_buffer<T>>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::operator ang::collections::vector_buffer<T> * (void)
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::vector_buffer<T>>::operator ang::collections::vector_buffer<T> const* (void)const
{
	return get();
}

template<typename T>
inline T const& ang::object_wrapper<ang::collections::vector_buffer<T>>::operator[](int idx)const
{
#ifdef DEBUG_SAFE_CODE
	//if (is_empty()) throw(exception_t(except_code::invalid_memory));
	//if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

template<typename T>
inline T& ang::object_wrapper<ang::collections::vector_buffer<T>>::operator[](int idx)
{
#ifdef DEBUG_SAFE_CODE
	//if (is_empty()) throw(exception_t(except_code::invalid_memory));
	//if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

#endif//__ANG_VECTOR_INL__
