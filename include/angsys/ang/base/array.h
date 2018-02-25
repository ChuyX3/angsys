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
			wsize _size;
			element_type* _data;
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

			template<typename I>element_type& operator [](I idx) { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); ANG_ASSERT(wsize(idx) < _size, "array_view: operator [] overflow exception"); return _data[idx]; }
			template<typename I>element_type const& operator [](I idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); ANG_ASSERT(wsize(idx) < _size, "array_view: operator [] overflow exception"); return _data[idx]; }

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
			stack_array() {}
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
	}

	using collections::array_view;
}

#endif//__ANG_BASE_ARRAY_H__