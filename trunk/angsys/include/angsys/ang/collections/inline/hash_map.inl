#ifndef __ANG_COLLECTIONS_HASH_MAP_H__
#error Can't include HASH_MAP.inl, please include hash_map.h inside
#elif !defined __ANG_COLLECTIONS_HASH_MAP_INL__
#define __ANG_COLLECTIONS_HASH_MAP_INL__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// hash_map_object<K, T, allocator, hash_index_maker> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::hash_map_object()
	: m_count(0)
	, m_table()
{
	m_table.allocate(13);
	if (!m_table.is_empty())
	{
		memset((void*)m_table.get(), 0, sizeof(node_ptr_t) * m_table.size());
	}
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker> template<typename U, typename V>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::hash_map_object(ang::initializer_list<ang::collections::pair<U, V>> list)
	: hash_map_object()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert((K)it->key, (T)it->value);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::hash_map_object(const ang::nullptr_t&)
	: hash_map_object()
{
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::hash_map_object(hash_map_object&& other)
	: hash_map_object()
{
	m_count = other.m_count;
	m_table.move(other.m_table);

	other.m_count = 0;
	other.m_table = null;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::hash_map_object(const hash_map_object& other)
	: hash_map_object()
{
	extend(&other);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::hash_map_object(const hash_map_object* other)
	: hash_map_object()
{
	extend(other);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::hash_map_object(ang::collections::ienum<ang::collections::pair<K, T>> const* other)
	: hash_map_object()
{
	extend(other);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::~hash_map_object()
{
	clear();
}


template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline const ang::rtti_t& ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::class_info()
{
	static const cstr_view<char> name = strings::string_pool::instance()->save_string((((string("ang::collections::hash_map_object<"_s) += rtti::type_of<K>().type_name()) += ","_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<imap_type>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(hash_map_object<K, T, allocator, hash_index_maker>), alignof(hash_map_object<K, T, allocator, hash_index_maker>), parents, &default_query_interface);
	return info;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline const ang::rtti_t& ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::runtime_info()const
{
	return class_info();
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::query_interface(const ang::rtti_t& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == class_info().type_id())
	{
		if (out == null) return false;
		*out = static_cast<self_type*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == type_of<imap_type>().type_id()) {
		if (out == null) return false;
		*out = static_cast<imap_type*>(this);
		return true;
	}
	else if (id.type_id() == type_of<ienum_type>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ienum_type*>(this);
		return true;
	}
}


template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::is_empty()const {
	return m_count == 0;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline void ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::clear() {
	if (!m_table.is_empty())
	{
		for (wsize i = 0; i < m_table.size() && m_count > 0; ++i)
		{
			node_ptr_t temp = m_table[i];
			m_table[i] = null;
			while (temp)
			{
				node_ptr_t del = temp;
				temp = temp->next;
				m_alloc.template destroy<node_t>(del);
				m_alloc.deallocate(del);
				m_count--;
			}
		}
	}
	m_count = 0;
	m_table.clear();
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline void ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::empty() {
	if (!m_table.is_empty())
	{
		for (wsize i = 0; i < m_table.size() && m_count > 0; ++i)
		{
			node_ptr_t temp = m_table[i];
			m_table[i] = null;
			while (temp)
			{
				node_ptr_t del = temp;
				temp = temp->next;
				m_alloc.template destroy<node_t>(del);
				m_alloc.deallocate(del);
				m_count--;
			}
		}
	}
	m_count = 0;
	//m_table.clear();
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::move(hash_map_object& other)
{
	if (this == &other)
		return false;
	clear();
	m_count = other.m_count;
	m_table.move(other.m_table);
	other.m_count = 0;
	other.m_table.clear();
	return true;
}


template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
void ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::increase_capacity()
{
	auto new_size = algorithms::hash_table_get_next_size(m_table.size());
	node_array_t new_data;
	new_data.allocate(new_size);
	memset(new_data.get(), 0, sizeof(node_ptr_t) * new_size);

	if (m_table.size() && m_count)
	{
		wsize s = m_count;
		for (wsize i = 0; i < m_table.size() && s > 0; ++i) {
			node_ptr_t temp = m_table[i];
			m_table[i] = null;
			while (temp) {
				windex hash = index_maker::make(temp->data.key, new_size);
				node_ptr_t entry = temp;
				temp = temp->next;
				entry->next = new_data[hash];
				if (new_data[hash])new_data[hash]->prev = entry;
				new_data[hash] = entry;
				s--;
			}
		}
	}
	m_table.move(new_data);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::double_linked_node<ang::collections::pair<K, T>>* ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::find_node(K const& key)const
{
	if (m_table.size() == 0)
		return null;

	wsize idx = index_maker::make(key, m_table.size());
	node_ptr_t temp = m_table[idx];
	while (temp != null)
	{
		if (logic_operation<K, K, logic_operation_type::same>::operate(key, temp->data.key))
			return temp;
		temp = temp->next;
	}
	return null;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::pair<K, T>& ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::at(const ang::collections::base_iterator<ang::collections::pair<K, T>>& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw_exception(except_code::invalid_memory);
	if (it.current() == null)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_table.size())
		throw_exception(except_code::array_overflow);
#endif
	return node_ptr_t(it.current())->data;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::begin()
{
	for (auto i = 0U; i < m_table.size(); ++i)
	{
		if (m_table[i] != null)
			return iterator_t(const_cast<hash_map_object*>(this), m_table[i], i);
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::end()
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::begin()const
{
	for (auto i = 0U; i < m_table.size(); ++i)
	{
		if (m_table[i] != null)
			return iterator_t(const_cast<hash_map_object*>(this), m_table[i], i);
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::last()
{
	for (int i = (int)m_table.size() - 1; i >= 0; --i)
	{
		if (m_table[i] != null)
		{
			node_ptr_t node = m_table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::last()const
{
	for (int i = (int)m_table.size() - 1; i >= 0; --i)
	{
		if (m_table[i] != null)
		{
			node_ptr_t node = m_table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::end()const
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::backward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::rbegin()
{
	for (int i = (int)m_table.size() - 1; i >= 0; --i)
	{
		if (m_table[i] != null)
		{
			node_ptr_t node = m_table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::backward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::rend()
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_backward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::rbegin()const
{
	for (int i = (int)m_table.size() - 1; i >= 0; --i)
	{
		if (m_table[i] != null)
		{
			node_ptr_t node = m_table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_backward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::rend()const
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}


template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::increase(ang::collections::base_iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_table.size())
		throw_exception(except_code::array_overflow);
#endif
	auto node = node_ptr_t(it.current());

	if (node->next != null) {
		it.current(node->next);
		return true;
	}
	for (auto i = it.offset() + 1; i < m_table.size(); ++i)
	{
		if (m_table[i] != null) {
			it.current(m_table[i]);
			it.offset(i);
			return true;
		}
	}
	it.current(null);
	it.offset(0);
	return false;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::increase(ang::collections::base_iterator<ang::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_table.size())
		throw_exception(except_code::array_overflow);
#endif
	auto node = node_ptr_t(it.current());
	wsize i = it.offset();
	while (node && offset > 0)
	{
		offset--;
		if (node->next != null) {
			node = node->next;
			continue;
		}
		node = null;
		for (++i; i < m_table.size(); ++i)
		{
			if (m_table[i] != null) {
				node = m_table[i];
				break;
			}
		}
	}

	if (node != null)
	{
		it.current(node);
		it.offset(i);
		return true;
	}
	else
	{
		it.current(null);
		it.offset(0);
		return false;
	}
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::decrease(ang::collections::base_iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_table.size())
		throw_exception(except_code::array_overflow);
#endif
	auto node = node_ptr_t(it.current());

	if (node->prev != null) {
		it.current(node->prev);
		return true;
	}
	if (it.offset() == 0)
	{
		it.current(null);
		return false;
	}

	for (int i = (int)it.offset() - 1; i >= 0; --i)
	{
		if (m_table[i] != null) {
			node_ptr_t node = m_table[i];
			while (node->next != null)
				node = node->next;
			it.current(node);
			it.offset(i);
			return true;
		}
	}
	it.current(null);
	it.offset(0);
	return false;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::decrease(ang::collections::base_iterator<ang::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_table.size())
		throw_exception(except_code::array_overflow);
#endif
	auto node = node_ptr_t(it.current());
	int i = (int)it.offset();
	while (node && offset > 0)
	{
		offset--;
		if (node->prev != null) {
			node = node->prev;
			continue;
		}
		node = null;
		for (i--; i >= 0; --i)
		{
			if (m_table[i] != null) {
				node = m_table[i];
				while (node->next != null)
					node = node->next;
				break;
			}
		}
	}

	if (node != null)
	{
		it.current(node);
		it.offset(i);
		return true;
	}
	else
	{
		it.current(null);
		it.offset(0);
		return false;
	}
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline wsize ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::counter()const
{
	return m_count;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::copy(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return false;
	clear();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline void ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::extend(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::insert(K key, T value)
{
	if (m_count > (m_table.size() * 0.75))
		increase_capacity();

	if (find_node(key) != null)
		return false;
	wsize idx = index_maker::make(key, m_table.size());
	node_ptr_t entry = allocate_node<K, T>(ang::forward<K>(key), ang::forward<T>(value));

	if (m_table[idx])
	{
		entry->next = m_table[idx];
		m_table[idx]->prev = entry;
	}
	m_table[idx] = entry;
	m_count++;
	return true;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::insert(ang::collections::pair<K, T> pair)
{
	if (m_count > (m_table.size() * 0.75))
		increase_capacity();

	if (find_node(pair.key) != null)
		return false;
	wsize idx = index_maker::make(pair.key, m_table.size());
	node_ptr_t entry = allocate_node<K, T>(ang::forward<K>(pair.key), ang::forward<T>(pair.value));
	if (m_table[idx])
	{
		entry->next = m_table[idx];
		m_table[idx]->prev = entry;
	}
	m_table[idx] = entry;
	m_count++;
	return true;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::update(K key, T value)
{
	if (m_count > (m_table.size() * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(key);
	if (node == null)
	{
		wsize idx = index_maker::make(key, m_table.size());
		node = allocate_node<K, T>(ang::forward<K>(key), ang::forward<T>(value));
		if (m_table[idx])
		{
			node->next = m_table[idx];
			m_table[idx]->prev = node;
		}
		m_table[idx] = node;
		m_count++;
	}
	else
	{
		node->data.value = ang::move(value);
	}
	return true;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::update(ang::collections::pair<K, T> pair)
{
	if (m_count > (m_table.size() * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(pair.key);
	if (node == null)
	{
		wsize idx = index_maker::make(pair.key, m_table.size());
		node = allocate_node<K, T>(ang::forward<K>(pair.key), ang::forward<T>(pair.value));
		if (m_table[idx])
		{
			node->next = m_table[idx];
			m_table[idx]->prev = node;
		}
		m_table[idx] = node;
		m_count++;
	}
	else
	{
		node->data.value = ang::move(pair.value);
	}
	return true;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::remove(K const& key)
{
	if (m_table.is_empty())
		return false;

	wsize idx = index_maker::make(key, m_table.size());
	node_ptr_t temp = m_table[idx];

	while (temp != nullptr)
	{
		if (logic_operation<K, K, logic_operation_type::same>::operate(temp->data.key, key))
		{
			if (temp->prev)
			{
				temp->prev->next = temp->next;
				if (temp->next)
					temp->next->prev = temp->prev;
			}
			else
			{
				m_table[idx] = temp->next;
				if (temp->next)
					temp->next->prev = null;
			}
			m_alloc.template destroy<node_t>(temp);
			m_alloc.deallocate(temp);
			m_count--;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::remove(K const& key, T& value)
{
	if (m_table.is_empty())
		return false;

	wsize idx = index_maker::make(key, m_table.size());
	node_ptr_t temp = m_table[idx];

	while (temp != nullptr)
	{
		if (logic_operation<K, K, logic_operation_type::same>::operate(temp->data.key, key))
		{
			if (temp->prev)
			{
				temp->prev->next = temp->next;
				if (temp->next)
					temp->next->prev = temp->prev;
			}
			else
			{
				m_table[idx] = temp->next;
				if (temp->next)
					temp->next->prev = null;
			}
			value = ang::move(temp->data.value);
			m_alloc.template destroy<node_t>(temp);
			m_alloc.deallocate(temp);
			m_count--;
			return true;
		}
		temp = temp->next;
	}
	return false;
}


template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::remove(ang::collections::base_iterator<ang::collections::pair<K, T>>)
{
	return false;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::remove(ang::collections::base_iterator<ang::collections::pair<K, T>>, T& value)
{

	return false;
}


template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::has_key(const K& key)const
{
	return find_node(key) != null;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::find(const K& key)
{
	if (m_table.is_empty())
		return iterator_t(const_cast<hash_map_object*>(this));

	wsize idx = index_maker::make(key, m_table.size());
	node_ptr_t temp = m_table[idx];
	while (temp != null)
	{
		if (logic_operation<K, K, logic_operation_type::same>::operate(key, temp->data.key))
			return iterator_t(const_cast<hash_map_object*>(this), temp, idx);
		temp = temp->next;
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}


template<typename K, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, hash_index_maker>::find(const K& key)const
{
	if (m_table.is_empty())
		return iterator_t(const_cast<hash_map_object*>(this));

	wsize idx = index_maker::make(key, m_table.size());
	node_ptr_t temp = m_table[idx];
	while (temp != null)
	{
		if (logic_operation<K, K, logic_operation_type::same>::operate(key, temp->data.key))
			return iterator_t(const_cast<hash_map_object*>(this), temp, idx);
		temp = temp->next;
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::hash_map_object()
	: m_count(0)
	, m_table()
{
	m_table.allocate(13);
	if (!m_table.is_empty())
	{
		memset((void*)m_table.get(), 0, sizeof(node_ptr_t) * m_table.size());
	}
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::hash_map_object(ang::initializer_list<ang::collections::pair<ang::strings::basic_string<E, A>, T>> list)
	: hash_map_object()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(it->key, (T)it->value);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::hash_map_object(const ang::nullptr_t&)
	: hash_map_object()
{
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::hash_map_object(hash_map_object&& other)
	: hash_map_object()
{
	m_count = other.m_count;
	m_table.move(other.m_table);

	other.m_count = 0;
	other.m_table = null;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::hash_map_object(const hash_map_object& other)
	: hash_map_object()
{
	extend(&other);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::hash_map_object(const hash_map_object* other)
	: hash_map_object()
{
	extend(other);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::hash_map_object(ang::collections::ienum<ang::collections::pair<ang::strings::basic_string<E,A>, T>> const* other)
	: hash_map_object()
{
	extend(other);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::~hash_map_object()
{
	clear();
}


template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline const ang::rtti_t& ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::class_info()
{
	static const cstr_view<char> name = strings::string_pool::instance()->save_string((((string("ang::collections::hash_map_object<"_s) += rtti::type_of<ang::strings::basic_string<E, A>>().type_name()) += ","_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<imap_type>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>), alignof(hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>), parents, &default_query_interface);
	return info;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline const ang::rtti_t& ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::runtime_info()const
{
	return class_info();
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::query_interface(const ang::rtti_t& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == class_info().type_id())
	{
		if (out == null) return false;
		*out = static_cast<self_type*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == type_of<imap_type>().type_id()) {
		if (out == null) return false;
		*out = static_cast<imap_type*>(this);
		return true;
	}
	else if (id.type_id() == type_of<ienum_type>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ienum_type*>(this);
		return true;
	}
}


template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::is_empty()const {
	return m_count == 0;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline void ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::clear() {
	if (!m_table.is_empty())
	{
		for (wsize i = 0; i < m_table.size() && m_count > 0; ++i)
		{
			node_ptr_t temp = m_table[i];
			m_table[i] = null;
			while (temp)
			{
				node_ptr_t del = temp;
				temp = temp->next;
				m_alloc.template destroy<node_t>(del);
				m_alloc.deallocate(del);
				m_count--;
			}
		}
	}
	m_count = 0;
	m_table.clear();
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline void ang::collections::hash_map_object<ang::strings::basic_string<E, A>, T, allocator, hash_index_maker>::empty() {
	if (!m_table.is_empty())
	{
		for (wsize i = 0; i < m_table.size() && m_count > 0; ++i)
		{
			node_ptr_t temp = m_table[i];
			m_table[i] = null;
			while (temp)
			{
				node_ptr_t del = temp;
				temp = temp->next;
				m_alloc.template destroy<node_t>(del);
				m_alloc.deallocate(del);
				m_count--;
			}
		}
	}
	m_count = 0;
	//m_table.clear();
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::move(hash_map_object& other)
{
	if (this == &other)
		return false;
	clear();
	m_count = other.m_count;
	m_table.move(other.m_table);
	other.m_count = 0;
	other.m_table.clear();
	return true;
}


template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
void ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::increase_capacity()
{
	auto new_size = algorithms::hash_table_get_next_size(m_table.size());
	node_array_t new_data;
	new_data.allocate(new_size);
	memset(new_data.get(), 0, sizeof(node_ptr_t) * new_size);

	if (m_table.size() && m_count)
	{
		wsize s = m_count;
		for (wsize i = 0; i < m_table.size() && s > 0; ++i) {
			node_ptr_t temp = m_table[i];
			m_table[i] = null;
			while (temp) {
				windex hash = index_maker::make(temp->data.key, new_size);
				node_ptr_t entry = temp;
				temp = temp->next;
				entry->next = new_data[hash];
				if (new_data[hash])new_data[hash]->prev = entry;
				new_data[hash] = entry;
				s--;
			}
		}
	}
	m_table.move(new_data);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::double_linked_node<ang::collections::pair<ang::strings::basic_string<E,A>, T>>* ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::find_node(ang::raw_cstr_t const& key)const
{
	if (m_table.size() == 0)
		return null;

	wsize idx = hash_index_maker<raw_cstr_t>::make(key, m_table.size());
	node_ptr_t temp = m_table[idx];
	while (temp != null)
	{
		if (logic_operation<ang::strings::basic_string<E,A>, ang::strings::basic_string<E,A>, logic_operation_type::same>::operate(key, temp->data.key))
			return temp;
		temp = temp->next;
	}
	return null;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::pair<ang::strings::basic_string<E,A>, T>& ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::at(const ang::collections::base_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>>& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw_exception(except_code::invalid_memory);
	if (it.current() == null)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_table.size())
		throw_exception(except_code::array_overflow);
#endif
	return node_ptr_t(it.current())->data;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::forward_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::begin()
{
	for (auto i = 0U; i < m_table.size(); ++i)
	{
		if (m_table[i] != null)
			return iterator_t(const_cast<hash_map_object*>(this), m_table[i], i);
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::forward_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::end()
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_forward_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::begin()const
{
	for (auto i = 0U; i < m_table.size(); ++i)
	{
		if (m_table[i] != null)
			return iterator_t(const_cast<hash_map_object*>(this), m_table[i], i);
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::forward_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::last()
{
	for (int i = (int)m_table.size() - 1; i >= 0; --i)
	{
		if (m_table[i] != null)
		{
			node_ptr_t node = m_table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_forward_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::last()const
{
	for (int i = (int)m_table.size() - 1; i >= 0; --i)
	{
		if (m_table[i] != null)
		{
			node_ptr_t node = m_table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_forward_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::end()const
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::backward_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::rbegin()
{
	for (int i = (int)m_table.size() - 1; i >= 0; --i)
	{
		if (m_table[i] != null)
		{
			node_ptr_t node = m_table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::backward_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::rend()
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_backward_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::rbegin()const
{
	for (int i = (int)m_table.size() - 1; i >= 0; --i)
	{
		if (m_table[i] != null)
		{
			node_ptr_t node = m_table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_backward_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::rend()const
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}


template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::increase(ang::collections::base_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_table.size())
		throw_exception(except_code::array_overflow);
#endif
	auto node = node_ptr_t(it.current());

	if (node->next != null) {
		it.current(node->next);
		return true;
	}
	for (auto i = it.offset() + 1; i < m_table.size(); ++i)
	{
		if (m_table[i] != null) {
			it.current(m_table[i]);
			it.offset(i);
			return true;
		}
	}
	it.current(null);
	it.offset(0);
	return false;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::increase(ang::collections::base_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_table.size())
		throw_exception(except_code::array_overflow);
#endif
	auto node = node_ptr_t(it.current());
	wsize i = it.offset();
	while (node && offset > 0)
	{
		offset--;
		if (node->next != null) {
			node = node->next;
			continue;
		}
		node = null;
		for (++i; i < m_table.size(); ++i)
		{
			if (m_table[i] != null) {
				node = m_table[i];
				break;
			}
		}
	}

	if (node != null)
	{
		it.current(node);
		it.offset(i);
		return true;
	}
	else
	{
		it.current(null);
		it.offset(0);
		return false;
	}
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::decrease(ang::collections::base_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_table.size())
		throw_exception(except_code::array_overflow);
#endif
	auto node = node_ptr_t(it.current());

	if (node->prev != null) {
		it.current(node->prev);
		return true;
	}
	if (it.offset() == 0)
	{
		it.current(null);
		return false;
	}

	for (int i = (int)it.offset() - 1; i >= 0; --i)
	{
		if (m_table[i] != null) {
			node_ptr_t node = m_table[i];
			while (node->next != null)
				node = node->next;
			it.current(node);
			it.offset(i);
			return true;
		}
	}
	it.current(null);
	it.offset(0);
	return false;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::decrease(ang::collections::base_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
	if (it.offset() >= m_table.size())
		throw_exception(except_code::array_overflow);
#endif
	auto node = node_ptr_t(it.current());
	int i = (int)it.offset();
	while (node && offset > 0)
	{
		offset--;
		if (node->prev != null) {
			node = node->prev;
			continue;
		}
		node = null;
		for (i--; i >= 0; --i)
		{
			if (m_table[i] != null) {
				node = m_table[i];
				while (node->next != null)
					node = node->next;
				break;
			}
		}
	}

	if (node != null)
	{
		it.current(node);
		it.offset(i);
		return true;
	}
	else
	{
		it.current(null);
		it.offset(0);
		return false;
	}
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline wsize ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::counter()const
{
	return m_count;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::copy(ang::collections::ienum<ang::collections::pair<ang::strings::basic_string<E,A>, T>>const* items) {

	if (items == null)
		return false;
	clear();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline void ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::extend(ang::collections::ienum<ang::collections::pair<ang::strings::basic_string<E,A>, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::insert(ang::raw_cstr_t key, T value)
{
	if (m_count > (m_table.size() * 0.75))
		increase_capacity();

	if (find_node(key) != null)
		return false;
	wsize idx = hash_index_maker<raw_cstr_t>::make(key, m_table.size());
	node_ptr_t entry = allocate_node<ang::strings::basic_string<E,A>, T>(ang::forward<K>(key), ang::forward<T>(value));

	if (m_table[idx])
	{
		entry->next = m_table[idx];
		m_table[idx]->prev = entry;
	}
	m_table[idx] = entry;
	m_count++;
	return true;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::insert(ang::collections::pair<ang::strings::basic_string<E,A>, T> pair)
{
	if (m_count > (m_table.size() * 0.75))
		increase_capacity();

	if (find_node(pair.key) != null)
		return false;
	wsize idx = index_maker::make(pair.key, m_table.size());
	node_ptr_t entry = allocate_node<ang::strings::basic_string<E,A>, T>(ang::forward<K>(pair.key), ang::forward<T>(pair.value));
	if (m_table[idx])
	{
		entry->next = m_table[idx];
		m_table[idx]->prev = entry;
	}
	m_table[idx] = entry;
	m_count++;
	return true;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::update(ang::raw_cstr_t key, T value)
{
	if (m_count > (m_table.size() * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(key);
	if (node == null)
	{
		wsize idx = hash_index_maker<raw_cstr_t>::make(key, m_table.size());
		node = allocate_node<ang::strings::basic_string<E,A>, T>(ang::forward<K>(key), ang::forward<T>(value));
		if (m_table[idx])
		{
			node->next = m_table[idx];
			m_table[idx]->prev = node;
		}
		m_table[idx] = node;
		m_count++;
	}
	else
	{
		node->data.value = ang::move(value);
	}
	return true;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::update(ang::collections::pair<ang::strings::basic_string<E,A>, T> pair)
{
	if (m_count > (m_table.size() * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(pair.key);
	if (node == null)
	{
		wsize idx = index_maker::make(pair.key, m_table.size());
		node = allocate_node<ang::strings::basic_string<E,A>, T>(ang::forward<K>(pair.key), ang::forward<T>(pair.value));
		if (m_table[idx])
		{
			node->next = m_table[idx];
			m_table[idx]->prev = node;
		}
		m_table[idx] = node;
		m_count++;
	}
	else
	{
		node->data.value = ang::move(pair.value);
	}
	return true;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::remove(ang::raw_cstr_t key)
{
	if (m_table.is_empty())
		return false;

	wsize idx = hash_index_maker<raw_cstr_t>::make(key, m_table.size());
	node_ptr_t temp = m_table[idx];

	while (temp != nullptr)
	{
		if (logic_operation<ang::strings::basic_string<E,A>, ang::strings::basic_string<E,A>, logic_operation_type::same>::operate(temp->data.key, key))
		{
			if (temp->prev)
			{
				temp->prev->next = temp->next;
				if (temp->next)
					temp->next->prev = temp->prev;
			}
			else
			{
				m_table[idx] = temp->next;
				if (temp->next)
					temp->next->prev = null;
			}
			m_alloc.template destroy<node_t>(temp);
			m_alloc.deallocate(temp);
			m_count--;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::remove(ang::raw_cstr_t key, T& value)
{
	if (m_table.is_empty())
		return false;

	wsize idx = hash_index_maker<raw_cstr_t>::make(key, m_table.size());
	node_ptr_t temp = m_table[idx];

	while (temp != nullptr)
	{
		if (logic_operation<ang::strings::basic_string<E,A>, ang::strings::basic_string<E,A>, logic_operation_type::same>::operate(temp->data.key, key))
		{
			if (temp->prev)
			{
				temp->prev->next = temp->next;
				if (temp->next)
					temp->next->prev = temp->prev;
			}
			else
			{
				m_table[idx] = temp->next;
				if (temp->next)
					temp->next->prev = null;
			}
			value = ang::move(temp->data.value);
			m_alloc.template destroy<node_t>(temp);
			m_alloc.deallocate(temp);
			m_count--;
			return true;
		}
		temp = temp->next;
	}
	return false;
}


template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::remove(ang::collections::base_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>>)
{
	return false;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::remove(ang::collections::base_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>>, T& value)
{

	return false;
}


template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline bool ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::has_key(ang::raw_cstr_t key)const
{
	return find_node(key) != null;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::find(ang::raw_cstr_t key)
{
	if (m_table.is_empty())
		return iterator_t(const_cast<hash_map_object*>(this));

	wsize idx = hash_index_maker<raw_cstr_t>::make(key, m_table.size());
	node_ptr_t temp = m_table[idx];
	while (temp != null)
	{
		if (logic_operation<ang::strings::basic_string<E,A>, ang::strings::basic_string<E,A>, logic_operation_type::same>::operate(key, temp->data.key))
			return iterator_t(const_cast<hash_map_object*>(this), temp, idx);
		temp = temp->next;
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}


template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename>class hash_index_maker>
inline ang::collections::const_iterator<ang::collections::pair<ang::strings::basic_string<E,A>, T>> ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>::find(ang::raw_cstr_t key)const
{
	if (m_table.is_empty())
		return iterator_t(const_cast<hash_map_object*>(this));

	wsize idx = hash_index_maker<raw_cstr_t>::make(key, m_table.size());
	node_ptr_t temp = m_table[idx];
	while (temp != null)
	{
		if (logic_operation<ang::strings::basic_string<E,A>, ang::strings::basic_string<E,A>, logic_operation_type::same>::operate(key, temp->data.key))
			return iterator_t(const_cast<hash_map_object*>(this), temp, idx);
		temp = temp->next;
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::object_wrapper()
	: m_ptr(null)
{
	set(new ang::collections::hash_map_object<K, T, allocator, hash_index_maker>());
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::object_wrapper(type* ptr)
	: m_ptr(null)
{
	set(ptr ? ptr : new ang::collections::hash_map_object<K, T, allocator, hash_index_maker>());
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::object_wrapper(ang::initializer_list<pair_type> items)
	: m_ptr(null)
{
	set(new ang::collections::hash_map_object<K, T, allocator, hash_index_maker>());
	for (pair_type item : items)
	{
		get()->insert(item);
	}
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::object_wrapper(const collections::ienum<pair_type>* store)
	: m_ptr(null)
{
	set(new ang::collections::hash_map_object<K, T, allocator, hash_index_maker>());
	m_ptr->copy(store);
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::object_wrapper(object_wrapper && ptr)
	: m_ptr(null)
{
	if (ptr.is_empty())
	{
		set(new ang::collections::hash_map_object<K, T, allocator, hash_index_maker>());
	}
	else
	{
		auto temp = ptr.m_ptr;
		ptr.m_ptr = null;
		m_ptr = temp;
	
	}
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::object_wrapper(object_wrapper const& ptr)
	: m_ptr(null)
{
	set(new ang::collections::hash_map_object<K, T, allocator, hash_index_maker>());
	m_ptr->copy(ptr.get());
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::object_wrapper(ang::nullptr_t const&)
	: m_ptr(null)
{

}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::~object_wrapper()
{
	reset();
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
void ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::reset()
{
	if (m_ptr != null) 
		m_ptr->release();
	m_ptr = null;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
void ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
bool ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::is_empty()const
{
	return m_ptr == null;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker>* ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::get(void)const
{
	return m_ptr;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
void ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::set(type* ptr)
{
	auto temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker>** ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::addres_of(void)
{
	return &m_ptr;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker>** ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::addres_for_init(void)
{
	reset();
	return &m_ptr;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator = (type* ptr)
{
	set(ptr);
	return*this;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator = (const ang::nullptr_t&)
{
	reset();
	return*this;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator = (collections::ienum<pair_type> const* items)
{
	if (is_empty())
		set(new ang::collections::hash_map_object<K, T, allocator, hash_index_maker>());
	m_ptr->copy(items);
	return*this;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator = (object_wrapper && ptr)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator = (object_wrapper const& ptr)
{
	set(ptr.get());
	return*this;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator += (pair_type item)
{
	if (is_empty())
		set(new ang::collections::hash_map_object<K, T, allocator, hash_index_maker>());
	m_ptr->push(item);
	return*this;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator += (collections::ienum<pair_type> const* items)
{
	if (is_empty())
		set(new ang::collections::hash_map_object<K, T, allocator, hash_index_maker>());
	m_ptr->extend(item);
	return*this;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator += (object_wrapper const& ptr)
{
	if (is_empty())
		set(new ang::collections::hash_map_object<K, T, allocator, hash_index_maker>());
	m_ptr->extend(ptr.get());
	return*this;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper_ptr<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>> ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator & (void)
{
	return this;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker> * ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator -> (void)
{
	return get();
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::collections::hash_map_object<K, T, allocator, hash_index_maker> const* ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator -> (void)const
{
	return get();
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator ang::collections::hash_map_object<K, T, allocator, hash_index_maker> * (void)
{
	return get();
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator ang::collections::hash_map_object<K, T, allocator, hash_index_maker> const* (void)const
{
	return get();
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
typename ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::data_type& ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator [] (key_type const& key)
{
	if (is_empty()) set(new collections::hash_map_object<K, T, allocator, hash_index_maker>());
	auto it = m_ptr->find(key);
	if (!it.is_valid()) {
		m_ptr->insert(key, T());
		it = m_ptr->find(key);
	}
	return it->value;
}

template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
typename ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::data_type ang::object_wrapper<ang::collections::hash_map_object<K, T, allocator, hash_index_maker>>::operator [] (key_type const& key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = m_ptr->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::object_wrapper()
	: m_ptr(null)
{
	set(new ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>());
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::object_wrapper(type* ptr)
	: m_ptr(null)
{
	set(ptr ? ptr : new ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>());
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::object_wrapper(ang::initializer_list<pair_type> items)
	: m_ptr(null)
{
	set(new ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>());
	for (pair_type item : items)
	{
		get()->insert(item);
	}
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::object_wrapper(const collections::ienum<pair_type>* store)
	: m_ptr(null)
{
	set(new ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>());
	m_ptr->copy(store);
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::object_wrapper(object_wrapper && ptr)
	: m_ptr(null)
{
	if (ptr.is_empty())
	{
		set(new ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>());
	}
	else
	{
		auto temp = ptr.m_ptr;
		ptr.m_ptr = null;
		m_ptr = temp;

	}
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::object_wrapper(object_wrapper const& ptr)
	: m_ptr(null)
{
	set(new ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>());
	m_ptr->copy(ptr.get());
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::object_wrapper(ang::nullptr_t const&)
	: m_ptr(null)
{

}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::~object_wrapper()
{
	reset();
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
void ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::reset()
{
	if (m_ptr != null)
		m_ptr->release();
	m_ptr = null;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
void ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::reset_unsafe()
{
	m_ptr = null;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
bool ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::is_empty()const
{
	return m_ptr == null;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>* ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::get(void)const
{
	return m_ptr;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
void ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::set(type* ptr)
{
	auto temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>** ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::addres_of(void)
{
	return &m_ptr;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>** ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::addres_for_init(void)
{
	reset();
	return &m_ptr;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator = (type* ptr)
{
	set(ptr);
	return*this;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator = (const ang::nullptr_t&)
{
	reset();
	return*this;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator = (collections::ienum<pair_type> const* items)
{
	if (is_empty())
		set(new ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>());
	m_ptr->copy(items);
	return*this;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator = (object_wrapper && ptr)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator = (object_wrapper const& ptr)
{
	set(ptr.get());
	return*this;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator += (pair_type item)
{
	if (is_empty())
		set(new ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>());
	m_ptr->push(item);
	return*this;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator += (collections::ienum<pair_type> const* items)
{
	if (is_empty())
		set(new ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>());
	m_ptr->extend(item);
	return*this;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>& ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator += (object_wrapper const& ptr)
{
	if (is_empty())
		set(new ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>());
	m_ptr->extend(ptr.get());
	return*this;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper_ptr<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>> ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator & (void)
{
	return this;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker> * ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator -> (void)
{
	return get();
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker> const* ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator -> (void)const
{
	return get();
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker> * (void)
{
	return get();
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker> const* (void)const
{
	return get();
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
typename ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::data_type& ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator [] (ang::raw_cstr_t const& key)
{
	if (is_empty()) set(new collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>());
	auto it = m_ptr->find(key);
	if (!it.is_valid()) {
		m_ptr->insert(key, T());
		it = m_ptr->find(key);
	}
	return it->value;
}

template<ang::text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
typename ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::data_type ang::object_wrapper<ang::collections::hash_map_object<ang::strings::basic_string<E,A>, T, allocator, hash_index_maker>>::operator [] (ang::raw_cstr_t const& key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = m_ptr->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value;
}



#endif//__ANG_COLLECTIONS_HASH_MAP_INL__
