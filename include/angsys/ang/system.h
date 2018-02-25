/*********************************************************************************************************************/
/*   File Name: ang/system.h                                                                                         */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is the base for all library, it exposes the "object" class which is the class       */
/*   base for many other clases.                                                                                     */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */                                   
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_HPP__
#define __ANGSYS_HPP__

#include <ang/base/base.h>

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
	/******************************************************************/
	/* enum ang::comparision_result_t :                                */
	/*  -> represents the result of a comparison operation            */
	/******************************************************************/
	safe_enum(LINK, comparision_result, int)
	{
		minor = -1,
		same = 0,
		mayor = 1,
		diferent = 20
	};

	/******************************************************************/
	/* interface ang::iobject :                                       */
	/*  -> implements basic methods for smart pointer management      */
	/******************************************************************/
	ang_begin_interface(LINK iobject)
		visible vcall dword add_ref() pure;
		visible vcall dword release() pure;
	ang_end_interface();

	ang_object(object);
	typedef object_t objptr;

	template<typename T> struct is_object : integer_constant<bool, is_same_type<T, object>::value || is_inherited_from<T, object>::value> {};

	template<typename T, bool VALUE = is_interface<T>::value> struct is_smart_ptr: public false_type {};
	template<typename T> struct is_smart_ptr<T, true> : public true_type {};

	template<typename T, bool VALUE1 = is_object<T>::value, bool VALUE2 = is_interface<T>::value> 
	struct smart_ptr_type { static_assert(is_smart_ptr<T>::value, "T is not a smart type");  typedef T* smart_ptr_t; };

	template<typename T> struct smart_ptr_type<T, false, true> { typedef intf_wrapper<T> smart_ptr_t; typedef typename smart_ptr_t::type type; };
	template<typename T> struct smart_ptr_type<T, true, true> { typedef object_wrapper<T> smart_ptr_t; typedef typename smart_ptr_t::type type; };

	template<typename T> struct smart_ptr_type<T, false, false> { 
		//typedef shared_ptr<T> smart_ptr_t; typedef typename smart_ptr_t::type type;
		typedef T* smart_ptr_t; typedef T type;
	};


	/******************************************************************/
	/* template class ang::intf_wrapper :                             */
	/*  -> implements handling of smart pointers for intercafes       */
	/******************************************************************/
	template<class T> class intf_wrapper {
	public:
		typedef T				type;
	protected:
		type* _ptr;

	public:
		intf_wrapper() : _ptr(null) { static_assert(is_interface<type>::value && !is_object<type>::value, "ERROR: T is not a interface type..."); }
		intf_wrapper(type* ptr) : intf_wrapper() { set(ptr); }
		intf_wrapper(ang::nullptr_t const&) : intf_wrapper() { }
		intf_wrapper(intf_wrapper && ptr) : intf_wrapper() {
			T * temp = other._ptr;
			other._ptr = null;
			_ptr = temp;
		}
		intf_wrapper(intf_wrapper const& ptr) : intf_wrapper() { set(ptr.get()); }
		~intf_wrapper() { clean(); }

	public:
		void clean() {
			iobject * _obj = dyn_cast<iobject>(_ptr);
			if (_obj)_obj->release();
			_ptr = null;
		}
		bool is_empty()const { return _ptr == null; }
		type* get(void)const { return _ptr; }
		void set(type* ptr) {
			if (ptr == _ptr) return;
			iobject * _old = dyn_cast<iobject>(_ptr);
			iobject * _new = dyn_cast<iobject>(ptr);
			_ptr = ptr;
			if (_new)_new->add_ref();
			if (_old)_old->release();
		}
		void move(intf_wrapper& ptr) {
			if (this == &ptr)
				return;
			clean();
			_ptr = ptr._ptr;
			ptr._ptr = null;
		}
		type ** addres_of(void) { return &_ptr; }
		type ** addres_for_init(void) { clean(); return &_ptr; }

	public:
		intf_wrapper& operator = (type* ptr) { set(ptr); return*this; }
		intf_wrapper& operator = (ang::nullptr_t const&) { clean(); return*this; }
		intf_wrapper& operator = (intf_wrapper && ptr) { move(ptr); return*this; }
		intf_wrapper& operator = (intf_wrapper const& ptr) { set(ptr.get()); return*this; }

		intf_wrapper_ptr<type> operator & (void);
		type * operator -> (void) { return get(); }
		type const* operator -> (void)const { return get(); }
		operator type * (void) { return get(); }
		operator type const* (void)const { return get(); }
	};


	/******************************************************************/
	/* template class ang::intf_wrapper_ptr :                         */
	/*  -> reprecents a intf_wrapper pointer                          */
	/******************************************************************/
	template<typename T> class intf_wrapper_ptr {
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

	template<typename T> intf_wrapper_ptr<T> intf_wrapper<T>::operator & (void) { return this; }


	/******************************************************************/
	/* template class ang::object_wrapper :                           */
	/*  -> implements handling of smart pointers                      */
	/******************************************************************/
	template<typename T> class object_wrapper {
	public:
		typedef T type;

	private:
		type* _ptr;

	public:
		object_wrapper() : _ptr(null) { static_assert(is_object<type>::value, "ERROR: T is not a object type..."); }
		object_wrapper(type* ptr) : object_wrapper() { set(ptr); }
		object_wrapper(object_wrapper && ptr) : object_wrapper() { 
			T * temp = ptr._ptr;
			ptr._ptr = null;
			_ptr = temp;
		}
		object_wrapper(object_wrapper const& ptr) : object_wrapper() { set(ptr); }
		object_wrapper(ang::nullptr_t const&) : object_wrapper() { }
		~object_wrapper() { clean(); }

	public:
		void clean() {
			if (_ptr)_ptr->release(); 
			_ptr = null;
		}
		void clean_unsafe() { _ptr = null; }
		bool is_empty()const { return _ptr == null;	}
		type* get(void)const { return _ptr; }
		void set(type* ptr) {
			T * temp = _ptr;
			if (ptr == _ptr) return;
			_ptr = ptr;
			if (_ptr)_ptr->add_ref();
			if (temp)temp->release();
		}
		void move(object_wrapper& ptr) {
			if (this == &ptr) 
				return;
			clean();
			_ptr = ptr._ptr;
			ptr._ptr = null;
		}
		type ** addres_of(void) { return&_ptr; }
		type ** addres_for_init(void) { clean(); return&_ptr; }

	public:
		object_wrapper& operator = (type* ptr) { set(ptr); return*this;	}
		object_wrapper& operator = (ang::nullptr_t const&) { clean(); return*this; }
		object_wrapper& operator = (object_wrapper && ptr) { move(ptr); return*this; }
		object_wrapper& operator = (object_wrapper const& ptr) { set(ptr.get()); return*this; }


		object_wrapper_ptr<T> operator & (void);
		type * operator -> (void) { return get(); }
		type const* operator -> (void)const { return get(); }
		operator type * (void) { return get(); }
		operator type const* (void)const { return get(); }

		friend class safe_pointer;
	};

	template<typename T1, typename T2>
	bool operator == (object_wrapper<T1> const& obj1, object_wrapper<T2> const& obj2) {
		if ((object*)obj1.get() == (object*)obj2.get())
			return true;
		if ((!obj1.get() && obj2.get()) || (obj1.get() && !obj2.get()))
			return false;
		return *obj1.get() == *obj2.get();
	}

	template<typename T1, typename T2>
	bool operator != (object_wrapper<T1> const& obj1, object_wrapper<T2> const& obj2) {
		return operator == (obj1, obj2);
	}

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

	template<typename T> object_wrapper_ptr<T> object_wrapper<T>::operator & (void) { return this; }



	/******************************************************************/
	/* template class ang::object_wrapper<object> :                   */
	/*  -> specialization of object_wrapper<object> -> objptr         */
	/******************************************************************/
	template<> class LINK object_wrapper<object> {
	public:
		typedef object type;

	private:
		object* _ptr;

	public:
		object_wrapper();
		object_wrapper(object*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	//	template<wsize N> object_wrapper(const char(&ar)[N]);

	//	template<wsize N> object_wrapper(const wchar(&ar)[N]);

	//	template<typename T>
	//	inline object_wrapper(safe_str<T> const& str); //string convertible

	//	inline object_wrapper(bool); //int convertible

	//	inline object_wrapper(int); //int convertible
	//	inline object_wrapper(uint); //uint convertible

	//	inline object_wrapper(long); //long convertible
	//	inline object_wrapper(ulong); //ulong convertible

	//	inline object_wrapper(long64); //long convertible
	//	inline object_wrapper(ulong64); //ulong convertible

	//	inline object_wrapper(float); //float convertible
	//	inline object_wrapper(double); //double convertible

	//	template<typename T>//object convertible
	//	inline object_wrapper(object_wrapper<T>);

	//	template<typename T>//array convertible
	//	inline object_wrapper(initializer_list_t<T>);

		template<typename T> typename smart_ptr_type<T>::smart_ptr_t as() {
			return dyn_cast<typename smart_ptr_type<T>::type>(_ptr);
		}

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		object* get(void)const;
		void set(object*);
		object ** addres_of(void);

	public:
		object_wrapper& operator = (object*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<object> operator & (void);
		object * operator -> (void);
		object const* operator -> (void)const;
		operator object * (void);
		operator object const* (void)const;

		template<typename T> explicit operator T& () {
			return (T&)as<T>();
		}

		template<typename T> explicit operator T const& ()const {
			return (T const&)const_cast<objptr*>(this)->as<T>();
		}

		friend class safe_pointer;
	};

	template<> class LINK intf_wrapper<interface> {
	public:
		typedef interface type;

	protected:
		interface* _ptr;

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
		void clean();
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
		type * operator -> (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;

	};

}



#endif //__ANGSYS_HPP__