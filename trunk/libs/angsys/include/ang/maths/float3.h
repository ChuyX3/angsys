/*********************************************************************************************************************/
/*   File Name: ang/maths/float3.h                                                                                    */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_VECTOR_MATHS_FLOAT3_H__
#define __ANG_VECTOR_MATHS_FLOAT3_H__


namespace ang
{
	namespace maths
	{
		/********************************************************/
		/*                  float3 definition                 */
		/********************************************************/
		struct float3
		{
			intern_f32x3_t _vector;

			//float3(float3&&) = default;
			//float3& operator = (float3&&) = default;

			inline float3();
			inline float3(float val);
			inline float3(float x, float y, float z);
			inline float3(const float2&, float z);
			inline float3(const float3&);
			inline float3(const f32x3_t&);

			inline float3& operator = (const float3&);
			inline float3& operator += (const float3&);
			inline float3& operator -= (const float3&);
			inline float3& operator *= (float);
			inline float3& operator /= (float);

			template<uint> float& get() { throw_exception(except_code::array_overflow); }
			template<uint> float const& get()const { throw_exception(except_code::array_overflow); }
			template<uint> void set(float value) { throw_exception(except_code::array_overflow); }

			inline float operator [](uint)const;
			explicit inline operator float()const;//Length

			inline bool operator == (const float3&)const;//Comparing values
			inline bool operator != (const float3&)const;//Comparing values
			inline bool operator > (const float3&)const;//Comparing length
			inline bool operator < (const float3&)const;//Comparing length

			friend inline float3 operator + (const float3&, const float3&);
			friend inline float3 operator - (const float3&, const float3&);
			friend inline float3 operator - (const float3&);
			friend inline float3 operator * (const float3&, float);
			friend inline float3 operator * (float, const float3&);
			friend inline float operator * (const float3&, const float3&);
			friend inline float3 operator % (const float3&, const float3&);
			friend inline float3 operator / (const float3&, float);
		};

		template<> inline float& float3::get<0u>();
		template<> inline float& float3::get<1u>();
		template<> inline float& float3::get<2u>();
		template<> inline float const& float3::get<0u>()const;
		template<> inline float const& float3::get<1u>()const;
		template<> inline float const& float3::get<2u>()const;

		template<> inline void float3::set<0u>(float value);
		template<> inline void float3::set<1u>(float value);
		template<> inline void float3::set<2u>(float value);

		inline auto asin(float3 const& vec) { return float3(asin(vec[0]), asin(vec[1]), asin(vec[2])); }
		inline auto acos(float3 const& vec) { return float3(acos(vec[0]), acos(vec[1]), acos(vec[2])); }
		inline auto atan(float3 const& vec) { return float3(atan(vec[0]), atan(vec[1]), atan(vec[2])); }

		inline auto asinh(float3 const& vec) { return float3(asinh(vec[0]), asinh(vec[1]), asinh(vec[2])); }
		inline auto acosh(float3 const& vec) { return float3(acosh(vec[0]), acosh(vec[1]), acosh(vec[2])); }
		inline auto atanh(float3 const& vec) { return float3(atanh(vec[0]), atanh(vec[1]), atanh(vec[2])); }

		inline auto sin(float3 const& vec) { return float3(sin(vec[0]), sin(vec[1]), sin(vec[2])); }
		inline auto cos(float3 const& vec) { return float3(cos(vec[0]), cos(vec[1]), cos(vec[2])); }
		inline auto tan(float3 const& vec) { return float3(tan(vec[0]), tan(vec[1]), tan(vec[2])); }

		inline auto sinh(float3 const& vec) { return float3(sinh(vec[0]), sinh(vec[1]), sinh(vec[2])); }
		inline auto cosh(float3 const& vec) { return float3(cosh(vec[0]), cosh(vec[1]), cosh(vec[2])); }
		inline auto tanh(float3 const& vec) { return float3(tanh(vec[0]), tanh(vec[1]), tanh(vec[2])); }

		inline auto hypot(float3 const& vec) { return (float)vec; }
		inline float3 norm(float3 const& vec) { return  vec / (float)vec; }
		inline float3 cross(float3 const&, float3 const&);

		/********************************************************/
		/*                  double3 definition                   */
		/********************************************************/
		struct double3
		{
			intern_f64x3_t _vector;

			//double3(double3&&) = default;
			//double3& operator = (double3&&) = default;

			inline double3();
			inline double3(double val);
			inline double3(double x, double y, double z);
			inline double3(const double3&);

			inline double3& operator = (const double3&);
			inline double3& operator += (const double3&);
			inline double3& operator -= (const double3&);
			inline double3& operator *= (double);
			inline double3& operator /= (double);

			template<uint> double& get() { throw_exception(except_code::array_overflow); }
			template<uint> double const& get()const { throw_exception(except_code::array_overflow); }
			template<uint> void set(double value) { throw_exception(except_code::array_overflow); }

			inline double operator [](uint)const;
			explicit inline operator double()const;//Length

			inline bool operator == (const double3&)const;//Comparing values
			inline bool operator != (const double3&)const;//Comparing values
			inline bool operator > (const double3&)const;//Comparing length
			inline bool operator < (const double3&)const;//Comparing length

			friend inline double3 operator + (const double3&, const double3&);
			friend inline double3 operator - (const double3&, const double3&);
			friend inline double3 operator - (const double3&);
			friend inline double3 operator * (const double3&, double);
			friend inline double3 operator * (double, const double3&);
			friend inline double operator * (const double3&, const double3&);
			friend inline double3 operator % (const double3&, const double3&);
			friend inline double3 operator / (const double3&, double);
		};

		template<> inline double& double3::get<0u>();
		template<> inline double& double3::get<1u>();
		template<> inline double& double3::get<2u>();

		template<> inline double const& double3::get<0u>()const;
		template<> inline double const& double3::get<1u>()const;
		template<> inline double const& double3::get<2u>()const;

		template<> inline void double3::set<0u>(double value);
		template<> inline void double3::set<1u>(double value);
		template<> inline void double3::set<2u>(double value);


		inline auto asin(double3 const& vec) { return double3(asin(vec[0]), asin(vec[1]), asin(vec[2])); }
		inline auto acos(double3 const& vec) { return double3(acos(vec[0]), acos(vec[1]), acos(vec[2])); }
		inline auto atan(double3 const& vec) { return double3(atan(vec[0]), atan(vec[1]), atan(vec[2])); }

		inline auto asinh(double3 const& vec) { return double3(asinh(vec[0]), asinh(vec[1]), asinh(vec[2])); }
		inline auto acosh(double3 const& vec) { return double3(acosh(vec[0]), acosh(vec[1]), acosh(vec[2])); }
		inline auto atanh(double3 const& vec) { return double3(atanh(vec[0]), atanh(vec[1]), atanh(vec[2])); }

		inline auto sin(double3 const& vec) { return double3(sin(vec[0]), sin(vec[1]), sin(vec[2])); }
		inline auto cos(double3 const& vec) { return double3(cos(vec[0]), cos(vec[1]), cos(vec[2])); }
		inline auto tan(double3 const& vec) { return double3(tan(vec[0]), tan(vec[1]), tan(vec[2])); }

		inline auto sinh(double3 const& vec) { return double3(sinh(vec[0]), sinh(vec[1]), sinh(vec[2])); }
		inline auto cosh(double3 const& vec) { return double3(cosh(vec[0]), cosh(vec[1]), cosh(vec[2])); }
		inline auto tanh(double3 const& vec) { return double3(tanh(vec[0]), tanh(vec[1]), tanh(vec[2])); }

		inline auto hypot(double3 const& vec) { return (double)vec; }
		inline double3 norm(double3 const& vec) { return  vec / (double)vec; }
		inline double3 cross(double3 const&, double3 const&);
	}
}

ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ang::maths::float3);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ang::maths::double3);


#endif//__ANG_VECTOR_MATHS_FLOAT2_H__