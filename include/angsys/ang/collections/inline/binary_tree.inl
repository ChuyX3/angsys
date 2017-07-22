#ifndef __ANG_BINARY_TREE_H__
#error Can't include binary_tree.hpp, please include binary_tree.h inside
#elif !defined __ANG_BINARY_TREE_HPP__
#define __ANG_BINARY_TREE_HPP__

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K,T>::begin(ang::collections::binary_tree_node<K, T>* node, index& idx)
{
	return most_left(node, idx);
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K, T>::end(ang::collections::binary_tree_node<K, T>* node, index& idx)
{
	return most_right(node, idx);
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K, T>::next(ang::collections::binary_tree_node<K, T>* node, index& idx)
{
	if (node == null)
		return null;

	if (node->right() != null)	
		return most_left(node->right(), idx);
	else
	{
		if (node->type() == 1)	
			return node->parent();	
		else if (node->type() == 2) {
			while (node->parent() && node->type() == 2)
				node = node->parent();
			return node->parent();
		}
	}
	return null;
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K, T>::prev(ang::collections::binary_tree_node<K, T>* node, index& idx)
{
	if (node == null)
		return null;

	if (node->left() != null)
		return most_right(node->right(), idx);
	else
	{
		if (node->type() == 1)
			return node->parent();
		else if (node->type() == 2) {
			while (node->parent() && node->type() == 2)
				node = node->parent();
			return node->parent();
		}
	}
	return null;
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K, T>::most_left(ang::collections::binary_tree_node<K, T>* node, index& idx)
{
	while (node && node->left())
		node = node->left();
	return node;
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K, T>::most_right(ang::collections::binary_tree_node<K, T>* node, index& idx)
{
	while (node && node->right())
		node = node->right();
	return node;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T>
inline ang::collections::binary_tree_data<K, T>::binary_tree_data()
	: _count(0)
	, _root(null)
	, allocator(ang::memory::allocator_manager::get_allocator(0))
	, algorithm(ang::collections::algorithms::binary_tree_inorder_iteration<K, T>())
{
}

template<typename K, typename T>
inline ang::collections::binary_tree_data<K, T>::binary_tree_data(ang::initializer_list_t<ang::collections::pair<K, T>> list)
	: binary_tree_data()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<typename K, typename T>
inline ang::collections::binary_tree_data<K, T>::binary_tree_data(const ang::nullptr_t&)
	: binary_tree_data()
{
}

template<typename K, typename T>
inline ang::collections::binary_tree_data<K, T>::binary_tree_data(binary_tree_data&& other)
	: binary_tree_data()
{
	_count = other._count;
	_root = other._root;
	allocator = other.allocator;

	other._count = 0;
	other._root = null;
}

template<typename K, typename T>
inline ang::collections::binary_tree_data<K, T>::binary_tree_data(const binary_tree_data& other)
	: binary_tree_data()
{
	extend(&other);
}

template<typename K, typename T>
inline ang::collections::binary_tree_data<K, T>::binary_tree_data(const binary_tree_data* other)
	: binary_tree_data()
{
	extend(other);
}

template<typename K, typename T>
inline ang::collections::binary_tree_data<K, T>::binary_tree_data(ang::collections::ienum<ang::collections::pair<K, T>> const* other)
	: binary_tree_data()
{
	extend(other);
}

template<typename K, typename T>
ang::collections::binary_tree_data<K, T>::~binary_tree_data()
{
	clean();
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<binary_tree_data< K, T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::imap<K, T>::is_kind_of(name))
		return true;
	return false;
}

template<typename K, typename T>
inline ang::type_name_t ang::collections::binary_tree_data<K, T>::class_name()
{
	static string _type_name = ang::move("ang::collections::binary_tree_data<"_o + type_name<K>() + ","_s + type_name<T>() + ">"_s);
	return (ang::cstr_t)_type_name;
}

template<typename K, typename T>
inline ang::type_name_t ang::collections::binary_tree_data<K, T>::object_name()const
{
	return ang::collections::binary_tree_data<K, T>::class_name();
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<binary_tree_data<K, T>>())
	{
		*out = static_cast<ang::collections::binary_tree_data<K, T>*>(this);
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
inline bool ang::collections::binary_tree_data<K, T>::is_empty()const {
	return _count == 0;
}

template<typename K, typename T>
inline void ang::collections::binary_tree_data<K, T>::clean() {
	destroy_tree(_root);
	_root = null;
	_count = 0;
}

template<typename K, typename T>
inline void ang::collections::binary_tree_data<K, T>::destroy_tree(ang::collections::binary_tree_node<K, T>* node)const {
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
inline bool ang::collections::binary_tree_data<K, T>::move(binary_tree_data& other)
{
	if (this == &other)
		return false;
	clean();
	_count = other._count;
	_root = other._root;
	allocator = other.allocator;
	other._count = 0;
	other._root = null;
	return true;
}

template<typename K, typename T>
inline void ang::collections::binary_tree_data<K, T>::iteration_method(ang::collections::algorithms::iteration_method_t method)
{
	switch (method)
	{
	case algorithms::iteration_method::inorder:
		algorithm = algorithms::binary_tree_inorder_iteration();
	}
}

template<typename K, typename T>
inline void ang::collections::binary_tree_data<K, T>::set_allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == allocator)
		return;
	if (!alloc->can_delete_from(allocator))
		realloc(alloc);
	allocator = alloc;
}

template<typename K, typename T>
inline ang::memory::iallocator* ang::collections::binary_tree_data<K, T>::get_allocator()const
{
	return allocator;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::realloc(ang::memory::iallocator* alloc)
{
	throw(exception_t(except_code::unsupported));
	return false;
}


template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_data<K, T>::find_node(K const& key, ang::collections::binary_tree_node<K, T>* node)const
{
	if (_count == 0)
		return null;
	while (node != null)
	{
		if (key == node->key())
			return node;
		if (key < node->key())
			node = node->left();
		else
			node = node->right();
	}
	return null;
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_data<K, T>::most_left(ang::collections::binary_tree_node<K, T>* node)const
{
	if (node == null)
		return null;
	while (node->left() != null)
		node = node->left();
	return node;
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_data<K, T>::most_right(ang::collections::binary_tree_node<K, T>* node)const
{
	if (node == null)
		return null;
	while (node->right() != null)
		node = node->right();
	return node;
}

template<typename K, typename T>
inline ang::collections::pair<K, T>& ang::collections::binary_tree_data<K, T>::at(const ang::collections::iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	return node_ptr_t(it.current())->_pair;
}

template<typename K, typename T>
inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_data<K, T>::begin()const
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_data*>(this), algorithm.begin(_root, idx), 0);
}

template<typename K, typename T>
inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_data<K, T>::end()const
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_data*>(this), algorithm.end(_root, idx), 0);
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::next(ang::collections::iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = algorithm.next(node_ptr_t(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::next(ang::collections::iterator<ang::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = node_ptr_t(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = algorithm.next(node, idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::prev(ang::collections::iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = algorithm.prev(node_ptr_t(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::prev(ang::collections::iterator<ang::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = node_ptr_t(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = algorithm.prev(node, idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T>
inline uint ang::collections::binary_tree_data<K, T>::counter()const
{
	return _count;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::has_items()const {
	return _count > 0;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::copy(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return false;
	clean();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename K, typename T>
inline void ang::collections::binary_tree_data<K, T>::extend(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::insert(K key, T value)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, ang::move(key), ang::move(value));
		return true;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
			return false;
	}
	node_ptr_t parent;
	while (node->parent())
	{	
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return true;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::insert(ang::collections::pair<K, T> pair)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, ang::move(pair));
		return true;
	}
	node_ptr_t node = _root;
	K key = ang::move(pair.key_value());
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
			return false;
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return true;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::update(K key, T value)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, ang::move(key), ang::move(value));
		return true;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
		{
			node->value(ang::move(value));
			return true; //the key is updated
		}
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return false; //a new key was added
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::update(ang::collections::pair<K, T> pair)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, ang::move(pair));
		return true;
	}
	node_ptr_t node = _root;
	K key = ang::move(pair.key_value());
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
		{
			node->value(ang::move(pair.value()));
			return true; //the key is updated
		}
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return false; //a new key was added
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::remove(K const& key)
{
	bool removed = false;
	_root = remove_node(_root, key, removed);
	_root->parent(null);
	return removed;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::remove(K const& key, T& value)
{
	bool removed = false;
	_root = remove_node(_root, key, value, removed);
	_root->parent(null);
	return removed;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::remove(ang::collections::iterator<ang::collections::pair<K, T>> it)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	bool removed = false;
	if (node == _root)
	{
		_root = remove_node(node, node->key(), removed);
		_root->parent(null);
		return removed;
	}
	else
	{
		switch (node->type())
		{
		case 1:
			node->parent()->left(remove_node(node, node->key(), removed));
			return removed;
		case 2:
			node->parent()->right(remove_node(node, node->key(), removed));
			return removed;
		}
	}
	return false;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::remove(ang::collections::iterator<ang::collections::pair<K, T>> it, T& value)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	bool removed = false;
	if (node == _root)
	{
		_root = remove_node(node, node->key(), value, removed);
		return removed;
	}
	else
	{
		switch (node->type())
		{
		case 1:
			node->parent()->left(remove_node(node, node->key(), value, removed));
			return removed;
		case 2:
			node->parent()->right(remove_node(node, node->key(), value, removed));
			return removed;
		}
	}
	return false;
}

template<typename K, typename T>
inline bool ang::collections::binary_tree_data<K, T>::has_key(const K& key)const
{
	return find_node(key, _root) != null;
}

template<typename K, typename T>
inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_data<K, T>::find(const K& key)const
{
	return iterator_t(const_cast<binary_tree_data*>(this), null, 0);
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K,T>* ang::collections::binary_tree_data<K, T>::right_rotate(ang::collections::binary_tree_node<K, T>* y)
{
	node_ptr_t x = y->left();
	y->left(x->right());
	x->right(y);
	y->height(max(y->left()->height(), y->right()->height()) + 1);
	x->height(max(x->left()->height(), x->right()->height()) + 1);
	return x;
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_data<K, T>::left_rotate(ang::collections::binary_tree_node<K, T>* x)
{
	node_ptr_t y = x->right();
	x->right(y->left());
	y->left(x);
	x->height(max(x->left()->height(), x->right()->height()) + 1);
	y->height(max(y->left()->height(), y->right()->height()) + 1);
	return y;
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_data<K, T>::remove_node(ang::collections::binary_tree_node<K, T>* node, key_t const& key, bool& removed)
{
	if (node == null)
		return null;

	if (key < node->key())
		node->left(remove_node(node->left(), key, removed));
	else if (key > node->key())
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		if (!node->left() || !node->right())
		{
			node_ptr_t temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(ang::move(temp->key()));
				node->value(ang::move(temp->value()));

				allocator->destruct(temp);
				allocator->memory_release(temp);
			}
			else
			{
				allocator->destruct(node);
				allocator->memory_release(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_t temp = most_left(node->right());
			node->key(temp->key());
			node->value(ang::move(temp->value()));
			node->right(remove_node(node->right(), temp->key(), _removed));
		}

	}
	node->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();

	if (balance > 1 && node->left()->balance() >= 0)
		return right_rotate(node);
	else if (balance > 1 && node->left()->balance() < 0)
	{
		node->left(left_rotate(node->left()));
		return right_rotate(node);
	}
	else if (balance < -1 && node->right()->balance() <= 0)
		return left_rotate(node);
	else if (balance < -1 && node->right()->balance() > 0)
	{
		node->right(right_rotate(node->right()));
		return left_rotate(node);
	}
	return node;
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_data<K, T>::remove_node(ang::collections::binary_tree_node<K, T>* node, key_t const& key, T& value, bool& removed)
{
	if (node == null)
		return null;

	if (key < node->key())
		node->left(remove_node(node->left(), key, removed));
	else if (key > node->key())
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		value = ang::move(node->value());
		if (!node->left() || !node->right())
		{
			node_ptr_t temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(ang::move(temp->key()));
				node->value(ang::move(temp->value()));

				allocator->destruct(temp);
				allocator->memory_release(temp);
			}
			else
			{
				allocator->destruct(node);
				allocator->memory_release(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_t temp = most_left(node->right());
			node->key(temp->key());
			node->value(ang::move(temp->value()));
			node->right(remove_node(node->right(), temp->key(), _removed));
		}

	}
	node->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();

	if (balance > 1 && node->left()->balance() >= 0)
		return right_rotate(node);
	else if (balance > 1 && node->left()->balance() < 0)
	{
		node->left(left_rotate(node->left()));
		return right_rotate(node);
	}
	else if (balance < -1 && node->right()->balance() <= 0)
		return left_rotate(node);
	else if (balance < -1 && node->right()->balance() > 0)
	{
		node->right(right_rotate(node->right()));
		return left_rotate(node);
	}
	return node;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline ang::collections::binary_tree_data<ang::string, T>::binary_tree_data()
	: _count(0)
	, _root(null)
	, allocator(ang::memory::allocator_manager::get_allocator(0))
	, algorithm(ang::collections::algorithms::binary_tree_inorder_iteration<ang::string, T>())
{
}

template<typename T>
inline ang::collections::binary_tree_data<ang::string, T>::binary_tree_data(ang::initializer_list_t<ang::collections::pair<ang::string, T>> list)
	: binary_tree_data()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<typename T>
inline ang::collections::binary_tree_data<ang::string, T>::binary_tree_data(const ang::nullptr_t&)
	: binary_tree_data()
{
}

template<typename T>
inline ang::collections::binary_tree_data<ang::string, T>::binary_tree_data(binary_tree_data&& other)
	: binary_tree_data()
{
	_count = other._count;
	_root = other._root;
	allocator = other.allocator;

	other._count = 0;
	other._root = null;
}

template<typename T>
inline ang::collections::binary_tree_data<ang::string, T>::binary_tree_data(const binary_tree_data& other)
	: binary_tree_data()
{
	extend(&other);
}

template<typename T>
inline ang::collections::binary_tree_data<ang::string, T>::binary_tree_data(const binary_tree_data* other)
	: binary_tree_data()
{
	extend(other);
}

template<typename T>
inline ang::collections::binary_tree_data<ang::string, T>::binary_tree_data(ang::collections::ienum<ang::collections::pair<ang::string, T>> const* other)
	: binary_tree_data()
{
	extend(other);
}

template<typename T>
ang::collections::binary_tree_data<ang::string, T>::~binary_tree_data()
{
	clean();
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<binary_tree_data< ang::string, T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::imap<ang::string, T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T>
inline ang::type_name_t ang::collections::binary_tree_data<ang::string, T>::class_name()
{
	static string _type_name = ang::move("ang::collections::binary_tree_data<"_o + type_name<ang::string>() + ","_s + type_name<T>() + ">"_s);
	return (ang::cstr_t)_type_name;
}

template<typename T>
inline ang::type_name_t ang::collections::binary_tree_data<ang::string, T>::object_name()const
{
	return ang::collections::binary_tree_data<ang::string, T>::class_name();
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<binary_tree_data<ang::string, T>>())
	{
		*out = static_cast<ang::collections::binary_tree_data<ang::string, T>*>(this);
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
inline bool ang::collections::binary_tree_data<ang::string, T>::is_empty()const {
	return _count == 0;
}

template<typename T>
inline void ang::collections::binary_tree_data<ang::string, T>::clean() {
	destroy_tree(_root);
	_root = null;
	_count = 0;
}

template<typename T>
inline void ang::collections::binary_tree_data<ang::string, T>::destroy_tree(ang::collections::binary_tree_node<ang::string, T>* node)const {
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

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::move(binary_tree_data& other)
{
	if (this == &other)
		return false;
	clean();
	_count = other._count;
	_root = other._root;
	allocator = other.allocator;
	other._count = 0;
	other._root = null;
	return true;
}


template<typename T>
inline void ang::collections::binary_tree_data<ang::string, T>::set_allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == allocator)
		return;
	if (!alloc->can_delete_from(allocator))
		realloc(alloc);
	allocator = alloc;
}

template<typename T>
inline ang::memory::iallocator* ang::collections::binary_tree_data<ang::string, T>::get_allocator()const
{
	return allocator;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::realloc(ang::memory::iallocator* alloc)
{
	throw(exception_t(except_code::unsupported));
	return false;
}


template<typename T>
inline ang::collections::binary_tree_node<ang::string, T>* ang::collections::binary_tree_data<ang::string, T>::find_node(ang::cstr_t key, ang::collections::binary_tree_node<ang::string, T>* node)const
{
	if (_count == 0)
		return null;
	while (node != null)
	{
		if (key == node->key())
			return node;
		if (key < node->key())
			node = node->left();
		else
			node = node->right();
	}
	return null;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::string, T>* ang::collections::binary_tree_data<ang::string, T>::find_node(ang::cwstr_t key, ang::collections::binary_tree_node<ang::string, T>* node)const
{
	if (_count == 0)
		return null;
	while (node != null)
	{
		if (key == node->key())
			return node;
		if (key < node->key())
			node = node->left();
		else
			node = node->right();
	}
	return null;
}


template<typename T>
inline ang::collections::binary_tree_node<ang::string, T>* ang::collections::binary_tree_data<ang::string, T>::most_left(ang::collections::binary_tree_node<ang::string, T>* node)const
{
	if (node == null)
		return null;
	while (node->left() != null)
		node = node->left();
	return node;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::string, T>* ang::collections::binary_tree_data<ang::string, T>::most_right(ang::collections::binary_tree_node<ang::string, T>* node)const
{
	if (node == null)
		return null;
	while (node->right() != null)
		node = node->right();
	return node;
}

template<typename T>
inline ang::collections::pair<ang::string, T>& ang::collections::binary_tree_data<ang::string, T>::at(const ang::collections::iterator<ang::collections::pair<ang::string, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	return node_ptr_t(it.current())->_pair;
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::string, T>> ang::collections::binary_tree_data<ang::string, T>::begin()const
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_data*>(this), algorithm.begin(_root, idx), 0);
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::string, T>> ang::collections::binary_tree_data<ang::string, T>::end()const
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_data*>(this), algorithm.end(_root, idx), 0);
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::next(ang::collections::iterator<ang::collections::pair<ang::string, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = algorithm.next(node_ptr_t(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::next(ang::collections::iterator<ang::collections::pair<ang::string, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = node_ptr_t(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = algorithm.next(node, idx);
	it.current(node);
	return node != null;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::prev(ang::collections::iterator<ang::collections::pair<ang::string, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = algorithm.prev(node_ptr_t(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::prev(ang::collections::iterator<ang::collections::pair<ang::string, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = node_ptr_t(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = algorithm.prev(node, idx);
	it.current(node);
	return node != null;
}

template<typename T>
inline uint ang::collections::binary_tree_data<ang::string, T>::counter()const
{
	return _count;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::has_items()const {
	return _count > 0;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::copy(ang::collections::ienum<ang::collections::pair<ang::string, T>>const* items) {

	if (items == null)
		return false;
	clean();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename T>
inline void ang::collections::binary_tree_data<ang::string, T>::extend(ang::collections::ienum<ang::collections::pair<ang::string, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::insert(ang::cstr_t key, T value)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, key, ang::move(value));
		return true;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
			return false;
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return true;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::insert(ang::cwstr_t key, T value)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, key, ang::move(value));
		return true;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
			return false;
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return true;
}


template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::insert(ang::collections::pair<ang::string, T> pair)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, ang::move(pair));
		return true;
	}
	node_ptr_t node = _root;
	ang::string key = ang::move(pair.key_value());
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
			return false;
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return true;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::update(ang::cstr_t key, T value)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, key, ang::move(value));
		return true;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
		{
			node->value(ang::move(value));
			return true; //the key is updated
		}
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return false; //a new key was added
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::update(ang::cwstr_t key, T value)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, key, ang::move(value));
		return true;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
		{
			node->value(ang::move(value));
			return true; //the key is updated
		}
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return false; //a new key was added
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::update(ang::collections::pair<ang::string, T> pair)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, ang::move(pair));
		return true;
	}
	node_ptr_t node = _root;
	ang::string key = ang::move(pair.key_value());
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
		{
			node->value(ang::move(pair.value()));
			return true; //the key is updated
		}
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return false; //a new key was added
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::remove(ang::cstr_t key)
{
	bool removed = false;
	_root = remove_node(_root, key, removed);
	_root->parent(null);
	return removed;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::remove(ang::cwstr_t key)
{
	bool removed = false;
	_root = remove_node(_root, key, removed);
	_root->parent(null);
	return removed;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::remove(ang::cstr_t key, T& value)
{
	bool removed = false;
	_root = remove_node(_root, key, value, removed);
	_root->parent(null);
	return removed;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::remove(ang::cwstr_t key, T& value)
{
	bool removed = false;
	_root = remove_node(_root, key, value, removed);
	_root->parent(null);
	return removed;
}


template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::remove(ang::collections::iterator<ang::collections::pair<ang::string, T>> it)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	bool removed = false;
	if (node == _root)
	{
		_root = remove_node(node, node->key(), removed);
		_root->parent(null);
		return removed;
	}
	else
	{
		switch (node->type())
		{
		case 1:
			node->parent()->left(remove_node(node, node->key(), removed));
			return removed;
		case 2:
			node->parent()->right(remove_node(node, node->key(), removed));
			return removed;
		}
	}
	return false;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::remove(ang::collections::iterator<ang::collections::pair<ang::string, T>> it, T& value)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	bool removed = false;
	if (node == _root)
	{
		_root = remove_node(node, node->key(), value, removed);
		return removed;
	}
	else
	{
		switch (node->type())
		{
		case 1:
			node->parent()->left(remove_node(node, node->key(), value, removed));
			return removed;
		case 2:
			node->parent()->right(remove_node(node, node->key(), value, removed));
			return removed;
		}
	}
	return false;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::has_key(ang::cstr_t key)const
{
	return find_node(key, _root) != null;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::string, T>::has_key(ang::cwstr_t key)const
{
	return find_node(key, _root) != null;
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::string, T>> ang::collections::binary_tree_data<ang::string, T>::find(ang::cstr_t key)const
{
	return iterator_t(const_cast<binary_tree_data*>(this), null, 0);
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::string, T>> ang::collections::binary_tree_data<ang::string, T>::find(ang::cwstr_t key)const
{
	return iterator_t(const_cast<binary_tree_data*>(this), null, 0);
}

template<typename T>
inline ang::collections::binary_tree_node<ang::string, T>* ang::collections::binary_tree_data<ang::string, T>::right_rotate(ang::collections::binary_tree_node<ang::string, T>* y)
{
	node_ptr_t x = y->left();
	y->left(x->right());
	x->right(y);
	y->height(max(y->left()->height(), y->right()->height()) + 1);
	x->height(max(x->left()->height(), x->right()->height()) + 1);
	return x;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::string, T>* ang::collections::binary_tree_data<ang::string, T>::left_rotate(ang::collections::binary_tree_node<ang::string, T>* x)
{
	node_ptr_t y = x->right();
	x->right(y->left());
	y->left(x);
	x->height(max(x->left()->height(), x->right()->height()) + 1);
	y->height(max(y->left()->height(), y->right()->height()) + 1);
	return y;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::string, T>* ang::collections::binary_tree_data<ang::string, T>::remove_node(ang::collections::binary_tree_node<ang::string, T>* node, ang::cstr_t key, bool& removed)
{
	if (node == null)
		return null;

	if (key < node->key())
		node->left(remove_node(node->left(), key, removed));
	else if (key > node->key())
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		if (!node->left() || !node->right())
		{
			node_ptr_t temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(ang::move(temp->key()));
				node->value(ang::move(temp->value()));

				allocator->destruct(temp);
				allocator->memory_release(temp);
			}
			else
			{
				allocator->destruct(node);
				allocator->memory_release(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_t temp = most_left(node->right());
			node->key(temp->key());
			node->value(ang::move(temp->value()));
			node->right(remove_node(node->right(), temp->key(), _removed));
		}

	}
	node->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();

	if (balance > 1 && node->left()->balance() >= 0)
		return right_rotate(node);
	else if (balance > 1 && node->left()->balance() < 0)
	{
		node->left(left_rotate(node->left()));
		return right_rotate(node);
	}
	else if (balance < -1 && node->right()->balance() <= 0)
		return left_rotate(node);
	else if (balance < -1 && node->right()->balance() > 0)
	{
		node->right(right_rotate(node->right()));
		return left_rotate(node);
	}
	return node;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::string, T>* ang::collections::binary_tree_data<ang::string, T>::remove_node(ang::collections::binary_tree_node<ang::string, T>* node, ang::cwstr_t key, bool& removed)
{
	if (node == null)
		return null;

	if (key < node->key())
		node->left(remove_node(node->left(), key, removed));
	else if (key > node->key())
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		if (!node->left() || !node->right())
		{
			node_ptr_t temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(ang::move(temp->key()));
				node->value(ang::move(temp->value()));

				allocator->destruct(temp);
				allocator->memory_release(temp);
			}
			else
			{
				allocator->destruct(node);
				allocator->memory_release(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_t temp = most_left(node->right());
			node->key(temp->key());
			node->value(ang::move(temp->value()));
			node->right(remove_node(node->right(), temp->key(), _removed));
		}

	}
	node->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();

	if (balance > 1 && node->left()->balance() >= 0)
		return right_rotate(node);
	else if (balance > 1 && node->left()->balance() < 0)
	{
		node->left(left_rotate(node->left()));
		return right_rotate(node);
	}
	else if (balance < -1 && node->right()->balance() <= 0)
		return left_rotate(node);
	else if (balance < -1 && node->right()->balance() > 0)
	{
		node->right(right_rotate(node->right()));
		return left_rotate(node);
	}
	return node;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::string, T>* ang::collections::binary_tree_data<ang::string, T>::remove_node(ang::collections::binary_tree_node<ang::string, T>* node, ang::cstr_t key, T& value, bool& removed)
{
	if (node == null)
		return null;

	if (key < node->key())
		node->left(remove_node(node->left(), key, removed));
	else if (key > node->key())
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		value = ang::move(node->value());
		if (!node->left() || !node->right())
		{
			node_ptr_t temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(ang::move(temp->key()));
				node->value(ang::move(temp->value()));

				allocator->destruct(temp);
				allocator->memory_release(temp);
			}
			else
			{
				allocator->destruct(node);
				allocator->memory_release(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_t temp = most_left(node->right());
			node->key(temp->key());
			node->value(ang::move(temp->value()));
			node->right(remove_node(node->right(), temp->key(), _removed));
		}

	}
	node->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();

	if (balance > 1 && node->left()->balance() >= 0)
		return right_rotate(node);
	else if (balance > 1 && node->left()->balance() < 0)
	{
		node->left(left_rotate(node->left()));
		return right_rotate(node);
	}
	else if (balance < -1 && node->right()->balance() <= 0)
		return left_rotate(node);
	else if (balance < -1 && node->right()->balance() > 0)
	{
		node->right(right_rotate(node->right()));
		return left_rotate(node);
	}
	return node;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::string, T>* ang::collections::binary_tree_data<ang::string, T>::remove_node(ang::collections::binary_tree_node<ang::string, T>* node, ang::cwstr_t key, T& value, bool& removed)
{
	if (node == null)
		return null;

	if (key < node->key())
		node->left(remove_node(node->left(), key, removed));
	else if (key > node->key())
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		value = ang::move(node->value());
		if (!node->left() || !node->right())
		{
			node_ptr_t temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(ang::move(temp->key()));
				node->value(ang::move(temp->value()));

				allocator->destruct(temp);
				allocator->memory_release(temp);
			}
			else
			{
				allocator->destruct(node);
				allocator->memory_release(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_t temp = most_left(node->right());
			node->key(temp->key());
			node->value(ang::move(temp->value()));
			node->right(remove_node(node->right(), temp->key(), _removed));
		}

	}
	node->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();

	if (balance > 1 && node->left()->balance() >= 0)
		return right_rotate(node);
	else if (balance > 1 && node->left()->balance() < 0)
	{
		node->left(left_rotate(node->left()));
		return right_rotate(node);
	}
	else if (balance < -1 && node->right()->balance() <= 0)
		return left_rotate(node);
	else if (balance < -1 && node->right()->balance() > 0)
	{
		node->right(right_rotate(node->right()));
		return left_rotate(node);
	}
	return node;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::collections::binary_tree_data<ang::wstring, T>::binary_tree_data()
	: _count(0)
	, _root(null)
	, allocator(ang::memory::allocator_manager::get_allocator(0))
	, algorithm(ang::collections::algorithms::binary_tree_inorder_iteration<ang::wstring, T>())
{
}

template<typename T>
inline ang::collections::binary_tree_data<ang::wstring, T>::binary_tree_data(ang::initializer_list_t<ang::collections::pair<ang::wstring, T>> list)
	: binary_tree_data()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<typename T>
inline ang::collections::binary_tree_data<ang::wstring, T>::binary_tree_data(const ang::nullptr_t&)
	: binary_tree_data()
{
}

template<typename T>
inline ang::collections::binary_tree_data<ang::wstring, T>::binary_tree_data(binary_tree_data&& other)
	: binary_tree_data()
{
	_count = other._count;
	_root = other._root;
	allocator = other.allocator;

	other._count = 0;
	other._root = null;
}

template<typename T>
inline ang::collections::binary_tree_data<ang::wstring, T>::binary_tree_data(const binary_tree_data& other)
	: binary_tree_data()
{
	extend(&other);
}

template<typename T>
inline ang::collections::binary_tree_data<ang::wstring, T>::binary_tree_data(const binary_tree_data* other)
	: binary_tree_data()
{
	extend(other);
}

template<typename T>
inline ang::collections::binary_tree_data<ang::wstring, T>::binary_tree_data(ang::collections::ienum<ang::collections::pair<ang::wstring, T>> const* other)
	: binary_tree_data()
{
	extend(other);
}

template<typename T>
ang::collections::binary_tree_data<ang::wstring, T>::~binary_tree_data()
{
	clean();
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<binary_tree_data< ang::wstring, T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::imap<ang::wstring, T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T>
inline ang::type_name_t ang::collections::binary_tree_data<ang::wstring, T>::class_name()
{
	static string _type_name = ang::move("ang::collections::binary_tree_data<"_o + type_name<string>() + ","_s + type_name<T>() + ">"_s);
	return (ang::cstr_t)_type_name;
}

template<typename T>
inline ang::type_name_t ang::collections::binary_tree_data<ang::wstring, T>::object_name()const
{
	return ang::collections::binary_tree_data<ang::wstring, T>::class_name();
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<binary_tree_data<ang::wstring, T>>())
	{
		*out = static_cast<ang::collections::binary_tree_data<ang::wstring, T>*>(this);
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
inline bool ang::collections::binary_tree_data<ang::wstring, T>::is_empty()const {
	return _count == 0;
}

template<typename T>
inline void ang::collections::binary_tree_data<ang::wstring, T>::clean() {
	destroy_tree(_root);
	_root = null;
	_count = 0;
}

template<typename T>
inline void ang::collections::binary_tree_data<ang::wstring, T>::destroy_tree(ang::collections::binary_tree_node<ang::wstring, T>* node)const {
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

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::move(binary_tree_data& other)
{
	if (this == &other)
		return false;
	clean();
	_count = other._count;
	_root = other._root;
	allocator = other.allocator;
	other._count = 0;
	other._root = null;
	return true;
}


template<typename T>
inline void ang::collections::binary_tree_data<ang::wstring, T>::set_allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == allocator)
		return;
	if (!alloc->can_delete_from(allocator))
		realloc(alloc);
	allocator = alloc;
}

template<typename T>
inline ang::memory::iallocator* ang::collections::binary_tree_data<ang::wstring, T>::get_allocator()const
{
	return allocator;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::realloc(ang::memory::iallocator* alloc)
{
	throw(exception_t(except_code::unsupported));
	return false;
}


template<typename T>
inline ang::collections::binary_tree_node<ang::wstring, T>* ang::collections::binary_tree_data<ang::wstring, T>::find_node(ang::cstr_t key, ang::collections::binary_tree_node<ang::wstring, T>* node)const
{
	if (_count == 0)
		return null;
	while (node != null)
	{
		if (key == node->key())
			return node;
		if (key < node->key())
			node = node->left();
		else
			node = node->right();
	}
	return null;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::wstring, T>* ang::collections::binary_tree_data<ang::wstring, T>::find_node(ang::cwstr_t key, ang::collections::binary_tree_node<ang::wstring, T>* node)const
{
	if (_count == 0)
		return null;
	while (node != null)
	{
		if (key == node->key())
			return node;
		if (key < node->key())
			node = node->left();
		else
			node = node->right();
	}
	return null;
}


template<typename T>
inline ang::collections::binary_tree_node<ang::wstring, T>* ang::collections::binary_tree_data<ang::wstring, T>::most_left(ang::collections::binary_tree_node<ang::wstring, T>* node)const
{
	if (node == null)
		return null;
	while (node->left() != null)
		node = node->left();
	return node;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::wstring, T>* ang::collections::binary_tree_data<ang::wstring, T>::most_right(ang::collections::binary_tree_node<ang::wstring, T>* node)const
{
	if (node == null)
		return null;
	while (node->right() != null)
		node = node->right();
	return node;
}

template<typename T>
inline ang::collections::pair<ang::wstring, T>& ang::collections::binary_tree_data<ang::wstring, T>::at(const ang::collections::iterator<ang::collections::pair<ang::wstring, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	return node_ptr_t(it.current())->_pair;
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::wstring, T>> ang::collections::binary_tree_data<ang::wstring, T>::begin()const
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_data*>(this), algorithm.begin(_root, idx), 0);
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::wstring, T>> ang::collections::binary_tree_data<ang::wstring, T>::end()const
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_data*>(this), algorithm.end(_root, idx), 0);
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::next(ang::collections::iterator<ang::collections::pair<ang::wstring, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = algorithm.next(node_ptr_t(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::next(ang::collections::iterator<ang::collections::pair<ang::wstring, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = node_ptr_t(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = algorithm.next(node, idx);
	it.current(node);
	return node != null;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::prev(ang::collections::iterator<ang::collections::pair<ang::wstring, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = algorithm.prev(node_ptr_t(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::prev(ang::collections::iterator<ang::collections::pair<ang::wstring, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = node_ptr_t(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = algorithm.prev(node, idx);
	it.current(node);
	return node != null;
}

template<typename T>
inline uint ang::collections::binary_tree_data<ang::wstring, T>::counter()const
{
	return _count;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::has_items()const {
	return _count > 0;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::copy(ang::collections::ienum<ang::collections::pair<ang::wstring, T>>const* items) {

	if (items == null)
		return false;
	clean();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename T>
inline void ang::collections::binary_tree_data<ang::wstring, T>::extend(ang::collections::ienum<ang::collections::pair<ang::wstring, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::insert(ang::cstr_t key, T value)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, key, ang::move(value));
		return true;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
			return false;
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return true;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::insert(ang::cwstr_t key, T value)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, key, ang::move(value));
		return true;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
			return false;
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return true;
}


template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::insert(ang::collections::pair<ang::wstring, T> pair)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, ang::move(pair));
		return true;
	}
	node_ptr_t node = _root;
	ang::wstring key = ang::move(pair.key_value());
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
			return false;
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return true;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::update(ang::cstr_t key, T value)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, key, ang::move(value));
		return true;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
		{
			node->value(ang::move(value));
			return true; //the key is updated
		}
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return false; //a new key was added
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::update(ang::cwstr_t key, T value)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, key, ang::move(value));
		return true;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(value));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
		{
			node->value(ang::move(value));
			return true; //the key is updated
		}
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return false; //a new key was added
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::update(ang::collections::pair<ang::wstring, T> pair)
{
	if (_root == null)
	{
		_root = allocator->object_alloc<node_t>(1);
		allocator->construct(_root, ang::move(pair));
		return true;
	}
	node_ptr_t node = _root;
	ang::wstring key = ang::move(pair.key_value());
	while (true)
	{
		if (key < node->key())
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->left(_new_node);
				break;
			}
		}
		else if (key > node->key())
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
				allocator->construct(_new_node, key, ang::move(pair.value()));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
		{
			node->value(ang::move(pair.value()));
			return true; //the key is updated
		}
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && key < node->left()->key())
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key > node->right()->key()) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && key > node->left()->key())	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && key < node->right()->key()) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && key < node->left()->key())
		_root = right_rotate(node);
	else if (balance < -1 && key > node->right()->key()) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && key > node->left()->key())	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && key < node->right()->key()) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	return false; //a new key was added
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::remove(ang::cstr_t key)
{
	bool removed = false;
	_root = remove_node(_root, key, removed);
	_root->parent(null);
	return removed;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::remove(ang::cwstr_t key)
{
	bool removed = false;
	_root = remove_node(_root, key, removed);
	_root->parent(null);
	return removed;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::remove(ang::cstr_t key, T& value)
{
	bool removed = false;
	_root = remove_node(_root, key, value, removed);
	_root->parent(null);
	return removed;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::remove(ang::cwstr_t key, T& value)
{
	bool removed = false;
	_root = remove_node(_root, key, value, removed);
	_root->parent(null);
	return removed;
}


template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::remove(ang::collections::iterator<ang::collections::pair<ang::wstring, T>> it)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	bool removed = false;
	if (node == _root)
	{
		_root = remove_node(node, node->key(), removed);
		_root->parent(null);
		return removed;
	}
	else
	{
		switch (node->type())
		{
		case 1:
			node->parent()->left(remove_node(node, node->key(), removed));
			return removed;
		case 2:
			node->parent()->right(remove_node(node, node->key(), removed));
			return removed;
		}
	}
	return false;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::remove(ang::collections::iterator<ang::collections::pair<ang::wstring, T>> it, T& value)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	bool removed = false;
	if (node == _root)
	{
		_root = remove_node(node, node->key(), value, removed);
		return removed;
	}
	else
	{
		switch (node->type())
		{
		case 1:
			node->parent()->left(remove_node(node, node->key(), value, removed));
			return removed;
		case 2:
			node->parent()->right(remove_node(node, node->key(), value, removed));
			return removed;
		}
	}
	return false;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::has_key(ang::cstr_t key)const
{
	return find_node(key, _root) != null;
}

template<typename T>
inline bool ang::collections::binary_tree_data<ang::wstring, T>::has_key(ang::cwstr_t key)const
{
	return find_node(key, _root) != null;
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::wstring, T>> ang::collections::binary_tree_data<ang::wstring, T>::find(ang::cstr_t key)const
{
	return iterator_t(const_cast<binary_tree_data*>(this), null, 0);
}

template<typename T>
inline ang::collections::iterator<ang::collections::pair<ang::wstring, T>> ang::collections::binary_tree_data<ang::wstring, T>::find(ang::cwstr_t key)const
{
	return iterator_t(const_cast<binary_tree_data*>(this), null, 0);
}

template<typename T>
inline ang::collections::binary_tree_node<ang::wstring, T>* ang::collections::binary_tree_data<ang::wstring, T>::right_rotate(ang::collections::binary_tree_node<ang::wstring, T>* y)
{
	node_ptr_t x = y->left();
	y->left(x->right());
	x->right(y);
	y->height(max(y->left()->height(), y->right()->height()) + 1);
	x->height(max(x->left()->height(), x->right()->height()) + 1);
	return x;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::wstring, T>* ang::collections::binary_tree_data<ang::wstring, T>::left_rotate(ang::collections::binary_tree_node<ang::wstring, T>* x)
{
	node_ptr_t y = x->right();
	x->right(y->left());
	y->left(x);
	x->height(max(x->left()->height(), x->right()->height()) + 1);
	y->height(max(y->left()->height(), y->right()->height()) + 1);
	return y;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::wstring, T>* ang::collections::binary_tree_data<ang::wstring, T>::remove_node(ang::collections::binary_tree_node<ang::wstring, T>* node, ang::cstr_t key, bool& removed)
{
	if (node == null)
		return null;

	if (key < node->key())
		node->left(remove_node(node->left(), key, removed));
	else if (key > node->key())
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		if (!node->left() || !node->right())
		{
			node_ptr_t temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(ang::move(temp->key()));
				node->value(ang::move(temp->value()));

				allocator->destruct(temp);
				allocator->memory_release(temp);
			}
			else
			{
				allocator->destruct(node);
				allocator->memory_release(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_t temp = most_left(node->right());
			node->key(temp->key());
			node->value(ang::move(temp->value()));
			node->right(remove_node(node->right(), temp->key(), _removed));
		}

	}
	node->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();

	if (balance > 1 && node->left()->balance() >= 0)
		return right_rotate(node);
	else if (balance > 1 && node->left()->balance() < 0)
	{
		node->left(left_rotate(node->left()));
		return right_rotate(node);
	}
	else if (balance < -1 && node->right()->balance() <= 0)
		return left_rotate(node);
	else if (balance < -1 && node->right()->balance() > 0)
	{
		node->right(right_rotate(node->right()));
		return left_rotate(node);
	}
	return node;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::wstring, T>* ang::collections::binary_tree_data<ang::wstring, T>::remove_node(ang::collections::binary_tree_node<ang::wstring, T>* node, ang::cwstr_t key, bool& removed)
{
	if (node == null)
		return null;

	if (key < node->key())
		node->left(remove_node(node->left(), key, removed));
	else if (key > node->key())
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		if (!node->left() || !node->right())
		{
			node_ptr_t temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(ang::move(temp->key()));
				node->value(ang::move(temp->value()));

				allocator->destruct(temp);
				allocator->memory_release(temp);
			}
			else
			{
				allocator->destruct(node);
				allocator->memory_release(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_t temp = most_left(node->right());
			node->key(temp->key());
			node->value(ang::move(temp->value()));
			node->right(remove_node(node->right(), temp->key(), _removed));
		}

	}
	node->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();

	if (balance > 1 && node->left()->balance() >= 0)
		return right_rotate(node);
	else if (balance > 1 && node->left()->balance() < 0)
	{
		node->left(left_rotate(node->left()));
		return right_rotate(node);
	}
	else if (balance < -1 && node->right()->balance() <= 0)
		return left_rotate(node);
	else if (balance < -1 && node->right()->balance() > 0)
	{
		node->right(right_rotate(node->right()));
		return left_rotate(node);
	}
	return node;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::wstring, T>* ang::collections::binary_tree_data<ang::wstring, T>::remove_node(ang::collections::binary_tree_node<ang::wstring, T>* node, ang::cstr_t key, T& value, bool& removed)
{
	if (node == null)
		return null;

	if (key < node->key())
		node->left(remove_node(node->left(), key, removed));
	else if (key > node->key())
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		value = ang::move(node->value());
		if (!node->left() || !node->right())
		{
			node_ptr_t temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(ang::move(temp->key()));
				node->value(ang::move(temp->value()));

				allocator->destruct(temp);
				allocator->memory_release(temp);
			}
			else
			{
				allocator->destruct(node);
				allocator->memory_release(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_t temp = most_left(node->right());
			node->key(temp->key());
			node->value(ang::move(temp->value()));
			node->right(remove_node(node->right(), temp->key(), _removed));
		}

	}
	node->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();

	if (balance > 1 && node->left()->balance() >= 0)
		return right_rotate(node);
	else if (balance > 1 && node->left()->balance() < 0)
	{
		node->left(left_rotate(node->left()));
		return right_rotate(node);
	}
	else if (balance < -1 && node->right()->balance() <= 0)
		return left_rotate(node);
	else if (balance < -1 && node->right()->balance() > 0)
	{
		node->right(right_rotate(node->right()));
		return left_rotate(node);
	}
	return node;
}

template<typename T>
inline ang::collections::binary_tree_node<ang::wstring, T>* ang::collections::binary_tree_data<ang::wstring, T>::remove_node(ang::collections::binary_tree_node<ang::wstring, T>* node, ang::cwstr_t key, T& value, bool& removed)
{
	if (node == null)
		return null;

	if (key < node->key())
		node->left(remove_node(node->left(), key, removed));
	else if (key > node->key())
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		value = ang::move(node->value());
		if (!node->left() || !node->right())
		{
			node_ptr_t temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(ang::move(temp->key()));
				node->value(ang::move(temp->value()));

				allocator->destruct(temp);
				allocator->memory_release(temp);
			}
			else
			{
				allocator->destruct(node);
				allocator->memory_release(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_t temp = most_left(node->right());
			node->key(temp->key());
			node->value(ang::move(temp->value()));
			node->right(remove_node(node->right(), temp->key(), _removed));
		}

	}
	node->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();

	if (balance > 1 && node->left()->balance() >= 0)
		return right_rotate(node);
	else if (balance > 1 && node->left()->balance() < 0)
	{
		node->left(left_rotate(node->left()));
		return right_rotate(node);
	}
	else if (balance < -1 && node->right()->balance() <= 0)
		return left_rotate(node);
	else if (balance < -1 && node->right()->balance() > 0)
	{
		node->right(right_rotate(node->right()));
		return left_rotate(node);
	}
	return node;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::object_wrapper()
	: _ptr(null)
{

}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{

}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::object_wrapper(ang::collections::binary_tree_data<K, T>* ptr)
	: object_wrapper()
{
	set(ptr);
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::object_wrapper(ang::collections::ienum<ang::collections::pair<K, T>> const* store)
	: object_wrapper()
{
	set(new collections::binary_tree_data<K, T>(store));
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::object_wrapper(ang::initializer_list_t<ang::collections::pair<K, T>> list)
{
	set(new collections::binary_tree_data<K, T>(list));
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::object_wrapper(ang::object_wrapper<ang::collections::binary_tree_data<K, T>> && other)
{
	collections::binary_tree_data<K, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::object_wrapper(ang::object_wrapper<ang::collections::binary_tree_data<K, T>> const& other)
{
	set(other.get());
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::~object_wrapper()
{
	clean();
}

template<typename K, typename T>
inline void ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename K, typename T>
inline bool ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::is_empty()const
{
	return _ptr == null;
}

template<typename K, typename T>
inline ang::collections::binary_tree_data<K, T>* ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::get(void)const
{
	return _ptr;
}

template<typename K, typename T>
inline void ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::set(ang::collections::binary_tree_data<K, T>* ptr)
{
	collections::binary_tree_data<K, T> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename K, typename T>
inline ang::collections::binary_tree_data<K, T> ** ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::addres_of(void)
{
	return &_ptr;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>& ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator = (ang::object_wrapper<ang::collections::binary_tree_data<K, T>> && other)
{
	ang::collections::binary_tree_data<K, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>& ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator = (ang::object_wrapper<ang::collections::binary_tree_data<K, T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>& ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>& ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator = (ang::collections::binary_tree_data<K, T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>& ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator = (ang::collections::ienum<ang::collections::pair<K, T>> const* store)
{
	if (is_empty())
		set(new collections::binary_tree_data<K, T>(store));
	else
		_ptr->copy(store);
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>& ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator += (ang::collections::pair<K, T> pair)
{
	if (is_empty())
		set(new collections::binary_tree_data<K, T>());
	_ptr->update(ang::move(pair));
	return*this;
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator ang::object_t()const {
	return _ptr;
}

template<typename K, typename T>
inline ang::object_wrapper_ptr<ang::collections::binary_tree_data<K, T>> ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator & (void)
{
	return this;
}

template<typename K, typename T>
inline ang::collections::binary_tree_data<K, T> * ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator -> (void)
{
	return get();
}

template<typename K, typename T>
inline ang::collections::binary_tree_data<K, T> const* ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator -> (void)const
{
	return get();
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator ang::collections::binary_tree_data<K, T> * (void)
{
	return get();
}

template<typename K, typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator ang::collections::binary_tree_data<K, T> const* (void)const
{
	return get();
}

template<typename K, typename T>
inline T& ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator [] (K const& key)
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename K, typename T>
inline T ang::object_wrapper<ang::collections::binary_tree_data<K, T>>::operator [] (K const& key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::object_wrapper(ang::collections::binary_tree_data<ang::string, T>* ptr)
	: object_wrapper()
{
	set(ptr);
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::object_wrapper(ang::collections::ienum<ang::collections::pair<ang::string, T>> const* store)
	: object_wrapper()
{
	set(new collections::binary_tree_data<ang::string, T>(store));
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::object_wrapper(ang::initializer_list_t<ang::collections::pair<ang::string, T>> list)
{
	set(new collections::binary_tree_data<ang::string, T>(list));
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::object_wrapper(ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>> && other)
{
	collections::binary_tree_data<ang::string, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::object_wrapper(ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>> const& other)
{
	set(other.get());
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::~object_wrapper()
{
	clean();
}

template<typename T>
inline void ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T>
inline bool ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::collections::binary_tree_data<ang::string, T>* ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::set(ang::collections::binary_tree_data<ang::string, T>* ptr)
{
	collections::binary_tree_data<ang::string, T> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T>
inline ang::collections::binary_tree_data<ang::string, T> ** ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator = (ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>> && other)
{
	ang::collections::binary_tree_data<ang::string, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator = (ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator = (ang::collections::binary_tree_data<ang::string, T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator = (ang::collections::ienum<ang::collections::pair<ang::string, T>> const* store)
{
	if (is_empty())
		set(new collections::binary_tree_data<ang::string, T>(store));
	else
		_ptr->copy(store);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator += (ang::collections::pair<ang::string, T> pair)
{
	if (is_empty())
		set(new collections::binary_tree_data<ang::string, T>());
	_ptr->update(ang::move(pair));
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator ang::object_t()const {
	return _ptr;
}

template<typename T>
inline ang::object_wrapper_ptr<ang::collections::binary_tree_data<ang::string, T>> ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator & (void)
{
	return this;
}

template<typename T>
inline ang::collections::binary_tree_data<ang::string, T> * ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator -> (void)
{
	return get();
}

template<typename T>
inline ang::collections::binary_tree_data<ang::string, T> const* ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator ang::collections::binary_tree_data<ang::string, T> * (void)
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator ang::collections::binary_tree_data<ang::string, T> const* (void)const
{
	return get();
}

template<typename T>
inline T& ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator [] (ang::cstr_t key)
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T& ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator [] (ang::cwstr_t key)
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator [] (ang::cstr_t key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T ang::object_wrapper<ang::collections::binary_tree_data<ang::string, T>>::operator [] (ang::cwstr_t key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::object_wrapper(ang::collections::binary_tree_data<ang::wstring, T>* ptr)
	: object_wrapper()
{
	set(ptr);
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::object_wrapper(ang::collections::ienum<ang::collections::pair<ang::wstring, T>> const* store)
	: object_wrapper()
{
	set(new collections::binary_tree_data<ang::wstring, T>(store));
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::object_wrapper(ang::initializer_list_t<ang::collections::pair<ang::wstring, T>> list)
{
	set(new collections::binary_tree_data<ang::wstring, T>(list));
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::object_wrapper(ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>> && other)
{
	collections::binary_tree_data<ang::wstring, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::object_wrapper(ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>> const& other)
{
	set(other.get());
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::~object_wrapper()
{
	clean();
}

template<typename T>
inline void ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T>
inline bool ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::collections::binary_tree_data<ang::wstring, T>* ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::set(ang::collections::binary_tree_data<ang::wstring, T>* ptr)
{
	collections::binary_tree_data<ang::wstring, T> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T>
inline ang::collections::binary_tree_data<ang::wstring, T> ** ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator = (ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>> && other)
{
	ang::collections::binary_tree_data<ang::wstring, T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator = (ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator = (ang::collections::binary_tree_data<ang::wstring, T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator = (ang::collections::ienum<ang::collections::pair<ang::wstring, T>> const* store)
{
	if (is_empty())
		set(new collections::binary_tree_data<ang::wstring, T>(store));
	else
		_ptr->copy(store);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>& ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator += (ang::collections::pair<ang::wstring, T> pair)
{
	if (is_empty())
		set(new collections::binary_tree_data<ang::wstring, T>());
	_ptr->update(ang::move(pair));
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator ang::object_t()const {
	return _ptr;
}

template<typename T>
inline ang::object_wrapper_ptr<ang::collections::binary_tree_data<ang::wstring, T>> ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator & (void)
{
	return this;
}

template<typename T>
inline ang::collections::binary_tree_data<ang::wstring, T> * ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator -> (void)
{
	return get();
}

template<typename T>
inline ang::collections::binary_tree_data<ang::wstring, T> const* ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator ang::collections::binary_tree_data<ang::wstring, T> * (void)
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator ang::collections::binary_tree_data<ang::wstring, T> const* (void)const
{
	return get();
}

template<typename T>
inline T& ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator [] (ang::cstr_t key)
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T& ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator [] (ang::cwstr_t key)
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator [] (ang::cstr_t key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}

template<typename T>
inline T ang::object_wrapper<ang::collections::binary_tree_data<ang::wstring, T>>::operator [] (ang::cwstr_t key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}


#endif//__ANG_BINARY_TREE_HPP__