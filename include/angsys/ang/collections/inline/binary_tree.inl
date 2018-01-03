#ifndef __ANG_BINARY_TREE_HPP__
#error Can't include binary_tree.hpp, please include binary_tree.h inside
#elif !defined __ANG_BINARY_TREE_INL__
#define __ANG_BINARY_TREE_INL__

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K,T>::begin(ang::collections::binary_tree_node<K, T>* node, index& idx)
{
	return most_left(node, idx);
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K, T>::end(ang::collections::binary_tree_node<K, T>* node, index& idx)
{
	idx = 0;
	return null;
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K, T>::rbegin(ang::collections::binary_tree_node<K, T>* node, index& idx)
{
	return most_right(node, idx);
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K, T>::rend(ang::collections::binary_tree_node<K, T>* node, index& idx)
{
	idx = 0;
	return null;
}

template<typename K, typename T>
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K, T>::increase(ang::collections::binary_tree_node<K, T>* node, index& idx)
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
inline ang::collections::binary_tree_node<K, T>* ang::collections::algorithms::binary_tree_inorder_iteration<K, T>::decrease(ang::collections::binary_tree_node<K, T>* node, index& idx)
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


ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION_TEMPLATE(ang::collections::binary_tree_node, K, T);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_object<K, T, allocator>::binary_tree_object()
	: _count(0)
	, _root(null)
	, algorithm(ang::collections::algorithms::binary_tree_inorder_iteration<K, T>())
{
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_object<K, T, allocator>::binary_tree_object(ang::initializer_list_t<ang::collections::pair<K, T>> list)
	: binary_tree_object()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_object<K, T, allocator>::binary_tree_object(const ang::nullptr_t&)
	: binary_tree_object()
{
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_object<K, T, allocator>::binary_tree_object(binary_tree_object&& other)
	: binary_tree_object()
{
	_count = other._count;
	_root = other._root;

	other._count = 0;
	other._root = null;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_object<K, T, allocator>::binary_tree_object(const binary_tree_object& other)
	: binary_tree_object()
{
	extend(&other);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_object<K, T, allocator>::binary_tree_object(const binary_tree_object* other)
	: binary_tree_object()
{
	extend(other);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_object<K, T, allocator>::binary_tree_object(ang::collections::ienum<ang::collections::pair<K, T>> const* other)
	: binary_tree_object()
{
	extend(other);
}

template<typename K, typename T, template<typename> class allocator>
ang::collections::binary_tree_object<K, T, allocator>::~binary_tree_object()
{
	clean();
}

template<typename K, typename T, template<typename> class allocator>
inline ang::type_name_t ang::collections::binary_tree_object<K, T, allocator>::class_name()
{
	string _class_name;
	static type_name_t name = runtime_data_base::regist_typename(ang::move(_class_name << "ang::collections::binary_tree_object<"_s << args_list_type_of<K, T>() << ">"_s));
	return name;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::type_name_t ang::collections::binary_tree_object<K, T, allocator>::object_name()const
{
	return ang::collections::binary_tree_object<K, T, allocator>::class_name();
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::is_inherited_of(ang::type_name_t name)
{
	return name == type_of<binary_tree_object< K, T>>()
		|| ang::object::is_inherited_of(name)
		|| ang::collections::imap<K, T>::is_inherited_of(name);
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::is_kind_of(ang::type_name_t name)const
{
	return name == type_of<binary_tree_object< K, T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::imap<K, T>::is_kind_of(name);
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_of<binary_tree_object<K, T, allocator>>())
	{
		*out = static_cast<ang::collections::binary_tree_object<K, T, allocator>*>(this);
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


template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::is_empty()const {
	return _count == 0;
}

template<typename K, typename T, template<typename> class allocator>
inline void ang::collections::binary_tree_object<K, T, allocator>::clean() {
	destroy_tree(_root);
	_root = null;
	_count = 0;
}

template<typename K, typename T, template<typename> class allocator>
inline void ang::collections::binary_tree_object<K, T, allocator>::destroy_tree(ang::collections::binary_tree_node<K, T>* node)const {
	if (node != NULL)
	{
		destroy_tree(node->left());
		node->left(null);

		destroy_tree(node->right());
		node->right(null);

		allocator<node_t>::destruct_and_free(node);
	}
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::move(binary_tree_object& other)
{
	if (this == &other)
		return false;
	clean();
	_count = other._count;
	_root = other._root;
	other._count = 0;
	other._root = null;
	return true;
}

template<typename K, typename T, template<typename> class allocator>
inline void ang::collections::binary_tree_object<K, T, allocator>::iteration_method(ang::collections::algorithms::iteration_method_t method)
{
	switch (method)
	{
	case algorithms::iteration_method::inorder:
		algorithm = algorithms::binary_tree_inorder_iteration<K,T>();
		break;
	default:
		break;
	}
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_object<K, T, allocator>::find_node(K const& key, ang::collections::binary_tree_node<K, T>* node)const
{
	//if (_count == 0)
	//	return null;
	while (node != null)
	{
		if (comparision_operations<K, K>::template compare<comparision_same>(key, node->key()))
			return node;
		if (comparision_operations<K, K>::template compare<comparision_minor>(key, node->key()))
			node = node->left();
		else
			node = node->right();
	}
	return null;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_object<K, T, allocator>::most_left(ang::collections::binary_tree_node<K, T>* node)const
{
	if (node == null)
		return null;
	while (node->left() != null)
		node = node->left();
	return node;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_object<K, T, allocator>::most_right(ang::collections::binary_tree_node<K, T>* node)const
{
	if (node == null)
		return null;
	while (node->right() != null)
		node = node->right();
	return node;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::pair<K, T>& ang::collections::binary_tree_object<K, T, allocator>::at(const ang::collections::base_iterator<ang::collections::pair<K, T>>& it)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	return node_ptr_t(it.current())->_pair;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::begin()
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_object*>(this), algorithm.begin(_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::begin()const
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_object*>(this), algorithm.begin(_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::end()
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_object*>(this), algorithm.end(_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::end()const
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_object*>(this), algorithm.end(_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::last()
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_object*>(this), algorithm.rbegin(_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_forward_iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::last()const
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_object*>(this), algorithm.rbegin(_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::backward_iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::rbegin()
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_object*>(this), algorithm.rbegin(_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_backward_iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::rbegin()const
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_object*>(this), algorithm.rbegin(_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::backward_iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::rend()
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_object*>(this), algorithm.rend(_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_backward_iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::rend()const
{
	index idx = 0;
	return iterator_t(const_cast<binary_tree_object*>(this), algorithm.rend(_root, idx), 0);
}


template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::increase(ang::collections::base_iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = algorithm.increase(node_ptr_t(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::increase(ang::collections::base_iterator<ang::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = node_ptr_t(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = algorithm.increase(node, idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::decrease(ang::collections::base_iterator<ang::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = algorithm.decrease(node_ptr_t(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::decrease(ang::collections::base_iterator<ang::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	index idx = 0;
	node_ptr_t node = node_ptr_t(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = algorithm.decrease(node, idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline wsize ang::collections::binary_tree_object<K, T, allocator>::counter()const
{
	return _count;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::copy(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return false;
	clean();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename K, typename T, template<typename> class allocator>
inline void ang::collections::binary_tree_object<K, T, allocator>::extend(ang::collections::ienum<ang::collections::pair<K, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::insert(K key, T value)
{
	if (_root == null)
	{
		_root = allocator<node_t>::template alloc_and_construct<K&&,T&&>(ang::move(key), ang::move(value));
		_count++;
		return true;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (comparision_operations<K, K>::template compare<comparision_minor>(key, node->key()))
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator<node_t>::template alloc_and_construct<K const&,T&&>(key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (comparision_operations<K, K>::template compare<comparision_major>(key, node->key()))
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator<node_t>::template alloc_and_construct<K const&, T&&>(key, ang::move(value));
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
		if (balance > 1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->left()->key()))
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->right()->key())) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->left()->key()))	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->right()->key())) // Right Left Case
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
	if (balance > 1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->left()->key()))
		_root = right_rotate(node);
	else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->right()->key())) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->left()->key()))	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->right()->key())) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	_count++;
	return true;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::insert(ang::collections::pair<K, T> pair)
{
	if (_root == null)
	{
		_root = allocator<node_t>::template alloc_and_construct<pair_t&&>(ang::move(pair));
		_count++;
		return true;
	}
	node_ptr_t node = _root;
	K key = ang::move(pair.key);
	while (true)
	{
		if (comparision_operations<K, K>::template compare<comparision_minor>(key, node->key()))
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator<node_t>::template alloc_and_construct<K const&, T&&>(pair.key, ang::move(pair.value));
				node->left(_new_node);
				break;
			}
		}
		else if (comparision_operations<K, K>::template compare<comparision_major>(key, node->key()))
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator<node_t>::template alloc_and_construct<K const&, T&&>(pair.key, ang::move(pair.value));
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
		if (balance > 1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->left()->key()))
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->right()->key())) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->left()->key()))	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->right()->key())) // Right Left Case
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
	if (balance > 1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->left()->key()))
		_root = right_rotate(node);
	else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->right()->key())) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->left()->key()))	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->right()->key())) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	_count++;
	return true;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::update(K key, T value)
{
	if (_root == null)
	{
		_root = allocator<node_t>::template alloc_and_construct<K const&, T&&>(key, ang::move(value));
		_count++;
		return false;
	}
	node_ptr_t node = _root;
	while (true)
	{
		if (comparision_operations<K, K>::template compare<comparision_minor>(key, node->key()))
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator<node_t>::template alloc_and_construct<K const&, T&&>(key, ang::move(value));
				node->left(_new_node);
				break;
			}
		}
		else if (comparision_operations<K, K>::template compare<comparision_major>(key, node->key()))
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator<node_t>::template alloc_and_construct<K const&, T&&>(key, ang::move(value));
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
		if (balance > 1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->left()->key()))
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->right()->key())) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->left()->key()))	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->right()->key())) // Right Left Case
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
	if (balance > 1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->left()->key()))
		_root = right_rotate(node);
	else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->right()->key())) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->left()->key()))	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->right()->key())) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	_count++;
	return false; //a new key was added
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::update(ang::collections::pair<K, T> pair)
{
	if (_root == null)
	{
		_root = allocator<node_t>::template alloc_and_construct<pair_t&&>(ang::move(pair));
		_count++;
		return false;
	}
	node_ptr_t node = _root;
	K key = ang::move(pair.key);
	while (true)
	{
		if (comparision_operations<K, K>::template compare<comparision_minor>(key, node->key()))
		{
			if (node->left())
				node = node->left();
			else
			{
				node_ptr_t _new_node = allocator<node_t>::template alloc_and_construct<K const&, T&&>(pair.key, ang::move(pair.value));
				node->left(_new_node);
				break;
			}
		}
		else if (comparision_operations<K, K>::template compare<comparision_major>(key, node->key()))
		{
			if (node->right())
				node = node->right();
			else
			{
				node_ptr_t _new_node = allocator<node_t>::template alloc_and_construct<K const&, T&&>(pair.key, ang::move(pair.value));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
		{
			node->value(ang::move(pair.value));
			return true; //the key is updated
		}
	}
	node_ptr_t parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->left()->key()))
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->right()->key())) // Right Right Case
		{
			parent = node->parent();
			node->type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->left()->key()))	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->right()->key())) // Right Left Case
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
	if (balance > 1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->left()->key()))
		_root = right_rotate(node);
	else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->right()->key())) // Right Right Case
		_root = left_rotate(node);
	else if (balance > 1 && comparision_operations<K, K>::template compare<comparision_major>(key, node->left()->key()))	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		_root = right_rotate(node);
	}
	else if (balance < -1 && comparision_operations<K, K>::template compare<comparision_minor>(key, node->right()->key())) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		_root = left_rotate(node);
	}
	_root->parent(null);
	_count++;
	return false; //a new key was added
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::remove(K const& key)
{
	bool removed = false;
	_root = remove_node(_root, key, removed);
	_root->parent(null);
	if (removed) _count--;
	return removed;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::remove(K const& key, T& value)
{
	bool removed = false;
	_root = remove_node(_root, key, value, removed);
	_root->parent(null);
	if (removed) _count--;
	return removed;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::remove(ang::collections::base_iterator<ang::collections::pair<K, T>> it)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	bool removed = false;
	if (node == _root)
	{
		_root = remove_node(node, node->key(), removed);
		_root->parent(null);
		if (removed) _count--;
		return removed;
	}
	else
	{
		switch (node->type())
		{
		case 1:
			node->parent()->left(remove_node(node, node->key(), removed));
			if (removed) _count--;
			return removed;
		case 2:
			node->parent()->right(remove_node(node, node->key(), removed));
			if (removed) _count--;
			return removed;
		}
	}
	return false;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::remove(ang::collections::base_iterator<ang::collections::pair<K, T>> it, T& value)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	bool removed = false;
	if (node == _root)
	{
		_root = remove_node(node, node->key(), value, removed);
		if (removed) _count--;
		return removed;
	}
	else
	{
		switch (node->type())
		{
		case 1:
			node->parent()->left(remove_node(node, node->key(), value, removed));
			if (removed) _count--;
			return removed;
		case 2:
			node->parent()->right(remove_node(node, node->key(), value, removed));
			if (removed) _count--;
			return removed;
		}
	}
	return false;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::collections::binary_tree_object<K, T, allocator>::has_key(const K& key)const
{
	return find_node(key, _root) != null;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::find(const K& key)
{
	//if (comparision_operations<K, K>::template compare<comparision_minor>(key, node->key()))
	node_ptr_t node = find_node(key, _root);
	if(node && comparision_operations<K, K>::template compare<comparision_same>(node->key(), key))
		return iterator_t(const_cast<binary_tree_object*>(this), node, 0);
	return iterator_t(const_cast<binary_tree_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::const_iterator<ang::collections::pair<K, T>> ang::collections::binary_tree_object<K, T, allocator>::find(const K& key)const
{
	node_ptr_t node = find_node(key, _root);
	if (node && comparision_operations<K, K>::template compare<comparision_same>(node->key(), key))
		return iterator_t(const_cast<binary_tree_object*>(this), node, 0);
	return iterator_t(const_cast<binary_tree_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_node<K,T>* ang::collections::binary_tree_object<K, T, allocator>::right_rotate(ang::collections::binary_tree_node<K, T>* y)
{
	node_ptr_t x = y->left();
	y->left(x->right());
	x->right(y);
	y->height(max(y->left()->height(), y->right()->height()) + 1);
	x->height(max(x->left()->height(), x->right()->height()) + 1);
	return x;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_object<K, T, allocator>::left_rotate(ang::collections::binary_tree_node<K, T>* x)
{
	node_ptr_t y = x->right();
	x->right(y->left());
	y->left(x);
	x->height(max(x->left()->height(), x->right()->height()) + 1);
	y->height(max(y->left()->height(), y->right()->height()) + 1);
	return y;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_object<K, T, allocator>::remove_node(ang::collections::binary_tree_node<K, T>* node, key_t const& key, bool& removed)
{
	if (node == null)
		return null;

	if (comparision_operations<K, K>::template compare<comparision_minor>(key, node->key()))
		node->left(remove_node(node->left(), key, removed));
	else if (comparision_operations<K, K>::template compare<comparision_major>(key, node->key()))
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

				allocator<node_t>::destruct_and_free(temp);
			}
			else
			{
				allocator<node_t>::destruct_and_free(node);
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

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_node<K, T>* ang::collections::binary_tree_object<K, T, allocator>::remove_node(ang::collections::binary_tree_node<K, T>* node, key_t const& key, T& value, bool& removed)
{
	if (node == null)
		return null;

	if (comparision_operations<K, K>::template compare<comparision_minor>(key, node->key()))
		node->left(remove_node(node->left(), key, removed));
	else if (comparision_operations<K, K>::template compare<comparision_major>(key, node->key()))
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

				allocator<node_t>::destruct_and_free(temp);
			}
			else
			{
				allocator<node_t>::destruct_and_free(node);
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

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::object_wrapper()
	: _ptr(null)
{

}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{

}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::object_wrapper(ang::collections::binary_tree_object<K, T, allocator>* ptr)
	: object_wrapper()
{
	set(ptr);
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::object_wrapper(ang::collections::ienum<ang::collections::pair<K, T>> const* store)
	: object_wrapper()
{
	set(new collections::binary_tree_object<K, T, allocator>(store));
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::object_wrapper(ang::initializer_list_t<ang::collections::pair<K, T>> list)
{
	set(new collections::binary_tree_object<K, T, allocator>(list));
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>> && other)
{
	collections::binary_tree_object<K, T, allocator> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>> const& other)
{
	set(other.get());
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::~object_wrapper()
{
	clean();
}

template<typename K, typename T, template<typename> class allocator>
inline void ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::is_empty()const
{
	return _ptr == null;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_object<K, T, allocator>* ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::get(void)const
{
	return _ptr;
}

template<typename K, typename T, template<typename> class allocator>
inline void ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::set(ang::collections::binary_tree_object<K, T, allocator>* ptr)
{
	collections::binary_tree_object<K, T, allocator> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_object<K, T, allocator> ** ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::addres_of(void)
{
	return &_ptr;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>& ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator = (ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>> && other)
{
	ang::collections::binary_tree_object<K, T, allocator> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>& ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator = (ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>& ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>& ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator = (ang::collections::binary_tree_object<K, T, allocator>* ptr)
{
	set(ptr);
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>& ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator = (ang::collections::ienum<ang::collections::pair<K, T>> const* store)
{
	if (is_empty())
		set(new collections::binary_tree_object<K, T, allocator>(store));
	else
		_ptr->copy(store);
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>& ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator += (ang::collections::pair<K, T> pair)
{
	if (is_empty())
		set(new collections::binary_tree_object<K, T, allocator>());
	_ptr->update(ang::move(pair));
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator ang::object_t()const {
	return _ptr;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper_ptr<ang::collections::binary_tree_object<K, T, allocator>> ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator & (void)
{
	return this;
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_object<K, T, allocator> * ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator -> (void)
{
	return get();
}

template<typename K, typename T, template<typename> class allocator>
inline ang::collections::binary_tree_object<K, T, allocator> const* ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator -> (void)const
{
	return get();
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator ang::collections::binary_tree_object<K, T, allocator> * (void)
{
	return get();
}

template<typename K, typename T, template<typename> class allocator>
inline ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator ang::collections::binary_tree_object<K, T, allocator> const* (void)const
{
	return get();
}

template<typename K, typename T, template<typename> class allocator>
inline T& ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator [] (K const& key)
{
	if (is_empty()) set(new collections::binary_tree_object<K, T, allocator>());
	auto it = _ptr->find(key);
	if (!it.is_valid()) {
		_ptr->insert(key, T());
		it = _ptr->find(key);
	}// throw(exception_t(except_code::array_overflow));
	return it->value;
}

template<typename K, typename T, template<typename> class allocator>
inline T ang::object_wrapper<ang::collections::binary_tree_object<K, T, allocator>>::operator [] (K const& key)const
{
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(except_code::array_overflow));
	return it->value();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif//__ANG_BINARY_TREE_INL__