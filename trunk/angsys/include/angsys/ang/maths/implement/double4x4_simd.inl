#ifndef __ANG_GRAPH_MATHS_H__
#error ...
#elif !defined __ANG_GRAPH_MATHS_FLOAT4X4_HPP__
#define __ANG_GRAPH_MATHS_FLOAT4X4_HPP__

inline ang::math::float4x4::float4x4() {
	memset(_mat, 0, sizeof(_mat));
}

inline ang::math::float4x4::float4x4(float_t val) {
	_mat[0] = _mm_set1_ps(val);
	_mat[1] = _mm_set1_ps(val);
	_mat[2] = _mm_set1_ps(val);
	_mat[3] = _mm_set1_ps(val);
}

inline ang::math::float4x4::float4x4(float4_t const& v0, float4_t const& v1, float4_t const& v2, float4_t const& v3) {
	_mat[0] = v0._vector;
	_mat[1] = v1._vector;
	_mat[2] = v2._vector;
	_mat[3] = v3._vector;
}

inline ang::math::float4x4::float4x4(const float4x4& mat) {
	_mat[0] = mat._mat[0];
	_mat[1] = mat._mat[1];
	_mat[2] = mat._mat[2];
	_mat[3] = mat._mat[3];
}

inline ang::math::float4x4_t& ang::math::float4x4::operator = (const float4x4_t& mat) {
	_mat[0] = mat._mat[0];
	_mat[1] = mat._mat[1];
	_mat[2] = mat._mat[2];
	_mat[3] = mat._mat[3];
	return*this;
}

inline ang::math::float4x4_t& ang::math::float4x4::operator += (const float4x4_t& mat) {
	_mat[0] = _mm_add_ps(_mat[0], mat._mat[0]);
	_mat[1] = _mm_add_ps(_mat[0], mat._mat[0]);
	_mat[2] = _mm_add_ps(_mat[0], mat._mat[0]);
	_mat[3] = _mm_add_ps(_mat[0], mat._mat[0]);
	return*this;
}

inline ang::math::float4x4_t& ang::math::float4x4::operator -= (const float4x4_t& mat) {
	_mat[0] = _mm_sub_ps(_mat[0], mat._mat[0]);
	_mat[1] = _mm_sub_ps(_mat[0], mat._mat[0]);
	_mat[2] = _mm_sub_ps(_mat[0], mat._mat[0]);
	_mat[3] = _mm_sub_ps(_mat[0], mat._mat[0]);
	return*this;
}

inline ang::math::float4x4_t& ang::math::float4x4::operator *= (float_t k) {
	_mat[0] = _mm_mul_ps(_mat[0], _mm_set1_ps(k));
	_mat[1] = _mm_mul_ps(_mat[1], _mm_set1_ps(k));
	_mat[2] = _mm_mul_ps(_mat[2], _mm_set1_ps(k));
	_mat[3] = _mm_mul_ps(_mat[3], _mm_set1_ps(k));
	return*this;
}

inline ang::math::float4x4_t& ang::math::float4x4::operator /= (float_t k) {
	_mat[0] = _mm_div_ps(_mat[0], _mm_set1_ps(k));
	_mat[1] = _mm_div_ps(_mat[1], _mm_set1_ps(k));
	_mat[2] = _mm_div_ps(_mat[2], _mm_set1_ps(k));
	_mat[3] = _mm_div_ps(_mat[3], _mm_set1_ps(k));
	return*this;
}


template<> inline ang::math::float4_t& ang::math::float4x4::get<0>() { return*reinterpret_cast<ang::math::float4_t*>(&_mat[0]); }
template<> inline ang::math::float4_t& ang::math::float4x4::get<1>() { return*reinterpret_cast<ang::math::float4_t*>(&_mat[1]); }
template<> inline ang::math::float4_t& ang::math::float4x4::get<2>() { return*reinterpret_cast<ang::math::float4_t*>(&_mat[2]); }
template<> inline ang::math::float4_t& ang::math::float4x4::get<3>() { return*reinterpret_cast<ang::math::float4_t*>(&_mat[3]); }

template<> inline ang::math::float4_t const& ang::math::float4x4::get<0>()const { return*reinterpret_cast<ang::math::float4_t const*>(&_mat[0]); }
template<> inline ang::math::float4_t const& ang::math::float4x4::get<1>()const { return*reinterpret_cast<ang::math::float4_t const*>(&_mat[1]); }
template<> inline ang::math::float4_t const& ang::math::float4x4::get<2>()const { return*reinterpret_cast<ang::math::float4_t const*>(&_mat[2]); }
template<> inline ang::math::float4_t const& ang::math::float4x4::get<3>()const { return*reinterpret_cast<ang::math::float4_t const*>(&_mat[3]); }


inline ang::math::float4_t& ang::math::float4x4::operator [](ang::int_t idx) {
	return *reinterpret_cast<ang::math::float4_t*>(&_mat[idx]);
}

inline ang::math::float4_t const& ang::math::float4x4::operator [](ang::int_t idx)const {
	return *reinterpret_cast<ang::math::float4_t const*>(&_mat[idx]);
}

inline ang::bool_t ang::math::float4x4::operator == (const ang::math::float4x4_t& mat)const {
	return get<0>() == mat.get<0>()
		&& get<1>() == mat.get<1>()
		&& get<2>() == mat.get<2>()
		&& get<3>() == mat.get<3>();
}

inline ang::bool_t ang::math::float4x4::operator != (const ang::math::float4x4_t& mat)const {
	return get<0>() != mat.get<0>()
		|| get<1>() != mat.get<1>()
		|| get<2>() != mat.get<2>()
		|| get<3>() != mat.get<3>();
}


#endif//__ANG_GRAPH_MATHS_FLOAT4X4_HPP__