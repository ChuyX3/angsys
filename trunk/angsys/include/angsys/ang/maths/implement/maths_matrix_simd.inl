#ifndef __ANG_VECTOR_MATHS_H__
#error ...
#elif !defined  __ANG_MATHS_MATRIX_HPP__
#define __ANG_MATHS_MATRIX_HPP__

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::zero() { return ang::move(float4x4(0)); }
inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::identity() { return ang::move(float4x4({ 1,0,0,0 }, { 0,1,0,0 }, { 0,0,1,0 }, { 0,0,0,1 })); }

inline ang::maths::double4x4 ANG_VECCALL ang::maths::matrix::zerol() { return ang::move(double4x4(0)); }
inline ang::maths::double4x4 ANG_VECCALL ang::maths::matrix::identityl() { return ang::move(double4x4({ 1,0,0,0 }, { 0,1,0,0 }, { 0,0,1,0 }, { 0,0,0,1 })); }


#if defined WINDOWS_PLATFORM && defined _ARM_

#else

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::transpose(ang::maths::float4x4 const& mat) {
	intern_f32x4x4_t out, temp;                     		
	temp[0] = _mm_shuffle_ps(mat._mat[0], mat._mat[1], 0x44); //0x44 : 0100 0100b : 0,1,0,1 => [a00, a01, a10, a11]
	temp[1] = _mm_shuffle_ps(mat._mat[2], mat._mat[3], 0x44); //0x44 : 0100 0100b : 0,1,0,1 => [a20, a21, a30, a31]
	temp[2] = _mm_shuffle_ps(mat._mat[0], mat._mat[1], 0xEE); //0xee : 1110 1110b : 2,3,2,3 => [a02, a03, a12, a13]
	temp[3] = _mm_shuffle_ps(mat._mat[2], mat._mat[3], 0xEE); //0xee : 1110 1110b : 2,3,2,3 => [a22, a23, a32, a33]	
	out[0] = _mm_shuffle_ps(temp[0], temp[1], 0x88); //0x88 : 1000 1000 : 2,0,2,0 => [a00,a10,a20,a30]
	out[1] = _mm_shuffle_ps(temp[0], temp[1], 0xDD); // 0xDD : 1101 1101 : 3,1,3,1 => [a01,a11,a21,a31]
	out[2] = _mm_shuffle_ps(temp[2], temp[3], 0x88); //0x88 : 1000 1000 : 2,0,2,0 => [a02,a12,a22,a32]
	out[3] = _mm_shuffle_ps(temp[2], temp[3], 0xDD); //0xDD : 1101 1101 : 3,1,3,1 => [a03,a13,a23,a33]
	return ang::move(*reinterpret_cast<float4x4*>(out));
}


inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::invert(ang::maths::float4x4 const& mat) {
	intern_f32x4x4_t out;
	out[0] = _mm_sub_ps(_mm_set1_ps(0), mat._mat[0]);
	out[1] = _mm_sub_ps(_mm_set1_ps(0), mat._mat[1]);
	out[2] = _mm_sub_ps(_mm_set1_ps(0), mat._mat[2]);
	out[3] = _mm_sub_ps(_mm_set1_ps(0), mat._mat[3]);
	return ang::move(*reinterpret_cast<float4x4*>(out));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::add(ang::maths::float4x4 const& mat1, ang::maths::float4x4 const& mat2) {
	intern_f32x4x4_t out;
	out[0] = _mm_add_ps(mat1._mat[0], mat2._mat[0]);
	out[1] = _mm_add_ps(mat1._mat[1], mat2._mat[1]);
	out[2] = _mm_add_ps(mat1._mat[2], mat2._mat[2]);
	out[3] = _mm_add_ps(mat1._mat[3], mat2._mat[3]);
	return ang::move(*reinterpret_cast<float4x4*>(out));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::sub(ang::maths::float4x4 const& mat1, ang::maths::float4x4 const& mat2) {
	intern_f32x4x4_t out;
	out[0] = _mm_sub_ps(mat1._mat[0], mat2._mat[0]);
	out[1] = _mm_sub_ps(mat1._mat[1], mat2._mat[1]);
	out[2] = _mm_sub_ps(mat1._mat[2], mat2._mat[2]);
	out[3] = _mm_sub_ps(mat1._mat[3], mat2._mat[3]);
	return ang::move(*reinterpret_cast<float4x4*>(out));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::mul(ang::maths::float4x4 const& mat1, ang::maths::float4x4 const& mat2) {
	intern_f32x4x4_t mat, temp, mat2_trans;
	{//transpose
		temp[0] = _mm_shuffle_ps(mat2._mat[0], mat2._mat[1], 0x44); //0x44 : 0100 0100b : 0,1,0,1 => [a00, a01, a10, a11]
		temp[1] = _mm_shuffle_ps(mat2._mat[2], mat2._mat[3], 0x44); //0x44 : 0100 0100b : 0,1,0,1 => [a20, a21, a30, a31]
		temp[2] = _mm_shuffle_ps(mat2._mat[0], mat2._mat[1], 0xEE); //0xee : 1110 1110b : 2,3,2,3 => [a02, a03, a12, a13]
		temp[3] = _mm_shuffle_ps(mat2._mat[2], mat2._mat[3], 0xEE); //0xee : 1110 1110b : 2,3,2,3 => [a22, a23, a32, a33]	
		mat2_trans[0] = _mm_shuffle_ps(temp[0], temp[1], 0x88); //[a00,a10,a20,a30]
		mat2_trans[1] = _mm_shuffle_ps(temp[0], temp[1], 0xDD); //[a01,a11,a21,a31]
		mat2_trans[2] = _mm_shuffle_ps(temp[2], temp[3], 0x88); //[a02,a12,a22,a32]
		mat2_trans[3] = _mm_shuffle_ps(temp[2], temp[3], 0xDD); //[a03,a13,a23,a33]
	}

	temp[0] = _mm_mul_ps(mat1._mat[0], mat2_trans[0]);
	temp[1] = _mm_mul_ps(mat1._mat[0], mat2_trans[1]);
	temp[2] = _mm_mul_ps(mat1._mat[0], mat2_trans[2]);
	temp[3] = _mm_mul_ps(mat1._mat[0], mat2_trans[3]);
	temp[0] = _mm_hadd_ps(temp[0], temp[1]);
	temp[1] = _mm_hadd_ps(temp[2], temp[3]);
	mat[0] = _mm_hadd_ps(temp[0], temp[1]);

	temp[0] = _mm_mul_ps(mat1._mat[1], mat2_trans[0]);
	temp[1] = _mm_mul_ps(mat1._mat[1], mat2_trans[1]);
	temp[2] = _mm_mul_ps(mat1._mat[1], mat2_trans[2]);
	temp[3] = _mm_mul_ps(mat1._mat[1], mat2_trans[3]);
	temp[0] = _mm_hadd_ps(temp[0], temp[1]);
	temp[1] = _mm_hadd_ps(temp[2], temp[3]);
	mat[1] = _mm_hadd_ps(temp[0], temp[1]);

	temp[0] = _mm_mul_ps(mat1._mat[2], mat2_trans[0]);
	temp[1] = _mm_mul_ps(mat1._mat[2], mat2_trans[1]);
	temp[2] = _mm_mul_ps(mat1._mat[2], mat2_trans[2]);
	temp[3] = _mm_mul_ps(mat1._mat[2], mat2_trans[3]);
	temp[0] = _mm_hadd_ps(temp[0], temp[1]);
	temp[1] = _mm_hadd_ps(temp[2], temp[3]);
	mat[2] = _mm_hadd_ps(temp[0], temp[1]);

	temp[0] = _mm_mul_ps(mat1._mat[3], mat2_trans[0]);
	temp[1] = _mm_mul_ps(mat1._mat[3], mat2_trans[1]);
	temp[2] = _mm_mul_ps(mat1._mat[3], mat2_trans[2]);
	temp[3] = _mm_mul_ps(mat1._mat[3], mat2_trans[3]);
	temp[0] = _mm_hadd_ps(temp[0], temp[1]);
	temp[1] = _mm_hadd_ps(temp[2], temp[3]);
	mat[3] = _mm_hadd_ps(temp[0], temp[1]);

	return ang::move(*reinterpret_cast<float4x4*>(mat));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::mul(ang::maths::float4x4 const& mat1, float k) {
	intern_f32x4x4_t mat;
	mat[0] = _mm_mul_ps(mat1._mat[0], _mm_set1_ps(k));
	mat[1] = _mm_mul_ps(mat1._mat[1], _mm_set1_ps(k));
	mat[2] = _mm_mul_ps(mat1._mat[2], _mm_set1_ps(k));
	mat[3] = _mm_mul_ps(mat1._mat[3], _mm_set1_ps(k));
	return ang::move(*reinterpret_cast<float4x4*>(mat));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::mul(float k, ang::maths::float4x4 const& mat1) {
	intern_f32x4x4_t mat;
	mat[0] = _mm_mul_ps(mat1._mat[0], _mm_set1_ps(k));
	mat[1] = _mm_mul_ps(mat1._mat[1], _mm_set1_ps(k));
	mat[2] = _mm_mul_ps(mat1._mat[2], _mm_set1_ps(k));
	mat[3] = _mm_mul_ps(mat1._mat[3], _mm_set1_ps(k));
	return ang::move(*reinterpret_cast<float4x4*>(mat));
}

inline ang::maths::float4 ANG_VECCALL ang::maths::matrix::mul(ang::maths::float4x4 const& mat, ang::maths::float4 const& vec) {
	intern_f32x4x4_t temp;
	temp[0] = _mm_mul_ps(mat._mat[0], vec._vector); //[a00*v0, a01*v1, a02*v2, a03*v3]
	temp[1] = _mm_mul_ps(mat._mat[1], vec._vector); //[a10*v0, a11*v1, a12*v2, a13*v3]
	temp[2] = _mm_mul_ps(mat._mat[2], vec._vector); //[a20*v0, a21*v1, a22*v2, a23*v3]
	temp[3] = _mm_mul_ps(mat._mat[3], vec._vector); //[a30*v0, a31*v1, a32*v2, a33*v3]
	temp[0] = _mm_hadd_ps(temp[0], temp[1]); //[a00*v0 + a01*v1, a02*v2 + a03*v3, a10*v0 + a11*v1, a12*v2 + a13*v3]
	temp[1] = _mm_hadd_ps(temp[2], temp[3]); //[a20*v0 + a21*v1, a22*v2 + a23*v3, a30*v0 + a31*v1, a32*v2 + a33*v3]
	return reinterpret_cast<float4&>(_mm_hadd_ps(temp[0], temp[1])); //[a00*v0 + a01*v1 + a02*v2 + a03*v3
																	 //, a10*v0 + a11*v1 + a12*v2 + a13*v3
																	 //, a20*v0 + a21*v1 + a22*v2 + a23*v3
																	 //, a30*v0 + a31*v1 + a32*v2 + a33*v3]
}

inline ang::maths::float4 ANG_VECCALL ang::maths::matrix::mul(ang::maths::float4 const& vec, ang::maths::float4x4 const& mat)
{
	intern_f32x4x4_t temp, mat_trans;
	{//transpose
		temp[0] = _mm_shuffle_ps(mat._mat[0], mat._mat[1], 0x44); //0x44 : 0100 0100b : 0,1,0,1 => [a00, a01, a10, a11]
		temp[1] = _mm_shuffle_ps(mat._mat[2], mat._mat[3], 0x44); //0x44 : 0100 0100b : 0,1,0,1 => [a20, a21, a30, a31]
		temp[2] = _mm_shuffle_ps(mat._mat[0], mat._mat[1], 0xEE); //0xee : 1110 1110b : 2,3,2,3 => [a02, a03, a12, a13]
		temp[3] = _mm_shuffle_ps(mat._mat[2], mat._mat[3], 0xEE); //0xee : 1110 1110b : 2,3,2,3 => [a22, a23, a32, a33]	
		mat_trans[0] = _mm_shuffle_ps(temp[0], temp[1], 0x88);
		mat_trans[1] = _mm_shuffle_ps(temp[0], temp[1], 0xDD);
		mat_trans[2] = _mm_shuffle_ps(temp[2], temp[3], 0x88);
		mat_trans[3] = _mm_shuffle_ps(temp[2], temp[3], 0xDD);
	}

	temp[0] = _mm_mul_ps(vec._vector, mat_trans[0]); //[v0*a00, v1*a10, v2*a20, v3*a30]
	temp[1] = _mm_mul_ps(vec._vector, mat_trans[1]); //[v0*a01, v1*a11, v2*a21, v3*a31]
	temp[2] = _mm_mul_ps(vec._vector, mat_trans[2]); //[v0*a02, v1*a12, v2*a22, v3*a32]
	temp[3] = _mm_mul_ps(vec._vector, mat_trans[3]); //[v0*a03, v1*a13, v2*a23, v3*a33]
	temp[0] = _mm_hadd_ps(temp[0], temp[1]);	//[v0*a00 + v1*a10, v2*a20 + v3*a30, v0*a01 + v1*a11, v2*a21 + v3*a31]
	temp[1] = _mm_hadd_ps(temp[2], temp[3]);	//[v0*a02 + v1*a12, v2*a22 + v3*a32, v0*a03 + v1*a13, v2*a23 + v3*a33]
	return  reinterpret_cast<float4&>(_mm_hadd_ps(temp[0], temp[1]));	//[v0*a00 + v1*a10 + v2*a20 + v3*a30
																		//,v0*a01 + v1*a11 + v2*a21 + v3*a31
																		//,v0*a02 + v1*a12 + v2*a22 + v3*a32
																		//,v0*a03 + v1*a13 + v2*a23 + v3*a33]
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::div(ang::maths::float4x4 const& mat1, float k) {
	intern_f32x4x4_t mat;
	mat[0] = _mm_div_ps(mat1._mat[0], _mm_set1_ps(k));
	mat[1] = _mm_div_ps(mat1._mat[1], _mm_set1_ps(k));
	mat[2] = _mm_div_ps(mat1._mat[2], _mm_set1_ps(k));
	mat[3] = _mm_div_ps(mat1._mat[3], _mm_set1_ps(k));
	return ang::move(*reinterpret_cast<float4x4*>(mat));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::rotation_x(float angle) {
	intern_f32x4x4_t m;
	float s = sin(angle);
	float c = cos(angle);
	m[0] = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
	m[1] = _mm_setr_ps(0.0f, c, s, 0.0f);
	m[2] = _mm_setr_ps(0.0f, -s, c, 0.0f);
	m[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
	return ang::move(*reinterpret_cast<float4x4*>(m));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::rotation_y(float angle) {
	intern_f32x4x4_t m;
	float s = sin(angle);
	float c = cos(angle);
	m[0] = _mm_setr_ps(c, 0.0f, -s, 0.0f);
	m[1] = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
	m[2] = _mm_setr_ps(s, 0.0f, c, 0.0f);
	m[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
	return ang::move(*reinterpret_cast<float4x4*>(m));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::rotation_z(float angle) {
	intern_f32x4x4_t m; 
	float s = sin(angle);
	float c = cos(angle);
	m[0] = _mm_setr_ps(c, s, 0.0f, 0.0f);
	m[1] = _mm_setr_ps(-s, c, 0.0f, 0.0f);
	m[2] = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
	m[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
	return ang::move(*reinterpret_cast<float4x4*>(m));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::rotation_axis(float angle, float x, float y, float z)
{
	intern_f32x4x4_t m;
	intern_f32x4_t xyz = _mm_setr_ps(x, y, z, 0);
	intern_f32x4_t xyz2 = _mm_mul_ps(xyz, xyz);
	intern_f32x4_t mag = _mm_hadd_ps(xyz2, xyz2);
	mag = _mm_hadd_ps(mag, mag);

	if (mag.m128_f32[0] <= 0.0f) return ang::move(identity());

	mag = _mm_set1_ps(sqrt(mag.m128_f32[0]));
	xyz = _mm_div_ps(xyz, mag);
	xyz2 = _mm_div_ps(xyz2, mag);

	float s = sin(angle);
	float c = cos(angle);
	float _c = 1 - c;
	intern_f32x4_t xyz2_c = _mm_mul_ps(xyz2, _mm_set1_ps(_c));
	intern_f32x4_t xyzs = _mm_mul_ps(xyzs, _mm_set1_ps(s));
	intern_f32x4_t m_xyz_c = _mm_mul_ps(xyz, _mm_shuffle_ps(xyz, xyz, 0XD9));
	m_xyz_c = _mm_mul_ps(m_xyz_c, _mm_set1_ps(_c));

	m[0] = _mm_setr_ps(xyz2_c.m128_f32[0] + c, m_xyz_c.m128_f32[0] + xyzs.m128_f32[2], m_xyz_c.m128_f32[1] - xyzs.m128_f32[1], 0);
	m[1] = _mm_setr_ps(m_xyz_c.m128_f32[0] - xyzs.m128_f32[2], xyz2_c.m128_f32[1] + c, m_xyz_c.m128_f32[2] + xyzs.m128_f32[0], 0);
	m[2] = _mm_setr_ps(m_xyz_c.m128_f32[1] + xyzs.m128_f32[1], m_xyz_c.m128_f32[2] - xyzs.m128_f32[0], xyz2_c.m128_f32[2] + c, 0);
	m[3] = _mm_set1_ps(0);
	return ang::move(*reinterpret_cast<float4x4*>(m));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::scale(float scale)
{
	ang::maths::intern_f32x4x4_t m;
	m[0] = _mm_setr_ps(scale, 0, 0, 0);
	m[1] = _mm_setr_ps(0, scale, 0, 0);
	m[2] = _mm_setr_ps(0, 0, scale, 0);
	m[3] = _mm_setr_ps(0, 0, 0, 1);
	return ang::move(*reinterpret_cast<float4x4*>(m));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::scale(ang::maths::float3 const& vec)
{
	ang::maths::intern_f32x4x4_t m;
	m[0] = _mm_setr_ps(vec.get<0>(), 0, 0, 0);
	m[1] = _mm_setr_ps(0, vec.get<1>(), 0, 0);
	m[2] = _mm_setr_ps(0, 0, vec.get<2>(), 0);
	m[3] = _mm_setr_ps(0, 0, 0, 1);
	return ang::move(*reinterpret_cast<float4x4*>(m));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::scale(float x, float y, float z)
{
	ang::maths::intern_f32x4x4_t m;
	m[0] = _mm_setr_ps(x, 0, 0, 0);
	m[1] = _mm_setr_ps(0, y, 0, 0);
	m[2] = _mm_setr_ps(0, 0, z, 0);
	m[3] = _mm_setr_ps(0, 0, 0, 1);
	return ang::move(*reinterpret_cast<float4x4*>(m));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::translation(ang::maths::float3 const& vec)
{
	ang::maths::intern_f32x4x4_t m;
	m[0] = _mm_setr_ps(1, 0, 0, 0);
	m[1] = _mm_setr_ps(0, 1, 0, 0);
	m[2] = _mm_setr_ps(0, 0, 1, 0);
	m[3] = _mm_setr_ps(vec.get<0>(), vec.get<1>(), vec.get<2>(), 1);
	return ang::move(*reinterpret_cast<float4x4*>(m));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::translation(float x, float y, float z)
{
	ang::maths::intern_f32x4x4_t m;
	m[0] = _mm_setr_ps(1, 0, 0, 0);
	m[1] = _mm_setr_ps(0, 1, 0, 0);
	m[2] = _mm_setr_ps(0, 0, 1, 0);
	m[3] = _mm_setr_ps(x, y, z, 1);
	return ang::move(*reinterpret_cast<float4x4*>(m));
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::perspective(float fovY, float aspect, float near_plane, float far_plane)
{
	ang::maths::intern_f32x4x4_t m;
	float height = 2.0f * near_plane * tan(fovY * 0.5f);
	float width = height * aspect;
	float n2 = 2.0f * near_plane;
	float rcpnmf = 1.f / (near_plane - far_plane);

	m[0] = _mm_setr_ps(n2 / width, 0, 0, 0);
	m[1] = _mm_setr_ps(0, n2 / height, 0, 0);
	m[2] = _mm_setr_ps(0, 0, (far_plane + near_plane) * rcpnmf, -1.0f); //opengl _mm_setr_ps(0, 0, (far_plane + near_plane) * rcpnmf, -1.0f);
	m[3] = _mm_setr_ps(0, 0, far_plane * rcpnmf * n2, 0);

	return ang::move(*reinterpret_cast<float4x4*>(m));
}

#endif

#endif//__ANG_GRAPH_MATHS_HPP__