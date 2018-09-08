/*********************************************************************************************************************/
/*   File Name: ang/collections/vector.hpp                                                                             */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: Implements vector object                                                                      */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_VECTOR_HPP__
#define __ANG_VECTOR_HPP__

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
		template<typename T, template<typename> class allocator = memory::default_allocator> class vector_buffer;
		template<typename T, template<typename> class allocator = memory::default_allocator> using vector = object_wrapper<vector_buffer<T, allocator>>;
	}

	using collections::vector;

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
			typedef iarray<T>					iarray;
			typedef ienum<T>					ienum_t;
			typedef ilist<T>					ilist_t;
			typedef base_iterator<T>			base_iterator_t;
			typedef iterator<T>					iterator_t;
			typedef const_iterator<T>			const_iterator_t;
			typedef forward_iterator<T>			forward_iterator_t;
			typedef const_forward_iterator<T>	const_forward_iterator_t;
			typedef backward_iterator<T>		backward_iterator_t;
			typedef const_backward_iterator<T>	const_backward_iterator_t;


		protected:
			wsize _size;
			wsize _capacity;
			type* _data;

		public:
			inline vector_buffer();
			inline vector_buffer(wsize reserve);
			template<typename U>inline vector_buffer(ang::initializer_list_t<U> list);
			inline vector_buffer(const ang::nullptr_t&);
			inline vector_buffer(vector_buffer&& ar);
			inline vector_buffer(const vector_buffer& ar);
			inline vector_buffer(const vector_buffer* ar);
			inline vector_buffer(const ienum_t* store);
			template<typename U> inline vector_buffer(array_view<U> const&);
			template<typename U, template<typename> class allocator2> inline vector_buffer(scope_array<U, allocator2> const&);
			template<typename U, wsize SIZE> inline vector_buffer(stack_array<U, SIZE> const&);

		private:
			virtual ~vector_buffer();

		public: //methods
			inline bool is_empty()const;
			inline T* data()const;
			inline wsize size()const;
			inline void size(wsize);
			inline wsize capacity()const;
			inline void capacity(wsize size, bool save = false);

			inline void clean();
			inline void empty();
			inline bool move(vector_buffer<T, allocator>&);

			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

			template<typename U> inline void expand(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void expand(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void expand(stack_array<U, SIZE> const&);

		private:
	
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
			inline iterator_t find(const T&, bool invert = false)const override;
			inline iterator_t find(const T&, base_iterator_t next_to, bool invert = false)const override;

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
			inline static type_name_t class_name();
			inline static bool is_inherited_of(type_name_t);
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;
			inline comparision_result_t compare(const object&)const override;

		protected: //Memory Operations
			inline bool realloc(wsize size, bool save = true);
		};

		template<>
		class vector_buffer<void> final {};

	}

	/******************************************************************/
	/* template class ang::object_wrapper<vector_buffer> :            */
	/*  -> specialization of object_wrapper<vector_buffer> -> vector  */
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
		template<typename U> object_wrapper(ang::initializer_list_t<U> list);

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
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::vector_buffer<T, allocator>* get(void)const;
		void set(collections::vector_buffer<T, allocator>*);
		collections::vector_buffer<T, allocator>** addres_of(void);

		collections::forward_iterator<T> begin() { return _ptr ? _ptr->begin() : collections::forward_iterator<T>(); }
		collections::forward_iterator<T> end() { return _ptr ? _ptr->end() : collections::forward_iterator<T>(); }
		collections::forward_iterator<const T> begin()const { return _ptr ? ((type const*)_ptr)->begin() : collections::forward_iterator<const T>(); }
		collections::forward_iterator<const T> end()const { return _ptr ? ((type const*)_ptr)->end() : collections::forward_iterator<const T>(); }

	public:
		object_wrapper& operator = (collections::vector_buffer<T, allocator>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator = (T(&ar)[N]) {
			if (is_empty()) set(new collections::vector_buffer<T, allocator>(to_array(ar, N)));
			else _ptr->copy(to_array(ar, N));
			return *this;
		}

		template<wsize N>
		object_wrapper& operator += (T(&ar)[N]) {
			if(is_empty()) set(new collections::vector_buffer<T, allocator>(to_array(ar, N)));
			else _ptr->expand(to_array(ar, N));
			return *this;
		}

		
		object_wrapper& operator += (T const& value) {
			if (is_empty()) set(new collections::vector_buffer<T, allocator>());
			_ptr->push(value);
			return *this;
		}

		object_wrapper_ptr<collections::vector_buffer<T, allocator>> operator & (void);
		collections::vector_buffer<T, allocator> * operator -> (void);
		collections::vector_buffer<T, allocator> const* operator -> (void)const;
		explicit operator collections::vector_buffer<T, allocator> * (void);
		explicit operator collections::vector_buffer<T, allocator> const* (void)const;
		operator array_view<T>()const { return _ptr ? collections::to_array(_ptr->data(), _ptr->size()) : array_view<T>(); }
		template<typename I>T& operator[](I const& idx);
		template<typename I>T const& operator[](I const& idx)const;
	};


	template<typename T, template<typename> class allocator>
	class object_wrapper<collections::vector_buffer<object_wrapper<T>, allocator>>
	{
	public:
		typedef collections::vector_buffer<object_wrapper<T>, allocator> type;
		typedef typename collections::vector_buffer<object_wrapper<T>, allocator>::type data_type;

	private:
		collections::vector_buffer<object_wrapper<T>, allocator>* _ptr;

	public:
		object_wrapper();
		object_wrapper(const collections::ienum<object_wrapper<T>>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		object_wrapper(collections::vector_buffer<object_wrapper<T>, allocator>*);
		object_wrapper(ang::initializer_list_t<object_wrapper<T>> list);
		~object_wrapper();

		inline object_wrapper(array_view<object_wrapper<T>> const& ar) : _ptr(null) {
			set(new collections::vector_buffer<object_wrapper<T>, allocator>(ar));
		}
		template<template<typename> class allocator2> inline object_wrapper(scope_array<object_wrapper<T>, allocator2> const& ar) : _ptr(null) {
			set(new collections::vector_buffer<object_wrapper<T>, allocator>(ar));
		}
		template<wsize SIZE> inline object_wrapper(stack_array<object_wrapper<T>, SIZE> const& ar) : _ptr(null) {
			set(new collections::vector_buffer<object_wrapper<T>, allocator>(ar));
		}
		template<wsize N> inline object_wrapper(object_wrapper<T>(&ar)[N]) : _ptr(null) {
			set(new collections::vector_buffer<T, allocator>(to_array(ar, N)));
		}


	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::vector_buffer<object_wrapper<T>, allocator>* get(void)const;
		void set(collections::vector_buffer<object_wrapper<T>, allocator>*);
		collections::vector_buffer<object_wrapper<T>, allocator>** addres_of(void);

		collections::forward_iterator<object_wrapper<T>> begin() { return _ptr ? _ptr->begin() : collections::forward_iterator<object_wrapper<T>>(); }
		collections::forward_iterator<object_wrapper<T>> end() { return _ptr ? _ptr->end() : collections::forward_iterator<object_wrapper<T>>(); }
		collections::forward_iterator<const object_wrapper<T>> begin()const { return _ptr ? ((type const*)_ptr)->begin() : collections::forward_iterator<const object_wrapper<T>>(); }
		collections::forward_iterator<const object_wrapper<T>> end()const { return _ptr ? ((type const*)_ptr)->end() : collections::forward_iterator<const object_wrapper<T>>(); }

	public:
		object_wrapper& operator = (collections::vector_buffer<object_wrapper<T>, allocator>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<object_wrapper<T>> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator = (object_wrapper<T>(&ar)[N]) {
			if (is_empty()) set(new collections::vector_buffer<object_wrapper<T>, allocator>(to_array(ar, N)));
			else _ptr->copy(to_array(ar, N));
			return *this;
		}

		template<wsize N>
		object_wrapper& operator += (object_wrapper<T>(&ar)[N]) {
			if (is_empty()) set(new collections::vector_buffer<object_wrapper<T>, allocator>(to_array(ar, N)));
			else _ptr->expand(to_array(ar, N));
			return *this;
		}


		object_wrapper& operator += (object_wrapper<T> const& value) {
			if (is_empty()) set(new collections::vector_buffer<object_wrapper<T>, allocator>());
			_ptr->push(value);
			return *this;
		}

		object_wrapper_ptr<collections::vector_buffer<object_wrapper<T>, allocator>> operator & (void);
		collections::vector_buffer<object_wrapper<T>, allocator> * operator -> (void);
		collections::vector_buffer<object_wrapper<T>, allocator> const* operator -> (void)const;
		explicit operator collections::vector_buffer<object_wrapper<T>, allocator> * (void);
		explicit operator collections::vector_buffer<object_wrapper<T>, allocator> const* (void)const;
		operator array_view<object_wrapper<T>>()const { return _ptr ? collections::to_array(_ptr->data(), _ptr->size()) : array_view<object_wrapper<T>>(); }
		template<typename I>object_wrapper<T>& operator[](I const& idx);
		template<typename I>object_wrapper<T> const& operator[](I const& idx)const;
	};

}

#include <ang/collections/vector_definition.hpp>


ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, char, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, byte, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, wchar, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, char16_t, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, char32_t, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, short, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ushort, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, int, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, uint, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, long, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ulong, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, long64, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ulong64, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, float, ang::memory::default_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, double, ang::memory::default_allocator)

ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, char, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, byte, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, wchar, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, char16_t, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, char32_t, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, short, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ushort, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, int, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, uint, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, long, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ulong, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, long64, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ulong64, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, float, ang::memory::aligned16_allocator)
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, double, ang::memory::aligned16_allocator)


ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::strings::string_base<ang::text::encoding::ascii>, ang::memory::default_allocator);
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::strings::string_base<ang::text::encoding::unicode>, ang::memory::default_allocator);
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::strings::string_base<ang::text::encoding::utf8>, ang::memory::default_allocator);
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::strings::string_base<ang::text::encoding::utf16>, ang::memory::default_allocator);
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::strings::string_base<ang::text::encoding::utf16_se>, ang::memory::default_allocator);
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::strings::string_base<ang::text::encoding::utf16_le>, ang::memory::default_allocator);
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::strings::string_base<ang::text::encoding::utf16_be>, ang::memory::default_allocator);
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::strings::string_base<ang::text::encoding::utf32>, ang::memory::default_allocator);
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::strings::string_base<ang::text::encoding::utf32_se>, ang::memory::default_allocator);
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::strings::string_base<ang::text::encoding::utf32_le>, ang::memory::default_allocator);
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::strings::string_base<ang::text::encoding::utf32_be>, ang::memory::default_allocator);


#ifdef  LINK
#undef  LINK
#endif//LINK

#include<ang/collections/inline/vector.inl>

#endif//__ANG_VECTOR_HPP__
