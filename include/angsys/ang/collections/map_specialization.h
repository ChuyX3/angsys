#ifndef __ANG_MAP_H__
#error ...
#elif !defined __ANG_MAP_SPEZIALIZATION_H__
#define __ANG_MAP_SPEZIALIZATION_H__

namespace ang
{

	namespace collections
	{
		template<typename T>
		class map_data<string, T> final
			: public object
			, public imap<string, T>
			, public memory::iallocator_client
		{
		public:
			typedef T							value_t;
			typedef string						key_t;
			typedef pair<string, T>				pair_t;
			typedef map_data<string, T>			self_t;
			typedef icollection<pair<string, T>>icollection_t;
			typedef ienum<pair<string, T>>		ienum_t;
			typedef imap<string, T>				imap_t;
			typedef iterator<pair<string, T >>	iterator_t;

		protected:
			typedef map_node<string, T> node_t;
			typedef map_node<string, T> *node_ptr_t;
			uint _size;
			uint _capacity;
			node_ptr_t* _table;
			memory::iallocator* allocator;

		public:
			inline map_data();
			inline map_data(std::initializer_list<pair<string, T>> list);
			inline map_data(const std::nullptr_t&);
			inline map_data(map_data&& ar);
			inline map_data(const map_data& ar);
			inline map_data(const map_data* ar);
			inline map_data(const ienum_t* store);

		private:
			virtual ~map_data();

		public: //methods
			inline bool is_empty()const;
			inline void clean();
			inline bool move(map_data&);

		public: //iallocator_client overrides
			inline void set_allocator(memory::iallocator*)override;
			inline memory::iallocator* get_allocator()const override;

		public: //ienum overrides
			inline pair_t& at(const iterator_t& it)const override;
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

		public: //imap overrides
			inline void extend(const ienum_t*) override;
			inline bool insert(cstr_t, T) override;
			inline bool insert(cwstr_t, T) override;
			inline bool insert(pair_t) override;
			inline bool update(cstr_t, T) override;
			inline bool update(cwstr_t, T) override;
			inline bool update(pair<string, T>) override;
			inline bool remove(cstr_t) override;
			inline bool remove(cwstr_t) override;
			inline bool remove(cstr_t, T&) override;
			inline bool remove(cwstr_t, T&) override;
			inline bool remove(iterator<pair<string, T>> it) override;
			inline bool remove(iterator<pair<string, T>> it, T&) override;
			inline bool has_key(cstr_t)const override;
			inline bool has_key(cwstr_t)const override;
			inline iterator<pair<string, T>> find(cstr_t)const override;
			inline iterator<pair<string, T>> find(cwstr_t)const override;

		private:
			iterator<pair<string, T>> find_index(index idx)const override { return iterator_t(); }
			index index_of(iterator<pair<string, T>> it)const override { return -1; }
			iterator<pair<string, T>> find(const pair<string, T>&, bool)const override { return iterator_t(); }
			iterator<pair<string, T>> find(const pair<string, T>&, iterator<pair<string, T>>, bool)const override { return iterator_t(); }

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool operator == (const map_data& ar);
			inline bool operator != (const map_data& ar);
			inline explicit operator T*()const;

		protected: //Memory Operations
			void increase_capacity();
			inline uint hash_index(cstr_t)const;
			inline uint hash_index(cwstr_t)const;
			inline node_ptr_t find_node(cstr_t)const;
			inline node_ptr_t find_node(cwstr_t)const;
			inline bool realloc(memory::iallocator* alloc);
		};



		template<typename T>
		class map_data<wstring , T> final
			: public object
			, public imap<wstring, T>
			, public memory::iallocator_client
		{
		public:
			typedef T							value_t;
			typedef wstring						key_t;
			typedef pair<wstring, T>				pair_t;
			typedef map_data<wstring, T>			self_t;
			typedef icollection<pair<wstring, T>>icollection_t;
			typedef ienum<pair<wstring, T>>		ienum_t;
			typedef imap<wstring, T>				imap_t;
			typedef iterator<pair<wstring, T >>	iterator_t;

		protected:
			typedef map_node<wstring, T> node_t;
			typedef map_node<wstring, T> *node_ptr_t;
			uint _size;
			uint _capacity;
			node_ptr_t* _table;
			memory::iallocator* allocator;

		public:
			inline map_data();
			inline map_data(std::initializer_list<pair<wstring, T>> list);
			inline map_data(const std::nullptr_t&);
			inline map_data(map_data&& ar);
			inline map_data(const map_data& ar);
			inline map_data(const map_data* ar);
			inline map_data(const ienum_t* store);

		private:
			virtual ~map_data();

		public: //methods
			inline bool is_empty()const;
			inline void clean();
			inline bool move(map_data&);

		public: //iallocator_client overrides
			inline void set_allocator(memory::iallocator*)override;
			inline memory::iallocator* get_allocator()const override;

		public: //ienum overrides
			inline pair_t& at(const iterator_t& it)const override;
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

		public: //imap overrides
			inline void extend(const ienum_t*) override;
			inline bool insert(cstr_t, T) override;
			inline bool insert(cwstr_t, T) override;
			inline bool insert(pair_t) override;
			inline bool update(cstr_t, T) override;
			inline bool update(cwstr_t, T) override;
			inline bool update(pair<wstring, T>) override;
			inline bool remove(cstr_t) override;
			inline bool remove(cwstr_t) override;
			inline bool remove(cstr_t, T&) override;
			inline bool remove(cwstr_t, T&) override;
			inline bool remove(iterator<pair<wstring, T>> it) override;
			inline bool remove(iterator<pair<wstring, T>> it, T&) override;
			inline bool has_key(cstr_t)const override;
			inline bool has_key(cwstr_t)const override;
			inline iterator<pair<wstring, T>> find(cstr_t)const override;
			inline iterator<pair<wstring, T>> find(cwstr_t)const override;

		private:
			iterator<pair<wstring, T>> find_index(index idx)const override { return iterator_t(this, null); }
			index index_of(iterator<pair<wstring, T>> it)const override { return -1; }
			iterator<pair<wstring, T>> find(const pair<wstring, T>&, bool)const override { return iterator_t(this, null); }
			iterator<pair<wstring, T>> find(const pair<wstring, T>&, iterator<pair<wstring, T>>, bool)const override { return iterator_t(this, null); }

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool operator == (const map_data& ar);
			inline bool operator != (const map_data& ar);
			inline explicit operator T*()const;

		protected: //Memory Operations
			void increase_capacity();
			inline uint hash_index(cstr_t)const;
			inline uint hash_index(cwstr_t)const;
			inline node_ptr_t find_node(cstr_t)const;
			inline node_ptr_t find_node(cwstr_t)const;
			inline bool realloc(memory::iallocator* alloc);
		};
	}

	template<typename T>
	class object_wrapper<collections::map_data<string, T>>
	{
	public:
		typedef collections::map_data<string, T> type;

	private:
		collections::map_data<string, T>* _ptr;

	public:
		object_wrapper();
		object_wrapper(std::nullptr_t const&);
		object_wrapper(collections::map_data<string, T>*);
		object_wrapper(collections::ienum<collections::pair<string, T>> const* store);
		object_wrapper(std::initializer_list<collections::pair<string, T>>);
		object_wrapper(object_wrapper<collections::map_data<string, T>> &&);
		object_wrapper(object_wrapper<collections::map_data<string, T>> const&);
		~object_wrapper();

	public:
		void clean();
		bool is_empty()const;
		collections::map_data<string, T>* get(void)const;
		void set(collections::map_data<string, T>*);
		collections::map_data<string, T> ** addres_of(void);

	public:
		object_wrapper& operator = (object_wrapper<collections::map_data<string, T>> &&);
		object_wrapper& operator = (object_wrapper<collections::map_data<string, T>> const&);
		object_wrapper& operator = (std::nullptr_t const&);
		object_wrapper& operator = (collections::map_data<string, T>*);
		object_wrapper& operator = (collections::ienum<collections::pair<string, T>> const* store);

		object_wrapper& operator += (collections::pair<string, T>);

		inline operator objptr()const;

		object_wrapper_ptr<collections::map_data<string, T>> operator & (void);
		collections::map_data<string, T> * operator -> (void);
		collections::map_data<string, T> const* operator -> (void)const;
		explicit operator collections::map_data<string, T> * (void);
		explicit operator collections::map_data<string, T> const* (void)const;

		T& operator [] (cstr_t);
		T& operator [] (cwstr_t);
		T operator [] (cstr_t)const;
		T operator [] (cwstr_t)const;
	};

	template<typename T>
	class object_wrapper<collections::map_data<wstring, T>>
	{
	public:
		typedef collections::map_data<wstring, T> type;

	private:
		collections::map_data<wstring, T>* _ptr;

	public:
		object_wrapper();
		object_wrapper(std::nullptr_t const&);
		object_wrapper(collections::map_data<wstring, T>*);
		object_wrapper(collections::ienum<collections::pair<wstring, T>> const* store);
		object_wrapper(std::initializer_list<collections::pair<wstring, T>>);
		object_wrapper(object_wrapper<collections::map_data<wstring, T>> &&);
		object_wrapper(object_wrapper<collections::map_data<wstring, T>> const&);
		~object_wrapper();

	public:
		void clean();
		bool is_empty()const;
		collections::map_data<wstring, T>* get(void)const;
		void set(collections::map_data<wstring, T>*);
		collections::map_data<wstring, T> ** addres_of(void);

	public:
		object_wrapper& operator = (object_wrapper<collections::map_data<wstring, T>> &&);
		object_wrapper& operator = (object_wrapper<collections::map_data<wstring, T>> const&);
		object_wrapper& operator = (std::nullptr_t const&);
		object_wrapper& operator = (collections::map_data<wstring, T>*);
		object_wrapper& operator = (collections::ienum<collections::pair<wstring, T>> const* store);

		object_wrapper& operator += (collections::pair<wstring, T>);

		inline operator objptr()const;

		object_wrapper_ptr<collections::map_data<wstring, T>> operator & (void);
		collections::map_data<wstring, T> * operator -> (void);
		collections::map_data<wstring, T> const* operator -> (void)const;
		explicit operator collections::map_data<wstring, T> * (void);
		explicit operator collections::map_data<wstring, T> const* (void)const;

		T& operator [] (cstr_t);
		T& operator [] (cwstr_t);
		T operator [] (cstr_t)const;
		T operator [] (cwstr_t)const;
	};

}

#endif//__ANG_MAP_SPEZIALIZATION_H__
