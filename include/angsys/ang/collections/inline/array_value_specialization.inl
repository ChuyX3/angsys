/*********************************************************************************************************************/
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

///////////////////////////////////////////////////////////////////////////////////

#if !defined MY_TYPE && !defined MY_ALLOCATOR
#error ...
#endif

ang::collections::scope_array<MY_TYPE, MY_ALLOCATOR>::scope_array()
	: _size(0)
	, _data(null) {

}

ang::collections::scope_array<MY_TYPE, MY_ALLOCATOR>::scope_array(ang::nullptr_t const&)
	: _size(0)
	, _data(null) {

}

ang::collections::scope_array<MY_TYPE, MY_ALLOCATOR>::scope_array(wsize sz, MY_TYPE* val)
	: _size(0)
	, _data(ang::null) {
	_size = min(wsize(-1) / sizeof(MY_TYPE), sz);
	if (_size > 0) {
		_data = MY_ALLOCATOR<MY_TYPE>::alloc(_size);
		for (index i = 0; i < _size; ++i)
			MY_ALLOCATOR<MY_TYPE>::template construct<MY_TYPE const&>((MY_TYPE*)&_data[i], val[i]);
	}
}

ang::collections::scope_array<MY_TYPE, MY_ALLOCATOR>::scope_array(scope_array const& other)
	: _size(other._size)
	, _data(null) {
	if (_size > 0) {
		_data = MY_ALLOCATOR<MY_TYPE>::alloc(_size);
		for (index i = 0; i < _size; ++i)
			MY_ALLOCATOR<MY_TYPE>::template construct<MY_TYPE const&>((MY_TYPE*)&_data[i], other._data[i]);
	}
}

ang::collections::scope_array<MY_TYPE, MY_ALLOCATOR>::scope_array(scope_array&& other)
	: _size(ang::move(other._size))
	, _data(ang::move(other._data)) {
}

void ang::collections::scope_array<MY_TYPE, MY_ALLOCATOR>::clean()
{
	if (_data)
	{
		//for (index i = 0; i < _size; ++i)
		//	MY_ALLOCATOR<MY_TYPE>::destruct((MY_TYPE*)&_data[i]);
		MY_ALLOCATOR<MY_TYPE>::free(_data);
	}
	_size = 0;
	_data = null;
}

MY_TYPE* ang::collections::scope_array<MY_TYPE, MY_ALLOCATOR>::alloc(wsize size)
{
	clean();
	if (size > 0)
	{
		_size = size;
		_data = MY_ALLOCATOR<MY_TYPE>::alloc(_size);
		//for (index i = 0; i < _size; ++i)
		//	MY_ALLOCATOR<MY_TYPE>::template construct<MY_TYPE const&>((MY_TYPE*)&_data[i], MY_TYPE());
	}
	return _data;
}

void ang::collections::scope_array<MY_TYPE, MY_ALLOCATOR>::set(MY_TYPE* val, wsize sz)
{
	clean();
	_size = min(wsize(-1) / sizeof(MY_TYPE), sz);
	if (_size > 0) {
		_data = MY_ALLOCATOR<MY_TYPE>::alloc(_size);
		for (index i = 0; i < _size; ++i)
			MY_ALLOCATOR<MY_TYPE>::template construct<MY_TYPE const&>((MY_TYPE*)&_data[i], val[i]);
	}
}

void ang::collections::scope_array<MY_TYPE, MY_ALLOCATOR>::move(scope_array& other)
{
	if (other.get() == this->get())
		return;
	clean();
	_size = other._size;
	_data = other._data;
	other._size = 0;
	other._data = null;
}

void ang::collections::scope_array<MY_TYPE, MY_ALLOCATOR>::move(ang::array_view<MY_TYPE>& other)
{
	clean();
	if (other.size() > 0)
		_data = MY_ALLOCATOR<MY_TYPE>::alloc(other.size());
	_size = other.size();

	for (windex i = 0; i < _size; ++i)
		MY_ALLOCATOR<MY_TYPE>::template construct<MY_TYPE&&>(&_data[i], ang::move(other.get()[i]));
	other.set(null, 0);
}

///////////////////////////////////////////////////////////////////////////////////

ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::array_buffer()
	: object()
	, _data(null)
{
}

ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::array_buffer(wsize reserve)
	: array_buffer()
{
	_data.alloc(reserve);
}

ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::array_buffer(const std::nullptr_t&)
	: array_buffer()
{

}

ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::array_buffer(ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>&& ar)
	: array_buffer()
{
	_data.move(ar._data);
}

ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::array_buffer(const ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>& ar)
	: array_buffer()
{
	copy(&static_cast<const ienum_t&>(ar));
}

ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::array_buffer(const ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>* ar)
	: array_buffer()
{
	if (ar) copy(static_cast<const ienum_t*>(ar));
}

ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::array_buffer(const ang::collections::ienum<MY_TYPE>* store)
	: array_buffer()
{
	copy(store);
}

ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::~array_buffer()
{
	clean();
}

ang::type_name_t ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::class_name()
{
	static type_name_t name = "ang::collections::array<" ANG_UTILS_TO_STRING(ANG_EXPAND(MY_TYPE)) ">";
	return name;
}

bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::is_inherited_of(ang::type_name_t name)
{
	if (name == type_of<array_buffer<MY_TYPE, MY_ALLOCATOR>>()
		|| ang::object::is_inherited_of(name)
		|| ang::collections::iarray<MY_TYPE>::is_inherited_of(name))
		return true;
	return false;
}

ang::type_name_t ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::object_name()const
{
	return ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::class_name();
}

bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_of<array_buffer<MY_TYPE, MY_ALLOCATOR>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::iarray<MY_TYPE>::is_kind_of(name))
		return true;
	return false;
}

bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (className == type_of<array_buffer<MY_TYPE, MY_ALLOCATOR>>())
	{
		*out = static_cast<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>*>(this);
		return true;
	}
	else if (ang::object::query_object(className, out))
	{
		return true;
	}
	else if (ang::collections::iarray<MY_TYPE>::query_object(className, out))
	{
		return true;
	}
	return false;
}

void ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::clean()
{
	_data.clean();
}


bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::is_empty()const
{
	return null == _data.get();
}


MY_TYPE* ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::data()const
{
	return _data.get();
}


wsize ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::size()const
{
	return _data.size();
}


void ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::size(wsize size)
{
	if (size == _data.size())
		return;
	_data.alloc(size);
}

ang::text::encoding_t ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::encoding()const
{
	return text::encoding::binary;
}

pointer ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::buffer_ptr()const
{
	return (pointer)_data.get();
}

wsize ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::buffer_size()const
{
	return _data.size() * (wsize)sizeof(MY_TYPE);
}

wsize ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::mem_copy(wsize _s, pointer _p, text::encoding_t)
{
	throw(exception_t(except_code::unsupported));
	return 0;
}

ang::ibuffer_view_t ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::map_buffer(windex start, wsize size)
{
	throw(exception_t(except_code::unsupported));
	return null;
}

bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::unmap_buffer(ang::ibuffer_view_t&, wsize used)
{
	return false;
}

bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::can_realloc_buffer()const { return false; };

bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::realloc_buffer(wsize) { return false; };

bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::move(ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>& ar)
{
	if (&ar == this)
		return false;
	clean();
	_data.move(ar._data);
	return true;
}

void ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::copy(const ang::collections::ienum<MY_TYPE>* _items)
{
	if (_items == null)
		return;
	size(_items->counter());
	windex i = 0;
	for (MY_TYPE const& value : *_items)
		_data.get()[i++] = value;
}

wsize ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::counter()const
{
	return _data.size();
}

MY_TYPE& ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::at(ang::collections::base_iterator<MY_TYPE>const& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw(exception_t(except_code::invalid_memory));
	if (_data.get() != it.current())
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _data.size())
		throw(exception_t(except_code::array_overflow));
#endif
	return _data[it.offset()];
}

ang::collections::iterator<MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::at(windex idx)
{
	if (idx >= _data.size())
		return  iterator_t(const_cast<self_t*>(this), null, 0);
	return iterator_t(const_cast<self_t*>(this), (pointer)&_data, idx);
}

ang::collections::iterator<const MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::at(windex idx)const
{
	if (idx >= _data.size())
		return  const_iterator_t(const_cast<self_t*>(this), null, 0);
	return const_iterator_t(const_cast<self_t*>(this), (pointer)&_data, idx);
}

ang::collections::iterator<MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::find(MY_TYPE const& datum, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (auto i = size(); i > 0; --i)
		{
			if (comparision_operations<MY_TYPE, MY_TYPE>::template compare<comparision_same>(_data[i - 1] , datum))
				return at(i - 1);
		}
		else for (auto i = 0U; i < size(); ++i)
		{
			if (comparision_operations<MY_TYPE, MY_TYPE>::template compare<comparision_same>(_data[i] , datum))
				return at(i);
		}
	}
	return at(invalid_index);
}

ang::collections::iterator<MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::find(MY_TYPE const& datum, ang::collections::base_iterator<MY_TYPE> next_to, bool invert)const
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

ang::collections::forward_iterator<MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::begin()
{
	return forward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data));
}

ang::collections::forward_iterator<MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::end()
{
	return forward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), _data.size());
}

ang::collections::forward_iterator<const MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::begin()const
{
	return const_forward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data));
}

ang::collections::forward_iterator<const MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::end()const
{
	return const_forward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), _data.size());
}

ang::collections::forward_iterator<MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::last()
{
	return _data.size() ? forward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), _data.size() - 1) : end();
}

ang::collections::forward_iterator<const MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::last()const
{
	return _data.size() ? const_forward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), _data.size() - 1) : end();
}

ang::collections::backward_iterator<MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::rbegin()
{
	return backward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), _data.size() - 1);
}

ang::collections::backward_iterator<MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::rend()
{
	return backward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), invalid_index);
}

ang::collections::backward_iterator<const MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::rbegin()const
{
	return const_backward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), _data.size() - 1);
}

ang::collections::backward_iterator<const MY_TYPE> ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::rend()const
{
	return const_backward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), invalid_index);
}

bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::increase(ang::collections::base_iterator<MY_TYPE>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data.get())
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _data.size())
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + 1);
	if (it.offset() > _data.size()) it.offset(_data.size());
	return true;
}

bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::increase(ang::collections::base_iterator<MY_TYPE>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data.get())
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _data.size())
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + val);
	if (it.offset() >= _data.size())
		it.offset(_data.size());
	return true;
}

bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::decrease(ang::collections::base_iterator<MY_TYPE>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data.get())
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _data.size())
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() - 1);
	if ((int)it.offset() < -1)
		it.offset(invalid_index);
	return true;
}

bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::decrease(ang::collections::base_iterator<MY_TYPE>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != _data.get())
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _data.size())
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() - val);
	if ((int)it.offset() < -1)
		it.offset(invalid_index);
	return true;
}

ang::comparision_result_t ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::compare(const ang::object& obj)const
{
	if (obj.is_kind_of(class_name()))
	{
		array_buffer<MY_TYPE, MY_ALLOCATOR>const& other = static_cast<array_buffer<MY_TYPE, MY_ALLOCATOR>const&>(obj);
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


bool ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::realloc(wsize size)
{
	_data.alloc(size);
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper()
	: _ptr(null)
{

}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper(ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>* ptr)
	: object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>()
{
	set(ptr);
}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>()
{
}

ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>()
{
	set(new collections::array_buffer<MY_TYPE, MY_ALLOCATOR>(store));
}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>> && other)
	: object_wrapper<collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>()
{
	collections::array_buffer<MY_TYPE, MY_ALLOCATOR> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>> const& other)
	: object_wrapper<collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>()
{
	set(other.get());
}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::~object_wrapper()
{
	clean();
}


void ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}


void ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::clean_unsafe()
{
	_ptr = null;
}


bool ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::is_empty()const
{
	return _ptr == null;
}


ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>* ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::set(ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>* ptr)
{
	ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}


ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>** ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::addres_of(void)
{
	return &_ptr;
}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>& ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::operator = (ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>& ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::operator = (const std::nullptr_t&)
{
	clean();
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>& ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (_ptr == null)
		set(new collections::array_buffer<MY_TYPE, MY_ALLOCATOR>(items));
	else
		_ptr->copy(items);
	return *this;
}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>& ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::operator = (ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>& ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::operator = (ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>> const& other)
{
	set(other._ptr);
	return*this;
}


ang::object_wrapper_ptr<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>> ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::operator & (void)
{
	return this;
}


ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR> * ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::operator -> (void)
{
	return get();
}


ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR> const* ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::operator -> (void)const
{
	return get();
}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::operator ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR> * (void)
{
	return get();
}


ang::object_wrapper<ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR>>::operator ang::collections::array_buffer<MY_TYPE, MY_ALLOCATOR> const* (void)const
{
	return get();
}

///////////////////////////////////////////////////////////////


