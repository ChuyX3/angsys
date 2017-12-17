/*********************************************************************************************************************/
/*   File Name: ang/collections/inline/array.hpp                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_HPP__
#error Can't include array.inl, please include angsys.h inside
#elif !defined __ANG_COLLECTIONS_ARRAY_INL__
#define __ANG_COLLECTIONS_ARRAY_INL__


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// array_buffer<T, allocator> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::array_buffer()
	: object()
	, _data(null)
{
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::array_buffer(wsize reserve)
	: array_buffer()
{
	_data.alloc(reserve);
}

template<typename T, template <typename> class allocator> template<typename U>
inline ang::collections::array_buffer<T, allocator>::array_buffer(std::initializer_list<U> list)
	: array_buffer()
{
	wsize c = 0;
	_data.alloc(list.size());
	for (auto it = list.begin(); it != list.end(); ++it)
		_data[c++] = *it;	
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::array_buffer(const std::nullptr_t&)
	: array_buffer()
{

}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::array_buffer(ang::collections::array_buffer<T, allocator>&& ar)
	: array_buffer()
{
	_data.move(ar._data);
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::array_buffer(const ang::collections::array_buffer<T, allocator>& ar)
	: array_buffer()
{
	copy(&static_cast<const ienum_t&>(ar));
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::array_buffer(const ang::collections::array_buffer<T, allocator>* ar)
	: array_buffer()
{
	if (ar) copy(static_cast<const ienum_t*>(ar));
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::array_buffer(const ang::collections::ienum<T>* store)
	: array_buffer()
{
	copy(store);
}

template<typename T, template <typename> class allocator> template<typename U>
inline ang::collections::array_buffer<T, allocator>::array_buffer(ang::array_view<U> const& ar)
	: array_buffer()
{
	copy(ar);
}

template<typename T, template <typename> class allocator> template<typename U, template<typename> class allocator2>
inline ang::collections::array_buffer<T, allocator>::array_buffer(ang::scope_array<U, allocator2> const& ar)
	: array_buffer()
{
	copy(ar);
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::~array_buffer()
{
	clean();
}

template<typename T, template <typename> class allocator>
inline ang::type_name_t ang::collections::array_buffer<T, allocator>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::collections::array<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::is_inherited_of(ang::type_name_t name)
{
	if (name == type_of<array_buffer<T, allocator>>()
		|| ang::object::is_inherited_of(name)
		|| ang::collections::iarray<T>::is_inherited_of(name))
		return true;
	return false;
}

template<typename T, template <typename> class allocator>
inline ang::type_name_t ang::collections::array_buffer<T, allocator>::object_name()const
{
	return ang::collections::array_buffer<T, allocator>::class_name();
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_of<array_buffer<T, allocator>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::iarray<T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (className == type_of<array_buffer<T, allocator>>())
	{
		*out = static_cast<ang::collections::array_buffer<T, allocator>*>(this);
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

template<typename T, template <typename> class allocator>
inline void ang::collections::array_buffer<T, allocator>::clean()
{
	_data.clean();
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::is_empty()const
{
	return null == _data.get();
}

template<typename T, template <typename> class allocator>
inline T* ang::collections::array_buffer<T, allocator>::data()const
{
	return _data.get();
}

template<typename T, template <typename> class allocator>
inline wsize ang::collections::array_buffer<T, allocator>::size()const
{
	return _data.size();
}

template<typename T, template <typename> class allocator>
inline void ang::collections::array_buffer<T, allocator>::size(wsize size)
{
	if (size == _data.size())
		return;
	_data.alloc(size);
}

template<typename T, template <typename> class allocator> template<typename U>
inline void ang::collections::array_buffer<T, allocator>::copy(ang::array_view<U> const& ar)
{
	_data.copy(ar);
}

template<typename T, template <typename> class allocator> template<typename U, template<typename> class allocator2>
inline void ang::collections::array_buffer<T, allocator>::copy(ang::scope_array<U, allocator2> const& ar)
{
	_data.copy(ar);
}

template<typename T, template<typename> class allocator>
inline ang::text::encoding_t ang::collections::array_buffer<T, allocator>::encoding()const
{
	return text::encoding::binary;
}

template<typename T, template <typename> class allocator>
inline pointer ang::collections::array_buffer<T, allocator>::buffer_ptr()const
{
	return (pointer)_data.get();
}

template<typename T, template <typename> class allocator>
inline wsize ang::collections::array_buffer<T, allocator>::buffer_size()const
{
	return _data.size() * (wsize)sizeof(T);
}

template<typename T, template <typename> class allocator>
inline wsize ang::collections::array_buffer<T, allocator>::mem_copy(wsize _s, pointer _p, text::encoding_t)
{
	throw(exception_t(except_code::unsupported));
	return 0;
}

template<typename T, template <typename> class allocator>
inline ang::ibuffer_view_t ang::collections::array_buffer<T, allocator>::map_buffer(windex start, wsize size)
{
	throw(exception_t(except_code::unsupported));
	return null;
}

template<typename T, template <typename> class allocator>
bool ang::collections::array_buffer<T, allocator>::unmap_buffer(ang::ibuffer_view_t&, wsize used)
{
	return false;
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::can_realloc_buffer()const { return false; };

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::realloc_buffer(wsize) { return false; };


template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::move(ang::collections::array_buffer<T, allocator>& ar)
{
	if (&ar == this)
		return false;
	clean();
	_data.move(ar._data);
	return true;
}

template<typename T, template <typename> class allocator>
inline void ang::collections::array_buffer<T, allocator>::copy(const ang::collections::ienum<T>* _items)
{
	if (_items == null)
		return;
	size(_items->counter());
	windex i = 0;
	for (T const& value : *_items)
		_data.get()[i++] = value;
}

template<typename T, template <typename> class allocator>
inline wsize ang::collections::array_buffer<T, allocator>::counter()const
{
	return _data.size();
}

template<typename T, template <typename> class allocator>
inline T& ang::collections::array_buffer<T, allocator>::at(ang::collections::base_iterator<T>const& it)
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

template<typename T, template <typename> class allocator>
inline ang::collections::iterator<T> ang::collections::array_buffer<T, allocator>::at(windex idx)
{
	if (idx >= _data.size())
		return  iterator_t(const_cast<self_t*>(this), null, 0);
	return iterator_t(const_cast<self_t*>(this), (pointer)&_data, idx);
}

template<typename T, template <typename> class allocator>
inline ang::collections::iterator<const T> ang::collections::array_buffer<T, allocator>::at(windex idx)const
{
	if (idx >= _data.size())
		return  const_iterator_t(const_cast<self_t*>(this), null, 0);
	return const_iterator_t(const_cast<self_t*>(this), (pointer)&_data, idx);
}

template<typename T, template <typename> class allocator>
inline ang::collections::iterator<T> ang::collections::array_buffer<T, allocator>::find(T const& datum, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (auto i = size(); i > 0; --i)
		{
			if (comparision_operations<T, T>::template compare<comparision_same>(_data[i - 1] , datum))
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

template<typename T, template <typename> class allocator>
inline ang::collections::iterator<T> ang::collections::array_buffer<T, allocator>::find(T const& datum, ang::collections::base_iterator<T> next_to, bool invert)const
{
	if (!is_empty() && next_to.parent() == this)
	{
		if (invert) for (auto i = next_to.offset() + 1; i > 0; --i)
		{
			if (comparision_operations<T, T>::template compare<comparision_same>(_data[i - 1], datum))
				return at(i - 1);
		}
		else for (auto i = next_to.offset(); i < size(); ++i)
		{
			if (comparision_operations<T, T>::template compare<comparision_same>(_data[i], datum))
				return at(i);
		}
	}
	return at(invalid_index);
}

template<typename T, template <typename> class allocator>
inline ang::collections::forward_iterator<T> ang::collections::array_buffer<T, allocator>::begin()
{
	return forward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data));
}

template<typename T, template <typename> class allocator>
inline ang::collections::forward_iterator<T> ang::collections::array_buffer<T, allocator>::end()
{
	return forward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), _data.size());
}

template<typename T, template <typename> class allocator>
inline ang::collections::forward_iterator<const T> ang::collections::array_buffer<T, allocator>::begin()const
{
	return const_forward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data));
}

template<typename T, template <typename> class allocator>
inline ang::collections::forward_iterator<const T> ang::collections::array_buffer<T, allocator>::end()const
{
	return const_forward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), _data.size());
}


template<typename T, template <typename> class allocator>
inline ang::collections::backward_iterator<T> ang::collections::array_buffer<T, allocator>::rbegin()
{
	return backward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), _data.size() - 1);
}

template<typename T, template <typename> class allocator>
inline ang::collections::backward_iterator<T> ang::collections::array_buffer<T, allocator>::rend()
{
	return backward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), invalid_index);
}

template<typename T, template <typename> class allocator>
inline ang::collections::backward_iterator<const T> ang::collections::array_buffer<T, allocator>::rbegin()const
{
	return const_backward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), _data.size() - 1);
}

template<typename T, template <typename> class allocator>
inline ang::collections::backward_iterator<const T> ang::collections::array_buffer<T, allocator>::rend()const
{
	return const_backward_iterator_t(const_cast<array_buffer*>(this), position_t(&_data), invalid_index);
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::increase(ang::collections::base_iterator<T>& it)const
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

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::increase(ang::collections::base_iterator<T>& it, int val)const
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

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::decrease(ang::collections::base_iterator<T>& it)const
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

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::decrease(ang::collections::base_iterator<T>& it, int val)const
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

template<typename T, template <typename> class allocator>
inline ang::comparision_result_t ang::collections::array_buffer<T, allocator>::compare(const ang::object& obj)const
{
	if (obj.is_kind_of(class_name()))
	{
		array_buffer<T, allocator>const& other = static_cast<array_buffer<T, allocator>const&>(obj);
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


template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::realloc(wsize size)
{
	_data.alloc(size);
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(ang::collections::array_buffer<T, allocator>* ptr)
	: object_wrapper<ang::collections::array_buffer<T, allocator>>()
{
	set(ptr);
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper<ang::collections::array_buffer<T, allocator>>()
{
}

template<typename T, template <typename> class allocator> template<typename U>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(std::initializer_list<U> list)
	: object_wrapper<ang::collections::array_buffer<T, allocator>>()
{
	set(new collections::array_buffer<T, allocator>(ang::move(list)));
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: object_wrapper<ang::collections::array_buffer<T, allocator>>()
{
	set(new collections::array_buffer<T, allocator>(store));
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<T, allocator>> && other)
	: object_wrapper<collections::array_buffer<T, allocator>>()
{
	collections::array_buffer<T, allocator> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<T, allocator>> const& other)
	: object_wrapper<collections::array_buffer<T, allocator>>()
{
	set(other.get());
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::~object_wrapper()
{
	clean();
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::clean_unsafe()
{
	_ptr = null;
}

template<typename T, template <typename> class allocator>
inline bool ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::is_empty()const
{
	return _ptr == null;
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>* ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::get(void)const
{
	return _ptr;
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::set(ang::collections::array_buffer<T, allocator>* ptr)
{
	ang::collections::array_buffer<T, allocator> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>** ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::addres_of(void)
{
	return &_ptr;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>& ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator = (ang::collections::array_buffer<T, allocator>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>& ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator = (const std::nullptr_t&)
{
	clean();
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>& ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (_ptr == null)
		set(new collections::array_buffer<T, allocator>(items));
	else
		_ptr->copy(items);
	return *this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>& ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator = (ang::object_wrapper<ang::collections::array_buffer<T, allocator>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>& ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator = (ang::object_wrapper<ang::collections::array_buffer<T, allocator>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper_ptr<ang::collections::array_buffer<T, allocator>> ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator & (void)
{
	return this;
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator> * ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator -> (void)
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator> const* ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator -> (void)const
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator ang::collections::array_buffer<T, allocator> * (void)
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator ang::collections::array_buffer<T, allocator> const* (void)const
{
	return get();
}

template<typename T, template <typename> class allocator> template<typename I>
inline T const& ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator[](I const& idx)const
{
	static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
		if (is_empty()) throw(exception_t(except_code::invalid_memory));
		if ((idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

template<typename T, template <typename> class allocator> template<typename I>
inline T & ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator[](I const& idx)
{
	static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
		if (is_empty()) throw(exception_t(except_code::invalid_memory));
		if ((idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

///////////////////////////////////////////////////////////////


template<typename T, template <typename> class allocator>
ang::collections::scope_array<ang::object_wrapper<T>, allocator>::scope_array()
	: _size(0)
	, _data(null) {

}

template<typename T, template <typename> class allocator>
ang::collections::scope_array<ang::object_wrapper<T>, allocator>::scope_array(ang::nullptr_t const&)
	: _size(0)
	, _data(null) {

}

template<typename T, template <typename> class allocator>
ang::collections::scope_array<ang::object_wrapper<T>, allocator>::scope_array(wsize sz, ang::object_wrapper<T>* val)
	: _size(0)
	, _data(ang::null) {
	_size = min(wsize(-1) / sizeof(T), sz);
	if (_size > 0) {
		_data = allocator<ang::object_wrapper<T>>::alloc(_size);
		for (windex i = 0; i < _size; ++i)
			allocator<ang::object_wrapper<T>>::template construct<ang::object_wrapper<T> const&>((ang::object_wrapper<T>*)&_data[i], val[i]);
	}
}

template<typename T, template <typename> class allocator>
ang::collections::scope_array<ang::object_wrapper<T>, allocator>::scope_array(scope_array const& other)
	: _size(other._size)
	, _data(null) {
	if (_size > 0) {
		_data = allocator<ang::object_wrapper<T>>::alloc(_size);
		for (windex i = 0; i < _size; ++i)
			allocator<ang::object_wrapper<T>>::template construct<ang::object_wrapper<T> const&>((ang::object_wrapper<T>*)&_data[i], other._data[i]);
	}
}

template<typename T, template <typename> class allocator>
ang::collections::scope_array<ang::object_wrapper<T>, allocator>::scope_array(scope_array&& other)
	: _size(ang::move(other._size))
	, _data(ang::move(other._data)) {
}

template<typename T, template <typename> class allocator>
template<typename U, wsize N>
ang::collections::scope_array<ang::object_wrapper<T>, allocator>::scope_array(U(&ar)[N])
	: _size(N)
	, _data(null) {
	if (_size > 0) {
		_data = allocator<ang::object_wrapper<T>>::alloc(_size);
		for (windex i = 0; i < _size; ++i)
			allocator<ang::object_wrapper<T>>::template construct<U const&>((ang::object_wrapper<T>*)&_data[i], ar[i]);
	}
}

template<typename T, template <typename> class allocator>
template<typename U, wsize N>
ang::collections::scope_array<ang::object_wrapper<T>, allocator>::scope_array(const U(&ar)[N])
	: _size(N)
	, _data(null) {
	if (_size > 0) {
		_data = allocator<ang::object_wrapper<T>>::alloc(_size);
		for (windex i = 0; i < _size; ++i)
			allocator<ang::object_wrapper<T>>::template construct<U const&>((ang::object_wrapper<T>*)&_data[i], ar[i]);
	}
}

template<typename T, template <typename> class allocator>
void ang::collections::scope_array<ang::object_wrapper<T>, allocator>::clean()
{
	if (_data)
	{
		for (windex i = 0; i < _size; ++i)
			allocator<ang::object_wrapper<T>>::destruct((ang::object_wrapper<T>*)&_data[i]);
		allocator<ang::object_wrapper<T>>::free(_data);
	}
	_size = 0;
	_data = null;
}


template<typename T, template <typename> class allocator>
ang::object_wrapper<T>* ang::collections::scope_array<ang::object_wrapper<T>, allocator>::alloc(wsize size)
{
	clean();
	if (size > 0)
	{
		_size = size;
		_data = allocator<ang::object_wrapper<T>>::alloc(_size);
		for (windex i = 0; i < _size; ++i)
			allocator<ang::object_wrapper<T>>::template construct<T*>((ang::object_wrapper<T>*)&_data[i], null);
	}
	return _data;
}

template<typename T, template <typename> class allocator>
void ang::collections::scope_array<ang::object_wrapper<T>, allocator>::set(ang::object_wrapper<T>* val, wsize sz)
{
	clean();
	_size = min(wsize(-1) / sizeof(T), sz);
	if (_size > 0) {
		_data = allocator<ang::object_wrapper<T>>::alloc(_size);
		for (windex i = 0; i < _size; ++i)
			allocator<ang::object_wrapper<T>>::template construct<ang::object_wrapper<T> const&>((ang::object_wrapper<T>*)&_data[i], val[i]);
	}
}

template<typename T, template <typename> class allocator>
void ang::collections::scope_array<ang::object_wrapper<T>, allocator>::move(scope_array& other)
{
	if (&other == this)
		return;
	clean();
	_size = other._size;
	_data = other._data;
	other._size = 0;
	other._data = null;
}

template<typename T, template <typename> class allocator>
void ang::collections::scope_array<ang::object_wrapper<T>, allocator>::move(ang::array_view<ang::object_wrapper<T>>& other)
{
	clean();
	if (other.size() > 0)
		_data = allocator<ang::object_wrapper<T>>::alloc(other.size());
	_size = other.size();

	for (windex i = 0; i < _size; ++i)
		allocator<ang::object_wrapper<T>>::template construct<ang::object_wrapper<T>&&>((ang::object_wrapper<T>*)&_data[i], ang::move(other.get()[i]));
	other.set(null, 0);
}

template<typename T, template <typename> class allocator> template<typename U, template<typename> class allocator2>
void ang::collections::scope_array<ang::object_wrapper<T>, allocator>::copy(ang::scope_array<U, allocator2> const& other)
{
	clean();
	if (other.size() > 0)
		_data = allocator<ang::object_wrapper<T>>::alloc(other.size());
	_size = other.size();

	for (windex i = 0; i < _size; ++i)
		allocator<ang::object_wrapper<T>>::template construct<U const&>((ang::object_wrapper<T>*)&_data[i], other[i]);
	other.set(null, 0);
}

template<typename T, template <typename> class allocator> template<typename U>
void ang::collections::scope_array<ang::object_wrapper<T>, allocator>::copy(ang::array_view<U> const& other)
{
	clean();
	if (other.size() > 0)
		_data = allocator<ang::object_wrapper<T>>::alloc(other.size());
	_size = other.size();

	for (windex i = 0; i < _size; ++i)
		allocator<ang::object_wrapper<T>>::template construct<U const&>((ang::object_wrapper<T>*)&_data[i], other[i]);
}

template<typename T, template <typename> class allocator>
template<typename U, wsize N> void ang::collections::scope_array<ang::object_wrapper<T>, allocator>::copy(U(&ar)[N]) {
	clean();
	_size = N;
	if (_size > 0) {
		_data = allocator<ang::object_wrapper<T>>::alloc(_size);
		for (windex i = 0; i < _size; ++i)
			allocator<ang::object_wrapper<T>>::template construct<U const&>((ang::object_wrapper<T>*)&_data[i], ar[i]);
	}
}

template<typename T, template <typename> class allocator>
template<typename U, wsize N> void ang::collections::scope_array<ang::object_wrapper<T>, allocator>::copy(const U(&ar)[N]) {
	clean();
	_size = N;
	if (_size > 0) {
		_data = allocator<ang::object_wrapper<T>>::alloc(_size);
		for (windex i = 0; i < _size; ++i)
			allocator<ang::object_wrapper<T>>::template construct<U const&>((ang::object_wrapper<T>*)&_data[i], ar[i]);
	}
}


#endif//__ANG_COLLECTIONS_ARRAY_INL__