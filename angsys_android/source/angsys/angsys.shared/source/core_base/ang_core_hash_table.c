#include <angtypes.h>
#include "ang_core_base.h"
#include "ang_core_hash_table.h"

#define new(_TYPE) (_TYPE*)ang_alloc_unmanaged_memory(sizeof(_TYPE))
#define delete(_PTR) ang_free_unmanaged_memory(_PTR)


#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
#elif defined WINDOWS_PLATFORM
#endif

typedef struct ang_hash_table_node ang_hash_table_node_t, *ang_hash_table_node_ptr_t;

extern wsize ang_hash_table_get_next_size(wsize);

static void ang_hash_table_clean(ang_hash_table_ptr_t);
static ang_bool_t ang_hash_table_insert(ang_hash_table_ptr_t, ulong64 key, ulong64 value);
static ang_bool_t ang_hash_table_insert_pair(ang_hash_table_ptr_t, ang_hash_table_pair_t);
static ang_bool_t ang_hash_table_remove(ang_hash_table_ptr_t, ulong64 key, ulong64* out);
static ang_bool_t ang_hash_table_is_empty(ang_hash_table_ptr_t);
static ang_bool_t ang_hash_table_contains(ang_hash_table_ptr_t, ulong64 key);
static ang_bool_t ang_hash_table_find(ang_hash_table_ptr_t, ulong64 key, ulong64* out);
static ang_hash_table_node_ptr_t ang_hash_table_find_node(ang_hash_table_ptr_t, ulong64 key);
static wsize ang_hash_table_size(ang_hash_table_ptr_t);
static void ang_hash_table_increase_capacity(ang_hash_table_ptr_t _this);

struct ang_hash_table_node
{
	ang_hash_table_pair_t pair;
	ang_hash_table_node_ptr_t next;
};

typedef struct ang_hash_table_impl
{
	void(*clean)(ang_hash_table_ptr_t);
	ang_bool_t(*insert)(ang_hash_table_ptr_t, ulong64 key, ulong64 value);
	ang_bool_t(*insert_pair)(ang_hash_table_ptr_t, ang_hash_table_pair_t);
	ang_bool_t(*remove)(ang_hash_table_ptr_t, ulong64 key, ulong64* out);
	ang_bool_t(*is_empty)(ang_hash_table_ptr_t);
	ang_bool_t(*contains)(ang_hash_table_ptr_t, ulong64 key);
	ang_bool_t(*find)(ang_hash_table_ptr_t, ulong64 key, ulong64* out);
	wsize(*size)(ang_hash_table_ptr_t);

	wsize _size;
	wsize _capacity;
	ang_hash_table_create_hash_callback_t hash_callback;
	ang_hash_table_delete_data_callback_t delete_callback;
	ang_hash_table_node_ptr_t* _table;
}ang_hash_table_impl_t, *ang_hash_table_impl_ptr_t;

ang_hash_table_ptr_t ang_hash_table_create(ang_hash_table_delete_data_callback_t delete_callback, ang_hash_table_create_hash_callback_t hash_callback)
{
	if (delete_callback == NULL || hash_callback == NULL)
		return NULL;
	ang_hash_table_impl_ptr_t this = new(ang_hash_table_impl_t);
	memset(this, 0, sizeof(ang_hash_table_impl_t));

	this->clean = &ang_hash_table_clean;
	this->insert = &ang_hash_table_insert;
	this->insert_pair = &ang_hash_table_insert_pair;
	this->remove = &ang_hash_table_remove;
	this->is_empty = &ang_hash_table_is_empty;
	this->contains = &ang_hash_table_contains;
	this->find = &ang_hash_table_find;
	this->size = &ang_hash_table_size;

	this->_size = 0;
	this->_capacity = 127;
	this->_table = (ang_hash_table_node_ptr_t*)ang_alloc_unmanaged_memory(this->_capacity * sizeof(ang_hash_table_node_ptr_t));
	memset(this->_table, 0, sizeof(ang_hash_table_node_ptr_t*) * this->_capacity);

	return (ang_hash_table_ptr_t)this;
}

void ang_hash_table_destroy(ang_hash_table_ptr_t _this)
{
	if (_this == NULL) return;
	_this->clean(_this);
	delete(_this);
}

static void ang_hash_table_clean(ang_hash_table_ptr_t _this)
{
	wsize i;
	ang_hash_table_node_ptr_t temp, todelete;
	ang_hash_table_impl_ptr_t this = (ang_hash_table_impl_ptr_t)_this;

	for (i = 0; i < this->_capacity && this->_size > 0; ++i)
	{
		temp = this->_table[i];
		this->_table[i] = NULL;
		while (temp) {
			todelete = temp;
			this->delete_callback(todelete->pair);
			temp = temp->next;
			delete(todelete);
			this->_size--;
		}
	}

	this->_capacity = 0;
	this->_size = 0;
	if (this->_table)delete(this->_table);
	this->_table = NULL;
}

static ang_bool_t ang_hash_table_insert(ang_hash_table_ptr_t _this, ulong64 key, ulong64 value)
{
	ulong64 hash;
	ang_hash_table_node_ptr_t entry;
	ang_hash_table_impl_ptr_t this = (ang_hash_table_impl_ptr_t)_this;
	if (this->_size > (this->_capacity * 0.75))
		ang_hash_table_increase_capacity(_this);

	if (ang_hash_table_find_node(_this, key) != NULL)
		return ang_false;
	hash = this->hash_callback(key, this->_capacity);
	entry = new(ang_hash_table_node_t);
	entry->pair.key = key;
	entry->pair.value = value;
	entry->next = this->_table[hash];
	this->_table[hash] = entry;
	this->_size++;
	return ang_true;
}

static ang_bool_t ang_hash_table_insert_pair(ang_hash_table_ptr_t _this, ang_hash_table_pair_t pair)
{
	ulong64 hash;
	ang_hash_table_node_ptr_t entry;
	ang_hash_table_impl_ptr_t this = (ang_hash_table_impl_ptr_t)_this;
	if (this->_size > (this->_capacity * 0.75))
		ang_hash_table_increase_capacity(_this);

	if (ang_hash_table_find_node(_this, pair.key) != NULL)
		return ang_false;
	hash = this->hash_callback(pair.key, this->_capacity);
	entry = new(ang_hash_table_node_t);
	entry->pair = pair;
	entry->next = this->_table[hash];
	this->_table[hash] = entry;
	this->_size++;
	return ang_true;
}

static ang_bool_t ang_hash_table_remove(ang_hash_table_ptr_t _this, ulong64 key, ulong64* out)
{
	ulong64 hash;
	ang_hash_table_node_ptr_t temp, *prev;
	ang_hash_table_impl_ptr_t this = (ang_hash_table_impl_ptr_t)_this;
	if (this->_capacity == 0)
		return ang_false;

	hash = this->hash_callback(key, this->_capacity);
	temp = this->_table[hash];
	prev = &this->_table[hash];

	while (temp != NULL)
	{
		if (temp->pair.key == key)
		{
			*prev = temp->next;
			if (out)*out = temp->pair.value;
			else this->delete_callback(temp->pair);
			delete(temp);
			this->_size--;
			return ang_true;
		}
		prev = &temp->next;
		temp = temp->next;
	}
	return ang_false;
}

static ang_bool_t ang_hash_table_is_empty(ang_hash_table_ptr_t _this)
{
	ang_hash_table_impl_ptr_t this = (ang_hash_table_impl_ptr_t)_this;
	return this->_size == 0;
}

static ang_bool_t ang_hash_table_contains(ang_hash_table_ptr_t _this, ulong64 key)
{
	return ang_hash_table_find_node(_this, key) != NULL;
}

static ang_hash_table_node_ptr_t ang_hash_table_find_node(ang_hash_table_ptr_t _this, ulong64 key)
{
	ulong64 hash;
	ang_hash_table_node_ptr_t temp;
	ang_hash_table_impl_ptr_t this = (ang_hash_table_impl_ptr_t)_this;
	if (this->_capacity == 0)
		return NULL;
	hash = this->hash_callback(key, this->_capacity);
	temp = this->_table[hash];
	while (temp != NULL)
	{
		if (key == temp->pair.key)
			return temp;
		temp = temp->next;
	}
	return NULL;
}

static ang_bool_t ang_hash_table_find(ang_hash_table_ptr_t _this, ulong64 key, ulong64* out)
{
	ulong64 hash;
	ang_hash_table_node_ptr_t temp;
	ang_hash_table_impl_ptr_t this = (ang_hash_table_impl_ptr_t)_this;
	hash = this->hash_callback(key, this->_capacity);
	temp = this->_table[hash];
	while (temp != NULL)
	{
		if (key == temp->pair.key)
		{
			if(out) *out = temp->pair.value;
			return ang_true;
		}
		temp = temp->next;
	}
	return ang_false;
}

static wsize ang_hash_table_size(ang_hash_table_ptr_t _this)
{
	ang_hash_table_impl_ptr_t this = (ang_hash_table_impl_ptr_t)_this;
	return this->_size;
}

static void ang_hash_table_increase_capacity(ang_hash_table_ptr_t _this)
{
	wsize new_size, s, i, hash;
	ang_hash_table_node_ptr_t *new_data, temp, entry;
	ang_hash_table_impl_ptr_t this = (ang_hash_table_impl_ptr_t)_this;

	new_size = ang_hash_table_get_next_size(this->_capacity + 20);
	new_data = (ang_hash_table_node_ptr_t*)ang_alloc_unmanaged_memory(new_size * sizeof(ang_hash_table_node_ptr_t));
	memset(new_data, 0, sizeof(ang_hash_table_node_ptr_t*) * new_size);

	if (this->_table && this->_size)
	{
		s = this->_size;
		for (i = 0; i < this->_capacity && s > 0; ++i) {
			temp = this->_table[i];
			this->_table[i] = NULL;
			while (temp) {
				hash = this->hash_callback(temp->pair.key, new_size);
				entry = temp;
				temp = temp->next;
				entry->next = new_data[hash];
				new_data[hash] = entry;
				s--;
			}
		}
		delete(this->_table);
	}
	this->_capacity = new_size;
	this->_table = new_data;
}