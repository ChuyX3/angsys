#ifndef __ANG_COLLECTIONS_H__
#elif !defined __ANG_COLLECTIONS_HASH_MAP_H__
#define __ANG_COLLECTIONS_HASH_MAP_H__

namespace ang
{
	namespace collections
	{

		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> class hash_map_object;
		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> using hash_map = object_wrapper<hash_map_object<K, T, allocator>>;

		/******************************************************************/
		/* template class hash_map_object :                               */
		/*  -> implements a hash table object                             */
		/******************************************************************/
		template<typename K, typename T, template<typename> class allocator>
		class hash_map_object final
			: public object
			, public imap<K, T>
		{
		public:
			typedef pair<K, T>							type, pair_type;
			typedef T									value_type;
			typedef K									key_type;
			typedef hash_map_object<K, T, allocator>	self_type;
			typedef allocator<pair<K, T>>				allocator_t;
			typedef imap<K, T>							imap_type;
			typedef ienum<pair<K, T>>					ienum_type;
			typedef base_iterator<T>					base_iterator_t;
			typedef iterator<T>							iterator_t;
			typedef const_iterator<T>					const_iterator_t;
			typedef forward_iterator<T>					forward_iterator_t;
			typedef const_forward_iterator<T>			const_forward_iterator_t;
			typedef backward_iterator<T>				backward_iterator_t;
			typedef const_backward_iterator<T>			const_backward_iterator_t;

		protected:
			allocator_t alloc;
			wsize _count;
			scope_array<double_linked_node<type>*> _data;

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
			inline bool move(hash_map_object<T, allocator>&);

			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

			template<typename U> inline void expand(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void expand(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void expand(stack_array<U, SIZE> const&);

		public: //ibuffer overrides
			inline bool is_readonly()const override;
			inline text::encoding_t encoding()const override;
			inline pointer buffer_ptr() override;
			inline const_pointer buffer_ptr()const override;
			inline wsize buffer_size()const override;
			inline wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override;
			inline ibuffer_view_t map_buffer(windex, wsize) override;
			inline bool unmap_buffer(ibuffer_view_t&, wsize) override;
			inline bool can_realloc_buffer()const override;
			inline bool realloc_buffer(wsize) override;

		public: //ienum overrides
			inline wsize counter()const override;

			inline T& at(base_iterator_t const&) override;
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
			inline bool copy(const ienum<pair<K, T>>*) override;
			inline void extend(const ienum<pair<K, T>>*) override;
			inline bool insert(K, T) override;
			inline bool insert(pair<K, T>) override;
			inline bool update(K, T) override;
			inline bool update(pair<K, T>) override;
			inline bool remove(K const&) override;
			inline bool remove(K const&, T&) override;
			inline bool remove(base_iterator<pair<K, T>> it) override;
			inline bool remove(base_iterator<pair<K, T>> it, T&) override;
			inline bool has_key(const K&)const override;
			inline iterator<pair<K, T>> find(const K&) override;
			inline const_iterator<pair<K, T>> find(const K&)const override;

		public: //overrides
			ANG_DECLARE_INTERFACE();
			inline comparision_result_t compare(const object*)const override;

		protected: //Memory Operations
			inline bool realloc(wsize size, bool save = true);
		};

	}//collections


}//ang


#endif //__ANG_COLLECTIONS_HASH_MAP_H__