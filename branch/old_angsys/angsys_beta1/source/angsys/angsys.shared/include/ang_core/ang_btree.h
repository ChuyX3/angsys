#ifndef __ANG_CORE_BTREE_H__
#define __ANG_CORE_BTREE_H__
#include "ang_core_types.h"

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#define MIN_DEGREE 40

typedef struct _ang_btree_pair
{
	ang_size_t key;
	ang_void_ptr_t value;
}ang_btree_pair_t, *ang_btree_pair_ptr_t;

typedef void (*ang_btree_del_func_t)(ang_btree_pair_ptr_t);


typedef struct _ang_btree_node
{
	struct _ang_btree_node* children[2 * MIN_DEGREE];
	ang_btree_pair_t pairs[2 * MIN_DEGREE - 1];
	struct _ang_btree_node* parent;
	ang_int16_t count;
	ang_int16_t index;
	ang_bool_t is_leaf;
}ang_btree_node, ang_btree_node_t, *ang_btree_node_ptr_t;

//ang_int16_t ang_btree_node_find_index(ang_btree_node_ptr_t node, ang_size_t key);
//ang_btree_node_t* ang_btree_node_get_most_right(ang_btree_node_ptr_t node);
//ang_btree_node_t* ang_btree_node_get_most_left(ang_btree_node_ptr_t node);
//ang_bool_t ang_btree_node_clean(ang_btree_node_ptr_t node, ang_btree_del_func_t del_func);

typedef struct _ang_btree
{
	ang_int32_t count;
	ang_btree_node_ptr_t root;
	ang_btree_del_func_t del_func;
}ang_btree, ang_btree_t, *ang_btree_ptr_t;

ang_btree_ptr_t ang_btree_create(ang_void_ptr_t);
ang_void_ptr_t ang_btree_destroy(ang_btree_ptr_t);

ang_bool_t ang_btree_insert(ang_btree_ptr_t tree, ang_size_t key, ang_void_ptr_t value);
ang_bool_t ang_btree_find(ang_btree_ptr_t tree, ang_size_t key, ang_void_ptr_t* value);
ang_bool_t ang_btree_update_value(ang_btree_ptr_t tree, ang_size_t key, ang_void_ptr_t value, ang_void_ptr_t* old_value);
ang_bool_t ang_btree_remove(ang_btree_ptr_t tree, ang_size_t key, ang_void_ptr_t* value);

void ang_btree_iterate(ang_btree_ptr_t tree, void(*iteration_watcher)(ang_btree_pair_ptr_t));

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//__ANG_CORE_BTREE_H__