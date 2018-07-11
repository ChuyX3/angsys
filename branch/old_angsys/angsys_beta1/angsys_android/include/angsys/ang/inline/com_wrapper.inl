#ifndef __COM_WRAPPER_H__
#elif !defined __COM_WRAPPER_HPP__
#define __COM_WRAPPER_HPP__


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// vector_buffer<com_wrapper<T>> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::collections::vector_buffer<ang::com_wrapper<T>>::vector_buffer()
	: object()
	, _size(0)
	, _capacity(0)
	, _data(null)
{
	allocator = memory::allocator_manager::get_allocator(0);
}

template<typename T>
inline ang::collections::vector_buffer<ang::com_wrapper<T>>::vector_buffer(uint sz)
	: vector_buffer()
{
	capacity(sz);
}


template<typename T>
inline ang::collections::vector_buffer<ang::com_wrapper<T>>::vector_buffer(ang::initializer_list_t<ang::com_wrapper<T>> list)
	: vector_buffer()
{
	capacity((uint)list.size());
	for (auto it = list.begin(); it != list.end(); ++it)
		append(*it);
}

template<typename T>
inline ang::collections::vector_buffer<ang::com_wrapper<T>>::vector_buffer(const ang::nullptr_t&)
	: vector_buffer()
{

}

template<typename T>
inline ang::collections::vector_buffer<ang::com_wrapper<T>>::vector_buffer(ang::collections::vector_buffer<ang::com_wrapper<T>>&& ar)
	: vector_buffer()
{
	_size = ar._size;
	_data = ar._data;
	_capacity = ar._capacity;
	allocator = ar.allocator;
	ar._size = 0;
	ar._capacity = 0;
	ar._data = null;
	ar.allocator = 0;
}

template<typename T>
inline ang::collections::vector_buffer<ang::com_wrapper<T>>::vector_buffer(const ang::collections::vector_buffer<ang::com_wrapper<T>>& ar)
	: vector_buffer()
{
	copy(static_cast<const ienum_t&>(ar));
}

template<typename T>
inline ang::collections::vector_buffer<ang::com_wrapper<T>>::vector_buffer(const ang::collections::vector_buffer<ang::com_wrapper<T>>* ar)
	: vector_buffer()
{
	if (ar) copy(*static_cast<const ienum_t*>(ar));
}

template<typename T>
inline ang::collections::vector_buffer<ang::com_wrapper<T>>::vector_buffer(const ang::collections::ienum<ang::com_wrapper<T>>*store)
	: vector_buffer()
{
	copy(store);
}

template<typename T>
inline ang::collections::vector_buffer<ang::com_wrapper<T>>::~vector_buffer()
{
	clean();
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::is_empty()const
{
	return null == _data;
}

template<typename T>
inline ang::com_wrapper<T>* ang::collections::vector_buffer<ang::com_wrapper<T>>::data()const
{
	return _data;
}

template<typename T>
inline uint ang::collections::vector_buffer<ang::com_wrapper<T>>::size()const
{
	return _size;
}

template<typename T>
inline uint ang::collections::vector_buffer<ang::com_wrapper<T>>::capacity()const
{
	return _capacity;
}

template<typename T>
inline void ang::collections::vector_buffer<ang::com_wrapper<T>>::capacity(uint size, bool save)
{
	realloc(size, save);
}

template<typename T>
inline void ang::collections::vector_buffer<ang::com_wrapper<T>>::set_allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == allocator)
		return;

	if (!alloc->can_delete_from(allocator))
		realloc(alloc);

	allocator = alloc;
}

template<typename T>
inline ang::memory::iallocator* ang::collections::vector_buffer<ang::com_wrapper<T>>::get_allocator()const
{
	return allocator;
}

template<typename T>
inline pointer ang::collections::vector_buffer<ang::com_wrapper<T>>::buffer_ptr()const
{
	return (pointer)_data;
}

template<typename T>
inline wsize ang::collections::vector_buffer<ang::com_wrapper<T>>::buffer_size()const
{
	return _capacity * (wsize)sizeof(T);
}

template<typename T>
inline wsize ang::collections::vector_buffer<ang::com_wrapper<T>>::mem_copy(wsize _s, pointer _p)
{
	throw(exception_t(except_code::unsupported));
	return 0;
}

template<typename T>
inline pointer ang::collections::vector_buffer<ang::com_wrapper<T>>::map_buffer(windex start, wsize size)
{
	throw(exception_t(except_code::unsupported));
	return null;
}

template<typename T>
void ang::collections::vector_buffer<ang::com_wrapper<T>>::unmap_buffer(pointer ptr, wsize used)
{
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::can_realloc_buffer()const { return true; };

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::realloc_buffer(wsize size) { return realloc(size, true); };

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::has_items()const
{
	return bool(size() != 0);
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::move(ang::collections::vector_buffer<ang::com_wrapper<T>>& ar)
{
	if (&ar == this)
		return false;
	clean();
	_size = ar._size;
	_data = ar._data;
	_capacity = ar._capacity;
	allocator = ar.allocator;
	ar._data = null;
	ar._size = 0;
	ar.allocator = 0;
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::copy(const ang::collections::ienum<ang::com_wrapper<T>>* _items)
{
	if (_items == null)
		return false;
	capacity(_items->counter(), false);
	for (auto it = _items->begin(); it.is_valid(); ++it)
		append(const_cast<com_wrapper<T>&>(*it));
	return true;
}

template<typename T>
inline ang::com_wrapper<T>& ang::collections::vector_buffer<ang::com_wrapper<T>>::at(const ang::collections::iterator<ang::com_wrapper<T>>& it)const
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
inline ang::collections::iterator<ang::com_wrapper<T>> ang::collections::vector_buffer<ang::com_wrapper<T>>::find_index(index idx)const
{
	if (idx >= _size)
		return  iterator_t(const_cast<self_t*>(this), null, 0);
	return iterator_t(const_cast<self_t*>(this), (pointer)_data, idx);
}

template<typename T>
inline index ang::collections::vector_buffer<ang::com_wrapper<T>>::index_of(ang::collections::iterator<ang::com_wrapper<T>> it)const
{
	if (!it.is_valid() || it.parent() != this)
		return invalid_index;
	return it.offset();
}

template<typename T>
inline ang::collections::iterator<ang::com_wrapper<T>> ang::collections::vector_buffer<ang::com_wrapper<T>>::find(ang::com_wrapper<T> const& datum, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (iterator_t it = begin(); it.is_valid(); ++it)
		{
			if ((*it).get() == datum.get())
				return it;
		}
		else for (iterator_t it = end(); it.is_valid(); --it)
		{
			if ((*it).get() == datum.get())
				return it;
		}
	}
	return iterator_t(const_cast<self_t*>(this), 0);
}

template<typename T>
inline ang::collections::iterator<ang::com_wrapper<T>> ang::collections::vector_buffer<ang::com_wrapper<T>>::find(ang::com_wrapper<T> const& datum, ang::collections::iterator<ang::com_wrapper<T>> nextTo, bool invert)const
{
	if (!is_empty() && nextTo.parent() == this)
	{
		if (invert)for (iterator_t it = nextTo.is_valid() ? nextTo : begin(); it.is_valid(); ++it)
		{
			if ((*it).get() == datum.get())
				return it;
		}

		else for (iterator_t it = nextTo.is_valid() ? nextTo : end(); it.is_valid(); --it)
		{
			if ((*it).get() == datum.get())
				return it;
		}
	}
	return iterator_t(const_cast<vector_buffer*>(this), 0);
}

template<typename T>
inline ang::collections::iterator<ang::com_wrapper<T>> ang::collections::vector_buffer<ang::com_wrapper<T>>::begin()const
{
	return iterator_t(const_cast<vector_buffer*>(this), position_t((is_empty()) ? null : _data));
}

template<typename T>
inline ang::collections::iterator<ang::com_wrapper<T>> ang::collections::vector_buffer<ang::com_wrapper<T>>::end()const
{
	return iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1);
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::next(ang::collections::iterator<ang::com_wrapper<T>>& it)const
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
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::next(ang::collections::iterator<ang::com_wrapper<T>>& it, int val)const
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
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::prev(ang::collections::iterator<ang::com_wrapper<T>>& it)const
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
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::prev(ang::collections::iterator<ang::com_wrapper<T>>& it, int val)const
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
inline uint ang::collections::vector_buffer<ang::com_wrapper<T>>::counter()const
{
	return _size;
}

template<typename T>
inline void ang::collections::vector_buffer<ang::com_wrapper<T>>::extend(const ang::collections::ienum<com_wrapper<T>>* items)
{
	if (!items)return;
	capacity(counter() + items->counter());
	for (auto it = items->begin(); it.is_valid(); ++it)
		append(*it);
}

template<typename T>
inline void ang::collections::vector_buffer<ang::com_wrapper<T>>::append(ang::com_wrapper<T> const& value, bool last)
{
	if ((capacity() - counter()) <= 1U) //TODO: optimization
		capacity(capacity() + 1, true);

	if (last)
	{
		get_allocator()->template construct<type>(&_data[_size++], value);
	}
	else
	{
		memory::iallocator* alloc = get_allocator();
		for (auto i = _size; i > 0U; --i)
		{
			alloc->construct<type>(&_data[i], ang::move(_data[i - 1U]));
			alloc->destruct<type>(&_data[i - 1U]);
		}
		alloc->construct<type>(&_data[0], value);
	}
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::insert(index idx, ang::com_wrapper<T> const& value)
{
	if (idx == 0U)
	{
		append(value, false);
		return true;
	}
	else if (idx >= _size)
	{
		append(value, true);
		return true;
	}

	if ((capacity() - counter()) <= 1U) //TODO: optimization
		capacity(capacity() + 1, true);

	memory::iallocator* alloc = get_allocator();
	for (auto i = _size; i > idx; --i)
	{
		alloc->construct<type>(&_data[i], ang::move(_data[i - 1U]));
		alloc->destruct<type>(&_data[i - 1U]);
	}
	alloc->construct<type>(&_data[idx], value);
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::insert(ang::collections::iterator<ang::com_wrapper<T>> it, ang::com_wrapper<T> const& value)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return insert((index_t)it.offset(), value);
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::pop(bool last)
{
	if (_size == 0)
		return false;
	if (!last) for (index_t i = 1U; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);

	get_allocator()->template destruct<type>(&(_data[--_size]));
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::pop(ang::com_wrapper<T>& value, bool last)
{
	if (_size == 0)
		return false;
	if (!last)
	{
		value = ang::move(_data[0]);
		for (index_t i = 1U; i < _size; ++i)
			_data[i - 1] = ang::move(_data[i]);
	}
	else
	{
		value = ang::move(_data[_size - 1U]);
	}
	get_allocator()->template destruct<type>(&(_data[--_size]));
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::pop_at(index idx)
{
	if (_size == 0U)
		return false;
	if (idx == 0U)
		return pop(false);
	for (index_t i = idx; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);
	get_allocator()->template destruct<type>(&(_data[--_size]));
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::pop_at(ang::collections::iterator<ang::com_wrapper<T>> it)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return pop_at((index_t)it.offset());
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::pop_at(index idx, ang::com_wrapper<T>& value)
{
	if (_size == 0U)
		return false;
	if (idx == 0U)
		return pop(value, false);
	value = ang::move(_data[0]);
	for (index_t i = idx; i < _size; ++i)
		_data[i - 1] = ang::move(_data[i]);
	get_allocator()->template destruct<type>(&(_data[--_size]));
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::pop_at(ang::collections::iterator<ang::com_wrapper<T>> it, ang::com_wrapper<T>& value)
{
	if (it.parent() != this || it.current() != _data)
		return false;
	return pop_at((index_t)it.offset(), value);
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<vector_buffer<ang::com_wrapper<T>>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::ilist<com_wrapper<T>>::is_kind_of(name))
		return true;
	return false;
}

template<typename T>
inline ang::type_name_t ang::collections::vector_buffer<ang::com_wrapper<T>>::class_name()
{
	static ang::string className = ang::string("ang::collections::vector<"_s) + type_name<type>() + ">"_s;
	return className->cstr();
}

template<typename T>
inline ang::type_name_t ang::collections::vector_buffer<ang::com_wrapper<T>>::object_name()const
{
	return ang::collections::vector_buffer<ang::com_wrapper<T>>::class_name();
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (className == type_name<vector_buffer<ang::com_wrapper<T>>>())
	{
		*out = static_cast<ang::collections::vector_buffer<ang::com_wrapper<T>>*>(this);
		return true;
	}
	else if (ang::object::query_object(className, out))
	{
		return true;
	}
	else if (ang::collections::ilist<com_wrapper<T>>::query_object(className, out))
	{
		return true;
	}
	return false;
}

template<typename T>
inline void ang::collections::vector_buffer<ang::com_wrapper<T>>::clean()
{
	if (_data != null)
	{
		for (uint i = 0; i < _size; ++i)
			allocator->destruct<type>(&_data[i]);
		_size = 0U;
		_capacity = 0U;
		allocator->memory_release(_data);
	}
	_data = null;
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::operator == (const ang::collections::vector_buffer<ang::com_wrapper<T>>& ar)
{
	if (size() != ar.size())
		return false;

	for (index_t i = 0, l = size(); i < l; i++)
		if (_data[i] != ar._data[i])
			return false;
	return true;
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::operator != (const ang::collections::vector_buffer<ang::com_wrapper<T>>& ar)
{
	return !operator == (ar);
}

template<typename T>
inline ang::collections::vector_buffer<ang::com_wrapper<T>>::operator T**()const
{
	return _data;
}

template<typename T>
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::realloc(uint new_size, bool save)
{
	if (capacity() >= new_size)
		return true;

	uint sz = 8U, temp = 0U;
	while (sz <= new_size)
		sz *= 2U;
	memory::iallocator* alloc = get_allocator();
	type* new_buffer = alloc->object_alloc<type>(sz);

	if (save)
	{
		temp = _size;
		for (index_t i = 0U; i < _size; ++i)
		{
			alloc->construct<type>(&new_buffer[i], ang::move(_data[i]));
			alloc->destruct<type>(&_data[i]);
		}
		alloc->memory_release(_data);
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
inline bool ang::collections::vector_buffer<ang::com_wrapper<T>>::realloc(ang::memory::iallocator* alloc)
{
	if (capacity() == 0U)
		return true;

	memory::iallocator* this_alloc = get_allocator();
	type* new_buffer = alloc->object_alloc<type>(_capacity);
	for (index_t i = 0U; i < _size; ++i)
	{
		alloc->construct<type>(&new_buffer[i], ang::move(_data[i]));
		this_alloc->destruct<type>(&_data[i]);
	}
	this_alloc->memory_release(_data);
	_data = new_buffer;
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif//__COM_WRAPPER_HPP__
