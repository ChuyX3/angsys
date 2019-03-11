#ifndef __ANG_COLLECTIONS_H__
#elif !defined __ANG_COLLECTIONS_ARRAY_H__
#define __ANG_COLLECTIONS_ARRAY_H__

namespace ang
{
	namespace collections
	{
		/******************************************************************/
		/* template class array_buffer :                                  */
		/*  -> implements the memory buffer of an array object            */
		/******************************************************************/
		template<typename T, template<typename> class A>
		class array_buffer final
			: public smart<array_buffer<T,A>, ivariant, ibuffer, iarray<T>>
		{
		public:
			typedef T							type;
			typedef A<T>						allocator_t;
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
			allocator_t m_alloc;
			wsize m_size;
			type* m_data;

		public:
			inline array_buffer();
			inline array_buffer(wsize reserve);
			template<typename U>inline array_buffer(ang::initializer_list<U> list);
			inline array_buffer(const ang::nullptr_t&);
			inline array_buffer(array_buffer&& ar);
			inline array_buffer(const array_buffer& ar);
			inline array_buffer(const array_buffer* ar);
			inline array_buffer(const ienum_type* store);
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

			inline void clear()override;
			inline bool move(array_buffer<T, A>&);
			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);
			template<typename U> inline void copy(ienum_ptr<U> const&);

		public: //ivariant overrides
			inline rtti_t const& value_type()const override;
			inline bool set_value(rtti_t const&, unknown_t)override;
			inline bool get_value(rtti_t const&, unknown_t)const override;
			inline variant clone()const override;
			inline wstring to_string()const override;
			inline wstring to_string(text::text_format_t)const override;

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
			inline void copy(const ienum_type*) override;
			inline iterator<T> find(core::delegates::function<bool(T const&)>, bool invert = false)const override;
			inline iterator<T> find(core::delegates::function<bool(T const&)>, base_iterator_t next_to, bool invert = false)const override;
			inline ienum_ptr<T> find_all(core::delegates::function<bool(T const&)>)const override;

		public: //overrides
			ANG_DECLARE_INTERFACE();
			inline comparision_result_t compare(const object*)const override;

		protected: //memory operations
			inline bool realloc(wsize size);

		};

	}//collections


	/******************************************************************/
	/* template class ang::object_wrapper<array_buffer> :             */
	/*  -> specialization of object_wrapper<array_buffer> -> array    */
	/******************************************************************/
	template<typename T, template<typename> class A>
	class object_wrapper<collections::array_buffer<T, A>>
	{
	public:
		typedef collections::array_buffer<T, A> type;
		typedef typename collections::array_buffer<T, A>::type data_type;

	private:
		collections::array_buffer<T, A>* m_ptr;

	public:
		object_wrapper();
		template<typename U> inline object_wrapper(array_view<U> const& ar) : m_ptr(null) {
			set(new collections::array_buffer<T, A>(ar));
		}
		template<typename U, template<typename> class allocator2> inline object_wrapper(scope_array<U, allocator2> const& ar) : m_ptr(null) {
			set(new collections::array_buffer<T, A>(ar));
		}
		template<typename U, wsize SIZE> inline object_wrapper(stack_array<U, SIZE> const& ar) : m_ptr(null) {
			set(new collections::array_buffer<T, A>(ar));
		}
		template<typename U> object_wrapper(ang::initializer_list<U> list);
		object_wrapper(wsize reserve);
		object_wrapper(collections::array_buffer<T, A>*);
		object_wrapper(const collections::ienum<data_type>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		template<typename U, wsize N>
		object_wrapper(U(&ar)[N]) : m_ptr(null) {
			set(new collections::array_buffer<T, A>(to_array(ar, N)));
		}
		~object_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		collections::array_buffer<T, A>* get(void)const;
		void set(collections::array_buffer<T, A>*);
		collections::array_buffer<T, A>** addres_of(void);

		collections::forward_iterator<T> begin() { return m_ptr ? m_ptr->begin() : collections::iterator<T>(); }
		collections::forward_iterator<T> end() { return m_ptr ? m_ptr->end() : collections::iterator<T>(); }
		collections::forward_iterator<const T> begin()const { return m_ptr ? m_ptr->begin() : collections::iterator<const T>(); }
		collections::forward_iterator<const T> end()const { return m_ptr ? m_ptr->end() : collections::iterator<const T>(); }

	public:
		object_wrapper& operator = (collections::array_buffer<T, A>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator = (T(&ar)[N]) {
			if (is_empty())
				set(new collections::array_buffer<T, A>(to_array(ar, N)));
			else
				get()->copy(to_array(ar, N));
			return *this;
		}
		template<typename U>
		object_wrapper& operator = (collections::ienum_ptr<U> const items) {
			if (is_empty())
				set(new collections::array_buffer<T, A>());
			get()->copy(items);
			return *this;
		}
		object_wrapper_ptr<collections::array_buffer<T, A>> operator & (void);
		collections::array_buffer<T, A> * operator -> (void);
		collections::array_buffer<T, A> const* operator -> (void)const;
		explicit operator collections::array_buffer<T, A> * (void);
		explicit operator collections::array_buffer<T, A> const* (void)const;
		operator array_view<T>()const { return m_ptr ? to_array(m_ptr->data(), m_ptr->size()) : array_view<T>(); }
		operator array_view<const T>()const { return m_ptr ? to_array((const T*)m_ptr->data(), m_ptr->size()) : array_view<T>(); }
		template<typename I>T& operator[](I const& idx);
		template<typename I>T const& operator[](I const& idx)const;
	};


	template<typename T, template<typename>class A>
	struct property_helper<array<T,A>, smart_type::none> {
		using type = array<T, A>;
		using ret_type = type&;
		using ptr_type = collections::array_buffer<T, A>*;
		using arg_type = array_view<T>;
		using property_class = base_property<type>;
		using getter_type = ret_type(*)(property_class const*);
		using setter_type = void(*)(property_class const*, arg_type);
	};

	template<typename T, template<typename>class A>
	struct property_helper<const array<T, A>, smart_type::none> {
		using type = array<T, A>;
		using ret_type = array_view<const T>;
		using ptr_type = collections::array_buffer<T,A>*;
		using arg_type = array_view<T>;
		using property_class = base_property<type>;
		using getter_type = ret_type(*)(property_class const*);
		using setter_type = void(*)(property_class const*, arg_type);
	};


	/*template<typename T, template<typename>class A,
		typename property_helper<array<T, A>>::getter_type getter,
		typename property_helper<array<T, A>>::setter_type setter>
	struct property<array<T,A>, getter, setter> : property_helper<array<T, A>>::property_class {
		inline typename property_helper<array<T, A>>::ret_type operator = (typename property_helper<array<T, A>>::arg_type value) {
			setter(this, forward<typename property_helper<array<T, A>>::arg_type>(value));
			return getter(this);
		}
		inline operator typename property_helper<array<T, A>>::ret_type()const {
			return getter(this);
		}
		inline typename property_helper<array<T, A>>::ptr_type operator ->()const {
			return getter(this);
		}
		inline auto& operator [](windex i) {
			return getter(this)[i];
		}
		inline auto operator [](windex i)const {
			return getter(this)[i];
		}
	};

	template<typename T, template<typename>class A,
		typename property_helper<const array<T, A>>::getter_type getter,
		typename property_helper<const array<T, A>>::setter_type setter>
		struct property<const array<T, A>, getter, setter> : property_helper<const array<T, A>>::property_class {
		inline typename property_helper<const array<T, A>>::ret_type operator = (typename property_helper<const array<T, A>>::arg_type value) {
			setter(this, forward<typename property_helper<const array<T, A>>::arg_type>(value));
			return getter(this);
		}
		inline operator typename property_helper<const array<T, A>>::ret_type()const {
			return getter(this);
		}
		inline typename property_helper<const array<T, A>>::ptr_type operator ->()const {
			return getter(this);
		}
		inline auto operator [](windex i)const {
			return getter(this)[i];
		}
	};*/

}//ang


#endif //__ANG_COLLECTIONS_ITERATORS_H__