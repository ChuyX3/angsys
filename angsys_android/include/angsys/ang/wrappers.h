#ifndef  __ANGSYS_H__
#error ...
#elif !defined __ANG_WRAPPERS_H__
#define __ANG_WRAPPERS_H__

namespace ang
{

	/******************************************************************/
	/* template class ang::object_wrapper :                           */
	/*  -> implements handling of smart pointers                      */
	/******************************************************************/
	template<typename T>
	class object_wrapper
	{
	public:
		typedef T type;

	private:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(type*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<T> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;

		friend safe_pointer;
	};

	/******************************************************************/
	/* template class ang::object_wrapper_ptr :                       */
	/*  -> reprecents a object_wrapper pointer                        */
	/******************************************************************/
	template<typename T>
	class object_wrapper_ptr
	{
		object_wrapper<T>* _ptr;
	public:
		object_wrapper_ptr(ang::nullptr_t const&) : _ptr(null) {}
		object_wrapper_ptr(T** ptr) : _ptr( reinterpret_cast<object_wrapper<T>*>(ptr)) {}
		object_wrapper_ptr(object_wrapper<T>*ptr) : _ptr(ptr) {}
		object_wrapper_ptr(object_wrapper_ptr && ptr) : _ptr(ptr._ptr) { ptr._ptr = null; }
		object_wrapper_ptr(object_wrapper_ptr const& ptr) : _ptr(ptr._ptr) {}
		~object_wrapper_ptr() { _ptr = null; }
		bool is_empty()const { return _ptr == null; }
		object_wrapper<T>& operator *()const { return *_ptr; }
		object_wrapper<T>* operator ->()const { return _ptr; }
		operator object_wrapper<T>*()const { return _ptr; }
		operator unknown_ptr_t()const { return _ptr->addres_of(); }
		operator T**()const { return _ptr->addres_of(); }
	};


	/******************************************************************/
	/* template class ang::intf_wrapper :                             */
	/*  -> implements handling of smart pointers for intercafes       */
	/******************************************************************/
	template<class T> class intf_wrapper
	{
	public:
		typedef T				type;
	protected:
		type* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(type*);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();

	public:
		void clean();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;
	};


	/******************************************************************/
	/* template class ang::intf_wrapper_ptr :                         */
	/*  -> reprecents a intf_wrapper pointer                          */
	/******************************************************************/
	template<typename T>
	class intf_wrapper_ptr
	{
		intf_wrapper<T>* _ptr;
	public:
		intf_wrapper_ptr(intf_wrapper<T>*ptr) : _ptr(ptr) {}
		intf_wrapper_ptr(intf_wrapper_ptr && ptr) : _ptr(ptr._ptr) { ptr._ptr = null; }
		intf_wrapper_ptr(intf_wrapper_ptr const& ptr) : _ptr(ptr._ptr) {}
		~intf_wrapper_ptr() { _ptr = null; }

		bool is_empty()const { return _ptr == null; }
		intf_wrapper<T>* operator ->()const { return _ptr; }
		operator intf_wrapper<T>*()const { return _ptr; }
		operator T**()const { return _ptr->addres_of(); }
		operator unknown_ptr_t()const { return _ptr->addres_of(); }
		intf_wrapper<T>& operator *() { return *_ptr; }
	};
}
#endif//__ANG_WRAPPERS_H__
