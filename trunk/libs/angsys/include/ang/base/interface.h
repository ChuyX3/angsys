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

#define ang_declare_object(_NAME) class _NAME; typedef ang::object_wrapper<_NAME> _NAME##_t; typedef ang::object_wrapper_ptr<_NAME> _NAME##_ptr_t;
#define ang_declare_interface(_NAME) struct _NAME; typedef ang::intf_wrapper<_NAME> _NAME##_t; typedef ang::intf_wrapper_ptr<_NAME> _NAME##_ptr_t;

#define ANG_DECLARE_CLASS_INFO() static ang::rtti_t const& class_info();
#define ANG_DECLARE_RUNTIME_INFO() virtual ang::rtti_t const& runtime_info()const = 0
#define ANG_DECLARE_QUERY_INTERFACE() virtual bool query_interface(ang::rtti_t const&, ang::unknown_ptr_t) = 0

#define ANG_OVERRIDE_CLASS_INFO() static ang::rtti_t const& class_info();
#define ANG_OVERRIDE_RUNTIME_INFO() virtual ang::rtti_t const& runtime_info()const override;
#define ANG_OVERRIDE_QUERY_INTERFACE() virtual bool query_interface(ang::rtti_t const&, ang::unknown_ptr_t) override;

#define ANG_DECLARE_INTERFACE() public: \
	ANG_OVERRIDE_CLASS_INFO() \
	ANG_OVERRIDE_RUNTIME_INFO() \
	ANG_OVERRIDE_QUERY_INTERFACE()

#define ang_interface struct nvt

#define ANG_IMPLEMENT_INTERFACE_CLASS_INFO(_CLASS) \
ang::rtti_t const& ang::intf_class_info<_CLASS>::class_info() { \
	static inherit_pack_info_t parents = ang::rtti_from_type<_CLASS>::types(); \
	static type_name_t name = #_CLASS; \
	static rtti_t const& info = rtti::regist(name, gender::class_type, size_of<_CLASS>(), align_of<wsize>(), parents, &iintf::default_query_interface); \
	return info; \
 }

#define ANG_IMPLEMENT_OBJECT_CLASS_INFO(_CLASS) \
ang::rtti_t const& ang::intf_class_info<_CLASS>::class_info() { \
	static inherit_pack_info_t parents = ang::rtti_from_type<_CLASS>::types(); \
	static type_name_t name = #_CLASS; \
	static rtti_t const& info = rtti::regist(name, gender::class_type, size_of<_CLASS>(), align_of<_CLASS>(), parents, &iintf::default_query_interface); \
	return info; \
 }

#define ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(_CLASS, _NAME) \
ang::rtti_t const& ang::intf_class_info<_CLASS>::class_info() { \
	static inherit_pack_info_t parents = ang::rtti_from_type<_CLASS>::types(); \
	static rtti_t const& info = rtti::regist(_NAME, gender::class_type, size_of<_CLASS>(), align_of<_CLASS>(), parents, &iintf::default_query_interface); \
	return info; \
 }

#define ANG_DECLARE_CLASS_INFO_OVERRIDE(_LINK, _NAME) \
template<> struct _LINK ang::intf_class_info<_NAME> { \
	static ang::rtti_t const& class_info(); \
};

namespace ang
{
	template<typename T> class intf_wrapper;
	template<typename T> class intf_wrapper_ptr;
	template<typename T> class object_wrapper;
	template<typename T> class object_wrapper_ptr;

	template<typename T, typename IID, typename...Ts> struct intf;
	ang_declare_object(object);
	ang_declare_interface(iobject);

	using iintf = intf<void, iid("ang::intf")>;
	using intf_t = intf_wrapper<iintf>;
	using intf_ptr_t = intf_wrapper_ptr<iintf>;
	using intfptr = intf_t;
	using objptr = object_t;

	template<> struct nvt LINK intf<void, iid("ang::intf")>
	{
		static bool default_query_interface(rtti_t const&, unknown_t, rtti_t const&, unknown_ptr_t);
		static rtti_t const& class_info();
		virtual rtti_t const& runtime_info()const = 0;
		virtual bool query_interface(rtti_t const&, unknown_ptr_t) = 0;
	};

	namespace text
	{
		ang_declare_interface(istring);
	}


	template<typename T, typename = void> struct has_class_info_function : false_type {};
	template<typename T> struct has_class_info_function<T, void_t<decltype(T::class_info())>> : true_type {};

	template<typename T> struct is_interface : or_expression<
		is_same_type<T, iintf>::value,
		is_inherited_from<T, iintf>::value> {
	};

	template<typename...Ts>
	struct are_interface : and_expression<is_interface<Ts>::value...> {
	};

	template<> struct are_interface<> : true_type { }; //empty args

	template<typename T> struct intf_class_info;

	template<typename T>
	struct intf_class_info {
		static rtti_t const& class_info();
	};

	template<typename T, typename IID, typename...Ts>
	struct nvt intf : Ts... {
		using iid_type = IID;
		using base_pack = type_pack<Ts...>;
		using intf_pack = base_pack;
		static_assert(are_interface<Ts...>::value);
		static rtti_t const& class_info() { return intf_class_info<T>::class_info(); }
		virtual rtti_t const& runtime_info()const = 0;
		virtual bool query_interface(rtti_t const&, unknown_ptr_t) = 0;
	};

	template<typename T, typename IID>
	struct nvt intf<T, IID> : iintf {
		using iid_type = IID;
		using base_pack = type_pack<iintf>;
		using intf_pack = base_pack;
		static rtti_t const& class_info() { return intf_class_info<T>::class_info(); }
		virtual rtti_t const& runtime_info()const = 0;
		virtual bool query_interface(rtti_t const&, unknown_ptr_t) = 0;
	};

	namespace runtime
	{
		template<typename T> struct type_info_builder<intf_wrapper<T>, gender::class_type> : type_info_builder<T> { };
		template<typename T> struct type_info_builder<object_wrapper<T>, gender::class_type> : type_info_builder<T> { };	
	}
}

#endif//__ANG_BASE_INTERFACE_H__
