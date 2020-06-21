/*********************************************************************************************************************/
/*   File Name: ang/base/inline/array.inl                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_ARRAY_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_ARRAY_INL__
#define __ANG_BASE_ARRAY_INL__
template<typename T>
inline ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> const& ang::collections::array_node<T>::iterator() {
	static algorithms::iteration_algorithm<array_node<T>> s_iterator = algorithms::array_iteration<T>();
	return s_iterator;
}

template<typename T>
inline ang::collections::array_view<T>::array_view()
	: array_node<T>(null, null) {
}

template<typename T>
inline ang::collections::array_view<T>::array_view(typename ang::collections::array_view<T>::type first, typename ang::collections::array_view<T>::type last)
	: array_node<T>(first, last) {
}

template<typename T> template<typename U>
inline ang::collections::array_view<T>::array_view(ang::algorithms::forward_iterator<U, array_node<T>> first, ang::algorithms::forward_iterator<U, array_node<T>> last)
	: array_node<T>((T*)(typename remove_reference<U>::type*)first, (T*)(typename remove_reference<U>::type*)last) {
}

template<typename T>
inline ang::collections::array_view<T>::array_view(ang::nullptr_t const&)
	: array_node<T>(null, null) {
}

template<typename T>
inline ang::collections::array_view<T>::array_view(ang::collections::array_view<T> const& other)
	: array_node<T>(other) {
}

template<typename T>
inline ang::collections::array_view<T>::array_view(ang::collections::array_view<T> && other)
	: array_node<T>(ang::forward<array_view<T>>(other)) {
}

template<typename T> template<wsize N>
inline ang::collections::array_view<T>::array_view(T(&ar)[N])
	: array_node<T>((type)ar, (type)ar + N) {
}

template<typename T>
inline ang::collections::array_view<T>::~array_view() {
	m_first = null;
	m_last = null;
}

template<typename T>
inline bool ang::collections::array_view<T>::is_empty()const {
	return m_first == null;
}

template<typename T>
inline typename ang::collections::array_view<T>::type ang::collections::array_view<T>::get() {
	return m_first;
}

template<typename T>
inline typename ang::collections::array_view<T>::type const ang::collections::array_view<T>::get()const {
	return m_first;
}

template<typename T>
inline void ang::collections::array_view<T>::set(typename ang::collections::array_view<T>::type first, typename ang::collections::array_view<T>::type last) {
	m_first = first;
	m_last = max(first, last);
}

template<typename T>
inline typename ang::collections::array_view<T>::type ang::collections::array_view<T>::data()const {
	return m_first;
}

template<typename T>
inline wsize ang::collections::array_view<T>::size()const {
	return wsize(m_last - m_first);
}

template<typename T>
inline typename ang::collections::array_view<T>::iterator_t ang::collections::array_view<T>::begin(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return iterator_t(iter, iter.begin(static_cast<array_node<T>*>(const_cast<array_view<T>*>(this)), &idx), idx);
}

template<typename T>
inline typename ang::collections::array_view<T>::iterator_t ang::collections::array_view<T>::end(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return iterator_t(iter, iter.end(static_cast<array_node<T>*>(const_cast<array_view<T>*>(this)), &idx), idx);
}	

template<typename T>
inline typename ang::collections::array_view<T>::reverse_iterator_t ang::collections::array_view<T>::rbegin(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return reverse_iterator_t(iter, iter.rbegin(static_cast<array_node<T>*>(const_cast<array_view<T>*>(this)), &idx), idx);
}

template<typename T>
inline typename ang::collections::array_view<T>::reverse_iterator_t ang::collections::array_view<T>::rend(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return reverse_iterator_t(iter, iter.rend(static_cast<array_node<T>*>(const_cast<array_view<T>*>(this)), &idx), idx);
}

template<typename T>
inline void ang::collections::array_view<T>::clear() {
	m_first = null;
	m_last = null;
}

template<typename T>
inline ang::collections::array_view<T>& ang::collections::array_view<T>::operator = (ang::nullptr_t const&) {
	set(null, 0);
	return*this;
}

template<typename T>
inline ang::collections::array_view<T>& ang::collections::array_view<T>::operator = (typename ang::collections::array_view<T>::type val) {
	set(ang::move(val), 1u);
	return*this;
}

template<typename T>
inline ang::collections::array_view<T>& ang::collections::array_view<T>::operator = (ang::collections::array_view<T> const& val) {
	set(val.begin(), val.end());
	return*this; 
}

template<typename T>
inline ang::collections::array_view<T>& ang::collections::array_view<T>::operator = (ang::collections::array_view<T> && val) {
	m_first = ang::move(val.m_first);
	m_last = ang::move(val.m_last);
	return*this;
}

template<typename T> template<wsize N>
inline ang::collections::array_view<T>& ang::collections::array_view<T>::operator = (typename ang::collections::array_view<T>::element_type(&ar)[N]) {
	set(ar, (type)ar + N);
	return*this;
}

template<typename T>
inline ang::collections::array_view<T>::operator typename ang::collections::array_view<T>::type& () {
	return get();
}

template<typename T>
inline ang::collections::array_view<T>::operator typename ang::collections::array_view<T>::type ()const {
	return get();
}

template<typename T> template<typename I>
inline typename ang::collections::array_view<T>::element_type& ang::collections::array_view<T>::operator [](I idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
	return m_first[idx];
}

template<typename T> template<typename I>
inline typename ang::collections::array_view<T>::element_type const& ang::collections::array_view<T>::operator [](I idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
	return m_first[idx];
}

template<typename T>
inline typename ang::collections::array_view<T>::element_type** ang::collections::array_view<T>::operator & () {
	return &m_first;
}

template<typename T>
inline typename ang::collections::array_view<T>::element_type*const* ang::collections::array_view<T>::operator & ()const {
	return &m_first;
}


template<typename T> template<typename T2, template<typename> class A2>
inline bool ang::collections::array_view<T>::operator == (ang::collections::vector<T2, A2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return false;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return false;
		}
		return true;
	}
	else
		return false;
}

template<typename T> template<typename T2, template<typename> class A2>
inline bool ang::collections::array_view<T>::operator != (ang::collections::vector<T2, A2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return true;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return true;
		}
		return false;
	}
	else
		return true;
}

template<typename T> template<typename T2, template<typename> class A2>
inline bool ang::collections::array_view<T>::operator == (ang::collections::array<T2, A2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return false;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return false;
		}
		return true;
	}
	else
		return false;
}

template<typename T> template<typename T2, template<typename> class A2>
inline bool ang::collections::array_view<T>::operator != (ang::collections::array<T2, A2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return true;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return true;
		}
		return false;
	}
	else
		return true;
}

template<typename T> template<typename T2>
inline bool ang::collections::array_view<T>::operator == (ang::collections::array_view<T2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return false;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return false;
		}
		return true;
	}
	else
		return false;
}

template<typename T> template<typename T2>
inline bool ang::collections::array_view<T>::operator != (ang::collections::array_view<T2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return true;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return true;
		}
		return false;
	}
	else
		return true;
}
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, wsize SIZE>
inline ang::collections::stack_array<T, SIZE>::stack_array()
	: array_node<T>((type)m_data, (type)m_data + SIZE) {
}

template<typename T, wsize SIZE> template<wsize OTHER_SIZE>
inline ang::collections::stack_array<T, SIZE>::stack_array(const ang::collections::stack_array<T, OTHER_SIZE>& other)
	: array_node<T>((type)m_data, (type)m_data + SIZE) {
	for (windex i = 0; i < min<OTHER_SIZE, SIZE>(); ++i) {
		*(m_first + i) = other.m_data[i];
	}
}

template<typename T, wsize SIZE> template<typename T2, wsize N>
inline ang::collections::stack_array<T, SIZE>::stack_array(T2(&ar)[N])
	: array_node<T>((type)m_data, (type)m_data + SIZE) {
	for (windex i = 0; i < min<N, SIZE>(); ++i) {
		*(m_first + i) = ar[i];
	}
}

template<typename T, wsize SIZE>
inline ang::collections::stack_array<T, SIZE>::~stack_array() {
	m_first = null;
	m_last = null;
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::type ang::collections::stack_array<T, SIZE>::get() {
	return m_first;
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::const_type ang::collections::stack_array<T, SIZE>::get()const {
	return m_first;
}

template<typename T, wsize SIZE>
inline constexpr wsize ang::collections::stack_array<T, SIZE>::size()const {
	return SIZE;
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::type ang::collections::stack_array<T, SIZE>::data()const {
	return (type)m_first;
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::iterator_t ang::collections::stack_array<T, SIZE>::begin(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter) {
	wsize idx = 0;
	return iterator_t(iter, iter.begin(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::iterator_t ang::collections::stack_array<T, SIZE>::end(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter) {
	wsize idx = 0;
	return iterator_t(iter, iter.end(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::const_iterator_t ang::collections::stack_array<T, SIZE>::begin(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return const_iterator_t(iter, iter.begin(static_cast<array_node<T>*>(const_cast<stack_array<T, SIZE>*>(this)), &idx), idx);
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::const_iterator_t ang::collections::stack_array<T, SIZE>::end(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return const_iterator_t(iter, iter.end(static_cast<array_node<T> const*>(const_cast<stack_array<T, SIZE>*>(this)), &idx), idx);
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::reverse_iterator_t ang::collections::stack_array<T, SIZE>::rbegin(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter) {
	wsize idx = 0;
	return reverse_iterator_t(iter, iter.rbegin(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::reverse_iterator_t ang::collections::stack_array<T, SIZE>::rend(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter) {
	wsize idx = 0;
	return reverse_iterator_t(iter, iter.rend(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::reverse_const_iterator_t ang::collections::stack_array<T, SIZE>::rbegin(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return reverse_const_iterator_t(iter, iter.rbegin(static_cast<array_node<T> const*>(const_cast<stack_array<T, SIZE>*>(this)), &idx), idx);
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::reverse_const_iterator_t ang::collections::stack_array<T, SIZE>::rend(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return reverse_const_iterator_t(iter, iter.rend(static_cast<array_node<T> const*>(const_cast<stack_array<T, SIZE>*>(this)), &idx), idx);
}

template<typename T, wsize SIZE> template<typename IT>
inline void ang::collections::stack_array<T, SIZE>::copy(IT first, IT last) {
	for (windex i = 0; i < SIZE && first != last; i++, first++) {
		*(m_first + i) = (element_type)*first;
	}
}

template<typename T, wsize SIZE> template<wsize OTHER_SIZE>
inline ang::collections::stack_array<T, SIZE>& ang::collections::stack_array<T, SIZE>::operator = (const ang::collections::stack_array<element_type, OTHER_SIZE>& other) {
	for (windex i = 0; i < min<OTHER_SIZE, SIZE>(); ++i) {
		*(m_first + i) = other[i];
	}
	return*this;
}

template<typename T, wsize SIZE> template<typename T2, wsize N>
inline ang::collections::stack_array<T, SIZE>& ang::collections::stack_array<T, SIZE>::operator = (T2(&ar)[N]) {
	for (windex i = 0; i < min<N, SIZE>(); ++i) {
		*(m_first + i) = ar[i];
	}
	return*this;
}

template<typename T, wsize SIZE>
inline ang::collections::stack_array<T, SIZE>::operator type () {
	return get();
}

template<typename T, wsize SIZE>
inline ang::collections::stack_array<T, SIZE>::operator const_type ()const {
	return get();
}

template<typename T, wsize SIZE>
inline ang::collections::stack_array<T, SIZE>::operator ang::collections::array_view<T>()const {
	return array_view<T>(get(), size());
}

template<typename T, wsize SIZE> template<typename I>
inline typename ang::collections::stack_array<T, SIZE>::element_type& ang::collections::stack_array<T, SIZE>::operator [](I idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
	return *(m_first + idx);
}

template<typename T, wsize SIZE> template<typename I>
inline typename ang::collections::stack_array<T, SIZE>::element_type const& ang::collections::stack_array<T, SIZE>::operator [](I idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
	return *(m_first + idx);
}

template<typename T, wsize SIZE>
inline typename ang::collections::stack_array<T, SIZE>::element_type** ang::collections::stack_array<T, SIZE>::operator & () {
	return (element_type**)&m_first;
}
		
/////////////////////////////////////////////////////////////////////////////////////////

template<typename T, template<typename>class A>
inline ang::collections::array<T,A>::array()
	: array_node<T>(null, null) {
}

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>::array(ang::nullptr_t const&)
	: array_node<T>(null, null) {
}

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>::array(wsize sz)
	: array_node<T>(null, null) {
	sz = min(wsize(-1) / size_of<T>(), sz);
	if (sz > 0) {
		m_first = A<element_type>::allocate(sz);
		m_last = m_first + sz;
		if constexpr(!is_trivially_constructible<T>::value)
			for (windex i = 0; i < sz; ++i)
				A<element_type>::template construct<T>((T*)&m_first[i]);
	}
}	

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>::array(ang::collections::array<T, A> const& other)
	: array_node<T>(null, null) {
	wsize sz = other.size();
	if (sz > 0) {
		m_first = A<element_type>::allocate(sz);
		m_last = m_first + sz;
		if constexpr (is_trivially_constructible<T>::value) {
			for (windex i = 0; i < sz; ++i)
				m_first[i] = other.m_first[i];
		} else {
			for (windex i = 0; i < sz; ++i)
				A<element_type>::template construct<T, T const&>((T*)&m_first[i], other.m_first[i]);
		}
					
	}
}

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>::array(ang::collections::array<T, A> && other)
	: array_node<T>(null, null) {
	m_first = other.m_first;
	m_last = other.m_last;
	other.m_first = null;
	other.m_last = null;
}

template<typename T, template<typename>class A> template<typename T2>
inline ang::collections::array<T, A>::array(ang::collections::array_view<T2> const& ar)
	: array_node<T>(null, null) {
	if (ar.size() > 0) {
		wsize sz = ar.size();
		m_first = A<element_type>::allocate(sz);
		m_last = m_first + sz;
		if constexpr (is_trivially_constructible<T>::value) {
			for (windex i = 0; i < sz; ++i)
				m_first[i] = ar[i];
		} else {
			for (windex i = 0; i < sz; ++i)
				A<element_type>::template construct<T, T const&>((T*)&m_first[i], ar[i]);
		}
	}
}

template<typename T, template<typename>class A> template<typename T2>
inline ang::collections::array<T, A>::array(ang::initializer_list<T2> const& list)
	: array_node<T>(null, null) {
	if (list.size() > 0) {
		wsize sz = list.size();
		m_first = A<element_type>::allocate(sz);
		m_last = m_first + sz;
		windex i = 0;
		if constexpr (is_trivially_constructible<T>::value) {
			for (T2 const& item : list)
				m_first[i++] = item;
		} else {
			for (T2 const& item : list)
				A<element_type>::template construct<T, T2 const&>((T*)&m_first[i++], item);
		}
	}
}

template<typename T, template<typename>class A> template<typename T2, wsize N>
inline ang::collections::array<T, A>::array(T2(&ar)[N])
	: array_node<T>(null, null) {
	if (N > 0) {
		m_first = A<element_type>::allocate(N);
		m_last = m_first + N;
		if constexpr (is_trivially_constructible<T>::value) {
			for (windex i = 0; i < N; ++i)
				m_first[i] = ar[i];
		}
		else {
			for (windex i = 0; i < N; ++i)
				A<element_type>::template construct<T, T const&>((T*)&m_first[i], ar[i]);
		}
	}
}

template<typename T, template<typename>class A> template<typename IT>
inline ang::collections::array<T, A>::array(IT first, IT last)
	: array_node<T>(null, null) {
	wsize sz = 0;
	for (auto it = first; it != last; it++) sz++;

	if (sz > 0) {
		m_first = A<element_type>::allocate(sz);
		m_last = m_first + sz;
		if constexpr (is_trivially_constructible<T>::value) {
			if (first) for (auto it = m_first; it != m_last && first != last; it++, first++)
				*it = (element_type)*first;
		}
		else {
			if (first) for (auto it = m_first; it != m_last && first != last; it++, first++)
				A<element_type>::template construct<T, T const&>((T*)it, (element_type)*first);
			else for (auto it = m_first; it != m_last; it++)
				A<element_type>::template construct<T>((T*)it);
		}
	}
}

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>::~array() {
	clear();
}

template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::type ang::collections::array<T, A>::get() {
	return m_first;
}

template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::const_type ang::collections::array<T, A>::get()const {
	return m_first;
}

template<typename T, template<typename>class A>
inline bool ang::collections::array<T, A>::is_empty()const {
	return m_first == null;
}

template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::type ang::collections::array<T, A>::data()const {
	return m_first; 
}

template<typename T, template<typename>class A>
inline wsize ang::collections::array<T, A>::size()const {
	return wsize(m_last - m_first);
}

template<typename T, template<typename>class A>
inline void ang::collections::array<T, A>::size(wsize sz) {
	sz = min(wsize(-1) / size_of<T>(), sz);
	clear();
	if (sz > 0) {	
		m_first = A<element_type>::allocate(sz);
		m_last = m_first + sz;
		if constexpr (!is_trivially_constructible<T>::value) {
			for (windex i = 0; i < sz; ++i)
				A<element_type>::template construct<T>((T*)&m_first[i]);
		}
	}		
}

template<typename T, template<typename>class A>
inline ang::collections::array_view<T> ang::collections::array<T, A>::view()const {
	return array_view<T>(m_first, m_last);
}

template<typename T, template<typename>class A>
inline ang::collections::array_view<const T> ang::collections::array<T, A>::const_view()const {
	return array_view<const T>((const T*)m_first, (const T*)m_last);
}
	
template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::iterator_t ang::collections::array<T, A>::begin(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter) {
	wsize idx = 0;
	return iterator_t(iter, iter.begin(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::iterator_t ang::collections::array<T, A>::end(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter) {
	wsize idx = 0;
	return iterator_t(iter, iter.end(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::const_iterator_t ang::collections::array<T, A>::begin(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return const_iterator_t(iter, iter.begin(static_cast<array_node<T>*>(const_cast<array<T, A>*>(this)), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::const_iterator_t ang::collections::array<T, A>::end(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return const_iterator_t(iter, iter.end(static_cast<array_node<T>*>(const_cast<array<T, A>*>(this)), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::reverse_iterator_t ang::collections::array<T, A>::rbegin(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter) {
	wsize idx = 0;
	return reverse_iterator_t(iter, iter.rbegin(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::reverse_iterator_t ang::collections::array<T, A>::rend(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter) {
	wsize idx = 0;
	return reverse_iterator_t(iter, iter.rend(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::reverse_const_iterator_t ang::collections::array<T, A>::rbegin(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return reverse_const_iterator_t(iter, iter.rbegin(static_cast<array_node<T> const*>(const_cast<array<T, A>*>(this)), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::reverse_const_iterator_t ang::collections::array<T, A>::rend(ang::algorithms::iteration_algorithm<ang::collections::array_node<T>> iter)const {
	wsize idx = 0;
	return reverse_const_iterator_t(iter, iter.rend(static_cast<array_node<T> const*>(const_cast<array<T, A>*>(this)), &idx), idx);
}

template<typename T, template<typename>class A>
inline void ang::collections::array<T, A>::clear() {
	if (m_first) {
		if constexpr (!is_trivially_destructible<T>::value) {
			for (windex i = 0; i < size(); ++i)
				A<element_type>::template destroy<T>((T*)&m_first[i]);
		}
		A<element_type>::deallocate(m_first);
	}
	m_first = null;
	m_last = null;
}

template<typename T, template<typename>class A>
inline void ang::collections::array<T, A>::move(ang::collections::array<T, A>& other) {
	if ((array*)&other == this) return;
	clear();
	m_first = other.m_first;
	m_last = other.m_last;
	other.m_first = null;
	other.m_last = null;
}

template<typename T, template<typename>class A> template<typename IT>
inline void ang::collections::array<T, A>::copy(IT first, IT last) {
	clear();
	wsize sz = min(wsize(-1) / size_of<T>(), wsize(max(first, last) - first));
	if (sz > 0) {
		m_first = A<element_type>::allocate(sz);
		m_last = m_first + sz;
		if constexpr (is_trivially_constructible<T>::value) {
			for (auto it = m_first; first != last && it != m_last; it++, first++)
				*it = *first;
		} else {
			for (auto it = m_first; first != last && it != m_last; it++, first++)
				A<element_type>::template construct<T, decltype(*first)>((T*)it, *first);
		}	
	}
}

template<typename T, template<typename>class A> template<typename T2, template<typename> class A2>
inline void ang::collections::array<T, A>::copy(ang::collections::array<T2, A2> const& other) {
	clear();
	wsize sz = other.size();
	if (sz > 0)
		m_first = A<element_type>::allocate(sz);
	m_last = m_first + sz;
	if constexpr (is_trivially_constructible<T>::value) {
		for (windex i = 0; i < sz; ++i)
			m_first[i] = other[i];
	}
	else {
		for (windex i = 0; i < sz; ++i)
			A<element_type>::template construct<T, T2 const&>(&m_first[i], other[i]);
	}
}

template<typename T, template<typename>class A> template<typename T2>
inline void ang::collections::array<T, A>::copy(ang::collections::array_view<T2> const& other) {
	clear();
	wsize sz = other.size();
	if (sz > 0)
		m_first = A<element_type>::allocate(sz);
	m_last = m_first + sz;
	if constexpr (is_trivially_constructible<T>::value) {
		for (windex i = 0; i < sz; ++i)
			m_first[i] = other[i];
	}
	else {
		for (windex i = 0; i < sz; ++i)
			A<element_type>::template construct<T, T2 const&>(&m_first[i], other[i]);
	}
}

template<typename T, template<typename>class A> template<typename T2>
inline void ang::collections::array<T, A>::copy(ang::initializer_list<T2> const& list) {
	clear();
	wsize sz = list.size();
	if (sz > 0)
		m_first = A<element_type>::allocate(sz);
	m_last = m_first + sz;
	windex i = 0;
	if constexpr (is_trivially_constructible<T>::value) {
		for (T2 const& item : list)
			m_first[i++] = item;
	}
	else {
		for (T2 const& item : list)
			A<element_type>::template construct<T, T2 const&>(&m_first[i++], item);
	}
}

template<typename T, template<typename>class A> template<typename T2, wsize N>
inline void ang::collections::array<T, A>::copy(T2(&ar)[N]) {
	clear();
	if (N > 0) {
		m_first = A<element_type>::allocate(N);
		m_last = m_first + N;
		if constexpr (is_trivially_constructible<T>::value) {
			for (windex i = 0; i < N; ++i)
				m_first[i] = ar[i];
		}
		else {
			for (windex i = 0; i < N; ++i)
				A<element_type>::template construct<T, T2 const&>(&m_first[i], ar[i]);
		}
	}
}

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>& ang::collections::array<T, A>::operator = (typename ang::collections::array<T, A>::type first) {
	copy(first, first + 1u);
	return*this;
}

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>& ang::collections::array<T, A>::operator = (ang::nullptr_t const&) {
	clear();
	return*this;
}

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>& ang::collections::array<T, A>::operator = (ang::collections::array<T, A> const& val) {
	copy(val.m_first, val.m_last); 
	return*this;
}

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>& ang::collections::array<T, A>::operator = (ang::collections::array<T, A> && val) {
	move(val);
	return*this;
}

template<typename T, template<typename>class A> template<typename T2>
inline ang::collections::array<T, A>& ang::collections::array<T, A>::operator = (ang::collections::array_view<T2> const& ar) {
	copy(ar);
	return*this;
}

template<typename T, template<typename>class A> template<typename T2>
inline ang::collections::array<T, A>& ang::collections::array<T, A>::operator = (ang::initializer_list<T2> const& list) {
	copy(list);
	return*this;
}

template<typename T, template<typename>class A> template<typename T2, wsize N>
inline ang::collections::array<T, A>& ang::collections::array<T, A>::operator = (T2(&ar)[N]) {
	copy((type)ar, (type)ar + N);
	return*this;
}

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>::operator typename ang::collections::array<T, A>::type& () {
	return get();
}

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>::operator typename ang::collections::array<T, A>::type ()const {
	return get();
}

template<typename T, template<typename>class A>
inline ang::collections::array<T, A>::operator ang::collections::array_view<T> const& ()const {
	return *reinterpret_cast<array_view<T>*>(const_cast<array<T, A>*>(this));
}

template<typename T, template<typename>class A> template<typename I>
inline typename ang::collections::array<T, A>::element_type& ang::collections::array<T, A>::operator [](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
	return m_first[idx];
}

template<typename T, template<typename>class A> template<typename I>
inline typename ang::collections::array<T, A>::element_type const& ang::collections::array<T, A>::operator [](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
	return m_first[idx]; 
}

template<typename T, template<typename>class A>
inline typename ang::collections::array<T, A>::element_type** ang::collections::array<T, A>::operator & () {
	return &m_first;
}


template<typename T, template<typename>class A> template<typename T2, template<typename> class A2>
inline bool ang::collections::array<T, A>::operator == (ang::collections::vector<T2, A2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return false;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return false;
		}
		return true;
	}
	else
		return false;
}

template<typename T, template<typename>class A> template<typename T2, template<typename> class A2>
inline bool ang::collections::array<T, A>::operator != (ang::collections::vector<T2, A2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return true;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return true;
		}
		return false;
	}
	else
		return true;
}

template<typename T, template<typename>class A> template<typename T2, template<typename> class A2>
inline bool ang::collections::array<T, A>::operator == (ang::collections::array<T2, A2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return false;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return false;
		}
		return true;
	}
	else
		return false;
}

template<typename T, template<typename>class A> template<typename T2, template<typename> class A2>
inline bool ang::collections::array<T, A>::operator != (ang::collections::array<T2, A2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return true;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return true;
		}
		return false;
	}
	else
		return true;
}

template<typename T, template<typename>class A> template<typename T2>
inline bool ang::collections::array<T, A>::operator == (ang::collections::array_view<T2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return false;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return false;
		}
		return true;
	}
	else
		return false;
}

template<typename T, template<typename>class A> template<typename T2>
inline bool ang::collections::array<T, A>::operator != (ang::collections::array_view<T2> const& items)const
{
	if constexpr (has_logic_operation<logic_operation_type::same, T, T2>::value) {
		if (size() != items.size())
			return true;

		for (int i = 0; i < size(); i++) {
			if (!logic_operation<logic_operation_type::same, T, T2>::operate(m_first[i], items[i]))
				return true;
		}
		return false;
	}
	else
		return true;
}
#endif//__ANG_BASE_ARRAY_H__
