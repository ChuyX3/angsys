/*********************************************************************************************************************/
/*   File Name: ang/maths/maths.h                                                                                    */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_MATHS_H__
#define __ANG_MATHS_H__

#include <math.h>

namespace ang
{
	namespace maths
	{
		struct constants {
			static constexpr double e = 2.71828182845904523536;   // e
			static constexpr double log2e = 1.44269504088896340736;   // log2(e)
			static constexpr double log10e = 0.434294481903251827651;  // log10(e)
			static constexpr double ln2 = 0.693147180559945309417;  // ln(2)
			static constexpr double ln10 = 2.30258509299404568402;   // ln(10)
			static constexpr double pi = 3.14159265358979323846;   // pi
			static constexpr double sqrt2 = 1.41421356237309504880;   // sqrt(2)
		};

		template<typename...> struct __sum_tn_res;
		template<typename T> struct __sum_tn_res<T> { typedef T type; };
		template<typename T, typename... Tn> struct __sum_tn_res<T, Tn...> {
			typedef typename __sum_tn_res<Tn...>::type res_types;
			typedef decltype(declval<T>() + declval<res_types>()) type;
		};

		template<class T> T sum(T _X){
			return _X;
		}

		template<class T, class... Tn> auto sum(T _X, Tn... _Y) -> typename __sum_tn_res<T, Tn...>::type {
			return _X + sum(_Y...);
		}

		template<class T> T sum_cuad(T _X) {
			return _X * _X;
		}

		template<class T, class... Tn> auto sum_cuad(T _X, Tn... _Y) -> typename __sum_tn_res<T, Tn...>::type {
			return  _X * _X + sum_cuad(_Y...);
		}

		inline auto ceil(double _X) { return ::ceil(ang::move(_X)); }
		inline auto ceil(float _X) { return ::ceilf(ang::move(_X)); }
		inline auto ceil(long double _X) { return ::ceill(ang::move(_X)); }

		inline auto floor(double _X) { return ::floor(ang::move(_X)); }
		inline auto floor(float _X) { return ::floorf(ang::move(_X)); }
		inline auto floor(long double _X) { return ::floorl(ang::move(_X)); }

		inline auto mod(double _X, double _Y) { return ::fmod(ang::move(_X), ang::move(_Y)); }
		inline auto mod(float _X, float _Y) { return ::fmodf(ang::move(_X), ang::move(_Y)); }

		inline auto frexp(double _X, int& _Y) { return ::frexp(ang::move(_X), &_Y); }
		inline auto frexp(float _X, int& _Y) { return ::frexpf(ang::move(_X), &_Y); }
		inline auto frexp(long double _X, int& _Y) { return ::frexpl(ang::move(_X), &_Y); }

		inline auto ldexp(double _X, int _Y) { return ::ldexp(ang::move(_X), ang::move(_Y)); }
		inline auto ldexp(float _X, int _Y) { return ::ldexpf(ang::move(_X), ang::move(_Y)); }
		inline auto ldexp(long double _X, int _Y) { return ::ldexpl(ang::move(_X), ang::move(_Y)); }

		inline auto modf(double _X, double& _Y) { return ::modf(ang::move(_X), &_Y); }
		inline auto modf(float _X, float& _Y) { return ::modff(ang::move(_X), &_Y); }


		inline auto abs(int _X) { return ::abs(ang::move(_X)); }
		inline auto abs(long _X) { return ::labs(ang::move(_X)); }
		inline auto abs(long64 _X) { return ::llabs(ang::move(_X)); }
		inline auto abs(double _X) { return ::fabs(ang::move(_X)); }
		inline auto abs(float _X) { return ::fabsf(ang::move(_X)); }
		inline auto abs(long double _X) { return ::fabsl(ang::move(_X)); }

		inline auto pow(double _X, double _Y) { return ::pow(ang::move(_X), ang::move(_Y)); }
		inline auto pow(float _X, float _Y) { return ::powf(ang::move(_X), ang::move(_Y)); }

		inline auto sqrt(double _X) { return ::sqrt(ang::move(_X)); }
		inline auto sqrt(float _X) { return ::sqrtf(ang::move(_X)); }

		inline auto cbrt(double _X) { return ::cbrt(ang::move(_X)); }
		inline auto cbrt(float _X) { return ::cbrtf(ang::move(_X)); }

		inline auto exp2(double _X) { return ::exp2(ang::move(_X)); }
		inline auto exp2(float _X) { return ::exp2f(ang::move(_X)); }

		inline auto exp(double _X) { return ::exp(ang::move(_X)); }
		inline auto exp(float _X) { return ::expf(ang::move(_X)); }

		inline auto expm1(double _X) { return ::expm1(ang::move(_X)); }
		inline auto expm1(float _X) { return ::expm1f(ang::move(_X)); }

		inline auto log(double _X) { return ::log(ang::move(_X)); }
		inline auto log(float _X) { return ::logf(ang::move(_X)); }

		inline auto log10(double _X) { return ::log10(ang::move(_X)); }
		inline auto log10(float _X) { return ::log10f(ang::move(_X)); }

		template<typename T, typename ...Tn>
		inline auto hypot(T a0, Tn... an) -> typename __sum_tn_res<T, Tn...>::type {
			return (typename __sum_tn_res<T, Tn...>::type)sqrt(sum_cuad(a0, an...));
		}

		inline auto asin(double _X) { return ::asin(_X); }
		inline auto asin(float _X) { return ::asinf(_X); }
	
		inline auto asinh(double _X) { return ::asinh(ang::move(_X)); }
		inline auto asinh(float _X) { return ::asinhf(ang::move(_X)); }

		inline auto acos(double _X) { return ::acos(ang::move(_X)); }
		inline auto acos(float _X) { return ::acosf(ang::move(_X)); }
	
		inline auto acosh(double _X) { return ::acosh(ang::move(_X)); }
		inline auto acosh(float _X) { return ::acoshf(ang::move(_X)); }

		inline auto atan(double _X) { return ::atan(ang::move(_X)); }
		inline auto atan(float _X) { return ::atanf(ang::move(_X)); }

		inline auto atanh(double _X) { return ::atanh(ang::move(_X)); }
		inline auto atanh(float _X) { return ::atanhf(ang::move(_X)); }

		inline auto atan2(double _Y, double _X) { return ::atan2(ang::move(_Y), ang::move(_X)); }
		inline auto atan2(float _Y, float _X) { return ::atan2f(ang::move(_Y), ang::move(_X)); }
		
		inline auto sin(double _X) { return ::sin(ang::move(_X)); }
		inline auto sin(float _X) { return ::sinf(ang::move(_X)); }

		inline auto sinh(double _X) { return ::sinh(ang::move(_X)); }
		inline auto sinh(float _X) { return ::sinhf(ang::move(_X)); }
	
		inline auto cos(double _X) { return ::cos(ang::move(_X)); }
		inline auto cos(float _X) { return ::cosf(ang::move(_X)); }

		inline auto cosh(double _X) { return ::cosh(ang::move(_X)); }
		inline auto cosh(float _X) { return ::coshf(ang::move(_X)); }
	
		inline auto tan(double _X) { return ::tan(ang::move(_X)); }
		inline auto tan(float _X) { return ::tanf(ang::move(_X)); }

		inline auto tanh(double _X) { return ::tanh(ang::move(_X)); }
		inline auto tanh(float _X) { return ::tanhf(ang::move(_X)); }

		inline auto erf(double _X) { return ::erf(ang::move(_X)); }
		inline auto erf(float _X) { return ::erff(ang::move(_X)); }

		inline auto erfc(double _X) { return ::erfc(ang::move(_X)); }
		inline auto erfc(float _X) { return ::erfcf(ang::move(_X)); }

	
		using ::ilogb; using ::lgamma;
		using ::log1p; using ::log2; using ::logb;
		using ::llrint; using ::lrint; using ::nearbyint;
		using ::rint; using ::llround; using ::lround;
		using ::fdim; using ::fma; using ::fmax; using ::fmin;
		using ::round; using ::trunc;
		using ::remainder; using ::remquo;
		using ::copysign; using ::nan; using ::nextafter;
		using ::scalbn; using ::scalbln;
		using ::nexttoward; using ::tgamma;
	}
}

#endif//__ANG_MATHS_H__
