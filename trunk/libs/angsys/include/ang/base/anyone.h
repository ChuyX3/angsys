/*********************************************************************************************************************/
/*   File Name: ang/base/anyone.h                                                                                     */
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
#elif !defined __ANG_BASE_ANYONE_H__
#define __ANG_BASE_ANYONE_H__

namespace ang //constants
{
	
	template<typename...Ts> struct anyone;
	template<typename...Ts>
	struct anyone<type_pack<Ts...>> : anyone<Ts...> {};

	template<wsize N, typename...Ts>
	struct __anyone_storage;

	template<wsize N>
	struct __anyone_storage<N> {
		using types = type_pack<>;
		wsize m_current;
	};

	template<wsize N, typename T>
	struct __anyone_storage<N, T> {
		using types = type_pack<T>;
		static constexpr wsize INDEX = N;
		static constexpr bool is_trivially_destructible = is_trivially_destructible<T>::value;

		struct {
			wsize m_current;
			typename remove_const<T>::type m_first;
		};

		__anyone_storage() {

		}

		template<typename...A>
		constexpr explicit __anyone_storage(integer_constant<wsize, 0>, A&&...args)
			: m_first(ang::forward<A>(args)...) {
		}

		template<wsize IDX, typename...A, enable_if<(IDX > 0), int> = 0>
		constexpr explicit __anyone_storage(integer_constant<wsize, IDX>, A&&...args) {
		}

		~__anyone_storage() {

		}

		template<wsize I, enable_if<is_same<wsize, I, 0>::value, bool> = true>
		auto& get_element() {
			return m_first;
		}
		template<wsize I, enable_if<is_same<wsize, I, 0>::value, bool> = true>
		auto const& get_element()const {
			return m_first;
		}
	};

	template<wsize N, typename T, typename...Ts>
	struct __anyone_storage<N, T, Ts...> {
		using types = type_pack<T, Ts...>;
		static constexpr wsize INDEX = N;
		static constexpr bool is_trivially_destructible = and_expression<is_trivially_destructible<T>::value, is_trivially_destructible<Ts>::value...>::value;

		union
		{
			struct {
				wsize m_current;
				typename remove_const<T>::type m_first;
			};
			typename __anyone_storage<(INDEX + 1),Ts...> m_rest;
		};

		__anyone_storage() {

		}

		template<typename...A>
		constexpr explicit __anyone_storage(integer_constant<wsize,0>, A&&...args)
			: m_first(ang::forward<A>(args)...) {
		}

		template<wsize IDX, typename...A, enable_if<(IDX > 0), int> = 0>
		constexpr explicit __anyone_storage(integer_constant<wsize, IDX>, A&&...args)
			: m_rest(integer_constant<wsize, IDX - 1>{}, ang::forward<A>(args)...) {
		}

		~__anyone_storage() {

		}

		template<wsize I, enable_if<is_same<wsize, I, 0>::value, bool> = true>
		auto& get_element() {
			return m_first;
		}
		template<wsize I, enable_if<is_same<wsize, I, 0>::value, bool> = true>
		auto const& get_element()const {
			return m_first;
		}

		template<wsize I, enable_if<is_greater<wsize, I, 0>::value, bool> = true>
		auto& get_element() {
			return m_rest.get_element<I - 1>();
		}
		template<wsize I, enable_if<is_greater<wsize, I, 0>::value, bool> = true>
		auto const& get_element()const {
			return m_rest.get_element<I - 1>();
		}
	};

	template<typename TP>
	struct __anyone_storage_from_pack;

	template<typename...Ts>
	struct __anyone_storage_from_pack<type_pack<Ts...>> {
		using type = __anyone_storage<0, Ts...>;
	};


	template<typename FN, typename TP, typename IP = typename value_pack_from_range<wsize, 0, type_pack_size<wsize, typename remove_reference<TP>::type::types>::value - 1>::type>
	struct __anyone_element_visitor;

	template<typename FN, typename TP, wsize... IDXS>
	struct __anyone_element_visitor<FN, TP, value_pack<wsize, IDXS...>> {

		using return_type = decltype(ang::declval<FN>()(integer_constant<wsize, 0>{}, ang::declval<typename remove_reference<TP>::type>().get_element<0>()));
		using function_type = return_type(*)(TP&&, FN&&);

		inline static return_type visit(wsize idx, TP&& pack, FN&& func) {
			return s_function_array[idx](ang::forward<TP>(pack), ang::forward<FN>(func));
		}
	private:
		template<wsize I> inline static return_type visit1(TP&& pack, FN&& func) {
			return static_cast<FN&&>(func)(integer_constant<wsize, I>{}, static_cast<TP&&>(pack).get_element<I>());
		}

		static constexpr function_type s_function_array[] = { &visit1<IDXS>... };
	};

	template<typename T, typename TP
		, bool = type_pack_contains<T, TP>::value
		, wsize IDX = 0
		, bool = is_less<wsize, IDX, type_pack_size<wsize, TP>::value>::value
		, bool = is_constructible<typename type_pack_at<IDX, TP>::type, T>::value>
	struct __anyone_get_first_constructible;

	template<typename T, typename TP
		, bool = type_pack_contains<T, TP>::value
		, wsize IDX = 0
		, bool = is_less<wsize, IDX, type_pack_size<wsize, TP>::value>::value
		, bool = is_constructible<T, typename type_pack_at<IDX, TP>::type>::value>
		struct __anyone_get_first_constructible_inverse;

	template<typename T, typename TP
		, bool = type_pack_contains<T, TP>::value
		, wsize IDX = 0
		, bool = is_less<wsize, IDX, type_pack_size<wsize,TP>::value>::value
		, bool = is_assignable<typename type_pack_at<IDX, TP>::type, T>::value>
	struct __anyone_get_first_assignable;


	template<typename T, wsize IDX, bool TEST, typename...Ts >
	struct __anyone_get_first_constructible<T, type_pack<Ts...>, true, IDX, true, TEST> {
		static constexpr wsize index = type_pack_contains_at<T, type_pack<Ts...>>::value;
		using type = T;
	};

	template<typename T, wsize IDX, typename...Ts >
	struct __anyone_get_first_constructible<T, type_pack<Ts...>, false, IDX, false, false> {
		static constexpr wsize index = invalid_index;
	};

	template<typename T, wsize IDX, typename...Ts >
	struct __anyone_get_first_constructible<T, type_pack<Ts...>, false, IDX, false, true> {
		static constexpr wsize index = invalid_index;
	};

	template<windex IDX, typename T, typename...Ts >
	struct __anyone_get_first_constructible<T, type_pack<Ts...>, false, IDX, true, true> {
		static constexpr wsize index = IDX;
		using type = typename type_pack_at<index, type_pack<Ts...>>::type;
	};

	template<windex IDX, typename T, typename...Ts >
	struct __anyone_get_first_constructible<T, type_pack<Ts...>, false, IDX, true, false>
		: __anyone_get_first_constructible<T, type_pack<Ts...>, false, IDX + 1> {
	};

	template<typename T, wsize IDX, bool TEST, typename...Ts >
	struct __anyone_get_first_constructible_inverse<T, type_pack<Ts...>, true, IDX, true, TEST> {
		static constexpr wsize index = type_pack_contains_at<T, type_pack<Ts...>>::value;
		using type = T;
	};

	template<typename T, wsize IDX, bool TEST, typename...Ts >
	struct __anyone_get_first_constructible_inverse<T, type_pack<Ts...>, false, IDX, false, TEST> {
		static constexpr wsize index = invalid_index;
		using type = void;
	};

	template<windex IDX, typename T, typename...Ts >
	struct __anyone_get_first_constructible_inverse<T, type_pack<Ts...>, false, IDX, true, true> {
		static constexpr wsize index = IDX;
		using type = typename type_pack_at<IDX, type_pack<Ts...>>::type;
	};

	template<windex IDX, typename T, typename...Ts >
	struct __anyone_get_first_constructible_inverse<T, type_pack<Ts...>, false, IDX, true, false>
		: __anyone_get_first_constructible_inverse<T, type_pack<Ts...>, false, IDX + 1> {
	};


	template<typename T, wsize IDX, bool TEST, typename...Ts >
	struct __anyone_get_first_assignable<T, type_pack<Ts...>, true, IDX, true, TEST> {
		static constexpr wsize index = type_pack_contains_at<T, type_pack<Ts...>>::value;
		using type = T;
	};

	template<windex IDX, typename T, typename...Ts >
	struct __anyone_get_first_assignable<T, type_pack<Ts...>, false, IDX, true, true> {
		static constexpr wsize index = IDX;
		using type = typename type_pack_at<IDX, type_pack<Ts...>>::type;
	};

	template<windex IDX, bool TEST, typename T, typename...Ts >
	struct __anyone_get_first_assignable<T, type_pack<Ts...>, false, IDX, false, TEST> {
		static constexpr wsize index = invalid_index;
		using type = void;
	};

	template<windex IDX, typename T, typename...Ts >
	struct __anyone_get_first_assignable<T, type_pack<Ts...>, false, IDX, true, false> {
		static constexpr wsize index = value_selector<wsize
			, __anyone_get_first_assignable<T, type_pack<Ts...>, false, IDX + 1>::index
			, invalid_index, is_less<wsize, IDX, type_pack_size<wsize, type_pack<Ts...>>::value>::value>::value;
		using type = enable_if< is_less<wsize, index, type_pack_size<wsize, type_pack<Ts...>>::value>::value, typename type_pack_at<index, type_pack<Ts...>>::type>;
	};

	template<typename...Ts>
	struct anyone //: __anyone_storage<Ts...>
	{
		using types = typename ang::unique_type_pack<Ts...>::type;
		using data_type = typename __anyone_storage_from_pack<types>::type;
		static constexpr wsize SIZE = type_pack_size<wsize, types>::value;

	private:
		union
		{
			data_type m_data;
			wsize m_current;
		};
		

	public: /*constructors and destructor*/
		anyone()
			: m_data()
			, m_current(invalid_index){
		}

		anyone(anyone && value)
			: m_data()
			, m_current(invalid_index) {
			if (value.is_valid()) {
				visit_element(value.m_current, value.data(), [this](auto idx, auto& item)
				{
					this->set<decltype(idx)::value>(ang::move(item));
				});
			}
		}

		anyone(anyone const& value)
			: m_data()
			, m_current(invalid_index) {
			if (value.is_valid()) {
				visit_element(value.m_current, value.data(), [this](auto idx, auto const& item)
				{
					this->set<decltype(idx)::value>(item);
				});
			}
		}

		template<typename U, enable_if<is_less<wsize, __anyone_get_first_constructible<U, types>::index, SIZE>::value, bool> = true>
		anyone(U&& args)
			: m_data(integer_constant<wsize, __anyone_get_first_constructible<U, types>::index>{}, ang::forward<U>(args))
			, m_current(type_index_in_pack<typename __anyone_get_first_constructible<U, types>::type, types>::value)
		{

		}

		~anyone() {
			reset();
		}

	public: /*getters and setters*/
		template<typename U, enable_if<is_less<wsize, __anyone_get_first_constructible_inverse<U, types>::index, SIZE>::value, bool> = true>
		auto& get() {
			static constexpr wsize IDX = __anyone_get_first_constructible_inverse<U, types>::index;
			if (IDX != m_current)
				throw - 1;
			return data().get_element<IDX>();
		}

		template<typename U, enable_if<is_less<wsize, __anyone_get_first_constructible_inverse<U, types>::index, SIZE>::value, bool> = true>
		auto const& get()const {
			static constexpr wsize IDX = __anyone_get_first_constructible_inverse<U, types>::index;
			if (IDX != m_current)
				throw - 1;
			return data().get_element<IDX>();
		}

		template<wsize IDX, enable_if<is_less<wsize, IDX, SIZE>::value, bool> = true>
		typename type_pack_at<IDX, types>::type const& get()const {
			return get<typename type_pack_at<IDX, types>::type>();
		}

		template<wsize IDX, enable_if<is_less<wsize, IDX, SIZE>::value, bool> = true>
		typename type_pack_at<IDX, types>::type & get() {
			return get<typename type_pack_at<IDX, types>::type>();
			
		}

		template<typename U, enable_if<is_less<wsize, type_pack_contains_at<U, types>::value, SIZE>::value, bool> = true>
		void set(U&& arg) {
			static constexpr wsize IDX = __anyone_get_first_constructible<U, types>::index;
			if (is_valid() && m_current == IDX)
			{
				if (visit_element(m_current, data(), [&](auto, auto& item)->bool
				{
					return assign(item, forward<U>(arg));
				})) { return; }	
			}	
			reset();	
			if (visit_element(IDX, data(), [&](auto, auto& item)
			{
				return construct(item, forward<U>(arg));
			})) {
				m_current = IDX;
			}	
		}

		template<typename U, enable_if<is_less<wsize, type_pack_contains_at<U, types>::value, SIZE>::value, bool> = true>
		void set(U const& arg) {
			static constexpr wsize IDX = __anyone_get_first_constructible<U, types>::index;
			if (is_valid() && m_current == IDX)
			{
				if (visit_element(m_current, data(), [&](auto, auto& item)->bool
				{
					return assign(item, arg);
				})) {
					return;
				}
			}
			reset();
			if (visit_element(IDX, data(), [&](auto, auto& item)
			{
				return construct(item, arg);
			})) {
				m_current = IDX;
			}
		}

		template<typename U, enable_if<
			and_expression<!is_less<wsize, type_pack_contains_at<U, types>::value, SIZE>::value,
			or_expression<is_less<wsize, __anyone_get_first_assignable<U, types>::index, SIZE>::value
			, is_less<wsize, __anyone_get_first_constructible<U, types>::index, SIZE>::value>::value>::value, bool> = true>
		void set(U&& arg) {
			if (is_valid())
			{
				if (visit_element(m_current, data(), [&](auto, auto& item)->bool
				{
					return assign(item, forward<U>(arg));
				})) { return; }
				reset();
			}
			if (!is_valid()) {
				static constexpr wsize IDX = __anyone_get_first_constructible<U, types>::index;
				if (visit_element(IDX, data(), [&](auto, auto& item)
				{
					return construct(item, forward<U>(arg));
				})) {
					m_current = IDX;
				}
			}
		}

		template<typename U, enable_if<
			and_expression<!is_less<wsize, type_pack_contains_at<U, types>::value, SIZE>::value,
			or_expression<is_less<wsize, __anyone_get_first_assignable<U, types>::index, SIZE>::value
			, is_less<wsize, __anyone_get_first_constructible<U, types>::index, SIZE>::value>::value>::value, bool> = true>
		void set(U const& arg) {
			if (is_valid())
			{
				if (visit_element(m_current, data(), [&](auto, auto& item)->bool
				{
					return assign(item, arg);
				})) {
					return;
				}
				reset();
			}
			if (!is_valid()) {
				static constexpr wsize IDX = __anyone_get_first_constructible<U, types>::index;
				if (visit_element(IDX, data(), [&](auto, auto& item)
				{
					return construct(item, arg);
				})) {
					m_current = IDX;
				}
			}
		}

		template<wsize IDX, enable_if<and_expression<(IDX < SIZE), is_copy_assignable<typename type_pack_at<IDX, types>::type>::value>::value, bool> = true>
		void set(typename type_pack_at<IDX, types>::type const& arg) {
			set<typename type_pack_at<IDX, types>::type>(arg);
		}

		template<wsize IDX, enable_if<and_expression<(IDX < SIZE), is_move_assignable<typename type_pack_at<IDX, types>::type>::value>::value, bool> = true>
		void set(typename type_pack_at<IDX, types>::type && arg) {
			set<typename type_pack_at<IDX, types>::type>(ang::forward<typename type_pack_at<IDX, types>::type>(arg));
		}

	public: /*operators*/
		anyone& operator = (anyone && value) {
			if (value.is_valid() && this != &value) {
				reset();
				visit_element(value.m_current, value.data(), [this](auto idx, auto& item)
				{
					this->set<decltype(idx)::value>(ang::move(item));
				});
			}
			return *this;
		}
		anyone& operator = (const anyone& value) {
			if (value.is_valid() && this != &value) {
				visit_element(value.m_current, value.data(), [this](auto idx, auto const& item)
				{
					this->set<decltype(idx)::value>(item);
				});
			}
			return *this;
		}
	
		template<typename U>
		anyone& operator = (U value) {
			set<U>(ang::forward<U>(value));
			return *this;
		}

	
		template<typename... Ts2>
		anyone& operator = (anyone<Ts2...> const& value) {
			if (value.is_valid()) {
				visit_element(value.m_current, value.data(), [this](auto idx, auto const& item)
				{
					this->set<decltype(idx)::value>(item);
				});
			}
			return *this;
		}

		//template<enable_if<!is_same_type<anyone<types>, anyone<type_pack<Ts...>>>::value, bool> = true>
		operator anyone<types>& () {
			return reinterpret_cast<anyone<types>&>(*this);
		}

		//template<enable_if<!is_same_type<anyone<types>, anyone<type_pack<Ts...>>>::value, bool> = true>
		operator anyone<types> const& ()const {
			return reinterpret_cast<anyone<types>&>(*this);
		}

		template<typename T>
		operator T const& ()const {
			return get<T>();
		}

		template<typename T>
		operator T& () {
			return get<T>();
		}


	public: /*utilities*/
		bool is_valid()const { 
			return m_current < SIZE;
		}
		wsize current()const {
			return m_current;
		}
		void reset() {
			reset(yes_expression<data_type::is_trivially_destructible>{});
			m_current = invalid_index;
		}

		template<text::encoding E = text::encoding::utf8, template<typename>class A = memory::buffer_allocator>
		text::fast_string<E,A> to_string()const {
			text::fast_string<E, A> out;
			if (is_valid()) {
				visit_element(m_current, data(), [&](auto idx, auto const& item)
				{
					out.append(ang::to_string(item));
				});
			}
			return ang::move(out);
		}

		template<text::encoding E = text::encoding::utf8, template<typename>class A = memory::buffer_allocator>
		text::fast_string<E, A> to_string(text::text_format_t f)const {
			text::fast_string<E, A> out;
			if (is_valid()) {
				visit_element(m_current, data(), [&](auto idx, auto const& item)
				{
					out.append(ang::to_string(item, f));
				});
			}
			return ang::move(out);
		}

		template<text::encoding E = text::encoding::utf8, template<typename>class A = memory::buffer_allocator>
		text::fast_string<E, A>& to_string(text::fast_string<E, A> &out)const {
			if (is_valid()) {
				visit_element(m_current, data(), [&](auto idx, auto const& item)->void
				{
					out.append(ang::to_string(item));
				});
			}
			return out;
		}

		template<text::encoding E = text::encoding::utf8, template<typename>class A = memory::buffer_allocator>
		text::fast_string<E, A>& to_string(text::fast_string<E, A> &out, text::text_format_t f)const {
			if (is_valid()) {
				visit_element(m_current, data(), [&](auto idx, auto const& item)
				{
					out.append(ang::to_string(item, f));
				});
			}
			return out;
		}

	private: /*internal utilities*/
		constexpr data_type& data() & noexcept {
			return m_data;
		}
		constexpr const data_type& data() const & noexcept {
			return m_data;
		}
		constexpr data_type&& data() && noexcept {
			return (ang::move(m_data));
		}
		constexpr const data_type&& data() const && noexcept {
			return (ang::move(m_data));
		}

		template<typename T, typename U, enable_if<is_constructible<T, U&&>::value, bool> = true>
		static constexpr bool construct(T& obj, U&& arg) {
			::new((void*)(T*)&obj) T(ang::forward<U>(arg));
			return true;
		}

		template<typename T, typename U, enable_if<!is_constructible<T, U&&>::value, bool> = false>
		static constexpr bool construct(T& obj, U&& arg) {
			return false;
		}

		template<typename T, typename U, enable_if<is_assignable<T&, U&&>::value, bool> = true>
		static constexpr bool assign(T& obj, U&& arg) {
			obj = ang::forward<U>(arg);
			return true;
		}

		template<typename T, typename U, enable_if<!is_assignable<T&, U&&>::value, bool> = false>
		static constexpr bool assign(T& obj, U&& arg) {
			return false;
		}

		template<typename T>
		static void destroy(T& obj) {
			obj.~T();
		}

		void reset(true_type) {

		}
		void reset(false_type) {
			if (m_current < SIZE) {
				visit_element(m_current, data(), [](auto, auto& item)
				{
					destroy(item);
				});
			}
		}

		template<typename TP, typename FN>
		static auto visit_element(wsize idx, TP&& pack, FN&& func) {
			return __anyone_element_visitor<FN, TP>::visit(idx, static_cast<TP&&>(pack), static_cast<FN&&>(func));
		}
	};

	using var = anyone<
		char,
		byte,
		wchar_t,
		char16_t,
		char32_t,
		short,
		ushort,
		int,
		uint,
		long,
		ulong,
		long64,
		ulong64,
		float,
		double,
		mstring>;

	using var_args_t = vector<var>;
//	unique_type_pack<>;

}

#endif//__ANG_BASE_ANYONE_H__
