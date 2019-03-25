/*********************************************************************************************************************/
/*   File Name: ang/collections/inline/array.hpp                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error Can't include array.inl, please include angsys.h inside
#elif !defined __ANG_COLLECTIONS_ARRAY_INL__
#define __ANG_COLLECTIONS_ARRAY_INL__


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// array_buffer<T, allocator> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::array_buffer()
	: base()
	, m_size(0)
	, m_data(null)
{
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::array_buffer(wsize reserve)
	: array_buffer()
{
	realloc(reserve);
}

template<typename T, template <typename> class allocator> template<typename U>
inline ang::collections::array_buffer<T, allocator>::array_buffer(std::initializer_list<U> list)
	: array_buffer()
{
	wsize c = 0;
	realloc(list.size());
	for (auto it = list.begin(); it != list.end(); ++it)
		m_data[c++] = *it;	
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
	m_size = ang::move(ar.m_size);
	m_data = ang::move(ar.m_data);
	ar.m_size = 0;
	ar.m_data = null;
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::array_buffer(const ang::collections::array_buffer<T, allocator>& ar)
	: array_buffer()
{
	copy(&static_cast<const ienum_type&>(ar));
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>::array_buffer(const ang::collections::array_buffer<T, allocator>* ar)
	: array_buffer()
{
	if (ar) copy(static_cast<const ienum_type*>(ar));
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
}

template<typename T, template <typename> class allocator>
inline ang::rtti_t const& ang::collections::array_buffer<T, allocator>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::collections::array<"_s) += ang::type_of<T>().type_name()) += ">"_s);
	static ang::rtti_t const* parents[] = { &runtime::type_of<iarray<T>>() };
	static ang::rtti_t const& info = rtti::regist(name, genre::class_type, size_of<ang::collections::array_buffer<T, allocator>>(), alignof(ang::collections::array_buffer<T, allocator>), parents, &intf::default_query_interface);
	return info;
}

template<typename T, template <typename> class allocator>
inline ang::rtti_t const& ang::collections::array_buffer<T, allocator>::runtime_info()const
{
	return class_info();
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == class_info().type_id())
	{
		if (out == null) return false;
		*out = static_cast<array_buffer<T, allocator>*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == ang::type_of<iarray<T>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<iarray<T>*>(this);
		return true;
	}
	else if (id.type_id() == ang::type_of<ienum<T>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ienum<T>*>(this);
		return true;
	}
	return false;
}

template<typename T, template <typename> class allocator>
inline void ang::collections::array_buffer<T, allocator>::dispose()
{
	clear();
}

template<typename T, template <typename> class allocator>
inline void ang::collections::array_buffer<T, allocator>::clear()
{
	if (m_data) {
		for (windex i = 0; i < m_size; ++i)
			m_alloc.template destroy<T>((T*)&m_data[i]);
		m_alloc.deallocate(m_data);
	}
	m_size = 0;
	m_data = null;
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::is_empty()const
{
	return null == m_data;
}

template<typename T, template <typename> class allocator>
inline ang::rtti_t const& ang::collections::array_buffer<T, allocator>::value_type()const
{
	return ang::type_of<array_view<T>>();
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::set_value(ang::rtti_t const& id, ang::unknown_t value)
{
	if (id.is_type_of<array_view<T>>())
	{
		array_view<T>& ar = *reinterpret_cast<array_view<T>*>(value);
		copy(ar);
		return true;
	}
	return false;
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::get_value(ang::rtti_t const& id, ang::unknown_t value)const
{
	if (id.is_type_of<array_view<T>>())
	{
		array_view<T>& ar = *reinterpret_cast<array_view<T>*>(value);
		ar.set(m_data, m_size);
	}
	return false;
}

template<typename T, template <typename> class allocator>
inline ang::variant ang::collections::array_buffer<T, allocator>::clone()const 
{
	return (ivariant*) new ang::collections::array_buffer<T, allocator>(*this);
}

template<typename T, template <typename> class allocator>
inline ang::string ang::collections::array_buffer<T, allocator>::to_string()const
{
	return class_info().type_name();
}

template<typename T, template <typename> class allocator>
inline ang::string ang::collections::array_buffer<T, allocator>::to_string(ang::text::text_format_t)const
{
	return class_info().type_name();
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::is_readonly()const
{
	return false;
}

template<typename T, template <typename> class allocator>
inline T* ang::collections::array_buffer<T, allocator>::data()const
{
	return m_data;
}

template<typename T, template <typename> class allocator>
inline wsize ang::collections::array_buffer<T, allocator>::size()const
{
	return m_size;
}

template<typename T, template <typename> class allocator>
inline void ang::collections::array_buffer<T, allocator>::size(wsize size)
{
	if (size == m_size)
		return;
	realloc(size);
}

template<typename T, template <typename> class allocator> template<typename U>
inline void ang::collections::array_buffer<T, allocator>::copy(ang::array_view<U> const& ar)
{
	clear();
	if (ar.size() > 0)
		m_data = m_alloc.allocate(ar.size());
	m_size = ar.size();

	for (windex i = 0; i < m_size; ++i)
		m_alloc.template construct<T, U const&>((T*)&m_data[i], ar[i]);
	//ar.set(null, 0);
}

template<typename T, template <typename> class allocator> template<typename U, template<typename> class allocator2>
inline void ang::collections::array_buffer<T, allocator>::copy(ang::scope_array<U, allocator2> const& ar)
{
	clear();
	if (ar.size() > 0)
		m_data = m_alloc.allocate(ar.size());
	m_size = ar.size();

	for (windex i = 0; i < m_size; ++i)
		m_alloc.template construct<T, U const&>(&m_data[i], ar[i]);
	ar.set(null, 0);
}

template<typename T, template<typename> class allocator>  template<typename U>
inline void ang::collections::array_buffer<T, allocator>::copy(ang::collections::ienum_ptr<U> const& items)
{
	clear();
	if (items->counter() > 0)
		m_data = m_alloc.allocate(items->counter());
	m_size = items->counter();

	windex i = 0;
	for(U const& item : items)
		m_alloc.template construct<T, U const&>(&m_data[i++], item);
}

template<typename T, template<typename> class allocator>
inline ang::text::encoding_t ang::collections::array_buffer<T, allocator>::encoding()const
{
	return text::encoding::binary;
}

template<typename T, template <typename> class allocator>
inline pointer ang::collections::array_buffer<T, allocator>::buffer_ptr()
{
	return (pointer)m_data;
}

template<typename T, template <typename> class allocator>
inline const_pointer ang::collections::array_buffer<T, allocator>::buffer_ptr()const
{
	return (pointer)m_data;
}

template<typename T, template <typename> class allocator>
inline wsize ang::collections::array_buffer<T, allocator>::buffer_size()const
{
	return m_size * (wsize)size_of<T>();
}

template<typename T, template <typename> class allocator>
inline wsize ang::collections::array_buffer<T, allocator>::mem_copy(wsize _s, pointer _p, text::encoding_t)
{
	throw_exception(except_code::unsupported);
	return 0;
}

template<typename T, template <typename> class allocator>
inline ang::ibuffer_view_t ang::collections::array_buffer<T, allocator>::map_buffer(windex start, wsize size)
{
	throw_exception(except_code::unsupported);
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
	clear();
	m_data.move(ar.m_data);
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
		m_data[i++] = value;
}

template<typename T, template <typename> class allocator>
inline wsize ang::collections::array_buffer<T, allocator>::counter()const
{
	return m_size;
}

template<typename T, template <typename> class allocator>
inline T& ang::collections::array_buffer<T, allocator>::at(ang::collections::base_iterator<T>const& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw_exception(except_code::invalid_memory);
	if (&m_data != it.current())
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_size)
		throw_exception(except_code::array_overflow);
#endif
	return m_data[it.offset()];
}

template<typename T, template <typename> class allocator>
inline ang::collections::iterator<T> ang::collections::array_buffer<T, allocator>::at(windex idx)
{
	if (idx >= m_size)
		return  iterator_t(const_cast<self*>(this), null, 0);
	return iterator_t(const_cast<self*>(this), (pointer)&m_data, idx);
}

template<typename T, template <typename> class allocator>
inline ang::collections::iterator<const T> ang::collections::array_buffer<T, allocator>::at(windex idx)const
{
	if (idx >= m_size)
		return  const_iterator_t(const_cast<self*>(this), null, 0);
	return const_iterator_t(const_cast<self*>(this), (pointer)&m_data, idx);
}

template<typename T, template <typename> class allocator>
inline ang::collections::iterator<T> ang::collections::array_buffer<T, allocator>::find(ang::core::delegates::function<bool(T const&)> cond, bool invert)const
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

template<typename T, template <typename> class allocator>
inline ang::collections::iterator<T> ang::collections::array_buffer<T, allocator>::find(ang::core::delegates::function<bool(T const&)> cond, ang::collections::base_iterator<T> next_to, bool invert)const
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

template<typename T, template <typename> class allocator>
inline ang::collections::ienum_ptr<T> ang::collections::array_buffer<T, allocator>::find_all(ang::core::delegates::function<bool(T const&)> cond)const
{
	vector<T, allocator> out = new vector_buffer<T, allocator>();
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

template<typename T, template <typename> class allocator>
inline ang::collections::forward_iterator<T> ang::collections::array_buffer<T, allocator>::begin()
{
	return forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_data));
}

template<typename T, template <typename> class allocator>
inline ang::collections::forward_iterator<T> ang::collections::array_buffer<T, allocator>::end()
{
	return forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_data), m_size);
}

template<typename T, template <typename> class allocator>
inline ang::collections::forward_iterator<const T> ang::collections::array_buffer<T, allocator>::begin()const
{
	return const_forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_data));
}

template<typename T, template <typename> class allocator>
inline ang::collections::forward_iterator<const T> ang::collections::array_buffer<T, allocator>::end()const
{
	return const_forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_data), m_size);
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<T> ang::collections::array_buffer<T, allocator>::last()
{
	return m_size ? forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_data), m_size - 1) : end();
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<const T> ang::collections::array_buffer<T, allocator>::last()const
{
	return m_size ? const_forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_data), m_size - 1) : end();
}

template<typename T, template <typename> class allocator>
inline ang::collections::backward_iterator<T> ang::collections::array_buffer<T, allocator>::rbegin()
{
	return backward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_data), m_size - 1);
}

template<typename T, template <typename> class allocator>
inline ang::collections::backward_iterator<T> ang::collections::array_buffer<T, allocator>::rend()
{
	return backward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_data), (wsize)invalid_index);
}

template<typename T, template <typename> class allocator>
inline ang::collections::backward_iterator<const T> ang::collections::array_buffer<T, allocator>::rbegin()const
{
	return const_backward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_data), m_size - 1);
}

template<typename T, template <typename> class allocator>
inline ang::collections::backward_iterator<const T> ang::collections::array_buffer<T, allocator>::rend()const
{
	return const_backward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_data), (wsize)invalid_index);
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::increase(ang::collections::base_iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != &m_data)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_size)
		throw_exception(except_code::array_overflow);
#endif
	it.offset(it.offset() + 1);
	if (it.offset() > m_size) it.offset(m_size);
	return true;
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::increase(ang::collections::base_iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != &m_data)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_size)
		throw_exception(except_code::array_overflow);
#endif
	it.offset(it.offset() + val);
	if (it.offset() >= m_size)
		it.offset(m_size);
	return true;
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::decrease(ang::collections::base_iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != &m_data)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_size)
		throw_exception(except_code::array_overflow);
#endif
	it.offset(it.offset() - 1);
	if ((int)it.offset() < -1)
		it.offset((wsize)invalid_index);
	return true;
}

template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::decrease(ang::collections::base_iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != &m_data)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_size)
		throw_exception(except_code::array_overflow);
#endif
	it.offset(it.offset() - val);
	if ((int)it.offset() < -1)
		it.offset((wsize)invalid_index);
	return true;
}

template<typename T, template <typename> class allocator>
inline ang::comparision_result_t ang::collections::array_buffer<T, allocator>::compare(const ang::object* obj)const
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
			if (logic_operation<logic_operation_type::same, T, T>::operate(m_data[i++],*it))
				return comparision_result::diferent;
		return comparision_result::same;
	}
	return comparision_result::diferent;
}


template<typename T, template <typename> class allocator>
inline bool ang::collections::array_buffer<T, allocator>::realloc(wsize size)
{
	clear();
	if (size > 0)
	{
		m_size = size;
		m_data = m_alloc.allocate(m_size);
		for (windex i = 0; i < m_size; ++i)
			m_alloc.template construct<T, T const&>((T*)&m_data[i], T());
	}
	return m_data != null;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper()
	: m_ptr(null)
{
	set(new ang::collections::array_buffer<T, allocator>());
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(wsize reserve)
	: m_ptr(null)
{
	set(new ang::collections::array_buffer<T, allocator>(reserve));
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(ang::collections::array_buffer<T, allocator>* ptr)
	: m_ptr(null)
{
	set(ptr ? ptr : new ang::collections::array_buffer<T, allocator>());
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(std::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T, template <typename> class allocator> template<typename U>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(std::initializer_list<U> list)
	: m_ptr(null)
{
	set(new collections::array_buffer<T, allocator>(ang::move(list)));
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: m_ptr(null)
{
	set(new collections::array_buffer<T, allocator>(store));
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<T, allocator>> && other)
	: m_ptr(null)
{
	collections::array_buffer<T, allocator> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<T, allocator>> const& other)
	: m_ptr(null)
{
	set(other.get());
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::~object_wrapper()
{
	reset();
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T, template <typename> class allocator>
inline bool ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>* ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::get(void)const
{
	return m_ptr;
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::set(ang::collections::array_buffer<T, allocator>* ptr)
{
	ang::collections::array_buffer<T, allocator> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, template <typename> class allocator>
inline ang::collections::array_buffer<T, allocator>** ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::addres_of(void)
{
	return &m_ptr;
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
	reset();
	return*this;
}

//template<typename T, template <typename> class allocator>
//inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>& ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator = (ang::collections::ienum<data_type> const* items)
//{
//	if (m_ptr == null)
//		set(new collections::array_buffer<T, allocator>(items));
//	else
//		m_ptr->copy(items);
//	return *this;
//}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>& ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator = (ang::object_wrapper<ang::collections::array_buffer<T, allocator>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::array_buffer<T, allocator>>& ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator = (ang::object_wrapper<ang::collections::array_buffer<T, allocator>> const& other)
{
	set(other.m_ptr);
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
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
		if (is_empty()) throw_exception(except_code::invalid_memory);
		if ((idx >= m_ptr->size()) || (idx < 0)) throw_exception(except_code::array_overflow);
#endif
	return m_ptr->data()[idx];
}

template<typename T, template <typename> class allocator> template<typename I>
inline T & ang::object_wrapper<ang::collections::array_buffer<T, allocator>>::operator[](I const& idx)
{
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
		if (is_empty()) throw_exception(except_code::invalid_memory);
		if ((idx >= m_ptr->size()) || (idx < 0)) throw_exception(except_code::array_overflow);
#endif
	return m_ptr->data()[idx];
}

///////////////////////////////////////////////////////////////


#endif//__ANG_COLLECTIONS_ARRAY_INL__