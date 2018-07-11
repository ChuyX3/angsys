/*********************************************************************************************************************/
/*   File Name: angsys.hpp                                                                                             */
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

#include <angbase.hpp>
#include <ang/wrapper_specialization_definition.hpp>

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
	ANG_BEGIN_ENUM(LINK, comparision_result, int)
		minor = -1,
		same = 0,
		mayor = 1,
		diferent = 20
	ANG_END_ENUM(comparision_result);

	/******************************************************************/
	/* interface ang::interface_t :                                   */
	/*  -> implements basic methods for dynamic naming and conversion */
	/******************************************************************/
	typedef struct LINK _interface
	{
		visible static type_name_t class_name();
		visible static bool is_inherited_of(type_name_t);
		visible virtual type_name_t object_name()const pure;
		visible virtual bool is_kind_of(type_name_t)const pure;
		visible virtual bool query_object(type_name_t, unknown_ptr_t) pure;
	}interface_t, *interface_ptr_t;

	typedef intf_wrapper<interface_t> intfptr;

	/******************************************************************/
	/* interface ang::iobject :                                       */
	/*  -> implements basic methods for smart pointer management      */
	/******************************************************************/
	ANG_BEGIN_INTERFACE(LINK, iobject)
		visible vcall dword add_ref() pure;
		visible vcall dword release() pure;
	ANG_END_INTERFACE();


	class exception;
	typedef object_wrapper<exception> exception_t;

	template<typename T> class value_wrapper;
	template<typename T> using wrapper = object_wrapper<value_wrapper<T>>;
	template<class T> using shared_ptr = object_wrapper<value_wrapper<T>>;
	template<typename T, typename... Ts> shared_ptr<T> make_shared(Ts const& ... args) {
		return new value_wrapper<T>(args...);
	}

	template<bool VALUE, typename T>
	struct rule<VALUE, object_wrapper<T>> {
		static_assert(VALUE, "T is not inherited from object class...");
	};

	template<bool VALUE, typename T>
	struct rule<VALUE, intf_wrapper<T>> {
		static_assert(VALUE, "T is not an interface type...");
	};

	template<typename T> struct is_object_type
		: public integer_constant<bool, or_expression<is_same<object, T>::value, is_base_of<object, T>::value>::value> {
	};

	template<typename T> struct is_interface_type
		: public integer_constant<bool, and_expression<has_runtime_type_info<T>::value, not_expression<is_object_type<T>::value>::value>::value> {
	};

	template<typename T, bool IS_OBJECT = is_object_type<T>::value, bool IS_INTERFACE = is_interface_type<T>::value>
	struct is_smart_ptr_type : public false_type {};
	template<typename T> struct is_smart_ptr_type<T, true, false> : public true_type {};
	template<typename T> struct is_smart_ptr_type<T, false, true> : public true_type {};


	template<typename T, bool IS_OBJECT = is_object_type<T>::value, bool IS_INTERFACE = is_interface_type<T>::value> struct smart_ptr_type
	{
		static_assert(is_smart_ptr_type<T>::value, "T is not a smart type");  typedef T* smart_ptr_t;
	};
	template<typename T> struct smart_ptr_type<T, true, false> { typedef object_wrapper<T> smart_ptr_t; typedef typename smart_ptr_t::type type; };
	template<typename T> struct smart_ptr_type<T, false, true> { typedef intf_wrapper<T> smart_ptr_t; typedef typename smart_ptr_t::type type; };

	template<typename T> struct smart_ptr_type<object_wrapper<T>, false, false> { typedef object_wrapper<T> smart_ptr_t; typedef typename smart_ptr_t::type type; };
	template<typename T> struct smart_ptr_type<intf_wrapper<T>, false, false> { typedef intf_wrapper<T> smart_ptr_t; typedef typename smart_ptr_t::type type; };

	template<typename T> struct smart_ptr_type<T, false, false> { typedef shared_ptr<T> smart_ptr_t; typedef typename smart_ptr_t::type type; };

	/******************************************************************/
	/* template class ang::object_wrapper :                           */
	/*  -> implements handling of smart pointers                      */
	/******************************************************************/
	template<typename T>
	class object_wrapper : rule<is_object_type<T>::value, object_wrapper<T>>
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

		friend class safe_pointer;
	};

	template<typename T>
	struct constructor<object_wrapper<T>> {
		static const object_wrapper<T> default_value;
	};

	template<typename T> const object_wrapper<T> constructor<object_wrapper<T>>::default_value = nullptr;

	template<typename T1, typename T2>
	bool operator == (object_wrapper<T1> const& obj1, object_wrapper<T2> const& obj2) {
		if ((pointer)obj1.get() == (pointer)obj2.get())
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

	/******************************************************************/
	/* template class ang::intf_wrapper :                             */
	/*  -> implements handling of smart pointers for intercafes       */
	/******************************************************************/

	template<class T> class intf_wrapper : rule<is_interface_type<T>::value, intf_wrapper<T>>
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


	template<typename T>
	struct constructor<intf_wrapper<T>> {
		static const intf_wrapper<T> default_value;
	};

	template<typename T> const intf_wrapper<T> constructor<intf_wrapper<T>>::default_value = nullptr;

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


	/******************************************************************/
	/* template class ang::object_wrapper<object> :                   */
	/*  -> specialization of object_wrapper<object> -> objptr         */
	/******************************************************************/
	template<> class LINK object_wrapper<object>
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

		template<typename T>
		inline object_wrapper(safe_str<T> const& str); //string convertible

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

		template<typename T> typename smart_ptr_type<T>::smart_ptr_t as();

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

		template<typename T>
		explicit operator T& () {
			return (T&)as<T>();
		}

		template<typename T>
		explicit operator T const& ()const {
			return (T const&)const_cast<objptr*>(this)->as<T>();
		}

		friend class safe_pointer;
	};

	template<> class LINK intf_wrapper<interface_t>
	{
	public:
		typedef interface_t type;
	protected:
		interface_t* _ptr;

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

		template<typename T> typename smart_ptr_type<T>::smart_ptr_t as();

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

#include <ang/istream.hpp>
#include <ang/ibuffer.hpp>
#include <ang/collections.hpp>

namespace ang
{
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
		virtual string to_string()const;
		virtual wsize serialize(streams::itext_output_stream_t)const;
		virtual wsize serialize(streams::ibinary_output_stream_t)const;
		virtual wsize deserialize(streams::itext_input_stream_t);
		virtual wsize deserialize(streams::ibinary_input_stream_t);

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
		pointer operator new[](wsize)noexcept;
		void operator delete[](pointer)noexcept;

	public:

		bool operator == (object const& obj)const;

		bool operator != (object const& obj)const;

		template<typename T>
		typename smart_ptr_type<T>::smart_ptr_t as();

		template<typename T>
		bool as(T*& out);

	};


	/******************************************************************/
	/* class ang::safe_pointer :                                      */
	/*  -> implements a weak reference to a smart pointer             */
	/******************************************************************/
	class LINK safe_pointer
	{
	public:
		typedef interface_t type;

	private:
		pointer _info;
		wsize _offset;

	public:
		safe_pointer();
		safe_pointer(safe_pointer&&);
		safe_pointer(safe_pointer const&);
		safe_pointer(interface_t*);
		safe_pointer(ang::nullptr_t const&);
		template< typename T>
		safe_pointer(object_wrapper<T> obj) : safe_pointer(reinterpret_cast<interface_t*>(obj.get())) {}
		template< typename T>
		safe_pointer(intf_wrapper<T> intf) : safe_pointer(reinterpret_cast<interface_t*>(intf.get())) {}

		template< typename T>
		safe_pointer(T* ptr) : safe_pointer(reinterpret_cast<interface_t*>(ptr)) {
			static_assert(is_smart_ptr_type<T>::value, "T is not samrt pointer type");
		}
		~safe_pointer();

	private:
		template<typename T> void set(T* ptr) {
			static_assert(is_smart_ptr_type<T>::value, "T is not samrt pointer type");
			set((interface_t*) ptr);
		}
		void set(interface_t*);
		void clean();

	public: //properties
		bool is_valid()const;
		template<typename T>
		typename smart_ptr_type<T>::smart_ptr_t lock()const {
			return lock<intfptr>().as<T>();
		}
		template<typename T>
		safe_pointer& operator = (T* ptr) {
			static_assert(is_smart_ptr_type<T>::value, "T is not samrt pointer type");
			return operator = ((interface_t*)ptr);
		}
		template< typename T> safe_pointer& operator = (intf_wrapper<T> obj) { return operator = ((interface_t*)obj.get()); }
		template< typename T> safe_pointer& operator = (object_wrapper<T> obj) { return operator = ((interface_t*)obj.get()); }
		safe_pointer& operator = (interface_t*);
		safe_pointer& operator = (safe_pointer&&);
		safe_pointer& operator = (safe_pointer const&);
		safe_pointer& operator = (ang::nullptr_t const&);
	};

	template<> intfptr LINK safe_pointer::lock<intfptr>()const;

	template<class T>
	class weak_ptr : public safe_pointer
	{
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

#include <ang/runtime.hpp>
#include <ang/buffers.hpp>
#include <ang/collections/array.hpp>

#include <ang/string.hpp>
#include <ang/value.hpp>
#include <ang/exceptions.hpp>
#include <ang/singleton.hpp>

#ifdef  LINK
#undef  LINK
#endif//LINK

#include <ang/inline/object_wrapper.inl>
#include <ang/inline/intf_wrapper.inl>
#include <ang/inline/convertible.inl>
#include <ang/inline/runtime.inl>
#include <ang/collections/inline/collections.inl>
#include <ang/collections/inline/array.inl>
#include <ang/inline/string.inl>

ANG_REGISTER_RUNTIME_TYPENAME(ang::safe_pointer);

namespace ang
{

	inline bool object::operator == (object const& obj)const {
		return compare(obj) == comparision_result::same;
	}

	inline bool object::operator != (object const& obj)const {
		return compare(obj) != comparision_result::same;
	}

	template<typename T>
	inline typename smart_ptr_type<T>::smart_ptr_t object::as() {
		return interface_cast<typename smart_ptr_type<T>::type>(this);
	}

	template<typename T>
	inline bool object::as(T*& out) {
		return interface_cast<typename smart_ptr_type<T>::type>(this, out);
	}

	template<typename T>
	inline typename smart_ptr_type<T>::smart_ptr_t objptr::as() {
		return interface_cast<typename smart_ptr_type<T>::type>(get());
	}

	template<typename T>
	inline typename smart_ptr_type<T>::smart_ptr_t intfptr::as() {
		return interface_cast<typename smart_ptr_type<T>::type>(get());
	}

	namespace interop
	{
		template<typename To, typename From>
		struct __string_cast_impl {
			static To string_cast(From const& str) {
				return srt;
			}
		};

		template<typename To, typename From>
		To string_cast(From const& str) {
			return ang::move(__string_cast_impl<To, From>::string_cast(str));
		}

#ifdef _MANAGED //Microsoft CLR
		template<> inline ang::string string_cast<ang::string>(System::String^ const& text) {
			// convert .NET System::String to std::string
			const char* cstr = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(text)).ToPointer();
			ang::string sstr = ang::cstr_t(cstr, text->Length);
			System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)cstr));
			return ang::move(sstr);
		}

		template<> inline ang::wstring string_cast<ang::wstring>(System::String^ const& text) {
			// convert .NET System::String to std::string
			const wchar_t* cstr = (const wchar_t*)(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(text)).ToPointer();
			ang::wstring sstr = ang::cwstr_t(cstr, text->Length);
			System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)cstr));
			return ang::move(sstr);
		}

		template<> inline System::String^ string_cast<System::String^>(ang::cstr_t const& cstr) {
			return gcnew System::String(cstr.cstr(), 0, cstr.size());
		}

		template<> inline System::String^ string_cast<System::String^>(ang::cwstr_t const& cstr) {
			return gcnew System::String(cstr.cstr(), 0, cstr.size());
		}

		template<> inline System::String^ string_cast<System::String^>(ang::string const& text) {
			auto cstr = (ang::cstr_t)text;
			return gcnew System::String(cstr.cstr(), 0, cstr.size());
		}

		template<> inline System::String^ string_cast<System::String^>(ang::wstring const& text) {
			auto cstr = (ang::cwstr_t)text;
			return gcnew System::String(cstr.cstr(), 0, cstr.size());
		}
#endif

#ifdef _WINRT //Windows Runtime

		template<text::encoding_enum ENCODING>
		struct __string_cast_impl<strings::string_base<ENCODING>, Platform::String^> {
			static strings::string_base<ENCODING> string_cast(Platform::String^ const& str) {
				return cwstr_t(str->Data(), str->Length());
			}
		};

		template<text::encoding_enum ENCODING>
		struct __string_cast_impl<Platform::String^, strings::string_base<ENCODING>> {
			static Platform::String^ string_cast(strings::string_base<ENCODING> const& str) {
				wstring wstr = str;
				return ref new Platform::String(wstr->cstr());
			}
		};

		template<typename T>
		struct __string_cast_impl<Platform::String^, safe_str<T>> {
			static Platform::String^ string_cast(safe_str<T> const& str) {
				wstring wstr = str;
				return ref new Platform::String(wstr->cstr());
			}
		};

#endif
	}
}



#endif //__ANGSYS_HPP__