/*********************************************************************************************************************/
/*   File Name: ang/base/inline/vector.inl                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_VECTOR_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_VECTOR_INL__
#define __ANG_BASE_VECTOR_INL__


template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>::vector()
	: array_node<T>(null, null)
	, m_end(null) {
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>::vector(ang::nullptr_t const&)
	: vector() {
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>::vector(wsize sz)
	: vector() {
	size(sz);
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>::vector(ang::collections::vector<T, A> const& other)
	: vector() {
	capacity(other.size(), false);
	for (auto const& value : other) {
		push_back(value);
	}
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>::vector(ang::collections::vector<T, A> && other)
	: vector() {	
	m_first = other.m_first;
	m_last = other.m_last;
	m_end = other.m_end;	
	other.m_first = null;
	other.m_last = null;
	other.m_end = null;
}

template<typename T, template<typename>class A> template<typename IT>
inline ang::collections::vector<T, A>::vector(IT first, IT last)
	: vector() {
	for (auto it = first; it != last; it++) {
		push_back(*it);
	}
}

template<typename T, template<typename>class A> template<typename T2, template<typename>class A2>
inline ang::collections::vector<T, A>::vector(ang::collections::vector<T2, A2> const& other)
	: vector() {
	capacity(other.size(), false);
	for (auto const& value : other) {
		push_back(value);
	}
}

template<typename T, template<typename>class A> template<typename T2>
inline ang::collections::vector<T, A>::vector(ang::collections::array_view<T2> const& ar)
	: vector() {
	capacity(ar.size(), false);
	for (T2 const& val : ar) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename T2>
inline ang::collections::vector<T, A>::vector(ang::initializer_list<T2> const& list)
	: vector() {
	capacity(list.size(), false);
	for (T2 const& val : list) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename T2, wsize N>
inline ang::collections::vector<T, A>::vector(T2(&ar)[N])
	: vector() {
	capacity(N, false);
	for (T2 const& val : ar) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename T2, wsize N>
inline ang::collections::vector<T, A>::vector(const T2(&ar)[N])
	: vector() {
	capacity(N, false);
	for (T2 const& val : ar) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>::~vector() {
	clear();
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::type  ang::collections::vector<T, A>::get() {
	return m_first;
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::const_type ang::collections::vector<T, A>::get()const {
	return m_first;
}

template<typename T, template<typename>class A>
inline bool ang::collections::vector<T, A>::is_empty()const {
	return m_last == m_first;
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::type ang::collections::vector<T, A>::data()const {
	return m_first;
}

template<typename T, template<typename>class A>
inline wsize ang::collections::vector<T, A>::size()const {
	return wsize(m_last - m_first);
}

template<typename T, template<typename>class A>
inline void ang::collections::vector<T, A>::size(wsize sz) {
	if (sz < capacity()) {
		if constexpr (!is_trivially_constructible<T>::value || !is_trivially_destructible<T>::value)
		{
			if (sz != size()) {
				if constexpr (!is_trivially_destructible<T>::value)
					for (type it = m_first; it != m_last; ++it)
						A<element_type>::template destroy<T>(it);
				m_last = m_first + sz;
				if constexpr (!is_trivially_constructible<T>::value)
					for (type it = m_first; it != m_last; ++it)
						A<element_type>::template construct<T>(it);
			}
			else {

				for (type it = m_first; it != m_last; ++it) {
					if constexpr (!is_trivially_destructible<T>::value)
						A<element_type>::template destroy<T>(it);
					if constexpr (!is_trivially_constructible<T>::value)
						A<element_type>::template construct<T>(it);
				}
			}
		}
		else m_last = m_first + sz;
		return;
	}
	clear();
	wsize cp = 8;//minimum capacity
	while (cp <= sz) cp *= 2;

	m_first = A<element_type>::allocate(cp);
	m_last = m_first + sz;
	m_end = m_first + cp;
	if constexpr (is_trivially_constructible<T>::value)
		for (type it = m_first; it != m_last; ++it)
			A<element_type>::template construct<T>(it);
}

template<typename T, template<typename>class A>
inline wsize ang::collections::vector<T, A>::capacity()const {
	return wsize(m_end - m_first);
}

template<typename T, template<typename>class A>
inline void ang::collections::vector<T, A>::capacity(wsize sz, bool save) {
	if (sz <= capacity()) {	 
		if (!save) {
			if constexpr (!is_trivially_destructible<T>::value)
				for (type it = m_first; it != m_last; ++it)
					A<element_type>::template destroy<T>(it);
			m_last = m_first;
		}
		return;
	}
	wsize cp = 8;//minimum capacity
	while (cp <= sz)
		cp *= 2;
	sz = size();
	type new_data = A<element_type>::allocate(cp);
	if (save) {
		for (type it = m_first, it2 = new_data; it != m_last; ++it, it2++) {
			if constexpr (!is_trivially_constructible<T>::value)
				A<element_type>::template construct<T, T&&>(it2, ang::move(*it));
			else 
				*it2 = ang::move(*it);
			if constexpr (!is_trivially_destructible<T>::value)
				A<element_type>::template destroy<T>(it);
		}
	}
	else {
		if constexpr (!is_trivially_destructible<T>::value) {
			for (type it = m_first; it != m_last; ++it)
				A<element_type>::template destroy<T>(it);
		}
					
	}
	if (m_first)A<element_type>::deallocate(m_first);
	m_first = new_data;
	m_last = save ? m_first + sz : m_first;
	m_end = m_first + cp;
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::element_type& ang::collections::vector<T, A>::front() {
	wsize idx = 0;
	return *node_type::iterator().begin(static_cast<array_node<T>*>(this), &idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::element_type& ang::collections::vector<T, A>::back() {
	wsize idx = 0;
	return *node_type::iterator().rend(static_cast<array_node<T>*>(this), &idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::element_type const& ang::collections::vector<T, A>::front()const {
	wsize idx = 0;
	return *node_type::iterator().begin(static_cast<array_node<T>*>(this), &idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::element_type const& ang::collections::vector<T, A>::back()const {
	wsize idx = 0;
	return *node_type::iterator().rend(static_cast<array_node<T>*>(this), &idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::iterator_t ang::collections::vector<T, A>::begin() {
	wsize idx = 0;
	return iterator_t(node_type::iterator(), node_type::iterator().begin(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::iterator_t ang::collections::vector<T, A>::end() {
	wsize idx = 0;
	return iterator_t(node_type::iterator(), node_type::iterator().end(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::const_iterator_t ang::collections::vector<T, A>::begin()const {
	wsize idx = 0;
	return const_iterator_t(node_type::iterator(), node_type::iterator().begin(static_cast<array_node<T>*>(const_cast<vector<T, A>*>(this)), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::const_iterator_t ang::collections::vector<T, A>::end()const {
	wsize idx = 0;
	return const_iterator_t(node_type::iterator(), node_type::iterator().end(static_cast<array_node<T>*>(const_cast<vector<T, A>*>(this)), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::reverse_iterator_t ang::collections::vector<T, A>::rbegin() {
	wsize idx = 0;
	return reverse_iterator_t(node_type::iterator(), node_type::iterator().rbegin(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::reverse_iterator_t ang::collections::vector<T, A>::rend() {
	wsize idx = 0;
	return reverse_iterator_t(node_type::iterator(), node_type::iterator().rend(static_cast<array_node<T>*>(this), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::reverse_const_iterator_t ang::collections::vector<T, A>::rbegin()const {
	wsize idx = 0;
	return reverse_const_iterator_t(node_type::iterator(), node_type::iterator().rbegin(static_cast<array_node<T> const*>(const_cast<vector<T, A>*>(this)), &idx), idx);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::reverse_const_iterator_t ang::collections::vector<T, A>::rend()const {
	wsize idx = 0;
	return reverse_const_iterator_t(node_type::iterator(), node_type::iterator().rend(static_cast<array_node<T> const*>(const_cast<vector<T, A>*>(this)), &idx), idx);
}

template<typename T, template<typename>class A>
inline void ang::collections::vector<T, A>::clear() {
	if (m_first) {
		if constexpr (!is_trivially_destructible<T>::value) {
			for (type it = m_first; it != m_last; ++it)
				A<element_type>::template destroy<T>(it);
		}
		A<element_type>::deallocate(m_first);
	}
	m_first = null;
	m_last = null;
	m_end = null;
}

template<typename T, template<typename>class A>
inline void ang::collections::vector<T, A>::push_front(typename ang::collections::vector<T, A>::element_type && value) {
	if (size() == capacity())
		capacity(size() + 1);

	for (auto i = size(); i > 0U; --i)
	{
		if constexpr (!is_trivially_constructible<T>::value)
			A<element_type>::template construct<T, T&&>((T*)&m_first[i], ang::move(m_first[i - 1U]));
		else 
			m_first[i] = ang::move(m_first[i - 1U]);
		if constexpr (!is_trivially_destructible<T>::value)
			A<element_type>::destroy((T*)&m_first[i - 1U]);
	}

	if constexpr (!is_trivially_constructible<T>::value)
		A<element_type>::template construct<T, T>((T*)&m_first[0], ang::forward<element_type>(value));
	else
		m_first[0] = ang::forward<element_type>(value);
	m_last++;
}

template<typename T, template<typename>class A>
inline void ang::collections::vector<T, A>::push_front(typename ang::collections::vector<T, A>::element_type const& value) {
	if (size() == capacity())
		capacity(size() + 1);

	for (auto i = size(); i > 0U; --i)
	{
		if constexpr (!is_trivially_constructible<T>::value)
			A<element_type>::template construct<T, T&&>((T*)&m_first[i], ang::move(m_first[i - 1U]));
		else
			m_first[i] = ang::move(m_first[i - 1U]);
		if constexpr (!is_trivially_destructible<T>::value)
			A<element_type>::destroy((T*)&m_first[i - 1U]);
	}

	if constexpr (!is_trivially_constructible<T>::value)
		A<element_type>::template construct<T, T const&>((T*)&m_first[0], value);
	else
		m_first[0] = value;
	m_last++;
}

template<typename T, template<typename>class A>
inline void ang::collections::vector<T, A>::push_back(typename ang::collections::vector<T, A>::element_type && value) {
	if (size() == capacity())
		capacity(size() + 1);
	if constexpr (!is_trivially_constructible<T>::value)
		A<element_type>::template construct<T, T>(m_last, ang::forward<element_type>(value));
	else
		*m_last = ang::forward<element_type>(value);
	m_last++;
}

template<typename T, template<typename>class A>
inline void ang::collections::vector<T, A>::push_back(typename ang::collections::vector<T, A>::element_type const& value) {
	if (size() == capacity())
		capacity(size() + 1);
	if constexpr (!is_trivially_constructible<T>::value)
		A<element_type>::template construct<T, T const&>(m_last, value);
	else
		*m_last = value;
	m_last++;
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::iterator_t ang::collections::vector<T, A>::insert(windex idx, T&& value) {
	idx = min(idx, size());
	if (size() == capacity()) {
		wsize sz = size();
		wsize cp = 8;//minimum capacity
		while (cp <= sz)
			cp *= 2;
		type new_data = A<element_type>::allocate(cp);
					
		for (wsize i = 0; i < idx; i++) {
			if constexpr (!is_trivially_constructible<T>::value)
				A<element_type>::template construct<T, T&&>((T*)&new_data[i], ang::move(m_first[i]));
			else
				new_data[i] = ang::move(m_first[i]);
			if constexpr (!is_trivially_destructible<T>::value)
				A<element_type>::destroy((T*)&m_first[i]);
		}
		if constexpr (!is_trivially_constructible<T>::value)
			A<element_type>::template construct<T, T&&>((T*)&new_data[idx], ang::forward<T>(value));
		else
			new_data[idx] = ang::forward<T>(value);
		for (wsize i = idx; i < size(); i++) {
			if constexpr (!is_trivially_constructible<T>::value)
				A<element_type>::template construct<T, T&&>((T*)&new_data[i + 1], ang::move(m_first[i]));
			else
				new_data[i + 1] = ang::move(m_first[i]);
			if constexpr (!is_trivially_destructible<T>::value)
				A<element_type>::destroy((T*)&m_first[i]);
		}
		A<element_type>::deallocate(m_first);
		m_first = new_data;
		m_last = m_first + sz + 1;
		m_end = m_first + cp;
	}
	else {
		for (auto i = size(); i > idx; --i) {
			if constexpr (!is_trivially_constructible<T>::value)
				A<element_type>::template construct<T, T&&>((T*)&m_first[i], ang::move(m_first[i - 1U]));
			else
				m_first[i] = ang::move(m_first[i - 1U]);
			if constexpr (!is_trivially_destructible<T>::value)
				A<element_type>::destroy((T*)&m_first[i - 1U]);
		}
		if constexpr (!is_trivially_constructible<T>::value)
			A<element_type>::template construct<T, T&&>((T*)&m_first[idx], ang::forward<T>(value));
		else
			m_first[idx] = ang::forward<T>(value);
		m_last++;
	}
	return iterator_t(node_type::iterator(), static_cast<node_type*>(this), wsize(m_first + idx));
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::iterator_t ang::collections::vector<T, A>::insert(windex idx, T const& value) {
	idx = min(idx, size());
	if (size() == capacity()) {
		wsize sz = size();
		wsize cp = 8;//minimum capacity
		while (cp <= sz)
			cp *= 2;
		type new_data = A<element_type>::allocate(cp);

		for (wsize i = 0; i < idx; i++) {
			if constexpr (!is_trivially_constructible<T>::value)
				A<element_type>::template construct<T, T&&>((T*)&new_data[i], ang::move(m_first[i]));
			else
				new_data[i] = ang::move(m_first[i]);
			if constexpr (!is_trivially_destructible<T>::value)
				A<element_type>::destroy((T*)&m_first[i]);
		}
		if constexpr (!is_trivially_constructible<T>::value)
			A<element_type>::template construct<T, T const&>((T*)&new_data[idx], value);
		else
			new_data[idx] = value;
		for (wsize i = idx; i < size(); i++) {
			if constexpr (!is_trivially_constructible<T>::value)
				A<element_type>::template construct<T, T&&>((T*)&new_data[i + 1], ang::move(m_first[i]));
			else
				new_data[i + 1] = ang::move(m_first[i]);
			if constexpr (!is_trivially_destructible<T>::value)
				A<element_type>::destroy((T*)&m_first[i]);
		}
		A<element_type>::deallocate(m_first);
		m_first = new_data;
		m_end = m_last + sz + 1;
		m_end = m_first + cp;
	}
	else {
		for (auto i = size(); i > idx; --i) {
			if constexpr (!is_trivially_constructible<T>::value)
				A<element_type>::template construct<T, T&&>((T*)&m_first[i], ang::move(m_first[i - 1U]));
			else
				m_first[i] = ang::move(m_first[i - 1U]);
			if constexpr (!is_trivially_destructible<T>::value)
				A<element_type>::destroy((T*)&m_first[i - 1U]);
		}
		if constexpr (!is_trivially_constructible<T>::value)
			A<element_type>::template construct<T, T const&>((T*)&m_first[idx], value);
		else
			m_first[idx] = value;
		m_last++;
	}
	return iterator_t(node_type::iterator(), static_cast<node_type*>(this), wsize(m_first + idx));
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::iterator_t ang::collections::vector<T, A>::insert(typename ang::collections::vector<T, A>::iterator_t it, T&& value) {
	if (it < m_first)
		return end();
	return insert(wsize(m_first - it), ang::forward<T>(value));
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::iterator_t ang::collections::vector<T, A>::insert(typename ang::collections::vector<T, A>::iterator_t it, T const& value) {
	if (it < m_first)
		return end();
	return insert(wsize(m_first - it), value);
}

template<typename T, template<typename>class A>
inline bool ang::collections::vector<T, A>::pop_front(typename ang::collections::vector<T, A>::element_type& value) {
	if (size() == 0)
		return false;
	value = ang::move(m_first[0]);
	for (type it = m_first + 1; it != m_last; ++it)
		*(it - 1) = ang::move(*it);
	if constexpr (!is_trivially_destructible<T>::value)
		A<element_type>::destroy(--m_last);
	else m_last--;
	return true;
}

template<typename T, template<typename>class A>
inline bool ang::collections::vector<T, A>::pop_front() {
	if (size() == 0)
		return false;
	wsize sz = size();
	for (windex i = 1U; i < sz; ++i)
		m_first[i - 1] = ang::move(m_first[i]);
	if constexpr (!is_trivially_destructible<T>::value)
		A<element_type>::destroy(--m_last);
	else m_last--;
	return true;
}

template<typename T, template<typename>class A>
inline bool ang::collections::vector<T, A>::pop_back(typename ang::collections::vector<T, A>::element_type& value) {
	if (size() == 0)
		return false;
	value = ang::move(*(--m_last));
	if constexpr (!is_trivially_destructible<T>::value)
		A<element_type>::template destroy<T>(m_last);
}

template<typename T, template<typename>class A>
inline bool ang::collections::vector<T, A>::pop_back() {
	if (size() == 0)
		return false;
	m_last--;
	if constexpr (!is_trivially_destructible<T>::value)
		A<element_type>::template destroy<T>(m_last);
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::iterator_t ang::collections::vector<T, A>::remove(windex idx) {
	if (idx >= size())
		return end();
	m_last--;
	for (type i = m_first + idx; i < m_last; i++)
		*i = ang::move(*(i + 1));
	if constexpr (!is_trivially_destructible<T>::value)
		A<element_type>::destroy(m_last);
	return iterator_t(node_type::iterator(), static_cast<node_type*>(this), wsize(m_first + idx));
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::iterator_t ang::collections::vector<T, A>::remove(windex idx, typename ang::collections::vector<T, A>::element_type& value) {
	if (idx >= size())
		return end();
	value = ang::move(m_first[idx]);
	m_last--;
	for (type i = m_first + idx; i < m_last; i++)
		*i = ang::move(*(i + 1));
	if constexpr (!is_trivially_destructible<T>::value)
		A<element_type>::destroy(m_last);
	return iterator_t(node_type::iterator(), static_cast<node_type*>(this), wsize(m_first + idx));
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::iterator_t ang::collections::vector<T, A>::remove(typename ang::collections::vector<T, A>::iterator_t it) {
	if (it.get_index() >= windex(m_last) || it.get_index() < windex(m_first))
		return end();
	m_last--;
	for (type i = it; i < m_last; i++)
		*i = ang::move(*(i + 1));
	if constexpr (!is_trivially_destructible<T>::value)
		A<element_type>::destroy(m_last);
	return it;
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::iterator_t ang::collections::vector<T, A>::remove(typename ang::collections::vector<T, A>::iterator_t it, typename ang::collections::vector<T, A>::element_type& value) {
	if (it.get_index() >= windex(m_last) || it.get_index() < windex(m_first))
		return end();
	value = ang::move(*it);
	m_last--;
	for (type i = it; i < m_last; i++)
		*i = ang::move(*(i + 1));
	if constexpr (!is_trivially_destructible<T>::value)
		A<element_type>::destroy(m_last);
	return it;
}

template<typename T, template<typename>class A>
inline void ang::collections::vector<T, A>::move(ang::collections::vector<T, A>& ar) {
	if ((vector*)&ar == this) return;
	clear();
	m_first = ar.m_first;
	m_last = ar.m_last;
	m_end = ar.m_end;
	ar.m_first = null;
	ar.m_last = null;
	ar.m_end = null;
}

template<typename T, template<typename>class A> template<typename IT>
inline void ang::collections::vector<T, A>::copy(IT first, IT last) {
	//wsize sz = min(wsize(-1) / size_of<T>(), wsize(max(first, last) - first));
	//capacity(sz, false);
	clear();
	for (auto it = first; it != last; it++) {
		push_back(*it);
	}
}

template<typename T, template<typename>class A> template<typename T2, template<typename> class A2>
inline void ang::collections::vector<T, A>::copy(ang::collections::vector<T2, A2> const& ar) {
	capacity(ar.size(), false);
	for (T2 const& val : ar) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename T2>
inline void ang::collections::vector<T, A>::copy(ang::collections::array_view<T2> const& ar) {
	capacity(ar.size(), false);
	for (T2 const& val : ar) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename T2>
inline void ang::collections::vector<T, A>::copy(ang::initializer_list<T2> const& list) {
	capacity(list.size(), false);
	for (T2 const& val : list) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename T2, wsize N>
inline void ang::collections::vector<T, A>::copy(T2(&ar)[N]) {
	capacity(N, false);
	for (T2 const& val : ar) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename T2, wsize N>
inline void ang::collections::vector<T, A>::copy(const T2(&ar)[N]) {
	capacity(N, false);
	for (T2 const& val : ar) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename IT>
inline void ang::collections::vector<T, A>::extend(IT first, IT last) {
	//wsize sz = min(wsize(-1) / size_of<T>(), wsize(max(first, last) - first));
	//capacity(size() + sz, true);
	for (auto it = first; it != last; it++) {
		push_back(*it);
	}
}

template<typename T, template<typename>class A> template<typename T2, template<typename> class A2>
inline void ang::collections::vector<T, A>::extend(ang::collections::vector<T2, A2> const& ar) {
	capacity(size() + ar.size(), true);
	for (T2 const& val : ar) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename T2>
inline void ang::collections::vector<T, A>::extend(ang::collections::array_view<T2> const& ar) {
	capacity(size() + ar.size(), true);
	for (T2 const& val : ar) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename T2>
inline void ang::collections::vector<T, A>::extend(ang::initializer_list<T2> const& list) {
	capacity(size() + list.size(), true);
	for (T2 const& val : list) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename T2, wsize N>
inline void ang::collections::vector<T, A>::extend(T2(&ar)[N]) {
	capacity(size() + N, true);
	for (T2 const& val : ar) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A> template<typename T2, wsize N>
inline void ang::collections::vector<T, A>::extend(const T2(&ar)[N]) {
	capacity(size() + N, true);
	for (T2 const& val : ar) {
		push_back((T)val);
	}
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>& ang::collections::vector<T, A>::operator = (typename ang::collections::vector<T, A>::type val) {
	if (val)
		copy(val, val + 1);
	else
		clear();
	return*this;
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>& ang::collections::vector<T, A>::operator = (ang::collections::vector<T, A> const& val) {
	copy(val.m_first, val.m_last);
	return*this; 
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>& ang::collections::vector<T, A>::operator = (ang::collections::vector<T, A> && val) {
	move(val);
	return*this; 
}

template<typename T, template<typename>class A> template<typename T2> 
inline ang::collections::vector<T, A>& ang::collections::vector<T, A>::operator = (ang::collections::array_view<T2> const& ar) {
	copy(ar);
	return*this;
}

template<typename T, template<typename>class A> template<typename T2>
inline ang::collections::vector<T, A>& ang::collections::vector<T, A>::operator = (ang::initializer_list<T2> const& list) {
	copy(list);
	return*this;
}


template<typename T, template<typename>class A> template<typename T2, wsize N>
inline ang::collections::vector<T, A>& ang::collections::vector<T, A>::operator = (T2(&ar)[N]) {
	copy(ar);
	return*this;
}

template<typename T, template<typename>class A> template<typename T2, wsize N>
inline ang::collections::vector<T, A>& ang::collections::vector<T, A>::operator = (const T2(&ar)[N]) {
	copy(ar);
	return*this;
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>::operator typename ang::collections::vector<T, A>::type& () {
	return get();
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>::operator typename ang::collections::vector<T, A>::type ()const {
	return get();
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>::operator ang::collections::array_view<T> const&()const {
	return *reinterpret_cast<array_view<T>*>(const_cast<vector<T, A>*>(this));
}

template<typename T, template<typename>class A> template<typename I>
inline typename ang::collections::vector<T, A>::element_type& ang::collections::vector<T, A>::operator [](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
	return m_first[idx];
}

template<typename T, template<typename>class A> template<typename I>
inline typename ang::collections::vector<T, A>::element_type const& ang::collections::vector<T, A>::operator [](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
	return m_first[idx];
}

template<typename T, template<typename>class A>
inline typename ang::collections::vector<T, A>::element_type** ang::collections::vector<T, A>::operator & () {
	return &m_first;
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>& ang::collections::vector<T, A>::operator += (typename ang::collections::vector<T, A>::element_type && val) {
	push_back(forward<element_type>(val));
	return*this;
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>& ang::collections::vector<T, A>::operator += (typename ang::collections::vector<T, A>::element_type const& val) {
	push_back(val);
	return*this;
}

template<typename T, template<typename>class A>
inline ang::collections::vector<T, A>& ang::collections::vector<T, A>::operator -= (typename ang::collections::vector<T, A>::element_type& val) {
	pop_back(val);
	return*this;
}
	

template<typename T, template<typename>class A> template<typename T2, template<typename> class A2>
inline bool ang::collections::vector<T, A>::operator == (ang::collections::vector<T2, A2> const& items)const
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
inline bool ang::collections::vector<T, A>::operator != (ang::collections::vector<T2, A2> const& items)const
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
inline bool ang::collections::vector<T, A>::operator == (ang::collections::array<T2, A2> const& items)const
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
inline bool ang::collections::vector<T, A>::operator != (ang::collections::array<T2, A2> const& items)const
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
inline bool ang::collections::vector<T, A>::operator == (ang::collections::array_view<T2> const& items)const
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
inline bool ang::collections::vector<T, A>::operator != (ang::collections::array_view<T2> const& items)const
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
