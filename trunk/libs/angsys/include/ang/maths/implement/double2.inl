/*********************************************************************************************************************/
/*   File Name: ang/maths/double2.inl                                                                                */
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


inline ang::maths::double2::double2() {
	_vector.x = 0;
	_vector.y = 0;
}

inline ang::maths::double2::double2(double val) {
	_vector.x = val;
	_vector.y = val;
}

inline ang::maths::double2::double2(double x, double y) {
	_vector.x = x;
	_vector.y = y;
}

inline ang::maths::double2::double2(const ang::maths::double2& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
}

inline ang::maths::double2& ang::maths::double2::operator = (const ang::maths::double2& vec) {
	_vector.x = vec._vector.x;
	_vector.y = vec._vector.y;
	return*this;
}

inline ang::maths::double2& ang::maths::double2::operator += (const ang::maths::double2& vec) {
	_vector.x += vec._vector.x;
	_vector.y += vec._vector.y;
	return*this;
}

inline ang::maths::double2& ang::maths::double2::operator -= (const ang::maths::double2& vec) {
	_vector.x -= vec._vector.x;
	_vector.y -= vec._vector.y;
	return*this;
}

inline ang::maths::double2& ang::maths::double2::operator *= (double val) {
	_vector.x *= val;
	_vector.y *= val;
	return*this;
}

inline ang::maths::double2& ang::maths::double2::operator /= (double val) {
	_vector.x /= val;
	_vector.y /= val;
	return*this;
}

template<> inline double& ang::maths::double2::get<0>() { return _vector.x; }
template<> inline double& ang::maths::double2::get<1>() { return _vector.y; }
template<> inline double const& ang::maths::double2::get<0>()const { return _vector.x; }
template<> inline double const& ang::maths::double2::get<1>()const { return _vector.y; }

template<> inline void ang::maths::double2::set<0>(double value) { _vector.x = value; }
template<> inline void ang::maths::double2::set<1>(double value) { _vector.y = value; }

inline double ang::maths::double2::operator [](uint idx)const {
	return _vector.xy[idx];
}

inline ang::maths::double2::operator double()const {
	return hypot(_vector.x, _vector.y);
}

inline bool ang::maths::double2::operator == (const double2& vec)const {
	return _vector.x == vec._vector.x
		&& _vector.y == vec._vector.y;
}

inline bool ang::maths::double2::operator != (const double2& vec)const {
	return _vector.x != vec._vector.x
		|| _vector.y != vec._vector.y;
}

inline bool ang::maths::double2::operator > (const double2& vec)const {
	return hypot(_vector.x, _vector.y) > hypot(vec._vector.x, vec._vector.y);
}

inline bool ang::maths::double2::operator < (const double2& vec)const {
	return hypot(_vector.x, _vector.y) < hypot(vec._vector.x, vec._vector.y);
}


inline ang::maths::double2 ang::maths::operator + (const ang::maths::double2& vec1, const ang::maths::double2&  vec2) {
	return double2(vec1.get<0>() + vec2.get<0>(), vec1.get<1>() + vec2.get<1>());
}

inline ang::maths::double2 ang::maths::operator - (const ang::maths::double2& vec1, const ang::maths::double2& vec2) {
	return double2(vec1.get<0>() - vec2.get<0>(), vec1.get<1>() - vec2.get<1>());
}

inline ang::maths::double2 ang::maths::operator - (const ang::maths::double2& vec) {
	return double2(-vec.get<0>(), -vec.get<1>());
}

inline ang::maths::double2 ang::maths::operator * (const ang::maths::double2& vec, double k) {
	return double2(vec.get<0>() * k, vec.get<1>() * k);
}

inline ang::maths::double2 ang::maths::operator * (double k, const ang::maths::double2& vec) {
	return double2(vec.get<0>() * k, vec.get<1>() * k);
}

inline double ang::maths::operator * (const ang::maths::double2& vec1, const ang::maths::double2& vec2) {
	return vec1.get<0>() * vec2.get<0>() + vec1.get<1>() * vec2.get<1>();
}

inline ang::maths::double2 ang::maths::operator % (const ang::maths::double2& vec1, const ang::maths::double2& vec2) {
	return double2(vec1.get<0>() * vec2.get<0>(), vec1.get<1>() * vec2.get<1>());
}

inline ang::maths::double2 ang::maths::operator / (const ang::maths::double2& vec, double k) {
	return double2(vec.get<0>() / k, vec.get<1>() / k);
}

#endif//__ANG_VECTOR_MATHS_DOUBLE2_INL__