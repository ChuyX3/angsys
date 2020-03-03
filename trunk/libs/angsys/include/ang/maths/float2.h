/*********************************************************************************************************************/
/*   File Name: ang/maths/float2.h                                                                                    */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_VECTOR_MATHS_FLOAT2_H__
#define __ANG_VECTOR_MATHS_FLOAT2_H__

namespace ang
{
	namespace maths
	{
		/********************************************************/
		/*                  float2 definition                 */
		/********************************************************/
		struct float2
		{
			intern_f32x2_t _vector;

			float2(float2&&) = default;
			float2& operator = (float2&&) = default;

			inline float2();
			inline float2(float val);
			inline float2(float x, float y);
			inline float2(const float2&);
			inline float2(const f32x2&);

			inline float2& operator = (const float2&);
			inline float2& operator += (const float2&);
			inline float2& operator -= (const float2&);
			inline float2& operator *= (float);
			inline float2& operator /= (float);

			template<uint> float& get() { throw_exception(except_code::array_overflow); }
			template<uint> float const& get()const { throw_exception(except_code::array_overflow); }
			template<uint> void set(float value) { throw_exception(except_code::array_overflow); }

			inline float operator [](uint)const;
			explicit inline operator float()const;//Length

			inline bool operator == (const float2&)const;//Comparing values
			inline bool operator != (const float2&)const;//Comparing values
			inline bool operator > (const float2&)const;//Comparing length
			inline bool operator < (const float2&)const;//Comparing length

			friend inline float2 operator + (const float2&, const float2&);
			friend inline float2 operator - (const float2&, const float2&);
			friend inline float2 operator - (const float2&);
			friend inline float2 operator * (const float2&, float);
			friend inline float2 operator * (float, const float2&);
			friend inline float operator * (const float2&, const float2&);
			friend inline float2 operator % (const float2&, const float2&);
			friend inline float2 operator / (const float2&, float);
		};

		template<> inline float& float2::get<0u>();
		template<> inline float& float2::get<1u>();
		template<> inline float const& float2::get<0u>()const;
		template<> inline float const& float2::get<1u>()const;
		template<> inline void float2::set<0u>(float value);
		template<> inline void float2::set<1u>(float value);

		inline auto asin(float2 const& vec) { return float2(asin(vec[0]), asin(vec[1])); }
		inline auto acos(float2 const& vec) { return float2(acos(vec[0]), acos(vec[1])); }
		inline auto atan(float2 const& vec) { return float2(atan(vec[0]), atan(vec[1])); }

		inline auto asinh(float2 const& vec) { return float2(asinh(vec[0]), asinh(vec[1])); }
		inline auto acosh(float2 const& vec) { return float2(acosh(vec[0]), acosh(vec[1])); }
		inline auto atanh(float2 const& vec) { return float2(atanh(vec[0]), atanh(vec[1])); }

		inline auto sin(float2 const& vec) { return float2(sin(vec[0]), sin(vec[1])); }
		inline auto cos(float2 const& vec) { return float2(cos(vec[0]), cos(vec[1])); }
		inline auto tan(float2 const& vec) { return float2(tan(vec[0]), tan(vec[1])); }

		inline auto sinh(float2 const& vec) { return float2(sinh(vec[0]), sinh(vec[1])); }
		inline auto cosh(float2 const& vec) { return float2(cosh(vec[0]), cosh(vec[1])); }
		inline auto tanh(float2 const& vec) { return float2(tanh(vec[0]), tanh(vec[1])); }

		inline auto hypot(float2 const& vec) { return (float)vec; }
		inline float2 norm(float2 const& vec) { return  vec / (float)vec; }

		/********************************************************/
		/*                  double2 definition                   */
		/********************************************************/
		struct double2
		{
			intern_f64x2_t _vector;

			double2(double2&&) = default;
			double2& operator = (double2&&) = default;

			inline double2();
			inline double2(double val);
			inline double2(double x, double y);
			inline double2(const double2&);

			inline double2& operator = (const double2&);
			inline double2& operator += (const double2&);
			inline double2& operator -= (const double2&);
			inline double2& operator *= (double);
			inline double2& operator /= (double);

			template<uint> double& get() { throw_exception(except_code::array_overflow); }
			template<uint> double const& get()const { throw_exception(except_code::array_overflow); }
			template<uint> void set(double value) { throw_exception(except_code::array_overflow); }

			inline double operator [](uint)const;
			explicit inline operator double()const;//Length

			inline bool operator == (const double2&)const;//Comparing values
			inline bool operator != (const double2&)const;//Comparing values
			inline bool operator > (const double2&)const;//Comparing length
			inline bool operator < (const double2&)const;//Comparing length

			friend inline double2 operator + (const double2&, const double2&);
			friend inline double2 operator - (const double2&, const double2&);
			friend inline double2 operator - (const double2&);
			friend inline double2 operator * (const double2&, double);
			friend inline double2 operator * (double, const double2&);
			friend inline double operator * (const double2&, const double2&);
			friend inline double2 operator % (const double2&, const double2&);
			friend inline double2 operator / (const double2&, double);
		};

		template<> inline double& double2::get<0u>();
		template<> inline double& double2::get<1u>();
		template<> inline double const& double2::get<0u>()const;
		template<> inline double const& double2::get<1u>()const;
		template<> inline void double2::set<0u>(double value);
		template<> inline void double2::set<1u>(double value);

		inline auto asin(double2 const& vec) { return double2(asin(vec[0]), asin(vec[1])); }
		inline auto acos(double2 const& vec) { return double2(acos(vec[0]), acos(vec[1])); }
		inline auto atan(double2 const& vec) { return double2(atan(vec[0]), atan(vec[1])); }

		inline auto asinh(double2 const& vec) { return double2(asinh(vec[0]), asinh(vec[1])); }
		inline auto acosh(double2 const& vec) { return double2(acosh(vec[0]), acosh(vec[1])); }
		inline auto atanh(double2 const& vec) { return double2(atanh(vec[0]), atanh(vec[1])); }

		inline auto sin(double2 const& vec) { return double2(sin(vec[0]), sin(vec[1])); }
		inline auto cos(double2 const& vec) { return double2(cos(vec[0]), cos(vec[1])); }
		inline auto tan(double2 const& vec) { return double2(tan(vec[0]), tan(vec[1])); }

		inline auto sinh(double2 const& vec) { return double2(sinh(vec[0]), sinh(vec[1])); }
		inline auto cosh(double2 const& vec) { return double2(cosh(vec[0]), cosh(vec[1])); }
		inline auto tanh(double2 const& vec) { return double2(tanh(vec[0]), tanh(vec[1])); }

		inline auto hypot(double2 const& vec) { return (double)vec; }
		inline double2 norm(double2 const& vec) { return  vec / (double)vec; }
	}
}

ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ang::maths::float2);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ang::maths::double2);

#endif//__ANG_VECTOR_MATHS_FLOAT2_H__