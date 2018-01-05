
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// vector_buffer<T, allocator> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined MY_TYPE && !defined MY_ALLOCATOR
#error ...
#endif

ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::vector_buffer()
	: object()
	, _size(0)
	, _capacity(0)
	, _data(null)
{
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::vector_buffer(const ang::nullptr_t&)
	: vector_buffer()
{
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::vector_buffer(ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>&& ar)
	: vector_buffer()
{
	_size = ar._size;
	_data = ar._data;
	_capacity = ar._capacity;
	ar._size = 0;
	ar._capacity = 0;
	ar._data = null;
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::vector_buffer(const ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>& ar)
	: vector_buffer()
{
	copy(to_array(ar.data(), ar.size()));
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::vector_buffer(const ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>* ar)
	: vector_buffer()
{
	if (ar)copy(to_array(ar->data(), ar->size()));
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::vector_buffer(const ang::collections::ienum<MY_TYPE>* store)
	: vector_buffer()
{
	copy(store);
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::~vector_buffer()
{
	clean();
}

ang::type_name_t ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::class_name()
{
	static type_name_t name = "ang::collections::vector<" ANG_UTILS_TO_STRING(ANG_EXPAND(MY_TYPE)) ">";
	return name;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::is_inherited_of(ang::type_name_t name)
{
	if (name == type_of<vector_buffer<MY_TYPE, MY_ALLOCATOR>>()
		|| ang::object::is_inherited_of(name)
		|| ang::collections::ilist<MY_TYPE>::is_inherited_of(name))
		return true;
	return false;
}

ang::type_name_t ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::object_name()const
{
	return ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::class_name();
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_of<vector_buffer<MY_TYPE, MY_ALLOCATOR>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::ilist<MY_TYPE>::is_kind_of(name))
		return true;
	return false;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (className == type_of<vector_buffer<MY_TYPE, MY_ALLOCATOR>>())
	{
		*out = static_cast<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>*>(this);
		return true;
	}
	else if (ang::object::query_object(className, out))
	{
		return true;
	}
	else if (ang::collections::ilist<MY_TYPE>::query_object(className, out))
	{
		return true;
	}
	return false;
}


bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::is_empty()const
{
	return size() == 0;
}

MY_TYPE* ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::data()const
{
	return _data;
}

wsize ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::size()const
{
	return _size;
}

wsize ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::capacity()const
{
	return _capacity;
}

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::capacity(wsize size, bool save)
{
	realloc(size, save);
}

ang::text::encoding_t ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::encoding()const
{
	return text::encoding::binary;
}

pointer ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::buffer_ptr()const
{
	return (pointer)_data;
}

wsize ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::buffer_size()const
{
	return _capacity * sizeof(MY_TYPE);
}

wsize ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::mem_copy(wsize _s, pointer _p, text::encoding_t)
{
	_s = min(_s, (_capacity - 1) * sizeof(MY_TYPE));
	memcpy(_data, _p, _s);
	_size = _s / sizeof(MY_TYPE);
	return _s;
}

ang::ibuffer_view_t ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::map_buffer(windex start, wsize size)
{
	return null;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::unmap_buffer(ang::ibuffer_view_t&, wsize used)
{
	return false;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::can_realloc_buffer()const { return true; };

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::realloc_buffer(wsize size) { return realloc(size, true); };

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::move(ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>& ar)
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

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::copy(const ang::collections::ienum<MY_TYPE>* _items)
{
	if (!_items)
		return;
	capacity(_items->counter(), false);
	for (MY_TYPE const& value : *_items)
		push(value);
}

wsize ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::counter()const
{
	return _size;
}

MY_TYPE& ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::at(ang::collections::base_iterator<MY_TYPE> const& it)
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

ang::collections::iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::at(windex idx)
{
	if (idx >= _size)
		return  iterator_t(const_cast<self_t*>(this), null, 0);
	return iterator_t(const_cast<self_t*>(this), (pointer)_data, idx);
}

ang::collections::iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::at(windex idx)const
{
	if (idx >= _size)
		return  const_iterator_t(const_cast<self_t*>(this), null, 0);
	return const_iterator_t(const_cast<self_t*>(this), (pointer)_data, idx);
}

ang::collections::iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::find(MY_TYPE const& datum, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (auto i = size(); i > 0; --i)
		{
			if (comparision_operations<MY_TYPE, MY_TYPE>::template compare<comparision_same>(_data[i - 1], datum))
				return at(i - 1);
		}
		else for (auto i = 0U; i < size(); ++i)
		{
			if (comparision_operations<MY_TYPE, MY_TYPE>::template compare<comparision_same>(_data[i], datum))
				return at(i);
		}
	}
	return at(invalid_index);
}

ang::collections::iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::find(MY_TYPE const& datum, ang::collections::base_iterator<MY_TYPE> next_to, bool invert)const
{
	if (!is_empty() && next_to.parent() == this)
	{
		if (invert) for (auto i = next_to.offset() + 1; i > 0; --i)
		{
			if (comparision_operations<MY_TYPE, MY_TYPE>::template compare<comparision_same>(_data[i - 1], datum))
				return at(i - 1);
		}
		else for (auto i = next_to.offset(); i < size(); ++i)
		{
			if (comparision_operations<MY_TYPE, MY_TYPE>::template compare<comparision_same>(_data[i], datum))
				return at(i);
		}
	}
	return at(invalid_index);
}

ang::collections::forward_iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::begin()
{
	return forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data));
}

ang::collections::forward_iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::end()
{
	return forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size);
}

ang::collections::forward_iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::begin()const
{
	return const_forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data));
}

ang::collections::forward_iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::end()const
{
	return const_forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size);
}

ang::collections::forward_iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::last()
{
	return _size ? forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1) : end();
}

ang::collections::forward_iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::last()const
{
	return _size ? const_forward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1) : end();
}

ang::collections::backward_iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::rbegin()
{
	return backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1);
}

ang::collections::backward_iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::rend()
{
	return backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), invalid_index);
}

ang::collections::backward_iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::rbegin()const
{
	return const_backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1);
}

ang::collections::backward_iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::rend()const
{
	return const_backward_iterator_t(const_cast<vector_buffer*>(this), position_t(_data), invalid_index);
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::increase(ang::collections::base_iterator<MY_TYPE>& it)const
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

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::increase(ang::collections::base_iterator<MY_TYPE>& it, int val)const
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

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::decrease(ang::collections::base_iterator<MY_TYPE>& it)const
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

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::decrease(ang::collections::base_iterator<MY_TYPE>& it, int val)const
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

ang::comparision_result_t ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::compare(const ang::object& obj)const
{
	if (obj.is_kind_of(class_name()))
	{
		vector_buffer<MY_TYPE, MY_ALLOCATOR>const& other = static_cast<vector_buffer<MY_TYPE, MY_ALLOCATOR>const&>(obj);
		if (counter() > other.counter())
			return comparision_result::mayor;
		else if (counter() < other.counter())
			return comparision_result::minor;
		else for (windex i = 0, c = counter(); i < c; ++i)
			if (comparision_operations<MY_TYPE, MY_TYPE>::template compare<comparision_diferent>(_data[i], other._data[i]))
				return comparision_result::diferent;
		return comparision_result::same;
	}
	return comparision_result::diferent;
}

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::extend(const ang::collections::ienum<MY_TYPE>* items)
{
	if (!items) return;
	capacity(counter() + items->counter());
	for (MY_TYPE const& value : *items)
		push(value);
}

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::push(MY_TYPE const& value, bool last)
{
	if ((capacity() - counter()) <= 1U) //TODO: optimization
		capacity(capacity() + 1, true);

	if (last)
	{
		MY_ALLOCATOR<MY_TYPE>::template construct<MY_TYPE const&>((MY_TYPE*)&_data[_size++], value);
	}
	else
	{
		for (auto i = _size; i > 0U; --i)
			MY_ALLOCATOR<MY_TYPE>::template construct<MY_TYPE&&>((MY_TYPE*)&_data[i], ang::move(_data[i - 1U]));

		MY_ALLOCATOR<MY_TYPE>::template construct<MY_TYPE const&>((MY_TYPE*)&_data[0], value);
		_size++;
	}
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::insert(windex idx, MY_TYPE const& value)
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
		MY_ALLOCATOR<MY_TYPE>::template construct<MY_TYPE&&>((MY_TYPE*)&_data[i], ang::move(_data[i - 1U]));
		MY_ALLOCATOR<MY_TYPE>::destruct((MY_TYPE*)&_data[i - 1U]);
	}
	MY_ALLOCATOR<MY_TYPE>::template construct<MY_TYPE const&>((MY_TYPE*)&_data[idx], value);
	_size++;
	return true;
}


bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::insert(ang::collections::base_iterator<MY_TYPE> it, MY_TYPE const& value)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return insert(it.offset(), value);
}


bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::pop(bool last)
{
	if (_size == 0)
		return false;
	if (!last) for (windex i = 1U; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);
	MY_ALLOCATOR<MY_TYPE>::destruct((MY_TYPE*)&(_data[--_size]));
	return true;
}


bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::pop(MY_TYPE& value, bool last)
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
	MY_ALLOCATOR<MY_TYPE>::destruct((MY_TYPE*)&(_data[--_size]));
	return true;
}


bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::pop_at(windex idx)
{
	if (_size == 0U)
		return false;
	if (idx == 0U)
		return pop(false);
	for (windex i = idx; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);
	MY_ALLOCATOR<MY_TYPE>::destruct((MY_TYPE*)&(_data[--_size]));
	return true;
}


bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::pop_at(ang::collections::base_iterator<MY_TYPE> it)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return pop_at(it.offset());
}


bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::pop_at(windex idx, MY_TYPE& value)
{
	if (_size == 0U)
		return false;
	if (idx == 0U)
		return pop(value, false);
	value = ang::move(_data[0]);
	for (windex i = idx; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);
	MY_ALLOCATOR<MY_TYPE>::destruct((MY_TYPE*)&(_data[--_size]));
	return true;
}


bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::pop_at(ang::collections::base_iterator<MY_TYPE> it, MY_TYPE& value)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return pop_at(it.offset(), value);
}


void ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::clean()
{
	if (_data != null)
	{
		for (wsize i = 0; i < _size; ++i)
			MY_ALLOCATOR<MY_TYPE>::destruct((MY_TYPE*)&_data[i]);
		_size = 0U;
		_capacity = 0U;
		MY_ALLOCATOR<MY_TYPE>::free(_data);
	}
	_data = null;
}


bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::realloc(wsize new_size, bool save)
{
	new_size++;
	if (capacity() >= new_size)
		return true;

	wsize sz = 8U, temp = 0U;
	while (sz <= new_size)
		sz *= 2U;
	MY_TYPE* new_buffer = MY_ALLOCATOR<MY_TYPE>::alloc(sz);

	if (save)
	{
		temp = _size;
		for (windex i = 0U; i < _size; ++i)
		{
			MY_ALLOCATOR<MY_TYPE>::template construct<MY_TYPE&&>((MY_TYPE*)&new_buffer[i], ang::move(_data[i]));
			MY_ALLOCATOR<MY_TYPE>::destruct((MY_TYPE*)&_data[i]);
		}
		MY_ALLOCATOR<MY_TYPE>::free(_data);
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

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper()
	: _ptr(null)
{

}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper(ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>* ptr)
	: object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>()
{
	set(ptr);
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>()
{
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>()
{
	set(new collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>(store));
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>> && other)
	: object_wrapper<collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>()
{
	collections::vector_buffer<MY_TYPE, MY_ALLOCATOR> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>> const& other)
	: object_wrapper<collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>()
{
	set(other.get());
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::is_empty()const
{
	return _ptr == null;
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>* ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::set(ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>* ptr)
{
	ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>** ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::operator = (ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::operator = (const std::nullptr_t&)
{
	clean();
	return*this;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (_ptr == null)
		set(new collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>(items));
	else
		_ptr->copy(items);
	return *this;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>> const& other)
{
	set(other._ptr);
	return*this;
}

ang::object_wrapper_ptr<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>> ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::operator & (void)
{
	return this;
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR> * ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::operator -> (void)
{
	return get();
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR> const* ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::operator ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR> * (void)
{
	return get();
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>>::operator ang::collections::vector_buffer<MY_TYPE, MY_ALLOCATOR> const* (void)const
{
	return get();
}
