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

#define ang_object(_NAME) typedef ang::object_wrapper<class _NAME> _NAME##_t; typedef ang::object_wrapper_ptr<_NAME> _NAME##_ptr_t
#define ang_interface(_NAME) typedef ang::intf_wrapper<struct _NAME> _NAME##_t; typedef ang::intf_wrapper_ptr<_NAME> _NAME##_ptr_t

#define ANG_UTILS_TO_STRING(_VALUE) #_VALUE
#define ANG_UTILS_TO_STRING_OBJ(_VALUE) ang::cstr_t(#_VALUE)

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


#define ang_begin__(...)	__VA_ARGS__ : ang::interface {
#define ang_begin(...)	ANG_EXPAND(ang_begin__(__VA_ARGS__))
#define ANG_END()	}


#define ang_begin_interface(_NAME) \
struct ang_begin(_NAME) \
	ANG_DECLARE_CLASS_INFO() \
	ANG_DECLARE_RUNTIME_INFO() \
	ANG_DECLARE_QUERY_INTERFACE()

#define ang_begin_interface_inline(_NAME) \
struct ang_begin(_NAME) \
	inline ANG_DECLARE_CLASS_INFO() \
	ANG_DECLARE_RUNTIME_INFO() \
	ANG_DECLARE_QUERY_INTERFACE()

#define ang_end_interface() ANG_END()


namespace ang //constants
{
	template<typename T> class intf_wrapper;
	template<typename T> class intf_wrapper_ptr;
	template<typename T> class object_wrapper;
	template<typename T> class object_wrapper_ptr;

	ang_interface(interface);
	typedef interface_t intfptr;

	struct LINK interface {
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
		template<typename T, bool IS_INTERFACE = has_class_info_function<T>::value>
		struct __type_info_builder_genre_class_helper : false_type {
			static rtti_t const& type_of() { return T::class_info(); }
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<T, false> : true_type {
			static rtti_t const& type_of() {
				rtti_t const& info = rtti::regist("class<'unknown'>", genre::class_type, sizeof(T), alignof(T));
				return info;
			}
		};

		template<typename T>
		struct type_info_builder<T, genre::class_type> : __type_info_builder_genre_class_helper<T> {
		};

	
		template<typename T> struct type_info_builder<intf_wrapper<T>, genre::class_type> : type_info_builder<T> { };
		template<typename T> struct type_info_builder<object_wrapper<T>, genre::class_type> : type_info_builder<T> { };
		
	}
}

#endif//__ANG_BASE_INTERFACE_H__