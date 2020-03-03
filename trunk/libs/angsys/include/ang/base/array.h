/*********************************************************************************************************************/
/*   File Name: ang/base/array.h                                                                                   */
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
#elif !defined __ANG_BASE_ARRAY_H__
#define __ANG_BASE_ARRAY_H__

namespace ang //constants
{

	namespace collections
	{
		template<typename T>
		struct array_node {
			using type = typename remove_reference<T>::type*;
			static algorithms::iteration_algorithm<array_node<T>> const& iterator();

			array_node(type first, type last)
				: m_first(first)
				, m_last(last) {
			}
			array_node(array_node && node)
				: m_first(ang::move(node.m_first))
				, m_last(ang::move(node.m_last)) {
			}
			array_node(array_node const& node)
				: m_first(node.m_first)
				, m_last(node.m_last) {
			}
			array_node& operator = (array_node && node) {
				if (this != &node) {
					m_first = ang::move(node.m_first);
					m_last = ang::move(node.m_last);
				}
				return*this;
			}
			array_node& operator = (array_node const& node) {
				if (this != &node) {
					m_first = node.m_first;
					m_last = node.m_last;
				}
				return*this;
			}

			type m_first;
			type m_last;
		};	
	}

	namespace algorithms
	{
		template<typename T>
		struct array_iteration
			: iteration_algorithm<collections::array_node<T>>
		{
			using type =  typename collections::array_node<T>::type;
			using node_ptr_type = typename iteration_algorithm<collections::array_node<T>>::node_ptr_type;
			using iteration_callback_type = typename iteration_algorithm<collections::array_node<T>>::iteration_callback_type;

			array_iteration() {
				vtable = &s_vtable[0];
			}

		private:
			static node_ptr_type begin(node_ptr_type node, windex* idx) {
				return most_left(node, idx);
			}
			static node_ptr_type end(node_ptr_type node, windex* idx) {
				if (node && idx) {
					*idx = reinterpret_cast<windex>(node->m_last);
					return node;
				}
				return null;
			}
			static node_ptr_type rbegin(node_ptr_type node, windex* idx) {
				return most_right(node, idx);
			}
			static node_ptr_type rend(node_ptr_type node, windex* idx) {
				if (node && idx) {
					*idx = reinterpret_cast<windex>(node->m_first - 1);
					return node;
				}
				return null;
			}
			static node_ptr_type increase(node_ptr_type node, windex* idx) {
				if (node && idx) {
					(*reinterpret_cast<type*>(idx))++;
					return node;
				}
				return null;
			}
			static node_ptr_type decrease(node_ptr_type node, windex* idx) {
				if (node && idx) {
					(*reinterpret_cast<type*>(idx))--;
					return node;
				}
				return null;
			}
			static node_ptr_type most_left(node_ptr_type node, windex* idx) {
				if (node && idx) {
					*idx = reinterpret_cast<windex>(node->m_first);
					return node;
				}
				return null;
			}
			static node_ptr_type most_right(node_ptr_type node, windex* idx) {
				if (node && idx) {
					*idx = reinterpret_cast<windex>(max<type, type>(node->m_first, node->m_last - 1));
					return node;
				}
				return null;
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
		struct node_value<U, collections::array_node<T> > {
			using type = U;
			using type_ptr = typename remove_reference<U>::type*;
			static U get(collections::array_node<T>* node, windex at) {	
				return *static_cast<typename remove_reference<U>::type*>(reinterpret_cast<typename collections::array_node<T>::type>(at));
			}
			static typename remove_reference<U>::type* get_ptr(collections::array_node<T>* node, windex at) {
				return static_cast<typename remove_reference<U>::type*>(reinterpret_cast<typename collections::array_node<T>::type>(at));
			}
		};
	}

	namespace collections
	{
		template<typename T>
		struct array_view : protected array_node<T>
		{	
			using type = typename remove_reference<T>::type*;
			using element_type = typename remove_reference<typename remove_const<T>::type>::type;
			using iterator_t = algorithms::forward_iterator<typename remove_reference<T>::type&, array_node<T>>;
			using reverse_iterator_t = algorithms::backward_iterator<typename remove_reference<T>::type&, array_node<T>>;
			using node_type = array_node<T>;
			using iteration_type = algorithms::array_iteration<T>;

		public: /*constructors and destructor*/
			array_view();
			array_view(type first, type last);
			template<typename U>
			array_view(algorithms::forward_iterator<U, array_node<T>> first, algorithms::forward_iterator<U, array_node<T>> last);
			array_view(nullptr_t const&);
			array_view(array_view const& other);
			array_view(array_view && other);
			template<wsize N>
			array_view(T(&ar)[N]);
			~array_view();

		public: /*properties*/
			bool is_empty()const;
			type get();
			type const get()const;
			void set(type first, type last);
			type data()const;
			wsize size()const;

		public: /*iterators*/
			iterator_t begin(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;
			iterator_t end(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;
			reverse_iterator_t rbegin(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;
			reverse_iterator_t rend(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;

		public: /*utilities*/
			void clear();

		public: /*operators*/
			array_view& operator = (nullptr_t const&);
			array_view& operator = (type val);
			array_view& operator = (array_view const& val);
			array_view& operator = (array_view && val);
			template<wsize N>
			array_view& operator = (element_type(&ar)[N]);
			operator type& ();
			operator type ()const;
			template<typename I>
			element_type& operator [](I idx);
			template<typename I>
			element_type const& operator [](I idx)const;
			element_type** operator & ();
			element_type*const* operator & ()const;
		};

		template<typename T, wsize N>
		inline array_view<T> to_array(T(&ar)[N]) {
			return array_view<T>(ar);
		}

		template<typename T>
		inline array_view<T> to_array(T* first, T* last) {
			return array_view<T>(first, last);
		}

		template<typename T, typename U>
		inline array_view<T> to_array(algorithms::forward_iterator<U, array_node<T>> first, algorithms::forward_iterator<U, array_node<T>> last) {
			return array_view<T>(first, last);
		}
		//algorithms::forward_iterator<element_type&, array_node<T>>;

		template<typename T, wsize SIZE>
		struct stack_array : protected array_node<T>
		{
			static_assert(!is_const<T>::value, "T is constant type");
			using type = typename remove_reference<T>::type*;
			using const_type = typename remove_reference<T>::type const*;
			using element_type = typename remove_reference<T>::type;
			using iterator_t = algorithms::forward_iterator<element_type&, array_node<T>>;
			using const_iterator_t = algorithms::forward_iterator<element_type const&, array_node<T>>;
			using reverse_iterator_t = algorithms::backward_iterator<element_type&, array_node<T>>;
			using reverse_const_iterator_t = algorithms::backward_iterator<element_type const&, array_node<T>>;
			using node_type = array_node<T>;
			using iteration_type = algorithms::array_iteration<T>;

		private:
			element_type m_data[SIZE];

		public: /*constructors and destructor*/
			stack_array();
			stack_array(stack_array && other) = default;
			template<wsize OTHER_SIZE>
			stack_array(const stack_array<T, OTHER_SIZE>& other);
			template<typename T2, wsize N>
			stack_array(T2(&ar)[N]);
			~stack_array();

		public: /*properties*/
			type get();
			const_type get()const;
			constexpr wsize size()const;
			type data()const;

		public: /*iterators*/
			iterator_t begin(algorithms::iteration_algorithm<array_node<T>> = iteration_type());
			iterator_t end(algorithms::iteration_algorithm<array_node<T>> = iteration_type());
			const_iterator_t begin(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;
			const_iterator_t end(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;

			reverse_iterator_t rbegin(algorithms::iteration_algorithm<array_node<T>> = iteration_type());
			reverse_iterator_t rend(algorithms::iteration_algorithm<array_node<T>> = iteration_type());
			reverse_const_iterator_t rbegin(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;
			reverse_const_iterator_t rend(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;

		public: /*utilities*/
			template<typename IT>
			void copy(IT first, IT last);

		public: /*operators*/
			stack_array& operator = (stack_array && val) = default;
			template<wsize OTHER_SIZE>
			stack_array& operator = (const stack_array<element_type, OTHER_SIZE>& other);
			template<typename T2, wsize N>
			stack_array& operator = (T2(&ar)[N]);
			operator type();
			operator const_type()const;
			operator array_view<T>()const;
			template<typename I>
			element_type& operator [](I idx);
			template<typename I>
			element_type const& operator [](I idx)const;
			element_type** operator & ();
		};

		template<typename T, template<typename> class A>
		struct array : protected array_node<T>
		{
			static_assert(!is_const<T>::value, "T is constant type");
			using type = typename remove_reference<T>::type*;
			using const_type = typename remove_reference<T>::type const*;
			using element_type = typename remove_reference<T>::type;
			using iterator_t = algorithms::forward_iterator<element_type&, array_node<T>>;
			using const_iterator_t = algorithms::forward_iterator<element_type const&, array_node<T>>;
			using reverse_iterator_t = algorithms::backward_iterator<element_type&, array_node<T>>;
			using reverse_const_iterator_t = algorithms::backward_iterator<element_type const&, array_node<T>>;
			using node_type = array_node<T>;
			using iteration_type = algorithms::array_iteration<T>;

		public: /*constructors and destructor*/
			array();
			array(ang::nullptr_t const&);
			array(wsize sz);
			array(array const& other);
			array(array && other);
			template<typename T2>
			array(array_view<T2> const& ar);
			template<typename T2>
			array(initializer_list<T2> const& list);
			template<typename T2, wsize N>
			array(T2(&ar)[N]);
			template<typename IT>
			array(IT first, IT last);
			~array();

		public: /*properties*/
			type get();
			const_type get()const;
			bool is_empty()const;
			type data()const;
			wsize size()const;
			void size(wsize sz);
			array_view<T> view()const;
			array_view<const T> const_view()const;

		public: /*iterators*/
			iterator_t begin(algorithms::iteration_algorithm<array_node<T>> = iteration_type());
			iterator_t end(algorithms::iteration_algorithm<array_node<T>> = iteration_type());
			const_iterator_t begin(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;
			const_iterator_t end(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;

			reverse_iterator_t rbegin(algorithms::iteration_algorithm<array_node<T>> = iteration_type());
			reverse_iterator_t rend(algorithms::iteration_algorithm<array_node<T>> = iteration_type());
			reverse_const_iterator_t rbegin(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;
			reverse_const_iterator_t rend(algorithms::iteration_algorithm<array_node<T>> = iteration_type())const;

		public: /*utilities*/
			void clear();
			void move(array& other);
			template<typename IT>
			void copy(IT first, IT last);
			template<typename T2, template<typename> class A2> 
			void copy(array<T2, A2> const& other);
			template<typename T2> 
			void copy(array_view<T2> const& other);
			template<typename T2>
			void copy(initializer_list<T2> const& list);
			template<typename T2, wsize N> 
			void copy(T2(&ar)[N]);

		public: /*operators*/
			array& operator = (type first);
			array& operator = (nullptr_t const& first);
			array& operator = (array const& val);
			array& operator = (array && val);
			template<typename T2>
			array& operator = (array_view<T2> const& ar);
			template<typename T2>
			array& operator = (initializer_list<T2> const& list);
			template<typename T2, wsize N> 
			array& operator = (T2(&ar)[N]);
			explicit operator type& ();
			explicit operator type ()const;
			operator array_view<T>()const;
			operator array_view<const T>()const;
			template<typename I>
			element_type& operator [](I const& idx);
			template<typename I>
			element_type const& operator [](I const& idx)const;
			element_type** operator & ();
		};
	}

	namespace algorithms
	{
		template<class T, wsize SIZE> inline constexpr wsize array_size(const collections::stack_array<T, SIZE>&) { return SIZE; }

		template<class T> inline wsize array_size(const collections::array_view<T>& arr) { return arr.size(); }

		template<typename K, typename T, template<typename, typename> class operation = less>
		wsize binary_search(K const& key, array_view<T> const& vec) {
			long64 first = 0, last = (long64)vec.size() - 1;
			long64 mid;
			while (first <= last) {
				mid = (first + last) / 2;
				if (operation<T, K>::operate(vec[mid], key)) first = mid + 1;
				else if (operation<K, T>::operate(key, vec[mid])) last = mid - 1;
				else return (wsize)mid;
			}
			return (wsize)invalid_index;
		}
	}

	using collections::to_array;
}

#endif//__ANG_BASE_ARRAY_H__
