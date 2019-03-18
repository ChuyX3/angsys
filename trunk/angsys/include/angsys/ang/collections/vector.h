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
		template<typename T, template<typename> class A>
		class vector_buffer final
			: public smart<vector_buffer<T,A>, object, ivariant, ibuffer, ilist<T>>
		{
		public:
			typedef T							type;
			typedef vector_buffer<T, A> self_t;
			typedef A<T>				allocator_t;
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
			allocator_t m_alloc;
			wsize m_size;
			wsize m_capacity;
			type* m_data;

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
			inline void size(wsize, bool save);
			inline wsize capacity()const;
			inline void capacity(wsize size, bool save = false);

			inline void clear();
			inline void empty();
			inline bool move(vector_buffer<T, A>&);

			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

			template<typename U> inline void extend(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void extend(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void extend(stack_array<U, SIZE> const&);

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
			inline void copy(const ienum<T>*) override;
			inline iterator_t find(core::delegates::function<bool(T const&)>, bool invert = false)const override;
			inline iterator_t find(core::delegates::function<bool(T const&)>, base_iterator_t next_to, bool invert = false)const override;
			inline collections::ienum_ptr<T> find_all(core::delegates::function<bool(T const&)>)const override;
			template<typename U> inline collections::ienum_ptr<U> find_all(core::delegates::function<bool(T const&, U& out)>)const;

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
	template<typename T, template<typename> class A>
	class object_wrapper<collections::vector_buffer<T, A>>
	{
	public:
		typedef collections::vector_buffer<T, A> type;
		typedef typename collections::vector_buffer<T, A>::type data_type;

	private:
		type* m_ptr;

	public:
		object_wrapper();
		template<typename U> inline object_wrapper(array_view<U> const& ar) : m_ptr(null) {
			set(new type(ar));
		}
		template<typename U, template<typename> class allocator2> inline object_wrapper(scope_array<U, allocator2> const& ar) : m_ptr(null) {
			set(new type(ar));
		}
		template<typename U, wsize SIZE> inline object_wrapper(stack_array<U, SIZE> const& ar) : m_ptr(null) {
			set(new type(ar));
		}
		template<typename U> object_wrapper(ang::initializer_list<U> list);
		object_wrapper(wsize reserve);
		object_wrapper(type*);
		object_wrapper(const collections::ienum<data_type>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		template<typename U, wsize N>
		object_wrapper(U(&ar)[N]) : m_ptr(null) {
			set(new type(to_array(ar, N)));
		}
		~object_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type** addres_of(void);
		type** addres_for_init(void);

		collections::forward_iterator<T> begin() { return m_ptr ? m_ptr->begin() : collections::iterator<T>(); }
		collections::forward_iterator<T> end() { return m_ptr ? m_ptr->end() : collections::iterator<T>(); }
		collections::forward_iterator<const T> begin()const { return m_ptr ? m_ptr->begin() : collections::iterator<const T>(); }
		collections::forward_iterator<const T> end()const { return m_ptr ? m_ptr->end() : collections::iterator<const T>(); }

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator = (T(&ar)[N]) {
			set(new type(to_array(ar, N)));
			return *this;
		}

		object_wrapper& operator += (T items);
		object_wrapper& operator += (collections::ienum<data_type> const* items);
		object_wrapper& operator += (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator += (T(&ar)[N]) {
			if(is_empty())set(new type(to_array(ar, N)));
			else m_ptr->extend(to_array(ar, N));
			return *this;
		}

		object_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		explicit operator type * (void);
		explicit operator type const* (void)const;
		operator array_view<T>()const { return m_ptr ? collections::to_array(m_ptr->data(), m_ptr->size()) : array_view<T>(); }
		operator array_view<const T>()const { return m_ptr ? collections::to_array((const T*)m_ptr->data(), m_ptr->size()) : array_view<const T>(); }
		template<typename I>T& operator[](I const& idx);
		template<typename I>T const& operator[](I const& idx)const;
	};




	/******************************************************************/
	 /* template class ang::object_wrapper<vector_buffer> :             */
	 /*  -> specialization of object_wrapper<vector_buffer> -> array    */
	 /******************************************************************/
	template<template<typename> class A>
	class object_wrapper<collections::vector_buffer<var, A>>
	{
	public:
		typedef collections::vector_buffer<var, A> type;
		typedef typename collections::vector_buffer<var, A>::type data_type;

	private:
		type* m_ptr;

	public:
		object_wrapper();
		template<typename U> inline object_wrapper(array_view<U> const& ar) : m_ptr(null) {
			set(new type());
			for (U& val : ar) {
				get()->push(var(val));
			}
		}
		template<typename U, template<typename> class allocator2> inline object_wrapper(scope_array<U, allocator2> const& ar) : m_ptr(null) {
			set(new type());
			for (U& val : ar) {
				get()->push(var(val));
			}
		}
		template<typename U, wsize SIZE> inline object_wrapper(stack_array<U, SIZE> const& ar) : m_ptr(null) {
			set(new type());
			for (U& val : ar) {
				get()->push(var(val));
			}
		}
		
		object_wrapper(ang::initializer_list<var> list);
		object_wrapper(wsize reserve);
		object_wrapper(type*);
		object_wrapper(const collections::ienum<data_type>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		template<typename U, wsize N>
		object_wrapper(U(&ar)[N]) : m_ptr(null) {
			set(new type(to_array(ar, N)));
		}
		~object_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type** addres_of(void);
		type** addres_for_init(void);

		collections::forward_iterator<var> begin() { return m_ptr ? m_ptr->begin() : collections::iterator<var>(); }
		collections::forward_iterator<var> end() { return m_ptr ? m_ptr->end() : collections::iterator<var>(); }
		collections::forward_iterator<const var> begin()const { return m_ptr ? m_ptr->begin() : collections::iterator<const var>(); }
		collections::forward_iterator<const var> end()const { return m_ptr ? m_ptr->end() : collections::iterator<const var>(); }

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator = (var(&ar)[N]) {
			set(new type(to_array(ar, N)));
			return *this;
		}

		object_wrapper& operator += (var items);
		object_wrapper& operator += (collections::ienum<data_type> const* items);
		object_wrapper& operator += (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator += (var(&ar)[N]) {
			if (is_empty())set(new type(to_array(ar, N)));
			else m_ptr->extend(to_array(ar, N));
			return *this;
		}

		object_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		explicit operator type * (void);
		explicit operator type const* (void)const;
		operator array_view<var>()const { return m_ptr ? collections::to_array(m_ptr->data(), m_ptr->size()) : array_view<var>(); }
		operator array_view<const var>()const { return m_ptr ? collections::to_array((const var*)m_ptr->data(), m_ptr->size()) : array_view<const var>(); }
		template<typename I>var& operator[](I const& idx);
		template<typename I>var const& operator[](I const& idx)const;
	};


	template<typename T, template<typename>class A>
	struct property_helper<vector<T, A>, smart_type::none> {
		using type = vector<T, A>;
		using ret_type = type & ;
		using ptr_type = collections::vector_buffer<T, A>*;
		using arg_type = array_view<T>;
		using property_class = base_property<type>;
		using getter_type = ret_type(*)(property_class const*);
		using setter_type = void(*)(property_class const*, arg_type);
	};

	template<typename T, template<typename>class A>
	struct property_helper<const vector<T, A>, smart_type::none> {
		using type = vector<T, A>;
		using ret_type = array_view<const T>;
		using ptr_type = collections::vector_buffer<T, A>*;
		using arg_type = array_view<T>;
		using property_class = base_property<type>;
		using getter_type = ret_type(*)(property_class const*);
		using setter_type = void(*)(property_class const*, arg_type);
	};


}//ang


#endif //__ANG_COLLECTIONS_VECTOR_H__