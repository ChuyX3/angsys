#ifndef __ANG_HASH_TABLE_HPP__
#define __ANG_HASH_TABLE_HPP__

#include <angsys.hpp>

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
		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> class hash_table_object;
		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> using hash_table = object_wrapper<hash_table_object<K, T, allocator>>;
	}

	namespace collections
	{

		template<typename K, typename T>
		struct hash_table_node
		{
			typedef K key_t;
			typedef T value_t;
			typedef collections::pair<K,T> pair_t;
			typedef hash_table_node<K, T> self_t, *self_ptr_t;

			//static inline type_name_t class_name();
			//static inline bool is_inherited_of(type_name_t);

			hash_table_node(K key) { pair.key = ang::move(key); }
			hash_table_node(K key, value_t val) { pair.key = ang::move(key); pair.value = ang::move(val); }
			hash_table_node(pair_t p) { pair = ang::move(p); }
			
			pair_t pair;
			self_ptr_t prev = null;
			self_ptr_t next = null;
		};

		template<typename K, typename T, template<typename> class allocator>
		class hash_table_object final
			: public object
			, public imap<K, T>
		{
		public:
			typedef T										value_t;
			typedef K										key_t;
			typedef pair<K, T>								pair_t;
			typedef hash_table_object<K, T, allocator>		self_t;
			typedef ienum<pair<K, T>>						ienum_t;
			typedef imap<K, T>								imap_t;
			typedef iterator<pair<K, T >>					iterator_t;
			typedef const_iterator<pair<K, T >>				const_iterator_t;
			typedef base_iterator<pair<K, T >>				base_iterator_t;
			typedef forward_iterator<pair<K, T >>			forward_iterator_t;
			typedef const_forward_iterator<pair<K, T >>		const_forward_iterator_t;
			typedef backward_iterator<pair<K, T >>			backward_iterator_t;
			typedef const_backward_iterator<pair<K, T >>	const_backward_iterator_t;

		protected:
			typedef hash_table_node<K, T> node_t;
			typedef hash_table_node<K, T> *node_ptr_t;
			long64 _size;
			long64 _capacity;
			node_ptr_t* _table;

		public:
			inline hash_table_object();
			inline hash_table_object(ang::initializer_list_t<pair<K, T>> list);
			inline hash_table_object(const ang::nullptr_t&);
			inline hash_table_object(hash_table_object&& ar);
			inline hash_table_object(const hash_table_object& ar);
			inline hash_table_object(const hash_table_object* ar);
			inline hash_table_object(const ienum_t* store);

		private:
			virtual ~hash_table_object();

		public: //methods
			inline bool is_empty()const;
			inline void clean();
			inline bool move(hash_table_object&);

		public: //ienum overrides
			inline wsize counter()const override;
			inline pair_t& at(const base_iterator_t& it) override;
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
			inline bool copy(const ienum_t*) override;
			inline void extend(const ienum_t*) override;
			inline bool insert(K,T) override;
			inline bool insert(pair_t) override;
			inline bool update(K, T) override;
			inline bool update(pair<K, T>) override;
			inline bool remove(K const&) override;
			inline bool remove(K const&,T&) override;
			inline bool remove(base_iterator<pair<K, T>> it) override;
			inline bool remove(base_iterator<pair<K, T>> it, T&) override;
			inline bool has_key(const K&)const override;
			inline iterator<pair<K, T>> find(const K&) override;
			inline const_iterator<pair<K, T>> find(const K&)const override;

		public: //overrides
			inline static type_name_t class_name();
			inline static bool is_inherited_of(type_name_t);
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;
			//inline comparision_result_t compare(const object&)const override {
			//	return comparision_result::diferent;
			//}

		protected: //Memory Operations
			void increase_capacity();
			inline wsize hash_index(key_t const&)const;
			inline node_ptr_t find_node(key_t const&)const;
		};



		template<text::encoding_enum ENCODING, typename T, template<typename> class allocator>
		class hash_table_object<strings::string_base<ENCODING>, T, allocator> final
			: public object
			, public imap<strings::string_base<ENCODING>, T>
		{
		public:
			typedef T																	value_t;
			typedef strings::string_base<ENCODING>										key_t;
			typedef pair<strings::string_base<ENCODING>, T>								pair_t;
			typedef hash_table_object<strings::string_base<ENCODING>, T, allocator>		self_t;
			typedef ienum<pair<strings::string_base<ENCODING>, T>>						ienum_t;
			typedef imap<strings::string_base<ENCODING>, T>								imap_t;
			typedef iterator<pair<strings::string_base<ENCODING>, T >>					iterator_t;
			typedef const_iterator<pair<strings::string_base<ENCODING>, T >>			const_iterator_t;
			typedef base_iterator<pair<strings::string_base<ENCODING>, T >>				base_iterator_t;
			typedef forward_iterator<pair<strings::string_base<ENCODING>, T >>			forward_iterator_t;
			typedef const_forward_iterator<pair<strings::string_base<ENCODING>, T >>	const_forward_iterator_t;
			typedef backward_iterator<pair<strings::string_base<ENCODING>, T >>			backward_iterator_t;
			typedef const_backward_iterator<pair<strings::string_base<ENCODING>, T >>	const_backward_iterator_t;

		protected:
			typedef hash_table_node<strings::string_base<ENCODING>, T> node_t;
			typedef hash_table_node<strings::string_base<ENCODING>, T> *node_ptr_t;
			long64 _size;
			long64 _capacity;
			node_ptr_t* _table;

		public:
			inline hash_table_object();
			inline hash_table_object(ang::initializer_list_t<pair<strings::string_base<ENCODING>, T>> list);
			inline hash_table_object(const ang::nullptr_t&);
			inline hash_table_object(hash_table_object&& ar);
			inline hash_table_object(const hash_table_object& ar);
			inline hash_table_object(const hash_table_object* ar);
			inline hash_table_object(const ienum_t* store);

		private:
			virtual ~hash_table_object();

		public: //methods
			inline bool is_empty()const;
			inline void clean();
			inline bool move(hash_table_object&);

		public: //ienum overrides
			inline wsize counter()const override;
			inline pair_t& at(const base_iterator_t& it) override;
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
			inline bool copy(const ienum_t*) override;
			inline void extend(const ienum_t*) override;
			inline bool insert(strings::string_base<ENCODING>, T) override;
			inline bool insert(pair_t) override;
			inline bool update(strings::string_base<ENCODING>, T) override;
			inline bool update(pair<strings::string_base<ENCODING>, T>) override;
			inline bool remove(base_iterator<pair<strings::string_base<ENCODING>, T>> it) override;
			inline bool remove(base_iterator<pair<strings::string_base<ENCODING>, T>> it, T&) override;

			template <typename K> inline bool has_key(safe_str<K> k)const { return has_key(raw_str(k)); }
			template <typename K> inline bool remove(safe_str<K> k) { return remove(raw_str(k)); }
			template <typename K> inline bool remove(safe_str<K> k, T& value) { return remove(raw_str(k), value); }
			template <typename K> inline iterator<pair<strings::string_base<ENCODING>, T>> find(safe_str<K> k) { return find(raw_str(k)); }
			template <typename K> inline const_iterator<pair<strings::string_base<ENCODING>, T>> find(safe_str<K> k)const { return find(raw_str(k)); }

			template <text::encoding_enum OTHER_ENCODING> inline bool has_key(strings::string_base<OTHER_ENCODING>const& k)const { return has_key(raw_str((typename strings::safe_str_by_encoding<OTHER_ENCODING>::ctype)k)); }
			template <text::encoding_enum OTHER_ENCODING> inline bool remove(strings::string_base<OTHER_ENCODING>const& k) { return remove(raw_str((typename strings::safe_str_by_encoding<OTHER_ENCODING>::ctype)k)); }
			template <text::encoding_enum OTHER_ENCODING> inline bool remove(strings::string_base<OTHER_ENCODING>const& k, T& value) { return remove(raw_str((typename strings::safe_str_by_encoding<OTHER_ENCODING>::ctype)k), value); }
			template <text::encoding_enum OTHER_ENCODING> inline iterator<pair<strings::string_base<ENCODING>, T>> find(strings::string_base<OTHER_ENCODING>const& k) { return find(raw_str((typename strings::safe_str_by_encoding<OTHER_ENCODING>::ctype)k)); }
			template <text::encoding_enum OTHER_ENCODING> inline const_iterator<pair<strings::string_base<ENCODING>, T>> find(strings::string_base<OTHER_ENCODING>const& k)const { return find(raw_str((typename strings::safe_str_by_encoding<OTHER_ENCODING>::ctype)k)); }

		protected: //overrides
			inline bool remove(raw_str_t) override;
			inline bool remove(raw_str_t, T&) override;
			inline bool has_key(raw_str_t)const override;
			inline iterator<pair<strings::string_base<ENCODING>, T>> find(raw_str_t) override;
			inline const_iterator<pair<strings::string_base<ENCODING>, T>> find(raw_str_t)const override;


		public: //overrides
			inline static type_name_t class_name();
			inline static bool is_inherited_of(type_name_t);
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;
			//inline comparision_result_t compare(const object&)const override {
			//	return comparision_result::diferent;
			//}

		protected: //Memory Operations
			void increase_capacity();
			inline wsize hash_index(key_t const&)const;
			inline node_ptr_t find_node(key_t const&)const;
		};



		wsize LINK hash_table_get_next_size(wsize);
		wsize LINK create_hash_index(int, wsize);
		wsize LINK create_hash_index(wsize, wsize);
		wsize LINK create_hash_index(long, wsize);
		wsize LINK create_hash_index(ulong, wsize);
		wsize LINK create_hash_index(long64, wsize);
		wsize LINK create_hash_index(ulong64, wsize);
		wsize LINK create_hash_index(float, wsize);
		wsize LINK create_hash_index(double, wsize);
		wsize LINK create_hash_index(pointer, wsize);
		wsize LINK create_hash_index(raw_str_t, wsize);
	}


	template<typename K, typename T, template<typename> class allocator>
	class object_wrapper<collections::hash_table_object<K,T, allocator>>
	{
	public:
		typedef collections::hash_table_object<K,T, allocator> type;

	private:
		collections::hash_table_object<K,T, allocator>* _ptr;

	public:
		object_wrapper();
		object_wrapper(ang::nullptr_t const&);
		object_wrapper(collections::hash_table_object<K,T, allocator>*);
		object_wrapper(collections::ienum<collections::pair<K,T>> const* store);
		object_wrapper(ang::initializer_list_t<collections::pair<K,T>>);
		object_wrapper(object_wrapper<collections::hash_table_object<K,T, allocator>> &&);
		object_wrapper(object_wrapper<collections::hash_table_object<K,T, allocator>> const&);
		~object_wrapper();

	public:
		void clean();
		bool is_empty()const;
		collections::hash_table_object<K,T, allocator>* get(void)const;
		void set(collections::hash_table_object<K,T, allocator>*);
		collections::hash_table_object<K,T, allocator> ** addres_of(void);

		collections::forward_iterator<object_wrapper<T>> begin() { return _ptr ? _ptr->begin() : collections::forward_iterator<T>(); }
		collections::forward_iterator<object_wrapper<T>> end() { return _ptr ? _ptr->end() : collections::forward_iterator<T>(); }
		collections::forward_iterator<const object_wrapper<T>> begin()const { return _ptr ? ((type const*)_ptr)->begin() : collections::forward_iterator<const object_wrapper<T>>(); }
		collections::forward_iterator<const object_wrapper<T>> end()const { return _ptr ? ((type const*)_ptr)->end() : collections::forward_iterator<const object_wrapper<T>>(); }

	public:
		object_wrapper& operator = (object_wrapper<collections::hash_table_object<K,T, allocator>> &&);
		object_wrapper& operator = (object_wrapper<collections::hash_table_object<K,T, allocator>> const&);
		object_wrapper& operator = (ang::nullptr_t const&);
		object_wrapper& operator = (collections::hash_table_object<K,T, allocator>*);
		object_wrapper& operator = (collections::ienum<collections::pair<K, T>> const* store);

		object_wrapper& operator += (collections::pair<K, T>);

		inline operator objptr()const;

		object_wrapper_ptr<collections::hash_table_object<K,T, allocator>> operator & (void);
		collections::hash_table_object<K,T, allocator> * operator -> (void);
		collections::hash_table_object<K,T, allocator> const* operator -> (void)const;
		explicit operator collections::hash_table_object<K,T, allocator> * (void);
		explicit operator collections::hash_table_object<K,T, allocator> const* (void)const;

		T& operator [] (K const&);
		T operator [] (K const&)const;
	};

}

//#include<ang/collections/hash_table_specialization.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#include<ang/collections/inline/hash_table.inl>

#endif//__ANG_HASH_TABLE_H__
