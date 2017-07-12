/*********************************************************************************************************************/
/*   File Name: ang/inline/runtime.hpp                                                                               */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error Can't include AngDef.h, please include Angsys.h inside
#elif !defined __RUNTIMEINFO_HPP__
#define __RUNTIMEINFO_HPP__

namespace ang
{
	namespace runtime
	{
		template<typename T>
		struct runtime_type_builder
		{
			typedef T type;
			static inline type_name_t type_name() {
				return type::class_name();
			}

			static inline bool is_type_of(type_name_t name) {
				return type::is_child_of(name);
			}

			static inline type_name_t runtime_type_name(const type& var) {
				return var.object_name();
			}

			template<typename new_t>
			static inline auto interface_cast(T* _old) -> typename runtime_type_builder<new_t>::type* {
				if (_old == null)
					return null;
				typename runtime_type_builder<new_t>::type* _new = null;
				_old->query_object(runtime::type_name<new_t>(), (unknown_ptr_t)(void**)&_new);
				return _new;
			}
		};

		template<class T>
		struct runtime_type_builder<const T>
		{
			typedef void type;
			static inline type_name_t type_name() {
				static string out = "const "_o + runtime_type_builder<type>::type_name();
				return out->cstr();
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_name();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_name<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<class T>
		struct runtime_type_builder<T&>
		{
			typedef void type;
			static inline type_name_t type_name() {
				static string out = runtime_type_builder<type>::type_name() + "&"_o;
				return out->cstr();
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_name();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_name<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<class T>
		struct runtime_type_builder<T&&>
		{
			typedef void type;
			static inline type_name_t type_name() {
				static string out = runtime_type_builder<type>::type_name() + "&&"_o;
				return out->cstr();
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_name();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_name<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<>
		struct runtime_type_builder<void>
		{
			typedef void type;
			static inline type_name_t type_name() {
				return "void";
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_name();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_name<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<class T, class... Ts>
		struct runtime_type_builder<T(Ts...)>
		{
			typedef void type;
			static inline type_name_t type_name() {
				static string _class_name = string(runtime::type_name<T>()) + "("_s + runtime::args_list_type_name<Ts...>() + ")";
				return _class_name;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_name();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_name<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<class T, class... Ts>
		struct runtime_type_builder<T(*)(Ts...)>
		{
			typedef void type;
			static inline type_name_t type_name() {
				static string _class_name = string(runtime::type_name<T>()) + "(*)("_s + runtime::args_list_type_name<Ts...>() + ")";
				return _class_name;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_name();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_name<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<class T,class O, class... Ts>
		struct runtime_type_builder<T(O::*)(Ts...)>
		{
			typedef void type;
			static inline type_name_t type_name() {
				static string _class_name = string(runtime::type_name<T>()) + "("_s + runtime::type_name<O>() + "::*)("_s + runtime::args_list_type_name<Ts...>() + ")";
				return _class_name;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_name();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_name<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

#if defined WINDOWS_PLATFORM && !defined _WIN64
		template<class T, class... Ts>
		struct runtime_type_builder<T(__stdcall*)(Ts...)>
		{
			typedef void type;
			static inline type_name_t type_name() {
				static string _class_name = string(runtime::type_name<T>()) + "(stdcall*)("_s + runtime::args_list_type_name<Ts...>() + ")";
				return _class_name;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_name();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_name<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};
#endif

		template<typename T>
		struct runtime_type_builder<object_wrapper<T>>
		{
			typedef typename object_wrapper<T>::type type;
			static inline type_name_t type_name() {
				return type::class_name();
			}
			static inline bool is_type_of(type_name_t name) {
				return type::is_child_of(name);
			}
			static inline type_name_t runtime_type_name(const object_wrapper<type>& var) {
				return (var.get()) ? var->object_name() : type::class_name();
			}
		};

		template<typename T>
		struct runtime_type_builder<intf_wrapper<T>>
		{
			typedef typename intf_wrapper<T>::type type;
			static inline type_name_t type_name() {
				return type::class_name();
			}
			static inline bool is_type_of(type_name_t name) {
				return type::is_child_of(name);
			}
			static inline type_name_t runtime_type_name(const intf_wrapper<type>& var) {
				return (var.get()) ? var->object_name() : type::class_name();
			}
		};
		
		template<typename T>
		struct runtime_type_builder<value<T>>
		{
			typedef typename value<T>::type type;
			static inline type_name_t type_name() {
				return runtime::type_name<type>();
			}
			static inline bool is_type_of(type_name_t name) {
				return runtime::is_type_of<type>(name);
			}
		};

		template<typename T>
		struct runtime_type_builder<T*>
		{
			typedef T type;
			static inline type_name_t type_name() {
				static string out = string(runtime_type_builder<type>::type_name()) + "*"_s;
				return out->cstr();
			}
			static inline bool is_type_of(type_name_t name) {
				return strings::algorithms::string_compare(name, type_name()) == 0;
			}
			static inline runtime_type_info_t runtime_type() {
				return runtime_type_info_t{ type_name(), &dynamic_constructor,dynamic_destructor };
			}
			static bool dynamic_constructor(unknown_ptr_t out) {
				if (out.get() == null)
					return false;
				*out = (pointer)ang::memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator)->object_alloc<type>(1);
				return true;
			}
			static bool dynamic_destructor(unknown_ptr_t uknown) {
				if (uknown.get() == null)
					return false;
				ang::memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator)->memory_release(*uknown);
				*uknown = null;
				return true;
			}
			template<typename new_t>
			static inline auto interface_cast(T* _old) {
				return runtime_type_builder<T>::template interface_cast<new_t>(_old);
			}
		};

		template<typename... args_t>
		inline void args_list_type_name(string& out);

		template<typename T, typename... args_t>
		inline void args_list_type_name_builder(string& out)
		{
			out << type_name<T>();
			args_list_type_name<args_t...>(out);
		}

		template<typename... args_t>
		inline void args_list_type_name(string& out)
		{
			out << ",";
			args_list_type_name_builder<args_t...>(out);
		}

		template<>
		inline void args_list_type_name<>(string& out) { }

		template<typename... args_t>
		inline type_name_t args_list_type_name()
		{
			static string _args_list_type_name;
			if (_args_list_type_name.is_empty())
				args_list_type_name_builder<args_t...>(_args_list_type_name);
			return _args_list_type_name->cstr();
		}
		template<>
		inline type_name_t args_list_type_name<>()
		{
			return type_name<void>();
		}


		template<typename T>
		inline type_name_t type_name() {
			return runtime_type_builder<T>::type_name();
		}

		template<typename T>
		inline type_name_t type_name(const T&) {
			return runtime_type_builder<T>::type_name();
		}

		template<typename T>
		inline bool is_type_of(type_name_t name) {
			return runtime_type_builder<T>::is_type_of(name);
		}

		template<typename T>
		inline bool is_type_of(const T&, type_name_t name) {
			return runtime_type_builder<T>::is_type_of(name);
		}

		template<typename T>
		inline type_name_t runtime_type_name(const T& var) {
			return runtime_type_builder<T>::runtime_type_name(var);
		}

		template<typename T>
		inline runtime_type_info_t runtime_type() {
			return runtime_type_builder<T>::runtime_type();
		}

		template<typename new_t, typename old_t>
		inline auto interface_cast(old_t* _old) {
			return runtime_type_builder<old_t>::template interface_cast<new_t>(_old);
		}

		template<typename new_t, typename old_t>
		inline auto interface_cast(const old_t* _old) {
			return runtime_type_builder<old_t>::template interface_cast<new_t>(const_cast<old_t*>(_old));
		}

		template<typename new_t, typename old_t>
		inline bool interface_cast(old_t* _old, new_t*& _new) {
			_new = runtime_type_builder<old_t>::template interface_cast<new_t>(_old);
			return _new != null;
		}

		template<typename new_t, typename old_t>
		inline bool interface_cast(old_t* _old, object_wrapper<new_t>& _new) {
			_new = runtime_type_builder<old_t>::template interface_cast<new_t>(_old);
			return _new != null;
		}

		template<typename new_t, typename old_t>
		inline bool interface_cast(old_t* _old, intf_wrapper<new_t>& _new) {
			_new = runtime_type_builder<old_t>::template interface_cast<new_t>(_old);
			return _new != null;
		}

		template<typename new_t, typename old_t>
		inline bool interface_cast(const old_t* _old, const new_t*& _new) {
			_new = runtime_type_builder<old_t>::template interface_cast<new_t>(const_cast<old_t*>(_old));
			return _new != null;
		}

		template<class T> inline static bool runtime_type_info_database::contruct_dynamic_object(object_wrapper<T>& out) {
			if (!contruct_dynamic_object(type_name<T>(), &out)) return false;
			return true;
		}

		template<class T> inline static bool runtime_type_info_database::contruct_dynamic_object(intf_wrapper<T>& out) {
			if (!contruct_dynamic_object(type_name<T>(), &out))	return false;
			return true;
		}

	}//Runtime
}//ang



ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(bool);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(char);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(wchar);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(ang::mbyte);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(byte);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(short);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(ushort);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(int);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(uint);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(long);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(ulong);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(long64);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(ulong64);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(float);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(double);

ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(pointer);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(const char*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(char*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(const wchar*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(wchar*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(mchar*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(const mchar*);


ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(bool*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(short*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(ushort*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(int*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(uint*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(long*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(ulong*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(long64*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(ulong64*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(float*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(double*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(pointer*);


//ANG_REGISTER_RUNTIME_TYPE_INFORMATION(Ang::ExcepType);
//ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::text::encoding_t);
//ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::collections::iteration_method_t)

//ANG_REGISTER_RUNTIME_DYNAMICOBJECT_INFORMATION_ARGS1(ang::Buffer, Ang::WSizeT);
//ANG_REGISTER_RUNTIME_DYNAMICOBJECT_INFORMATION_ARGS1(ang::TinyString, Ang::CWStr);
//ANG_REGISTER_RUNTIME_DYNAMICOBJECT_INFORMATION_ARGS1(ang::TinyAString, Ang::CAStr);

//ANG_REGISTER_RUNTIME_DYNAMICOBJECT_INFORMATION(ang::String);
//ANG_REGISTER_RUNTIME_DYNAMICOBJECT_INFORMATION(ang::AString);
//ANG_REGISTER_RUNTIME_DYNAMICOBJECT_INFORMATION(ang::MString);

#endif//__RUNTIMEINFO_HPP__