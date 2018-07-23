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
			element_type* _data;
			wsize _size;
			static element_type _dummy_data[1];

		public:
			array_view() : _size(0), _data(null) {}
			array_view(wsize size, type val) : _size(size), _data(val) {}
			array_view(nullptr_t const&) : _size(0), _data(null) {}
			array_view(array_view const& other) : _size(other._size), _data(other._data) {}
			array_view(array_view && other) : _size(ang::move(other._size)), _data(ang::move(other._data)) {}
			template<wsize N>
			array_view(T(&ar)[N]) : _size(N), _data(ar) { }
			~array_view() {}

		public: /*getters and setters*/
			type & get() { return _data; }
			type const& get()const { return _data; }
			void set(type val, wsize size) { _data = ang::move(val); _size = ang::move(size); }
			type data()const { return _data; }
			wsize size()const { return _size; }

			type begin()const { 
				return _data ? _data : _dummy_data;
			}
			type end()const { 
				return _data ? _data + _size : _dummy_data;
			}

		public: /*operators*/
			array_view& operator = (type val) { set(ang::move(val), 1u); return*this; }
			array_view& operator = (array_view const& val) { set(val._data, val._size); return*this; }
			array_view& operator = (array_view && val) { set(ang::move(val._data), ang::move(val._size)); return*this; }
			template<wsize N> array_view& operator = (element_type(&ar)[N]) { set(ar, N); return*this; }

			operator type& () { return get(); }
			operator type ()const { return get(); }

			template<typename I>element_type& operator [](I idx) { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return _data[idx]; }
			template<typename I>element_type const& operator [](I idx)const { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return _data[idx]; }

			element_type** operator & () { return &get(); }
		};

		template<typename T> T array_view<T>::_dummy_data[1];

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
			T _data[_SIZE];

		public:
			stack_array() { }
			stack_array(stack_array && other) = default;

			template<wsize _OTHER_SIZE>
			stack_array(const stack_array<T, _OTHER_SIZE>& other) {
				for (windex i = 0; i < min<_OTHER_SIZE, _SIZE>(); ++i) {
					_data[i] = other._data[i];
				}
			}

			template<wsize N>
			stack_array(T(&ar)[N]) {
				for (windex i = 0; i < min<N, _SIZE>(); ++i) {
					_data[i] = ar[i];
				}
			}

			template<wsize N>
			stack_array(const T(&ar)[N]) {
				for (windex i = 0; i < min<N, _SIZE>(); ++i) {
					_data[i] = ar[i];
				}
			}

			~stack_array() {}

		public: /*getters and setters*/
			type get() { return &_data[0]; }
			ctype get()const { return &_data[0]; }

			void copy(type val, wsize size) {
				for (windex i = 0, c = min(size, _SIZE); i < c; ++i) {
					_data[i] = val[i];
				}
			}

			type data()const { return (type)_data; }
			constexpr wsize size()const { return _SIZE; }

			type begin()const { return (type)_data; }
			type end()const { return (type)_data + _SIZE; }

		public: /*operators*/
			stack_array& operator = (stack_array && val) = default;
			template<wsize _OTHER_SIZE>
			stack_array& operator = (const stack_array<element_type, _OTHER_SIZE>& other) {
				for (windex i = 0; i < min<_OTHER_SIZE, _SIZE>(); ++i) {
					_data[i] = other._data[i];
				}
				return*this;
			}
			template<wsize N> stack_array& operator = (element_type(&ar)[N]) {
				for (windex i = 0; i < min<N, _SIZE>(); ++i) {
					_data[i] = ar[i];
				}
				return*this;
			}

			template<wsize N> stack_array& operator = (const element_type(&ar)[N]) {
				for (windex i = 0; i < min<N, _SIZE>(); ++i) {
					_data[i] = ar[i];
				}
				return*this;
			}

			operator type () { return get(); }
			operator ctype ()const { return get(); }
			operator array_view<T>()const { return array_view<T>(size(), get()); }

			template<typename I>element_type& operator [](I idx) { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return _data[idx]; }
			template<typename I>element_type const& operator [](I idx)const { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return _data[idx]; }

			element_type** operator & () { return &get(); }
		};

		template<typename T, template<typename> class allocator = memory::default_allocator>
		struct scope_array {
		public:
			typedef T* type;
			typedef T element_type;

		private:
			element_type* _data; wsize _size;
			allocator<element_type> alloc;

		public:
			scope_array() : _size(0), _data(null) { }
			scope_array(ang::nullptr_t const&) : scope_array() {}
			scope_array(wsize sz, type val = null) : scope_array() {
				_size = min(wsize(-1) / size_of<T>(), sz);
				if (_size > 0) {
					_data = alloc.allocate(_size);
					if(val) for (windex i = 0; i < _size; ++i)
						alloc.template construct<T, T const&>((T*)&_data[i], val[i]);
					else for (windex i = 0; i < _size; ++i)
						alloc.template construct<T>((T*)&_data[i]);
				}
			}			
			scope_array(scope_array const& other) : scope_array() {
				_size = min(wsize(-1) / size_of<T>(), other._size);
				if (_size > 0) {
					_data = alloc.allocate(_size);
					for (windex i = 0; i < _size; ++i)
						alloc.template construct<T, T const&>((T*)&_data[i], other._data[i]);
				}
			}
			scope_array(scope_array && other) : scope_array() {
				_size = other._size;
				_data = other._data;
				other._size = 0;
				other._data = null;
			}
			template<typename U, wsize N>scope_array(U(&ar)[N]) : scope_array() {
				_size = min(wsize(-1) / size_of<T>(), N);
				if (_size > 0) {
					_data = alloc.allocate(_size);
					for (windex i = 0; i < _size; ++i)
						alloc.template construct<T, U const&>((T*)&_data[i], ar[i]);
				}
			}
			template<typename U, wsize N>scope_array(const U(&ar)[N]) : scope_array() {
				_size = min(wsize(-1) / size_of<T>(), N);
				if (_size > 0) {
					_data = alloc.allocate(_size);
					for (windex i = 0; i < _size; ++i)
						alloc.template construct<T, U const&>((T*)&_data[i], ar[i]);
				}
			}
			~scope_array() { clear(); }

		public: /*getters and setters*/
			type & get() { return _data; }
			type const& get()const { return _data; }
			void set(type val, wsize sz) {
				clear();
				_size = min(wsize(-1) / size_of<T>(), sz);
				if (_size > 0) {
					_data = alloc.allocate(_size);
					for (windex i = 0; i < _size; ++i)
						alloc.template construct<T, T const&>((T*)&_data[i], val[i]);
				}
			}
			void move(scope_array& other) {
				if ((scope_array*)&other == this) return;
				clear();
				_size = other._size;
				_data = other._data;
				other._size = 0;
				other._data = null;
			}
			void move(array_view<T>& other) {
				clear();
				if (other.size() > 0)
					_data = alloc.allocate(other.size());
				_size = other.size();

				for (windex i = 0; i < _size; ++i)
					alloc.template construct<T, T&&>(&_data[i], ang::move(other.get()[i]));
				other.set(null, 0);
			}
			template<typename U, template<typename> class allocator2> void copy(scope_array<U, allocator2> const& other) {
				clear();
				if (other.size() > 0)
					_data = alloc.allocate(other.size());
				_size = other.size();
				for (windex i = 0; i < _size; ++i)
					alloc.template construct<T, U const&>(&_data[i], other[i]);
				other.set(null, 0);
			}
			template<typename U> void copy(array_view<U> const& other) {
				clear();
				if (other.size() > 0)
					_data = alloc.allocate(other.size());
				_size = other.size();
				for (windex i = 0; i < _size; ++i)
					alloc.template construct<T, U const&>(&_data[i], other[i]);
			}
			template<typename U, wsize N> void copy(U(&ar)[N]) {
				clear(); _size = N;
				if (_size > 0) {
					_data = alloc.allocate(_size);
					for (windex i = 0; i < _size; ++i)
						alloc.template construct<T, U const&>((T*)&_data[i], ar[i]);
				}
			}
			template<typename U, wsize N> void copy(const U(&ar)[N]) {
				clear(); _size = N;
				if (_size > 0) {
					_data = alloc.allocate(_size);
					for (windex i = 0; i < _size; ++i)
						alloc.template construct<T, U const&>((T*)&_data[i], ar[i]);
				}
			}

			type data()const { return _data; }
			wsize size()const { return _size; }
			void clear() {
				if (_data) {
					for (windex i = 0; i < _size; ++i)
						alloc.template destroy<T>((T*)&_data[i]);
					alloc.deallocate(_data);
				}
				_size = 0;
				_data = null;
			}
			type allocate(wsize size) {
				clear(); 
				if (size > 0) {
					_size = size;
					_data = alloc.allocate(_size);
					for (index i = 0; i < _size; ++i)
						alloc.template construct<T, T const&>((T*)&_data[i], default_value<T>::value);
				}
				return _data;
			}
			bool is_empty()const { return _size == 0; }
			type begin()const { return _data; }
			type end()const { return _data + _size; }

		public: /*operators*/
			scope_array& operator = (type val) { set(ang::move(val), 1u); return*this; }
			scope_array& operator = (scope_array const& val) { set(val._data, val._size); return*this; }
			scope_array& operator = (scope_array && val) { move(val); return*this; }
			template<typename U, wsize N> scope_array& operator = (U(&ar)[N]) { copy(ar); return*this; }
			template<typename U, wsize N> scope_array& operator = (const U(&ar)[N]) { copy(ar); return*this; }

			explicit operator type& () { return get(); }
			explicit operator type ()const { return get(); }
			operator array_view<T>()const { return array_view<T>(size(), get()); }

			template<typename I>element_type& operator [](I const& idx) { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return _data[idx]; }
			template<typename I>element_type const& operator [](I const& idx)const { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return _data[idx]; }

			element_type** operator & () { return &get(); }
		};
	}

	using collections::array_view;
	using collections::stack_array;
	using collections::scope_array;

	namespace algorithms
	{
		template<typename T, wsize N> inline constexpr wsize array_size(const T(&ar)[N]) { return N; }
		template<class T, wsize SIZE> inline constexpr wsize array_size(const stack_array<T, SIZE>&) { return SIZE; }
		template<class T> inline wsize array_size(const array_view<T>& arr) { return arr.size(); }

		template<typename K, typename T>
		wsize binary_search(K const& key, array_view<T> const& vector) {
			long64 first = 0, last = (long64)vector.size() - 1;
			long64 mid;
			while (first <= last) {
				mid = (first + last) / 2;
				if (logic_operation<T, K, logic_operation_type::same>::operate(vector[mid], key)) return (wsize)mid;
				else if (logic_operation<T, K, logic_operation_type::same>::operate(vector[mid], key)) last = mid - 1;
				else first = mid + 1;
			}
			return invalid_index;
		}
	}
}

#endif//__ANG_BASE_ARRAY_H__
