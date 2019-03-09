/*********************************************************************************************************************/
/*   File Name: ang/collections/list.h                                                                             */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: Implements list object                                                                      */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/
#ifndef __ANG_COLLECTIONS_H__
#elif !defined __ANG_COLLECTION_LIST_H__
#define __ANG_COLLECTION_LIST_H__


namespace ang
{
	
	namespace collections
	{
		template<typename T, template<typename> class allocator = memory::default_allocator> class list_object;
		template<typename T, template<typename> class allocator = memory::default_allocator> using list = object_wrapper<list_object<T, allocator>>;

		/******************************************************************/
		/* template class list_object :                                   */
		/*  -> implements a linked list object object                     */
		/******************************************************************/
		template<typename T, template<typename> class A>
		class list_object final
			: public smart<list_object<T, A>, ilist<T>>
		{
		public:
			typedef T							type;
			typedef list_object<T, A>			self_t;
			typedef double_linked_node<T>		node_t, *node_ptr_t;
			typedef A<node_t>					allocator_t;
			typedef ilist<T>					ilist_t;
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
			allocator_t m_alloc;
			wsize m_count;
			node_ptr_t m_head;
			node_ptr_t m_tail;

		public:
			inline list_object();
			inline list_object(const ang::nullptr_t&);
			inline list_object(list_object&&);
			inline list_object(const list_object&);
			inline list_object(const list_object*);
			inline list_object(const ienum_t*);
			template<typename U> inline list_object(ang::initializer_list<U> list);
			template<typename U> inline list_object(array_view<U> const&);
			template<typename U, template<typename> class A2> inline list_object(scope_array<U, A2> const&);
			template<typename U, wsize SIZE> inline list_object(stack_array<U, SIZE> const&);

		private:
			virtual ~list_object();

		public: //overrides
			ANG_DECLARE_INTERFACE();
			inline comparision_result_t compare(const object*)const override;

			inline void clear()override;
			inline bool is_empty()const;
			inline bool move(list_object<T, A>&);

			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class A2> inline void copy(scope_array<U, A2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

			template<typename U> inline void extend(array_view<U>const&);
			template<typename U, template<typename> class A2> inline void extend(scope_array<U, A2>const&);
			template<typename U, wsize SIZE> inline void extend(stack_array<U, SIZE> const&);

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
		
		};


	}

	/******************************************************************/
	/* template class ang::object_wrapper<list_object> :              */
	/*  -> specialization of object_wrapper<list_object> -> list      */
	/******************************************************************/
	template<typename T, template<typename> class allocator>
	class object_wrapper<collections::list_object<T, allocator>>
	{
	public:
		typedef collections::list_object<T, allocator> type;
		typedef typename collections::list_object<T, allocator>::type data_type;

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
			if (is_empty())set(new type(to_array(ar, N)));
			else m_ptr->extend(to_array(ar, N));
			return *this;
		}

		object_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		explicit operator type * (void);
		explicit operator type const* (void)const;
		template<typename I>T& operator[](I const& idx);
		template<typename I>T const& operator[](I const& idx)const;
	};

}


#endif//__ANG_COLLECTION_LIST_H__
