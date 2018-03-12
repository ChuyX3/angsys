#ifndef __ANGSYS_H__
#error ...
#elif !defined __SAMRT_PTR_H__
#define __SAMRT_PTR_H__
namespace ang
{
	template<typename T> class weak_ptr;

	template<typename T> struct is_object : integer_constant<bool, is_same_type<T, object>::value || is_inherited_from<T, object>::value> {};

	template<typename T, bool VALUE = is_interface<T>::value> struct is_smart_ptr : public false_type {};
	template<typename T> struct is_smart_ptr<T, true> : public true_type {};

	template<typename T, bool VALUE1 = is_object<T>::value, bool VALUE2 = is_interface<T>::value>
	struct smart_ptr_type { static_assert(is_smart_ptr<T>::value, "T is not a smart type");  typedef T* smart_ptr_t; };

	template<typename T> struct smart_ptr_type<T, false, true> { typedef intf_wrapper<T> smart_ptr_t; typedef intf_wrapper<const T> const_smart_ptr_t; typedef typename smart_ptr_t::type type; typedef typename smart_ptr_t::type const const_type; };
	
	template<typename T> struct smart_ptr_type<T, true, true> { typedef object_wrapper<T> smart_ptr_t; typedef object_wrapper<const T> const_smart_ptr_t; typedef typename smart_ptr_t::type type; typedef typename smart_ptr_t::type const const_type; };

	template<typename T> struct smart_ptr_type<T, false, false> {
		//typedef shared_ptr<T> smart_ptr_t; typedef typename smart_ptr_t::type type;
		typedef T* smart_ptr_t; typedef T type;
	};
}

#include <ang/inline/intf_wrapper.hpp>
#include <ang/inline/object_wrapper.hpp>
#include <ang/inline/wrapper_specialization_definition.hpp>


#endif//__SAMRT_PTR_H__