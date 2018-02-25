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

namespace ang //constants
{
	template<typename...>
	struct voider { using type = void; };

	//void type
	template <typename... Ts> using void_t = typename voider<Ts...>::type;

	template<typename T, T VALUE>
	struct integer_constant {
		typedef T value_type;
		typedef integer_constant<T, VALUE> self_type;

		static constexpr value_type value = VALUE;
		constexpr operator value_type() const { return (value); }
		constexpr value_type operator()() const { return (value); }
	};

	typedef integer_constant<bool, true> true_type;
	typedef integer_constant<bool, false> false_type;

	typedef integer_constant<windex, -1> invalid_index_type;
	extern const invalid_index_type invalid_index;

	typedef decltype(nullptr) nullptr_t;
	nullptr_t const null = {};

}

namespace ang //type utils
{
	template<typename T> struct remove_constant { typedef T type; };
	template<typename T> struct remove_constant<const T>	{ typedef T type; };
	template<typename T> struct remove_constant<const T&> { typedef T& type; };
	template<typename T> struct remove_constant<const T*> { typedef T* type; };
	template<typename T> struct remove_constant<T* const> { typedef T* type; };

	template<typename T> struct remove_reference { typedef T type; };
	template<typename T> struct remove_reference<T&> { typedef T type; };
	template<typename T> struct remove_reference<T &&> { typedef T type; };
	
	template<typename T> struct remove_pointer { typedef T type; };
	template<typename T> struct remove_pointer<T*> { typedef T type; };
	template<typename T> struct remove_pointer<T**> { typedef T type; };

	template<typename T> struct is_lvalue_reference : public false_type { };
	template<typename T> struct is_lvalue_reference<T&> : true_type { };

	template<typename T> typename remove_reference<T>::type& declval();
}

namespace ang //move operations
{
	template<typename T>
	constexpr inline typename remove_reference<T>::type&& move(T&& src) { 
		return static_cast<typename remove_reference<T>::type&&>(src);
	}

	template<typename T>
	inline void move(typename remove_reference<T>::type& dest, typename remove_reference<T>::type&& src) {
		dest = move(src);
	}

	template<typename T>
	inline constexpr T&& forward(typename remove_reference<T>::type& _Arg) {
		return (static_cast<T&&>(_Arg));
	}

	template<typename T>
	inline constexpr T&& forward(typename remove_reference<T>::type&& _Arg) {
		static_assert(!is_lvalue_reference<T>::value, "bad forward call");
		return (static_cast<T&&>(_Arg));
	}
}

namespace ang //expressions
{
	template<bool VALUE> struct yes_expression : true_type { };
	template<> struct yes_expression<false> : false_type { };

	template<bool VALUE1, bool VALUE2> struct and_expression : false_type { };
	template<> struct and_expression<true, true> : true_type { };

	template<bool VALUE1, bool VALUE2> struct or_expression : true_type { };
	template<> struct or_expression<false, false> : false_type { };

	template<bool VALUE1, bool VALUE2> struct xor_expression : false_type { };
	template<> struct and_expression<true, false> : true_type { };
	template<> struct and_expression<false, true> : true_type { };


	template<bool VALUE> struct not_expression : false_type { };
	template<> struct not_expression<false> : true_type { };

	template<bool VALUE1, bool VALUE2> struct nand_expression : not_expression<and_expression<VALUE1, VALUE2>::value> { };

	template<bool VALUE1, bool VALUE2> struct nor_expression : not_expression<or_expression<VALUE1, VALUE2>::value> { };

	template<bool VALUE1, bool VALUE2> struct xnor_expression : not_expression<xor_expression<VALUE1, VALUE2>::value> { };


	template<bool VALUE> struct is_valid {};
	template<> struct is_valid<true> : true_type {};
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

	template<typename T1, typename T2> inline auto min(T1 a, T2 b) { typedef decltype(a + b) r_t; return (a < b ? r_t(a) : r_t(b)); }
	template<typename T1, typename T2> inline auto max(T1 a, T2 b) { typedef decltype(a + b) r_t; return (a > b ? r_t(a) : r_t(b)); }

	template<wsize ALIGMENT, wsize VALUE> constexpr wsize align_up() {
		static_assert(ALIGMENT != 0, "Error: align_to<0>(...)");
		return (VALUE % ALIGMENT) && VALUE ? VALUE + ALIGMENT - (VALUE % ALIGMENT) : VALUE;
	}

	template<wsize ALIGMENT> wsize align_up(wsize VALUE) {
		static_assert(ALIGMENT != 0, "Error: align_to<0>(...)");
		return (VALUE % ALIGMENT) && VALUE ? VALUE + ALIGMENT - (VALUE % ALIGMENT) : VALUE;
	}

	template<typename T1, typename T2> inline auto align_up(T1 ALIGMENT, T2 VALUE) -> decltype(VALUE + ALIGMENT) {
		NN_ASSERT(ALIGMENT != 0, "Error: align_to<0>(...)");
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
		NN_ASSERT(ALIGMENT != 0, "Error: align_to<0>(...)");
		return (VALUE % ALIGMENT) && VALUE ? VALUE - (VALUE % ALIGMENT) : VALUE;
	}

}

namespace ang //testing
{
	typedef enum class genre {
		value_type = 0,
		enum_type,
		union_type,
		class_type,
		function_type,
	}genre_t;


	template<typename> struct is_void : false_type { };
	template<> struct is_void<void> : true_type { };
	template<> struct is_void<const void> : true_type { };

	template<typename T1, typename T2> struct is_same_type : false_type { };
	template<typename T> struct is_same_type<T,T> : true_type { };

	template<typename T> struct is_enum : yes_expression<__is_enum(T)> {};
	template<typename T> struct is_union : yes_expression<__is_union(T)> {};
	template<typename T> struct is_class : yes_expression<__is_class(T)> {};

	template<typename base_type, typename child_type>
	struct is_base_of : public yes_expression<__is_base_of(base_type, child_type)> { };

	template<typename child_type, typename base_type>
	struct is_inherited_from : public yes_expression<__is_base_of(base_type, child_type)> { };

	template<typename T, typename = void> struct is_functor : false_type { };
	template<typename T> struct is_functor<T, void_t<decltype(&T::operator())>> : true_type { };

	template<typename T> struct is_function : false_type { };
	template<typename T, typename... Ts> struct is_function<T(Ts...)> : true_type { };
	template<typename T, typename... Ts> struct is_function<T(*)(Ts...)> : true_type { };
	template<typename T, typename O, typename... Ts> struct is_function<T(O::*)(Ts...)> : true_type { };

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
	> struct __genre_of {
		static constexpr genre_t value = genre::value_type;
	};

	template<typename T> struct __genre_of<T, true, false, false, false> {
		static constexpr genre_t value = genre::enum_type;
	};

	template<typename T> struct __genre_of<T, false, true, false, false> {
		static constexpr genre_t value = genre::union_type;
	};

	template<typename T> struct __genre_of<T, false, false, true, false> {
		static constexpr genre_t value = genre::class_type;
	};

	template<typename T> struct __genre_of<T, false, false, false, true> {
		static constexpr genre_t value = genre::function_type;
	};

	template<typename T> inline constexpr genre_t genre_of() { return __genre_of<T>::value; }
	template<typename T> inline constexpr genre_t genre_of(T const&) { return __genre_of<T>::value; }


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

	template<typename T> struct is_floating_value : false_type { };
	template<> struct is_floating_value<ang_float32_t> : true_type { };
	template<> struct is_floating_value<ang_float64_t> : true_type { };

	template<typename T, typename = void>
	struct __is_safe_enum_base : false_type { };

	template<typename T>
	struct __is_safe_enum_base<T, void_t<typename T::type>> : yes_expression<is_enum<typename T::type>::value> { };

	template<typename T, genre_t TYPE = genre_of<T>(), bool IS_SAFE_ENUM = __is_safe_enum_base<T>::value> struct value;
}

namespace ang //operations
{
	typedef enum class logic_operation_type : uint
	{
		not,
		same,
		diferent,
		same_or_minor,
		same_or_major,
		minor,
		major
	}logic_operation_t;

	typedef enum class boolean_operation_type : uint
	{
		not,
		and,
		or,
		xor,
		nan,
		nor,
		xnor,
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
	template<typename T> struct has_logic_operation<logic_operation_type::not, T, T, void_t<decltype(!declval<T>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::same, T1, T2, void_t<decltype(declval<T1>() == declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::diferent, T1, T2, void_t<decltype(declval<T1>() != declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::same_or_minor, T1, T2, void_t<decltype(declval<T1>() <= declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::same_or_major, T1, T2, void_t<decltype(declval<T1>() >= declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::minor, T1, T2, void_t<decltype(declval<T1>() < declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_logic_operation<logic_operation_type::major, T1, T2, void_t<decltype(declval<T1>() > declval<T2>())>> : true_type { };
	

	template<typename T1, typename T2, logic_operation_type TYPE>
	struct logic_operation { static bool operate(const T1&, const T2&) { return false; }  };

	template<typename T> struct logic_operation<T, T, logic_operation_type::not> {
		static_assert(has_logic_operation<logic_operation_type::not, T, T>::value, "template parameter T has no logic operator");
		static bool operate(const T& value) { return !value; }
	};

	template<typename T1, typename T2> struct logic_operation<T1, T2, logic_operation_type::same> {
		static_assert(has_logic_operation<logic_operation_type::same, T1, T2>::value, "template parameter T has no logic operator");
		static bool operate(const T1& value1, const T1& value2) { return value1 == value2; }
	};

	template<typename T1, typename T2> struct logic_operation<T1, T2, logic_operation_type::diferent> {
		static_assert(has_logic_operation<logic_operation_type::diferent, T1, T2>::value, "template parameter T has no logic operator");
		static bool operate(const T1& value1, const T1& value2) { return value1 != value2; }
	};

	template<typename T1, typename T2> struct logic_operation<T1, T2, logic_operation_type::same_or_minor> {
		static_assert(has_logic_operation<logic_operation_type::same_or_minor, T1, T2>::value, "template parameter T has no logic operator");
		static bool operate(const T1& value1, const T1& value2) { return value1 <= value2; }
	};

	template<typename T1, typename T2> struct logic_operation<T1, T2, logic_operation_type::same_or_major> {
		static_assert(has_logic_operation<logic_operation_type::same_or_major, T1, T2>::value, "template parameter T has no logic operator");
		static bool operate(const T1& value1, const T1& value2) { return value1 >= value2; }
	};

	template<typename T1, typename T2> struct logic_operation<T1, T2, logic_operation_type::minor> {
		static_assert(has_logic_operation<logic_operation_type::minor, T1, T2>::value, "template parameter T has no logic operator");
		static bool operate(const T1& value1, const T1& value2) { return value1 < value2; }
	};

	template<typename T1, typename T2> struct logic_operation<T1, T2, logic_operation_type::major> {
		static_assert(has_logic_operation<logic_operation_type::major, T1, T2>::value, "template parameter T has no logic operator");
		static bool operate(const T1& value1, const T1& value2) { return value1 > value2; }
	};


	template<boolean_operation_type TYPE, typename T1, typename T2 = T1, typename = void> struct has_boolean_operation : false_type { };
	template<typename T> struct has_boolean_operation<boolean_operation_type::not, T, T, void_t<decltype(~declval<T>())>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::and, T1, T2, void_t<decltype(declval<T1>() & declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::or, T1, T2, void_t<decltype(declval<T1>() | declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::xor , T1, T2, void_t<decltype(declval<T1>() ^ declval<T2>())>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::nan, T1, T2, void_t<decltype(~(declval<T1>() & declval<T2>()))>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::nor, T1, T2, void_t<decltype(~(declval<T1>() | declval<T2>()))>> : true_type { };
	template<typename T1, typename T2> struct has_boolean_operation<boolean_operation_type::xnor, T1, T2, void_t<decltype(~(declval<T1>() ^ declval<T2>()))>> : true_type { };

}

namespace ang
{
	template<typename T, T VALUE = T(0) >
	struct default_value { 
		static const T value = VALUE;
		inline operator T const& ()const { return value; }
		inline T const& get()const { return value; }
	};

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
}

#endif//__ANG_BASE_UTILS_H__