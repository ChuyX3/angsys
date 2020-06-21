/*********************************************************************************************************************/
/*   File Name: ang/collections/list.h                                                                               */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: Implements list object                                                                        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/
#ifndef __ANG_COLLECTIONS_LIST_H__
#define __ANG_COLLECTIONS_LIST_H__

#include <angsys.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANG_DYNAMIC_LIBRARY

#ifdef ANG_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_EXPORTS
#else//#elif defined ANG_STATIC_LIBRARY
#define LINK
#endif//ANG_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

namespace ang
{
	namespace collections
	{
		template<typename T, template<typename>class A = memory::default_allocator> class list_object;
		template<typename T, template<typename>class A = memory::default_allocator> using smart_list = object_wrapper<list_object<T, A>>;
	}

	template<typename T, template<typename>class A>
	struct smart_ptr_type<collections::list<T, A>, false, false> {
		static constexpr ang::smart_type smart_type = ang::smart_type::none;
		using smart_ptr_t = collections::smart_list<T>;
		using type = collections::list_object<T, A>;
	};
	
	namespace collections
	{
		/******************************************************************/
		/* template class list_object :                                   */
		/*  -> implements a linked list object                            */
		/******************************************************************/
		template<typename T, template<typename>class A>
		class list_object
			: public implement<list_object<T,A>
			, iid("ang::collections::list")
			, ilist<T>
			, ifinder<T>
			, ienum<T>>
			, public list<typename auto_type<T>::type, A>
		{
		public:
			static_assert(!is_const<T>::value, "T is constant type");
			using element_type = typename auto_type<T>::type;
		
			using ilist_type = ilist<T>;
			using ienum_type = ienum<T>;
			using base_iterator_t = collections::base_iterator<T>;
			using iterator_t = collections::iterator<T>;
			using const_iterator_t = collections::const_iterator<T>;
			using forward_iterator_t = collections::forward_iterator<T>;
			using const_forward_iterator_t = collections::const_forward_iterator<T>;
			using backward_iterator_t = collections::backward_iterator<T>;
			using const_backward_iterator_t = collections::const_backward_iterator<T>;

			using fast_list_t = list<typename auto_type<T>::type, A>;
			using node_type = doubly_linked_node<element_type>;
			using node_type_ptr = doubly_linked_node<element_type>*;

		public:
			list_object();
			list_object(const ang::nullptr_t&);
			list_object(wsize sz);
			list_object(list<element_type, A> && ar);
			list_object(list<element_type, A> const& ar);
			list_object(list_object&& ar);
			list_object(const list_object& ar);
			list_object(const ienum_ptr<T>& store);
			template<typename T2>
			list_object(ang::initializer_list<T2> list);
			template<typename T2>
			list_object(array_view<T2> const&);
			template<typename IT>
			list_object(IT first, IT last);

		private:
			virtual ~list_object();

		public: /*methods*/
			using list::copy;
			using list::exten;
			template<typename T2> void copy(ienum_ptr<T2> const&);
			template<typename T2> void exten(ienum_ptr<T2> const&);

		protected: /*overrides*/
			void dispose()override;

		public: /*overrides*/
			string to_string()const override;
			string to_string(text::text_format_t)const override;

		public: /*ienum overrides*/
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

		public: /*ilist overrides*/
			void copy(const ienum_ptr<T>&) override;
			void extend(const ienum_ptr<T>&) override;
			void push_front(typename auto_type<T>::arg_type &&) override;
			void push_front(typename auto_type<T>::arg_type const&) override;
			void push_back(typename auto_type<T>::arg_type &&) override;
			void push_back(typename auto_type<T>::arg_type const&) override;
			iterator_t insert(base_iterator_t it, typename auto_type<T>::arg_type &&) override;
			iterator_t insert(base_iterator_t it, typename auto_type<T>::arg_type const&) override;
			bool pop_front(typename auto_type<T>::arg_ptr_type = null) override;
			bool pop_back(typename auto_type<T>::arg_ptr_type = null) override;
			iterator_t remove(base_iterator_t it, typename auto_type<T>::arg_ptr_type = null) override;
		};
	}

	/******************************************************************/
	/* template class ang::object_wrapper<list_object> :            */
	/*  -> specialization of object_wrapper<list_object> -> list      */
	/******************************************************************/
	template<typename T, template<typename> class A>
	class object_wrapper<collections::list_object<T, A>>
	{
	public:
		typedef collections::list_object<T, A> type;
		typedef typename collections::list_object<T, A>::element_type element_type;

	private:
		collections::list_object<T, A>* m_ptr;

	public:
		object_wrapper();
		object_wrapper(type* ar);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(const ang::nullptr_t&);
		object_wrapper(wsize sz);
		object_wrapper(collections::list<T, A> && ar);
		object_wrapper(collections::list<T, A> const& ar);
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
			set(new type(to_array(ar, N)));
		}
		~object_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		collections::list_object<T, A>* get(void)const;
		void set(collections::list_object<T, A>*);
		collections::list_object<T, A>** addres_of(void);

		collections::forward_iterator<T> begin() { return m_ptr ? m_ptr->begin() : collections::iterator<T>(); }
		collections::forward_iterator<T> end() { return m_ptr ? m_ptr->end() : collections::iterator<T>(); }
		collections::forward_iterator<const T> begin()const { return m_ptr ? m_ptr->begin() : collections::iterator<const T>(); }
		collections::forward_iterator<const T> end()const { return m_ptr ? m_ptr->end() : collections::iterator<const T>(); }

	public:
		object_wrapper& operator = (collections::list_object<T, A>*);
		object_wrapper& operator = (const ang::nullptr_t&);

		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator = (typename collections::auto_type<T>::type(&ar)[N]) {
			if (is_empty())
				set(new collections::list_object<T, A>(collections::to_array(ar, N)));
			else
				get()->copy(to_array(ar, N));
			return *this;
		}
		template<typename T2>
		object_wrapper& operator = (collections::ienum_ptr<T2> const items) {
			if (is_empty())
				set(new collections::list_object<T, A>());
			get()->copy(items);
			return *this;
		}
		object_wrapper& operator += (typename collections::auto_type<T>::arg_type && item) {
			if (is_empty())
				set(new collections::list_object<T, A>());
			get()->push_back(ang::forward<typename collections::auto_type<T>::arg_type>(item));
			return *this;
		}
		object_wrapper& operator += (typename collections::auto_type<T>::arg_type const& item) {
			if (is_empty())
				set(new collections::list_object<T, A>());
			get()->push_back(item);
			return *this;
		}
		object_wrapper_ptr<collections::list_object<T, A>> operator & (void);
		collections::list_object<T, A> * operator -> (void)const;
		explicit operator collections::list_object<T, A> * (void);
		explicit operator collections::list_object<T, A> const* (void)const;
	};
}

#include <ang/collections/inline/list.inl>

#endif//__ANG_COLLECTIONS_LIST_H__
