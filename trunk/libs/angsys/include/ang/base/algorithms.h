/*********************************************************************************************************************/
/*   File Name: ang/base/text.h                                                                                      */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_ALGORITHMS_H__
#define __ANG_BASE_ALGORITHMS_H__

namespace ang
{
	namespace algorithms
	{
		template<typename K> struct hash;
		template<typename N> struct base_iterator;
		template<typename T, typename N> struct iterator;
		template<typename T, typename N> struct forward_iterator;
		template<typename T, typename N> struct backward_iterator;
		template<typename T, typename N> struct node_value;
		template<typename N> struct iteration_algorithm;

		template<typename T, wsize N> inline constexpr wsize array_size(const T(&ar)[N]) { return N; }
		
		template<typename K, typename T, wsize N, template<typename, typename> class operation = less>
		wsize binary_search(K const& key, const T(&vec)[N]) {
			long64 first = 0, last = N - 1;
			long64 mid;
			while (first <= last) {
				mid = (first + last) / 2;
				if (operation<T, K>::operate(vec[mid], key)) first = mid + 1;
				else if (operation<K, T>::operate(key, vec[mid])) last = mid - 1;
				else return (wsize)mid;
			}
			return (wsize)invalid_index;
		}

		template<typename N>
		struct iteration_algorithm
		{
			using node_ptr_type = N*;
			using iteration_callback_type = node_ptr_type(*)(node_ptr_type, wsize*);

			iteration_algorithm(iteration_algorithm const& algorithm) {
				vtable = algorithm.vtable;
			}
			iteration_algorithm& operator = (iteration_algorithm const& algorithm) {
				vtable = algorithm.vtable;
				return*this;
			}
			iteration_algorithm() {
				vtable = null;
			}

			node_ptr_type begin(node_ptr_type node, windex* idx)const {
				return vtable[0](node, idx);
			}
			node_ptr_type end(node_ptr_type node, windex* idx)const {
				return vtable[1](node, idx);
			}
			node_ptr_type rbegin(node_ptr_type node, windex* idx)const {
				return vtable[2](node, idx);
			}
			node_ptr_type rend(node_ptr_type node, windex* idx)const {
				return vtable[3](node, idx);
			}
			node_ptr_type increase(node_ptr_type node, windex* idx)const {
				return vtable[4](node, idx);
			}
			node_ptr_type decrease(node_ptr_type node, windex* idx)const {
				return vtable[5](node, idx);
			}
			node_ptr_type most_left(node_ptr_type node, windex* idx)const {
				return vtable[6](node, idx);
			}
			node_ptr_type most_right(node_ptr_type node, windex* idx)const {
				return vtable[7](node, idx);
			}

		protected:
			iteration_callback_type const* vtable;
		};

		template<typename T, typename N>
		struct node_value {
			using type = T;
			using tyupe_ptr = typename remove_reference<T>::type*;
			static type get(N* node, windex at);
			static tyupe_ptr get_ptr(N* node, windex at);
		};

		template<typename N>
		struct base_iterator {
			base_iterator()
				: node(null)
				, index(0)
				, m_algorithm() {
			}
			base_iterator(iteration_algorithm<N> const& iter, N* node, wsize idx)
				: node(node)
				, index(idx)
				, m_algorithm(iter) {
			}
			base_iterator(base_iterator && iter)
				: node(ang::move(iter.node))
				, index(ang::move(iter.index))
				, m_algorithm(iter.m_algorithm) {
			}
			base_iterator(base_iterator const& iter)
				: node(iter.node)
				, index(iter.index)
				, m_algorithm(iter.m_algorithm) {
			}

			base_iterator& operator = (base_iterator && it) {
				if (this != &it) {
					node = ang::move(it.node);
					index = ang::move(it.index);
					m_algorithm = it.m_algorithm;
				}
				return *this;
			}
			base_iterator& operator = (base_iterator const& it) {
				if (this != &it) {
					node = it.node;
					index = it.index;
					m_algorithm = it.m_algorithm;
				}
				return *this;
			}

			bool operator == (base_iterator const& it) {
				return (node == it.node) && (index == it.index);
			}
			bool operator != (base_iterator const& it) {
				return (node != it.node) || (index != it.index);
			}
			bool is_valid()const {
				return node != null;
			}
			iteration_algorithm<N> const& algorithm()const {
				return m_algorithm;
			}
			N* get_node()const {
				return node;
			}
			ulong64 get_index()const {
				return index;
			}
			operator bool()const {
				return is_valid();
			}
			bool operator!()const {
				return !is_valid();
			}
		protected:
			N* node;
			ulong64 index;
			iteration_algorithm<N> m_algorithm;
		};


		template<typename T, typename N>
		struct iterator : base_iterator<N> {
			template<typename...U> iterator(U&&... it)
				: base_iterator<N>(ang::forward<U>(it)...) {
			}
			iterator& operator = (base_iterator<N> const& it) {
				if (this != &it) {
					node = it.node;
					index = it.index;
					m_algorithm = it.m_algorithm;
				}
				return *this;
			}
			iterator& operator = (iterator const& it) {
				if (this != &it) {
					node = it.node;
					index = it.index;
					m_algorithm = it.m_algorithm;
				}
				return *this;
			}
			iterator& operator = (nullptr_t const&) {
				node = null;
				index = 0;
				return*this;
			}
			T operator*()const {
				return node_value<T, N>::get(node, index);
			}
			operator typename node_value<T, N>::type_ptr ()const {
				return node_value<T, N>::get_ptr(node, index);
			}
			typename typename node_value<T, N>::type_ptr operator->()const {
				return node_value<T, N>::get_ptr(node, index);
			}
		};

		template<typename T, typename N>
		struct forward_iterator : iterator<T, N> {
			template<typename...U> forward_iterator(U&&... it)
				: iterator<T, N>(ang::forward<U>(it)...) {
			}
			forward_iterator& operator = (base_iterator<N> const& it) {
				if (this != &it) {
					node = it.get_node();
					index = it.get_index();
					m_algorithm = it.algorithm();
				}
				return *this;
			}
			forward_iterator& operator = (forward_iterator const& it) {
				if (this != &it) {
					node = it.get_node();
					index = it.get_index();
					m_algorithm = it.algorithm();
				}
				return *this;
			}
			forward_iterator& operator = (nullptr_t const&) {
				node = null;
				index = 0;
				return*this;
			}
			forward_iterator& operator ++() {
				node = m_algorithm.increase(node, (windex*)&index);
				return*this;
			}
			forward_iterator operator ++(int) {
				forward_iterator it = *this;
				node = m_algorithm.increase(node, (windex*)&index);
				return ang::move(it);
			}
			forward_iterator& operator --() {
				node = m_algorithm.decrerase(node, (windex*)&index);
				return*this;
			}
			forward_iterator operator --(int) {
				forward_iterator it = *this;
				node = m_algorithm.decrerase(node, (windex*)&index);
				return ang::move(it);
			}
		};

		template<typename T, typename N>
		struct backward_iterator : iterator<T, N> {
			template<typename...U> backward_iterator(U&&... it)
				: iterator<T, N>(forward<U>(it)...) {
			}
			backward_iterator& operator = (base_iterator<N> const& it) {
				if (this != &it) {
					node = it.node;
					index = it.index;
					m_algorithm = it.m_algorithm;
				}
				return *this;
			}
			backward_iterator& operator = (backward_iterator const& it) {
				if (this != &iter) {
					node = it.node;
					index = it.index;
					m_algorithm = it.m_algorithm;
				}
				return *this;
			}
			backward_iterator& operator = (nullptr_t const&) {
				node = null;
				index = 0;
				return*this;
			}
			backward_iterator& operator ++() {
				node = algorithm().decrease(node, &index);
				return*this;
			}
			backward_iterator operator ++(int) {
				backward_iterator it = node;
				node = algorithm().decrease(node, &index);
				return ang::move(it);
			}
			backward_iterator& operator --() {
				node = algorithm().increase(node, &index);
				return*this;
			}
			backward_iterator operator --(int) {
				backward_iterator it = node;
				node = algorithm().increase(node, &index);
				return ang::move(it);
			}
		};

		template<typename K>
		struct hash {
			using type = typename remove_const<typename remove_reference<K>::type>::type;
			static_assert(is_value_type<type>::value);
			static ulong64 make(type const& value) {
				union { type k; ulong64 val; } key;
				key.val = 0;
				key.k = value;
				return (ulong64)((2475825 + key.val + 1));
			}
			ulong64 operator()(type const& value)const {
				return make(value);
			}
		};

		wsize LINK hash_table_get_next_size(wsize);
	}

}

#endif /*__ANG_BASE_ALGORITHMS_H__*/