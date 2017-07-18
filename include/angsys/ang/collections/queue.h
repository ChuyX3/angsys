/*********************************************************************************************************************/
/*   File Name: ang/collections/queue.h                                                                             */
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

#ifndef __ANG_QUEUE_H__
#define __ANG_QUEUE_H__

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
		template<typename T> class queue_data;
		template<typename T> using queue = object_wrapper<queue_data<T>>;
	}

	namespace collections
	{

		/******************************************************************/
		/* template class queue_data :                                     */
		/*  -> implements the buffer memory of queue object                */
		/******************************************************************/
		template<typename T>
		class queue_data final
			: public object
			, public ilist<T>
			, public memory::iallocator_client
		{
		public:
			typedef T					type;
			typedef queue_data<T>		self_t;
			typedef ilist<T>			ilist_t;
			typedef icollection<T>		icollection_t;
			typedef ienum<T>			ienum_t;
			typedef iterator<T>			iterator_t;
			typedef linked_node<T> node_t;
			typedef linked_node<T>* node_ptr_t;

		protected:
			uint _count;
			node_ptr_t _head;
			node_ptr_t _tail;
			memory::iallocator* allocator;

		public:
			inline queue_data();
			inline queue_data(ang::initializer_list_t<T> queue);
			inline queue_data(const ang::nullptr_t&);
			inline queue_data(queue_data&& ar);
			inline queue_data(const queue_data& ar);
			inline queue_data(const queue_data* ar);
			inline queue_data(const ienum_t* store);

		private:
			virtual ~queue_data();

		public: //methods
			inline bool is_empty()const;
			inline uint size()const;
			inline void clean();

			inline bool move(queue_data&);
			inline bool copy(const static_array<T> items) {
				clean();
				if (items.size() != 0) {
					for (auto it = items.begin(); it != items.end(); ++it)
						append(*it);
				}
				return true;
			}
			template<class U> inline bool copy_from(const collections::ienum<U>* items) {
				if (!items) return false;
				clean();
				for (auto it = items->begin(); it.is_valid(); ++it)
					append(*it);
				return true;
			}

		public: //iallocator_client overrides
			inline void set_allocator(memory::iallocator*)override;
			inline memory::iallocator* get_allocator()const override;

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
			inline iterator_t find(const T&, bool = false)const override;
			inline iterator_t find(const T&, iterator_t nextTo, bool = false)const override;

		public: //ilist
			inline void extend(const ienum_t*) override;
			inline void append(T const&, bool = true) override;
			inline bool pop(bool = true) override;
			inline bool pop(T&, bool = true) override;

			inline bool insert(index idx, T const&) override { return false; }
			inline bool insert(iterator_t it, T const&) override { return false; }
			inline bool pop_at(index idx) override { return false; }
			inline bool pop_at(iterator_t it) override { return false; }
			inline bool pop_at(index idx, T&) override { return false; }
			inline bool pop_at(iterator_t it, T&) override { return false; }

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool operator == (const queue_data& ar);
			inline bool operator != (const queue_data& ar);
			inline explicit operator T*()const;

		protected: //Memory Operations
			inline bool realloc(uint size, bool save = true);
			inline bool realloc(memory::iallocator* alloc);
		};

		template<>
		class queue_data<void> final {};
	}

	/******************************************************************/
	/* template class ang::object_wrapper<queue_data> :            */
	/*  -> specialization of object_wrapper<queue_data> -> queue  */
	/******************************************************************/
	template<typename T>
	class object_wrapper<collections::queue_data<T>>
	{
	public:
		typedef collections::queue_data<T> type;
		typedef typename collections::queue_data<T>::type data_type;

	private:
		collections::queue_data<T>* _ptr;

	public:
		object_wrapper();
		object_wrapper(collections::queue_data<T>*);
		object_wrapper(ang::initializer_list_t<data_type> queue);
		object_wrapper(const collections::ienum<data_type>* store);
		object_wrapper(static_array<data_type> store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::queue_data<T>* get(void)const;
		void set(collections::queue_data<T>*);
		collections::queue_data<T>** addres_of(void);

	public:
		object_wrapper& operator = (collections::queue_data<T>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (T const&);

		object_wrapper_ptr<collections::queue_data<T>> operator & (void);
		collections::queue_data<T> * operator -> (void);
		collections::queue_data<T> const* operator -> (void)const;
		explicit operator collections::queue_data<T> * (void);
		explicit operator collections::queue_data<T> const* (void)const;
		
		friend safe_pointer;
	};

	/******************************************************************/
	/* template class ang::object_wrapper<queue_data> :            */
	/*  -> specialization of object_wrapper<queue_data> -> queue  */
	/******************************************************************/
	/*template<typename T>
	class object_wrapper<collections::queue_data<object_wrapper<T>>>
	{
	public:
		typedef collections::queue_data<object_wrapper<T>> type;
		typedef typename collections::queue_data<object_wrapper<T>>::type data_type;

	private:
		collections::queue_data<object_wrapper<T>>* _ptr;

	public:
		object_wrapper();
		object_wrapper(collections::queue_data<object_wrapper<T>>*);
		object_wrapper(ang::initializer_queue_t<object_wrapper<T>> queue);
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
		collections::queue_data<object_wrapper<T>>* get(void)const;
		void set(collections::queue_data<object_wrapper<T>>*);
		collections::queue_data<object_wrapper<T>>** addres_of(void);

	public:
		object_wrapper& operator = (collections::queue_data<object_wrapper<T>>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<object_wrapper<T>> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (object_wrapper<T>);

		object_wrapper_ptr<collections::queue_data<object_wrapper<T>>> operator & (void);
		collections::queue_data<object_wrapper<T>> * operator -> (void);
		collections::queue_data<object_wrapper<T>> const* operator -> (void)const;
		explicit operator collections::queue_data<object_wrapper<T>> * (void);
		explicit operator collections::queue_data<object_wrapper<T>> const* (void)const;

		friend safe_pointer;
	};
*/
	template<class T, typename F>
	static void foreach(collections::queue<T> const& store, F func) {
		if(!store.is_empty())for (collections::iterator<T> it = store->begin(); it.is_valid(); ++it)
			func((T&)*it);
	}

}

//#include<ang/collections/vector_specialization.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#include<ang/collections/inline/queue.inl>

#endif//__ANG_QUEUE_H__
