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
		struct pair {
			pair() : key() , value() { }
			pair(K k, T v) : key(k), value(v) { }
			pair(pair const& other) : key(other.key), value(other.value) { }
			pair(pair && other) : key(ang::move(other.key)), value(ang::move(other.value)) { }

			K key;
			T value;
		};

		template<wsize N, typename T, typename... Ts> struct __tuple_get_set_impl {
			typedef typename __tuple_get_set_impl<N - 1, Ts...>::arg_type arg_type;
			typedef typename __tuple_get_set_impl<N - 1, Ts...>::return_type return_type;

			static return_type& get(tuple<T, Ts...>& tuple_) { return __tuple_get_set_impl<N - 1, Ts...>::get(tuple_.rest()); }
			static return_type const& get(tuple<T, Ts...> const& tuple_) { return __tuple_get_set_impl<N - 1, Ts...>::get(tuple_.rest()); }
			static void set(tuple<T, Ts...>& tuple_, arg_type value) { __tuple_get_set_impl<N - 1, Ts...>::set(tuple_.rest(), ang::forward<arg_type>(value)); }
		};

		template<typename T, typename... Ts> struct __tuple_get_set_impl<0, T, Ts...> {
			typedef T return_type;
			typedef T arg_type;
			static return_type& get(tuple<T, Ts...>& tuple_) { return tuple_.first(); }
			static return_type const& get(tuple<T, Ts...> const& tuple_) { return tuple_.first(); }
			static void set(tuple<T, Ts...>& tuple_, arg_type value) { tuple_.first() = ang::move(value); }
		};

		template<> struct __tuple_get_set_impl<0, void> {
			typedef void return_type;
			typedef void arg_type;
			static void get(tuple<void>& tuple_) {}
			static void get(tuple<void> const& tuple_) {}
			static void set(tuple<void>&);
		};

		template<> struct tuple<void> {
			static constexpr wsize field_count = 0;

			template<wsize N> void get()const {}
			template<wsize N> void set()const {}

			void first()const { }
			tuple<void>& rest() { return *this; }
			tuple<void> const& rest()const { return *this; }
		};

		template<typename T>
		struct tuple<T> {
			static constexpr wsize field_count = 1;

			tuple(T arg)
				: m_first(ang::forward<T>(arg))
			{
			}

			tuple(tuple<T>&& arg)
				: m_first(ang::move(arg.m_first))
			{
			}

			tuple(tuple<T> const& arg)
				: m_first(arg.m_first)
			{
			}

			T& first() { return m_first; }
			T const& first()const { return m_first; }

			tuple<void> & rest() { return m_rest; }
			tuple<void> const& rest()const { return m_rest; }

			template<wsize N> auto get()const { return __tuple_get_set_impl<N, T>::get(*this); }
			template<wsize N> void set(typename __tuple_get_set_impl<N, T>::arg_type value)const {
				__tuple_get_set_impl<N, T>::set(*this, ang::forward<__tuple_get_set_impl<N, T>::arg_type>(value));
			}

			tuple& operator = (tuple<T>&& arg)
			{
				m_first = ang::forward<T>(arg.m_first);
				return *this;
			}

			tuple& operator = (tuple<T> const& arg)
			{
				m_first = ang::forward<T>(arg.m_first);
				return *this;
			}

			inline bool operator == (tuple<T> const& arg)const {
				return logic_operation<logic_operation_type::same, T, T>::operate(m_first, arg.m_first);
			}

			inline bool operator != (tuple<T> const& arg)const {
				return logic_operation<logic_operation_type::diferent, T, T>::operate(m_first, arg.m_first);
			}

		private:
			T m_first;
			static tuple<void> m_rest;
		};

		template<typename T> tuple<void> tuple<T>::m_rest;


		template<typename T, typename... Ts>
		struct tuple {
			static constexpr wsize field_count = tuple<Ts...>::field_count + 1;

			tuple(T arg, Ts...args)
				: m_first(ang::forward<T>(arg))
				, m_rest(ang::forward<Ts>(args)...)
			{
			}

			tuple(tuple<T, Ts...>&& arg)
				: m_first(ang::move(arg.m_first))
				, m_rest(ang::move(arg.m_rest))
			{
			}

			tuple(tuple<T, Ts...> const& arg)
				: m_first(arg.m_first)
				, m_rest(arg.m_rest)
			{
			}

			T& first() { return m_first; }
			T const& first()const { return m_first; }

			tuple<Ts...>& rest() { return m_rest; }
			tuple<Ts...> const& rest()const { return m_rest; }

			template<wsize N> auto get()const { return __tuple_get_set_impl<N, T, Ts...>::get(*this); }
			template<wsize N> void set(typename __tuple_get_set_impl<N, T, Ts...>::arg_type value) {
				__tuple_get_set_impl<N, T, Ts...>::set(*this, ang::forward<typename __tuple_get_set_impl<N, T, Ts...>::arg_type>(value));
			}

			tuple& operator = (tuple<T, Ts...>&& arg)
			{
				m_first = ang::forward<T>(arg.m_first);
				m_rest = ang::forward<tuple<Ts...>>(arg.m_rest);
				return *this;
			}

			tuple& operator = (tuple<T, Ts...> const& arg)
			{
				m_first = ang::forward<T>(arg.m_first);
				m_rest = ang::forward<tuple<Ts...>>(arg.m_rest);
				return *this;
			}

			inline bool operator == (tuple<T, Ts...> const& arg)const {
				return logic_operation<logic_operation_type::same, T, T>::operate(m_first, arg.m_first)
					&& logic_operation<logic_operation_type::same, tuple<Ts...>, tuple<Ts...>>::operate(m_rest, arg.m_rest);
			}

			inline bool operator != (tuple<T, Ts...> const& arg)const {
				return logic_operation<logic_operation_type::diferent, T, T>::operate(m_first, arg.m_first)
					|| logic_operation<logic_operation_type::diferent, tuple<Ts...>, tuple<Ts...>>::operate(m_rest, arg.m_rest);
			}

		private:
			T m_first;
			tuple<Ts...> m_rest;
		};
	}

	template<typename F, typename... Ts> struct unpack_args_helper;

	template<typename F, typename T, typename... Ts>
	struct unpack_args_helper<F, T, Ts...> {
		template<typename...A>
		static auto expand(F func, collections::tuple<T, Ts...>& pack, A... args) {
			return unpack_args_helper<F, Ts...>::expand(func, pack.rest(), ang::forward<A>(args)..., ang::forward<T>(pack.first()));
		}
		static auto expand(F func, collections::tuple<T, Ts...>& pack) {
			return unpack_args_helper<F, Ts...>::expand(func, pack.rest(), ang::forward<T>(pack.first()));
		}
	};

	template<typename F>
	struct unpack_args_helper<F> {
		template<typename...A>
		static auto expand(F func, collections::tuple<void>& pack, A... args) {
			return func(ang::forward<A>(args)...);
		}
	};

	template<typename... Ts>
	auto pack_args(Ts... args) {
		return ang::move(collections::tuple<Ts...>(ang::forward<Ts>(args)...));
	}

	template<typename F, typename... Ts>
	auto unpack_args(F func, collections::tuple<Ts...> args) ->decltype(func(declval<Ts>()...)) {
		return unpack_args_helper<F, Ts...>::expand(func, args);
	}

	template<typename F>
	auto unpack_args(F func, collections::tuple<void>) ->decltype(func()) {
		return func();
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
