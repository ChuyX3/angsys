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

			inline float4& operator = (const float4&);
			inline float4& operator += (const float4&);
			inline float4& operator -= (const float4&);
			inline float4& operator *= (float);
			inline float4& operator /= (float);

			template<uint> float& get() { throw(exception(except_code::array_overflow)); }
			template<uint> float const& get()const { throw(exception(except_code::array_overflow)); }
			template<uint> void set(float value) { throw(exception(except_code::array_overflow)); }

			explicit inline operator float3()const {
				return *reinterpret_cast<float3 const*>(&_vector);
			}

			inline float operator [](uint)const;
			explicit inline operator float()const;//Length

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

		inline auto asin(float4 const& _V) { return float4(asin(_V[0]), asin(_V[1]), asin(_V[2]), asin(_V[3])); }
		inline auto acos(float4 const& _V) { return float4(acos(_V[0]), acos(_V[1]), acos(_V[2]), acos(_V[3])); }
		inline auto atan(float4 const& _V) { return float4(atan(_V[0]), atan(_V[1]), atan(_V[2]), atan(_V[3])); }

		inline auto asinh(float4 const& _V) { return float4(asinh(_V[0]), asinh(_V[1]), asinh(_V[2]), asinh(_V[3])); }
		inline auto acosh(float4 const& _V) { return float4(acosh(_V[0]), acosh(_V[1]), acosh(_V[2]), acosh(_V[3])); }
		inline auto atanh(float4 const& _V) { return float4(atanh(_V[0]), atanh(_V[1]), atanh(_V[2]), atanh(_V[3])); }

		inline auto sin(float4 const& _V) { return float4(sin(_V[0]), sin(_V[1]), sin(_V[2]), sin(_V[3])); }
		inline auto cos(float4 const& _V) { return float4(cos(_V[0]), cos(_V[1]), cos(_V[2]), cos(_V[3])); }
		inline auto tan(float4 const& _V) { return float4(tan(_V[0]), tan(_V[1]), tan(_V[2]), tan(_V[3])); }

		inline auto sinh(float4 const& _V) { return float4(sinh(_V[0]), sinh(_V[1]), sinh(_V[2]), sinh(_V[3])); }
		inline auto cosh(float4 const& _V) { return float4(cosh(_V[0]), cosh(_V[1]), cosh(_V[2]), cosh(_V[3])); }
		inline auto tanh(float4 const& _V) { return float4(tanh(_V[0]), tanh(_V[1]), tanh(_V[2]), tanh(_V[3])); }

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

			template<uint> double& get() { throw(exception(except_code::array_overflow)); }
			template<uint> double const& get()const { throw(exception(except_code::array_overflow)); }
			template<uint> void set(double value) { throw(exception(except_code::array_overflow)); }

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

		inline auto asin(double4 const& _V) { return double4(asin(_V[0]), asin(_V[1]), asin(_V[2]), asin(_V[3])); }
		inline auto acos(double4 const& _V) { return double4(acos(_V[0]), acos(_V[1]), acos(_V[2]), acos(_V[3])); }
		inline auto atan(double4 const& _V) { return double4(atan(_V[0]), atan(_V[1]), atan(_V[2]), atan(_V[3])); }

		inline auto asinh(double4 const& _V) { return double4(asinh(_V[0]), asinh(_V[1]), asinh(_V[2]), asinh(_V[3])); }
		inline auto acosh(double4 const& _V) { return double4(acosh(_V[0]), acosh(_V[1]), acosh(_V[2]), acosh(_V[3])); }
		inline auto atanh(double4 const& _V) { return double4(atanh(_V[0]), atanh(_V[1]), atanh(_V[2]), atanh(_V[3])); }

		inline auto sin(double4 const& _V) { return double4(sin(_V[0]), sin(_V[1]), sin(_V[2]), sin(_V[3])); }
		inline auto cos(double4 const& _V) { return double4(cos(_V[0]), cos(_V[1]), cos(_V[2]), cos(_V[3])); }
		inline auto tan(double4 const& _V) { return double4(tan(_V[0]), tan(_V[1]), tan(_V[2]), tan(_V[3])); }

		inline auto sinh(double4 const& _V) { return double4(sinh(_V[0]), sinh(_V[1]), sinh(_V[2]), sinh(_V[3])); }
		inline auto cosh(double4 const& _V) { return double4(cosh(_V[0]), cosh(_V[1]), cosh(_V[2]), cosh(_V[3])); }
		inline auto tanh(double4 const& _V) { return double4(tanh(_V[0]), tanh(_V[1]), tanh(_V[2]), tanh(_V[3])); }

		inline auto hypot(double4 const& vec) { return (double)vec; }
		inline double4 norm(double4 const& vec) { return  vec / (double)vec; }

	}
}

#endif//__ANG_GRAPH_MATHS_FLOAT4_H__