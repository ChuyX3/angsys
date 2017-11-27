/*********************************************************************************************************************/
/*   File Name: ang/utils.h                                                                                          */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is declare useful templates functions and clases.                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_UTILS_HPP__
#define __ANG_UTILS_HPP__

#define ANG_NUM_ARGS(...) ANG_NUM_ARGS_( __VA_ARGS__, ANG_PP_RSEQ_N())
#define ANG_NUM_ARGS_(...) ANG_PPARG_N( __VA_ARGS__ )

#define ANG_CATAB(A, B) ANG_CATAB_(A, B)
#define ANG_CATAB_(A, B) A##B

#define ANG_PPARG_N( \
        _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,\
		_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,\
		_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,\
		_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,_76,_77,_78,_79,_80,\
		_81,_82,_83,_84,_85,_86,_87,_88,_89,_90,_91,_92,_93,_94,_95,_96,_97,_98,_99,_100,\
		N,...) N

#define ANG_PP_RSEQ_N() \
		100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,\
		80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,\
		60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,\
		40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,\
		20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0

#include <ang/base/inline/APPLY_FUNCX_N.inl>

#define APPLY_FUNCX_(FUNCX, M, ...) M(FUNCX, __VA_ARGS__)
#define APPLY_FUNCX_N(FUNCX, ...) APPLY_FUNCX_(FUNCX, ANG_CATAB(APPLY_FUNCX, ANG_NUM_ARGS(__VA_ARGS__)), __VA_ARGS__ )

#define DECLARE_VARS0() 
#define DECLARE_VARS1(A0) A0 arg0;
#define DECLARE_VARS2(A0,A1) A0 arg0; A1 arg1;
#define DECLARE_VARS3(A0,A1,A2) A0 arg0; A1 arg1; A2 arg2;
#define DECLARE_VARS4(A0,A1,A2,A3) A0 arg0; A1 arg1; A2 arg2; A3 arg3;
#define DECLARE_VARS5(A0,A1,A2,A3,A4) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4;
#define DECLARE_VARS6(A0,A1,A2,A3,A4,A5) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5;
#define DECLARE_VARS7(A0,A1,A2,A3,A4,A5,A6) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5; A6 arg6;
#define DECLARE_VARS8(A0,A1,A2,A3,A4,A5,A6,A7) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5; A6 arg6; A7 arg7;
#define DECLARE_VARS9(A0,A1,A2,A3,A4,A5,A6,A7,A8) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5; A6 arg6; A7 arg7; A8 arg8;
#define DECLARE_VARS10(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9) A0 arg0; A1 arg1; A2 arg2; A3 arg3; A4 arg4; A5 arg5; A6 arg6; A7 arg7; A8 arg8; A9 arg9;
#define DECLARE_VARS_(_DECLARE_VARS_, ...) _DECLARE_VARS_(__VA_ARGS__)
#define DECLARE_VARSN(...) DECLARE_VARS_(ANG_CATAB(DECLARE_VARS, ANG_NUM_ARGS(__VA_ARGS__)), __VA_ARGS__)

#define FUNCX_TOSTRING(A0) #A0
#define FUNCX_TYPENAME(A0) typename A0

#define TEMPLATE(...) template<APPLY_FUNCX_N(FUNCX_TYPENAME, __VA_ARGS__)>


namespace ang
{

	template <typename... Ts> using void_t = void;

	template<typename T, T _Val>
	struct integer_constant {
		static constexpr T value = _Val;
		typedef T value_type;
		typedef integer_constant<T, _Val> type;
		constexpr operator value_type() const { return (value); }
		constexpr value_type operator()() const { return (value); }
		static constexpr value_type get() { return (value); }
	};

	typedef integer_constant<bool, true> true_type;
	typedef integer_constant<bool, false> false_type;


	template<bool VALUE, typename = void>
	struct rule {
		rule() { static_assert(VALUE, "ERROR..."); }
	};

	template<typename T>
	struct remove_constant
	{
		typedef T type;
	};

	template<typename T>
	struct remove_constant<const T>
	{
		typedef T type;
	};

	template<typename T>
	struct remove_constant<const T&>
	{
		typedef T& type;
	};

	template<typename T>
	struct remove_constant<const T*>
	{
		typedef T* type;
	};

	template<typename T>
	struct remove_constant<T* const>
	{
		typedef T* type;
	};

	template<typename T>
	struct remove_reference
	{
		typedef T type;
	};

	template<typename T>
	struct remove_reference<T&>
	{
		typedef T type;
	};

	template<typename T>
	struct remove_reference<T &&>
	{
		typedef T type;
	};

	template<typename T>
	struct remove_pointer
	{
		typedef T type;
	};

	template<typename T>
	struct remove_pointer<T*>
	{
		typedef T type;
	};

	template<typename T>
	struct remove_pointer<T**>
	{
		typedef T type;
	};

	// TEMPLATE CLASS is_lvalue_reference
	template<typename T>
	struct is_lvalue_reference
		: public false_type
	{	// determine whether T is an lvalue reference
	};

	template<typename T>
	struct is_lvalue_reference<T&>
		: true_type
	{	// determine whether T is an lvalue reference
	};

	template<typename T>
	typename remove_reference<T>::type& declval();

	template<typename T>
	constexpr inline typename remove_reference<T>::type&& move(T&& src) { return static_cast<typename remove_reference<T>::type&&>(src); }

	template<typename T>
	inline void move(typename remove_reference<T>::type& dest, typename remove_reference<T>::type&& src) { dest = move(src); }

	template<typename T> inline
		constexpr T&& forward(
			typename remove_reference<T>::type& _Arg)
	{	// forward an lvalue as either an lvalue or an rvalue
		return (static_cast<T&&>(_Arg));
	}

	template<typename T> inline
		constexpr T&& forward(
			typename remove_reference<T>::type&& _Arg)
	{	// forward an rvalue as an rvalue
		static_assert(!is_lvalue_reference<T>::value, "bad forward call");
		return (static_cast<T&&>(_Arg));
	}

	

	template<typename>
	struct integer_value
	{
		static const bool is_integer_value = false;
	};

	template<typename>
	struct floating_value
	{
		static const bool if_floating_value = false;
	};

	template<> struct integer_value<bool> { static const bool is_integer_value = true; };
	template<> struct integer_value<char> { static const bool is_integer_value = true; };
	template<> struct integer_value<uchar> { static const bool is_integer_value = true; };
	template<> struct integer_value<wchar_t> { static const bool is_integer_value = true; };
	template<> struct integer_value<char16_t> { static const bool is_integer_value = true; };
	template<> struct integer_value<char32_t> { static const bool is_integer_value = true; };
	template<> struct integer_value<short> { static const bool is_integer_value = true; };
	template<> struct integer_value<ushort> { static const bool is_integer_value = true; };
	template<> struct integer_value<int> { static const bool is_integer_value = true; };
	template<> struct integer_value<uint> { static const bool is_integer_value = true; };
	template<> struct integer_value<long> { static const bool is_integer_value = true; };
	template<> struct integer_value<ulong> { static const bool is_integer_value = true; };
	template<> struct integer_value<long64> { static const bool is_integer_value = true; };
	template<> struct integer_value<ulong64> { static const bool is_integer_value = true; };

	template<typename T> struct char_value : public false_type{};
	template<typename T> struct char_value<const T> : public char_value<T> {};
	template<typename T> struct char_value<T&> : public char_value<T> {};
	template<typename T> struct char_value<const T&> : public char_value<T> {};

	template<> struct char_value<char> : public true_type {};
	template<> struct char_value<byte> : public true_type {};
	template<> struct char_value<wchar> : public true_type {};
	template<> struct char_value<char16_t> : public true_type {};
	template<> struct char_value<char32_t> : public true_type {};

	enum comparision_type
	{
		comparision_same = 0,
		comparision_diferent,
		comparision_same_or_minor,
		comparision_same_or_major,
		comparision_minor,
		comparision_major,
	};

	template<typename T1, typename T2, comparision_type, typename = void>
	struct has_comparision_operator : public false_type { };

	template <typename T1, typename T2> struct has_comparision_operator<T1, T2, comparision_same, void_t<decltype(declval<T1>() == declval<T2>())> > : public true_type {};
	template <typename T1, typename T2> struct has_comparision_operator<T1, T2, comparision_diferent, void_t<decltype(declval<T1>() != declval<T2>())> > : public true_type {};
	template <typename T1, typename T2> struct has_comparision_operator<T1, T2, comparision_same_or_minor, void_t<decltype(declval<T1>() <= declval<T2>())> > : public true_type {};
	template <typename T1, typename T2> struct has_comparision_operator<T1, T2, comparision_same_or_major, void_t<decltype(declval<T1>() >= declval<T2>())> > : public true_type {};
	template <typename T1, typename T2> struct has_comparision_operator<T1, T2, comparision_minor, void_t<decltype(declval<T1>() < declval<T2>())> > : public true_type {};
	template <typename T1, typename T2> struct has_comparision_operator<T1, T2, comparision_major, void_t<decltype(declval<T1>() > declval<T2>())> > : public true_type {};

	template<typename T, T N1, T N2, comparision_type TYPE, bool HAS_COMPARISION_OPERATOR = has_comparision_operator<T, T, TYPE>::value >
	struct comparision { static constexpr bool compare() { static_assert(HAS_COMPARISION_OPERATOR, "this type has no comparision operator"); return false; } };

	template<typename T, T N1, T N2>
	struct comparision<T, N1, N2, comparision_same, true> { static constexpr bool compare() { return N1 == N2; } };

	template<typename T, T N1, T N2>
	struct comparision<T, N1, N2, comparision_diferent, true> { static constexpr bool compare() { return N1 != N2; } };

	template<typename T, T N1, T N2>
	struct comparision<T, N1, N2, comparision_same_or_minor, true> { static constexpr bool compare() { return N1 <= N2; } };

	template<typename T, T N1, T N2>
	struct comparision<T, N1, N2, comparision_same_or_major, true> { static constexpr bool compare() { return N1 >= N2; } };

	template<typename T, T N1, T N2>
	struct comparision<T, N1, N2, comparision_minor, true> { static constexpr bool compare() { return N1 < N2; } };

	template<typename T, T N1, T N2>
	struct comparision<T, N1, N2, comparision_major, true> { static constexpr bool compare() { return N1 > N2; } };

	template<typename T, T N1, T N2, bool val> struct value_selector;

	template<typename T, T N1, T N2> struct value_selector<T, N1, N2, true> {
		static constexpr T select() { return  N1; }
	};

	template<typename T, T N1, T N2> struct value_selector<T, N1, N2, false> {
		static constexpr T select() { return  N2; }
	};

	template<wsize N1, wsize N2> constexpr wsize select_min_size() { return value_selector<wsize, N1, N2, comparision<wsize, N1, N2, comparision_minor>::compare()>::select(); }
	template<wsize N1, wsize N2> constexpr wsize select_max_size() { return value_selector<wsize, N1, N2, comparision<wsize, N1, N2, comparision_major>::compare()>::select(); }

	template<typename T1, typename T2, comparision_type TYPE, bool HAS_COMPARISION_OPERATOR = has_comparision_operator<T1,T2, TYPE>::value>
	struct comparision_operation { static bool compare(const T1&, const T2&) { return false; } };

	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_same, true> { static bool compare(const T1& a1, const T2& a2) { return a1 == a2; } };
	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_same, false> { static bool compare(const T1& a1, const T2& a2) { return memcmp((void*)(T1 const*)&a1, (void*)(T2 const*)&a2, select_min_size<sizeof(T1), sizeof(T2)>()) == 0; } };

	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_diferent, true> { static bool compare(const T1& a1, const T2& a2) { return a1 != a2; } };
	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_diferent, false> { static bool compare(const T1& a1, const T2& a2) { return memcmp((void*)(T1 const*)&a1, (void*)(T2 const*)&a2, select_min_size<sizeof(T1), sizeof(T2)>()) != 0; } };

	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_same_or_minor, true> { static bool compare(const T1& a1, const T2& a2) { return a1 <= a2; } };
	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_same_or_minor, false> { static bool compare(const T1& a1, const T2& a2) { return memcmp((void*)(T1 const*)&a1, (void*)(T2 const*)&a2, select_min_size<sizeof(T1), sizeof(T2)>()) <= 0; } };

	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_same_or_major, true> { static bool compare(const T1& a1, const T2& a2) { return a1 >= a2; } };
	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_same_or_major, false> { static bool compare(const T1& a1, const T2& a2) { return memcmp((void*)(T1 const*)&a1, (void*)(T2 const*)&a2, select_min_size<sizeof(T1), sizeof(T2)>()) >= 0; } };

	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_minor, true> { static bool compare(const T1& a1, const T2& a2) { return a1 < a2; } };
	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_minor, false> { static bool compare(const T1& a1, const T2& a2) { return memcmp((void*)(T1 const*)&a1, (void*)(T2 const*)&a2, select_min_size<sizeof(T1), sizeof(T2)>()) < 0; } };

	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_major, true> { static bool compare(const T1& a1, const T2& a2) { return a1 > a2; } };
	template<typename T1, typename T2> struct comparision_operation<T1, T2, comparision_major, false> { static bool compare(const T1& a1, const T2& a2) { return memcmp((void*)(T1 const*)&a1, (void*)(T2 const*)&a2, sizeof(T2)) > 0; } };

	template<typename T1, typename T2>
	struct comparision_operations
	{
		template<comparision_type TYPE> static bool compare(T1 const& a1, T2 const& a2) { return comparision_operation<T1, T2, TYPE>::compare(a1, a2); }
	};


	template<typename T1, typename T2>
	struct operation_result
	{
		typedef decltype(declval<T1>() + declval<T2>()) type;
	};

	template<typename T1, typename T2>
	inline typename operation_result<T1,T2>::type min(T1 a, T2 b) { typedef typename operation_result<T1, T2>::type r_t; return move(a < b ? r_t(a) : r_t(b)); }

	template<typename T1, typename T2>
	inline typename operation_result<T1, T2>::type max(T1 a, T2 b) { typedef typename operation_result<T1, T2>::type r_t; return move(a > b ? r_t(a) : r_t(b)); }

	template<typename T>
	inline void swap(T& a, T&b) {
		//&a != &b ? (a ^= b, b ^= a, a ^= b) : a;
		T c(move(a)); a = move(b); b = move(c);
	}

	template<> inline void swap<pointer>(pointer& a, pointer&b) { 
		swap<ang_size_t>(reinterpret_cast<ang_size_t&>(a), reinterpret_cast<ang_size_t&>(b));
	}

	template<typename T, wsize N>
	wsize array_size(const T(&ar)[N]) {
		return N;
	}

	template<bool VALUE1>
	struct not_expression : public false_type { };
	template<> struct not_expression<false> : public true_type { };

	template<bool VALUE1, bool VALUE2>
	struct and_expression : public false_type { };
	template<> struct and_expression<true, true> : public true_type { };
	template<bool VALUE1, bool VALUE2> using nor_expression = and_expression<VALUE1, VALUE2>;

	template<bool VALUE1, bool VALUE2>
	struct or_expression : public true_type { };
	template<> struct or_expression<false, false> : public false_type { };
	template<bool VALUE1, bool VALUE2> using nand_expression = or_expression<VALUE1, VALUE2>;

	template<bool VALUE1, bool VALUE2>
	struct xor_expression : public false_type { };
	template<> struct xor_expression<true,false> : public true_type { };
	template<> struct xor_expression<false, true> : public true_type { };

	template<bool VALUE1, bool VALUE2>
	struct xnor_expression : public false_type { };
	template<> struct xnor_expression<true, true> : public true_type { };
	template<> struct xnor_expression<false, false> : public true_type { };



	template<bool VALUE1, bool VALUE2>
	constexpr bool xor_operator() { return  xor_expression<VALUE1, VALUE2>::value; }

	template<bool VALUE1, bool VALUE2>
	constexpr bool xnor_operator() { return  xnor_expression<VALUE1, VALUE2>::value; }

	template<typename T1, typename T2>
	struct is_same : public integer_constant<bool, false> { };

	template<typename T1>
	struct is_same<T1, T1> : public integer_constant<bool, true> { };

	template<typename base_t, typename derived_t>
	struct is_base_of : public integer_constant<bool, __is_base_of(base_t, derived_t)> { };

	template<typename T>
	struct is_function : public false_type {};

	template<typename T, typename... Ts>
	struct is_function<T(Ts...)> : public true_type {};

	template<typename T, typename... Ts>
	struct is_function<T(*)(Ts...)> : public true_type {};

	template<typename U, typename T, typename... Ts>
	struct is_function<T(U::*)(Ts...)> : public true_type {};

	template<typename return_t, typename... args_t>
	using function_cast = return_t(*)(args_t...);

	template<typename return_t, typename... args_t>
	using function_type = return_t(*)(args_t...);


	template<wsize ALIGMENT> wsize align_to(wsize value) {
		static_assert(ALIGMENT == 0, "Error: align_to<0>(...)");
		wsize res = (value % ALIGMENT);
		return res && value ? value + (ALIGMENT - res) : value;
	}

	template<wsize ALIGMENT, wsize VALUE> constexpr wsize align_to(){
		static_assert(ALIGMENT != 0, "Error: align_to<0>(...)");
		return (VALUE % ALIGMENT) && VALUE ? VALUE + (ALIGMENT - (VALUE % ALIGMENT)) : VALUE;
	}

}//ang


#endif//__ANG_UTILS_HPP__
