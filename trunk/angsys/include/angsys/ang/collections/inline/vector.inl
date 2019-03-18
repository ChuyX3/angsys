#ifndef __ANG_COLLECTIONS_VECTOR_H__
#error Can't include vector.inl, please include vector.hpp inside
#elif !defined __ANG_COLLECTIONS_VECTOR_INL__
#define __ANG_COLLECTIONS_VECTOR_INL__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// vector_buffer<T, allocator> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, template<typename> class A>
inline ang::collections::vector_buffer<T, A>::vector_buffer()
	: m_size(0)
	, m_capacity(0)
	, m_data(null)
{
}

template<typename T, template<typename> class A>  template<typename U>
inline ang::collections::vector_buffer<T, A>::vector_buffer(ang::initializer_list<U> list)
	: vector_buffer()
{
	capacity((wsize)list.size());
	for (auto it = list.begin(); it != list.end(); ++it)
		push(*it);
}


template<typename T, template<typename> class A>
inline ang::collections::vector_buffer<T, A>::vector_buffer(const ang::nullptr_t&)
	: vector_buffer()
{

}

template<typename T, template<typename> class A>
inline ang::collections::vector_buffer<T, A>::vector_buffer(ang::collections::vector_buffer<T, A>&& ar)
	: vector_buffer()
{
	m_size = ar.m_size;
	m_data = ar.m_data;
	m_capacity = ar.m_capacity;
	ar.m_size = 0;
	ar.m_capacity = 0;
	ar.m_data = null;
}

template<typename T, template<typename> class A>
inline ang::collections::vector_buffer<T, A>::vector_buffer(const ang::collections::vector_buffer<T, A>& ar)
	: vector_buffer()
{
	copy(to_array(ar.data(), ar.size()));
}

template<typename T, template<typename> class A>
inline ang::collections::vector_buffer<T, A>::vector_buffer(const ang::collections::vector_buffer<T, A>* ar)
	: vector_buffer()
{
	if (ar)copy(to_array(ar->data(), ar->size()));
}

template<typename T, template<typename> class A>
inline ang::collections::vector_buffer<T, A>::vector_buffer(const ang::collections::ienum<T>* store)
	: vector_buffer()
{
	copy(store);
}


template<typename T, template <typename> class A> template<typename U>
inline ang::collections::vector_buffer<T, A>::vector_buffer(ang::array_view<U> const& ar)
	: vector_buffer()
{
	copy(ar);
}

template<typename T, template <typename> class A> template<typename U, template<typename> class A2>
inline ang::collections::vector_buffer<T, A>::vector_buffer(ang::scope_array<U, A2> const& ar)
	: vector_buffer()
{
	copy(ar);
}

template<typename T, template<typename> class A>
inline ang::collections::vector_buffer<T, A>::~vector_buffer()
{
}

template<typename T, template <typename> class A>
inline ang::rtti_t const& ang::collections::vector_buffer<T, A>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::collections::vector<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<ilist<T>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::vector_buffer<T, A>), alignof(ang::collections::vector_buffer<T, A>), parents, &default_query_interface);
	return info;
}

template<typename T, template <typename> class A>
inline ang::rtti_t const& ang::collections::vector_buffer<T, A>::runtime_info()const
{
	return class_info();
}

template<typename T, template <typename> class A>
inline bool ang::collections::vector_buffer<T, A>::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == class_info().type_id())
	{
		if (out == null) return false;
		*out = static_cast<vector_buffer<T, A>*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == type_of<ilist<T>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ilist<T>*>(this);
		return true;
	}
	else if (id.type_id() == type_of<iarray<T>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<iarray<T>*>(this);
		return true;
	}
	else if (id.type_id() == type_of<ienum<T>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ienum<T>*>(this);
		return true;
	}
	return false;
}


template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::is_empty()const
{
	return size() == 0;
}

template<typename T, template<typename> class A>
inline T* ang::collections::vector_buffer<T, A>::data()const
{
	return m_data;
}

template<typename T, template<typename> class A>
inline wsize ang::collections::vector_buffer<T, A>::size()const
{
	return m_size;
}

template<typename T, template<typename> class A>
inline void ang::collections::vector_buffer<T, A>::size(wsize size, bool save)
{
	if (m_size != size)
	{
		realloc(size, save);
		for (int i = size - m_size; i > 0 && i < int(size); ++i)
		{
			m_alloc.template construct<T, T const&>((T*)&m_data[i], default_value<T>::value);
		}
	}
}

template<typename T, template<typename> class A>
inline wsize ang::collections::vector_buffer<T, A>::capacity()const
{
	return m_capacity;
}

template<typename T, template<typename> class A>
inline void ang::collections::vector_buffer<T, A>::capacity(wsize size, bool save)
{
	realloc(size, save);
}

template<typename T, template<typename> class A> template<typename U>
inline void ang::collections::vector_buffer<T, A>::copy(ang::array_view<U> const& ar)
{
	capacity(ar.size());
	wsize l = ar.size();
	for (wsize i = 0U; i < l; i++)
		push(ar[i]);
}

template<typename T, template<typename> class A>  template<typename U, template<typename> class A2>
inline void ang::collections::vector_buffer<T, A>::copy(ang::scope_array<U, A2> const& ar)
{
	capacity(ar.size());
	wsize l = ar.size();
	for (wsize i = 0U; i < l; i++)
		push(ar[i]);
}

template<typename T, template<typename> class A>  template<typename U, wsize SIZE>
inline void ang::collections::vector_buffer<T, A>::copy(ang::stack_array<U, SIZE> const& ar)
{
	capacity(SIZE);
	for (wsize i = 0U; i < SIZE; i++)
		push(ar[i]);
}

template<typename T, template<typename> class A> template<typename U>
inline void ang::collections::vector_buffer<T, A>::extend(ang::array_view<U> const& ar)
{
	capacity(size() + ar.size(), true);
	wsize l = ar.size();
	for (wsize i = 0U; i < l; i++)
		push(ar[i]);
}

template<typename T, template<typename> class A>  template<typename U, template<typename> class A2>
inline void ang::collections::vector_buffer<T, A>::extend(ang::scope_array<U, A2> const& ar)
{
	capacity(size() + ar.size(), true);
	wsize l = ar.size();
	for (wsize i = 0U; i < l; i++)
		push(ar[i]);
}

template<typename T, template<typename> class A>  template<typename U, wsize SIZE>
inline void ang::collections::vector_buffer<T, A>::extend(ang::stack_array<U, SIZE> const& ar)
{
	capacity(size() + SIZE, true);
	for (wsize i = 0U; i < SIZE; i++)
		push(ar[i]);
}

template<typename T, template <typename> class A>
inline ang::rtti_t const& ang::collections::vector_buffer<T, A>::value_type()const
{
	return type_of<array_view<T>>();
}

template<typename T, template <typename> class A>
inline bool ang::collections::vector_buffer<T, A>::set_value(ang::rtti_t const& id, ang::unknown_t value)
{
	if (id.is_type_of<array_view<T>>())
	{
		array_view<T>& ar = *reinterpret_cast<array_view<T>*>(value);
		copy(ar);
		return true;
	}
	return false;
}

template<typename T, template <typename> class A>
inline bool ang::collections::vector_buffer<T, A>::get_value(ang::rtti_t const& id, ang::unknown_t value)const
{
	if (id.is_type_of<array_view<T>>())
	{
		array_view<T>& ar = *reinterpret_cast<array_view<T>*>(value);
		ar.set(m_data, m_size);
	}
	return false;
}

template<typename T, template <typename> class A>
inline ang::variant ang::collections::vector_buffer<T, A>::clone()const
{
	return (ivariant*) new ang::collections::vector_buffer<T, A>(*this);
}

template<typename T, template <typename> class A>
inline ang::wstring ang::collections::vector_buffer<T, A>::to_string()const
{
	return class_info().type_name();
}

template<typename T, template <typename> class A>
inline ang::wstring ang::collections::vector_buffer<T, A>::to_string(ang::text::text_format_t)const
{
	return class_info().type_name();
}


template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::is_readonly()const
{
	return false;
}

template<typename T, template<typename> class A>
inline ang::text::encoding_t ang::collections::vector_buffer<T, A>::encoding()const
{
	return text::encoding::binary;
}

template<typename T, template<typename> class A>
inline const_pointer ang::collections::vector_buffer<T, A>::buffer_ptr()const
{
	return (pointer)m_data;
}

template<typename T, template<typename> class A>
inline pointer ang::collections::vector_buffer<T, A>::buffer_ptr()
{
	return (pointer)m_data;
}

template<typename T, template<typename> class A>
inline wsize ang::collections::vector_buffer<T, A>::buffer_size()const
{
	return m_capacity * sizeof(T);
}

template<typename T, template<typename> class A>
inline wsize ang::collections::vector_buffer<T, A>::mem_copy(wsize _s, pointer _p, text::encoding_t)
{
	throw(exception_t(except_code::unsupported));
	return 0;
}

template<typename T, template<typename> class A>
inline ang::ibuffer_view_t ang::collections::vector_buffer<T, A>::map_buffer(windex start, wsize size)
{
	return null;
}

template<typename T, template<typename> class A>
bool ang::collections::vector_buffer<T, A>::unmap_buffer(ang::ibuffer_view_t&, wsize used)
{
	return false;
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::can_realloc_buffer()const { return true; };

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::realloc_buffer(wsize size) { return realloc(size, true); };

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::move(ang::collections::vector_buffer<T, A>& ar)
{
	if (&ar == this)
		return false;
	clear();
	m_size = ar.m_size;
	m_data = ar.m_data;
	m_capacity = ar.m_capacity;
	ar.m_data = null;
	ar.m_size = 0;
	ar.m_capacity = 0;
	return true;
}

template<typename T, template<typename> class A>
inline void ang::collections::vector_buffer<T, A>::copy(const ang::collections::ienum<T>* _items)
{
	if (!_items)
		return;
	capacity(_items->counter(), false);
	for (T const& value : *_items)
		push(value);	
}


template<typename T, template<typename> class A>
inline wsize ang::collections::vector_buffer<T, A>::counter()const
{
	return m_size;
}

template<typename T, template<typename> class A>
inline T& ang::collections::vector_buffer<T, A>::at(ang::collections::base_iterator<T> const& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw(exception_t(except_code::invalid_memory));
	if (m_data != it.current())
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= m_size)
		throw(exception_t(except_code::array_overflow));
#endif
	return m_data[it.offset()];
}

template<typename T, template<typename> class A>
inline ang::collections::iterator<T> ang::collections::vector_buffer<T, A>::at(windex idx)
{
	if (idx >= m_size)
		return  iterator_t(const_cast<self_t*>(this), null, 0);
	return iterator_t(const_cast<self_t*>(this), (pointer)m_data, idx);
}

template<typename T, template<typename> class A>
inline ang::collections::iterator<const T> ang::collections::vector_buffer<T, A>::at(windex idx)const
{
	if (idx >= m_size)
		return  const_iterator_t(const_cast<self_t*>(this), null, 0);
	return const_iterator_t(const_cast<self_t*>(this), (pointer)m_data, idx);
}

template<typename T, template <typename> class A>
inline ang::collections::iterator<T> ang::collections::vector_buffer<T, A>::find(ang::core::delegates::function<bool(T const&)> cond, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (auto i = size(); i > 0; --i)
		{
			if (cond(m_data[i - 1]))
				return at(i - 1);
		}
		else for (auto i = 0U; i < size(); ++i)
		{
			if (cond(m_data[i]))
				return at(i);
		}
	}
	return at((wsize)invalid_index);
}

template<typename T, template <typename> class A>
inline ang::collections::iterator<T> ang::collections::vector_buffer<T, A>::find(ang::core::delegates::function<bool(T const&)> cond, ang::collections::base_iterator<T> next_to, bool invert)const
{
	if (!is_empty() && next_to.parent() == this)
	{
		if (invert) for (auto i = next_to.offset() + 1; i > 0; --i)
		{
			if (cond(m_data[i - 1]))
				return at(i - 1);
		}
		else for (auto i = next_to.offset(); i < size(); ++i)
		{
			if (cond(m_data[i]))
				return at(i);
		}
	}
	return at((wsize)invalid_index);
}

template<typename T, template <typename> class A>
inline ang::collections::ienum_ptr<T> ang::collections::vector_buffer<T, A>::find_all(ang::core::delegates::function<bool(T const&)> cond)const
{
	vector<T, A> out = new vector_buffer<T, A>();
	if (!is_empty())
	{
		for (auto i = size(); i > 0; --i)
		{
			if (cond(m_data[i - 1]))
				out += m_data[i - 1];
		}
	}
	return out->is_empty() ? null : out.get();
}

template<typename T, template <typename> class A> template<typename U>
inline ang::collections::ienum_ptr<U> ang::collections::vector_buffer<T, A>::find_all(ang::core::delegates::function<bool(T const&, U& out)> cond)const
{
	vector<U, A> out = new vector_buffer<U, A>();
	if (!is_empty())
	{
		for (auto i = size(); i > 0; --i)
		{
			U val = default_value<U>::value;
			if (cond(m_data[i - 1], val))
				out += ang::move(val);
		}
	}
	return out->is_empty() ? null : out.get();
}


template<typename T, template<typename> class A>
inline ang::collections::forward_iterator<T> ang::collections::vector_buffer<T, A>::begin()
{
	return forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data));
}

template<typename T, template<typename> class A>
inline ang::collections::forward_iterator<T> ang::collections::vector_buffer<T, A>::end()
{
	return forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size);
}

template<typename T, template<typename> class A>
inline ang::collections::forward_iterator<const T> ang::collections::vector_buffer<T, A>::begin()const
{
	return const_forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data));
}

template<typename T, template<typename> class A>
inline ang::collections::forward_iterator<const T> ang::collections::vector_buffer<T, A>::end()const
{
	return const_forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size);
}

template<typename T, template<typename> class A>
inline ang::collections::forward_iterator<T> ang::collections::vector_buffer<T, A>::last()
{
	return m_size ? forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size - 1) : end();
}

template<typename T, template<typename> class A>
inline ang::collections::forward_iterator<const T> ang::collections::vector_buffer<T, A>::last()const
{
	return m_size ? const_forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size - 1) : end();
}

template<typename T, template<typename> class A>
inline ang::collections::backward_iterator<T> ang::collections::vector_buffer<T, A>::rbegin()
{
	return backward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size - 1);
}

template<typename T, template<typename> class A>
inline ang::collections::backward_iterator<T> ang::collections::vector_buffer<T, A>::rend()
{
	return backward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), (wsize)invalid_index);
}

template<typename T, template<typename> class A>
inline ang::collections::backward_iterator<const T> ang::collections::vector_buffer<T, A>::rbegin()const
{
	return const_backward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size - 1);
}

template<typename T, template<typename> class A>
inline ang::collections::backward_iterator<const T> ang::collections::vector_buffer<T, A>::rend()const
{
	return const_backward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), (wsize)invalid_index);
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::increase(ang::collections::base_iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != m_data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= m_size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + 1);
	if (it.offset() > m_size) it.offset(m_size);
	return true;
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::increase(ang::collections::base_iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != m_data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= m_size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() + val);
	if (it.offset() >= m_size)
		it.offset(m_size);
	return true;
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::decrease(ang::collections::base_iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != m_data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= m_size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() - 1);
	if ((int)it.offset() < -1)
		it.offset((wsize)invalid_index);
	return true;
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::decrease(ang::collections::base_iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != m_data)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= m_size)
		throw(exception_t(except_code::array_overflow));
#endif
	it.offset(it.offset() - val);
	if ((int)it.offset() < -1)
		it.offset((wsize)invalid_index);
	return true;
}

template<typename T, template<typename> class A>
inline ang::comparision_result_t ang::collections::vector_buffer<T, A>::compare(const ang::object* obj)const
{
	windex i = 0;
	intf_wrapper<ienum<T>> other = const_cast<object*>(obj)->as<ienum<T>>();

	if (!other.is_empty())
	{
		if (counter() > other->counter())
			return comparision_result::mayor;
		else if (counter() < other->counter())
			return comparision_result::minor;
		else for (auto it = other->begin(); it != other->end(); ++it)
			if (logic_operation<T, T, logic_operation_type::same>::operate(m_data[i++], *it))
				return comparision_result::diferent;
		return comparision_result::same;
	}
	return comparision_result::diferent;
}


template<typename T, template<typename> class A>
inline void ang::collections::vector_buffer<T, A>::extend(const ang::collections::ienum<T>* items)
{
	if (!items) return;
	capacity(counter() + items->counter());
	for (T const& value : *items)
		push(value);
}

template<typename T, template<typename> class A>
inline void ang::collections::vector_buffer<T, A>::push(T const& value, bool last)
{
	if ((capacity() - counter()) <= 1U) //TODO: optimization
		capacity(capacity() + 1, true);

	if (last)
	{	
		m_alloc.template construct<T, T const&>((T*)&m_data[m_size++], value);
	}
	else
	{
		for (auto i = m_size; i > 0U; --i)
		{
			m_alloc.template construct<T,T&&>((T*)&m_data[i], ang::move(m_data[i - 1U]));
			m_alloc.destroy((T*)&m_data[i - 1U]);
		}
		m_alloc.template construct<T,T const&>((T*)&m_data[0], value);
		m_size++;
	}
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::insert(windex idx, T const& value)
{
	if (idx == 0U)
	{
		push(value, false);
		return true;
	}
	else if (idx >= m_size)
	{
		push(value, true);
		return true;
	}

	if ((capacity() - counter()) <= 1U) //TODO: optimization
		capacity(capacity() + 1, true);

	for (auto i = m_size; i > idx; --i)
	{
		m_alloc.template construct<T,T&&>((T*)&m_data[i], ang::move(m_data[i - 1U]));
		m_alloc.destroy((T*)&m_data[i - 1U]);
	}
	m_alloc.template construct<T,T const&>((T*)&m_data[idx], value);
	m_size++;
	return true;
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::insert(ang::collections::base_iterator<T> it, T const& value)
{
	if (it.parent() != this || it.current() != m_data)
		return false;
	return insert(it.offset(), value);
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::pop(bool last)
{
	if (m_size == 0)
		return false;
	if (!last) for (windex i = 1U; i < m_size; ++i)
		m_data[i - 1] = ang::move(m_data[i]);
	m_alloc.destroy((T*)&(m_data[--m_size]));
	return true;
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::pop(T& value, bool last)
{
	if (m_size == 0)
		return false;
	if (!last)
	{
		value = ang::move(m_data[0]);
		for (windex i = 1U; i < m_size; ++i)
			m_data[i - 1] = ang::move(m_data[i]);
	}
	else
	{
		value = ang::move(m_data[m_size - 1U]);
	}
	m_alloc.destroy((T*)&(m_data[--m_size]));
	return true;
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::pop_at(windex idx)
{
	if (m_size == 0U)
		return false;
	if (idx == 0U)
		return pop(false);
	for (windex i = idx; i < m_size; ++i)
		m_data[i - 1] = ang::move(m_data[i]);
	m_alloc.destroy((T*)&(m_data[--m_size]));
	return true;
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::pop_at(ang::collections::base_iterator<T> it)
{
	if (it.parent() != this || it.current() != m_data)
		return false;
	return pop_at(it.offset());
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::pop_at(windex idx, T& value)
{
	if (m_size == 0U)
		return false;
	if (idx == 0U)
		return pop(value, false);
	value = ang::move(m_data[0]);
	for (windex i = idx; i < m_size; ++i)
		m_data[i - 1] = ang::move(m_data[i]);
	m_alloc.destroy((T*)&(m_data[--m_size]));
	return true;
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::pop_at(ang::collections::base_iterator<T> it, T& value)
{
	if (it.parent() != this || it.current() != m_data)
		return false;
	return pop_at(it.offset(), value);
}

template<typename T, template<typename> class A>
inline void ang::collections::vector_buffer<T, A>::clear()
{
	if (m_data != null)
	{
		for (wsize i = 0; i < m_size; ++i)
			m_alloc.destroy((T*)&m_data[i]);
		m_size = 0U;
		m_capacity = 0U;
		m_alloc.deallocate(m_data);
	}
	m_data = null;
}

template<typename T, template<typename> class A>
inline void ang::collections::vector_buffer<T, A>::empty()
{
	if (m_data != null)
	{
		for (wsize i = 0; i < m_size; ++i)
			m_alloc.destroy((T*)&m_data[i]);
		m_size = 0U;
	}
}

template<typename T, template<typename> class A>
inline bool ang::collections::vector_buffer<T, A>::realloc(wsize new_size, bool save)
{
	new_size++;
	if (capacity() >= new_size)
		return true;

	wsize sz = 8U, temp = 0U;
	while (sz <= new_size)
		sz *= 2U;
	T* new_buffer = m_alloc.allocate(sz);

	if (save)
	{
		temp = m_size;
		for (windex i = 0U; i < m_size; ++i)
		{
			m_alloc.template construct<T,T&&>((T*)&new_buffer[i], ang::move(m_data[i]));
			m_alloc.destroy((T*)&m_data[i]);
		}
		m_alloc.deallocate(m_data);
	}
	else
	{
		clear();
	}
	m_data = new_buffer;
	m_size = temp;
	m_capacity = sz;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>::object_wrapper()
	: m_ptr(null)
{
	set(new ang::collections::vector_buffer<T, A>());
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>::object_wrapper(ang::collections::vector_buffer<T, A>* ptr)
	: m_ptr(null)
{
	set(ptr ? ptr : new ang::collections::vector_buffer<T, A>());
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>::object_wrapper(std::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T, template <typename> class A> template<typename U>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>::object_wrapper(std::initializer_list<U> list)
	: m_ptr(null)
{
	set(new collections::vector_buffer<T, A>(ang::move(list)));
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: m_ptr(null)
{
	set(new collections::vector_buffer<T, A>(store));
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<T, A>> && ptr)
	: m_ptr(null)
{
	if (ptr.is_empty())
	{
		set(new ang::collections::vector_buffer<T, A>());
	}
	else
	{
		auto temp = ptr.m_ptr;
		ptr.m_ptr = null;
		m_ptr = temp;
	}
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<T, A>> const& ptr)
	: m_ptr(null)
{
	set(!ptr.is_empty() ? ptr.get() : new ang::collections::vector_buffer<T, A>());
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>::~object_wrapper()
{
	reset();
}

template<typename T, template <typename> class A>
inline void ang::object_wrapper<ang::collections::vector_buffer<T, A>>::reset()
{
	if (m_ptr) 
		m_ptr->release();
	m_ptr = null;
}

template<typename T, template <typename> class A>
inline void ang::object_wrapper<ang::collections::vector_buffer<T, A>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T, template <typename> class A>
inline bool ang::object_wrapper<ang::collections::vector_buffer<T, A>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T, template <typename> class A>
inline ang::collections::vector_buffer<T, A>* ang::object_wrapper<ang::collections::vector_buffer<T, A>>::get(void)const
{
	return m_ptr;
}

template<typename T, template <typename> class A>
inline void ang::object_wrapper<ang::collections::vector_buffer<T, A>>::set(ang::collections::vector_buffer<T, A>* ptr)
{
	ang::collections::vector_buffer<T, A> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, template <typename> class A>
inline ang::collections::vector_buffer<T, A>** ang::object_wrapper<ang::collections::vector_buffer<T, A>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T, template <typename> class A>
inline ang::collections::vector_buffer<T, A>** ang::object_wrapper<ang::collections::vector_buffer<T, A>>::addres_for_init(void)
{
	reset();
	return &m_ptr;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>& ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator = (ang::collections::vector_buffer<T, A>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>& ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator = (const std::nullptr_t&)
{
	reset();
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>& ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (m_ptr == null)
		set(new collections::vector_buffer<T, A>(items));
	else
		m_ptr->copy(items);
	return *this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>& ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<T, A>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>& ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<T, A>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>& ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator += (T item)
{
	if (is_empty())
		set(new collections::vector_buffer<T, A>());
	m_ptr->push(item);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>& ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator += (collections::ienum<data_type> const* items)
{
	if (is_empty())
		set(new collections::vector_buffer<T, A>(items));
	else m_ptr->extend(items);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>& ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator += (object_wrapper const& items)
{
	if (is_empty())
		set(new collections::vector_buffer<T, A>(items.get()));
	else m_ptr->extend((array_view<T>)items);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper_ptr<ang::collections::vector_buffer<T, A>> ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator & (void)
{
	return this;
}

template<typename T, template <typename> class A>
inline ang::collections::vector_buffer<T, A> * ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator -> (void)
{
	return get();
}

template<typename T, template <typename> class A>
inline ang::collections::vector_buffer<T, A> const* ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator -> (void)const
{
	return get();
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator ang::collections::vector_buffer<T, A> * (void)
{
	return get();
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator ang::collections::vector_buffer<T, A> const* (void)const
{
	return get();
}

template<typename T, template <typename> class A> template<typename I>
inline T const& ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator[](I const& idx)const
{
	static_assert(is_integer_value<I>::value, "no integer value is no accepted"); 
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return m_ptr->data()[idx];
}

template<typename T, template <typename> class A> template<typename I>
inline T & ang::object_wrapper<ang::collections::vector_buffer<T, A>>::operator[](I const& idx)
{
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return m_ptr->data()[idx];
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::object_wrapper()
	: m_ptr(null)
{
	set(new ang::collections::vector_buffer<ang::var, A>());
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::object_wrapper(ang::collections::vector_buffer<ang::var, A>* ptr)
	: m_ptr(null)
{
	set(ptr ? ptr : new ang::collections::vector_buffer<ang::var, A>());
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::object_wrapper(std::nullptr_t const&)
	: m_ptr(null)
{
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::object_wrapper(std::initializer_list<ang::var> list)
	: m_ptr(null)
{
	set(new collections::vector_buffer<ang::var, A>(ang::move(list)));
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: m_ptr(null)
{
	set(new collections::vector_buffer<ang::var, A>(store));
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>> && ptr)
	: m_ptr(null)
{
	if (ptr.is_empty())
	{
		set(new ang::collections::vector_buffer<ang::var, A>());
	}
	else
	{
		auto temp = ptr.m_ptr;
		ptr.m_ptr = null;
		m_ptr = temp;
	}
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>> const& ptr)
	: m_ptr(null)
{
	set(!ptr.is_empty() ? ptr.get() : new ang::collections::vector_buffer<ang::var, A>());
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::~object_wrapper()
{
	reset();
}

template<template <typename> class A>
inline void ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::reset()
{
	if (m_ptr)
		m_ptr->release();
	m_ptr = null;
}

template<template <typename> class A>
inline void ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::reset_unsafe()
{
	m_ptr = null;
}

template<template <typename> class A>
inline bool ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::is_empty()const
{
	return m_ptr == null;
}

template<template <typename> class A>
inline ang::collections::vector_buffer<ang::var, A>* ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::get(void)const
{
	return m_ptr;
}

template<template <typename> class A>
inline void ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::set(ang::collections::vector_buffer<ang::var, A>* ptr)
{
	ang::collections::vector_buffer<ang::var, A> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<template <typename> class A>
inline ang::collections::vector_buffer<ang::var, A>** ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::addres_of(void)
{
	return &m_ptr;
}

template<template <typename> class A>
inline ang::collections::vector_buffer<ang::var, A>** ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::addres_for_init(void)
{
	reset();
	return &m_ptr;
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>& ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator = (ang::collections::vector_buffer<ang::var, A>* ptr)
{
	set(ptr);
	return*this;
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>& ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator = (const std::nullptr_t&)
{
	clear();
	return*this;
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>& ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (m_ptr == null)
		set(new collections::vector_buffer<ang::var, A>(items));
	else
		m_ptr->copy(items);
	return *this;
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>& ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>& ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>& ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator += (ang::var item)
{
	if (is_empty())
		set(new collections::vector_buffer<ang::var, A>());
	m_ptr->push(item);
	return*this;
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>& ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator += (collections::ienum<data_type> const* items)
{
	if (is_empty())
		set(new collections::vector_buffer<ang::var, A>(items));
	else m_ptr->extend(items);
	return*this;
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>& ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator += (object_wrapper const& items)
{
	if (is_empty())
		set(new collections::vector_buffer<ang::var, A>(items.get()));
	else m_ptr->extend((array_view<ang::var>)items);
	return*this;
}

template<template <typename> class A>
inline ang::object_wrapper_ptr<ang::collections::vector_buffer<ang::var, A>> ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator & (void)
{
	return this;
}

template<template <typename> class A>
inline ang::collections::vector_buffer<ang::var, A> * ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator -> (void)
{
	return get();
}

template<template <typename> class A>
inline ang::collections::vector_buffer<ang::var, A> const* ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator -> (void)const
{
	return get();
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator ang::collections::vector_buffer<ang::var, A> * (void)
{
	return get();
}

template<template <typename> class A>
inline ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator ang::collections::vector_buffer<ang::var, A> const* (void)const
{
	return get();
}

template<template <typename> class A> template<typename I>
inline ang::var const& ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator[](I const& idx)const
{
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return m_ptr->data()[idx];
}

template<template <typename> class A> template<typename I>
inline ang::var & ang::object_wrapper<ang::collections::vector_buffer<ang::var, A>>::operator[](I const& idx)
{
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return m_ptr->data()[idx];
}


#endif//__ANG_VECTOR_INL__
