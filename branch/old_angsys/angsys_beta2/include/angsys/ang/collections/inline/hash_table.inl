#ifndef __ANG_HASH_TABLE_HPP__
#error Can't include hash_table.inl, please include hash_table.h inside
#elif !defined __ANG_HASH_TABLE_INL__
#define __ANG_HASH_TABLE_INL__


ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION_TEMPLATE(ang::collections::hash_table_node, K, T);

template<typename K, typename T, template<typename> class allocator>
ang::collections::hash_table_object<K, T, allocator>::hash_table_object()
	: _size(0)
	, _capacity(0)
	, _table(0)
{
	_table = allocator<node_ptr_t>::alloc(13);
	if (_table)
	{
		_capacity = 13;
		memset(_table, 0, sizeof(node_ptr_t) * 13);
	}
}

template<typename K, typename T, template<typename> class allocator>
ang::collections::hash_table_object<K, T, allocator>::hash_table_object(ang::initializer_list_t<ang::collections::pair<K, T>> list)
	: hash_table_object()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<typename K, typename T, template<typename> class allocator>
ang::collections::hash_table_object<K, T, allocator>::hash_table_object(const ang::nullptr_t&)
	: hash_table_object()
{
}

template<typename K, typename T, template<typename> class allocator>
ang::collections::hash_table_object<K, T, allocator>::hash_table_object(hash_table_object&& other)
	: _size(0)
	, _capacity(0)
	, _table(0)
{
	_size = other._size;
	_capacity = other._capacity;
	_table = other._table;

	other._size = 0;
	other._capacity = 0;
	other._table = null;
}

template<typename K, typename T, template<typename> class allocator>
ang::collections::hash_table_object<K, T, allocator>::hash_table_object(const hash_table_object& other)
	: hash_table_object()
{
	extend(&other);
}

template<typename K, typename T, template<typename> class allocator>
ang::collections::hash_table_object<K, T, allocator>::hash_table_object(const hash_table_object* other)
	: hash_table_object()
{
	extend(other);
}

template<typename K, typename T, template<typename> class allocator>
ang::collections::hash_table_object<K, T, allocator>::hash_table_object( ang::collections::ienum<ang::collections::pair<K,T>> const* other)
	: hash_table_object()
{
	extend(other);
}

template<typename K, typename T, template<typename> class allocator>
ang::collections::hash_table_object<K, T, allocator>::~hash_table_object()
{
	clean();
}


template<typename K, typename T, template<typename> class allocator>
inline ang::type_name_t ang::collections::hash_table_object<K, T, allocator>::class_name()
{
	string _class_name;
	static type_name_t name = runtime_data_base::regist_typename(ang::move(_class_name << "ang::collections::hash_table<"_s << args_list_type_of<K, T>() << ">"_s));
	return name;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::type_name_t ang::collections::hash_table_object<K, T, allocator>::object_name()const
{
	return ang::collections::hash_table_object<K, T, allocator>::class_name();
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::is_inherited_of(ang::type_name_t name)
{
	return name == type_of<hash_table_object< K, T, allocator>>()
		|| ang::object::is_inherited_of(name)
		|| ang::collections::imap<K, T>::is_inherited_of(name);
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::is_kind_of(ang::type_name_t name)const
{
	return name == type_of<hash_table_object<K, T, allocator>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::imap<K, T>::is_kind_of(name);
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_of<hash_table_object<K, T, allocator>>())
	{
		*out = static_cast<ang::collections::hash_table_object<K, T, allocator>*>(this);
		return true;
	}
	else if (ang::object::query_object(name, out))
	{
		return true;
	}
	else if (ang::collections::imap<K,T>::query_object(name, out))
	{
		return true;
	}
	return false;
}


template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::is_empty()const {
	return _size == 0;
}

template<typename K, typename T, template<typename> class allocator>
inline void ang::collections::hash_table_object<K, T, allocator>::clean() {
	if (_table)
	{
		for (wsize i = 0; i < _capacity && _size > 0; ++i)
		{
			node_ptr_t temp = _table[i];
			_table[i] = null;
			while (temp)
			{
				node_ptr_t del = temp;
				temp = temp->next;
				allocator<node_t>::destruct_and_free(del);
				_size--;
			}
		}
	}
	_capacity = 0;
	_size = 0;
	if (_table)allocator<node_ptr_t>::free(_table);
	_table = null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::move(hash_table_object& other)
{
	if (this == &other)
		return false;
	clean();
	_size = other._size;
	_capacity = other._capacity;
	_table = other._table;
	other._size = 0;
	other._capacity = 0;
	other._table = null;
	return true;
}


template<typename K, typename T, template<typename> class allocator>
void ang::collections::hash_table_object<K, T, allocator>::increase_capacity()
{
	auto new_size = hash_table_get_next_size(_capacity);
	node_ptr_t* new_data = allocator<node_ptr_t>::alloc(new_size);
	memset(new_data, 0, sizeof(node_ptr_t) * new_size);

	if (_table && _capacity && _size)
	{
		wsize s = _size;
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

template<typename K, typename T, template<typename> class allocator>
inline wsize ang::collections::hash_table_object<K, T, allocator>::hash_index(K const& key)const
{
	return create_hash_index(key, _capacity ? _capacity : 1);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::hash_table_node<K, T>* ang::collections::hash_table_object<K, T, allocator>::find_node(K const& key)const
{
	if (_capacity == 0)
		return null;

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (comparision_operations<K,K>::template compare<comparision_same>(key, (raw_str_t)temp->pair.key))
			return temp;
		temp = temp->next;
	}
	return null;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::pair<K,T>& ang::collections::hash_table_object<K, T, allocator>::at(const ang::collections::base_iterator<ang::collections::pair<K,T>>& it)
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

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<ang::collections::pair<K,T>> ang::collections::hash_table_object<K, T, allocator>::begin()
{
	for (auto i = 0U; i < _capacity; ++i)
	{
		if(_table[i] != null)
			return iterator_t(const_cast<hash_table_object*>(this), _table[i], i);
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_table_object<K, T, allocator>::end()
{
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_table_object<K, T, allocator>::begin()const
{
	for (auto i = 0U; i < _capacity; ++i)
	{
		if (_table[i] != null)
			return iterator_t(const_cast<hash_table_object*>(this), _table[i], i);
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_table_object<K, T, allocator>::last()
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_table_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_table_object<K, T, allocator>::last()const
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_table_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::hash_table_object<K, T, allocator>::end()const
{
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::backward_iterator<ang::collections::pair<K,T>> ang::collections::hash_table_object<K, T, allocator>::rbegin()
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_table_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::backward_iterator<ang::collections::pair<K, T>> ang::collections::hash_table_object<K, T, allocator>::rend()
{
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_backward_iterator<ang::collections::pair<K, T>> ang::collections::hash_table_object<K, T, allocator>::rbegin()const
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_table_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_backward_iterator<ang::collections::pair<K, T>> ang::collections::hash_table_object<K, T, allocator>::rend()const
{
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}


template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::increase(ang::collections::base_iterator<ang::collections::pair<K, T>>& it)const
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

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::increase(ang::collections::base_iterator<ang::collections::pair<K,T>>& it, int offset)const
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

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::decrease(ang::collections::base_iterator<ang::collections::pair<K,T>>& it)const
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

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::decrease(ang::collections::base_iterator<ang::collections::pair<K,T>>& it, int offset)const
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

template<typename K, typename T, template<typename> class allocator>
inline wsize ang::collections::hash_table_object<K, T, allocator>::counter()const
{
	return _size;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::copy(ang::collections::ienum<ang::collections::pair<K,T>>const* items) {

	if (items == null)
		return false;
	clean();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename K, typename T, template<typename> class allocator>
inline void ang::collections::hash_table_object<K, T, allocator>::extend(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::insert(K key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find_node(key) != null)
		return false;
	wsize idx = hash_index(key);
	node_ptr_t entry = allocator<node_t>::template alloc_and_construct<K&&,T&&>(
		ang::move(key),
		ang::move(value)
	);
	
	if (_table[idx])
	{
		entry->next = _table[idx];
		_table[idx]->prev = entry;
	}
	_table[idx] = entry;
	_size++;
	return true;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::insert(ang::collections::pair<K,T> pair)
{
	if (_size > (_capacity * 0.75))
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
	_size++;
	return true;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::update(K key, T value)
{
	if (_size > (_capacity * 0.75))
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
		_size++;
	}
	else
	{
		node->pair.value = ang::move(value);
	}
	return true;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::update(ang::collections::pair<K,T> pair)
{
	if (_size > (_capacity * 0.75))
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
		_size++;
	}
	else
	{
		node->pair.value = ang::move(pair.value);
	}
	return true;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::remove(K const& key)
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
			_size--;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::remove(K const& key, T& value)
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
			_size--;
			return true;
		}
		temp = temp->next;
	}
	return false;
}


template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::remove(ang::collections::base_iterator<ang::collections::pair<K, T>>)
{
	return false;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::remove(ang::collections::base_iterator<ang::collections::pair<K, T>>, T& value)
{
	
	return false;
}


template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<K, T, allocator>::has_key(const K& key)const
{
	return find_node(key) != null;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::hash_table_object<K, T, allocator>::find(const K& key)
{
	if (_table == null)
		return iterator_t(const_cast<hash_table_object*>(this));

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (comparision_operations<K, K>::template compare<comparision_same>(key, (raw_str_t)temp->pair.key))
			return iterator_t(const_cast<hash_table_object*>(this), temp, idx);
		temp = temp->next;
	}	
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}


template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_iterator<ang::collections::pair<K, T>> ang::collections::hash_table_object<K, T, allocator>::find(const K& key)const
{
	if (_table == null)
		return iterator_t(const_cast<hash_table_object*>(this));

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (comparision_operations<K, K>::template compare<comparision_same>(key, (raw_str_t)temp->pair.key))
			return iterator_t(const_cast<hash_table_object*>(this), temp, idx);
		temp = temp->next;
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}


////////////////////////////////////////////////////////////////////////////////////////


template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::object_wrapper()
	: _ptr(null)
{

}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{

}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::object_wrapper(ang::collections::hash_table_object<K, T, allocator>* ptr)
	: object_wrapper()
{
	set(ptr);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::object_wrapper(ang::collections::ienum<ang::collections::pair<K, T>> const* store)
	: object_wrapper()
{
	set(new collections::hash_table_object<K, T, allocator>(store));
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::object_wrapper(ang::initializer_list_t<ang::collections::pair<K, T>> list)
{
	set(new collections::hash_table_object<K, T, allocator>(list));
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>> && other)
{
	collections::hash_table_object<K, T, allocator> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>> const& other)
{
	set(other.get());
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::~object_wrapper()
{
	clean();
}

template<typename K, typename T, template<typename> class allocator>
inline void ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::is_empty()const
{
	return _ptr == null;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::hash_table_object<K, T, allocator>* ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::get(void)const
{
	return _ptr;
}

template<typename K, typename T, template<typename> class allocator>
inline void ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::set(ang::collections::hash_table_object<K, T, allocator>* ptr)
{
	collections::hash_table_object<K, T, allocator> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::hash_table_object<K, T, allocator> ** ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::addres_of(void)
{
	return &_ptr;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator = (ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>> && other)
{
	ang::collections::hash_table_object<K, T, allocator> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator = (ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator = (ang::collections::hash_table_object<K, T, allocator>* ptr)
{
	set(ptr);
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator = (ang::collections::ienum<ang::collections::pair<K, T>> const* store)
{
	if (is_empty())
		set(new collections::hash_table_object<K, T, allocator>(store));
	else
		_ptr->copy(store);
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator += (ang::collections::pair<K, T> pair)
{
	if (is_empty())
		set(new collections::hash_table_object<K, T, allocator>());
	_ptr->update(ang::move(pair));
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator ang::object_t()const {
	return _ptr;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper_ptr<ang::collections::hash_table_object<K, T, allocator>> ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator & (void)
{
	return this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::hash_table_object<K, T, allocator> * ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator -> (void)
{
	return get();
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::hash_table_object<K, T, allocator> const* ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator -> (void)const
{
	return get();
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator ang::collections::hash_table_object<K, T, allocator> * (void)
{
	return get();
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator ang::collections::hash_table_object<K, T, allocator> const* (void)const
{
	return get();
}

template<typename K, typename T, template<typename> class allocator>
inline T& ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator [] (K const& key)
{
	if (is_empty()) set(new collections::hash_table_object<K, T, allocator>());
	auto it = _ptr->find(key);
	if (!it.is_valid()) {
		_ptr->insert(key, T());
		it = _ptr->find(key);
	}// throw(exception_t(except_code::array_overflow));
	return it->value;
}

template<typename K, typename T, template<typename> class allocator>
inline T ang::object_wrapper<ang::collections::hash_table_object<K, T, allocator>>::operator [] (K const& key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value;
}


///////////////////////////////////////////////////////////////////////////////////////////////


template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::hash_table_object()
	: _size(0)
	, _capacity(0)
	, _table(0)
{
	_table = allocator<node_ptr_t>::alloc(13);
	if (_table)
	{
		_capacity = 13;
		memset(_table, 0, sizeof(node_ptr_t) * 13);
	}
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::hash_table_object(ang::initializer_list_t<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> list)
	: hash_table_object()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::hash_table_object(const ang::nullptr_t&)
	: hash_table_object()
{
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::hash_table_object(hash_table_object&& other)
	: _size(0)
	, _capacity(0)
	, _table(0)
{
	_size = other._size;
	_capacity = other._capacity;
	_table = other._table;

	other._size = 0;
	other._capacity = 0;
	other._table = null;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::hash_table_object(const hash_table_object& other)
	: hash_table_object()
{
	extend(&other);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::hash_table_object(const hash_table_object* other)
	: hash_table_object()
{
	extend(other);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::hash_table_object(ang::collections::ienum<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> const* other)
	: hash_table_object()
{
	extend(other);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::~hash_table_object()
{
	clean();
}


template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::type_name_t ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::class_name()
{
	string _class_name;
	static type_name_t name = runtime_data_base::regist_typename(ang::move(_class_name << "ang::collections::hash_table<"_s << args_list_type_of<ang::strings::string_base<ENCODING>, T>() << ">"_s));
	return name;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::type_name_t ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::object_name()const
{
	return ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::class_name();
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::is_inherited_of(ang::type_name_t name)
{
	return name == type_of<hash_table_object< ang::strings::string_base<ENCODING>, T, allocator>>()
		|| ang::object::is_inherited_of(name)
		|| ang::collections::imap<ang::strings::string_base<ENCODING>, T>::is_inherited_of(name);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::is_kind_of(ang::type_name_t name)const
{
	return name == type_of<hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::imap<ang::strings::string_base<ENCODING>, T>::is_kind_of(name);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_of<hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>())
	{
		*out = static_cast<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>*>(this);
		return true;
	}
	else if (ang::object::query_object(name, out))
	{
		return true;
	}
	else if (ang::collections::imap<ang::strings::string_base<ENCODING>, T>::query_object(name, out))
	{
		return true;
	}
	return false;
}


template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::is_empty()const {
	return _size == 0;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline void ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::clean() {
	if (_table)
	{
		for (wsize i = 0; i < _capacity && _size > 0; ++i)
		{
			node_ptr_t temp = _table[i];
			_table[i] = null;
			while (temp)
			{
				node_ptr_t del = temp;
				temp = temp->next;
				allocator<node_t>::destruct_and_free(del);
				_size--;
			}
		}
	}
	_capacity = 0;
	_size = 0;
	if (_table)allocator<node_ptr_t>::free(_table);
	_table = null;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::move(hash_table_object& other)
{
	if (this == &other)
		return false;
	clean();
	_size = other._size;
	_capacity = other._capacity;
	_table = other._table;
	other._size = 0;
	other._capacity = 0;
	other._table = null;
	return true;
}


template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
void ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::increase_capacity()
{
	wsize new_size = hash_table_get_next_size(_capacity);
	node_ptr_t* new_data = allocator<node_ptr_t>::alloc(new_size);
	memset(new_data, 0, sizeof(node_ptr_t) * new_size);

	if (_table && _capacity && _size)
	{
		wsize s = _size;
		for (wsize i = 0; i < _capacity && s > 0; ++i) {
			node_ptr_t temp = _table[i];
			_table[i] = null;
			while (temp) {
				windex idx = create_hash_index((raw_str_t)temp->pair.key, new_size);
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

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline wsize ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::hash_index(ang::raw_str_t key)const
{
	return create_hash_index(key, _capacity ? _capacity : 1);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::hash_table_node<ang::strings::string_base<ENCODING>, T>* ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::find_node(ang::raw_str_t key)const
{
	if (_capacity == 0)
		return null;

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (comparision_operations<raw_str_t, raw_str_t>::template compare<comparision_same>(key, (raw_str_t)temp->pair.key))
			return temp;
		temp = temp->next;
	}
	return null;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::pair<ang::strings::string_base<ENCODING>, T>& ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::at(const ang::collections::base_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>>& it)
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

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::begin()
{
	for (auto i = 0U; i < _capacity; ++i)
	{
		if (_table[i] != null)
			return iterator_t(const_cast<hash_table_object*>(this), _table[i], i);
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::end()
{
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::const_forward_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::begin()const
{
	for (auto i = 0U; i < _capacity; ++i)
	{
		if (_table[i] != null)
			return iterator_t(const_cast<hash_table_object*>(this), _table[i], i);
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::last()
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_table_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::const_forward_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::last()const
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_table_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::const_forward_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::end()const
{
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::backward_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::rbegin()
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_table_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::backward_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::rend()
{
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::const_backward_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::rbegin()const
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next != null)
				node = node->next;
			return iterator_t(const_cast<hash_table_object*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::const_backward_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::rend()const
{
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}


template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::increase(ang::collections::base_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>>& it)const
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

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::increase(ang::collections::base_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>>& it, int offset)const
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

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::decrease(ang::collections::base_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>>& it)const
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

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::decrease(ang::collections::base_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>>& it, int offset)const
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

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline wsize ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::counter()const
{
	return _size;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::copy(ang::collections::ienum<ang::collections::pair<ang::strings::string_base<ENCODING>, T>>const* items) {

	if (items == null)
		return false;
	clean();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline void ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::extend(ang::collections::ienum<ang::collections::pair<ang::strings::string_base<ENCODING>, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::insert(ang::strings::string_base<ENCODING> key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find_node((raw_str_t)key) != null)
		return false;
	wsize idx = hash_index((raw_str_t)key);
	node_ptr_t entry = allocator<node_t>::template alloc_and_construct<ang::strings::string_base<ENCODING>&&, T&&>(
		ang::move(key),
		ang::move(value)
		);

	if (_table[idx])
	{
		entry->next = _table[idx];
		_table[idx]->prev = entry;
	}
	_table[idx] = entry;
	_size++;
	return true;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::insert(ang::collections::pair<ang::strings::string_base<ENCODING>, T> pair)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find_node((raw_str_t)pair.key) != null)
		return false;
	wsize idx = hash_index((raw_str_t)pair.key);
	node_ptr_t entry = allocator<node_t>::template alloc_and_construct<ang::strings::string_base<ENCODING>&&, T&&>(
		ang::move(pair.key),
		ang::move(pair.value)
		);
	if (_table[idx])
	{
		entry->next = _table[idx];
		_table[idx]->prev = entry;
	}
	_table[idx] = entry;
	_size++;
	return true;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::update(ang::strings::string_base<ENCODING> key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node((raw_str_t)key);
	if (node == null)
	{
		wsize idx = hash_index((raw_str_t)key);
		node = allocator<node_t>::template alloc_and_construct<ang::strings::string_base<ENCODING>&&, T&&>(
			ang::move(key),
			ang::move(value)
			);
		if (_table[idx])
		{
			node->next = _table[idx];
			_table[idx]->prev = node;
		}
		_table[idx] = node;
		_size++;
	}
	else
	{
		node->pair.value = ang::move(value);
	}
	return true;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::update(ang::collections::pair<ang::strings::string_base<ENCODING>, T> pair)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node((raw_str_t)pair.key);
	if (node == null)
	{
		wsize idx = hash_index((raw_str_t)pair.key);
		node = allocator<node_t>::template alloc_and_construct<ang::strings::string_base<ENCODING>&&, T&&>(
			ang::move(pair.key),
			ang::move(pair.value)
			);
		if (_table[idx])
		{
			node->next = _table[idx];
			_table[idx]->prev = node;
		}
		_table[idx] = node;
		_size++;
	}
	else
	{
		node->pair.value = ang::move(pair.value);
	}
	return true;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::remove(ang::raw_str_t key)
{
	if (_table == null)
		return false;

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (comparision_operations<raw_str_t, raw_str_t>::template compare<comparision_same>((raw_str_t)temp->pair.key, key))
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
			_size--;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::remove(ang::raw_str_t key, T& value)
{
	if (_table == null)
		return false;

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (comparision_operations<raw_str_t, raw_str_t>::template compare<comparision_same>((raw_str_t)temp->pair.key, key))
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
			_size--;
			return true;
		}
		temp = temp->next;
	}
	return false;
}


template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::remove(ang::collections::base_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>>)
{
	return false;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::remove(ang::collections::base_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>>, T& value)
{

	return false;
}


template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::has_key(ang::raw_str_t key)const
{
	return find_node(key) != null;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::find(ang::raw_str_t key)
{
	if (_table == null)
		return iterator_t(const_cast<hash_table_object*>(this));

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (comparision_operations<raw_str_t, raw_str_t>::template compare<comparision_same>(key, (raw_str_t)temp->pair.key))
			return iterator_t(const_cast<hash_table_object*>(this), temp, idx);
		temp = temp->next;
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}


template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::const_iterator<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>::find(raw_str_t key)const
{
	if (_table == null)
		return iterator_t(const_cast<hash_table_object*>(this));

	wsize idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (comparision_operations<raw_str_t, raw_str_t>::template compare<comparision_same>(key, (raw_str_t)temp->pair.key))
			return iterator_t(const_cast<hash_table_object*>(this), temp, idx);
		temp = temp->next;
	}
	return iterator_t(const_cast<hash_table_object*>(this), null, 0);
}


////////////////////////////////////////////////////////////////////////////////////////


template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::object_wrapper()
	: _ptr(null)
{

}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{

}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::object_wrapper(ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>* ptr)
	: object_wrapper()
{
	set(ptr);
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::object_wrapper(ang::collections::ienum<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> const* store)
	: object_wrapper()
{
	set(new collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>(store));
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::object_wrapper(ang::initializer_list_t<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> list)
{
	set(new collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>(list));
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>> && other)
{
	collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>> const& other)
{
	set(other.get());
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::~object_wrapper()
{
	clean();
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline void ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline bool ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::is_empty()const
{
	return _ptr == null;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>* ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::get(void)const
{
	return _ptr;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline void ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::set(ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>* ptr)
{
	collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator> ** ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::addres_of(void)
{
	return &_ptr;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator = (ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>> && other)
{
	ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator = (ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>> const& other)
{
	set(other._ptr);
	return*this;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator = (ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>* ptr)
{
	set(ptr);
	return*this;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator = (ang::collections::ienum<ang::collections::pair<ang::strings::string_base<ENCODING>, T>> const* store)
{
	if (is_empty())
		set(new collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>(store));
	else
		_ptr->copy(store);
	return*this;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>& ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator += (ang::collections::pair<ang::strings::string_base<ENCODING>, T> pair)
{
	if (is_empty())
		set(new collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>());
	_ptr->update(ang::move(pair));
	return*this;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator ang::object_t()const {
	return _ptr;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper_ptr<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>> ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator & (void)
{
	return this;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator> * ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator -> (void)
{
	return get();
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator> const* ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator -> (void)const
{
	return get();
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator> * (void)
{
	return get();
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator> const* (void)const
{
	return get();
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator> template<typename K>
inline T& ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator [] (ang::safe_str<K> const& key)
{
	if (is_empty()) set(new collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>());
	auto it = _ptr->find(key);
	if (!it.is_valid()) {
		_ptr->insert(key, T());
		it = _ptr->find(key);
	}// throw(exception_t(except_code::array_overflow));
	return it->value;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator> template<typename K, wsize N>
inline T& ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator [] (const K(&key_)[N])
{
	safe_str<const K> key = key_;
	if (is_empty()) set(new collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>());
	auto it = _ptr->find(key);
	if (!it.is_valid()) {
		_ptr->insert(key, T());
		it = _ptr->find(key);
	}// throw(exception_t(except_code::array_overflow));
	return it->value;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator> template<typename K>
inline T ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator [] (ang::safe_str<K> const& key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value;
}

template<ang::text::encoding_enum ENCODING, typename T, template<typename> class allocator> template<typename K, wsize N>
inline T ang::object_wrapper<ang::collections::hash_table_object<ang::strings::string_base<ENCODING>, T, allocator>>::operator [] (const K(&key_)[N])const
{
	safe_str<const K> key = key_;
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value;
}


///////////////////////////////////////////////////////////////////////////////////////////////





#endif//__ANG_HASH_TABLE_INL__