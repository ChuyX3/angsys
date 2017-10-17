#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_BASE_ARRAY_HPP__
#define __ANG_BASE_ARRAY_HPP__

namespace ang
{
	namespace collections
	{

		template<typename T> struct array_view
		{
		public:
			typedef T* type;
			typedef T element_type;

		private:
			wsize _size;
			element_type* _data;

		public:
			array_view() : _data(null), _size(0) {}
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

			type begin()const { return _data; }
			type end()const { return _data + _size; }

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

		template<typename T, wsize N>
		inline array_view<T> to_array(T(&ar)[N]) {
			return array_view<T>(ar);
		}

		template<typename T>
		inline array_view<T> to_array(T* ar, wsize N) {
			return array_view<T>(N, ar);
		}

		template<typename T, wsize _SIZE> struct stack_array
		{
		public:
			typedef T* type;
			typedef T const* ctype;
			typedef T* element_type;
		private:
			static constexpr wsize SIZE = _SIZE;
			T _data[_SIZE];

		public:
			stack_array() {}
			stack_array(stack_array && other) = default;

			template<wsize _OTHER_SIZE>
			stack_array(const stack_array<T, _OTHER_SIZE>& other) {
				for (windex i = 0; i < select_min_size<_OTHER_SIZE, _SIZE>(); ++i) {
					_data[i] = other._data[i];
				}
			}

			template<wsize N>
			stack_array(T(&ar)[N]) {
				for (windex i = 0; i < select_min_size<N, _SIZE>(); ++i) {
					_data[i] = ar[i];
				}
			}

			template<wsize N>
			stack_array(const T(&ar)[N]) {
				for (windex i = 0; i < select_min_size<N, _SIZE>(); ++i) {
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

			type data()const { return _data; }
			constexpr wsize size()const { return _SIZE; }

			type begin()const { return _data; }
			type end()const { return _data + _SIZE; }

		public: /*operators*/
			stack_array& operator = (stack_array && val) = default;
			template<wsize _OTHER_SIZE>
			stack_array& operator = (const stack_array<element_type, _OTHER_SIZE>& other) {
				for (windex i = 0; i < select_min_size<_OTHER_SIZE, _SIZE>(); ++i) {
					_data[i] = other._data[i];
				}
				return*this;
			}
			template<wsize N> stack_array& operator = (element_type(&ar)[N]) {
				for (windex i = 0; i < select_min_size<N, _SIZE>(); ++i) {
					_data[i] = ar[i];
				}
				return*this;
			}

			template<wsize N> stack_array& operator = (const element_type(&ar)[N]) {
				for (windex i = 0; i < select_min_size<N, _SIZE>(); ++i) {
					_data[i] = ar[i];
				}
				return*this;
			}

			operator type () { return get(); }
			operator ctype ()const { return get(); }
			operator array_view<T>()const { return array_view<T>(size(), get()); }

			template<typename I>element_type& operator [](I idx) { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); ANG_ASSERT(idx < _SIZE, "stack_array: operator [] overflow exception"); return _data[idx]; }
			template<typename I>element_type const& operator [](I idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); ANG_ASSERT(idx < _SIZE, "stack_array: operator [] overflow exception"); _data[idx]; }

			element_type** operator & () { return &get(); }
		};

		template<typename T, template<typename> class allocator>
		struct scope_array
		{
		public:
			typedef T* type;
			typedef T element_type;

		private:
			wsize _size;
			element_type* _data;

		public:
			scope_array();
			scope_array(wsize size, type val = null);
			scope_array(ang::nullptr_t const&);
			scope_array(scope_array const& other);
			scope_array(scope_array && other);
			template<typename U, wsize N>scope_array(U(&ar)[N]);
			template<typename U, wsize N>scope_array(const U(&ar)[N]);
			~scope_array() { clean(); }

		public: /*getters and setters*/
			type & get() { return _data; }
			type const& get()const { return _data; }
			void set(type val, wsize size);
			void move(scope_array&);
			void move(array_view<T>&);
			template<typename U, template<typename> class allocator2> void copy(scope_array<U, allocator2> const&);
			template<typename U> void copy(array_view<U> const&);
			template<typename U, wsize N> void copy(U(&ar)[N]);
			template<typename U, wsize N> void copy(const U(&ar)[N]);

			type data()const { return _data; }
			wsize size()const { return _size; }
			void clean();
			type alloc(wsize);

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

			template<typename I>element_type& operator [](I const& idx) { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); ANG_ASSERT(wsize(idx) < _size, "scope_array: operator [] overflow exception"); return _data[idx]; }
			template<typename I>element_type const& operator [](I const& idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); ANG_ASSERT(windex(idx) < _size, "scope_array: operator [] overflow exception"); return _data[idx]; }

			element_type** operator & () { return &get(); }
		};

	}

	template<class T, wsize SIZE>
	inline constexpr wsize array_size(const collections::stack_array<T, SIZE>&) { return SIZE; }

	template<class T>
	inline wsize array_size(const collections::array_view<T>& arr) { return arr.size(); }

	template<class T>
	inline wsize array_size(const collections::scope_array<T>& arr) { return arr.size(); }
}


#include <ang/base/inline/array.inl>

#endif//__ANG_BASE_ARRAY_HPP__