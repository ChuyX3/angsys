/*********************************************************************************************************************/
/*   File Name: angsys.h                                                                                             */
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

#ifndef __ANGSYS_H__
#define __ANGSYS_H__

#include "angbase.h"

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

#include <ang/runtime.h>

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

	template<typename T>
	bool operator == (object_wrapper<T> const& obj1, object_wrapper<T> const& obj2) {
		if (obj1.get() == obj2.get())
			return true;
		if ((!obj1.get() && obj2.get()) || (obj1.get() && !obj2.get()))
			return false;
		return *obj1.get() == *obj2.get();
	}
	template<typename T>
	bool operator != (object_wrapper<T> const& obj1, object_wrapper<T> const& obj2) {
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
		intf_wrapper<T>& operator *() { return *_ptr; }
	};

	/******************************************************************/
	/* class ang::object :                                            */
	/*  -> implements the base class for all library's objects        */
	/******************************************************************/
	class LINK object
		: public iobject
	{
	private:
		dword& ref_count;

	public:
		object(bool inc_ref = false);

	protected:
	
		virtual~object();

		object(object &&) = delete;
		object(object const&) = delete;
		object& operator = (object &&) = delete;
		object& operator = (object const&) = delete;

	public:
		virtual comparision_result_t compare(object const& obj)const;
		virtual string_t to_string()const;

	protected:
		virtual bool auto_release();
		virtual bool auto_release(ushort alignment);

	public:
		ANG_DECLARE_INTERFACE();

	public:
		virtual dword add_ref() override;
		virtual dword release() override;

	public:
		pointer operator new(wsize);
		void operator delete(pointer);

#ifdef _DEBUG
		pointer operator new(wsize, const char*, int);
		void operator delete(pointer, const char*, int);
#endif//_DEBUG

	protected: //for aligned objects
		pointer operator new(wsize, ushort alignment);
		void operator delete(pointer, ushort alignment);

	protected:
		pointer operator new[](wsize);
		void operator delete[](pointer) ;

	public:

		bool operator == (object const& obj)const {
			return compare(obj) == comparision_result::same;
		}

		bool operator != (object const& obj)const {
			return compare(obj) != comparision_result::same;
		}

		template<typename T>
		auto as() {
			return interface_cast<T>(this);
		}

		template<typename T>
		bool as(T*& out) {
			return interface_cast<T>(this, out);
		}

	};

	/******************************************************************/
	/* template class ang::object_wrapper<object> :                   */
	/*  -> specialization of object_wrapper<object> -> objptr         */
	/******************************************************************/
	template<>
	class LINK object_wrapper<object>
	{
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

		template<wsize N> object_wrapper(const char(&ar)[N]);

		template<wsize N> object_wrapper(const wchar(&ar)[N]);

		inline object_wrapper(cstr_t); //string convertible
		inline object_wrapper(cwstr_t); //string convertible

		inline object_wrapper(bool); //int convertible

		inline object_wrapper(int); //int convertible
		inline object_wrapper(uint); //uint convertible

		inline object_wrapper(long); //long convertible
		inline object_wrapper(ulong); //ulong convertible

		inline object_wrapper(long64); //long convertible
		inline object_wrapper(ulong64); //ulong convertible

		inline object_wrapper(float); //float convertible
		inline object_wrapper(double); //double convertible

		//template<typename T>//object convertible
		//inline object_wrapper(T const&);

		template<typename T>//object convertible
		inline object_wrapper(object_wrapper<T>);

		template<typename T>//array convertible
		inline object_wrapper(initializer_list_t<T>);
	
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

		friend safe_pointer;
	};

	/******************************************************************/
	/* class ang::safe_pointer :                                      */
	/*  -> implements a weak reference to a smart pointer             */
	/******************************************************************/
	class LINK safe_pointer
	{
	public:
		typedef object type;

	private:
		pointer _info;

	public:
		safe_pointer();
		safe_pointer(safe_pointer&&);
		safe_pointer(safe_pointer const&);
		safe_pointer(object*);
		safe_pointer(ang::nullptr_t const&);
		template< typename T>
		safe_pointer(object_wrapper<T> obj) : safe_pointer(obj.get()) {}
		~safe_pointer();

	private:
		void set(object*);
		void clean();

	public: //properties
		bool is_valid()const;
		template< typename T>
		object_wrapper<T> lock() {
			return is_valid() ? lock<object>()->as<T>() : null;
		}

		safe_pointer& operator = (object_t);
		safe_pointer& operator = (object*);
		safe_pointer& operator = (safe_pointer&&);
		safe_pointer& operator = (safe_pointer const&);
		template< typename T>
		safe_pointer& operator = (object_wrapper<T> obj) { return operator = (object_t(obj.get())); }
		safe_pointer& operator = (ang::nullptr_t const&);
	};

	template<> objptr LINK safe_pointer::lock<object>();



	template<typename T>
	class value_wrapper
		: public object
		, public value<T>
	{
	public:
		value_wrapper();
		value_wrapper(T const& val);
		value_wrapper(value<T> const& other);
		value_wrapper(value_wrapper const& other);

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;

	private:
		virtual~value_wrapper();
	};

	template<typename T>
	class object_wrapper<value_wrapper<T>>
	{
	public:
		typedef value_wrapper<T> type;

	private:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(type*);
		object_wrapper(value<T> const&);
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
		object_wrapper& operator = (value<T> const&);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<T> operator & (void);
		T * operator -> (void);
		T const* operator -> (void)const;
		explicit operator T& (void);
		explicit operator T const& (void)const;
		operator type * (void);
		operator type const* (void)const;

		friend safe_pointer;
	};

	template<typename T>
	using wrapper = object_wrapper<value_wrapper<T>>;

	template<class T> 
	using shared_ptr = object_wrapper<value_wrapper<T>>;
}

#include <ang/buffers.h>
#include <ang/collections.h>
#include <ang/collections/array.h>
#include <ang/string.h>
#include <ang/exceptions.h>
#include <ang/boolean.h>
#include <ang/interger.h>
#include <ang/floating.h>
#include <ang/singleton.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#include <ang/inline/object_wrapper.hpp>
#include <ang/inline/intf_wrapper.hpp>
#include <ang/inline/convertible.hpp>
#include <ang/inline/runtime.hpp>
#include <ang/collections/inline/collections.hpp>
#include <ang/collections/inline/array.hpp>

ANG_REGISTER_RUNTIME_TYPENAME(ang::safe_pointer);

#ifdef _MANAGED //Microsoft CLR
namespace ang
{
	namespace interop
	{
		template<class To, class From>
		To string_cast(From);

		template<> inline ang::string string_cast<ang::string>(System::String^ text) {
			// convert .NET System::String to std::string
			const char* cstr = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(text)).ToPointer();
			ang::string sstr = ang::cstr_t(cstr, text->Length);
			System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)cstr));
			return ang::move(sstr);
		}

		template<> inline ang::wstring string_cast<ang::wstring>(System::String^ text) {
			// convert .NET System::String to std::string
			const wchar_t* cstr = (const wchar_t*)(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(text)).ToPointer();
			ang::wstring sstr = ang::cwstr_t(cstr, text->Length);
			System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)cstr));
			return ang::move(sstr);
		}

		template<> inline System::String^ string_cast<System::String^>(ang::cstr_t cstr) {
			return gcnew System::String(cstr.cstr(), 0, cstr.size());
		}

		template<> inline System::String^ string_cast<System::String^>(ang::cwstr_t cstr) {
			return gcnew System::String(cstr.cstr(), 0, cstr.size());
		}

		template<> inline System::String^ string_cast<System::String^>(ang::string text) {
			auto cstr = (ang::cstr_t)text;
			return gcnew System::String(cstr.cstr(), 0, cstr.size());
		}

		template<> inline System::String^ string_cast<System::String^>(ang::wstring text) {
			auto cstr = (ang::cwstr_t)text;
			return gcnew System::String(cstr.cstr(), 0, cstr.size());
		}

	}
}
#endif


#endif //__ANGSYS_H__