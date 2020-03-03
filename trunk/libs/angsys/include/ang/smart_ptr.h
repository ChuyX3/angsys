/*********************************************************************************************************************/
/*   File Name: ang/smart_ptr.h                                                                                      */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __SAMRT_PTR_H__
#define __SAMRT_PTR_H__
namespace ang
{
	enum class smart_type
	{
		none,
		smart_object,
		smart_intf,
	};

	template<typename T> struct is_object : integer_constant<bool, is_same_type<T, object>::value || is_inherited_from<T, object>::value> {};

	template<typename T, bool VALUE = is_interface<typename remove_reference<T>::type>::value> struct is_smart_ptr : public false_type {};
	template<typename T> struct is_smart_ptr<T, true> : public true_type {};

	template<typename T, bool VALUE1 = is_object<typename remove_reference<T>::type>::value, bool VALUE2 = is_interface<typename remove_reference<T>::type>::value>
	struct smart_ptr_type;

	template<typename T> struct smart_ptr_type<T, false, true> { 
		static constexpr ang::smart_type smart_type = ang::smart_type::smart_intf;
		typedef intf_wrapper<typename remove_const<typename remove_reference<T>::type>::type> smart_ptr_t;
		typedef intf_wrapper_ptr<typename remove_const<typename remove_reference<T>::type>::type> smart_ptr_ptr_t;
		typedef typename smart_ptr_t::type type;
	};
	
	template<typename T> struct smart_ptr_type<T, true, true> {
		static constexpr ang::smart_type smart_type = ang::smart_type::smart_object;
		typedef object_wrapper<typename remove_const<typename remove_reference<T>::type>::type> smart_ptr_t;
		typedef object_wrapper_ptr<typename remove_const<typename remove_reference<T>::type>::type> smart_ptr_ptr_t;
		typedef typename smart_ptr_t::type type;
	};

	template<typename T> struct smart_ptr_type<T, false, false> {
		static constexpr ang::smart_type smart_type = ang::smart_type::none;
		typedef object_wrapper<variable<typename remove_const<typename remove_reference<T>::type>::type>>smart_ptr_t;
		typedef object_wrapper_ptr<variable<typename remove_const<typename remove_reference<T>::type>::type>>smart_ptr_ptr_t;
		typedef typename smart_ptr_t::type type;
	};

	template<typename T> struct smart_ptr_type<intf_wrapper<T>, false, false> {
		static constexpr ang::smart_type smart_type = ang::smart_type::none;
		typedef intf_wrapper<T> smart_ptr_t;
		typedef intf_wrapper_ptr<T> smart_ptr_ptr_t;
		typedef typename smart_ptr_t::type type;
	};

	template<typename T> struct smart_ptr_type<object_wrapper<T>, false, false> {
		static constexpr ang::smart_type smart_type = ang::smart_type::none;
		typedef object_wrapper<T> smart_ptr_t;
		typedef object_wrapper_ptr<T> smart_ptr_ptr_t;
		typedef typename smart_ptr_t::type type;
	};

	template<> struct smart_ptr_type<void, false, false> {
		static constexpr ang::smart_type smart_type = ang::smart_type::none;
		typedef object_t smart_ptr_t;
		typedef object_ptr_t smart_ptr_ptr_t;
		typedef object type;
	};

	template<typename T> using nullable = typename smart_ptr_type<T>::smart_ptr_t;
	template<typename T> using smart = typename smart_ptr_type<T>::smart_ptr_t;
	template<typename T> using shared_ptr = typename smart_ptr_type<T>::smart_ptr_t;
	template<typename T, typename...Ts> shared_ptr<T> make_shared(Ts&&... args);

	template<typename T, bool IS_OBJECT = is_object<T>::value>
	struct ___static_interface_cast_imp {
		static iintf* cast(T* ptr);
	};

	template<typename T>
	struct ___static_interface_cast_imp<T,true> {
		static iintf* cast(T* ptr);
	};

	template<typename T> iintf* static_interface_cast(T* ptr);
	
	/******************************************************************/
	/* interface ang::iobject :                                       */
	/*  -> implements basic methods for smart pointer management      */
	/******************************************************************/
	struct nvt LINK iobject
		: intf<iobject , iid("ang::iobject")>
	{
		virtual dword add_ref() = 0;
		virtual dword release() = 0;
		protected: virtual void dispose() = 0;
	};
}

#include <ang/inline/intf_wrapper.hpp>
#include <ang/inline/object_wrapper.hpp>
#include <ang/inline/wrapper_specialization_definition.hpp>

namespace ang
{
	namespace algorithms
	{
		template<typename T>
		struct hash<intf_wrapper<T>> {
			static ulong64 make(intf_wrapper<T> const& value) {
				union { T* k; ulong64 val; } key;
				key.val = 0;
				key.k = value.get();
				return (ulong64)((2475825 + key.val + 1));
			}
			ulong64 operator()(intf_wrapper<T> const& value)const {
				return make(value);
			}
		};

		template<typename T>
		struct hash<object_wrapper<T>> {
			static ulong64 make(object_wrapper<T> const& value) {
				union { T* k; ulong64 val; } key;
				key.val = 0;
				key.k = value.get();
				return (ulong64)((2475825 + key.val + 1));
			}
			ulong64 operator()(object_wrapper<T> const& value)const {
				return make(value);
			}
		};
	}

	namespace runtime
	{
		template<typename To, typename From> inline To* interface_cast(intf_wrapper<From> const& old) { return __dyn_cast_helper<To, From>::cast(old.get()); }
		template<typename To, typename From> inline To* interface_cast(object_wrapper<From> const& old) { return __dyn_cast_helper<To, From>::cast(old.get()); }
	}
}

#endif//__SAMRT_PTR_H__