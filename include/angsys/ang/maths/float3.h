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

			float3(float3&&) = default;
			float3& operator = (float3&&) = default;

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

			template<uint> float get()const { throw(exception(except_code::array_overflow)); }
			template<uint> void set(float value) { throw(exception(except_code::array_overflow)); }

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


		/********************************************************/
		/*                  double3 definition                   */
		/********************************************************/
		struct double3
		{
			intern_f64x3_t _vector;

			double3(double3&&) = default;
			double3& operator = (double3&&) = default;

			inline double3();
			inline double3(double val);
			inline double3(double x, double y, double z);
			inline double3(const double3&);

			inline double3& operator = (const double3&);
			inline double3& operator += (const double3&);
			inline double3& operator -= (const double3&);
			inline double3& operator *= (double);
			inline double3& operator /= (double);

			template<uint> double get()const { throw(exception(except_code::array_overflow)); }
			template<uint> void set(double value) { throw(exception(except_code::array_overflow)); }

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


		inline auto asin(float3 const& _V) { return float3(asin(_V[0]), asin(_V[1]), asin(_V[2])); }
		inline auto acos(float3 const& _V) { return float3(acos(_V[0]), acos(_V[1]), acos(_V[2])); }
		inline auto atan(float3 const& _V) { return float3(atan(_V[0]), atan(_V[1]), atan(_V[2])); }

		inline auto asinh(float3 const& _V) { return float3(asinh(_V[0]), asinh(_V[1]), asinh(_V[2])); }
		inline auto acosh(float3 const& _V) { return float3(acosh(_V[0]), acosh(_V[1]), acosh(_V[2])); }
		inline auto atanh(float3 const& _V) { return float3(atanh(_V[0]), atanh(_V[1]), atanh(_V[2])); }

		inline auto sin(float3 const& _V) { return float3(sin(_V[0]), sin(_V[1]), sin(_V[2])); }
		inline auto cos(float3 const& _V) { return float3(cos(_V[0]), cos(_V[1]), cos(_V[2])); }
		inline auto tan(float3 const& _V) { return float3(tan(_V[0]), tan(_V[1]), tan(_V[2])); }

		inline auto sinh(float3 const& _V) { return float3(sinh(_V[0]), sinh(_V[1]), sinh(_V[2])); }
		inline auto cosh(float3 const& _V) { return float3(cosh(_V[0]), cosh(_V[1]), cosh(_V[2])); }
		inline auto tanh(float3 const& _V) { return float3(tanh(_V[0]), tanh(_V[1]), tanh(_V[2])); }

		inline auto asin(double3 const& _V) { return double3(asin(_V[0]), asin(_V[1]), asin(_V[2])); }
		inline auto acos(double3 const& _V) { return double3(acos(_V[0]), acos(_V[1]), acos(_V[2])); }
		inline auto atan(double3 const& _V) { return double3(atan(_V[0]), atan(_V[1]), atan(_V[2])); }

		inline auto asinh(double3 const& _V) { return double3(asinh(_V[0]), asinh(_V[1]), asinh(_V[2])); }
		inline auto acosh(double3 const& _V) { return double3(acosh(_V[0]), acosh(_V[1]), acosh(_V[2])); }
		inline auto atanh(double3 const& _V) { return double3(atanh(_V[0]), atanh(_V[1]), atanh(_V[2])); }

		inline auto sin(double3 const& _V) { return double3(sin(_V[0]), sin(_V[1]), sin(_V[2])); }
		inline auto cos(double3 const& _V) { return double3(cos(_V[0]), cos(_V[1]), cos(_V[2])); }
		inline auto tan(double3 const& _V) { return double3(tan(_V[0]), tan(_V[1]), tan(_V[2])); }

		inline auto sinh(double3 const& _V) { return double3(sinh(_V[0]), sinh(_V[1]), sinh(_V[2])); }
		inline auto cosh(double3 const& _V) { return double3(cosh(_V[0]), cosh(_V[1]), cosh(_V[2])); }
		inline auto tanh(double3 const& _V) { return double3(tanh(_V[0]), tanh(_V[1]), tanh(_V[2])); }

		inline auto hypot(float3 const& vec) { return (float)vec; }
		inline auto hypot(double3 const& vec) { return (double)vec; }
	}
}

#endif//__ANG_VECTOR_MATHS_FLOAT2_H__