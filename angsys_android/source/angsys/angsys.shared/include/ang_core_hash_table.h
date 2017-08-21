#ifndef __ANG_CORE_HASH_TABLE_H__
#define __ANG_CORE_HASH_TABLE_H__

#include <angtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ang_hash_table ang_hash_table_t, *ang_hash_table_ptr_t;
typedef struct ang_hash_table_pair ang_hash_table_pair_t, *ang_hash_table_pair_ptr_t;

typedef void(*ang_hash_table_delete_data_callback_t)(ang_hash_table_pair_t data);
typedef ulong64(*ang_hash_table_create_hash_callback_t)(ulong64 key, ulong64 size);



struct ang_hash_table_pair
{
	ulong64 key;
	ulong64 value;
};

ang_hash_table_ptr_t ang_hash_table_create(ang_hash_table_delete_data_callback_t, ang_hash_table_create_hash_callback_t);
void ang_hash_table_destroy(ang_hash_table_ptr_t);

struct ang_hash_table
{
#ifdef __cplusplus
private:
	void(*_clean)(ang_hash_table_ptr_t);
	ang_bool_t(*_insert)(ang_hash_table_ptr_t, ulong64 key, ulong64 value);
	ang_bool_t(*_insert_pair)(ang_hash_table_ptr_t, ang_hash_table_pair_t);
	ang_bool_t(*_remove)(ang_hash_table_ptr_t, ulong64 key, ulong64* out);
	ang_bool_t(*_is_empty)(ang_hash_table_ptr_t);
	ang_bool_t(*_contains)(ang_hash_table_ptr_t, ulong64 key);
	ang_bool_t(*_find)(ang_hash_table_ptr_t, ulong64 key, ulong64* out);
	wsize(*_size)(ang_hash_table_ptr_t);
public:
	inline void clean() { _clean(this); }
	inline bool insert(ulong64 key, ulong64 value) { return _insert(this, key, value) ? true : false; }
	inline bool insert(ang_hash_table_pair_t pair) { return _insert_pair(this, pair) ? true : false; }
	inline bool remove(ulong64 key, ulong64* out = NULL) { return _remove(this, key, out) ? true : false; }
	inline bool is_empty()const { return _is_empty(const_cast<ang_hash_table_ptr_t>(this)) ? true : false; }
	inline bool contains(ulong64 key)const { return _contains(const_cast<ang_hash_table_ptr_t>(this), key) ? true : false; }
	inline bool find(ulong64 key, ulong64* out = NULL)const { return _find(const_cast<ang_hash_table_ptr_t>(this), key, out) ? true : false; }
	inline wsize size()const { return _size(const_cast<ang_hash_table_ptr_t>(this)); }

#else
	void(*clean)(ang_hash_table_ptr_t);
	ang_bool_t(*insert)(ang_hash_table_ptr_t, ulong64 key, ulong64 value);
	ang_bool_t(*insert_pair)(ang_hash_table_ptr_t, ang_hash_table_pair_t);
	ang_bool_t(*remove)(ang_hash_table_ptr_t, ulong64 key, ulong64* out);
	ang_bool_t(*is_empty)(ang_hash_table_ptr_t);
	ang_bool_t(*contains)(ang_hash_table_ptr_t, ulong64 key);
	ang_bool_t(*find)(ang_hash_table_ptr_t, ulong64 key, ulong64* out);
	wsize(*size)(ang_hash_table_ptr_t);
#endif

};



#ifdef __cplusplus
}
#endif

#endif//__ANG_CORE_HASH_TABLE_H__