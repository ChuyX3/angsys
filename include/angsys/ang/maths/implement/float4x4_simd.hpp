#ifndef __ANG_VECTOR_MATHS_H__
#error ...
#elif !defined __ANG_VECTOR_MATHS_FLOAT4X4_HPP__
#define __ANG_VECTOR_MATHS_FLOAT4X4_HPP__

inline ang::maths::float4x4::float4x4() {
	memset(_mat, 0, sizeof(_mat));
}

inline ang::maths::float4x4::float4x4(float val) {
	_mat[0] = _mm_set1_ps(val);
	_mat[1] = _mm_set1_ps(val);
	_mat[2] = _mm_set1_ps(val);
	_mat[3] = _mm_set1_ps(val);
}

inline ang::maths::float4x4::float4x4(ang::maths::float4 const& v0, ang::maths::float4 const& v1, ang::maths::float4 const& v2, ang::maths::float4 const& v3) {
	_mat[0] = v0._vector;
	_mat[1] = v1._vector;
	_mat[2] = v2._vector;
	_mat[3] = v3._vector;
}

inline ang::maths::float4x4::float4x4(const ang::maths::float4x4& mat) {
	_mat[0] = mat._mat[0];
	_mat[1] = mat._mat[1];
	_mat[2] = mat._mat[2];
	_mat[3] = mat._mat[3];
}

inline ang::maths::float4x4::float4x4(const ang::maths::f32x4x4_t& mat) {
	_mat[0] = _mm_load_ps(mat[0].xyzw);
	_mat[1] = _mm_load_ps(mat[1].xyzw);
	_mat[2] = _mm_load_ps(mat[2].xyzw);
	_mat[3] = _mm_load_ps(mat[3].xyzw);
}

inline ang::maths::float4x4& ang::maths::float4x4::operator = (const ang::maths::float4x4& mat) {
	_mat[0] = mat._mat[0];
	_mat[1] = mat._mat[1];
	_mat[2] = mat._mat[2];
	_mat[3] = mat._mat[3];
	return*this;
}

inline ang::maths::float4x4& ang::maths::float4x4::operator += (const ang::maths::float4x4& mat) {
	_mat[0] = _mm_add_ps(_mat[0], mat._mat[0]);
	_mat[1] = _mm_add_ps(_mat[0], mat._mat[0]);
	_mat[2] = _mm_add_ps(_mat[0], mat._mat[0]);
	_mat[3] = _mm_add_ps(_mat[0], mat._mat[0]);
	return*this;
}

inline ang::maths::float4x4& ang::maths::float4x4::operator -= (const ang::maths::float4x4& mat) {
	_mat[0] = _mm_sub_ps(_mat[0], mat._mat[0]);
	_mat[1] = _mm_sub_ps(_mat[0], mat._mat[0]);
	_mat[2] = _mm_sub_ps(_mat[0], mat._mat[0]);
	_mat[3] = _mm_sub_ps(_mat[0], mat._mat[0]);
	return*this;
}

inline ang::maths::float4x4& ang::maths::float4x4::operator *= (float k) {
	_mat[0] = _mm_mul_ps(_mat[0], _mm_set1_ps(k));
	_mat[1] = _mm_mul_ps(_mat[1], _mm_set1_ps(k));
	_mat[2] = _mm_mul_ps(_mat[2], _mm_set1_ps(k));
	_mat[3] = _mm_mul_ps(_mat[3], _mm_set1_ps(k));
	return*this;
}

inline ang::maths::float4x4& ang::maths::float4x4::operator /= (float k) {
	_mat[0] = _mm_div_ps(_mat[0], _mm_set1_ps(k));
	_mat[1] = _mm_div_ps(_mat[1], _mm_set1_ps(k));
	_mat[2] = _mm_div_ps(_mat[2], _mm_set1_ps(k));
	_mat[3] = _mm_div_ps(_mat[3], _mm_set1_ps(k));
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

template<> inline void ang::maths::float4x4::set<0>(ang::maths::float4 const& vec) { _mat[0] = vec._vector; }
template<> inline void ang::maths::float4x4::set<1>(ang::maths::float4 const& vec) { _mat[1] = vec._vector; }
template<> inline void ang::maths::float4x4::set<2>(ang::maths::float4 const& vec) { _mat[2] = vec._vector; }
template<> inline void ang::maths::float4x4::set<3>(ang::maths::float4 const& vec) { _mat[3] = vec._vector; }

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

#endif//__ANG_VECTOR_MATHS_FLOAT4X4_HPP__