/*********************************************************************************************************************/
/*   File Name: ang/interface.h                                                                                      */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is declare useful macros for implementation for interfaces. They are the base of    */
/*   the dynamic object conversions.                                                                                 */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_INTERFACE_H__
#define __ANG_INTERFACE_H__

#define intern	private:
#define inherit protected:
#define visible	public:
#define permit	friend
#define pure = 0
#define vcall virtual
#define scall static
#define ang_interface struct
#define ANG_INTERFACE(_NAME) ang_interface _NAME; typedef ang::intf_wrapper<_NAME> _NAME##_t

#define ANG_DECLARE_CLASSNAME() static ang::type_name_t class_name();
#define ANG_DECLARE_ISCHILDOF() static bool is_child_of(ang::type_name_t);
#define ANG_DECLARE_DYNAMICTYPE() static ang::intfptr dynamic_constructor(); static bool dynamic_destructor(ang::intfptr&);
#define ANG_DECLARE_DYNAMICTYPE_ARGS(...) static ang::intfptr dynamic_constructor(__VA_ARGS__); static bool dynamic_destructor(ang::intfptr&);
#define ANG_DECLARE_INLINE_DYNAMICTYPE() inline static ang::intfptr dynamic_constructor(); inline static bool dynamic_destructor(ang::intfptr&);

#define ANG_DECLARE_OBJECTNAME() virtual ang::type_name_t object_name()const;
#define ANG_DECLARE_ISKINDOF() virtual bool is_kind_of(ang::type_name_t)const;
#define ANG_DECLARE_QUERYOBJECT() virtual bool query_object(ang::type_name_t, ang::unknown_ptr_t);


#define ANG_OVERRIDE_CLASSNAME() static ang::type_name_t class_name();
#define ANG_OVERRIDE_ISCHILDOF() static bool is_child_of(ang::type_name_t);
#define ANG_OVERRIDE_DYNAMICTYPE() static ang::intfptr dynamic_constructor(); static bool dynamic_destructor(ang::intfptr&);
#define ANG_OVERRIDE_DYNAMICTYPE_ARGS(...) static ang::intfptr dynamic_constructor(__VA_ARGS__); static bool dynamic_destructor(ang::intfptr&);
#define ANG_OVERRIDE_INLINE_DYNAMICTYPE() inline static ang::intfptr dynamic_constructor(); inline static bool dynamic_destructor(ang::intfptr&);

#define ANG_OVERRIDE_OBJECTNAME() virtual ang::type_name_t object_name()const override;
#define ANG_OVERRIDE_ISKINDOF() virtual bool is_kind_of(ang::type_name_t)const override;
#define ANG_OVERRIDE_QUERYOBJECT() virtual bool query_object(ang::type_name_t, ang::unknown_ptr_t) override;

#define ANG_BEGIN()	{
#define ANG_END()	};

#define ANG_BEGIN_INTERFACE(_LINK, _CLASS) \
	ang_interface _LINK _CLASS ANG_BEGIN() \
		ANG_DECLARE_CLASSNAME() \
		ANG_DECLARE_ISCHILDOF() \
		ANG_DECLARE_OBJECTNAME() \
		ANG_DECLARE_ISKINDOF() \
		ANG_DECLARE_QUERYOBJECT()

#define ANG_BEGIN_INLINE_INTERFACE(_CLASS) \
	ang_interface _CLASS ANG_BEGIN() \
		inline ANG_DECLARE_CLASSNAME() \
		inline ANG_DECLARE_ISCHILDOF() \
		inline ANG_DECLARE_OBJECTNAME() \
		inline ANG_DECLARE_ISKINDOF() \
		inline ANG_DECLARE_QUERYOBJECT()

#define ANG_BEGIN_INTERFACE_SPECIALIZATION(_CLASS, ...) \
	ang_interface _CLASS<__VA_ARGS__> ANG_BEGIN() \
		inline ANG_DECLARE_CLASSNAME() \
		inline ANG_DECLARE_ISCHILDOF() \
		inline ANG_DECLARE_OBJECTNAME() \
		inline ANG_DECLARE_ISKINDOF() \
		inline ANG_DECLARE_QUERYOBJECT()

#define ANG_BEGIN_INTERFACE_FULL_SPECIALIZATION(_LINK, _CLASS, ...) \
	ang_interface _LINK _CLASS<__VA_ARGS__> ANG_BEGIN() \
		ANG_DECLARE_CLASSNAME() \
		ANG_DECLARE_ISCHILDOF() \
		ANG_DECLARE_OBJECTNAME() \
		ANG_DECLARE_ISKINDOF() \
		ANG_DECLARE_QUERYOBJECT()

#define ANG_BEGIN_INTERFACE_WITH_BASE(_LINK, _CLASS, ...) \
	ang_interface _LINK _CLASS : __VA_ARGS__ ANG_BEGIN() \
		ANG_DECLARE_CLASSNAME() \
		ANG_DECLARE_ISCHILDOF() \
		ANG_DECLARE_OBJECTNAME() \
		ANG_DECLARE_ISKINDOF() \
		ANG_DECLARE_QUERYOBJECT()

#define ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(_CLASS, ...) \
	ang_interface _CLASS : __VA_ARGS__ ANG_BEGIN() \
		inline ANG_DECLARE_CLASSNAME() \
		inline ANG_DECLARE_ISCHILDOF() \
		inline ANG_DECLARE_OBJECTNAME() \
		inline ANG_DECLARE_ISKINDOF() \
		inline ANG_DECLARE_QUERYOBJECT()

#define ANG_END_INTERFACE()	ANG_END()

#define ANG_DECLARE_INTERFACE() visible \
	ANG_OVERRIDE_CLASSNAME() \
	ANG_OVERRIDE_ISCHILDOF() \
	ANG_OVERRIDE_OBJECTNAME() \
	ANG_OVERRIDE_ISKINDOF() \
	ANG_OVERRIDE_QUERYOBJECT()

#define ANG_DECLARE_INLINE_INTERFACE() visible \
	inline ANG_OVERRIDE_CLASSNAME() \
	inline ANG_OVERRIDE_ISCHILDOF() \
	inline ANG_OVERRIDE_OBJECTNAME() \
	inline ANG_OVERRIDE_ISKINDOF() \
	inline ANG_OVERRIDE_QUERYOBJECT()

#define ANG_DECLARE_DYNAMIC_INTERFACE() visible \
	ANG_DECLARE_DYNAMICTYPE() \
	ANG_OVERRIDE_CLASSNAME() \
	ANG_OVERRIDE_ISCHILDOF() \
	ANG_OVERRIDE_OBJECTNAME() \
	ANG_OVERRIDE_ISKINDOF() \
	ANG_OVERRIDE_QUERYOBJECT()

#define ANG_DECLARE_DYNAMIC_INTERFACE_ARGS(...) visible \
	ANG_DECLARE_DYNAMICTYPE_ARGS(__VA_ARGS__) \
	ANG_OVERRIDE_CLASSNAME() \
	ANG_OVERRIDE_ISCHILDOF() \
	ANG_OVERRIDE_OBJECTNAME() \
	ANG_OVERRIDE_ISKINDOF() \
	ANG_OVERRIDE_QUERYOBJECT()


#define ANG_IMPLEMENT_CLASSNAME(_CLASS) ang::type_name_t _CLASS::class_name(){ return #_CLASS; }
#define ANG_IMPLEMENT_ISCHILDOF(_CLASS) bool _CLASS::is_child_of(ang::type_name_t name) { if (name == ang::type_name<_CLASS>())return true; return false; }
#define ANG_IMPLEMENT_ISCHILDOF_BASE(_CLASS, _BASE) bool _CLASS::is_child_of(ang::type_name_t name) { if (name == ang::type_name<_CLASS>())return true; return _BASE::is_child_of(name); }
#define ANG_IMPLEMENT_DYNAMICTYPE(_CLASS) ang::intfptr _CLASS::dynamic_constructor() { return reinterpret_cast<ang::interface_t*>(new _CLASS()); } \
									   bool _CLASS::dynamic_destructor(ang::intfptr& intf){ if(!intf->is_kind_of(type_name<_CLASS>()))return false; intf = null; return true; }
#define ANG_IMPLEMENT_OBJECTNAME(_CLASS) ang::type_name_t _CLASS::object_name()const{ return class_name(); }
#define ANG_IMPLEMENT_ISKINDOF(_CLASS) bool _CLASS::is_kind_of(ang::type_name_t name)const{ if (name == ang::type_name<_CLASS>())return true; return false; }
#define ANG_IMPLEMENT_ISKINDOF_BASE(_CLASS, _BASE) bool _CLASS::is_kind_of(ang::type_name_t name)const{ if (name == ang::type_name<_CLASS>())return true; return _BASE::is_kind_of(name); }
#define ANG_IMPLEMENT_QUERYOBJECT(_CLASS) bool _CLASS::query_object(ang::type_name_t name, ang::unknown_ptr_t out){ \
		if(out == null) return false; \
		if(name == ang::type_name<_CLASS>()){ *out = static_cast<_CLASS*>(this); return true; }return false; }
#define ANG_IMPLEMENT_QUERYOBJECT_BASE(_CLASS, _BASE) bool _CLASS::query_object(ang::type_name_t name, ang::unknown_ptr_t out) {\
		if(out == null) return false; \
		if(name == ang::type_name<_CLASS>()){ *out = static_cast<_CLASS*>(this); return true; }\
		else if(_BASE::query_object(name, out)) return true; return false; }

#define ANG_IMPLEMENT_DYNAMICTYPE_INTERFACE(_CLASS) ang::intfptr _CLASS::dynamic_constructor(){ return null; } bool _CLASS::dynamic_destructor(ang::intfptr &){ return false; }

#define ANG_IMPLEMENT_TEMPLATE_CLASSNAME(_CLASS) template<> inline ANG_IMPLEMENT_CLASSNAME(_CLASS)
#define ANG_IMPLEMENT_TEMPLATE_CLASSNAME_ARGS(_CLASS, ...) template<> inline ANG_IMPLEMENT_CLASSNAME(_CLASS<__VA_ARGS__>)

#define ANG_IMPLEMENT_TEMPLATE_DYNAMICTYPE(_CLASS) template<> inline ANG_IMPLEMENT_DYNAMICTYPE(_CLASS)
#define ANG_IMPLEMENT_TEMPLATE_DYNAMICTYPE_ARGS(_CLASS, ...) template<> inline ANG_IMPLEMENT_DYNAMICTYPE(_CLASS<__VA_ARGS__>)

#define ANG_IMPLEMENT_BASIC_INTERFACE(_CLASS, _BASE) \
			ANG_IMPLEMENT_CLASSNAME(_CLASS); \
			ANG_IMPLEMENT_ISCHILDOF_BASE(_CLASS, _BASE) \
			ANG_IMPLEMENT_OBJECTNAME(_CLASS); \
			ANG_IMPLEMENT_ISKINDOF_BASE(_CLASS, _BASE) \
			ANG_IMPLEMENT_QUERYOBJECT_BASE(_CLASS, _BASE)

#define ANG_IMPLEMENT_BASIC_DYNAMIC_INTERFACE(_CLASS, _BASE) \
			ANG_IMPLEMENT_DYNAMICTYPE_INTERFACE(_CLASS) \
			ANG_IMPLEMENT_CLASSNAME(_CLASS); \
			ANG_IMPLEMENT_ISCHILDOF_BASE(_CLASS, _BASE) \
			ANG_IMPLEMENT_OBJECTNAME(_CLASS); \
			ANG_IMPLEMENT_ISKINDOF_BASE(_CLASS, _BASE) \
			ANG_IMPLEMENT_QUERYOBJECT_BASE(_CLASS, _BASE)

#define ANG_IMPLEMENT_INTERFACE(_NAMESPACE,_CLASS) \
	ANG_IMPLEMENT_CLASSNAME(_NAMESPACE::_CLASS) \
	ANG_IMPLEMENT_ISCHILDOF(_NAMESPACE::_CLASS) \
	ANG_IMPLEMENT_OBJECTNAME(_NAMESPACE::_CLASS) \
	ANG_IMPLEMENT_ISKINDOF(_NAMESPACE::_CLASS) \
	ANG_IMPLEMENT_QUERYOBJECT(_NAMESPACE::_CLASS)

#endif//__ANGSYS_H__