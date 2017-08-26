/*********************************************************************************************************************/
/*   File Name: angsys.h                                                                                             */
/*   Autor: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                    */
/*   File description: this file declare all functions and clases for dynamic type naming and types conversions.     */
/*   Also it defines useful macros for the use of this functions and clases.                                         */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_RUNTIME_H__
#define __ANG_RUNTIME_H__

#define ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(_TYPE) \
template<> struct ang::runtime::runtime_type_builder<ang::value<_TYPE>> { \
	static inline ang::type_name_t type_of() { return #_TYPE; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
template<> struct ang::runtime::runtime_type_builder<_TYPE> { \
	static inline ang::type_name_t type_of() { return #_TYPE; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
template<> struct ang::runtime::runtime_type_builder<const _TYPE> { \
		static inline ang::type_name_t type_of() { return "const "#_TYPE; } \
		static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
		template<typename new_t> static inline new_t* interface_cast(_TYPE const* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
template<> struct ang::runtime::runtime_type_builder<_TYPE&> { \
		static inline ang::type_name_t type_of() { return #_TYPE"&"; } \
		static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
		template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
template<> struct ang::runtime::runtime_type_builder<_TYPE const&> { \
		static inline ang::type_name_t type_of() { return "const "#_TYPE"&"; } \
		static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
		template<typename new_t> static inline new_t* interface_cast(_TYPE const* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
template<> struct ang::runtime::runtime_type_builder<ang::static_array<_TYPE>> { \
	static inline ang::type_name_t type_of() { return #_TYPE"[]"; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
}; \
template<> struct ang::runtime::runtime_type_builder<ang::static_array<const _TYPE>> { \
	static inline ang::type_name_t type_of() { return "const "#_TYPE"[]"; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
}; \

#define ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION_TEMPLATE(_TYPE, ...) \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<_TYPE<__VA_ARGS__>> { \
	static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move(#_TYPE"<"_o + ang::runtime::args_list_type_of<__VA_ARGS__>() + ">"_s)); return name; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE<__VA_ARGS__>* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<ang::value<_TYPE<__VA_ARGS__>>> { \
	static inline ang::type_name_t type_of() { return runtime_type_builder<_TYPE<__VA_ARGS__>>::type_of(); } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE<__VA_ARGS__>* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<const _TYPE<__VA_ARGS__>> { \
		static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move("const "#_TYPE"<"_o + ang::runtime::args_list_type_of<__VA_ARGS__>() + ">"_s)); return name; } \
		static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
		template<typename new_t> static inline new_t* interface_cast(_TYPE<__VA_ARGS__> const* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<_TYPE<__VA_ARGS__>&> { \
		static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move(#_TYPE"<"_o + ang::runtime::args_list_type_of<__VA_ARGS__>() + ">&"_s)); return name; } \
		static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
		template<typename new_t> static inline new_t* interface_cast(_TYPE<__VA_ARGS__>* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<_TYPE<__VA_ARGS__> const&> { \
		static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move("const "#_TYPE"<"_o + ang::runtime::args_list_type_of<__VA_ARGS__>() + ">&"_s)); return name; } \
		static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
		template<typename new_t> static inline new_t* interface_cast(_TYPE<__VA_ARGS__> const* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<ang::static_array<_TYPE<__VA_ARGS__>>> { \
	static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move(#_TYPE"<"_o + ang::runtime::args_list_type_of<__VA_ARGS__>() + ">[]"_s)); return name;; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
}; \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<ang::static_array<const _TYPE<__VA_ARGS__>>> { \
	static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move("const "#_TYPE"<"_o + ang::runtime::args_list_type_of<__VA_ARGS__>() + ">[]"_s)); return name; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
}; \


#define ANG_REGISTER_RUNTIME_TYPE_INFORMATION(_TYPE) \
template<> struct ang::runtime::runtime_type_builder<_TYPE> { \
	static inline ang::type_name_t type_of() { return #_TYPE##_s; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
};

#define ANG_REGISTER_RUNTIME_TYPENAME(_TYPE) \
template<> struct ang::runtime::runtime_type_builder<_TYPE> { \
	typedef _TYPE type; \
	static inline ang::type_name_t type_of() { return #_TYPE; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
};

namespace ang
{
	namespace runtime
	{
		template<typename T>struct runtime_type_builder;

		template<typename... args_list>
		inline type_name_t args_list_type_of();

		template<typename T>
		inline type_name_t type_of();

		template<typename T>
		inline bool is_type_of(type_name_t);

		template<typename T>
		inline type_name_t type_of(const T& var);

		template<typename T>
		inline bool is_type_of(const T&, type_name_t);

		template<typename T>
		inline type_name_t runtime_type_of(const T& var);

		template<typename new_t, typename old_t>
		inline auto interface_cast(old_t* _old);

		template<typename new_t, typename old_t>
		inline auto interface_cast(const old_t* _old);

		template<typename new_t, typename old_t>
		inline bool interface_cast(old_t* _old, new_t*& _new);

		template<typename new_t, typename old_t>
		inline bool interface_cast(const old_t* _old, const new_t*& _new);

		struct runtime_data_base
		{
			static type_name_t regist_typename(string);
		};

		template<typename T>
		struct runtime_type_builder
		{
			typedef T type;
			static inline type_name_t type_of() { return type::class_name(); }
			static inline bool is_type_of(type_name_t name) { return type::is_child_of(name); }
			static inline type_name_t runtime_type_name(const type& var) { return var.object_name(); }
			template<typename new_t> static inline auto interface_cast(T* _old) -> typename runtime_type_builder<new_t>::type* {
				if (_old == null) return null;
				typename runtime_type_builder<new_t>::type* _new = null;
				_old->query_object(runtime::type_of<new_t>(), (unknown_ptr_t)(void**)&_new);
				return _new;
			}
		};

		template<>
		struct runtime_type_builder<void>
		{
			typedef void type;
			static inline type_name_t type_of() { return "void"_s; }
			static inline bool is_type_of(type_name_t name) { return name == type_of(); }
			template<typename new_t> static inline auto interface_cast(void* _old) { 
				if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old;
				return null;
			}
		};

	}

	using namespace runtime;
}

#endif//__ANG_RUNTIME_H__
