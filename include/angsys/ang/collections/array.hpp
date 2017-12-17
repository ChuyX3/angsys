#ifndef __ANGSYS_HPP__
#elif !defined __ANG_COLLECTIONS_ARRAY_HPP__
#define __ANG_COLLECTIONS_ARRAY_HPP__

namespace ang
{
	namespace collections
	{
		template<typename T, template<typename> class allocator = memory::default_allocator> class array_buffer;
		template<typename T, template<typename> class allocator = memory::default_allocator> using array = object_wrapper<array_buffer<T, allocator>>;
	}

	using collections::array;

	namespace collections
	{
		/******************************************************************/
		/* template class array_buffer :                                  */
		/*  -> implements the memory buffer of an array object            */
		/******************************************************************/
		template<typename T, template<typename> class allocator>
		class array_buffer final
			: public object
			, public ibuffer
			, public iarray<T>
		{
		public:
			typedef T							type;
			typedef array_buffer<T, allocator> 	self_t;
			typedef iarray<T>					iarray_t;
			typedef ienum<T>					ienum_t;
			typedef base_iterator<T>			base_iterator_t;
			typedef iterator<T>					iterator_t;
			typedef const_iterator<T>			const_iterator_t;
			typedef forward_iterator<T>			forward_iterator_t;
			typedef const_forward_iterator<T>	const_forward_iterator_t;
			typedef backward_iterator<T>		backward_iterator_t;
			typedef const_backward_iterator<T>	const_backward_iterator_t;

		protected:
			scope_array<T, allocator> _data;

		public:
			inline array_buffer();
			inline array_buffer(wsize reserve);
			template<typename U>inline array_buffer(ang::initializer_list_t<U> list);
			inline array_buffer(const ang::nullptr_t&);
			inline array_buffer(array_buffer&& ar);
			inline array_buffer(const array_buffer& ar);
			inline array_buffer(const array_buffer* ar);
			inline array_buffer(const ienum_t* store);
			template<typename U> inline array_buffer(array_view<U> const&);
			template<typename U, template<typename> class allocator2> inline array_buffer(scope_array<U, allocator2> const&);
			template<typename U, wsize SIZE> inline array_buffer(stack_array<U, SIZE> const&);

		private:
			virtual ~array_buffer();

		public: //methods
			inline bool is_empty()const;
			inline T* data()const;
			inline wsize size()const;
			inline void size(wsize size);

			inline void clean();
			inline bool move(array_buffer<T, allocator>&);
			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

		public: //ibuffer overrides
			inline text::encoding_t encoding()const override;
			inline pointer buffer_ptr()const override;
			inline wsize buffer_size()const override;
			inline wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override;
			inline ibuffer_view_t map_buffer(windex, wsize) override;
			inline bool unmap_buffer(ibuffer_view_t&, wsize) override;
			inline bool can_realloc_buffer()const override;
			inline bool realloc_buffer(wsize) override;

		public: //ienum overrides
			inline wsize counter()const override;

			inline T& at(base_iterator_t const&) override;
			inline bool increase(base_iterator_t&)const override;
			inline bool increase(base_iterator_t&, int offset)const override;
			inline bool decrease(base_iterator_t&)const override;
			inline bool decrease(base_iterator_t&, int offset)const override;

			inline forward_iterator_t begin() override;
			inline forward_iterator_t end() override;
			inline const_forward_iterator_t begin()const override;
			inline const_forward_iterator_t end()const override;

			inline backward_iterator_t rbegin() override;
			inline backward_iterator_t rend() override;
			inline const_backward_iterator_t rbegin()const override;
			inline const_backward_iterator_t rend()const override;

		public: //iarray overrides
			inline iterator_t at(windex) override;
			inline const_iterator_t at(windex)const override;
			inline void copy(const ienum<T>*) override;
			inline iterator<T> find(const T&, bool invert = false)const override;
			inline iterator<T> find(const T&, base_iterator_t next_to, bool invert = false)const override;

		public: //overrides
			inline static type_name_t class_name();
			inline static bool is_inherited_of(type_name_t);
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;
			inline comparision_result_t compare(const object&)const override;

		protected: //memory operations
			inline bool realloc(wsize size);

		};
	}//collections

	 /******************************************************************/
	 /* template class ang::object_wrapper<array_buffer> :             */
	 /*  -> specialization of object_wrapper<array_buffer> -> array    */
	 /******************************************************************/
	template<typename T, template<typename> class allocator>
	class object_wrapper<collections::array_buffer<T, allocator>>
	{
	public:
		typedef collections::array_buffer<T, allocator> type;
		typedef typename collections::array_buffer<T, allocator>::type data_type;

	private:
		collections::array_buffer<T, allocator>* _ptr;

	public:
		object_wrapper();
		template<typename U> inline object_wrapper(array_view<U> const& ar) : _ptr(null) {
			set(new collections::array_buffer<T, allocator>(ar));
		}
		template<typename U, template<typename> class allocator2> inline object_wrapper(scope_array<U, allocator2> const& ar) : _ptr(null) {
			set(new collections::array_buffer<T, allocator>(ar));
		}
		template<typename U, wsize SIZE> inline object_wrapper(stack_array<U, SIZE> const& ar) : _ptr(null) {
			set(new collections::array_buffer<T, allocator>(ar));
		}
		object_wrapper(collections::array_buffer<T, allocator>*);
		template<typename U> object_wrapper(ang::initializer_list_t<U> list);
		object_wrapper(const collections::ienum<data_type>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		template<typename U, wsize N>
		object_wrapper(U(&ar)[N]) : _ptr(null) {
			set(new collections::array_buffer<T, allocator>(to_array(ar, N)));
		}
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::array_buffer<T, allocator>* get(void)const;
		void set(collections::array_buffer<T, allocator>*);
		collections::array_buffer<T, allocator>** addres_of(void);

		collections::forward_iterator<T> begin() { return _ptr ? _ptr->begin() : collections::iterator<T>(); }
		collections::forward_iterator<T> end() { return _ptr ? _ptr->end() : collections::iterator<T>(); }
		collections::forward_iterator<const T> begin()const { return _ptr ? _ptr->begin() : collections::iterator<const T>(); }
		collections::forward_iterator<const T> end()const { return _ptr ? _ptr->end() : collections::iterator<const T>(); }

	public:
		object_wrapper& operator = (collections::array_buffer<T, allocator>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator = (T(&ar)[N]) {
			set(new collections::array_buffer<T, allocator>(to_array(ar, N)));
			return *this;
		}

		object_wrapper_ptr<collections::array_buffer<T, allocator>> operator & (void);
		collections::array_buffer<T, allocator> * operator -> (void);
		collections::array_buffer<T, allocator> const* operator -> (void)const;
		explicit operator collections::array_buffer<T, allocator> * (void);
		explicit operator collections::array_buffer<T, allocator> const* (void)const;
		operator array_view<T>()const { return _ptr ? to_array(_ptr->data(), _ptr->size()) : array_view<T>(); }
		template<typename I>T& operator[](I const& idx);
		template<typename I>T const& operator[](I const& idx)const;
	};

	namespace collections
	{
		template<typename T, template<typename> class allocator>
		struct scope_array<object_wrapper<T>, allocator>
		{
		public:
			typedef object_wrapper<T>* type;
			typedef object_wrapper<T> element_type;

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
			void move(array_view<object_wrapper<T>>&);
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

			operator type& () { return get(); }
			operator type ()const { return get(); }
			operator array_view<object_wrapper<T>>()const { return array_view<object_wrapper<T>>(size(), get()); }

			template<typename I>element_type& operator [](I  const& idx) { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); ANG_ASSERT((wsize)idx < _size, "scope_array: operator [] overflow exception"); return _data[idx]; }
			template<typename I>element_type const& operator [](I const& idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); ANG_ASSERT((wsize)idx < _size, "scope_array: operator [] overflow exception"); return _data[idx]; }

			element_type** operator & () { return &get(); }
		};
	}
}//ang


#endif //__ANG_COLLECTIONS_ARRAY_HPP__