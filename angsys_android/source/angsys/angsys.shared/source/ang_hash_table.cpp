#include "pch.h"
#include "ang_inlines.h"


template<class K>
inline uint _create_hash_index(K const& k, uint const TS)
{
	union {
		K k;
		ulong64 val;
	}key;
	key.val = 0;
	key.k = k;
	return (uint)((2475825 + key.val + 1) % TS);
}

uint ang_create_hash_index(int key, uint TS) {
	return _create_hash_index<int>(key, TS);
}
uint ang_create_hash_index(uint key, uint TS) {
	return _create_hash_index<uint>(key, TS);
}
uint ang_create_hash_index(long key, uint TS) {
	return _create_hash_index<long64>(key, TS);
}
uint ang_create_hash_index(ulong key, uint TS) {
	return _create_hash_index<ulong64>(key, TS);
}
uint ang_create_hash_index(long64 key, uint TS) {
	return _create_hash_index<long64>(key, TS);
}
uint ang_create_hash_index(ulong64 key, uint TS) {
	return _create_hash_index<ulong64>(key, TS);
}
uint ang_create_hash_index(pointer key, uint TS) {
	return _create_hash_index<pointer>(key, TS);
}
uint ang_create_hash_index(const char* key, uint TS) {
	ulong h = 75025;
	for (int i = 0; key[i] != 0; i++)
		h = (h << 5) + h + key[i] + 1;
	return uint(h % TS);
}
uint ang_create_hash_index(const wchar* key, uint TS) {
	ulong h = 75025;
	for (int i = 0; key[i] != 0; i++)
		h = (h << 5) + h + key[i] + 1;
	return uint(h % TS);
}

namespace ang
{
	namespace collections
	{
		uint hash_table_get_next_size(uint size) { return ang_hash_table_get_next_size(size); }
		uint create_hash_index(int value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(uint value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(long value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(ulong value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(long64 value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(ulong64 value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(float value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(double value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(pointer value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(cstr_t value, uint TS) { return ang_create_hash_index(value, TS); }
		uint create_hash_index(cwstr_t value, uint TS) { return ang_create_hash_index(value, TS); }
	}
}

uint list[] =
{
	7u,
	13u,
	31u,
	61u,
	127u,
	251u,
	509u,
	1021u,
	2039u,
	4093u,
	8191u,
	16381u,
};

extern "C" wsize ang_hash_table_get_next_size(wsize size)
{
	uint out = 0;
	if (size >= 16381)
	{
		out = 16381;
		while (size >= out)
			out *= 2;
		return out;
	}
	else
	{
		for (auto i = 0U; i < ang::array_size(list); ++i)
			if (size < list[i])
				return list[i];
		return -1;
	}
}
