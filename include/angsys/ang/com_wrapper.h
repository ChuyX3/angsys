#ifndef __COM_WRAPPER_H__
#define __COM_WRAPPER_H__

#include<angsys.h>
#include <ang/collections/vector.h>
#include <ang/core/async.h>

namespace ang
{
	template<typename T> class com_wrapper_ptr;

	template<typename T>
	class com_wrapper
	{
	public:
		typedef T type;

	private:
		type* _ptr;

	public:
		com_wrapper() : _ptr(null) {}
		com_wrapper(type* ptr) : _ptr(null) { set(ptr); }
		com_wrapper(com_wrapper && other) {
			T * temp = other._ptr;
			other._ptr = null;
			_ptr = temp;
		}
		com_wrapper(com_wrapper const& other) : _ptr(null) {
			set(other._ptr);
		}
		com_wrapper(std::nullptr_t const&) : _ptr(null) {}
		~com_wrapper() { clean(); }

	public:
		void clean() {
			if (_ptr)_ptr->Release();
			_ptr = null;
		}
		bool is_empty()const { return _ptr == null; }
		type* get(void)const { return _ptr; }
		void set(type* ptr) {
			T * temp = _ptr;
			if (ptr == _ptr) return;
			_ptr = ptr;
			if (_ptr)_ptr->AddRef();
			if (temp)temp->Release();
		}
		type ** addres_of(void) { return &_ptr; }

	public:
		com_wrapper& operator = (type* ptr) {
			set(ptr);
			return*this;
		}
		com_wrapper& operator = (com_wrapper && other) {
			T * temp = other._ptr;
			other._ptr = null;
			_ptr = temp;
			return*this;
		}
		com_wrapper& operator = (com_wrapper const& other) {
			set(other._ptr);
			return*this;
		}

		com_wrapper_ptr<T> operator & (void) { return this; }
		type * operator -> (void) { return get(); }
		type const* operator -> (void)const { return get(); }
		operator type * (void) { return get(); }
		operator type const* (void)const { return get(); }
	};

	template<typename T>
	class com_wrapper_ptr
	{
		com_wrapper<T>* _ptr;
	public:
		com_wrapper_ptr(std::nullptr_t const&) : _ptr(null) {}
		com_wrapper_ptr(com_wrapper<T>*ptr) : _ptr(ptr) {}
		com_wrapper_ptr(com_wrapper_ptr && ptr) : _ptr(ptr._ptr) { ptr._ptr = null; }
		com_wrapper_ptr(com_wrapper_ptr const& ptr) : _ptr(ptr._ptr) {}
		~com_wrapper_ptr() { _ptr = null; }
		bool_t is_empty()const { return _ptr == null; }
		com_wrapper<T>& operator *()const { return *_ptr; }
		com_wrapper<T>* operator ->()const { return _ptr; }
		operator com_wrapper<T>*()const { return _ptr; }
		operator T**()const { return _ptr->addres_of(); }
		explicit operator void**()const { return (void**)_ptr->addres_of(); }
	};


	template<typename T>
	class safe_thread_wrapper
	{
	public:
		typedef T type;

	private:
		type* _ptr;
		core::async::scope_locker _locker;

	public:
		safe_thread_wrapper(type* ptr, core::async::mutex_t mutex) : _ptr(null), _locker(mutex){ set(ptr); }
		~safe_thread_wrapper() { clean(); }

	public:
		void clean() {
			if (_ptr)_ptr->Release();
			_ptr = null;
		}
		void set(type* ptr) {
			T * temp = _ptr;
			if (ptr == _ptr) return;
			_ptr = ptr;
			if (_ptr)_ptr->AddRef();
			if (temp)temp->Release();
		}
		type* get(void)const { return _ptr; }

	public:
		type * operator -> (void) { return get(); }
		type const* operator -> (void)const { return get(); }
		operator type * (void) { return get(); }
		operator type const* (void)const { return get(); }
	};



	namespace runtime
	{
		template<typename T>
		struct runtime_type_builder<com_wrapper<T>>
		{
			typedef typename com_wrapper<T>::type type;
			static inline type_name_t type_name() {
				return runtime::type_name<type>();
			}
		};

	}

	namespace collections
	{
		/******************************************************************/
		/* template class vector_buffer<com_wrapper> :                 */
		/*  -> partial specialization of vector_buffer<com_wrapper>    */
		/******************************************************************/
		template<typename T>
		class vector_buffer<com_wrapper<T>> final
			: public object
			, public ibuffer
			, public ilist<com_wrapper<T>>
			, public memory::iallocator_client
		{
		public:
			typedef com_wrapper<T>					type;
			typedef vector_buffer<com_wrapper<T>>		self_t;
			typedef icollection<com_wrapper<T>>		icollection_t;
			typedef ienum<com_wrapper<T>>			ienum_t;
			typedef ilist<com_wrapper<T>>			ilist_t;
			typedef iterator<com_wrapper<T>>			iterator_t;

		protected:
			uint _size;
			uint _capacity;
			type* _data;
			memory::iallocator* allocator;

		public:
			inline vector_buffer();
			inline vector_buffer(uint);
			inline vector_buffer(ang::initializer_list_t<com_wrapper<T>> list);
			inline vector_buffer(const ang::nullptr_t&);
			inline vector_buffer(vector_buffer&& ar);
			inline vector_buffer(const vector_buffer& ar);
			inline vector_buffer(const vector_buffer* ar);
			inline vector_buffer(const ienum_t* store);

		private:
			virtual ~vector_buffer();

		public: //methods
			inline bool is_empty()const;
			inline type* data()const;
			inline uint size()const;
			inline uint capacity()const;
			inline void capacity(uint size, bool save = false);

			inline void clean();
			inline void empty();

			inline bool move(vector_buffer&);
			template<class U> inline bool copy_from(const collections::ienum<U>* items) {
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

		public: //ibuffer overrides
			inline pointer buffer_ptr()const override;
			inline wsize buffer_size()const override;
			inline wsize mem_copy(wsize, pointer) override;
			inline pointer map_buffer(windex, wsize) override;
			inline void unmap_buffer(pointer, wsize) override;
			inline bool can_realloc_buffer()const override;
			inline bool realloc_buffer(wsize) override;

		public: //ienum overrides
			inline com_wrapper<T>& at(const iterator_t& it)const override;
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
			inline index index_of(iterator_t it)const override;
			inline iterator_t find_index(index idx)const override;
			inline iterator_t find(const com_wrapper<T>&, bool invert = false)const override;
			inline iterator_t find(const com_wrapper<T>&, iterator_t nextTo, bool invert = false)const override;

		public: //ilist overrides
			inline void extend(const ienum_t*) override;
			inline void append(com_wrapper<T> const&, bool last = true) override;
			inline bool insert(index idx, com_wrapper<T> const&) override;
			inline bool insert(iterator_t it, com_wrapper<T> const&) override;
			inline bool pop(bool last = true) override;
			inline bool pop(com_wrapper<T>&, bool last = true) override;
			inline bool pop_at(index idx) override;
			inline bool pop_at(iterator_t it) override;
			inline bool pop_at(index idx, com_wrapper<T>&) override;
			inline bool pop_at(iterator_t it, com_wrapper<T>&) override;

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		public: //Operators
			inline bool operator == (const vector_buffer& ar);
			inline bool operator != (const vector_buffer& ar);
			inline explicit operator T**()const;

		protected: //Memory Operations
			inline bool realloc(uint size, bool save = true);
			inline bool realloc(memory::iallocator* alloc);
		};
	}
}

#include <ang/inline/com_wrapper.hpp>

#endif//__COM_WRAPPER_H__