/*********************************************************************************************************************/
/*   File Name: ang/maths/double2_simd.inl                                                                           */
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
#elif !defined __ANG_VECTOR_MATHS_DOUBLE2_INL__
#define __ANG_VECTOR_MATHS_DOUBLE2_INL__

inline ang::maths::double2::double2()
: _vector(_mm_set1_pd(0)) {

}

inline ang::maths::double2::double2(double val)
	: _vector(_mm_set1_pd(val)) {

}

inline ang::maths::double2::double2(double x, double y)
	: _vector(_mm_setr_pd(x, y)) {

}

inline ang::maths::double2::double2(const ang::maths::double2& vec)
	: _vector(vec._vector) {

}

inline ang::maths::double2& ang::maths::double2::operator = (const ang::maths::double2& vec) {
	_vector = vec._vector;
	return*this;
}

inline ang::maths::double2& ang::maths::double2::operator += (const ang::maths::double2& vec) {
	_vector = _mm_add_pd(_vector, vec._vector);
	return*this;
}

inline ang::maths::double2& ang::maths::double2::operator -= (const ang::maths::double2& vec) {
	_vector = _mm_sub_pd(_vector, vec._vector);
	return*this;
}

inline ang::maths::double2& ang::maths::double2::operator *= (double k) {
	_vector = _mm_mul_pd(_vector, _mm_setr_pd(k, k));
	return*this;
}

inline ang::maths::double2& ang::maths::double2::operator /= (double k) {
	_vector = _mm_div_pd(_vector, _mm_setr_pd(k, k));
	return*this;
}

template<> inline double& ang::maths::double2::get<0>() { return reinterpret_cast<double*>(&_vector)[0]; }
template<> inline double& ang::maths::double2::get<1>() { return reinterpret_cast<double*>(&_vector)[1]; }
template<> inline double const& ang::maths::double2::get<0>()const { return reinterpret_cast<double const*>(&_vector)[0]; }
template<> inline double const& ang::maths::double2::get<1>()const { return reinterpret_cast<double const*>(&_vector)[1]; }

template<> inline void ang::maths::double2::set<0>(double value) {
	_vector = _mm_setr_pd(value, get<1>()); 
}

template<> inline void ang::maths::double2::set<1>(double value) {
	_vector = _mm_setr_pd(get<0>(), value);
}

inline double ang::maths::double2::operator [](uint idx)const {
	return reinterpret_cast<double const*>(&_vector)[idx];
}

inline ang::maths::double2::operator double()const {
	auto cuad = _mm_mul_pd(_vector, _vector);
	cuad = _mm_hadd_pd(cuad, cuad);
	return sqrt(reinterpret_cast<double const*>(&cuad)[0]);
}

inline bool ang::maths::double2::operator == (const double2& vec)const {
	
	auto cuad = _mm_cmpeq_pd(_vector, vec._vector);
	return _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&cuad)) == 0xffff;
}

inline bool ang::maths::double2::operator != (const double2& vec)const {
	auto cuad = _mm_cmpneq_pd(_vector, vec._vector);
	return _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&cuad)) == 0xffff;
}

inline bool ang::maths::double2::operator > (const double2& vec)const {
	return hypot(*this) > hypot(vec);
}

inline bool ang::maths::double2::operator < (const double2& vec)const {
	return hypot(*this) < hypot(vec);
}


inline ang::maths::double2 ang::maths::operator + (const ang::maths::double2& vec1, const ang::maths::double2&  vec2) {
	auto cuad = _mm_add_pd(vec1._vector, vec2._vector);
	return reinterpret_cast<double2&>(cuad);
}

inline ang::maths::double2 ang::maths::operator - (const ang::maths::double2& vec1, const ang::maths::double2& vec2) {
	auto cuad = _mm_sub_pd(vec1._vector, vec2._vector);
	return reinterpret_cast<double2&>(cuad);
}

inline ang::maths::double2 ang::maths::operator - (const ang::maths::double2& vec) {
	auto cuad = _mm_sub_pd(_mm_set1_pd(0), vec._vector);
	return reinterpret_cast<double2&>(cuad);
}

inline ang::maths::double2 ang::maths::operator * (const ang::maths::double2& vec, double k) {
	auto cuad = _mm_mul_pd(vec._vector, _mm_set1_pd(k));
	return reinterpret_cast<double2&>(cuad);
}

inline ang::maths::double2 ang::maths::operator * (double k, const ang::maths::double2& vec) {
	auto cuad = _mm_mul_pd(vec._vector, _mm_set1_pd(k));
	return reinterpret_cast<double2&>(cuad);
}

inline double ang::maths::operator * (const ang::maths::double2& vec1, const ang::maths::double2& vec2) {
	auto cuad = _mm_mul_pd(vec1._vector, vec2._vector);
	cuad = _mm_hadd_pd(cuad, cuad);
	return reinterpret_cast<double2&>(cuad)[0];
}

inline ang::maths::double2 ang::maths::operator % (const ang::maths::double2& vec1, const ang::maths::double2& vec2) {
	auto cuad = _mm_mul_pd(vec1._vector, vec2._vector);
	return reinterpret_cast<double2&>(cuad);
}

inline ang::maths::double2 ang::maths::operator / (const ang::maths::double2& vec, double k) {
	auto cuad = _mm_div_pd(vec._vector, _mm_set1_pd(k));
	return reinterpret_cast<double2&>(cuad);
}

#endif//__ANG_VECTOR_MATHS_DOUBLE2_INL__