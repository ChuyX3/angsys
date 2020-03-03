#ifndef __ANG_VECTOR_MATHS_H__
#error ...
#elif !defined __ANG_VECTOR_MATHS_FLOAT3_HPP__
#define __ANG_VECTOR_MATHS_FLOAT3_HPP__

inline ang::maths::float3  ang::maths::cross(ang::maths::float3 const& vec1, ang::maths::float3 const& vec2)
{
	return{
		vec1.get<1>() * vec2.get<2>() - vec1.get<2>() * vec2.get<1>(),
		vec1.get<2>() * vec2.get<0>() - vec1.get<0>() * vec2.get<2>(),
		vec1.get<0>() * vec2.get<1>() - vec1.get<1>() * vec2.get<0>()
	};
}

inline ang::maths::float3::float3() {
	_vector.x = 0;
	_vector.y = 0;
	_vector.z = 0;
}

inline ang::maths::float3::float3(float val) {
	_vector.x = val;
	_vector.y = val;
	_vector.z = val;
}

inline ang::maths::float3::float3(float x, float y, float z) {
	_vector.x = x;
	_vector.y = y;
	_vector.z = z;
}

inline ang::maths::float3::float3(const ang::maths::float2& vec, float z) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
	_vector.z = z;
}


inline ang::maths::float3::float3(const ang::maths::float3& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
	_vector.z = vec._vector.z;
}

inline ang::maths::float3::float3(const ang::maths::f32x3_t& vec) {
	_vector.x = vec.x;
	_vector.y = vec.y;
	_vector.z = vec.z;
}

inline ang::maths::float3& ang::maths::float3::operator = (const ang::maths::float3& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
	_vector.z = vec._vector.z;
	return*this;
}

inline ang::maths::float3& ang::maths::float3::operator += (const ang::maths::float3& vec) {
	_vector.x += vec._vector.x;
	_vector.y += vec._vector.y;
	_vector.z += vec._vector.z;
	return*this;
}

inline ang::maths::float3& ang::maths::float3::operator -= (const ang::maths::float3& vec) {
	_vector.x -= vec._vector.x;
	_vector.y -= vec._vector.y;
	_vector.z -= vec._vector.z;
	return*this;
}

inline ang::maths::float3& ang::maths::float3::operator *= (float val) {
	_vector.x *= val;
	_vector.y *= val;
	_vector.z *= val;
	return*this;
}

inline ang::maths::float3& ang::maths::float3::operator /= (float val) {
	_vector.x /= val;
	_vector.y /= val;
	_vector.z /= val;
	return*this;
}

template<> inline float& ang::maths::float3::get<0u>() { return _vector.x; }
template<> inline float& ang::maths::float3::get<1u>() { return _vector.y; }
template<> inline float& ang::maths::float3::get<2u>() { return _vector.z; }

template<> inline float const& ang::maths::float3::get<0u>()const { return _vector.x; }
template<> inline float const& ang::maths::float3::get<1u>()const { return _vector.y; }
template<> inline float const& ang::maths::float3::get<2u>()const { return _vector.z; }

template<> inline void ang::maths::float3::set<0u>(float value) { _vector.x = value; }
template<> inline void ang::maths::float3::set<1u>(float value) { _vector.y = value; }
template<> inline void ang::maths::float3::set<2u>(float value) { _vector.z = value; }

inline float ang::maths::float3::operator [](uint idx)const {
	return _vector.xyz[idx];
}

inline ang::maths::float3::operator float()const {
	return hypot(_vector.x, _vector.y, _vector.z);
}

inline bool ang::maths::float3::operator == (const float3& vec)const {
	return _vector.x == vec._vector.x
		&& _vector.y == vec._vector.y
		&& _vector.z == vec._vector.z;
}

inline bool ang::maths::float3::operator != (const float3& vec)const {
	return _vector.x != vec._vector.x
		|| _vector.y != vec._vector.y
		|| _vector.z != vec._vector.z;
}

inline bool ang::maths::float3::operator > (const float3& vec)const {
	return hypot(_vector.x, _vector.y, _vector.z) > hypot(vec._vector.x, vec._vector.y, vec._vector.z);
}

inline bool ang::maths::float3::operator < (const float3& vec)const {
	return hypot(_vector.x, _vector.y, _vector.z) < hypot(vec._vector.x, vec._vector.y, vec._vector.z);
}


inline ang::maths::float3 ang::maths::operator + (const ang::maths::float3& vec1, const ang::maths::float3&  vec2) {
	return float3(vec1.get<0>() + vec2.get<0>(), vec1.get<1>() + vec2.get<1>(), vec1.get<2>() + vec2.get<2>());
}

inline ang::maths::float3 ang::maths::operator - (const ang::maths::float3& vec1, const ang::maths::float3& vec2) {
	return float3(vec1.get<0>() - vec2.get<0>(), vec1.get<1>() - vec2.get<1>(), vec1.get<2>() - vec2.get<2>());
}

inline ang::maths::float3 ang::maths::operator - (const ang::maths::float3& vec) {
	return float3(-vec.get<0>(), -vec.get<1>(), -vec.get<2>());
}

inline ang::maths::float3 ang::maths::operator * (const ang::maths::float3& vec, float k) {
	return float3(vec.get<0>() * k, vec.get<1>() * k, vec.get<2>() * k);
}

inline ang::maths::float3 ang::maths::operator * (float k, const ang::maths::float3& vec) {
	return float3(vec.get<0>() * k, vec.get<1>() * k, vec.get<2>() * k);
}

inline float ang::maths::operator * (const ang::maths::float3& vec1, const ang::maths::float3& vec2) {
	return vec1.get<0>() * vec2.get<0>() + vec1.get<1>() * vec2.get<1>() + vec1.get<2>() * vec2.get<2>();
}

inline ang::maths::float3 ang::maths::operator % (const ang::maths::float3& vec1, const ang::maths::float3& vec2) {
	return float3(vec1.get<0>() * vec2.get<0>(), vec1.get<1>() * vec2.get<1>(), vec1.get<2>() * vec2.get<2>());
}

inline ang::maths::float3 ang::maths::operator / (const ang::maths::float3& vec, float k) {
	return float3(vec.get<0>() / k, vec.get<1>() / k, vec.get<2>() / k);
}

#endif//__ANG_VECTOR_MATHS_FLOAT3_HPP__