/*********************************************************************************************************************/
/*   File Name: ang/base/binary_tree.h                                                                                  */
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
#elif !defined __ANG_BASE_BINARY_TREE_H__
#define __ANG_BASE_BINARY_TREE_H__

namespace ang
{
	namespace collections
	{
		template<typename T, template<typename> class A = memory::default_allocator, template<typename, typename> class O = less> struct binary_set;
		template<typename K, typename T, template<typename> class A = memory::default_allocator, template<typename, typename> class O = less> struct binary_map;


		template<typename T>
		struct binary_linked_node
		{
			using value_type = typename remove_reference<T>::type;
			using self = binary_linked_node<T>;
			using self_ptr = binary_linked_node<T>*;

			binary_linked_node()
				: data()
				, height(1)
				, nodes{ null,null,null } {
			}
			binary_linked_node(value_type&& value)
				: data(ang::forward<type>(value))
				, height(1)
				, nodes{ null,null,null } {
			}
			binary_linked_node(value_type const& value)
				: data(value)
				, height(1)
				, nodes{ null,null,null } {
			}
			self_ptr const& get_parent()const {
				return nodes[0];
			}
			void set_parent(self_ptr n) {
				nodes[0] = ang::move(n);
			}
			self_ptr get_left()const {
				return nodes[1];
			}
			void set_left(self_ptr n) {
				nodes[1] = ang::move(n);
				if (nodes[1]) {
					nodes[1]->nodes[0] = this;
				}
			}
			self_ptr get_right()const {
				return nodes[2];
			}
			void set_right(self_ptr n) {
				nodes[2] = ang::move(n);
				if (nodes[2]) {
					nodes[2]->nodes[0] = this;
				}
			}
			int get_type()const {
				return nodes[0] == null ? 0 : nodes[0]->nodes[1] == this ? 1 : 2;
			}
			int get_balance()const {
				return this != null ? get_left()->get_height() - get_right()->get_height() : 0;
			}
			int get_height()const {
				return this ? height : 0;
			}

			value_type data;
			int height;
			self_ptr nodes[3];
		};

		template<typename K, typename T>
		struct binary_linked_node<pair<K, T>>
		{
			using key_type = typename remove_reference<K>::type;
			using value_type = typename remove_reference<T>::type;
			using pair_type = pair<key_type, value_type>;
			using self = binary_linked_node<pair<K, T>>;
			using self_ptr = binary_linked_node<pair<K, T>>*;

			binary_linked_node()
				: data()
				, height(1)
				, nodes{ null,null,null } {
			}
			binary_linked_node(pair_type&& value)
				: data(ang::forward<type>(value))
				, height(1)
				, nodes{ null,null,null } {
			}
			binary_linked_node(pair_type const& value)
				: data(value)
				, height(1)
				, nodes{ null,null,null } {
			}
			binary_linked_node(key_type const& key, value_type && value)
				: data(key, ang::forward<value_type>(value))
				, height(1)
				, nodes{ null,null,null } {
			}
			binary_linked_node(key_type const& key, value_type const& value)
				: data(key, value)
				, height(1)
				, nodes{ null,null,null } {
			}
			self_ptr const& get_parent()const {
				return nodes[0];
			}
			void set_parent(self_ptr n) {
				nodes[0] = ang::move(n);
			}
			self_ptr get_left()const {
				return nodes[1];
			}
			void set_left(self_ptr n) {
				nodes[1] = ang::move(n);
				if (nodes[1]) {
					nodes[1]->nodes[0] = this;
				}
			}
			self_ptr get_right()const {
				return nodes[2];
			}
			void set_right(self_ptr n) {
				nodes[2] = ang::move(n);
				if (nodes[2]) {
					nodes[2]->nodes[0] = this;
				}
			}
			int get_type()const {
				return nodes[0] == null ? 0 : nodes[0]->nodes[1] == this ? 1 : 2;
			}
			int get_balance()const {
				return this != null ? get_left()->get_height() - get_right()->get_height() : 0;
			}
			int get_height()const {
				return this ? height : 0;
			}

			pair_type data;
			int height;
			self_ptr nodes[3];
		};

		template<typename T>
		struct binary_node_inorder_iterator
			: algorithms::iteration_algorithm<binary_linked_node<T>>
		{
			using node_ptr_type = typename algorithms::iteration_algorithm<binary_linked_node<T>>::node_ptr_type;
			using iteration_callback_type = typename algorithms::iteration_algorithm<binary_linked_node<T>>::iteration_callback_type;

			binary_node_inorder_iterator() {
				vtable = &s_vtable[0];
			}

		private:
			static node_ptr_type begin(node_ptr_type node, wsize*) {
				return most_left(node, null);
			}
			static node_ptr_type end(node_ptr_type node, wsize*) {
				return null;
			}
			static node_ptr_type rbegin(node_ptr_type node, wsize*) {
				return most_right(node, null);
			}
			static node_ptr_type rend(node_ptr_type node, wsize*) {
				return null;
			}
			static node_ptr_type increase(node_ptr_type node, wsize*) {
				if (node == null)
					return null;

				if (node->get_right() != null)
					return most_left(node->get_right(), null);
				else
				{
					if (node->get_type() == 1)
						return node->get_parent();
					else if (node->get_type() == 2) {
						while (node->get_parent() && node->get_type() == 2)
							node = node->get_parent();
						return node->get_parent();
					}
				}
				return null;
			}
			static node_ptr_type decrease(node_ptr_type node, wsize*) {
				if (node == null)
					return null;

				if (node->get_left() != null)
					return most_right(node->get_right(), null);
				else
				{
					if (node->get_type() == 1)
						return node->get_parent();
					else if (node->get_type() == 2) {
						while (node->get_parent() && node->get_type() == 2)
							node = node->get_parent();
						return node->get_parent();
					}
				}
				return null;
			}
			static node_ptr_type most_left(node_ptr_type node, wsize*) {
				while (node && node->get_left())
					node = node->get_left();
				return node;
			}
			static node_ptr_type most_right(node_ptr_type node, wsize*) {
				while (node && node->get_right())
					node = node->get_right();
				return node;
			}

			static constexpr iteration_callback_type s_vtable[] = {
				&begin,
				&end,
				&rbegin,
				&rend,
				&increase,
				&decrease,
				&most_left,
				&most_right
			};
		};

		template<typename K, typename T, template<typename> class A, template<typename, typename> class O>
		struct binary_map
		{
			static_assert(!is_const<T>::value, "T is constant type");
			using key_type = typename remove_reference<K>::type;
			using element_type = typename remove_reference<T>::type;
			using pair_type = pair<key_type, element_type>;
			using pair_view_type = pair<const key_type, element_type>;
			using node_type = binary_linked_node<pair_type>;
			using node_ptr_type = value<node_type*>;
			template<typename R> struct iterator;
			using iterator_t = iterator<pair_view_type&>;
			using const_iterator_t = iterator<const pair_view_type&>;
			using inorder_type = binary_node_inorder_iterator<pair_type>;
			using algorithm_type = algorithms::iteration_algorithm<node_type>;

			typedef struct base_iterator {
				base_iterator(algorithm_type iter = inorder_type())
					: node(null)
					, m_algorithm(iter) {
				}
				base_iterator(node_ptr_type ptr, algorithm_type iter = inorder_type())
					: node(ptr)
					, m_algorithm(iter) {
				}
				base_iterator(base_iterator&& it)
					: node(it.node)
					, m_algorithm(it.m_algorithm) {
					it.node = null;
				}
				base_iterator(base_iterator const& it)
					: node(it.node)
					, m_algorithm(it.m_algorithm) {
				}
				base_iterator& operator = (base_iterator const& it) {
					node = it.node;
					m_algorithm = it.m_algorithm;
					return*this;
				}
				base_iterator& operator = (nullptr_t const&) {
					node = null;
					return*this;
				}
				bool operator == (base_iterator const& it) {
					return node == it.node;
				}
				bool operator != (base_iterator const& it) {
					return node != it.node;
				}
				bool is_valid()const {
					return node != null;
				}

			protected:
				friend binary_map<K, T, A, O>;
				mutable node_ptr_type node;
				algorithm_type m_algorithm;
			}base_iterator_t;

			template<typename R>
			struct iterator : base_iterator {
				friend binary_map<K, T, A, O>;
				template<typename...U> iterator(U&&... it)
					: base_iterator(forward<U>(it)...) {
				}
				iterator& operator = (base_iterator const& it) {
					node = it.node;
					return*this;
				}
				iterator& operator = (iterator const& it) {
					node = it.node;
					return*this;
				}
				iterator& operator = (nullptr_t const&) {
					node = null;
					return*this;
				}
				iterator& operator ++() {
					
					node = m_algorithm.increase(node, null);
					return*this;
				}
				iterator operator ++(int) {
					iterator it = node;
					node = m_algorithm.increase(node, null);
					return ang::move(it);
				}
				iterator& operator --() {
					node = m_algorithm.decrerase(node, null);
					return*this;
				}
				iterator operator --(int) {
					iterator it = node;
					node = m_algorithm.decrerase(node, null);
					return ang::move(it);
				}
				R operator*()const {
					return (R)node->data;
				}
			};

		protected:
			wsize m_size;
			node_ptr_type m_root;
			O<key_type, key_type> less_that;

		public:
			binary_map()
				: m_size(0)
				, m_root(null) {
			}
			binary_map(const std::nullptr_t&)
				: m_size(0)
				, m_root(null) {

			}
			binary_map(binary_map&& ar)
				: m_size(0)
				, m_root(null) {
				m_size = ar.m_size;
				m_root = ar.m_root;
				ar.m_root = null;
				ar.m_size = 0;
			}
			binary_map(const binary_map& map)
				: m_size(0)
				, m_root(null) {
				for (auto const& pair : map) {
					insert(pair.key, pair.value);
				}
			}
			template<typename U, typename V>
			inline binary_map(std::initializer_list<pair<U, V>> list) {
				for (auto const& pair : list) {
					insert((key_type)pair.key, (element_type)pair.value);
				}
			}
			virtual ~binary_map() {
				clear();
			}

		public: /*iterators*/
			iterator_t begin(algorithm_type it = inorder_type()) {
				return iterator_t(it.begin(m_root, null), it);
			}
			iterator_t end(algorithm_type it = inorder_type()) {
				return iterator_t(it.end(m_root, null), it);
			}
			const_iterator_t begin(algorithm_type it = inorder_type())const {
				return const_iterator_t(it.begin(m_root, null), it);
			}
			const_iterator_t end(algorithm_type it = inorder_type())const {
				return const_iterator_t(it.end(m_root, null), it);
			}

		public: /*utils*/
			bool is_empty()const {
				return m_size == 0;
			}
			wsize size()const {
				return m_size;
			}
			void clear() {
				destroy_tree(m_root);
				m_root = null;
				m_size = 0;
			}
			bool contains(key_type const& key)const {
				return find_node(key, m_root) != null;
			}
			iterator_t find(key_type const& key) {
				return find_node(key, m_root);
			}
			const_iterator_t find(key_type const& key)const {
				return find_node(key, m_root);
			}
			bool insert(key_type const& key, element_type && value) {
				return insert_node(key, ang::forward<element_type>(value)) != null;
			}
			bool insert(key_type const& key, element_type const& value) {
				return insert_node(key, value) != null;
			}
			bool update(key_type const& key, element_type && value) {
				return insert_node(key, ang::forward<element_type>(value), true) != null;
			}
			bool update(key_type const& key, element_type const& value) {
				return insert_node(key, value, true) != null;
			}

		protected: /*utils*/
			template<typename...Ts>
			node_ptr_type create_node(Ts&&... args) {
				return A<node_type>::template construct<node_type, Ts...>(A<node_type>::allocate(1), ang::forward<Ts>(args)...);
			}
			void destroy_node(node_ptr_type node) {
				A<node_type>::template destroy<node_type>(node);
				A<node_type>::deallocate(node);
			}
			void destroy_tree(node_ptr_type node) {
				if (node != null) {
					destroy_tree(node->get_left());
					node->set_left(null);
					destroy_tree(node->get_right());
					node->set_right(null);
					destroy_node(node);
				}
			}
			node_ptr_type find_node(key_type const& key, node_ptr_type node)const {
				while (node != null) {
					if (less_that(key, node->data.key))
						node = node->nodes[1];
					else if (less_that(node->data.key, key))
						node = node->nodes[2];
					else
						return node;
				}
				return null;
			}
			static node_ptr_type most_left(node_ptr_type node) {
				if (node == null)
					return null;
				while (node->nodes[1] != null)
					node = node->nodes[1];
				return node;
			}
			static node_ptr_type most_right(node_ptr_type node) {
				if (node == null)
					return null;
				while (node->nodes[2] != null)
					node = node->nodes[2];
				return node;
			}
			static node_ptr_type right_rotate(node_ptr_type y) {
				node_ptr_type x = y->get_left();
				y->set_left(x->get_right());
				x->set_right(y);
				y->height = max(y->get_left()->get_height(), y->get_right()->get_height()) + 1;
				x->height = max(x->get_left()->get_height(), x->get_right()->get_height()) + 1;
				return x;
			}
			static node_ptr_type left_rotate(node_ptr_type x) {
				node_ptr_type y = x->get_right();
				x->set_right(y->get_left());
				y->set_left(x);
				x->height = max(x->get_left()->get_height(), x->get_right()->get_height()) + 1;
				y->height = max(y->get_left()->get_height(), y->get_right()->get_height()) + 1;
				return y;
			}
			template<typename U, typename V>
			node_ptr_type insert_node(U&& key, V && value, bool update = false) {
				if (m_root == null) {
					m_root = create_node(ang::forward<U>(key), ang::forward<V>(value));
					m_size++;
					return m_root;
				}
				node_ptr_type new_node = null;
				node_ptr_type node = m_root;
				while (true) {
					if (less_that(key, node->data.key)) {
						if (node->get_left() != null)
							node = node->get_left();
						else {
							new_node = create_node(ang::forward<U>(key), ang::forward<V>(value));
							node->set_left(new_node);
							break;
						}
					}
					else if (less_that(node->data.key, key)) {
						if (node->get_right() != null)
							node = node->get_right();
						else {
							new_node = create_node(ang::forward<U>(key), ang::forward<V>(value));
							node->set_right(new_node);
							break;
						}
					}
					else if (update) {
						node->data.value = ang::forward<V>(value);
					}
					else {
						return null;
					}
				}
				node_ptr_type parent;
				while (node->get_parent() != null) {
					node->height = 1 + max(node->get_left()->get_height(), node->get_right()->get_height());
					int balance = node->get_balance();
					if (balance > 1 && less_that(key, node->get_left()->data.key)) {
						parent = node->get_parent();
						node->get_type() == 1 ? parent->set_left(right_rotate(node)) : parent->set_right(right_rotate(node));
						node = parent;
					}
					else if (balance < -1 && less_that(node->get_right()->data.key, key)) { // Right Right Case
						parent = node->get_parent();
						node->get_type() == 1 ? parent->set_left(left_rotate(node)) : parent->set_right(left_rotate(node));
						node = parent;
					}
					else if (balance > 1 && less_that(node->get_left()->data.key, key)) { // Left Right Case
						node->set_left(left_rotate(node->get_left()));
						parent = node->get_parent();
						node->get_type() == 1 ? parent->set_left(right_rotate(node)) : parent->set_right(right_rotate(node));
						node = parent;
					}
					else if (balance < -1 && less_that(key, node->get_right()->data.key)) { // Right Left Case
						node->set_right(right_rotate(node->get_right()));
						parent = node->get_parent();
						node->get_type() == 1 ? parent->set_left(left_rotate(node)) : parent->set_right(left_rotate(node));
						node = parent;
					}
					else
						node = node->get_parent();
				}

				m_root->height = 1 + max(node->get_left()->get_height(), node->get_right()->get_height());
				int balance = node->get_balance();
				if (balance > 1 && less_that(key, node->get_left()->data.key))
					m_root = right_rotate(node);
				else if (balance < -1 && less_that(node->get_right()->data.key, key)) // Right Right Case
					m_root = left_rotate(node);
				else if (balance > 1 && less_that(node->get_left()->data.key, key)) { // Left Right Case
					node->set_left(left_rotate(node->get_left()));
					m_root = right_rotate(node);
				}
				else if (balance < -1 && less_that(key, node->get_right()->data.key)) { // Right Left Case
					node->set_right(right_rotate(node->get_right()));
					m_root = left_rotate(node);
				}
				m_root->set_parent(null);
				m_size++;
				return new_node;
			}
			node_ptr_type remove_node(node_ptr_type node, key_type const& key, bool& removed) {
				if (node == null)
					return null;

				if (less_that(key, node->data.key))
					node->set_left(remove_node(node->get_left(), key, removed));
				else if (less_that(node->data.key, key))
					node->set_right(remove_node(node->get_right(), key, removed));
				else {
					removed = true;
					if (!node->get_left() || !node->get_right()) {
						node_ptr_type temp = node->get_left() ? node->get_left() : node->get_right();
						if (temp != null) {
							node->set_left(temp->get_left());
							node->set_right(temp->get_right());
							node->height = temp->get_height();
							node->data = ang::move(temp->data);
							destroy_node(temp);
						}
						else
						{
							destroy_node(node);
							return null;
						}
					}
					else
					{
						bool _removed = false;
						node_ptr_type temp = most_left(node->get_right());
						node->data.key = temp->data.key;
						node->data.value = ang::move(temp->data.value);
						node->set_right(remove_node(node->get_right(), temp->data.key, _removed));
					}
				}
				node->height = 1 + max(node->get_left()->get_height(), node->get_right()->get_height());
				int balance = node->get_balance();

				if (balance > 1 && node->get_left()->get_balance() >= 0)
					return right_rotate(node);
				else if (balance > 1 && node->get_left()->get_balance() < 0)
				{
					node->set_left(left_rotate(node->get_left()));
					return right_rotate(node);
				}
				else if (balance < -1 && node->get_right()->get_balance() <= 0)
					return left_rotate(node);
				else if (balance < -1 && node->get_right()->get_balance() > 0)
				{
					node->set_right(right_rotate(node->get_right()));
					return left_rotate(node);
				}
				return node;
			}
			node_ptr_type remove_node(node_ptr_type node, key_type const& key, element_type& out, bool& removed) {
				if (node == null)
					return null;

				if (less_that(key, node->data.key))
					node->set_left(remove_node(node->get_left(), key, out, removed));
				else if (less_that(node->data.key, key))
					node->set_right(remove_node(node->get_right(), key, out, removed));
				else {
					removed = true;
					out = ang::move(node->data.value);
					if (!node->get_left() || !node->get_right()) {
						node_ptr_type temp = node->get_left() ? node->get_left() : node->get_right();
						if (temp != null) {
							node->set_left(temp->get_left());
							node->set_right(temp->get_right());
							node->height = temp->get_height();
							node->data = ang::move(temp->data);
							destroy_node(temp);
						}
						else
						{
							destroy_node(node);
							return null;
						}
					}
					else
					{
						bool _removed = false;
						node_ptr_type temp = most_left(node->get_right());
						node->data.key = temp->data.key;
						node->data.value = ang::move(temp->data.value);
						node->set_right(remove_node(node->get_right(), temp->data.key, _removed));
					}
				}
				node->height = 1 + max(node->get_left()->get_height(), node->get_right()->get_height());
				int balance = node->get_balance();

				if (balance > 1 && node->get_left()->get_balance() >= 0)
					return right_rotate(node);
				else if (balance > 1 && node->get_left()->get_balance() < 0)
				{
					node->set_left(left_rotate(node->get_left()));
					return right_rotate(node);
				}
				else if (balance < -1 && node->get_right()->get_balance() <= 0)
					return left_rotate(node);
				else if (balance < -1 && node->get_right()->get_balance() > 0)
				{
					node->set_right(right_rotate(node->get_right()));
					return left_rotate(node);
				}
				return node;
			}
		};

	}
}

#endif//__ANG_BASE_BINARY_TREE_H__
