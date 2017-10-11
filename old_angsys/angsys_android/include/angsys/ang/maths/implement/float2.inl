#ifndef __ANG_VECTOR_MATHS_H__
#error ...
#elif !defined __ANG_VECTOR_MATHS_FLOAT2_HPP__
#define __ANG_VECTOR_MATHS_FLOAT2_HPP__

inline ang::maths::float2::float2() {
	_vector.x = 0;
	_vector.y = 0;
}

inline ang::maths::float2::float2(float val) {
	_vector.x = val;
	_vector.y = val;
}

inline ang::maths::float2::float2(float x, float y) {
	_vector.x = x;
	_vector.y = y;
}

inline ang::maths::float2::float2(const ang::maths::float2& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
}

inline ang::maths::float2::float2(const ang::maths::f32x2_t& vec) {
	_vector.x = vec.x;
	_vector.y = vec.y;
}

inline ang::maths::float2& ang::maths::float2::operator = (const ang::maths::float2& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
	return*this;
}

inline ang::maths::float2& ang::maths::float2::operator += (const ang::maths::float2& vec) {
	_vector.x += vec._vector.x;
	_vector.y += vec._vector.y;
	return*this;
}

inline ang::maths::float2& ang::maths::float2::operator -= (const ang::maths::float2& vec) {
	_vector.x -= vec._vector.x;
	_vector.y -= vec._vector.y;
	return*this;
}

inline ang::maths::float2& ang::maths::float2::operator *= (float val) {
	_vector.x *= val;
	_vector.y *= val;
	return*this;
}

inline ang::maths::float2& ang::maths::float2::operator /= (float val) {
	_vector.x /= val;
	_vector.y /= val;
	return*this;
}

template<> inline float ang::maths::float2::get<0u>()const { return _vector.x; }
template<> inline float ang::maths::float2::get<1u>()const { return _vector.y; }

template<> inline void ang::maths::float2::set<0u>(float value) { _vector.x = value; }
template<> inline void ang::maths::float2::set<1u>(float value) { _vector.y = value; }

inline float ang::maths::float2::operator [](uint idx)const {
	return _vector.xy[idx];
}

inline ang::maths::float2::operator float()const {
	return hypot(_vector.x, _vector.y);
}

inline bool ang::maths::float2::operator == (const float2& vec)const {
	return _vector.x == vec._vector.x
		&& _vector.y == vec._vector.y;
}

inline bool ang::maths::float2::operator != (const float2& vec)const {
	return _vector.x != vec._vector.x
		|| _vector.y != vec._vector.y;
}

inline bool ang::maths::float2::operator > (const float2& vec)const {
	return hypot(_vector.x, _vector.y) > hypot(vec._vector.x, vec._vector.y);
}

inline bool ang::maths::float2::operator < (const float2& vec)const {
	return hypot(_vector.x, _vector.y) < hypot(vec._vector.x, vec._vector.y);
}


inline ang::maths::float2 ang::maths::operator + (const ang::maths::float2& vec1, const ang::maths::float2&  vec2) {
	return float2(vec1.get<0>() + vec2.get<0>(), vec1.get<1>() + vec2.get<1>());
}

inline ang::maths::float2 ang::maths::operator - (const ang::maths::float2& vec1, const ang::maths::float2& vec2) {
	return float2(vec1.get<0>() - vec2.get<0>(), vec1.get<1>() - vec2.get<1>());
}

inline ang::maths::float2 ang::maths::operator - (const ang::maths::float2& vec) {
	return float2(-vec.get<0>(), -vec.get<1>());
}

inline ang::maths::float2 ang::maths::operator * (const ang::maths::float2& vec, float k) {
	return float2(vec.get<0>() * k, vec.get<1>() * k);
}

inline ang::maths::float2 ang::maths::operator * (float k, const ang::maths::float2& vec) {
	return float2(vec.get<0>() * k, vec.get<1>() * k);
}

inline float ang::maths::operator * (const ang::maths::float2& vec1, const ang::maths::float2& vec2) {
	return vec1.get<0>() * vec2.get<0>() + vec1.get<1>() * vec2.get<1>();
}

inline ang::maths::float2 ang::maths::operator % (const ang::maths::float2& vec1, const ang::maths::float2& vec2) {
	return float2(vec1.get<0>() * vec2.get<0>(), vec1.get<1>() * vec2.get<1>());
}

inline ang::maths::float2 ang::maths::operator / (const ang::maths::float2& vec, float k) {
	return float2(vec.get<0>() / k, vec.get<1>() / k);
}

#endif//__ANG_VECTOR_MATHS_FLOAT2_HPP__