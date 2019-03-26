#ifndef __ANG_COLLECTIONS_VECTOR_H__
#error Can't include vector.inl, please include vector.hpp inside
#else

DECLARE_ALLOC(MY_TYPE, MY_ALLOC);

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::vector_buffer()
	: m_size(0)
	, m_capacity(0)
	, m_data(null)
{
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::vector_buffer(const ang::nullptr_t&)
	: vector_buffer()
{

}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::vector_buffer(ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>&& ar)
	: vector_buffer()
{
	m_size = ar.m_size;
	m_data = ar.m_data;
	m_capacity = ar.m_capacity;
	ar.m_size = 0;
	ar.m_capacity = 0;
	ar.m_data = null;
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::vector_buffer(const ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>& ar)
	: vector_buffer()
{
	copy(to_array(ar.data(), ar.size()));
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::vector_buffer(const ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>* ar)
	: vector_buffer()
{
	if (ar)copy(to_array(ar->data(), ar->size()));
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::vector_buffer(const ang::collections::ienum<MY_TYPE>* store)
	: vector_buffer()
{
	copy(store);
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::~vector_buffer()
{
}

ang::rtti_t const& ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::class_info()
{
	static const char name[] = ANG_UTILS_TO_STRING(ANG_EXPAND_ARGS(ang::collections::vector<MY_TYPE, MY_ALLOC>));
	static rtti_t const* parents[] = { &runtime::type_of<ilist<MY_TYPE>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>(), align_of<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>(), parents, &default_query_interface);
	return info;
}

ang::rtti_t const& ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::runtime_info()const
{
	return class_info();
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == class_info().type_id())
	{
		if (out == null) return false;
		*out = static_cast<vector_buffer<MY_TYPE, MY_ALLOC>*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == ang::type_of<ilist<MY_TYPE>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ilist<MY_TYPE>*>(this);
		return true;
	}
	else if (id.type_id() == ang::type_of<iarray<MY_TYPE>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<iarray<MY_TYPE>*>(this);
		return true;
	}
	else if (id.type_id() == ang::type_of<ienum<MY_TYPE>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ienum<MY_TYPE>*>(this);
		return true;
	}
	return false;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::is_empty()const
{
	return size() == 0;
}

MY_TYPE* ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::data()const
{
	return m_data;
}

wsize ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::size()const
{
	return m_size;
}

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::size(wsize size, bool save)
{
	if (m_size != size)
	{
		realloc(size, save);
		if (m_size <= size)
			memset(&m_data[m_size], 0, size - m_size);
		else
			memset(&m_data[size], 0, m_size - size);
		m_size = size;
	}
}

wsize ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::capacity()const
{
	return m_capacity;
}

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::capacity(wsize size, bool save)
{
	realloc(size, save);
}

ang::rtti_t const& ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::value_type()const
{
	return ang::type_of<array_view<MY_TYPE>>();
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::set_value(ang::rtti_t const& id, ang::unknown_t value)
{
	if (id.is_type_of<array_view<MY_TYPE>>())
	{
		array_view<MY_TYPE>& ar = *reinterpret_cast<array_view<MY_TYPE>*>(value);
		copy(ar);
		return true;
	}
	return false;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::get_value(ang::rtti_t const& id, ang::unknown_t value)const
{
	if (id.is_type_of<array_view<MY_TYPE>>())
	{
		array_view<MY_TYPE>& ar = *reinterpret_cast<array_view<MY_TYPE>*>(value);
		ar.set(m_data, m_size);
	}
	return false;
}

ang::variant ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::clone()const
{
	return (ivariant*) new ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>(*this);
}

ang::string ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::to_string()const
{
	return class_info().type_name();
}

ang::string ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::to_string(ang::text::text_format_t)const
{
	return class_info().type_name();
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::is_readonly()const
{
	return false;
}

ang::text::encoding_t ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::encoding()const
{
	return text::encoding::binary;
}

const_pointer ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::buffer_ptr()const
{
	return (pointer)m_data;
}

pointer ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::buffer_ptr()
{
	return (pointer)m_data;
}

wsize ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::buffer_size()const
{
	return m_capacity * sizeof(MY_TYPE);
}

wsize ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::mem_copy(wsize _s, pointer _p, text::encoding_t)
{
	throw(exception_t(except_code::unsupported));
	return 0;
}

ang::ibuffer_view_t ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::map_buffer(windex start, wsize size)
{
	return null;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::unmap_buffer(ang::ibuffer_view_t&, wsize used)
{
	return false;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::can_realloc_buffer()const { return true; };

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::realloc_buffer(wsize size) { return realloc(size, true); };

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::move(ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>& ar)
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

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::copy(const ang::collections::ienum<MY_TYPE>* _items)
{
	if (!_items)
		return;
	capacity(_items->counter(), false);
	for (MY_TYPE const& value : *_items)
		push(value);
}

wsize ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::counter()const
{
	return m_size;
}

MY_TYPE& ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::at(ang::collections::base_iterator<MY_TYPE> const& it)
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

ang::collections::iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::at(windex idx)
{
	if (idx >= m_size)
		return  iterator_t(const_cast<self*>(this), null, 0);
	return iterator_t(const_cast<self*>(this), (pointer)m_data, idx);
}

ang::collections::iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::at(windex idx)const
{
	if (idx >= m_size)
		return  const_iterator_t(const_cast<self*>(this), null, 0);
	return const_iterator_t(const_cast<self*>(this), (pointer)m_data, idx);
}

ang::collections::iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::find(ang::core::delegates::function<bool(MY_TYPE const&)> cond, bool invert)const
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

ang::collections::iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::find(ang::core::delegates::function<bool(MY_TYPE const&)> cond, ang::collections::base_iterator<MY_TYPE> next_to, bool invert)const
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

ang::collections::ienum_ptr<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::find_all(ang::core::delegates::function<bool(MY_TYPE const&)> cond)const
{
	vector<MY_TYPE, MY_ALLOC> out = new vector_buffer<MY_TYPE, MY_ALLOC>();
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

ang::collections::forward_iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::begin()
{
	return forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data));
}

ang::collections::forward_iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::end()
{
	return forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size);
}

ang::collections::forward_iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::begin()const
{
	return const_forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data));
}

ang::collections::forward_iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::end()const
{
	return const_forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size);
}

ang::collections::forward_iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::last()
{
	return m_size ? forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size - 1) : end();
}

ang::collections::forward_iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::last()const
{
	return m_size ? const_forward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size - 1) : end();
}

ang::collections::backward_iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::rbegin()
{
	return backward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size - 1);
}

ang::collections::backward_iterator<MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::rend()
{
	return backward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), (wsize)invalid_index);
}

ang::collections::backward_iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::rbegin()const
{
	return const_backward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), m_size - 1);
}

ang::collections::backward_iterator<const MY_TYPE> ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::rend()const
{
	return const_backward_iterator_t(const_cast<vector_buffer*>(this), pointer(m_data), (wsize)invalid_index);
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::increase(ang::collections::base_iterator<MY_TYPE>& it)const
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

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::increase(ang::collections::base_iterator<MY_TYPE>& it, int val)const
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

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::decrease(ang::collections::base_iterator<MY_TYPE>& it)const
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

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::decrease(ang::collections::base_iterator<MY_TYPE>& it, int val)const
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

ang::comparision_result_t ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::compare(const ang::object* obj)const
{
	windex i = 0;
	intf_wrapper<ienum<MY_TYPE>> other = const_cast<object*>(obj)->as<ienum<MY_TYPE>>();

	if (!other.is_empty())
	{
		if (counter() > other->counter())
			return comparision_result::mayor;
		else if (counter() < other->counter())
			return comparision_result::minor;
		else for (auto it = other->begin(); it != other->end(); ++it)
			if (logic_operation<logic_operation_type::same, MY_TYPE, MY_TYPE>::operate(m_data[i++], *it))
				return comparision_result::diferent;
		return comparision_result::same;
	}
	return comparision_result::diferent;
}

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::extend(const ang::collections::ienum<MY_TYPE>* items)
{
	if (!items) return;
	capacity(counter() + items->counter());
	for (MY_TYPE const& value : *items)
		push(value);
}

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::push(MY_TYPE const& value, bool last)
{
	if ((capacity() - counter()) <= 1U) //TODO: optimization
		capacity(capacity() + 1, true);

	if (last)
	{
		m_data[m_size++] = value;
	}
	else
	{
		for (auto i = m_size; i > 0U; --i)
			m_data[i] = m_data[i - 1U];	
		m_data[0] = value;
		m_size++;
	}
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::insert(windex idx, MY_TYPE const& value)
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
		m_data[i] = m_data[i - 1U];
	m_data[idx] = value;
	m_size++;
	return true;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::insert(ang::collections::base_iterator<MY_TYPE> it, MY_TYPE const& value)
{
	if (it.parent() != this || it.current() != m_data)
		return false;
	return insert(it.offset(), value);
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::pop(bool last)
{
	if (m_size == 0)
		return false;
	if (!last) for (windex i = 1U; i < m_size; ++i)
		m_data[i - 1] = m_data[i];
	m_data[--m_size] = default_value<MY_TYPE>::value;
	return true;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::pop(MY_TYPE& value, bool last)
{
	if (m_size == 0)
		return false;
	if (!last)
	{
		value = ang::move(m_data[0]);
		for (windex i = 1U; i < m_size; ++i)
			m_data[i - 1] = m_data[i];
	}
	else
	{
		value = ang::move(m_data[m_size - 1U]);
	}
	m_data[--m_size] = default_value<MY_TYPE>::value;
	return true;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::pop_at(windex idx)
{
	if (m_size == 0U)
		return false;
	if (idx == 0U)
		return pop(false);
	for (windex i = idx; i < m_size; ++i)
		m_data[i - 1] = ang::move(m_data[i]);
	m_data[--m_size] = default_value<MY_TYPE>::value;
	return true;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::pop_at(ang::collections::base_iterator<MY_TYPE> it)
{
	if (it.parent() != this || it.current() != m_data)
		return false;
	return pop_at(it.offset());
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::pop_at(windex idx, MY_TYPE& value)
{
	if (m_size == 0U)
		return false;
	if (idx == 0U)
		return pop(value, false);
	value = ang::move(m_data[0]);
	for (windex i = idx; i < m_size; ++i)
		m_data[i - 1] = ang::move(m_data[i]);
	m_data[--m_size] = default_value<MY_TYPE>::value;
	return true;
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::pop_at(ang::collections::base_iterator<MY_TYPE> it, MY_TYPE& value)
{
	if (it.parent() != this || it.current() != m_data)
		return false;
	return pop_at(it.offset(), value);
}

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::dispose()
{
	clear();
}

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::clear()
{
	if (m_data != null)
	{
		m_size = 0U;
		m_capacity = 0U;
		m_alloc.deallocate(m_data);
	}
	m_data = null;
}

void ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::empty()
{
	if (m_data != null)
	{
		while(m_size)
			m_data[--m_size] = default_value<MY_TYPE>::value;
	}
}

bool ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>::realloc(wsize new_size, bool save)
{
	new_size++;
	if (capacity() >= new_size)
		return true;

	wsize sz = 16U, temp = 0U;
	while (sz <= new_size)
		sz *= 2U;
	MY_TYPE* new_buffer = m_alloc.allocate(sz);

	if (save)
	{
		temp = m_size;
		memcpy(new_buffer, m_data, min(m_size, sz));
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

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::object_wrapper()
	: m_ptr(null)
{
	set(new ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>());
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::object_wrapper(ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>* ptr)
	: m_ptr(null)
{
	set(ptr ? ptr : new ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>());
}


ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::object_wrapper(std::nullptr_t const&)
	: m_ptr(null)
{
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: m_ptr(null)
{
	set(new collections::vector_buffer<MY_TYPE, MY_ALLOC>(store));
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>> && ptr)
	: m_ptr(null)
{
	if (ptr.is_empty())
	{
		set(new ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>());
	}
	else
	{
		auto temp = ptr.m_ptr;
		ptr.m_ptr = null;
		m_ptr = temp;
	}
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>> const& ptr)
	: m_ptr(null)
{
	set(!ptr.is_empty() ? ptr.get() : new ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>());
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::~object_wrapper()
{
	reset();
}

void ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::reset()
{
	if (m_ptr)
		m_ptr->release();
	m_ptr = null;
}

void ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::reset_unsafe()
{
	m_ptr = null;
}

bool ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::is_empty()const
{
	return m_ptr == null;
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>* ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::get(void)const
{
	return m_ptr;
}

void ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::set(ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>* ptr)
{
	ang::collections::vector_buffer<MY_TYPE, MY_ALLOC> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>** ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::addres_of(void)
{
	return &m_ptr;
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>** ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::addres_for_init(void)
{
	reset();
	return &m_ptr;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator = (ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator = (const std::nullptr_t&)
{
	reset();
	return*this;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (m_ptr == null)
		set(new collections::vector_buffer<MY_TYPE, MY_ALLOC>(items));
	else
		m_ptr->copy(items);
	return *this;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>> const& other)
{
	set(other.m_ptr);
	return*this;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator += (MY_TYPE item)
{
	if (is_empty())
		set(new collections::vector_buffer<MY_TYPE, MY_ALLOC>());
	m_ptr->push(item);
	return*this;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator += (collections::ienum<data_type> const* items)
{
	if (is_empty())
		set(new collections::vector_buffer<MY_TYPE, MY_ALLOC>(items));
	else m_ptr->extend(items);
	return*this;
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator += (object_wrapper const& items)
{
	if (is_empty())
		set(new collections::vector_buffer<MY_TYPE, MY_ALLOC>(items.get()));
	else m_ptr->extend((array_view<MY_TYPE>)items);
	return*this;
}

ang::object_wrapper_ptr<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>> ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator & (void)
{
	return this;
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC> * ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator -> (void)
{
	return get();
}

ang::collections::vector_buffer<MY_TYPE, MY_ALLOC> const* ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator ang::collections::vector_buffer<MY_TYPE, MY_ALLOC> * (void)
{
	return get();
}

ang::object_wrapper<ang::collections::vector_buffer<MY_TYPE, MY_ALLOC>>::operator ang::collections::vector_buffer<MY_TYPE, MY_ALLOC> const* (void)const
{
	return get();
}


#endif//__ANG_VECTOR_INL__
