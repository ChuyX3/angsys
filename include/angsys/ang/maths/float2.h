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

			template<uint> float get()const { throw(exception(except_code::array_overflow)); }
			template<uint> void set(float value) { throw(exception(except_code::array_overflow)); }

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

			template<uint> double get()const { throw(exception(except_code::array_overflow)); }
			template<uint> void set(double value) { throw(exception(except_code::array_overflow)); }

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
	
		inline auto asin(float2 const& _V) { return float2(asin(_V[0]), asin(_V[1])); }
		inline auto acos(float2 const& _V) { return float2(acos(_V[0]), acos(_V[1])); }
		inline auto atan(float2 const& _V) { return float2(atan(_V[0]), atan(_V[1])); }

		inline auto asinh(float2 const& _V) { return float2(asinh(_V[0]), asinh(_V[1])); }
		inline auto acosh(float2 const& _V) { return float2(acosh(_V[0]), acosh(_V[1])); }
		inline auto atanh(float2 const& _V) { return float2(atanh(_V[0]), atanh(_V[1])); }

		inline auto sin(float2 const& _V) { return float2(sin(_V[0]), sin(_V[1])); }
		inline auto cos(float2 const& _V) { return float2(cos(_V[0]), cos(_V[1])); }
		inline auto tan(float2 const& _V) { return float2(tan(_V[0]), tan(_V[1])); }

		inline auto sinh(float2 const& _V) { return float2(sinh(_V[0]), sinh(_V[1])); }
		inline auto cosh(float2 const& _V) { return float2(cosh(_V[0]), cosh(_V[1])); }
		inline auto tanh(float2 const& _V) { return float2(tanh(_V[0]), tanh(_V[1])); }

		inline auto asin(double2 const& _V) { return double2(asin(_V[0]), asin(_V[1])); }
		inline auto acos(double2 const& _V) { return double2(acos(_V[0]), acos(_V[1])); }
		inline auto atan(double2 const& _V) { return double2(atan(_V[0]), atan(_V[1])); }

		inline auto asinh(double2 const& _V) { return double2(asinh(_V[0]), asinh(_V[1])); }
		inline auto acosh(double2 const& _V) { return double2(acosh(_V[0]), acosh(_V[1])); }
		inline auto atanh(double2 const& _V) { return double2(atanh(_V[0]), atanh(_V[1])); }

		inline auto sin(double2 const& _V) { return double2(sin(_V[0]), sin(_V[1])); }
		inline auto cos(double2 const& _V) { return double2(cos(_V[0]), cos(_V[1])); }
		inline auto tan(double2 const& _V) { return double2(tan(_V[0]), tan(_V[1])); }

		inline auto sinh(double2 const& _V) { return double2(sinh(_V[0]), sinh(_V[1])); }
		inline auto cosh(double2 const& _V) { return double2(cosh(_V[0]), cosh(_V[1])); }
		inline auto tanh(double2 const& _V) { return double2(tanh(_V[0]), tanh(_V[1])); }

		inline auto hypot(float2 const& vec) { return (float)vec; }
		inline auto hypot(double2 const& vec) { return (double)vec; }
	}
}

#endif//__ANG_VECTOR_MATHS_FLOAT2_H__