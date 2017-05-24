#ifndef __ANG_CORE_INLINES_H__
#error ...
#elif !defined __ANG_BTREE_NODE_HPP__
#define __ANG_BTREE_NODE_HPP__

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::ang_btree_node_t(ang_bool_t isleaf) 
	: parent(null), count(0), index(0), isleaf(isleaf) {
	memset(children, 0, sizeof(children));
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::~ang_btree_node_t()
{
	//recursive_delete();
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>* ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::get_child(ang_int16_t index)const {
	if (index < 0 || index >= (2 * min_degree))
		return null;
	return children[index];
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline void ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::set_child(ang_int16_t index, ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>* child) {
	if (index < 0 || index >= (2 * min_degree))
		return;
	children[index] = child;
	if (child != null)
	{
		children[index]->set_parent(this);
		children[index]->set_index(index);
	}
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline const _pair_t& ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::get_pair(ang_int16_t index)const {
	static _pair_t dummy;
	if (index < 0 || index >= (2 * min_degree - 1))
		return dummy;
	return pairs[index];
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline void ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::set_pair(ang_int16_t index, const pair_t& pair) {
	if (index < 0 || index >= (2 * min_degree - 1))
		return;
	pairs[index] = pair;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>* ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::get_parent()const {
	return parent;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline void ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::set_parent(ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>* parent) {
	this->parent = parent;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_int16_t ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::get_index()const {
	return index;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline void ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::set_index(ang_int16_t index) {
	this->index = index;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_int16_t ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::get_count()const {
	return count;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline void ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::set_count(ang_int16_t count) {
	this->count = count;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline void ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::increase_count() {
	count++;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline void ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::decrease_count() {
	count--;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_bool_t ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::is_leaf()const {
	return isleaf;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline void ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::is_leaf(ang_bool_t value) {
	isleaf = value;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_int16_t ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::find_index(key_t key)const {
	
	ang_int16_t min_idx;
	ang_int16_t max_idx;
	ang_int16_t cur_idx;
	_pair_t cur_key;
	//ang_int16_t flag = 1;

	min_idx = 0;
	max_idx = get_count();

	do {
		//if ((max_idx - min_idx) <= 1)
		//	flag--;
		cur_idx = (max_idx + min_idx) / 2;
		cur_key = get_pair(cur_idx);
		if (cur_key.get_key() > key)
			max_idx = cur_idx - 1;
		else if (cur_key.get_key() < key)
			min_idx = cur_idx + 1;
		else return cur_idx;
	} while (min_idx <= max_idx);

	if (min_idx > max_idx)
	{
		max_idx = min(min_idx, get_count());
	}

	return max_idx;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>* ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::get_most_right()const {
	self_t* node = const_cast<self_t*>(this);
	while (node != null && node->is_leaf() == ang_false)
		node = node->get_child(node->get_count());
	return node;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>* ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::get_most_left()const {
	self_t* node = const_cast<self_t*>(this);
	while (node != null && node->is_leaf() == ang_false)
		node = node->get_child(0);
	return node;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_int16_t ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::find_index(const pair_t& key)const {
	
	ang_int16_t min_idx;
	ang_int16_t max_idx;
	ang_int16_t cur_idx;
	_pair_t cur_key;
	ang_int16_t flag = 1;

	min_idx = 0;
	max_idx = get_count();

	do {
		if ((max_idx - min_idx) <= 1)
			flag--;
		cur_idx = (max_idx + min_idx) / 2;
		cur_key = get_pair(cur_idx);
		if (cur_key > key)
			max_idx = cur_idx;
		else if (cur_key < key)
			min_idx = cur_idx;
		else return cur_idx;
	} while (flag > 0);
	return max_idx;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_bool_t ang_btree_node_t<_key_t, _value_t, min_degree, _pair_t>::recursive_delete() {
	if (isleaf == ang_true)
		return false;
	for (ang_int16_t i = 0; i <= count; ++i) {
		if(children[i]) 
			children[i]->recursive_delete();
		children[i] = null;
	}
	count = 0;
	return ang_true;
}

#endif//__ANG_BTREE_NODE_HPP__