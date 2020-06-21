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
#elif !defined __ANG_BASE_VECTOR_H__
#define __ANG_BASE_VECTOR_H__

namespace ang //constants
{
	namespace collections
	{


		template<typename T, template<typename> class A>
		struct vector : protected array_node<T>
		{
		public:
			static_assert(!is_const<T>::value, "T is constant type");
			using type = typename remove_reference<T>::type*;
			using const_type = typename remove_reference<T>::type const*;
			using element_type = typename remove_reference<T>::type;

			using iterator_t = algorithms::forward_iterator<element_type&, array_node<T>>;
			using const_iterator_t = algorithms::forward_iterator<element_type const&, array_node<T>>;
			using reverse_iterator_t = algorithms::backward_iterator<element_type&, array_node<T>>;
			using reverse_const_iterator_t = algorithms::backward_iterator<element_type const&, array_node<T>>;
			using node_type = array_node<T>;

		protected:
			element_type* m_end;

		public: /*constructors and destructor*/
			vector();
			vector(ang::nullptr_t const&);
			vector(wsize sz);
			vector(vector const& other);
			vector(vector && other);
			template<typename IT>
			vector(IT first, IT last);
			template<typename T2, template<typename>class A2>
			vector(vector<T2, A2> const& other);
			template<typename T2>
			vector(array_view<T2> const& ar);
			template<typename T2>
			vector(initializer_list<T2> const& list);
			template<typename T2, wsize N>
			vector(T2(&ar)[N]);
			template<typename T2, wsize N>
			vector(const T2(&ar)[N]);
			~vector();

		public: /*properties*/
			type  get();
			const_type get()const;
			bool is_empty()const;
			type data()const;
			wsize size()const;
			void size(wsize sz);
			wsize capacity()const;
			void capacity(wsize sz, bool save = true);

		public: /*iterators*/
			element_type& front();
			element_type const& front()const;
			element_type& back();
			element_type const& back()const;

			iterator_t begin();
			iterator_t end();
			const_iterator_t begin()const;
			const_iterator_t end()const;

			reverse_iterator_t rbegin();
			reverse_iterator_t rend();
			reverse_const_iterator_t rbegin()const;
			reverse_const_iterator_t rend()const;

		public: /*utilities*/
			void clear();
			void push_front(element_type && value);
			void push_front(element_type const& value);
			void push_back(element_type && value);
			void push_back(element_type const& value);
			iterator_t insert(windex idx, T&& value);
			iterator_t insert(windex idx, T const& value);
			iterator_t insert(iterator_t it, T&& value);
			iterator_t insert(iterator_t it, T const& value);
			bool pop_front(element_type& value);
			bool pop_front();
			bool pop_back(element_type& value);
			bool pop_back();
			iterator_t remove(windex idx);
			iterator_t remove(windex idx, element_type& value);
			iterator_t remove(iterator_t it);
			iterator_t remove(iterator_t it, element_type& value);
			void move(vector& ar);
			template<typename IT>
			void copy(IT first, IT last);
			template<typename T2, template<typename> class A2>
			void copy(vector<T2, A2> const& ar);
			template<typename T2>
			void copy(array_view<T2> const& ar);
			template<typename T2>
			void copy(initializer_list<T2> const& list);
			template<typename T2, wsize N> 
			void copy(T2(&ar)[N]);
			template<typename T2, wsize N>
			void copy(const T2(&ar)[N]);
			template<typename IT>
			void extend(IT first, IT last);
			template<typename T2, template<typename> class A2>
			void extend(vector<T2, A2> const& ar);
			template<typename T2>
			void extend(array_view<T2> const& ar);
			template<typename T2>
			void extend(initializer_list<T2> const& list);
			template<typename T2, wsize N>
			void extend(T2(&ar)[N]);
			template<typename T2, wsize N>
			void extend(const T2(&ar)[N]);

		public: /*operators*/
			vector& operator = (type val);
			vector& operator = (vector const& val);
			vector& operator = (vector && val);
			template<typename T2> 
			vector& operator = (array_view<T2> const& ar);
			template<typename T2>
			vector& operator = (initializer_list<T2> const& list);
			template<typename T2, wsize N>
			vector& operator = (T2(&ar)[N]);
			template<typename T2, wsize N>
			vector& operator = (const T2(&ar)[N]);
			explicit operator type& ();
			explicit operator type ()const;
			operator array_view<T> const&()const;
			template<typename I>
			element_type& operator [](I const& idx);
			template<typename I>
			element_type const& operator [](I const& idx)const;
			element_type** operator & ();
			vector& operator += (element_type && val);
			vector& operator += (element_type const& val);
			vector& operator -= (element_type& val);

			template<typename T2, template<typename> class A2>
			bool operator == (vector<T2, A2> const& items)const;

			template<typename T2, template<typename> class A2>
			bool operator != (vector<T2, A2> const& items)const;

			template<typename T2, template<typename> class A2>
			bool operator == (array<T2, A2> const& items)const;

			template<typename T2, template<typename> class A2>
			bool operator != (array<T2, A2> const& items)const;

			template<typename T2>
			bool operator == (array_view<T2> const& items)const;

			template<typename T2>
			bool operator != (array_view<T2> const& items)const;
		};

	}

	using collections::vector;
}

#endif//__ANG_BASE_ARRAY_H__
