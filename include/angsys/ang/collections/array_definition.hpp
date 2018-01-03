#ifndef __ANG_COLLECTIONS_ARRAY_HPP__
#error ...
#else
#define ANG_ARRAY_VALUE_SPECIALIZATION_DECLARATION(_LINK, MY_TYPE, MY_ALLOCATOR) \
namespace ang { \
	namespace collections { \
		template<> struct _LINK scope_array<MY_TYPE, MY_ALLOCATOR> { \
		public: \
			typedef MY_TYPE* type; \
			typedef MY_TYPE element_type; \
		private: \
			wsize _size; \
			element_type* _data; \
		public: \
			scope_array(); \
			scope_array(wsize size, type val = null); \
			scope_array(ang::nullptr_t const&); \
			scope_array(scope_array const& other); \
			scope_array(scope_array && other); \
			template<typename U, wsize N> inline scope_array(U(&ar)[N]) : _size(N), _data(null) { \
				if (_size > 0) { \
					_data = MY_ALLOCATOR<MY_TYPE>::alloc(_size); \
					for (index i = 0; i < _size; ++i) MY_ALLOCATOR<MY_TYPE>::template construct<U const&>((MY_TYPE*)&_data[i], ar[i]); \
				} \
			} \
			template<typename U, wsize N> inline scope_array(const U(&ar)[N]) : _size(N), _data(null) { \
				if (_size > 0) { \
					_data = MY_ALLOCATOR<MY_TYPE>::alloc(_size); \
					for (index i = 0; i < _size; ++i) MY_ALLOCATOR<MY_TYPE>::template construct<U const&>((MY_TYPE*)&_data[i], ar[i]); \
				} \
			} \
			inline~scope_array() { clean(); } \
		public: /*getters and setters*/ \
			inline type & get() { return _data; } \
			inline type const& get()const { return _data; } \
			void set(type val, wsize size); \
			void move(scope_array&); \
			void move(array_view<MY_TYPE>&); \
			template<typename U, template<typename> class allocator2> inline void copy(ang::scope_array<U, allocator2> const& other) { \
				clean(); \
				if (other.size() > 0) _data = MY_ALLOCATOR<MY_TYPE>::alloc(other.size()); \
				_size = other.size(); \
				for (windex i = 0; i < _size; ++i) MY_ALLOCATOR<MY_TYPE>::template construct<U const&>(&_data[i], other[i]); \
				other.set(null, 0); \
			} \
			template<typename U> inline void copy(ang::array_view<U> const& other) { \
				clean(); \
				if (other.size() > 0) _data = MY_ALLOCATOR<MY_TYPE>::alloc(other.size()); \
				_size = other.size(); \
				for (windex i = 0; i < _size; ++i) MY_ALLOCATOR<MY_TYPE>::template construct<U const&>(&_data[i], other[i]); \
			} \
			template<typename U, wsize N> inline void copy(U(&ar)[N]) { \
				clean(); \
				_size = N; \
				if (_size > 0) { \
					_data = MY_ALLOCATOR<MY_TYPE>::alloc(_size); \
					for (index i = 0; i < _size; ++i) MY_ALLOCATOR<MY_TYPE>::template construct<U const&>((MY_TYPE*)&_data[i], ar[i]); \
				} \
			} \
			template<typename U, wsize N> inline void copy(const U(&ar)[N]) { \
				clean(); \
				_size = N; \
				if (_size > 0) { \
					_data = MY_ALLOCATOR<MY_TYPE>::alloc(_size); \
					for (index i = 0; i < _size; ++i) MY_ALLOCATOR<MY_TYPE>::template construct<U const&>((MY_TYPE*)&_data[i], ar[i]); \
				} \
			} \
			inline type data()const { return _data; } \
			inline wsize size()const { return _size; } \
			void clean(); \
			type alloc(wsize); \
			inline type begin()const { return _data; } \
			inline type end()const { return _data + _size; } \
		public: /*operators*/ \
			inline scope_array& operator = (type val) { set(ang::move(val), 1u); return*this; } \
			inline scope_array& operator = (scope_array const& val) { set(val._data, val._size); return*this; } \
			inline scope_array& operator = (scope_array && val) { move(val); return*this; } \
			template<typename U, wsize N> inline scope_array& operator = (U(&ar)[N]) { copy(ar); return*this; } \
			template<typename U, wsize N> inline scope_array& operator = (const U(&ar)[N]) { copy(ar); return*this; } \
			inline explicit operator type& () { return get(); } \
			inline explicit operator type ()const { return get(); } \
			inline operator array_view<MY_TYPE>()const { return array_view<MY_TYPE>(size(), get()); } \
			template<typename I> inline element_type& operator [](I const& idx) { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); ANG_ASSERT(wsize(idx) < _size, "scope_array: operator [] overflow exception"); return _data[idx]; } \
			template<typename I> inline element_type const& operator [](I const& idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); ANG_ASSERT(windex(idx) < _size, "scope_array: operator [] overflow exception"); return _data[idx]; } \
			inline element_type** operator & () { return &get(); } \
		}; \
		template<> class _LINK array_buffer<MY_TYPE, MY_ALLOCATOR> final \
			: public object \
			, public ibuffer \
			, public iarray<MY_TYPE> { \
		public: \
			typedef MY_TYPE								type; \
			typedef array_buffer<MY_TYPE, MY_ALLOCATOR> self_t; \
			typedef iarray<MY_TYPE>						iarray_t; \
			typedef ienum<MY_TYPE>						ienum_t; \
			typedef base_iterator<MY_TYPE>				base_iterator_t; \
			typedef iterator<MY_TYPE>					iterator_t; \
			typedef const_iterator<MY_TYPE>				const_iterator_t; \
			typedef forward_iterator<MY_TYPE>			forward_iterator_t; \
			typedef const_forward_iterator<MY_TYPE>		const_forward_iterator_t; \
			typedef backward_iterator<MY_TYPE>			backward_iterator_t; \
			typedef const_backward_iterator<MY_TYPE>	const_backward_iterator_t; \
		protected: \
			scope_array<MY_TYPE, MY_ALLOCATOR> _data; \
		public: \
			array_buffer(); \
			array_buffer(wsize reserve); \
			template<typename U>inline array_buffer(std::initializer_list<U> list) : array_buffer() { \
				wsize c = 0; \
				_data.alloc(list.size()); \
				for (auto it = list.begin(); it != list.end(); ++it) _data[c++] = *it; \
			} \
			array_buffer(const ang::nullptr_t&); \
			array_buffer(array_buffer&& ar); \
			array_buffer(const array_buffer& ar); \
			array_buffer(const array_buffer* ar); \
			array_buffer(const ienum_t* store); \
			template<typename U> inline array_buffer(ang::array_view<U> const& ar) : array_buffer() { copy(ar); } \
			template<typename U, template<typename> class allocator2> inline array_buffer(ang::scope_array<U, allocator2> const& ar) : array_buffer() { copy(ar); } \
			template<typename U, wsize SIZE> inline array_buffer(stack_array<U, SIZE> const& ar) : array_buffer() { copy(ar); } \
		private: \
			virtual ~array_buffer(); \
		public:  \
			bool is_empty()const; \
			MY_TYPE* data()const; \
			wsize size()const; \
			void size(wsize size); \
			void clean(); \
			bool move(array_buffer<MY_TYPE, MY_ALLOCATOR>&); \
			template<typename U> inline void copy(array_view<U>const& ar){ _data.copy(ar); } \
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const& ar){ _data.copy(ar); } \
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const& ar){ _data.copy(ar); } \
		public: \
			virtual text::encoding_t encoding()const override; \
			virtual pointer buffer_ptr()const override; \
			virtual wsize buffer_size()const override; \
			virtual wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override; \
			virtual ibuffer_view_t map_buffer(windex, wsize) override; \
			virtual bool unmap_buffer(ibuffer_view_t&, wsize) override; \
			virtual bool can_realloc_buffer()const override; \
			virtual bool realloc_buffer(wsize) override; \
		public: \
			virtual wsize counter()const override; \
			virtual MY_TYPE& at(base_iterator_t const&) override; \
			virtual bool increase(base_iterator_t&)const override; \
			virtual bool increase(base_iterator_t&, int offset)const override; \
			virtual bool decrease(base_iterator_t&)const override; \
			virtual bool decrease(base_iterator_t&, int offset)const override; \
			virtual forward_iterator_t begin() override; \
			virtual forward_iterator_t end() override; \
			virtual const_forward_iterator_t begin()const override; \
			virtual const_forward_iterator_t end()const override; \
			virtual forward_iterator_t last() override; \
			virtual const_forward_iterator_t last()const override; \
			virtual backward_iterator_t rbegin() override; \
			virtual backward_iterator_t rend() override; \
			virtual const_backward_iterator_t rbegin()const override; \
			virtual const_backward_iterator_t rend()const override; \
		public: \
			virtual iterator_t at(windex) override; \
			virtual const_iterator_t at(windex)const override; \
			virtual void copy(const ienum<MY_TYPE>*) override; \
			virtual iterator<MY_TYPE> find(const MY_TYPE&, bool invert = false)const override; \
			virtual iterator<MY_TYPE> find(const MY_TYPE&, base_iterator_t next_to, bool invert = false)const override; \
		public: \
			static type_name_t class_name(); \
			static bool is_inherited_of(type_name_t); \
			virtual type_name_t object_name()const override; \
			virtual bool is_kind_of(type_name_t)const override; \
			virtual bool query_object(type_name_t, unknown_ptr_t) override; \
			virtual comparision_result_t compare(const object&)const override; \
		protected:  \
			bool realloc(wsize size); \
		}; \
	} \
	template<> class _LINK object_wrapper<collections::array_buffer<MY_TYPE, MY_ALLOCATOR>> { \
	public: \
		typedef collections::array_buffer<MY_TYPE, MY_ALLOCATOR> type; \
		typedef typename collections::array_buffer<MY_TYPE, MY_ALLOCATOR>::type data_type; \
	private: \
		collections::array_buffer<MY_TYPE, MY_ALLOCATOR>* _ptr; \
	public: \
		object_wrapper(); \
		template<typename U> inline object_wrapper(array_view<U> const& ar) : _ptr(null) { set(new collections::array_buffer<MY_TYPE, MY_ALLOCATOR>(ar)); } \
		template<typename U, template<typename> class allocator2> inline object_wrapper(scope_array<U, allocator2> const& ar) : _ptr(null) { set(new collections::array_buffer<MY_TYPE, MY_ALLOCATOR>(ar)); } \
		template<typename U, wsize SIZE> inline object_wrapper(stack_array<U, SIZE> const& ar) : _ptr(null) { set(new collections::array_buffer<MY_TYPE, MY_ALLOCATOR>(ar)); } \
		object_wrapper(collections::array_buffer<MY_TYPE, MY_ALLOCATOR>*); \
		template<typename U> inline object_wrapper(ang::initializer_list_t<U> list) : _ptr(null) { set(new collections::array_buffer<MY_TYPE, MY_ALLOCATOR>(ang::move(list))); } \
		object_wrapper(const collections::ienum<data_type>* store); \
		object_wrapper(object_wrapper &&); \
		object_wrapper(object_wrapper const&); \
		object_wrapper(ang::nullptr_t const&); \
		template<typename U, wsize N> inline object_wrapper(U(&ar)[N]) : _ptr(null) { set(new collections::array_buffer<MY_TYPE, MY_ALLOCATOR>(to_array(ar, N))); } \
		~object_wrapper(); \
	public: \
		void clean(); \
		void clean_unsafe(); \
		bool is_empty()const; \
		collections::array_buffer<MY_TYPE, MY_ALLOCATOR>* get(void)const; \
		void set(collections::array_buffer<MY_TYPE, MY_ALLOCATOR>*); \
		collections::array_buffer<MY_TYPE, MY_ALLOCATOR>** addres_of(void); \
		inline collections::forward_iterator<MY_TYPE> begin() { return _ptr ? _ptr->begin() : collections::iterator<MY_TYPE>(); } \
		inline collections::forward_iterator<MY_TYPE> end() { return _ptr ? _ptr->end() : collections::iterator<MY_TYPE>(); } \
		inline collections::forward_iterator<const MY_TYPE> begin()const { return _ptr ? _ptr->begin() : collections::iterator<const MY_TYPE>(); } \
		inline collections::forward_iterator<const MY_TYPE> end()const { return _ptr ? _ptr->end() : collections::iterator<const MY_TYPE>(); } \
	public: \
		object_wrapper& operator = (collections::array_buffer<MY_TYPE, MY_ALLOCATOR>*); \
		object_wrapper& operator = (const ang::nullptr_t&); \
		object_wrapper& operator = (collections::ienum<data_type> const* items); \
		object_wrapper& operator = (object_wrapper &&); \
		object_wrapper& operator = (object_wrapper const&); \
		template<wsize N> inline object_wrapper& operator = (MY_TYPE(&ar)[N]) { \
			set(new collections::array_buffer<MY_TYPE, MY_ALLOCATOR>(collections::to_array(ar, N))); \
			return *this; \
		} \
		object_wrapper_ptr<collections::array_buffer<MY_TYPE, MY_ALLOCATOR>> operator & (void); \
		collections::array_buffer<MY_TYPE, MY_ALLOCATOR> * operator -> (void); \
		collections::array_buffer<MY_TYPE, MY_ALLOCATOR> const* operator -> (void)const; \
		explicit operator collections::array_buffer<MY_TYPE, MY_ALLOCATOR> * (void); \
		explicit operator collections::array_buffer<MY_TYPE, MY_ALLOCATOR> const* (void)const; \
		inline operator array_view<MY_TYPE>()const { return _ptr ? collections::to_array(_ptr->data(), _ptr->size()) : array_view<MY_TYPE>(); } \
		template<typename I>inline MY_TYPE& operator[](I const& idx) { \
			static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); \
			return _ptr->data()[idx]; \
		} \
		template<typename I>inline MY_TYPE const& operator[](I const& idx)const { \
			static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); \
			return _ptr->data()[idx]; \
		} \
	}; \
}

#endif //__ANG_COLLECTIONS_ARRAY_HPP__