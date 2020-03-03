/*********************************************************************************************************************/
/*   File Name: ang/base/utils.h                                                                                     */
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
#elif !defined __ANG_BASE_UTILS_H__
#define __ANG_BASE_UTILS_H__

#include <type_traits>

namespace ang //constants
{
	template<typename...>
	struct voider { using type = void; };

	//void type
	template <typename... Ts> using void_t = typename voider<Ts...>::type;

	template<typename T, T VALUE>
	using integer_constant = std::integral_constant<T, VALUE>;

	typedef integer_constant<bool, true> true_type;
	typedef integer_constant<bool, false> false_type;

	typedef integer_constant<long64, -1> invalid_index_type;
	invalid_index_type const invalid_index = {};

	struct invalid_handle_t : integer_constant<wsize, ~wsize(0)> {
		template<class T>
		constexpr inline operator T*() const {
			return reinterpret_cast<T*>(value);
		}

		template<class C, class T>
		constexpr inline operator T C::*() const {
			return reinterpret_cast<T C::*>(value);
		}

	private:
		void operator&() const;
	};
	constexpr invalid_handle_t invalid_handle = {};

	template<wsize IDX> using index_type = integer_constant<wsize, IDX>;

	typedef decltype(nullptr) nullptr_t;
	nullptr_t constexpr null = {};

	template<typename T> using initializer_list = std::initializer_list<T>;

}

namespace ang //type utils
{
	using std::is_const;
	using std::is_reference;
	using std::is_pointer;
	using std::remove_const;
	using std::remove_reference;
	using std::remove_pointer;
	using std::is_lvalue_reference;

	template<typename T> 
	struct add_reference { 
		using lvalue_type = typename std::add_lvalue_reference<T>::type;
		using rvalue_type = typename std::add_rvalue_reference<T>::type;
	};

	template<typename T> typename add_reference<T>::rvalue_type declval() noexcept;
}

namespace ang //move operations
{
	using std::move;
	using std::forward;
}

namespace ang //expressions
{
	template<bool VALUE> struct yes_expression : true_type { };
	template<> struct yes_expression<false> : false_type { };

	template<bool...VALUE> struct and_expression;
	template<bool VALUE1, bool VALUE2> struct __and_expression : false_type { };
	template<> struct __and_expression<true, true> : true_type { };
	template<> struct and_expression<> : false_type { };
	template<bool VALUE> struct and_expression<VALUE> : yes_expression<VALUE> { };
	template<bool VALUE1, bool VALUE2> struct and_expression<VALUE1, VALUE2> : __and_expression<VALUE1, VALUE2> { };
	template<bool VALUE1, bool...VALUES>  struct and_expression<VALUE1, VALUES...> : __and_expression<VALUE1, and_expression<VALUES...>::value> { };

	template<bool...VALUE> struct or_expression;
	template<bool VALUE1, bool VALUE2> struct __or_expression : true_type { };
	template<> struct __or_expression<false, false> : false_type { };
	template<> struct or_expression<> : false_type { };
	template<bool VALUE> struct or_expression<VALUE> : yes_expression<VALUE> { };
	template<bool VALUE1, bool VALUE2> struct or_expression<VALUE1, VALUE2> : __or_expression<VALUE1, VALUE2> { };
	template<bool VALUE1, bool...VALUES>  struct or_expression<VALUE1, VALUES...> : __or_expression<VALUE1, or_expression<VALUES...>::value> { };

	template<bool VALUE1, bool VALUE2> struct xor_expression : false_type { };
	template<> struct xor_expression<true, false> : true_type { };
	template<> struct xor_expression<false, true> : true_type { };


	template<bool VALUE> struct not_expression : false_type { };
	template<> struct not_expression<false> : true_type { };

	template<bool...VALUE> struct nand_expression : not_expression<and_expression<VALUE...>::value> { };

	template<bool...VALUE> struct nor_expression : not_expression<or_expression<VALUE...>::value> { };

	template<bool VALUE1, bool VALUE2> struct xnor_expression : not_expression<xor_expression<VALUE1, VALUE2>::value> { };

	template<bool TEST, typename T> using enable_if = std::enable_if_t<TEST, T>;

	template<bool TEST, typename T1, typename T2>
	using type_selector = std::conditional<TEST, T1, T2>;
}

namespace ang //comparision
{
	template<typename T, T N1, T N2, bool EXPRESSION> struct value_selector;

	template<typename T, T N1, T N2> struct value_selector<T, N1, N2, true> {
		static constexpr T value = N1;
	};

	template<typename T, T N1, T N2> struct value_selector<T, N1, N2, false> {
		static constexpr T value = N2;
	};

	template<wsize N1, wsize N2> inline constexpr wsize min() { return value_selector<wsize, N1, N2, (N1 < N2)>::value; }
	template<wsize N1, wsize N2> inline constexpr wsize max() { return value_selector<wsize, N1, N2, (N1 > N2)>::value; }

	template<typename T1, typename T2>
	struct __min_max_type {
		using type = decltype(declval<T1>() + declval<T2>());
	};

	template<typename T>
	struct __min_max_type<T*, T*> {
		using type = T*;
	};

	template<typename T, T VALUE1, T VALUE2> struct is_same : yes_expression<(VALUE1 == VALUE2)> { };
	template<typename T, T VALUE1, T VALUE2> struct is_diferent : yes_expression<(VALUE1 != VALUE2)> { };
	template<typename T, T VALUE1, T VALUE2> struct is_greater : yes_expression<(VALUE1 > VALUE2)> { };
	template<typename T, T VALUE1, T VALUE2> struct is_less : yes_expression<(VALUE1 < VALUE2)> { };


	template<typename T1, typename T2> inline auto min(T1 a, T2 b) { typedef typename __min_max_type<T1,T2>::type r_t; return (a < b ? r_t(a) : r_t(b)); }
	template<typename T1, typename T2> inline auto max(T1 a, T2 b) { typedef typename __min_max_type<T1, T2>::type r_t; return (a > b ? r_t(a) : r_t(b)); }

	template<wsize ALIGMENT, wsize VALUE> constexpr wsize align_up() {
		static_assert(ALIGMENT != 0, "Error: align_to<0>(...)");
		return (VALUE % ALIGMENT) && VALUE ? VALUE + ALIGMENT - (VALUE % ALIGMENT) : VALUE;
	}

	template<wsize ALIGMENT> wsize align_up(wsize VALUE) {
		static_assert(ALIGMENT != 0, "Error: align_to<0>(...)");
		return (VALUE % ALIGMENT) && VALUE ? VALUE + ALIGMENT - (VALUE % ALIGMENT) : VALUE;
	}

	template<typename T1, typename T2> inline auto align_up(T1 ALIGMENT, T2 VALUE) -> decltype(VALUE + ALIGMENT) {
		return (VALUE % ALIGMENT) && VALUE ? VALUE + ALIGMENT - (VALUE % ALIGMENT) : VALUE;
	}

	template<wsize ALIGMENT, wsize VALUE> constexpr wsize align_down() {
		static_assert(ALIGMENT != 0, "Error: align_to<0>(...)");
		return (VALUE % ALIGMENT) && VALUE ? VALUE - (VALUE % ALIGMENT) : VALUE;
	}

	template<wsize ALIGMENT> wsize align_down(wsize VALUE) {
		static_assert(ALIGMENT != 0, "Error: align_to<0>(...)");
		return (VALUE % ALIGMENT) && VALUE ? VALUE - (VALUE % ALIGMENT) : VALUE;
	}

	template<typename T1, typename T2> inline auto align_down(T1 ALIGMENT, T2 VALUE) -> decltype(VALUE + ALIGMENT) {
		return (VALUE % ALIGMENT) && VALUE ? VALUE - (VALUE % ALIGMENT) : VALUE;
	}

}

namespace ang //testing
{
	typedef enum class gender {
		value_type = 0,
		enum_type,
		union_type,
		class_type,
		function_type,
	}gender_t;


	template<typename> struct is_void : false_type { };
	template<> struct is_void<void> : true_type { };
	template<> struct is_void<const void> : true_type { };

	template<typename T1, typename T2> struct is_same_type : false_type { };
	template<typename T> struct is_same_type<T,T> : true_type { };

	using std::underlying_type;
	using std::is_enum;
	using std::is_union;
	using std::is_class;
	using std::is_abstract;
	using std::is_final;
	using std::is_constructible;
	using std::is_copy_constructible;
	using std::is_default_constructible;
	using std::is_assignable;
	using std::is_copy_assignable;
	using std::is_move_assignable;

	using std::is_trivially_copyable;
	using std::is_trivially_constructible;
	using std::is_trivially_copy_constructible;
	using std::is_trivially_default_constructible;
	using std::is_trivially_move_constructible;
	using std::is_trivially_assignable;
	using std::is_trivially_copy_assignable;
	using std::is_trivially_move_assignable;
	using std::is_trivially_destructible;
	//using std::is_base_of;

	template<typename T> using enum_type = typename underlying_type<T>::type;

	template<typename T, typename = void> struct is_complete_impl : false_type {};
	template<typename T> struct is_complete_impl<T, void_t<integer_constant<bool, sizeof(T) == sizeof(T)>>> : true_type { };
	template <typename T> constexpr bool ___is_complete() { return is_complete_impl<T>::value; }
	template <typename T> struct is_complete : yes_expression<___is_complete<T>()> { };

	template<typename T, bool IS_COMPLETE = is_complete<T>::value> struct is_abstract_safe : is_abstract<T> {};
	template<typename T> struct is_abstract_safe<T,false> : true_type {};

	template<typename base_type, typename child_type>
	struct is_base_of : std::is_base_of<base_type, child_type >{
	};

	template<typename child_type, typename base_type>
	using is_inherited_from = is_base_of<base_type, child_type>;

	template<typename T1, typename T2>
	struct are_convertible_types
		: public integer_constant<bool, 
		is_same_type<T1,T2>::value ||
		is_inherited_from<T1, T2>::value ||
		is_inherited_from<T2, T1>::value> {
	};

	template<typename T, typename = void> struct is_functor : false_type { };
	template<typename T> struct is_functor<T, void_t<decltype(&T::operator())>> : true_type { };

	template<typename T> struct is_function : false_type { };
	template<typename T, typename... Ts> struct is_function<T(Ts...)> : true_type { };
	template<typename T, typename... Ts> struct is_function<T(*)(Ts...)> : true_type { };
	template<typename T, typename O, typename... Ts> struct is_function<T(O::*)(Ts...)> : true_type { };
#if defined WINDOWS_PLATFORM && defined _X86_
	template<typename T, typename... Ts> struct is_function<T(__stdcall*)(Ts...)> : true_type { };
	template<typename T, typename... Ts> struct is_function<T(__vectorcall*)(Ts...)> : true_type { };
	template<typename T, typename... Ts> struct is_function<T(__fastcall*)(Ts...)> : true_type { };
#endif
	template<typename T>
	struct is_calleable : or_expression<
		is_function<T>::value,
		is_functor<T>::value> {
	};


	template<
		typename T,
		bool = is_enum<T>::value,
		bool = is_union<T>::value,
		bool = is_class<T>::value,
		bool = is_function<T>::value
	> struct __gender_of {
		static constexpr gender_t value = gender::value_type;
	};

	template<typename T> struct __gender_of<T, true, false, false, false> {
		static constexpr gender_t value = gender::enum_type;
	};

	template<typename T> struct __gender_of<T, false, true, false, false> {
		static constexpr gender_t value = gender::union_type;
	};

	template<typename T> struct __gender_of<T, false, false, true, false> {
		static constexpr gender_t value = gender::class_type;
	};

	template<typename T> struct __gender_of<T, false, false, false, true> {
		static constexpr gender_t value = gender::function_type;
	};

	template<typename T> inline constexpr gender_t gender_of() { return __gender_of<T>::value; }
	template<typename T> inline constexpr gender_t gender_of(T const&) { return __gender_of<T>::value; }

	template<typename T>
	struct is_value_type : or_expression<gender_of<T>() == gender::value_type, gender_of<T>() == gender::enum_type> { };

	template<typename T> struct is_integer_value : false_type { };
	
	template<> struct is_integer_value<ang_int8_t> : true_type { };
	template<> struct is_integer_value<ang_uint8_t> : true_type { };
	template<> struct is_integer_value<ang_int16_t> : true_type { };
	template<> struct is_integer_value<ang_uint16_t> : true_type { };
	template<> struct is_integer_value<ang_int32_t> : true_type { };
	template<> struct is_integer_value<ang_uint32_t> : true_type { };
	template<> struct is_integer_value<ang_lint32_t> : true_type { };
	template<> struct is_integer_value<ang_ulint32_t> : true_type { };
	template<> struct is_integer_value<ang_int64_t> : true_type { };
	template<> struct is_integer_value<ang_uint64_t> : true_type { };

	template<typename T> struct is_unsigned_value : false_type { };
	template<> struct is_unsigned_value<ang_uint8_t> : true_type { };
	template<> struct is_unsigned_value<ang_uint16_t> : true_type { };
	template<> struct is_unsigned_value<ang_uint32_t> : true_type { };
	template<> struct is_unsigned_value<ang_ulint32_t> : true_type { };
	template<> struct is_unsigned_value<ang_uint64_t> : true_type { };

	template<typename T> struct is_floating_value : false_type { };
	template<> struct is_floating_value<ang_float32_t> : true_type { };
	template<> struct is_floating_value<ang_float64_t> : true_type { };


	template<typename T, gender_t TYPE = gender_of<T>()> struct value;

	template<typename T, bool IS_COMPLETE = is_complete<T>::value>
	struct size_of_impl { static constexpr wsize value = sizeof(T); };

	template<typename T>
	struct size_of_impl<T, false> { static constexpr wsize value = 0; };

	template<typename T, bool IS_ABSTRACT = is_abstract_safe<T>::value>
	struct align_of_impl { static constexpr wsize value = alignof(T); };

	template<typename T>
	struct align_of_impl<T, true> { static constexpr wsize value = alignof(wsize); };

	template<typename T> constexpr wsize size_of() { return size_of_impl<T>::value; }
	template<typename T> constexpr wsize align_of() { return align_of_impl<T>::value; }

	template<typename T> constexpr wsize size_of(T const&) { return size_of_impl<T>::value; }
	template<typename T> constexpr wsize align_of(T const&) { return align_of_impl<T>::value; }

	template<typename T, wsize N> constexpr wsize size_of(T(&)[N]) { return size_of_impl<T>::value * N; }
	template<typename T, wsize N> constexpr wsize size_of(const T(&)[N]) { return size_of_impl<T>::value * N; }
}

namespace ang //operations
{
	typedef enum class logic_operation_type : uint
	{
		affirmation,
		negation,
		same,
		diferent,
		same_or_minor,
		same_or_major,
		minor,
		major
	}logic_operation_t;

	typedef enum class boolean_operation_type : uint
	{
		not_operation,
		and_operation,
		or_operation,
		xor_operation,
		nan_operation,
		nor_operation,
		xnor_operation,
	}boolean_operation_t;

	typedef enum class arithmetic_operation_type : uint
	{
		negative,
		addition,
		substraction,
		mutiplication,
		division,
	}arithmetic_operation_t;

	template<logic_operation_type TYPE, typename T1, typename T2 = T1, typename = void> struct has_logic_operation : false_type { };
	template<typename T> struct has_logic_operation<logic_operation_type::affirmation, T, T, void_t<decltype(!!declval<T>())>> : true_type { };
	template<typename T> struct has_logic_operation<logic_operation_type::negation, T, T, void_t<decltype(!declval<T>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::same, T1, T2, void_t<decltype(declval<T1>() == declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::diferent, T1, T2, void_t<decltype(declval<T1>() != declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::same_or_minor, T1, T2, void_t<decltype(declval<T1>() <= declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::same_or_major, T1, T2, void_t<decltype(declval<T1>() >= declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::minor, T1, T2, void_t<decltype(declval<T1>() < declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::major, T1, T2, void_t<decltype(declval<T1>() > declval<T2>())>> : true_type { };
	
	template<logic_operation_type TYPE, typename T1, typename T2, bool VALUE1 = is_value_type<T1>::value, bool VALUE2 = is_value_type<T2>::value>
	struct logic_operation;

	template<logic_operation_type TYPE, typename T1, typename T2, bool VALUE = has_logic_operation<TYPE, T1, T2>::value>
	struct __logic_operation_helper;


	template<typename T> struct __logic_operation_helper<logic_operation_type::affirmation, T, T, false> {
		static bool ANG_DEPRECATE(operate, "template parameter T has no logic operator")(const T& value) { return (T const*)&value != null; }
	};

	template<typename T> struct __logic_operation_helper<logic_operation_type::negation, T, T, false> {
		static bool ANG_DEPRECATE(operate, "template parameter T has no logic operator")(const T& value) { return (T const*)&value == null; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::same, T1, T2, false> {
		static bool ANG_DEPRECATE(operate, "template parameter T has no logic operator")(const T1& value1, const T2& value2) { return memcmp(&value1,&value2, min<size_of<T1>(), size_of<T2>()>()) == 0; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::diferent, T1, T2,  false> {
		static bool ANG_DEPRECATE(operate, "template parameter T has no logic operator")(const T1& value1, const T2& value2) { return memcmp(&value1, &value2, min<size_of<T1>(), size_of<T2>()>()) != 0; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::same_or_minor, T1, T2, false> {
		static bool ANG_DEPRECATE(operate, "template parameter T has no logic operator")(const T1& value1, const T2& value2) { return memcmp(&value1, &value2, min<size_of<T1>(), size_of<T2>()>()) <= 0; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::same_or_major, T1, T2, false> {
		static bool ANG_DEPRECATE(operate, "template parameter T has no logic operator")(const T1& value1, const T2& value2) { return memcmp(&value1, &value2, min<size_of<T1>(), size_of<T2>()>()) >= 0; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::minor, T1, T2, false> {
		static bool ANG_DEPRECATE(operate, "template parameter T has no logic operator")(const T1& value1, const T2& value2) { return memcmp(&value1, &value2, min<size_of<T1>(), size_of<T2>()>()) < 0; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::major, T1, T2, false> {
		static bool ANG_DEPRECATE(operate, "template parameter T has no logic operator")(const T1& value1, const T2& value2) { return memcmp(&value1, &value2, min<size_of<T1>(), size_of<T2>()>()) > 0; }
	};

	template<typename T> struct __logic_operation_helper<logic_operation_type::affirmation, T, T, true> {
		static constexpr bool operate(const T& value) { return !!value; }
	};

	template<typename T> struct __logic_operation_helper<logic_operation_type::negation, T, T, true> {
		static constexpr bool operate(const T& value) { return !value; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::same, T1, T2, true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 == value2; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::diferent, T1, T2,  true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 != value2; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::same_or_minor, T1, T2,  true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 <= value2; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::same_or_major, T1, T2,  true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 >= value2; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::minor, T1, T2, true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 < value2; }
	};

	template<typename T1, typename T2> struct __logic_operation_helper<logic_operation_type::major, T1, T2, true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 > value2; }
	};

	template<logic_operation_type TYPE, typename T1, typename T2, bool VALUE1, bool VALUE2> struct logic_operation {
		static constexpr bool operate(const T1& value1, const T2& value2) { return __logic_operation_helper<TYPE, T1, T2>::operate(value1, value2); }
		constexpr bool operator()(const T1& value1, const T2& value2)const { return operate(value1, value2); }
	};

	template<typename T, bool VALUE> struct logic_operation<logic_operation_type::negation, T,T, VALUE, VALUE> {
		static constexpr bool operate(const T& value) { return __logic_operation_helper<logic_operation_type::negation, T, T>::operate(value); }
		constexpr bool operator()(const T& value1)const { return operate(value1); }
	};

	template<typename T>
	struct logic_operation<logic_operation_type::affirmation, T, T, true, true> {
		static constexpr bool operate(const T& value) { return !!value; }
		constexpr bool operator()(const T& value1)const { return operate(value1); }
	};

	template<typename T>
	struct logic_operation<logic_operation_type::negation, T, T, true, true> {
		static constexpr bool operate(const T& value) { return !value; }
		constexpr bool operator()(const T& value1)const { return operate(value1); }
	};

	template<typename T1, typename T2>
	struct logic_operation<logic_operation_type::same, T1, T2, true, true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 == (T1)value2; }
		constexpr bool operator()(const T1& value1, const T2& value2)const { return operate(value1, value2); }
	};

	template<typename T1, typename T2>
	struct logic_operation<logic_operation_type::diferent, T1, T2, true, true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 != (T1)value2; }
		constexpr bool operator()(const T1& value1, const T2& value2)const { return operate(value1, value2); }
	};

	template<typename T1, typename T2>
	struct logic_operation<logic_operation_type::same_or_major, T1, T2, true, true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 >= (T1)value2; }
		constexpr bool operator()(const T1& value1, const T2& value2)const { return operate(value1, value2); }
	};

	template<typename T1, typename T2>
	struct logic_operation<logic_operation_type::same_or_minor, T1, T2, true, true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 <= (T1)value2; }
		constexpr bool operator()(const T1& value1, const T2& value2)const { return operate(value1, value2); }
	};

	template<typename T1, typename T2>
	struct logic_operation<logic_operation_type::major, T1, T2, true, true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 > (T1)value2; }
		constexpr bool operator()(const T1& value1, const T2& value2)const { return operate(value1, value2); }
	};

	template<typename T1, typename T2>
	struct logic_operation<logic_operation_type::minor, T1, T2, true, true> {
		static constexpr bool operate(const T1& value1, const T2& value2) { return value1 < (T1)value2; }
		constexpr bool operator()(const T1& value1, const T2& value2)const { return operate(value1, value2); }
	};


	template<typename T> using negate = logic_operation<logic_operation_type::negation, T, T>;
	template<typename T1, typename T2 = T1> using equal_to = logic_operation<logic_operation_type::same, T1, T2>;
	template<typename T1, typename T2 = T1> using not_equal_to = logic_operation<logic_operation_type::diferent, T1, T2>;
	template<typename T1, typename T2 = T1> using less_equal = logic_operation<logic_operation_type::same_or_minor, T1, T2>;
	template<typename T1, typename T2 = T1> using greater_equal = logic_operation<logic_operation_type::same_or_major, T1, T2>;
	template<typename T1, typename T2 = T1> using less = logic_operation<logic_operation_type::minor, T1, T2>;
	template<typename T1, typename T2 = T1> using greater = logic_operation<logic_operation_type::major, T1, T2>;


	template<boolean_operation_type TYPE, typename T1, typename T2 = T1, typename = void> struct has_boolean_operation : false_type { };
	template<typename T> struct has_boolean_operation<boolean_operation_type::not_operation, T, T, void_t<decltype(~declval<T>())>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::and_operation, T1, T2, void_t<decltype(declval<T1>() & declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::or_operation, T1, T2, void_t<decltype(declval<T1>() | declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::xor_operation, T1, T2, void_t<decltype(declval<T1>() ^ declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::nan_operation, T1, T2, void_t<decltype(~(declval<T1>() & declval<T2>()))>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::nor_operation, T1, T2, void_t<decltype(~(declval<T1>() | declval<T2>()))>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::xnor_operation, T1, T2, void_t<decltype(~(declval<T1>() ^ declval<T2>()))>> : true_type { };

	template<typename...Ts> struct __and_operation;
	template<typename...Ts>
	struct __and_operation<bool, Ts...> {
		static inline bool operate(bool first, Ts&&... rest) {
			return first && __and_operation<Ts...>::operate(ang::forward<Ts>(rest)...);
		}
	};

	template<typename...Ts>
	inline bool and_operation(Ts&&... args) {
		return __and_operation<Ts...>::operate(ang::forward<Ts>(args)...);
	}

	template<typename...Ts> struct or_operation;
	template<typename...Ts>
	struct or_operation<bool, Ts...> {
		static inline bool operate(bool first, Ts&&... rest) {
			return first || or_operation<Ts...>::operate(ang::forward<Ts>(rest)...);
		}
	};

	template<typename T, bool IS_POINTER = is_pointer<T>::value> struct __to_pointer_helper;

	template<typename T>
	struct __to_pointer_helper<T, true> {
		using arg_type = T;
		using ptr_type = T;
		static constexpr T value(arg_type ptr) { return ptr; }
	};

	template<typename T>
	struct __to_pointer_helper<T, false> {
		using arg_type = T & ;
		using ptr_type = T * ;
		static T* value(arg_type val) { return addressof(ang::forward<T>(val)); }
	};

	template<typename T>
	auto to_pointer(typename __to_pointer_helper<T>::arg_type val)->decltype(__to_pointer_helper<T>::value(ang::forward<T>(val))) {
		return __to_pointer_helper<T>::value(ang::forward<T>(val));
	};

}

namespace ang
{

	template<typename T, gender_t G = gender_of<T>()> struct default_value;

	template<typename T>
	struct default_value<T, gender::value_type> {
		static constexpr T value = 0;
		inline operator T const& ()const { return value; }
		inline T const& get()const { return value; }
	};

	template<typename T>
	struct default_value<T, gender::enum_type> {
		static constexpr T value = T(0);
		inline operator T const& ()const { return value; }
		inline T const& get()const { return value; }
	};

	template<typename T>
	struct default_value<T, gender::class_type> {
		static const T value;
		inline operator T const& ()const { return value; }
		inline T const& get()const { return value; }
	};
	template<typename T> const T default_value<T, gender::class_type>::value;


	template<wsize SIZE> struct integer_type_by_size { };

	template<> struct integer_type_by_size<1U> {
		typedef char int_t;
		typedef unsigned char uint_t;
	};

	template<> struct integer_type_by_size<2U> {
		typedef short int_t;
		typedef unsigned short uint_t;
	};

	template<> struct integer_type_by_size<4U> {
		typedef int int_t;
		typedef unsigned int uint_t;
	};

	template<> struct integer_type_by_size<8U> {
		typedef long long int_t;
		typedef unsigned long long uint_t;
	};

	template<typename T, bool IS_FUNCTION = is_function<T>::value>
	struct __adressof {
		static T* value(T& val) {
			return (reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(val))));
		}
	};

	template<typename T>
	struct __adressof<T, true> {
		static T* value(T& val) {
			return std::addressof(val);
		}
	};

	template<typename T>
	inline T *addressof(T& val) noexcept {
		return __adressof<T>::value(val);
	}

	template<typename T> struct _function_type
	{
		typedef T type;
		typedef T* type_ptr;
		static_assert(is_function<T>::value, "");
	};

	template<typename T, typename...Ts> struct _function_type<T(Ts...)>
	{
		using type = T(Ts...);
		using type_ptr = T(*)(Ts...);
	};

	template<typename T> using function_type = typename _function_type<T>::type_ptr;


	template<typename T, typename U>
	constexpr wsize offset_of() {
		static_assert(is_base_of<U, T>::value);
		return reinterpret_cast<wsize>(static_cast<U*>(reinterpret_cast<T*>(0X10000))) - 0X10000;
	}

	template<typename T, typename U>
	constexpr wsize offset_of(U T::*member) {
		return wsize((byte*)&((T*)nullptr->*member) - (byte*)nullptr);
	}

	template<typename T, typename U, typename V>
	T* field_to_parent(U T::*member, V* field) {
		static_assert(are_convertible_types<U, V>::value, "can convert type V to U");
		return (T*)(wsize(field) - offset_of(member));
	}

	

}

#endif//__ANG_BASE_UTILS_H__
