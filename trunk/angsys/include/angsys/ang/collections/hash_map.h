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
				windex i = 0, c = value.size();

				for (char32_t n = text::to_char32<false, text::is_endian_swapped<E>::value>(key.cstr(), i);
					n != 0 && i < key.size();
					n = text::to_char32<false, text::is_endian_swapped<E>::value>(key.cstr(), i))
				{
					h = (h << 5) + h + dword(n) + 1;
				}
				return ang_uint64_t(h % TS);
			}
		};

		template<text::encoding E, template<typename>class A>
		struct hash_index_maker<strings::basic_string<E,A>> {
			static inline long64 make(const strings::basic_string<E, A>& key_, ulong64 TS) {
				ulong64 h = 75025;
				windex i = 0, c = value.size();
				auto key = key_->cstr();

				for (char32_t n = text::to_char32<false, text::is_endian_swapped<E>::value>(key.cstr(), i);
					n != 0 && i < key.size();
					n = text::to_char32<false, text::is_endian_swapped<E>::value>(key.cstr(), i))
				{
					h = (h << 5) + h + dword(n) + 1;
				}
				return ang_uint64_t(h % TS);
			}
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
			: public object
			, public imap<K, T>
		{
		public:
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
			allocator<node_t> alloc;
			wsize _count;
			node_array_t _table;

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
			inline T* data()const;
			inline wsize size()const;
			//inline void size(wsize);
			inline wsize capacity()const;
			inline void capacity(wsize size, bool save = false);

			inline void clear();
			inline void empty();
			inline bool move(hash_map_object<K, T, allocator, hash_index_maker>&);

			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

			template<typename U> inline void expand(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void expand(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void expand(stack_array<U, SIZE> const&);

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
			inline bool insert(K, T) override;
			inline bool insert(type) override;
			inline bool update(K, T) override;
			inline bool update(type) override;
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
				node_ptr_t node = alloc.allocate(1);
				alloc.template construct<node_t>(node, forward<U>(key), forward<V>(val));
				return node;
			}
			inline void increase_capacity();
			inline wsize hash_index(key_type const&)const;
			inline node_ptr_t find_node(key_type const&)const;
		};


		template<text::encoding E, template<typename>class A, typename T, template<typename> class allocator, template<typename> class hash_index_maker>
		class hash_map_object<strings::basic_string<E,A>, T, allocator, hash_index_maker> final
			: public object
			, public imap<strings::basic_string<E,A>,T>
		{
		public:
			typedef pair<strings::basic_string<E,A>, T>							type, pair_type;
			typedef T									value_type;
			typedef strings::basic_string<E,A>									key_type;
			typedef hash_map_object<strings::basic_string<E,A>, T, allocator, hash_index_maker>	self_type;
			typedef hash_index_maker<strings::basic_string<E,A>>					index_maker;
			typedef imap<strings::basic_string<E,A>, T>							imap_type;
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
			allocator<node_t> alloc;
			wsize _count;
			node_array_t _table;

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
			inline T* data()const;
			inline wsize size()const;
			//inline void size(wsize);
			inline wsize capacity()const;
			inline void capacity(wsize size, bool save = false);

			inline void clear();
			inline void empty();
			inline bool move(hash_map_object<strings::basic_string<E,A>, T, allocator, hash_index_maker>&);

			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

			template<typename U> inline void expand(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void expand(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void expand(stack_array<U, SIZE> const&);

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
			inline bool insert(raw_cstr_t, T) override;
			inline bool insert(type) override;
			inline bool update(raw_cstr_t, T) override;
			inline bool update(type) override;
			inline bool remove(raw_cstr_t) override;
			inline bool remove(raw_cstr_t, T&) override;
			inline bool remove(base_iterator<type> it) override;
			inline bool remove(base_iterator<type> it, T&) override;
			inline bool has_key(raw_cstr_t)const override;
			inline iterator<type> find(raw_cstr_t) override;
			inline const_iterator<type> find(raw_cstr_t)const override;

			template<typename U, text::encoding E2> inline bool insert(str_view<U, E2> str, T value) { return insert((raw_cstr_t)str, forward<T>(value)); }
			template<typename U, text::encoding E2> inline bool update(str_view<U, E2> str, T value) { return update((raw_cstr_t)str, forward<T>(value)); }
			template<typename U, text::encoding E2> inline bool remove(str_view<U, E2> str) { return remove((raw_cstr_t)str); }
			template<typename U, text::encoding E2> inline bool remove(str_view<U, E2> str, T& value) { return remove((raw_cstr_t)str, value); }
			template<typename U, text::encoding E2> inline bool has_key(str_view<U, E2> str)const { return has_key((raw_cstr_t)str); }
			template<typename U, text::encoding E2> inline iterator<type> find(str_view<U, E2> str) { return find((raw_cstr_t)str); }
			template<typename U, text::encoding E2> inline const_iterator<type> find(str_view<U, E2> str)const { return find((raw_cstr_t)str); }

			template<typename U, wsize N> inline bool insert(const U (&str)[N], T value) { return insert((raw_cstr_t)str, forward<T>(value)); }
			template<typename U, wsize N> inline bool update(const U (&str)[N], T value) { return update((raw_cstr_t)str, forward<T>(value)); }
			template<typename U, wsize N> inline bool remove(const U (&str)[N]) { return remove((raw_cstr_t)str); }
			template<typename U, wsize N> inline bool remove(const U (&str)[N], T& value) { return remove((raw_cstr_t)str, value); }
			template<typename U, wsize N> inline bool has_key(const U (&str)[N])const { return has_key((raw_cstr_t)str); }
			template<typename U, wsize N> inline iterator<type> find(const U (&str)[N]) { return find((raw_cstr_t)str); }
			template<typename U, wsize N> inline const_iterator<type> find(const U (&str)[N])const { return find((raw_cstr_t)str); }

			template<text::encoding E2, template<typename>class A2> inline bool insert(strings::basic_string<E2, A2>const& str, T value) { return insert((raw_cstr_t)str, forward<T>(value)); }
			template<text::encoding E2, template<typename>class A2> inline bool update(strings::basic_string<E2, A2>const& str, T value) { return update((raw_cstr_t)str, forward<T>(value)); }
			template<text::encoding E2, template<typename>class A2> inline bool remove(strings::basic_string<E2, A2>const& str) { return remove((raw_cstr_t)str); }
			template<text::encoding E2, template<typename>class A2> inline bool remove(strings::basic_string<E2, A2>const& str, T& value) { return remove((raw_cstr_t)str, value); }
			template<text::encoding E2, template<typename>class A2> inline bool has_key(strings::basic_string<E2, A2>const& str)const { return has_key((raw_cstr_t)str); }
			template<text::encoding E2, template<typename>class A2> inline iterator<type> find(strings::basic_string<E2, A2>const& str) { return find((raw_cstr_t)str); }
			template<text::encoding E2, template<typename>class A2> inline const_iterator<type> find(strings::basic_string<E2, A2>const& str)const { return find((raw_cstr_t)str); }

		public: //overrides
			ANG_DECLARE_INTERFACE();
			//inline comparision_result_t compare(const object*)const override;

		protected: //Memory Operations
			template<typename U, typename V> inline node_ptr_t allocate_node(U key, V val) {
				node_ptr_t node = alloc.allocate(1);
				alloc.template construct<node_t>(node, forward<U>(key), forward<V>(val));
				return node;
			}
			inline void increase_capacity();
			inline wsize hash_index(key_type const&)const;
			inline node_ptr_t find_node(key_type const&)const;
		};

	}//collections


}//ang


#ifdef  LINK
#undef  LINK
#endif//LINK

#include <ang/collections/inline/hash_map.inl>

#endif //__ANG_COLLECTIONS_HASH_MAP_H__