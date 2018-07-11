/*********************************************************************************************************************/
/*   File Name: angtypes.h                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types for multiplatform use                                  */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGC_CORE_HASH_TABLE_H__
#define __ANGC_CORE_HASH_TABLE_H__

#include <angtypes.h>

#ifdef LINK
#undef LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef struct ang_hash_table ang_hash_table_t, *ang_hash_table_ptr_t;
typedef struct ang_hash_table_pair ang_hash_table_pair_t, *ang_hash_table_pair_ptr_t;

#ifndef ang_hash_table_delete_data_callback_DEFINE
#define ang_hash_table_delete_data_callback_DEFINE
typedef void(*ang_hash_table_delete_data_callback_t)(ang_hash_table_pair_t data);
typedef void(*ang_hash_table_iterate_data_callback_t)(pointer context, ang_hash_table_pair_t data);
typedef ulong64(*ang_hash_table_create_hash_callback_t)(ulong64 key, ulong64 size);
#endif

struct ang_hash_table_pair
{
	ulong64 key;
	ulong64 value;
};

ang_hash_table_ptr_t ang_hash_table_create(ang_hash_table_delete_data_callback_t, ang_hash_table_create_hash_callback_t);
void ang_hash_table_destroy(ang_hash_table_ptr_t);


wsize ang_hash_table_get_next_size(wsize);
ang_uint64_t ang_create_hash_index_int32(ang_int32_t key, ang_uint64_t TS);
ang_uint64_t ang_create_hash_index_uint32(ang_uint32_t key, ang_uint64_t TS);
ang_uint64_t ang_create_hash_index_int64(ang_int64_t key, ang_uint64_t TS);
ang_uint64_t ang_create_hash_index_uint64(ang_uint64_t key, ang_uint64_t TS);
ang_uint64_t ang_create_hash_index_voidptr(ang_void_ptr_t key, ang_uint64_t TS);
ang_uint64_t ang_create_hash_index_cstr(ang_cstr_t key, ang_uint64_t TS);
ang_uint64_t ang_create_hash_index_cwstr(ang_cwstr_t key, ang_uint64_t TS);

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
	void(*_iterate)(ang_hash_table_ptr_t, pointer, ang_hash_table_iterate_data_callback_t);
public:
	inline void clean() { _clean(this); }
	inline bool insert(ulong64 key, ulong64 value) { return _insert(this, key, value) ? true : false; }
	inline bool insert(ang_hash_table_pair_t pair) { return _insert_pair(this, pair) ? true : false; }
	inline bool remove(ulong64 key, ulong64* out = NULL) { return _remove(this, key, out) ? true : false; }
	inline bool is_empty()const { return _is_empty(const_cast<ang_hash_table_ptr_t>(this)) ? true : false; }
	inline bool contains(ulong64 key)const { return _contains(const_cast<ang_hash_table_ptr_t>(this), key) ? true : false; }
	inline bool find(ulong64 key, ulong64* out = NULL)const { return _find(const_cast<ang_hash_table_ptr_t>(this), key, out) ? true : false; }
	inline wsize size()const { return _size(const_cast<ang_hash_table_ptr_t>(this)); }
	inline void iterate(pointer context, ang_hash_table_iterate_data_callback_t callback)const{ _iterate(const_cast<ang_hash_table_ptr_t>(this), context, callback); }
#else
	void(*clean)(ang_hash_table_ptr_t);
	ang_bool_t(*insert)(ang_hash_table_ptr_t, ulong64 key, ulong64 value);
	ang_bool_t(*insert_pair)(ang_hash_table_ptr_t, ang_hash_table_pair_t);
	ang_bool_t(*remove)(ang_hash_table_ptr_t, ulong64 key, ulong64* out);
	ang_bool_t(*is_empty)(ang_hash_table_ptr_t);
	ang_bool_t(*contains)(ang_hash_table_ptr_t, ulong64 key);
	ang_bool_t(*find)(ang_hash_table_ptr_t, ulong64 key, ulong64* out);
	wsize(*size)(ang_hash_table_ptr_t);
	void (*iterate)(ang_hash_table_ptr_t, pointer, ang_hash_table_iterate_data_callback_t);
#endif

};

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

template<typename K>
struct hash_table_context
{
	static ulong64 hash_table_context_create_hash_number(K key, ulong64 size) { return key % size; }
	static void hash_table_context_delete_data(ang_hash_table_pair_t pair) { pair.value = 0; pair.key = 0; }
};

template<typename K, typename T, typename context = hash_table_context<K>>
class ang_unorder_map
{
	static void delete_data(ang_hash_table_pair_t data) {
		context::hash_table_context_delete_data(data);
	}
	static ulong64 create_hash_number(ulong64 key, ulong64 size) {
		return context::hash_table_context_create_hash_number((K)(key), size);
	}

	ang_hash_table_ptr_t _table;
public:
	ang_unorder_map() {
		_table = ang_hash_table_create(&ang_unorder_map<K, T, context>::delete_data, &ang_unorder_map<K, T, context>::create_hash_number);
	}
	~ang_unorder_map() {
		ang_hash_table_destroy(_table);
		_table = NULL;
	}

	inline void clean() { _table->clean(); }
	inline bool insert(K key, T value) { return _table->insert((ulong64)key, (ulong64)value); }
	inline bool insert(ang_hash_table_pair_t pair) { return _table->insert(pair); }
	inline bool remove(K key, T* out = NULL) {
		ulong64 _out;
		if (!_table->remove((ulong64)key, &_out))
			return false;
		if (out != NULL) *out = (T)_out;
		return true;
	}
	inline bool is_empty()const { return _table->is_empty(); }
	inline bool contains(K key)const { return _table->contains((ulong64)key); }
	inline bool find(K key, T* out = NULL)const { return _table->find((ulong64)key, (ulong64*)out); }
	inline wsize size()const { return _table->size(); }
	template<typename F> inline void print(F callback)const { 
		_table->iterate((pointer)&callback, [](pointer args, ang_hash_table_pair_t pair) {
			(*reinterpret_cast<F*>(args))((K)pair.key, (T)pair.value);
		});
	}
};
#endif

#endif//__ANGC_CORE_HASH_TABLE_H__