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
		/*  -> implements the memory buffer of an array object            */
		/******************************************************************/
		template<typename T>
		class array_buffer final
			: public object
			, public ibuffer
			, public iarray<T>
		{
		public:
			typedef T							type;
			typedef array_buffer<T>				self_t;
			typedef iarray<T>					iarray_t;
			typedef ienum<T>					ienum_t;
			typedef base_iterator<T>			base_iterator_t;
			typedef iterator<T>					iterator_t;
			typedef const_iterator<T>			const_iterator_t;
			typedef forward_iterator<T>			forward_iterator_t;
			typedef const_forward_iterator<T>	const_forward_iterator_t;
			typedef backward_iterator<T>		backward_iterator_t;
			typedef const_backward_iterator<T>	const_backward_iterator_t;

		protected:
			uint _size;
			type* _data;
			memory::iallocator* _allocator;

		public:
			inline array_buffer();
			inline array_buffer(ang::initializer_list_t<T> list);
			inline array_buffer(const ang::nullptr_t&);
			inline array_buffer(array_buffer&& ar);
			inline array_buffer(const array_buffer& ar);
			inline array_buffer(const array_buffer* ar);
			inline array_buffer(const ienum_t* store);
			inline array_buffer(static_array<type>);
			inline array_buffer(static_const_array<type>);

		private:
			virtual ~array_buffer();

		public: //methods
			inline bool is_empty()const;
			inline T* data()const;
			inline uint size()const;
			inline void size(uint size);

			inline void clean();
			inline bool move(array_buffer<T>&);
			inline void copy(static_array<type>);
			inline void copy(static_const_array<type>);
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

		public: //overrides
			inline static type_name_t class_name();
			inline static bool is_child_of(type_name_t);
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;
			inline comparision_result_t compare(const object&)const override;

		protected: //memory operations
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
		object_wrapper(static_array<T>);
		object_wrapper(static_const_array<T>);
		object_wrapper(collections::array_buffer<T>*);
		object_wrapper(std::initializer_list<data_type> list);
		object_wrapper(const collections::ienum<data_type>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		template<wsize N>
		object_wrapper(T(&ar)[N]) : _ptr(null) {
			set(new collections::array_buffer<T>(static_array<T>(ar, N)));
		}
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::array_buffer<T>* get(void)const;
		void set(collections::array_buffer<T>*);
		collections::array_buffer<T>** addres_of(void);

		collections::forward_iterator<T> begin() { return _ptr ? _ptr->begin() : collections::iterator<T>(); }
		collections::forward_iterator<T> end() { return _ptr ? _ptr->end() : collections::iterator<T>(); }
		collections::forward_iterator<const T> begin()const { return _ptr ? _ptr->begin() : collections::iterator<const T>(); }
		collections::forward_iterator<const T> end()const { return _ptr ? _ptr->end() : collections::iterator<const T>(); }

	public:
		object_wrapper& operator = (collections::array_buffer<T>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator = (T(&ar)[N]) {
			set(new collections::array_buffer<T>(static_array<T>(ar, N)));
			return *this;
		}

		object_wrapper_ptr<collections::array_buffer<T>> operator & (void);
		collections::array_buffer<T> * operator -> (void);
		collections::array_buffer<T> const* operator -> (void)const;
		explicit operator collections::array_buffer<T> * (void);
		explicit operator collections::array_buffer<T> const* (void)const;
		operator static_array<T>()const { return _ptr ? static_array<T>(_ptr->data(), _ptr->size()) : static_array<T>(); }
		T const& operator[](int)const;
		T & operator[](int);
	};

	/******************************************************************/
	/* template class ang::object_wrapper<array_buffer> :             */
	/*  -> specialization of object_wrapper<array_buffer> -> array    */
	/******************************************************************/
	//template<typename T>
	//class object_wrapper<collections::array_buffer<object_wrapper<T>>>
	//{
	//public:
	//	typedef collections::array_buffer<object_wrapper<T>> type;
	//	typedef typename collections::array_buffer<object_wrapper<T>>::type data_type;

	//private:
	//	collections::array_buffer<object_wrapper<T>>* _ptr;

	//public:
	//	object_wrapper();
	//	explicit object_wrapper(uint size);
	//	object_wrapper(collections::array_buffer<object_wrapper<T>>*);
	//	object_wrapper(std::initializer_list<object_wrapper<T>> list);
	//	object_wrapper(const collections::ienum<object_wrapper<T>>* store);
	//	object_wrapper(object_wrapper &&);
	//	object_wrapper(object_wrapper const&);
	//	object_wrapper(ang::nullptr_t const&);
	//	~object_wrapper();

	//public:
	//	void clean();
	//	void clean_unsafe();
	//	bool is_empty()const;
	//	collections::array_buffer<object_wrapper<T>>* get(void)const;
	//	void set(collections::array_buffer<object_wrapper<T>>*);
	//	collections::array_buffer<object_wrapper<T>>** addres_of(void);

	//	collections::iterator<T> begin() { return _ptr ? _ptr->begin() : collections::iterator<T>(); }
	//	collections::iterator<T> end() { return _ptr ? _ptr->end() : collections::iterator<T>(); }
	//	collections::iterator<const T> begin()const { return _ptr ? _ptr->begin() : collections::iterator<const T>(); }
	//	collections::iterator<const T> end()const { return _ptr ? _ptr->end() : collections::iterator<const T>(); }

	//public:
	//	object_wrapper& operator = (collections::array_buffer<object_wrapper<T>>*);
	//	object_wrapper& operator = (const ang::nullptr_t&);
	//	object_wrapper& operator = (collections::ienum<object_wrapper<T>> const* items);
	//	object_wrapper& operator = (object_wrapper &&);
	//	object_wrapper& operator = (object_wrapper const&);

	//	object_wrapper_ptr<collections::array_buffer<object_wrapper<T>>> operator & (void);
	//	collections::array_buffer<object_wrapper<T>> * operator -> (void);
	//	collections::array_buffer<object_wrapper<T>> const* operator -> (void)const;
	//	explicit operator collections::array_buffer<object_wrapper<T>> * (void);
	//	explicit operator collections::array_buffer<object_wrapper<T>> const* (void)const;

	//	operator static_array<object_wrapper<T>>()const { return _ptr ? static_array<object_wrapper<T>>(_ptr->data(), _ptr->size()) : static_array<object_wrapper<T>>(); }

	//	object_wrapper<T> const& operator[](index)const;
	//	object_wrapper<T> & operator[](index);
	//};

	template<> class object_wrapper<collections::array_buffer<void>> { };

}

//#include <ang/collections/array_specialization.h>

#endif//__ANG_ARRAY_H__
