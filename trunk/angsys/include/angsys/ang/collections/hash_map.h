#ifndef __ANG_COLLECTIONS_HASH_MAP_H__
#define __ANG_COLLECTIONS_HASH_MAP_H__

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
#else
#define LINK
#endif

namespace ang
{
	namespace algorithms
	{
		wsize LINK hash_table_get_next_size(wsize);
		
		template<typename K>
		struct hash_index_maker {
			static inline ulong64 make(const K& key_, ulong64 TS) {
				static_assert(genre_of<K>() == genre::value_type || genre_of<K>() == genre::enum_type, "invaid type for template hash_index_maker");
				union { K k; ulong64 val; } key;
				key.val = 0;
				key.k = key_;
				return (ulong64)((2475825 + key.val + 1) % TS);
			}
		};

		template<typename T, text::encoding E>
		struct hash_index_maker<str_view<T, E>> {
			static inline ulong64 make(const str_view<T, E>& key, ulong64 TS) {
				ulong64 h = 75025;
				windex i = 0, c = key.size();

				for (char32_t n = text::to_char32<false, text::is_endian_swapped<E>::value>(key.cstr(), i);
					n != 0 && i < c;
					n = text::to_char32<false, text::is_endian_swapped<E>::value>(key.cstr(), i))
				{
					h = (h << 5) + h + dword(n) + 1;
				}
				return ang_uint64_t(h % TS);
			}
		};

		template<text::encoding E, template<typename>class A>
		struct hash_index_maker<text::basic_string<E,A>> {
			static inline long64 make(const text::basic_string<E, A>& key_, ulong64 TS) {
				ulong64 h = 75025;
				auto key = key_->cstr();
				windex i = 0, c = key.size();

				for (char32_t n = text::to_char32<false, text::is_endian_swapped<E>::value>(key.cstr(), i);
					n != 0 && i < c;
					n = text::to_char32<false, text::is_endian_swapped<E>::value>(key.cstr(), i))
				{
					h = (h << 5) + h + dword(n) + 1;
				}
				return ang_uint64_t(h % TS);
			}
		};

		template<> struct LINK hash_index_maker<str_t> {
			static long64 make(str_t const& key_, ulong64 TS);
		};

		template<> struct LINK hash_index_maker<cstr_t> {
			static long64 make(cstr_t const& key_, ulong64 TS);
		};
	}

	namespace collections
	{


		template<typename K, typename T, template<typename> class allocator = memory::default_allocator, template<typename> class hash_index_maker = algorithms::hash_index_maker> class hash_map_object;
		template<typename K, typename T, template<typename> class allocator = memory::default_allocator, template<typename> class hash_index_maker = algorithms::hash_index_maker> using hash_map = object_wrapper<hash_map_object<K, T, allocator, hash_index_maker>>;

		/******************************************************************/
		/* template class hash_map_object :                               */
		/*  -> implements a hash table object                             */
		/******************************************************************/
		template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
		class hash_map_object final
			: public smart<hash_map_object<K, T, allocator>, imap<K, T>>
		{
		public:
			typedef smart<hash_map_object<K, T, allocator>, imap<K, T>> base;
			typedef pair<K, T>							type, pair_type;
			typedef T									value_type;
			typedef K									key_type;
			typedef hash_map_object<K, T, allocator, hash_index_maker>	self_type;
			typedef hash_index_maker<K>					index_maker;
			typedef imap<K, T>							imap_type;
			typedef ienum<type>							ienum_type;
			typedef base_iterator<type>					base_iterator_t;
			typedef iterator<type>						iterator_t;
			typedef const_iterator<type>				const_iterator_t;
			typedef forward_iterator<type>				forward_iterator_t;
			typedef const_forward_iterator<type>		const_forward_iterator_t;
			typedef backward_iterator<type>				backward_iterator_t;
			typedef const_backward_iterator<type>		const_backward_iterator_t;
			typedef double_linked_node<type>			node_t, *node_ptr_t;
			typedef scope_array<node_ptr_t, allocator>	node_array_t;


		protected:
			allocator<node_t> m_alloc;
			wsize m_count;
			node_array_t m_table;

		public:
			inline hash_map_object();
			inline hash_map_object(wsize reserve);
			template<typename U, typename V>inline hash_map_object(ang::initializer_list<pair<U, V>> list);
			inline hash_map_object(const ang::nullptr_t&);
			inline hash_map_object(hash_map_object&& ar);
			inline hash_map_object(const hash_map_object& ar);
			inline hash_map_object(const hash_map_object* ar);
			inline hash_map_object(const ienum_type* store);

		private:
			virtual ~hash_map_object();

		public: //methods
			inline bool is_empty()const;
			//inline pair_type* data()const;
			//inline wsize size()const;
			//inline void size(wsize);
			//inline wsize capacity()const;
			//inline void capacity(wsize size, bool save = false);

			inline void clear();
			inline void empty();
			inline bool move(hash_map_object<K, T, allocator, hash_index_maker>&);

			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

			template<typename U> inline void extend(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void extend(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void extend(stack_array<U, SIZE> const&);

		public: //ienum overrides
			inline wsize counter()const override;

			inline type& at(base_iterator_t const&) override;
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

		public: //imap overrides
			inline bool copy(const ienum<type>*) override;
			inline void extend(const ienum<type>*) override;
			inline iterator<type> insert(K, T) override;
			inline iterator<type> insert(type) override;
			inline iterator<type> update(K, T) override;
			inline iterator<type> update(type) override;
			inline bool remove(K const&) override;
			inline bool remove(K const&, T&) override;
			inline bool remove(base_iterator<type> it) override;
			inline bool remove(base_iterator<type> it, T&) override;
			inline bool has_key(const K&)const override;
			inline iterator<type> find(const K&) override;
			inline const_iterator<type> find(const K&)const override;

		public: //overrides
			ANG_DECLARE_INTERFACE();
			//inline comparision_result_t compare(const object*)const override;
	
		protected: //Memory Operations
			template<typename U, typename V> inline node_ptr_t allocate_node(U key, V val) {
				node_ptr_t node = m_alloc.allocate(1);
				m_alloc.template construct<node_t>(node, forward<U>(key), forward<V>(val));
				return node;
			}
			inline void increase_capacity();
			inline wsize hash_index(key_type const&)const;
			inline node_ptr_t find_node(key_type const&)const;
		};


		template<text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
		class hash_map_object<text::basic_string<E,A>, T, allocator, hash_index_maker> final
			: public smart<hash_map_object<text::basic_string<E, A>, T, allocator>, imap<text::basic_string<E, A>, T>>
		{
		public:
			typedef pair<text::basic_string<E,A>, T>							type, pair_type;
			typedef T															value_type;
			typedef text::basic_string<E,A>										key_type;
			typedef hash_map_object<text::basic_string<E,A>, T, allocator, hash_index_maker> self_type;
			typedef hash_index_maker<text::basic_string<E,A>>					index_maker;
			typedef imap<text::basic_string<E,A>, T>							imap_type;
			typedef ienum<type>													ienum_type;
			typedef base_iterator<type>											base_iterator_t;
			typedef iterator<type>												iterator_t;
			typedef const_iterator<type>										const_iterator_t;
			typedef forward_iterator<type>										forward_iterator_t;
			typedef const_forward_iterator<type>								const_forward_iterator_t;
			typedef backward_iterator<type>										backward_iterator_t;
			typedef const_backward_iterator<type>								const_backward_iterator_t;
			typedef double_linked_node<type>									node_t, *node_ptr_t;
			typedef scope_array<node_ptr_t, allocator>							node_array_t;


		protected:
			allocator<node_t> m_alloc;
			wsize m_count;
			node_array_t m_table;

		public:
			inline hash_map_object();
			inline hash_map_object(wsize reserve);
			inline hash_map_object(ang::initializer_list<pair_type> list);
			inline hash_map_object(const ang::nullptr_t&);
			inline hash_map_object(hash_map_object&& ar);
			inline hash_map_object(const hash_map_object& ar);
			inline hash_map_object(const hash_map_object* ar);
			inline hash_map_object(const ienum_type* store);

		private:
			virtual ~hash_map_object();

		public: //methods
			inline bool is_empty()const;
			//inline T* data()const;
			//inline wsize size()const;
			//inline void size(wsize);
			//inline wsize capacity()const;
			//inline void capacity(wsize size, bool save = false);

			inline void clear();
			inline void empty();
			inline bool move(hash_map_object<text::basic_string<E,A>, T, allocator, hash_index_maker>&);

			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

			template<typename U> inline void extend(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void extend(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void extend(stack_array<U, SIZE> const&);

		public: //ienum overrides
			inline wsize counter()const override;

			inline type& at(base_iterator_t const&) override;
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

		public: //imap overrides
			inline bool copy(const ienum<type>*) override;
			inline void extend(const ienum<type>*) override;
			inline iterator<type> insert(cstr_t, T) override;
			inline iterator<type> insert(type) override;
			inline iterator<type> update(cstr_t, T) override;
			inline iterator<type> update(type) override;
			inline bool remove(cstr_t) override;
			inline bool remove(cstr_t, T&) override;
			inline bool remove(base_iterator<type> it) override;
			inline bool remove(base_iterator<type> it, T&) override;
			inline bool has_key(cstr_t)const override;
			inline iterator<type> find(cstr_t) override;
			inline const_iterator<type> find(cstr_t)const override;

		public: //overrides
			ANG_DECLARE_INTERFACE();
			//inline comparision_result_t compare(const object*)const override;

		protected: //Memory Operations
			template<typename U, typename V> inline node_ptr_t allocate_node(U key, V val) {
				node_ptr_t node = m_alloc.allocate(1);
				m_alloc.template construct<node_t>(node, forward<U>(key), forward<V>(val));
				return node;
			}
			inline void increase_capacity();
			inline wsize hash_index(cstr_t const&)const;
			inline node_ptr_t find_node(cstr_t const&)const;
		};

		template<typename T, template<typename> class allocator, template<typename> class hash_index_maker>
		class hash_map_object<string, T, allocator, hash_index_maker> final
			: public smart<hash_map_object<string, T, allocator>, imap<string, T>>
		{
		public:
			typedef pair<string, T>						type, pair_type;
			typedef T									value_type;
			typedef string								key_type;
			typedef hash_map_object<string, T, allocator, hash_index_maker>	self_type;
			typedef hash_index_maker<cstr_t>			index_maker;
			typedef imap<string, T>						imap_type;
			typedef ienum<type>							ienum_type;
			typedef base_iterator<type>					base_iterator_t;
			typedef iterator<type>						iterator_t;
			typedef const_iterator<type>				const_iterator_t;
			typedef forward_iterator<type>				forward_iterator_t;
			typedef const_forward_iterator<type>		const_forward_iterator_t;
			typedef backward_iterator<type>				backward_iterator_t;
			typedef const_backward_iterator<type>		const_backward_iterator_t;
			typedef double_linked_node<type>			node_t, *node_ptr_t;
			typedef scope_array<node_ptr_t, allocator>	node_array_t;


		protected:
			allocator<node_t> m_alloc;
			wsize m_count;
			node_array_t m_table;

		public:
			inline hash_map_object();
			inline hash_map_object(wsize reserve);
			inline hash_map_object(ang::initializer_list<pair_type> list);
			inline hash_map_object(const ang::nullptr_t&);
			inline hash_map_object(hash_map_object&& ar);
			inline hash_map_object(const hash_map_object& ar);
			inline hash_map_object(const hash_map_object* ar);
			inline hash_map_object(const ienum_type* store);

		private:
			virtual ~hash_map_object();

		public: //methods
			inline bool is_empty()const;

			inline void clear();
			inline void empty();
			inline bool move(hash_map_object<string, T, allocator, hash_index_maker>&);

			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

			template<typename U> inline void extend(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void extend(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void extend(stack_array<U, SIZE> const&);

		public: //ienum overrides
			inline wsize counter()const override;

			inline type& at(base_iterator_t const&) override;
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

		public: //imap overrides
			inline bool copy(const ienum<type>*) override;
			inline void extend(const ienum<type>*) override;
			inline iterator<type> insert(cstr_t, T) override;
			inline iterator<type> insert(type) override;
			inline iterator<type> update(cstr_t, T) override;
			inline iterator<type> update(type) override;
			inline bool remove(cstr_t) override;
			inline bool remove(cstr_t, T&) override;
			inline bool remove(base_iterator<type> it) override;
			inline bool remove(base_iterator<type> it, T&) override;
			inline bool has_key(cstr_t)const override;
			inline iterator<type> find(cstr_t) override;
			inline const_iterator<type> find(cstr_t)const override;

		public: //overrides
			ANG_DECLARE_INTERFACE();
			//inline comparision_result_t compare(const object*)const override;

		protected: //Memory Operations
			template<typename U, typename V> inline node_ptr_t allocate_node(U key, V val) {
				node_ptr_t node = m_alloc.allocate(1);
				m_alloc.template construct<node_t>(node, forward<U>(key), forward<V>(val));
				return node;
			}
			inline void increase_capacity();
			inline wsize hash_index(cstr_t const&)const;
			inline node_ptr_t find_node(cstr_t const&)const;
		};

	}//collections


	/********************************************************************/
	/* template class ang::object_wrapper<hash_map_object> :            */
	/*  -> specialization of object_wrapper<hash_map_object> -> has_map */
	/********************************************************************/
	template<typename K, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
	class object_wrapper<collections::hash_map_object<K, T, allocator, hash_index_maker>>
	{
	public:
		typedef collections::hash_map_object<K, T, allocator, hash_index_maker> type;
		typedef typename collections::hash_map_object<K, T, allocator, hash_index_maker>::type pair_type;
		typedef typename collections::hash_map_object<K, T, allocator, hash_index_maker>::value_type data_type;
		typedef typename collections::hash_map_object<K, T, allocator, hash_index_maker>::key_type key_type;

	private:
		type* m_ptr;

	public:
		object_wrapper();
		object_wrapper(type*);
		object_wrapper(ang::initializer_list<pair_type> list);
		object_wrapper(const collections::ienum<pair_type>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type** addres_of(void);
		type** addres_for_init(void);

		collections::forward_iterator<pair_type> begin() { return m_ptr ? m_ptr->begin() : collections::iterator<pair_type>(); }
		collections::forward_iterator<pair_type> end() { return m_ptr ? m_ptr->end() : collections::iterator<pair_type>(); }
		collections::forward_iterator<const pair_type> begin()const { return m_ptr ? m_ptr->begin() : collections::iterator<const pair_type>(); }
		collections::forward_iterator<const pair_type> end()const { return m_ptr ? m_ptr->end() : collections::iterator<const pair_type>(); }

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<pair_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (pair_type item);
		object_wrapper& operator += (collections::ienum<pair_type> const* items);
		object_wrapper& operator += (object_wrapper const&);

		object_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		explicit operator type * (void);
		explicit operator type const* (void)const;

		data_type& operator [] (key_type const&);
		data_type operator [] (key_type const&)const;
	};


	/********************************************************************/
	/* template class ang::object_wrapper<hash_map_object> :            */
	/*  -> specialization of object_wrapper<hash_map_object> -> has_map */
	/********************************************************************/
	template<text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
	class object_wrapper<collections::hash_map_object<text::basic_string<E, A>, T, allocator, hash_index_maker>>
	{
	public:
		typedef collections::hash_map_object<text::basic_string<E,A>, T, allocator, hash_index_maker> type;
		typedef typename collections::hash_map_object<text::basic_string<E,A>, T, allocator, hash_index_maker>::type pair_type;
		typedef typename collections::hash_map_object<text::basic_string<E,A>, T, allocator, hash_index_maker>::value_type data_type;
		typedef typename collections::hash_map_object<text::basic_string<E,A>, T, allocator, hash_index_maker>::key_type key_type;

	private:
		type* m_ptr;

	public:
		object_wrapper();
		object_wrapper(type*);
		object_wrapper(ang::initializer_list<pair_type> list);
		object_wrapper(const collections::ienum<pair_type>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type** addres_of(void);
		type** addres_for_init(void);

		collections::forward_iterator<pair_type> begin() { return m_ptr ? m_ptr->begin() : collections::iterator<pair_type>(); }
		collections::forward_iterator<pair_type> end() { return m_ptr ? m_ptr->end() : collections::iterator<pair_type>(); }
		collections::forward_iterator<const pair_type> begin()const { return m_ptr ? m_ptr->begin() : collections::iterator<const pair_type>(); }
		collections::forward_iterator<const pair_type> end()const { return m_ptr ? m_ptr->end() : collections::iterator<const pair_type>(); }

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<pair_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (pair_type item);
		object_wrapper& operator += (collections::ienum<pair_type> const* items);
		object_wrapper& operator += (object_wrapper const&);

		object_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		explicit operator type * (void);
		explicit operator type const* (void)const;

		data_type& operator [] (cstr_t const&);
		data_type operator [] (cstr_t const&)const;
	};


	/********************************************************************/
	/* template class ang::object_wrapper<hash_map_object> :            */
	/*  -> specialization of object_wrapper<hash_map_object> -> has_map */
	/********************************************************************/
	template<typename T, template<typename> class allocator, template<typename> class hash_index_maker>
	class object_wrapper<collections::hash_map_object<string, T, allocator, hash_index_maker>>
	{
	public:
		typedef collections::hash_map_object<string, T, allocator, hash_index_maker> type;
		typedef typename collections::hash_map_object<string, T, allocator, hash_index_maker>::type pair_type;
		typedef typename collections::hash_map_object<string, T, allocator, hash_index_maker>::value_type data_type;
		typedef typename collections::hash_map_object<string, T, allocator, hash_index_maker>::key_type key_type;

	private:
		type* m_ptr;

	public:
		object_wrapper();
		object_wrapper(type*);
		object_wrapper(ang::initializer_list<pair_type> list);
		object_wrapper(const collections::ienum<pair_type>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type** addres_of(void);
		type** addres_for_init(void);

		collections::forward_iterator<pair_type> begin() { return m_ptr ? m_ptr->begin() : collections::iterator<pair_type>(); }
		collections::forward_iterator<pair_type> end() { return m_ptr ? m_ptr->end() : collections::iterator<pair_type>(); }
		collections::forward_iterator<const pair_type> begin()const { return m_ptr ? m_ptr->begin() : collections::iterator<const pair_type>(); }
		collections::forward_iterator<const pair_type> end()const { return m_ptr ? m_ptr->end() : collections::iterator<const pair_type>(); }

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (const ang::nullptr_t&);
		object_wrapper& operator = (collections::ienum<pair_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (pair_type item);
		object_wrapper& operator += (collections::ienum<pair_type> const* items);
		object_wrapper& operator += (object_wrapper const&);

		object_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		explicit operator type * (void);
		explicit operator type const* (void)const;

		data_type& operator [] (cstr_t const&);
		data_type operator [] (cstr_t const&)const;
	};

}//ang


#ifdef  LINK
#undef  LINK
#endif//LINK

#include <ang/collections/inline/hash_map.inl>

#endif //__ANG_COLLECTIONS_HASH_MAP_H__