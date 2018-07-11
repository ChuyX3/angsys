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
			: _ptr(null) { 
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
			T * temp = ptr._ptr;
			ptr._ptr = null;
			_ptr = temp;
		}

		intf_wrapper(intf_wrapper const& ptr)
			: intf_wrapper() { 
			set(ptr.get());
		}

		~intf_wrapper() { 
			clear(); 
		}

	public:
		void clear() {
			iobject * _obj = dyn_cast<iobject>(_ptr);
			if (_obj)_obj->release();
			_ptr = null;
		}

		bool is_empty()const { 
			return _ptr == null;
		}

		type* get(void)const {
			return _ptr;
		}

		void set(type* ptr) {
			if (ptr == _ptr)
				return;
			iobject * _old = dyn_cast<iobject>(_ptr);
			iobject * _new = dyn_cast<iobject>(ptr);
			_ptr = ptr;
			if (_new)_new->add_ref();
			if (_old)_old->release();
		}

		void move(intf_wrapper& ptr) {
			if (this == &ptr)
				return;
			clear();
			_ptr = ptr._ptr;
			ptr._ptr = null;
		}

		type ** addres_of(void) { 
			return &_ptr;
		}

		type ** addres_for_init(void) {
			clear(); 
			return &_ptr;
		}

		template<typename U> typename smart_ptr_type<U>::smart_ptr_t as() {
			return  this ? dyn_cast<typename smart_ptr_type<U>::type>(_ptr) : null;
		}

		template<typename U> bool as(U*& out) {
			out = this ? dyn_cast<typename smart_ptr_type<U>::type>(_ptr) : null;
			return out != null;
		}


	public: //operator
		intf_wrapper& operator = (type* ptr) { 
			set(ptr); 
			return*this;
		}

		intf_wrapper& operator = (ang::nullptr_t const&) { 
			clear(); 
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

		type* operator -> (void)const { 
			return get();
		}

		operator type* (void)const {
			return get(); 
		}

	protected:
		type* _ptr;

	};

	template<class T>
	class intf_wrapper<const T>
	{
	public:
		typedef T type;
		typedef T* type_ptr;
		typedef T& type_ref;
		typedef T const* ctype_ptr;
		typedef T const& ctype_ref;

		intf_wrapper()
			: _ptr(null) {
			static_assert(is_interface<type>::value && !is_object<type>::value, "ERROR: T is not a interface type...");
		}

		intf_wrapper(ctype_ptr ptr)
			: intf_wrapper() {
			set(ptr);
		}

		intf_wrapper(ang::nullptr_t const&)
			: intf_wrapper() {
		}

		intf_wrapper(intf_wrapper && ptr)
			: intf_wrapper() {
			T * temp = ptr._ptr;
			ptr._ptr = null;
			_ptr = temp;
		}

		intf_wrapper(intf_wrapper const& ptr)
			: intf_wrapper() {
			set(ptr.get());
		}

		~intf_wrapper() {
			clear();
		}

	public:
		void clear() {
			iobject * _obj = dyn_cast<iobject>(const_cast<type_ptr>(_ptr));
			if (_obj)_obj->release();
			_ptr = null;
		}

		bool is_empty()const {
			return _ptr == null;
		}

		ctype_ptr get(void)const {
			return _ptr;
		}

		void set(ctype_ptr ptr) {
			if (ptr == _ptr)
				return;
			iobject * _old = dyn_cast<iobject>(const_cast<type_ptr>(_ptr));
			iobject * _new = dyn_cast<iobject>(const_cast<type_ptr>(ptr));
			_ptr = ptr;
			if (_new)_new->add_ref();
			if (_old)_old->release();
		}

		void move(intf_wrapper& ptr) {
			if (this == &ptr)
				return;
			clear();
			_ptr = ptr._ptr;
			ptr._ptr = null;
		}

		ctype_ptr* addres_of(void) {
			return &_ptr;
		}

		ctype_ptr* addres_for_init(void) {
			clear();
			return &_ptr;
		}

		template<typename U> typename smart_ptr_type<U>::const_smart_ptr_t as() {
			return  this ? dyn_cast<typename smart_ptr_type<U>::const_type>(_ptr) : null;
		}

		template<typename U> bool as(U const*& out) {
			out = this ? dyn_cast<typename smart_ptr_type<U>::const_type>(_ptr) : null;
			return out != null;
		}


	public: //operator
		intf_wrapper& operator = (ctype_ptr ptr) {
			set(ptr);
			return*this;
		}

		intf_wrapper& operator = (ang::nullptr_t const&) {
			clear();
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

		intf_wrapper_ptr<const type> operator & (void);

		type const* operator -> (void)const {
			return get();
		}

		operator type const* (void)const {
			return get();
		}

	protected:
		type const* _ptr;

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
			: _ptr(ptr) {
		}

		intf_wrapper_ptr(intf_wrapper_ptr && ptr)
			: _ptr(ptr._ptr) { 
			ptr._ptr = null;
		}

		intf_wrapper_ptr(intf_wrapper_ptr const& ptr)
			: _ptr(ptr._ptr) {
		}

		~intf_wrapper_ptr() { 
			_ptr = null;
		}

		bool is_empty()const {
			return _ptr == null;
		}

		intf_wrapper<T>* operator ->()const {
			return _ptr;
		}

		operator intf_wrapper<T>*()const {
			return _ptr;
		}

		operator T**()const { 
			return _ptr->addres_of();
		}

		operator unknown_ptr_t()const {
			return _ptr->addres_of();
		}

		intf_wrapper<T>& operator *() {
			return *_ptr;
		}

	private:
		intf_wrapper<T>* _ptr;

	};

	template<typename T>
	class intf_wrapper_ptr<const T>
	{
	public:
		intf_wrapper_ptr(intf_wrapper<const T>*ptr)
			: _ptr(ptr) {
		}

		intf_wrapper_ptr(intf_wrapper_ptr && ptr)
			: _ptr(ptr._ptr) {
			ptr._ptr = null;
		}

		intf_wrapper_ptr(intf_wrapper_ptr const& ptr)
			: _ptr(ptr._ptr) {
		}

		~intf_wrapper_ptr() {
			_ptr = null;
		}

		bool is_empty()const {
			return _ptr == null;
		}

		intf_wrapper<const T>* operator ->()const {
			return _ptr;
		}

		operator intf_wrapper<const T>*()const {
			return _ptr;
		}

		operator T const**()const {
			return _ptr->addres_of();
		}

		operator unknown_ptr_t()const {
			return _ptr->addres_of();
		}

		intf_wrapper<const T>& operator *() {
			return *_ptr;
		}

	private:
		intf_wrapper<const T>* _ptr;

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
		intf_wrapper(type*);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);

		template<typename T> intf_wrapper(T*);
		template<typename T> intf_wrapper(intf_wrapper<T> const&);

		~intf_wrapper();

	public:
		void clear();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

		template<typename T> typename smart_ptr_type<T>::smart_ptr_t as() {
			return dyn_cast<typename smart_ptr_type<T>::type>(_ptr);
		}

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);
		template<typename T> intf_wrapper& operator = (T*);
		template<typename T> intf_wrapper& operator = (intf_wrapper<T> const&);

		intf_wrapper_ptr<type> operator & (void);
		type* operator -> (void)const;
		operator type* (void)const;

	private:
		interface* _ptr;

	};

	template<>
	class LINK intf_wrapper<const interface>
	{
	public:
		typedef interface type;
		typedef interface* type_ptr;
		typedef interface& type_ref;
		typedef interface const* ctype_ptr;
		typedef interface const& ctype_ref;

	public:
		intf_wrapper();
		intf_wrapper(ctype_ptr);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);

		template<typename T> intf_wrapper(T const*);
		template<typename T> intf_wrapper(intf_wrapper<const T> const&);

		~intf_wrapper();

	public:
		void clear();
		bool is_empty()const;
		type const* get(void)const;
		void set(type const*);
		type const** addres_of(void);

		template<typename T> typename smart_ptr_type<T>::const_smart_ptr_t as() {
			return dyn_cast<typename smart_ptr_type<T>::const_type>(_ptr);
		}

	public:
		intf_wrapper& operator = (type const*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);
		template<typename T> intf_wrapper& operator = (T const*);
		template<typename T> intf_wrapper& operator = (intf_wrapper<const T> const&);

		intf_wrapper_ptr<const type> operator & (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;

	private:
		interface const* _ptr;

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
		void clear();

	public: //properties
		bool is_valid()const;
		template< typename T>
		typename smart_ptr_type<T>::smart_ptr_t lock() {
			return is_valid() ? dyn_cast<typename smart_ptr_type<T>::type>(lock<interface>().get()) : nullptr;
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
			auto ptr = safe_pointer::lock<intfptr>();
			return reinterpret_cast<T*>(ptr.get());
		}

		weak_ptr& operator = (typename smart_ptr_type<T>::smart_ptr_t obj) { safe_pointer::operator=(obj.get()); return *this; }
		weak_ptr& operator = (T* obj) { safe_pointer::operator=(obj);  return *this; }
		weak_ptr& operator = (weak_ptr&& other) { safe_pointer::operator=(other); return *this; }
		weak_ptr& operator = (weak_ptr const& other) { safe_pointer::operator=(other);  return *this; }
		weak_ptr& operator = (ang::nullptr_t const&) { safe_pointer::operator=(null); return *this; }
	};
}


template<typename T>
inline ang::intf_wrapper_ptr<T> ang::intf_wrapper<T>::operator & (void) {
	return this;
}

template<typename T>
inline ang::intf_wrapper_ptr<const T> ang::intf_wrapper<const T>::operator & (void) {
	return this;
}

#endif//__INTF_WRAPPER_HPP__