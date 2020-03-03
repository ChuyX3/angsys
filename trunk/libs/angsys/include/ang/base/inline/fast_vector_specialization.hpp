/*********************************************************************************************************************/
/*   File Name: coffe/base/vector.h                                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) coffe sys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __COFFE_BASE_H__
#error coffe/base/base.h is not included
#else

namespace coffe //constants
{
#
	namespace collections
	{
		template<>
		struct fast_vector<MY_TYPE, MY_ALLOCATOR>
		{
		public:
			using type = typename remove_reference<MY_TYPE>::type*;
			using const_type = typename remove_reference<MY_TYPE>::type const*;
			using element_type = typename remove_reference<MY_TYPE>::type;
			using iterator_t = type;
			using const_iterator_t = const_type;

		private:
			element_type* m_data;
			wsize m_size;
			wsize m_capacity;

		public: /*constructors and destructor*/
			fast_vector()
				: m_data(null)
				, m_size(0)
				, m_capacity(0) {
			}
			fast_vector(coffe::nullptr_t const&) 
				: fast_vector() {
			}
			fast_vector(wsize sz)
				: fast_vector() {
				size(sz);
			}
			fast_vector(type val, wsize sz) : fast_vector() {
				capacity(sz, false);
				for (wsize i = 0; i < sz; i++) {
					push(val[i]);
				}
			}
			fast_vector(fast_vector const& other)
				: fast_vector() {
				capacity(other.size(), false);
				for (auto const& value : other) {
					push(value);
				}
			}
			fast_vector(fast_vector && other)
				: fast_vector() {
				m_size = other.m_size;
				m_capacity = other.m_capacity;
				m_data = other.m_data;
				other.m_size = 0;
				other.m_capacity = 0;
				other.m_data = null;
			}
			template<typename T2>
			fast_vector(array_view<T2> const& ar)
				: fast_vector() {
				capacity(ar.size(), false);
				for (T2 const& val : ar) {
					push((MY_TYPE)val);
				}
			}
			template<typename T2>
			fast_vector(initializer_list<T2> const& list)
				: fast_vector() {
				capacity(list.size(), false);
				for (T2 const& val : list) {
					push((MY_TYPE)val);
				}
			}
			template<typename T2, wsize N>
			fast_vector(T2(&ar)[N]) : fast_vector() {
				capacity(N, false);
				for (T2 const& val : ar) {
					push((MY_TYPE)val);
				}
			}
			template<typename T2, wsize N>
			fast_vector(const T2(&ar)[N]) : fast_vector() {
				capacity(N, false);
				for (T2 const& val : ar) {
					push((MY_TYPE)val);
				}
			}
			~fast_vector() {
				clear();
			}

		public: /*properties*/
			type  get() { 
				return m_data;
			}
			const_type get()const { 
				return m_data;
			}
			bool is_empty()const {
				return m_size == 0;
			}
			type data()const {
				return m_data;
			}
			wsize size()const {
				return m_size;
			}
			void size(wsize sz) {
				if (sz < m_capacity) {
					m_size = sz;
					return;
				}
				clear();
				wsize cp = 8;//minimum capacity
				while (cp < sz) cp *= 2;
				m_size = sz;
				m_capacity = cp;
				m_data = MY_ALLOCATOR<element_type>::allocate(cp);
			}
			wsize capacity()const { 
				return m_capacity;
			}
			void capacity(wsize size, bool save = true) {
				if (size < m_capacity) {
					return;
				}
				wsize sz = 16;//minimum capacity
				while (sz < size)
					sz *= 2;
				type new_data = MY_ALLOCATOR<element_type>::allocate(sz);
				if (save) {
					for (windex i = 0; i < m_size; ++i) {
						new_data[i] = coffe::move(m_data[i]);
					}
				}
				else {
					m_size = 0;
				}
				if (m_data)
					MY_ALLOCATOR<element_type>::deallocate(m_data);
				m_capacity = sz;
				m_data = new_data;
			}

		public: /*iterators*/
			iterator_t begin() {
				return m_data;
			}
			iterator_t end() {
				return m_data + m_size;
			}
			const_iterator_t begin()const {
				return m_data;
			}
			const_iterator_t end()const {
				return m_data + m_size;
			}

		public: /*utilities*/
			void clear() {
				if (m_data) {
					MY_ALLOCATOR<element_type>::deallocate(m_data);
				}
				m_size = 0;
				m_capacity = 0;
				m_data = null;
			}
			void push(element_type && value) {
				if (size() == capacity())
					capacity(size() + 1);
				m_data[m_size++] = coffe::forward<element_type>(value);
			}
			void push(element_type const& value) {
				if (size() == capacity())
					capacity(size() + 1);
				m_data[m_size++] = value;
			}
			bool pop(element_type& value) {
				if (size() == 0)
					return false;
				value = coffe::move(m_data[m_size]);
				m_size--;
			}
			bool pop() {
				if (size() == 0)
					return false;
				m_size--;
			}
			void move(fast_vector& ar) {
				if ((fast_vector*)&ar == this) return;
				clear();
				m_size = ar.m_size;
				m_capacity = ar.m_capacity;
				m_data = ar.m_data;
				ar.m_size = 0;
				ar.m_capacity = 0;
				ar.m_data = null;
			}
			void copy(type ar, wsize sz) {
				capacity(sz, false);
				for (wsize i = 0; i < sz && ar; i++) {
					push(ar[i]);
				}
			}
			template<typename T2, template<typename> class A2>
			void copy(fast_vector<T2, A2> const& ar) {
				capacity(ar.size(), false);
				for (T2 const& val : ar) {
					push((MY_TYPE)val);
				}
			}
			template<typename T2>
			void copy(array_view<T2> const& ar) {
				capacity(ar.size(), false);
				for (T2 const& val : ar) {
					push((MY_TYPE)val);
				}
			}
			template<typename T2>
			void copy(initializer_list<T2> const& list) {
				capacity(list.size(), false);
				for (T2 const& val : list) {
					push((MY_TYPE)val);
				}
			}
			template<typename T2, wsize N> 
			void copy(T2(&ar)[N]) {
				capacity(N, false);
				for (T2 const& val : ar) {
					push((MY_TYPE)val);
				}
			}
			template<typename T2, wsize N>
			void copy(const T2(&ar)[N]) {
				capacity(N, false);
				for (T2 const& val : ar) {
					push((MY_TYPE)val);
				}
			}

		public: /*operators*/
			fast_vector& operator = (type val) {
				copy(coffe::move(val), 1u);
				return*this;
			}
			fast_vector& operator = (fast_vector const& val) {
				copy(val.m_data, val.m_size);
				return*this; 
			}
			fast_vector& operator = (fast_vector && val) {
				move(val);
				return*this; 
			}
			template<typename T2> 
			fast_vector& operator = (array_view<T2> const& ar) {
				copy(ar);
				return*this;
			}
			template<typename T2>
			fast_vector& operator = (initializer_list<T2> const& list) {
				copy(list);
				return*this;
			}
			template<typename T2, wsize N>
			fast_vector& operator = (T2(&ar)[N]) {
				copy(ar);
				return*this;
			}
			template<typename T2, wsize N> 
			fast_vector& operator = (const T2(&ar)[N]) {
				copy(ar);
				return*this;
			}
			explicit operator type () { 
				return get();
			}
			explicit operator const_type ()const {
				return get();
			}
			operator array_view<MY_TYPE>()const { 
				return array_view<MY_TYPE>(data(), size());
			}
			template<typename I>
			element_type& operator [](I const& idx) {
				static_assert(is_integer_value<I>::value, "no integer value is no accepted");
				return m_data[idx];
			}
			template<typename I>
			element_type const& operator [](I const& idx)const {
				static_assert(is_integer_value<I>::value, "no integer value is no accepted");
				return m_data[idx];
			}
			element_type** operator & () {
				return &m_data;
			}
		};



	}

	using collections::fast_vector;

}

#endif//__COFFE_BASE_ARRAY_H__
