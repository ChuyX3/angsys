/*********************************************************************************************************************/
/*   File Name: ang/maths/vectors.h                                                                                    */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/


#ifndef __ANG_VECTOR_MATHS_H__
#define __ANG_VECTOR_MATHS_H__

#include <angsys.h>

//#define ANG_FORCE_NO_USE_SIMD_TECHNOLOGY

#ifndef ANG_FORCE_NO_USE_SIMD_TECHNOLOGY
#define ANG_USE_SIMD_TECHNOLOGY
#endif//ANG_FORCE_NO_USE_SIMD_TECHNOLOGY

#if defined WINDOWS_PLATFORM
#ifdef ANG_USE_SIMD_TECHNOLOGY
#ifdef _ARM_
#include <arm_neon.h>
#else
#include <intrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#endif
#endif//ANG_USE_SIMD_TECHNOLOGY
#ifdef _ARM_
#define ANG_VECCALL __fastcall
#else //defined _X64_ || defined _X86_
#define ANG_VECCALL __vectorcall
#endif

#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#ifdef ANG_USE_SIMD_TECHNOLOGY
#include <pmmintrin.h>
#define ANG_VECCALL __attribute__((fastcall))
#endif//ANG_USE_SIMD_TECHNOLOGY
#endif

#define ANG_ALIGN(_X) alignas(_X)

namespace ang
{
	namespace maths
	{
		typedef union f32x2 {
			float xy[2];
			struct {
				float x;
				float y;
			};
		}f32x2_t, f32x2x2[2], f32x2x2_t[2];

		typedef union ANG_ALIGN(8) pack_f32x2 {
			float xy[2];
			struct {
				float x;
				float y;
			};
		}pack_f32x2_t, pack_f32x2x2[2], pack_f32x2x2_t[2];

		typedef union f32x3 {
			float xyz[3];
			struct {
				float x;
				float y;
				float z;
			};
		}f32x3_t, f32x3x3[3], f32x3x3_t[3];

		typedef union ANG_ALIGN(16) pack_f32x3 {
			float xyz[3];
			struct {
				float x;
				float y;
				float z;
			};
		}pack_f32x3_t, pack_f32x3x3[3], pack_f32x3x3_t[3];

		typedef union f32x4 {
			float xyzw[4];
			struct {
				float x;
				float y;
				float z;
				float w;
			};
		}f32x4_t, f32x4x4[4], f32x4x4_t[4];

		typedef union  ANG_ALIGN(16) pack_f32x4 {
			float xyzw[4];
			struct {
				float x;
				float y;
				float z;
				float w;
			};
		}pack_f32x4_t, pack_f32x4x4[4], pack_f32x4x4_t[4];


		typedef union f64x2 {
			double xy[2];
			struct {
				double x;
				double y;
			};
		}f64x2_t, f64x2x2[2], f64x2x2_t[2];

		typedef union ANG_ALIGN(16) pack_f64x2 {
			double xy[2];
			struct {
				double x;
				double y;
			};
		}pack_f64x2_t, pack_f64x2x2[2], pack_f64x2x2_t[2];

		typedef union f64x3 {
			double xyz[3];
			struct {
				double x;
				double y;
				double z;
			};
		}f64x3_t, f64x3x3[3], f64x3x3_t[3];

		typedef union ANG_ALIGN(16) pack_f64x3 {
			double xyz[3];
			struct {
				double x;
				double y;
				double z;
			};
		}pack_f64x3_t, pack_f64x3x3[3], pack_f64x3x3_t[3];

		typedef union f64x4 {
			double xyzw[4];
			struct {
				double x;
				double y;
				double z;
				double w;
			};
		}f64x4_t, f64x4x4[4], f64x4x4_t[4];

		typedef union ANG_ALIGN(16) pack_f64x4 {
			double xyzw[4];
			struct {
				double x;
				double y;
				double z;
				double w;
			};
		}pack_f64x4_t, pack_f64x4x4[4], pack_f64x4x4_t[4];

#ifdef ANG_USE_SIMD_TECHNOLOGY
#if defined WINDOWS_PLATFORM && defined _ARM_
		typedef pack_f32x2_t intern_f32x2_t, intern_f32x2x4_t[4];
		typedef pack_f32x3_t intern_f32x3_t, intern_f32x3x4_t[4];
		typedef __n128 intern_f32x4_t, intern_f32x4x4_t[4];

		typedef __n128 intern_f64x2_t, intern_f64x2x2_t[2];
		typedef pack_f64x3_t intern_f64x3_t, intern_f64x3x3_t[3];
		typedef union
		{
			__n128 xyzw[2];
			struct {
				__n128 xy;
				__n128 zw;
			};
		}intern_f64x4_t, intern_f64x4x4_t[4];
#else
		typedef pack_f32x2_t intern_f32x2_t, intern_f32x2x4_t[4];
		typedef pack_f32x3_t intern_f32x3_t, intern_f32x3x4_t[4];
		typedef __m128 intern_f32x4_t, intern_f32x4x4_t[4];

		typedef __m128d intern_f64x2_t, intern_f64x2x2_t[2];
		typedef pack_f64x3_t intern_f64x3_t, intern_f64x3x3_t[3];
		typedef union 
		{
			__m128d xyzw[2];
			struct {
				__m128d xy;
				__m128d zw;
			};
		}intern_f64x4_t, intern_f64x4x4_t[4];
#endif//PLATFORM
#else
		typedef pack_f32x2_t intern_f32x2_t, intern_f32x2x4_t[4];
		typedef pack_f32x3_t intern_f32x3_t, intern_f32x3x4_t[4];
		typedef pack_f32x4_t intern_f32x4_t, intern_f32x4x4_t[4];
		typedef pack_f64x2_t intern_f64x2_t, intern_f64x2x2_t[2];
		typedef pack_f64x3_t intern_f64x3_t, intern_f64x3x3_t[3];
		typedef pack_f64x4_t intern_f64x4_t, intern_f64x4x4_t[4];
#endif
		typedef ang_int16_t short2[2];
		typedef ang_int16_t short3[3];
		typedef ang_int16_t short4[4];

		typedef ang_uint16_t ushort2[2];
		typedef ang_uint16_t ushort3[3];
		typedef ang_uint16_t ushort4[4];

		typedef ang_int32_t int2[2];
		typedef ang_int32_t int3[3];
		typedef ang_int32_t int4[4];

		typedef ang_uint32_t uint2[2];
		typedef ang_uint32_t uint3[3];
		typedef ang_uint32_t uint4[4];

		typedef struct float2 fvec2, vec2;
		typedef struct float3 fvec3, vec3;
		typedef struct float4 fvec4, vec4;
		typedef struct float4x4 fmat4, mat4;

		typedef struct double2 dvec2, vec2l;
		typedef struct double3 dvec3, vec3l;
		typedef struct double4 dvec4, vec4l;
		typedef struct double4x4 dmat4, mat4l;

		namespace matrix
		{
			inline mat4 ANG_VECCALL zero();
			inline mat4 ANG_VECCALL identity();

			inline mat4l ANG_VECCALL zerol();
			inline mat4l ANG_VECCALL identityl();

			inline mat4 ANG_VECCALL transpose(mat4 const&);
			inline mat4 ANG_VECCALL invert(mat4 const&);
			inline mat4 ANG_VECCALL add(mat4 const&, mat4 const&);
			inline mat4 ANG_VECCALL sub(mat4 const&, mat4 const&);
			inline mat4 ANG_VECCALL mul(mat4 const&, mat4 const&);
			inline mat4 ANG_VECCALL mul(mat4 const&, float);
			inline mat4 ANG_VECCALL mul(float, mat4 const&);
			inline fvec4 ANG_VECCALL mul(mat4 const&, fvec4 const&);
			inline fvec4 ANG_VECCALL mul(fvec4 const&, mat4 const&);
			inline mat4 ANG_VECCALL div(mat4 const&, float);

			inline mat4l ANG_VECCALL transpose(mat4l const&);
			inline mat4l ANG_VECCALL invert(mat4l const&);
			inline mat4l ANG_VECCALL add(mat4l const&, mat4l const&);
			inline mat4l ANG_VECCALL sub(mat4l const&, mat4l const&);
			inline mat4l ANG_VECCALL mul(mat4l const&, mat4l const&);
			inline mat4l ANG_VECCALL mul(mat4l const&, double);
			inline mat4l ANG_VECCALL mul(double, mat4l const&);
			inline vec4l ANG_VECCALL mul(mat4l const&, vec4l const&);
			inline vec4l ANG_VECCALL mul(vec4l const&, mat4l const&);
			inline mat4l ANG_VECCALL div(mat4l const&, double);

			inline mat4 ANG_VECCALL rotation_x(float angle);
			inline mat4 ANG_VECCALL rotation_y(float angle);
			inline mat4 ANG_VECCALL rotation_z(float angle);
			inline mat4 ANG_VECCALL rotation_axis(float angle, float x, float y, float z);

			inline mat4 ANG_VECCALL scale(float scale);
			inline mat4 ANG_VECCALL scale(vec3 const& vec);
			inline mat4 ANG_VECCALL scale(float x, float y, float z);

			inline mat4 ANG_VECCALL translation(vec3 const& vec);
			inline mat4 ANG_VECCALL translation(float x, float y, float z);

			inline mat4 ANG_VECCALL look_at(vec3 eye, vec3 at, vec3 up);
			inline mat4 ANG_VECCALL fps(vec3 eye, float pith, float yaw, float roll = 0);

			inline mat4 ANG_VECCALL perspective(float fovY, float aspect, float near_plane, float far_plane);
		}
	}
}

#include <ang/maths/float2.h>
#include <ang/maths/float3.h>
#include <ang/maths/float4.h>
#include <ang/maths/float4x4.h>


#include <ang/maths/implement/float2.inl>
#include <ang/maths/implement/float3.inl>
#include <ang/maths/implement/double3.inl>

#ifdef ANG_USE_SIMD_TECHNOLOGY
#if defined WINDOWS_PLATFORM && defined _ARM_
#include <ang/maths/implement/double2_arm.inl>
#include <ang/maths/implement/float4_arm.inl>
#include <ang/maths/implement/double4_arm.inl>
#include <ang/maths/implement/float4x4_arm.inl>
#else 
#include <ang/maths/implement/double2_simd.inl>
#include <ang/maths/implement/float4_simd.inl>
#include <ang/maths/implement/double4_simd.inl>
#include <ang/maths/implement/float4x4_simd.inl>
#include <ang/maths/implement/maths_matrix_simd.inl>
#endif
#else
#include <ang/maths/implement/double2.inl>
#include <ang/maths/implement/float4.inl>
#include <ang/maths/implement/double4.inl>
#include <ang/maths/implement/float4x4.inl>
#include <ang/maths/implement/maths_matrix.inl>
#endif


//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::float2);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::float3);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::float4);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::float4x4);
//
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::double2);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::double3);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::double4);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::double4x4);



#define ANG_VECTOR_MATHS_DEFINE_UNALIGNED_TYPES
#ifdef ANG_VECTOR_MATHS_DEFINE_UNALIGNED_TYPES

#define float2 ul_float2
#define float3 ul_float3
#define float4 ul_float4
#define float4x4 ul_float4x4
#define double2 ul_double2
#define double3 ul_double3
#define double4 ul_double4
#define double4x4 ul_double4x4
#define zero() ul_zero()
#define identity() ul_identity()

#define rotation_x(...) ul_rotation_x(__VA_ARGS__)
#define rotation_y(...) ul_rotation_y(__VA_ARGS__)
#define rotation_z(...) ul_rotation_z(__VA_ARGS__)
#define rotation_axis(...) ul_rotation_axis(__VA_ARGS__)
#define scale(...) ul_scale(__VA_ARGS__)
#define translation(...) ul_translation(__VA_ARGS__)
#define perspective(...) ul_perspective(__VA_ARGS__)

#define intern_f32x2_t f32x2_t
#define intern_f32x3_t f32x3_t
#define intern_f32x4_t f32x4_t
#define intern_f32x4x4_t f32x4x4_t
#define intern_f64x2_t f64x2_t
#define intern_f64x3_t f64x3_t
#define intern_f64x4_t f64x4_t
#define intern_f64x4x4_t f64x4x4_t
#define zerol() ul_zerol()
#define identityl() ul_identityl()

#undef __ANG_VECTOR_MATHS_FLOAT2_H__
#include <ang/maths/float2.h>
#undef __ANG_VECTOR_MATHS_FLOAT3_H__
#include <ang/maths/float3.h>
#undef __ANG_VECTOR_MATHS_FLOAT4_H__
#include <ang/maths/float4.h>
#undef __ANG_VECTOR_MATHS_FLOAT4X4_H__
#include <ang/maths/float4x4.h>

namespace ang
{
	namespace maths
	{
		namespace matrix
		{
			inline float4x4 ANG_VECCALL zero();
			inline float4x4 ANG_VECCALL identity();

			inline double4x4 ANG_VECCALL zerol();
			inline double4x4 ANG_VECCALL identityl();

			inline float4x4 ANG_VECCALL transpose(float4x4 const&);
			inline float4x4 ANG_VECCALL invert(float4x4 const&);
			inline float4x4 ANG_VECCALL add(float4x4 const&, float4x4 const&);
			inline float4x4 ANG_VECCALL sub(float4x4 const&, float4x4 const&);
			inline float4x4 ANG_VECCALL mul(float4x4 const&, float4x4 const&);
			inline float4x4 ANG_VECCALL mul(float4x4 const&, float);
			inline float4x4 ANG_VECCALL mul(float, float4x4 const&);
			inline float4 ANG_VECCALL mul(float4x4 const&, float4 const&);
			inline float4 ANG_VECCALL mul(float4 const&, float4x4 const&);
			inline float4x4 ANG_VECCALL div(float4x4 const&, float);

			inline double4x4 ANG_VECCALL transpose(double4x4 const&);
			inline double4x4 ANG_VECCALL invert(double4x4 const&);
			inline double4x4 ANG_VECCALL add(double4x4 const&, double4x4 const&);
			inline double4x4 ANG_VECCALL sub(double4x4 const&, double4x4 const&);
			inline double4x4 ANG_VECCALL mul(double4x4 const&, double4x4 const&);
			inline double4x4 ANG_VECCALL mul(double4x4 const&, double);
			inline double4x4 ANG_VECCALL mul(double, double4x4 const&);
			inline double4 ANG_VECCALL mul(double4x4 const&, double4 const&);
			inline double4 ANG_VECCALL mul(double4 const&, double4x4 const&);
			inline double4x4 ANG_VECCALL div(double4x4 const&, double);

			inline float4x4 ANG_VECCALL rotation_x(float angle);
			inline float4x4 ANG_VECCALL rotation_y(float angle);
			inline float4x4 ANG_VECCALL rotation_z(float angle);
			inline float4x4 ANG_VECCALL rotation_axis(float angle, float x, float y, float z);

			inline float4x4 ANG_VECCALL scale(float scale);
			inline float4x4 ANG_VECCALL scale(float3 const& vec);
			inline float4x4 ANG_VECCALL scale(float x, float y, float z);

			inline float4x4 ANG_VECCALL translation(float3 const& vec);
			inline float4x4 ANG_VECCALL translation(float x, float y, float z);

			inline float4x4 ANG_VECCALL perspective(float fovY, float aspect, float near_plane, float far_plane);
		}
	}
}

#undef __ANG_VECTOR_MATHS_FLOAT2_INL__
#include <ang/maths/implement/float2.inl>
#undef __ANG_VECTOR_MATHS_FLOAT3_INL__
#include <ang/maths/implement/float3.inl>
#undef __ANG_VECTOR_MATHS_FLOAT4_INL__
#include <ang/maths/implement/float4.inl>
#undef __ANG_VECTOR_MATHS_FLOAT4X4_INL__
#include <ang/maths/implement/float4x4.inl>

#undef __ANG_VECTOR_MATHS_DOUBLE2_INL__
#include <ang/maths/implement/double2.inl>
#undef __ANG_VECTOR_MATHS_DOUBLE3_INL__
#include <ang/maths/implement/double3.inl>
#undef __ANG_VECTOR_MATHS_DOUBLE4_INL__
#include <ang/maths/implement/double4.inl>
//#undef __ANG_VECTOR_MATHS_DOUBLE4X4_HPP__
//#include <ang/maths/implement/double4x4.inl>

#undef __ANG_MATHS_MATRIX_INL__
#include <ang/maths/implement/maths_matrix.inl>

//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::float2);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::float3);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::float4);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::float4x4);
//
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::double2);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::double3);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::double4);
//ANG_REGISTER_RUNTIME_TYPENAME(ang::maths::double4x4);

#undef float2
#undef float3
#undef float4
#undef float4x4
#undef double2
#undef double3
#undef double4
#undef double4x4
#undef zero
#undef identity
#undef rotation_x
#undef rotation_y
#undef rotation_z 
#undef rotation_axis
#undef scale
#undef translation
#undef perspective

#undef intern_f32x2_t
#undef intern_f32x3_t
#undef intern_f32x4_t
#undef intern_f32x4x4_t
#undef intern_f64x2_t
#undef intern_f64x3_t
#undef intern_f64x4_t
#undef intern_f64x4x4_t
#undef zerol
#undef identityl

#endif//ANG_VECTOR_MATHS_DEFINE_UNALIGNED_TYPES


#endif//__ANG_VECTOR_MATHS_H__
