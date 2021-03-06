#include "pch.h"
#include <angsys.h>
#include "core_hash_table.h"

using namespace ang;
using namespace ang::text;

typedef collections::internal_hash_map<castr_t, const_string>* pool_handle;

string_pool::string_pool()
{
	pool = ang_alloc_unmanaged_memory(sizeof(collections::internal_hash_map<castr_t, const_string>));
	pool = new(pool)collections::internal_hash_map<castr_t, const_string>();
}

string_pool::~string_pool()
{
	pool_handle(pool)->~internal_hash_map();
	ang_free_unmanaged_memory(pool);
}

castr_t string_pool::save_string(castr_t str)
{
	const_string value;
	if (!pool_handle(pool)->find(str, &value)) {
		value = new(str)const_string_buffer();
		pool_handle(pool)->insert(value->cstr(), value);
	}	
	return value->cstr();
}

castr_t string_pool::save_string(astring const& str)
{
	const_string value;
	if (!pool_handle(pool)->find(str, &value)) {
		value = new(str->cstr())const_string_buffer();
		pool_handle(pool)->insert(value->cstr(), value);
	}
	return value->cstr();
}

castr_t string_pool::save_string(const_string str)
{
	const_string value;
	if (!pool_handle(pool)->find(str->cstr(), &value)) {
		value = str;
		pool_handle(pool)->insert(value->cstr(), value);
	}
	return value->cstr();
}

castr_t string_pool::save_string(const_string_buffer* str)
{
	const_string value;
	if (!pool_handle(pool)->find(str->cstr(), &value)) {
		value = str;
		pool_handle(pool)->insert(value->cstr(), value);
	}
	return value->cstr();
}

const_string string_pool::find_string(castr_t str)
{
	const_string value;
	if (pool_handle(pool)->find(str, &value))
		return null;
	return value;
}