
#ifndef __ANG_VECTOR_HPP__
#error ...
#else

#define ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(_LINK, MY_TYPE, MY_ALLOCATOR) \
namespace ang { \
	namespace collections { \
		template<> class _LINK vector_buffer<MY_TYPE, MY_ALLOCATOR> final \
			: public object, public ibuffer, public ilist<MY_TYPE> { \
		public: \
			typedef MY_TYPE									type; \
			typedef vector_buffer<MY_TYPE, MY_ALLOCATOR>	self_t; \
			typedef iarray<MY_TYPE>							iarray; \
			typedef ienum<MY_TYPE>							ienum_t; \
			typedef ilist<MY_TYPE>							ilist_t; \
			typedef base_iterator<MY_TYPE>					base_iterator_t; \
			typedef iterator<MY_TYPE>						iterator_t; \
			typedef const_iterator<MY_TYPE>					const_iterator_t; \
			typedef forward_iterator<MY_TYPE>				forward_iterator_t; \
			typedef const_forward_iterator<MY_TYPE>			const_forward_iterator_t; \
			typedef backward_iterator<MY_TYPE>				backward_iterator_t; \
			typedef const_backward_iterator<MY_TYPE>		const_backward_iterator_t; \
		protected: \
			wsize _size; \
			wsize _capacity; \
			type* _data; \
		public: \
			vector_buffer(); \
			vector_buffer(wsize reserve); \
			template<typename U> inline vector_buffer(ang::initializer_list_t<U> list) : vector_buffer() { \
				capacity((wsize)list.size()); \
				for (auto it = list.begin(); it != list.end(); ++it) push(*it); \
			} \
			vector_buffer(const ang::nullptr_t&); \
			vector_buffer(vector_buffer&& ar); \
			vector_buffer(const vector_buffer& ar); \
			vector_buffer(const vector_buffer* ar); \
			vector_buffer(const ienum_t* store); \
			template<typename U> inline vector_buffer(array_view<U> const& ar) : vector_buffer() { copy(ar); } \
			template<typename U, template<typename> class allocator2> inline vector_buffer(scope_array<U, allocator2> const& ar) { copy(ar); } \
			template<typename U, wsize SIZE> inline vector_buffer(stack_array<U, SIZE> const& ar) { copy(ar); } \
		private: \
			virtual ~vector_buffer(); \
		public: \
			bool is_empty()const; \
			MY_TYPE* data()const; \
			wsize size()const; \
			void size(wsize); \
			wsize capacity()const; \
			void capacity(wsize size, bool save = false); \
			void clean(); \
			void empty(); \
			bool move(vector_buffer<MY_TYPE, MY_ALLOCATOR>&); \
			template<typename U> inline void copy(array_view<U>const& ar) { \
				wsize l = ar.size(); \
				capacity(ar.size()); \
				for (wsize i = 0U; i < l; i++) push(ar[i]); \
			} \
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const& ar) { \
				wsize l = ar.size(); \
				capacity(ar.size()); \
				for (wsize i = 0U; i < l; i++) push(ar[i]); \
			} \
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const& ar) { \
				capacity(SIZE); \
				for (wsize i = 0U; i < SIZE; i++) push(ar[i]); \
			} \
			template<typename U> inline void expand(array_view<U>const& ar) { \
				capacity(size() + ar.size(), true); \
				wsize l = ar.size(); \
				for (wsize i = 0U; i < l; i++) push(ar[i]); \
			} \
			template<typename U, template<typename> class allocator2> inline void expand(scope_array<U, allocator2>const& ar) { \
				capacity(size() + ar.size(), true); \
				wsize l = ar.size(); \
				for (wsize i = 0U; i < l; i++) push(ar[i]); \
			} \
			template<typename U, wsize SIZE> inline void expand(stack_array<U, SIZE> const& ar) { \
				capacity(size() + SIZE, true); \
				for (wsize i = 0U; i < SIZE; i++) push(ar[i]); \
			} \
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
			virtual iterator_t find(const MY_TYPE&, bool invert = false)const override; \
			virtual iterator_t find(const MY_TYPE&, base_iterator_t next_to, bool invert = false)const override; \
		public: \
			virtual void extend(const ienum<MY_TYPE>*) override; \
			virtual void push(MY_TYPE const&, bool last = true) override; \
			virtual bool insert(windex idx, MY_TYPE const&) override; \
			virtual bool insert(base_iterator_t it, MY_TYPE const&) override; \
			virtual bool pop(bool last = true) override; \
			virtual bool pop(MY_TYPE&, bool last = true) override; \
			virtual bool pop_at(windex idx) override; \
			virtual bool pop_at(base_iterator_t it) override; \
			virtual bool pop_at(windex idx, MY_TYPE&) override; \
			virtual bool pop_at(base_iterator_t it, MY_TYPE&) override; \
		public: \
			inline static type_name_t class_name(); \
			inline static bool is_inherited_of(type_name_t); \
			virtual type_name_t object_name()const override; \
			virtual bool is_kind_of(type_name_t)const override; \
			virtual bool query_object(type_name_t, unknown_ptr_t) override; \
			virtual comparision_result_t compare(const object&)const override; \
		protected: \
			bool realloc(wsize size, bool save = true); \
		}; \
	} \
	template<> class _LINK object_wrapper<collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>> { \
	public: \
		typedef collections::vector_buffer<MY_TYPE, MY_ALLOCATOR> type; \
		typedef typename collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>::type data_type; \
	private: \
		collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>* _ptr; \
	public: \
		object_wrapper(); \
		object_wrapper(collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>*); \
		object_wrapper(const collections::ienum<data_type>* store); \
		object_wrapper(object_wrapper &&); \
		object_wrapper(object_wrapper const&); \
		object_wrapper(ang::nullptr_t const&); \
		template<typename U> inline object_wrapper(array_view<U> const& ar) : _ptr(null) { set(new collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>(ar)); } \
		template<typename U, template<typename> class allocator2> inline object_wrapper(scope_array<U, allocator2> const& ar) : _ptr(null) { set(new collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>(ar)); } \
		template<typename U, wsize SIZE> inline object_wrapper(stack_array<U, SIZE> const& ar) : _ptr(null) { set(new collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>(ar)); } \
		template<typename U> object_wrapper(ang::initializer_list_t<U> list) : _ptr(null) { set(new collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>(ang::move(list))); } \
		template<typename U, wsize N> object_wrapper(U(&ar)[N]) : _ptr(null) { set(new collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>(to_array(ar, N))); } \
		~object_wrapper(); \
	public: \
		void clean(); \
		void clean_unsafe(); \
		bool is_empty()const; \
		collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>* get(void)const; \
		void set(collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>*); \
		collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>** addres_of(void); \
		collections::forward_iterator<MY_TYPE> begin() { return _ptr ? _ptr->begin() : collections::forward_iterator<MY_TYPE>(); } \
		collections::forward_iterator<MY_TYPE> end() { return _ptr ? _ptr->end() : collections::forward_iterator<MY_TYPE>(); } \
		collections::forward_iterator<const MY_TYPE> begin()const { return _ptr ? ((type const*)_ptr)->begin() : collections::forward_iterator<const MY_TYPE>(); } \
		collections::forward_iterator<const MY_TYPE> end()const { return _ptr ? ((type const*)_ptr)->end() : collections::forward_iterator<const MY_TYPE>(); } \
	public: \
		object_wrapper& operator = (collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>*); \
		object_wrapper& operator = (const ang::nullptr_t&); \
		object_wrapper& operator = (collections::ienum<data_type> const* items); \
		object_wrapper& operator = (object_wrapper &&); \
		object_wrapper& operator = (object_wrapper const&); \
		template<wsize N> object_wrapper& operator = (MY_TYPE(&ar)[N]) { \
			if (is_empty()) set(new collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>(to_array(ar, N))); \
			else _ptr->copy(to_array(ar, N)); \
			return *this; \
		} \
		template<wsize N> object_wrapper& operator += (MY_TYPE(&ar)[N]) { \
			if(is_empty()) set(new collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>(to_array(ar, N))); \
			else _ptr->expand(to_array(ar, N)); \
			return *this; \
		} \
		object_wrapper& operator += (MY_TYPE const& value) { \
			if (is_empty()) set(new collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>()); \
			_ptr->push(value); \
			return *this; \
		} \
		object_wrapper_ptr<collections::vector_buffer<MY_TYPE, MY_ALLOCATOR>> operator & (void); \
		collections::vector_buffer<MY_TYPE, MY_ALLOCATOR> * operator -> (void); \
		collections::vector_buffer<MY_TYPE, MY_ALLOCATOR> const* operator -> (void)const; \
		explicit operator collections::vector_buffer<MY_TYPE, MY_ALLOCATOR> * (void); \
		explicit operator collections::vector_buffer<MY_TYPE, MY_ALLOCATOR> const* (void)const; \
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

#endif//__ANG_VECTOR_HPP__
