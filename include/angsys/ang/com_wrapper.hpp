#ifndef __COM_WRAPPER_HPP__
#define __COM_WRAPPER_HPP__

#include <angsys.hpp>
#include <ang/collections/vector.hpp>
#include <ang/core/async.hpp>

namespace ang
{
	template<typename T> class com_wrapper_ptr;

	template<typename T>
	class com_wrapper //: public rule<is_base_of<IUnknown, T>::value>
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
		bool is_empty()const { return _ptr == null; }
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
			: public runtime_type_builder<typename com_wrapper<T>::type> {
		};
	}

	namespace collections
	{
		/******************************************************************/
		/* template class vector_buffer :                                 */
		/*  -> implements the buffer memory of vector object              */
		/******************************************************************/
		template<typename T, template<typename> class allocator>
		class vector_buffer<com_wrapper<T>, allocator> final
			: public object
			, public ibuffer
			, public ilist<com_wrapper<T>>
		{
		public:
			typedef T											type;
			typedef vector_buffer<com_wrapper<T>, allocator>	self_t;
			typedef iarray<com_wrapper<T>>						iarray;
			typedef ienum<com_wrapper<T>>						ienum_t;
			typedef ilist<com_wrapper<T>>						ilist_t;
			typedef base_iterator<com_wrapper<T>>				base_iterator_t;
			typedef iterator<com_wrapper<T>>					iterator_t;
			typedef const_iterator<com_wrapper<T>>				const_iterator_t;
			typedef forward_iterator<com_wrapper<T>>			forward_iterator_t;
			typedef const_forward_iterator<com_wrapper<T>>		const_forward_iterator_t;
			typedef backward_iterator<com_wrapper<T>>			backward_iterator_t;
			typedef const_backward_iterator<com_wrapper<T>>		const_backward_iterator_t;


		protected:
			wsize _size;
			wsize _capacity;
			type* _data;

		public:
			inline vector_buffer();
			inline vector_buffer(wsize reserve);
			template<typename U>inline vector_buffer(ang::initializer_list_t<U> list);
			inline vector_buffer(const ang::nullptr_t&);
			inline vector_buffer(vector_buffer&& ar);
			inline vector_buffer(const vector_buffer& ar);
			inline vector_buffer(const vector_buffer* ar);
			inline vector_buffer(const ienum_t* store);
			template<typename U> inline vector_buffer(array_view<U> const&);
			template<typename U, template<typename> class allocator2> inline vector_buffer(scope_array<U, allocator2> const&);
			template<typename U, wsize SIZE> inline vector_buffer(stack_array<U, SIZE> const&);

		private:
			virtual ~vector_buffer();

		public: //methods
			inline bool is_empty()const;
			inline com_wrapper<T>* data()const;
			inline wsize size()const;
			inline wsize capacity()const;
			inline void capacity(wsize size, bool save = false);

			inline void clean();
			inline void empty();
			inline bool move(vector_buffer<com_wrapper<T>, allocator>&);

			template<typename U> inline void copy(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void copy(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void copy(stack_array<U, SIZE> const&);

			template<typename U> inline void expand(array_view<U>const&);
			template<typename U, template<typename> class allocator2> inline void expand(scope_array<U, allocator2>const&);
			template<typename U, wsize SIZE> inline void expand(stack_array<U, SIZE> const&);

		private:

		public: //ibuffer overrides
			inline text::encoding_t encoding()const override;
			inline pointer buffer_ptr()const override;
			inline wsize buffer_size()const override;
			inline wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override;
			inline ibuffer_view_t map_buffer(windex, wsize) override;
			inline bool unmap_buffer(ibuffer_view_t&, wsize) override;
			inline bool can_realloc_buffer()const override;
			inline bool realloc_buffer(wsize) override;

		public: //ienum overrides
			inline wsize counter()const override;

			inline com_wrapper<T>& at(base_iterator_t const&) override;
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

		public: //iarray overrides
			inline iterator_t at(windex) override;
			inline const_iterator_t at(windex)const override;
			inline void copy(const ienum<com_wrapper<T>>*) override;
			inline iterator_t find(const com_wrapper<T>&, bool invert = false)const override;
			inline iterator_t find(const com_wrapper<T>&, base_iterator_t next_to, bool invert = false)const override;

		public: //ilist overrides
			inline void extend(const ienum<com_wrapper<T>>*) override;
			inline void push(com_wrapper<T> const&, bool last = true) override;
			inline bool insert(windex idx, com_wrapper<T> const&) override;
			inline bool insert(base_iterator_t it, com_wrapper<T> const&) override;
			inline bool pop(bool last = true) override;
			inline bool pop(com_wrapper<T>&, bool last = true) override;
			inline bool pop_at(windex idx) override;
			inline bool pop_at(base_iterator_t it) override;
			inline bool pop_at(windex idx, com_wrapper<T>&) override;
			inline bool pop_at(base_iterator_t it, com_wrapper<T>&) override;

		public: //overrides
			inline static type_name_t class_name();
			inline static bool is_inherited_of(type_name_t);
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;
			inline comparision_result_t compare(const object&)const override;

		protected: //Memory Operations
			inline bool realloc(wsize size, bool save = true);
		};
	}
}

#include <ang/inline/com_wrapper.inl>

#endif//__COM_WRAPPER_HPP__