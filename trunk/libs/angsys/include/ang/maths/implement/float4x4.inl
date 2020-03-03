/*********************************************************************************************************************/
/*   File Name: ang/maths/float4x4.inl                                                                               */
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
#elif !defined __ANG_VECTOR_MATHS_FLOAT4X4_INL__
#define __ANG_VECTOR_MATHS_FLOAT4X4_INL__

inline ang::maths::float4x4::float4x4() {
	memset(_mat, 0, sizeof(_mat));
}

inline ang::maths::float4x4::float4x4(float val) {
	_mat[0].xyzw[0] = val; _mat[0].xyzw[1] = val; _mat[0].xyzw[2] = val; _mat[0].xyzw[3] = val;
	_mat[1].xyzw[0] = val; _mat[1].xyzw[1] = val; _mat[1].xyzw[2] = val; _mat[1].xyzw[3] = val;
	_mat[2].xyzw[0] = val; _mat[2].xyzw[1] = val; _mat[2].xyzw[2] = val; _mat[2].xyzw[3] = val;
	_mat[3].xyzw[0] = val; _mat[3].xyzw[1] = val; _mat[3].xyzw[2] = val; _mat[3].xyzw[3] = val;
}

inline ang::maths::float4x4::float4x4(ang::maths::float4 const& v0, ang::maths::float4 const& v1, ang::maths::float4 const& v2, ang::maths::float4 const& v3) {
	_mat[0].xyzw[0] = v0.get<0>(); _mat[0].xyzw[1] = v0.get<1>(); _mat[0].xyzw[2] = v0.get<2>(); _mat[0].xyzw[3] = v0.get<3>();
	_mat[1].xyzw[0] = v1.get<0>(); _mat[1].xyzw[1] = v1.get<1>(); _mat[1].xyzw[2] = v1.get<2>(); _mat[1].xyzw[3] = v1.get<3>();
	_mat[2].xyzw[0] = v2.get<0>(); _mat[2].xyzw[1] = v2.get<1>(); _mat[2].xyzw[2] = v2.get<2>(); _mat[2].xyzw[3] = v2.get<3>();
	_mat[3].xyzw[0] = v3.get<0>(); _mat[3].xyzw[1] = v3.get<1>(); _mat[3].xyzw[2] = v3.get<2>(); _mat[3].xyzw[3] = v3.get<3>();
}

inline ang::maths::float4x4::float4x4(const ang::maths::float4x4& mat) {
	memcpy(_mat, mat._mat, sizeof(_mat));
}

inline ang::maths::float4x4::float4x4(const ang::maths::f32x4x4_t& mat) {
	memcpy(_mat, mat, sizeof(_mat));
}

inline ang::maths::float4x4& ang::maths::float4x4::operator = (const ang::maths::float4x4& mat) {
	memcpy(_mat, mat._mat, sizeof(_mat));
	return*this;
}

inline ang::maths::float4x4& ang::maths::float4x4::operator += (const ang::maths::float4x4& mat) {
	set<0, 0>(get<0, 0>() + mat.get<0, 0>()); set<0, 1>(get<0, 1>() + mat.get<0, 1>());	set<0, 2>(get<0, 2>() + mat.get<0, 2>()); set<0, 3>(get<0, 3>() + mat.get<0, 3>());
	set<1, 0>(get<1, 0>() + mat.get<1, 0>()); set<1, 1>(get<1, 1>() + mat.get<1, 1>());	set<1, 2>(get<1, 2>() + mat.get<1, 2>()); set<1, 3>(get<1, 3>() + mat.get<1, 3>());
	set<2, 0>(get<2, 0>() + mat.get<2, 0>()); set<2, 1>(get<2, 1>() + mat.get<2, 1>());	set<2, 2>(get<2, 2>() + mat.get<2, 2>()); set<2, 3>(get<2, 3>() + mat.get<2, 3>());
	set<3, 0>(get<3, 0>() + mat.get<3, 0>()); set<3, 1>(get<3, 1>() + mat.get<3, 1>());	set<3, 2>(get<3, 2>() + mat.get<3, 2>()); set<3, 3>(get<3, 3>() + mat.get<3, 3>());
	return*this;
}

inline ang::maths::float4x4& ang::maths::float4x4::operator -= (const ang::maths::float4x4& mat) {
	set<0, 0>(get<0, 0>() - mat.get<0, 0>()); set<0, 1>(get<0, 1>() - mat.get<0, 1>());	set<0, 2>(get<0, 2>() - mat.get<0, 2>()); set<0, 3>(get<0, 3>() - mat.get<0, 3>());
	set<1, 0>(get<1, 0>() - mat.get<1, 0>()); set<1, 1>(get<1, 1>() - mat.get<1, 1>());	set<1, 2>(get<1, 2>() - mat.get<1, 2>()); set<1, 3>(get<1, 3>() - mat.get<1, 3>());
	set<2, 0>(get<2, 0>() - mat.get<2, 0>()); set<2, 1>(get<2, 1>() - mat.get<2, 1>());	set<2, 2>(get<2, 2>() - mat.get<2, 2>()); set<2, 3>(get<2, 3>() - mat.get<2, 3>());
	set<3, 0>(get<3, 0>() - mat.get<3, 0>()); set<3, 1>(get<3, 1>() - mat.get<3, 1>());	set<3, 2>(get<3, 2>() - mat.get<3, 2>()); set<3, 3>(get<3, 3>() - mat.get<3, 3>());
	return*this;
}

inline ang::maths::float4x4& ang::maths::float4x4::operator *= (float k) {
	set<0, 0>(get<0, 0>() * k); set<0, 1>(get<0, 1>() * k);	set<0, 2>(get<0, 2>() * k); set<0, 3>(get<0, 3>() * k);
	set<1, 0>(get<1, 0>() * k); set<1, 1>(get<1, 1>() * k);	set<1, 2>(get<1, 2>() * k); set<1, 3>(get<1, 3>() * k);
	set<2, 0>(get<2, 0>() * k); set<2, 1>(get<2, 1>() * k);	set<2, 2>(get<2, 2>() * k); set<2, 3>(get<2, 3>() * k);
	set<3, 0>(get<3, 0>() * k); set<3, 1>(get<3, 1>() * k);	set<3, 2>(get<3, 2>() * k); set<3, 3>(get<3, 3>() * k);
	return*this;
}

inline ang::maths::float4x4& ang::maths::float4x4::operator /= (float k) {
	set<0, 0>(get<0, 0>() / k); set<0, 1>(get<0, 1>() / k);	set<0, 2>(get<0, 2>() / k); set<0, 3>(get<0, 3>() / k);
	set<1, 0>(get<1, 0>() / k); set<1, 1>(get<1, 1>() / k);	set<1, 2>(get<1, 2>() / k); set<1, 3>(get<1, 3>() / k);
	set<2, 0>(get<2, 0>() / k); set<2, 1>(get<2, 1>() / k);	set<2, 2>(get<2, 2>() / k); set<2, 3>(get<2, 3>() / k);
	set<3, 0>(get<3, 0>() / k); set<3, 1>(get<3, 1>() / k);	set<3, 2>(get<3, 2>() / k); set<3, 3>(get<3, 3>() / k);
	return*this;
}


template<> inline ang::maths::float4& ang::maths::float4x4::get<0>() { return*reinterpret_cast<ang::maths::float4*>(&_mat[0]); }
template<> inline ang::maths::float4& ang::maths::float4x4::get<1>() { return*reinterpret_cast<ang::maths::float4*>(&_mat[1]); }
template<> inline ang::maths::float4& ang::maths::float4x4::get<2>() { return*reinterpret_cast<ang::maths::float4*>(&_mat[2]); }
template<> inline ang::maths::float4& ang::maths::float4x4::get<3>() { return*reinterpret_cast<ang::maths::float4*>(&_mat[3]); }

template<> inline ang::maths::float4 const& ang::maths::float4x4::get<0>()const { return*reinterpret_cast<ang::maths::float4 const*>(&_mat[0]); }
template<> inline ang::maths::float4 const& ang::maths::float4x4::get<1>()const { return*reinterpret_cast<ang::maths::float4 const*>(&_mat[1]); }
template<> inline ang::maths::float4 const& ang::maths::float4x4::get<2>()const { return*reinterpret_cast<ang::maths::float4 const*>(&_mat[2]); }
template<> inline ang::maths::float4 const& ang::maths::float4x4::get<3>()const { return*reinterpret_cast<ang::maths::float4 const*>(&_mat[3]); }

template<> inline void ang::maths::float4x4::set<0>(ang::maths::float4 const& vec) { *reinterpret_cast<ang::maths::float4 *>(&_mat[0]) = vec; }
template<> inline void ang::maths::float4x4::set<1>(ang::maths::float4 const& vec) { *reinterpret_cast<ang::maths::float4 *>(&_mat[1]) = vec; }
template<> inline void ang::maths::float4x4::set<2>(ang::maths::float4 const& vec) { *reinterpret_cast<ang::maths::float4 *>(&_mat[2]) = vec; }
template<> inline void ang::maths::float4x4::set<3>(ang::maths::float4 const& vec) { *reinterpret_cast<ang::maths::float4 *>(&_mat[3]) = vec; }


inline ang::maths::float4& ang::maths::float4x4::operator [](uint idx) {
	return *reinterpret_cast<ang::maths::float4*>(&_mat[idx]);
}

inline ang::maths::float4 const& ang::maths::float4x4::operator [](uint idx)const {
	return *reinterpret_cast<ang::maths::float4 const*>(&_mat[idx]);
}

inline bool ang::maths::float4x4::operator == (const ang::maths::float4x4& mat)const {
	return get<0>() == mat.get<0>()
		&& get<1>() == mat.get<1>()
		&& get<2>() == mat.get<2>()
		&& get<3>() == mat.get<3>();
}

inline bool ang::maths::float4x4::operator != (const ang::maths::float4x4& mat)const {
	return get<0>() != mat.get<0>()
		|| get<1>() != mat.get<1>()
		|| get<2>() != mat.get<2>()
		|| get<3>() != mat.get<3>();
}


inline ang::maths::float4x4 ang::maths::operator - (const ang::maths::float4x4& mat) {
	return ang::move(matrix::invert(mat));
}

inline ang::maths::float4x4 ang::maths::operator ~ (const ang::maths::float4x4& mat) {
	return ang::move(matrix::transpose(mat));
}

inline ang::maths::float4x4 ang::maths::operator + (const ang::maths::float4x4& mat1, const ang::maths::float4x4& mat2) {
	return ang::move(matrix::add(mat1, mat2));
}

inline ang::maths::float4x4 ang::maths::operator - (const ang::maths::float4x4& mat1, const ang::maths::float4x4& mat2) {
	return ang::move(matrix::sub(mat1, mat2));
}

inline ang::maths::float4x4 ang::maths::operator * (ang::maths::float4x4 const& mat1, ang::maths::float4x4 const& mat2) {
	return ang::move(matrix::mul(mat1, mat2));
}

inline ang::maths::float4 ang::maths::operator * (ang::maths::float4x4 const& mat, const ang::maths::float4& vec) {
	return ang::move(matrix::mul(mat, vec));
}

inline ang::maths::float4 ang::maths::operator * (const ang::maths::float4& vec, ang::maths::float4x4 const& mat) {
	return ang::move(matrix::mul(vec, mat));
}

inline ang::maths::float4x4 ang::maths::operator * (ang::maths::float4x4 const& mat, float k) {
	return ang::move(matrix::mul(mat, k));
}

inline ang::maths::float4x4 ang::maths::operator * (float k, ang::maths::float4x4 const& mat) {
	return ang::move(matrix::mul(mat, k));
}

inline ang::maths::float4x4 ang::maths::operator / (const ang::maths::float4x4& mat, float k) {
	ang::move(matrix::div(mat, k));
}


#endif//__ANG_VECTOR_MATHS_FLOAT4X4_INL__