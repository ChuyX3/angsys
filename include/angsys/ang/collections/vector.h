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
			, public memory::iallocator_client
		{
		public:
			typedef T					type;
			typedef vector_buffer<T>		self_t;
			typedef icollection<T>		icollection_t;
			typedef ienum<T>			ienum_t;
			typedef ilist<T>			ilist_t;
			typedef iterator<T>			iterator_t;

		protected:
			uint _size;
			uint _capacity;
			type* _data;
			memory::iallocator* allocator;

		public:
			inline vector_buffer();
			inline vector_buffer(ang::initializer_list_t<T> list);
			inline vector_buffer(const ang::nullptr_t&);
			inline vector_buffer(vector_buffer&& ar);
			inline vector_buffer(const vector_buffer& ar);
			inline vector_buffer(const vector_buffer* ar);
			inline vector_buffer(const ienum_t* store);
			inline explicit vector_buffer(uint size, T const* ar = null);

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
			inline bool copy(uint size, T const* ar);
			template<class U> inline bool copy_from(const collections::ienum<U>* items) {
				if (!items) return false;
				if (items->counter() > counter())
					capacity(items->counter(), false);		
				for (auto it = items->begin(); it.is_valid(); ++it)
					append(*it);
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

		public: //ilist overrides
			inline void extend(const ienum_t*) override;
			inline void append(T const&, bool last = true) override;
			inline bool insert(index idx, T const&) override;
			inline bool insert(iterator_t it, T const&) override;
			inline bool pop(bool last = true) override;
			inline bool pop(T&, bool last = true) override;
			inline bool pop_at(index idx) override;
			inline bool pop_at(iterator_t it) override;
			inline bool pop_at(index idx, T&) override;
			inline bool pop_at(iterator_t it, T&) override;

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool operator == (const vector_buffer& ar);
			inline bool operator != (const vector_buffer& ar);
			inline explicit operator T*()const;

		protected: //Memory Operations
			inline bool realloc(uint size, bool save = true);
			inline bool realloc(memory::iallocator* alloc);
		};

		/******************************************************************/
		/* template class vector_buffer<object_wrapper> :                 */
		/*  -> partial specialization of vector_buffer<object_wrapper>    */
		/******************************************************************/
		template<typename T>
		class vector_buffer<object_wrapper<T>> final
			: public object
			, public ibuffer
			, public ilist<object_wrapper<T>>
			, public memory::iallocator_client
		{
		public:
			typedef object_wrapper<T>					type;
			typedef vector_buffer<object_wrapper<T>>		self_t;
			typedef icollection<object_wrapper<T>>		icollection_t;
			typedef ienum<object_wrapper<T>>			ienum_t;
			typedef ilist<object_wrapper<T>>			ilist_t;
			typedef iterator<object_wrapper<T>>			iterator_t;

		protected:
			uint _size;
			uint _capacity;
			type* _data;
			memory::iallocator* allocator;

		public:
			inline vector_buffer();
			inline vector_buffer(ang::initializer_list_t<object_wrapper<T>> list);
			inline vector_buffer(const ang::nullptr_t&);
			inline vector_buffer(vector_buffer&& ar);
			inline vector_buffer(const vector_buffer& ar);
			inline vector_buffer(const vector_buffer* ar);
			inline vector_buffer(const ienum_t* store);
			inline vector_buffer(uint, object_wrapper<T>* ar = null);

		private:
			virtual ~vector_buffer();

		public: //methods
			inline bool is_empty()const;
			inline type* data()const;
			inline uint size()const;
			inline uint capacity()const;
			inline void capacity(uint size, bool save = false);

			inline void clean();
			inline void empty();

			inline bool move(vector_buffer&);
			template<class U> inline bool copy_from(const collections::ienum<U>* items) {
				if (!items) return false;
				if (items->counter() > counter())
					capacity(items->counter(), false);
				for (auto it = items->begin(); it.is_valid(); ++it)
					append(*it);
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

		public: //ilist overrides
			inline void extend(const ienum_t*) override;
			inline void append(object_wrapper<T> const&, bool last = true) override;
			inline bool insert(index idx, object_wrapper<T> const&) override;
			inline bool insert(iterator_t it, object_wrapper<T> const&) override;
			inline bool pop(bool last = true) override;
			inline bool pop(object_wrapper<T>&, bool last = true) override;
			inline bool pop_at(index idx) override;
			inline bool pop_at(iterator_t it) override;
			inline bool pop_at(index idx, object_wrapper<T>&) override;
			inline bool pop_at(iterator_t it, object_wrapper<T>&) override;

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool operator == (const vector_buffer& ar);
			inline bool operator != (const vector_buffer& ar);
			inline explicit operator T**()const;

		protected: //Memory Operations
			inline bool realloc(uint size, bool save = true);
			inline bool realloc(memory::iallocator* alloc);
		};

		/******************************************************************/
		/* template class vector_buffer<intf_wrapper> :                   */
		/*  -> partial specialization of vector_buffer<intf_wrapper>      */
		/******************************************************************/
		template<typename T>
		class vector_buffer<intf_wrapper<T>> final
			: public object
			, public ibuffer
			, public ilist<intf_wrapper<T>>
			, public memory::iallocator_client
		{
		public:
			typedef intf_wrapper<T>					type;
			typedef vector_buffer<intf_wrapper<T>>		self_t;
			typedef icollection<intf_wrapper<T>>		icollection_t;
			typedef ienum<intf_wrapper<T>>			ienum_t;
			typedef ilist<intf_wrapper<T>>			ilist_t;
			typedef iterator<intf_wrapper<T>>			iterator_t;

		protected:
			uint _size;
			uint _capacity;
			type* _data;
			memory::iallocator* allocator;

		public:
			inline vector_buffer();
			inline vector_buffer(ang::initializer_list_t<intf_wrapper<T>> list);
			inline vector_buffer(const ang::nullptr_t&);
			inline vector_buffer(vector_buffer&& ar);
			inline vector_buffer(const vector_buffer& ar);
			inline vector_buffer(const vector_buffer* ar);
			inline vector_buffer(const ienum_t* store);
			inline vector_buffer(uint, intf_wrapper<T>* ar = null);

		private:
			virtual ~vector_buffer();

		public: //methods
			inline bool is_empty()const;
			inline type* data()const;
			inline uint size()const;
			inline uint capacity()const;
			inline void capacity(uint size, bool save = false);

			inline void clean();
			inline void empty();

			inline bool move(vector_buffer&);
			template<class U> inline bool copy_from(const collections::ienum<U>* items) {
				if (!items) return false;
				if (items->counter() > counter())
					capacity(items->counter(), false);
				for (auto it = items->begin(); it.is_valid(); ++it)
					append(*it);
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
			inline intf_wrapper<T>& at(const iterator_t& it)const override;
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
			inline iterator_t find(const intf_wrapper<T>&, bool invert = false)const override;
			inline iterator_t find(const intf_wrapper<T>&, iterator_t nextTo, bool invert = false)const override;

		public: //ilist overrides
			inline void extend(const ienum_t*) override;
			inline void append(intf_wrapper<T> const&, bool last = true) override;
			inline bool insert(index idx, intf_wrapper<T> const&) override;
			inline bool insert(iterator_t it, intf_wrapper<T> const&) override;
			inline bool pop(bool last = true) override;
			inline bool pop(intf_wrapper<T>&, bool last = true) override;
			inline bool pop_at(index idx) override;
			inline bool pop_at(iterator_t it) override;
			inline bool pop_at(index idx, intf_wrapper<T>&) override;
			inline bool pop_at(iterator_t it, intf_wrapper<T>&) override;

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool operator == (const vector_buffer& ar);
			inline bool operator != (const vector_buffer& ar);
			inline explicit operator T**()const;

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
		object_wrapper(collections::vector_buffer<T>*);
		object_wrapper(ang::initializer_list_t<data_type> list);
		object_wrapper(const collections::ienum<data_type>* store);
		object_wrapper(static_array<data_type> store);
		explicit object_wrapper(uint size, data_type const* ar = null);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::vector_buffer<T>* get(void)const;
		void set(collections::vector_buffer<T>*);
		collections::vector_buffer<T>** addres_of(void);

	public:
		object_wrapper& operator = (collections::vector_buffer<T>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (T const&);

		object_wrapper_ptr<collections::vector_buffer<T>> operator & (void);
		collections::vector_buffer<T> * operator -> (void);
		collections::vector_buffer<T> const* operator -> (void)const;
		explicit operator collections::vector_buffer<T> * (void);
		explicit operator collections::vector_buffer<T> const* (void)const;
		operator static_array<T>()const { return _ptr ? static_array<T>(_ptr->data(), _ptr->size()) : static_array<T>(); }

		data_type const& operator[](int idx)const;
		data_type & operator[](int idx);

		friend safe_pointer;
	};

	/******************************************************************/
	/* template class ang::object_wrapper<vector_buffer> :            */
	/*  -> specialization of object_wrapper<vector_buffer> -> vector  */
	/******************************************************************/
	template<typename T>
	class object_wrapper<collections::vector_buffer<object_wrapper<T>>>
	{
	public:
		typedef collections::vector_buffer<object_wrapper<T>> type;
		typedef typename collections::vector_buffer<object_wrapper<T>>::type data_type;

	private:
		collections::vector_buffer<object_wrapper<T>>* _ptr;

	public:
		object_wrapper();
		object_wrapper(collections::vector_buffer<object_wrapper<T>>*);
		object_wrapper(ang::initializer_list_t<object_wrapper<T>> list);
		object_wrapper(const collections::ienum<object_wrapper<T>>* store);
		object_wrapper(static_array<object_wrapper<T>> store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::vector_buffer<object_wrapper<T>>* get(void)const;
		void set(collections::vector_buffer<object_wrapper<T>>*);
		collections::vector_buffer<object_wrapper<T>>** addres_of(void);

	public:
		object_wrapper& operator = (collections::vector_buffer<object_wrapper<T>>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<object_wrapper<T>> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (object_wrapper<T>);

		object_wrapper_ptr<collections::vector_buffer<object_wrapper<T>>> operator & (void);
		collections::vector_buffer<object_wrapper<T>> * operator -> (void);
		collections::vector_buffer<object_wrapper<T>> const* operator -> (void)const;
		explicit operator collections::vector_buffer<object_wrapper<T>> * (void);
		explicit operator collections::vector_buffer<object_wrapper<T>> const* (void)const;
		operator static_array<object_wrapper<T>>()const { return _ptr ? static_array<object_wrapper<T>>(_ptr->data(), _ptr->size()) : static_array<object_wrapper<T>>(); }

		object_wrapper<T> const& operator[](int idx)const;
		object_wrapper<T> & operator[](int idx);

		friend safe_pointer;
	};

	template<class T, typename F>
	static void foreach(collections::vector<T> const& store, F func) {
		if(!store.is_empty())for (collections::iterator<T> it = store->begin(); it.is_valid(); ++it)
			func((T&)*it);
	}

}

#include<ang/collections/vector_specialization.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#include<ang/collections/inline/vector.hpp>

#endif//__ANG_VECTOR_H__
