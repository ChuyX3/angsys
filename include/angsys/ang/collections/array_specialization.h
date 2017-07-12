/*********************************************************************************************************************/
/*   File Name: ang/collections/array_specialization.h                                                               */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: Implements array object specializations                                                       */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_ARRAY_H__
#error ...
#elif !defined __ANG_ARRAY_SPECIALIZATION_H__
#define __ANG_ARRAY_SPECIALIZATION_H__

namespace ang
{
	/******************************************************************/
	/* template class ang::object_wrapper<array_buffer> :             */
	/*  -> specialization of object_wrapper<array_buffer> -> array    */
	/******************************************************************/
	template<>
	class LINK object_wrapper<collections::array_buffer<objptr>>
	{
	public:
		typedef collections::array_buffer<objptr> type;
		typedef objptr data_type;

	private:
		collections::array_buffer<objptr>* _ptr;

	public:
		object_wrapper();
		explicit object_wrapper(uint size);
		object_wrapper(collections::array_buffer<objptr>*);
		object_wrapper(ang::initializer_list_t<objptr> list);
		object_wrapper(const collections::ienum<objptr>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::array_buffer<objptr>* get(void)const;
		void set(collections::array_buffer<objptr>*);
		collections::array_buffer<objptr>** addres_of(void);

	public:
		object_wrapper& operator = (collections::array_buffer<objptr>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<objptr> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<collections::array_buffer<objptr>> operator & (void);
		collections::array_buffer<objptr> * operator -> (void);
		collections::array_buffer<objptr> const* operator -> (void)const;
		explicit operator collections::array_buffer<objptr> * (void);
		explicit operator collections::array_buffer<objptr> const* (void)const;

		operator static_array<objptr>()const { return _ptr ? static_array<objptr>(_ptr->data(), _ptr->size()) : static_array<objptr>(); }

		objptr const& operator[](int idx)const;
		objptr & operator[](int idx);

		friend safe_pointer;
	};

	/******************************************************************/
	/* template class ang::object_wrapper<array_buffer> :             */
	/*  -> specialization of object_wrapper<array_buffer> -> array    */
	/******************************************************************/
	template<>
	class LINK object_wrapper<collections::array_buffer<wstring>>
	{
	public:
		typedef collections::array_buffer<wstring> type;
		typedef wstring data_type;

	private:
		collections::array_buffer<wstring>* _ptr;

	public:
		object_wrapper();
		explicit object_wrapper(uint size);
		object_wrapper(collections::array_buffer<wstring>*);
		object_wrapper(ang::initializer_list_t<wstring> list);
		object_wrapper(ang::initializer_list_t<cstr_t> list);
		object_wrapper(ang::initializer_list_t<cwstr_t> list);
		object_wrapper(const collections::ienum<wstring>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::array_buffer<wstring>* get(void)const;
		void set(collections::array_buffer<wstring>*);
		collections::array_buffer<wstring>** addres_of(void);

	public:
		object_wrapper& operator = (collections::array_buffer<wstring>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<wstring> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<collections::array_buffer<wstring>> operator & (void);
		collections::array_buffer<wstring> * operator -> (void);
		collections::array_buffer<wstring> const* operator -> (void)const;
		explicit operator collections::array_buffer<wstring> * (void);
		explicit operator collections::array_buffer<wstring> const* (void)const;

		operator static_array<wstring>()const { return _ptr ? static_array<wstring>(_ptr->data(), _ptr->size()) : static_array<wstring>(); }

		wstring const& operator[](int idx)const;
		wstring & operator[](int idx);

		friend safe_pointer;
	};

	/******************************************************************/
	/* template class ang::object_wrapper<array_buffer> :             */
	/*  -> specialization of object_wrapper<array_buffer> -> array    */
	/******************************************************************/
	template<>
	class LINK object_wrapper<collections::array_buffer<string>>
	{
	public:
		typedef collections::array_buffer<string> type;
		typedef string data_type;

	private:
		collections::array_buffer<string>* _ptr;

	public:
		object_wrapper();
		explicit object_wrapper(uint size);
		object_wrapper(collections::array_buffer<string>*);
		object_wrapper(ang::initializer_list_t<string> list);
		object_wrapper(ang::initializer_list_t<cstr_t> list);
		object_wrapper(ang::initializer_list_t<cwstr_t> list);
		object_wrapper(const collections::ienum<string>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::array_buffer<string>* get(void)const;
		void set(collections::array_buffer<string>*);
		collections::array_buffer<string>** addres_of(void);

	public:
		object_wrapper& operator = (collections::array_buffer<string>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<string> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<collections::array_buffer<string>> operator & (void);
		collections::array_buffer<string> * operator -> (void);
		collections::array_buffer<string> const* operator -> (void)const;
		explicit operator collections::array_buffer<string> * (void);
		explicit operator collections::array_buffer<string> const* (void)const;
		operator static_array<string>()const { return _ptr ? static_array<string>(_ptr->data(), _ptr->size()) : static_array<string>(); }

		string const& operator[](int idx)const;
		string & operator[](int idx);

		friend safe_pointer;
	};

}

#define ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(_LINK, _TYPE) namespace ang { namespace collections { \
template<> class _LINK array_buffer<_TYPE> final \
	: public object \
	, public ibuffer \
	, public icollection<_TYPE> \
	, public memory::iallocator_client \
{ \
public: \
	typedef _TYPE							type; \
	typedef array_buffer<_TYPE>				self_t; \
	typedef icollection<_TYPE>				icollection_t; \
	typedef ienum<_TYPE>					ienum_t; \
	typedef iterator<_TYPE>					iterator_t; \
protected: \
	uint _size; \
	type* _data; \
	memory::iallocator* allocator; \
public: \
	array_buffer(); \
	array_buffer(ang::initializer_list_t<_TYPE> list); \
	array_buffer(const ang::nullptr_t&); \
	array_buffer(array_buffer&& ar); \
	array_buffer(const array_buffer& ar); \
	array_buffer(const array_buffer* ar); \
	array_buffer(const ienum_t* store); \
	explicit array_buffer(uint size, _TYPE const* ar = null); \
private: \
	virtual ~array_buffer(); \
public: /*methods*/ \
	bool is_empty()const; \
	_TYPE* data()const; \
	uint size()const; \
	void size(uint size); \
	void clean(); \
	bool move(array_buffer<_TYPE>&); \
	bool copy(uint size, _TYPE const* ar); \
	template<class U> bool copy_from(const collections::ienum<U>& items) { \
		if (items.counter() != counter()) \
			size(items.counter()); \
		index i = 0; \
		for (auto it = items.begin(); it.is_valid(); ++it) \
			_data[i++] = *it; \
		return true; \
	} \
public: /*iallocator_client overrides*/ \
	void set_allocator(memory::iallocator*)override; \
	memory::iallocator* get_allocator()const override; \
public: /*ibuffer overrides*/ \
	pointer buffer_ptr()const override; \
	wsize buffer_size()const override; \
	wsize mem_copy(wsize, pointer) override; \
	pointer map_buffer(windex, wsize) override; \
	void unmap_buffer(pointer, wsize) override; \
	bool can_realloc_buffer()const override; \
	bool realloc_buffer(wsize) override; \
public: /*ienum overrides*/ \
	_TYPE& at(const iterator_t& it)const override; \
	iterator_t begin()const override; \
	iterator_t end()const override; \
	bool next(iterator_t& it)const override; \
	bool next(iterator_t& it, int offset)const override; \
	bool prev(iterator_t& it)const override; \
	bool prev(iterator_t& it, int offset)const override; \
	uint counter()const override; \
public: /*icollection overrides*/ \
	bool has_items()const override; \
	bool copy(const ienum_t*) override; \
	index index_of(iterator_t it)const override; \
	iterator_t find_index(index idx)const override; \
	iterator_t find(const _TYPE&, bool invert = false)const override; \
	iterator_t find(const _TYPE&, iterator_t nextTo, bool invert = false)const override; \
public: /*overrides*/ \
	static type_name_t class_name(); \
	type_name_t object_name()const override; \
	bool is_kind_of(type_name_t)const override; \
	bool query_object(type_name_t, unknown_ptr_t) override; \
public: /*Operators*/ \
	bool operator == (const array_buffer& ar); \
	bool operator != (const array_buffer& ar); \
	explicit operator _TYPE*()const; \
protected: /*Memory Operations*/ \
	bool realloc(uint size); \
	bool realloc(memory::iallocator* alloc); \
};} \
template<> class _LINK object_wrapper<collections::array_buffer<_TYPE>> \
{ \
public: \
	typedef collections::array_buffer<_TYPE> type; \
	typedef typename collections::array_buffer<_TYPE>::type data_type; \
private: \
	collections::array_buffer<_TYPE>* _ptr; \
public: \
	object_wrapper(); \
	object_wrapper(collections::array_buffer<_TYPE>*); \
	object_wrapper(ang::initializer_list_t<data_type> list); \
	object_wrapper(const collections::ienum<data_type>* store); \
	explicit object_wrapper(uint size, data_type const* ar = null); \
	object_wrapper(object_wrapper &&); \
	object_wrapper(object_wrapper const&); \
	object_wrapper(ang::nullptr_t const&); \
	~object_wrapper(); \
public: \
	void clean(); \
	void clean_unsafe(); \
	bool is_empty()const; \
	collections::array_buffer<_TYPE>* get(void)const; \
	void set(collections::array_buffer<_TYPE>*); \
	collections::array_buffer<_TYPE>** addres_of(void); \
public: \
	object_wrapper& operator = (collections::array_buffer<_TYPE>*); \
	object_wrapper& operator = (const ang::nullptr_t&); \
	object_wrapper& operator = (collections::ienum<data_type> const* items); \
	object_wrapper& operator = (object_wrapper &&); \
	object_wrapper& operator = (object_wrapper const&); \
	object_wrapper_ptr<collections::array_buffer<_TYPE>> operator & (void); \
	collections::array_buffer<_TYPE> * operator -> (void); \
	collections::array_buffer<_TYPE> const* operator -> (void)const; \
	explicit operator collections::array_buffer<_TYPE> * (void); \
	explicit operator collections::array_buffer<_TYPE> const* (void)const; \
	operator static_array<_TYPE>()const { return _ptr ? static_array<_TYPE>(_ptr->data(), _ptr->size()) : static_array<_TYPE>(); }\
	data_type const& operator[](int idx)const; \
	data_type & operator[](int idx); \
	friend safe_pointer; \
};}

ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, char)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, wchar)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, mchar)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, short)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, ushort)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, int)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, uint)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, long)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, ulong)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, long64)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, ulong64)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, float)
ANG_DECLARE_ARRAY_DATA_SPECIALIZATION(LINK, double)

#endif//__ANG_ARRAY_H__
