#ifndef __ANG_MAP_H__
#define __ANG_MAP_H__

#include <ang/collections/hash_table.h>


namespace ang
{
	namespace collections
	{
		template<typename K, typename T> using map_data = hash_table_data<K, T>;
		template<typename K, typename T> using map = object_wrapper<hash_table_data<K, T>>;
	}
}

#endif//__ANG_MAP_H__
