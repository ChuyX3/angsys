#ifndef __ANG_MAP_H__
#define __ANG_MAP_H__

#include <ang/collections/hash_table.hpp>
#include <ang/collections/binary_tree.hpp>

namespace ang
{
	namespace collections
	{
		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> using map_object = binary_tree_object<K, T, allocator>;
		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> using map = object_wrapper<binary_tree_object<K, T, allocator>>;

		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> using unordered_map_object = hash_table_object<K, T, allocator>;
		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> using unordered_map = object_wrapper<unordered_map_object<K, T, allocator>>;
	}
}

#endif//__ANG_MAP_H__
