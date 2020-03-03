/*********************************************************************************************************************/
/*   File Name: ang/maths/float4.h                                                                                   */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_VECTOR_MATHS_FLOAT4_H__
#define __ANG_VECTOR_MATHS_FLOAT4_H__


namespace ang
{
	namespace maths
	{
		/********************************************************/
		/*                  float4 definition                 */
		/********************************************************/
		struct float4
		{
			intern_f32x4_t _vector;

			float4(float4&&) = default;
			float4& operator = (float4&&) = default;

			inline float4();
			inline explicit float4(float val);
			inline float4(float3 const& xyz, float w =  0);
			inline float4(float x, float y, float z, float w);
			inline float4(const float4&);
			inline float4(intern_f32x4_t const&);

			inline float4& operator = (const float4&);
			inline float4& operator += (const float4&);
			inline float4& operator -= (const float4&);
			inline float4& operator *= (float);
			inline float4& operator /= (float);

			template<uint> float& get() { throw_exception(except_code::array_overflow); }
			template<uint> float const& get()const { throw_exception(except_code::array_overflow); }
			template<uint> void set(float value) { throw_exception(except_code::array_overflow); }

			explicit inline operator float3()const {
				return *reinterpret_cast<float3 const*>(&_vector);
			}

			inline float operator [](uint)const;
			explicit inline operator float()const;//Length

			inline operator intern_f32x4_t&();
			inline operator intern_f32x4_t const&()const;

			inline bool operator == (const float4&)const;//Comparing values
			inline bool operator != (const float4&)const;//Comparing values
			inline bool operator > (const float4&)const;//Comparing length
			inline bool operator < (const float4&)const;//Comparing length

			friend inline float4 operator + (const float4&, const float4&);
			friend inline float4 operator - (const float4&, const float4&);
			friend inline float4 operator - (const float4&);
			friend inline float4 operator * (const float4&, float);
			friend inline float4 operator * (float, const float4&);
			friend inline float operator * (const float4&, const float4&);
			friend inline float4 operator % (const float4&, const float4&);
			friend inline float4 operator / (const float4&, float);
		};

		template<> inline float& float4::get<0u>();
		template<> inline float& float4::get<1u>();
		template<> inline float& float4::get<2u>();
		template<> inline float& float4::get<3u>();
		template<> inline float const& float4::get<0u>()const;
		template<> inline float const& float4::get<1u>()const;
		template<> inline float const& float4::get<2u>()const;
		template<> inline float const& float4::get<3u>()const;

		template<> inline void float4::set<0u>(float value);
		template<> inline void float4::set<1u>(float value);
		template<> inline void float4::set<2u>(float value);
		template<> inline void float4::set<3u>(float value);

		inline auto asin(float4 const& vec) { return float4(asin(vec[0]), asin(vec[1]), asin(vec[2]), asin(vec[3])); }
		inline auto acos(float4 const& vec) { return float4(acos(vec[0]), acos(vec[1]), acos(vec[2]), acos(vec[3])); }
		inline auto atan(float4 const& vec) { return float4(atan(vec[0]), atan(vec[1]), atan(vec[2]), atan(vec[3])); }

		inline auto asinh(float4 const& vec) { return float4(asinh(vec[0]), asinh(vec[1]), asinh(vec[2]), asinh(vec[3])); }
		inline auto acosh(float4 const& vec) { return float4(acosh(vec[0]), acosh(vec[1]), acosh(vec[2]), acosh(vec[3])); }
		inline auto atanh(float4 const& vec) { return float4(atanh(vec[0]), atanh(vec[1]), atanh(vec[2]), atanh(vec[3])); }

		inline auto sin(float4 const& vec) { return float4(sin(vec[0]), sin(vec[1]), sin(vec[2]), sin(vec[3])); }
		inline auto cos(float4 const& vec) { return float4(cos(vec[0]), cos(vec[1]), cos(vec[2]), cos(vec[3])); }
		inline auto tan(float4 const& vec) { return float4(tan(vec[0]), tan(vec[1]), tan(vec[2]), tan(vec[3])); }

		inline auto sinh(float4 const& vec) { return float4(sinh(vec[0]), sinh(vec[1]), sinh(vec[2]), sinh(vec[3])); }
		inline auto cosh(float4 const& vec) { return float4(cosh(vec[0]), cosh(vec[1]), cosh(vec[2]), cosh(vec[3])); }
		inline auto tanh(float4 const& vec) { return float4(tanh(vec[0]), tanh(vec[1]), tanh(vec[2]), tanh(vec[3])); }

		inline auto hypot(float4 const& vec) { return (float)vec; }
		inline float4 norm(float4 const& vec) { return  vec / (float)vec; }

		/********************************************************/
		/*                  double4 definition                   */
		/********************************************************/
		struct double4
		{

			intern_f64x4_t _vector;

			double4(double4&&) = default;
			double4& operator = (double4&&) = default;

			inline double4();
			inline double4(double val);
			inline double4(double x, double y, double z, double w);
			inline double4(const double4&);

			inline double4& operator = (const double4&);
			inline double4& operator += (const double4&);
			inline double4& operator -= (const double4&);
			inline double4& operator *= (double);
			inline double4& operator /= (double);

			template<uint> double& get() { throw_exception(except_code::array_overflow); }
			template<uint> double const& get()const { throw_exception(except_code::array_overflow); }
			template<uint> void set(double value) { throw_exception(except_code::array_overflow); }

			inline double operator [](uint)const;
			explicit inline operator double()const;//Length

			inline bool operator == (const double4&)const;//Comparing values
			inline bool operator != (const double4&)const;//Comparing values
			inline bool operator > (const double4&)const;//Comparing length
			inline bool operator < (const double4&)const;//Comparing length

			friend inline double4 operator + (const double4&, const double4&);
			friend inline double4 operator - (const double4&, const double4&);
			friend inline double4 operator - (const double4&);
			friend inline double4 operator * (const double4&, double);
			friend inline double4 operator * (double, const double4&);
			friend inline double operator * (const double4&, const double4&);
			friend inline double4 operator % (const double4&, const double4&);
			friend inline double4 operator / (const double4&, double);
		};

		template<> inline double& double4::get<0u>();
		template<> inline double& double4::get<1u>();
		template<> inline double& double4::get<2u>();
		template<> inline double& double4::get<3u>();
		template<> inline double const& double4::get<0u>()const;
		template<> inline double const& double4::get<1u>()const;
		template<> inline double const& double4::get<2u>()const;
		template<> inline double const& double4::get<3u>()const;

		template<> inline void double4::set<0u>(double value);
		template<> inline void double4::set<1u>(double value);
		template<> inline void double4::set<2u>(double value);
		template<> inline void double4::set<3u>(double value);

		inline auto asin(double4 const& vec) { return double4(asin(vec[0]), asin(vec[1]), asin(vec[2]), asin(vec[3])); }
		inline auto acos(double4 const& vec) { return double4(acos(vec[0]), acos(vec[1]), acos(vec[2]), acos(vec[3])); }
		inline auto atan(double4 const& vec) { return double4(atan(vec[0]), atan(vec[1]), atan(vec[2]), atan(vec[3])); }

		inline auto asinh(double4 const& vec) { return double4(asinh(vec[0]), asinh(vec[1]), asinh(vec[2]), asinh(vec[3])); }
		inline auto acosh(double4 const& vec) { return double4(acosh(vec[0]), acosh(vec[1]), acosh(vec[2]), acosh(vec[3])); }
		inline auto atanh(double4 const& vec) { return double4(atanh(vec[0]), atanh(vec[1]), atanh(vec[2]), atanh(vec[3])); }

		inline auto sin(double4 const& vec) { return double4(sin(vec[0]), sin(vec[1]), sin(vec[2]), sin(vec[3])); }
		inline auto cos(double4 const& vec) { return double4(cos(vec[0]), cos(vec[1]), cos(vec[2]), cos(vec[3])); }
		inline auto tan(double4 const& vec) { return double4(tan(vec[0]), tan(vec[1]), tan(vec[2]), tan(vec[3])); }

		inline auto sinh(double4 const& vec) { return double4(sinh(vec[0]), sinh(vec[1]), sinh(vec[2]), sinh(vec[3])); }
		inline auto cosh(double4 const& vec) { return double4(cosh(vec[0]), cosh(vec[1]), cosh(vec[2]), cosh(vec[3])); }
		inline auto tanh(double4 const& vec) { return double4(tanh(vec[0]), tanh(vec[1]), tanh(vec[2]), tanh(vec[3])); }

		inline auto hypot(double4 const& vec) { return (double)vec; }
		inline double4 norm(double4 const& vec) { return  vec / (double)vec; }

	}
}

ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ang::maths::float4);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ang::maths::double4);


#endif//__ANG_GRAPH_MATHS_FLOAT4_H__