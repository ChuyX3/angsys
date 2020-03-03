/*********************************************************************************************************************/
/*   File Name: coffe/collections/btree.h                                                                             */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: Implements list object                                                                      */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Copyright (C) coffe sys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __COFFE_BTREE_H__
#define __COFFE_BTREE_H__

#include <coffe.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined COFFE_DYNAMIC_LIBRARY

#ifdef COFFE_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//COFFE_EXPORTS
#else//#elif defined COFFE_STATIC_LIBRARY
#define LINK
#endif//COFFE_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace coffe
{
	namespace collections
	{
		template<typename T, wsize N = 10> struct btree_node;
		template<typename T, wsize N = 10> using btree_set_node = btree_node<T, N>;
		template<typename K, typename T, wsize N = 10> using btree_map_node = btree_node<pair<K, T>, N>;
		template<typename T, wsize N = 10, template<typename> class allocator = memory::default_allocator> class btree_set_object;
		template<typename K, typename T, wsize N = 10, template<typename> class allocator = memory::default_allocator> class btree_map_object;
		template<typename T, wsize N = 10, template<typename> class allocator = memory::default_allocator> using btree_set = object_wrapper<btree_set_object<T, N, allocator>>;
		template<typename K, typename T, wsize N = 10, template<typename> class allocator = memory::default_allocator> using btree_map = object_wrapper<btree_map_object<K, T, N, allocator>>;


		template<typename T, wsize N>
		struct btree_node
		{
			//typedef K key_t;
			typedef T type;
			//typedef collections::pair<K, T> pair_t;
			typedef btree_node<T, N> self_type, *self_ptr_type;

			btree_node(T key) { m_data = coffe::move(key); }

			inline type const& key(windex i)const {
				throw_exception_if(i >= (2 * N), error_code::array_overflow);
				return m_data[i];
			}

			inline type& value(windex i) {
				throw_exception_if(i >= (2 * N), error_code::array_overflow);
				return m_data[i];
			}
			inline type const& value(windex i)const {
				throw_exception_if(i >= (2 * N), error_code::array_overflow);
				return m_data[i];
			}
			inline void value(windex i, type val) {
				throw_exception_if(i >= (2 * N), error_code::array_overflow);
				m_data[i] = coffe::move(val);
			}

			inline self_ptr_type& parent() {
				return m_parent;
			}
			inline self_ptr_type const& parent()const {
				return m_parent;
			}
			inline void parent(self_ptr_type val) {
				m_parent = coffe::move(val);
				if (m_parent == null)
					m_node_index = 0;
			}

			inline self_ptr_type child(windex i)const {
				throw_exception_if(i > (2 * N), error_code::array_overflow);
				return m_children[i];
			}
			inline void child(windex i, self_ptr_type val) {
				throw_exception_if(i > (2 * N), error_code::array_overflow);
				m_children[i] = coffe::move(val);
				if (m_children[i]) {
					m_children[i]->m_node_index = i;
					m_children[i]->m_parent = this;
				}
			}

			inline int node_index()const { return this ? m_node_index : 0; }
			inline int width()const { return this ? m_width : 0; }
			inline void width(int n) { m_width = coffe::move(n); }

			stack_array<type, N * 2> m_data;
			self_ptr_type m_parent = null;
			stack_array<self_ptr_type, N * 2 + 1> m_children;
			int m_node_index = 0;
			int m_width = 0;

		};

		template<typename K, typename T, wsize N>
		struct btree_node<pair<K, T>, N>
		{
			//typedef K key_t;
			typedef K key_type;
			typedef T value_type;
			typedef pair<K, T> pair_type;
			typedef btree_node<T, N> self_type, *self_ptr_type;

			btree_node() {  }

			inline key_type const& key(windex i)const {
				throw_exception_if(i >= (2 * N), error_code::array_overflow);
				return m_data[i].key;
			}
			inline void key(windex i, key_type val) {
				throw_exception_if(i >= (2 * N), error_code::array_overflow);
				m_data[i].key = coffe::move(val);
			}
			inline value_type& value(windex i) {
				throw_exception_if(i >= (2 * N), error_code::array_overflow);
				return m_data[i].value;
			}
			inline value_type const& value(windex i)const {
				throw_exception_if(i >= (2 * N), error_code::array_overflow);
				return m_data[i].value;
			}
			inline void value(windex i, value_type val) {
				throw_exception_if(i >= (2 * N), error_code::array_overflow);
				m_data[i].value = coffe::move(val);
			}

			inline self_ptr_type& parent() {
				return m_parent;
			}
			inline self_ptr_type const& parent()const {
				return m_parent;
			}
			inline void parent(self_ptr_type val) {
				m_parent = coffe::move(val);
				if (m_parent == null)
					m_node_index = 0;
			}

			inline self_ptr_type child(windex i)const {
				throw_exception_if(i > (2 * N), error_code::array_overflow);
				return m_children[i];
			}
			inline void child(windex i, self_ptr_type val) {
				throw_exception_if(i > (2 * N), error_code::array_overflow);
				m_children[i] = coffe::move(val);
				if (m_children[i]) {
					m_children[i]->m_node_index = i;
					m_children[i]->m_parent = this;
				}
			}

			inline int node_index()const { return this ? m_node_index : 0; }
			inline int width()const { return this ? m_width : 0; }
			inline void width(int n) { m_width = coffe::move(n); }

			stack_array<pair_type, N * 2> m_data;
			self_ptr_type m_parent = null;
			stack_array<self_ptr_type, N * 2 + 1> m_children;
			int m_node_index = 0;
			int m_width = 0;

		};

	}

	namespace algorithms
	{
		template<typename T, wsize N>
		class btree_inorder_iteration : public iteration_algorithm<collections::btree_node<T, N>>
		{
		public:
			btree_inorder_iteration() {
				iteration_algorithm<collections::btree_node<T, N>>::begin = &begin;
				iteration_algorithm<collections::btree_node<T, N>>::end = &end;
				iteration_algorithm<collections::btree_node<T, N>>::rbegin = &begin;
				iteration_algorithm<collections::btree_node<T, N>>::rend = &end;
				iteration_algorithm<collections::btree_node<T, N>>::increase = &increase;
				iteration_algorithm<collections::btree_node<T, N>>::decrease = &decrease;
				//iteration_algorithm<collections::btree_node<T, N>>::most_left = &most_left;
				//iteration_algorithm<collections::btree_node<T, N>>::most_right = &most_right;
			}

			inline static collections::btree_node<T, N>* begin(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* end(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* rbegin(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* rend(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* increase(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* decrease(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* most_left(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* most_right(collections::btree_node<T, N>*, wsize&);
		};

		template<typename T, wsize N>
		class btree_preorder_iteration : public iteration_algorithm<collections::btree_node<T, N>>
		{
		public:
			btree_preorder_iteration() {
				iteration_algorithm<collections::btree_node<T, N>>::begin = &begin;
				iteration_algorithm<collections::btree_node<T, N>>::end = &end;
				iteration_algorithm<collections::btree_node<T, N>>::rbegin = &begin;
				iteration_algorithm<collections::btree_node<T, N>>::rend = &end;
				iteration_algorithm<collections::btree_node<T, N>>::increase = &increase;
				iteration_algorithm<collections::btree_node<T, N>>::decrease = &decrease;
				//iteration_algorithm<collections::btree_node<T, N>>::most_left = &most_left;
				//iteration_algorithm<collections::btree_node<T, N>>::most_right = &most_right;
			}

			inline static collections::btree_node<T, N>* begin(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* end(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* rbegin(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* rend(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* increase(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* decrease(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* most_left(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* most_right(collections::btree_node<T, N>*, wsize&);
		};

		template<typename T, wsize N>
		class btree_postorder_iteration : public iteration_algorithm<collections::btree_node<T, N>>
		{
		public:
			btree_postorder_iteration() {
				iteration_algorithm<collections::btree_node<T, N>>::begin = &begin;
				iteration_algorithm<collections::btree_node<T, N>>::end = &end;
				iteration_algorithm<collections::btree_node<T, N>>::rbegin = &begin;
				iteration_algorithm<collections::btree_node<T, N>>::rend = &end;
				iteration_algorithm<collections::btree_node<T, N>>::increase = &increase;
				iteration_algorithm<collections::btree_node<T, N>>::decrease = &decrease;
				//iteration_algorithm<collections::btree_node<T, N>>::most_left = &most_left;
				//iteration_algorithm<collections::btree_node<T, N>>::most_right = &most_right;
			}

			inline static collections::btree_node<T, N>* begin(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* end(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* rbegin(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* rend(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* increase(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* decrease(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* most_left(collections::btree_node<T, N>*, wsize&);
			inline static collections::btree_node<T, N>* most_right(collections::btree_node<T, N>*, wsize&);
		};
	}

	namespace collections
	{
		/******************************************************************/
		/* template class btree_map_object :                             */
		/*  -> implements the buffer memory of binary_map object        */
		/******************************************************************/
		template<typename K, typename T, wsize N, template<typename> class allocator>
		class btree_map_object final
			: public smart<btree_map_object<K, T, N, allocator>, imap<K, T>>
		{
		public:
			typedef T										value_type;
			typedef K										key_type;
			typedef pair<K, T>								pair_type;
			typedef btree_map_object<K, T, N, allocator>	self_type;
			typedef ienum<pair<K, T>>						ienum_type;
			typedef imap<K, T>								imap_type;
			typedef iterator<pair<K, T >>					iterator_type;
			typedef const_iterator<pair<K, T >>				const_iterator_type;
			typedef base_iterator<pair<K, T >>				base_iterator_type;
			typedef forward_iterator<pair<K, T >>			forward_iterator_type;
			typedef const_forward_iterator<pair<K, T >>		const_forward_iterator_type;
			typedef backward_iterator<pair<K, T >>			backward_iterator_type;
			typedef const_backward_iterator<pair<K, T >>	const_backward_iterator_type;


		protected:
			typedef btree_map_node<K, T, N> node_type;
			typedef btree_map_node<K, T, N> *node_ptr_type;
			wsize m_count;
			node_ptr_type m_root;
			allocator<node_type> m_alloc;
			algorithms::iteration_algorithm<node_type> m_algorithm;

		public:
			inline btree_map_object();
			inline btree_map_object(std::initializer_list<pair<K, T>> list);
			inline btree_map_object(const std::nullptr_t&);
			inline btree_map_object(btree_map_object&& ar);
			inline btree_map_object(const btree_map_object& ar);
			inline btree_map_object(const btree_map_object* ar);
			inline btree_map_object(const ienum_type* store);

		private:
			virtual ~btree_map_object();

		protected:
			inline void dispose()override;

		public: //methods
			inline bool is_empty()const;
			inline void clear();
			inline bool move(btree_map_object&);
			inline void iteration_method(iteration_method_t);

		public: //ienum overrides
			inline wsize counter()const override;
			inline pair_type& at(const base_iterator_type& it) override;
			inline bool increase(base_iterator_type&)const override;
			inline bool increase(base_iterator_type&, int offset)const override;
			inline bool decrease(base_iterator_type&)const override;
			inline bool decrease(base_iterator_type&, int offset)const override;

			inline forward_iterator_type begin() override;
			inline forward_iterator_type end() override;
			inline const_forward_iterator_type begin()const override;
			inline const_forward_iterator_type end()const override;
			inline forward_iterator_type last() override;
			inline const_forward_iterator_type last()const override;

			inline backward_iterator_type rbegin() override;
			inline backward_iterator_type rend() override;
			inline const_backward_iterator_type rbegin()const override;
			inline const_backward_iterator_type rend()const override;

		public: //imap overrides
			inline bool copy(const ienum_type*) override;
			inline void extend(const ienum_type*) override;
			inline iterator_type insert(K, T) override;
			inline iterator_type insert(pair<K, T>) override;
			inline iterator_type update(K, T) override;
			inline iterator_type update(pair<K, T>) override;
			inline bool remove(K const&) override;
			inline bool remove(K const&, T&) override;
			inline bool remove(base_iterator_type it) override;
			inline bool remove(base_iterator_type it, T&) override;
			inline bool has_key(const K&)const override;
			inline iterator_type find(const K&) override;
			inline const_iterator_type find(const K&)const override;

		public: //overrides
			COFFE_DECLARE_INTERFACE();

		protected:

		};
	}

}

#ifdef  LINK
#undef  LINK
#endif//LINK

#include<coffe/collections/inline/btree.inl>

#endif//__COFFE_BTREE_H__
