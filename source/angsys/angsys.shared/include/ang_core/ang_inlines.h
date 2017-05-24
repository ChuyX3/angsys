#ifndef __ANG_CORE_INLINES_H__
#define __ANG_CORE_INLINES_H__

#include "angtypes.h"

#ifdef __cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>

#define MIN_DEGREE 10

#define ang_new(_type) ang_template_allocator<_type>::alloc()
#define ang_new_args(_type, ...) ang_template_allocator<_type>::alloc({__VA_ARGS__})
#define ang_delete(_type, _ptr) ang_template_allocator<_type>::free(_ptr)

template<typename T>
struct ang_template_allocator
{
private:
	T _value;

	template<class... Ts>
	inline ang_template_allocator(Ts&&... args) : _value(args...) {
	}
	inline~ang_template_allocator() {}

	ang_void_ptr_t operator new(ang_size_t size){
		return ::malloc(size);
	}
	void operator delete(ang_void_ptr_t ptr) {
		::free(ptr);
	}

	ang_void_ptr_t operator new[](ang_size_t size) {
		return ::malloc(size);
	}
	void operator delete[](ang_void_ptr_t ptr) {
		::free(ptr);
	}

public:
	template<class... Ts>
	inline static T* alloc(Ts&&... args) {
		return reinterpret_cast<T*>(new ang_template_allocator<T>(args...));
	}
	inline static void free(T* ptr) {
		delete reinterpret_cast<ang_template_allocator<T>*>(ptr);
	}

	inline static T* alloc_array(ang_size_t size) {
		return reinterpret_cast<T*>(new ang_template_allocator<T>[size]);
	}
	inline static void free_array(T* ptr) {
		delete[] ptr;
	}
};

template<> ang_char_t* ang_template_allocator<ang_char_t>::alloc_array(ang_size_t size) {
	return (ang_char_t*)::malloc(size * sizeof(ang_char_t));
}

template<> void ang_template_allocator<ang_char_t>::free_array(ang_char_t* ptr) {
	::free(ptr);
}

template<> ang_wchar_t* ang_template_allocator<ang_wchar_t>::alloc_array(ang_size_t size) {
	return (ang_wchar_t*)::malloc(size * sizeof(ang_wchar_t));
}

template<> void ang_template_allocator<ang_wchar_t>::free_array(ang_wchar_t* ptr) {
	::free(ptr);
}


template<typename _char_t>
class ang_base_string
{
public:
	typedef ang_base_string<_char_t> self_t;
	typedef _char_t char_t;
	typedef _char_t* str_t;
	typedef _char_t const* cstr_t;

protected:

	union
	{
		ang_uint16_t _flag_is_local_data;
		struct
		{
			ang_uint16_t _unused;
			ang_size_t _buffer_size_used;
			ang_size_t _buffer_capacity;
			str_t _buffer_ptr;
		};
		struct //32 bytes
		{
			ang_uint16_t _local_size;
			char_t _local_buffer[30];
		};
	}_data;

public:
	ang_base_string() {
		memset(&_data, 0, sizeof(_data));
	}
	ang_base_string(ang_size_t reserv) : ang_base_string() {
		realloc(reserv, false);
	}
	ang_base_string(cstr_t cstr) : ang_base_string() {
		copy(cstr);
	}
	ang_base_string(const self_t& cstr) : ang_base_string() {
		copy(cstr);
	}
	ang_base_string(self_t && str) : ang_base_string() {
		memcpy(&_data, &str._data, sizeof(_data));
		memset(&str._data, sizeof(_data));
	}
	~ang_base_string() {
		clean();
	}

private:
	static ang_size_t length(cstr_t cstr, ang_size_t max = -1)
	{
		if (cstr == null || max == 0U)
			return 0U;
		ang_size_t idx = 0;
		while (idx < max && cstr[idx] != 0) idx++;
		return idx;
	}
	static ang_size_t copy(str_t str, cstr_t cstr, ang_size_t max = -1)
	{
		if (str == null || cstr == null || max == 0U)
			return 0U;
		ang_size_t idx = 0;
		while (idx < max && cstr[idx] != 0)
			str[idx] = cstr[idx++];
		str[idx] = 0;
		return idx;
	}
	static ang_int32_t compare(cstr_t first, cstr_t second)
	{
		if (first == second)
			return 0;
		if ((first == null) && (second == null))
			return 0;
		else if (first == null)
			return 1;
		else if (second == null)
			return -1;
		ang_size_t c = 0;
		while (true) {
			if ((first[c] == 0) && (second[c] == 0))
				return 0;
			else if (first[c] > second[c])
				return 1;
			else if (first[c] < second[c])
				return -1;
			else
				c++;
		}
	}

	ang_bool_t is_local_data()const {
		return  ang_bool_t(_data._flag_is_local_data != 0);
	}
	ang_bool_t realloc(ang_size_t new_size, ang_bool_t save = true) {
		if (get_capacity() >= new_size)
			return false;
		ang_size_t size = 32;
		while (size <= new_size)
			size *= 2;
		str_t new_buffer = ang_template_allocator<char_t>::alloc_array(size);
		ang_size_t len = 0;
		new_buffer[0] = 0;
		if (save)
			len = copy(new_buffer, get_cstr(), get_length());

		clean();
		_data._buffer_size_used = len;
		_data._buffer_capacity = size - 1;
		_data._buffer_ptr = new_buffer;
		return true;
	}
	void set_length(ang_size_t len) {
		if (is_local_data())
			_data._local_size = (ang_uint16_t)len;
		else
			_data._buffer_size_used = len;
	}

public:
	ang_bool_t is_empty()const {
		return (_data._local_size == 0 && _data._buffer_size_used == 0) ? true : false;
	}
	str_t get_str() {
		return is_local_data() ? _data._local_buffer : _data._buffer_ptr;
	}
	cstr_t get_cstr() const {
		return is_local_data() ? _data._local_buffer : _data._buffer_ptr;
	}
	ang_size_t get_length() const {
		return is_local_data() ? _data._local_size : _data._buffer_size_used;
	}
	ang_size_t get_capacity() const {
		return is_local_data() ? 29U : 0;
	}

	void clean() {
		if (!is_local_data())
			ang_template_allocator<char_t>::free_array(_data._buffer_ptr);
		memset(&_data, 0, sizeof(_data));
	}
	ang_size_t copy(cstr_t cstr)
	{
		ang_size_t len = length(cstr);

		if (len == 0)
			return 0U;
		if (len <= 29)
		{
			clean();
			_data._local_size = (ang_uint16_t)copy(_data._local_buffer, cstr, 29);

		}
		else
		{
			realloc(len, false);
			_data._buffer_size_used = copy(_data._buffer_ptr, cstr, _data._buffer_capacity);
		}
		return get_length();
	}
	ang_size_t concat(cstr_t cstr)
	{
		ang_size_t my_len = get_length();
		ang_size_t len = length(cstr);

		if (len == 0)
			return 0U;

		if ((my_len + len) < 29)
		{
			_data._local_size = my_len + copy(&_data._local_buffer[my_len], cstr, 29 - my_len);
		}
		else
		{
			realloc(len);
			_data._buffer_size_used = my_len + copy(&_data._buffer_ptr[my_len], cstr, _data._buffer_size_capacity - my_len);
		}
		return get_length() - my_len;
	}
	ang_size_t copy(self_t const& cstr)
	{
		ang_size_t len = cstr.get_length();

		if (len == 0)
			return 0U;
		if (len <= 29)
		{
			clean();
			_data._local_size = (ang_uint16_t)copy(_data._local_buffer, cstr.get_cstr(), 29);

		}
		else
		{
			realloc(len, false);
			_data._buffer_size_used = copy(_data._buffer_ptr, cstr.get_cstr(), _data._buffer_capacity);
		}
		return get_length();
	}
	ang_size_t concat(self_t const& cstr)
	{
		ang_size_t my_len = get_length();
		ang_size_t len = cstr.get_length();

		if (len == 0)
			return 0U;

		if ((my_len + len) < 29)
		{
			_data._local_size = my_len + copy(&_data._local_buffer[my_len], cstr.get_cstr(), 29 - my_len);
		}
		else
		{
			realloc(len);
			_data._buffer_size_used = my_len + copy(&_data._buffer_ptr[my_len], cstr.get_cstr(), _data._buffer_size_capacity - my_len);
		}
		return get_length() - my_len;
	}
	ang_int32_t compare(cstr_t cstr)const {
		return compare(get_cstr(), cstr);
	}
	ang_size_t format(cstr_t cstr, ...) { return 0; }

	str_t begin() { return  get_str(); }
	str_t end() { return  &get_str()[get_length()]; }
	cstr_t begin()const { return  get_cstr(); }
	cstr_t end()const { return  &get_cstr()[get_length()]; }

	template<typename _other_char_t>
	ang_size_t copy(_other_char_t const* _beg, _other_char_t const* _end)
	{
		ang_size_t len = ((ang_size_t)_end - (ang_size_t)_beg) / sizeof(_other_char_t);
		realloc(len, false);
		ang_size_t max = get_capacity();
		str_t my_beg = begin();
		ang_size_t idx = 0;
		while (idx < max && _beg != _end)
		{
			my_beg[idx++] = *_beg;
			_beg++;
		}
		my_beg[idx] = 0;
		return idx;
	}

	self_t& operator = (self_t&& other) {
		clean();
		memcpy(&_data, &other._data, sizeof(_data));
		memset(&other._data, 0, sizeof(_data));
		return *this;
	}
	self_t& operator = (self_t const& other) {
		copy(other);
		return *this;
	}
	self_t& operator += (self_t const& other) {

		concat(other);
		return *this;
	}
	self_t& operator = (cstr_t other) {

		copy(other);
		return *this;
	}
	self_t& operator += (cstr_t other) {

		concat(other);
		return *this;
	}
	operator str_t () { return get_str(); }
	operator cstr_t ()const { return get_cstr(); }
	char_t& operator [](ang_index_t idx) { return get_str()[idx]; }
	char_t operator [](ang_index_t idx)const { return get_cstr()[idx]; }

	friend ang_bool_t operator == (self_t const& first, self_t const& second) {
		return (self_t::compare(first.get_cstr(), second.get_cstr()) == 0) ? true : false;
	}
	friend ang_bool_t operator != (self_t const& first, self_t const& second) {
		return (self_t::compare(first.get_cstr(), second.get_cstr()) != 0) ? true : false;
	}
	friend ang_bool_t operator >= (self_t const& first, self_t const& second) {
		return (self_t::compare(first.get_cstr(), second.get_cstr()) >= 0) ? true : false;
	}
	friend ang_bool_t operator <= (self_t const& first, self_t const& second) {
		return (self_t::compare(first.get_cstr(), second.get_cstr()) <= 0) ? true : false;
	}
	friend ang_bool_t operator > (self_t const& first, self_t const& second) {
		return (self_t::compare(first.get_cstr(), second.get_cstr()) > 0) ? true : false;
	}
	friend ang_bool_t operator < (self_t const& first, self_t const& second) {
		return (self_t::compare(first.get_cstr(), second.get_cstr()) < 0) ? true : false;
	}

	friend ang_bool_t operator == (self_t const& first, cstr_t second) {
		return (self_t::compare(first.get_cstr(), second) == 0) ? true : false;
	}
	friend ang_bool_t operator != (self_t const& first, cstr_t second) {
		return (self_t::compare(first.get_cstr(), second) != 0) ? true : false;
	}
	friend ang_bool_t operator >= (self_t const& first, cstr_t second) {
		return (self_t::compare(first.get_cstr(), second) >= 0) ? true : false;
	}
	friend ang_bool_t operator <= (self_t const& first, cstr_t second) {
		return (self_t::compare(first.get_cstr(), second) <= 0) ? true : false;
	}
	friend ang_bool_t operator > (self_t const& first, cstr_t second) {
		return (self_t::compare(first.get_cstr(), second) > 0) ? true : false;
	}
	friend ang_bool_t operator < (self_t const& first, cstr_t second) {
		return (self_t::compare(first.get_cstr(), second) < 0) ? true : false;
	}

	friend ang_bool_t operator == (cstr_t first, self_t const& second) {
		return (self_t::compare(first, second.get_cstr()) == 0) ? true : false;
	}
	friend ang_bool_t operator != (cstr_t first, self_t const& second) {
		return (self_t::compare(first, second.get_cstr()) != 0) ? true : false;
	}
	friend ang_bool_t operator >= (cstr_t first, self_t const& second) {
		return (self_t::compare(first, second.get_cstr()) >= 0) ? true : false;
	}
	friend ang_bool_t operator <= (cstr_t first, self_t const& second) {
		return (self_t::compare(first, second.get_cstr()) <= 0) ? true : false;
	}
	friend ang_bool_t operator > (cstr_t first, self_t const& second) {
		return (self_t::compare(first, second.get_cstr()) > 0) ? true : false;
	}
	friend ang_bool_t operator < (cstr_t first, self_t const& second) {
		return (self_t::compare(first, second.get_cstr()) < 0) ? true : false;
	}
};

template<> ang_size_t ang_base_string<ang_char_t>::format(ang_char_t const* fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
#ifdef WINDOWS_PLATFORM
	ang_size_t size = _vscprintf(fmt, arg_list);
#else
	ang_size_t size = vsnprintf(0, NULL, fmt, arg_list);
#endif
	if (size <= 0)
		return 0U;
	realloc(size, false);
	set_length((ang_size_t)vsprintf(get_str(), fmt, arg_list));
	va_end(arg_list);
	return get_length();
}

template<> ang_size_t ang_base_string<ang_wchar_t>::format(ang_wchar_t const* fmt, ...)
{
	va_list arg_list;
	va_start(arg_list, fmt);
#ifdef WINDOWS_PLATFORM
	ang_size_t size = _vscwprintf(fmt, arg_list);
#else
	ang_size_t size = vswprintf(0, NULL, fmt, arg_list);
#endif
	if (size <= 0)
		return 0U;
	realloc(size, false);
	set_length((ang_size_t)vswprintf(get_str(), get_capacity(), fmt, arg_list));
	va_end(arg_list);
	return get_length();
}

typedef ang_base_string<ang_char_t> ang_string_t;
typedef ang_base_string<ang_wchar_t> ang_wstring_t;

template<typename _key_t, typename _value_t>
class ang_pair_t
{
public:
	typedef _key_t key_t;
	typedef _value_t value_t;
	typedef _value_t& value_ref_t;
	typedef const _value_t& const_value_ref_t;
	typedef ang_pair_t<_key_t, _value_t> self_t;

private:
	key_t _key;
	value_t _value;

public:
	ang_pair_t();
	ang_pair_t(const self_t&);
	ang_pair_t(key_t, const_value_ref_t value);
	~ang_pair_t();

	inline key_t get_key()const { return _key; }
	inline value_t get_value()const { return _value; }
	inline void set_key(key_t key) { _key = key; }
	inline void set_value(const_value_ref_t value) { _value = value; }

	inline ang_pair_t& operator = (const self_t&other) {
		_key = other._key;
		_value = other._value;
		return *this;
	}

	ang_pair_t(self_t&&) = default;
	ang_pair_t& operator = (self_t&&) = default;
};


template<class K, class T>
struct ang_hash_node
{
	ang_pair_t<K,T> pair;
	ang_hash_node<K, T>* next;

	ang_hash_node(K k, T const& val)
		: pair(ang::move(k), val)
		, next(nullptr) {
	}
	ang_hash_node(ang_pair_t<K, T> p)
		: pair(ang::move(p))
		, next(nullptr) {
	}
	ang_hash_node(ang_hash_node<K,T> const& other)
		: pair(other.key, other.value)
		, next(nullptr) {
	}
};

template<class K>
using ang_hash_function = ang_uint32_t(*)(K, ang_uint32_t const);

template<class K, class T, ang_uint32_t TS = 119>
class ang_hash_table
{
public:
	static const ang_uint32_t HASH_TABLE_SIZE = TS;

	typedef ang_hash_node<K, T> node_t;
	typedef ang_hash_node<K, T>* node_ptr_t;
private:
	ang_uint64_t _size;
	node_ptr_t _table[HASH_TABLE_SIZE];
	ang_hash_function<K> _hash_function;
public:
	ang_hash_table();
	~ang_hash_table();

	bool set_hash_function(ang_hash_function<K>);

	void clean();
	bool insert(ang_pair_t<K, T>);
	bool insert(K, T);
	bool remove(K const& key, T&);
	bool is_empty()const;
	bool contains(K const& key)const;
	bool find(K const& key, T&)const;
	ang_uint64_t size()const;

	template<class F>
	void print(F f) {
		for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
			ang_hash_node<K, T>* temp = _table[i];
			while (temp) {
				f(temp->pair.get_value());
				temp = temp->next;
			}
		}
	}

	template<class F>
	void clean(F f) {
		for (int i = 0; i < HASH_TABLE_SIZE && _size > 0; ++i) {
			ang_hash_node<K, T>* temp = _table[i];
			_table[i] = nullptr;
			while (temp) {
				ang_hash_node<K, T>* del = temp;
				f(temp->pair.get_value());
				temp = temp->next;
				ang_template_allocator<ang_hash_node<K, T>>::free(del);
				_size--;
			}
		}
	}

private:
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


template<typename _key_t, typename _value_t> bool operator == (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second);
template<typename _key_t, typename _value_t> bool operator != (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second);
template<typename _key_t, typename _value_t> bool operator >= (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second);
template<typename _key_t, typename _value_t> bool operator <= (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second);
template<typename _key_t, typename _value_t> bool operator > (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second);
template<typename _key_t, typename _value_t> bool operator < (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second);

template<typename _key_t, typename _value_t> bool operator == (ang_pair_t<_key_t, _value_t> const& first, _key_t second);
template<typename _key_t, typename _value_t> bool operator != (ang_pair_t<_key_t, _value_t> const& first, _key_t second);
template<typename _key_t, typename _value_t> bool operator >= (ang_pair_t<_key_t, _value_t> const& first, _key_t second);
template<typename _key_t, typename _value_t> bool operator <= (ang_pair_t<_key_t, _value_t> const& first, _key_t second);
template<typename _key_t, typename _value_t> bool operator > (ang_pair_t<_key_t, _value_t> const& first, _key_t second);
template<typename _key_t, typename _value_t> bool operator < (ang_pair_t<_key_t, _value_t> const& first, _key_t second);

template<typename _key_t, typename _value_t> bool operator == (_key_t first, ang_pair_t<_key_t, _value_t> const& second);
template<typename _key_t, typename _value_t> bool operator != (_key_t first, ang_pair_t<_key_t, _value_t> const& second);
template<typename _key_t, typename _value_t> bool operator >= (_key_t first, ang_pair_t<_key_t, _value_t> const& second);
template<typename _key_t, typename _value_t> bool operator <= (_key_t first, ang_pair_t<_key_t, _value_t> const& second);
template<typename _key_t, typename _value_t> bool operator > (_key_t first, ang_pair_t<_key_t, _value_t> const& second);
template<typename _key_t, typename _value_t> bool operator < (_key_t first, ang_pair_t<_key_t, _value_t> const& second);


#include "ang_core/ang_pair.hpp"
#include "ang_core/ang_hash_table.hpp"

#endif//__cplusplus
#endif//__ANG_CORE_INLINES_H__
