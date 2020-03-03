#ifndef __ANG_VECTOR_MATHS_H__
#error ...
#elif !defined __ANG_VECTOR_MATHS_DOUBLE4_HPP__
#define __ANG_VECTOR_MATHS_DOUBLE4_HPP__

inline ang::maths::double4::double4() {
	_vector.x = 0;
	_vector.y = 0;
	_vector.z = 0;
	_vector.w = 1;
}

inline ang::maths::double4::double4(double val) {
	_vector.x = val;
	_vector.y = val;
	_vector.z = val;
	_vector.w = val;
}

inline ang::maths::double4::double4(double x, double y, double z, double w) {
	_vector.x = x;
	_vector.y = y;
	_vector.z = z;
	_vector.w = w;
}

inline ang::maths::double4::double4(const ang::maths::double4& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
	_vector.z = vec._vector.z;
	_vector.w = vec._vector.w;
}

inline ang::maths::double4& ang::maths::double4::operator = (const ang::maths::double4& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
	_vector.z = vec._vector.z;
	_vector.w = vec._vector.w;
	return*this;
}

inline ang::maths::double4& ang::maths::double4::operator += (const ang::maths::double4& vec) {
	_vector.x += vec._vector.x;
	_vector.y += vec._vector.y;
	_vector.z += vec._vector.z;
	_vector.w += vec._vector.w;
	return*this;
}

inline ang::maths::double4& ang::maths::double4::operator -= (const ang::maths::double4& vec) {
	_vector.x -= vec._vector.x;
	_vector.y -= vec._vector.y;
	_vector.z -= vec._vector.z;
	_vector.w -= vec._vector.w;
	return*this;
}

inline ang::maths::double4& ang::maths::double4::operator *= (double k) {
	_vector.x *= k;
	_vector.y *= k;
	_vector.z *= k;
	_vector.w *= k;
	return*this;
}
inline ang::maths::double4& ang::maths::double4::operator /= (double k) {
	_vector.x /= k;
	_vector.y /= k;
	_vector.z /= k;
	_vector.w /= k;
	return*this;
}

template<> inline double& ang::maths::double4::get<0>() { return _vector.x; }
template<> inline double& ang::maths::double4::get<1>() { return _vector.y; }
template<> inline double& ang::maths::double4::get<2>() { return _vector.z; }
template<> inline double& ang::maths::double4::get<3>() { return _vector.w; }
template<> inline double const& ang::maths::double4::get<0>()const { return _vector.x; }
template<> inline double const& ang::maths::double4::get<1>()const { return _vector.y; }
template<> inline double const& ang::maths::double4::get<2>()const { return _vector.z; }
template<> inline double const& ang::maths::double4::get<3>()const { return _vector.w; }

template<> inline void ang::maths::double4::set<0>(double value) {
	_vector.x = value;
}

template<> inline void ang::maths::double4::set<1>(double value) {
	_vector.y = value;
}

template<> inline void ang::maths::double4::set<2>(double value) {
	_vector.z = value;
}

template<> inline void ang::maths::double4::set<3>(double value) {
	_vector.w = value;
}

inline double ang::maths::double4::operator [](uint idx)const {
	return _vector.xyzw[idx];
}

inline ang::maths::double4::operator double()const {
	return hypot(_vector.x, _vector.y, _vector.z, _vector.w);
}

inline bool ang::maths::double4::operator == (const ang::maths::double4& vec)const {

	return _vector.x == vec._vector.x
		&& _vector.y == vec._vector.y
		&& _vector.z == vec._vector.z
		&& _vector.w == vec._vector.w;
}

inline bool ang::maths::double4::operator != (const ang::maths::double4& vec)const {
	return _vector.x != vec._vector.x
		|| _vector.y != vec._vector.y
		|| _vector.z != vec._vector.z
		|| _vector.w != vec._vector.w;
}

inline bool ang::maths::double4::operator > (const ang::maths::double4& vec)const {
	return hypot(*this) > hypot(vec);
}

inline bool ang::maths::double4::operator < (const ang::maths::double4& vec)const {
	return hypot(*this) < hypot(vec);
}

inline ang::maths::double4 ang::maths::operator + (const ang::maths::double4& vec1, const ang::maths::double4&  vec2) {

	return{ vec1._vector.x + vec2._vector.x, vec1._vector.y + vec2._vector.y , vec1._vector.z + vec2._vector.z , vec1._vector.w + vec2._vector.w };
}

inline ang::maths::double4 ang::maths::operator - (const ang::maths::double4& vec1, const ang::maths::double4& vec2) {
	return{ vec1._vector.x - vec2._vector.x, vec1._vector.y - vec2._vector.y , vec1._vector.z - vec2._vector.z , vec1._vector.w - vec2._vector.w };
}

inline ang::maths::double4 ang::maths::operator - (const ang::maths::double4& vec) {
	return{ -vec._vector.x, -vec._vector.y, -vec._vector.z, -vec._vector.w };
}

inline ang::maths::double4 ang::maths::operator * (const ang::maths::double4& vec, double k) {
	return{ vec._vector.x * k, vec._vector.y * k, vec._vector.z * k, vec._vector.w * k };
}

inline ang::maths::double4 ang::maths::operator * (double k, const ang::maths::double4& vec) {
	return{ vec._vector.x * k, vec._vector.y * k, vec._vector.z * k, vec._vector.w * k };
}

inline double ang::maths::operator * (const ang::maths::double4& vec1, const ang::maths::double4& vec2) {
	return  vec1._vector.x * vec2._vector.x + vec1._vector.y * vec2._vector.y + vec1._vector.z * vec2._vector.z + vec1._vector.w * vec2._vector.w;
}

inline ang::maths::double4 ang::maths::operator % (const ang::maths::double4& vec1, const ang::maths::double4& vec2) {
	return{ vec1._vector.x * vec2._vector.x, vec1._vector.y * vec2._vector.y , vec1._vector.z * vec2._vector.z , vec1._vector.w * vec2._vector.w };
}

inline ang::maths::double4 ang::maths::operator / (const ang::maths::double4& vec, double k) {
	return{ vec._vector.x / k, vec._vector.y / k, vec._vector.z / k, vec._vector.w / k };
}

#endif//__ANG_VECTOR_MATHS_DOUBLE4_HPP__