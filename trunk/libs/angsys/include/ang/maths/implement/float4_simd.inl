/*********************************************************************************************************************/
/*   File Name: ang/maths/float4_simd.inl                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_VECTOR_MATHS_H__
#error ...
#elif !defined __ANG_VECTOR_MATHS_FLOAT4_INL__
#define __ANG_VECTOR_MATHS_FLOAT4_INL__


namespace ang
{
	namespace maths
	{
		inline float _mm_dot_ps(__m128 const& v0, __m128 const& v1) {
			__m128 __temp = _mm_mul_ps(v0, v1);
			__temp = _mm_hadd_ps(__temp, __temp);
			__temp = _mm_hadd_ps(__temp, __temp);
			return reinterpret_cast<float*>(&__temp)[0];
		}
	}
}


inline ang::maths::float4::float4()
	: _vector(_mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f))
{

}

inline ang::maths::float4::float4(float val)
	: _vector(_mm_set_ps1(val))
{

}

inline ang::maths::float4::float4(ang::maths::float3 const& xyz, float w)
	: _vector(_mm_set_ps(w, xyz.get<2>(), xyz.get<1>(), xyz.get<0>()))
{

}

inline ang::maths::float4::float4(float x, float y, float z, float w)
	: _vector(_mm_set_ps(w, z, y, x))
{

}

inline ang::maths::float4::float4(const ang::maths::float4& vec)
	: _vector(vec._vector)
{

}

inline ang::maths::float4::float4(const ang::maths::intern_f32x4_t& vec)
	: _vector(vec)
{

}

inline ang::maths::float4& ang::maths::float4::operator = (const ang::maths::float4& vec) {
	_vector = vec._vector;
	return*this;
}

inline ang::maths::float4& ang::maths::float4::operator += (const ang::maths::float4& vec) {
	_vector = _mm_add_ps(_vector, vec._vector);
	return*this;
}

inline ang::maths::float4& ang::maths::float4::operator -= (const ang::maths::float4& vec) {
	_vector = _mm_sub_ps(_vector, vec._vector);
	return*this;
}
inline ang::maths::float4& ang::maths::float4::operator *= (float k) {
	_vector = _mm_mul_ps(_vector, _mm_set_ps1(k));
	return*this;
}
inline ang::maths::float4& ang::maths::float4::operator /= (float k) {
	_vector = _mm_div_ps(_vector, _mm_set_ps1(k));
	return*this;
}

template<> inline float& ang::maths::float4::get<0u>() {
	return reinterpret_cast<float*>(&_vector)[0];
}
template<> inline float& ang::maths::float4::get<1u>() {
	return reinterpret_cast<float*>(&_vector)[1];
}
template<> inline float& ang::maths::float4::get<2u>() {
	return reinterpret_cast<float*>(&_vector)[2];
}
template<> inline float& ang::maths::float4::get<3u>() {
	return reinterpret_cast<float*>(&_vector)[3];
}
template<> inline float const& ang::maths::float4::get<0u>()const {
	return reinterpret_cast<float const*>(&_vector)[0];
}
template<> inline float const& ang::maths::float4::get<1u>()const {
	return reinterpret_cast<float const*>(&_vector)[1];
}
template<> inline float const& ang::maths::float4::get<2u>()const {
	return reinterpret_cast<float const*>(&_vector)[2];
}
template<> inline float const& ang::maths::float4::get<3u>()const {
	return reinterpret_cast<float const*>(&_vector)[3];
}
template<> inline void ang::maths::float4::set<0u>(float value) {
	_vector = _mm_setr_ps(value, get<1u>(), get<2u>(), get<3u>());
}

template<> inline void ang::maths::float4::set<1u>(float value) {
	_vector = _mm_setr_ps(get<0u>(), value, get<2u>(), get<3u>());
}

template<> inline void ang::maths::float4::set<2u>(float value) {
	_vector = _mm_setr_ps(get<0u>(), get<1u>(), value, get<3u>());
}

template<> inline void ang::maths::float4::set<3u>(float value) {
	_vector = _mm_setr_ps(get<0u>(), get<1u>(), get<2u>(), value);
}

inline float ang::maths::float4::operator [](uint idx)const {
	return reinterpret_cast<float const*>(&_vector)[idx];
}

inline ang::maths::float4::operator float()const {
	auto cuad = _mm_mul_ps(_vector, _vector);
	cuad = _mm_hadd_ps(cuad, cuad);
	cuad = _mm_hadd_ps(cuad, cuad);
	return sqrt(reinterpret_cast<float*>(&cuad)[0]);
}

inline ang::maths::float4::operator ang::maths::intern_f32x4_t&() {
	return _vector;
}

inline ang::maths::float4::operator ang::maths::intern_f32x4_t const&()const {
	return _vector;
}


inline bool ang::maths::float4::operator == (const ang::maths::float4& vec)const {
	auto cuad = _mm_cmpeq_ps(_vector, vec._vector);
	return _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&cuad)) == 0xffff;
}

inline bool ang::maths::float4::operator != (const ang::maths::float4& vec)const {
	auto cuad = _mm_cmpneq_ps(_vector, vec._vector);
	return _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&cuad)) == 0xffff;
}

inline bool ang::maths::float4::operator > (const ang::maths::float4& vec)const {
	return hypot(*this) > hypot(vec);
}

inline bool ang::maths::float4::operator < (const ang::maths::float4& vec)const {
	return hypot(*this) < hypot(vec);
}


inline ang::maths::float4 ang::maths::operator + (const ang::maths::float4& vec1, const ang::maths::float4&  vec2) {
	auto cuad = _mm_add_ps(vec1._vector, vec2._vector);
	return reinterpret_cast<float4&>(cuad);
}

inline ang::maths::float4 ang::maths::operator - (const ang::maths::float4& vec1, const ang::maths::float4& vec2) {
	auto cuad = _mm_sub_ps(vec1._vector, vec2._vector);
	return reinterpret_cast<float4&>(cuad);
}

inline ang::maths::float4 ang::maths::operator - (const ang::maths::float4& vec) {
	auto cuad = _mm_sub_ps(_mm_set_ps1(0), vec._vector);
	return reinterpret_cast<float4&>(cuad);
}

inline ang::maths::float4 ang::maths::operator * (const ang::maths::float4& vec, float k) {
	auto cuad = _mm_mul_ps(vec._vector, _mm_set_ps1(k));
	return reinterpret_cast<float4&>(cuad);
}

inline ang::maths::float4 ang::maths::operator * (float k, const ang::maths::float4& vec) {
	auto cuad = _mm_mul_ps(vec._vector, _mm_set_ps1(k));
	return reinterpret_cast<float4&>(cuad);
}

inline float ang::maths::operator * (const ang::maths::float4& vec1, const ang::maths::float4& vec2) {
	return _mm_dot_ps(vec1._vector, vec2._vector);
}

inline ang::maths::float4 ang::maths::operator % (const ang::maths::float4& vec1, const ang::maths::float4& vec2) {
	auto cuad = _mm_mul_ps(vec1._vector, vec2._vector);
	return reinterpret_cast<float4&>(cuad);
}

inline ang::maths::float4 ang::maths::operator / (const ang::maths::float4& vec, float k) {
	auto cuad = _mm_div_ps(vec._vector, _mm_set_ps1(k));
	return reinterpret_cast<float4&>(cuad);
}

#endif//__ANG_VECTOR_MATHS_FLOAT4_INL__