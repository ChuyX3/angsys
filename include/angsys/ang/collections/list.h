#ifndef __ANG_LIST_H__
#define __ANG_LIST_H__

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
		template<typename T> class list_data;
		template<typename T> struct list_node;
		template<typename T> using list_node_t = list_node<T>*;
		template<typename T> using list = object_wrapper<list_data<T>>;

	}

	namespace collections
	{
		template<typename T>
		struct list_node
		{
			list_node* _prev;
			T _value;
			list_node* _next;

			list_node() : _prev(null), _value(), _next(null){ }
			list_node(T v) : _prev(null), _value(ang::move(v)), _next(null) { }

			list_node* prev()const { return _prev; }
			T const& value()const { return _value; }
			T& value() { return _value; }
			list_node* next()const { return _next; }

			void prev(list_node* val) { _prev = val; }
			void value(T val) { _value = ang::move(val); }
			void next(list_node* val) { _next = val; }
		};

		template<typename T>
		class list_data final
			: public object
			, public ilist<T>
			, public memory::iallocator_client
		{
		public:
			typedef T							type;
			typedef list_data<T>				self_t;
			typedef icollection<T>		icollection_t;
			typedef ienum<T>				ienum_t;
			typedef ilist<T>				ilist_t;
			typedef iterator<T>			iterator_t;
			typedef list_node<T>		node_t;
			typedef list_node_t<T>		node_ptr_t;

		protected:
			uint_t _count;
			node_ptr_t _first;
			node_ptr_t _last;
			memory::iallocator* allocator;

		public:
			inline list_data();
			inline list_data(std::initializer_list<T> list);
			inline list_data(const std::nullptr_t&);
			inline list_data(list_data&& ar);
			inline list_data(const list_data& ar);
			inline list_data(const list_data* ar);
			inline list_data(const ienum_t* store);
			inline explicit list_data(uint_t size, T const* ar = null);

		private:
			virtual ~list_data();

		public: //methods
			inline bool_t is_empty()const;
			inline void clean();

			inline bool_t move(list_data&);
			inline bool_t copy(uint_t size, T const* ar);
			template<class U> inline bool_t copy_from(const collections::ienum<U>* items) {
				if (!items) return false;
				if (items->counter() > counter())
					capacity(items->counter(), false);		
				for (auto it = items->begin(); it.is_valid(); ++it)
					append(*it);
				return true;
			}

		public: //iallocator_client overrides
			inline void set_allocator(memory::iallocator*)override;
			inline memory::iallocator* get_allocator()const override;

		public: //ienum overrides
			inline T& at(const iterator_t& it)const override;
			inline iterator_t begin()const override;
			inline iterator_t end()const override;
			inline bool_t next(iterator_t& it)const override;
			inline bool_t next(iterator_t& it, int_t offset)const override;
			inline bool_t prev(iterator_t& it)const override;
			inline bool_t prev(iterator_t& it, int_t offset)const override;
			inline uint_t counter()const override;

		public: //icollection overrides
			inline bool_t has_items()const override;
			inline bool_t copy(const ienum_t*) override;
			inline index_t index_of(iterator_t it)const override;
			inline iterator_t find_index(index_t idx)const override;
			inline iterator_t find(const T&, bool_t invert = false)const override;
			inline iterator_t find(const T&, iterator_t nextTo, bool_t invert = false)const override;

		public: //ilist overrides
			inline void extend(const ienum_t*) override;
			inline void append(T, bool_t last = true) override;
			inline bool_t insert(index_t idx, T) override;
			inline bool_t insert(iterator_t it, T) override;
			inline bool_t pop(bool_t last = true) override;
			inline bool_t pop(T&, bool_t last = true) override;
			inline bool_t pop_at(index_t idx) override;
			inline bool_t pop_at(iterator_t it) override;
			inline bool_t pop_at(index_t idx, T&) override;
			inline bool_t pop_at(iterator_t it, T&) override;

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool_t is_kind_of(type_name_t)const override;
			inline bool_t query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool_t operator == (const list_data& ar);
			inline bool_t operator != (const list_data& ar);
			inline explicit operator T*()const;

		protected: //Memory Operations
			inline bool_t realloc(memory::iallocator* alloc);
		};

		template<typename T>
		class list_data<object_wrapper<T>> final
			: public object
			, public ilist<object_wrapper<T>>
			, public memory::iallocator_client
		{
		public:
			typedef object_wrapper<T>					type;
			typedef list_data<object_wrapper<T>>		self_t;
			typedef icollection<object_wrapper<T>>		icollection_t;
			typedef ienum<object_wrapper<T>>			ienum_t;
			typedef ilist<object_wrapper<T>>			ilist_t;
			typedef iterator<object_wrapper<T>>			iterator_t;

		protected:
			uint_t _count;
			list_node_t<T> first;
			list_node_t<T> last;
			memory::iallocator* allocator;

		public:
			inline list_data();
			inline list_data(std::initializer_list<object_wrapper<T>> list);
			inline list_data(const std::nullptr_t&);
			inline list_data(list_data&& ar);
			inline list_data(const list_data& ar);
			inline list_data(const list_data* ar);
			inline list_data(const ienum_t* store);

		private:
			virtual ~list_data();

		public: //methods
			inline bool_t is_empty()const;
			inline void clean();

			inline bool_t move(list_data&);
			template<class U> inline bool_t copy_from(const collections::ienum<U>* items) {
				if (!items) return false;
				if (items->counter() > counter())
					capacity(items->counter(), false);
				for (auto it = items->begin(); it.is_valid(); ++it)
					append(*it);
				return true;
			}

		public: //iallocator_client overrides
			inline void set_allocator(memory::iallocator*)override;
			inline memory::iallocator* get_allocator()const override;

		public: //ienum overrides
			inline object_wrapper<T>& at(const iterator_t& it)const override;
			inline iterator_t begin()const override;
			inline iterator_t end()const override;
			inline bool_t next(iterator_t& it)const override;
			inline bool_t next(iterator_t& it, int_t offset)const override;
			inline bool_t prev(iterator_t& it)const override;
			inline bool_t prev(iterator_t& it, int_t offset)const override;
			inline uint_t counter()const override;

		public: //icollection overrides
			inline bool_t has_items()const override;
			inline bool_t copy(const ienum_t*) override;
			inline index_t index_of(iterator_t it)const override;
			inline iterator_t find_index(index_t idx)const override;
			inline iterator_t find(const object_wrapper<T>&, bool_t invert = false)const override;
			inline iterator_t find(const object_wrapper<T>&, iterator_t nextTo, bool_t invert = false)const override;

		public: //ilist overrides
			inline void extend(const ienum_t*) override;
			inline void append(object_wrapper<T>, bool_t last = true) override;
			inline bool_t insert(index_t idx, object_wrapper<T>) override;
			inline bool_t insert(iterator_t it, object_wrapper<T>) override;
			inline bool_t pop(bool_t last = true) override;
			inline bool_t pop(object_wrapper<T>&, bool_t last = true) override;
			inline bool_t pop_at(index_t idx) override;
			inline bool_t pop_at(iterator_t it) override;
			inline bool_t pop_at(index_t idx, object_wrapper<T>&) override;
			inline bool_t pop_at(iterator_t it, object_wrapper<T>&) override;

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool_t is_kind_of(type_name_t)const override;
			inline bool_t query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool_t operator == (const list_data& ar);
			inline bool_t operator != (const list_data& ar);
			inline explicit operator T**()const;

		protected: //Memory Operations
			inline bool_t realloc(memory::iallocator* alloc);
		};


		template<typename T>
		class list_data<intf_wrapper<T>> final
			: public object
			, public ibuffer
			, public ilist<intf_wrapper<T>>
			, public memory::iallocator_client
		{
		public:
			typedef intf_wrapper<T>					type;
			typedef list_data<intf_wrapper<T>>		self_t;
			typedef icollection<intf_wrapper<T>>		icollection_t;
			typedef ienum<intf_wrapper<T>>			ienum_t;
			typedef ilist<intf_wrapper<T>>			ilist_t;
			typedef iterator<intf_wrapper<T>>			iterator_t;

		protected:
			uint_t _count;
			list_node_t<T> first;
			list_node_t<T> last;
			memory::iallocator* allocator;

		public:
			inline list_data();
			inline list_data(std::initializer_list<intf_wrapper<T>> list);
			inline list_data(const std::nullptr_t&);
			inline list_data(list_data&& ar);
			inline list_data(const list_data& ar);
			inline list_data(const list_data* ar);
			inline list_data(const ienum_t* store);

		private:
			virtual ~list_data();

		public: //methods
			inline bool_t is_empty()const;
			inline void clean();

			inline bool_t move(list_data&);
			template<class U> inline bool_t copy_from(const collections::ienum<U>* items) {
				if (!items) return false;
				if (items->counter() > counter())
					capacity(items->counter(), false);
				for (auto it = items->begin(); it.is_valid(); ++it)
					append(*it);
				return true;
			}

		public: //iallocator_client overrides
			inline void set_allocator(memory::iallocator*)override;
			inline memory::iallocator* get_allocator()const override;

		public: //ienum overrides
			inline intf_wrapper<T>& at(const iterator_t& it)const override;
			inline iterator_t begin()const override;
			inline iterator_t end()const override;
			inline bool_t next(iterator_t& it)const override;
			inline bool_t next(iterator_t& it, int_t offset)const override;
			inline bool_t prev(iterator_t& it)const override;
			inline bool_t prev(iterator_t& it, int_t offset)const override;
			inline uint_t counter()const override;

		public: //icollection overrides
			inline bool_t has_items()const override;
			inline bool_t copy(const ienum_t*) override;
			inline index_t index_of(iterator_t it)const override;
			inline iterator_t find_index(index_t idx)const override;
			inline iterator_t find(const intf_wrapper<T>&, bool_t invert = false)const override;
			inline iterator_t find(const intf_wrapper<T>&, iterator_t nextTo, bool_t invert = false)const override;

		public: //ilist overrides
			inline void extend(const ienum_t*) override;
			inline void append(intf_wrapper<T>, bool_t last = true) override;
			inline bool_t insert(index_t idx, intf_wrapper<T>) override;
			inline bool_t insert(iterator_t it, intf_wrapper<T>) override;
			inline bool_t pop(bool_t last = true) override;
			inline bool_t pop(intf_wrapper<T>&, bool_t last = true) override;
			inline bool_t pop_at(index_t idx) override;
			inline bool_t pop_at(iterator_t it) override;
			inline bool_t pop_at(index_t idx, intf_wrapper<T>&) override;
			inline bool_t pop_at(iterator_t it, intf_wrapper<T>&) override;

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool_t is_kind_of(type_name_t)const override;
			inline bool_t query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool_t operator == (const list_data& ar);
			inline bool_t operator != (const list_data& ar);
			inline explicit operator T**()const;

		protected: //Memory Operations
			inline bool_t realloc(memory::iallocator* alloc);
		};


		template<>
		class list_data<void> final {};
	}

	template<typename T>
	class object_wrapper<collections::list_data<T>>
	{
	public:
		typedef collections::list_data<T> type;
		typedef typename collections::list_data<T>::type data_type;

	private:
		collections::list_data<T>* _ptr;

	public:
		object_wrapper();
		object_wrapper(collections::list_data<T>*);
		object_wrapper(std::initializer_list<data_type> list);
		object_wrapper(const collections::ienum<data_type>* store);
		explicit object_wrapper(uint_t size, data_type const* ar = null);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool_t is_empty()const;
		collections::list_data<T>* get(void)const;
		void set(collections::list_data<T>*);
		collections::list_data<T>** addres_of(void);

	public:
		object_wrapper& operator = (collections::list_data<T>*);
		object_wrapper& operator = (const std::nullptr_t&);
		object_wrapper& operator = (collections::ienum<data_type> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (T);

		object_wrapper_ptr<collections::list_data<T>> operator & (void);
		collections::list_data<T> * operator -> (void);
		collections::list_data<T> const* operator -> (void)const;
		explicit operator collections::list_data<T> * (void);
		explicit operator collections::list_data<T> const* (void)const;

		data_type const& operator[](index_t index)const;
		data_type & operator[](index_t index);

		friend safe_wrapper;
	};


	template<typename T>
	class object_wrapper<collections::list_data<object_wrapper<T>>>
	{
	public:
		typedef collections::list_data<object_wrapper<T>> type;
		typedef typename collections::list_data<object_wrapper<T>>::type data_type;

	private:
		collections::list_data<object_wrapper<T>>* _ptr;

	public:
		object_wrapper();
		object_wrapper(collections::list_data<object_wrapper<T>>*);
		object_wrapper(std::initializer_list<object_wrapper<T>> list);
		object_wrapper(const collections::ienum<object_wrapper<T>>* store);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool_t is_empty()const;
		collections::list_data<object_wrapper<T>>* get(void)const;
		void set(collections::list_data<object_wrapper<T>>*);
		collections::list_data<object_wrapper<T>>** addres_of(void);

	public:
		object_wrapper& operator = (collections::list_data<object_wrapper<T>>*);
		object_wrapper& operator = (const std::nullptr_t&);
		object_wrapper& operator = (collections::ienum<object_wrapper<T>> const* items);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (object_wrapper<T>);

		object_wrapper_ptr<collections::list_data<object_wrapper<T>>> operator & (void);
		collections::list_data<object_wrapper<T>> * operator -> (void);
		collections::list_data<object_wrapper<T>> const* operator -> (void)const;
		explicit operator collections::list_data<object_wrapper<T>> * (void);
		explicit operator collections::list_data<object_wrapper<T>> const* (void)const;

		object_wrapper<T> const& operator[](index_t index)const;
		object_wrapper<T> & operator[](index_t index);

		friend safe_wrapper;
	};

	template<class T, typename F>
	static void foreach(collections::list<T> const& store, F func) {
		for (collections::iterator<T> it = store->begin(); it.is_valid(); ++it)
			func((T&)*it);
	}

}

//#include<ang/collections/vector_specialization.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#include<ang/collections/inline/list.hpp>

#endif//__ANG_LIST_H__
