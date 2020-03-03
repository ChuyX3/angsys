#ifndef __COFFE_BTREE_H__
#error Can't include btree.h, please include btree.h inside
#elif !defined __COFFE_BTREE_INL__
#define __COFFE_BTREE_INL__

template<typename T, wsize N>
inline coffe::collections::btree_node<T, N>* coffe::algorithms::btree_inorder_iteration<T, N>::begin(coffe::collections::btree_node<T, N>* node, wsize& idx)
{
	return most_left(node, idx);
}

template<typename T, wsize N>
inline coffe::collections::btree_node<T, N>* coffe::algorithms::btree_inorder_iteration<T, N>::end(coffe::collections::btree_node<T, N>* node, wsize& idx)
{
	idx = 0;
	return null;
}

template<typename T, wsize N>
inline coffe::collections::btree_node<T, N>* coffe::algorithms::btree_inorder_iteration<T, N>::rbegin(coffe::collections::btree_node<T, N>* node, wsize& idx)
{
	return most_right(node, idx);
}

template<typename T, wsize N>
inline coffe::collections::btree_node<T, N>* coffe::algorithms::btree_inorder_iteration<T, N>::rend(coffe::collections::btree_node<T, N>* node, wsize& idx)
{
	idx = 0;
	return null;
}

template<typename T, wsize N>
inline coffe::collections::btree_node<T, N>* coffe::algorithms::btree_inorder_iteration<T, N>::increase(coffe::collections::btree_node<T, N>* node, wsize& idx)
{
	if (node == null)
		return null;
	

	return null;
}

template<typename T, wsize N>
inline coffe::collections::btree_node<T, N>* coffe::algorithms::btree_inorder_iteration<T, N>::decrease(coffe::collections::btree_node<T, N>* node, wsize& idx)
{

	return null;
}

template<typename T, wsize N>
inline coffe::collections::btree_node<T, N>* coffe::algorithms::btree_inorder_iteration<T, N>::most_left(coffe::collections::btree_node<T, N>* node, wsize& idx)
{
	while (node && node->child(0))
		node = node->child(0);
	return node;
}

template<typename T, wsize N>
inline coffe::collections::btree_node<T, N>* coffe::algorithms::btree_inorder_iteration<T, N>::most_right(coffe::collections::btree_node<T, N>* node, wsize& idx)
{
	while (node && node->child(node->width()))
		node = node->child(node->width());
	return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::btree_map_object<K, T, N, allocator>::btree_map_object()
	: m_count(0)
	, m_root(null)
	, m_algorithm(coffe::algorithms::binary_tree_inorder_iteration<pair<K, T>>())
{
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::btree_map_object<K, T, N, allocator>::btree_map_object(coffe::initializer_list<coffe::collections::pair<K, T>> list)
	: btree_map_object()
{
	for (auto it = list.begin(); it < list.end(); ++it)
		insert(*it);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::btree_map_object<K, T, N, allocator>::btree_map_object(const coffe::nullptr_t&)
	: btree_map_object()
{
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::btree_map_object<K, T, N, allocator>::btree_map_object(btree_map_object&& other)
	: btree_map_object()
{
	m_count = other.m_count;
	m_root = other.m_root;

	other.m_count = 0;
	other.m_root = null;
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::btree_map_object<K, T, N, allocator>::btree_map_object(const btree_map_object& other)
	: btree_map_object()
{
	extend(&other);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::btree_map_object<K, T, N, allocator>::btree_map_object(const btree_map_object* other)
	: btree_map_object()
{
	extend(other);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::btree_map_object<K, T, N, allocator>::btree_map_object(coffe::collections::ienum<coffe::collections::pair<K, T>> const* other)
	: btree_map_object()
{
	extend(other);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
coffe::collections::btree_map_object<K, T, N, allocator>::~btree_map_object()
{
	//clear();
}


template<typename K, typename T, wsize N, template<typename> class allocator>
inline const coffe::rtti_t& coffe::collections::btree_map_object<K, T, N, allocator>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((((astring("coffe::collections::btree_map<"_s) += coffe::type_of<K>().type_name()) += ","_s) += coffe::type_of<T>().type_name()) += ">"_s);
	static coffe::rtti_t const* parents[] = { &coffe::type_of<imap_type>() };
	static coffe::rtti_t const& info = coffe::rtti::regist(name, genre::class_type, size_of<btree_map_object<K, T, N, allocator>>(), align_of<btree_map_object<K, T, N, allocator>>(), parents, &intf::default_query_interface);
	return info;
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline const coffe::rtti_t& coffe::collections::btree_map_object<K, T, N, allocator>::runtime_info()const
{
	return class_info();
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline bool coffe::collections::btree_map_object<K, T, N, allocator>::query_interface(const coffe::rtti_t& id, coffe::unknown_ptr_t out)
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


template<typename K, typename T, wsize N, template<typename> class allocator>
void coffe::collections::btree_map_object<K, T, N, allocator>::dispose()
{
	clear();
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline bool coffe::collections::btree_map_object<K, T, N, allocator>::is_empty()const {
	return m_count == 0;
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline void coffe::collections::btree_map_object<K, T, N, allocator>::clear() {
	destroy_tree(m_root);
	m_root = null;
	m_count = 0;
}

//template<typename K, typename T, wsize N, template<typename> class allocator>
//inline void coffe::collections::btree_map_object<K, T, N, allocator>::destroy_tree(coffe::collections::binary_map_node<K, T>* node) {
//	if (node != null)
//	{
//		destroy_tree(node->left());
//		node->left(null);
//
//		destroy_tree(node->right());
//		node->right(null);
//
//		m_alloc.destroy(node);
//		m_alloc.deallocate(node);
//	}
//}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline bool coffe::collections::btree_map_object<K, T, N, allocator>::move(btree_map_object& other)
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

template<typename K, typename T, wsize N, template<typename> class allocator>
inline void coffe::collections::btree_map_object<K, T, N, allocator>::iteration_method(coffe::collections::iteration_method_t method)
{
	switch (method)
	{
	case collections::iteration_method::inorder:
		m_algorithm = algorithms::binary_tree_inorder_iteration<pair_type>();
		break;
	default:
		break;
	}
}

//template<typename K, typename T, wsize N, template<typename> class allocator>
//inline coffe::collections::binary_map_node<K, T>* coffe::collections::btree_map_object<K, T, N, allocator>::find_node(K const& key, coffe::collections::binary_map_node<K, T>* node)const
//{
//	//if (m_count == 0)
//	//	return null;
//	while (node != null)
//	{
//		if (logic_operation<logic_operation_type::same, K, K>::operate(key, node->key()))
//			return node;
//		if (logic_operation<logic_operation_type::minor, K, K>::operate(key, node->key()))
//			node = node->left();
//		else
//			node = node->right();
//	}
//	return null;
//}

//template<typename K, typename T, wsize N, template<typename> class allocator>
//inline coffe::collections::binary_map_node<K, T>* coffe::collections::btree_map_object<K, T, N, allocator>::most_left(coffe::collections::binary_map_node<K, T>* node)const
//{
//	if (node == null)
//		return null;
//	while (node->left() != null)
//		node = node->left();
//	return node;
//}
//
//template<typename K, typename T, wsize N, template<typename> class allocator>
//inline coffe::collections::binary_map_node<K, T>* coffe::collections::btree_map_object<K, T, N, allocator>::most_right(coffe::collections::binary_map_node<K, T>* node)const
//{
//	if (node == null)
//		return null;
//	while (node->right() != null)
//		node = node->right();
//	return node;
//}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::pair<K, T>& coffe::collections::btree_map_object<K, T, N, allocator>::at(const coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(error_code::invalid_param));
#endif
	return node_ptr_type(it.current())->m_data;
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::forward_iterator<coffe::collections::pair<K, T>> coffe::collections::btree_map_object<K, T, N, allocator>::begin()
{
	wsize idx = 0;
	return iterator_type(const_cast<btree_map_object*>(this), m_algorithm.begin(m_root, idx), 0);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::const_forward_iterator<coffe::collections::pair<K, T>> coffe::collections::btree_map_object<K, T, N, allocator>::begin()const
{
	wsize idx = 0;
	return iterator_type(const_cast<btree_map_object*>(this), m_algorithm.begin(m_root, idx), 0);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::forward_iterator<coffe::collections::pair<K, T>> coffe::collections::btree_map_object<K, T, N, allocator>::end()
{
	wsize idx = 0;
	return iterator_type(const_cast<btree_map_object*>(this), m_algorithm.end(m_root, idx), 0);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::const_forward_iterator<coffe::collections::pair<K, T>> coffe::collections::btree_map_object<K, T, N, allocator>::end()const
{
	wsize idx = 0;
	return iterator_type(const_cast<btree_map_object*>(this), m_algorithm.end(m_root, idx), 0);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::forward_iterator<coffe::collections::pair<K, T>> coffe::collections::btree_map_object<K, T, N, allocator>::last()
{
	wsize idx = 0;
	return iterator_type(const_cast<btree_map_object*>(this), m_algorithm.rbegin(m_root, idx), 0);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::const_forward_iterator<coffe::collections::pair<K, T>> coffe::collections::btree_map_object<K, T, N, allocator>::last()const
{
	wsize idx = 0;
	return iterator_type(const_cast<btree_map_object*>(this), m_algorithm.rbegin(m_root, idx), 0);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::backward_iterator<coffe::collections::pair<K, T>> coffe::collections::btree_map_object<K, T, N, allocator>::rbegin()
{
	wsize idx = 0;
	return iterator_type(const_cast<btree_map_object*>(this), m_algorithm.rbegin(m_root, idx), 0);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::const_backward_iterator<coffe::collections::pair<K, T>> coffe::collections::btree_map_object<K, T, N, allocator>::rbegin()const
{
	wsize idx = 0;
	return iterator_type(const_cast<btree_map_object*>(this), m_algorithm.rbegin(m_root, idx), 0);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::backward_iterator<coffe::collections::pair<K, T>> coffe::collections::btree_map_object<K, T, N, allocator>::rend()
{
	wsize idx = 0;
	return iterator_type(const_cast<btree_map_object*>(this), m_algorithm.rend(m_root, idx), 0);
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline coffe::collections::const_backward_iterator<coffe::collections::pair<K, T>> coffe::collections::btree_map_object<K, T, N, allocator>::rend()const
{
	wsize idx = 0;
	return iterator_type(const_cast<btree_map_object*>(this), m_algorithm.rend(m_root, idx), 0);
}


template<typename K, typename T, wsize N, template<typename> class allocator>
inline bool coffe::collections::btree_map_object<K, T, N, allocator>::increase(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it)const
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

template<typename K, typename T, wsize N, template<typename> class allocator>
inline bool coffe::collections::btree_map_object<K, T, N, allocator>::increase(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it, int offset)const
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

template<typename K, typename T, wsize N, template<typename> class allocator>
inline bool coffe::collections::btree_map_object<K, T, N, allocator>::decrease(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it)const
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

template<typename K, typename T, wsize N, template<typename> class allocator>
inline bool coffe::collections::btree_map_object<K, T, N, allocator>::decrease(coffe::collections::base_iterator<coffe::collections::pair<K, T>>& it, int offset)const
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

template<typename K, typename T, wsize N, template<typename> class allocator>
inline wsize coffe::collections::btree_map_object<K, T, N, allocator>::counter()const
{
	return m_count;
}

template<typename K, typename T, wsize N, template<typename> class allocator>
inline bool coffe::collections::btree_map_object<K, T, N, allocator>::copy(coffe::collections::ienum<coffe::collections::pair<K, T>>const* items) {

	if (items == null)
		return false;
	clear();
	for (auto it = items->begin(); it.is_valid(); ++it)
		insert(*it);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif//__COFFE_BTREE_INL__
