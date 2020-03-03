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
#elif !defined __ANG_BASE_LIST_H__
#define __ANG_BASE_LIST_H__

namespace ang //constants
{

	namespace collections
	{
		template<typename T>
		struct singly_linked_node
		{
			using type = typename remove_reference<T>::type;
			using self = singly_linked_node<T>;
			using self_ptr = singly_linked_node<T>*;

			singly_linked_node()
				: data()
				, next(null) {
			}
			singly_linked_node(type&& value)
				: data(ang::forward<type>(value))
				, next(null) {
			}
			~singly_linked_node() {
				next = null;
			}

			type data;
			self_ptr next;
		};

		template<typename T>
		struct doubly_linked_node : inherit<doubly_linked_node<T>>
		{
			using type = typename remove_reference<T>::type;
			using self = doubly_linked_node<T>;
			using self_ptr = doubly_linked_node<T>*;

			doubly_linked_node()
				: data()
				, prev(null)
				, next(null) {
			}
			doubly_linked_node(type&& value)
				: data(ang::forward<type>(value))
				, prev(null)
				, next(null) {
			}
			~doubly_linked_node() {
				prev = null;
				next = null;
			}

			void set_next(self_ptr node) {
				next = node;
				if (next) next->prev = this;
			}
			void set_prev(self_ptr node) {
				prev = node;
				if (prev) prev->next = this;
			}

			type data;
			self_ptr prev;
			self_ptr next;
		};

		template<typename T, template<typename> class A = memory::default_allocator> struct list;
		template<typename T, template<typename> class A = memory::default_allocator> struct queue;
		template<typename T, template<typename> class A = memory::default_allocator> struct stack;

	}

	namespace algorithms
	{
		template<typename T>
		struct singly_linked_list_iteration
			: iteration_algorithm<collections::singly_linked_node<T>>
		{
			using type = typename collections::singly_linked_node<T>::type;
			using node_ptr_type = typename iteration_algorithm<collections::singly_linked_node<T>>::node_ptr_type;
			using iteration_callback_type = typename iteration_algorithm<collections::singly_linked_node<T>>::iteration_callback_type;

			singly_linked_list_iteration() {
				vtable = &s_vtable[0];
			}

		private:
			static node_ptr_type begin(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return most_left(node);
			}
			static node_ptr_type end(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return null;
			}
			static node_ptr_type rbegin(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return null;
			}
			static node_ptr_type rend(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return null;
			}
			static node_ptr_type increase(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return node != null ? node->next : null;
			}
			static node_ptr_type decrease(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return null;
			}
			static node_ptr_type most_left(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return null;
			}
			static node_ptr_type most_right(node_ptr_type node, windex* idx = null) {
				if (idx)*idx = 0;
				while (node != null && node->next != null)
					node = node->next;
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

		template<typename T>
		struct doubly_linked_list_iteration
			: iteration_algorithm<collections::doubly_linked_node<T>>
		{
			using type = typename collections::doubly_linked_node<T>::type;
			using node_ptr_type = typename iteration_algorithm<collections::doubly_linked_node<T>>::node_ptr_type;
			using iteration_callback_type = typename iteration_algorithm<collections::doubly_linked_node<T>>::iteration_callback_type;

			doubly_linked_list_iteration() {
				vtable = &s_vtable[0];
			}

		private:
			static node_ptr_type begin(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return most_left(node);
			}
			static node_ptr_type end(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return null;
			}
			static node_ptr_type rbegin(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return most_right(node);
			}
			static node_ptr_type rend(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return null;
			}
			static node_ptr_type increase(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return node != null ? node->next : null;
			}
			static node_ptr_type decrease(node_ptr_type node, windex* idx) {
				if (idx)*idx = 0;
				return node != null ? node->prev : null;
			}
			static node_ptr_type most_left(node_ptr_type node, windex* idx = null) {
				while (node != null && node->prev != null)
					node = node->prev;
				return node;
			}
			static node_ptr_type most_right(node_ptr_type node, windex* idx = null) {
				if (idx)*idx = 0;
				while (node != null && node->next != null)
					node = node->next;
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

		template<typename U, typename T>
		struct node_value<U, collections::singly_linked_node<T> > {
			using type = U;
			using type_ptr = typename remove_reference<U>::type*;
			static type get(collections::singly_linked_node<T>* node, windex) {
				return static_cast<type>(node->data);
			}
			static type_ptr get_ptr(collections::singly_linked_node<T>* node, windex) {
				return static_cast<type_ptr>(&node->data);
			}
		};

		template<typename U, typename T>
		struct node_value<U, collections::doubly_linked_node<T> > {
			using type = U;
			using type_ptr = typename remove_reference<U>::type*;
			static type get(collections::doubly_linked_node<T>* node, windex) {
				return static_cast<type>(node->data);
			}
			static type_ptr get_ptr(collections::doubly_linked_node<T>* node, windex) {
				return static_cast<type_ptr>(&node->data);
			}
		};
	}

	namespace collections
	{
		template<typename T, template<typename> class A>
		struct list 
		{
			static_assert(!is_const<T>::value, "T is constant type");
			using element_type = typename remove_reference<T>::type;
			using node_type = doubly_linked_node<element_type>;
			using node_type_ptr = doubly_linked_node<element_type>*;
			using base_iterator_t = algorithms::base_iterator<node_type>;
			using iterator_t = algorithms::forward_iterator<element_type&, node_type>;
			using const_iterator_t = algorithms::forward_iterator<element_type const&, node_type>;
			using reverse_iterator_t = algorithms::backward_iterator<element_type&, node_type>;
			using reverse_const_iterator_t = algorithms::backward_iterator<element_type const&, node_type>;
			using iteration_type = algorithms::doubly_linked_list_iteration<T>;

		protected:
			node_type_ptr m_first;
			node_type_ptr m_last;
			wsize m_size;

		public:
			list()
				: m_first(null)
				, m_last(null)
				, m_size(0) {
			}
			list(ang::nullptr_t const&)
				: list() {
			}
			list(list const& other)
				: list() {
				for (auto const& value : other) {
					push_back(value);
				}
			}
			list(list && other)
				: list() {
				m_first = other.m_first;
				m_last = other.m_last;
				m_size = other.m_size;
				other.m_first = null;
				other.m_last = null;
				other.m_size = 0;
			}
			template<typename T2>
			list(array_view<T2> const& ar)
				: list() {
				for (T2 const& val : ar) {
					push_back((T)val);
				}
			}
			template<typename T2>
			list(initializer_list<T2> const& items)
				: list() {
				for (T2 const& val : items) {
					push_back(val);
				}
			}
			template<typename T2, wsize N>
			list(T2(&ar)[N]) : list() {
				for (T2 const& val : ar) {
					push_back(val);
				}
			}
			template<typename T2, wsize N>
			list(const T2(&ar)[N]) : list() {
				for (T2 const& val : ar) {
					push_back((T)val);
				}
			}
			~list() {
				clear();
			}

		public: /*properties*/
			bool is_empty()const {
				return m_size == 0;
			}
			wsize size()const {
				return m_size;
			}
			element_type& front() { return m_first->data; }
			element_type const& front()const { return m_first->data; }
			element_type& back() { return m_last->data; }
			element_type const& back()const { return m_last->data; }
		
		public: /*iterators*/
			iterator_t begin(algorithms::iteration_algorithm<node_type> iter = iteration_type()) {
				return iterator_t(iter, m_first, 0);
			}
			iterator_t end(algorithms::iteration_algorithm<node_type> iter = iteration_type()) {
				return iterator_t(iter, null, 0);
			}
			const_iterator_t begin(algorithms::iteration_algorithm<node_type> iter = iteration_type())const {
				return const_iterator_t(iter, const_cast<list*>(this)->m_first, 0);
			}
			const_iterator_t end(algorithms::iteration_algorithm<node_type> iter = iteration_type())const {
				return const_iterator_t(iter, null, 0);
			}
			reverse_iterator_t rbegin(algorithms::iteration_algorithm<node_type> iter = iteration_type()) {
				return reverse_iterator_t(iter, m_last, 0);
			}
			reverse_iterator_t rend(algorithms::iteration_algorithm<node_type> iter = iteration_type()) {
				return reverse_iterator_t(iter, null, 0);
			}
			reverse_const_iterator_t rbegin(algorithms::iteration_algorithm<node_type> = iteration_type())const {
				return reverse_iterator_t(iter, const_cast<list*>(this)->m_last, 0);
			}
			reverse_const_iterator_t rend(algorithms::iteration_algorithm<node_type> iter = iteration_type())const {
				return reverse_iterator_t(iteration_type(), null, 0);
			}

		public: /*utilities*/
			void clear() {
				node_type_ptr to_del, node = m_first;
				while (node != null)
				{
					to_del = node;
					node = node->next;
					A<node_type>::template destroy<node_type>(to_del);
					A<node_type>::deallocate(to_del);
				}
				m_first = null;
				m_last = null;
				m_size = 0;
			}
			void push_back(element_type && value) {
				node_type_ptr node = create_node(ang::forward<element_type>(value));
				if (m_first == null) {
					m_first = m_last = node;
				}
				else {
					m_last->set_next(node);
					m_last = node;
				}
				m_size++;
			}
			void push_back(element_type const& value) {
				node_type_ptr node = create_node(value);
				if (m_first == null) {
					m_first = m_last = node;
				}
				else {
					m_last->set_next(node);
					m_last = node;
				}
				m_size++;
			}
			void push_front(element_type && value) {
				node_type_ptr node = create_node(value);
				if (m_first == null) {
					m_first = m_last = node;
				}
				else {
					m_first->set_prev(node);
					m_first = node;
				}
				m_size++;
			}
			void push_front(element_type const& value) {
				node_type_ptr node = create_node(value);
				if (m_first == null) {
					m_first = m_last = node;
				}
				else {
					m_first->set_prev(node);
					m_first = node;
				}
				m_size++;
			}
			bool pop_front(element_type& value) {
				if (m_first == null)
					return false;
				if (m_first == m_last) {
					value = ang::move(m_first->data);
					A<node_type>::template destroy<node_type>(m_first);
					A<node_type>::deallocate(m_first);
					m_first = m_last = null;
				}
				else {
					node_type_ptr node = m_first;
					m_first = m_first->next;
					m_first->prev = null;
					value = ang::move(node->data);
					A<node_type>::template destroy<node_type>(node);
					A<node_type>::deallocate(node);
				}
				m_size--;
				return true;
			}
			bool pop_front() {
				if (m_first == null)
					return false;
				if (m_first == m_last) {
					A<node_type>::template destroy<node_type>(m_first);
					A<node_type>::deallocate(m_first);
					m_first = m_last = null;
				}
				else {
					node_type_ptr node = m_first;
					m_first = m_first->next;
					m_first->prev = null;
					A<node_type>::template destroy<node_type>(node);
					A<node_type>::deallocate(node);
				}
				m_size--;
				return true;
			}
			bool pop_back(element_type& value) {
				if (m_first == null)
					return false;
				if (m_first == m_last) {
					value = ang::move(m_first->data);
					A<node_type>::template destroy<node_type>(m_first);
					A<node_type>::deallocate(m_first);
					m_first = m_last = null;
				} else {
					node_type_ptr node = m_last;
					m_last = m_last->prev;
					m_last->next = null;
					value = ang::move(node->data);
					A<node_type>::template destroy<node_type>(node);
					A<node_type>::deallocate(node);
				}
				m_size--;
				return true;
			}
			bool pop_back() {
				if (m_first == null)
					return false;
				if (m_first == m_last) {
					A<node_type>::template destroy<node_type>(m_first);
					A<node_type>::deallocate(m_first);
					m_first = m_last = null;
				}
				else {
					node_type_ptr node = m_last;
					m_last = m_last->prev;
					m_last->next = null;
					A<node_type>::template destroy<node_type>(node);
					A<node_type>::deallocate(node);
				}
				m_size--;
				return true;
			}
			base_iterator_t& insert(base_iterator_t& at, element_type && value) {
				if (at.get_node() == m_first) {
					push_front(ang::forward<element_type>(value));
					at = base_iterator_t(at.algorithm(), m_first, 0);
				}		
				if (at.get_node() == null) {
					push_back(ang::forward<element_type>(value));
					at = base_iterator_t(at.algorithm(), m_last, 0);
				}
				else {
					node_type_ptr node = create_node(ang::forward<element_type>(value));
					node->set_prev(at.get_node()->prev);
					node->set_next(at.get_node());
					at = base_iterator_t(at.algorithm(), node, 0);
					m_size++;
				}
				return at;
			}
			base_iterator_t& insert(base_iterator_t& at, element_type const& value) {
				if (at.get_node() == m_first) {
					push_front(value);
					at = base_iterator_t(at.algorithm(), m_first, 0);
				}
				else if (at.get_node() == null) {
					push_back(value);
					at = base_iterator_t(at.algorithm(), m_last, 0);
				}
				else {
					node_type_ptr node = create_node(value);
					node->set_prev(at.get_node()->prev);
					node->set_next(at.get_node());
					at = base_iterator_t(at.algorithm(), node, 0);
					m_size++;
				}
				return at;
			}
			bool remove(base_iterator_t& at, element_type & value) {
				if (at.get_node() == null)
					return false;

				else if (at.get_node() == m_first) {
					if (!pop_front(value))
						return false;
					at = base_iterator_t(at.algorithm(), m_first, 0);
				}
				else if (at.get_node() == m_last) {
					if (!pop_back(value))
						return false;
					at = base_iterator_t(at.algorithm(), null, 0);
				}
				else {
					node_type_ptr node = at.get_node();
					at = base_iterator_t(at.algorithm(), node->next, 0);
					node->prev->set_next(node->next);
					value = ang::move(node->data);
					A<node_type>::template destroy<node_type>(node);
					A<node_type>::deallocate(node);
					m_size--;		
				}
				return true;
			}	
			bool remove(base_iterator_t& at) {
				if (at.get_node() == null)
					return false;
				else if (at.get_node() == m_first) {
					if (!pop_front())
						return false;
					at = base_iterator_t(at.algorithm(), m_first, 0);
				}
				else if (at.get_node() == m_last) {
					if (!pop_back())
						return false;
					at = base_iterator_t(at.algorithm(), null, 0);
				}
				else {
					node_type_ptr node = at.get_node();
					at = base_iterator_t(at.algorithm(), node->next, 0);
					node->prev->set_next(node->next);
					A<node_type>::template destroy<node_type>(node);
					A<node_type>::deallocate(node);
					m_size--;
				}
				return true;
			}
			void move(list& items) {
				if (&items == this)
					return;
				clear();
				m_size = items.m_size;
				m_first = items.m_first;
				m_last = items.m_last;
				items.m_size = 0;
				items.m_first = null;
				items.m_last = null;
			}
			template<typename T2, template<typename> class A2>
			void copy(list<T2, A2> const& items) {
				clear();
				for (T2 const& val : items) {
					push_back((T)val);
				}
			}
			template<typename T2>
			void copy(array_view<T2> const& ar) {
				clear();
				for (T2 const& val : ar) {
					push_back((T)val);
				}
			}
			template<typename T2>
			void copy(initializer_list<T2> const& items) {
				clear();
				for (T2 const& val : items) {
					push_back((T)val);
				}
			}
			template<typename T2, wsize N>
			void copy(T2(&ar)[N]) {
				clear();
				for (T2 const& val : ar) {
					push_back((T)val);
				}
			}
			template<typename T2, wsize N>
			void copy(const T2(&ar)[N]) {
				clear();
				for (T2 const& val : ar) {
					push_back((T)val);
				}
			}

			template<typename T2, template<typename> class A2>
			void exten(list<T2, A2> const& items) {
				for (T2 const& val : items) {
					push_back((T)val);
				}
			}
			template<typename T2>
			void exten(array_view<T2> const& ar) {
				for (T2 const& val : ar) {
					push_back((T)val);
				}
			}
			template<typename T2>
			void exten(initializer_list<T2> const& items) {
				for (T2 const& val : items) {
					push_back((T)val);
				}
			}
			template<typename T2, wsize N>
			void exten(T2(&ar)[N]) {
				for (T2 const& val : ar) {
					push_back((T)val);
				}
			}
			template<typename T2, wsize N>
			void exten(const T2(&ar)[N]) {
				for (T2 const& val : ar) {
					push_back((T)val);
				}
			}

		protected: /*utilities*/
			template<typename... Ts> node_type_ptr create_node(Ts...val) {
				node_type_ptr node = A<node_type>::allocate(1);
				A<node_type>::template construct<node_type, Ts...>(node, ang::forward<Ts>(val)...);
				return node;
			}

		public: /*operators*/
			list& operator = (list const& val) {
				copy(val);
				return*this;
			}
			list& operator = (list && val) {
				move(val);
				return*this;
			}
			template<typename T2>
			list& operator = (array_view<T2> const& ar) {
				copy(ar);
				return*this;
			}
			template<typename T2>
			list& operator = (initializer_list<T2> const& items) {
				copy(items);
				return*this;
			}
			template<typename T2, wsize N>
			list& operator = (T2(&ar)[N]) {
				copy(ar);
				return*this;
			}
			template<typename T2, wsize N>
			list& operator = (const T2(&ar)[N]) {
				copy(ar);
				return*this;
			}
			list& operator += (element_type && val) {
				push_back(forward<element_type>(val));
				return*this;
			}
			list& operator += (element_type const& val) {
				push_back(val);
				return*this;
			}
		};


		template<typename T, template<typename> class A>
		struct stack
		{
			static_assert(!is_const<T>::value, "T is constant type");
			using element_type = typename remove_reference<T>::type;
			using node_type = singly_linked_node<element_type>;
			using node_type_ptr = singly_linked_node<element_type>*;
			using base_iterator_t = algorithms::base_iterator<node_type>;
			using iterator_t = algorithms::forward_iterator<element_type&, node_type>;
			using const_iterator_t = algorithms::forward_iterator<element_type const&, node_type>;
			using iteration_type = algorithms::singly_linked_list_iteration<T>;

		protected:
			node_type_ptr m_first;
			node_type_ptr m_last;
			wsize m_size;

		public:
			stack()
				: m_first(null)
				, m_last(null)
				, m_size(0) {
			}
			stack(ang::nullptr_t const&)
				: stack() {
			}
			stack(stack const& other)
				: stack() {
				for (auto const& value : other) {
					push(value);
				}
			}
			stack(stack && other)
				: stack() {
				m_first = other.m_first;
				m_last = other.m_last;
				m_size = other.m_size;
				other.m_first = null;
				other.m_last = null;
				other.m_size = null;
			}
			template<typename T2>
			stack(array_view<T2> const& ar)
				: stack() {
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			template<typename T2>
			stack(initializer_list<T2> const& items)
				: stack() {
				for (T2 const& val : items) {
					push(val);
				}
			}
			template<typename T2, wsize N>
			stack(T2(&ar)[N]) : stack() {
				for (T2 const& val : ar) {
					push(val);
				}
			}
			template<typename T2, wsize N>
			stack(const T2(&ar)[N]) : stack() {
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			~stack() {
				clear();
			}

		public: /*properties*/
			bool is_empty()const {
				return m_size == 0;
			}
			wsize size()const {
				return m_size;
			}
			iterator_t top() {
				return iterator_t(iteration_type(), m_first, 0);
			}
			const_iterator_t top()const {
				return const_iterator_t(iteration_type(), m_first, 0);
			}

		public: /*iterators*/
			iterator_t begin(algorithms::iteration_algorithm<node_type> iter = iteration_type()) {
				return iterator_t(iter, m_first, 0);
			}
			iterator_t end(algorithms::iteration_algorithm<node_type> iter = iteration_type()) {
				return iterator_t(iter, null, 0);
			}
			const_iterator_t begin(algorithms::iteration_algorithm<node_type> iter = iteration_type())const {
				return const_iterator_t(iter, const_cast<stack*>(this)->m_first, 0);
			}
			const_iterator_t end(algorithms::iteration_algorithm<node_type> iter = iteration_type())const {
				return const_iterator_t(iter, null, 0);
			}

		public: /*utilities*/
			void clear() {
				node_type_ptr to_del, node = m_first;
				while (node != null)
				{
					to_del = node;
					node = node->next;
					A<node_type>::template destroy<node_type>(to_del);
					A<node_type>::deallocate(to_del);
				}
				m_first = null;
				m_last = null;
				m_size = 0;
			}
			void push(element_type && value) {
				node_type_ptr node = create_node(ang::forward<element_type>(value));
				if (m_first == null) {
					m_first = m_last = node;
				}
				else {
					m_last->set_next(node);
					m_last = node;
				}
				m_size++;
			}
			void push(element_type const& value) {
				node_type_ptr node = create_node(value);
				if (m_first == null) {
					m_first = m_last = node;
				}
				else {
					m_last->set_next(node);
					m_last = node;
				}
				m_size++;
			}
			bool pop(element_type& value) {
				if (m_first == null)
					return false;
				if (m_first == m_last) {
					value = ang::move(m_first->data);
					A<node_type>::template destroy<node_type>(m_first);
					A<node_type>::deallocate(m_first);
					m_first = m_last = null;
				}
				else {
					node_type_ptr node = m_first;
					m_first = m_first->next;
					value = ang::move(node->data);
					A<node_type>::template destroy<node_type>(node);
					A<node_type>::deallocate(node);
				}
				m_size--;
				return true;
			}
			bool pop() {
				if (m_first == null)
					return false;
				if (m_first == m_last) {
					A<node_type>::template destroy<node_type>(m_first);
					A<node_type>::deallocate(m_first);
					m_first = m_last = null;
				}
				else {
					node_type_ptr node = m_first;
					m_first = m_first->next;
					A<node_type>::template destroy<node_type>(node);
					A<node_type>::deallocate(node);
				}
				m_size--;
				return true;
			}
			void move(stack& items) {
				if (&items == this)
					return;
				clear();
				m_size = items.m_size;
				m_first = items.m_first;
				m_last = items.m_last;
				items.m_size = 0;
				items.m_first = null;
				items.m_last = null;
			}
			template<typename T2, template<typename> class A2>
			void copy(stack<T2, A2> const& items) {
				clear();
				for (T2 const& val : items) {
					push((T)val);
				}
			}
			template<typename T2>
			void copy(array_view<T2> const& ar) {
				clear();
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			template<typename T2>
			void copy(initializer_list<T2> const& items) {
				clear();
				for (T2 const& val : items) {
					push((T)val);
				}
			}
			template<typename T2, wsize N>
			void copy(T2(&ar)[N]) {
				clear();
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			template<typename T2, wsize N>
			void copy(const T2(&ar)[N]) {
				clear();
				for (T2 const& val : ar) {
					push((T)val);
				}
			}

			template<typename T2, template<typename> class A2>
			void exten(stack<T2, A2> const& items) {
				for (T2 const& val : items) {
					push((T)val);
				}
			}
			template<typename T2>
			void exten(array_view<T2> const& ar) {
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			template<typename T2>
			void exten(initializer_list<T2> const& items) {
				for (T2 const& val : items) {
					push((T)val);
				}
			}
			template<typename T2, wsize N>
			void exten(T2(&ar)[N]) {
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			template<typename T2, wsize N>
			void exten(const T2(&ar)[N]) {
				for (T2 const& val : ar) {
					push((T)val);
				}
			}

		protected: /*utilities*/
			template<typename... Ts> node_type_ptr create_node(Ts...val) {
				node_type_ptr node = A<node_type>::allocate(1);
				A<node_type>::template construct<node_type, Ts...>(node, ang::forward<Ts>(val)...);
				return node;
			}

		public: /*operators*/
			stack& operator = (stack const& val) {
				copy(val);
				return*this;
			}
			stack& operator = (stack && val) {
				move(val);
				return*this;
			}
			template<typename T2>
			stack& operator = (array_view<T2> const& ar) {
				copy(ar);
				return*this;
			}
			template<typename T2>
			stack& operator = (initializer_list<T2> const& items) {
				copy(items);
				return*this;
			}
			template<typename T2, wsize N>
			stack& operator = (T2(&ar)[N]) {
				copy(ar);
				return*this;
			}
			template<typename T2, wsize N>
			stack& operator = (const T2(&ar)[N]) {
				copy(ar);
				return*this;
			}
			stack& operator += (element_type && val) {
				push(forward<element_type>(val));
				return*this;
			}
			stack& operator += (element_type const& val) {
				push(val);
				return*this;
			}
			stack& operator -= (element_type& val) {
				pop(val);
				return*this;
			}
		};


		template<typename T, template<typename> class A>
		struct queue
		{
			static_assert(!is_const<T>::value, "T is constant type");
			using element_type = typename remove_reference<T>::type;
			using node_type = doubly_linked_node<element_type>;
			using node_type_ptr = doubly_linked_node<element_type>*;
			using base_iterator_t = algorithms::base_iterator<node_type>;
			using iterator_t = algorithms::forward_iterator<element_type&, node_type>;
			using const_iterator_t = algorithms::forward_iterator<element_type const&, node_type>;
			using iteration_type = algorithms::doubly_linked_list_iteration<T>;

		protected:
			node_type_ptr m_first;
			node_type_ptr m_last;
			wsize m_size;

		public:
			queue()
				: m_first(null)
				, m_last(null)
				, m_size(0) {
			}
			queue(ang::nullptr_t const&)
				: queue() {
			}
			queue(queue const& other)
				: queue() {
				for (auto const& value : other) {
					push(value);
				}
			}
			queue(queue && other)
				: queue() {
				m_first = other.m_first;
				m_last = other.m_last;
				m_size = other.m_size;
				other.m_first = null;
				other.m_last = null;
				other.m_size = null;
			}
			template<typename T2>
			queue(array_view<T2> const& ar)
				: queue() {
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			template<typename T2>
			queue(initializer_list<T2> const& items)
				: queue() {
				for (T2 const& val : items) {
					push(val);
				}
			}
			template<typename T2, wsize N>
			queue(T2(&ar)[N]) : queue() {
				for (T2 const& val : ar) {
					push(val);
				}
			}
			template<typename T2, wsize N>
			queue(const T2(&ar)[N]) : queue() {
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			~queue() {
				clear();
			}

		public: /*properties*/
			bool is_empty()const {
				return m_size == 0;
			}
			wsize size()const {
				return m_size;
			}
			iterator_t front() {
				return iterator_t(iteration_type(), m_first, 0);
			}
			const_iterator_t front()const {
				return const_iterator_t(iteration_type(), m_first, 0);
			}
			iterator_t back() {
				return iterator_t(iteration_type(), m_last, 0);
			}
			const_iterator_t back()const {
				return const_iterator_t(iteration_type(), m_last, 0);
			}

		public: /*iterators*/
			iterator_t begin(algorithms::iteration_algorithm<node_type> iter = iteration_type()) {
				return iterator_t(iter, m_first, 0);
			}
			iterator_t end(algorithms::iteration_algorithm<node_type> iter = iteration_type()) {
				return iterator_t(iter, null, 0);
			}
			const_iterator_t begin(algorithms::iteration_algorithm<node_type> iter = iteration_type())const {
				return const_iterator_t(iter, const_cast<queue*>(this)->m_first, 0);
			}
			const_iterator_t end(algorithms::iteration_algorithm<node_type> iter = iteration_type())const {
				return const_iterator_t(iter, null, 0);
			}

		public: /*utilities*/
			void clear() {
				node_type_ptr to_del, node = m_first;
				while (node != null)
				{
					to_del = node;
					node = node->next;
					A<node_type>::template destroy<node_type>(to_del);
					A<node_type>::deallocate(to_del);
				}
				m_first = null;
				m_last = null;
				m_size = 0;
			}
			void push(element_type && value) {
				node_type_ptr node = create_node(ang::forward<element_type>(value));
				if (m_first == null) {
					m_first = m_last = node;
				}
				else {
					m_last->set_next(node);
					m_last = node;
				}
				m_size++;
			}
			void push(element_type const& value) {
				node_type_ptr node = create_node(value);
				if (m_first == null) {
					m_first = m_last = node;
				}
				else {
					m_last->set_next(node);
					m_last = node;
				}
				m_size++;
			}
			bool pop(element_type& value) {
				if (m_first == null)
					return false;
				if (m_first == m_last) {
					value = ang::move(m_first->data);
					A<node_type>::template destroy<node_type>(m_first);
					A<node_type>::deallocate(m_first);
					m_first = m_last = null;
				}
				else {
					node_type_ptr node = m_first;
					m_first = m_first->next;
					value = ang::move(node->data);
					A<node_type>::template destroy<node_type>(node);
					A<node_type>::deallocate(node);
				}
				m_size--;
				return true;
			}
			bool pop() {
				if (m_first == null)
					return false;
				if (m_first == m_last) {
					A<node_type>::template destroy<node_type>(m_first);
					A<node_type>::deallocate(m_first);
					m_first = m_last = null;
				}
				else {
					node_type_ptr node = m_first;
					m_first = m_first->next;
					A<node_type>::template destroy<node_type>(node);
					A<node_type>::deallocate(node);
				}
				m_size--;
				return true;
			}
			void move(queue& items) {
				if (&items == this)
					return;
				clear();
				m_size = items.m_size;
				m_first = items.m_first;
				m_last = items.m_last;
				items.m_size = 0;
				items.m_first = null;
				items.m_last = null;
			}
			template<typename T2, template<typename> class A2>
			void copy(queue<T2, A2> const& items) {
				clear();
				for (T2 const& val : items) {
					push((T)val);
				}
			}
			template<typename T2>
			void copy(array_view<T2> const& ar) {
				clear();
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			template<typename T2>
			void copy(initializer_list<T2> const& items) {
				clear();
				for (T2 const& val : items) {
					push((T)val);
				}
			}
			template<typename T2, wsize N>
			void copy(T2(&ar)[N]) {
				clear();
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			template<typename T2, wsize N>
			void copy(const T2(&ar)[N]) {
				clear();
				for (T2 const& val : ar) {
					push((T)val);
				}
			}

			template<typename T2, template<typename> class A2>
			void exten(queue<T2, A2> const& items) {
				for (T2 const& val : items) {
					push((T)val);
				}
			}
			template<typename T2>
			void exten(array_view<T2> const& ar) {
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			template<typename T2>
			void exten(initializer_list<T2> const& items) {
				for (T2 const& val : items) {
					push((T)val);
				}
			}
			template<typename T2, wsize N>
			void exten(T2(&ar)[N]) {
				for (T2 const& val : ar) {
					push((T)val);
				}
			}
			template<typename T2, wsize N>
			void exten(const T2(&ar)[N]) {
				for (T2 const& val : ar) {
					push((T)val);
				}
			}

		protected: /*utilities*/
			template<typename... Ts> node_type_ptr create_node(Ts...val) {
				node_type_ptr node = A<node_type>::allocate(1);
				A<node_type>::template construct<node_type, Ts...>(node, ang::forward<Ts>(val)...);
				return node;
			}

		public: /*operators*/
			queue& operator = (queue const& val) {
				copy(val);
				return*this;
			}
			queue& operator = (queue && val) {
				move(val);
				return*this;
			}
			template<typename T2>
			queue& operator = (array_view<T2> const& ar) {
				copy(ar);
				return*this;
			}
			template<typename T2>
			queue& operator = (initializer_list<T2> const& items) {
				copy(items);
				return*this;
			}
			template<typename T2, wsize N>
			queue& operator = (T2(&ar)[N]) {
				copy(ar);
				return*this;
			}
			template<typename T2, wsize N>
			queue& operator = (const T2(&ar)[N]) {
				copy(ar);
				return*this;
			}
			queue& operator += (element_type && val) {
				push(forward<element_type>(val));
				return*this;
			}
			queue& operator += (element_type const& val) {
				push(val);
				return*this;
			}
			queue& operator -= (element_type& val) {
				pop(val);
				return*this;
			}
		};

	}

	using collections::list;
	using collections::stack;
	using collections::queue;

}


#endif//__ANG_BASE_LIST_H__
