#ifndef __COFFE_COLLECTIONS_HASH_MAP_H__
#define __COFFE_COLLECTIONS_HASH_MAP_H__

#include <coffe.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined COFFE_DYNAMIC_LIBRARY

#ifdef COFFE_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//COFFE_EXPORTS
#else//#elif defined COFFE_STATIC_LIBRARY
#define LINK
#endif//COFFE_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

namespace coffe
{
	namespace collections
	{
		template<typename K, typename T, template<typename>class A = memory::default_allocator, template<typename>class H = algorithms::hash> class hash_map_object;
		template<typename K, typename T, template<typename>class A = memory::default_allocator, template<typename>class H = algorithms::hash> using smart_hash_map = object_wrapper<hash_map_object<K, T, A, H>>;
	}

	template<typename K, typename T>
	struct smart_ptr_type<collections::pair<K,T>[], false, false> {
		static constexpr coffe::smart_type smart_type = coffe::smart_type::none;
		using smart_ptr_t = collections::hash_map<typename remove_const<K>::type,T>;
		using type = collections::hash_map_object<typename remove_const<K>::type, T>;
	};

	template<typename K, typename T, wsize N>
	struct smart_ptr_type<collections::pair<K, T>[N], false, false> {
		static constexpr coffe::smart_type smart_type = coffe::smart_type::none;
		using smart_ptr_t = collections::hash_map<typename remove_const<K>::type, T>;
		using type = collections::hash_map_object<typename remove_const<K>::type, T>;
	};

	template<typename K, typename T, template<typename>class A, template<typename>class H>
	struct smart_ptr_type<collections::hash_map<K, T, A, H>, false, false> {
		static constexpr coffe::smart_type smart_type = coffe::smart_type::none;
		using smart_ptr_t = collections::hash_map<typename remove_const<K>::type, T, A, H>;
		using type = collections::hash_map_object<typename remove_const<K>::type, T, A, H>;
	};

	template<typename K, typename T, template<typename>class A, template<typename>class H>
	struct smart_ptr_type<collections::hash_map<K, object_wrapper<T>, A, H>, false, false> {
		static constexpr coffe::smart_type smart_type = coffe::smart_type::none;
		using smart_ptr_t = collections::hash_map<typename remove_const<K>::type, T, A, H>;
		using type = collections::hash_map_object<typename remove_const<K>::type, T, A, H>;
	};

	template<typename K, typename T, template<typename>class A, template<typename>class H>
	struct smart_ptr_type<collections::hash_map<K, intf_wrapper<T>, A, H>, false, false> {
		static constexpr coffe::smart_type smart_type = coffe::smart_type::none;
		using smart_ptr_t = collections::hash_map<typename remove_const<K>::type, T, A, H>;
		using type = collections::hash_map_object<typename remove_const<K>::type, T, A, H>;
	};

	namespace collections
	{
		/******************************************************************/
		/* template class hash_map_object :                               */
		/*  -> implements a hash table object                             */
		/******************************************************************/
		template<typename K, typename T, template<typename> class A, template<typename> class H>
		class hash_map_object final
			: public implement<hash_map_object<K, T, A>
			, iid("coffe::collections::hash_map")
			, imap<K, T>
			, ienumerable<typename imap<K, T>::pair_view_type>
			, ienum<typename imap<K, T>::pair_view_type>>
			, public hash_map<typename auto_type<K>::type, typename auto_type<T>::type>
		{
		public:
			//using base = implement<hash_map_object<K, T, A, H>, , imap<K, T>>;
			using pair_type = typename imap<K, T>::pair_type;
			using pair_view_type = typename imap<K, T>::pair_view_type;
			using value_type = typename auto_type<T>::type;
			using key_type = typename auto_type<K>::type;
			using self_type = hash_map_object<K, T, A, H>;
			using index_maker = H<key_type>;
			using imap_type = imap<K, T>;
			using ienum_type = ienum<pair_view_type>;
			using ienum_ptr_type = ienum_ptr<pair_view_type>;
			using base_iterator_t = collections::base_iterator<pair_view_type>;
			using iterator_t = collections::iterator<pair_view_type>;
			using const_iterator_t = collections::const_iterator<pair_view_type>;
			using forward_iterator_t = collections::forward_iterator<pair_view_type>;
			using const_forward_iterator_t = collections::const_forward_iterator<pair_view_type>;
			using backward_iterator_t = collections::backward_iterator<pair_view_type>;
			using const_backward_iterator_t = collections::const_backward_iterator<pair_view_type>;
			using fast_hash_map_t = hash_map<typename auto_type<K>::type, typename auto_type<T>::type>;

		private:
			using node_type = doubly_linked_node<pair_type>;
			using node_type_ptr = value<doubly_linked_node<pair_type>*>; //for auto initializing to null
			using node_type_ptr_ptr = value<doubly_linked_node<pair_type>**>; //for auto initializing to null

		public:
			hash_map_object();
			hash_map_object(wsize reserve);
			hash_map_object(const coffe::nullptr_t&);
			hash_map_object(hash_map_object<K, T, A, H>&& ar);
			hash_map_object(const hash_map_object<K, T, A, H>& ar);
			hash_map_object(fast_hash_map_t&& ar);
			hash_map_object(const fast_hash_map_t& ar);
			hash_map_object(const collections::ienum_ptr<pair_view_type>& store);
			template<typename U, typename V>inline hash_map_object(coffe::array_view<pair<U, V>> list);
			template<typename U, typename V>inline hash_map_object(coffe::initializer_list<pair<U, V>> list);

		private:
			virtual ~hash_map_object();

		public: //methods
			template<typename U, typename V> inline void copy(array_view<pair<U, V>>const&);
			template<typename U, typename V> inline void extend(array_view<pair<U, V>>const&);

		public: //ienum overrides
			wsize counter()const override;
			typename auto_type<pair_view_type>::return_type at(base_iterator_t const&) override;
			bool increase(base_iterator_t&)const override;
			bool increase(base_iterator_t&, int offset)const override;
			bool decrease(base_iterator_t&)const override;
			bool decrease(base_iterator_t&, int offset)const override;
			forward_iterator_t begin() override;
			forward_iterator_t end() override;
			const_forward_iterator_t begin()const override;
			const_forward_iterator_t end()const override;
			forward_iterator_t last() override;
			const_forward_iterator_t last()const override;
			backward_iterator_t rbegin() override;
			backward_iterator_t rend() override;
			const_backward_iterator_t rbegin()const override;
			const_backward_iterator_t rend()const override;

			iterator_t find(function<bool(typename auto_type<pair_view_type>::arg_type const&)>, bool invert = false)const override;
			iterator_t find(function<bool(typename auto_type<pair_view_type>::arg_type const&)>, base_iterator_t next_to, bool invert = false)const override;
			ienum_ptr_type find_all(function<bool(typename auto_type<pair_view_type>::arg_type const&)>)const override;

		public: //imap overrides
			ienum_ptr_type enumerate(iteration_method_t = iteration_method::def)const override;
			void copy(const ienum_ptr_type&) override;
			void extend(const ienum_ptr_type&) override;
			iterator_t insert(typename auto_type<K>::arg_type const&, typename auto_type<T>::arg_type &&) override;
			iterator_t insert(typename auto_type<K>::arg_type const&, typename auto_type<T>::arg_type const&) override;
			iterator_t insert(pair_type &&) override;
			iterator_t insert(pair_type const&) override;
			iterator_t update(typename auto_type<K>::arg_type const&, typename auto_type<T>::arg_type &&) override;
			iterator_t update(typename auto_type<K>::arg_type const&, typename auto_type<T>::arg_type const&) override;
			iterator_t update(pair_type&&) override;
			iterator_t update(pair_type const&) override;
			bool remove(typename auto_type<K>::arg_type const&, typename auto_type<T>::arg_ptr_type = null) override;
			bool remove(base_iterator_t it, typename auto_type<T>::arg_ptr_type = null) override;
			bool has_key(typename auto_type<K>::arg_type const&)const override;
			iterator_t find(typename auto_type<K>::arg_type const&) override;
			const_iterator_t find(typename auto_type<K>::arg_type const&)const override;
		};


		
	}//collections

	/********************************************************************/
	/* template class coffe::object_wrapper<hash_map_object> :          */
	/*  -> specialization of object_wrapper<hash_map_object> -> has_map */
	/********************************************************************/
	template<typename K, typename T, template<typename> class A, template<typename> class H>
	class object_wrapper<collections::hash_map_object<K, T, A, H>>
	{
	public:
		typedef collections::hash_map_object<K, T, A, H> type;
		typedef typename collections::hash_map_object<K, T, A, H>::key_type key_type;
		typedef typename collections::hash_map_object<K, T, A, H>::value_type value_type;
		typedef typename collections::hash_map_object<K, T, A, H>::pair_type pair_type;
		typedef typename collections::hash_map_object<K, T, A, H>::pair_view_type pair_view_type;

	private:
		collections::hash_map_object<K, T, A, H>* m_ptr;

	public:
		object_wrapper();
		object_wrapper(type* map);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(const coffe::nullptr_t&);
		object_wrapper(typename type::fast_hash_map_t && map);
		object_wrapper(typename type::fast_hash_map_t const& map);
		object_wrapper(const collections::ienum_ptr<pair_view_type>& store);
		template<typename U, typename V>inline object_wrapper(coffe::array_view<collections::pair<U, V>> items);
		template<typename U, typename V>inline object_wrapper(coffe::initializer_list<collections::pair<U, V>> list);

		~object_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type** addres_of(void);

		collections::forward_iterator<pair_view_type> begin() { return m_ptr ? m_ptr->begin() : collections::iterator<pair_view_type>(); }
		collections::forward_iterator<pair_view_type> end() { return m_ptr ? m_ptr->end() : collections::iterator<pair_view_type>(); }
		collections::forward_iterator<const pair_view_type> begin()const { return m_ptr ? ((const type*)m_ptr)->begin() : collections::iterator<const pair_view_type>(); }
		collections::forward_iterator<const pair_view_type> end()const { return m_ptr ? ((const type*)m_ptr)->end() : collections::iterator<const pair_view_type>(); }

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (const coffe::nullptr_t&);

		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<typename U, typename V>
		object_wrapper& operator = (collections::ienum_ptr<collections::pair<U, V>> const& items) {
			if (is_empty())
				set(new type());
			get()->copy(items);
			return *this;
		}
		object_wrapper& operator += (pair_type const&);
		object_wrapper& operator -= (key_type const&);
		object_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		explicit operator type * (void);
		explicit operator type const* (void)const;
		
		value_type& operator[](key_type const& key);
		value_type const& operator[](key_type const& key)const;
	};


}//coffe


#ifdef  LINK
#undef  LINK
#endif//LINK

#include <coffe/collections/inline/hash.inl>

#endif //__COFFE_COLLECTIONS_HASH_MAP_H__
