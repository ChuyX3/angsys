#ifndef __ANG_INLINES_H__
#define __ANG_INLINES_H__

#include "angbase.h"

#define ang_new(_type) ang_allocator<_type>:: template contruct<void>(ang_allocator<_type>::alloc(1))
#define ang_new_args(_type, ...) ang_allocator<_type>:: template construct<void>(ang_allocator<_type>::alloc(1), __VA_ARGS__)
#define ang_delete(_ptr) ang_allocator<typename ang::remove_reference<decltype(*_ptr)>::type>::destruct_and_free(_ptr)

//ANG_EXTERN ang_void_ptr_t ang_alloc_unmanaged_memory(ang_size_t);
//ANG_EXTERN void ang_free_unmanaged_memory(ang_void_ptr_t);

#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
inline ang_void_ptr_t operator new(ang_size_t, ang_void_ptr_t ptr) throw()
{
	return ptr;
}

inline void operator delete(ang_void_ptr_t, ang_void_ptr_t) throw()
{
	return;
}
#endif

template<typename T>
struct ang_allocator
{
	static T* alloc(ang_size_t count) { 
		return reinterpret_cast<T*>(ang_alloc_unmanaged_memory(count * sizeof(T)));
	}
	static void free(T* ptr) {
		ang_free_unmanaged_memory(ptr);
	}

	static void destruct_and_free(T* ptr) {
		ptr->~T();
		ang_free_unmanaged_memory(ptr);
	}

	template<typename... args_t>
	static T* construct(T* obj, args_t... args) {
		return new(obj) T(args...);
	}

	static void destruct(T* obj) {
		obj->~T();
	}
};

template<class key_t, class value_t>
struct ang_pair
{
	key_t key;
	value_t value;
};


template<class K, class T, ang_size_t N>
ang_size_t ang_binary_search(K const& key, const T(&vector)[N])
{
	long64 first = 0, last = N - 1;
	long64 mid;
	while (first <= last)
	{
		mid = (first + last) / 2;
		if (vector[mid].key == key) return (ang_size_t)mid;
		else if (vector[mid].key > key) last = mid - 1;
		else first = mid + 1;
	}
	return (ang_size_t)-1;
}


template<class K, class T>
struct ang_hash_node
{
	ang_pair<K, T> pair;
	ang_hash_node<K, T>* next;

	ang_hash_node(K k, T const& val)
		: pair(ang::move(k), val)
		, next(nullptr) {
	}
	ang_hash_node(ang_pair<K, T> p)
		: pair(ang::move(p))
		, next(nullptr) {
	}
	ang_hash_node(ang_hash_node<K, T> const& other)
		: pair(other.key, other.value)
		, next(nullptr) {
	}
};

template<class K, class T>
class ang_hash_table
{
public:
	typedef ang_hash_node<K, T> node_t;
	typedef ang_hash_node<K, T>* node_ptr_t;

private:
	ang_uint32_t _size;
	ang_uint32_t _capacity;
	node_ptr_t* _table;

public:
	ang_hash_table();
	~ang_hash_table();

	void clean();
	bool insert(ang_pair<K, T>);
	bool insert(K, T);
	bool remove(K const& key, T&);
	bool is_empty()const;
	bool contains(K const& key)const;
	bool find(K const& key, T&)const;
	ang_uint64_t size()const;

	template<class F>
	void print(F f) {
		for (ang_uint32_t i = 0; i < _capacity; ++i) {
			ang_hash_node<K, T>* temp = _table[i];
			while (temp) {
				f(temp->pair.value);
				temp = temp->next;
			}
		}
	}

	template<class F>
	void clean(F f) {
		for (ang_uint32_t i = 0; i < _capacity && _size > 0; ++i) {
			ang_hash_node<K, T>* temp = _table[i];
			_table[i] = nullptr;
			while (temp) {
				ang_hash_node<K, T>* del = temp;
				f(temp->pair.value);
				temp = temp->next;
				ang_delete(del);
				_size--;
			}
		}
		_capacity = 0;
		_size = 0;
		if (_table)ang_delete(_table);
		_table = NULL;
	}

private:
	void increase_capacity();
	ang_uint32_t hash_index(K const& key)const;
	ang_hash_node<K, T>* find(K const& key)const;
};

ang_uint32_t ang_create_hash_index(ang_int32_t, ang_uint32_t);
ang_uint32_t ang_create_hash_index(ang_uint32_t, ang_uint32_t);
ang_uint32_t ang_create_hash_index(ang_int64_t, ang_uint32_t);
ang_uint32_t ang_create_hash_index(ang_uint64_t, ang_uint32_t);
ang_uint32_t ang_create_hash_index(ang_void_ptr_t, ang_uint32_t);
ang_uint32_t ang_create_hash_index(ang_cstr_t, ang_uint32_t);
ang_uint32_t ang_create_hash_index(ang_cwstr_t, ang_uint32_t);

ang_uint32_t ang_hash_table_get_next_size(ang_uint32_t);

#include "ang_hash_table.inl"

#endif//__ANG_INLINES_H__