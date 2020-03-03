/*********************************************************************************************************************/
/*   File Name: ang/base/packs.h                                                                                     */
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
#elif !defined __ANG_BASE_PACKS_H__
#define __ANG_BASE_PACKS_H__

namespace ang //constants
{
	namespace packs
	{

		template<typename T, T...VALUES> struct value_pack;

		template<typename VP> struct value_pack_size;
		template<typename VP> struct value_pack_first;
		template<wsize IDX, typename VP, bool = (IDX == 0)> struct value_pack_get_at;
		template<typename VP, typename VP::type VALUE> struct value_pack_index_of;

		template<typename T>
		struct value_pack_size<value_pack<T>> : integer_constant<wsize, 0> {};

		template<typename T, T FIRST, T...REST>
		struct value_pack_size<value_pack<T, FIRST, REST...>>
			: integer_constant<wsize, (value_pack_size<value_pack<T, REST...>>::value + 1)> {
		};

		template<typename VP, typename VP::type VALUE, wsize IDX = 0>
		struct __value_pack_index_of {
			static constexpr wsize value = -1;
		};

		template<typename T, T VALUE, T FIRST, T...VALUES, wsize IDX>
		struct __value_pack_index_of<value_pack<T, FIRST, VALUES...>, VALUE, IDX> {
			static constexpr wsize value = value_selector<T, IDX,
				__value_pack_index_of<value_pack<T, VALUES...>, VALUE, (IDX + 1)>::value,
				(VALUE == FIRST)>::value;
		};

		template<typename VP, typename VP::type VALUE>
		struct value_pack_index_of : index_type<__value_pack_index_of<VP, VALUE>::value> { };

		template<typename T, T FIRST, T...REST>
		struct value_pack_first<value_pack<T, FIRST, REST...>> {
			static constexpr T value = FIRST;
		};

		template<wsize IDX, typename VP, bool>
		struct value_pack_get_at {
			using bool_type = false_type;
		};

		template<wsize I, typename T, T FIRST, T...REST>
		struct value_pack_get_at<I, value_pack<T, FIRST, REST...>, true>
			: integer_constant<T, FIRST> {
			using bool_type = true_type;
		};

		template<wsize I, typename T, T FIRST, T...REST>
		struct value_pack_get_at<I, value_pack<T, FIRST, REST...>, false>
			: integer_constant<T, value_pack_get_at<I - 1, value_pack<T, REST...>>::value> {
			using bool_type = typename value_pack_get_at<I - 1, value_pack<T, REST...>>::bool_type;
		};

		template<typename T, T FIRST, T...REST>
		struct value_pack<T, FIRST, REST...> {
			using type = T;
			template<wsize AT>
			static constexpr T get() {
				return value_pack_get_at<AT, value_pack<T, FIRST, REST...>>::value;
			}

			static constexpr T pack[] = { FIRST , REST... };
		};

		template<typename T, typename VP1, typename VP2>
		struct value_pack_add {
			using type = value_pack<T>;
		};

		template<typename T, T... V1, T...V2>
		struct value_pack_add<T, value_pack<T, V1...>, value_pack<T, V2...>> {
			using type = value_pack<T, V1..., V2...>;
		};

		template<typename T, T FIRST, T END, bool = (END >= FIRST)>
		struct value_pack_from_range {
			using type = value_pack<T>;
		};

		template<typename T, T VALUE>
		struct value_pack_from_range<T, VALUE, VALUE, true> {
			using type = value_pack<T, VALUE>;
		};

		template<typename T, T FIRST, T END>
		struct value_pack_from_range<T, FIRST, END, true> {
			using type = typename value_pack_add<T, value_pack<T, FIRST>, typename value_pack_from_range<T, T(long64(FIRST) + 1), END>::type >::type;
		};

		//////////////////////////////////////////////////////////////////////////////////////

		template<char... str> using char_pack = value_pack<char, str...>;

		//////////////////////////////////////////////////////////////////////////////////////

		template<wsize...IDX> using index_pack = value_pack<wsize, IDX...>;

		template<typename T, T FIRST, T END>
		struct index_pack_from_value_range;

		template<typename T, T VALUE>
		struct index_pack_from_value_range<T, VALUE, VALUE> {
			using type = index_pack<wsize(VALUE)>;
		};

		template<typename T, T FIRST, T END>
		struct index_pack_from_value_range {
			using type = typename value_pack_add<wsize, index_pack<wsize(FIRST)>, typename index_pack_from_value_range<T, T(long64(FIRST) + 1), END>::type >::type;
		};

		//////////////////////////////////////////////////////////////////////////////////////

		template<typename...Ts> struct type_pack;

		template<typename T, typename...Ts>
		struct type_pack<T, Ts...> {
			using first_type = T;
			using rest_types = type_pack<Ts...>;
			type_pack() {}
		};

		template<>
		struct type_pack<> {
			type_pack() {}
		};



		template<typename T, typename TP >
		struct __type_pack_contains {
			using type = false_type;
		};

		template<typename T, typename... Ts>
		struct __type_pack_contains<T, type_pack<T, Ts...>>
		{
			using type = true_type;
		};

		template<typename T, typename U, typename... Ts>
		struct __type_pack_contains<T, type_pack<U, Ts...>>
		{
			using type = typename __type_pack_contains<T, type_pack<Ts...>>::type;
		};

		template<typename T, typename U>
		struct type_pack_contains : false_type { };

		template<typename T, typename...Ts>
		struct type_pack_contains<T, type_pack<Ts...>> : __type_pack_contains<T, type_pack<Ts...>>::type { };

		template<typename T, typename TP, wsize IDX = 0, bool = type_pack_contains<T, TP>::value>
		struct __type_pack_contains_at : index_type<-1> {};

		template<typename T, wsize IDX, typename...Ts>
		struct __type_pack_contains_at<T, type_pack<T, Ts...>, IDX, true> : index_type<IDX> { };

		template<typename T, typename U, wsize IDX, typename...Ts>
		struct __type_pack_contains_at<T, type_pack<U, Ts...>, IDX, true> {
			static constexpr wsize value = __type_pack_contains_at<T, type_pack<Ts...>, IDX + 1>::value;
		};

		template<typename T, typename TP>
		struct type_pack_contains_at : index_type<__type_pack_contains_at<T, TP>::value> {};

		////////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename T, typename...Ts> struct __type_pack_size;

		template<typename T> struct __type_pack_size<T> {
			static constexpr T value = 0;
		};

		template<typename T, typename U, typename...Ts>
		struct __type_pack_size<T, U, Ts...> {
			static constexpr T value = (1 + __type_pack_size<T, Ts...>::value);
		};

		template<typename T, typename TP>
		struct type_pack_size : integer_constant<T, 0> {};

		template<typename T, typename...Ts>
		struct type_pack_size<T, type_pack<Ts...>> : integer_constant<T, __type_pack_size<T, Ts...>::value> {};

		////////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename TP> struct type_pack_first;
		template<typename TP> struct type_pack_last;
		template<wsize IDX, typename TP, bool = is_less<wsize, IDX, type_pack_size<wsize, TP>::value>::value> struct type_pack_at;

		template<typename T, typename...Ts>
		struct type_pack_first<type_pack<T, Ts...>> {
			using type = T;
		};

		template<typename T>
		struct type_pack_last<type_pack<T>> {
			using type = T;
		};

		template<typename T, typename...Ts>
		struct type_pack_last<type_pack<T, Ts...>> {
			using type = typename type_pack_last<type_pack<Ts...>>::type;
		};

		template<wsize IDX, typename T, typename...Ts>
		struct type_pack_at<IDX, type_pack<T, Ts...>, false> {
			using type = void;
		};

		template<typename T, typename...Ts>
		struct type_pack_at<0, type_pack<T, Ts...>, true> {
			using type = T;
		};

		template<wsize IDX, typename T, typename...Ts>
		struct type_pack_at<IDX, type_pack<T, Ts...>, true> {
			using type = typename type_pack_at<IDX - 1, type_pack<Ts...>, true>::type;
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename TP, typename T, bool = type_pack_contains<T, TP>::value>
		struct __append_unique_type_to_pack;

		template<typename T, typename...Ts>
		struct __append_unique_type_to_pack<type_pack<Ts...>, T, true>
		{
			using type = type_pack<Ts...>;
		};

		template<typename T, typename...Ts>
		struct __append_unique_type_to_pack<type_pack<Ts...>, T, false>
		{
			using type = type_pack<Ts..., T>;
		};

		template<typename TP, typename...Ts> struct __unique_type_pack;

		template<typename...TPTs, typename T, typename...Ts>
		struct __unique_type_pack<type_pack<TPTs...>, T, Ts...>
		{
			using type = typename __unique_type_pack<typename __append_unique_type_to_pack<type_pack<TPTs...>, T>::type, Ts...>::type;
		};

		template<typename...Ts>
		struct __unique_type_pack<type_pack<Ts...>> {
			using type = type_pack<Ts...>;
		};

		template<typename...Ts> struct unique_type_pack {
			using type = typename __unique_type_pack<type_pack<>, Ts...>::type;
		};
	}

	using namespace packs;
}

#endif//__ANG_BASE_PACKS_H__
