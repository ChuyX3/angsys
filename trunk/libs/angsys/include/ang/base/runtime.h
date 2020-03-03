/*********************************************************************************************************************/
/*   File Name: ang/base/runtime.h                                                                                     */
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
#elif !defined __ANG_BASE_RUNTIME_H__
#define __ANG_BASE_RUNTIME_H__

namespace ang //constants
{

	template<typename... Ts> struct inherit;
	template<typename T, typename U, typename...Ts> struct inherit<T, U, Ts...> : U, Ts...{
	protected:
		using self = T;
		using base_pack = type_pack<U, Ts...>;
		using base = inherit<T, U, Ts...>;
		template<typename...Arsg>inherit(Arsg&&... args)
			: U(forward<Arsg>(args)...) { }
	};

	template<typename T> struct inherit<T> {
	protected:
		using self = T;
	};

	template<typename T, typename TP> struct inherit_from_pack;

	template<typename T, typename... Ts> struct inherit_from_pack<T, type_pack<Ts...>> {
		using type = inherit<T, Ts...>;
	};

	template<typename T, typename TP>
	struct __is_child_in_pack {
		using type = false_type;
	};

	template<typename T, typename U, typename... Ts>
	struct __is_child_in_pack<T, type_pack<U, Ts...>> {
		using type = typename type_selector<is_base_of<T, U>::value, true_type, typename __is_child_in_pack<T, type_pack<Ts...>>::type>::type;
	};

	template<typename T, typename TP>
	struct is_child_in_pack : false_type { };

	template<typename T, typename...Ts>
	struct is_child_in_pack<T, type_pack<Ts...>> : __is_child_in_pack<T, type_pack<Ts...>>::type { };

	template<typename TP1, typename TP2 = type_pack<>> struct __unique_child_pack;

	template<typename...TPTs, typename T, typename...Ts>
	struct __unique_child_pack<type_pack<T, TPTs...>, type_pack<Ts...>> {
		using type = typename type_selector<is_child_in_pack<T, type_pack<TPTs..., Ts...>>::value
			, typename __unique_child_pack<type_pack<TPTs...>, type_pack<Ts...>>::type
			, typename __unique_child_pack<type_pack<TPTs...>, type_pack<Ts..., T>>::type >::type;
	};

	template<typename...Ts>
	struct __unique_child_pack<type_pack<>, type_pack<Ts...>> {
		using type = type_pack<Ts...>;
	};

	template<typename...Ts>
	using unique_child_pack = typename __unique_child_pack<type_pack<Ts...>>::type;

	template<typename T, typename...Ts>
	using unique_inherit = typename inherit_from_pack<T, unique_child_pack<Ts...>>::type;


	namespace runtime
	{
		template<typename P> struct iid_type;

		typedef void* unknown_t;
		typedef void const* const_unknown_t;
		typedef void** unknown_ptr_t;
		typedef wsize type_id_t;
		typedef cstr_view<char> type_name_t;
		typedef struct __type_info type_info_t, rtti, rtti_t;
		typedef struct inherit_info inherit_info_t;
		typedef array_view<inherit_info_t> inherit_pack_info_t;
		template<typename T, gender_t TYPE = gender_of<T>()> struct type_info_builder;
		
		typedef bool(*dynamic_cast_proc)(const rtti_t&, unknown_t, const rtti_t&, unknown_ptr_t);

		template<typename T>
		struct type_info_builder<T, gender::value_type> { static constexpr gender gender = gender::value_type; static rtti_t const& type_of(); };

		template<typename T>
		struct type_info_builder<T, gender::union_type> { static constexpr gender gender = gender::union_type; static rtti_t const& type_of(); };

		template<typename T>
		struct type_info_builder<T, gender::enum_type> { static constexpr gender gender = gender::enum_type; static rtti_t const& type_of(); };

		template<typename T>
		struct type_info_builder<T, gender::function_type> { static constexpr gender gender = gender::function_type; static rtti_t const& type_of(); };

		template<typename T>
		struct type_info_builder<T, gender::class_type> { static constexpr gender gender = gender::class_type; static rtti_t const& type_of(); };

		template<char... str>
		struct iid_type<char_pack<str...>> {
			static constexpr char value[sizeof...(str) + 1] = { str...,0 };
			static type_name_t type_name() {
				return type_name_t(value);
			}
		};

		template<typename... Ps> struct __add_char_pack;

		template<char... str>
		struct __add_char_pack<char_pack<str...>> {
			using type = char_pack<str...>;
		};

		template<char... str1, char... str2, typename...Ps>
		struct __add_char_pack<char_pack<str1...>, char_pack<str2...>, Ps...> {
			using type = typename __add_char_pack<char_pack<str1..., str2...>, Ps...>::type;
		};

		template<char... str, typename...Ps>
		struct __add_char_pack<char_pack<str...>, char_pack<0>, Ps...> {
			using type = char_pack<str...>;
		};

		template<char... str>
		using iid_pack_type = typename __add_char_pack<char_pack<str>...>::type;

		template<char... str>
		using iid_t = iid_type<iid_pack_type<str...>>;

#define iid(ARGS) ang::iid_t<ANG_SPLIT_STRING(ARGS)>

		struct inherit_info {
			inherit_info() : base(null), offset(0) { }
			inherit_info(rtti_t const* b, wsize o) : base(b), offset(o) { }
			inherit_info(inherit_info const& other) : base(other.base), offset(other.offset) { }
			inherit_info(inherit_info && other) : base(ang::move(other.base)), offset(ang::move(other.offset)) { }

			rtti_t const* base;
			wsize offset;
		};

		struct LINK __type_info {
			__type_info(rtti_t&&) = delete;
			__type_info(const rtti_t&) = delete;
			rtti_t& operator = (rtti_t&&) = delete;
			rtti_t& operator = (const rtti_t&) = delete;

			inline type_name_t const& type_name()const { return m_type_name; }
			inline type_id_t type_id()const { return (type_id_t)m_type_name.cstr(); }
			inline gender_t gender()const { return m_gender; }
			inline wsize size()const { return m_size; }
			inline wsize aligment()const { return m_aligment; }
			inline inherit_pack_info_t const& parents()const { return m_parents; }
			inline bool is_type_of(rtti_t const& id)const {
				if (type_id() == id.type_id())
					return true;
				for (inherit_info_t const& pair : m_parents) {
					if (pair.base->is_type_of(id))
						return true;
				}
				return false;
			}
			template<typename T> bool is_type_of()const {
				return is_type_of(type_of<T>());
			}
			template<typename T> bool is_type_of(const T&)const {
				return is_type_of(type_of<T>());
			}

			template<typename T> inline static rtti_t const& type_of() {
				return type_info_builder<T, gender_of<T>()>::type_of();
			}
			template<typename T> inline static rtti_t const& type_of(const T&) {
				return type_info_builder<T>::type_of();
			}
			template<typename To, typename From> static inline To* interface_cast(From* from) {
				To* out = null;
				type_of<From>().m_dyn_cast(type_of<From>(), from, rtti::type_of<To>(), (unknown_ptr_t)&out);
				return out;
			}
			template<typename To, typename From> static inline To const* interface_cast(From const* from) {
				To* out = null;
				type_of<From>().m_dyn_cast(type_of<From>(), const_cast<From*>(from), rtti::type_of<To>(), (unknown_ptr_t)&out);
				return out;
			}

			static rtti_t const& regist(type_name_t name, gender_t g, wsize sz, wsize a);
			static rtti_t const& regist(type_name_t name, gender_t g, wsize sz, wsize a, inherit_pack_info_t parent, dynamic_cast_proc cast = nullptr);

			static rtti_t const& regist(fast_astring_t&& name, gender_t g, wsize sz, wsize a);
			static rtti_t const& regist(fast_astring_t&& name, gender_t g, wsize sz, wsize a, inherit_pack_info_t parent, dynamic_cast_proc cast = nullptr);
			
			inline ~__type_info() {}
		private:
			inline __type_info(type_name_t name, gender_t g, wsize sz, wsize a, inherit_pack_info_t parents, dynamic_cast_proc cast) {
				m_type_name = name;
				m_gender = g;
				m_size = sz;
				m_aligment = a;
				m_parents = ang::move(parents);
				m_dyn_cast = cast;
			}

			type_name_t m_type_name;
			gender_t m_gender;
			wsize m_size;
			wsize m_aligment;
			dynamic_cast_proc m_dyn_cast;
			inherit_pack_info_t m_parents;
		};

		template<typename To, typename From, bool VALUE = is_same_type<To, From>::value || is_base_of<To, From>::value >
		struct __dyn_cast_helper {
			static To* cast(From* old) { return old ? static_cast<To*>(old) : null; }
			static To const* cast(From const* old) { return old ? static_cast<To const*>(old) : null; }
		};

		template<typename To, typename From>
		struct __dyn_cast_helper<To, From, false> {
			static To* cast(From* old) { return old ? rtti::interface_cast<To>(old) : null; }
			static To const* cast(From const* old) { return old ? rtti::interface_cast<To>(old) : null; }
		};

		template<typename T> inline rtti_t const& type_of() { return rtti::type_of<T>(); }
		template<typename T> inline rtti_t const& type_of(const T&) { return rtti::type_of<T>(); }
		template<typename T> inline type_id_t type_id() { return rtti::type_of<T>().type_id(); }
		template<typename T> inline type_id_t type_id(const T&) { return rtti::type_of<T>().type_id(); }
		template<typename T> inline type_name_t type_name() { return rtti::type_of<T>().type_name(); }
		template<typename T> inline type_name_t type_name(const T&) { return rtti::type_of<T>().type_name(); }
		template<typename T> inline bool is_type_of(rtti_t const& id) { return rtti::type_of<T>().is_type_of(id); }
		template<typename T, typename U> inline bool is_type_of() { return rtti::type_of<T>().is_type_of(rtti::type_of<U>()); }
		template<typename T, typename U> inline bool is_type_of(const U&) { return rtti::type_of<T>().is_type_of(rtti::type_of<U>()); }
		template<typename To, typename From> inline To* interface_cast(From* old) { return __dyn_cast_helper<To, From>::cast(old); }
		template<typename To, typename From> inline To const* interface_cast(From const* old) { return __dyn_cast_helper<To, From>::cast(old); }
		//template<typename... Ts> type_name_t args_list_type_name();

		//template<typename T, gender_t TYPE>
		//struct type_info_builder<const T, TYPE> : type_info_builder<T, TYPE> {};

		//template<typename T, gender_t TYPE>
		//struct type_info_builder<T&, TYPE> : type_info_builder<T, TYPE> {};

		//template<typename T, gender_t TYPE>
		//struct type_info_builder<T const&, TYPE> : type_info_builder<T, TYPE> {};

		//template<typename T, gender_t TYPE>
		//struct type_info_builder<T&&, TYPE> : type_info_builder<T, TYPE> {};

	}
	
	template<typename T, typename TP> struct rtti_from_pack;

	template<typename T, typename...Ts>
	struct rtti_from_pack<T, type_pack<Ts...>> {
		inline static runtime::inherit_pack_info_t types();
	};

	template<typename T> struct rtti_from_type {
		inline static runtime::inherit_pack_info_t types() {
			return rtti_from_pack<T, typename T::base_pack>::types();
		}
	};

	using namespace runtime;
}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(_LINK, _TYPE) \
namespace ang { namespace runtime{ \
template<> struct _LINK type_info_builder<_TYPE, ang::gender_of<_TYPE>()> { \
	static rtti_t const& type_of(); \
};}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(_TYPE) \
namespace ang { namespace runtime{ \
template<> struct type_info_builder<_TYPE, ang::gender_of<_TYPE>()> { \
	static rtti_t const& type_of() { static rtti_t const& info = rtti::regist(type_name_t(#_TYPE), gender_of<_TYPE>(), size_of<_TYPE>(), align_of<_TYPE>()); return info; } \
};}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE_OVERRIDE(_TYPE, _NAME) \
namespace ang { namespace runtime{ \
template<> struct _LINK type_info_builder<_TYPE, ang::gender_of<_TYPE>()> { \
	static rtti_t const& type_of() { static rtti_t const& info = rtti::regist(type_name_t(_NAME), gender_of<_TYPE>(), size_of<_TYPE>(), align_of<_TYPE>()); return info; } \
};}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(_TYPE) \
namespace ang { namespace runtime{ \
ang::runtime::rtti_t const& type_info_builder<_TYPE, ang::gender_of<_TYPE>()>::type_of() { static rtti_t const& info = rtti::regist(ang::runtime::type_name_t(#_TYPE), gender_of<_TYPE>(), size_of<_TYPE>(), align_of<_TYPE>()); return info; } \
}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(_TYPE, _NAME) \
namespace ang { namespace runtime{ \
ang::runtime::rtti_t const& type_info_builder<_TYPE, ang::gender_of<_TYPE>()>::type_of() { static rtti_t const& info = rtti::regist(ang::runtime::type_name_t(_NAME), gender_of<_TYPE>(), size_of<_TYPE>(), align_of<_TYPE>()); return info; } \
}}


ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, void);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, pointer);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, char);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, byte);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, wchar);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, char16_t);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, char32_t);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, short);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ushort);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, int);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, uint);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, long);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ulong);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, long64);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ulong64);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, float);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, double);


ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<char>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<byte>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<wchar>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<char16_t>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<char32_t>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<short>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<ushort>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<int>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<uint>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<long>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<ulong>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<long64>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<ulong64>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<float>);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::array_view<double>);


#endif//__ANG_BASE_RUNTIME_H__
