
#ifndef __ANG_CORE_INLINES_H__
#error ...
#elif !defined __ANG_BTREE_INLINE_HPP__
#define __ANG_BTREE_INLINE_HPP__

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::insert(const pair_t& pair)
{
	ang_int16_t index;
	ang_bool_t result = ang_false;
	node_ptr_t node = null;

	node = root;

	if (node == null)
	{
		root = ang_template_allocator<node_t>::alloc();
		root->is_leaf(ang_true);
		root->set_parent(null);
		root->set_pair(0, pair);
		root->increase_count();
		count++;
		return ang_true;
	}

	if (root->get_count() == 2 * min_degree - 1)
	{
		node = root;
		root = ang_template_allocator<node_t>::alloc();
		root->is_leaf(ang_false);
		root->set_child(0, node);
		split_node(root, 0);
		node = root->get_pair(0) < pair ? root->get_child(1) : root->get_child(0);
	}

	while (node != null)
	{
		index = node->find_index(pair);
		if (index < node->get_count())
		{
			if (node->get_pair(index) == pair)
				break;
			//ANG_ASSERT(node->get_pair(index) < pair);
		}



		if (node->is_leaf() == ang_false)
		{
			if (node->get_child(index)->get_count() == (2 * min_degree - 1))
			{
				if (split_node(node, index) == ang_false)
					break; //unknown error
				continue;
			}

			node = node->get_child(index);
		}
		else
		{
			for (ang_int16_t i = node->get_count(); i > index; --i)
				node->set_pair(i, node->get_pair(i - 1));
			node->set_pair(index, pair);
			node->increase_count();
			result = ang_true;
			break;
		}
	}
	if(result == ang_true)
		count++;
	return result;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
inline ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::insert(key_t key, const value_t& value)
{
	return insert(pair_t(key, value));
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::find(key_t key)const
{
	ang_int16_t index;
	ang_bool_t result = ang_false;
	node_ptr_t node = null;

	node = root;
	while (node != null)
	{
		index = node->find_index(key);
		if (index < node->get_count() && node->get_pair(index) == key)
		{
			result = ang_true;
			break;
		}
		node = node->get_child(index);
	}
	return result;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::find(key_t key, value_t& value)const
{
	ang_int16_t index;
	ang_bool_t result = ang_false;
	node_ptr_t node = null;

	node = root;
	while (node != null)
	{
		index = node->find_index(key);
		if (index < node->get_count() && node->get_pair(index).get_key() == key)
		{
			result = ang_true;
			value = node->get_pair(index).get_value();
			break;
		}
		node = node->get_child(index);
	}
	return result;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::update_value(key_t key, value_t value)
{
	ang_int16_t index;
	ang_bool_t result = ang_false;
	node_ptr_t node = null;

	node = root;
	while (node != null)
	{
		index = node->find_index(key);
		if (index < node->get_count() && node->get_pair(index) == key)
		{
			result = ang_true;
			node->set_pair(index, { key, value });
			break;
		}
		node = node->get_child(index);
	}
	return result;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::remove(key_t _key, value_t& value)
{
	ang_int16_t index = 0;
	node_ptr_t temp, node = null;
	key_t key = _key;
	pair_t null_pair;
	ang_bool_t result = ang_false;

	enum {
		REMOVE_STATE_FIND, //find the node that will be removed
		REMOVE_STATE_REMOVE,// remove the node 
		REMOVE_STATE_BALANCE, // balance the tree
		REMOVE_STATE_END
	}state;

	node = root;
	state = REMOVE_STATE_FIND;
	while (node != null && state != REMOVE_STATE_END)
	{
		if (REMOVE_STATE_FIND == state)
		{
			//find the index of the node to remove
			index = node->find_index(key);

			//if the index is found, thene change state to REMOVE_STATE_REMOVE
			if (index < node->get_count() && node->get_pair(index).get_key() == key)
			{
				state = REMOVE_STATE_REMOVE;
			}
			else // if the index is not found and the current node is a leaf, actually the index do not exist in this tree
			{	//but if it is not a leaf then the child is not NULL and now will find in
				node = node->get_child(index);
			}
		}
		else if (REMOVE_STATE_REMOVE == state)
		{
			if (node->is_leaf() == ang_true)
			{
				if (result == ang_false)
				{
					value = node->get_pair(index).get_value();
					result = ang_true;
					count--;
				}

				for (ang_int16_t i = index + 1; i < node->get_count(); ++i)
					node->set_pair(i - 1, node->get_pair(i));
				node->set_pair(node->get_count() - 1, null_pair);
				node->decrease_count();
				// Reduce the count of keys
				state = REMOVE_STATE_BALANCE;

			}
			else
			{
				if (node->get_child(index)->get_count() >= min_degree)
				{

					temp = node->get_child(index)->get_most_right();
					if (result == ang_false)
					{
						value = node->get_pair(index).get_value();
						result = ang_true;
						count--;

					}
					node->set_pair(index, temp->get_pair(temp->get_count() - 1));
					key = temp->get_pair(temp->get_count() - 1).get_key();
					state = REMOVE_STATE_FIND;

					node = node->get_child(index);
				}
				else if (node->get_child(index + 1)->get_count() >= min_degree)
				{
					temp = node->get_child(index + 1)->get_most_left();
					if (result == ang_false)
					{
						value = node->get_pair(index).get_value();
						result = ang_true;
						count--;
					}
					node->set_pair(index, temp->get_pair(0));
					key = temp->get_pair(0).get_key();
					state = REMOVE_STATE_FIND;

					node = node->get_child(index + 1);
				}
				else
				{
					//key = node->get_pair(index).get_key();
					merge_node(node, index);
					state = REMOVE_STATE_FIND;
				}
			}
		}
		else if (REMOVE_STATE_BALANCE == state)
		{
			if (node == root /*|| node->get_parent() == null*/)
			{
				if (node->get_count() == 0)
				{
					if (root->is_leaf() == ang_true)
					{
						root = null;
					}
					else
					{
						root = node->get_child(0);
						root->set_parent(null);
						root->set_index(-1);//invalid index
						node->set_child(0, null);
					}
					ang_delete(node_t, node);
				}
				state = REMOVE_STATE_END;
			}
			else
			{
				index = node->get_index();
				node = node->get_parent();
				
				if (node->get_child(index)->get_count() < (min_degree - 1))
				{
					/*if (index > 0 && node->get_child(index - 1)->get_count() > min_degree)
					{
					}
					else if (index < node->get_count() && node->get_child(index + 1)->get_count() > min_degree)
					{
					}*/
					if (index < node->get_count())
					{
						if ((node->get_child(index)->get_count() + node->get_child(index + 1)->get_count()) < (2 * min_degree - 2))
						{
							merge_node(node, index);
						}
						else
						{
							balance_node(node, index);
						}
					}
					else if (index > 0)
					{
						if ((node->get_child(index)->get_count() + node->get_child(index - 1)->get_count()) < (2 * min_degree - 2))
						{
							merge_node(node, index - 1);
						}
						else
						{
							balance_node(node, index - 1);
						}
					}
					
				}

				//state = REMOVE_STATE_END;
			}
		}
	}
	return result;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::remove(key_t key)
{
	value_t value;
	return remove(key, value);
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::split_node(node_ptr_t node, ang_int16_t index)
{
	node_ptr_t left;
	node_ptr_t right;
	pair_t null_pair;

	if (node == null || index >= (2 * min_degree) || node->get_child(index) == null)
		return ang_false;

	left = node->get_child(index);
	right = ang_new(node_t);
	right->is_leaf(left->is_leaf());
	if (left->is_leaf() == ang_false)
	{
		for (ang_int16_t i = 0; i < min_degree - 1; i++)
		{
			right->set_pair(i, left->get_pair(i + min_degree));
			right->set_child(i, left->get_child(i + min_degree));
			left->set_pair(i + min_degree, null_pair);
			left->set_child(i + min_degree, null);
		}
		right->set_child(min_degree - 1, left->get_child(2 * min_degree - 1));
		left->set_child(2 * min_degree - 1, null);
	}
	else
	{
		for (ang_int16_t i = 0; i < min_degree - 1; i++)
		{
			right->set_pair(i, left->get_pair(i + min_degree));
			left->set_pair(i + min_degree, null_pair);
		}
	}
	
	right->set_count( min_degree - 1);
	left->set_count( min_degree - 1);

	for (ang_int16_t i = node->get_count(); i > index; --i)
	{
		node->set_pair(i, node->get_pair(i - 1));
		node->set_child(i + 1, node->get_child(i));
	}

	node->set_pair(index, left->get_pair(min_degree - 1));
	left->set_pair(min_degree - 1, null_pair);
	node->set_child(index + 1, right);
	node->increase_count();

	return ang_true;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::merge_node(node_ptr_t node, ang_int16_t index)
{
	ang_int16_t c;
	node_ptr_t child;
	node_ptr_t sibling;

	node_t node_copy;
	node->copy_debug(&node_copy);

	pair_t null_pair;
	if (node == null)
		return ang_false;
	child = node->get_child(index);
	sibling = node->get_child(index + 1);

	if (*((ang_size_t*)sibling) == 0xdddddddd)
	{
		int a = 0;
		//ang_throw_exception(0, "");
	}


	child->set_pair(child->get_count(), node->get_pair(index));
	child->increase_count();
	c = child->get_count();

	if (child->is_leaf() == ang_false)
	{
		child->set_child(c, sibling->get_child(0));
		sibling->set_child(0, null);
		for (ang_int16_t i = 0; i < sibling->get_count(); ++i)
		{
			child->set_pair(c + i, sibling->get_pair(i));
			child->set_child(c + i + 1, sibling->get_child(i + 1));
			sibling->set_pair(i, null_pair);
			sibling->set_child(i + 1, null);
		}
	}
	else
	{
		for (ang_int16_t i = 0; i < sibling->get_count(); ++i)
		{
			child->set_pair(c + i, sibling->get_pair(i));
			sibling->set_pair(i, null_pair);
		}
	}
		
	for (ang_int16_t i = index + 1; i < node->get_count(); ++i)
	{
		node->set_pair(i - 1, node->get_pair(i));
		node->set_child(i, node->get_child(i + 1));
	}
	node->set_pair(node->get_count() - 1, null_pair);
	node->set_child(node->get_count(), null);

	child->set_count(child->get_count() + sibling->get_count());
	sibling->set_count(0);
	node->decrease_count();

	ang_template_allocator<node_t>::free(sibling);

	node_copy.clear_debug();
	return ang_true;
}



template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::balance_node(node_ptr_t node, ang_int16_t index)
{
	ang_int16_t c1, c2;
	node_ptr_t child;
	node_ptr_t sibling;
	pair_t null_pair;
	if (node == null)
		return ang_false;
	child = node->get_child(index);
	sibling = node->get_child(index + 1);
	
	c1 = child->get_count() + sibling->get_count();
	c2 = c1 / 2;
	c1 = c1 - c2;
	if (child->get_count() - c1 > 0)
	{
		lend_to_next(node, index, child->get_count() - c1);
	}
	else
	{
		borrow_from_next(node, index, c1 - child->get_count());
	}
	return ang_true;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::lend_to_next(node_ptr_t node, ang_int16_t index, ang_int16_t count)
{
	node_ptr_t child = node->get_child(index);
	node_ptr_t sibling = node->get_child(index + 1);

	for (ang_int16_t i = sibling->get_count(); i > 0; --i)
	{
		sibling->set_pair(i + count - 1, sibling->get_pair(i - 1));
		sibling->set_child(i + count, sibling->get_child(i));
	}
	sibling->set_child(count, sibling->get_child(0));
	sibling->set_pair(count - 1, node->get_pair(index));

	for (ang_int16_t i = count - 2; i >= 0; --i)
	{
		sibling->set_pair(i, child->get_pair(child->get_count() + i - count + 1));
		sibling->set_child(i + 1, child->get_child(child->get_count() + i - count + 2));

		child->set_pair(child->get_count() + i - count + 1, pair_t());
		child->set_child(child->get_count() + i - count + 2, null);
	}

	node->set_pair(index, child->get_pair(child->get_count() - count));
	sibling->set_child(0, child->get_child(child->get_count() - count + 1));

	child->set_pair(child->get_count() - count, pair_t());
	child->set_child(child->get_count() - count + 1, null);

	child->set_count(child->get_count() - count);
	sibling->set_count(sibling->get_count() + count);

	return ang_true;
}

template<typename _key_t, typename _value_t, ang_int16_t min_degree, typename _pair_t>
ang_bool_t ang_btree_t<_key_t, _value_t, min_degree, _pair_t>::borrow_from_next(node_ptr_t node, ang_int16_t index, ang_int16_t count)
{
	node_ptr_t child = node->get_child(index);
	node_ptr_t sibling = node->get_child(index + 1);

	child->set_pair(child->get_count(), node->get_pair(index));
	child->increase_count();

	for (ang_int16_t i = 0, c = count - 1; i < c; ++i)
	{
		child->set_pair(child->get_count(), sibling->get_pair(i));
		child->set_child(child->get_count(), sibling->get_child(i));

		sibling->set_pair(i, pair_t());
		sibling->set_child(i, null);
		child->increase_count();
	}

	node->set_pair(index, sibling->get_pair(count - 1));
	child->set_child(child->get_count(), sibling->get_child(count - 1));

	sibling->set_pair(count - 1, pair_t());
	sibling->set_child(count - 1, null);

	for (ang_int16_t i = count, c = sibling->get_count(); i < c; ++i)
	{
		sibling->set_pair(i - count, sibling->get_pair(i));
		sibling->set_child(i - count, sibling->get_child(i));
		sibling->set_pair(i, pair_t());
		sibling->set_child(i, null);
	}

	sibling->set_child(sibling->get_count() - count, sibling->get_child(sibling->get_count()));
	sibling->set_count(sibling->get_count() - count);

	return ang_true;
}

#endif//__ANG_BTREE_INLINE_HPP__