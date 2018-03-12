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

#include <ang/system.h>

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

namespace ang
{
	namespace collections
	{
		template<typename T>
		struct hash_code_maker {
			static ulong64 make(T const& value, ulong64 const TS) {
				union { T k; ulong64 val; } key;
				key.val = 0;
				key.k = value;
				return (ulong64)((2475825 + key.val + 1) % TS);
			}
		};

		template<typename T, text::encoding E>
		struct hash_code_maker<str_view<T, E>> {
			static ulong64 make(str_view<T, E> const& value, ulong64 const TS) {
				ulong64 h = 75025;
		
				windex i = 0, c = value.size();

				for (char32_t n = text::to_char32<false, text::is_endian_swapped<E>::value>(value.cstr(), i);
					n != 0;
					n = text::to_char32<false, text::is_endian_swapped<E>::value>(value.cstr(), i))
				{
					h = (h << 5) + h + n + 1;
				}
				return ang_uint64_t(h % TS);
			}
		};

		static const ang_uint32_t list[] = {
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

		inline wsize ang_hash_table_get_next_size(wsize size) {
			wsize out = 0;
			if (size >= 16381) {
				out = 16381;
				while (size >= out)
					out *= 2;
				return out;
			}
			else {
				for (auto i = 0U; i < algorithms::array_size(list); ++i)
					if (size < list[i])
						return list[i];
				return -1;
			}
		}

		template<typename K, typename T, 
			template<typename> class allocator = memory::unmanaged_allocator,
			template<typename> class hash_code_maker = collections::hash_code_maker>
		class hash_map {
		private:
			typedef linked_node<pair<K, T>> node_t, *node_ptr_t;

			wsize _size;
			allocator<node_t> alloc;
			scope_array<node_ptr_t, allocator> _table;

		public:
			hash_map() {
				_table.allocate(127);
			}
			~hash_map() {
				clear();
			}

			inline void clear() {
				for (wsize i = 0; i < _table.size() && _size > 0; ++i) {
					node_ptr_t temp = _table[i];
					_table[i] = null;
					while (temp) {
						node_ptr_t to_delete = temp;
						temp = temp->next;
						alloc.template destroy<node_t>(to_delete);
						alloc.deallocate(to_delete);
						_size--;
					}
				}
				_size = 0;
				_table.clear();
			}
			inline bool insert(K key, T value) {
				if (_size > (_table.size() * 0.75))
					increase_capacity();
				if (find_node(key) != null)
					return false;


				ulong64 hash = hash_code_maker<K>::make(key, _table.size());
				node_ptr_t entry = alloc.allocate(1);
				alloc.template construct<node_t>(entry);
				entry->data.key = key;
				entry->data.value = value;
				entry->next = _table[hash];
				_table[hash] = entry;
				_size++;
				return true;
			}
			inline bool remove(K key, T* out = null) {
				if (_table.is_empty())
					return false;

				ulong64 hash = hash_code_maker<K>::make(key, _table.size());

				node_ptr_t *prev = &_table[hash];
				node_ptr_t temp = *prev;

				while (temp != null) {
					if (logic_operation<K,K, logic_operation_type::same>::operate(temp->data.key, key)) {
						*prev = temp->next;
						if (out)*out = temp->data.value;
						alloc.template destroy<node_t>(temp);
						alloc.deallocate(temp);
						_size--;
						return true;
					}
					prev = &temp->next;
					temp = temp->next;
				}
				return false;
			}
			inline bool is_empty()const { return _size == 0; }
			inline bool contains(K key)const { return find_node(key) != null; }
			inline bool find(K key, T* out = null)const {
				node_ptr_t node = find_node(key);
				if (node == null) return false;
				if (out)*out = node->data.value;
				return true;
			}
			inline wsize size()const { return _size; }

			template<typename F> inline void iterate(F const& callback)const {
				for (windex i = 0; i < _size; i++) {
					node_ptr_t temp = _table[i];
					while (temp != null) {
						callback(temp->data);
						temp = temp->next;
					}
				}
			}

		private:
			node_ptr_t find_node(K const& key)const {
				if (_table.size() == 0)
					return null;
				ulong64 hash = hash_code_maker<K>::make(key, _table.size());
				node_ptr_t temp = _table[hash];
				while (temp != null) {
					if (logic_operation<K, K, logic_operation_type::same>::operate(temp->data.key, key))
						return temp;
					temp = temp->next;
				}
				return null;
			}
			void increase_capacity() {
		
				scope_array<node_ptr_t, allocator> new_data;
				wsize new_size = ang_hash_table_get_next_size(_table.size() + 20);
				new_data.allocate(new_size);
			
				if (!_table.is_empty() && _size)
				{
					for (wsize i = 0, s = _size; i < _table.size() && s > 0; ++i) {
						node_ptr_t temp = _table[i];
						_table[i] = null;
						while (temp) {
							ulong64 hash = hash_code_maker<K>::make(temp->data.key, _table.size());
							node_ptr_t entry = temp;
							temp = temp->next;
							entry->next = new_data[hash];
							new_data[hash] = entry;
							s--;
						}
					}
					_table.clear();
				}
				_table.move(new_data);
			}
		};


		template<typename T,
			template<typename> class allocator = memory::unmanaged_allocator,
			template<typename> class hash_code_maker = collections::hash_code_maker>
		class hash_set {
		private:
			typedef linked_node<T> node_t, *node_ptr_t;

			wsize _size;
			allocator<node_t> alloc;
			scope_array<node_ptr_t, allocator> _table;

		public:
			hash_set() {
				_table.allocate(127);
			}
			~hash_set() {
				clear();
			}

			inline void clear() {
				for (wsize i = 0; i < _table.size() && _size > 0; ++i) {
					node_ptr_t temp = _table[i];
					_table[i] = null;
					while (temp) {
						node_ptr_t to_delete = temp;
						temp = temp->next;
						alloc.template destroy<node_t>(to_delete);
						alloc.deallocate(to_delete);
						_size--;
					}
				}
				_size = 0;
				_table.clear();
			}
			inline bool insert(T value) {
				if (_size > (_table.size() * 0.75))
					increase_capacity();
				if (find_node(value) != null)
					return false;
				ulong64 hash = hash_code_maker<T>::make(value, _table.size());
				node_ptr_t entry = alloc.allocate(1);
				alloc.template construct<node_t>(entry);
				entry->data.value = ang::move(value);
				entry->next = _table[hash];
				_table[hash] = entry;
				_size++;
				return true;
			}
			inline bool remove(T const& out) {
				if (_table.is_empty())
					return false;

				ulong64 hash = hash_code_maker<T>::make(key, _table.size());

				node_ptr_t *prev = &_table[hash];
				node_ptr_t temp = *prev;

				while (temp != null) {
					if (logic_operation<T, T, logic_operation_type::same>::operate(temp->data.value, value)) {
						*prev = temp->next;
						if (out)*out = temp->data.value;
						alloc.template destroy<node_t>(temp);
						alloc.deallocate(temp);
						_size--;
						return true;
					}
					prev = &temp->next;
					temp = temp->next;
				}
				return false;
			}
			inline bool is_empty()const { return _size == 0; }
			inline bool contains(T const& value)const { return find_node(value) != null; }
			inline bool find(T const& value)const {
				node_ptr_t node = find_node(value);
				if (node == null)
					return false;
				return true;
			}
			inline wsize size()const { return _size; }

			template<typename F> inline void iterate(F const& callback)const {
				for (windex i = 0; i < _size; i++) {
					node_ptr_t temp = _table[i];
					while (temp != null) {
						callback(temp->data);
						temp = temp->next;
					}
				}
			}

		private:
			node_ptr_t find_node(T const& value)const {
				if (_table.size() == 0)
					return null;
				ulong64 hash = hash_code_maker<T>::make(value, _table.size());
				node_ptr_t temp = _table[hash];
				while (temp != null) {
					if (logic_operation<T, T, logic_operation_type::same>::operate(temp->data, value))
						return temp;
					temp = temp->next;
				}
				return null;
			}
			void increase_capacity() {

				scope_array<node_ptr_t, allocator> new_data;
				wsize new_size = ang_hash_table_get_next_size(_table.size() + 20);
				new_data.allocate(new_size);

				if (!_table.is_empty() && _size)
				{
					for (wsize i = 0, s = _size; i < _table.size() && s > 0; ++i) {
						node_ptr_t temp = _table[i];
						_table[i] = null;
						while (temp) {
							ulong64 hash = hash_code_maker<T>::make(temp->data, _table.size());
							node_ptr_t entry = temp;
							temp = temp->next;
							entry->next = new_data[hash];
							new_data[hash] = entry;
							s--;
						}
					}
					_table.clear();
				}
				_table.move(new_data);
			}
		};

	}
}


#endif//__ANGC_CORE_HASH_TABLE_H__