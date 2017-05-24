/*********************************************************************************************************************/
/*   File Name: ang/collections/array.h                                                                              */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: Implements array object                                                                       */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_ARRAY_H__
#define __ANG_ARRAY_H__

namespace ang
{
	namespace collections
	{
		/******************************************************************/
		/* template class array_buffer :                                  */
		/*  -> implements the buffer memory of array object               */
		/******************************************************************/
		template<typename T>
		class array_buffer final
			: public object
			, public ibuffer
			, public icollection<T>
			, public memory::iallocator_client
		{
		public:
			typedef T							type;
			typedef array_buffer<T>				self_t;
			typedef icollection<T>				icollection_t;
			typedef ienum<T>					ienum_t;
			typedef iterator<T>					iterator_t;

		protected:
			uint _size;
			type* _data;
			memory::iallocator* allocator;

		public:
			inline array_buffer();
			inline array_buffer(ang::initializer_list_t<T> list);
			inline array_buffer(const ang::nullptr_t&);
			inline array_buffer(array_buffer&& ar);
			inline array_buffer(const array_buffer& ar);
			inline array_buffer(const array_buffer* ar);
			inline array_buffer(const ienum_t* store);
			inline explicit array_buffer(uint size, T const* ar = null);

		private:
			virtual ~array_buffer();

		public: //methods
			inline bool is_empty()const;
			inline T* data()const;
			inline uint size()const;
			inline void size(uint size);

			inline void clean();
			inline bool move(array_buffer<T>&);
			inline bool copy(uint size, T const* ar);
			template<class U> inline bool copy_from(const collections::ienum<U>& items) {
				if (items.counter() != counter())
					size(items.counter());
				index i = 0;
				for (auto it = items.begin(); it.is_valid(); ++it)
					_data[i++] = *it;
				return true;
			}

		public: //iallocator_client overrides
			inline void set_allocator(memory::iallocator*)override;
			inline memory::iallocator* get_allocator()const override;

		public: //ibuffer overrides
			inline pointer buffer_ptr()const override;
			inline wsize buffer_size()const override;
			inline wsize mem_copy(wsize, pointer) override;
			inline pointer map_buffer(windex, wsize) override;
			inline void unmap_buffer(pointer, wsize) override;
			inline bool can_realloc_buffer()const override;
			inline bool realloc_buffer(wsize) override;

		public: //ienum overrides
			inline T& at(const iterator_t& it)const override;
			inline iterator_t begin()const override;
			inline iterator_t end()const override;
			inline bool next(iterator_t& it)const override;
			inline bool next(iterator_t& it, int offset)const override;
			inline bool prev(iterator_t& it)const override;
			inline bool prev(iterator_t& it, int offset)const override;
			inline uint counter()const override;

		public: //icollection overrides
			inline bool has_items()const override;
			inline bool copy(const ienum_t*) override;
			inline index index_of(iterator_t it)const override;
			inline iterator_t find_index(index idx)const override;
			inline iterator_t find(const T&, bool invert = false)const override;
			inline iterator_t find(const T&, iterator_t nextTo, bool invert = false)const override;

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool operator == (const array_buffer& ar);
			inline bool operator != (const array_buffer& ar);
			inline explicit operator T*()const;

		protected: //Memory Operations
			inline bool realloc(uint size);
			inline bool realloc(memory::iallocator* alloc);

		};

		/******************************************************************/
		/* template class array_buffer<object_wrapper> :                  */
		/*  -> partial specialization of array_buffer<object_wrapper>     */
		/******************************************************************/
		template<typename T>
		class array_buffer<object_wrapper<T>> final
			: public object
			, public ibuffer
			, public icollection<object_wrapper<T>>
			, public memory::iallocator_client
		{
		public:
			typedef object_wrapper<T>					type;
			typedef array_buffer<object_wrapper<T>>		self_t;
			typedef icollection<object_wrapper<T>>		icollection_t;
			typedef ienum<object_wrapper<T>>			ienum_t;
			typedef iterator<object_wrapper<T>>			iterator_t;

		protected:
			uint _size;
			type* _data;
			memory::iallocator* allocator;

		public:
			inline array_buffer();
			inline explicit array_buffer(uint);
			inline array_buffer(std::initializer_list<object_wrapper<T>> list);
			inline array_buffer(const ang::nullptr_t&);
			inline array_buffer(array_buffer&& ar);
			inline array_buffer(const array_buffer& ar);
			inline array_buffer(const array_buffer* ar);
			inline array_buffer(const ienum_t* store);

		private:
			virtual ~array_buffer();

		public: //methods
			inline bool is_empty()const;
			inline type* data()const;
			inline uint size()const;
			inline void size(uint size);

			inline void clean();
			inline bool move(array_buffer&);
			template<class U> inline bool copy_from(const collections::ienum<U>& items) {
				if (items.counter() != counter())
					size(items.counter());
				index i = 0;
				for (auto it = items.begin(); it.is_valid(); ++it)
					_data[i++] = *it;
				return true;
			}

		protected:
			inline object_wrapper<T>& get(index);
			inline object_wrapper<T> const& get(index)const;

		public: //iallocator_client overrides
			inline void set_allocator(memory::iallocator*)override;
			inline memory::iallocator* get_allocator()const override;

		public: //ibuffer overrides
			inline pointer buffer_ptr()const override;
			inline wsize buffer_size()const override;
			inline wsize mem_copy(wsize, pointer) override;
			inline pointer map_buffer(windex, wsize) override;
			inline void unmap_buffer(pointer, wsize) override;
			inline bool can_realloc_buffer()const override;
			inline bool realloc_buffer(wsize) override;

		public: //ienum overrides
			inline object_wrapper<T>& at(const iterator_t& it)const override;
			inline iterator_t begin()const override;
			inline iterator_t end()const override;
			inline bool next(iterator_t& it)const override;
			inline bool next(iterator_t& it, int offset)const override;
			inline bool prev(iterator_t& it)const override;
			inline bool prev(iterator_t& it, int offset)const override;
			inline uint counter()const override;

		public: //icollection overrides
			inline bool has_items()const override;
			inline bool copy(const ienum_t*) override;
			inline index index_of(iterator_t it)const override;
			inline iterator_t find_index(index idx)const override;
			inline iterator_t find(const object_wrapper<T>&, bool invert = false)const override;
			inline iterator_t find(const object_wrapper<T>&, iterator_t nextTo, bool invert = false)const override;

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool operator == (const array_buffer& ar);
			inline bool operator != (const array_buffer& ar);
			inline explicit operator T**()const;

		protected: //Memory Operations
			inline bool realloc(uint size);
			inline bool realloc(memory::iallocator* alloc);

		};

		template<>
		class array_buffer<void> final {};
	}

	/******************************************************************/
	/* template class ang::object_wrapper<array_buffer> :             */
	/*  -> specialization of object_wrapper<array_buffer> -> array    */
	/******************************************************************/
	template<typename T>
	class object_wrapper<collections::array_buffer<T>>
	{
	public:
		typedef collections::array_buffer<T> type;
		typedef typename collections::array_buffer<T>::type data_type;

	private:
		collections::array_buffer<T>* _ptr;

	public:
		object_wrapper();
		object_wrapper(collections::array_buffer<T>*);
		object_wrapper(std::initializer_list<data_type> list);
		object_wrapper(const collections::ienum<data_type>* store);
		explicit object_wrapper(uint size, data_type const* ar = null);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::array_buffer<T>* get(void)const;
		void set(collections::array_buffer<T>*);
		collections::array_buffer<T>** addres_of(void);

	public:
		object_wrapper& operator = (collections::array_buffer<T>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<collections::array_buffer<T>> operator & (void);
		collections::array_buffer<T> * operator -> (void);
		collections::array_buffer<T> const* operator -> (void)const;
		explicit operator collections::array_buffer<T> * (void);
		explicit operator collections::array_buffer<T> const* (void)const;

		operator static_array<T>()const { return _ptr ? static_array<T>(_ptr->data(), _ptr->size()) : static_array<T>(); }

		data_type const& operator[](index index)const;
		data_type & operator[](index index);

		friend safe_pointer;
	};

	/******************************************************************/
	/* template class ang::object_wrapper<array_buffer> :             */
	/*  -> specialization of object_wrapper<array_buffer> -> array    */
	/******************************************************************/
	template<typename T>
	class object_wrapper<collections::array_buffer<object_wrapper<T>>>
	{
	public:
		typedef collections::array_buffer<object_wrapper<T>> type;
		typedef typename collections::array_buffer<object_wrapper<T>>::type data_type;

	private:
		collections::array_buffer<object_wrapper<T>>* _ptr;

	public:
		object_wrapper();
		explicit object_wrapper(uint size);
		object_wrapper(collections::array_buffer<object_wrapper<T>>*);
		object_wrapper(std::initializer_list<object_wrapper<T>> list);
		object_wrapper(const collections::ienum<object_wrapper<T>>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::array_buffer<object_wrapper<T>>* get(void)const;
		void set(collections::array_buffer<object_wrapper<T>>*);
		collections::array_buffer<object_wrapper<T>>** addres_of(void);

	public:
		object_wrapper& operator = (collections::array_buffer<object_wrapper<T>>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<object_wrapper<T>> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<collections::array_buffer<object_wrapper<T>>> operator & (void);
		collections::array_buffer<object_wrapper<T>> * operator -> (void);
		collections::array_buffer<object_wrapper<T>> const* operator -> (void)const;
		explicit operator collections::array_buffer<object_wrapper<T>> * (void);
		explicit operator collections::array_buffer<object_wrapper<T>> const* (void)const;

		operator static_array<object_wrapper<T>>()const { return _ptr ? static_array<object_wrapper<T>>(_ptr->data(), _ptr->size()) : static_array<object_wrapper<T>>(); }

		object_wrapper<T> const& operator[](index)const;
		object_wrapper<T> & operator[](index);

		friend safe_pointer;
	};

	template<> class object_wrapper<collections::array_buffer<void>> {
	};

	template<class T, typename F>
	static void foreach(collections::array<T> const& store, F func) {
		for (collections::iterator<T> it = store->begin(); it.is_valid(); ++it)
			func((T&)*it);
	}
}

#include <ang/collections/array_specialization.h>

#endif//__ANG_ARRAY_H__
