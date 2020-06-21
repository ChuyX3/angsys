#ifndef __COFFE_BINARY_TREE_H__
#error Can't include binary_tree.h, please include binary_tree.h inside
#elif !defined __COFFE_BINARY_TREE_INL__
#define __COFFE_BINARY_TREE_INL__

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_preorder_iteratior<T>::begin(coffe::collections::binary_node<T>* node, wsize& idx)
{
	return node;
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_preorder_iteratior<T>::end(coffe::collections::binary_node<T>* node, wsize& idx)
{
	idx = 0;
	return null;
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_preorder_iteratior<T>::rbegin(coffe::collections::binary_node<T>* node, wsize& idx)
{
	return most_right(node, idx);
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_preorder_iteratior<T>::rend(coffe::collections::binary_node<T>* node, wsize& idx)
{
	idx = 0;
	return null;
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_preorder_iteratior<T>::increase(coffe::collections::binary_node<T>* node, wsize& idx)
{
	if (node == null)
		return null;

	if (node->left() != null)	
		return node->left();

	else if(node->right() != null)
		return node->right();

	else 
	{
		while (node)
		{
			if(node->node_type() == 2)
				node = node->parent();
			else {
				node = node->parent();
				if (node != null && node->right() != null)
					return node->right();
			}
		}	
	}
	return null;
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_preorder_iteratior<T>::decrease(coffe::collections::binary_node<T>* node, wsize& idx)
{
	if (node == null)
		return null;

	if (node->node_type() == 2)
	{
		if(node->parent()->left())
			return most_right(node->left(), idx);
	}

	while (node)
	{
		if (node->node_type() == 1)
			node = node->parent();
		else {
			node = node->parent();
			if (node != null && node->left() != null)
				
		}
	}


	if (node->node_type() != 2)
		return most_right(node->right(), idx);
	else
	{
		if (node->node_type() == 1)
			return node->parent();
		else if (node->node_type() == 2) {
			while (node->parent() && node->node_type() == 2)
				node = node->parent();
			return node->parent();
		}
	}
	return null;
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_preorder_iteratior<T>::most_left(coffe::collections::binary_node<T>* node, wsize& idx)
{
	while (node && node->left())
		node = node->left();
	return node;
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_preorder_iteratior<T>::most_right(coffe::collections::binary_node<T>* node, wsize& idx)
{
	while (node && node->right())
		node = node->right();
	return node;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_inorder_iteratior<T>::begin(coffe::collections::binary_node<T>* node, wsize& idx)
{
	return most_left(node, idx);
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_inorder_iteratior<T>::end(coffe::collections::binary_node<T>* node, wsize& idx)
{
	idx = 0;
	return null;
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_inorder_iteratior<T>::rbegin(coffe::collections::binary_node<T>* node, wsize& idx)
{
	return most_right(node, idx);
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_inorder_iteratior<T>::rend(coffe::collections::binary_node<T>* node, wsize& idx)
{
	idx = 0;
	return null;
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_inorder_iteratior<T>::increase(coffe::collections::binary_node<T>* node, wsize& idx)
{
	if (node == null)
		return null;

	if (node->right() != null)
		return most_left(node->right(), idx);
	else
	{
		if (node->node_type() == 1)
			return node->parent();
		else if (node->node_type() == 2) {
			while (node->parent() && node->node_type() == 2)
				node = node->parent();
			return node->parent();
		}
	}
	return null;
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_inorder_iteratior<T>::decrease(coffe::collections::binary_node<T>* node, wsize& idx)
{
	if (node == null)
		return null;

	if (node->left() != null)
		return most_right(node->right(), idx);
	else
	{
		if (node->node_type() == 1)
			return node->parent();
		else if (node->node_type() == 2) {
			while (node->parent() && node->node_type() == 2)
				node = node->parent();
			return node->parent();
		}
	}
	return null;
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_inorder_iteratior<T>::most_left(coffe::collections::binary_node<T>* node, wsize& idx)
{
	while (node && node->left())
		node = node->left();
	return node;
}

template<typename T>
inline coffe::collections::binary_node<T>* coffe::algorithms::binary_tree_inorder_iteratior<T>::most_right(coffe::collections::binary_node<T>* node, wsize& idx)
{
	while (node && node->right())
		node = node->right();
	return node;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_enum<K, T, allocator>::binary_map_enum(coffe::collections::binary_map<K,T,allocator> map, coffe::collections::iteration_method_t method)
	: m_map(map.get())
	, m_algorithm()
{
	switch (method)
	{
	default:
	case iteration_method::inorder:
		m_algorithm = coffe::algorithms::binary_tree_inorder_iteratior<pair<K, T>>();
		break;
	case iteration_method::preorder:
		m_algorithm = coffe::algorithms::binary_tree_preorder_iteratior<pair<K, T>>();
		break;
	}
}

template<typename K, typename T, template<typename> class allocator>
coffe::collections::binary_map_enum<K, T, allocator>::~binary_map_enum()
{
	//clear();
}

template<typename K, typename T, template<typename> class allocator>
void coffe::collections::binary_map_enum<K, T, allocator>::dispose()
{
	m_map = null;
}

template<typename K, typename T, template<typename> class allocator>
inline const coffe::rtti_t& coffe::collections::binary_map_enum<K, T, allocator>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((((astring("coffe::collections::binary_map<"_s) += coffe::type_of<K>().type_name()) += ","_s) += coffe::type_of<T>().type_name()) += ">"_s);
	static coffe::rtti_t const* parents[] = { 
		&coffe::type_of<object>(),
		&coffe::type_of<ienum_type>()
	};
	static coffe::rtti_t const& info = coffe::rtti::regist(name, genre::class_type, size_of<binary_map_enum<K, T, allocator>>(), align_of<binary_map_enum<K, T, allocator>>(), parents, &intf::default_query_interface);
	return info;
}

template<typename K, typename T, template<typename> class allocator>
inline const coffe::rtti_t& coffe::collections::binary_map_enum<K, T, allocator>::runtime_info()const
{
	return class_info();
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_enum<K, T, allocator>::query_interface(const coffe::rtti_t& id, coffe::unknown_ptr_t out)
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
	else if (id.type_id() == coffe::type_of<ienum_type>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ienum_type*>(this);
		return true;
	}
}


template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::pair<K, T>& coffe::collections::binary_map_enum<K, T, allocator>::at(const coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(error_code::invalid_param));
#endif
	return node_ptr_type(it.current())->m_data;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_enum<K, T, allocator>::begin()
{
	wsize idx = 0;
	return iterator_type(this, m_algorithm.begin(m_map->m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::const_forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_enum<K, T, allocator>::begin()const
{
	wsize idx = 0;
	return iterator_type(const_cast<self_type*>(this), m_algorithm.begin(m_map->m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_enum<K, T, allocator>::end()
{
	wsize idx = 0;
	return iterator_type(this, m_algorithm.end(m_map->m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::const_forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_enum<K, T, allocator>::end()const
{
	wsize idx = 0;
	return iterator_type(const_cast<self_type*>(this), m_algorithm.end(m_map->m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_enum<K, T, allocator>::last()
{
	wsize idx = 0;
	return iterator_type(this, m_algorithm.rbegin(m_map->m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::const_forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_enum<K, T, allocator>::last()const
{
	wsize idx = 0;
	return iterator_type(const_cast<self_type*>(this), m_algorithm.rbegin(m_map->m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::backward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_enum<K, T, allocator>::rbegin()
{
	wsize idx = 0;
	return iterator_type(this, m_algorithm.rbegin(m_map->m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::const_backward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_enum<K, T, allocator>::rbegin()const
{
	wsize idx = 0;
	return iterator_type(const_cast<self_type*>(this), m_algorithm.rbegin(m_map->m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::backward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_enum<K, T, allocator>::rend()
{
	wsize idx = 0;
	return iterator_type(this, m_algorithm.rend(m_map->m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::const_backward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_enum<K, T, allocator>::rend()const
{
	wsize idx = 0;
	return iterator_type(const_cast<self_type*>(this), m_algorithm.rend(m_map->m_root, idx), 0);
}


template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_enum<K, T, allocator>::increase(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(error_code::invalid_param));
#endif
	wsize idx = 0;
	node_ptr_type node = m_algorithm.increase(node_ptr_type(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_enum<K, T, allocator>::increase(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(error_code::invalid_param));
#endif
	wsize idx = 0;
	node_ptr_type node = node_ptr_type(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = m_algorithm.increase(node, idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_enum<K, T, allocator>::decrease(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(error_code::invalid_param));
#endif
	wsize idx = 0;
	node_ptr_type node = m_algorithm.decrease(node_ptr_type(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_enum<K, T, allocator>::decrease(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(error_code::invalid_param));
#endif
	wsize idx = 0;
	node_ptr_type node = node_ptr_type(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = m_algorithm.decrease(node, idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline wsize coffe::collections::binary_map_enum<K, T, allocator>::counter()const
{
	return m_map->m_count;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_object<K, T, allocator>::binary_map_object()
	: m_count(0)
	, m_root(null)
{
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_object<K, T, allocator>::binary_map_object(coffe::initializer_list<coffe::collections::pair<K, T>> list)
	: binary_map_object()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_object<K, T, allocator>::binary_map_object(const coffe::nullptr_t&)
	: binary_map_object()
{
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_object<K, T, allocator>::binary_map_object(binary_map_object&& other)
	: binary_map_object()
{
	m_count = other.m_count;
	m_root = other.m_root;

	other.m_count = 0;
	other.m_root = null;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_object<K, T, allocator>::binary_map_object(const binary_map_object& other)
	: binary_map_object()
{
	extend(&other);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_object<K, T, allocator>::binary_map_object(const binary_map_object* other)
	: binary_map_object()
{
	extend(other);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_object<K, T, allocator>::binary_map_object(coffe::collections::ienum<coffe::collections::pair<K, T>> const* other)
	: binary_map_object()
{
	extend(other);
}

template<typename K, typename T, template<typename> class allocator>
coffe::collections::binary_map_object<K, T, allocator>::~binary_map_object()
{
	//clear();
}


template<typename K, typename T, template<typename> class allocator>
inline const coffe::rtti_t& coffe::collections::binary_map_object<K, T, allocator>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((((astring("coffe::collections::binary_map<"_s) += coffe::type_of<K>().type_name()) += ","_s) += coffe::type_of<T>().type_name()) += ">"_s);
	static coffe::rtti_t const* parents[] = {
		&coffe::type_of<object>(),
		&coffe::type_of<imap_type>(),
		&coffe::type_of<ienum_type>()
	};
	static coffe::rtti_t const& info = coffe::rtti::regist(name, genre::class_type, size_of<binary_map_object<K, T, allocator>>(), align_of<binary_map_object<K, T, allocator>>(), parents, &intf::default_query_interface);
	return info;
}

template<typename K, typename T, template<typename> class allocator>
inline const coffe::rtti_t& coffe::collections::binary_map_object<K, T, allocator>::runtime_info()const
{
	return class_info();
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::query_interface(const coffe::rtti_t& id, coffe::unknown_ptr_t out)
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
	else if (id.type_id() == coffe::type_of<imap_type>().type_id()) {
		if (out == null) return false;
		*out = static_cast<imap_type*>(this);
		return true;
	}
	else if (id.type_id() == coffe::type_of<ienum_type>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ienum_type*>(this);
		return true;
	}
}


template<typename K, typename T, template<typename> class allocator>
void coffe::collections::binary_map_object<K, T, allocator>::dispose()
{
	clear();
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::is_empty()const {
	return m_count == 0;
}

template<typename K, typename T, template<typename> class allocator>
inline void coffe::collections::binary_map_object<K, T, allocator>::clear() {
	destroy_tree(m_root);
	m_root = null;
	m_count = 0;
}

template<typename K, typename T, template<typename> class allocator>
inline void coffe::collections::binary_map_object<K, T, allocator>::destroy_tree(coffe::collections::binary_map_node<K, T>* node) {
	if (node != null)
	{
		destroy_tree(node->left());
		node->left(null);

		destroy_tree(node->right());
		node->right(null);

		m_alloc.destroy(node);
		m_alloc.deallocate(node);
	}
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::move(binary_map_object& other)
{
	if (this == &other)
		return false;
	clear();
	m_count = other.m_count;
	m_root = other.m_root;
	other.m_count = 0;
	other.m_root = null;
	return true;
}


template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::pair<K, T>& coffe::collections::binary_map_object<K, T, allocator>::at(const coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(error_code::invalid_param));
#endif
	return node_ptr_type(it.current())->m_data;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::begin()
{
	wsize idx = 0;
	return iterator_type(const_cast<binary_map_object*>(this), three_iterator::begin(m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::const_forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::begin()const
{
	wsize idx = 0;
	return iterator_type(const_cast<binary_map_object*>(this), three_iterator::begin(m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::end()
{
	wsize idx = 0;
	return iterator_type(const_cast<binary_map_object*>(this), three_iterator::end(m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::const_forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::end()const
{
	wsize idx = 0;
	return iterator_type(const_cast<binary_map_object*>(this), three_iterator::end(m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::last()
{
	wsize idx = 0;
	return iterator_type(const_cast<binary_map_object*>(this), three_iterator::rbegin(m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::const_forward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::last()const
{
	wsize idx = 0;
	return iterator_type(const_cast<binary_map_object*>(this), three_iterator::rbegin(m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::backward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::rbegin()
{
	wsize idx = 0;
	return iterator_type(const_cast<binary_map_object*>(this), three_iterator::rbegin(m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::const_backward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::rbegin()const
{
	wsize idx = 0;
	return iterator_type(const_cast<binary_map_object*>(this), three_iterator::rbegin(m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::backward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::rend()
{
	wsize idx = 0;
	return iterator_type(const_cast<binary_map_object*>(this), three_iterator::rend(m_root, idx), 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::const_backward_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::rend()const
{
	wsize idx = 0;
	return iterator_type(const_cast<binary_map_object*>(this), three_iterator::rend(m_root, idx), 0);
}


template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::increase(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(error_code::invalid_param));
#endif
	wsize idx = 0;
	node_ptr_type node = three_iterator::increase(node_ptr_type(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::increase(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(error_code::invalid_param));
#endif
	wsize idx = 0;
	node_ptr_type node = node_ptr_type(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = three_iterator::increase(node, idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::decrease(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(error_code::invalid_param));
#endif
	wsize idx = 0;
	node_ptr_type node = three_iterator::decrease(node_ptr_type(it.current()), idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::decrease(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(error_code::invalid_param));
#endif
	wsize idx = 0;
	node_ptr_type node = node_ptr_type(it.current());
	for (int i = 0; i < offset && node != null; ++i)
		node = three_iterator::decrease(node, idx);
	it.current(node);
	return node != null;
}

template<typename K, typename T, template<typename> class allocator>
inline wsize coffe::collections::binary_map_object<K, T, allocator>::counter()const
{
	return m_count;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::ienum_ptr<coffe::collections::pair<K,T>> coffe::collections::binary_map_object<K, T, allocator>::enumerate(iteration_method_t method)const
{
	return new binary_map_enum<K, T, allocator>(const_cast<self_type*>(this), method);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_node<K, T>* coffe::collections::binary_map_object<K, T, allocator>::find_node(K const& key, coffe::collections::binary_map_node<K, T>* node)const
{
	//if (m_count == 0)
	//	return null;
	while (node != null)
	{
		if (logic_operation<logic_operation_type::same, K, K>::operate(key, node->key()))
			return node;
		if (logic_operation<logic_operation_type::minor, K, K>::operate(key, node->key()))
			node = node->left();
		else
			node = node->right();
	}
	return null;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_node<K, T>* coffe::collections::binary_map_object<K, T, allocator>::most_left(coffe::collections::binary_map_node<K, T>* node)const
{
	if (node == null)
		return null;
	while (node->left() != null)
		node = node->left();
	return node;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_node<K, T>* coffe::collections::binary_map_object<K, T, allocator>::most_right(coffe::collections::binary_map_node<K, T>* node)const
{
	if (node == null)
		return null;
	while (node->right() != null)
		node = node->right();
	return node;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::copy(coffe::collections::ienum<coffe::collections::pair<K, T>>const* items) {

	if (items == null)
		return false;
	clear();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

template<typename K, typename T, template<typename> class allocator>
inline void coffe::collections::binary_map_object<K, T, allocator>::extend(coffe::collections::ienum<coffe::collections::pair<K, T>>const* items) {

	if (items == null)
		return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::insert(K key, T value)
{
	if (m_root == null)
	{
		m_root = (node_ptr_type)m_alloc.allocate(1);
		m_alloc.template construct<node_type, pair_type&&>(m_root, pair_type(coffe::move(key), coffe::move(value)));
		m_count++;
		return iterator_type(this, m_root);
	}
	node_ptr_type node = m_root;
	node_ptr_type _new_node = null;
	while (true)
	{
		if (logic_operation<logic_operation_type::minor, K, K>::operate(key, node->key()))
		{
			if (node->left())
				node = node->left();
			else
			{
				_new_node = (node_ptr_type)m_alloc.allocate(1);
				m_alloc.template construct<node_type, pair_type&&>(_new_node, pair_type(key, coffe::move(value)));
				node->left(_new_node);
				break;
			}
		}
		else if (logic_operation<logic_operation_type::major, K, K>::operate(key, node->key()))
		{
			if (node->right())
				node = node->right();
			else
			{
				_new_node = (node_ptr_type)m_alloc.allocate(1);
				m_alloc.template construct<node_type, pair_type&&>(_new_node, pair_type(key, coffe::move(value)));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
			return end();
	}
	node_ptr_type parent;
	while (node->parent())
	{	
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->left()->key()))
		{
			parent = node->parent();
			node->node_type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->right()->key())) // Right Right Case
		{
			parent = node->parent();
			node->node_type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->left()->key()))	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->node_type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->right()->key())) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->node_type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	m_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->left()->key()))
		m_root = right_rotate(node);
	else if (balance < -1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->right()->key())) // Right Right Case
		m_root = left_rotate(node);
	else if (balance > 1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->left()->key()))	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		m_root = right_rotate(node);
	}
	else if (balance < -1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->right()->key())) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		m_root = left_rotate(node);
	}
	m_root->parent(null);
	m_count++;
	return iterator_type(this, _new_node);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::iterator<coffe::collections::pair<K,T>> coffe::collections::binary_map_object<K, T, allocator>::insert(coffe::collections::pair<K, T> pair)
{
	if (m_root == null)
	{
		m_root = (node_ptr_type)m_alloc.allocate(1);
		m_alloc.template construct<node_type, pair_type&&>(m_root, coffe::move(pair));
		m_count++;
		return iterator_type(this, m_root);
	}
	node_ptr_type node = m_root;
	node_ptr_type _new_node = null;
	K key = coffe::move(pair.key);
	while (true)
	{
		if (logic_operation<logic_operation_type::minor, K, K>::operate(key, node->key()))
		{
			if (node->left())
				node = node->left();
			else
			{
				_new_node = (node_ptr_type)m_alloc.allocate(1);
				m_alloc.template construct<node_type, pair_type&&>(_new_node, pair_type(key, coffe::move(pair.value)));
				node->left(_new_node);
				break;
			}
		}
		else if (logic_operation<logic_operation_type::major, K, K>::operate(key, node->key()))
		{
			if (node->right())
				node = node->right();
			else
			{
				_new_node = (node_ptr_type)m_alloc.allocate(1);
				m_alloc.template construct<node_type, pair_type&&>(_new_node, pair_type(key, coffe::move(pair.value)));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
			return end();
	}
	node_ptr_type parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->left()->key()))
		{
			parent = node->parent();
			node->node_type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->right()->key())) // Right Right Case
		{
			parent = node->parent();
			node->node_type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->left()->key()))	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->node_type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->right()->key())) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->node_type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	m_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->left()->key()))
		m_root = right_rotate(node);
	else if (balance < -1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->right()->key())) // Right Right Case
		m_root = left_rotate(node);
	else if (balance > 1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->left()->key()))	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		m_root = right_rotate(node);
	}
	else if (balance < -1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->right()->key())) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		m_root = left_rotate(node);
	}
	m_root->parent(null);
	m_count++;
	return iterator_type(this, _new_node);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::update(K key, T value)
{
	if (m_root == null)
	{
		m_root = (node_ptr_type)m_alloc.allocate(1);
		m_alloc.template construct<node_type, pair_type&&>(m_root, pair_type(coffe::move(key), coffe::move(value)));
		m_count++;
		return iterator_type(this, m_root);
	}
	node_ptr_type _new_node = null;
	node_ptr_type node = m_root;
	while (true)
	{
		if (logic_operation<logic_operation_type::minor, K, K>::operate(key, node->key()))
		{
			if (node->left())
				node = node->left();
			else
			{
				_new_node = (node_ptr_type)m_alloc.allocate(1);
				m_alloc.template construct<node_type, pair_type&&>(_new_node, pair_type(key, coffe::move(value)));
				node->left(_new_node);
				break;
			}
		}
		else if (logic_operation<logic_operation_type::major, K, K>::operate(key, node->key()))
		{
			if (node->right())
				node = node->right();
			else
			{
				_new_node = (node_ptr_type)m_alloc.allocate(1);
				m_alloc.template construct<node_type, pair_type&&>(_new_node, pair_type(key, coffe::move(value)));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
		{
			node->value(coffe::move(value));
			return iterator_type(this, node); //the key is updated
		}
	}
	node_ptr_type parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->left()->key()))
		{
			parent = node->parent();
			node->node_type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->right()->key())) // Right Right Case
		{
			parent = node->parent();
			node->node_type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->left()->key()))	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->node_type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->right()->key())) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->node_type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	m_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->left()->key()))
		m_root = right_rotate(node);
	else if (balance < -1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->right()->key())) // Right Right Case
		m_root = left_rotate(node);
	else if (balance > 1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->left()->key()))	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		m_root = right_rotate(node);
	}
	else if (balance < -1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->right()->key())) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		m_root = left_rotate(node);
	}
	m_root->parent(null);
	m_count++;
	return iterator_type(this, _new_node); //a new key was added
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::update(coffe::collections::pair<K, T> pair)
{
	if (m_root == null)
	{
		m_root = (node_ptr_type)m_alloc.allocate(1);
		m_alloc.template construct<node_type, pair_type&&>(m_root, coffe::move(pair));
		m_count++;
		return iterator_type(this, m_root);
	}
	node_ptr_type node = m_root;
	node_ptr_type _new_node = null;
	K key = coffe::move(pair.key);
	while (true)
	{
		if (logic_operation<logic_operation_type::minor, K, K>::operate(key, node->key()))
		{
			if (node->left())
				node = node->left();
			else
			{
				_new_node = (node_ptr_type)m_alloc.allocate(1);
				m_alloc.template construct<node_type, pair_type&&>(_new_node, pair_type(key, coffe::move(pair.value)));
				node->left(_new_node);
				break;
			}
		}
		else if (logic_operation<logic_operation_type::major, K, K>::operate(key, node->key()))
		{
			if (node->right())
				node = node->right();
			else
			{
				_new_node = (node_ptr_type)m_alloc.allocate(1);
				m_alloc.template construct<node_type, pair_type&&>(_new_node, pair_type(key, coffe::move(pair.value)));
				node->right(_new_node);
				break;
			}
		}
		else //if (key == node->key())
		{
			node->value(coffe::move(pair.value));
			return iterator_type(this, node); //the key is updated
		}
	}
	node_ptr_type parent;
	while (node->parent())
	{
		node->height(1 + max(node->left()->height(), node->right()->height()));
		int balance = node->balance();
		if (balance > 1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->left()->key()))
		{
			parent = node->parent();
			node->node_type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->right()->key())) // Right Right Case
		{
			parent = node->parent();
			node->node_type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else if (balance > 1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->left()->key()))	// Left Right Case
		{
			node->left(left_rotate(node->left()));
			parent = node->parent();
			node->node_type() == 1 ? parent->left(right_rotate(node)) : parent->right(right_rotate(node));
			node = parent;
		}
		else if (balance < -1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->right()->key())) // Right Left Case
		{
			node->right(right_rotate(node->right()));
			parent = node->parent();
			node->node_type() == 1 ? parent->left(left_rotate(node)) : parent->right(left_rotate(node));
			node = parent;
		}
		else
			node = node->parent();
	}

	m_root->height(1 + max(node->left()->height(), node->right()->height()));
	int balance = node->balance();
	if (balance > 1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->left()->key()))
		m_root = right_rotate(node);
	else if (balance < -1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->right()->key())) // Right Right Case
		m_root = left_rotate(node);
	else if (balance > 1 && logic_operation<logic_operation_type::major, K, K>::operate(key, node->left()->key()))	// Left Right Case
	{
		node->left(left_rotate(node->left()));
		m_root = right_rotate(node);
	}
	else if (balance < -1 && logic_operation<logic_operation_type::minor, K, K>::operate(key, node->right()->key())) // Right Left Case
	{
		node->right(right_rotate(node->right()));
		m_root = left_rotate(node);
	}
	m_root->parent(null);
	m_count++;
	return iterator_type(this, _new_node); //a new key was added
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::remove(K const& key)
{
	bool removed = false;
	m_root = remove_node(m_root, key, removed);
	m_root->parent(null);
	if (removed) m_count--;
	return removed;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::remove(K const& key, T& value)
{
	bool removed = false;
	m_root = remove_node(m_root, key, value, removed);
	m_root->parent(null);
	if (removed) m_count--;
	return removed;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::remove(coffe::collections::base_iterator<coffe::collections::pair<K, T>> it)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(error_code::invalid_param);
#endif
	node_ptr_type node = reinterpret_cast<node_ptr_type>(it.current());
	bool removed = false;
	if (node == m_root)
	{
		m_root = remove_node(node, node->key(), removed);
		m_root->parent(null);
		if (removed) m_count--;
		return removed;
	}
	else
	{
		switch (node->node_type())
		{
		case 1:
			node->parent()->left(remove_node(node, node->key(), removed));
			if (removed) m_count--;
			return removed;
		case 2:
			node->parent()->right(remove_node(node, node->key(), removed));
			if (removed) m_count--;
			return removed;
		}
	}
	return false;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::remove(coffe::collections::base_iterator<coffe::collections::pair<K, T>> it, T& value)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(error_code::invalid_param);
#endif
	node_ptr_type node = reinterpret_cast<node_ptr_type>(it.current());
	bool removed = false;
	if (node == m_root)
	{
		m_root = remove_node(node, node->key(), value, removed);
		if (removed) m_count--;
		return removed;
	}
	else
	{
		switch (node->node_type())
		{
		case 1:
			node->parent()->left(remove_node(node, node->key(), value, removed));
			if (removed) m_count--;
			return removed;
		case 2:
			node->parent()->right(remove_node(node, node->key(), value, removed));
			if (removed) m_count--;
			return removed;
		}
	}
	return false;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::collections::binary_map_object<K, T, allocator>::has_key(const K& key)const
{
	return find_node(key, m_root) != null;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::find(const K& key)
{
	//if (logic_operation<logic_operation_type::minor, K, K>::operate(key, node->key()))
	node_ptr_type node = find_node(key, m_root);
	if(node && logic_operation<logic_operation_type::same, K, K>::operate(node->key(), key))
		return iterator_type(const_cast<binary_map_object*>(this), node, 0);
	return iterator_type(const_cast<binary_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::const_iterator<coffe::collections::pair<K, T>> coffe::collections::binary_map_object<K, T, allocator>::find(const K& key)const
{
	node_ptr_type node = find_node(key, m_root);
	if (node && logic_operation<logic_operation_type::same, K, K>::operate(node->key(), key))
		return iterator_type(const_cast<binary_map_object*>(this), node, 0);
	return iterator_type(const_cast<binary_map_object*>(this), null, 0);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_node<K,T>* coffe::collections::binary_map_object<K, T, allocator>::right_rotate(coffe::collections::binary_map_node<K, T>* y)
{
	node_ptr_type x = y->left();
	y->left(x->right());
	x->right(y);
	y->height(max(y->left()->height(), y->right()->height()) + 1);
	x->height(max(x->left()->height(), x->right()->height()) + 1);
	return x;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_node<K, T>* coffe::collections::binary_map_object<K, T, allocator>::left_rotate(coffe::collections::binary_map_node<K, T>* x)
{
	node_ptr_type y = x->right();
	x->right(y->left());
	y->left(x);
	x->height(max(x->left()->height(), x->right()->height()) + 1);
	y->height(max(y->left()->height(), y->right()->height()) + 1);
	return y;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_node<K, T>* coffe::collections::binary_map_object<K, T, allocator>::remove_node(coffe::collections::binary_map_node<K, T>* node, K const& key, bool& removed)
{
	if (node == null)
		return null;

	if (logic_operation<logic_operation_type::minor, K, K>::operate(key, node->key()))
		node->left(remove_node(node->left(), key, removed));
	else if (logic_operation<logic_operation_type::major, K, K>::operate(key, node->key()))
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		if (!node->left() || !node->right())
		{
			node_ptr_type temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(coffe::move(temp->key()));
				node->value(coffe::move(temp->value()));
				m_alloc.destroy(temp);
				m_alloc.deallocate(temp);
			}
			else
			{
				m_alloc.destroy(node);
				m_alloc.deallocate(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_type temp = most_left(node->right());
			node->key(temp->key());
			node->value(coffe::move(temp->value()));
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
inline coffe::collections::binary_map_node<K, T>* coffe::collections::binary_map_object<K, T, allocator>::remove_node(coffe::collections::binary_map_node<K, T>* node, K const& key, T& value, bool& removed)
{
	if (node == null)
		return null;

	if (logic_operation<logic_operation_type::minor, K, K>::operate(key, node->key()))
		node->left(remove_node(node->left(), key, removed));
	else if (logic_operation<logic_operation_type::major, K, K>::operate(key, node->key()))
		node->right(remove_node(node->right(), key, removed));
	else
	{
		removed = true;
		value = coffe::move(node->value());
		if (!node->left() || !node->right())
		{
			node_ptr_type temp = node->left() ? node->left() : node->right();

			if (temp)
			{
				node->left(temp->left());
				node->right(temp->right());
				node->height(temp->height());
				node->key(coffe::move(temp->key()));
				node->value(coffe::move(temp->value()));
				m_alloc.destroy(temp);
				m_alloc.deallocate(temp);
			}
			else
			{
				m_alloc.destroy(node);
				m_alloc.deallocate(node);
				return null;
			}
		}
		else
		{
			bool _removed = false;
			node_ptr_type temp = most_left(node->right());
			node->key(temp->key());
			node->value(coffe::move(temp->value()));
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
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::object_wrapper()
	: m_ptr(null)
{
	set(new coffe::collections::binary_map_object<K, T, allocator>());
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::object_wrapper(coffe::nullptr_t const&)
	: m_ptr(null)
{

}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::object_wrapper(coffe::collections::binary_map_object<K, T, allocator>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::object_wrapper(coffe::collections::ienum<coffe::collections::pair<K, T>> const* store)
	: m_ptr(null)
{
	set(new collections::binary_map_object<K, T, allocator>(store));
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::object_wrapper(coffe::initializer_list<coffe::collections::pair<K, T>> list)
	: m_ptr(null)
{
	set(new collections::binary_map_object<K, T, allocator>(list));
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::object_wrapper(coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>> && other)
	: m_ptr(null)
{
	collections::binary_map_object<K, T, allocator> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::object_wrapper(coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>> const& other)
	: m_ptr(null)
{
	set(other.get());
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::~object_wrapper()
{
	reset();
}

template<typename K, typename T, template<typename> class allocator>
inline void coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

template<typename K, typename T, template<typename> class allocator>
inline bool coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::is_empty()const
{
	return m_ptr == null;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_object<K, T, allocator>* coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::get(void)const
{
	return m_ptr;
}

template<typename K, typename T, template<typename> class allocator>
inline void coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::set(coffe::collections::binary_map_object<K, T, allocator>* ptr)
{
	collections::binary_map_object<K, T, allocator> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_object<K, T, allocator> ** coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::addres_of(void)
{
	return &m_ptr;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>& coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator = (coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>> && other)
{
	coffe::collections::binary_map_object<K, T, allocator> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>& coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator = (coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>& coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator = (coffe::nullptr_t const&)
{
	reset();
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>& coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator = (coffe::collections::binary_map_object<K, T, allocator>* ptr)
{
	set(ptr);
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>& coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator = (coffe::collections::ienum<coffe::collections::pair<K, T>> const* store)
{
	if (is_empty())
		set(new collections::binary_map_object<K, T, allocator>(store));
	else
		m_ptr->copy(store);
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>& coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator += (coffe::collections::pair<K, T> pair)
{
	if (is_empty())
		set(new collections::binary_map_object<K, T, allocator>());
	m_ptr->update(coffe::move(pair));
	return*this;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator coffe::object_t()const {
	return m_ptr;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper_ptr<coffe::collections::binary_map_object<K, T, allocator>> coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator & (void)
{
	return this;
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::collections::binary_map_object<K, T, allocator> * coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator -> (void)const
{
	return get();
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator coffe::collections::binary_map_object<K, T, allocator> * (void)
{
	return get();
}

template<typename K, typename T, template<typename> class allocator>
inline coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator coffe::collections::binary_map_object<K, T, allocator> const* (void)const
{
	return get();
}

template<typename K, typename T, template<typename> class allocator>
inline T& coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator [] (K const& key)
{
	if (is_empty()) set(new collections::binary_map_object<K, T, allocator>());
	auto it = m_ptr->find(key);
	if (!it.is_valid()) {
		m_ptr->insert(key, T());
		it = m_ptr->find(key);
	}// throw(exception_t(error_code::array_overflow));
	return it->value;
}

template<typename K, typename T, template<typename> class allocator>
inline T coffe::object_wrapper<coffe::collections::binary_map_object<K, T, allocator>>::operator [] (K const& key)const
{
	if (is_empty()) throw(exception_t(error_code::invalid_memory));
	auto it = get()->find(key);
	if (!it.is_valid()) throw(exception_t(error_code::array_overflow));
	return it->value();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif//__COFFE_BINARY_TREE_INL__
