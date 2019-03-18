/*********************************************************************************************************************/
/*   File Name: ang/collections/inline/list_specialization.hpp                                                                             */
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
#ifndef __ANG_COLLECTION_LIST_H__
#else

namespace ang
{
	namespace collections
	{

		
		template<> class MY_LINKAGE list_object<MY_TYPE, MY_ALLOC> final
			: public smart<list_object<MY_TYPE, MY_ALLOC>, ilist<MY_TYPE>>
		{
		public:
			typedef MY_TYPE								type;
			typedef list_object<MY_TYPE, MY_ALLOC>		self_t;
			typedef double_linked_node<MY_TYPE>			node_t, *node_ptr_t;
			typedef MY_ALLOC<node_t>					allocator_t;
			typedef ilist<MY_TYPE>						ilist_t;
			typedef iarray<MY_TYPE>						iarray_t;
			typedef ienum<MY_TYPE>						ienum_t;
			typedef base_iterator<MY_TYPE>				base_iterator_t;
			typedef iterator<MY_TYPE>					iterator_t;
			typedef const_iterator<MY_TYPE>				const_iterator_t;
			typedef forward_iterator<MY_TYPE>			forward_iterator_t;
			typedef const_forward_iterator<MY_TYPE>		const_forward_iterator_t;
			typedef backward_iterator<MY_TYPE>			backward_iterator_t;
			typedef const_backward_iterator<MY_TYPE>	const_backward_iterator_t;


		protected:
			//static allocator_t m_alloc;
			wsize m_count;
			node_ptr_t m_head;
			node_ptr_t m_tail;

		public:
			list_object();
			list_object(const ang::nullptr_t&);
			list_object(list_object&&);
			list_object(const list_object&);
			list_object(const list_object*);
			list_object(const ienum_t*);

			template<typename U> inline list_object(ang::initializer_list<U> items) 
				: list_object() {
				for (U& item : items)
					push((MY_TYPE)item);
			}

			template<typename U> inline list_object(array_view<U> const& items)
				: list_object() {
				for (U& item : items)
					push((MY_TYPE)item);
			}

			template<typename U, template<typename> class A2> inline list_object(scope_array<U, A2> const& items)
				: list_object() {
				for (U& item : items)
					push((MY_TYPE)item);
			}

			template<typename U, wsize SIZE> inline list_object(stack_array<U, SIZE> const& items)
				: list_object() {
				for (U& item : items)
					push((MY_TYPE)item);
			}

		private:
			virtual ~list_object();

		public: //overrides
			ANG_DECLARE_INTERFACE();
			comparision_result_t compare(const object*)const override;

			void clear()override;
			bool is_empty()const;
			bool move(list_object<MY_TYPE, MY_ALLOC>&);

			template<typename U> inline void copy(array_view<U>const& items) {
				clear();
				for (U& item : items)
					push((MY_TYPE)item);
			}
			template<typename U, template<typename> class A2> inline void copy(scope_array<U, A2>const& items) {
				clear();
				for (U& item : items)
					push((MY_TYPE)item);
			}
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const& items) {
				clear();
				for (U& item : items)
					push((MY_TYPE)item);
			}

			template<typename U> inline void extend(array_view<U>const& items) {
				for (U& item : items)
					push((MY_TYPE)item);
			}
			template<typename U, template<typename> class A2> inline void extend(scope_array<U, A2>const& items) {
				for (U& item : items)
					push((MY_TYPE)item);
			}
			template<typename U, wsize SIZE> inline void extend(stack_array<U, SIZE> const& items) {
				for (U& item : items)
					push((MY_TYPE)item);
			}

		public: //ienum overrides
			wsize counter()const override;

			MY_TYPE& at(base_iterator_t const&) override;
			bool increase(base_iterator_t&)const override;
			bool increase(base_iterator_t&, int offset)const override;
			bool decrease(base_iterator_t&)const override;
			bool decrease(base_iterator_t&, int offset)const override;

			forward_iterator_t begin() override;
			forward_iterator_t end() override;
			const_forward_iterator_t begin()const override;
			const_forward_iterator_t end()const override;
			forward_iterator_t last() override;
			const_forward_iterator_t last()const override;

			backward_iterator_t rbegin() override;
			backward_iterator_t rend() override;
			const_backward_iterator_t rbegin()const override;
			const_backward_iterator_t rend()const override;

		public: //iarray overrides
			iterator_t at(windex) override;
			const_iterator_t at(windex)const override;
			void copy(const ienum<MY_TYPE>*) override;
			iterator_t find(core::delegates::function<bool(MY_TYPE const&)>, bool invert = false)const override;
			iterator_t find(core::delegates::function<bool(MY_TYPE const&)>, base_iterator_t next_to, bool invert = false)const override;
			collections::ienum_ptr<MY_TYPE> find_all(core::delegates::function<bool(MY_TYPE const&)>)const override;

		public: //ilist overrides
			void extend(const ienum<MY_TYPE>*) override;
			void push(MY_TYPE const&, bool last = true) override;
			bool insert(windex idx, MY_TYPE const&) override;
			bool insert(base_iterator_t it, MY_TYPE const&) override;
			bool pop(bool last = true) override;
			bool pop(MY_TYPE&, bool last = true) override;
			bool pop_at(windex idx) override;
			bool pop_at(base_iterator_t it) override;
			bool pop_at(windex idx, MY_TYPE&) override;
			bool pop_at(base_iterator_t it, MY_TYPE&) override;
		
		};

	}

	/******************************************************************/
	/* template class ang::object_wrapper<list_object> :              */
	/*  -> specialization of object_wrapper<list_object> -> list      */
	/******************************************************************/
	template<> class MY_LINKAGE object_wrapper<collections::list_object<MY_TYPE, MY_ALLOC>>
	{
	public:
		typedef collections::list_object<MY_TYPE, MY_ALLOC> type;
		typedef typename collections::list_object<MY_TYPE, MY_ALLOC>::type data_type;

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
		template<typename U> object_wrapper(ang::initializer_list<U> list) : m_ptr(null) {
			set(new type(list));
		}
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

		collections::forward_iterator<MY_TYPE> begin() { return m_ptr ? m_ptr->begin() : collections::iterator<MY_TYPE>(); }
		collections::forward_iterator<MY_TYPE> end() { return m_ptr ? m_ptr->end() : collections::iterator<MY_TYPE>(); }
		collections::forward_iterator<const MY_TYPE> begin()const { return m_ptr ? m_ptr->begin() : collections::iterator<MY_TYPE>(); }
		collections::forward_iterator<const MY_TYPE> end()const { return m_ptr ? m_ptr->end() : collections::iterator<MY_TYPE>(); }

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator = (MY_TYPE(&ar)[N]) {
			set(new type(to_array(ar, N)));
			return *this;
		}

		object_wrapper& operator += (MY_TYPE items);
		object_wrapper& operator += (collections::ienum<data_type> const* items);
		object_wrapper& operator += (object_wrapper const&);
		template<wsize N> object_wrapper& operator += (MY_TYPE(&ar)[N]) {
			if (is_empty())
				set(new type());
			for(MY_TYPE const& item : ar)
				m_ptr->push(item);
			return *this;
		}

		object_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		explicit operator type * (void);
		explicit operator type const* (void)const;

		template<typename I>MY_TYPE& operator[](I const& idx);
		template<typename I>MY_TYPE const& operator[](I const& idx)const;
	};

}


#endif//__ANG_COLLECTION_H__
