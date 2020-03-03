/*********************************************************************************************************************/
/*   File Name: ang/base/tuple.h                                                                                     */
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
#elif !defined __ANG_BASE_TUPLE_H__
#define __ANG_BASE_TUPLE_H__

namespace ang //constants
{
	namespace collections
	{
		template<typename K, typename T> struct pair;
		template<typename T, typename... Ts> struct tuple;

		template<typename K, typename T>
		struct pair<const K, T> {
			pair() : key(), value() { }
			pair(K k, T v) : key(k), value(v) { }
			pair(pair const& other) : key(other.key), value(other.value) { }
			pair(pair && other) : key(ang::move(other.key)), value(ang::move(other.value)) { }
			
			pair& operator = (pair const& other) {
				if (this != &other) {
					value = other.value;
				}
				return*this;
			}
			pair& operator = (pair && other) {
				if (this != &other) {
					value = ang::move(other.value);
				}
				return*this;
			}

			operator pair<K, T>()const;

			const K key;
			T value;
		};

		template<typename K, typename T>
		struct pair {
			pair() : key(), value() { }
			pair(K k, T v) : key(k), value(v) { }
			pair(pair const& other) : key(other.key), value(other.value) { }
			pair(pair && other) : key(ang::move(other.key)), value(ang::move(other.value)) { }

			pair& operator = (pair const& other) {
				if (this != &other) {
					key = other.key;
					value = other.value;
				}
				return*this;
			}
			pair& operator = (pair && other) {
				if (this != &other) {
					key = ang::move(other.key);
					value = ang::move(other.value);
				}
				return*this;
			}

			operator pair<const K, T> const&() {
				return *reinterpret_cast<pair<const K, T>*>(this);
			}

			operator pair<const K, T> const&()const {
				return *reinterpret_cast<pair<const K, T> const*>(this);
			}

			K key;
			T value;
		};

		template<typename K, typename T> inline pair<const K, T>::operator pair<K, T>()const {
			return pair<K, T>(key, value);
		}

		////////////////////////////////////////////////////////////////////

		template<typename... Ts> struct __tuple_storage;

		template<typename...Ts>
		struct __tuple_storage<void, Ts...> {
			__tuple_storage() {
				static_assert(false, "void is not a valid type for tuple<...> argument");
			}
			
		};

		template<typename T>
		struct __tuple_storage<T> {
			T m_first;
			__tuple_storage()
				: m_first(default_value<T>::value) {
			}
			template<typename U>
			__tuple_storage(U&& args)
				: m_first(ang::forward<U>(args)) {
			}
			__tuple_storage(__tuple_storage&& args)
				: m_first(ang::move(args.m_first)) {
			}
			__tuple_storage(__tuple_storage const& args)
				: m_first(args.m_first) {
			}
			~__tuple_storage() {
			}

			__tuple_storage& operator = (__tuple_storage&& args) {
				if (this != &args) {
					m_first = ang::move(args.m_first);
				}
				return*this;
			}
			__tuple_storage& operator = (__tuple_storage const& args) {
				if (this != &args) {
					m_first = args.m_first;
				}
				return*this;
			}

			bool operator == (__tuple_storage const& arg)const {
				return logic_operation<logic_operation_type::same, T, T>::operate(m_first, arg.m_first);
			}

			bool operator != (__tuple_storage const& arg)const {
				return logic_operation<logic_operation_type::diferent, T, T>::operate(m_first, arg.m_first);
			}
		};

		template<typename T, typename...Ts>
		struct __tuple_storage<T, Ts...> {
			T m_first;
			__tuple_storage<Ts...> m_rest;

			__tuple_storage()
				: m_first(default_value<T>::value)
				, m_rest() {
			}
			template<typename U, typename...Us>
			__tuple_storage(U&& arg, Us&&... args)
				: m_first(ang::forward<U>(arg))
				, m_rest(ang::forward<Us>(args)...) {
			}
			__tuple_storage(__tuple_storage&& args)
				: m_first(ang::move(args.m_first))
				, m_rest(ang::move(args.m_rest)) {
			}
			__tuple_storage(__tuple_storage const& args)
				: m_first(args.m_first)
				, m_rest(args.m_rest) {
			}
			~__tuple_storage() {
			}

			__tuple_storage& operator = (__tuple_storage&& args) {
				if (this != &args) {
					m_first = ang::move(args.m_first);
					m_rest = ang::move(args.m_rest);
				}
				return*this;
			}
			__tuple_storage& operator = (__tuple_storage const& args) {
				if (this != &args) {
					m_first = args.m_first;
					m_rest = args.m_rest;
				}
				return*this;
			}

			bool operator == (__tuple_storage const& arg)const {
				return logic_operation<logic_operation_type::same, T, T>::operate(m_first, arg.m_first)
					&& logic_operation<logic_operation_type::same, __tuple_storage<Ts...>, __tuple_storage<Ts...>>::operate(m_rest, arg.m_rest);
			}

			bool operator != (__tuple_storage const& arg)const {
				return logic_operation<logic_operation_type::diferent, T, T>::operate(m_first, arg.m_first)
					|| logic_operation<logic_operation_type::diferent, __tuple_storage<Ts...>, __tuple_storage<Ts...>>::operate(m_rest, arg.m_rest);
			}
		};

		template<typename TP> struct __tuple_storage_from_pack;

		template<typename...Ts>
		struct __tuple_storage_from_pack<type_pack<Ts...>> {
			using type = __tuple_storage<Ts...>;
		};

		template<wsize N, typename TP> struct __tuple_visit_element;

		template<wsize N, typename T, typename... Ts>
		struct __tuple_visit_element<N, type_pack<T, Ts...>> {
			static auto& get(__tuple_storage<T, Ts...>& tuple_) { return __tuple_visit_element<N - 1, type_pack<Ts...>>::get(tuple_.m_rest); }
			static auto const& get(__tuple_storage<T, Ts...> const& tuple_) { return __tuple_visit_element<N - 1, type_pack<Ts...>>::get(tuple_.m_rest); }
		};

		template<typename T, typename... Ts> struct __tuple_visit_element<0, type_pack<T, Ts...>> {
			static auto& get(__tuple_storage<T, Ts...>& tuple_) { return tuple_.m_first; }
			static auto const& get(__tuple_storage<T, Ts...> const& tuple_) { return tuple_.m_first; }
		};

		template<typename T, typename...Ts>
		struct tuple {
			using types = type_pack<T, Ts...>;
			using data_type = typename __tuple_storage_from_pack<types>::type;
			static constexpr wsize SIZE = type_pack_size<wsize, types>::value;

			tuple()
				: m_data()
			{
			}

			template<typename U, typename...Us>
			tuple(U&& arg, Us&&...args)
				: m_data(ang::forward<U>(arg), ang::forward<Us>(args)...)
			{
			}

			tuple(tuple<T, Ts...>&& arg)
				: m_data(ang::move(arg.m_data))
			{
			}

			tuple(tuple<T, Ts...> const& arg)
				: m_data(arg.m_data)
			{
			}

			template<wsize N, enable_if<is_less<wsize, N, SIZE>::value, bool> = true>
				typename type_pack_at<N, types>::type const& get()const {
					return __tuple_visit_element<N, types>::get(data());
				}

			template<wsize N, enable_if<is_less<wsize, N, SIZE>::value, bool> = true>
			typename type_pack_at<N, types>::type& get() {
				return __tuple_visit_element<N, types>::get(data());
			}

			template<wsize N, enable_if<and_expression<is_less<wsize, N, SIZE>::value, is_move_assignable<typename type_pack_at<N, types>::type>::value>::value, bool> = true>
			void set(typename type_pack_at<N, types>::type && args) {

				__tuple_visit_element<N, types>::get(data()) = ang::move(args);
			}

			template<wsize N, enable_if<and_expression<is_less<wsize, N, SIZE>::value, is_copy_assignable<typename type_pack_at<N, types>::type>::value>::value, bool> = true>
			void set(typename type_pack_at<N, types>::type const& args) {
				__tuple_visit_element<N, types>::get(data()) = args;
			}

			tuple& operator = (tuple<T, Ts...>&& arg)
			{
				m_data = ang::forward<data_type>(arg.m_data);
				return *this;
			}

			tuple& operator = (tuple<T, Ts...> const& arg)
			{
				m_data = arg.m_data;
				return *this;
			}

			inline bool operator == (tuple<T, Ts...> const& arg)const {
				return logic_operation<logic_operation_type::same, data_type, data_type>::operate(m_data, arg.m_data);
			}
			inline bool operator != (tuple<T, Ts...> const& arg)const {
				return logic_operation<logic_operation_type::diferent, data_type, data_type>::operate(m_data, arg.m_data);
			}

		private:
			data_type& data()& {
				return m_data;
			}
			data_type && data() && {
				return m_data;
			}
			data_type const& data() const& {
				return m_data;
			}

			data_type m_data;
		};

		template<typename...Ts>
		struct tuple<type_pack<Ts...>>
			: tuple<Ts...> { };


		template<typename F, typename TP, wsize = type_pack_size<wsize, TP>::value>
		struct unpack_tuple_helper;

		template<typename F, wsize N, typename...Ts>
		struct unpack_tuple_helper<F, type_pack<Ts...>, N>{
			template<typename...As>
			static auto expand(F&& func, collections::tuple<Ts...> const& pack, As&&... args) {
				return unpack_tuple_helper << type_pack<Ts...>, (N - 1) > ::expand(
					ang::forward<F>(func),
					pack,
					pack.get<N - 1>(),
					ang::forward<As>(args)...
				);
			}

			static auto expand(F&& func, collections::tuple<Ts...> const& pack) {
				return unpack_tuple_helper << type_pack<Ts...>, (N - 1) > ::expand(
					ang::forward<F>(func),
					pack,
					pack.get<N - 1>()
				);
			}
		};

		template<typename F, typename...Ts>
		struct unpack_tuple_helper<F, type_pack<Ts...>, 0> {
			template<typename...As>
			static auto expand(F&& func, collections::tuple<Ts...> const&, As&&... args) {
				return static_cast<F&&>(func)(ang::forward<As>(args)...);
			}
		};

	}

	

	template<typename T, typename TP, long64 INDEX>
	struct __type_index_in_pack;

	template<typename T, long64 INDEX>
	struct __type_index_in_pack<T, type_pack<>,INDEX> : invalid_index_type { };

	template<typename T, long64 INDEX, typename...Ts>
	struct __type_index_in_pack<T, type_pack<T, Ts...>, INDEX> : integer_constant<long64, INDEX> {	};

	template<typename T, typename U, long64 INDEX, typename...Ts>
	struct __type_index_in_pack<T, type_pack<U, Ts...>, INDEX> : __type_index_in_pack<T, type_pack<Ts...>, INDEX + 1> {	};

	template<typename T, typename TP>
	struct type_index_in_pack : invalid_index_type { };

	template<typename T, typename...Ts>
	struct type_index_in_pack<T, type_pack<Ts...>> : __type_index_in_pack<T, type_pack<Ts...>, 0> { };


	template<typename... Ts>
	auto pack_tuple(Ts... args) {
		return ang::move(collections::tuple<Ts...>(ang::forward<Ts>(args)...));
	}

	template<typename F, typename...Ts>
	auto unpack_tuple(F&& func, collections::tuple<Ts...> const& pack)->decltype(func(declval<Ts>()...)) {
		return collections::unpack_tuple_helper<F, type_pack<Ts...>>::expand(ang::forward<F>(func), pack);
	}


	template<typename K, typename T, typename U, bool VALUE>
	struct logic_operation<logic_operation_type::same, collections::pair<K, T>, U,  false, VALUE> {
		static bool operate(const collections::pair<K, T>& pair, const U& value) { 
			return logic_operation<logic_operation_type::same, K, U>::operate(pair.key, value);
		}
	};

	template<typename K, typename T, typename U, bool VALUE>
	struct logic_operation<logic_operation_type::same, U, collections::pair<K, T>, VALUE, false> {
		static bool operate(const U& value, const collections::pair<K, T>& pair) {
			return logic_operation<logic_operation_type::same, U, K>::operate(value, pair.key);
		}
	};

	template<typename K, typename T, typename U, bool VALUE>
	struct logic_operation<logic_operation_type::same_or_major, collections::pair<K, T>, U, false, VALUE> {
		static bool operate(const collections::pair<K, T>& pair, const U& value) {
			return logic_operation<logic_operation_type::same_or_major, K, U>::operate(pair.key, value);
		}
	};

	template<typename K, typename T, typename U, bool VALUE>
	struct logic_operation<logic_operation_type::same_or_major, U, collections::pair<K, T>, VALUE, false> {
		static bool operate(const U& value, const collections::pair<K, T>& pair) {
			return logic_operation<logic_operation_type::same_or_major, U, K>::operate(value, pair.key);
		}
	};

	template<typename K, typename T, typename U, bool VALUE>
	struct logic_operation<logic_operation_type::same_or_minor, collections::pair<K, T>, U, false, VALUE> {
		static bool operate(const collections::pair<K, T>& pair, const U& value) {
			return logic_operation<logic_operation_type::same_or_minor, K, U>::operate(pair.key, value);
		}
	};

	template<typename K, typename T, typename U, bool VALUE>
	struct logic_operation<logic_operation_type::same_or_minor, U, collections::pair<K, T>, VALUE, false> {
		static bool operate(const U& value, const collections::pair<K, T>& pair) {
			return logic_operation<logic_operation_type::same_or_minor, U, K>::operate(value, pair.key);
		}
	};

	template<typename K, typename T, typename U, bool VALUE>
	struct logic_operation<logic_operation_type::major, collections::pair<K, T>, U, false, VALUE> {
		static bool operate(const collections::pair<K, T>& pair, const U& value) {
			return logic_operation<logic_operation_type::major, K, U>::operate(pair.key, value);
		}
	};

	template<typename K, typename T, typename U, bool VALUE>
	struct logic_operation<logic_operation_type::major, U, collections::pair<K, T>, VALUE, false> {
		static bool operate(const U& value, const collections::pair<K, T>& pair) {
			return logic_operation<logic_operation_type::major, U, K>::operate(value, pair.key);
		}
	};

	template<typename K, typename T, typename U, bool VALUE>
	struct logic_operation<logic_operation_type::minor, collections::pair<K, T>, U, false, VALUE> {
		static bool operate(const collections::pair<K, T>& pair, const U& value) {
			return logic_operation<logic_operation_type::minor, K, U>::operate(pair.key, value);
		}
	};

	template<typename K, typename T, typename U, bool VALUE>
	struct logic_operation<logic_operation_type::minor, U, collections::pair<K, T>, VALUE, false> {
		static bool operate(const U& value, const collections::pair<K, T>& pair) {
			return logic_operation<logic_operation_type::minor, U, K>::operate(value, pair.key);
		}
	};

}

#endif//__ANG_BASE_TUPLE_H__
