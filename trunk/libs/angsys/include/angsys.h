/*********************************************************************************************************************/
/*   File Name: angsys.h                                                                                             */
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
#define __ANGSYS_H__

#include <ang/base/base.h>

#ifdef LINK
#undef LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANG_DYNAMIC_LIBRARY

#ifdef ANG_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_EXPORTS
#else//#elif defined ANG_STATIC_LIBRARY
#define LINK
#endif//ANG_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

#ifdef __cplusplus

namespace ang
{
	ang_declare_interface(ivariable);
	typedef ivariable_t variant, ivar;
	//typedef vector<ivariable_t> variable_args_t;
	template<typename T> class weak_ptr;
	template<typename T, gender_t TYPE = gender_of<T>()> class variable;
	template<typename T, gender_t TYPE = gender_of<T>()> using variable_ptr = object_wrapper<variable<T, TYPE>>;

	typedef variable<bool> boolean;
	typedef variable<int> integer;
	typedef variable<uint> uinteger;
	typedef variable<long> linteger;
	typedef variable<ulong> ulinteger;
	typedef variable<long64> integer64;
	typedef variable<ulong64> uinteger64;
	typedef variable<float> floating;
	typedef variable<double> floating64;

	typedef object_wrapper<variable<double>> boolean_t;
	typedef object_wrapper<variable<double>> integer_t;
	typedef object_wrapper<variable<double>> uinteger_t;
	typedef object_wrapper<variable<double>> integer64_t;
	typedef object_wrapper<variable<double>> uinteger64_t;
	typedef object_wrapper<variable<double>> floating_t;
	typedef object_wrapper<variable<double>> floating64_t;

	namespace delegates
	{
		template<typename T> class listener;
		template<typename T> class function_base;
		template<typename T> using function = object_wrapper<function_base<T>>;
		template<typename T> struct ifunction;
		template<typename T> using ifunction_ptr = intf_wrapper<ifunction<T>>;
		template<typename T> using method = intf_wrapper<ifunction<T>>;
	}
	using delegates::function;

	namespace text
	{
		template<encoding E, template<typename>class A = memory::buffer_allocator> class basic_string_buffer;
		template<encoding E, template<typename>class A = memory::buffer_allocator> using basic_string = object_wrapper<basic_string_buffer<E, A>>;
		typedef istring string_buffer;
		typedef basic_string_buffer<encoding::ascii, memory::buffer_allocator> ascii_string_buffer;
		typedef basic_string_buffer<encoding::unicode, memory::buffer_allocator> unicode_string_buffer;
		typedef basic_string_buffer<encoding::utf8, memory::buffer_allocator> utf8_string_buffer;
		typedef basic_string_buffer<encoding::utf16, memory::buffer_allocator> utf16_string_buffer;
		typedef basic_string_buffer<encoding::utf32, memory::buffer_allocator> utf32_string_buffer;

		typedef istring_t string;
		typedef istring_ptr_t string_ptr, string_ptr_t;

		typedef object_wrapper<ascii_string_buffer> ascii_string;
		typedef object_wrapper<unicode_string_buffer> unicode_string;
		typedef object_wrapper<utf8_string_buffer> utf8_string;
		typedef object_wrapper<utf16_string_buffer> utf16_string;
		typedef object_wrapper<utf32_string_buffer> utf32_string;
	}

	using text::string;
	using text::string_ptr;
}

#include <ang/smart_ptr.h>
#include <ang/object.h>
#include <ang/property.h>
#include <ang/ibuffers.h>
#include <ang/collections.h>
#include <ang/strings/istring.h>
#include <ang/ivariable.h>
#include <ang/variable.h>
#include <ang/collections/array.h>
#include <ang/collections/vector.h>
#include <ang/collections/hash.h>
#include <ang/delegates/function.h>
#include <ang/delegates/listener.h>
#include <ang/strings/string.h>
#include <ang/singleton.h>
#include <ang/optional.h>

namespace ang
{

	template<typename T, bool IS_OBJECT>
	inline iintf* ___static_interface_cast_imp<T, IS_OBJECT>::cast(T* ptr)
	{
		static_assert(is_interface<T>::value, "");
		return reinterpret_cast<iintf*>(ptr);
	}

	template<typename T>
	inline iintf* ___static_interface_cast_imp<T, true>::cast(T* ptr)
	{
		return static_cast<iobject*>(static_cast<object*>(ptr));
	}

	template<typename T>
	iintf* static_interface_cast(T* ptr) {
		return ___static_interface_cast_imp<T>::cast(ptr);
	}

	template<typename T, typename... Ts>
	inline rtti_t const& implement<T, Ts...>::class_info() {
		return intf_class_info<T>::class_info();
	}

	template<typename T, typename... Ts>
	inline rtti_t const& implement<T, Ts...>::runtime_info()const {
		return class_info();
	};

	template<typename T, typename... Ts>
	inline bool implement<T, Ts...>::query_interface(rtti_t const& id, unknown_ptr_t out) {
		if (id.type_id() == class_info().type_id()) {
			if (out == null)
				return false;
			*out = static_cast<T*>(this);
		}
		else if (object_base::query_interface(id, out)) {
			return true;
		}
		else {
			return __query_interface_pack<T, intf_pack>::query_interface(static_cast<T*>(this), id, out);
		}
		return false;
	}

	template<typename T, typename CP, typename... Ts>
	inline rtti_t const& implement<T, ang::iid_type<CP>, Ts...>::class_info() {
		return intf_class_info<T>::class_info();
	}

	template<typename T, typename CP, typename... Ts>
	inline rtti_t const& implement<T, ang::iid_type<CP>, Ts...>::runtime_info()const {
		return class_info();
	};

	template<typename T, typename CP, typename... Ts>
	inline bool implement<T, ang::iid_type<CP>, Ts...>::query_interface(rtti_t const& id, unknown_ptr_t out) {
		rtti_t const& id2 = class_info();
		if (id.type_id() == id2.type_id()) {
			if (out == null)
				return false;
			*out = static_cast<T*>(this);
		}
		else if (object_base::query_interface(id, out)) {
			return true;
		}
		else {
			return __query_interface_pack<T, intf_pack>::query_interface(static_cast<T*>(this), id, out);
		}
		return false;
	}

	template<typename T>
	struct entrypoint {
		template<typename char_t>static auto run(int arc, char_t* arv[]) {
#ifdef ANG_DYNAMIC_INITIALIZE
			::ang_library_start();
#endif
			static constexpr auto encoding = text::encoding_by_char_type<char_t>::value;
			array<string> args(arc);
			for (int i = 0; i < arc; i++)
				args[i] = cstr_t(arv[i], text::text_encoder<encoding>::lenght(arv[i]), encoding);
			auto result = T::main(args);
			args.clear();
#ifdef ANG_DYNAMIC_INITIALIZE
			::ang_library_shutdown();
#endif
			return ang::move(result);
		}
	};

}


#endif//__cplusplus

#ifdef LINK
#undef LINK
#endif//LINK

//#include <ang/inline/>
#include <ang/collections/inline/collections.inl>
#include <ang/delegates/inline/function.inl>
#include <ang/delegates/inline/listener.inl>
#include <ang/inline/variable.inl>
#include <ang/collections/inline/array.inl>
#include <ang/collections/inline/vector.inl>

#endif//__ANGSYS_H__
