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

#include <angsys.h>

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
	namespace algorithms
	{
		wsize LINK hash_table_get_next_size(wsize);
	}



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

		template<typename K, typename T, 
			template<typename> class allocator = memory::unmanaged_allocator,
			template<typename> class hash_code_maker = collections::hash_code_maker>
		class internal_hash_map {
		private:
			typedef linked_node<pair<K, T>> node_t, *node_ptr_t;

			wsize m_size;
			allocator<node_t> alloc;
			scope_array<node_ptr_t, allocator> m_table;

		public:
			internal_hash_map() {
				m_size = 0;
				m_table.allocate(127);
			}
			~internal_hash_map() {
				clear();
			}

			inline void clear() {
				for (wsize i = 0; i < m_table.size() && m_size > 0; ++i) {
					node_ptr_t temp = m_table[i];
					m_table[i] = null;
					while (temp) {
						node_ptr_t to_delete = temp;
						temp = temp->next;
						alloc.template destroy<node_t>(to_delete);
						alloc.deallocate(to_delete);
						m_size--;
					}
				}
				m_size = 0;
				m_table.clear();
			}
			template<typename F>inline void clear(F f) {
				for (wsize i = 0; i < m_table.size() && m_size > 0; ++i) {
					node_ptr_t temp = m_table[i];
					m_table[i] = null;
					while (temp) {
						node_ptr_t to_delete = temp;
						temp = temp->next;
						f(to_delete->data);
						alloc.template destroy<node_t>(to_delete);
						alloc.deallocate(to_delete);
						m_size--;
					}
				}
				m_size = 0;
				m_table.clear();
			}
			inline bool insert(K key, T value) {
				if (m_size > (m_table.size() * 0.75))
					increase_capacity();
				if (find_node(key) != null)
					return false;


				ulong64 hash = hash_code_maker<K>::make(key, m_table.size());
				node_ptr_t entry = alloc.allocate(1);
				alloc.template construct<node_t>(entry);
				entry->data.key = key;
				entry->data.value = value;
				entry->next = m_table[hash];
				m_table[hash] = entry;
				m_size++;
				return true;
			}
			inline bool remove(K key, T* out = null) {
				if (m_table.is_empty())
					return false;

				ulong64 hash = hash_code_maker<K>::make(key, m_table.size());

				node_ptr_t *prev = &m_table[hash];
				node_ptr_t temp = *prev;

				while (temp != null) {
					if (logic_operation<logic_operation_type::same, K,K>::operate(temp->data.key, key)) {
						*prev = temp->next;
						if (out)*out = temp->data.value;
						alloc.template destroy<node_t>(temp);
						alloc.deallocate(temp);
						m_size--;
						return true;
					}
					prev = &temp->next;
					temp = temp->next;
				}
				return false;
			}
			inline bool is_empty()const { return m_size == 0; }
			inline bool contains(K key)const {
				return find_node(key) != null;
			}
			inline bool find(K key, T* out = null)const {
				node_ptr_t node = find_node(key);
				if (node == null) return false;
				if (out)*out = node->data.value;
				return true;
			}
			inline wsize size()const { return m_size; }

			template<typename F> inline void iterate(F const& callback)const {
				for (windex i = 0; i < m_size; i++) {
					node_ptr_t temp = m_table[i];
					while (temp != null) {
						callback(temp->data);
						temp = temp->next;
					}
				}
			}

		private:
			node_ptr_t find_node(K const& key)const {
				if (m_table.size() == 0)
					return null;
				ulong64 hash = hash_code_maker<K>::make(key, m_table.size());
				node_ptr_t temp = m_table[hash];
				while (temp != null) {
					if (logic_operation<logic_operation_type::same, K, K>::operate(temp->data.key, key))
						return temp;
					temp = temp->next;
				}
				return null;
			}
			void increase_capacity() {
		
				scope_array<node_ptr_t, allocator> new_data;
				wsize new_size = algorithms::hash_table_get_next_size(m_table.size() + 20);
				new_data.allocate(new_size);
			
				if (!m_table.is_empty() && m_size)
				{
					for (wsize i = 0; i < m_table.size(); ++i) {
						node_ptr_t temp = m_table[i];
						m_table[i] = null;
						while (temp) {
							ulong64 hash = hash_code_maker<K>::make(temp->data.key, new_data.size());
							node_ptr_t entry = temp;
							temp = temp->next;
							entry->next = new_data[hash];
							new_data[hash] = entry;
						}
					}
					m_table.clear();
				}
				m_table.move(new_data);
			}
		};


		template<typename T,
			template<typename> class allocator = memory::unmanaged_allocator,
			template<typename> class hash_code_maker = collections::hash_code_maker>
		class internal_hash_set {
		private:
			typedef linked_node<T> node_t, *node_ptr_t;

			wsize m_size;
			allocator<node_t> alloc;
			scope_array<node_ptr_t, allocator> m_table;

		public:
			internal_hash_set() {
				m_size = 0;
				m_table.allocate(127);
			}
			~internal_hash_set() {
				clear();
			}

			inline void clear() {
				for (wsize i = 0; i < m_table.size() && m_size > 0; ++i) {
					node_ptr_t temp = m_table[i];
					m_table[i] = null;
					while (temp) {
						node_ptr_t to_delete = temp;
						temp = temp->next;
						alloc.template destroy<node_t>(to_delete);
						alloc.deallocate(to_delete);
						m_size--;
					}
				}
				m_size = 0;
				m_table.clear();
			}
			template<typename F>inline void clear(F f) {
				for (wsize i = 0; i < m_table.size() && m_size > 0; ++i) {
					node_ptr_t temp = m_table[i];
					m_table[i] = null;
					while (temp) {
						node_ptr_t to_delete = temp;
						temp = temp->next;
						f(to_delete->data);
						alloc.template destroy<node_t>(to_delete);
						alloc.deallocate(to_delete);
						m_size--;
					}
				}
				m_size = 0;
				m_table.clear();
			}
			inline bool insert(T value) {
				if (m_size > (m_table.size() * 0.75))
					increase_capacity();
				if (find_node(value) != null)
					return false;
				ulong64 hash = hash_code_maker<T>::make(value, m_table.size());
				node_ptr_t entry = alloc.allocate(1);
				alloc.template construct<node_t>(entry);
				entry->data = ang::move(value);
				entry->next = m_table[hash];
				m_table[hash] = entry;
				m_size++;
				return true;
			}
			inline bool remove(T const& out) {
				if (m_table.is_empty())
					return false;

				ulong64 hash = hash_code_maker<T>::make(out, m_table.size());

				node_ptr_t *prev = &m_table[hash];
				node_ptr_t temp = *prev;

				while (temp != null) {
					if (logic_operation<logic_operation_type::same, T, T>::operate(temp->data, out)) {
						*prev = temp->next;
						if (out)*out = temp->data;
						alloc.template destroy<node_t>(temp);
						alloc.deallocate(temp);
						m_size--;
						return true;
					}
					prev = &temp->next;
					temp = temp->next;
				}
				return false;
			}
			inline bool is_empty()const { return m_size == 0; }
			inline bool contains(T const& value)const { return find_node(value) != null; }
			inline bool find(T const& value, T* out = null)const {
				node_ptr_t node = find_node(value);
				if (node == null) return false;
				if (out) *out = node->data;
				return true;
			}
			inline wsize size()const { return m_size; }

			template<typename F> inline void iterate(F const& callback)const {
				for (windex i = 0; i < m_size; i++) {
					node_ptr_t temp = m_table[i];
					while (temp != null) {
						callback(temp->data);
						temp = temp->next;
					}
				}
			}

		private:
			node_ptr_t find_node(T const& value)const {
				if (m_table.size() == 0)
					return null;
				ulong64 hash = hash_code_maker<T>::make(value, m_table.size());
				node_ptr_t temp = m_table[hash];
				while (temp != null) {
					if (logic_operation<logic_operation_type::same, T, T>::operate(temp->data, value))
						return temp;
					temp = temp->next;
				}
				return null;
			}
			void increase_capacity() {

				scope_array<node_ptr_t, allocator> new_data;
				wsize new_size = algorithms::hash_table_get_next_size(m_table.size() + 20);
				new_data.allocate(new_size);

				if (!m_table.is_empty() && m_size)
				{
					for (wsize i = 0; i < m_table.size(); ++i) {
						node_ptr_t temp = m_table[i];
						m_table[i] = null;
						while (temp) {
							ulong64 hash = hash_code_maker<T>::make(temp->data, new_data.size());
							node_ptr_t entry = temp;
							temp = temp->next;
							entry->next = new_data[hash];
							new_data[hash] = entry;
						}
					}
					m_table.clear();
				}
				m_table.move(new_data);
			}
		};

	}
}


#endif//__ANGC_CORE_HASH_TABLE_H__