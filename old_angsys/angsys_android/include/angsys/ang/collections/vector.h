/*********************************************************************************************************************/
/*   File Name: ang/collections/vector.h                                                                             */
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

#ifndef __ANG_VECTOR_H__
#define __ANG_VECTOR_H__

#include <angsys.h>


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
		template<typename T> class vector_buffer;
		template<typename T> using vector = object_wrapper<vector_buffer<T>>;
	}

	using collections::vector;

	namespace collections
	{
		/******************************************************************/
		/* template class vector_buffer :                                 */
		/*  -> implements the buffer memory of vector object              */
		/******************************************************************/
		template<typename T>
		class vector_buffer final
			: public object
			, public ibuffer
			, public ilist<T>
		{
		public:
			typedef T							type;
			typedef vector_buffer<T>			self_t;
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
			uint _size;
			uint _capacity;
			type* _data;
			memory::iallocator* _allocator;

		public:
			inline vector_buffer();
			inline vector_buffer(ang::initializer_list_t<T> list);
			inline vector_buffer(const ang::nullptr_t&);
			inline vector_buffer(vector_buffer&& ar);
			inline vector_buffer(const vector_buffer& ar);
			inline vector_buffer(const vector_buffer* ar);
			inline vector_buffer(const ienum_t* store);
			inline vector_buffer(static_array<type>);
			inline vector_buffer(static_const_array<type>);

		private:
			virtual ~vector_buffer();

		public: //methods
			inline bool is_empty()const;
			inline T* data()const;
			inline uint size()const;
			inline uint capacity()const;
			inline void capacity(uint size, bool save = false);

			inline void clean();
			inline void empty();
			inline bool move(vector_buffer&);
			inline void copy(static_array<type>);
			inline void copy(static_const_array<type>);
			inline void extend(static_array<type>);
			inline void extend(static_const_array<type>);
			inline void allocator(memory::iallocator*);
			inline memory::iallocator* allocator()const;

		public: //ibuffer overrides
			inline pointer buffer_ptr()const override;
			inline wsize buffer_size()const override;
			inline wsize mem_copy(wsize, pointer) override;
			inline pointer map_buffer(windex, wsize) override;
			inline void unmap_buffer(pointer, wsize) override;
			inline bool can_realloc_buffer()const override;
			inline bool realloc_buffer(wsize) override;

		public: //ienum overrides
			inline uint counter()const override;

			inline T& at(base_iterator_t const&) override;
			inline bool increase(base_iterator_t&)const override;
			inline bool increase(base_iterator_t&, int offset)const override;
			inline bool decrease(base_iterator_t&)const override;
			inline bool decrease(base_iterator_t&, int offset)const override;

			inline forward_iterator_t begin() override;
			inline forward_iterator_t end() override;
			inline const_forward_iterator_t begin()const override;
			inline const_forward_iterator_t end()const override;

			inline backward_iterator_t rbegin() override;
			inline backward_iterator_t rend() override;
			inline const_backward_iterator_t rbegin()const override;
			inline const_backward_iterator_t rend()const override;

		public: //iarray overrides
			inline iterator_t at(index) override;
			inline const_iterator_t at(index)const override;
			inline void copy(const ienum<T>*) override;
			inline index find(const T&, bool invert = false)const override;
			inline index find(const T&, base_iterator_t next_to, bool invert = false)const override;

		public: //ilist overrides
			inline void extend(const ienum<T>*) override;
			inline void push(T const&, bool last = true) override;
			inline bool insert(index idx, T const&) override;
			inline bool insert(base_iterator_t it, T const&) override;
			inline bool pop(bool last = true) override;
			inline bool pop(T&, bool last = true) override;
			inline bool pop_at(index idx) override;
			inline bool pop_at(base_iterator_t it) override;
			inline bool pop_at(index idx, T&) override;
			inline bool pop_at(base_iterator_t it, T&) override;

		public: //overrides
			inline static type_name_t class_name();
			inline static bool is_child_of(type_name_t);
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;
			inline comparision_result_t compare(const object&)const override;

		protected: //Memory Operations
			inline bool realloc(uint size, bool save = true);
			inline bool realloc(memory::iallocator* alloc);
		};

		template<>
		class vector_buffer<void> final {};
	}

	/******************************************************************/
	/* template class ang::object_wrapper<vector_buffer> :            */
	/*  -> specialization of object_wrapper<vector_buffer> -> vector  */
	/******************************************************************/
	template<typename T>
	class object_wrapper<collections::vector_buffer<T>>
	{
	public:
		typedef collections::vector_buffer<T> type;
		typedef typename collections::vector_buffer<T>::type data_type;

	private:
		collections::vector_buffer<T>* _ptr;

	public:
		object_wrapper();
		object_wrapper(static_array<T>);
		object_wrapper(static_const_array<T>);
		object_wrapper(collections::vector_buffer<T>*);
		object_wrapper(std::initializer_list<T> list);
		object_wrapper(const collections::ienum<T>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		template<wsize N>
		object_wrapper(T(&ar)[N]) : _ptr(null) {
			set(new collections::vector_buffer<T>(static_array<T>(ar, N)));
		}
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::vector_buffer<T>* get(void)const;
		void set(collections::vector_buffer<T>*);
		collections::vector_buffer<T>** addres_of(void);

		collections::forward_iterator<T> begin() { return _ptr ? _ptr->begin() : collections::forward_iterator<T>(); }
		collections::forward_iterator<T> end() { return _ptr ? _ptr->end() : collections::forward_iterator<T>(); }
		collections::forward_iterator<const T> begin()const { return _ptr ? ((type const*)_ptr)->begin() : collections::forward_iterator<const T>(); }
		collections::forward_iterator<const T> end()const { return _ptr ? ((type const*)_ptr)->end() : collections::forward_iterator<const T>(); }

	public:
		object_wrapper& operator = (collections::vector_buffer<T>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<T> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator += (T const&);
		template<wsize N>
		object_wrapper& operator = (T(&ar)[N]) {
			set(new collections::array_buffer<T>(static_array<T>(ar, N)));
			return *this;
		}
		template<wsize N>
		object_wrapper& operator += (T(&ar)[N]) {
			if(is_empty()) set(new collections::array_buffer<T>(static_array<T>(ar, N)));
			else _ptr->extend(static_array<T>(ar, N));
			return *this;
		}

		object_wrapper_ptr<collections::vector_buffer<T>> operator & (void);
		collections::vector_buffer<T> * operator -> (void);
		collections::vector_buffer<T> const* operator -> (void)const;
		explicit operator collections::vector_buffer<T> * (void);
		explicit operator collections::vector_buffer<T> const* (void)const;
		operator static_array<T>()const { return _ptr ? static_array<T>(_ptr->data(), _ptr->size()) : static_array<T>(); }

		T const& operator[](int)const;
		T & operator[](int);
	};

}

//#include<ang/collections/vector_specialization.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#include<ang/collections/inline/vector.inl>

#endif//__ANG_VECTOR_H__
