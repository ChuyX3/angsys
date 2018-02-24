#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_TUPLE_H__
#define __ANG_BASE_TUPLE_H__

namespace ang //constants
{

	template<typename T, typename... Ts> struct tuple;

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

		T& first() { return first_; }
		T const& first()const { return first_; }

		tuple<void> & rest() { return rest_; }
		tuple<void> const& rest()const { return rest_; }

		template<wsize N> auto get()const { return __tuple_get_set_impl<N, T>::get(*this); }
		template<wsize N> void set(typename __tuple_get_set_impl<N, T>::arg_type value)const {
			__tuple_get_set_impl<N, T>::set(*this, ang::forward<__tuple_get_set_impl<N, T>::arg_type>(value));
		}

	private:
		T first_;
		static tuple<void> rest_;
	};

	template<typename T, typename... Ts>
	struct tuple {
		static constexpr wsize field_count = tuple<Ts...>::field_count + 1;

		T& first() { return first_; }
		T const& first()const { return first_; }

		tuple<Ts...>& rest() { return rest_; }
		tuple<Ts...> const& rest()const { return rest_; }

		template<wsize N> auto get()const { return __tuple_get_set_impl<N, T, Ts...>::get(*this); }
		template<wsize N> void set(typename __tuple_get_set_impl<N, T, Ts...>::arg_type value) {
			__tuple_get_set_impl<N, T, Ts...>::set(*this, ang::forward<typename __tuple_get_set_impl<N, T, Ts...>::arg_type>(value));
		}

	private:
		T first_;
		tuple<Ts...> rest_;
	};

}

#endif//__ANG_BASE_TUPLE_H__