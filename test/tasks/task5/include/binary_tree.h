#pragma once

namespace ang
{
	namespace collections
	{
		template<typename K, typename T> class binary_three_data;
		template<typename K, typename T> using binary_three = object_wrapper<binary_three_data<K, T>>;
	}

	namespace collections
	{

		template<typename K, typename T>
		struct binary_three_node
		{
			typedef K key_t;
			typedef T value_t;
			typedef collections::pair<K, T> pair_t;
			typedef binary_three_node<K, T> self_t, *self_ptr_t;

			inline static type_name_t class_name();

			binary_three_node(K key) : _pair(key) {}
			binary_three_node(K key, value_t val) : _pair(ang::move(key), ang::move(val)) {}
			binary_three_node(pair_t p) : _pair(ang::move(p)) {}

			inline key_t& key() {
				return _pair.key_value();
			}
			inline key_t const& key()const {
				return _pair.key_value();
			}
			inline void key(key_t p) {
				_pair.key_value(ang::move(p));
			}

			inline value_t& value() {
				return _pair.value();
			}
			inline value_t const& value()const {
				return _pair.value();
			}
			inline void value(value_t val) {
				_pair.value(ang::move(val));
			}

			inline self_ptr_t& parent() {
				return _parent;
			}
			inline self_ptr_t const& parent()const {
				return _parent;
			}
			inline void parent(self_ptr_t n) {
				_parent = ang::move(n);
			}

			inline self_ptr_t& left() {
				return _left;
			}
			inline self_ptr_t const& left()const {
				return _left;
			}
			inline void left(self_ptr_t n) {
				_left = ang::move(n);
				if (_left) {
					_left->_type = 1;
					_left->parent(this);
				}
			}

			inline self_ptr_t& right() {
				return _right;
			}
			inline self_ptr_t const& right()const {
				return _right;
			}
			inline void right(self_ptr_t n) {
				_right = ang::move(n);
				if (_right) {
					_left->_type = 2;
					_right->parent(this);
				}
			}

			int _type = 0;
			int _balance = 0;
			pair_t _pair;
			self_ptr_t _parent = null;
			self_ptr_t _left = null;
			self_ptr_t _right = null;
		};

		template<typename K, typename T>
		class binary_three_data final
			: public object
			, public imap<K, T>
			, public memory::iallocator_client
		{
		public:
			typedef T							value_t;
			typedef K							key_t;
			typedef pair<K, T>					pair_t;
			typedef binary_three_data<K, T>				self_t;
			typedef icollection<pair<K, T>>		icollection_t;
			typedef ienum<pair<K, T>>			ienum_t;
			typedef imap<K, T>					imap_t;
			typedef iterator<pair<K, T >>		iterator_t;

		protected:
			typedef binary_three_node<K, T> node_t;
			typedef binary_three_node<K, T> *node_ptr_t;
			uint _size;
			node_ptr_t _root;
			memory::iallocator* allocator;

		public:
			inline binary_three_data();
			inline binary_three_data(std::initializer_list<pair<K, T>> list);
			inline binary_three_data(const std::nullptr_t&);
			inline binary_three_data(binary_three_data&& ar);
			inline binary_three_data(const binary_three_data& ar);
			inline binary_three_data(const binary_three_data* ar);
			inline binary_three_data(const ienum_t* store);

		private:
			virtual ~binary_three_data();

		public: //methods
			inline bool is_empty()const;
			inline void clean();
			inline bool move(binary_three_data&);

		public: //iallocator_client overrides
			inline void set_allocator(memory::iallocator*)override;
			inline memory::iallocator* get_allocator()const override;

		public: //ienum overrides
			inline pair_t& at(const iterator_t& it)const override;
			inline iterator_t begin()const override;
			inline iterator_t end()const override;
			inline bool next(iterator_t& it)const override;
			inline bool next(iterator_t& it, int offset)const override;
			inline bool prev(iterator_t& it)const override;
			inline bool prev(iterator_t& it, int offset)const override;
			inline uint counter()const override;

		public: //icollection overrides
			inline bool has_items()const override;
			inline bool copy(const ienum_t*) override;

		public: //imap overrides
			inline void extend(const ienum_t*) override;
			inline bool insert(K, T) override;
			inline bool insert(pair_t) override;
			inline bool update(K, T) override;
			inline bool update(pair<K, T>) override;
			inline bool remove(K const&) override;
			inline bool remove(K const&, T&) override;
			inline bool remove(iterator<pair<K, T>> it) override;
			inline bool remove(iterator<pair<K, T>> it, T&) override;
			inline bool has_key(const K&)const override;
			inline iterator<pair<K, T>> find(const K&)const override;

		private:
			inline iterator<pair<K, T>> find_index(index idx)const override { return iterator_t(const_cast<binary_three_data<K, T>*>(this), null); }
			inline index index_of(iterator<pair<K, T>> it)const override { return -1; }
			inline iterator<pair<K, T>> find(const pair<K, T>&, bool)const override { return iterator_t(const_cast<binary_three_data<K, T>*>(this), null); }
			inline iterator<pair<K, T>> find(const pair<K, T>&, iterator<pair<K, T>>, bool)const override { return iterator_t(const_cast<binary_three_data<K, T>*>(this), null); }

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool operator == (const binary_three_data& ar);
			inline bool operator != (const binary_three_data& ar);
			inline explicit operator T*()const;

		protected: //Memory Operations
			inline void destroy_tree(node_ptr_t)const;
			inline node_ptr_t find_node(key_t const&, node_ptr_t)const;
			inline node_ptr_t most_left(node_ptr_t)const;
			inline node_ptr_t most_right(node_ptr_t)const;
			inline bool realloc(memory::iallocator* alloc);
		};
	}

}


template<typename K, typename T>
inline ang::collections::binary_three_data<K, T>::binary_three_data()
	: _size(0)
	, _root(null)
	, allocator(ang::memory::allocator_manager::get_allocator(0))
{
}

template<typename K, typename T>
inline ang::collections::binary_three_data<K, T>::binary_three_data(ang::initializer_list_t<ang::collections::pair<K, T>> list)
	: binary_three_data()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<typename K, typename T>
inline ang::collections::binary_three_data<K, T>::binary_three_data(const ang::nullptr_t&)
	: binary_three_data()
{
}

template<typename K, typename T>
inline ang::collections::binary_three_data<K, T>::binary_three_data(binary_three_data&& other)
	: _size(0)
	, _root(null)
	, allocator(ang::memory::allocator_manager::get_allocator(0))
{
	_size = other._size;
	_root = other._root;
	allocator = other.allocator;

	other._size = 0;
	other._root = null;
}

template<typename K, typename T>
inline ang::collections::binary_three_data<K, T>::binary_three_data(const binary_three_data& other)
	: binary_three_data()
{
	extend(&other);
}

template<typename K, typename T>
inline ang::collections::binary_three_data<K, T>::binary_three_data(const binary_three_data* other)
	: binary_three_data()
{
	extend(other);
}

template<typename K, typename T>
inline ang::collections::binary_three_data<K, T>::binary_three_data(ang::collections::ienum<ang::collections::pair<K, T>> const* other)
	: binary_three_data()
{
	extend(other);
}

template<typename K, typename T>
ang::collections::binary_three_data<K, T>::~binary_three_data()
{
	clean();
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<binary_three_data< K, T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::imap<K, T>::is_kind_of(name))
		return true;
	return false;
}

template<typename K, typename T>
inline ang::type_name_t ang::collections::binary_three_data<K, T>::class_name()
{
	static string _type_name = ang::move("ang::collections::binary_three_data<"_o + type_name<K>() + ","_s + type_name<T>() + ">"_s);
	return (ang::cstr_t)_type_name;
}

template<typename K, typename T>
inline ang::type_name_t ang::collections::binary_three_data<K, T>::object_name()const
{
	return ang::collections::binary_three_data<K, T>::class_name();
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<binary_three_data<K, T>>())
	{
		*out = static_cast<ang::collections::binary_three_data<K, T>*>(this);
		return true;
	}
	else if (ang::object::query_object(name, out))
	{
		return true;
	}
	else if (ang::collections::imap<K, T>::query_object(name, out))
	{
		return true;
	}
	return false;
}


template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::is_empty()const {
	return _size == 0;
}

template<typename K, typename T>
inline void ang::collections::binary_three_data<K, T>::clean() {
	destroy_tree(_root);
	_root = null;
	_size = 0;
}

template<typename K, typename T>
inline void ang::collections::binary_three_data<K, T>::destroy_tree(ang::collections::binary_three_node<K,T>* node)const {
	if (node != NULL)
	{
		destroy_tree(node->left());
		node->left(null);

		destroy_tree(node->right());
		node->right(null);

		allocator->destruct(node);
		allocator->memory_release(node);
	}
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::move(binary_three_data& other)
{
	if (this == &other)
		return false;
	clean();
	_size = other._size;
	_root = other._root;
	allocator = other.allocator;
	other._size = 0;
	other._root = null;
	return true;
}


template<typename K, typename T>
inline void ang::collections::binary_three_data<K, T>::set_allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == allocator)
		return;
	if (!alloc->can_delete_from(allocator))
		realloc(alloc);
	allocator = alloc;
}

template<typename K, typename T>
inline ang::memory::iallocator* ang::collections::binary_three_data<K, T>::get_allocator()const
{
	return allocator;
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::realloc(ang::memory::iallocator* alloc)
{
	throw(exception_t(except_code::unsupported));
	return false;
}


template<typename K, typename T>
inline ang::collections::binary_three_node<K, T>* ang::collections::binary_three_data<K, T>::find_node(K const& key, ang::collections::binary_three_node<K, T>* node)const
{
	if (_size == 0)
		return null;
	if (node != null)
	{
		if (key == node->key())
			return node;
		if (key < node->key())
			return find_node(key, node->left());
		else
			return find_node(key, node->right());
	}
	return null;
}

template<typename K, typename T>
inline ang::collections::binary_three_node<K, T>* ang::collections::binary_three_data<K, T>::most_left(ang::collections::binary_three_node<K, T>* node)const
{
	if (node == null)
		return null;
	while (node->left() != null)
		node = node->left();
	return node;
}

template<typename K, typename T>
inline ang::collections::binary_three_node<K, T>* ang::collections::binary_three_data<K, T>::most_right(ang::collections::binary_three_node<K, T>* node)const
{
	if (node == null)
		return null;
	while (node->right() != null)
		node = node->right();
	return node;
}

template<typename K, typename T>
inline ang::collections::pair<K, T>& ang::collections::binary_three_data<K, T>::at(const ang::collections::iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw(exception_t(except_code::invalid_memory));
	if (it.current() == null || it.parent() != this)
		throw(exception_t(except_code::invalid_param));
#endif
	return node_ptr_t(it.current())->_pair;
}

template<typename K, typename T>
inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::binary_three_data<K, T>::begin()const
{
	return iterator_t(const_cast<binary_three_data*>(this), most_left(_root), 0);
}

template<typename K, typename T>
inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::binary_three_data<K, T>::end()const
{
	return iterator_t(const_cast<binary_three_data*>(this), most_right(_root), 0);
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::next(ang::collections::iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.current() == null || it.parent() != this)
		throw(exception_t(except_code::array_overflow));
#endif
	node_ptr_t node = node_ptr_t(it.current());


	if (node->right() != null)
	{
		it.current(most_left(node->right()));
		return true;
	}
	else
	{
		if (node->_type == 0 || node->parent() == null) {
			it.current(null);
			return false;
		}
		if (node->_type == 1)
		{
			it.current(node->parent());
			return true;
		}
		else if (node->_type == 2) {
			while (node->parent() && node->_type == 2)
				node = node->parent();

			it.current(node->parent());
			if (node->parent() == null)
				return false;
			else
				return true;
		}
	}
	it.current(null);
	return false;
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::next(ang::collections::iterator<ang::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.current() == null || it.parent() != this)
		throw(exception_t(except_code::array_overflow));
#endif

	node_ptr_t node = node_ptr_t(it.current());
	for (int i = 0; i < offset; ++i)
	{
		if (node == null) {
			it.current(null);
			return false;
		}
		if (node->right() != null)
		{
			node = most_left(node->right());
		}
		else
		{
			if (node->_type == 0 || node->parent() == null) {
				it.current(null);
				return false;
			}
			if (node->_type == 1)
			{
				node = node->parent();
				return true;
			}
			else if (node->_type == 2) {
				while (node->parent() && node->_type == 2)
					node = node->parent();
				node = node->parent();
			}
		}
	}
	it.current(node);
	return node != null;
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::prev(ang::collections::iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
	if (it.offset() >= _capacity)
		throw(exception_t(except_code::array_overflow));
#endif
	node_ptr_t node = node_ptr_t(it.current());

	if (node->left() != null)
	{
		it.current(most_right(node->right()));
		return true;
	}
	else
	{
		if (node->_type == 0 || node->parent() == null) {
			it.current(null);
			return false;
		}
		if (node->_type == 1)
		{
			it.current(node->parent());
			return true;
		}
		else if (node->_type == 2) {
			while (node->parent() && node->_type == 2)
				node = node->parent();

			it.current(node->parent());
			if (node->parent() == null)
				return false;
			else
				return true;
		}
	}
	it.current(null);
	return false;
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::prev(ang::collections::iterator<ang::collections::pair<K, T>>& it, int offset)const
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
inline uint ang::collections::binary_three_data<K, T>::counter()const
{
	return _size;
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::has_items()const {
	return _size > 0;
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::copy(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return false;
	clean();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename K, typename T>
inline void ang::collections::binary_three_data<K, T>::extend(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::insert(K key, T value)
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
inline bool ang::collections::binary_three_data<K, T>::insert(ang::collections::pair<K, T> pair)
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
inline bool ang::collections::binary_three_data<K, T>::update(K key, T value)
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
inline bool ang::collections::binary_three_data<K, T>::update(ang::collections::pair<K, T> pair)
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
inline bool ang::collections::binary_three_data<K, T>::remove(K const& key)
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
inline bool ang::collections::binary_three_data<K, T>::remove(K const& key, T& value)
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
inline bool ang::collections::binary_three_data<K, T>::remove(ang::collections::iterator<ang::collections::pair<K, T>>)
{
	return false;
}

template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::remove(ang::collections::iterator<ang::collections::pair<K, T>>, T& value)
{

	return false;
}


template<typename K, typename T>
inline bool ang::collections::binary_three_data<K, T>::has_key(const K& key)const
{
	return find_node(key) != null;
}

template<typename K, typename T>
inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::binary_three_data<K, T>::find(const K& key)const
{
	if (_table == null)
		return iterator_t(const_cast<binary_three_data*>(this));

	uint idx = hash_index(key);
	node_ptr_t temp = _table[idx];
	while (temp != null)
	{
		if (key == temp->key())
			return iterator_t(const_cast<binary_three_data*>(this), temp, idx);
		temp = temp->next();
	}
	return iterator_t(const_cast<binary_three_data*>(this), null, 0);
}

