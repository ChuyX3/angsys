#ifndef __ANG_COLLECTIONS_HASH_MAP_H__
#error Can't include HASH_MAP.inl, please include hash_map.h inside
#elif !defined __ANG_COLLECTIONS_HASH_MAP_INL__
#define __ANG_COLLECTIONS_HASH_MAP_INL__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// hash_map_object<K, T, allocator, maker> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
ang::collections::hash_map_object<K, T, allocator, maker>::hash_map_object()
	: _count(0)
	, _table()
{
	_table.allocate(13);
	if (!_table.is_empty())
	{
		memset((void*)_table.get(), 0, sizeof(node_ptr_t) * _table.size());
	}
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker> template<typename U, typename V>
ang::collections::hash_map_object<K, T, allocator, maker>::hash_map_object(ang::initializer_list<ang::collections::pair<U, V>> list)
	: hash_map_object()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert((K)it->key, (T)it->value);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
ang::collections::hash_map_object<K, T, allocator, maker>::hash_map_object(const ang::nullptr_t&)
	: hash_map_object()
{
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
ang::collections::hash_map_object<K, T, allocator, maker>::hash_map_object(hash_map_object&& other)
	: _count(0)
	, _table(0)
{
	_count = other._count;
	_table.move(other._table);

	other._count = 0;
	other._table = null;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
ang::collections::hash_map_object<K, T, allocator, maker>::hash_map_object(const hash_map_object& other)
	: hash_map_object()
{
	extend(&other);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
ang::collections::hash_map_object<K, T, allocator, maker>::hash_map_object(const hash_map_object* other)
	: hash_map_object()
{
	extend(other);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
ang::collections::hash_map_object<K, T, allocator, maker>::hash_map_object(ang::collections::ienum<ang::collections::pair<K, T>> const* other)
	: hash_map_object()
{
	extend(other);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
ang::collections::hash_map_object<K, T, allocator, maker>::~hash_map_object()
{
	clear();
}


template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline const ang::rtti_t& ang::collections::hash_map_object<K, T, allocator, maker>::class_info()
{
	static const cstr_view<char> name = strings::string_pool::instance()->save_string((((string("ang::collections::hash_map_object<"_s) += rtti::type_of<K>().type_name()) += ","_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<interface>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::core::delegates::ifunction<T(Ts...)>), alignof(wsize), parents, &default_query_interface);
	return info;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline const ang::rtti_t& ang::collections::hash_map_object<K, T, allocator, maker>::runtime_info()const
{
	return class_info();
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::query_interface(const ang::rtti_t& id, ang::unknown_ptr_t out)
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


template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::is_empty()const {
	return _count == 0;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline void ang::collections::hash_map_object<K, T, allocator, maker>::clear() {
	if (_table)
	{
		for (wsize i = 0; i < _capacity && _count > 0; ++i)
		{
			node_ptr_t temp = _table[i];
			_table[i] = null;
			while (temp)
			{
				node_ptr_t del = temp;
				temp = temp->next;
				allocator<node_t>::destruct_and_free(del);
				_count--;
			}
		}
	}
	_capacity = 0;
	_count = 0;
	if (_table)allocator<node_ptr_t>::free(_table);
	_table = null;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::move(hash_map_object& other)
{
	if (this == &other)
		return false;
	clear();
	_count = other._count;
	_capacity = other._capacity;
	_table = other._table;
	other._count = 0;
	other._capacity = 0;
	other._table = null;
	return true;
}


template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
void ang::collections::hash_map_object<K, T, allocator, maker>::increase_capacity()
{
	auto new_size = hash_table_get_next_size(_capacity);
	node_ptr_t* new_data = allocator<node_ptr_t>::alloc(new_size);
	memset(new_data, 0, sizeof(node_ptr_t) * new_size);

	if (_table && _capacity && _count)
	{
		wsize s = _count;
		for (wsize i = 0; i < _capacity && s > 0; ++i) {
			node_ptr_t temp = _table[i];
			_table[i] = null;
			while (temp) {
				windex idx = create_hash_index(temp->pair.key, new_size);
				node_ptr_t entry = temp;
				temp = temp->next;
				entry->next = new_data[idx];
				if (new_data[idx])new_data[idx]->prev = entry;
				new_data[idx] = entry;
				s--;
			}
		}
		allocator<node_ptr_t>::free(_table);
	}
	_capacity = new_size;
	_table = new_data;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline wsize ang::collections::hash_map_object<K, T, allocator, maker>::hash_index(K const& key)const
{
	return create_hash_index(key, _capacity ? _capacity : 1);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::hash_table_node<K, T>* ang::collections::hash_map_object<K, T, allocator, maker>::find_node(K const& key)const
{
	if (_capacity == 0)
		return null;

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (comparision_operations<K, K>::template compare<comparision_same>(key, (raw_str_t)temp->pair.key))
			return temp;
		temp = temp->next;
	}
	return null;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::pair<K, T>& ang::collections::hash_map_object<K, T, allocator, maker>::at(const ang::collections::base_iterator<ang::collections::pair<K, T>>& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw(exception_t(except_code::invalid_memory));
	if (it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	return node_ptr_t(it.current())->pair;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::begin()
{
	for (auto i = 0U; i < _capacity; ++i)
	{
		if (_table[i] != null)
			return iterator_t(const_cast<hash_map_object*>(this), _table[i], i);
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::end()
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::begin()const
{
	for (auto i = 0U; i < _capacity; ++i)
	{
		if (_table[i] != null)
			return iterator_t(const_cast<hash_map_object*>(this), _table[i], i);
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::last()
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::last()const
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::end()const
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::backward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::rbegin()
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::backward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::rend()
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::const_backward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::rbegin()const
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_map_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::const_backward_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::rend()const
{
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}


template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::increase(ang::collections::base_iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	auto node = node_ptr_t(it.current());

	if (node->next != null) {
		it.current(node->next);
		return true;
	}
	for (auto i = it.offset() + 1; i < _capacity; ++i)
	{
		if (_table[i] != null) {
			it.current(_table[i]);
			it.offset(i);
			return true;
		}
	}
	it.current(null);
	it.offset(0);
	return false;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::increase(ang::collections::base_iterator<ang::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
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
		for (++i; i < _capacity; ++i)
		{
			if (_table[i] != null) {
				node = _table[i];
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

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::decrease(ang::collections::base_iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
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
		if (_table[i] != null) {
			node_ptr_t node = _table[i];
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

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::decrease(ang::collections::base_iterator<ang::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
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
			if (_table[i] != null) {
				node = _table[i];
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

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline wsize ang::collections::hash_map_object<K, T, allocator, maker>::counter()const
{
	return _count;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::copy(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return false;
	clear();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline void ang::collections::hash_map_object<K, T, allocator, maker>::extend(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::insert(K key, T value)
{
	if (_count > (_capacity * 0.75))
		increase_capacity();

	if (find_node(key) != null)
		return false;
	wsize idx = hash_index(key);
	node_ptr_t entry = allocator<node_t>::template alloc_and_construct<K&&, T&&>(
		ang::move(key),
		ang::move(value)
		);

	if (_table[idx])
	{
		entry->next = _table[idx];
		_table[idx]->prev = entry;
	}
	_table[idx] = entry;
	_count++;
	return true;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::insert(ang::collections::pair<K, T> pair)
{
	if (_count > (_capacity * 0.75))
		increase_capacity();

	if (find_node(pair.key) != null)
		return false;
	wsize idx = hash_index(pair.key);
	node_ptr_t entry = allocator<node_t>::template alloc_and_construct<K&&, T&&>(
		ang::move(pair.key),
		ang::move(pair.value)
		);
	if (_table[idx])
	{
		entry->next = _table[idx];
		_table[idx]->prev = entry;
	}
	_table[idx] = entry;
	_count++;
	return true;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::update(K key, T value)
{
	if (_count > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(key);
	if (node == null)
	{
		wsize idx = hash_index(key);
		node = allocator<node_t>::template alloc_and_construct<K&&, T&&>(
			ang::move(key),
			ang::move(value)
			);
		if (_table[idx])
		{
			node->next = _table[idx];
			_table[idx]->prev = node;
		}
		_table[idx] = node;
		_count++;
	}
	else
	{
		node->pair.value = ang::move(value);
	}
	return true;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::update(ang::collections::pair<K, T> pair)
{
	if (_count > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(pair.key);
	if (node == null)
	{
		wsize idx = hash_index(pair.key);
		node = allocator<node_t>::template alloc_and_construct<K&&, T&&>(
			ang::move(pair.key),
			ang::move(pair.value)
			);
		if (_table[idx])
		{
			node->next = _table[idx];
			_table[idx]->prev = node;
		}
		_table[idx] = node;
		_count++;
	}
	else
	{
		node->pair.value = ang::move(pair.value);
	}
	return true;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::remove(K const& key)
{
	if (_table == null)
		return false;

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (comparision_operations<K, K>::template compare<comparision_same>((raw_str_t)temp->pair.key, key))
		{
			if (temp->prev)
			{
				temp->prev->next = temp->next;
				if (temp->next)
					temp->next->prev = temp->prev;
			}
			else
			{
				_table[idx] = temp->next;
				if (temp->next)
					temp->next->prev = null;
			}
			allocator<node_t>::destruct_and_free(temp);
			_count--;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::remove(K const& key, T& value)
{
	if (_table == null)
		return false;

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (comparision_operations<K, K>::template compare<comparision_same>((raw_str_t)temp->pair.key, key))
		{
			if (temp->prev)
			{
				temp->prev->next = temp->next;
				if (temp->next)
					temp->next->prev = temp->prev;
			}
			else
			{
				_table[idx] = temp->next;
				if (temp->next)
					temp->next->prev = null;
			}
			value = ang::move(temp->pair.value);
			allocator<node_t>::destruct_and_free(temp);
			_count--;
			return true;
		}
		temp = temp->next;
	}
	return false;
}


template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::remove(ang::collections::base_iterator<ang::collections::pair<K, T>>)
{
	return false;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::remove(ang::collections::base_iterator<ang::collections::pair<K, T>>, T& value)
{

	return false;
}


template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline bool ang::collections::hash_map_object<K, T, allocator, maker>::has_key(const K& key)const
{
	return find_node(key) != null;
}

template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::find(const K& key)
{
	if (_table == null)
		return iterator_t(const_cast<hash_map_object*>(this));

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (comparision_operations<K, K>::template compare<comparision_same>(key, (raw_str_t)temp->pair.key))
			return iterator_t(const_cast<hash_map_object*>(this), temp, idx);
		temp = temp->next;
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}


template<typename K, typename T, template<typename> class allocator, template<typename>class maker>
inline ang::collections::const_iterator<ang::collections::pair<K, T>> ang::collections::hash_map_object<K, T, allocator, maker>::find(const K& key)const
{
	if (_table == null)
		return iterator_t(const_cast<hash_map_object*>(this));

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (comparision_operations<K, K>::template compare<comparision_same>(key, (raw_str_t)temp->pair.key))
			return iterator_t(const_cast<hash_map_object*>(this), temp, idx);
		temp = temp->next;
	}
	return iterator_t(const_cast<hash_map_object*>(this), null, 0);
}


#endif//__ANG_VECTOR_INL__
