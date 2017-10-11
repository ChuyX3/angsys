#ifndef __ANG_VECTOR_MATHS_H__
#error ...
#elif !defined __ANG_VECTOR_MATHS_DOUBLE2_HPP__
#define __ANG_VECTOR_MATHS_DOUBLE2_HPP__

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

template<> inline double ang::maths::double2::get<0>()const {
	//double_t res[2];
	//_mm_store_pd(res, _vector);
	return _vector.m128d_f64[0];
}
template<> inline double ang::maths::double2::get<1>()const {
//	double_t res[2];
//	_mm_store_pd(res, _vector);
	return _vector.m128d_f64[1];
}

template<> inline void ang::maths::double2::set<0>(double value) {
	_vector = _mm_setr_pd(value, get<1>()); 
}

template<> inline void ang::maths::double2::set<1>(double value) {
	_vector = _mm_setr_pd(get<0>(), value);
}

inline double ang::maths::double2::operator [](uint idx)const {
	return _vector.m128d_f64[idx];
}

inline ang::maths::double2::operator double()const {
	auto cuad = _mm_mul_pd(_vector, _vector);
	return sqrt(_mm_hadd_pd(cuad, cuad).m128d_f64[0]);
}

inline ang::bool_t ang::maths::double2::operator == (const double2& vec)const {
	
	return _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&_mm_cmpeq_pd(_vector, vec._vector))) == 0xffff;
}

inline ang::bool_t ang::maths::double2::operator != (const double2& vec)const {
	return _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&_mm_cmpneq_pd(_vector, vec._vector))) == 0xffff;
}

inline ang::bool_t ang::maths::double2::operator > (const double2& vec)const {
	return hypot(*this) > hypot(vec);
}

inline ang::bool_t ang::maths::double2::operator < (const double2& vec)const {
	return hypot(*this) < hypot(vec);
}


inline ang::maths::double2 ang::maths::operator + (const ang::maths::double2& vec1, const ang::maths::double2&  vec2) {
	
	return reinterpret_cast<double2&>(_mm_add_pd(vec1._vector, vec2._vector));
}

inline ang::maths::double2 ang::maths::operator - (const ang::maths::double2& vec1, const ang::maths::double2& vec2) {
	return reinterpret_cast<double2&>(_mm_sub_pd(vec1._vector, vec2._vector));
}

inline ang::maths::double2 ang::maths::operator - (const ang::maths::double2& vec) {
	return reinterpret_cast<double2&>(_mm_sub_pd(_mm_set1_pd(0), vec._vector));
}

inline ang::maths::double2 ang::maths::operator * (const ang::maths::double2& vec, double k) {
	return reinterpret_cast<double2&>(_mm_mul_pd(vec._vector, _mm_set1_pd(k)));
}

inline ang::maths::double2 ang::maths::operator * (double k, const ang::maths::double2& vec) {
	return reinterpret_cast<double2&>(_mm_mul_pd(vec._vector, _mm_set1_pd(k)));
}

inline double_t ang::maths::operator * (const ang::maths::double2& vec1, const ang::maths::double2& vec2) {
	auto vec = _mm_mul_pd(vec1._vector, vec2._vector);
	return _mm_hadd_pd(vec, vec).m128d_f64[0];
}

inline ang::maths::double2 ang::maths::operator % (const ang::maths::double2& vec1, const ang::maths::double2& vec2) {
	return reinterpret_cast<double2&>(_mm_mul_pd(vec1._vector, vec2._vector));
}

inline ang::maths::double2 ang::maths::operator / (const ang::maths::double2& vec, double k) {
	return reinterpret_cast<double2&>(_mm_div_pd(vec._vector, _mm_set1_pd(k)));
}

#endif//__ANG_VECTOR_MATHS_DOUBLE2_HPP__