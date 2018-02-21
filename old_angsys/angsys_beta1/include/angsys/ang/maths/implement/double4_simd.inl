#ifndef __ANG_VECTOR_MATHS_H__
#error ...
#elif !defined __ANG_VECTOR_MATHS_DOUBLE4_HPP__
#define __ANG_VECTOR_MATHS_DOUBLE4_HPP__

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

template<> inline double ang::maths::double4::get<0>()const {
	return _vector.xy.m128d_f64[0];
}
template<> inline double ang::maths::double4::get<1>()const {
	return _vector.xy.m128d_f64[1];
}
template<> inline double ang::maths::double4::get<2>()const {
	return _vector.zw.m128d_f64[0];
}
template<> inline double ang::maths::double4::get<3>()const {
	return _vector.zw.m128d_f64[1];
}
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
	return _vector.xy.m128d_f64[idx];
}

inline ang::maths::double4::operator double()const {
	auto cuad1 = _mm_mul_pd(_vector.xy, _vector.xy);
	auto cuad2 = _mm_mul_pd(_vector.zw, _vector.zw);
	cuad1 = _mm_hadd_pd(cuad1, cuad2);
	return sqrt(_mm_hadd_pd(cuad1, cuad1).m128d_f64[0]);
}

inline bool ang::maths::double4::operator == (const ang::maths::double4& vec)const {

	return _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&_mm_cmpeq_pd(_vector.xy, vec._vector.xy))) == 0xffff
		&& _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&_mm_cmpeq_pd(_vector.zw, vec._vector.zw))) == 0xffff;
}

inline bool ang::maths::double4::operator != (const ang::maths::double4& vec)const {
	return _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&_mm_cmpneq_pd(_vector.xy, vec._vector.xy))) == 0xffff
		&& _mm_movemask_epi8(*reinterpret_cast<__m128i*>(&_mm_cmpneq_pd(_vector.zw, vec._vector.zw))) == 0xffff;
}

inline bool ang::maths::double4::operator > (const ang::maths::double4& vec)const {
	return hypot(*this) > hypot(vec);
}

inline bool ang::maths::double4::operator < (const ang::maths::double4& vec)const {
	return hypot(*this) < hypot(vec);
}


inline ang::maths::double4 ang::maths::operator + (const ang::maths::double4& vec1, const ang::maths::double4&  vec2) {

	return reinterpret_cast<double4&>( _mm_add_pd(vec1._vector.xy, vec2._vector.xy), _mm_add_pd(vec1._vector.zw, vec2._vector.zw) );
}

inline ang::maths::double4 ang::maths::operator - (const ang::maths::double4& vec1, const ang::maths::double4& vec2) {
	return reinterpret_cast<double4&>( _mm_sub_pd(vec1._vector.xy, vec2._vector.xy), _mm_sub_pd(vec1._vector.zw, vec2._vector.zw) );
}

inline ang::maths::double4 ang::maths::operator - (const ang::maths::double4& vec) {
	return reinterpret_cast<double4&>( _mm_sub_pd(_mm_set1_pd(0), vec._vector.xy), _mm_sub_pd(_mm_set1_pd(0), vec._vector.zw) );
}

inline ang::maths::double4 ang::maths::operator * (const ang::maths::double4& vec, double k) {
	return reinterpret_cast<double4&>( _mm_mul_pd(vec._vector.xy, _mm_set1_pd(k)), _mm_mul_pd(vec._vector.zw, _mm_set1_pd(k)) );
}

inline ang::maths::double4 ang::maths::operator * (double k, const ang::maths::double4& vec) {
	return reinterpret_cast<double4&>( _mm_mul_pd(vec._vector.xy, _mm_set1_pd(k)), _mm_mul_pd(vec._vector.zw, _mm_set1_pd(k)) );
}

inline double ang::maths::operator * (const ang::maths::double4& vec1, const ang::maths::double4& vec2) {
	auto xy = _mm_mul_pd(vec1._vector.xy, vec2._vector.xy);
	auto zw = _mm_mul_pd(vec1._vector.zw, vec2._vector.zw);
	auto xyzw = _mm_hadd_pd(xy, zw);
	return _mm_hadd_pd(xyzw, xyzw).m128d_f64[0];
}

inline ang::maths::double4 ang::maths::operator % (const ang::maths::double4& vec1, const ang::maths::double4& vec2) {
	return reinterpret_cast<double4&>( _mm_mul_pd(vec1._vector.xy, vec2._vector.xy), _mm_mul_pd(vec1._vector.zw, vec2._vector.zw) );
}

inline ang::maths::double4 ang::maths::operator / (const ang::maths::double4& vec, double k) {
	return reinterpret_cast<double4&>( _mm_div_pd(vec._vector.xy, _mm_set1_pd(k)), _mm_div_pd(vec._vector.zw, _mm_set1_pd(k)) );
}

#endif//__ANG_VECTOR_MATHS_DOUBLE4_HPP__