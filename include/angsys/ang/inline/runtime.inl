/*********************************************************************************************************************/
/*   File Name: ang/inline/runtime.hpp                                                                               */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error Can't include AngDef.h, please include Angsys.h inside
#elif !defined __RUNTIMEINFO_HPP__
#define __RUNTIMEINFO_HPP__

namespace ang
{
	namespace runtime
	{
		template<class T>
		struct runtime_type_builder<const T>
		{
			typedef T type;
			static inline type_name_t type_of() {
				static type_name_t out = runtime_data_base::regist_typename(ang::move("const "_o += runtime_type_builder<type>::type_of()));
				return out;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_of();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_of<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<class T>
		struct runtime_type_builder<T&>
		{
			typedef T type;
			static inline type_name_t type_of() {
				static type_name_t out = runtime_data_base::regist_typename(ang::move(runtime_type_builder<type>::type_of() + "&"_o));
				return out;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_of();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_of<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<class T>
		struct runtime_type_builder<const T&>
		{
			typedef T type;
			static inline type_name_t type_of() {
				static type_name_t out = runtime_data_base::regist_typename(ang::move(("const "_o += runtime_type_builder<type>::type_of()) += "&"_s));
				return out;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_of();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_of<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<class T>
		struct runtime_type_builder<T*>
		{
			typedef T* type;
			static inline type_name_t type_of() {
				static type_name_t out = runtime_data_base::regist_typename(ang::move(string(runtime_type_builder<T>::type_of()) += "*"_s));
				return out;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_of();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_of<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<class T>
		struct runtime_type_builder<const T*>
		{
			typedef T type;
			static inline type_name_t type_of() {
				static type_name_t out = runtime_data_base::regist_typename(ang::move(("const "_o += runtime_type_builder<T>::type_of()) += "*"_s));
				return out;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_of();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_of<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<typename T>
		struct runtime_type_builder<safe_str<T>>
		{
			typedef typename safe_str<T>::type type;
			static inline type_name_t type_of() { return runtime::type_of<type>(); }
			static inline bool is_type_of(type_name_t name) { return runtime::is_type_of<type>(name); }
		};

		template<typename T>
		struct runtime_type_builder<collections::array_view<T>>
		{
			typedef typename collections::array_view<T>::type type;
			static inline type_name_t type_of() { return runtime::type_of<type>(); }
			static inline bool is_type_of(type_name_t name) { return runtime::is_type_of<type>(name); }
		};

		template<typename T>
		struct runtime_type_builder<object_wrapper<T>>
		{
			typedef typename object_wrapper<T>::type type;
			static inline type_name_t type_of() {
				return type::class_name();
			}
			static inline bool is_type_of(type_name_t name) {
				return type::is_inherited_of(name);
			}
			static inline type_name_t runtime_type_of(const object_wrapper<type>& var) {
				return (var.get()) ? var->object_name() : type::class_name();
			}
		};

		template<typename T>
		struct runtime_type_builder<intf_wrapper<T>>
		{
			typedef typename intf_wrapper<T>::type type;
			static inline type_name_t type_of() {
				return type::class_name();
			}
			static inline bool is_type_of(type_name_t name) {
				return type::is_inherited_of(name);
			}
			static inline type_name_t runtime_type_of(const intf_wrapper<type>& var) {
				return (var.get()) ? var->object_name() : type::class_name();
			}
		};

		template<class T, class... Ts>
		struct runtime_type_builder<T(Ts...)>
		{
			typedef T(type)(Ts...);
			static inline type_name_t type_of() {
				static type_name_t name = runtime_data_base::regist_typename(ang::move(((string(runtime::type_of<T>()) += "("_s) += runtime::args_list_type_of<Ts...>()) += ")"_s));
				return name;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_of();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_of<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<class T, class... Ts>
		struct runtime_type_builder<T(*)(Ts...)>
		{
			typedef T(*type)(Ts...);
			static inline type_name_t type_of() {
				static type_name_t name = runtime_data_base::regist_typename(ang::move(((string(runtime::type_of<T>()) += "(*)("_s) += runtime::args_list_type_of<Ts...>()) += ")"_s));
				return name;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_of();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_of<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<class T, class O, class... Ts>
		struct runtime_type_builder<T(O::*)(Ts...)>
		{
			typedef T(O::*type)(Ts...);
			static inline type_name_t type_of() {
				static type_name_t name = runtime_data_base::regist_typename(ang::move((((string(runtime::type_of<T>()) += "("_s + runtime::type_of<O>()) += "::*)("_s) += runtime::args_list_type_of<Ts...>()) += ")"_s));
				return name;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_of();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_of<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

#if defined WINDOWS_PLATFORM && !defined _WIN64
		template<class T, class... Ts>
		struct runtime_type_builder<T(__stdcall*)(Ts...)>
		{
			typedef T(__stdcall*type)(Ts...);
			static inline type_name_t type_of() {
				static type_name_t name = runtime_data_base::regist_typename(ang::move(((string(runtime::type_of<T>()) += "(stdcall*)("_s) += runtime::args_list_type_of<Ts...>()) += ")"_s));
				return name;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_of();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_of<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};
#endif

		template<typename... args_t>
		inline string& args_list_type_of(string& out);

		template<typename T, typename... args_t>
		inline string& args_list_type_of_builder(string& out)
		{
			out << type_of<T>();
			return args_list_type_of<args_t...>(out);
		}

		template<typename... args_t>
		inline string& args_list_type_of(string& out)
		{
			out << ","_s;
			return args_list_type_of_builder<args_t...>(out);
		}

		template<>
		inline string& args_list_type_of<>(string& out) { return out; }

		template<typename... args_t>
		inline type_name_t args_list_type_of()
		{
			static string out;
			static type_name_t name = runtime_data_base::regist_typename(
				ang::move(args_list_type_of_builder<args_t...>(out))
			);
			return name;
		}

		template<>
		inline type_name_t args_list_type_of<>()
		{
			return type_of<void>();
		}

		template<typename T>
		inline type_name_t type_of() {
			return runtime_type_builder<T>::type_of();
		}

		template<typename T>
		inline type_name_t type_of(const T&) {
			return runtime_type_builder<T>::type_of();
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
		inline type_name_t runtime_type_of(const T& var) {
			return runtime_type_builder<T>::runtime_type_of(var);
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

	}//Runtime
}//ang


ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(bool);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(char);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(wchar);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(char16_t);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(char32_t);
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

ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(char*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(wchar*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(char16_t*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(char32_t*);
ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(mchar*);
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