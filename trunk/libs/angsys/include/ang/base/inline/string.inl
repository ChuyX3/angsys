/*********************************************************************************************************************/
/*   File Name: ang/base/inline/string.inl                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, 2019.                                                        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_STRING_INL__
#define __ANG_BASE_STRING_INL__

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E,A>::fast_string() {
	m_data.m_storage_type = storage_type_stack;
	m_data.m_allocated_length = 0;
	m_data.m_allocated_capacity = 0;
	m_data.m_stack_buffer[0] = 0;
	m_data.m_allocated_buffer = null;
}

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>::fast_string(ang::text::fast_string<E,A> && value) {
	if (value.m_data.m_storage_type != storage_type_allocated) {
		m_data.m_storage_type = storage_type_stack;
		m_data.m_allocated_length = 0;
		m_data.m_allocated_capacity = 0;
		m_data.m_allocated_buffer = null;
		m_data.m_stack_buffer[0] = 0;
		copy(value.cstr());
	}
	else {
		m_data.m_storage_type = storage_type_allocated;
		m_data.m_allocated_length = value.m_data.m_allocated_length;
		m_data.m_allocated_capacity = value.m_data.m_allocated_capacity;
		m_data.m_allocated_buffer = value.m_data.m_allocated_buffer;
	}
	value.m_data.m_storage_type = storage_type_stack;
	value.m_data.m_allocated_length = 0;
	value.m_data.m_allocated_capacity = 0;
	value.m_data.m_allocated_buffer = null;
	value.m_data.m_stack_buffer[0] = 0;
}

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>::fast_string(ang::text::fast_string<E, A> const& value) {
	m_data.m_storage_type = storage_type_stack;
	m_data.m_allocated_length = 0;
	m_data.m_allocated_capacity = 0;
	m_data.m_allocated_buffer = null;
	m_data.m_stack_buffer[0] = 0;
	copy(value.cstr());
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2>
inline ang::text::fast_string<E, A>::fast_string(ang::text::fast_string<E2,A2> const& value) {
	m_data.m_storage_type = storage_type_stack;
	m_data.m_allocated_length = 0;
	m_data.m_allocated_capacity = 0;
	m_data.m_allocated_buffer = null;
	m_data.m_stack_buffer[0] = 0;
	copy(value.cstr());
}

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>::fast_string(ang::nullptr_t const&) {
	m_data.m_storage_type = storage_type_stack;
	m_data.m_allocated_length = 0;
	m_data.m_allocated_capacity = 0;
	m_data.m_allocated_buffer = null;
	m_data.m_stack_buffer[0] = 0;
}
			
template<ang::text::encoding E, template<typename>class A> template<typename T2, ang::text::encoding E2>
inline ang::text::fast_string<E, A>::fast_string(ang::str_view<const T2, E2> const& value) {
	m_data.m_storage_type = storage_type_stack;
	m_data.m_allocated_length = 0;
	m_data.m_allocated_capacity = 0;
	m_data.m_allocated_buffer = null;
	m_data.m_stack_buffer[0] = 0;
	copy(value);
}

template<ang::text::encoding E, template<typename>class A> template<typename T2, wsize N>
inline ang::text::fast_string<E, A>::fast_string(T2 const(&value)[N]) {
	m_data.m_storage_type = storage_type_stack;
	m_data.m_allocated_length = 0;
	m_data.m_allocated_capacity = 0;
	m_data.m_allocated_buffer = null;
	m_data.m_stack_buffer[0] = 0;
	copy(str_view<const T2>(value, N - 1));
}

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>::~fast_string() {
	clear();
}

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>& ang::text::fast_string<E, A>::operator = (ang::text::fast_string<E, A> && value) {
	if (this != &value) {
		clear();
		if (value.m_data.m_storage_type != storage_type_allocated) {
			m_data.m_storage_type = storage_type_stack;
			m_data.m_allocated_length = 0;
			m_data.m_allocated_capacity = 0;
			m_data.m_allocated_buffer = null;
			m_data.m_stack_buffer[0] = 0;
			copy(value.cstr());
		}
		else {
			m_data.m_storage_type = storage_type_allocated;
			m_data.m_allocated_length = value.m_data.m_allocated_length;
			m_data.m_allocated_capacity = value.m_data.m_allocated_capacity;
			m_data.m_allocated_buffer = value.m_data.m_allocated_buffer;
		}
		value.m_data.m_storage_type = storage_type_stack;
		value.m_data.m_allocated_length = 0;
		value.m_data.m_allocated_capacity = 0;
		value.m_data.m_allocated_buffer = null;
		value.m_data.m_stack_buffer[0] = 0;
	}
	return *this;
}

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>& ang::text::fast_string<E, A>::operator = (fast_string const& value) {
	copy(value.cstr());
	return *this;
}

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>& ang::text::fast_string<E, A>::operator = (ang::nullptr_t const&) {
	clear();
	return *this;
}

template<ang::text::encoding E, template<typename>class A> template<typename T2, ang::text::encoding E2>
inline ang::text::fast_string<E, A>& ang::text::fast_string<E, A>::operator = (ang::str_view<const T2, E2> const& value) {
	copy(value);
	return *this;
}

template<ang::text::encoding E, template<typename>class A> template<typename T2, wsize N>
inline ang::text::fast_string<E, A>& ang::text::fast_string<E, A>::operator = (T2 const(&value)[N]) {
	copy(str_view<const T2>(value, N - 1));
	return *this;
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename> class A2 >
inline ang::text::fast_string<E, A>& ang::text::fast_string<E, A>::operator += (ang::text::fast_string<E2,A2> const& value) {
	concat(value.cstr());
	return *this;
}

template<ang::text::encoding E, template<typename>class A> template<typename T2, ang::text::encoding E2>
inline ang::text::fast_string<E, A>& ang::text::fast_string<E, A>::operator += (ang::str_view<const T2, E2> const& value) {
	concat(value);
	return *this;
}

template<ang::text::encoding E, template<typename>class A> template<typename index_t>
inline typename ang::text::fast_string<E, A>::type& ang::text::fast_string<E, A>::operator[](index_t const& i) {
	return str()[i];
}

template<ang::text::encoding E, template<typename>class A> template<typename index_t>
inline typename ang::text::fast_string<E, A>::type const& ang::text::fast_string<E, A>::operator[](index_t const& i)const {
	return cstr()[i];
}

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>::operator typename ang::text::fast_string<E, A>::view_t() { return str(); }

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>::operator typename ang::text::fast_string<E, A>::cview_t()const { return cstr(); }

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>::operator ang::raw_str_t() { return str(); }

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>::operator ang::raw_cstr_t()const { return cstr(); }


template<ang::text::encoding E, template<typename>class A>
inline void ang::text::fast_string<E, A>::attach(view_t value) {
	clear();
	m_data.m_storage_type = storage_type_attached;
	m_data.m_attached_buffer = value.str();
	m_data.m_attached_length = value.size();
	m_data.m_attached_capacity = 0;
}

template<ang::text::encoding E, template<typename>class A>
inline void ang::text::fast_string<E, A>::attach(cview_t value) {
	m_data.m_storage_type = storage_type_attached;
	m_data.m_attached_buffer = (ptr_t)value.cstr();
	m_data.m_attached_length = value.size();
	m_data.m_attached_capacity = 0;
}

template<ang::text::encoding E, template<typename>class A>
inline void ang::text::fast_string<E, A>::move(ang::text::fast_string<E, A>& value) {
	if (this != &value) {
		clear();
		if (value.m_data.m_storage_type != storage_type_allocated) {
			copy(value.cstr());
		}
		else {
			m_data.m_storage_type = storage_type_allocated;
			m_data.m_allocated_length = value.m_data.m_allocated_length;
			m_data.m_allocated_capacity = value.m_data.m_allocated_capacity;
			m_data.m_allocated_buffer = value.m_data.m_allocated_buffer;
		}
		value.m_data.m_storage_type = storage_type_stack;
		value.m_data.m_allocated_length = 0;
		value.m_data.m_allocated_capacity = 0;
		value.m_data.m_allocated_buffer = null;
	}
}

template<ang::text::encoding E, template<typename>class A>
inline bool ang::text::fast_string<E, A>::is_empty()const {
	return length() == 0;
}

template<ang::text::encoding E, template<typename>class A>
inline void ang::text::fast_string<E, A>::length(wsize len) {
	if (m_data.m_storage_type == storage_type_allocated) {
		m_data.m_allocated_length = min(len, m_data.m_allocated_capacity);
	}
	else if(m_data.m_storage_type == storage_type_attached) {
		m_data.m_attached_length = len;
	}
	else {
		m_data.m_stack_length = min(len, CAPACITY - 1);
	}
}

template<ang::text::encoding E, template<typename>class A>
inline wsize ang::text::fast_string<E, A>::capacity() const {
	return m_data.m_storage_type == storage_type_allocated ? m_data.m_allocated_capacity : CAPACITY - 1;
}

template<ang::text::encoding E, template<typename>class A>
inline void ang::text::fast_string<E, A>::realloc(wsize new_size, bool save) {
	if (capacity() > new_size)
		return;
	wsize sz = CAPACITY, i = 0, j = 0;
	while (sz <= (new_size + 1))
		sz *= 2;
	ptr_t ptr = A<type>::allocate(sz + 1);
	if (save) {
		cview_t my_data = cstr();
		encoder::convert(ptr, i, my_data, j, true, sz, my_data.size());
	}
	clear();
	m_data.m_storage_type = storage_type_allocated;
	m_data.m_allocated_buffer = ptr;
	m_data.m_allocated_length = i;
	m_data.m_allocated_capacity = sz;
}

template<ang::text::encoding E, template<typename>class A>
inline void ang::text::fast_string<E, A>::clear() {
	if (m_data.m_storage_type == storage_type_allocated) {
		A<type>::deallocate(m_data.m_allocated_buffer);
	}
	m_data.m_storage_type = storage_type_stack;
	m_data.m_allocated_length = 0;
	m_data.m_allocated_capacity = 0;
	m_data.m_allocated_buffer = null;
}

template<ang::text::encoding E, template<typename>class A>
inline wsize ang::text::fast_string<E, A>::length()const {
	return m_data.m_storage_type == storage_type_allocated ? m_data.m_allocated_length
		: m_data.m_storage_type == storage_type_attached ? m_data.m_attached_length
		: m_data.m_stack_length;
}

template<ang::text::encoding E, template<typename>class A>
inline typename ang::text::fast_string<E, A>::view_t ang::text::fast_string<E, A>::str() {
	return m_data.m_storage_type == storage_type_allocated ? view_t(m_data.m_allocated_buffer, m_data.m_allocated_length)
		: m_data.m_storage_type == storage_type_attached ? view_t(m_data.m_attached_buffer, m_data.m_attached_length)
		: view_t(m_data.m_stack_length ? m_data.m_stack_buffer : null, m_data.m_stack_length);
}

template<ang::text::encoding E, template<typename>class A>
inline typename ang::text::fast_string<E, A>::cview_t ang::text::fast_string<E, A>::cstr()const {
	return m_data.m_storage_type == storage_type_allocated ? cview_t(m_data.m_allocated_buffer, m_data.m_allocated_length)
		: m_data.m_storage_type == storage_type_attached ? cview_t(m_data.m_attached_buffer, m_data.m_attached_length)
		: cview_t(m_data.m_stack_length ? m_data.m_stack_buffer : null, m_data.m_stack_length);
}

template<ang::text::encoding E, template<typename>class A>
inline typename ang::text::fast_string<E, A>::cview_t ang::text::fast_string<E, A>::to_string()const {
	return cstr();
}

template<ang::text::encoding E, template<typename>class A>
inline typename ang::text::fast_string<E, A>::type& ang::text::fast_string<E, A>::at(windex i) {
	return str()[i];
}

template<ang::text::encoding E, template<typename>class A>
inline typename ang::text::fast_string<E, A>::type const& ang::text::fast_string<E, A>::at(windex i)const {
	return cstr()[i];
}

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A> ang::text::fast_string<E, A>::sub_string(windex start, windex end)const {
	cview_t my_data = cstr();
	if (start >= end || start >= my_data.size())
		return null;
	fast_string new_str;
	new_str.copy(cview_t(&my_data[start], min(end - start, my_data.size())));
	return ang::move(new_str);
}

template<ang::text::encoding E, template<typename>class A>
inline ang::text::fast_string<E, A>& ang::text::fast_string<E, A>::sub_string(ang::text::fast_string<E, A>& out, windex start, windex end)const {
	cview_t my_data = cstr();
	if (start >= end || start >= my_data.size())
		return out;
	out.copy(cview_t(&my_data[start], min(end - start, my_data.size())));
	return out;
}

template<ang::text::encoding E, template<typename>class A> template<typename T2, ang::text::encoding E2>
inline int ang::text::fast_string<E, A>::compare(ang::str_view<const T2, E2> const& value)const {
	if constexpr (E2 == ang::text::encoding::auto_detect)
		return text::encoder::compare(cstr(), value);
	else
		return encoder::compare(cstr(), value);
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2>
inline int ang::text::fast_string<E, A>::compare(ang::text::fast_string<E2, A2> const& value)const {
	return encoder::compare(cstr(), value.cstr());
}

template<ang::text::encoding E, template<typename>class A> template<typename T2, ang::text::encoding E2>
inline windex ang::text::fast_string<E, A>::compare_until(ang::str_view<const T2, E2> const& value)const {
	if constexpr (E2 == ang::text::encoding::auto_detect)
		return text::encoder::compare_until(cstr(), value);
	else
		return encoder::compare_until(cstr(), value);
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2>
inline windex ang::text::fast_string<E, A>::compare_until(ang::text::fast_string<E2, A2> const& value)const {
	return encoder::compare_until(cstr(), value.cstr());
}

template<ang::text::encoding E, template<typename>class A> template<typename T2,  ang::text::encoding E2>
inline windex ang::text::fast_string<E, A>::find(ang::str_view<const T2, E2> const& value, windex start, windex end)const {
	auto my_data = cstr();
	my_data.set(my_data.cstr(), min(end, my_data.size()));
	if constexpr (E2 == ang::text::encoding::auto_detect)
		return text::encoder::find(my_data, value, start);
	else
		return encoder::find(my_data, my_data.size(), value, value.size(), start);
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2>
inline windex ang::text::fast_string<E, A>::find(ang::text::fast_string<E2, A2> const& value, windex start, windex end)const {
	auto my_data = cstr();
	auto your_data = value.cstr();
	return encoder::find(my_data, min(end, my_data.size()), your_data, your_data.size(), start);
}

template<ang::text::encoding E, template<typename>class A> template<typename T2,  ang::text::encoding E2>
inline windex ang::text::fast_string<E, A>::find_reverse(ang::str_view<const T2, E2> const& value, windex start, windex end)const {
	auto my_data = cstr();
	my_data.set(my_data.cstr() + min(end, my_data.size()), max(my_data.size() - end, 0));
	if constexpr (E2 == ang::text::encoding::auto_detect)
		return text::encoder::find_reverse(my_data, value, start);
	else
		return encoder::find_reverse(my_data, my_data.size(), value, value.size(), start);
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2>
inline windex ang::text::fast_string<E, A>::find_reverse(fast_string<E2, A2> const& value, windex start, windex end)const {
	auto my_data = cstr();
	auto your_data = value.cstr();
	return encoder::find_reverse(my_data + min(end, my_data.size()), max(my_data.size() - end, 0), your_data, your_data.size(), start);
}

template<ang::text::encoding E, template<typename>class A> template<typename T2,  ang::text::encoding E2>
inline ang::vector<ang::text::fast_string<E,A>> ang::text::fast_string<E, A>::split(str_view<const T2, E2> const& value)const {
	vector<fast_string> list;
	
	windex beg = 0, end = 0;
	fast_string _word;
	cview_t data = cstr();
	if constexpr (E2 == ang::text::encoding::auto_detect)
		end = text::encoder::find(data, value, 0);
	else
		end = encoder::find(data, data.size(), value, value.size(), 0);

	if (end == invalid_index) {
		list.push_back(data);
		return ang::move(list);
	}
	do {
		_word = sub_string(beg, end);
		if (!_word.is_empty())
			list.push_back(ang::move(_word));

		beg = end + value.size();
		if constexpr (E2 == ang::text::encoding::auto_detect)
			end = text::encoder::find(data, value, beg);
		else
			end = encoder::find(data, data.size(), value, value.size(), beg);
	} while (end != invalid_index);

	_word = sub_string(beg, end);
	if (!_word.is_empty())
		list.push_back(ang::move(_word));
	return ang::move(list);
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2>
inline ang::vector<ang::text::fast_string<E,A>> ang::text::fast_string<E, A>::split(fast_string<E2, A2> const& value)const {
	return split(value.cstr());
}

template<ang::text::encoding E, template<typename>class A> template<typename T2,  ang::text::encoding E2>
inline wsize ang::text::fast_string<E, A>::copy(ang::str_view<const T2, E2> const& value) {
	wsize sz;
	if constexpr(native_encoding<ENCODING>::value == native_encoding<E>::value)
		sz = value.size();
	else
		sz = encoder::size(value);
	wsize j = 0;
	if (sz < (CAPACITY - 1))
	{
		clear();
		if constexpr (E2 == ang::text::encoding::auto_detect)
			text::encoder::convert(ang::str_t(m_data.m_stack_buffer, CAPACITY - 1, E), m_data.m_stack_length, value, j, true, CAPACITY - 1, value.size());
		else
			encoder::convert(m_data.m_stack_buffer, m_data.m_stack_length, value, j, true, CAPACITY - 1, value.size());
	}
	else {
		realloc(sz + 1, false);
		if constexpr (E2 == ang::text::encoding::auto_detect)
			text::encoder::convert(ang::str_t(m_data.m_stack_buffer, m_data.m_allocated_capacity, E), m_data.m_allocated_length, value, j, true, m_data.m_allocated_capacity, value.size());
		else
			encoder::convert(m_data.m_allocated_buffer, m_data.m_allocated_length, value, j, true, m_data.m_allocated_capacity, value.size());
	}
	return j;
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2>
inline wsize ang::text::fast_string<E, A>::copy(ang::text::fast_string<E2, A2> const& value) {
	return copy(value.cstr());
}

template<ang::text::encoding E, template<typename>class A> template<typename T2,  ang::text::encoding E2>
inline wsize ang::text::fast_string<E, A>::append(ang::str_view<const T2, E2> const& value) {
	wsize sz;
	if constexpr (native_encoding<ENCODING>::value == native_encoding<E>::value)
		sz = value.size();
	else
		sz = encoder::size(value);
	wsize j = 0, i = length();
	if ((i + sz) < (CAPACITY - 1))
	{
		if constexpr (E2 == ang::text::encoding::auto_detect)
			text::encoder::convert(ang::str_t(m_data.m_stack_buffer, CAPACITY - 1, E), m_data.m_stack_length, value, j, true, CAPACITY - 1, value.size());
		else
			encoder::convert(m_data.m_stack_buffer, m_data.m_stack_length, value, j, true, CAPACITY - 1, value.size());
	}
	else {
		realloc(sz + i + 1, true);
		if constexpr (E2 == ang::text::encoding::auto_detect)
			text::encoder::convert(ang::str_t(m_data.m_stack_buffer, m_data.m_allocated_capacity, E), m_data.m_allocated_length, value, j, true, m_data.m_allocated_capacity, value.size());
		else
			encoder::convert(m_data.m_allocated_buffer, m_data.m_allocated_length, value, j, true, m_data.m_allocated_capacity, value.size());
	}
	return j;
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2>
inline wsize ang::text::fast_string<E, A>::append(ang::text::fast_string<E2, A2> const& value) {
	return append(value.cstr());
}

template<ang::text::encoding E, template<typename>class A> template<typename T2, ang::text::encoding E2>
inline wsize ang::text::fast_string<E, A>::insert(ang::str_view<const T2, E2> const& value, windex i) {
	wsize sz;
	if constexpr (native_encoding<ENCODING>::value == native_encoding<E>::value)
		sz = value.size();
	else
		sz = encoder::size(value);
	wsize j = 0;
	i = length();
	if ((i + sz) < (CAPACITY - 1))
	{
		if constexpr (E2 == ang::text::encoding::auto_detect)
			text::encoder::convert(ang::str_t(m_data.m_stack_buffer, CAPACITY - 1, E), m_data.m_stack_length, value, j, true, CAPACITY - 1, value.size());
		else
			encoder::convert(m_data.m_stack_buffer, m_data.m_stack_length, value, j, true, CAPACITY - 1, value.size());
	}
	else {
		realloc(sz + i + 1, true);
		if constexpr (E2 == ang::text::encoding::auto_detect)
			text::encoder::convert(ang::str_t(m_data.m_stack_buffer, m_data.m_allocated_capacity, E), m_data.m_allocated_length, value, j, true, m_data.m_allocated_capacity, value.size());
		else
			encoder::convert(m_data.m_allocated_buffer, m_data.m_allocated_length, value, j, true, m_data.m_allocated_capacity, value.size());
	}
	return j;
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2>
inline wsize ang::text::fast_string<E, A>::insert(ang::text::fast_string<E2, A2> const& value, windex i) {
	return insert(value.cstr(), i);
}

template<ang::text::encoding E, template<typename>class A> template<typename T2, ang::text::encoding E2>
inline wsize ang::text::fast_string<E, A>::replace(ang::str_view<const T2, E2> const& value, windex beg, wsize end) {
	wsize sz;
	if constexpr (native_encoding<ENCODING>::value == native_encoding<E>::value)
		sz = value.size();
	else
		sz = encoder::size(value);
	wsize j = 0;

	return 0;
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2>
inline wsize ang::text::fast_string<E, A>::replace(ang::text::fast_string<E2, A2> const& value, windex beg, wsize end) {
	return replace(value.cstr(), beg, end);
}


template<ang::text::encoding E, template<typename>class A> template<typename T2, ang::text::encoding E2, typename...Ts>
inline wsize ang::text::fast_string<E, A>::format(str_view<const T2, E2> const& value, Ts&&...args) {
	clear();
	text_parser<E>::format(*this, value, ang::forward<Ts>(args)...);
	return length();
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2, typename...Ts>
inline wsize ang::text::fast_string<E, A>::format(fast_string<E2, A2> const& value, Ts&&...args) {
	clear();
	text_parser<E>::format(*this, value.cstr(), ang::forward<Ts>(args)...);
	return length();
}

template<ang::text::encoding E, template<typename>class A> template<typename T2, ang::text::encoding E2, typename...Ts>
inline wsize ang::text::fast_string<E, A>::append_format(str_view<const T2, E2> const& value, Ts&&...args) {
	auto sz = length();
	text_parser<E>::format(*this, value, ang::forward<Ts>(args)...);
	return length() - sz;
}

template<ang::text::encoding E, template<typename>class A> template<ang::text::encoding E2, template<typename>class A2, typename...Ts>
inline wsize ang::text::fast_string<E, A>::append_format(fast_string<E2, A2> const& value, Ts&&...args) {
	auto sz = length();
	text_parser<E>::format(*this, value.cstr(), ang::forward<Ts>(args)...);
	return length() - sz;
}


#endif//__ANG_BASE_STRING_H__
