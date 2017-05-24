#include "compile_config.h"
#include <ang_core_types.h>
#include "ang_core\ang_btree.h"

#define ANG_NEW(_TYPE)  _TYPE##_create(ang_alloc_unmanaged_memory(sizeof(_TYPE)))
#define ANG_DELETE(_PTR)  ang_free_unmanaged_memory(_PTR)
#define ANG_SAFE_DELETE(_TYPE, _PTR)  if(_PTR)ang_free_unmanaged_memory(_TYPE##_destroy(_PTR))


static ang_bool_t node_clean(ang_btree_node_ptr_t node, ang_btree_del_func_t del_func);

static ang_btree_node_ptr_t ang_btree_node_create(ang_void_ptr_t ptr)
{
	ang_btree_node_ptr_t node = (ang_btree_node_ptr_t)ptr;
	memset(node, 0, sizeof(ang_btree_node_t));
	return node;
}

static ang_void_ptr_t ang_btree_node_destroy(ang_btree_node_ptr_t node)
{
	node_clean(node, NULL);
	return node;
}

static ang_int16_t find_index(ang_btree_node_ptr_t node, ang_size_t key)
{
	ang_int16_t min_idx;
	ang_int16_t max_idx;
	ang_int16_t cur_idx;
	ang_size_t cur_key;
	//ang_int16_t flag = 1;

	min_idx = 0;
	max_idx = node->count;

	do {
		//if ((max_idx - min_idx) <= 1)
		//	flag--;
		cur_idx = (max_idx + min_idx) / 2;
		cur_key = node->pairs[cur_idx].key;
		if (cur_key > key)
			max_idx = cur_idx - 1;
		else if (cur_key < key)
			min_idx = cur_idx + 1;
		else return cur_idx;
	} while (min_idx <= max_idx);

	if (min_idx > max_idx)
	{
		max_idx = min(min_idx, node->count);
	}

	return max_idx;
}

static ang_btree_node_t* get_most_right(ang_btree_node_ptr_t node)
{
	while (node != NULL && node->is_leaf == ang_false)
		node = node->children[node->count];
	return node;
}

static ang_btree_node_t* get_most_left(ang_btree_node_ptr_t node)
{
	while (node != NULL && node->is_leaf == ang_false)
		node = node->children[0];
	return node;
}

static ang_bool_t node_clean(ang_btree_node_ptr_t node, ang_btree_del_func_t del_func)
{
	if (node->is_leaf == ang_false) {
		for (ang_int16_t i = 0; i <= node->count; ++i) {
			if (node->children[i]) {
				node_clean(node->children[i], del_func);
				ANG_DELETE(node->children[i]);
			}
			node->children[i] = NULL;
		}
	}
	if(del_func) for (ang_int16_t i = 0; i < node->count; ++i) {
		del_func(&node->pairs[i]);
	}
	node->count = 0;
	return ang_true;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


static ang_bool_t split_node(ang_btree_node_ptr_t node, ang_int16_t index);
static ang_bool_t merge_node(ang_btree_node_ptr_t node, ang_int16_t index);
static ang_bool_t balance_node(ang_btree_node_ptr_t node, ang_int16_t index);
static ang_bool_t lend_to_next(ang_btree_node_ptr_t node, ang_int16_t index, ang_int16_t count);
static ang_bool_t borrow_from_next(ang_btree_node_ptr_t node, ang_int16_t index, ang_int16_t count);

ang_btree_ptr_t ang_btree_create(ang_void_ptr_t ptr)
{
	ang_btree_ptr_t tree = (ang_btree_ptr_t)ptr;
	memset(tree, 0, sizeof(ang_btree_t));
	return tree;
}

ang_void_ptr_t ang_btree_destroy(ang_btree_ptr_t tree)
{
	node_clean(tree->root, tree->del_func);
	ANG_DELETE(tree->root);
	return tree;
}

static void node_iterate(ang_btree_node_ptr_t node, void(*iteration_watcher)(ang_btree_pair_ptr_t))
{
	ang_int16_t i;
	if (node->is_leaf == ang_false)
	{
		for (i = 0; i < node->count; ++i)
		{
			node_iterate(node->children[i], iteration_watcher);
			iteration_watcher(node->pairs + i);
		}
		node_iterate(node->children[node->count], iteration_watcher);
	}
	else for (i = 0; i < node->count; ++i)
		iteration_watcher(node->pairs + i);	
}

void ang_btree_iterate(ang_btree_ptr_t tree, void(*iteration_watcher)(ang_btree_pair_ptr_t))
{
}

ang_bool_t ang_btree_insert(ang_btree_ptr_t tree, ang_size_t key, ang_void_ptr_t value)
{
	ang_int16_t index;
	ang_bool_t result = ang_false;
	ang_btree_node_ptr_t node = NULL;

	node = tree->root;

	if (node == NULL)
	{
		tree->root = ANG_NEW(ang_btree_node);
		tree->root->is_leaf = ang_true;
		tree->root->pairs[0].key = key;
		tree->root->pairs[0].value = value;
		tree->root->count++;
		tree->root->index = -1;
		tree->count++;
		return ang_true;
	}

	if (tree->root->count == (2 * MIN_DEGREE - 1))
	{
		node = tree->root;
		tree->root = ANG_NEW(ang_btree_node);
		tree->root->children[0] = node;
		node->parent = tree->root;
		split_node(tree->root, 0);
		tree->root->index = -1;
		node = tree->root->pairs[0].key < key ? tree->root->children[1] : tree->root->children[0];
	}

	while (node != NULL)
	{
		index = find_index(node, key);
		if (index < node->count)
		{
			if (node->pairs[index].key == key)
				break;
		}

		if (node->is_leaf == ang_false)
		{
			if (node->children[index]->count == (2 * MIN_DEGREE - 1))
			{
				if (split_node(node, index) == ang_false)
					break; //unknown error
				continue;
			}

			node = node->children[index];
		}
		else
		{
			for (ang_int16_t i = node->count; i > index; --i)
				node->pairs[i] = node->pairs[i - 1];
			node->pairs[index].key = key;
			node->pairs[index].value = value;
			node->count++;
			result = ang_true;
			break;
		}
	}
	if (result == ang_true)
		tree->count++;
	return result;
}

ang_bool_t ang_btree_find(ang_btree_ptr_t tree, ang_size_t key, ang_void_ptr_t* value)
{
	ang_int16_t index;
	ang_bool_t result = ang_false;
	ang_btree_node_ptr_t node = tree->root;
	while (node != NULL)
	{
		index = find_index(node, key);
		if (index < node->count && node->pairs[index].key == key)
		{
			result = ang_true;
			if(value)
				*value = node->pairs[index].value;
			break;
		}
		node = node->children[index];
	}
	return result;
}

ang_bool_t ang_btree_update_value(ang_btree_ptr_t tree, ang_size_t key, ang_void_ptr_t value, ang_void_ptr_t* old_value)
{
	ang_int16_t index;
	ang_bool_t result = ang_false;
	ang_btree_node_ptr_t node = tree->root;
	while (node != NULL)
	{
		index = find_index(node, key);
		if (index < node->count && node->pairs[index].key == key)
		{
			result = ang_true;
			if (old_value)
				*old_value = node->pairs[index].value;
			else if (tree->del_func)
				tree->del_func(node->pairs[index].value);
			node->pairs[index].value = value;
			break;
		}
		node = node->children[index];
	}
	return result;
}

ang_bool_t ang_btree_remove(ang_btree_ptr_t tree, ang_size_t _key, ang_void_ptr_t* value)
{
	ang_int16_t index = 0;
	ang_btree_node_ptr_t temp, node = NULL;
	ang_size_t key = _key;
	ang_btree_pair_t null_pair = { 0,NULL };
	ang_bool_t result = ang_false;
	ang_int16_t i;

#define REMOVE_STATE_FIND		(0)  //find the node that will be removed
#define	REMOVE_STATE_REMOVE		(1) // remove the node 
#define	REMOVE_STATE_BALANCE	(2)  // balance the tree
#define	REMOVE_STATE_END		(3)

	int state;

	node = tree->root;
	state = REMOVE_STATE_FIND;
	while (node != NULL && state != REMOVE_STATE_END)
	{
		if (REMOVE_STATE_FIND == state)
		{
			//find the index of the node to remove
			index = find_index(node, key);

			//if the index is found, thene change state to REMOVE_STATE_REMOVE
			if (index < node->count && node->pairs[index].key == key)
			{
				state = REMOVE_STATE_REMOVE;
			}
			else // if the index is not found and the current node is a leaf, actually the index do not exist in this tree
			{	//but if it is not a leaf then the child is not NULL and now will find in
				node = node->children[index];
			}
		}
		else if (REMOVE_STATE_REMOVE == state)
		{
			if (node->is_leaf == ang_true)
			{
				if (result == ang_false)
				{
					if (value)*value = node->pairs[index].value;
					else if (tree->del_func)tree->del_func(node->pairs[index].value);
					result = ang_true;
					tree->count--;
				}

				for (i = index + 1; i < node->count; ++i)
					node->pairs[i - 1] = node->pairs[i];
				node->pairs[node->count - 1] = null_pair;
				node->count--;
				// Reduce the count of keys
				state = REMOVE_STATE_BALANCE;
			}
			else
			{
				if (node->children[index]->count >= MIN_DEGREE)
				{

					temp = get_most_right(node->children[index]);
					if (result == ang_false)
					{
						if (value)*value = node->pairs[index].value;
						else if (tree->del_func)tree->del_func(node->pairs[index].value);
						result = ang_true;
						tree->count--;
					}
					node->pairs[index] = temp->pairs[temp->count - 1];
					key = temp->pairs[temp->count - 1].key;
					state = REMOVE_STATE_FIND;
					node = node->children[index];
				}
				else if (node->children[index + 1]->count >= MIN_DEGREE)
				{
					temp = get_most_left(node->children[index + 1]);
					if (result == ang_false)
					{
						if (value)*value = node->pairs[index].value;
						else if (tree->del_func)tree->del_func(node->pairs[index].value);
						result = ang_true;
						tree->count--;
					}
					node->pairs[index] = temp->pairs[0];
					key = temp->pairs[0].key;
					state = REMOVE_STATE_FIND;
					node = node->children[index + 1];
				}
				else
				{
					//key = node->pairs[index].key;
					merge_node(node, index);
					state = REMOVE_STATE_FIND;
				}
			}
		}
		else if (REMOVE_STATE_BALANCE == state)
		{
			if (node == tree->root)
			{
				if (node->count == 0)
				{
					if (tree->root->is_leaf == ang_true)
					{
						tree->root = NULL;
					}
					else
					{
						tree->root = node->children[0];
						tree->root->parent = NULL;
						tree->root->index = -1;//invalid index
						node->children[0] = NULL;
					}
					ANG_DELETE(node);
				}
				state = REMOVE_STATE_END;
			}
			else
			{
				index = node->index;
				node = node->parent;

				if (node->children[index]->count < (MIN_DEGREE - 1))
				{
					if (index < node->count)
					{
						if ((node->children[index]->count + node->children[index + 1]->count) < (2 * MIN_DEGREE - 2))
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
						if ((node->children[index]->count + node->children[index - 1]->count) < (2 * MIN_DEGREE - 2))
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



ang_bool_t split_node(ang_btree_node_ptr_t node, ang_int16_t index)
{
	ang_int16_t i;
	ang_btree_node_ptr_t left;
	ang_btree_node_ptr_t right;
	ang_btree_pair_t null_pair = { 0,NULL };

	if (node == NULL || index >= (2 * MIN_DEGREE) || node->children[index] == NULL)
		return ang_false;

	left = node->children[index];
	right = ANG_NEW(ang_btree_node);
	right->is_leaf = left->is_leaf;
	if (left->is_leaf == ang_false)
	{
		for (i = 0; i < MIN_DEGREE - 1; i++)
		{
			right->pairs[i] = left->pairs[i + MIN_DEGREE];
			right->children[i] = left->children[i + MIN_DEGREE];
			if (right->children[i]) {
				right->children[i]->index = i;
				right->children[i]->parent = right;
			}
			left->pairs[i + MIN_DEGREE] = null_pair;
			left->children[i + MIN_DEGREE] = NULL;
		}
		right->children[MIN_DEGREE - 1] = left->children[2 * MIN_DEGREE - 1];
		if (right->children[MIN_DEGREE - 1]) {
			right->children[MIN_DEGREE - 1]->index = MIN_DEGREE - 1;
			right->children[MIN_DEGREE - 1]->parent = right;
		}
		left->children[2 * MIN_DEGREE - 1] = NULL;
	}
	else
	{
		for (i = 0; i < MIN_DEGREE - 1; i++)
		{
			right->pairs[i] = left->pairs[i + MIN_DEGREE];
			left->pairs[i + MIN_DEGREE] = null_pair;
		}
	}

	right->count = MIN_DEGREE - 1;
	left->count = MIN_DEGREE - 1;

	for (i = node->count; i > index; --i)
	{
		node->pairs[i] = node->pairs[i - 1];
		node->children[i + 1] = node->children[i];
		if (node->children[i + 1])
			node->children[i + 1]->index = i + 1;
	}

	node->pairs[index] = left->pairs[MIN_DEGREE - 1];
	left->pairs[MIN_DEGREE - 1] = null_pair;
	node->children[index + 1] = right;
	right->index = index + 1;
	right->parent = node;
	node->count++;
	return ang_true;
}

ang_bool_t merge_node(ang_btree_node_ptr_t node, ang_int16_t index)
{
	ang_int16_t c, i;
	ang_btree_node_ptr_t child;
	ang_btree_node_ptr_t sibling;

	ang_btree_pair_t null_pair = { 0,NULL };

	if (node == NULL)
		return ang_false;
	child = node->children[index];
	sibling = node->children[index + 1];

	child->pairs[child->count] = node->pairs[index];
	child->count++;
	c = child->count;

	if (child->is_leaf == ang_false)
	{
		child->children[c] = sibling->children[0];
		if (child->children[c])
		{
			child->children[c]->index = c;
			child->children[c]->parent = child;
		}
		sibling->children[0] = NULL;
		for (i = 0; i < sibling->count; ++i)
		{
			child->pairs[c + i] = sibling->pairs[i];
			child->children[c + i + 1] = sibling->children[i + 1];
			if (child->children[c +i + 1])
			{
				child->children[c + i + 1]->index = c + i + 1;
				child->children[c + i + 1]->parent = child;
			}

			sibling->pairs[i] = null_pair;
			sibling->children[i + 1] = NULL;
		}
	}
	else
	{
		for (i = 0; i < sibling->count; ++i)
		{
			child->pairs[c + i] = sibling->pairs[i];
			sibling->pairs[i] = null_pair;
		}
	}

	for (i = index + 1; i < node->count; ++i)
	{
		node->pairs[i - 1] = node->pairs[i];
		node->children[i] = node->children[i + 1];
		if (node->children[i])
			node->children[i]->index = i;
	}
	node->pairs[node->count - 1] = null_pair;
	node->children[node->count] = NULL;

	child->count = child->count + sibling->count;
	sibling->count = 0;
	node->count--;
	ANG_DELETE(sibling);
	return ang_true;
}

ang_bool_t balance_node(ang_btree_node_ptr_t node, ang_int16_t index)
{
	ang_int16_t c1, c2;
	ang_btree_node_ptr_t child;
	ang_btree_node_ptr_t sibling;


	if (node == NULL)
		return ang_false;
	child = node->children[index];
	sibling = node->children[index + 1];

	c1 = child->count + sibling->count;
	c2 = c1 / 2;
	c1 = c1 - c2;
	if (child->count - c1 > 0)
	{
		lend_to_next(node, index, child->count - c1);
	}
	else
	{
		borrow_from_next(node, index, c1 - child->count);
	}
	return ang_true;
}

ang_bool_t lend_to_next(ang_btree_node_ptr_t node, ang_int16_t index, ang_int16_t count)
{
	ang_btree_node_ptr_t child = node->children[index];
	ang_btree_node_ptr_t sibling = node->children[index + 1];
	ang_btree_pair_t null_pair = { 0,NULL };

	for (ang_int16_t i = sibling->count; i > 0; --i)
	{
		sibling->pairs[i + count - 1] = sibling->pairs[i - 1];
		sibling->children[i + count] = sibling->children[i];
		sibling->children[i + count]->index = i + count;	
	}
	sibling->children[count] = sibling->children[0];
	if(sibling->children[count])sibling->children[count]->count = count;;
	sibling->pairs[count - 1] = node->pairs[index];

	for (ang_int16_t i = count - 2; i >= 0; --i)
	{
		sibling->pairs[i] = child->pairs[child->count + i - count + 1];
		sibling->children[i + 1] = child->children[child->count + i - count + 2];
		sibling->children[i + 1]->index = i + 1;
		sibling->children[i + 1]->parent = sibling;
		
		child->pairs[child->count + i - count + 1] = null_pair;
		child->children[child->count + i - count + 2] = NULL;
	}

	node->pairs[index] = child->pairs[child->count - count];
	sibling->children[0] = child->children[child->count - count + 1];
	sibling->children[0]->index = 0;
	sibling->children[0]->parent = sibling;

	child->pairs[child->count - count] = null_pair;
	child->children[child->count - count + 1] = NULL;

	child->count = child->count - count;
	sibling->count = sibling->count + count;
	return ang_true;
}

ang_bool_t borrow_from_next(ang_btree_node_ptr_t node, ang_int16_t index, ang_int16_t count)
{
	ang_btree_node_ptr_t child = node->children[index];
	ang_btree_node_ptr_t sibling = node->children[index + 1];
	ang_btree_pair_t null_pair = { 0,NULL };

	child->pairs[child->count] = node->pairs[index];
	child->count++;

	for (ang_int16_t i = 0, c = count - 1; i < c; ++i)
	{
		child->pairs[child->count] = sibling->pairs[i];
		child->children[child->count] = sibling->children[i];
		child->children[child->count]->index = child->count;
		child->children[child->count]->parent = child;
		sibling->pairs[i] = null_pair;
		sibling->children[i] = NULL;
		child->count++;
	}

	node->pairs[index] = sibling->pairs[count - 1];
	child->children[child->count] = sibling->children[count - 1];
	child->children[child->count]->index = child->count;
	child->children[child->count]->parent = child;
	child->count++;

	sibling->pairs[count - 1] = null_pair;
	sibling->children[count - 1] = NULL;

	for (ang_int16_t i = count, c = sibling->count; i < c; ++i)
	{
		sibling->pairs[i - count] = sibling->pairs[i];
		sibling->children[i - count] = sibling->children[i];
		sibling->children[i - count]->index = i - count;
		sibling->pairs[i] = null_pair;
		sibling->children[i] = NULL;
	}

	sibling->children[sibling->count - count] = sibling->children[sibling->count];
	sibling->children[sibling->count - count]->index = sibling->count - count;
	sibling->count = sibling->count - count;

	return ang_true;
}


