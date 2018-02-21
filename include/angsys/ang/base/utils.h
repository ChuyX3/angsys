#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_UTILS_H__
#define __ANG_BASE_UTILS_H__

namespace ang //constants
{
	//void type
	template <typename... Ts> using void_t = void;

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
	template<> struct and_expression<false, false> : false_type { };

	template<bool VALUE1, bool VALUE2> struct xor_expression : false_type { };
	template<> struct and_expression<true, false> : true_type { };
	template<> struct and_expression<false, true> : true_type { };


	template<bool VALUE> struct not_expression : false_type { };
	template<> struct not_expression<false> : true_type { };

	template<bool VALUE1, bool VALUE2> struct nand_expression : not_expression<and_expression<VALUE1, VALUE2>::value> { };

	template<bool VALUE1, bool VALUE2> struct nor_expression : not_expression<or_expression<VALUE1, VALUE2>::value> { };

	template<bool VALUE1, bool VALUE2> struct xnor_expression : not_expression<xor_expression<VALUE1, VALUE2>::value> { };

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
}

namespace ang //operations
{
	typedef enum class logic_operation_type : uint
	{
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

	template<logic_operation_t TYPE, typename T1, typename T2> struct logic_operation;


}

#endif//__ANG_BASE_UTILS_H__