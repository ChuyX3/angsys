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
		ulong64 LINK create_hash_index(int, ulong64);
		ulong64 LINK create_hash_index(uint, ulong64);
		ulong64 LINK create_hash_index(long, ulong64);
		ulong64 LINK create_hash_index(ulong, ulong64);
		ulong64 LINK create_hash_index(long64, ulong64);
		ulong64 LINK create_hash_index(ulong64, ulong64);
		ulong64 LINK create_hash_index(float, ulong64);
		ulong64 LINK create_hash_index(double, ulong64);
		ulong64 LINK create_hash_index(pointer, ulong64);
		ulong64 LINK create_hash_index(raw_cstr_t, ulong64);

		template<typename K>
		struct hash_index_maker {
			static inline long64 make(const K& key, ulong64 SZ) {
				return create_hash_index(key, SZ);
			}
		};

		template<typename T, text::encoding E>
		struct hash_index_maker<str_view<T, E>> {
			static inline long64 make(const str_view<T, E>& key, ulong64 SZ) {
				return create_hash_index(raw_cstr(key), SZ);
			}
		};

		template<text::encoding E, template<typename>class A>
		struct hash_index_maker<strings::basic_string<E,A>> {
			static inline long64 make(const strings::basic_string<E, A>& key, ulong64 SZ) {
				return create_hash_index(raw_cstr(key->cstr()), SZ);
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
			inline comparision_result_t compare(const object*)const override;
	
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