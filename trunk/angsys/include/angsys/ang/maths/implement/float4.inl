#ifndef __ANG_VECTOR_MATHS_H__
#error ...
#elif !defined __ANG_VECTOR_MATHS_FLOAT4_HPP__
#define __ANG_VECTOR_MATHS_FLOAT4_HPP__

inline ang::maths::float4::float4() {
	_vector.x = 0;
	_vector.y = 0;
	_vector.z = 0;
	_vector.w = 1;
}

inline ang::maths::float4::float4(float val) {
	_vector.x = val;
	_vector.y = val;
	_vector.z = val;
	_vector.w = val;
}

inline ang::maths::float4::float4(ang::maths::float3 const& xyz, float w) {
	_vector.x = xyz._vector.x;
	_vector.y = xyz._vector.y;
	_vector.z = xyz._vector.z;
	_vector.w = w;
}

inline ang::maths::float4::float4(float x, float y, float z, float w) {
	_vector.x = x;
	_vector.y = y;
	_vector.z = z;
	_vector.w = w;
}

inline ang::maths::float4::float4(const ang::maths::float4& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
	_vector.z = vec._vector.z;
	_vector.w = vec._vector.w;
}

inline ang::maths::float4& ang::maths::float4::operator = (const ang::maths::float4& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
	_vector.z = vec._vector.z;
	_vector.w = vec._vector.w;
	return*this;
}

inline ang::maths::float4& ang::maths::float4::operator += (const ang::maths::float4& vec) {
	_vector.x += vec._vector.x;
	_vector.y += vec._vector.y;
	_vector.z += vec._vector.z;
	_vector.w += vec._vector.w;
	return*this;
}

inline ang::maths::float4& ang::maths::float4::operator -= (const ang::maths::float4& vec) {
	_vector.x -= vec._vector.x;
	_vector.y -= vec._vector.y;
	_vector.z -= vec._vector.z;
	_vector.w -= vec._vector.w;
	return*this;
}

inline ang::maths::float4& ang::maths::float4::operator *= (float k) {
	_vector.x *= k;
	_vector.y *= k;
	_vector.z *= k;
	_vector.w *= k;
	return*this;
}
inline ang::maths::float4& ang::maths::float4::operator /= (float k) {
	_vector.x /= k;
	_vector.y /= k;
	_vector.z /= k;
	_vector.w /= k;
	return*this;
}

template<> inline float ang::maths::float4::get<0>()const {
	return _vector.x;
}
template<> inline float ang::maths::float4::get<1>()const {
	return _vector.y;
}
template<> inline float ang::maths::float4::get<2>()const {
	return _vector.z;
}
template<> inline float ang::maths::float4::get<3>()const {
	return _vector.w;
}
template<> inline void ang::maths::float4::set<0>(float value) {
	_vector.x = value;
}

template<> inline void ang::maths::float4::set<1>(float value) {
	_vector.y = value;
}

template<> inline void ang::maths::float4::set<2>(float value) {
	_vector.z = value;
}

template<> inline void ang::maths::float4::set<3>(float value) {
	_vector.w = value;
}

inline float ang::maths::float4::operator [](uint idx)const {
	return _vector.xyzw[idx];
}

inline ang::maths::float4::operator float()const {
	return hypot(_vector.x, _vector.y, _vector.z, _vector.w);
}

inline bool ang::maths::float4::operator == (const ang::maths::float4& vec)const {

	return _vector.x == vec._vector.x
		&& _vector.y == vec._vector.y
		&& _vector.z == vec._vector.z
		&& _vector.w == vec._vector.w;
}

inline bool ang::maths::float4::operator != (const ang::maths::float4& vec)const {
	return _vector.x != vec._vector.x
		|| _vector.y != vec._vector.y
		|| _vector.z != vec._vector.z
		|| _vector.w != vec._vector.w;
}

inline bool ang::maths::float4::operator > (const ang::maths::float4& vec)const {
	return hypot(*this) > hypot(vec);
}

inline bool ang::maths::float4::operator < (const ang::maths::float4& vec)const {
	return hypot(*this) < hypot(vec);
}

inline ang::maths::float4 ang::maths::operator + (const ang::maths::float4& vec1, const ang::maths::float4&  vec2) {

	return{ vec1._vector.x + vec2._vector.x, vec1._vector.y + vec2._vector.y , vec1._vector.z + vec2._vector.z , vec1._vector.w + vec2._vector.w };
}

inline ang::maths::float4 ang::maths::operator - (const ang::maths::float4& vec1, const ang::maths::float4& vec2) {
	return{ vec1._vector.x - vec2._vector.x, vec1._vector.y - vec2._vector.y , vec1._vector.z - vec2._vector.z , vec1._vector.w - vec2._vector.w };
}

inline ang::maths::float4 ang::maths::operator - (const ang::maths::float4& vec) {
	return{ -vec._vector.x, -vec._vector.y, -vec._vector.z, -vec._vector.w };
}

inline ang::maths::float4 ang::maths::operator * (const ang::maths::float4& vec, float k) {
	return{ vec._vector.x * k, vec._vector.y * k, vec._vector.z * k, vec._vector.w * k };
}

inline ang::maths::float4 ang::maths::operator * (float k, const ang::maths::float4& vec) {
	return{ vec._vector.x * k, vec._vector.y * k, vec._vector.z * k, vec._vector.w * k };
}

inline float ang::maths::operator * (const ang::maths::float4& vec1, const ang::maths::float4& vec2) {
	return  vec1._vector.x * vec2._vector.x + vec1._vector.y * vec2._vector.y + vec1._vector.z * vec2._vector.z + vec1._vector.w * vec2._vector.w;
}

inline ang::maths::float4 ang::maths::operator % (const ang::maths::float4& vec1, const ang::maths::float4& vec2) {
	return{ vec1._vector.x * vec2._vector.x, vec1._vector.y * vec2._vector.y , vec1._vector.z * vec2._vector.z , vec1._vector.w * vec2._vector.w };
}

inline ang::maths::float4 ang::maths::operator / (const ang::maths::float4& vec, float k) {
	return{ vec._vector.x / k, vec._vector.y / k, vec._vector.z / k, vec._vector.w / k };
}

#endif//__ANG_GRAPH_MATHS_FLOAT4_HPP__