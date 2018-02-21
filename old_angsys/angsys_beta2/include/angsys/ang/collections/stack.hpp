/*********************************************************************************************************************/
/*   File Name: ang/collections/queue.hpp                                                                             */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: Implements queue object                                                                      */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_STACK_HPP__
#define __ANG_STACK_HPP__

#include <angsys.hpp>


#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace collections
	{
		template<typename T, template<typename> class allocator = memory::default_allocator> class stack_object;
		template<typename T, template<typename> class allocator = memory::default_allocator> using stack = object_wrapper<stack_object<T, allocator>>;
	}

	namespace collections
	{
		/******************************************************************/
		/* template class stack_object :                                 */
		/*  -> implements the buffer memory of stack object              */
		/******************************************************************/
		template<typename T, template<typename> class allocator>
		class stack_object final
			: public object
			, public isequence<T>
		{
		public:
			typedef T							type;
			typedef stack_object<T, allocator>	self_t;
			typedef ienum<T>					ienum_t;
			typedef isequence<T>				isequence_t;
			typedef base_iterator<T>			base_iterator_t;
			typedef iterator<T>					iterator_t;
			typedef const_iterator<T>			const_iterator_t;
			typedef forward_iterator<T>			forward_iterator_t;
			typedef const_forward_iterator<T>	const_forward_iterator_t;
			typedef backward_iterator<T>		backward_iterator_t;
			typedef const_backward_iterator<T>	const_backward_iterator_t;


		protected:
			typedef linked_node<T> node_t;
			typedef linked_node<T>* node_ptr_t;

			wsize _size;
			node_ptr_t _head;
			node_ptr_t _tail;

		public:
			inline stack_object();
			template<typename U>inline stack_object(ang::initializer_list_t<U> list);
			inline stack_object(const ang::nullptr_t&);
			inline stack_object(stack_object&& ar);
			inline stack_object(const stack_object& ar);
			inline stack_object(const stack_object* ar);
			inline stack_object(const ienum_t* store);

		private:
			virtual ~stack_object();

		public: //methods
			inline bool is_empty()const;
			inline wsize size()const;

			inline void clean();
			inline bool move(stack_object<T, allocator>&);

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

			inline backward_iterator_t rbegin() override { return backward_iterator_t(this); } //unsupported
			inline backward_iterator_t rend() override { return backward_iterator_t(this); } //unsupported
			inline const_backward_iterator_t rbegin()const override { return const_backward_iterator_t(const_cast<self_t*>(this)); } //unsupported
			inline const_backward_iterator_t rend()const override { return const_backward_iterator_t(const_cast<self_t*>(this)); } //unsupported

		public: //isequence overrides
			inline void extend(const ienum<T>*) override;
			inline void push(T const&) override;
			inline bool pop() override;
			inline bool pop(T&) override;

		public: //overrides
			inline static type_name_t class_name();
			inline static bool is_inherited_of(type_name_t);
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;
			inline comparision_result_t compare(const object&)const override;

		};

		template<>
		class stack_object<void> final {};
	}

	/******************************************************************/
	/* template class ang::object_wrapper<stack_object> :            */
	/*  -> specialization of object_wrapper<stack_object> -> stack  */
	/******************************************************************/
	template<typename T, template<typename> class allocator>
	class object_wrapper<collections::stack_object<T, allocator>>
	{
	public:
		typedef collections::stack_object<T, allocator> type;
		typedef typename collections::stack_object<T, allocator>::type data_type;

	private:
		collections::stack_object<T, allocator>* _ptr;

	public:
		object_wrapper();
		object_wrapper(collections::stack_object<T, allocator>*);
		template<typename U> object_wrapper(ang::initializer_list_t<U> list);
		object_wrapper(const collections::ienum<data_type>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::stack_object<T, allocator>* get(void)const;
		void set(collections::stack_object<T, allocator>*);
		collections::stack_object<T, allocator>** addres_of(void);

		collections::forward_iterator<T> begin() { return _ptr ? _ptr->begin() : collections::forward_iterator<T>(); }
		collections::forward_iterator<T> end() { return _ptr ? _ptr->end() : collections::forward_iterator<T>(); }
		collections::forward_iterator<const T> begin()const { return _ptr ? ((type const*)_ptr)->begin() : collections::forward_iterator<const T>(); }
		collections::forward_iterator<const T> end()const { return _ptr ? ((type const*)_ptr)->end() : collections::forward_iterator<const T>(); }

	public:
		object_wrapper& operator = (collections::stack_object<T, allocator>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		
		object_wrapper& operator += (T const& value) {
			if (is_empty()) set(new collections::stack_object<T, allocator>());
			_ptr->push(value);
			return *this;
		}

		object_wrapper_ptr<collections::stack_object<T, allocator>> operator & (void);
		collections::stack_object<T, allocator> * operator -> (void);
		collections::stack_object<T, allocator> const* operator -> (void)const;
		explicit operator collections::stack_object<T, allocator> * (void);
		explicit operator collections::stack_object<T, allocator> const* (void)const;
	};

}

//#include<ang/collections/stack_specialization.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#include<ang/collections/inline/stack.inl>

#endif//__ANG_STACK_HPP__
