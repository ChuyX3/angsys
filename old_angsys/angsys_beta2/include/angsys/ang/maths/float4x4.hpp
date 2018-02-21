#ifndef __ANG_VECTOR_MATHS_FLOAT4X4_H__
#define __ANG_VECTOR_MATHS_FLOAT4X4_H__

namespace ang
{
	namespace maths
	{
		/********************************************************/
		/*                  float4x4 definition                 */
		/********************************************************/
		struct float4x4
		{
			intern_f32x4x4_t _mat;

			float4x4(float4x4&&) = default;
			float4x4& operator = (float4x4&&) = default;

			inline float4x4();
			inline float4x4(float val);
			inline float4x4(float4 const& v0, float4 const& v1, float4 const& v2, float4 const& v3);
			inline float4x4(const float4x4&);
			inline float4x4(f32x4x4_t const& mat);

			inline float4x4& operator = (const float4x4&);
			inline float4x4& operator += (const float4x4&);
			inline float4x4& operator -= (const float4x4&);
			inline float4x4& operator *= (float);
			inline float4x4& operator /= (float);

			template<uint> float4& get() { throw(exception(except_code::array_overflow)); }
			template<uint> float4 const& get()const { throw(exception(except_code::array_overflow)); }
			template<uint> void set(float4 const& value) { throw(exception(except_code::array_overflow)); }

			template<uint X, uint Y> float get()const { return get<X>().get<Y>(); }
			template<uint X, uint Y> void set(float value) { get<X>().set<Y>(value); }

			inline float4& operator [](uint);
			inline float4 const& operator [](uint)const;

			inline bool operator == (const float4x4&)const;//Comparing values
			inline bool operator != (const float4x4&)const;//Comparing values

			friend inline float4x4 operator - (const float4x4&);//invert
			friend inline float4x4 operator ~ (const float4x4&);//traspouse
			friend inline float4x4 operator + (const float4x4&, const float4x4&);
			friend inline float4x4 operator - (const float4x4&, const float4x4&);
			friend inline float4x4 operator * (float4x4 const&, float4x4 const&);
			friend inline float4 operator * (float4x4 const&, const float4&);
			friend inline float4 operator * (const float4&, float4x4 const&);
			friend inline float4x4 operator * (float4x4 const&, float);
			friend inline float4x4 operator * (float, float4x4 const&);
			friend inline float4x4 operator / (const float4x4&, float);
		};


		/********************************************************/
		/*                  double4x4 definition                */
		/********************************************************/
		struct double4x4
		{
			intern_f64x4x4_t _mat;

			double4x4(double4x4&&) = default;
			double4x4& operator = (double4x4&&) = default;

			inline double4x4();
			inline double4x4(double val);
			inline double4x4(double4 const& v0, double4 const& v1, double4 const& v2, double4 const& v3);
			inline double4x4(const double4x4&);

			inline double4x4& operator = (const double4&);
			inline double4x4& operator += (const double4&);
			inline double4x4& operator -= (const double4&);
			inline double4x4& operator *= (double);
			inline double4x4& operator /= (double);

			template<uint> double4& get() { throw(exception(except_code::array_overflow)); }
			template<uint> double4 const& get()const { throw(exception(except_code::array_overflow)); }
			template<uint> void set(double4 const& value) { throw(exception(except_code::array_overflow)); }

			template<uint X, uint Y> double get()const { return get<X>().get<Y>(); }
			template<uint X, uint Y> void set(double value) { get<X>().set<Y>(value); }

			inline double4& operator [](uint);
			inline double4 const& operator [](uint)const;

			inline bool operator == (const double4x4&)const;//comparing values
			inline bool operator != (const double4x4&)const;//comparing values

			friend inline double4x4 operator - (const double4x4&);//invert
			friend inline double4x4 operator ~ (const double4x4&);//traspouse
			friend inline double4x4 operator + (const double4x4&, const double4x4&);
			friend inline double4x4 operator - (const double4x4&, const double4x4&);
			friend inline double4x4 operator * (double4x4 const&, double4x4 const&);
			friend inline double4 operator * (double4x4 const&, const double4&);
			friend inline double4 operator * (const double4&, double4x4 const&);
			friend inline double4x4 operator * (double4x4 const&, double);
			friend inline double4x4 operator * (double, double4x4 const&);
			friend inline double4x4 operator / (const double4x4&, double);
		};
	}
}

#endif//__ANG_VECTOR_MATHS_FLOAT4X4_H__