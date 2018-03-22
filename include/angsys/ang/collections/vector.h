#ifndef __ANG_COLLECTIONS_H__
#elif !defined __ANG_COLLECTIONS_VECTOR_H__
#define __ANG_COLLECTIONS_VECTOR_H__

namespace ang
{
	namespace collections
	{
		/******************************************************************/
		/* template class vector_buffer :                                 */
		/*  -> implements the buffer memory of vector object              */
		/******************************************************************/
		template<typename T, template<typename> class allocator>
		class vector_buffer final
			: public object
			, public ibuffer
			, public ilist<T>
		{
		public:
			typedef T							type;
			typedef vector_buffer<T, allocator> self_t;
			typedef allocator<T>				allocator_t;
			typedef ilist<T>					ilist_t;
			typedef iarray<T>					iarray_t;
			typedef ienum<T>					ienum_type;
			typedef base_iterator<T>			base_iterator_t;
			typedef iterator<T>					iterator_t;
			typedef const_iterator<T>			const_iterator_t;
			typedef forward_iterator<T>			forward_iterator_t;
			typedef const_forward_iterator<T>	const_forward_iterator_t;
			typedef backward_iterator<T>		backward_iterator_t;
			typedef const_backward_iterator<T>	const_backward_iterator_t;


		protected:
			allocator_t alloc;
			wsize _size;
			wsize _capacity;
			type* _data;

		public:
			inline vector_buffer();
			inline vector_buffer(wsize reserve);
			template<typename U>inline vector_buffer(ang::initializer_list<U> list);
			inline vector_buffer(const ang::nullptr_t&);
			inline vector_buffer(vector_buffer&& ar);
			inline vector_buffer(const vector_buffer& ar);
			inline vector_buffer(const vector_buffer* ar);
			inline vector_buffer(const ienum_type* store);
			template<typename U> inline vector_buffer(array_view<U> const&);
			template<typename U, template<typename> class allocator2> inline vector_buffer(scope_array<U, allocator2> const&);
			template<typename U, wsize SIZE> inline vector_buffer(stack_array<U, SIZE> const&);

		private:
			virtual ~vector_buffer();

		public: //methods
			inline bool is_empty()const;
			inline T* data()const;
			inline wsize size()const;
			//inline void size(wsize);
			inline wsize capacity()const;
			inline void capacity(wsize size, bool save = false);

			inline void clear();
			inline void empty();
			inline bool move(vector_buffer<T, allocator>&);

			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

			template<typename U> inline void expand(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void expand(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void expand(stack_array<U, SIZE> const&);

		public: //ibuffer overrides
			inline bool is_readonly()const override;
			inline text::encoding_t encoding()const override;
			inline pointer buffer_ptr() override;
			inline const_pointer buffer_ptr()const override;
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
			inline forward_iterator_t last() override;
			inline const_forward_iterator_t last()const override;

			inline backward_iterator_t rbegin() override;
			inline backward_iterator_t rend() override;
			inline const_backward_iterator_t rbegin()const override;
			inline const_backward_iterator_t rend()const override;

		public: //iarray overrides
			inline iterator_t at(windex) override;
			inline const_iterator_t at(windex)const override;
			inline void copy(const ienum<T>*) override;
			inline iterator_t find(core::delegates::function<bool(T const&)>, bool invert = false)const override;
			inline iterator_t find(core::delegates::function<bool(T const&)>, base_iterator_t next_to, bool invert = false)const override;
			inline collections::ienum_ptr<T> find_all(core::delegates::function<bool(T const&)>)const override;

		public: //ilist overrides
			inline void extend(const ienum<T>*) override;
			inline void push(T const&, bool last = true) override;
			inline bool insert(windex idx, T const&) override;
			inline bool insert(base_iterator_t it, T const&) override;
			inline bool pop(bool last = true) override;
			inline bool pop(T&, bool last = true) override;
			inline bool pop_at(windex idx) override;
			inline bool pop_at(base_iterator_t it) override;
			inline bool pop_at(windex idx, T&) override;
			inline bool pop_at(base_iterator_t it, T&) override;

		public: //overrides
			ANG_DECLARE_INTERFACE();
			inline comparision_result_t compare(const object*)const override;

		protected: //Memory Operations
			inline bool realloc(wsize size, bool save = true);
		};

	}//collections


	 /******************************************************************/
	 /* template class ang::object_wrapper<vector_buffer> :             */
	 /*  -> specialization of object_wrapper<vector_buffer> -> array    */
	 /******************************************************************/
	template<typename T, template<typename> class allocator>
	class object_wrapper<collections::vector_buffer<T, allocator>>
	{
	public:
		typedef collections::vector_buffer<T, allocator> type;
		typedef typename collections::vector_buffer<T, allocator>::type data_type;

	private:
		collections::vector_buffer<T, allocator>* _ptr;

	public:
		object_wrapper();
		template<typename U> inline object_wrapper(array_view<U> const& ar) : _ptr(null) {
			set(new collections::vector_buffer<T, allocator>(ar));
		}
		template<typename U, template<typename> class allocator2> inline object_wrapper(scope_array<U, allocator2> const& ar) : _ptr(null) {
			set(new collections::vector_buffer<T, allocator>(ar));
		}
		template<typename U, wsize SIZE> inline object_wrapper(stack_array<U, SIZE> const& ar) : _ptr(null) {
			set(new collections::vector_buffer<T, allocator>(ar));
		}
		object_wrapper(collections::vector_buffer<T, allocator>*);
		template<typename U> object_wrapper(ang::initializer_list<U> list);
		object_wrapper(const collections::ienum<data_type>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		template<typename U, wsize N>
		object_wrapper(U(&ar)[N]) : _ptr(null) {
			set(new collections::vector_buffer<T, allocator>(to_array(ar, N)));
		}
		~object_wrapper();

	public:
		void clear();
		void clear_unsafe();
		bool is_empty()const;
		collections::vector_buffer<T, allocator>* get(void)const;
		void set(collections::vector_buffer<T, allocator>*);
		collections::vector_buffer<T, allocator>** addres_of(void);

		collections::forward_iterator<T> begin() { return _ptr ? _ptr->begin() : collections::iterator<T>(); }
		collections::forward_iterator<T> end() { return _ptr ? _ptr->end() : collections::iterator<T>(); }
		collections::forward_iterator<const T> begin()const { return _ptr ? _ptr->begin() : collections::iterator<const T>(); }
		collections::forward_iterator<const T> end()const { return _ptr ? _ptr->end() : collections::iterator<const T>(); }

	public:
		object_wrapper& operator = (collections::vector_buffer<T, allocator>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator = (T(&ar)[N]) {
			set(new collections::vector_buffer<T, allocator>(to_array(ar, N)));
			return *this;
		}

		object_wrapper& operator += (T items);
		object_wrapper& operator += (collections::ienum<data_type> const* items);
		object_wrapper& operator += (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator += (T(&ar)[N]) {
			if(is_empty())set(new collections::vector_buffer<T, allocator>(to_array(ar, N)));
			else _ptr->expand(to_array(ar, N));
			return *this;
		}

		object_wrapper_ptr<collections::vector_buffer<T, allocator>> operator & (void);
		collections::vector_buffer<T, allocator> * operator -> (void);
		collections::vector_buffer<T, allocator> const* operator -> (void)const;
		explicit operator collections::vector_buffer<T, allocator> * (void);
		explicit operator collections::vector_buffer<T, allocator> const* (void)const;
		operator array_view<T>()const { return _ptr ? to_array(_ptr->data(), _ptr->size()) : array_view<T>(); }
		template<typename I>T& operator[](I const& idx);
		template<typename I>T const& operator[](I const& idx)const;
	};

}//ang


#endif //__ANG_COLLECTIONS_VECTOR_H__