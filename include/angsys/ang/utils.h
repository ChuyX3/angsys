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
