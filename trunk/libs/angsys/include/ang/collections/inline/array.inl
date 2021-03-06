/*********************************************************************************************************************/
/*   File Name: ang/collections/inline/array.inl                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#elif !defined __ANG_ARRAY_INL__
#define __ANG_ARRAY_INL__

namespace ang
{
	template<typename T, template<typename>class A>
	struct intf_class_info<ang::collections::array_buffer<T, A>>
	{
		inline static ang::rtti_t const& class_info()
		{
			using _iid_type = typename ang::collections::array_buffer<T, A>::iid_type;
			static inherit_pack_info_t parents
				= ang::rtti_from_type<ang::collections::array_buffer<T, A>>::types();

			static ang::rtti_t const& info = rtti::regist(
				[]()->fast_astring_t {
					fast_astring_t out;
					out << _iid_type::type_name() << "<"_sv << ang::type_of<T>().type_name() << ">"_sv;
					return ang::move(out); }()
				, gender::class_type
				, size_of<ang::collections::array_buffer<T, A>>()
				, align_of<ang::collections::array_buffer<T, A>>()
				, parents
				, &iintf::default_query_interface);
			return info;
		}
	};
}

template<typename T, template<typename>class A>
inline ang::collections::array_buffer<T, A>::array_buffer()
	: array<typename ang::collections::auto_type<T>::type, A>()
{
}

template<typename T, template<typename>class A>
inline ang::collections::array_buffer<T, A>::array_buffer(const ang::nullptr_t&)
	: array<typename ang::collections::auto_type<T>::type, A>()
{
}

template<typename T, template<typename>class A> template<typename IT>
inline ang::collections::array_buffer<T, A>::array_buffer(IT first, IT last)
	: array<typename ang::collections::auto_type<T>::type, A>(first, last)
{
}

template<typename T, template<typename>class A>
inline ang::collections::array_buffer<T, A>::array_buffer(wsize sz)
	: array<typename ang::collections::auto_type<T>::type, A>(sz)
{
}

template<typename T, template<typename>class A>
inline ang::collections::array_buffer<T, A>::array_buffer(ang::array<typename ang::collections::auto_type<T>::type, A> && ar)
	: array<typename ang::collections::auto_type<T>::type, A>(ang::forward<array<element_type, A>>(ar))
{
}

template<typename T, template<typename>class A>
inline ang::collections::array_buffer<T, A>::array_buffer(ang::array<typename ang::collections::auto_type<T>::type, A> const& ar)
	: array<typename ang::collections::auto_type<T>::type, A>(ar)
{
}

template<typename T, template<typename>class A>
inline ang::collections::array_buffer<T, A>::array_buffer(ang::collections::array_buffer<T, A>&& ar)
	: array<typename ang::collections::auto_type<T>::type, A>(ang::forward<array<typename ang::collections::auto_type<T>::type, A>>(ar))
{
}

template<typename T, template<typename>class A>
inline ang::collections::array_buffer<T, A>::array_buffer(const ang::collections::array_buffer<T,A>& ar)
	: array<typename ang::collections::auto_type<T>::type, A>(ar)
{
}

template<typename T, template<typename>class A>
inline ang::collections::array_buffer<T, A>::array_buffer(const ang::collections::ienum_ptr<T>& store)
	: array<typename ang::collections::auto_type<T>::type, A>()
{
	copy(store);
}

template<typename T, template<typename>class A> template<typename T2>
inline ang::collections::array_buffer<T, A>::array_buffer(ang::initializer_list<T2> list)
	: array<typename ang::collections::auto_type<T>::type, A>(ang::forward<ang::initializer_list<T2>>(list))
{
}

template<typename T, template<typename>class A> template<typename T2>
inline ang::collections::array_buffer<T, A>::array_buffer(ang::array_view<T2> const& ar)
	: array<typename ang::collections::auto_type<T>::type, A>(ar)
{
}

template<typename T, template<typename>class A>
inline ang::collections::array_buffer<T, A>::~array_buffer()
{
}

template<typename T, template<typename>class A> template<typename T2> 
inline void ang::collections::array_buffer<T, A>::copy(ang::collections::ienum_ptr<T2> const& items)
{
	array<typename ang::collections::auto_type<T>::type, A>::clear();
	wsize i = 0, sz = items->counter();
	if (sz > 0) {
		m_first = A<element_type>::allocate(sz);
		m_last = m_first + sz;
		if constexpr (is_trivially_constructible<T>::value) {
			for (T2 const& item : items) {
				m_first[i++] = items;
			}
		}
		else {
			for (auto const& item : items) {
				A<T>::template construct<T, const T2&>((T*)&m_first[i++], item);
			}
		}
	}
}

template<typename T, template<typename>class A>
inline void ang::collections::array_buffer<T, A>::dispose()
{
	clear();
}

template<typename T, template<typename>class A>
inline ang::rtti_t const& ang::collections::array_buffer<T, A>::value_type()const
{
	return type_of<element_type>();
}

template<typename T, template<typename>class A>
inline bool ang::collections::array_buffer<T, A>::set_value(ang::rtti_t const& id, ang::unknown_t in)
{
	return false;
}

template<typename T, template<typename>class A>
inline bool ang::collections::array_buffer<T, A>::get_value(ang::rtti_t const& id, ang::unknown_t out)const
{
	return false;
}

template<typename T, template<typename>class A>
inline ang::variant ang::collections::array_buffer<T, A>::clone()const
{
	return new array_buffer(*this);
}

template<typename T, template<typename>class A>
inline ang::string ang::collections::array_buffer<T, A>::to_string()const
{
	text::utf8_string buffer = new text::utf8_string_buffer();
	ang::to_string((*buffer) << "{ "_sv, m_first[0]);
	for (wsize i = 1; i < size(); i++) {	
		ang::to_string((*buffer) << ", "_sv, m_first[i]);
	}
	(*buffer) << " }"_sv;
	return buffer.get();
}

template<typename T, template<typename>class A>
inline ang::string ang::collections::array_buffer<T, A>::to_string(ang::text::text_format_t format)const
{
	text::utf8_string buffer = new text::utf8_string_buffer();
	ang::to_string((*buffer) << "{ "_sv, m_first[0], format);
	for (wsize i = 1; i < size(); i++) {
		ang::to_string((*buffer) << ", "_sv, m_first[i], format);
	}
	(*buffer) << " }"_sv;
	return buffer.get();
}

template<typename T, template<typename>class A>
inline bool ang::collections::array_buffer<T, A>::is_readonly()const
{
	return false;
}

template<typename T, template<typename>class A>
inline ang::text::encoding_t ang::collections::array_buffer<T, A>::encoding()const
{
	return text::encoding::binary;
}

template<typename T, template<typename>class A>
inline pointer ang::collections::array_buffer<T, A>::buffer_ptr()
{
	return get();
}
template<typename T, template<typename>class A>
inline const_pointer ang::collections::array_buffer<T, A>::buffer_ptr()const
{
	return get();
}

template<typename T, template<typename>class A>
inline wsize ang::collections::array_buffer<T, A>::buffer_size()const
{
	return size() * size_of<T>();
}

template<typename T, template<typename>class A>
inline wsize ang::collections::array_buffer<T, A>::mem_copy(wsize sz, pointer ptr, text::encoding_t)
{
	if constexpr (is_trivially_constructible<T>::value && is_trivially_destructible<T>::value) {
		sz = min(sz, size() * size_of<T>());
		::memcpy(get(), ptr, sz);
		return sz;
	} else {
		return 0;
	}
}

template<typename T, template<typename>class A>
inline ang::ibuffer_view_t ang::collections::array_buffer<T, A>::map_buffer(windex, wsize)
{
	if constexpr (is_trivially_constructible<T>::value && is_trivially_destructible<T>::value) {
		//TODO:
		return null;
	}
	else {
		return null;
	}
}

template<typename T, template<typename>class A>
inline bool ang::collections::array_buffer<T, A>::unmap_buffer(ang::ibuffer_view_t&, wsize)
{
	if constexpr (is_trivially_constructible<T>::value && is_trivially_destructible<T>::value) {
		//TODO:
		return false;
	}
	else {
		return false;
	}
}

template<typename T, template<typename>class A>
inline bool ang::collections::array_buffer<T, A>::can_realloc_buffer()const
{
	return true;
}

template<typename T, template<typename>class A>
inline bool ang::collections::array_buffer<T, A>::realloc_buffer(wsize sz)
{
	size(align_up<align_of<T>()>(sz / size_of<T>()));
	return true;
}

template<typename T, template<typename>class A>
inline wsize ang::collections::array_buffer<T, A>::counter()const
{
	return size();
}

template<typename T, template<typename>class A>
inline typename ang::collections::auto_type<T>::return_type ang::collections::array_buffer<T, A>::at(typename ang::collections::array_buffer<T, A>::base_iterator_t const& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw_exception(error_code::invalid_memory);
	if (&m_first != it.current())
		throw_exception(error_code::invalid_param);
	if (it.offset() >= size())
		throw_exception(error_code::array_overflow);
#endif
	if constexpr(is_pointer<typename ang::collections::auto_type<T>::return_type>::value)
		return m_first + it.offset();
	else
		return m_first[it.offset()];
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::iterator_t ang::collections::array_buffer<T, A>::at(windex idx)
{
	if (idx >= size())
		return  iterator_t(const_cast<array_buffer<T, A>*>(this), null, 0);
	return iterator_t(const_cast<array_buffer<T, A>*>(this), (pointer)&m_first, idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::const_iterator_t ang::collections::array_buffer<T, A>::at(windex idx)const
{
	if (idx >= size())
		return  const_iterator_t(const_cast<array_buffer<T, A>*>(this), null, 0);
	return const_iterator_t(const_cast<array_buffer<T, A>*>(this), (pointer)&m_first, idx);
}

template<typename T, template<typename>class A>
inline void ang::collections::array_buffer<T, A>::copy(const ang::collections::ienum_ptr<T>& items)
{
	array<typename ang::collections::auto_type<T>::type, A>::clear();
	array<typename ang::collections::auto_type<T>::type, A>::size(items->counter());
	wsize i = 0;
	if constexpr (is_trivially_constructible<T>::value) {
		for (T const& item : items) {
			m_first[i++] = item;
		}
	}
	else {
		for (auto const& item : items) {
			A<T>::template construct<T, const T&>((T*)&m_first[i++], item);
		}
	}
}

template<typename T, template<typename>class A>
ang::collections::ienum_ptr<T> ang::collections::array_buffer<T, A>::items()const
{
	return const_cast<ang::collections::array_buffer<T, A>*>(this);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::iterator_t ang::collections::array_buffer<T, A>::find(function<bool(typename ang::collections::auto_type<T>::arg_type const&)> cond, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (auto i = size(); i > 0; --i)
		{
			if (cond(m_first[i - 1]))
				return at(i - 1);
		}
		else for (auto i = 0U; i < size(); ++i)
		{
			if (cond(m_first[i]))
				return at(i);
		}
	}
	return at((wsize)invalid_index);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::iterator_t ang::collections::array_buffer<T, A>::find(function<bool(typename ang::collections::auto_type<T>::arg_type const&)> cond, ang::collections::array_buffer<T, A>::base_iterator_t next_to, bool invert)const
{
	if (!is_empty() && next_to.parent() == this)
	{
		if (invert) for (auto i = next_to.offset() + 1; i > 0; --i)
		{
			if (cond(m_first[i - 1]))
				return at(i - 1);
		}
		else for (auto i = next_to.offset(); i < size(); ++i)
		{
			if (cond(m_first[i]))
				return at(i);
		}
	}
	return at((wsize)invalid_index);
}

template<typename T, template<typename>class A>
inline ang::collections::ienum_ptr<T> ang::collections::array_buffer<T, A>::find_all(function<bool(typename ang::collections::auto_type<T>::arg_type const&)> cond)const
{
	smart_vector<T, A> out = new vector_buffer<T, A>();
	if (!is_empty())
	{
		for (auto i = size(); i > 0; --i)
		{
			if (cond(m_first[i - 1]))
				out += m_first[i - 1];
		}
	}
	return out->is_empty() ? null : out.get();
	return null;
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::forward_iterator_t ang::collections::array_buffer<T, A>::begin()
{
	return forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_first));
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::forward_iterator_t ang::collections::array_buffer<T, A>::end()
{
	return forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_first), size());
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::const_forward_iterator_t ang::collections::array_buffer<T, A>::begin()const
{
	return const_forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_first));
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::const_forward_iterator_t ang::collections::array_buffer<T, A>::end()const
{
	return const_forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_first), size());
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::forward_iterator_t ang::collections::array_buffer<T, A>::last()
{
	return size() ? forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_first), size() - 1) : end();
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::const_forward_iterator_t ang::collections::array_buffer<T, A>::last()const
{
	return size() ? const_forward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_first), size() - 1) : end();
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::backward_iterator_t ang::collections::array_buffer<T, A>::rbegin()
{
	return backward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_first), size() - 1);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::backward_iterator_t ang::collections::array_buffer<T, A>::rend()
{
	return backward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_first), (wsize)invalid_index);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::const_backward_iterator_t ang::collections::array_buffer<T, A>::rbegin()const
{
	return const_backward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_first), size() - 1);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array_buffer<T, A>::const_backward_iterator_t ang::collections::array_buffer<T, A>::rend()const
{
	return const_backward_iterator_t(const_cast<array_buffer*>(this), pointer(&m_first), (wsize)invalid_index);
}


template<typename T, template<typename>class A>
inline bool ang::collections::array_buffer<T, A>::increase(typename ang::collections::array_buffer<T, A>::base_iterator_t& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != &m_first)
		throw_exception(error_code::invalid_param);
	if (it.offset() >= size())
		throw_exception(error_code::array_overflow);
#endif
	it.offset(it.offset() + 1);
	if (it.offset() > size()) it.offset(size());
	return true;
}

template<typename T, template<typename>class A>
inline bool ang::collections::array_buffer<T, A>::increase(typename ang::collections::array_buffer<T, A>::base_iterator_t& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != &m_first)
		throw_exception(error_code::invalid_param);
	if (it.offset() >= size())
		throw_exception(error_code::array_overflow);
#endif
	it.offset(it.offset() + offset);
	if (it.offset() >= size())
		it.offset(size());
	return true;
}

template<typename T, template<typename>class A>
inline bool ang::collections::array_buffer<T, A>::decrease(typename ang::collections::array_buffer<T, A>::base_iterator_t& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != &m_first)
		throw_exception(error_code::invalid_param);
	if (it.offset() >= size())
		throw_exception(error_code::array_overflow);
#endif
	it.offset(it.offset() - 1);
	if ((int)it.offset() < -1)
		it.offset((wsize)invalid_index);
	return true;
}

template<typename T, template<typename>class A>
inline bool ang::collections::array_buffer<T, A>::decrease(typename ang::collections::array_buffer<T, A>::base_iterator_t& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != &m_first)
		throw_exception(error_code::invalid_param);
	if (it.offset() >= size())
		throw_exception(error_code::array_overflow);
#endif
	it.offset(it.offset() - offset);
	if ((int)it.offset() < -1)
		it.offset((wsize)invalid_index);
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::object_wrapper()
	: m_ptr(null)
{
	set(new ang::collections::array_buffer<T, A>());
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::object_wrapper(ang::collections::array_buffer<T, A>* ptr)
	: m_ptr(null)
{
	set(ptr != null ? ptr : new ang::collections::array_buffer<T, A>());
}


template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<T, A>> && other)
	: m_ptr(null)
{
	collections::array_buffer<T, A> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::object_wrapper(ang::object_wrapper<ang::collections::array_buffer<T, A>> const& other)
	: m_ptr(null)
{
	set(other.get());
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::object_wrapper(std::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::object_wrapper(data_type data, wsize sz)
	: m_ptr(null)
{
	set(new ang::collections::array_buffer<T, A>(data, sz));
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::object_wrapper(wsize reserve)
	: m_ptr(null)
{
	set(new ang::collections::array_buffer<T, A>(reserve));
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::object_wrapper(ang::array<typename ang::collections::auto_type<T>::type, A> && ar)
	: m_ptr(null)
{
	set(new ang::collections::array_buffer<T, A>(ang::forward<ang::array<typename ang::collections::auto_type<T>::type, A>>(ar)));
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::object_wrapper(ang::array<typename ang::collections::auto_type<T>::type, A> const& ar)
	: m_ptr(null)
{
	set(new ang::collections::array_buffer<T, A>(ar));
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::object_wrapper(const ang::collections::ienum_ptr<T>& store)
	: m_ptr(null)
{
	set(new collections::array_buffer<T, A>(store));
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::~object_wrapper()
{
	reset();
}

template<typename T, template <typename> class A>
inline void ang::object_wrapper<ang::collections::array_buffer<T, A>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

template<typename T, template <typename> class A>
inline void ang::object_wrapper<ang::collections::array_buffer<T, A>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T, template <typename> class A>
inline bool ang::object_wrapper<ang::collections::array_buffer<T, A>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T, template <typename> class A>
inline ang::collections::array_buffer<T, A>* ang::object_wrapper<ang::collections::array_buffer<T, A>>::get(void)const
{
	return m_ptr;
}

template<typename T, template <typename> class A>
inline void ang::object_wrapper<ang::collections::array_buffer<T, A>>::set(ang::collections::array_buffer<T, A>* ptr)
{
	ang::collections::array_buffer<T, A> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, template <typename> class A>
inline ang::collections::array_buffer<T, A>** ang::object_wrapper<ang::collections::array_buffer<T, A>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>& ang::object_wrapper<ang::collections::array_buffer<T, A>>::operator = (ang::collections::array_buffer<T, A>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>& ang::object_wrapper<ang::collections::array_buffer<T, A>>::operator = (const std::nullptr_t&)
{
	reset();
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>& ang::object_wrapper<ang::collections::array_buffer<T, A>>::operator = (ang::object_wrapper<ang::collections::array_buffer<T, A>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>& ang::object_wrapper<ang::collections::array_buffer<T, A>>::operator = (ang::object_wrapper<ang::collections::array_buffer<T, A>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper_ptr<ang::collections::array_buffer<T, A>> ang::object_wrapper<ang::collections::array_buffer<T, A>>::operator & (void)
{
	return this;
}

template<typename T, template <typename> class A>
inline ang::collections::array_buffer<T, A> * ang::object_wrapper<ang::collections::array_buffer<T, A>>::operator -> (void)const
{
	return get();
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::operator ang::collections::array_buffer<T, A> * (void)
{
	return get();
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::array_buffer<T, A>>::operator ang::collections::array_buffer<T, A> const* (void)const
{
	return get();
}

template<typename T, template <typename> class A> template<typename I>
inline typename ang::collections::auto_type<T>::type const& ang::object_wrapper<ang::collections::array_buffer<T, A>>::operator[](I const& idx)const
{
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw_exception(error_code::invalid_memory);
	if ((idx >= m_ptr->size()) || (idx < 0)) throw_exception(error_code::array_overflow);
#endif
	return m_ptr->data()[idx];
}

template<typename T, template <typename> class A> template<typename I>
inline typename ang::collections::auto_type<T>::type & ang::object_wrapper<ang::collections::array_buffer<T, A>>::operator[](I const& idx)
{
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw_exception(error_code::invalid_memory);
	if ((idx >= m_ptr->size()) || (idx < 0)) throw_exception(error_code::array_overflow);
#endif
	return m_ptr->data()[idx];
}

///////////////////////////////////////////////////////////////



#endif // !__ANGSYS_H__

