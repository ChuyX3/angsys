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

#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_RUNTIME_H__
#define __ANG_RUNTIME_H__

#define ANG_REGISTER_RUNTIME_VALUE_TYPE_INFORMATION(_TYPE) \
template<> struct ang::runtime::runtime_type_builder<_TYPE> { \
	static inline ang::type_name_t type_name() { return #_TYPE; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_name(); } \
	template<class...Ts> static inline ang::unknown_t dynamic_constructor(Ts... args) { \
		auto alloc = ang::memory::allocator_manager::get_allocator(ang::memory::allocator_manager::default_allocator); \
		return alloc->construct<_TYPE, Ts...>(alloc->object_alloc<_TYPE>(1), args...); } \
	static inline void dynamic_destructor(ang::unknown_t uknown) {	\
		auto alloc = ang::memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator); \
		alloc->destruct(reinterpret_cast<_TYPE*>(uknown.get())); alloc->memory_release(uknown.get()); } \
	static inline ang::runtime_type_info_t runtime_type() { static runtime_type_info_t runtime_type_info(type_name(), &dynamic_constructor, &dynamic_destructor, true); return runtime_type_info; } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_name<new_t>())) return (new_t*)_old; return null; } \
}; \
template<> struct ang::runtime::runtime_type_builder<typename _TYPE::type> { \
	static inline ang::type_name_t type_name() { return #_TYPE; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_name(); } \
	template<class...Ts> static inline ang::unknown_t dynamic_constructor(Ts... args) { 	\
		auto alloc = ang::memory::allocator_manager::get_allocator(ang::memory::allocator_manager::default_allocator); \
		return alloc->construct<typename _TYPE::type, Ts...>(alloc->object_alloc<typename _TYPE::type>(1), args...); } \
	static inline void dynamic_destructor(ang::unknown_t uknown)  {	\
		auto alloc = ang::memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator); \
		alloc->destruct(reinterpret_cast<typename _TYPE::type*>(uknown.get())); alloc->memory_release(uknown.get()); } \
	static inline ang::runtime_type_info_t runtime_type() { static runtime_type_info_t runtime_type_info(type_name(), &dynamic_constructor, &dynamic_destructor, true); return runtime_type_info; } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_name<new_t>())) return (new_t*)_old; return null; } \
};

#define ANG_REGISTER_RUNTIME_TYPE_INFORMATION(_TYPE) \
template<> struct ang::runtime::runtime_type_builder<_TYPE> { \
	static inline ang::type_name_t type_name() { return #_TYPE##_s; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_name(); } \
	template<class...Ts> static inline ang::unknown_t dynamic_constructor(Ts... args) { \
		auto alloc = ang::memory::allocator_manager::get_allocator(ang::memory::allocator_manager::default_allocator); \
		return alloc->construct<_TYPE, Ts...>(alloc->object_alloc<_TYPE>(1), args...); } \
	static inline void dynamic_destructor(ang::unknown_t uknown) {	\
		auto alloc = ang::memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator); \
		alloc->destruct(reinterpret_cast<_TYPE*>(uknown.get())); alloc->memory_release(uknown.get()); } \
	static inline ang::runtime_type_info_t runtime_type() { static runtime_type_info_t runtime_type_info(type_name(), &dynamic_constructor, &dynamic_destructor, true); return runtime_type_info; } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_name<new_t>())) return (new_t*)_old; return null; } \
};

#define ANG_REGISTER_RUNTIME_TYPENAME(_TYPE) \
template<> struct ang::runtime::runtime_type_builder<_TYPE> { \
	typedef _TYPE type; \
	static inline ang::type_name_t type_name() { return #_TYPE; } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_name(); } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_name<new_t>())) return (new_t*)_old; return null; } \
};

#define ANG_REGISTER_RUNTIME_DYNAMICOBJECT_INFORMATION(_TYPE) \
template<> struct ang::runtime::runtime_type_builder<_TYPE> { \
	typedef _TYPE type; \
	static inline ang::type_name_t type_name() { return type::class_name(); } \
	static inline bool is_type_of(ang::type_name_t name) { return name == type_name(); } \
	static inline ang::type_name_t runtime_type_name(const type& var) { return var.object_name(); } \
	template<class...Ts> static ang::unknown_t dynamic_constructor(Ts... args) { return type::dynamic_constructor(args...); } \
	static void dynamic_destructor(ang::unknown_t uknown) { type::dynamic_destructor(reinterpret_cast<ang::interface_t*>(uknown.get())); } \
	static inline ang::runtime_type_info_t runtime_type() { static runtime_type_info_t runtime_type_info(type_name(), (ang::dynamic_type_constructor_t)&dynamic_constructor, &dynamic_destructor, true); return runtime_type_info; } \
	template<typename new_t> static inline new_t* interface_cast(_TYPE* _old) { if (is_type_of(runtime::type_name<new_t>())) return (new_t*)_old; return null; } \
};

namespace ang
{
	namespace runtime
	{
		typedef unknown_t(*dynamic_type_constructor_t)();
		typedef void(*dynamic_type_destructor_t)(unknown_t);
		template<typename T>struct runtime_type_builder;

		struct runtime_type_info_t
		{
			type_name_t type_name;
			dynamic_type_constructor_t constructor;
			dynamic_type_destructor_t destructor;
			inline runtime_type_info_t(type_name_t = null, dynamic_type_constructor_t = null, dynamic_type_destructor_t = null, bool = false);
			static inline type_name_t class_name() {
				return "ang::runtime::runtime_type_info_t"_s;
			}
		};

		template<typename... args_list>
		inline type_name_t args_list_type_name();

		template<typename T>
		inline type_name_t type_name();

		template<typename T>
		inline bool is_type_of(type_name_t);

		template<typename T>
		inline type_name_t type_name(const T& var);

		template<typename T>
		inline bool is_type_of(const T&, type_name_t);

		template<typename T>
		inline type_name_t runtime_type_name(const T& var);

		template<typename T>
		inline runtime_type_info_t runtime_type();

		template<typename new_t, typename old_t>
		inline auto interface_cast(old_t* _old);

		template<typename new_t, typename old_t>
		inline auto interface_cast(const old_t* _old);

		template<typename new_t, typename old_t>
		inline bool interface_cast(old_t* _old, new_t*& _new);

		template<typename new_t, typename old_t>
		inline bool interface_cast(const old_t* _old, const new_t*& _new);

		class LINK runtime_type_info_database
		{
		public:
			static type_name_t class_name();
			static bool regist_runtime_type_info(const runtime_type_info_t&);

			static runtime_type_info_t find_runtime_type_info(type_name_t);
			static unknown_t contruct_dynamic_object(type_name_t);
			static bool contruct_dynamic_object(type_name_t, intfptr&);
			static bool destruct_dynamic_object(type_name_t, unknown_t);

			template<class T>
			static bool contruct_dynamic_object(object_wrapper<T>& out) {
				intfptr _intf;
				if (!contruct_dynamic_object(type_name<T>(), _intf))
					return false;
				out = reinterpret_cast<T*>(_intf.get());
				return true;
			}

			template <typename... args_t>
			static unknown_t contruct_dynamic_object(type_name_t _type_name, args_t... args);
		};

		inline runtime_type_info_t::runtime_type_info_t(type_name_t name, dynamic_type_constructor_t cons, dynamic_type_destructor_t dest, bool regist) {
			type_name = name;
			constructor = cons;
			destructor = dest;
			if (regist) runtime_type_info_database::regist_runtime_type_info(*this);
		}
	}

	using namespace runtime;
}

#endif//__ANG_RUNTIME_H__
