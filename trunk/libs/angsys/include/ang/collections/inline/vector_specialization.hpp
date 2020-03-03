/*********************************************************************************************************************/
/*   File Name: coffe/collections/inline/list_specialization.hpp                                                                             */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: Implements list object                                                                      */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Copyright (C) coffe sys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/
#ifndef __COFFE_COLLECTIONS_VECTOR_H__
#else
namespace coffe
{
	namespace collections
	{
		/******************************************************************/
		/* template class vector_buffer :                                 */
		/*  -> implements the buffer memory of vector object              */
		/******************************************************************/
		template<>
		class MY_LINKAGE vector_buffer<MY_TYPE, MY_ALLOC > final
			: public smart<vector_buffer<MY_TYPE, MY_ALLOC>, ivariant, ibuffer, ilist<MY_TYPE>, ienum<MY_TYPE>>
		{
		public:
			using type							= MY_TYPE;
			template<typename T> using A		= MY_ALLOC<T>;
			using alloc_t						= A<type>;
			using ilist_t						= ilist<type>;
			using iarray_t						= iarray<type>;
			using ienum_type					= ienum<type>;
			using base_iterator_t				= base_iterator<type>;
			using iterator_t					= iterator<type>;
			using const_iterator_t				= const_iterator<type>;
			using forward_iterator_t			= forward_iterator<type>;
			using const_forward_iterator_t		= const_forward_iterator<type>;
			using backward_iterator_t			= backward_iterator<type>;
			using const_backward_iterator_t		= const_backward_iterator<type>;

		protected:
			wsize m_size;
			wsize m_capacity;
			type* m_data;

		public:
			vector_buffer();
			vector_buffer(wsize reserve);
			vector_buffer(const coffe::nullptr_t&);
			vector_buffer(vector_buffer&& ar);
			vector_buffer(const vector_buffer& ar);
			vector_buffer(const vector_buffer* ar);
			vector_buffer(const ienum_type* store);
			template<typename U>inline vector_buffer(coffe::initializer_list<U> items)
				: vector_buffer() {
				for (U const& item : items)
					push((type)item);
			}
			template<typename U> inline vector_buffer(array_view<U> const& items)
				: vector_buffer() {
				for (U const& item : items)
					push((type)item);
			}
			template<typename U, template<typename> class allocator2> inline vector_buffer(scope_array<U, allocator2> const& items)
				: vector_buffer() {
				for (U const& item : items)
					push((type)item);
			}
			template<typename U, wsize SIZE> inline vector_buffer(stack_array<U, SIZE> const& items)
				: vector_buffer() {
				for (U const& item : items)
					push((type)item);
			}

		private:
			virtual ~vector_buffer();

		protected: //overrides
			void dispose()override;

		public: //methods
			bool is_empty()const;
			type* data()const;
			wsize size()const;
			void size(wsize, bool save);
			wsize capacity()const;
			void capacity(wsize size, bool save = false);

			void clear();
			void empty();
			bool move(self&);

			template<typename U> inline void copy(array_view<U>const& items) {
				clear();
				for (U const& item : items)
					push((type)item);
			}
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const& items) {
				clear();
				for (U const& item : items)
					push((type)item);
			}
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const& items) {
				clear();
				for (U const& item : items)
					push((type)item);
			}

			template<typename U> inline void extend(array_view<U>const& items) {
				for (U const& item : items)
					push((type)item);
			}
			template<typename U, template<typename> class allocator2> inline void extend(scope_array<U, allocator2>const& items) {
				for (U const& item : items)
					push((type)item);
			}
			template<typename U, wsize SIZE> inline void extend(stack_array<U, SIZE> const& items) {
				for (U const& item : items)
					push((type)item);
			}

		public: //ivariant overrides
			rtti_t const& value_type()const override;
			bool set_value(rtti_t const&, unknown_t)override;
			bool get_value(rtti_t const&, unknown_t)const override;
			variant clone()const override;
			string to_string()const override;
			string to_string(text::text_format_t)const override;

		public: //ibuffer overrides
			bool is_readonly()const override;
			text::encoding_t encoding()const override;
			pointer buffer_ptr() override;
			const_pointer buffer_ptr()const override;
			wsize buffer_size()const override;
			wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override;
			ibuffer_view_t map_buffer(windex, wsize) override;
			bool unmap_buffer(ibuffer_view_t&, wsize) override;
			bool can_realloc_buffer()const override;
			bool realloc_buffer(wsize) override;

		public: //ienum overrides
			wsize counter()const override;

			type& at(base_iterator_t const&) override;
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
			ienum_ptr<MY_TYPE> enumerate(iteration_method_t = iteration_method::def)const override;
			iterator_t at(windex) override;
			const_iterator_t at(windex)const override;
			void copy(const ienum<type>*) override;
			iterator_t find(core::delegates::function<bool(type const&)>, bool invert = false)const override;
			iterator_t find(core::delegates::function<bool(type const&)>, base_iterator_t next_to, bool invert = false)const override;
			collections::ienum_ptr<type> find_all(core::delegates::function<bool(type const&)>)const override;
			template<typename U> 
			inline collections::ienum_ptr<U> find_all(core::delegates::function<bool(type const& val, U& out)>)const {
				vector<U, A> out = new vector_buffer<U, A>();
				if (!is_empty())
				{
					for (auto i = size(); i > 0; --i)
					{
						U val = default_value<U>::value;
						if (cond(m_data[i - 1], val))
							out += coffe::move(val);
					}
				}
				return out->is_empty() ? null : out.get();
			}

		public: //ilist overrides
			void extend(const ienum<type>*) override;
			void push(type const&, bool last = true) override;
			bool insert(windex idx, type const&) override;
			bool insert(base_iterator_t it, type const&) override;
			bool pop(bool last = true) override;
			bool pop(type&, bool last = true) override;
			bool pop_at(windex idx) override;
			bool pop_at(base_iterator_t it) override;
			bool pop_at(windex idx, type&) override;
			bool pop_at(base_iterator_t it, type&) override;

		public: //overrides
			COFFE_DECLARE_INTERFACE();
			comparision_result_t compare(const object*)const override;

		protected: //Memory Operations
			bool realloc(wsize size, bool save = true);
		};

	}//collections


	/******************************************************************/
	/* template class coffe::object_wrapper<vector_buffer> :            */
	/*  -> specialization of object_wrapper<vector_buffer> -> array   */
	/******************************************************************/
	template<>
	class MY_LINKAGE object_wrapper<collections::vector_buffer<MY_TYPE, MY_ALLOC>>
	{
	public:
		typedef collections::vector_buffer<MY_TYPE, MY_ALLOC> type;
		typedef typename collections::vector_buffer<MY_TYPE, MY_ALLOC>::type data_type;

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
		template<typename U> object_wrapper(coffe::initializer_list<U> list) : m_ptr(null) {
			set(new type(list));
		}
		explicit object_wrapper(wsize reserve);
		object_wrapper(type*);
		object_wrapper(const collections::ienum<data_type>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(coffe::nullptr_t const&);
		template<typename U, wsize N>
		inline object_wrapper(U(&ar)[N]) : m_ptr(null) {
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

		inline collections::forward_iterator<data_type> begin() { return m_ptr ? m_ptr->begin() : collections::iterator<data_type>(); }
		inline collections::forward_iterator<data_type> end() { return m_ptr ? m_ptr->end() : collections::iterator<data_type>(); }
		inline collections::forward_iterator<const data_type> begin()const { return m_ptr ? ((type const*) m_ptr)->begin() : collections::iterator<const data_type>(); }
		inline collections::forward_iterator<const data_type> end()const { return m_ptr ? ((type const*)m_ptr)->end() : collections::iterator<const data_type>(); }

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (const coffe::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		inline object_wrapper& operator = (data_type(&ar)[N]) {
			set(new type(to_array(ar, N)));
			return *this;
		}

		object_wrapper& operator += (data_type items);
		object_wrapper& operator += (collections::ienum<data_type> const* items);
		object_wrapper& operator += (object_wrapper const&);
		template<wsize N>
		inline object_wrapper& operator += (data_type(&ar)[N]) {
			if (is_empty())set(new type(to_array(ar, N)));
			else m_ptr->extend(to_array(ar, N));
			return *this;
		}

		object_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		explicit operator type * (void);
		explicit operator type const* (void)const;
		operator array_view<data_type>()const { return m_ptr ? collections::to_array(m_ptr->data(), m_ptr->size()) : array_view<data_type>(); }
		operator array_view<const data_type>()const { return m_ptr ? collections::to_array((const data_type*)m_ptr->data(), m_ptr->size()) : array_view<const data_type>(); }
		template<typename I> inline data_type& operator[](I const& i) {
			static_assert(is_integer_value<I>::value, "no integer value is no accepted");
		#ifdef DEBUG_SAFE_CODE
			if (is_empty())
				throw_exception(error_code::invalid_memory);
			if ((i >= m_ptr->size()) || (i < 0)) 
				throw_exception(error_code::array_overflow);
		#endif
			return m_ptr->data()[i];
		}
		template<typename I> inline data_type const& operator[](I const& i)const {
			static_assert(is_integer_value<I>::value, "no integer value is no accepted");
		#ifdef DEBUG_SAFE_CODE
			if (is_empty()) 
				throw_exception(error_code::invalid_memory);
			if ((i >= (I)m_ptr->size()) || (i < 0))
				throw_exception(error_code::array_overflow);
		#endif
			return m_ptr->data()[i];
		}

	};

}


#endif//__COFFE_COLLECTION_H__
