/*********************************************************************************************************************/
/*   File Name: ang/base/interface.h                                                                                 */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_INTERFACE_H__
#define __ANG_BASE_INTERFACE_H__

#define pure = 0
#define vcall virtual
#define scall static
#define visible public:
#define inherit protected:
#define intern private:
#define allow	friend

#define ang_object(_NAME) class _NAME; typedef ang::object_wrapper<_NAME> _NAME##_t; typedef ang::object_wrapper_ptr<_NAME> _NAME##_ptr_t;
#define ang_interface(_NAME) struct _NAME; typedef ang::intf_wrapper<_NAME> _NAME##_t; typedef ang::intf_wrapper_ptr<_NAME> _NAME##_ptr_t;

#define ANG_DECLARE_CLASS_INFO() scall ang::rtti_t const& class_info();
#define ANG_DECLARE_RUNTIME_INFO() vcall ang::rtti_t const& runtime_info()const pure;
#define ANG_DECLARE_QUERY_INTERFACE() vcall bool query_interface(ang::rtti_t const&, ang::unknown_ptr_t) pure;

#define ANG_OVERRIDE_CLASS_INFO() scall ang::rtti_t const& class_info();
#define ANG_OVERRIDE_RUNTIME_INFO() vcall ang::rtti_t const& runtime_info()const override;
#define ANG_OVERRIDE_QUERY_INTERFACE() vcall bool query_interface(ang::rtti_t const&, ang::unknown_ptr_t) override;

#define ANG_DECLARE_INTERFACE() visible \
	ANG_OVERRIDE_CLASS_INFO() \
	ANG_OVERRIDE_RUNTIME_INFO() \
	ANG_OVERRIDE_QUERY_INTERFACE()


#define DECLARE_BASE_1(A0) A0 : ang::interface
#define DECLARE_BASE_2(A0, A1) A0 : A1
#define DECLARE_BASE_3(A0, A1, A2) A0 : A1, A2
#define DECLARE_BASE_4(A0, A1, A2, A3) A0 : A1, A2, A3
#define DECLARE_BASE_5(A0, A1, A2, A3, A4) A0 : A1, A2, A3, A4
#define DECLARE_BASE_6(A0, A1, A2, A3, A4, A5) A0 : A1, A2, A3, A4, A5
#define DECLARE_BASE_7(A0, A1, A2, A3, A4, A5, A6) A0 : A1, A2, A3, A4, A5, A6
#define DECLARE_BASE_8(A0, A1, A2, A3, A4, A5, A6, A7) A0 : A1, A2, A3, A4, A5, A6, A7
#define DECLARE_BASE_9(A0, A1, A2, A3, A4, A5, A6, A7, A8) A0 : A1, A2, A3, A4, A5, A6, A7, A8
#define DECLARE_BASE_10(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9) A0 : A1, A2, A3, A4, A5, A6, A7, A8, A9

#ifdef WINDOWS_PLATFORM

#define DECLARE_BASES_(_DECLARE_BASE_, ...) ANG_EXPAND(_DECLARE_BASE_(__VA_ARGS__))

#define __ANG_BEGIN_INTERFACE__(...) ANG_EXPAND(DECLARE_BASES_(ANG_CATAB(DECLARE_BASE_, ANG_EXPAND(ANG_NUM_ARGS(__VA_ARGS__))), __VA_ARGS__))
#define __ANG_BEGIN_INTERFACE(...)	ANG_EXPAND(__ANG_BEGIN_INTERFACE__(__VA_ARGS__)) {
#define ANG_END()	}

#define ang_begin_interface(...) \
struct _DECLSPEC_NOVTABLE __ANG_BEGIN_INTERFACE(__VA_ARGS__) \
	ANG_DECLARE_CLASS_INFO() \
	ANG_DECLARE_RUNTIME_INFO() \
	ANG_DECLARE_QUERY_INTERFACE()

#define ang_begin_interface_inline(...) \
struct _DECLSPEC_NOVTABLE __ANG_BEGIN_INTERFACE(__VA_ARGS__) \
	inline ANG_DECLARE_CLASS_INFO() \
	inline ANG_DECLARE_RUNTIME_INFO() \
	inline ANG_DECLARE_QUERY_INTERFACE()

#define ang_end_interface() ANG_END()

#define ANG_IMPLEMENT_INTERFACE_CLASS_INFO(_CLASS, ...) ANG_EXPAND(ANG_IMPLEMENT_INTERFACE_CLASS_INFO_(_CLASS, __VA_ARGS__))
#define ANG_IMPLEMENT_INTERFACE_CLASS_INFO_(_CLASS, ...) \
ang::rtti_t const& _CLASS::class_info() { \
	static const char name[] = #_CLASS; \
	static rtti_t const* parents[] = ANG_EXPAND(TYPE_OF_PTR_ARRAY(__VA_ARGS__)); \
	static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<_CLASS>(), align_of<wsize>(), parents, &default_query_interface); \
	return info; \
 }
#define ANG_IMPLEMENT_INTERFACE_QUERY_INTERFACE(_CLASS, ...) \
bool _CLASS::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out) { ANG_EXPAND(QUERY_INTERFACE_SWITCH(_CLASS, __VA_ARGS__)) return false; }

#define ANG_IMPLEMENT_OBJECT_CLASS_INFO(_CLASS, ...) ANG_EXPAND(ANG_IMPLEMENT_OBJECT_CLASS_INFO_(_CLASS, __VA_ARGS__))
#define ANG_IMPLEMENT_OBJECT_CLASS_INFO_(_CLASS, ...) \
ang::rtti_t const& _CLASS::class_info() { \
	static const char name[] = #_CLASS; \
	static rtti_t const* parents[] = ANG_EXPAND(TYPE_OF_PTR_ARRAY(__VA_ARGS__)); \
	static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<_CLASS>(), align_of<_CLASS>(), parents, &default_query_interface); \
	return info; \
 }

#define ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(_CLASS) ang::rtti_t const& _CLASS::runtime_info()const { return _CLASS::class_info();  }
#define ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(_CLASS, _BASE, ...) \
bool _CLASS::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out) { \
	if(id.type_id() == _CLASS::class_info().type_id()) { if (out == null) return false; *out = static_cast<_CLASS*>(this); return true; } \
	else if(_BASE::query_interface(id,out )) { return true; } else ANG_EXPAND(QUERY_INTERFACE_SWITCH(__VA_ARGS__)) return false; }

#elif defined ANDROID_PLATFORM

#define DECLARE_BASES_(_DECLARE_BASE_, ...) _DECLARE_BASE_(__VA_ARGS__)

#define __ANG_BEGIN_INTERFACE__(...) DECLARE_BASES_(ANG_CATAB(DECLARE_BASE_, ANG_NUM_ARGS(__VA_ARGS__)), __VA_ARGS__)
#define __ANG_BEGIN_INTERFACE(...)	__ANG_BEGIN_INTERFACE__(__VA_ARGS__) {
#define ANG_END()	}

#define ang_begin_interface(...) \
struct _DECLSPEC_NOVTABLE __ANG_BEGIN_INTERFACE(__VA_ARGS__) \
	ANG_DECLARE_CLASS_INFO() \
	ANG_DECLARE_RUNTIME_INFO() \
	ANG_DECLARE_QUERY_INTERFACE()

#define ang_begin_interface_inline(...) \
struct _DECLSPEC_NOVTABLE __ANG_BEGIN_INTERFACE(__VA_ARGS__) \
	inline ANG_DECLARE_CLASS_INFO() \
	inline ANG_DECLARE_RUNTIME_INFO() \
	inline ANG_DECLARE_QUERY_INTERFACE()

#define ang_end_interface() ANG_END()

#define ANG_IMPLEMENT_INTERFACE_CLASS_INFO(_CLASS, ...) ANG_EXPAND(ANG_IMPLEMENT_INTERFACE_CLASS_INFO_(_CLASS, __VA_ARGS__))
#define ANG_IMPLEMENT_INTERFACE_CLASS_INFO_(_CLASS, ...) \
ang::rtti_t const& _CLASS::class_info() { \
	static const char name[] = #_CLASS; \
	static rtti_t const* parents[] = TYPE_OF_PTR_ARRAY(__VA_ARGS__); \
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(_CLASS), alignof(wsize), parents, &default_query_interface); \
	return info; \
 }
#define ANG_IMPLEMENT_INTERFACE_QUERY_INTERFACE(_CLASS, ...) \
bool _CLASS::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out) { QUERY_INTERFACE_SWITCH(_CLASS, __VA_ARGS__) return false; }

#define ANG_IMPLEMENT_OBJECT_CLASS_INFO(_CLASS, ...) ANG_IMPLEMENT_OBJECT_CLASS_INFO_(_CLASS, __VA_ARGS__)
#define ANG_IMPLEMENT_OBJECT_CLASS_INFO_(_CLASS, ...) \
ang::rtti_t const& _CLASS::class_info() { \
	static const char name[] = #_CLASS; \
	static rtti_t const* parents[] = TYPE_OF_PTR_ARRAY(__VA_ARGS__); \
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(_CLASS), alignof(_CLASS), parents, &default_query_interface); \
	return info; \
 }

#define ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(_CLASS) ang::rtti_t const& _CLASS::runtime_info()const { return _CLASS::class_info();  }
#define ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(_CLASS, _BASE, ...) \
bool _CLASS::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out) { \
	if(id.type_id() == _CLASS::class_info().type_id()) { if (out == null) return false; *out = static_cast<_CLASS*>(this); return true; } \
	else if(_BASE::query_interface(id,out )) { return true; } else QUERY_INTERFACE_SWITCH(__VA_ARGS__) return false; }


#endif // WINDOWS_PLATFORM

namespace ang //constants
{
	template<typename T> class intf_wrapper;
	template<typename T> class intf_wrapper_ptr;
	template<typename T> class object_wrapper;
	template<typename T> class object_wrapper_ptr;

	ang_interface(interface);
	typedef interface_t intfptr;

	struct _DECLSPEC_NOVTABLE LINK interface {
		visible scall bool default_query_interface(rtti_t const&, unknown_t, rtti_t const&, unknown_ptr_t);
		visible scall rtti_t const& class_info();
		visible vcall rtti_t const& runtime_info()const pure;
		visible vcall bool query_interface(rtti_t const&, unknown_ptr_t)pure;
	};

	template<typename T, typename = void> struct has_class_info_function : false_type {};
	template<typename T> struct has_class_info_function<T, void_t<decltype(T::class_info())>> : true_type {};

	template<typename T> struct is_interface : integer_constant<bool,
		is_same_type<T, interface>::value ||
		is_inherited_from<T, interface>::value> {};

	

	namespace runtime
	{
		
		template<typename T> struct type_info_builder<intf_wrapper<T>, genre::class_type> : type_info_builder<T> { };
		template<typename T> struct type_info_builder<object_wrapper<T>, genre::class_type> : type_info_builder<T> { };
		
	}
}

#endif//__ANG_BASE_INTERFACE_H__
