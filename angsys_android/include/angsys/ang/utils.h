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

#ifndef __ANG_UTILS_H__
#define __ANG_UTILS_H__

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
		_101,_102,_103,_104,_105,_106,_107,_108,_109,_110,_111,_112,_113,_114,_115,_116,_117,_118,_119,_120,\
		_121,_122,_123,_124,_125,_126,_127,_128,_129,_130,_131,_132,_133,_134,_135,_136,_137,_138,_139,_140,\
		_141,_142,_143,_144,_145,_146,_147,_148,_149,_150,_151,_152,_153,_154,_155,_156,_157,_158,_159,_160,\
		_161,_162,_163,_164,_165,_166,_167,_168,_169,_170,_171,_172,_173,_174,_175,_176,_177,_178,_179,_180,\
		_181,_182,_183,_184,_185,_186,_187,_188,_189,_190,_191,_192,_193,_194,_195,_196,_197,_198,_199,_200, \
		N,...) N

#define ANG_PP_RSEQ_N() \
		200,199,198,197,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181,\
		180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,\
		160,159,158,157,156,155,154,153,152,151,150,149,148,147,146,145,144,143,142,141,\
		140,139,138,137,136,135,134,133,132,131,130,129,128,127,126,125,124,123,122,121,\
		120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,\
		100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,\
		80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,\
		60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,\
		40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,\
		20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
		
#include <ang/APPLY_FUNCX_N.inl>

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

	template<class T, T _Val>
	struct integer_constant {
		static constexpr T value = _Val;
		typedef T value_type;
		typedef integer_constant<T, _Val> type;
		constexpr operator value_type() const { return (value); }
		constexpr value_type operator()() const { return (value); }
	};

	template<class T>
	struct remove_constant
	{
		typedef T type;
	};

	template<class T>
	struct remove_constant<const T>
	{
		typedef T type;
	};

	template<class T>
	struct remove_constant<const T&>
	{
		typedef T& type;
	};

	template<class T>
	struct remove_constant<const T*>
	{
		typedef T* type;
	};

	template<class T>
	struct remove_constant<T* const>
	{
		typedef T* type;
	};

	template<class T>
	struct remove_reference
	{
		typedef T type;
	};

	template<class T>
	struct remove_reference<T&>
	{
		typedef T type;
	};

	template<class T>
	struct remove_reference<T &&>
	{
		typedef T type;
	};

	template<class T>
	struct remove_pointer
	{
		typedef T type;
	};

	template<class T>
	struct remove_pointer<T*>
	{
		typedef T type;
	};

	template<class T>
	struct remove_pointer<T**>
	{
		typedef T type;
	};

	// TEMPLATE CLASS is_lvalue_reference
	template<class T>
	struct is_lvalue_reference
		: integer_constant<bool,false>
	{	// determine whether T is an lvalue reference
	};

	template<class T>
	struct is_lvalue_reference<T&>
		: integer_constant<bool, true>
	{	// determine whether T is an lvalue reference
	};

	template<typename T>
	typename remove_reference<T>::type& declval();

	template<class T>
	constexpr inline typename remove_reference<T>::type&& move(T&& src) { return static_cast<typename remove_reference<T>::type&&>(src); }

	template<class T>
	inline void move(typename remove_reference<T>::type& dest, typename remove_reference<T>::type&& src) { dest = move(src); }

	template<class T> inline
		constexpr T&& forward(
			typename remove_reference<T>::type& _Arg)
	{	// forward an lvalue as either an lvalue or an rvalue
		return (static_cast<T&&>(_Arg));
	}

	template<class T> inline
		constexpr T&& forward(
			typename remove_reference<T>::type&& _Arg)
	{	// forward an rvalue as an rvalue
		static_assert(!is_lvalue_reference<T>::value, "bad forward call");
		return (static_cast<T&&>(_Arg));
	}

	template<class T1, class T2>
	struct operation_result
	{
		typedef decltype(declval<T1>() + declval<T2>()) type;
	};

	template<class T1, class T2>
	inline typename operation_result<T1,T2>::type min(T1 a, T2 b) { typedef typename operation_result<T1, T2>::type r_t; return move(a < b ? r_t(a) : r_t(b)); }

	template<class T1, class T2>
	inline typename operation_result<T1, T2>::type max(T1 a, T2 b) { typedef typename operation_result<T1, T2>::type r_t; return move(a > b ? r_t(a) : r_t(b)); }

	template<class T>
	inline void swap(T& a, T&b) {
		//&a != &b ? (a ^= b, b ^= a, a ^= b) : a;
		T c(move(a)); a = move(b); b = move(c);
	}

	template<> inline void swap<pointer>(pointer& a, pointer&b) { 
		swap<ang_size_t>(reinterpret_cast<ang_size_t&>(a), reinterpret_cast<ang_size_t&>(b));
	}

	template<class T, wsize N>
	wsize array_size(const T(&ar)[N]) {
		return N;
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

	template<> struct integer_value<char> { static const bool is_integer_value = true; };
	template<> struct integer_value<uchar> { static const bool is_integer_value = true; };
	template<> struct integer_value<short> { static const bool is_integer_value = true; };
	template<> struct integer_value<ushort> { static const bool is_integer_value = true; };
	template<> struct integer_value<int> { static const bool is_integer_value = true; };
	template<> struct integer_value<uint> { static const bool is_integer_value = true; };
	template<> struct integer_value<long> { static const bool is_integer_value = true; };
	template<> struct integer_value<ulong> { static const bool is_integer_value = true; };
	template<> struct integer_value<long64> { static const bool is_integer_value = true; };
	template<> struct integer_value<ulong64> { static const bool is_integer_value = true; };
	template<> struct floating_value<float> { static const bool is_floating_value = true; };
	template<> struct floating_value<double> { static const bool if_floating_value = true; };

}//ang


#endif//__ANG_UTILS_H__
