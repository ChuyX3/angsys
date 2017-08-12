#ifndef __ANG_BTREE_H__
#error can't include btree.hpp, please include btree.h inside
#elif !defined __ANG_BTREE_HPP__
#define __ANG_BTREE_HPP__


template<typename _pair, ang::uint_t min_degree>
ang::collections::btree_node<_pair, min_degree>* ang::collections::btree_inorder_algorithm<_pair, min_degree>::begin(ang::collections::btree_node<_pair, min_degree>* node, ang::index_t& idx)
{
	if (node == null)
		return null;
	while (!node->is_leaf())
		node = node->first_child();
	idx = 0;
	return node;
}

template<typename _pair, ang::uint_t min_degree>
ang::collections::btree_node<_pair, min_degree>* ang::collections::btree_inorder_algorithm<_pair, min_degree>::end(ang::collections::btree_node<_pair, min_degree>* node, ang::index_t& idx)
{
	if (node == null)
		return null;
	while (!node->is_leaf())
		node = node->last_child();
	idx = node->count() - 1;
	return node;
}

template<typename _pair, ang::uint_t min_degree>
ang::collections::btree_node<_pair, min_degree>* ang::collections::btree_inorder_algorithm<_pair, min_degree>::next(ang::collections::btree_node<_pair, min_degree>* node, ang::index_t& idx)
{
	if (node == null)
		return null;
	if (node->is_leaf())
	{
		if ((idx + 1) < node->count())
		{
			idx++;
			return node;
		}
		else
		{
			do
			{
				idx = node->current_index();
				node = node->parent();
			} while (node && node->count() == idx);

		}
	}
	else
	{
		if (idx > node->count())
			return null;
		node = most_left(node->child(idx + 1), idx);
	}
	return node;
}

template<typename _pair, ang::uint_t min_degree>
ang::collections::btree_node<_pair, min_degree>* ang::collections::btree_inorder_algorithm<_pair, min_degree>::prev(ang::collections::btree_node<_pair, min_degree>* node, ang::index_t& idx)
{
	if (node == null)
		return null;
	if (node->is_leaf())
	{
		if (idx > 0)
		{
			idx--;
			return node;
		}
		else
		{
			do
			{
				idx = node->current_index();
				node = node->Parent();
			} while (node && 0 == idx);
			idx--;
		}
	}
	else
	{
		node = most_right(node->child(idx), idx);
	}
	return node;
}

template<typename _pair, ang::uint_t min_degree>
ang::collections::btree_node<_pair, min_degree>* ang::collections::btree_inorder_algorithm<_pair, min_degree>::most_left(ang::collections::btree_node<_pair, min_degree>* node, ang::index_t& idx)
{
	if (node == null)
		return null;
	while (node->first_child() != null)
		node = node->first_child();
	idx = 0;
	return node;
}

template<typename _pair, ang::uint_t min_degree>
ang::collections::btree_node<_pair, min_degree>* ang::collections::btree_inorder_algorithm<_pair, min_degree>::most_right(ang::collections::btree_node<_pair, min_degree>* node, ang::index_t& idx)
{
	if (node == null)
		return null;
	while (node->last_child() != null)
		node = node->last_child();
	idx = node->count() - 1;
	return node;
}


//////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _pair, ang::uint_t min_degree>
inline ang::type_name_t ang::collections::btree_node<_pair, min_degree>::class_name()
{
	static astring_t name = ang::astring("ang::collections::btree_node<") + type_name<_pair>() + "," + uinterger::to_astring(min_degree) + ">";
	return name->cstr();
}

////////////////////////////////////////////////////////
template<typename K, typename T, ang::uint_t _min_degree>
inline ang::collections::btree_data<K, T, _min_degree>::btree_data()
	: _count(0)
	, _root(null)
	, allocator(null)
	, iteration_algorithm(btree_inorder_algorithm_t())
{
	allocator = memory::allocator_manager::get_allocator(0);
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::collections::btree_data<K, T, _min_degree>::btree_data(std::initializer_list<typename ang::collections::btree_data<K, T, _min_degree>::pair_t> list)
	: btree_data()
{

}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::collections::btree_data<K, T, _min_degree>::btree_data(const std::nullptr_t&)
	: btree_data()
{

}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::collections::btree_data<K, T, _min_degree>::btree_data(const btree_data& ar)
	: btree_data()
{
	copy(ar);
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::collections::btree_data<K, T, _min_degree>::btree_data(const btree_data* ar)
	: btree_data()
{
	if(ar)copy(*ar);
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::collections::btree_data<K, T, _min_degree>::btree_data(const ienum_t& store)
	: btree_data()
{
	copy(store);
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::collections::btree_data<K, T, _min_degree>::~btree_data()
{
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::bool_t ang::collections::btree_data<K, T, _min_degree>::is_empty()const
{
	return _count == 0;
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::uint_t ang::collections::btree_data<K, T, _min_degree>::size()const
{
	return _count;
}

template<typename K, typename T, ang::uint_t _min_degree>
inline void ang::collections::btree_data<K, T, _min_degree>::clean()
{
	node_ptr_t node = root();
	root(null);
	_count = 0;
	remove_all_recursively(node);
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::bool_t ang::collections::btree_data<K, T, _min_degree>::move(btree_data& other)
{
	if (&other == this)
		return false;
	clean();

	_count = other._count;
	_root = other._root;
	allocator = other.allocator;

	other._count = 0;
	other._root = null;
}

template<typename K, typename T, ang::uint_t _min_degree>
inline typename ang::collections::btree_data<K, T, _min_degree>::node_ptr_t ang::collections::btree_data<K, T, _min_degree>::root()const
{
	return _root;
}

template<typename K, typename T, ang::uint_t _min_degree>
inline void ang::collections::btree_data<K, T, _min_degree>::root(node_ptr_t node)
{
	_root = node;
}

template<typename K, typename T, ang::uint_t _min_degree>
inline void ang::collections::btree_data<K, T, _min_degree>::set_allocator(ang::memory::iallocator*)
{
	//TODO:


}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::memory::iallocator* ang::collections::btree_data<K, T, _min_degree>::get_allocator()const
{
	return allocator;
}



template<typename K, typename T, ang::uint_t _min_degree>
inline typename ang::collections::btree_data<K, T, _min_degree>::pair_t& ang::collections::btree_data<K, T, _min_degree>::at(const iterator_t& it)const
{
	if (!has_items()) throw(exception(except_code::invalid_memory));
	if (it.Parent() != this) throw(exception(except_code::invalid_param));
	return reinterpret_cast<node_ptr_t>(it.current())->pair((index_t)it.offset());
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::collections::iterator<typename ang::collections::btree_data<K, T, _min_degree>::pair_t> ang::collections::btree_data<K, T, _min_degree>::begin()const
{
	index_t idx;
	node_ptr_t node = iteration_algorithm.begin(root(), idx);
	return iterator_t(const_cast<btree_data<K, T, _min_degree>*>(this), node, idx);
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::collections::iterator<typename ang::collections::btree_data<K, T, _min_degree>::pair_t> ang::collections::btree_data<K, T, _min_degree>::end()const
{
	index_t idx;
	node_ptr_t node = iteration_algorithm.end(root(), idx);
	return iterator_t(const_cast<btree_data<K, T, _min_degree>*>(this), node, idx);
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::bool_t ang::collections::btree_data<K, T, _min_degree>::next(iterator_t& it)const
{
	if (!has_items()) throw(exception(except_code::invalid_memory));
	if (it.Parent() != this) throw(exception(except_code::invalid_param));
	index_t idx = it.offset();
	node_ptr_t node = iteration_algorithm.next((node_ptr_t)it.current(), idx);
	it.current(node);
	it.offset(idx);
	if (node == null)
		return false;
	return true;
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::bool_t ang::collections::btree_data<K, T, _min_degree>::next(iterator_t& it, int_t offset)const
{
	if (!has_items()) throw(exception(except_code::invalid_memory));
	if (it.Parent() != this) throw(exception(except_code::invalid_param));
	index_t idx = it.offset();
	node_ptr_t node = (node_ptr_t)it.current();

	while (offset > 0) {
		node = iteration_algorithm.next(node, idx);
		if (node == null)
			break;
	}
	it.current(node);
	it.offset(idx);
	if (node == null)
		return false;
	return true;
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::bool_t ang::collections::btree_data<K, T, _min_degree>::prev(iterator_t& it)const
{
	if (!has_items()) throw(exception(except_code::invalid_memory));
	if (it.Parent() != this) throw(exception(except_code::invalid_param));
	index_t idx = it.offset();
	node_ptr_t node = iteration_algorithm.prev((node_ptr_t)it.current(), idx);
	it.current(node);
	it.offset(idx);
	if (node == null)
		return false;
	return true;
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::bool_t ang::collections::btree_data<K, T, _min_degree>::prev(iterator_t& it, int_t offset)const
{
	if (!has_items()) throw(exception(except_code::invalid_memory));
	if (it.Parent() != this) throw(exception(except_code::invalid_param));
	index_t idx = it.offset();
	node_ptr_t node = (node_ptr_t)it.current();

	while (offset > 0) {
		node = iteration_algorithm.prev(node, idx);
		if (node == null)
			break;
	}
	it.current(node);
	it.offset(idx);
	if (node == null)
		return false;
	return true;
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::uint_t ang::collections::btree_data<K, T, _min_degree>::counter()const
{
	return _count;
}

template<typename K, typename T, ang::uint_t _min_degree>
inline ang::bool_t ang::collections::btree_data<K, T, _min_degree>::has_items()const
{
	return _count != 0;
}






#endif//__ANG_BTREE_HPP__
