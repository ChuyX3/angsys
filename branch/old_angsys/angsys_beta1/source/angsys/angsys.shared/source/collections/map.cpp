#include "pch.h"
#include <angsys.h>
#include <ang/collections/map.h>
#include <ang/maths.h>

using namespace ang;
using namespace ang::collections;

template<class K>
inline uint_t _create_hash_index(K const& k, uint_t const TS)
{
	union {
		K k;
		ulong_t val;
	}key;
	key.k = k;
	return (uint_t)((2475825 + key.val + 1) % TS);
}

ang::uint_t ang::collections::create_hash_index(int_t key, uint_t TS) { 
	return _create_hash_index(key, TS);
}

ang::uint_t ang::collections::create_hash_index(uint_t key, uint_t TS) {
	return _create_hash_index(key, TS);
}

ang::uint_t ang::collections::create_hash_index(long_t key, uint_t TS) {
	return _create_hash_index(key, TS);
}

ang::uint_t ang::collections::create_hash_index(ulong_t key, uint_t TS) {
	return _create_hash_index(key, TS);
}

ang::uint_t ang::collections::create_hash_index(float_t key, uint_t TS) {
	return _create_hash_index(key, TS);
}

ang::uint_t ang::collections::create_hash_index(double_t key, uint_t TS) {
	return _create_hash_index(key, TS);
}

ang::uint_t ang::collections::create_hash_index(void_ptr_t key, uint_t TS) {
	return _create_hash_index(key, TS);
}

ang::uint_t ang::collections::create_hash_index(cstr_t key, uint_t TS) {
	ulong_t h = 75025;
	for (int i = 0; key[i] != 0; i++)
		h = (h << 5) + h + key[i] + 1;
	return uint_t(h % TS);
}

ang::uint_t ang::collections::create_hash_index(castr_t key, uint_t TS) {
	ulong_t h = 75025;
	for (int i = 0; key[i] != 0; i++)
		h = (h << 5) + h + key[i] + 1;
	return uint_t(h % TS);
}


ang_uint32_t ang_create_hash_index(ang_int32_t key, ang_uint32_t TS) {
	return ang::collections::create_hash_index(key, TS);
}
ang_uint32_t ang_create_hash_index(ang_uint32_t key, ang_uint32_t TS) {
	return ang::collections::create_hash_index(key, TS);
}
ang_uint32_t ang_create_hash_index(ang_int64_t key, ang_uint32_t TS) {
	return ang::collections::create_hash_index(key, TS);
}
ang_uint32_t ang_create_hash_index(ang_uint64_t key, ang_uint32_t TS) {
	return ang::collections::create_hash_index(key, TS);
}
ang_uint32_t ang_create_hash_index(ang_void_ptr_t key, ang_uint32_t TS) {
	return ang::collections::create_hash_index(key, TS);
}
ang_uint32_t ang_create_hash_index(ang_cstr_t key, ang_uint32_t TS) {
	return ang::collections::create_hash_index(key, TS);
}
ang_uint32_t ang_create_hash_index(ang_cwstr_t key, ang_uint32_t TS) {
	return ang::collections::create_hash_index(key, TS);
}