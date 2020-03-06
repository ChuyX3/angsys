/*********************************************************************************************************************/
/*   File Name: ang/collections/array.h                                                                              */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_COLLECTIONS_H__
#elif !defined __ANG_COLLECTIONS_ARRAY_H__
#define __ANG_COLLECTIONS_ARRAY_H__

namespace ang
{
	template<typename T>
	struct smart_ptr_type<T[], false, false> {
		static constexpr ang::smart_type smart_type = ang::smart_type::none;
		using smart_ptr_t = smart_array<T>;
		using type = collections::array_buffer<T>;
	};

	template<typename T, wsize N>
	struct smart_ptr_type<T[N], false, false> {
		static constexpr ang::smart_type smart_type = ang::smart_type::none;
		using smart_ptr_t = smart_array<T>;
		using type = collections::array_buffer<T>;
	};

	template<typename T, template<typename>class A>
	struct smart_ptr_type<array<T, A>, false, false> {
		static constexpr ang::smart_type smart_type = ang::smart_type::none;
		using smart_ptr_t = smart_array<T>;
		using type = collections::array_buffer<T, A>;
	};

	namespace collections
	{
		/******************************************************************/
		/* template class array_buffer :                                  */
		/*  -> implements the memory buffer of an array object            */
		/******************************************************************/
		template<typename T, template<typename> class A>
		class array_buffer final
			: public implement<array_buffer<T, A>
			, iid("ang::collections::array")
			, ivariable
			, ibuffer
			, ibuffer_view
			, iarray<T>
			, ifinder<T>
			, ienum<T>>
			, public array<typename auto_type<T>::type, A>
		{
		public:
			static_assert(!is_const<T>::value, "T is constant type");
			using element_type = typename auto_type<T>::type;
			using type = element_type *;
			using const_type = element_type const*;
			using iarray_t = iarray<T>;
			using ienum_type = ienum<T>;
			using base_iterator_t = collections::base_iterator<T>;
			using iterator_t = collections::iterator<T>;
			using const_iterator_t = collections::const_iterator<T>;
			using forward_iterator_t = collections::forward_iterator<T>;
			using const_forward_iterator_t = collections::const_forward_iterator<T>;
			using backward_iterator_t = collections::backward_iterator<T>;
			using const_backward_iterator_t = collections::const_backward_iterator<T>;

		public:
			array_buffer();
			array_buffer(const ang::nullptr_t&);
			array_buffer(wsize sz);
			array_buffer(array<element_type, A> && ar);
			array_buffer(array<element_type, A> const& ar);
			array_buffer(array_buffer&& ar);
			array_buffer(const array_buffer& ar);
			array_buffer(const ienum_ptr<T>& store);
			template<typename T2>
			array_buffer(ang::initializer_list<T2> list);
			template<typename T2>
			array_buffer(array_view<T2> const&);
			template<typename IT>
			array_buffer(IT first, IT last);
		private:
			virtual ~array_buffer();

		public: /*methods*/
			using array::copy;
			template<typename T2> void copy(ienum_ptr<T2> const&);

		protected: //overrides
			void dispose()override;

		private: /*ivariable overrides*/
			rtti_t const& value_type()const override;
			bool set_value(rtti_t const&, unknown_t)override;
			bool get_value(rtti_t const&, unknown_t)const override;
			variant clone()const override;
		
		public: /*ivariable overrides*/
			string to_string()const override;
			string to_string(text::text_format_t)const override;

		private: /*ibuffer overrides*/
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
			typename auto_type<T>::return_type at(base_iterator_t const&) override;
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

			iterator_t find(function<bool(typename auto_type<T>::arg_type const&)>, bool invert = false)const override;
			iterator_t find(function<bool(typename auto_type<T>::arg_type const&)>, base_iterator_t next_to, bool invert = false)const override;
			ienum_ptr<T> find_all(function<bool(typename auto_type<T>::arg_type const&)>)const override;

		public: //iarray overrides
			iterator_t at(windex) override;
			const_iterator_t at(windex)const override;
			void copy(const ienum_ptr<T>&) override;		

		};

	}//collections

	/******************************************************************/
	/* template class ang::object_wrapper<array_buffer> :           */
	/*  -> specialization of object_wrapper<array_buffer> -> array    */
	/******************************************************************/
	template<typename T, template<typename> class A>
	class object_wrapper<collections::array_buffer<T, A>>
	{
	public:
		typedef collections::array_buffer<T, A> type;
		typedef typename collections::array_buffer<T, A>::type data_type;
		typedef typename collections::array_buffer<T, A>::element_type element_type;

	private:
		collections::array_buffer<T, A>* m_ptr;

	public:
		object_wrapper();
		object_wrapper(type* ar);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(const ang::nullptr_t&);
		object_wrapper(data_type arr, wsize sz);
		object_wrapper(wsize sz);
		object_wrapper(array<typename collections::auto_type<T>::type, A> && ar);
		object_wrapper(array<typename collections::auto_type<T>::type, A> const& ar);
		object_wrapper(const collections::ienum_ptr<T>& store);

		template<typename T2>
		object_wrapper(ang::initializer_list<T2> list)
			: m_ptr(null) {
			set(new type(ang::forward<ang::initializer_list<T2>>(list)));
		}
		template<typename T2>
		object_wrapper(array_view<T2> const& ar)
			: m_ptr(null) {
			set(new type(ar));
		}
		template<typename T2, wsize N>
		object_wrapper(T2(&ar)[N])
			: m_ptr(null) {
			set(new type(ang::to_array(ang::forward<T2(&)[N]>(ar))));
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
		object_wrapper& operator = (typename collections::auto_type<T>::type(&ar)[N]) {
			if (is_empty())
				set(new collections::array_buffer<T, A>(ang::to_array(ang::forward<typename collections::auto_type<T>::type(&)[N]>(ar))));
			else
				get()->copy(ang::to_array(ang::forward<typename collections::auto_type<T>::type(&)[N]>(ar)));
			return *this;
		}
		template<typename T2>
		object_wrapper& operator = (collections::ienum_ptr<T2> const items) {
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
		operator array_view<T>()const { return m_ptr ? to_array(m_ptr->begin(), m_ptr->end()) : array_view<T>(); }
		operator array_view<const T>()const { return m_ptr ? to_array((const T*)m_ptr->begin(), m_ptr->end()) : array_view<T>(); }
		template<typename I>typename collections::auto_type<T>::type& operator[](I const& idx);
		template<typename I>typename collections::auto_type<T>::type const& operator[](I const& idx)const;
	};

	template<typename T, template<typename>class A>
	struct property_helper<smart_array<T,A>, smart_type::none> {
		using type = smart_array<T, A>;
		using ret_type = type&;
		using ptr_type = collections::array_buffer<T, A>*;
		using arg_type = array_view<T>;
		using property_class = base_property;
		using getter_type = ret_type(*)(property_class const*);
		using setter_type = void(*)(property_class*, arg_type);
	};

	template<typename T, template<typename>class A>
	struct property_helper<const smart_array<T, A>, smart_type::none> {
		using type = smart_array<T, A>;
		using ret_type = array_view<const T>;
		using ptr_type = collections::array_buffer<T,A>*;
		using arg_type = array_view<T>;
		using property_class = base_property;
		using getter_type = ret_type(*)(property_class const*);
		using setter_type = void(*)(property_class*, arg_type);
	};

}//ang


#endif //__ANG_COLLECTIONS_ITERATORS_H__
