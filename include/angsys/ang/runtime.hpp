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
template<> struct ang::runtime::runtime_type_builder<_TYPE> { \
	typedef _TYPE type;\
	static inline ang::type_name_t type_of() { return ANG_UTILS_TO_STRING_OBJ(_TYPE); } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
template<> struct ang::runtime::runtime_type_builder<const _TYPE> { \
	typedef _TYPE const type;\
	static inline ang::type_name_t type_of() { return "const " ANG_UTILS_TO_STRING(_TYPE) ""_s; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE const* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
template<> struct ang::runtime::runtime_type_builder<_TYPE&> { \
	typedef _TYPE type;\
	static inline ang::type_name_t type_of() { return ANG_UTILS_TO_STRING(_TYPE) "&"_s; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
template<> struct ang::runtime::runtime_type_builder<_TYPE const&> { \
	typedef _TYPE const type;\
	static inline ang::type_name_t type_of() { return "const " ANG_UTILS_TO_STRING(_TYPE) "&"_s; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE const* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
template<> struct ang::runtime::runtime_type_builder<ang::collections::array_view<_TYPE>> { \
	static inline ang::type_name_t type_of() { return ANG_UTILS_TO_STRING(_TYPE) "[]"_s; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
}; \
template<> struct ang::runtime::runtime_type_builder<ang::collections::array_view<const _TYPE>> { \
	static inline ang::type_name_t type_of() { return "const " ANG_UTILS_TO_STRING(_TYPE) "[]"_s; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
}; \
template<> struct ang::runtime::runtime_type_builder<ang::collections::scope_array<_TYPE>> { \
	static inline ang::type_name_t type_of() { return ANG_UTILS_TO_STRING(_TYPE) "[]"_s; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
}; \
template<> struct ang::runtime::runtime_type_builder<ang::collections::scope_array<const _TYPE>> { \
	static inline ang::type_name_t type_of() { return "const " ANG_UTILS_TO_STRING(_TYPE) "[]"_s; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
}; \
template<wsize _SIZE> struct ang::runtime::runtime_type_builder<ang::collections::stack_array<_TYPE, _SIZE>> { \
	static inline ang::type_name_t type_of() { return ANG_UTILS_TO_STRING(_TYPE) "[]"_s; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
}; \
template<wsize _SIZE> struct ang::runtime::runtime_type_builder<ang::collections::stack_array<const _TYPE, _SIZE>> { \
	static inline ang::type_name_t type_of() { return "const " ANG_UTILS_TO_STRING(_TYPE) "[]"_s; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
}; \

#define ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION_TEMPLATE(_TYPE, ...) \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<_TYPE<__VA_ARGS__>> { \
	static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move((ANG_UTILS_TO_STRING(_TYPE) "<"_o += ang::runtime::args_list_type_of<__VA_ARGS__>()) += ">"_s)); return name; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE<__VA_ARGS__>* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<const _TYPE<__VA_ARGS__>> { \
		static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move(("const " ANG_UTILS_TO_STRING(_TYPE) "<"_o += ang::runtime::args_list_type_of<__VA_ARGS__>()) += ">"_s)); return name; } \
		static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
		template<typename new_t> static inline new_t* interface_cast(_TYPE<__VA_ARGS__> const* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<_TYPE<__VA_ARGS__>&> { \
		static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move((ANG_UTILS_TO_STRING(_TYPE) "<"_o += ang::runtime::args_list_type_of<__VA_ARGS__>()) += ">&"_s)); return name; } \
		static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
		template<typename new_t> static inline new_t* interface_cast(_TYPE<__VA_ARGS__>* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<_TYPE<__VA_ARGS__> const&> { \
		static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move(("const " ANG_UTILS_TO_STRING(_TYPE) "<"_o += ang::runtime::args_list_type_of<__VA_ARGS__>()) += ">&"_s)); return name; } \
		static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
		template<typename new_t> static inline new_t* interface_cast(_TYPE<__VA_ARGS__> const* _old) { if (is_type_of(runtime::type_of<new_t>())) return (new_t*)_old; return null; } \
}; \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<ang::collections::array_view<_TYPE<__VA_ARGS__>>> { \
	static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move((ANG_UTILS_TO_STRING(_TYPE) "<"_o += ang::runtime::args_list_type_of<__VA_ARGS__>()) += ">[]"_s)); return name;; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_of(); } \
}; \
TEMPLATE(__VA_ARGS__) struct ang::runtime::runtime_type_builder<ang::collections::array_view<const _TYPE<__VA_ARGS__>>> { \
	static inline ang::type_name_t type_of() { static type_name_t  name = runtime_data_base::regist_typename(ang::move(("const " ANG_UTILS_TO_STRING(_TYPE) "<"_o += ang::runtime::args_list_type_of<__VA_ARGS__>()) += ">[]"_s)); return name; } \
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

		struct LINK runtime_data_base
		{
			static type_name_t regist_typename(string);
		};

		template<typename T>
		struct ANG_DEPRECATE("using runtime type info without regist the type can cause undefined probles...") __runtime_type_builder_impl_warning
		{
			typedef T type;
			static inline type_name_t type_of() { return "'undefined'"; }
			static inline bool is_type_of(type_name_t name) { return false; }
			static inline type_name_t runtime_type_name(const type& var) { return "'undefined'"; }
			template<typename new_t> static inline auto interface_cast(T* _old) -> typename runtime_type_builder<new_t>::type* {
				return null;
			}
		};

		template<typename T, bool FLAG = ang::has_runtime_type_info<T>::value>
		struct __runtime_type_builder_impl
		{
			typedef T type;
			static constexpr bool has_runtime_type_info = FLAG;
			static inline type_name_t type_of() { return __runtime_type_builder_impl_warning<T>::type_of(); }
			static inline bool is_type_of(type_name_t name) { return __runtime_type_builder_impl_warning<T>::is_type_of(name); }
			static inline type_name_t runtime_type_name(const type& var) { return __runtime_type_builder_impl_warning<T>::runtime_type_name(var); }
			template<typename new_t> static inline auto interface_cast(T* _old) -> typename runtime_type_builder<new_t>::type* {
				return __runtime_type_builder_impl_warning<T>::template interface_cast<new_t>(_old);
			}
		};

		template<typename T>
		struct __runtime_type_builder_impl<T, true>
		{
			typedef T type;
			static constexpr bool has_runtime_type_info = true;
			static inline type_name_t type_of() { return type::class_name(); }
			static inline bool is_type_of(type_name_t name) { return type::is_inherited_of(name); }
			static inline type_name_t runtime_type_name(const type& var) { return var.object_name(); }
			template<typename new_t> static inline auto interface_cast(T* _old) -> typename runtime_type_builder<new_t>::type* {
				if (_old == null) return null;
				typename runtime_type_builder<new_t>::type* _new = null;
				_old->query_object(runtime::type_of<new_t>(), (unknown_ptr_t)(void**)&_new);
				return _new;
			}
		};

		template<typename T>
		struct runtime_type_builder
		{
			typedef T type;
			static inline type_name_t type_of() { 
				ANG_RUNTIME_ERROR(__runtime_type_builder_impl<T>::has_runtime_type_info, "using type_of without regist the type can cause undefined probles...");
				return __runtime_type_builder_impl<T>::type_of(); 
			}
			static inline bool is_type_of(type_name_t name) { 
				ANG_RUNTIME_ERROR(__runtime_type_builder_impl<T>::has_runtime_type_info, "using is_type_of without regist the type can cause undefined probles...");
				return __runtime_type_builder_impl<T>::is_type_of(name); 
			}
			static inline type_name_t runtime_type_name(const type& var) { 
				ANG_RUNTIME_ERROR(__runtime_type_builder_impl<T>::has_runtime_type_info, "using runtime_type_name without regist the type can cause undefined probles...");
				return __runtime_type_builder_impl<T>::runtime_type_name(var); 
			}
			template<typename new_t> static inline auto interface_cast(T* _old) -> typename runtime_type_builder<new_t>::type* {
				ANG_RUNTIME_ERROR(__runtime_type_builder_impl<T>::has_runtime_type_info, "using interface_cast without regist the type can cause undefined probles...");
				return __runtime_type_builder_impl<T>::template interface_cast<new_t>(_old);
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
