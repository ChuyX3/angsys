#ifndef __SAMRT_PTR_H__
#error ...
#elif !defined __INTF_WRAPPER_HPP__
#define __INTF_WRAPPER_HPP__

namespace ang
{

	/******************************************************************/
	/* template class ang::intf_wrapper :                             */
	/*  -> implements handling of smart pointers for intercafes       */
	/******************************************************************/
	template<class T>
	class intf_wrapper
	{
	public:
		typedef T type;
		typedef T* type_ptr;
		typedef T& type_ref;
		typedef T const* ctype_ptr;
		typedef T const& ctype_ref;

		intf_wrapper()
			: m_ptr(null) { 
			static_assert(is_interface<type>::value && !is_object<type>::value, "ERROR: T is not a interface type...");
		}

		intf_wrapper(type* ptr)
			: intf_wrapper() { 
			set(ptr);
		}

		intf_wrapper(ang::nullptr_t const&) 
			: intf_wrapper() {
		}

		intf_wrapper(intf_wrapper && ptr)
			: intf_wrapper() {
			T * temp = ptr.m_ptr;
			ptr.m_ptr = null;
			m_ptr = temp;
		}

		intf_wrapper(intf_wrapper const& ptr)
			: intf_wrapper() { 
			set(ptr.get());
		}

		~intf_wrapper() { 
			reset(); 
		}

	public:
		void reset() {
			iobject * _obj = interface_cast<iobject>(m_ptr);
			if (_obj)_obj->release();
			m_ptr = null;
		}

		void reset_unsafe() {
		//	iobject * _obj = interface_cast<iobject>(m_ptr);
		//	if (_obj)_obj->release();
			m_ptr = null;
		}

		bool is_empty()const { 
			return m_ptr == null;
		}

		type* get(void)const {
			return m_ptr;
		}

		void set(type* ptr) {
			if (ptr == m_ptr)
				return;
			iobject * _old = interface_cast<iobject>(m_ptr);
			iobject * _new = interface_cast<iobject>(ptr);
			m_ptr = ptr;
			if (_new)_new->add_ref();
			if (_old)_old->release();
		}

		void move(intf_wrapper& ptr) {
			if (this == &ptr)
				return;
			reset();
			m_ptr = ptr.m_ptr;
			ptr.m_ptr = null;
		}

		type ** addres_of(void) { 
			return &m_ptr;
		}

		type ** addres_for_init(void) {
			reset(); 
			return &m_ptr;
		}

		template<typename U> typename smart_ptr_type<U>::smart_ptr_t as() {
			return  this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null;
		}

		template<typename U> bool as(U*& out) {
			out = this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null;
			return out != null;
		}


	public: //operator
		intf_wrapper& operator = (type* ptr) { 
			set(ptr); 
			return*this;
		}

		intf_wrapper& operator = (ang::nullptr_t const&) { 
			reset(); 
			return*this;
		}

		intf_wrapper& operator = (intf_wrapper && ptr) {
			move(ptr); 
			return*this;
		}

		intf_wrapper& operator = (intf_wrapper const& ptr) { 
			set(ptr.get());
			return*this;
		}

		intf_wrapper_ptr<type> operator & (void);

		type* operator -> (void) { 
			return get();
		}

		type const* operator -> (void)const {
			return get();
		}

		operator type* (void) {
			return get(); 
		}

		operator type const* (void)const {
			return get();
		}

	protected:
		type* m_ptr;

	};

	/******************************************************************/
	/* template class ang::intf_wrapper_ptr :                         */
	/*  -> reprecents a intf_wrapper pointer                          */
	/******************************************************************/
	template<typename T>
	class intf_wrapper_ptr
	{
	public:
		intf_wrapper_ptr(intf_wrapper<T>*ptr)
			: m_ptr(ptr) {
		}

		intf_wrapper_ptr(intf_wrapper_ptr && ptr)
			: m_ptr(ptr.m_ptr) { 
			ptr.m_ptr = null;
		}

		intf_wrapper_ptr(intf_wrapper_ptr const& ptr)
			: m_ptr(ptr.m_ptr) {
		}

		~intf_wrapper_ptr() { 
			m_ptr = null;
		}

		bool is_empty()const {
			return m_ptr == null;
		}

		intf_wrapper<T>* operator ->()const {
			return m_ptr;
		}

		operator intf_wrapper<T>*()const {
			return m_ptr;
		}

		operator T**()const { 
			return m_ptr->addres_of();
		}

		operator unknown_ptr_t()const {
			return m_ptr->addres_of();
		}

		intf_wrapper<T>& operator *() {
			return *m_ptr;
		}

	private:
		intf_wrapper<T>* m_ptr;

	};

	/******************************************************************/
	/* template class ang::intf_wrapper<interface> :                  */
	/*  -> specialization of intf_wrapper<interface> -> intfptr       */
	/******************************************************************/
	template<> 
	class LINK intf_wrapper<interface> 
	{
	public:
		typedef interface type;
		typedef interface* type_ptr;
		typedef interface& type_ref;
		typedef interface const* ctype_ptr;
		typedef interface const& ctype_ref;

	public:
		intf_wrapper();
		intf_wrapper(type_ptr);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);

		template<typename T> intf_wrapper(T*);
		template<typename T> intf_wrapper(intf_wrapper<T> const&);

		~intf_wrapper();

	public:
		type_ptr get(void)const;
		void set(type_ptr);
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type ** addres_of(void);
		type ** addres_for_init(void);

		template<typename U> typename smart_ptr_type<U>::smart_ptr_t as() {
			return interface_cast<typename smart_ptr_type<U>::type>(m_ptr);
		}

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);
		template<typename T> intf_wrapper& operator = (T*);
		template<typename T> intf_wrapper& operator = (intf_wrapper<T> const&);

		intf_wrapper_ptr<type> operator & (void);

		type* operator -> (void);
		type const* operator -> (void)const;
		operator type* (void);
		operator type const* (void)const;

	private:
		interface* m_ptr;

	};

	/******************************************************************/
	/* class ang::safe_pointer :                                      */
	/*  -> implements a basic weak smart pointer                      */
	/******************************************************************/
	class LINK safe_pointer
	{
	public:
		typedef interface type;

	private:
		pointer _info;
		wsize _offset;

	public:
		safe_pointer();
		safe_pointer(safe_pointer&&);
		safe_pointer(safe_pointer const&);
		safe_pointer(interface*);
		safe_pointer(std::nullptr_t const&);
		template< typename T>
		safe_pointer(intf_wrapper<T> obj) : safe_pointer((interface*)obj.get()) {}
		template< typename T>
		safe_pointer(object_wrapper<T> obj) : safe_pointer((interface*)obj.get()) {}
		~safe_pointer();

	private:
		void set(interface*);
		void reset();
		void reset_unsafe();

	public: //properties
		bool is_valid()const;
		template< typename T>
		typename smart_ptr_type<T>::smart_ptr_t lock() {
			return is_valid() ? interface_cast<typename smart_ptr_type<T>::type>(lock<interface>().get()) : nullptr;
		}

		safe_pointer& operator = (intfptr);
		safe_pointer& operator = (interface*);
		safe_pointer& operator = (safe_pointer&&);
		safe_pointer& operator = (safe_pointer const&);
		template< typename T>
		safe_pointer& operator = (intf_wrapper<T> obj) { return operator = ((interface*)obj.get()); }
		template< typename T>
		safe_pointer& operator = (object_wrapper<T> obj) { return operator = ((interface*)obj.get()); }

		safe_pointer& operator = (std::nullptr_t const&);
	};

	template<> LINK intfptr safe_pointer::lock<interface>();

	template<typename T>
	class weak_ptr : public safe_pointer
	{
	public:
		typedef T type;
		typedef T* type_ptr;
		typedef T& type_ref;
		typedef T const* ctype_ptr;
		typedef T const& ctype_ref;

	public:
		weak_ptr() : safe_pointer() {}
		weak_ptr(weak_ptr&& other) : safe_pointer((safe_pointer&&)other) {}
		weak_ptr(weak_ptr const& other) : safe_pointer((safe_pointer const&)other) {}
		weak_ptr(ang::nullptr_t const&) : safe_pointer(null) {}
		weak_ptr(T* obj) : safe_pointer(obj) {}
		weak_ptr(typename smart_ptr_type<T>::smart_ptr_t obj) : safe_pointer(obj.get()) {}
		~weak_ptr() {}

	public: //properties
		typename smart_ptr_type<T>::smart_ptr_t lock()const {
			auto ptr = const_cast<weak_ptr<T>*>(this)->safe_pointer::lock<intfptr>();
			return reinterpret_cast<T*>(ptr.get());
		}

		weak_ptr& operator = (typename smart_ptr_type<T>::smart_ptr_t obj) { safe_pointer::operator=(obj.get()); return *this; }
		weak_ptr& operator = (T* obj) { safe_pointer::operator=(obj);  return *this; }
		weak_ptr& operator = (weak_ptr&& other) { safe_pointer::operator=(other); return *this; }
		weak_ptr& operator = (weak_ptr const& other) { safe_pointer::operator=(other);  return *this; }
		weak_ptr& operator = (ang::nullptr_t const&) { safe_pointer::operator=(null); return *this; }
	};


	template<typename T, class owner, property_style TYPE>
	class property<const intf_wrapper<T>, owner, TYPE> {
	public:
		property(T* val) : value(val) {}
		T* get()const { return value.get(); }
		operator T*()const { return value.get(); }
		T* operator ->()const { return value.get(); }

	protected:
		void set(T* val) { value = val; }
		property& operator = (property const& val) { set(val.get()); return*this; }
		property& operator = (T* val) { set(val); return*this; }

	private:
		typename intf_wrapper<T> value;
		friend owner;
	};
}


template<typename T>
inline ang::intf_wrapper_ptr<T> ang::intf_wrapper<T>::operator & (void) {
	return this;
}


#endif//__INTF_WRAPPER_HPP__