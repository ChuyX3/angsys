/*********************************************************************************************************************/
/*   File Name: ang/base/vector.h                                                                                  */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                              */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_HASH_H__
#define __ANG_BASE_HASH_H__

namespace ang //constants
{
	namespace collections
	{		
		template<typename T, template<typename>class A = memory::default_allocator, template<typename>class H = algorithms::hash> struct hash_set;
		template<typename K, typename T, template<typename>class A = memory::default_allocator, template<typename>class H = algorithms::hash> struct hash_map;

		template<typename T, template<typename> class A, template<typename> class H>
		struct hash_set
		{
			static_assert(!is_const<T>::value, "T is constant type");
			using type = typename remove_reference<T>::type;
			using node_type = doubly_linked_node<type>;
			using node_type_ptr = value<doubly_linked_node<type>*>; //for auto initializing to null
			using node_type_ptr_ptr = value<doubly_linked_node<type>**>; //for auto initializing to null
			template<typename R>struct iterator;
			using iterator_t = iterator<type&>;
			using const_iterator_t = iterator<const type&>;
			struct list_type {
				list_type()
					: first(null)
					, last(null) {
				}
				list_type(list_type&& list)
					: first(ang::move(list.first))
					, last(ang::move(list.last)) {
				}
				list_type(list_type const& list)
					: first(list.first)
					, last(list.last) {
				}

				list_type& operator = (list_type&& list) {
					first = ang::move(list.first);
					last = ang::move(list.last);
					return*this;
				}
				list_type& operator = (list_type const& list) {
					first = list.first;
					last = list.last;
					return*this;
				}

				void push(node_type_ptr ptr) {
					if (first == null) {
						first = last = ptr;
						first->next = null;
						first->prev = null;
					}
					else {
						first->set_prev(ptr);
						first = ptr;
						first->prev = null;
					}
				}
				void pop(node_type_ptr_ptr ptr) {
					*ptr = last;
					if (last) {
						last = last->prev;
						if (last)
							last->next = null;
						else
							first = null;
					}
				}
				void remove(node_type_ptr ptr) {
					if (ptr == first) {
						first = first->next;
						if (first == null)
							last = null;
						else
							first->prev = null;
					}
					else if (ptr == last) {
						last = last->prev;
						if (last == null)
							first = null;
						else
							last->next = null;
					}
					else {
						ptr->prev->set_next(ptr->next);
						ptr->prev = null;
						ptr->next = null;
					}
				}
				node_type* first;
				node_type* last;
			};

			struct base_iterator
			{
				base_iterator()
					: table()
					, current(null)
					, idx(0) {
				}
				base_iterator(array_view<list_type> t, node_type_ptr n, windex i)
					: table(t)
					, current(n)
					, idx(i) {
				}
				base_iterator(base_iterator&& ptr)
					: table(ang::move(ptr.table))
					, current(ang::move(ptr.current))
					, idx(ang::move(ptr.idx)) {
					ptr.current = null;
					ptr.idx = 0;
				}
				base_iterator(base_iterator const& ptr)
					: table(ptr.table)
					, current(ptr.current)
					, idx(ptr.idx) {
				}
				bool is_valid()const {
					return current != null;
				}
				bool operator == (base_iterator const& it) {
					return current == it.current;
				}
				bool operator != (base_iterator const& it) {
					return current != it.current;
				}
				operator bool()const {
					return is_valid();
				}
			protected:
				friend hash_set<T, A, H>;
				mutable array_view<list_type> table;
				mutable node_type_ptr current;
				mutable ulong64 idx;
			};

			template<typename R>
			struct iterator : base_iterator {
				friend hash_set<T, A, H>;
				template<typename...Ts> iterator(Ts&&... it)
					: base_iterator(forward<Ts>(it)...) {
				}
				iterator& operator = (base_iterator const& it) {
					table = it.table;
					current = it.current;
					idx = it.idx;
					return*this;
				}
				iterator& operator = (iterator const& it) {
					table = it.table;
					current = it.current;
					idx = it.idx;
					return*this;
				}
				iterator& operator ++()const {
					current = current->next;
					while (current == null && idx < table.size())
						current = table[++idx].first;
					return *const_cast<iterator*>(this);
				}
				iterator operator ++(int)const {
					iterator it = *this;
					current = current->next;
					while (current == null && idx < table.size())
						current = table[++idx].first;
					return ang::move(it);
				}
				R operator*()const {
					return current->data;
				}
				typename remove_reference<R>::type* operator ->()const {
					return &current->data;
				}
			};

		protected:
			wsize m_size;
			array<list_type, A> m_table;
			H<type> m_hash;

		public: /*constructors and destructor*/
			hash_set()
				: m_size(0)
				, m_table(31) {
			}
			hash_set(wsize sz)
				: m_size(0)
				, m_table() {
				m_table.size(algorithms::hash_table_get_next_size(sz));
			}
			hash_set(hash_set && other)
				: m_size(other.m_size)
				, m_table(ang::move(other.m_table)) {
				other.m_size = 0;
			}
			hash_set(hash_set const& other)
				: m_size(other.m_size)
				, m_table(other.m_table) {
			}
			~hash_set() {
				clear();
			}

		public: /*properties*/
			bool is_empty()const {
				return m_size == 0;
			}
			wsize size()const {
				return m_size;
			}

		public: /*iterators*/
			iterator_t begin() {
				windex i = 0;
				node_type_ptr node = null;
				while (node == null && i < m_table.size())
					node = m_table[i++].first;
				return iterator_t(m_table.view(), node, --i);
			}
			iterator_t end() {
				return iterator_t(null, null, 0);
			}
			const_iterator_t begin()const {
				windex i = 0;
				node_type_ptr node = null;
				while (node == null && i < m_table.size())
					node = m_table[i++].first;
				return iterator_t(m_table.view(), node, --i);
			}
			const_iterator_t end()const {
				return iterator_t(null, null, 0);
			}

		public: /*utilities*/
			void clear() {
				for (wsize i = 0; i < m_table.size() && m_size > 0; ++i) {
					node_type_ptr temp = m_table[i].first;
					m_table[i].first = null;
					m_table[i].last = null;
					while (temp) {
						node_type_ptr to_delete = temp;
						temp = temp->next;
						A<node_type>::template destroy<node_type>(to_delete);
						A<node_type>::deallocate(to_delete);
						m_size--;
					}
				}
				m_size = 0;
				m_table.clear();
			}
			template<typename F>
			void clear(F const& callback) {
				for (wsize i = 0; i < m_table.size() && m_size > 0; ++i) {
					node_type_ptr temp = m_table[i].first;
					m_table[i].first = null;
					m_table[i].last = null;
					while (temp) {
						node_type_ptr to_delete = temp;
						temp = temp->next;
						callback(to_delete->data);
						A<node_type>::template destroy<node_type>(to_delete);
						A<node_type>::deallocate(to_delete);
						m_size--;
					}
				}
				m_size = 0;
				m_table.clear();
			}
			inline bool contains(type const& key)const {
				return find(key).is_valid();
			}
			inline iterator_t find(type const& key)const {
				ulong64 hash = 0;
				node_type_ptr node = find_node(key, hash);
				return iterator_t(m_table.view(), node, hash);
			}
			iterator_t insert(type&& value) {
				if (m_size > (m_table.size() * 0.75))
					realloc();
				ulong64 hash = 0;
				if (find_node(value, hash) != null)
					return end();
				m_table[hash].push(create_node(forward<type>(value)));
				m_size++;
				return iterator_t(m_table.view(), m_table[hash].first, hash);
			}
			iterator_t insert(type const& value) {
				if (m_size > (m_table.size() * 0.75))
					realloc();
				ulong64 hash = 0;
				if (find_node(value, hash) != null)
					return end();
				m_table[hash].push(create_node(value));
				m_size++;
				return iterator_t(m_table.view(), m_table[hash].first, hash);
			}

			bool remove(type & out) {
				if (m_table.is_empty())
					return false;
				ulong64 hash = m_hash(out) % m_table.size();

				node_type_ptr temp = m_table[hash].first;
				while (temp != null) {
					if (equal_to<type>::operate(temp->data, out)) {
						m_table[hash].remove(temp);
						out = ang::move(temp->data);
						A<node_type>::template destroy<node_type>(temp);
						A<node_type>::deallocate(temp);
						m_size--;
						return true;
					}
					temp = temp->next;
				}
				return false;
			}
			bool remove(type const& key) {
				if (m_table.is_empty())
					return false;
				ulong64 hash = m_hash(key) % m_table.size();
				node_type_ptr temp = m_table[hash].first;
				while (temp != null) {
					if (equal_to<type>::operate(temp->data, key)) {
						m_table[hash].remove(temp);
						A<node_type>::template destroy<node_type>(temp);
						A<node_type>::deallocate(temp);
						m_size--;
						return true;
					}
					temp = temp->next;
				}
				return false;
			}

			hash_set& operator = (hash_set&& map) {
				if (&map != this) {
					clear();
					m_size = map.size;
					m_table = ang::move(map.m_table);
					map.size = 0;
				}
				return*this;
			}
			hash_set& operator = (hash_set const& map) {
				if (&map != this) {
					clear();
					for (auto const& p : map)
						insert(p);
				}
				return*this;
			}
			hash_set& operator = (nullptr_t const&) {
				clear();
				return*this;
			}
			type const& operator [](type const& key)const {
				iterator_t it = find(key);
				if (it == end())
					throw_exception(error_code::array_overflow);
				return *it;
			}
			type& operator [](type const& key) {
				iterator_t it = find(key);
				if (it == end())
					it = insert(key);
				return *it;
			}

		protected: /*utilities*/
			template<typename Ts> node_type_ptr create_node(Ts&& args) {
				node_type_ptr node = A<node_type>::allocate(1);
				A<node_type>::template construct<node_type, type>(node, ang::forward<Ts>(args));
				return node;
			}
			node_type_ptr find_node(type const& key)const {
				if (m_table.size() == 0)
					return null;
				ulong64 hash = m_hash(key) % m_table.size();
				node_type_ptr temp = m_table[hash].first;
				while (temp != null) {
					if (equal_to<type>::operate(temp->data, key))
						return temp;
					temp = temp->next;
				}
				return null;
			}
			node_type_ptr find_node(type const& key, ulong64& hash)const {
				if (m_table.size() == 0)
					return null;
				hash = m_hash(key) % m_table.size();
				node_type_ptr temp = m_table[hash].first;
				while (temp != null) {
					if (equal_to<type>::operate(temp->data, key))
						return temp;
					temp = temp->next;
				}
				return null;
			}
			void realloc() {
				array<list_type, A> new_data;
				wsize new_size = algorithms::hash_table_get_next_size(m_table.size() + 20);
				new_data.size(new_size);

				if (!m_table.is_empty() && m_size)
				{
					for (wsize i = 0; i < m_table.size(); ++i) {
						node_type_ptr temp = m_table[i].first;
						m_table[i].first = null;
						m_table[i].last = null;
						while (temp) {
							ulong64 hash = m_hash(temp->data) % new_data.size();
							node_type_ptr entry = temp;
							temp = temp->next;
							new_data[hash].push(entry);
						}
					}
					m_table.clear();
				}
				m_table.move(new_data);
			}
		};
		
		template<typename K, typename T, template<typename> class A, template<typename> class H>
		struct hash_map
		{
			static_assert(!is_const<T>::value, "T is constant type");
			using key_type = typename remove_reference<K>::type;
			using element_type = typename remove_reference<T>::type;
			using pair_type = pair<key_type, element_type>;
			using pair_view_type = pair<const key_type, element_type>;
			using node_type = doubly_linked_node<pair_type>;
			using node_type_ptr = value<doubly_linked_node<pair_type>*>; //for auto initializing to null
			using node_type_ptr_ptr = value<doubly_linked_node<pair_type>**>; //for auto initializing to null
			template<typename R>struct iterator;
			using iterator_t = iterator<pair_type&>;
			using const_iterator_t = iterator<const pair_type&>;
			struct list_type {
				list_type()
					: first(null)
					, last(null) {
				}
				list_type(list_type&& list)
					: first(ang::move(list.first))
					, last(ang::move(list.last)) {
				}
				list_type(list_type const& list)
					: first(list.first)
					, last(list.last) {
				}

				list_type& operator = (list_type&& list) {
					first = ang::move(list.first);
					last = ang::move(list.last);
					return*this;
				}
				list_type& operator = (list_type const& list) {
					first = list.first;
					last = list.last;
					return*this;
				}

				void push(node_type_ptr ptr) {
					if (first == null) {
						first = last = ptr;
						first->next = null;
						first->prev = null;
					}
					else {
						first->set_prev(ptr);
						first = ptr;
						first->prev = null;
					}
				}
				void pop(node_type_ptr_ptr ptr) {
					*ptr = last;
					if (last) {
						last = last->prev;
						if (last)
							last->next = null;
						else
							first = null;
					}
				}
				void remove(node_type_ptr ptr) {
					if (ptr == first) {
						first = first->next;
						if (first == null)
							last = null;
						else
							first->prev = null;
					}
					else if (ptr == last) {
						last = last->prev;
						if (last == null)
							first = null;
						else
							last->next = null;
					}
					else {
						ptr->prev->set_next(ptr->next);
						ptr->prev = null;
						ptr->next = null;
					}
				}
				node_type* first;
				node_type* last;
			};

			struct base_iterator
			{
				base_iterator()
					: table()
					, current(null)
					, idx(0) {
				}
				base_iterator(array_view<list_type> t, node_type_ptr n, windex i)
					: table(t)
					, current(n)
					, idx(i) {
				}
				base_iterator(base_iterator&& ptr)
					: table(ang::move(ptr.table))
					, current(ang::move(ptr.current))
					, idx(ang::move(ptr.idx)) {
					ptr.current = null;
					ptr.idx = 0;
				}
				base_iterator(base_iterator const& ptr)
					: table(ptr.table)
					, current(ptr.current)
					, idx(ptr.idx) {
				}
				bool is_valid()const {
					return current != null;
				}
				bool operator == (base_iterator const& it) {
					return current == it.current;
				}
				bool operator != (base_iterator const& it) {
					return current != it.current;
				}
				operator bool()const {
					return is_valid();
				}
			protected:
				friend hash_map<K, T, A, H>;
				mutable array_view<list_type> table;
				mutable node_type_ptr current;
				mutable ulong64 idx;
			};
			template<typename R>
			struct iterator : base_iterator {
				friend hash_map<K, T, A, H>;
				template<typename...Ts> iterator(Ts&&... it)
					: base_iterator(forward<Ts>(it)...) {
				}
				iterator& operator = (base_iterator const& it) {
					table = it.table;
					current = it.current;
					idx = it.idx;
					return*this;
				}
				iterator& operator = (iterator const& it) {
					table = it.table;
					current = it.current;
					idx = it.idx;
					return*this;
				}
				iterator& operator ++()const {
					current = current->next;
					while (current == null && idx < table.size())
						current = table[++idx].first;
					return *const_cast<iterator*>(this);
				}
				iterator operator ++(int)const {
					iterator it = *this;
					current = current->next;
					while (current == null && idx < table.size())
						current = table[++idx].first;
					return ang::move(it);
				}
				R operator*()const {
					return current->data;
				}
				typename remove_reference<R>::type* operator ->()const {
					return &current->data;
				}
			};

		protected:
			wsize m_size;
			array<list_type, A> m_table;
			H<key_type> m_hash;

		public: /*constructors and destructor*/
			hash_map()
				: m_size(0)
				, m_table(31) {
			}
			hash_map(wsize sz)
				: m_size(0)
				, m_table() {
				m_table.size(algorithms::hash_table_get_next_size(sz));
			}
			hash_map(hash_map && other)
				: m_size(other.m_size)
				, m_table(ang::move(other.m_table)) {
				other.m_size = 0;
			}
			hash_map(hash_map const& other)
				: m_size(other.m_size)
				, m_table(other.m_table) {
			}
			~hash_map() {
				clear();
			}

		public: /*properties*/
			bool is_empty()const {
				return m_size == 0;
			}
			wsize size()const {
				return m_size;
			}

		public: /*iterators*/
			iterator_t begin() {
				windex i = 0;
				node_type_ptr node = null;
				while (node == null && i < m_table.size())
					node = m_table[i++].first;
				return iterator_t(m_table.view(), node, --i);
			}
			iterator_t end() {
				return iterator_t(null, null, 0);
			}
			const_iterator_t begin()const {
				windex i = 0;
				node_type_ptr node = null;
				while (node == null && i < m_table.size())
					node = m_table[i++].first;
				return iterator_t(m_table.view(), node, --i);
			}
			const_iterator_t end()const {
				return iterator_t(null, null, 0);
			}

		public: /*utilities*/
			void clear() {
				for (wsize i = 0; i < m_table.size() && m_size > 0; ++i) {
					node_type_ptr temp = m_table[i].first;
					m_table[i].first = null;
					m_table[i].last = null;
					while (temp) {
						node_type_ptr to_delete = temp;
						temp = temp->next;
						A<node_type>::template destroy<node_type>(to_delete);
						A<node_type>::deallocate(to_delete);
						m_size--;
					}
				}
				m_size = 0;
				m_table.clear();
			}
			template<typename F>
			void clear(F const& callback) {
				for (wsize i = 0; i < m_table.size() && m_size > 0; ++i) {
					node_type_ptr temp = m_table[i].first;
					m_table[i].first = null;
					m_table[i].last = null;
					while (temp) {
						node_type_ptr to_delete = temp;
						temp = temp->next;
						callback(to_delete->data);
						A<node_type>::template destroy<node_type>(to_delete);
						A<node_type>::deallocate(to_delete);
						m_size--;
					}
				}
				m_size = 0;
				m_table.clear();
			}
			inline bool contains(key_type const& key)const {
				return find(key).is_valid();
			}
			inline iterator_t find(key_type const& key)const {
				ulong64 hash = 0;
				node_type_ptr node = find_node(key, hash);
				return iterator_t(m_table.view(), node, hash);
			}
			iterator_t insert(key_type const& key, element_type&& value) {
				if (m_size > (m_table.size() * 0.75))
					realloc();
				ulong64 hash = 0;
				if (find_node(key, hash) != null)
					return end();
				m_table[hash].push(create_node(key, forward<element_type>(value)));
				m_size++;
				return iterator_t(m_table.view(), m_table[hash].first, hash);
			}
			iterator_t insert(key_type const& key, element_type const& value) {
				if (m_size > (m_table.size() * 0.75))
					realloc();
				ulong64 hash = 0;
				if (find_node(key, hash) != null)
					return end();
				m_table[hash].push(create_node(key, value));
				m_size++;
				return iterator_t(m_table.view(), m_table[hash].first, hash);
			}
			iterator_t insert(pair_type&& pair) {
				if (m_size > (m_table.size() * 0.75))
					realloc();
				ulong64 hash = 0;
				if (find_node(pair.key, hash) != null)
					return end();
				m_table[hash].push(create_node(forward<pair_type>(pair)));
				m_size++;
				return iterator_t(m_table.view(), m_table[hash].first, hash);
			}
			iterator_t insert(pair_type const& pair) {
				if (m_size > (m_table.size() * 0.75))
					realloc();
				ulong64 hash = 0;
				if (find_node(pair.key, hash) != null)
					return end();
				m_table[hash].push(create_node(pair));
				m_size++;
				return iterator_t(m_table.view(), m_table[hash].first, hash);
			}
			bool remove(key_type const& key, element_type& out) {
				if (m_table.is_empty())
					return false;
				ulong64 hash = m_hash(key) % m_table.size();
			
				node_type_ptr temp = m_table[hash].first;
				while (temp != null) {
					if (equal_to<key_type>::operate(temp->data.key, key)) {
						m_table[hash].remove(temp);
						out = ang::move(temp->data.value);
						A<node_type>::template destroy<node_type>(temp);
						A<node_type>::deallocate(temp);
						m_size--;
						return true;
					}
					temp = temp->next;
				}
				return false;
			}
			bool remove(key_type const& key) {
				if (m_table.is_empty())
					return false;
				ulong64 hash = m_hash(key) % m_table.size();
				node_type_ptr temp = m_table[hash].first;
				while (temp != null) {
					if (equal_to<key_type>::operate(temp->data.key, key)) {
						m_table[hash].remove(temp);
						A<node_type>::template destroy<node_type>(temp);
						A<node_type>::deallocate(temp);
						m_size--;
						return true;
					}
					temp = temp->next;
				}
				return false;
			}

			bool remove(base_iterator& it, element_type& out) {
				if (m_table.is_empty() || it.current == null)
					return false;
				ulong64 hash = it.idx;// m_hash(it.current->data.key) % m_table.size();

				node_type_ptr temp = m_table[hash].first;
				while (temp != null) {
					if (equal_to<key_type>::operate(temp->data.key, it.current->data.key)) {
						it.current = temp->next;
						m_table[hash].remove(temp);
						out = ang::move(temp->data.value);
						A<node_type>::template destroy<node_type>(temp);
						A<node_type>::deallocate(temp);
						m_size--;
						return true;
					}
					temp = temp->next;
				}
				return false;
			}
			bool remove(base_iterator& it) {
				if (m_table.is_empty() || it.current == null)
					return false;
				ulong64 hash = it.idx;// m_hash(it.current->data.key) % m_table.size();

				node_type_ptr temp = m_table[hash].first;
				while (temp != null) {
					if (equal_to<key_type>::operate(temp->data.key, it.current->data.key)) {
						it.current = temp->next;
						m_table[hash].remove(temp);
						A<node_type>::template destroy<node_type>(temp);
						A<node_type>::deallocate(temp);
						m_size--;
						return true;
					}
					temp = temp->next;
				}
				return false;
			}

			bool copy(hash_map<K, T, A, H> const& map) {
				clear();
				for (const pair_type& pair : map) {
					insert(pair);
				}
			}
			bool extend(hash_map<K, T, A, H> const& map) {
				for (const pair_type& pair : map) {
					insert(pair);
				}
			}
		protected: /*utilities*/
			template<typename... Ts> node_type_ptr create_node(Ts...args) {
				node_type_ptr node = A<node_type>::allocate(1);
				A<node_type>::template construct<node_type, pair_type>(node, pair_type(ang::forward<Ts>(args)...));
				return node;
			}
			node_type_ptr find_node(key_type const& key)const {
				if (m_table.size() == 0)
					return null;
				ulong64 hash = m_hash(key) % m_table.size();
				node_type_ptr temp = m_table[hash].first;
				while (temp != null) {
					if (equal_to<key_type>::operate(temp->data.key, key))
						return temp;
					temp = temp->next;
				}
				return null;
			}
			node_type_ptr find_node(key_type const& key, ulong64& hash)const {
				if (m_table.size() == 0)
					return null;
				hash = m_hash(key) % m_table.size();
				node_type_ptr temp = m_table[hash].first;
				while (temp != null) {
					if (equal_to<key_type>::operate(temp->data.key, key))
						return temp;
					temp = temp->next;
				}
				return null;
			}
			void realloc() {
				array<list_type, A> new_data;
				wsize new_size = algorithms::hash_table_get_next_size(m_table.size() + 20);
				new_data.size(new_size);

				if (!m_table.is_empty() && m_size)
				{
					for (wsize i = 0; i < m_table.size(); ++i) {
						node_type_ptr temp = m_table[i].first;
						m_table[i].first = null;
						m_table[i].last = null;
						while (temp) {
							ulong64 hash = m_hash(temp->data.key) % new_data.size();
							node_type_ptr entry = temp;
							temp = temp->next;
							new_data[hash].push(entry);
						}
					}
					m_table.clear();
				}
				m_table.move(new_data);
			}

			node_type_ptr get_node(base_iterator const& it)const {
				return it.current;
			}

			ulong64 get_index(base_iterator const& it)const {
				return it.idx;
			}

		public: /*operators*/
			hash_map& operator = (hash_map&& map) {
				if (&map != this) {
					clear();
					m_size = map.m_size;
					m_table = ang::move(map.m_table);
					map.m_size = 0;
				}
				return*this;
			}
			hash_map& operator = (hash_map const& map) {
				if (&map != this) {
					clear();
					for (auto const& p : map)
						insert(p);
				}
				return*this;
			}
			hash_map& operator = (nullptr_t const&) {
				clear();
				return*this;
			}
			element_type const& operator [](key_type const& key)const {
				iterator_t it = find(key);
				if (it == end())
					throw_exception(error_code::array_overflow);
				return it->value;
			}
			element_type& operator [](key_type const& key) {
				iterator_t it = find(key);
				if (it == end())
					it = insert(key, default_value<element_type>::value);
				return it->value;
			}
		};

		
	}
}


#endif//__ANG_BASE_LIST_H__
