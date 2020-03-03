/*********************************************************************************************************************/
/*   File Name: ang/maths/double4_simd.inl                                                                           */
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
#elif !defined __ANG_VECTOR_MATHS_DOUBLE4_INL__
#define __ANG_VECTOR_MATHS_DOUBLE4_INL__

inline ang::maths::double4::double4() {
	_vector.xy = _mm_set_pd(0.0f, 0.0f);
	_vector.zw = _mm_set_pd(1.0f, 0.0f);
}

inline ang::maths::double4::double4(double val) {
	_vector.xy = _mm_set1_pd(val);
	_vector.zw = _mm_set1_pd(val);
}

inline ang::maths::double4::double4(double x, double y, double z, double w) {
	_vector.xy = _mm_set_pd(y, x);
	_vector.zw = _mm_set_pd(w, z);
}

inline ang::maths::double4::double4(const ang::maths::double4& vec) {
	_vector.xy = vec._vector.xy;
	_vector.zw = vec._vector.zw;
}

inline ang::maths::double4& ang::maths::double4::operator = (const ang::maths::double4& vec) {
	_vector.xy = vec._vector.xy;
	_vector.zw = vec._vector.zw;
	return*this;
}

inline ang::maths::double4& ang::maths::double4::operator += (const ang::maths::double4& vec) {
	_vector.xy = _mm_add_pd(_vector.xy, vec._vector.xy);
	_vector.zw = _mm_add_pd(_vector.zw, vec._vector.zw);
	return*this;
}

inline ang::maths::double4& ang::maths::double4::operator -= (const ang::maths::double4& vec) {
	_vector.xy = _mm_sub_pd(_vector.xy, vec._vector.xy);
	_vector.zw = _mm_sub_pd(_vector.zw, vec._vector.zw);
	return*this;
}
inline ang::maths::double4& ang::maths::double4::operator *= (double k) {
	_vector.xy = _mm_mul_pd(_vector.xy, _mm_set1_pd(k));
	_vector.zw = _mm_mul_pd(_vector.zw, _mm_set1_pd(k));
	return*this;
}
inline ang::maths::double4& ang::maths::double4::operator /= (double k) {
	_vector.xy = _mm_div_pd(_vector.xy, _mm_set1_pd(k));
	_vector.zw = _mm_div_pd(_vector.zw, _mm_set1_pd(k));
	return*this;
}

template<> inline double& ang::maths::double4::get<0>() { return reinterpret_cast<double*>(&_vector)[0]; }
template<> inline double& ang::maths::double4::get<1>() { return  reinterpret_cast<double*>(&_vector)[1]; }
template<> inline double& ang::maths::double4::get<2>() { return  reinterpret_cast<double*>(&_vector)[2]; }
template<> inline double& ang::maths::double4::get<3>() { return  reinterpret_cast<double*>(&_vector)[3]; }
template<> inline double const& ang::maths::double4::get<0>()const { return reinterpret_cast<double const*>(&_vector)[0]; }
template<> inline double const& ang::maths::double4::get<1>()const { return reinterpret_cast<double const*>(&_vector)[1]; }
template<> inline double const& ang::maths::double4::get<2>()const { return reinterpret_cast<double const*>(&_vector)[2]; }
template<> inline double const& ang::maths::double4::get<3>()const { return reinterpret_cast<double const*>(&_vector)[3]; }
template<> inline void ang::maths::double4::set<0>(double value) {
	_vector.xy = _mm_setr_pd(value, get<1>());
}

template<> inline void ang::maths::double4::set<1>(double value) {
	_vector.xy = _mm_setr_pd(get<0>(), value);
}

template<> inline void ang::maths::double4::set<2>(double value) {
	_vector.zw = _mm_setr_pd(value, get<3>());
}

template<> inline void ang::maths::double4::set<3>(double value) {
	_vector.zw = _mm_setr_pd(get<2>(), value);
}

inline double ang::maths::double4::operator [](uint idx)const {
	return reinterpret_cast<double const*>(&_vector)[idx];
}

inline ang::maths::double4::operator double()const {
	auto cuad1 = _mm_mul_pd(_vector.xy, _vector.xy);
	auto cuad2 = _mm_mul_pd(_vector.zw, _vector.zw);
	cuad1 = _mm_hadd_pd(cuad1, cuad2);
	cuad1 = _mm_hadd_pd(cuad1, cuad1);
	return sqrt(reinterpret_cast<double*>(&cuad1)[0]);
}

inline bool ang::maths::double4::operator == (const ang::maths::double4& vec)const {
	auto caud1 = _mm_cmpeq_pd(_vector.xy, vec._vector.xy);
	auto cuad2 = _mm_cmpeq_pd(_vector.zw, vec._vector.zw);
	return _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&caud1)) == 0xffff
		&& _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&cuad2)) == 0xffff;
}

inline bool ang::maths::double4::operator != (const ang::maths::double4& vec)const {
	auto cuad1 = _mm_cmpneq_pd(_vector.xy, vec._vector.xy);
	auto cuad2 = _mm_cmpneq_pd(_vector.zw, vec._vector.zw);
	return _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&cuad1)) == 0xffff
		&& _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&cuad2)) == 0xffff;
}

inline bool ang::maths::double4::operator > (const ang::maths::double4& vec)const {
	return hypot(*this) > hypot(vec);
}

inline bool ang::maths::double4::operator < (const ang::maths::double4& vec)const {
	return hypot(*this) < hypot(vec);
}


inline ang::maths::double4 ang::maths::operator + (const ang::maths::double4& vec1, const ang::maths::double4&  vec2) {
	intern_f64x4_t cuad;
	cuad.xy = _mm_add_pd(vec1._vector.xy, vec2._vector.xy);
	cuad.zw = _mm_add_pd(vec1._vector.zw, vec2._vector.zw);
	return reinterpret_cast<double4&>(cuad);
}

inline ang::maths::double4 ang::maths::operator - (const ang::maths::double4& vec1, const ang::maths::double4& vec2) {
	intern_f64x4_t cuad;
	cuad.xy = _mm_sub_pd(vec1._vector.xy, vec2._vector.xy);
	cuad.zw = _mm_sub_pd(vec1._vector.zw, vec2._vector.zw);
	return reinterpret_cast<double4&>(cuad);
}

inline ang::maths::double4 ang::maths::operator - (const ang::maths::double4& vec) {
	intern_f64x4_t cuad;
	cuad.xy = _mm_sub_pd(_mm_set1_pd(0), vec._vector.xy);
	cuad.zw = _mm_sub_pd(_mm_set1_pd(0), vec._vector.zw);
	return reinterpret_cast<double4&>(cuad);
}

inline ang::maths::double4 ang::maths::operator * (const ang::maths::double4& vec, double k) {
	intern_f64x4_t cuad;
	cuad.xy = _mm_mul_pd(vec._vector.xy, _mm_set1_pd(k));
	cuad.zw = _mm_mul_pd(vec._vector.zw, _mm_set1_pd(k));
	return reinterpret_cast<double4&>(cuad);
}

inline ang::maths::double4 ang::maths::operator * (double k, const ang::maths::double4& vec) {
	intern_f64x4_t cuad;
	cuad.xy = _mm_mul_pd(vec._vector.xy, _mm_set1_pd(k));
	cuad.zw = _mm_mul_pd(vec._vector.zw, _mm_set1_pd(k));
	return reinterpret_cast<double4&>(cuad);
}

inline double ang::maths::operator * (const ang::maths::double4& vec1, const ang::maths::double4& vec2) {
	intern_f64x4_t cuad;
	cuad.xy = _mm_mul_pd(vec1._vector.xy, vec2._vector.xy);
	cuad.zw = _mm_mul_pd(vec1._vector.zw, vec2._vector.zw);
	cuad.xy = _mm_hadd_pd(cuad.xy, cuad.zw);
	cuad.xy = _mm_hadd_pd(cuad.xy, cuad.xy);
	return reinterpret_cast<double*>(&cuad)[0];
}

inline ang::maths::double4 ang::maths::operator % (const ang::maths::double4& vec1, const ang::maths::double4& vec2) {
	intern_f64x4_t cuad;
	cuad.xy = _mm_mul_pd(vec1._vector.xy, vec2._vector.xy);
	cuad.zw = _mm_mul_pd(vec1._vector.zw, vec2._vector.zw);
	return reinterpret_cast<double4&>(cuad);
}

inline ang::maths::double4 ang::maths::operator / (const ang::maths::double4& vec, double k) {
	intern_f64x4_t cuad;
	cuad.xy = _mm_div_pd(vec._vector.xy, _mm_set1_pd(k));
	cuad.zw = _mm_div_pd(vec._vector.zw, _mm_set1_pd(k));
	return reinterpret_cast<double4&>(cuad);
}

#endif//__ANG_VECTOR_MATHS_DOUBLE4_INL__