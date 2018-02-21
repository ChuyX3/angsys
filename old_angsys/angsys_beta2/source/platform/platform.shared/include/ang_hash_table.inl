#ifndef __ANG_INLINES_H__
#error ...
#elif !defined __ANG_INLINES_HASH_TABLE_HPP__
#define __ANG_INLINES_HASH_TABLE_HPP__

template<class K, class T>
ang_hash_table<K,T>::ang_hash_table()
{
	_size = 0;
	_capacity = 127;
	_table = ang_allocator<ang_hash_node<K, T>*>::alloc(127);
	memset(_table, 0, sizeof(ang_hash_node<K, T>*) * _capacity);
}

template<class K, class T>
ang_hash_table<K, T>::~ang_hash_table()
{
	clean();
}

template<class K, class T>
void ang_hash_table<K, T>::clean()
{
	for (ang_uint32_t i = 0; i < _capacity && _size > 0; ++i) {
		ang_hash_node<K, T>* temp = _table[i];
		_table[i] = nullptr;
		while (temp) {
			ang_hash_node<K, T>* del = temp;
			temp = temp->next;
			ang_delete(del);
			_size--;
		}
	}
	_capacity = 0;
	_size = 0;
	if (_table)ang_delete(_table);
	_table = NULL;
}

template<class K, class T>
bool ang_hash_table<K, T>::insert(ang_pair<K, T> pair)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find(pair.key) != NULL)
		return false;
	index idx = hash_index(pair.key);
	node_ptr_t entry = ang_allocator<ang_hash_node<K, T>>::alloc(1);
	ang_allocator<ang_hash_node<K, T>>::template construct<ang_pair<K, T>>(entry, ang::move(pair));
	entry->next = _table[idx];
	_table[idx] = entry;
	_size++;
	return true;
}

template<class K, class T>
bool ang_hash_table<K, T>::insert(K key, T value)
{
	if (_size > (_capacity * 0.75))
		increase_capacity();

	if (find(key) != NULL)
		return false;
	index idx = hash_index(key);
	node_ptr_t entry = ang_allocator<ang_hash_node<K, T>>::alloc(1);
	ang_allocator<ang_hash_node<K, T>>::template construct<ang_pair<K, T>>(entry, ang_pair<K, T>{ang::move(key), value});
	entry->next = _table[idx];
	_table[idx] = entry;
	_size++;
	return true;
}

template<class K, class T>
bool ang_hash_table<K, T>::remove(K const& key, T& out)
{
	if (_capacity == 0)
		return false;

	index idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	node_ptr_t* prev = &_table[idx];

	while (temp != nullptr)
	{
		if (temp->pair.key == key)
		{
			*prev = temp->next;
			out = ang::move(temp->pair.value);
			ang_delete(temp);
			_size--;
			return true;
		}
		prev = &temp->next;
		temp = temp->next;
	}
	return false;
}

template<class K, class T>
bool ang_hash_table<K, T>::is_empty()const { return _size == 0; }

template<class K, class T>
bool ang_hash_table<K, T>::contains(K const& key)const { return find(key) == NULL; }

template<class K, class T>
ang_uint64_t ang_hash_table<K, T>::size()const { return _size; }

template<class K, class T>
ang_uint32_t ang_hash_table<K, T>::hash_index(K const& key)const
{
	return ang_create_hash_index(key, _capacity);
}

template<class K, class T>
ang_hash_node<K, T>* ang_hash_table<K, T>::find(K const& key)const
{
	if (_capacity == 0)
		return NULL;

	index idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != nullptr)
	{
		if (key == temp->pair.key)
			return temp;
		temp = temp->next;
	}
	return NULL;
}

template<class K, class T>
bool ang_hash_table<K, T>::find(K const& key, T& out)const
{
	index idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != nullptr)
	{
		if (key == temp->pair.key)
		{
			out = temp->pair.value;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

template<class K, class T>
void ang_hash_table<K, T>::increase_capacity()
{
	auto new_size = ang_hash_table_get_next_size(_capacity);
	ang_hash_node<K, T>** new_data = ang_allocator<ang_hash_node<K, T>*>::alloc(new_size);
	memset(new_data, 0, sizeof(ang_hash_node<K, T>*) * new_size);

	if (_table && _capacity && _size)
	{
		ang_int32_t s = _size;
		for (ang_uint32_t i = 0; i < _capacity && s > 0; ++i) {
			ang_hash_node<K, T>* temp = _table[i];
			_table[i] = nullptr;
			while (temp) {
				index idx = ang_create_hash_index(temp->pair.key, new_size);
				node_ptr_t entry = temp;
				temp = temp->next;
				entry->next = new_data[idx];
				new_data[idx] = entry;
				s--;
			}
		}
		ang_delete(_table);
	}
	_capacity = new_size;
	_table = new_data;
}

#endif//__ANG_INLINES_HASH_TABLE_HPP__

