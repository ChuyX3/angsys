#ifndef __ANG_VECTOR_HPP__
#error Can't include vector.inl, please include vector.hpp inside
#elif !defined __ANG_VECTOR_INL__
#define __ANG_VECTOR_INL__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// vector_buffer<T, allocator> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, template<typename> class allocator>
inline ang::collections::vector_buffer<T, allocator>::vector_buffer()
	: object()
	, _size(0)
	, _capacity(0)
	, _data(null)
{

}

template<typename T, template<typename> class allocator>  template<typename U>
inline ang::collections::vector_buffer<T, allocator>::vector_buffer(ang::initializer_list_t<U> list)
	: vector_buffer()
{
	capacity((wsize)list.size());
	for (auto it = list.begin(); it != list.end(); ++it)
		push(*it);
}


template<typename T, template<typename> class allocator>
inline ang::collections::vector_buffer<T, allocator>::vector_buffer(const ang::nullptr_t&)
	: vector_buffer()
{

}

template<typename T, template<typename> class allocator>
inline ang::collections::vector_buffer<T, allocator>::vector_buffer(ang::collections::vector_buffer<T, allocator>&& ar)
	: vector_buffer()
{
	_size = ar._size;
	_data = ar._data;
	_capacity = ar._capacity;
	ar._size = 0;
	ar._capacity = 0;
	ar._data = null;
}

template<typename T, template<typename> class allocator>
inline ang::collections::vector_buffer<T, allocator>::vector_buffer(const ang::collections::vector_buffer<T, allocator>& ar)
	: vector_buffer()
{
	copy(to_array(ar.data(), ar.size()));
}

template<typename T, template<typename> class allocator>
inline ang::collections::vector_buffer<T, allocator>::vector_buffer(const ang::collections::vector_buffer<T, allocator>* ar)
	: vector_buffer()
{
	if (ar)copy(to_array(ar->data(), ar->size()));
}

template<typename T, template<typename> class allocator>
inline ang::collections::vector_buffer<T, allocator>::vector_buffer(const ang::collections::ienum<T>* store)
	: vector_buffer()
{
	copy(store);
}


template<typename T, template <typename> class allocator> template<typename U>
inline ang::collections::vector_buffer<T, allocator>::vector_buffer(ang::array_view<U> const& ar)
	: vector_buffer()
{
	copy(ar);
}

template<typename T, template <typename> class allocator> template<typename U, template<typename> class allocator2>
inline ang::collections::vector_buffer<T, allocator>::vector_buffer(ang::scope_array<U, allocator2> const& ar)
	: vector_buffer()
{
	copy(ar);
}

template<typename T, template<typename> class allocator>
inline ang::collections::vector_buffer<T, allocator>::~vector_buffer()
{
	clean();
}

template<typename T, template <typename> class allocator>
inline ang::type_name_t ang::collections::vector_buffer<T, allocator>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::collections::vector<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::is_inherited_of(ang::type_name_t name)
{
	if (name == type_of<vector_buffer<T, allocator>>()
		|| ang::object::is_inherited_of(name)
		|| ang::collections::ilist<T>::is_inherited_of(name))
		return true;
	return false;
}

template<typename T, template<typename> class allocator>
inline ang::type_name_t ang::collections::vector_buffer<T, allocator>::object_name()const
{
	return ang::collections::vector_buffer<T, allocator>::class_name();
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_of<vector_buffer<T, allocator>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::ilist<T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (className == type_of<vector_buffer<T, allocator>>())
	{
		*out = static_cast<ang::collections::vector_buffer<T, allocator>*>(this);
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


template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::is_empty()const
{
	return size() == 0;
}

template<typename T, template<typename> class allocator>
inline T* ang::collections::vector_buffer<T, allocator>::data()const
{
	return _data;
}

template<typename T, template<typename> class allocator>
inline wsize ang::collections::vector_buffer<T, allocator>::size()const
{
	return _size;
}

template<typename T, template<typename> class allocator>
inline wsize ang::collections::vector_buffer<T, allocator>::capacity()const
{
	return _capacity;
}

template<typename T, template<typename> class allocator>
inline void ang::collections::vector_buffer<T, allocator>::capacity(wsize size, bool save)
{
	realloc(size, save);
}

template<typename T, template<typename> class allocator> template<typename U>
inline void ang::collections::vector_buffer<T, allocator>::copy(ang::array_view<U> const& ar)
{
	capacity(ar.size());
	wsize l = ar.size();
	for (wsize i = 0U; i < l; i++)
		push(ar[i]);
}

template<typename T, template<typename> class allocator>  template<typename U, template<typename> class allocator2>
inline void ang::collections::vector_buffer<T, allocator>::copy(ang::scope_array<U, allocator2> const& ar)
{
	capacity(ar.size());
	wsize l = ar.size();
	for (wsize i = 0U; i < l; i++)
		push(ar[i]);
}

template<typename T, template<typename> class allocator>  template<typename U, wsize SIZE>
inline void ang::collections::vector_buffer<T, allocator>::copy(ang::stack_array<U, SIZE> const& ar)
{
	capacity(SIZE);
	for (wsize i = 0U; i < SIZE; i++)
		push(ar[i]);
}

template<typename T, template<typename> class allocator> template<typename U>
inline void ang::collections::vector_buffer<T, allocator>::expand(ang::array_view<U> const& ar)
{
	capacity(size() + ar.size());
	wsize l = ar.size();
	for (wsize i = 0U; i < l; i++)
		push(ar[i]);
}

template<typename T, template<typename> class allocator>  template<typename U, template<typename> class allocator2>
inline void ang::collections::vector_buffer<T, allocator>::expand(ang::scope_array<U, allocator2> const& ar)
{
	capacity(size() + ar.size());
	wsize l = ar.size();
	for (wsize i = 0U; i < l; i++)
		push(ar[i]);
}

template<typename T, template<typename> class allocator>  template<typename U, wsize SIZE>
inline void ang::collections::vector_buffer<T, allocator>::expand(ang::stack_array<U, SIZE> const& ar)
{
	capacity(size() + SIZE);
	for (wsize i = 0U; i < SIZE; i++)
		push(ar[i]);
}

template<typename T, template<typename> class allocator>
inline ang::text::encoding_t ang::collections::vector_buffer<T, allocator>::encoding()const
{
	return text::encoding::binary;
}

template<typename T, template<typename> class allocator>
inline pointer ang::collections::vector_buffer<T, allocator>::buffer_ptr()const
{
	return (pointer)_data;
}

template<typename T, template<typename> class allocator>
inline wsize ang::collections::vector_buffer<T, allocator>::buffer_size()const
{
	return _capacity * sizeof(T);
}

template<typename T, template<typename> class allocator>
inline wsize ang::collections::vector_buffer<T, allocator>::mem_copy(wsize _s, pointer _p, text::encoding_t)
{
	throw(exception_t(except_code::unsupported));
	return 0;
}

template<typename T, template<typename> class allocator>
inline ang::ibuffer_view_t ang::collections::vector_buffer<T, allocator>::map_buffer(windex start, wsize size)
{
	return null;
}

template<typename T, template<typename> class allocator>
bool ang::collections::vector_buffer<T, allocator>::unmap_buffer(ang::ibuffer_view_t&, wsize used)
{
	return false;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::can_realloc_buffer()const { return true; };

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::realloc_buffer(wsize size) { return realloc(size, true); };

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::move(ang::collections::vector_buffer<T, allocator>& ar)
{
	if (&ar == this)
		return false;
	clean();
	_size = ar._size;
	_data = ar._data;
	_capacity = ar._capacity;
	ar._data = null;
	ar._size = 0;
	ar._capacity = 0;
	return true;
}

template<typename T, template<typename> class allocator>
inline void ang::collections::vector_buffer<T, allocator>::copy(const ang::collections::ienum<T>* _items)
{
	if (!_items)
		return;
	capacity(_items->counter(), false);
	for (T const& value : *_items)
		push(value);	
}


template<typename T, template<typename> class allocator>
inline wsize ang::collections::vector_buffer<T, allocator>::counter()const
{
	return _size;
}

template<typename T, template<typename> class allocator>
inline T& ang::collections::vector_buffer<T, allocator>::at(ang::collections::base_iterator<T> const& it)
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

template<typename T, template<typename> class allocator>
inline ang::collections::iterator<T> ang::collections::vector_buffer<T, allocator>::at(windex idx)
{
	if (idx >= _size)
		return  iterator_t(const_cast<self_t*>(this), null, 0);
	return iterator_t(const_cast<self_t*>(this), (pointer)_data, idx);
}

template<typename T, template<typename> class allocator>
inline ang::collections::iterator<const T> ang::collections::vector_buffer<T, allocator>::at(windex idx)const
{
	if (idx >= _size)
		return  const_iterator_t(const_cast<self_t*>(this), null, 0);
	return const_iterator_t(const_cast<self_t*>(this), (pointer)_data, idx);
}

template<typename T, template<typename> class allocator>
inline ang::collections::iterator<T> ang::collections::vector_buffer<T, allocator>::find(T const& datum, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (auto i = size(); i > 0; --i)
		{
			if (comparision_operations<T, T>::template compare<comparision_same>(_data[i - 1], datum))
				return at(i - 1);
		}
		else for (auto i = 0U; i < size(); ++i)
		{
			if (comparision_operations<T, T>::template compare<comparision_same>(_data[i] , datum))
				return at(i);
		}
	}
	return at(invalid_index);
}

template<typename T, template<typename> class allocator>
inline ang::collections::iterator<T> ang::collections::vector_buffer<T, allocator>::find(T const& datum, ang::collections::base_iterator<T> next_to, bool invert)const
{
	if (!is_empty() && next_to.parent() == this)
	{
		if (invert) for (auto i = next_to.offset() + 1; i > 0; --i)
		{
			if (comparision_operations<T, T>::template compare<comparision_same>(_data[i - 1] , datum))
				return at(i - 1);
		}
		else for (auto i = next_to.offset(); i < size(); ++i)
		{
			if (comparision_operations<T, T>::template compare<comparision_same>(_data[i] , datum))
				return at(i);
		}
	}
	return at(invalid_index);
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<T> ang::collections::vector_buffer<T, allocator>::begin()
{
	return forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data));
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<T> ang::collections::vector_buffer<T, allocator>::end()
{
	return forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size);
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<const T> ang::collections::vector_buffer<T, allocator>::begin()const
{
	return const_forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data));
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<const T> ang::collections::vector_buffer<T, allocator>::end()const
{
	return const_forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size);
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<T> ang::collections::vector_buffer<T, allocator>::last()
{
	return _size ? forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1) : end();
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<const T> ang::collections::vector_buffer<T, allocator>::last()const
{
	return _size ? const_forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1) : end();
}

template<typename T, template<typename> class allocator>
inline ang::collections::backward_iterator<T> ang::collections::vector_buffer<T, allocator>::rbegin()
{
	return backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1);
}

template<typename T, template<typename> class allocator>
inline ang::collections::backward_iterator<T> ang::collections::vector_buffer<T, allocator>::rend()
{
	return backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), invalid_index);
}

template<typename T, template<typename> class allocator>
inline ang::collections::backward_iterator<const T> ang::collections::vector_buffer<T, allocator>::rbegin()const
{
	return const_backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1);
}

template<typename T, template<typename> class allocator>
inline ang::collections::backward_iterator<const T> ang::collections::vector_buffer<T, allocator>::rend()const
{
	return const_backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), invalid_index);
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::increase(ang::collections::base_iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + 1);
	if (it.offset() > _size) it.offset(_size);
	return true;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::increase(ang::collections::base_iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + val);
	if (it.offset() >= _size)
		it.offset(_size);
	return true;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::decrease(ang::collections::base_iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() - 1);
	if ((int)it.offset() < -1)
		it.offset(invalid_index);
	return true;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::decrease(ang::collections::base_iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() - val);
	if ((int)it.offset() < -1)
		it.offset(invalid_index);
	return true;
}

template<typename T, template<typename> class allocator>
inline ang::comparision_result_t ang::collections::vector_buffer<T, allocator>::compare(const ang::object& obj)const
{
	if (obj.is_kind_of(class_name()))
	{
		vector_buffer<T, allocator>const& other = static_cast<vector_buffer<T, allocator>const&>(obj);
		if (counter() > other.counter())
			return comparision_result::mayor;
		else if (counter() < other.counter())
			return comparision_result::minor;
		else for (windex i = 0, c = counter(); i < c; ++i)
			if (comparision_operations<T, T>::template compare<comparision_diferent>(_data[i], other._data[i]))
				return comparision_result::diferent;
		return comparision_result::same;
	}
	return comparision_result::diferent;
}


template<typename T, template<typename> class allocator>
inline void ang::collections::vector_buffer<T, allocator>::extend(const ang::collections::ienum<T>* items)
{
	if (!items) return;
	capacity(counter() + items->counter());
	for (T const& value : *items)
		push(value);
}

template<typename T, template<typename> class allocator>
inline void ang::collections::vector_buffer<T, allocator>::push(T const& value, bool last)
{
	if ((capacity() - counter()) <= 1U) //TODO: optimization
		capacity(capacity() + 1, true);

	if (last)
	{	
		allocator<T>::template construct<T const&>((T*)&_data[_size++], value);
	}
	else
	{
		for (auto i = _size; i > 0U; --i)
		{
			allocator<T>::template construct<T&&>((T*)&_data[i], ang::move(_data[i - 1U]));
			allocator<T>::destruct((T*)&_data[i - 1U]);
		}
		allocator<T>::template construct<T const&>((T*)&_data[0], value);
		_size++;
	}
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::insert(windex idx, T const& value)
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
		allocator<T>::template construct<T&&>((T*)&_data[i], ang::move(_data[i - 1U]));
		allocator<T>::destruct((T*)&_data[i - 1U]);
	}
	allocator<T>::template construct<T const&>((T*)&_data[idx], value);
	_size++;
	return true;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::insert(ang::collections::base_iterator<T> it, T const& value)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return insert(it.offset(), value);
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::pop(bool last)
{
	if (_size == 0)
		return false;
	if (!last) for (windex i = 1U; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);
	allocator<T>::destruct((T*)&(_data[--_size]));
	return true;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::pop(T& value, bool last)
{
	if (_size == 0)
		return false;
	if (!last)
	{
		value = ang::move(_data[0]);
		for (windex i = 1U; i < _size; ++i)
			_data[i - 1] = ang::move(_data[i]);
	}
	else
	{
		value = ang::move(_data[_size - 1U]);
	}
	allocator<T>::destruct((T*)&(_data[--_size]));
	return true;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::pop_at(windex idx)
{
	if (_size == 0U)
		return false;
	if (idx == 0U)
		return pop(false);
	for (windex i = idx; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);
	allocator<T>::destruct((T*)&(_data[--_size]));
	return true;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::pop_at(ang::collections::base_iterator<T> it)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return pop_at(it.offset());
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::pop_at(windex idx, T& value)
{
	if (_size == 0U)
		return false;
	if (idx == 0U)
		return pop(value, false);
	value = ang::move(_data[0]);
	for (windex i = idx; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);
	allocator<T>::destruct((T*)&(_data[--_size]));
	return true;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::pop_at(ang::collections::base_iterator<T> it, T& value)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return pop_at(it.offset(), value);
}

template<typename T, template<typename> class allocator>
inline void ang::collections::vector_buffer<T, allocator>::clean()
{
	if (_data != null)
	{
		for (wsize i = 0; i < _size; ++i)
			allocator<T>::destruct((T*)&_data[i]);
		_size = 0U;
		_capacity = 0U;
		allocator<T>::free(_data);
	}
	_data = null;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::vector_buffer<T, allocator>::realloc(wsize new_size, bool save)
{
	new_size++;
	if (capacity() >= new_size)
		return true;

	wsize sz = 8U, temp = 0U;
	while (sz <= new_size)
		sz *= 2U;
	T* new_buffer = allocator<T>::alloc(sz);

	if (save)
	{
		temp = _size;
		for (windex i = 0U; i < _size; ++i)
		{
			allocator<T>::template construct<T&&>((T*)&new_buffer[i], ang::move(_data[i]));
			allocator<T>::destruct((T*)&_data[i]);
		}
		allocator<T>::free(_data);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::object_wrapper(ang::collections::vector_buffer<T, allocator>* ptr)
	: object_wrapper<ang::collections::vector_buffer<T, allocator>>()
{
	set(ptr);
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper<ang::collections::vector_buffer<T, allocator>>()
{
}

template<typename T, template <typename> class allocator> template<typename U>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::object_wrapper(std::initializer_list<U> list)
	: object_wrapper<ang::collections::vector_buffer<T, allocator>>()
{
	set(new collections::vector_buffer<T, allocator>(ang::move(list)));
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: object_wrapper<ang::collections::vector_buffer<T, allocator>>()
{
	set(new collections::vector_buffer<T, allocator>(store));
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<T, allocator>> && other)
	: object_wrapper<collections::vector_buffer<T, allocator>>()
{
	collections::vector_buffer<T, allocator> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<T, allocator>> const& other)
	: object_wrapper<collections::vector_buffer<T, allocator>>()
{
	set(other.get());
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::~object_wrapper()
{
	clean();
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::clean_unsafe()
{
	_ptr = null;
}

template<typename T, template <typename> class allocator>
inline bool ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::is_empty()const
{
	return _ptr == null;
}

template<typename T, template <typename> class allocator>
inline ang::collections::vector_buffer<T, allocator>* ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::get(void)const
{
	return _ptr;
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::set(ang::collections::vector_buffer<T, allocator>* ptr)
{
	ang::collections::vector_buffer<T, allocator> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, template <typename> class allocator>
inline ang::collections::vector_buffer<T, allocator>** ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::addres_of(void)
{
	return &_ptr;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>& ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator = (ang::collections::vector_buffer<T, allocator>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>& ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator = (const std::nullptr_t&)
{
	clean();
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>& ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (_ptr == null)
		set(new collections::vector_buffer<T, allocator>(items));
	else
		_ptr->copy(items);
	return *this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>& ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<T, allocator>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>& ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<T, allocator>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper_ptr<ang::collections::vector_buffer<T, allocator>> ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator & (void)
{
	return this;
}

template<typename T, template <typename> class allocator>
inline ang::collections::vector_buffer<T, allocator> * ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator -> (void)
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::collections::vector_buffer<T, allocator> const* ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator -> (void)const
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator ang::collections::vector_buffer<T, allocator> * (void)
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator ang::collections::vector_buffer<T, allocator> const* (void)const
{
	return get();
}

template<typename T, template <typename> class allocator> template<typename I>
inline T const& ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator[](I const& idx)const
{
	static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); 
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

template<typename T, template <typename> class allocator> template<typename I>
inline T & ang::object_wrapper<ang::collections::vector_buffer<T, allocator>>::operator[](I const& idx)
{
	static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::object_wrapper(ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>* ptr)
	: object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>()
{
	set(ptr);
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>()
{
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::object_wrapper(std::initializer_list<ang::object_wrapper<T>> list)
	: object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>()
{
	set(new collections::vector_buffer<ang::object_wrapper<T>, allocator>(ang::move(list)));
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::object_wrapper(const ang::collections::ienum<ang::object_wrapper<T>>* store)
	: object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>()
{
	set(new collections::vector_buffer<ang::object_wrapper<T>, allocator>(store));
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>> && other)
	: object_wrapper<collections::vector_buffer<ang::object_wrapper<T>, allocator>>()
{
	collections::vector_buffer<ang::object_wrapper<T>, allocator> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>> const& other)
	: object_wrapper<collections::vector_buffer<ang::object_wrapper<T>, allocator>>()
{
	set(other.get());
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::~object_wrapper()
{
	clean();
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::clean_unsafe()
{
	_ptr = null;
}

template<typename T, template <typename> class allocator>
inline bool ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::is_empty()const
{
	return _ptr == null;
}

template<typename T, template <typename> class allocator>
inline ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>* ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::get(void)const
{
	return _ptr;
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::set(ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>* ptr)
{
	ang::collections::vector_buffer<ang::object_wrapper<T>, allocator> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, template <typename> class allocator>
inline ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>** ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::addres_of(void)
{
	return &_ptr;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator = (ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator = (const std::nullptr_t&)
{
	clean();
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator = (ang::collections::ienum<ang::object_wrapper<T>> const* items)
{
	if (_ptr == null)
		set(new collections::vector_buffer<ang::object_wrapper<T>, allocator>(items));
	else
		_ptr->copy(items);
	return *this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper_ptr<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>> ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator & (void)
{
	return this;
}

template<typename T, template <typename> class allocator>
inline ang::collections::vector_buffer<ang::object_wrapper<T>, allocator> * ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator -> (void)
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::collections::vector_buffer<ang::object_wrapper<T>, allocator> const* ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator -> (void)const
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator ang::collections::vector_buffer<ang::object_wrapper<T>, allocator> * (void)
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator ang::collections::vector_buffer<ang::object_wrapper<T>, allocator> const* (void)const
{
	return get();
}

template<typename T, template <typename> class allocator> template<typename I>
inline ang::object_wrapper<T> const& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator[](I const& idx)const
{
	static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

template<typename T, template <typename> class allocator> template<typename I>
inline ang::object_wrapper<T> & ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<T>, allocator>>::operator[](I const& idx)
{
	static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


#endif//__ANG_VECTOR_INL__
