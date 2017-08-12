#ifndef __ANG_HASH_TABLE_H__
#error Can't include hash_table.inl, please include hash_table.h inside
#elif !defined __ANG_HASH_TABLE_HPP__
#define __ANG_HASH_TABLE_HPP__


template<typename K, typename T>
inline ang::type_name_t ang::collections::hash_table_node<K,T>::class_name()
{
	static string _type_name = ang::move("ang::collections::hash_table_node<"_o + type_name<K>() + ","_s + type_name<T>() + ">"_s);
	return (ang::cstr_t)_type_name;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_node<K,T>::is_child_of(ang::type_name_t name)
{
	return name == class_name();
}



template<typename K, typename T>
ang::collections::hash_table_data<K, T>::hash_table_data()
	: _size(0)
	, _capacity(0)
	, _table(0)
	, allocator(ang::memory::allocator_manager::get_allocator(0))
{
	_table = allocator->object_alloc<node_ptr_t>(13);
	if (_table)
	{
		_capacity = 13;
		memset(_table, 0, sizeof(node_ptr_t) * 13);
	}
}

template<typename K, typename T>
ang::collections::hash_table_data<K, T>::hash_table_data(ang::initializer_list_t<ang::collections::pair<K, T>> list)
	: hash_table_data()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<typename K, typename T>
ang::collections::hash_table_data<K, T>::hash_table_data(const ang::nullptr_t&)
	: hash_table_data()
{
}

template<typename K, typename T>
ang::collections::hash_table_data<K, T>::hash_table_data(hash_table_data&& other)
	: _size(0)
	, _capacity(0)
	, _table(0)
	, allocator(ang::memory::allocator_manager::get_allocator(0))
{
	_size = other._size;
	_capacity = other._capacity;
	_table = other._table;
	allocator = other.allocator;

	other._size = 0;
	other._capacity = 0;
	other._table = null;
}

template<typename K, typename T>
ang::collections::hash_table_data<K, T>::hash_table_data(const hash_table_data& other)
	: hash_table_data()
{
	extend(&other);
}

template<typename K, typename T>
ang::collections::hash_table_data<K, T>::hash_table_data(const hash_table_data* other)
	: hash_table_data()
{
	extend(other);
}

template<typename K, typename T>
ang::collections::hash_table_data<K, T>::hash_table_data( ang::collections::ienum<ang::collections::pair<K,T>> const* other)
	: hash_table_data()
{
	extend(other);
}

template<typename K, typename T>
ang::collections::hash_table_data<K, T>::~hash_table_data()
{
	clean();
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<hash_table_data< K, T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::imap<K,T>::is_kind_of(name))
		return true;
	return false;
}

template<typename K, typename T>
inline ang::type_name_t ang::collections::hash_table_data<K, T>::class_name()
{
	static string _type_name = ang::move("ang::collections::hash_table_data<"_o + type_name<K>() + ","_s + type_name<T>() + ">"_s);
	return (ang::cstr_t)_type_name;
}

template<typename K, typename T>
inline ang::type_name_t ang::collections::hash_table_data<K, T>::object_name()const
{
	return ang::collections::hash_table_data<K, T>::class_name();
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<hash_table_data<K, T>>())
	{
		*out = static_cast<ang::collections::hash_table_data<K, T>*>(this);
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


template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::is_empty()const {
	return _size == 0;
}

template<typename K, typename T>
inline void ang::collections::hash_table_data<K, T>::clean() {
	if (_table)
	{
		for (uint i = 0; i < _capacity && _size > 0; ++i)
		{
			node_ptr_t temp = _table[i];
			_table[i] = null;
			while (temp)
			{
				node_ptr_t del = temp;
				temp = temp->next();
				allocator->destruct(del);
				allocator->memory_release(del);
				_size--;
			}
		}
	}
	_capacity = 0;
	_size = 0;
	if (_table)allocator->memory_release(_table);
	_table = null;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::move(hash_table_data& other)
{
	if (this == &other)
		return false;
	clean();
	_size = other._size;
	_capacity = other._capacity;
	_table = other._table;
	allocator = other.allocator;
	other._size = 0;
	other._capacity = 0;
	other._table = null;
	return true;
}


template<typename K, typename T>
inline void ang::collections::hash_table_data<K, T>::set_allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == allocator)
		return;
	if (!alloc->can_delete_from(allocator))
		realloc(alloc);
	allocator = alloc;
}

template<typename K, typename T>
inline ang::memory::iallocator* ang::collections::hash_table_data<K, T>::get_allocator()const
{
	return allocator;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::realloc(ang::memory::iallocator* alloc)
{
	throw(exception_t(except_code::unsupported));
	return false;
}

template<class K, class T>
void ang::collections::hash_table_data<K, T>::increase_capacity()
{
	auto new_size = hash_table_get_next_size(_capacity);
	node_ptr_t* new_data = allocator->object_alloc<node_ptr_t>(new_size);
	memset(new_data, 0, sizeof(node_ptr_t) * new_size);

	if (_table && _capacity && _size)
	{
		uint s = _size;
		for (uint i = 0; i < _capacity && s > 0; ++i) {
			node_ptr_t temp = _table[i];
			_table[i] = null;
			while (temp) {
				index idx = create_hash_index(temp->key(), new_size);
				node_ptr_t entry = temp;
				temp = temp->next();
				entry->next(new_data[idx]);
				new_data[idx] = entry;
				s--;
			}
		}
		allocator->memory_release(_table);
	}
	_capacity = new_size;
	_table = new_data;
}

template<typename K, typename T>
inline uint ang::collections::hash_table_data<K, T>::hash_index(K const& key)const
{
	return create_hash_index(key, _capacity ? _capacity : 1);
}

template<typename K, typename T>
inline ang::collections::hash_table_node<K, T>* ang::collections::hash_table_data<K, T>::find_node(K const& key)const
{
	if (_capacity == 0)
		return null;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (key == temp->key())
			return temp;
		temp = temp->next();
	}
	return null;
}

template<typename K, typename T>
inline ang::collections::pair<K,T>& ang::collections::hash_table_data<K, T>::at(const ang::collections::iterator<ang::collections::pair<K,T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw(exception_t(except_code::invalid_memory));
	if (it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	return node_ptr_t(it.current())->_pair;
}

template<typename K, typename T>
inline ang::collections::iterator<ang::collections::pair<K,T>> ang::collections::hash_table_data<K, T>::begin()const
{
	for (auto i = 0U; i < _capacity; ++i)
	{
		if(_table[i] != null)
			return iterator_t(const_cast<hash_table_data*>(this), _table[i], i);
	}
	return iterator_t(const_cast<hash_table_data*>(this), null, 0);
}

template<typename K, typename T>
inline ang::collections::iterator<ang::collections::pair<K,T>> ang::collections::hash_table_data<K, T>::end()const
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next() != null)
				node = node->next();
			return iterator_t(const_cast<hash_table_data*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_table_data*>(this), null, 0);
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::next(ang::collections::iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	auto node = node_ptr_t(it.current());

	if (node->next() != null) {
		it.current(node->next());
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

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::next(ang::collections::iterator<ang::collections::pair<K,T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	auto node = node_ptr_t(it.current());
	uint i = it.offset();
	while (node && offset > 0)
	{
		offset--;
		if (node->next() != null) {
			node = node->next();
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

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::prev(ang::collections::iterator<ang::collections::pair<K,T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	auto node = node_ptr_t(it.current());

	if (node->prev() != null) {
		it.current(node->prev());
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
			while (node->next() != null)
				node = node->next();
			it.current(node);
			it.offset(i);
			return true;
		}
	}
	it.current(null);
	it.offset(0);
	return false;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::prev(ang::collections::iterator<ang::collections::pair<K,T>>& it, int offset)const
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
		if (node->prev() != null) {
			node = node->prev();
			continue;
		}
		node = null;
		for (i--; i >= 0; --i)
		{
			if (_table[i] != null) {
				node = _table[i];
				while (node->next() != null)
					node = node->next();
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

template<typename K, typename T>
inline uint ang::collections::hash_table_data<K, T>::counter()const
{
	return _size;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::has_items()const {
	return _size > 0;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::copy(ang::collections::ienum<ang::collections::pair<K,T>>const* items) {

	if (items == null)
		return false;
	clean();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename K, typename T>
inline void ang::collections::hash_table_data<K, T>::extend(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::insert(K key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find_node(key) != null)
		return false;
	uint idx = hash_index(key);
	node_ptr_t entry = allocator->construct<node_t>(
		allocator->object_alloc<node_t>(1)
		, ang::move(key)
		, ang::move(value)
	);
	
	if (_table[idx])
	{
		entry->next(_table[idx]);
		_table[idx]->prev(entry);
	}
	_table[idx] = entry;
	_size++;
	return true;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::insert(ang::collections::pair<K,T> pair)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find_node(pair.key_value()) != null)
		return false;
	uint idx = hash_index(pair.key_value());
	node_ptr_t entry = allocator->construct<node_t>(
		allocator->object_alloc<node_t>(1)
		, ang::move(pair)
		);
	if (_table[idx])
	{
		entry->next(_table[idx]);
		_table[idx]->prev(entry);
	}
	_table[idx] = entry;
	_size++;
	return true;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::update(K key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(key);
	if (node == null)
	{
		uint idx = hash_index(key);
		node = allocator->construct<node_t>(
			allocator->object_alloc<node_t>(1)
			, ang::move(key)
			, ang::move(value)
			);
		if (_table[idx])
		{
			node->next(_table[idx]);
			_table[idx]->prev(node);
		}
		_table[idx] = node;
		_size++;
	}
	else
	{
		node->value(ang::move(value));
	}
	return true;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::update(ang::collections::pair<K,T> pair)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(pair.key_value());
	if (node == null)
	{
		uint idx = hash_index(pair.key_value());
		node = allocator->construct<node_t>(
			allocator->object_alloc<node_t>(1)
			, ang::move(pair)
			);
		if (_table[idx])
		{
			node->next(_table[idx]);
			_table[idx]->prev(node);
		}
		_table[idx] = node;
		_size++;
	}
	else
	{
		node->value(ang::move(pair.value()));
	}
	return true;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::remove(K const& key)
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (temp->key() == key)
		{
			if (temp->prev())
			{
				temp->prev()->next(temp->next());
				if (temp->next())
					temp->next()->prev(temp->prev());
			}
			else
			{
				_table[idx] = temp->next();
				if (temp->next())
					temp->next()->prev(null);
			}
			allocator->destruct(temp);
			allocator->memory_release(temp);
			_size--;
			return true;
		}
		temp = temp->next();
	}
	return false;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::remove(K const& key, T& value)
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (temp->key() == key)
		{
			if (temp->prev())
			{
				temp->prev()->next(temp->next());
				if (temp->next())
					temp->next()->prev(temp->prev());
			}
			else
			{
				_table[idx] = temp->next();
				if (temp->next())
					temp->next()->prev(null);
			}
			value = ang::move(temp->value());
			allocator->destruct(temp);
			allocator->memory_release(temp);
			_size--;
			return true;
		}
		temp = temp->next();
	}
	return false;
}


template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::remove(ang::collections::iterator<ang::collections::pair<K, T>>)
{
	return false;
}

template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::remove(ang::collections::iterator<ang::collections::pair<K, T>>, T& value)
{
	
	return false;
}


template<typename K, typename T>
inline bool ang::collections::hash_table_data<K, T>::has_key(const K& key)const
{
	return find_node(key) != null;
}

template<typename K, typename T>
inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::hash_table_data<K, T>::find(const K& key)const
{
	if (_table == null)
		return iterator_t(const_cast<hash_table_data*>(this));

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (key == temp->key())
			return iterator_t(const_cast<hash_table_data*>(this), temp, idx);
		temp = temp->next();
	}	
	return iterator_t(const_cast<hash_table_data*>(this), null, 0);
}


////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
ang::collections::hash_table_data<ang::string, T>::hash_table_data()
	: _size(0)
	, _capacity(0)
	, _table(0)
	, allocator(ang::memory::allocator_manager::get_allocator(0))
{
	_table = allocator->object_alloc<node_ptr_t>(13);
	if (_table)
	{
		_capacity = 13;
		memset(_table, 0, sizeof(node_ptr_t) * 13);
	}
}

template<typename T>
ang::collections::hash_table_data<ang::string, T>::hash_table_data(ang::initializer_list_t<ang::collections::pair<ang::string, T>> list)
	: hash_table_data()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<typename T>
ang::collections::hash_table_data<ang::string, T>::hash_table_data(const ang::nullptr_t&)
	: hash_table_data()
{
}

template<typename T>
ang::collections::hash_table_data<ang::string, T>::hash_table_data(hash_table_data&& other)
	: _size(0)
	, _capacity(0)
	, _table(0)
	, allocator(ang::memory::allocator_manager::get_allocator(0))
{
	_size = other._size;
	_capacity = other._capacity;
	_table = other._table;
	allocator = other.allocator;

	other._size = 0;
	other._capacity = 0;
	other._table = null;
}

template<typename T>
ang::collections::hash_table_data<ang::string, T>::hash_table_data(const hash_table_data& other)
	: hash_table_data()
{
	extend(&other);
}

template<typename T>
ang::collections::hash_table_data<ang::string, T>::hash_table_data(const hash_table_data* other)
	: hash_table_data()
{
	extend(other);
}

template<typename T>
ang::collections::hash_table_data<ang::string, T>::hash_table_data(ang::collections::ienum<ang::collections::pair<ang::string, T>> const* other)
	: hash_table_data()
{
	extend(other);
}

template<typename T>
ang::collections::hash_table_data<ang::string, T>::~hash_table_data()
{
	clean();
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<hash_table_data< ang::string, T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::imap<ang::string, T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T>
inline ang::type_name_t ang::collections::hash_table_data<ang::string, T>::class_name()
{
	static string _type_name = ang::move("ang::collections::hash_table_data<"_o + type_name<ang::string>() + ","_s + type_name<T>() + ">"_s);
	return (ang::cstr_t)_type_name;
}

template<typename T>
inline ang::type_name_t ang::collections::hash_table_data<ang::string, T>::object_name()const
{
	return ang::collections::hash_table_data<ang::string, T>::class_name();
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<hash_table_data<ang::string, T>>())
	{
		*out = static_cast<ang::collections::hash_table_data<ang::string, T>*>(this);
		return true;
	}
	else if (ang::object::query_object(name, out))
	{
		return true;
	}
	else if (ang::collections::imap<ang::string, T>::query_object(name, out))
	{
		return true;
	}
	return false;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::is_empty()const {
	return _size == 0;
}

template<typename T>
inline void ang::collections::hash_table_data<ang::string, T>::clean() {
	if (_table)
	{
		for (uint i = 0; i < _capacity && _size > 0; ++i)
		{
			node_ptr_t temp = _table[i];
			_table[i] = null;
			while (temp)
			{
				node_ptr_t del = temp;
				temp = temp->next();
				allocator->destruct(del);
				allocator->memory_release(del);
				_size--;
			}
		}
	}
	_capacity = 0;
	_size = 0;
	if (_table)allocator->memory_release(_table);
	_table = null;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::move(hash_table_data& other)
{
	if (this == &other)
		return false;
	clean();
	_size = other._size;
	_capacity = other._capacity;
	_table = other._table;
	allocator = other.allocator;
	other._size = 0;
	other._capacity = 0;
	other._table = null;
	return true;
}


template<typename T>
inline void ang::collections::hash_table_data<ang::string, T>::set_allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == allocator)
		return;
	if (!alloc->can_delete_from(allocator))
		realloc(alloc);
	allocator = alloc;
}

template<typename T>
inline ang::memory::iallocator* ang::collections::hash_table_data<ang::string, T>::get_allocator()const
{
	return allocator;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::realloc(ang::memory::iallocator* alloc)
{
	throw(exception_t(except_code::unsupported));
	return false;
}

template<class T>
void ang::collections::hash_table_data<ang::string, T>::increase_capacity()
{
	auto new_size = hash_table_get_next_size(_capacity);
	node_ptr_t* new_data = allocator->object_alloc<node_ptr_t>(new_size);
	memset(new_data, 0, sizeof(node_ptr_t) * new_size);

	if (_table && _capacity && _size)
	{
		uint s = _size;
		for (uint i = 0; i < _capacity && s > 0; ++i) {
			node_ptr_t temp = _table[i];
			_table[i] = null;
			while (temp) {
				index idx = create_hash_index(temp->key(), new_size);
				node_ptr_t entry = temp;
				temp = temp->next();
				entry->next(new_data[idx]);
				new_data[idx] = entry;
				s--;
			}
		}
		allocator->memory_release(_table);
	}
	_capacity = new_size;
	_table = new_data;
}

template<typename T>
inline uint ang::collections::hash_table_data<ang::string, T>::hash_index(ang::cstr_t key)const
{
	return create_hash_index(key, _capacity ? _capacity : 1);
}

template<typename T>
inline uint ang::collections::hash_table_data<ang::string, T>::hash_index(ang::cwstr_t key)const
{
	return create_hash_index(key, _capacity ? _capacity : 1);
}

template<typename T>
inline ang::collections::hash_table_node<ang::string, T>* ang::collections::hash_table_data<ang::string, T>::find_node(ang::cstr_t key)const
{
	if (_capacity == 0)
		return null;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (key == temp->key())
			return temp;
		temp = temp->next();
	}
	return null;
}

template<typename T>
inline ang::collections::hash_table_node<ang::string, T>* ang::collections::hash_table_data<ang::string, T>::find_node(ang::cwstr_t key)const
{
	if (_capacity == 0)
		return null;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (key == temp->key())
			return temp;
		temp = temp->next();
	}
	return null;
}

template<typename T>
inline ang::collections::pair<ang::string, T>& ang::collections::hash_table_data<ang::string, T>::at(const ang::collections::iterator<ang::collections::pair<ang::string, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw(exception_t(except_code::invalid_memory));
	if (it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	return node_ptr_t(it.current())->_pair;
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::string, T>> ang::collections::hash_table_data<ang::string, T>::begin()const
{
	for (auto i = 0U; i < _capacity; ++i)
	{
		if (_table[i] != null)
			return iterator_t(const_cast<hash_table_data*>(this), _table[i], i);
	}
	return iterator_t(const_cast<hash_table_data*>(this), null, 0);
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::string, T>> ang::collections::hash_table_data<ang::string, T>::end()const
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next() != null)
				node = node->next();
			return iterator_t(const_cast<hash_table_data*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_table_data*>(this), null, 0);
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::next(ang::collections::iterator<ang::collections::pair<ang::string, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	auto node = node_ptr_t(it.current());

	if (node->next() != null) {
		it.current(node->next());
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

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::next(ang::collections::iterator<ang::collections::pair<ang::string, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	auto node = node_ptr_t(it.current());
	uint i = it.offset();
	while (node && offset > 0)
	{
		offset--;
		if (node->next() != null) {
			node = node->next();
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

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::prev(ang::collections::iterator<ang::collections::pair<ang::string, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	auto node = node_ptr_t(it.current());

	if (node->prev() != null) {
		it.current(node->prev());
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
			while (node->next() != null)
				node = node->next();
			it.current(node);
			it.offset(i);
			return true;
		}
	}
	it.current(null);
	it.offset(0);
	return false;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::prev(ang::collections::iterator<ang::collections::pair<ang::string, T>>& it, int offset)const
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
		if (node->prev() != null) {
			node = node->prev();
			continue;
		}
		node = null;
		for (i--; i >= 0; --i)
		{
			if (_table[i] != null) {
				node = _table[i];
				while (node->next() != null)
					node = node->next();
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

template<typename T>
inline uint ang::collections::hash_table_data<ang::string, T>::counter()const
{
	return _size;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::has_items()const {
	return _size > 0;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::copy(ang::collections::ienum<ang::collections::pair<ang::string, T>>const* items) {

	if (items == null)
		return false;
	clean();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename T>
inline void ang::collections::hash_table_data<ang::string, T>::extend(ang::collections::ienum<ang::collections::pair<ang::string, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::insert(ang::cstr_t key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find_node(key) != null)
		return false;
	uint idx = hash_index(key);
	node_ptr_t entry = allocator->construct<node_t>(
		allocator->object_alloc<node_t>(1)
		, key
		, ang::move(value)
		);	
	if (_table[idx])
	{
		entry->next(_table[idx]);
		_table[idx]->prev(entry);
	}
	_table[idx] = entry;
	_size++;
	return true;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::insert(ang::cwstr_t key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find_node(key) != null)
		return false;
	uint idx = hash_index(key);
	node_ptr_t entry = allocator->construct<node_t>(
		allocator->object_alloc<node_t>(1)
		, key
		, ang::move(value)
		);
	if (_table[idx])
	{
		entry->next(_table[idx]);
		_table[idx]->prev(entry);
	}
	_table[idx] = entry;
	_size++;
	return true;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::insert(ang::collections::pair<ang::string, T> pair)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find_node(pair.key_value()) != null)
		return false;
	uint idx = hash_index(pair.key_value());
	node_ptr_t entry = allocator->construct<node_t>(
		allocator->object_alloc<node_t>(1)
		, ang::move(pair)
		);
	if (_table[idx])
	{
		entry->next(_table[idx]);
		_table[idx]->prev(entry);
	}
	_table[idx] = entry;
	_size++;
	return true;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::update(ang::cstr_t key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(key);
	if (node == null)
	{
		uint idx = hash_index(key);
		node = allocator->construct<node_t>(
			allocator->object_alloc<node_t>(1)
			, key
			, ang::move(value)
			);
		
		if (_table[idx])
		{
			node->next(_table[idx]);
			_table[idx]->prev(node);
		}
		_table[idx] = node;
		_size++;
	}
	else
	{
		node->value(ang::move(value));
	}
	return true;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::update(ang::cwstr_t key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(key);
	if (node == null)
	{
		uint idx = hash_index(key);
		node = allocator->construct<node_t>(
			allocator->object_alloc<node_t>(1)
			, key
			, ang::move(value)
			);

		if (_table[idx])
		{
			node->next(_table[idx]);
			_table[idx]->prev(node);
		}
		_table[idx] = node;
		_size++;
	}
	else
	{
		node->value(ang::move(value));
	}
	return true;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::update(ang::collections::pair<ang::string, T> pair)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(pair.key_value());
	if (node == null)
	{
		uint idx = hash_index(pair.key_value());
		node = allocator->construct<node_t>(
			allocator->object_alloc<node_t>(1)
			, ang::move(pair)
			);
		if (_table[idx])
		{
			node->next(_table[idx]);
			_table[idx]->prev(node);
		}
		_table[idx] = node;
		_size++;
	}
	else
	{
		node->value(ang::move(pair.value()));
	}
	return true;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::remove(ang::cstr_t key)
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (temp->key() == key)
		{
			if (temp->prev())
			{
				temp->prev()->next(temp->next());
				if (temp->next())
					temp->next()->prev(temp->prev());
			}
			else
			{
				_table[idx] = temp->next();
				if (temp->next())
					temp->next()->prev(null);
			}
			allocator->destruct(temp);
			allocator->memory_release(temp);
			_size--;
			return true;
		}
		temp = temp->next();
	}
	return false;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::remove(ang::cwstr_t key)
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (temp->key() == key)
		{
			if (temp->prev())
			{
				temp->prev()->next(temp->next());
				if (temp->next())
					temp->next()->prev(temp->prev());
			}
			else
			{
				_table[idx] = temp->next();
				if (temp->next())
					temp->next()->prev(null);
			}
			allocator->destruct(temp);
			allocator->memory_release(temp);
			_size--;
			return true;
		}
		temp = temp->next();
	}
	return false;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::remove(ang::cstr_t key, T& value)
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (temp->key() == key)
		{
			if (temp->prev())
			{
				temp->prev()->next(temp->next());
				if (temp->next())
					temp->next()->prev(temp->prev());
			}
			else
			{
				_table[idx] = temp->next();
				if (temp->next())
					temp->next()->prev(null);
			}
			value = ang::move(temp->value());
			allocator->destruct(temp);
			allocator->memory_release(temp);
			_size--;
			return true;
		}
		temp = temp->next();
	}
	return false;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::remove(ang::cwstr_t key, T& value)
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (temp->key() == key)
		{
			if (temp->prev())
			{
				temp->prev()->next(temp->next());
				if (temp->next())
					temp->next()->prev(temp->prev());
			}
			else
			{
				_table[idx] = temp->next();
				if (temp->next())
					temp->next()->prev(null);
			}
			value = ang::move(temp->value());
			allocator->destruct(temp);
			allocator->memory_release(temp);
			_size--;
			return true;
		}
		temp = temp->next();
	}
	return false;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::remove(ang::collections::iterator<ang::collections::pair<ang::string, T>>)
{
	return false;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::remove(ang::collections::iterator<ang::collections::pair<ang::string, T>>, T& value)
{

	return false;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::has_key(ang::cstr_t key)const
{
	return find_node(key) != null;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::string, T>::has_key(ang::cwstr_t key)const
{
	return find_node(key) != null;
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::string, T>> ang::collections::hash_table_data<ang::string, T>::find(ang::cstr_t key)const
{
	if (_table == null)
		return iterator_t(const_cast<hash_table_data*>(this));

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (key == temp->key())
			return iterator_t(const_cast<hash_table_data*>(this), temp, idx);
		temp = temp->next();
	}
	return iterator_t(const_cast<hash_table_data*>(this), null, 0);
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::string, T>> ang::collections::hash_table_data<ang::string, T>::find(ang::cwstr_t key)const
{
	if (_table == null)
		return iterator_t(const_cast<hash_table_data*>(this));

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (key == temp->key())
			return iterator_t(const_cast<hash_table_data*>(this), temp, idx);
		temp = temp->next();
	}
	return iterator_t(const_cast<hash_table_data*>(this), null, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
ang::collections::hash_table_data<ang::wstring, T>::hash_table_data()
	: _size(0)
	, _capacity(0)
	, _table(0)
	, allocator(ang::memory::allocator_manager::get_allocator(0))
{
	_table = allocator->object_alloc<node_ptr_t>(13);
	if (_table)
	{
		_capacity = 13;
		memset(_table, 0, sizeof(node_ptr_t) * 13);
	}
}

template<typename T>
ang::collections::hash_table_data<ang::wstring, T>::hash_table_data(ang::initializer_list_t<ang::collections::pair<ang::wstring, T>> list)
	: hash_table_data()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<typename T>
ang::collections::hash_table_data<ang::wstring, T>::hash_table_data(const ang::nullptr_t&)
	: hash_table_data()
{
}

template<typename T>
ang::collections::hash_table_data<ang::wstring, T>::hash_table_data(hash_table_data&& other)
	: _size(0)
	, _capacity(0)
	, _table(0)
	, allocator(ang::memory::allocator_manager::get_allocator(0))
{
	_size = other._size;
	_capacity = other._capacity;
	_table = other._table;
	allocator = other.allocator;

	other._size = 0;
	other._capacity = 0;
	other._table = null;
}

template<typename T>
ang::collections::hash_table_data<ang::wstring, T>::hash_table_data(const hash_table_data& other)
	: hash_table_data()
{
	extend(&other);
}

template<typename T>
ang::collections::hash_table_data<ang::wstring, T>::hash_table_data(const hash_table_data* other)
	: hash_table_data()
{
	extend(other);
}

template<typename T>
ang::collections::hash_table_data<ang::wstring, T>::hash_table_data(ang::collections::ienum<ang::collections::pair<ang::wstring, T>> const* other)
	: hash_table_data()
{
	extend(other);
}

template<typename T>
ang::collections::hash_table_data<ang::wstring, T>::~hash_table_data()
{
	clean();
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<hash_table_data< ang::wstring, T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::imap<ang::wstring, T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T>
inline ang::type_name_t ang::collections::hash_table_data<ang::wstring, T>::class_name()
{
	static string _type_name = ang::move("ang::collections::hash_table_data<"_o + type_name<ang::wstring>() + ","_s + type_name<T>() + ">"_s);
	return (ang::cstr_t)_type_name;
}

template<typename T>
inline ang::type_name_t ang::collections::hash_table_data<ang::wstring, T>::object_name()const
{
	return ang::collections::hash_table_data<ang::wstring, T>::class_name();
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<hash_table_data<ang::wstring, T>>())
	{
		*out = static_cast<ang::collections::hash_table_data<ang::wstring, T>*>(this);
		return true;
	}
	else if (ang::object::query_object(name, out))
	{
		return true;
	}
	else if (ang::collections::imap<ang::wstring, T>::query_object(name, out))
	{
		return true;
	}
	return false;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::is_empty()const {
	return _size == 0;
}

template<typename T>
inline void ang::collections::hash_table_data<ang::wstring, T>::clean() {
	if (_table)
	{
		for (uint i = 0; i < _capacity && _size > 0; ++i)
		{
			node_ptr_t temp = _table[i];
			_table[i] = null;
			while (temp)
			{
				node_ptr_t del = temp;
				temp = temp->next();
				allocator->destruct(del);
				allocator->memory_release(del);
				_size--;
			}
		}
	}
	_capacity = 0;
	_size = 0;
	if (_table)allocator->memory_release(_table);
	_table = null;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::move(hash_table_data& other)
{
	if (this == &other)
		return false;
	clean();
	_size = other._size;
	_capacity = other._capacity;
	_table = other._table;
	allocator = other.allocator;
	other._size = 0;
	other._capacity = 0;
	other._table = null;
	return true;
}


template<typename T>
inline void ang::collections::hash_table_data<ang::wstring, T>::set_allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == allocator)
		return;
	if (!alloc->can_delete_from(allocator))
		realloc(alloc);
	allocator = alloc;
}

template<typename T>
inline ang::memory::iallocator* ang::collections::hash_table_data<ang::wstring, T>::get_allocator()const
{
	return allocator;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::realloc(ang::memory::iallocator* alloc)
{
	throw(exception_t(except_code::unsupported));
	return false;
}

template<class T>
void ang::collections::hash_table_data<ang::wstring, T>::increase_capacity()
{
	auto new_size = hash_table_get_next_size(_capacity);
	node_ptr_t* new_data = allocator->object_alloc<node_ptr_t>(new_size);
	memset(new_data, 0, sizeof(node_ptr_t) * new_size);

	if (_table && _capacity && _size)
	{
		uint s = _size;
		for (uint i = 0; i < _capacity && s > 0; ++i) {
			node_ptr_t temp = _table[i];
			_table[i] = null;
			while (temp) {
				index idx = create_hash_index(temp->key(), new_size);
				node_ptr_t entry = temp;
				temp = temp->next();
				entry->next(new_data[idx]);
				new_data[idx] = entry;
				s--;
			}
		}
		allocator->memory_release(_table);
	}
	_capacity = new_size;
	_table = new_data;
}

template<typename T>
inline uint ang::collections::hash_table_data<ang::wstring, T>::hash_index(ang::cstr_t key)const
{
	return create_hash_index(key, _capacity ? _capacity : 1);
}

template<typename T>
inline uint ang::collections::hash_table_data<ang::wstring, T>::hash_index(ang::cwstr_t key)const
{
	return create_hash_index(key, _capacity ? _capacity : 1);
}

template<typename T>
inline ang::collections::hash_table_node<ang::wstring, T>* ang::collections::hash_table_data<ang::wstring, T>::find_node(ang::cstr_t key)const
{
	if (_capacity == 0)
		return null;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (key == temp->key())
			return temp;
		temp = temp->next();
	}
	return null;
}

template<typename T>
inline ang::collections::hash_table_node<ang::wstring, T>* ang::collections::hash_table_data<ang::wstring, T>::find_node(ang::cwstr_t key)const
{
	if (_capacity == 0)
		return null;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (key == temp->key())
			return temp;
		temp = temp->next();
	}
	return null;
}

template<typename T>
inline ang::collections::pair<ang::wstring, T>& ang::collections::hash_table_data<ang::wstring, T>::at(const ang::collections::iterator<ang::collections::pair<ang::wstring, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw(exception_t(except_code::invalid_memory));
	if (it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	return node_ptr_t(it.current())->_pair;
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::wstring, T>> ang::collections::hash_table_data<ang::wstring, T>::begin()const
{
	for (auto i = 0U; i < _capacity; ++i)
	{
		if (_table[i] != null)
			return iterator_t(const_cast<hash_table_data*>(this), _table[i], i);
	}
	return iterator_t(const_cast<hash_table_data*>(this), null, 0);
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::wstring, T>> ang::collections::hash_table_data<ang::wstring, T>::end()const
{
	for (int i = (int)_capacity - 1; i >= 0; --i)
	{
		if (_table[i] != null)
		{
			node_ptr_t node = _table[i];
			while (node->next() != null)
				node = node->next();
			return iterator_t(const_cast<hash_table_data*>(this), node, i);
		}
	}
	return iterator_t(const_cast<hash_table_data*>(this), null, 0);
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::next(ang::collections::iterator<ang::collections::pair<ang::wstring, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	auto node = node_ptr_t(it.current());

	if (node->next() != null) {
		it.current(node->next());
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

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::next(ang::collections::iterator<ang::collections::pair<ang::wstring, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	auto node = node_ptr_t(it.current());
	uint i = it.offset();
	while (node && offset > 0)
	{
		offset--;
		if (node->next() != null) {
			node = node->next();
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

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::prev(ang::collections::iterator<ang::collections::pair<ang::wstring, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	auto node = node_ptr_t(it.current());

	if (node->prev() != null) {
		it.current(node->prev());
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
			while (node->next() != null)
				node = node->next();
			it.current(node);
			it.offset(i);
			return true;
		}
	}
	it.current(null);
	it.offset(0);
	return false;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::prev(ang::collections::iterator<ang::collections::pair<ang::wstring, T>>& it, int offset)const
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
		if (node->prev() != null) {
			node = node->prev();
			continue;
		}
		node = null;
		for (i--; i >= 0; --i)
		{
			if (_table[i] != null) {
				node = _table[i];
				while (node->next() != null)
					node = node->next();
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

template<typename T>
inline uint ang::collections::hash_table_data<ang::wstring, T>::counter()const
{
	return _size;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::has_items()const {
	return _size > 0;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::copy(ang::collections::ienum<ang::collections::pair<ang::wstring, T>>const* items) {

	if (items == null)
		return false;
	clean();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename T>
inline void ang::collections::hash_table_data<ang::wstring, T>::extend(ang::collections::ienum<ang::collections::pair<ang::wstring, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::insert(ang::cstr_t key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find_node(key) != null)
		return false;
	uint idx = hash_index(key);
	node_ptr_t entry = allocator->construct<node_t>(
		allocator->object_alloc<node_t>(1)
		, key
		, ang::move(value)
		);
	if (_table[idx])
	{
		entry->next(_table[idx]);
		_table[idx]->prev(entry);
	}
	_table[idx] = entry;
	_size++;
	return true;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::insert(ang::cwstr_t key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find_node(key) != null)
		return false;
	uint idx = hash_index(key);
	node_ptr_t entry = allocator->construct<node_t>(
		allocator->object_alloc<node_t>(1)
		, key
		, ang::move(value)
		);
	if (_table[idx])
	{
		entry->next(_table[idx]);
		_table[idx]->prev(entry);
	}
	_table[idx] = entry;
	_size++;
	return true;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::insert(ang::collections::pair<ang::wstring, T> pair)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find_node(pair.key_value()) != null)
		return false;
	uint idx = hash_index(pair.key_value());
	node_ptr_t entry = allocator->construct<node_t>(
		allocator->object_alloc<node_t>(1)
		, ang::move(pair)
		);
	if (_table[idx])
	{
		entry->next(_table[idx]);
		_table[idx]->prev(entry);
	}
	_table[idx] = entry;
	_size++;
	return true;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::update(ang::cstr_t key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(key);
	if (node == null)
	{
		uint idx = hash_index(key);
		node = allocator->construct<node_t>(
			allocator->object_alloc<node_t>(1)
			, key
			, ang::move(value)
			);

		if (_table[idx])
		{
			node->next(_table[idx]);
			_table[idx]->prev(node);
		}
		_table[idx] = node;
		_size++;
	}
	else
	{
		node->value(ang::move(value));
	}
	return true;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::update(ang::cwstr_t key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(key);
	if (node == null)
	{
		uint idx = hash_index(key);
		node = allocator->construct<node_t>(
			allocator->object_alloc<node_t>(1)
			, key
			, ang::move(value)
			);

		if (_table[idx])
		{
			node->next(_table[idx]);
			_table[idx]->prev(node);
		}
		_table[idx] = node;
		_size++;
	}
	else
	{
		node->value(ang::move(value));
	}
	return true;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::update(ang::collections::pair<ang::wstring, T> pair)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	node_ptr_t node = find_node(pair.key_value());
	if (node == null)
	{
		uint idx = hash_index(pair.key_value());
		node = allocator->construct<node_t>(
			allocator->object_alloc<node_t>(1)
			, ang::move(pair)
			);
		if (_table[idx])
		{
			node->next(_table[idx]);
			_table[idx]->prev(node);
		}
		_table[idx] = node;
		_size++;
	}
	else
	{
		node->value(ang::move(pair.value()));
	}
	return true;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::remove(ang::cstr_t key)
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (temp->key() == key)
		{
			if (temp->prev())
			{
				temp->prev()->next(temp->next());
				if (temp->next())
					temp->next()->prev(temp->prev());
			}
			else
			{
				_table[idx] = temp->next();
				if (temp->next())
					temp->next()->prev(null);
			}
			allocator->destruct(temp);
			allocator->memory_release(temp);
			_size--;
			return true;
		}
		temp = temp->next();
	}
	return false;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::remove(ang::cwstr_t key)
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (temp->key() == key)
		{
			if (temp->prev())
			{
				temp->prev()->next(temp->next());
				if (temp->next())
					temp->next()->prev(temp->prev());
			}
			else
			{
				_table[idx] = temp->next();
				if (temp->next())
					temp->next()->prev(null);
			}
			allocator->destruct(temp);
			allocator->memory_release(temp);
			_size--;
			return true;
		}
		temp = temp->next();
	}
	return false;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::remove(ang::cstr_t key, T& value)
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (temp->key() == key)
		{
			if (temp->prev())
			{
				temp->prev()->next(temp->next());
				if (temp->next())
					temp->next()->prev(temp->prev());
			}
			else
			{
				_table[idx] = temp->next();
				if (temp->next())
					temp->next()->prev(null);
			}
			value = ang::move(temp->value());
			allocator->destruct(temp);
			allocator->memory_release(temp);
			_size--;
			return true;
		}
		temp = temp->next();
	}
	return false;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::remove(ang::cwstr_t key, T& value)
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];

	while (temp != nullptr)
	{
		if (temp->key() == key)
		{
			if (temp->prev())
			{
				temp->prev()->next(temp->next());
				if (temp->next())
					temp->next()->prev(temp->prev());
			}
			else
			{
				_table[idx] = temp->next();
				if (temp->next())
					temp->next()->prev(null);
			}
			value = ang::move(temp->value());
			allocator->destruct(temp);
			allocator->memory_release(temp);
			_size--;
			return true;
		}
		temp = temp->next();
	}
	return false;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::remove(ang::collections::iterator<ang::collections::pair<ang::wstring, T>>)
{
	return false;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::remove(ang::collections::iterator<ang::collections::pair<ang::wstring, T>>, T& value)
{

	return false;
}


template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::has_key(ang::cstr_t key)const
{
	return find_node(key) != null;
}

template<typename T>
inline bool ang::collections::hash_table_data<ang::wstring, T>::has_key(ang::cwstr_t key)const
{
	return find_node(key) != null;
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::wstring, T>> ang::collections::hash_table_data<ang::wstring, T>::find(ang::cstr_t key)const
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (key == temp->key())
			return iterator_t(const_cast<hash_table_data*>(this), temp, idx);
		temp = temp->next();
	}
	return iterator_t(const_cast<hash_table_data*>(this), null, 0);
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::wstring, T>> ang::collections::hash_table_data<ang::wstring, T>::find(ang::cwstr_t key)const
{
	if (_table == null)
		return false;

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (key == temp->key())
			return iterator_t(const_cast<hash_table_data*>(this), temp, idx);
		temp = temp->next();
	}
	return iterator_t(const_cast<hash_table_data*>(this), null, 0);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>::object_wrapper()
	: _ptr(null)
{

}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{

}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>::object_wrapper(ang::collections::hash_table_data<K, T>* ptr)
	: object_wrapper()
{
	set(ptr);
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>::object_wrapper(ang::collections::ienum<ang::collections::pair<K, T>> const* store)
	: object_wrapper()
{
	set(new collections::hash_table_data<K, T>(store));
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>::object_wrapper(ang::initializer_list_t<ang::collections::pair<K, T>> list)
{
	set(new collections::hash_table_data<K, T>(list));
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>::object_wrapper(ang::object_wrapper<ang::collections::hash_table_data<K, T>> && other)
{
	collections::hash_table_data<K, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>::object_wrapper(ang::object_wrapper<ang::collections::hash_table_data<K, T>> const& other)
{
	set(other.get());
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>::~object_wrapper()
{
	clean();
}

template<typename K, typename T>
inline void ang::object_wrapper<ang::collections::hash_table_data<K, T>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename K, typename T>
inline bool ang::object_wrapper<ang::collections::hash_table_data<K, T>>::is_empty()const
{
	return _ptr == null;
}

template<typename K, typename T>
inline ang::collections::hash_table_data<K, T>* ang::object_wrapper<ang::collections::hash_table_data<K, T>>::get(void)const
{
	return _ptr;
}

template<typename K, typename T>
inline void ang::object_wrapper<ang::collections::hash_table_data<K, T>>::set(ang::collections::hash_table_data<K, T>* ptr)
{
	collections::hash_table_data<K, T> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename K, typename T>
inline ang::collections::hash_table_data<K, T> ** ang::object_wrapper<ang::collections::hash_table_data<K, T>>::addres_of(void)
{
	return &_ptr;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>& ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator = (ang::object_wrapper<ang::collections::hash_table_data<K, T>> && other)
{
	ang::collections::hash_table_data<K, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>& ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator = (ang::object_wrapper<ang::collections::hash_table_data<K, T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>& ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>& ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator = (ang::collections::hash_table_data<K, T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>& ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator = (ang::collections::ienum<ang::collections::pair<K, T>> const* store)
{
	if (is_empty())
		set(new collections::hash_table_data<K, T>(store));
	else
		_ptr->copy(store);
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>& ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator += (ang::collections::pair<K, T> pair)
{
	if (is_empty())
		set(new collections::hash_table_data<K, T>());
	_ptr->update(ang::move(pair));
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator ang::object_t()const {
	return _ptr;
}

template<typename K, typename T>
inline ang::object_wrapper_ptr<ang::collections::hash_table_data<K, T>> ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator & (void)
{
	return this;
}

template<typename K, typename T>
inline ang::collections::hash_table_data<K, T> * ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator -> (void)
{
	return get();
}

template<typename K, typename T>
inline ang::collections::hash_table_data<K, T> const* ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator -> (void)const
{
	return get();
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator ang::collections::hash_table_data<K, T> * (void)
{
	return get();
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator ang::collections::hash_table_data<K, T> const* (void)const
{
	return get();
}

template<typename K, typename T>
inline T& ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator [] (K const& key)
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename K, typename T>
inline T ang::object_wrapper<ang::collections::hash_table_data<K, T>>::operator [] (K const& key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}


///////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::object_wrapper(ang::collections::hash_table_data<ang::string, T>* ptr)
	: object_wrapper()
{
	set(ptr);
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::object_wrapper(ang::collections::ienum<ang::collections::pair<ang::string, T>> const* store)
	: object_wrapper()
{
	set(new collections::hash_table_data<ang::string, T>(store));
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::object_wrapper(ang::initializer_list_t<ang::collections::pair<ang::string, T>> list)
{
	set(new collections::hash_table_data<ang::string, T>(list));
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::object_wrapper(ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>> && other)
{
	collections::hash_table_data<ang::string, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::object_wrapper(ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>> const& other)
{
	set(other.get());
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::~object_wrapper()
{
	clean();
}

template<typename T>
inline void ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T>
inline bool ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::collections::hash_table_data<ang::string, T>* ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::set(ang::collections::hash_table_data<ang::string, T>* ptr)
{
	collections::hash_table_data<ang::string, T> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T>
inline ang::collections::hash_table_data<ang::string, T> ** ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator = (ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>> && other)
{
	ang::collections::hash_table_data<ang::string, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator = (ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator = (ang::collections::hash_table_data<ang::string, T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator = (ang::collections::ienum<ang::collections::pair<ang::string, T>> const* store)
{
	if (is_empty())
		set(new collections::hash_table_data<ang::string, T>(store));
	else
		_ptr->copy(store);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator += (ang::collections::pair<ang::string, T> pair)
{
	if (is_empty())
		set(new collections::hash_table_data<ang::string, T>());
	_ptr->update(ang::move(pair));
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator ang::object_t()const {
	return _ptr;
}

template<typename T>
inline ang::object_wrapper_ptr<ang::collections::hash_table_data<ang::string, T>> ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator & (void)
{
	return this;
}

template<typename T>
inline ang::collections::hash_table_data<ang::string, T> * ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator -> (void)
{
	return get();
}

template<typename T>
inline ang::collections::hash_table_data<ang::string, T> const* ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator ang::collections::hash_table_data<ang::string, T> * (void)
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator ang::collections::hash_table_data<ang::string, T> const* (void)const
{
	return get();
}

template<typename T>
inline T& ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator [] (ang::cstr_t key)
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T& ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator [] (ang::cwstr_t key)
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator [] (ang::cstr_t key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T ang::object_wrapper<ang::collections::hash_table_data<ang::string, T>>::operator [] (ang::cwstr_t key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

///////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::object_wrapper(ang::collections::hash_table_data<ang::wstring, T>* ptr)
	: object_wrapper()
{
	set(ptr);
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::object_wrapper(ang::collections::ienum<ang::collections::pair<ang::wstring, T>> const* store)
	: object_wrapper()
{
	set(new collections::hash_table_data<ang::wstring, T>(store));
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::object_wrapper(ang::initializer_list_t<ang::collections::pair<ang::wstring, T>> list)
{
	set(new collections::hash_table_data<ang::wstring, T>(list));
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::object_wrapper(ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>> && other)
{
	collections::hash_table_data<ang::wstring, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::object_wrapper(ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>> const& other)
{
	set(other.get());
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::~object_wrapper()
{
	clean();
}

template<typename T>
inline void ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T>
inline bool ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::collections::hash_table_data<ang::wstring, T>* ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::set(ang::collections::hash_table_data<ang::wstring, T>* ptr)
{
	collections::hash_table_data<ang::wstring, T> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T>
inline ang::collections::hash_table_data<ang::wstring, T> ** ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator = (ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>> && other)
{
	ang::collections::hash_table_data<ang::wstring, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator = (ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator = (ang::collections::hash_table_data<ang::wstring, T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator = (ang::collections::ienum<ang::collections::pair<ang::wstring, T>> const* store)
{
	if (is_empty())
		set(new collections::hash_table_data<ang::wstring, T>(store));
	else
		_ptr->copy(store);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator += (ang::collections::pair<ang::wstring, T> pair)
{
	if (is_empty())
		set(new collections::hash_table_data<ang::wstring, T>());
	_ptr->update(ang::move(pair));
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator ang::object_t()const {
	return _ptr;
}

template<typename T>
inline ang::object_wrapper_ptr<ang::collections::hash_table_data<ang::wstring, T>> ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator & (void)
{
	return this;
}

template<typename T>
inline ang::collections::hash_table_data<ang::wstring, T> * ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator -> (void)
{
	return get();
}

template<typename T>
inline ang::collections::hash_table_data<ang::wstring, T> const* ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator ang::collections::hash_table_data<ang::wstring, T> * (void)
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator ang::collections::hash_table_data<ang::wstring, T> const* (void)const
{
	return get();
}

template<typename T>
inline T& ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator [] (ang::cstr_t key)
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T& ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator [] (ang::cwstr_t key)
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator [] (ang::cstr_t key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T ang::object_wrapper<ang::collections::hash_table_data<ang::wstring, T>>::operator [] (ang::cwstr_t key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

///////////////////////////////////////////////////////////////////////////////////////////////

#endif//__ANG_HASH_TABLE_HPP__