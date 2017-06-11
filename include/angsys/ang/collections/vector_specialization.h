/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_VECTOR_H__
#error ...
#elif !defined __ANG_VECTOR_SPECIALIZATION_H__
#define __ANG_VECTOR_SPECIALIZATION_H__

namespace ang
{
	template<> class LINK object_wrapper<collections::vector_buffer<objptr>>
	{
	public:
		typedef collections::vector_buffer<objptr> type;
		typedef typename collections::vector_buffer<objptr>::type data_type;

	private:
		collections::vector_buffer<objptr>* _ptr;

	public:
		object_wrapper();
		object_wrapper(collections::vector_buffer<objptr>*);
		object_wrapper(ang::initializer_list_t<objptr> list);
		object_wrapper(const collections::ienum<objptr>* store);
		object_wrapper(static_array<objptr> store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		template<class... Ts>
		object_wrapper(Ts const&... args) : _ptr(null) {
			set(new collections::vector_buffer<objptr>({ args... }));
		}
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::vector_buffer<objptr>* get(void)const;
		void set(collections::vector_buffer<objptr>*);
		collections::vector_buffer<objptr>** addres_of(void);

	public:
		object_wrapper& operator = (collections::vector_buffer<objptr>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<objptr> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (objptr);

		object_wrapper_ptr<collections::vector_buffer<objptr>> operator & (void);
		collections::vector_buffer<objptr> * operator -> (void);
		collections::vector_buffer<objptr> const* operator -> (void)const;
		explicit operator collections::vector_buffer<objptr> * (void);
		explicit operator collections::vector_buffer<objptr> const* (void)const;
		operator static_array<objptr>()const { return _ptr ? static_array<objptr>(_ptr->data(), _ptr->size()) : static_array<objptr>(); }

		objptr const& operator[](int idx)const;
		objptr & operator[](int idx);

		friend safe_pointer;
	};


	template<> class LINK object_wrapper<collections::vector_buffer<string>>
	{
	public:
		typedef collections::vector_buffer<string> type;
		typedef typename collections::vector_buffer<string>::type data_type;

	private:
		collections::vector_buffer<string>* _ptr;

	public:
		object_wrapper();
		object_wrapper(collections::vector_buffer<string>*);
		object_wrapper(ang::initializer_list_t<string> list);
		object_wrapper(ang::initializer_list_t<cstr_t> list);
		object_wrapper(ang::initializer_list_t<cwstr_t> list);
		object_wrapper(const collections::ienum<string>* store);
		object_wrapper(static_array<string> store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::vector_buffer<string>* get(void)const;
		void set(collections::vector_buffer<string>*);
		collections::vector_buffer<string>** addres_of(void);

	public:
		object_wrapper& operator = (collections::vector_buffer<string>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<string> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (string);

		object_wrapper_ptr<collections::vector_buffer<string>> operator & (void);
		collections::vector_buffer<string> * operator -> (void);
		collections::vector_buffer<string> const* operator -> (void)const;
		explicit operator collections::vector_buffer<string> * (void);
		explicit operator collections::vector_buffer<string> const* (void)const;
		operator static_array<string>()const { return _ptr ? static_array<string>(_ptr->data(), _ptr->size()) : static_array<string>(); }

		string const& operator[](int idx)const;
		string & operator[](int idx);

		friend safe_pointer;
	};


	template<> class LINK object_wrapper<collections::vector_buffer<wstring>>
	{
	public:
		typedef collections::vector_buffer<wstring> type;
		typedef typename collections::vector_buffer<wstring>::type data_type;

	private:
		collections::vector_buffer<wstring>* _ptr;

	public:
		object_wrapper();
		object_wrapper(collections::vector_buffer<wstring>*);
		object_wrapper(ang::initializer_list_t<wstring> list);
		object_wrapper(ang::initializer_list_t<cwstr_t> list);
		object_wrapper(ang::initializer_list_t<cstr_t> list);
		object_wrapper(const collections::ienum<wstring>* store);
		object_wrapper(static_array<wstring> store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		collections::vector_buffer<wstring>* get(void)const;
		void set(collections::vector_buffer<wstring>*);
		collections::vector_buffer<wstring>** addres_of(void);

	public:
		object_wrapper& operator = (collections::vector_buffer<wstring>*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<wstring> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (wstring);

		object_wrapper_ptr<collections::vector_buffer<wstring>> operator & (void);
		collections::vector_buffer<wstring> * operator -> (void);
		collections::vector_buffer<wstring> const* operator -> (void)const;
		explicit operator collections::vector_buffer<wstring> * (void);
		explicit operator collections::vector_buffer<wstring> const* (void)const;
		operator static_array<wstring>()const { return _ptr ? static_array<wstring>(_ptr->data(), _ptr->size()) : static_array<wstring>(); }

		wstring const& operator[](int idx)const;
		wstring & operator[](int idx);

		friend safe_pointer;
	};

}

#define ANG_DECLARE_OBJECT_VECTOR_SPECIALIZATION(_LINK, _TYPE)  namespace ang{ \
template<> class _LINK object_wrapper<collections::vector_buffer<object_wrapper<_TYPE>>> \
{ \
public: \
	typedef collections::vector_buffer<object_wrapper<_TYPE>> type; \
	typedef typename collections::vector_buffer<object_wrapper<_TYPE>>::type data_type; \
private: \
	collections::vector_buffer<object_wrapper<_TYPE>>* _ptr; \
public: \
	object_wrapper(); \
	object_wrapper(collections::vector_buffer<object_wrapper<_TYPE>>*); \
	object_wrapper(ang::initializer_list_t<object_wrapper<_TYPE>> list); \
	object_wrapper(const collections::ienum<object_wrapper<_TYPE>>* store); \
	object_wrapper(static_array<object_wrapper<_TYPE>> store);\
	object_wrapper(object_wrapper &&); \
	object_wrapper(object_wrapper const&); \
	object_wrapper(ang::nullptr_t const&); \
	~object_wrapper(); \
public: \
	void clean(); \
	void clean_unsafe(); \
	bool is_empty()const; \
	collections::vector_buffer<object_wrapper<_TYPE>>* get(void)const; \
	void set(collections::vector_buffer<object_wrapper<_TYPE>>*); \
	collections::vector_buffer<object_wrapper<_TYPE>>** addres_of(void); \
public: \
	object_wrapper& operator = (collections::vector_buffer<object_wrapper<_TYPE>>*); \
	object_wrapper& operator = (const ang::nullptr_t&); \
	object_wrapper& operator = (collections::ienum<object_wrapper<_TYPE>> const* items); \
	object_wrapper& operator = (object_wrapper &&); \
	object_wrapper& operator = (object_wrapper const&); \
	object_wrapper& operator += (object_wrapper<_TYPE>); \
	object_wrapper_ptr<collections::vector_buffer<object_wrapper<_TYPE>>> operator & (void); \
	collections::vector_buffer<object_wrapper<_TYPE>> * operator -> (void); \
	collections::vector_buffer<object_wrapper<_TYPE>> const* operator -> (void)const; \
	explicit operator collections::vector_buffer<object_wrapper<_TYPE>> * (void); \
	explicit operator collections::vector_buffer<object_wrapper<_TYPE>> const* (void)const; \
	operator static_array<object_wrapper<_TYPE>>()const { return _ptr ? static_array<object_wrapper<_TYPE>>(_ptr->data(), _ptr->size()) : static_array<object_wrapper<_TYPE>>(); }\
	object_wrapper<_TYPE> const& operator[](int idx)const; \
	object_wrapper<_TYPE> & operator[](int idx); \
	friend safe_pointer; \
};}


#define ANG_DECLARE_INTERFACE_VECTOR_SPECIALIZATION(_LINK, _TYPE)  namespace ang{ \
template<> class _LINK object_wrapper<collections::vector_buffer<intf_wrapper<_TYPE>>> \
{ \
public: \
	typedef collections::vector_buffer<intf_wrapper<_TYPE>> type; \
	typedef typename collections::vector_buffer<intf_wrapper<_TYPE>>::type data_type; \
private: \
	collections::vector_buffer<intf_wrapper<_TYPE>>* _ptr; \
public: \
	object_wrapper(); \
	object_wrapper(collections::vector_buffer<intf_wrapper<_TYPE>>*); \
	object_wrapper(ang::initializer_list_t<intf_wrapper<_TYPE>> list); \
	object_wrapper(const collections::ienum<intf_wrapper<_TYPE>>* store); \
	object_wrapper(static_array<intf_wrapper<_TYPE>> store);\
	object_wrapper(object_wrapper &&); \
	object_wrapper(object_wrapper const&); \
	object_wrapper(ang::nullptr_t const&); \
	~object_wrapper(); \
public: \
	void clean(); \
	void clean_unsafe(); \
	bool is_empty()const; \
	collections::vector_buffer<intf_wrapper<_TYPE>>* get(void)const; \
	void set(collections::vector_buffer<intf_wrapper<_TYPE>>*); \
	collections::vector_buffer<intf_wrapper<_TYPE>>** addres_of(void); \
public: \
	object_wrapper& operator = (collections::vector_buffer<intf_wrapper<_TYPE>>*); \
	object_wrapper& operator = (const ang::nullptr_t&); \
	object_wrapper& operator = (collections::ienum<intf_wrapper<_TYPE>> const* items); \
	object_wrapper& operator = (object_wrapper &&); \
	object_wrapper& operator = (object_wrapper const&); \
	object_wrapper& operator += (intf_wrapper<_TYPE>); \
	object_wrapper_ptr<collections::vector_buffer<intf_wrapper<_TYPE>>> operator & (void); \
	collections::vector_buffer<intf_wrapper<_TYPE>> * operator -> (void); \
	collections::vector_buffer<intf_wrapper<_TYPE>> const* operator -> (void)const; \
	explicit operator collections::vector_buffer<intf_wrapper<_TYPE>> * (void); \
	explicit operator collections::vector_buffer<intf_wrapper<_TYPE>> const* (void)const; \
	operator static_array<intf_wrapper<_TYPE>>()const { return _ptr ? static_array<intf_wrapper<_TYPE>>(_ptr->data(), _ptr->size()) : static_array<intf_wrapper<_TYPE>>(); }\
	intf_wrapper<_TYPE> const& operator[](int idx)const; \
	intf_wrapper<_TYPE> & operator[](int idx); \
	friend safe_pointer; \
};}


#define ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(_LINK, _TYPE)  namespace ang{ namespace collections { \
template<> class _LINK vector_buffer<_TYPE> final \
	: public object \
	, public ibuffer \
	, public ilist<_TYPE> \
	, public memory::iallocator_client \
{ \
public: \
	typedef _TYPE							type; \
	typedef vector_buffer<_TYPE>				self_t; \
	typedef icollection<_TYPE>		icollection_t; \
	typedef ienum<_TYPE>				ienum_t; \
	typedef ilist<_TYPE>				ilist_t; \
	typedef iterator<_TYPE>			iterator_t; \
protected: \
	uint _size; \
	uint _capacity; \
	type* _data; \
	memory::iallocator* allocator; \
public: \
	vector_buffer(); \
	vector_buffer(ang::initializer_list_t<_TYPE> list); \
	vector_buffer(const ang::nullptr_t&); \
	vector_buffer(vector_buffer&& ar); \
	vector_buffer(const vector_buffer& ar); \
	vector_buffer(const vector_buffer* ar); \
	vector_buffer(const ienum_t* store); \
	explicit vector_buffer(uint size, _TYPE const* ar = null); \
private: \
	virtual ~vector_buffer(); \
public: /*methods*/ \
	bool is_empty()const; \
	_TYPE* data()const; \
	uint size()const; \
	uint capacity()const; \
	void capacity(uint size, bool save = false); \
	void clean(); \
	void empty(); \
	bool move(vector_buffer&); \
	bool copy(uint size, _TYPE const* ar); \
	template<class U> inline bool copy_from(const collections::ienum<U>* items) { \
		if (!items) return false; \
		if (items->counter() > counter()) \
			capacity(items->counter(), false); \
		for (auto it = items->begin(); it.is_valid(); ++it) \
			append(*it); \
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
	bool realloc_buffer(wsize size)override; \
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
public: /*ilist overrides*/ \
	void extend(const ienum_t*) override; \
	void append(_TYPE const&, bool last = true) override; \
	bool insert(index idx, _TYPE const&) override; \
	bool insert(iterator_t it, _TYPE const&) override; \
	bool pop(bool last = true) override; \
	bool pop(_TYPE&, bool last = true) override; \
	bool pop_at(index idx) override; \
	bool pop_at(iterator_t it) override; \
	bool pop_at(index idx, _TYPE&) override; \
	bool pop_at(iterator_t it, _TYPE&) override; \
public: /*overrides*/ \
	static type_name_t class_name(); \
	type_name_t object_name()const override; \
	bool is_kind_of(type_name_t)const override; \
	bool query_object(type_name_t, unknown_ptr_t) override; \
public: /*Operators*/ \
	bool operator == (const vector_buffer& ar); \
	bool operator != (const vector_buffer& ar); \
	explicit operator _TYPE*()const; \
protected: /*Memory Operations*/ \
	bool realloc(uint size, bool save = true); \
	bool realloc(memory::iallocator* alloc); \
};} \
template<> class _LINK object_wrapper<collections::vector_buffer<_TYPE>> \
{ \
public: \
	typedef collections::vector_buffer<_TYPE> type; \
	typedef typename collections::vector_buffer<_TYPE>::type data_type; \
private: \
	collections::vector_buffer<_TYPE>* _ptr; \
public: \
	object_wrapper(); \
	object_wrapper(collections::vector_buffer<_TYPE>*); \
	object_wrapper(ang::initializer_list_t<data_type> list); \
	object_wrapper(const collections::ienum<data_type>* store); \
	explicit object_wrapper(uint size, data_type const* ar = null); \
	object_wrapper(static_array<_TYPE> store);\
	object_wrapper(object_wrapper &&); \
	object_wrapper(object_wrapper const&); \
	object_wrapper(ang::nullptr_t const&); \
	~object_wrapper(); \
public: \
	void clean(); \
	void clean_unsafe(); \
	bool is_empty()const; \
	collections::vector_buffer<_TYPE>* get(void)const; \
	void set(collections::vector_buffer<_TYPE>*); \
	collections::vector_buffer<_TYPE>** addres_of(void); \
public: \
	object_wrapper& operator = (collections::vector_buffer<_TYPE>*); \
	object_wrapper& operator = (const ang::nullptr_t&); \
	object_wrapper& operator = (collections::ienum<data_type> const* items); \
	object_wrapper& operator = (object_wrapper &&); \
	object_wrapper& operator = (object_wrapper const&); \
	object_wrapper& operator += (_TYPE); \
	object_wrapper_ptr<collections::vector_buffer<_TYPE>> operator & (void); \
	collections::vector_buffer<_TYPE> * operator -> (void); \
	collections::vector_buffer<_TYPE> const* operator -> (void)const; \
	explicit operator collections::vector_buffer<_TYPE> * (void); \
	explicit operator collections::vector_buffer<_TYPE> const* (void)const; \
	operator static_array<_TYPE>()const { return _ptr ? static_array<_TYPE>(_ptr->data(), _ptr->size()) : static_array<_TYPE>(); }\
	data_type const& operator[](int idx)const; \
	data_type & operator[](int idx); \
	friend safe_pointer; \
};}


ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, short)
ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, ushort)
ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, int)
ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, uint)
ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, long)
ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, ulong)
ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, long64)
ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, ulong64)
ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, float)
ANG_DECLARE_VECTOR_DATA_SPECIALIZATION(LINK, double)

#endif//__ANG_ARRAY_H__
