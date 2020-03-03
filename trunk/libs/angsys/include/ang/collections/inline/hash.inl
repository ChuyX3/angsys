#ifndef __COFFE_COLLECTIONS_HASH_MAP_H__
#error Can't include HASH_MAP.inl, please include hash_map.h inside
#elif !defined __COFFE_COLLECTIONS_HASH_MAP_INL__
#define __COFFE_COLLECTIONS_HASH_MAP_INL__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// hash_map_object<K, T, allocator, hash_index_maker> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace coffe
{
	template<typename K, typename T, template<typename>class A, template<typename>class H>
	struct intf_class_info<coffe::collections::hash_map_object<K, T, A, H>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<coffe::collections::hash_map_object<K, T, A, H>>::types();
			static coffe::rtti_t const& info = rtti::regist(
				[]()->fast_astring_t {
				fast_astring_t out;
				out << "coffe::collections::hash_map<"_sv << coffe::type_of<K>().type_name() << coffe::type_of<T>().type_name() << ">"_sv;
				return coffe::move(out); }()
					, gender::class_type
					, size_of<coffe::collections::hash_map_object<K, T, A, H>>()
					, align_of<coffe::collections::hash_map_object<K, T, A, H>>()
					, parents
					, &iintf::default_query_interface);
			return info;
		}
	};
}


template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::collections::hash_map_object<K, T, A, H>::hash_map_object()
	: fast_hash_map_t()
{

}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::collections::hash_map_object<K, T, A, H>::hash_map_object(wsize reserve)
	: fast_hash_map_t(reserve)
{

}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::collections::hash_map_object<K, T, A, H>::hash_map_object(const coffe::nullptr_t&)
	: fast_hash_map_t()
{
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::collections::hash_map_object<K, T, A, H>::hash_map_object(hash_map_object<K, T, A, H>&& map)
	: fast_hash_map_t(coffe::forward<hash_map_object<K, T, A, H>>(map))
{
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::collections::hash_map_object<K, T, A, H>::hash_map_object(const hash_map_object<K, T, A, H>& map)
	: fast_hash_map_t(map)
{
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::collections::hash_map_object<K, T, A, H>::hash_map_object(fast_hash_map_t&& map)
	: fast_hash_map_t(coffe::forward<fast_hash_map_t>(map))
{
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::collections::hash_map_object<K, T, A, H>::hash_map_object(const fast_hash_map_t& map)
	: fast_hash_map_t(map)
{
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::collections::hash_map_object<K, T, A, H>::hash_map_object(const coffe::collections::ienum_ptr<pair_view_type>& store)
	: fast_hash_map_t()
{
	for (pair_view_type const& pair : store) {
		insert(pair);
	}
}

template<typename K, typename T, template<typename> class A, template<typename> class H> template<typename U, typename V>
inline coffe::collections::hash_map_object<K, T, A, H>::hash_map_object(coffe::array_view<coffe::collections::pair<U, V>> items)
	: fast_hash_map_t()
{
	for (auto const& pair : items) {
		insert((key_type)pair.key, (value_type)pair.value);
	}
}

template<typename K, typename T, template<typename> class A, template<typename> class H> template<typename U, typename V>
inline coffe::collections::hash_map_object<K, T, A, H>::hash_map_object(coffe::initializer_list<coffe::collections::pair<U, V>> list)
	: fast_hash_map_t()
{
	for (auto const& pair : list) {
		insert((key_type)pair.key, (value_type)pair.value);
	}
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::collections::hash_map_object<K, T, A, H>::~hash_map_object()
{

}

template<typename K, typename T, template<typename> class A, template<typename> class H> template<typename U, typename V>
inline void coffe::collections::hash_map_object<K, T, A, H>::copy(coffe::collections::array_view<coffe::collections::pair<U,V>>const& items)
{
	fast_hash_map_t::clear();
	for (auto const& pair : items) {
		insert((key_type)pair.key, (value_type)pair.value);
	}
}

template<typename K, typename T, template<typename> class A, template<typename> class H> template<typename U, typename V>
inline void coffe::collections::hash_map_object<K, T, A, H>::extend(coffe::collections::array_view<coffe::collections::pair<U, V>>const& items)
{
	for (auto const& pair : items) {
		insert((key_type)pair.key, (value_type)pair.value);
	}
}


template<typename K, typename T, template<typename> class A, template<typename> class H>
inline wsize coffe::collections::hash_map_object<K, T, A, H>::counter()const
{
	return fast_hash_map_t::size();
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::auto_type<typename coffe::collections::imap<K, T>::pair_view_type>::return_type coffe::collections::hash_map_object<K, T, A, H>::at(typename coffe::collections::hash_map_object<K, T, A, H>::base_iterator_t const& it)
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(error_code::invalid_param);
	if (is_empty())
		throw_exception(error_code::invalid_memory);
#endif
	node_type* node = reinterpret_cast<node_type*>(it.current());
	return &(pair_view_type&)node->data;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline bool coffe::collections::hash_map_object<K, T, A, H>::increase(typename coffe::collections::hash_map_object<K, T, A, H>::base_iterator_t& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		return false;
#endif
	node_type* node = reinterpret_cast<node_type*>(it.current());
	it.current(node->next);
	return true;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline bool coffe::collections::hash_map_object<K, T, A, H>::increase(typename coffe::collections::hash_map_object<K, T, A, H>::base_iterator_t& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		return false;
#endif
	node_type* node = reinterpret_cast<node_type*>(it.current());
	while (node != null && offset > 0) {
		node = node->next;
		offset--;
	}
	it.current(node);
	return true;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline bool coffe::collections::hash_map_object<K, T, A, H>::decrease(typename coffe::collections::hash_map_object<K, T, A, H>::base_iterator_t& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		return false;
#endif
	node_type* node = reinterpret_cast<node_type*>(it.current());
	it.current(node->prev);
	return true;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline bool coffe::collections::hash_map_object<K, T, A, H>::decrease(typename coffe::collections::hash_map_object<K, T, A, H>::base_iterator_t& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		return false;
#endif
	node_type* node = reinterpret_cast<node_type*>(it.current());
	while (node != null && offset > 0) {
		node = node->prev;
		offset--;
	}
	it.current(node);
	return true;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::forward_iterator_t coffe::collections::hash_map_object<K, T, A, H>::begin()
{
	windex i = 0;
	node_type_ptr node = null;
	while (node == null && i < m_table.size())
		node = m_table[i++].first;
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, --i);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::forward_iterator_t coffe::collections::hash_map_object<K, T, A, H>::end()
{
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), null, 0);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::const_forward_iterator_t coffe::collections::hash_map_object<K, T, A, H>::begin()const
{
	windex i = 0;
	node_type_ptr node = null;
	while (node == null && i < m_table.size())
		node = m_table[i++].first;
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, --i);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::const_forward_iterator_t coffe::collections::hash_map_object<K, T, A, H>::end()const
{
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), null, 0);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::forward_iterator_t coffe::collections::hash_map_object<K, T, A, H>::last()
{
	windex i = m_table.size();
	node_type_ptr node = null;
	while (node == null && i != 0) {
		node = m_table[--i].last;
	}
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, i);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::const_forward_iterator_t coffe::collections::hash_map_object<K, T, A, H>::last()const
{
	windex i = m_table.size();
	node_type_ptr node = null;
	while (node == null && i != 0) {
		node = m_table[--i].last;
	}
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, i);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::backward_iterator_t coffe::collections::hash_map_object<K, T, A, H>::rbegin()
{
	windex i = m_table.size();
	node_type_ptr node = null;
	while (node == null && i != 0) {
		node = m_table[--i].last;
	}
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, i);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::backward_iterator_t coffe::collections::hash_map_object<K, T, A, H>::rend()
{
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), null, 0);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::const_backward_iterator_t coffe::collections::hash_map_object<K, T, A, H>::rbegin()const
{
	windex i = m_table.size();
	node_type_ptr node = null;
	while (node == null && i != 0) {
		node = m_table[--i].last;
	}
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, i);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::const_backward_iterator_t coffe::collections::hash_map_object<K, T, A, H>::rend()const
{
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), null, 0);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
typename coffe::collections::hash_map_object<K, T, A, H>::iterator_t coffe::collections::hash_map_object<K, T, A, H>::find(coffe::function<bool(typename coffe::collections::auto_type<typename coffe::collections::hash_map_object<K, T, A, H>::pair_view_type>::arg_type const&)> func, bool invert)const
{
	if (invert)
	{
		for (const_backward_iterator_t it = rbegin(); it != rend(); it++) {
			if (func(*it))
				return it;
		}
	}
	else
	{
		for (const_forward_iterator_t it = begin(); it != end(); it++) {
			if (func(*it))
				return it;
		}
	}
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
typename coffe::collections::hash_map_object<K, T, A, H>::iterator_t coffe::collections::hash_map_object<K, T, A, H>::find(coffe::function<bool(typename coffe::collections::auto_type<typename coffe::collections::hash_map_object<K, T, A, H>::pair_view_type>::arg_type const&)> func, base_iterator_t next_to, bool invert)const
{
	//TODO:
	if (invert)
	{
		for (const_backward_iterator_t it = rbegin(); it != rend(); it++) {
			if (func(*it))
				return it;
		}
	}
	else
	{
		for (const_forward_iterator_t it = begin(); it != end(); it++) {
			if (func(*it))
				return it;
		}
	}
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
typename coffe::collections::hash_map_object<K, T, A, H>::ienum_ptr_type coffe::collections::hash_map_object<K, T, A, H>::find_all(coffe::function<bool(typename coffe::collections::auto_type<typename coffe::collections::hash_map_object<K, T, A, H>::pair_view_type>::arg_type const&)> func)const
{
	smart_vector<pair_view_type> out = null;
	for (const_backward_iterator_t it = rbegin(); it != rend(); it++) {
		if (func(*it))
			out += *it;
	}
	return out.get();
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::ienum_ptr_type coffe::collections::hash_map_object<K, T, A, H>::enumerate(iteration_method_t)const
{
	return const_cast<coffe::collections::hash_map_object<K, T, A, H>*>(this);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline void coffe::collections::hash_map_object<K, T, A, H>::copy(typename const coffe::collections::hash_map_object<K, T, A, H>::ienum_ptr_type& items)
{
	fast_hash_map_t::clear();
	for (pair_view_type const& pair : items)
		insert(pair.key, pair.value);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline void coffe::collections::hash_map_object<K, T, A, H>::extend(typename const coffe::collections::hash_map_object<K, T, A, H>::ienum_ptr_type& items)
{
	for (pair_view_type const& pair : items)
		insert(pair.key, pair.value);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::iterator_t coffe::collections::hash_map_object<K, T, A, H>::insert(typename coffe::collections::auto_type<K>::arg_type const& key, typename coffe::collections::auto_type<T>::arg_type && value)
{
	typename fast_hash_map_t::iterator_t it = fast_hash_map_t::insert(key, coffe::forward<value_type>(value));
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), fast_hash_map_t::get_node(it), fast_hash_map_t::get_index(it));
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::iterator_t coffe::collections::hash_map_object<K, T, A, H>::insert(typename coffe::collections::auto_type<K>::arg_type const& key, typename coffe::collections::auto_type<T>::arg_type const& value)
{
	typename fast_hash_map_t::iterator_t it = fast_hash_map_t::insert(key, value);
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), fast_hash_map_t::get_node(it), fast_hash_map_t::get_index(it));
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::iterator_t coffe::collections::hash_map_object<K, T, A, H>::insert(typename coffe::collections::hash_map_object<K, T, A, H>::pair_type && pair)
{
	typename fast_hash_map_t::iterator_t it = fast_hash_map_t::insert(coffe::forward<pair_type>(pair));
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), fast_hash_map_t::get_node(it), fast_hash_map_t::get_index(it));
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::iterator_t coffe::collections::hash_map_object<K, T, A, H>::insert(typename coffe::collections::hash_map_object<K, T, A, H>::pair_type const& pair)
{
	typename fast_hash_map_t::iterator_t it = fast_hash_map_t::insert(pair);
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), fast_hash_map_t::get_node(it), fast_hash_map_t::get_index(it));
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::iterator_t coffe::collections::hash_map_object<K, T, A, H>::update(typename coffe::collections::auto_type<K>::arg_type const& key, typename coffe::collections::auto_type<T>::arg_type && value)
{
	ulong64 hash;
	node_type* node = fast_hash_map_t::find_node(key, hash);
	if (node == null)
		return end();
	node->data.value = coffe::forward<value_type>(value);
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, hash);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::iterator_t coffe::collections::hash_map_object<K, T, A, H>::update(typename coffe::collections::auto_type<K>::arg_type const& key, typename coffe::collections::auto_type<T>::arg_type const& value)
{
	ulong64 hash;
	node_type* node = fast_hash_map_t::find_node(key, hash);
	if (node == null)
		return end();
	node->data.value = value;
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, hash);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::iterator_t coffe::collections::hash_map_object<K, T, A, H>::update(typename coffe::collections::hash_map_object<K, T, A, H>::pair_type&& pair)
{
	ulong64 hash;
	node_type* node = fast_hash_map_t::find_node(pair.key, hash);
	if (node == null)
		return end();
	node->data.value = coffe::forward<value_type>(pair.value);
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, hash);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::iterator_t coffe::collections::hash_map_object<K, T, A, H>::update(typename coffe::collections::hash_map_object<K, T, A, H>::pair_type const& pair)
{
	ulong64 hash;
	node_type* node = fast_hash_map_t::find_node(pair.key, hash);
	if (node == null)
		return end();
	node->data.value = pair.value;
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, hash);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline bool coffe::collections::hash_map_object<K, T, A, H>::remove(typename coffe::collections::auto_type<K>::arg_type const& key, typename coffe::collections::auto_type<T>::arg_ptr_type out)
{
	return (out == null ? fast_hash_map_t::remove(key) : fast_hash_map_t::remove(key, *out));
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline bool coffe::collections::hash_map_object<K, T, A, H>::remove(coffe::collections::hash_map_object<K, T, A, H>::base_iterator_t it, typename auto_type<T>::arg_ptr_type out)
{
	if (it.parent() != this || it.current() == null)
		return false;
	typename fast_hash_map_t::base_iterator it_(m_table.view(), reinterpret_cast<node_type*>(it.current()), it.offset());
	return (out == null ? fast_hash_map_t::remove(it_) : fast_hash_map_t::remove(it_, *out));
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline bool coffe::collections::hash_map_object<K, T, A, H>::has_key(typename coffe::collections::auto_type<K>::arg_type const& key)const
{
	return fast_hash_map_t::contains(key);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::iterator_t coffe::collections::hash_map_object<K, T, A, H>::find(typename coffe::collections::auto_type<K>::arg_type const& key)
{
	ulong64 hash;
	node_type* node = fast_hash_map_t::find_node(key, hash);
	if (node == null)
		return end();
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, hash);
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline typename coffe::collections::hash_map_object<K, T, A, H>::const_iterator_t coffe::collections::hash_map_object<K, T, A, H>::find(typename coffe::collections::auto_type<K>::arg_type const& key)const
{
	ulong64 hash;
	node_type* node = fast_hash_map_t::find_node(key, hash);
	if (node == null)
		return end();
	return base_iterator_t(const_cast<hash_map_object<K, T, A, H>*>(this), node, hash);
}

////////////////////////////////////////////////////////////////////////

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::object_wrapper()
	: m_ptr(null)
{
	set(new collections::hash_map_object<K, T, A, H>());
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::object_wrapper(type* map)
	: m_ptr(null)
{
	set(map !=null ? map : new collections::hash_map_object<K, T, A, H>());
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::object_wrapper(object_wrapper && other)
	: m_ptr(null) 
{
	collections::hash_map_object<K, T, A, H> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::object_wrapper(object_wrapper const& other)
	: m_ptr(null)
{
	set(other.get());
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::object_wrapper(const coffe::nullptr_t&)
	: m_ptr(null)
{
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::object_wrapper(typename type::fast_hash_map_t && map)
	: m_ptr(null)
{
	set(new collections::hash_map_object<K, T, A, H>(coffe::forward<typename type::fast_hash_map_t>(map)));
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::object_wrapper(typename type::fast_hash_map_t const& map)
	: m_ptr(null)
{
	set(new collections::hash_map_object<K, T, A, H>(map));
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::object_wrapper(const collections::ienum_ptr<pair_view_type>& store)
	: m_ptr(null)
{
	set(new collections::hash_map_object<K, T, A, H>(store));
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
template<typename U, typename V>inline coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::object_wrapper(coffe::array_view<collections::pair<U, V>> items)
	: m_ptr(null)
{
	set(new collections::hash_map_object<K, T, A, H>(items));
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
template<typename U, typename V> inline coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::object_wrapper(coffe::initializer_list<collections::pair<U, V>> list)
	: m_ptr(null)
{
	set(new collections::hash_map_object<K, T, A, H>(list));
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
inline coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::~object_wrapper()
{
	reset();
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
void coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
void coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
bool coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::is_empty()const
{
	return m_ptr == null;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::collections::hash_map_object<K, T, A, H>* coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::get(void)const
{
	return m_ptr;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
void coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::set(coffe::collections::hash_map_object<K, T, A, H>* ptr)
{
	type * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::collections::hash_map_object<K, T, A, H>** coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::addres_of(void)
{
	return &m_ptr;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>& coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator = (coffe::collections::hash_map_object<K, T, A, H>* ptr)
{
	set(ptr);
	return *this;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>& coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator = (const coffe::nullptr_t&)
{
	reset();
	return *this;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>& coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator = (coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>> && other)
{
	if (this != &other)
	{
		reset();
		m_ptr = other.m_ptr;
		other.m_ptr = null;
	}
	return*this;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>& coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator = (coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>& coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator += (typename coffe::collections::hash_map_object<K, T, A, H>::pair_type const& pair)
{
	if (!is_empty())
		get()->insert(pair);
	return*this;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>& coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator -= (typename coffe::collections::hash_map_object<K, T, A, H>::key_type const& key)
{
	if (!is_empty())
		get()->remove(key);
	return*this;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::object_wrapper_ptr<coffe::collections::hash_map_object<K, T, A, H>> coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator & (void)
{
	return this;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::collections::hash_map_object<K, T, A, H> * coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator -> (void)
{
	return get();
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::collections::hash_map_object<K, T, A, H> const* coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator -> (void)const
{
	return get();
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator coffe::collections::hash_map_object<K, T, A, H> * (void)
{
	return get();
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator coffe::collections::hash_map_object<K, T, A, H> const* (void)const
{
	return get();
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
typename coffe::collections::hash_map_object<K, T, A, H>::value_type& coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator[](typename coffe::collections::hash_map_object<K, T, A, H>::key_type const& key)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw_exception(error_code::invalid_memory);
#endif
	auto it = get()->find();
	if (!it.is_valid())
		throw_exception(error_code::item_not_found);
	return it->value;
}

template<typename K, typename T, template<typename> class A, template<typename> class H>
typename coffe::collections::hash_map_object<K, T, A, H>::value_type const& coffe::object_wrapper<coffe::collections::hash_map_object<K, T, A, H>>::operator[](typename coffe::collections::hash_map_object<K, T, A, H>::key_type const& key)const
{
	if (is_empty())
		set(new collections::hash_map_object<K, T, A, H>());

	auto it = get()->find();
	if (!it.is_valid())
	{
		it = get()->insert(key, default_value<value_type>::value);
	}
	return it->value;
}


#endif//__COFFE_COLLECTIONS_HASH_MAP_INL__
