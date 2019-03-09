/*********************************************************************************************************************/
/*   File Name: ang/base/array.h                                                                                     */
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
#elif !defined __ANG_BASE_ARRAY_H__
#define __ANG_BASE_ARRAY_H__

namespace ang //constants
{

	namespace collections
	{
		template<typename T> struct array_view;

		template<typename T> struct array_view {
		public:
			typedef T* type;
			typedef T element_type;

		private:
			element_type* m_data;
			wsize m_size;

		public:
			array_view() : m_size(0), m_data(null) {}
			array_view(wsize size, type val) : m_size(size), m_data(val) {}
			array_view(nullptr_t const&) : m_size(0), m_data(null) {}
			array_view(array_view const& other) : m_size(other.m_size), m_data(other.m_data) {}
			array_view(array_view && other) : m_size(ang::move(other.m_size)), m_data(ang::move(other.m_data)) {}
			template<wsize N>
			array_view(T(&ar)[N]) : m_size(N), m_data(ar) { }
			~array_view() {}

		public: /*getters and setters*/
			type & get() { return m_data; }
			type const& get()const { return m_data; }
			void set(type val, wsize size) { m_data = ang::move(val); m_size = ang::move(size); }
			type data()const { return m_data; }
			wsize size()const { return m_size; }

			type begin()const { 
				static element_type _dummy_data[1];
				return m_data ? m_data : _dummy_data;
			}
			type end()const { 
				return m_data ? m_data + m_size : begin();
			}

		public: /*operators*/
			array_view& operator = (type val) { set(ang::move(val), 1u); return*this; }
			array_view& operator = (array_view const& val) { set(val.m_data, val.m_size); return*this; }
			array_view& operator = (array_view && val) { set(ang::move(val.m_data), ang::move(val.m_size)); return*this; }
			template<wsize N> array_view& operator = (element_type(&ar)[N]) { set(ar, N); return*this; }

			operator type& () { return get(); }
			operator type ()const { return get(); }

			template<typename I>element_type& operator [](I idx) { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return m_data[idx]; }
			template<typename I>element_type const& operator [](I idx)const { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return m_data[idx]; }

			element_type** operator & () { return &get(); }
		};

		template<typename T, wsize N>
		inline array_view<T> to_array(T(&ar)[N]) {
			return array_view<T>(ar);
		}

		template<typename T>
		inline array_view<T> to_array(T* ar, wsize N) {
			return array_view<T>(N, ar);
		}

		template<typename T, wsize _SIZE> struct stack_array {
		public:
			typedef T* type;
			typedef T const* ctype;
			typedef T element_type;
		private:
			static constexpr wsize SIZE = _SIZE;
			T m_data[_SIZE];

		public:
			stack_array() { }
			stack_array(stack_array && other) = default;

			template<wsize _OTHER_SIZE>
			stack_array(const stack_array<T, _OTHER_SIZE>& other) {
				for (windex i = 0; i < min<_OTHER_SIZE, _SIZE>(); ++i) {
					m_data[i] = other.m_data[i];
				}
			}

			template<wsize N>
			stack_array(T(&ar)[N]) {
				for (windex i = 0; i < min<N, _SIZE>(); ++i) {
					m_data[i] = ar[i];
				}
			}

			template<wsize N>
			stack_array(const T(&ar)[N]) {
				for (windex i = 0; i < min<N, _SIZE>(); ++i) {
					m_data[i] = ar[i];
				}
			}

			~stack_array() {}

		public: /*getters and setters*/
			type get() { return &m_data[0]; }
			ctype get()const { return &m_data[0]; }
	
			template<wsize i> T& get() { static_assert(i <SIZE, "array overflow"); return m_data[i]; }
			template<wsize i> T const& get()const { static_assert(i < SIZE, "array overflow"); return m_data[i]; }
			template<wsize i> void set(T&& value) { static_assert(i < SIZE, "array overflow"); m_data[i] = forward<T>(value); }

			void copy(type val, wsize size) {
				for (windex i = 0, c = min(size, _SIZE); i < c; ++i) {
					m_data[i] = val[i];
				}
			}

			type data()const { return (type)m_data; }
			constexpr wsize size()const { return _SIZE; }

			type begin()const { return (type)m_data; }
			type end()const { return (type)m_data + _SIZE; }

		public: /*operators*/
			stack_array& operator = (stack_array && val) = default;
			template<wsize _OTHER_SIZE>
			stack_array& operator = (const stack_array<element_type, _OTHER_SIZE>& other) {
				for (windex i = 0; i < min<_OTHER_SIZE, _SIZE>(); ++i) {
					m_data[i] = other.m_data[i];
				}
				return*this;
			}
			template<wsize N> stack_array& operator = (element_type(&ar)[N]) {
				for (windex i = 0; i < min<N, _SIZE>(); ++i) {
					m_data[i] = ar[i];
				}
				return*this;
			}

			template<wsize N> stack_array& operator = (const element_type(&ar)[N]) {
				for (windex i = 0; i < min<N, _SIZE>(); ++i) {
					m_data[i] = ar[i];
				}
				return*this;
			}

			operator type () { return get(); }
			operator ctype ()const { return get(); }
			operator array_view<T>()const { return array_view<T>(size(), get()); }

			template<typename I>element_type& operator [](I idx) { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return m_data[idx]; }
			template<typename I>element_type const& operator [](I idx)const { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return m_data[idx]; }

			element_type** operator & () { return &get(); }
		};

		template<typename T, template<typename> class allocator = memory::default_allocator>
		struct scope_array {
		public:
			typedef T* type;
			typedef T element_type;

		private:
			element_type* m_data; wsize m_size;
			allocator<element_type> alloc;

		public:
			scope_array() : m_size(0), m_data(null) { }
			scope_array(ang::nullptr_t const&) : scope_array() {}
			scope_array(wsize sz, type val = null) : scope_array() {
				m_size = min(wsize(-1) / size_of<T>(), sz);
				if (m_size > 0) {
					m_data = alloc.allocate(m_size);
					if(val) for (windex i = 0; i < m_size; ++i)
						alloc.template construct<T, T const&>((T*)&m_data[i], val[i]);
					else for (windex i = 0; i < m_size; ++i)
						alloc.template construct<T>((T*)&m_data[i]);
				}
			}			
			scope_array(scope_array const& other) : scope_array() {
				m_size = min(wsize(-1) / size_of<T>(), other.m_size);
				if (m_size > 0) {
					m_data = alloc.allocate(m_size);
					for (windex i = 0; i < m_size; ++i)
						alloc.template construct<T, T const&>((T*)&m_data[i], other.m_data[i]);
				}
			}
			scope_array(scope_array && other) : scope_array() {
				m_size = other.m_size;
				m_data = other.m_data;
				other.m_size = 0;
				other.m_data = null;
			}
			template<typename U, wsize N>scope_array(U(&ar)[N]) : scope_array() {
				m_size = min(wsize(-1) / size_of<T>(), N);
				if (m_size > 0) {
					m_data = alloc.allocate(m_size);
					for (windex i = 0; i < m_size; ++i)
						alloc.template construct<T, U const&>((T*)&m_data[i], ar[i]);
				}
			}
			template<typename U, wsize N>scope_array(const U(&ar)[N]) : scope_array() {
				m_size = min(wsize(-1) / size_of<T>(), N);
				if (m_size > 0) {
					m_data = alloc.allocate(m_size);
					for (windex i = 0; i < m_size; ++i)
						alloc.template construct<T, U const&>((T*)&m_data[i], ar[i]);
				}
			}
			~scope_array() { clear(); }

		public: /*getters and setters*/
			type & get() { return m_data; }
			type const& get()const { return m_data; }
			void set(type val, wsize sz) {
				clear();
				m_size = min(wsize(-1) / size_of<T>(), sz);
				if (m_size > 0) {
					m_data = alloc.allocate(m_size);
					for (windex i = 0; i < m_size; ++i)
						alloc.template construct<T, T const&>((T*)&m_data[i], val[i]);
				}
			}
			void move(scope_array& other) {
				if ((scope_array*)&other == this) return;
				clear();
				m_size = other.m_size;
				m_data = other.m_data;
				other.m_size = 0;
				other.m_data = null;
			}
			void move(array_view<T>& other) {
				clear();
				if (other.size() > 0)
					m_data = alloc.allocate(other.size());
				m_size = other.size();

				for (windex i = 0; i < m_size; ++i)
					alloc.template construct<T, T&&>(&m_data[i], ang::move(other.get()[i]));
				other.set(null, 0);
			}
			template<typename U, template<typename> class allocator2> void copy(scope_array<U, allocator2> const& other) {
				clear();
				if (other.size() > 0)
					m_data = alloc.allocate(other.size());
				m_size = other.size();
				for (windex i = 0; i < m_size; ++i)
					alloc.template construct<T, U const&>(&m_data[i], other[i]);
				other.set(null, 0);
			}
			template<typename U> void copy(array_view<U> const& other) {
				clear();
				if (other.size() > 0)
					m_data = alloc.allocate(other.size());
				m_size = other.size();
				for (windex i = 0; i < m_size; ++i)
					alloc.template construct<T, U const&>(&m_data[i], other[i]);
			}
			template<typename U, wsize N> void copy(U(&ar)[N]) {
				clear(); m_size = N;
				if (m_size > 0) {
					m_data = alloc.allocate(m_size);
					for (windex i = 0; i < m_size; ++i)
						alloc.template construct<T, U const&>((T*)&m_data[i], ar[i]);
				}
			}
			template<typename U, wsize N> void copy(const U(&ar)[N]) {
				clear(); m_size = N;
				if (m_size > 0) {
					m_data = alloc.allocate(m_size);
					for (windex i = 0; i < m_size; ++i)
						alloc.template construct<T, U const&>((T*)&m_data[i], ar[i]);
				}
			}

			type data()const { return m_data; }
			wsize size()const { return m_size; }
			void clear() {
				if (m_data) {
					for (windex i = 0; i < m_size; ++i)
						alloc.template destroy<T>((T*)&m_data[i]);
					alloc.deallocate(m_data);
				}
				m_size = 0;
				m_data = null;
			}
			type allocate(wsize size) {
				clear(); 
				if (size > 0) {
					m_size = size;
					m_data = alloc.allocate(m_size);
					for (index i = 0; i < m_size; ++i)
						alloc.template construct<T, T const&>((T*)&m_data[i], default_value<T>::value);
				}
				return m_data;
			}
			bool is_empty()const { return m_size == 0; }
			type begin()const { return m_data; }
			type end()const { return m_data + m_size; }

		public: /*operators*/
			scope_array& operator = (type val) { set(ang::move(val), 1u); return*this; }
			scope_array& operator = (scope_array const& val) { set(val.m_data, val.m_size); return*this; }
			scope_array& operator = (scope_array && val) { move(val); return*this; }
			template<typename U, wsize N> scope_array& operator = (U(&ar)[N]) { copy(ar); return*this; }
			template<typename U, wsize N> scope_array& operator = (const U(&ar)[N]) { copy(ar); return*this; }

			explicit operator type& () { return get(); }
			explicit operator type ()const { return get(); }
			operator array_view<T>()const { return array_view<T>(size(), get()); }

			template<typename I>element_type& operator [](I const& idx) { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return m_data[idx]; }
			template<typename I>element_type const& operator [](I const& idx)const { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return m_data[idx]; }

			element_type** operator & () { return &get(); }
		};
	}

	using collections::array_view;
	using collections::stack_array;
	using collections::scope_array;
	using collections::to_array;

	namespace algorithms
	{
		template<typename T, wsize N> inline constexpr wsize array_size(const T(&ar)[N]) { return N; }
		template<class T, wsize SIZE> inline constexpr wsize array_size(const stack_array<T, SIZE>&) { return SIZE; }
		template<class T> inline wsize array_size(const array_view<T>& arr) { return arr.size(); }

		template<typename K, typename T, template<typename, typename> class operation = logic_operation_minor>
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
}

#endif//__ANG_BASE_ARRAY_H__
