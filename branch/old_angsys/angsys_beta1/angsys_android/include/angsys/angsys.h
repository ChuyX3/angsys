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

#include <ang/wrappers.h>

namespace ang
{
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

	private:
		pointer operator new[](wsize)throw();
		void operator delete[](pointer)throw();

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


	template<class T>
	class weak_ptr : public safe_pointer
	{
	public:
		weak_ptr() : safe_pointer() {}
		weak_ptr(weak_ptr&& other) : safe_pointer((safe_pointer&&)other) {}
		weak_ptr(weak_ptr const& other) : safe_pointer((safe_pointer const&)other) {}
		weak_ptr(ang::nullptr_t const&) : safe_pointer(null) {}
		weak_ptr(T* obj) : safe_pointer(obj) {}
		weak_ptr(object_wrapper<T> obj) : safe_pointer(obj.get()) {}
		~weak_ptr() {}

	public: //properties
		object_wrapper<T> lock() {
			auto _obj = safe_pointer::lock<object>();
			return static_cast<T*>(_obj.get());
		}

		weak_ptr& operator = (object_wrapper<T> obj) { safe_pointer::operator=(obj.get()); return *this; }
		weak_ptr& operator = (T* obj) { safe_pointer::operator=(obj);  return *this; }
		weak_ptr& operator = (weak_ptr&& other) { safe_pointer::operator=(other); return *this; }
		weak_ptr& operator = (weak_ptr const& other) { safe_pointer::operator=(other);  return *this; }
		weak_ptr& operator = (ang::nullptr_t const&) { safe_pointer::operator=(null); return *this; }
	};


	template<typename T1, typename T2>
	bool operator == (object_wrapper<T1> const& obj1, object_wrapper<T2> const& obj2) {
		if (obj1.get() == obj2.get())
			return true;
		if ((!obj1.get() && obj2.get()) || (obj1.get() && !obj2.get()))
			return false;
		return obj1.get()->compare(*obj2.get()) == comparision_result::same;
	}
	template<typename T>
	bool operator != (object_wrapper<T> const& obj1, object_wrapper<T> const& obj2) {
		if (obj1.get() == obj2.get())
			return true;
		if ((!obj1.get() && obj2.get()) || (obj1.get() && !obj2.get()))
			return false;
		return obj1.get()->compare(*obj2.get()) != comparision_result::same;
	}

}

#include <ang/buffers.h>
#include <ang/collections.h>
#include <ang/collections/array.h>
#include <ang/string.h>
#include <ang/exceptions.h>

#ifdef LINK
#undef LINK
#endif//LINK

#include <ang/singleton.h>
#include <ang/inline/intf_wrapper.inl>
#include <ang/inline/object_wrapper.inl>
#include <ang/collections/inline/collections.inl>
#include <ang/inline/runtime.inl>
#include <ang/collections/inline/array.inl>

#endif//__ANGSYS_H__