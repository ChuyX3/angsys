#ifndef __ANG_MAP_H__
#define __ANG_MAP_H__

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
		template<typename K, typename T> class map_data;
		template<typename K, typename T> using map = object_wrapper<map_data<K, T>>;
	}

	namespace collections
	{

		template<typename K, typename T>
		struct map_node
		{
			typedef K key_t;
			typedef T value_t;
			typedef collections::pair<K,T> pair_t;
			typedef map_node<K, T> self_t, *self_ptr_t;

			static inline type_name_t class_name();
			static inline bool is_child_of(type_name_t);

			map_node(K key) : _pair(key){}
			map_node(K key, value_t val) : _pair(ang::move(key), ang::move(val)) {}
			map_node(pair_t p) : _pair(ang::move(p)) {}

			inline key_t& key() {
				return _pair.key_value();
			}
			inline key_t const& key()const {
				return _pair.key_value();
			}
			inline void key(key_t p) {
				_pair.key_value(ang::move(p));
			}
			
			inline value_t& value() {
				return _pair.value();
			}
			inline value_t const& value()const {
				return _pair.value();
			}
			inline void value(value_t val) {
				_pair.value(ang::move(val));
			}

			inline self_ptr_t& prev() {
				return _prev;
			}
			inline self_ptr_t const& prev()const {
				return _prev;
			}
			inline void prev(self_ptr_t n) {
				_prev = ang::move(n);
			}

			inline self_ptr_t& next() {
				return _next;
			}
			inline self_ptr_t const& next()const {
				return _next;
			}
			inline void next(self_ptr_t n) {
				_next = ang::move(n);
			}

			pair_t _pair;
			self_ptr_t _prev = null;
			self_ptr_t _next = null;
		};

		template<typename K, typename T>
		class map_data final
			: public object
			, public imap<K, T>
			, public memory::iallocator_client
		{
		public:
			typedef T							value_t;
			typedef K							key_t;
			typedef pair<K, T>					pair_t;
			typedef map_data<K, T>				self_t;
			typedef icollection<pair<K, T>>		icollection_t;
			typedef ienum<pair<K, T>>			ienum_t;
			typedef imap<K, T>					imap_t;
			typedef iterator<pair<K, T >>		iterator_t;

		protected:
			typedef map_node<K, T> node_t;
			typedef map_node<K, T> *node_ptr_t;
			uint _size;
			uint _capacity;
			node_ptr_t* _table;
			memory::iallocator* allocator;

		public:
			inline map_data();
			inline map_data(std::initializer_list<pair<K, T>> list);
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
			inline bool insert(K,T) override;
			inline bool insert(pair_t) override;
			inline bool update(K, T) override;
			inline bool update(pair<K, T>) override;
			inline bool remove(K const&) override;
			inline bool remove(K const&,T&) override;
			inline bool remove(iterator<pair<K, T>> it) override;
			inline bool remove(iterator<pair<K, T>> it, T&) override;
			inline bool has_key(const K&)const override;
			inline iterator<pair<K, T>> find(const K&)const override;

		private:
			inline iterator<pair<K, T>> find_index(index idx)const override { return iterator_t(const_cast<map_data<K, T>*>(this), null); }
			inline index index_of(iterator<pair<K, T>> it)const override { return -1; }
			inline iterator<pair<K, T>> find(const pair<K, T>&, bool)const override { return iterator_t(const_cast<map_data<K, T>*>(this), null); }
			inline iterator<pair<K, T>> find(const pair<K, T>&, iterator<pair<K, T>>, bool)const override { return iterator_t(const_cast<map_data<K,T>*>(this), null); }

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
			inline uint hash_index(key_t const&)const;
			inline node_ptr_t find_node(key_t const&)const;
			inline bool realloc(memory::iallocator* alloc);
		};

		uint LINK map_get_next_size(uint);
		uint LINK create_hash_index(int, uint);
		uint LINK create_hash_index(uint, uint);
		uint LINK create_hash_index(long, uint);
		uint LINK create_hash_index(ulong, uint);
		uint LINK create_hash_index(long64, uint);
		uint LINK create_hash_index(ulong64, uint);
		uint LINK create_hash_index(float, uint);
		uint LINK create_hash_index(double, uint);
		uint LINK create_hash_index(pointer, uint);
		uint LINK create_hash_index(cstr_t, uint);
		uint LINK create_hash_index(cwstr_t, uint);
	}


	template<typename K, typename T>
	class object_wrapper<collections::map_data<K,T>>
	{
	public:
		typedef collections::map_data<K,T> type;

	private:
		collections::map_data<K,T>* _ptr;

	public:
		object_wrapper();
		object_wrapper(std::nullptr_t const&);
		object_wrapper(collections::map_data<K,T>*);
		object_wrapper(collections::ienum<collections::pair<K,T>> const* store);
		object_wrapper(std::initializer_list<collections::pair<K,T>>);
		object_wrapper(object_wrapper<collections::map_data<K,T>> &&);
		object_wrapper(object_wrapper<collections::map_data<K,T>> const&);
		~object_wrapper();

	public:
		void clean();
		bool is_empty()const;
		collections::map_data<K,T>* get(void)const;
		void set(collections::map_data<K,T>*);
		collections::map_data<K,T> ** addres_of(void);

	public:
		object_wrapper& operator = (object_wrapper<collections::map_data<K,T>> &&);
		object_wrapper& operator = (object_wrapper<collections::map_data<K,T>> const&);
		object_wrapper& operator = (std::nullptr_t const&);
		object_wrapper& operator = (collections::map_data<K,T>*);
		object_wrapper& operator = (collections::ienum<collections::pair<K, T>> const* store);

		object_wrapper& operator += (collections::pair<K, T>);

		inline operator object_t()const;

		object_wrapper_ptr<collections::map_data<K,T>> operator & (void);
		collections::map_data<K,T> * operator -> (void);
		collections::map_data<K,T> const* operator -> (void)const;
		explicit operator collections::map_data<K,T> * (void);
		explicit operator collections::map_data<K,T> const* (void)const;

		T& operator [] (K const&);
		T operator [] (K const&)const;
	};

	template<class K, class T, typename F>
	static void foreach(collections::map<K,T> const& store, F func) {
		if (!store.is_empty())for (auto it = store->begin(); it.is_valid(); ++it)
			func((collections::pair<K,T>&)*it);
	}
}

#include<ang/collections/map_specialization.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#include<ang/collections/inline/map.inl>

#endif//__ANG_MAP_H__
