#ifndef __ANG_VECTOR_MATHS_H__
#error ...
#elif !defined __ANG_VECTOR_MATHS_DOUBLE3_HPP__
#define __ANG_VECTOR_MATHS_DOUBLE3_HPP__

inline ang::maths::double3 ang::maths::cross(ang::maths::double3 const& vec1, ang::maths::double3 const& vec2)
{
	return{
		vec1.get<1>() * vec2.get<2>() - vec1.get<2>() * vec2.get<1>(),
		vec1.get<2>() * vec2.get<0>() - vec1.get<0>() * vec2.get<2>(),
		vec1.get<0>() * vec2.get<1>() - vec1.get<1>() * vec2.get<0>()
	};
}

inline ang::maths::double3::double3() {
	_vector.x = 0;
	_vector.y = 0;
}

inline ang::maths::double3::double3(double val) {
	_vector.x = val;
	_vector.y = val;
}

inline ang::maths::double3::double3(double x, double y, double z) {
	_vector.x = x;
	_vector.y = y;
	_vector.z = z;
}

inline ang::maths::double3::double3(const ang::maths::double3& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
}

inline ang::maths::double3& ang::maths::double3::operator = (const ang::maths::double3& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
	_vector.z = vec._vector.z;
	return*this;
}

inline ang::maths::double3& ang::maths::double3::operator += (const ang::maths::double3& vec) {
	_vector.x += vec._vector.x;
	_vector.y += vec._vector.y;
	_vector.z += vec._vector.z;
	return*this;
}

inline ang::maths::double3& ang::maths::double3::operator -= (const ang::maths::double3& vec) {
	_vector.x -= vec._vector.x;
	_vector.y -= vec._vector.y;
	_vector.z -= vec._vector.z;
	return*this;
}

inline ang::maths::double3& ang::maths::double3::operator *= (double val) {
	_vector.x *= val;
	_vector.y *= val;
	_vector.z *= val;
	return*this;
}

inline ang::maths::double3& ang::maths::double3::operator /= (double val) {
	_vector.x /= val;
	_vector.y /= val;
	_vector.z /= val;
	return*this;
}

template<> inline double& ang::maths::double3::get<0u>() { return _vector.x; }
template<> inline double& ang::maths::double3::get<1u>() { return _vector.y; }
template<> inline double& ang::maths::double3::get<2u>() { return _vector.z; }

template<> inline double const& ang::maths::double3::get<0u>()const { return _vector.x; }
template<> inline double const& ang::maths::double3::get<1u>()const { return _vector.y; }
template<> inline double const& ang::maths::double3::get<2u>()const { return _vector.z; }

template<> inline void ang::maths::double3::set<0u>(double value) { _vector.x = value; }
template<> inline void ang::maths::double3::set<1u>(double value) { _vector.y = value; }
template<> inline void ang::maths::double3::set<3u>(double value) { _vector.z = value; }

inline double ang::maths::double3::operator [](uint idx)const {
	return _vector.xyz[idx];
}

inline ang::maths::double3::operator double()const {
	return hypot(_vector.x, _vector.y, _vector.z);
}

inline bool ang::maths::double3::operator == (const double3& vec)const {
	return _vector.x == vec._vector.x
		&& _vector.y == vec._vector.y
		&& _vector.z == vec._vector.z;
}

inline bool ang::maths::double3::operator != (const double3& vec)const {
	return _vector.x != vec._vector.x
		|| _vector.y != vec._vector.y
		|| _vector.z != vec._vector.z;
}

inline bool ang::maths::double3::operator > (const double3& vec)const {
	return hypot(_vector.x, _vector.y, _vector.z) > hypot(vec._vector.x, vec._vector.y, vec._vector.z);
}

inline bool ang::maths::double3::operator < (const double3& vec)const {
	return hypot(_vector.x, _vector.y, _vector.z) < hypot(vec._vector.x, vec._vector.y, vec._vector.z);
}


inline ang::maths::double3 ang::maths::operator + (const ang::maths::double3& vec1, const ang::maths::double3&  vec2) {
	return double3(vec1.get<0>() + vec2.get<0>(), vec1.get<1>() + vec2.get<1>(), vec1.get<2>() + vec2.get<2>());
}

inline ang::maths::double3 ang::maths::operator - (const ang::maths::double3& vec1, const ang::maths::double3& vec2) {
	return double3(vec1.get<0>() - vec2.get<0>(), vec1.get<1>() - vec2.get<1>(), vec1.get<2>() - vec2.get<2>());
}

inline ang::maths::double3 ang::maths::operator - (const ang::maths::double3& vec) {
	return double3(-vec.get<0>(), -vec.get<1>(), -vec.get<2>());
}

inline ang::maths::double3 ang::maths::operator * (const ang::maths::double3& vec, double k) {
	return double3(vec.get<0>() * k, vec.get<1>() * k, vec.get<2>() * k);
}

inline ang::maths::double3 ang::maths::operator * (double k, const ang::maths::double3& vec) {
	return double3(vec.get<0>() * k, vec.get<1>() * k, vec.get<2>() * k);
}

inline double ang::maths::operator * (const ang::maths::double3& vec1, const ang::maths::double3& vec2) {
	return vec1.get<0>() * vec2.get<0>() + vec1.get<1>() * vec2.get<1>() + vec1.get<2>() * vec2.get<2>();
}

inline ang::maths::double3 ang::maths::operator % (const ang::maths::double3& vec1, const ang::maths::double3& vec2) {
	return double3(vec1.get<0>() * vec2.get<0>(), vec1.get<1>() * vec2.get<1>(), vec1.get<2>() * vec2.get<2>());
}

inline ang::maths::double3 ang::maths::operator / (const ang::maths::double3& vec, double k) {
	return double3(vec.get<0>() / k, vec.get<1>() / k, vec.get<2>() / k);
}

#endif//__ANG_VECTOR_MATHS_DOUBLE3_HPP__