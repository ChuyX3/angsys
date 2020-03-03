/*********************************************************************************************************************/
/*   File Name: ang/maths/maths_matrix_simd.inl                                                                      */
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
#elif !defined  __ANG_MATHS_MATRIX_INL__
#define __ANG_MATHS_MATRIX_INL__

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


inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::invert(ang::maths::float4x4 const& m) {

#define _mm_permute_ps( v, c ) _mm_shuffle_ps( v, v, c )

	intern_f32x4x4_t out;
	float4x4 mt = transpose(m);
	float4 V00 = _mm_permute_ps(mt[2], _MM_SHUFFLE(1, 1, 0, 0));
	float4 V10 = _mm_permute_ps(mt[3], _MM_SHUFFLE(3, 2, 3, 2));
	float4 V01 = _mm_permute_ps(mt[0], _MM_SHUFFLE(1, 1, 0, 0));
	float4 V11 = _mm_permute_ps(mt[1], _MM_SHUFFLE(3, 2, 3, 2));
	float4 V02 = _mm_shuffle_ps(mt[2], mt[0], _MM_SHUFFLE(2, 0, 2, 0));
	float4 V12 = _mm_shuffle_ps(mt[3], mt[1], _MM_SHUFFLE(3, 1, 3, 1));

	float4 D0 = _mm_mul_ps(V00, V10);
	float4 D1 = _mm_mul_ps(V01, V11);
	float4 D2 = _mm_mul_ps(V02, V12);

	V00 = _mm_permute_ps(mt[2], _MM_SHUFFLE(3, 2, 3, 2));
	V10 = _mm_permute_ps(mt[3], _MM_SHUFFLE(1, 1, 0, 0));
	V01 = _mm_permute_ps(mt[0], _MM_SHUFFLE(3, 2, 3, 2));
	V11 = _mm_permute_ps(mt[1], _MM_SHUFFLE(1, 1, 0, 0));
	V02 = _mm_shuffle_ps(mt[2], mt[0], _MM_SHUFFLE(3, 1, 3, 1));
	V12 = _mm_shuffle_ps(mt[3], mt[1], _MM_SHUFFLE(2, 0, 2, 0));

	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	D0 = _mm_sub_ps(D0, V00);
	D1 = _mm_sub_ps(D1, V01);
	D2 = _mm_sub_ps(D2, V02);
	// V11 = D0Y,D0W,D2Y,D2Y
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
	V00 = _mm_permute_ps(mt[1], _MM_SHUFFLE(1, 0, 2, 1));
	V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
	V01 = _mm_permute_ps(mt[0], _MM_SHUFFLE(0, 1, 0, 2));
	V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
	// V13 = D1Y,D1W,D2W,D2W
	float4 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
	V02 = _mm_permute_ps(mt[3], _MM_SHUFFLE(1, 0, 2, 1));
	V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
	float4 V03 = _mm_permute_ps(mt[2], _MM_SHUFFLE(0, 1, 0, 2));
	V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

	float4 C0 = _mm_mul_ps(V00, V10);
	float4 C2 = _mm_mul_ps(V01, V11);
	float4 C4 = _mm_mul_ps(V02, V12);
	float4 C6 = _mm_mul_ps(V03, V13);

	// V11 = D0X,D0Y,D2X,D2X
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
	V00 = _mm_permute_ps(mt[1], _MM_SHUFFLE(2, 1, 3, 2));
	V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
	V01 = _mm_permute_ps(mt[0], _MM_SHUFFLE(1, 3, 2, 3));
	V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
	// V13 = D1X,D1Y,D2Z,D2Z
	V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
	V02 = _mm_permute_ps(mt[3], _MM_SHUFFLE(2, 1, 3, 2));
	V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
	V03 = _mm_permute_ps(mt[2], _MM_SHUFFLE(1, 3, 2, 3));
	V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	V03 = _mm_mul_ps(V03, V13);
	C0 = _mm_sub_ps(C0, V00);
	C2 = _mm_sub_ps(C2, V01);
	C4 = _mm_sub_ps(C4, V02);
	C6 = _mm_sub_ps(C6, V03);

	V00 = _mm_permute_ps(mt[1], _MM_SHUFFLE(0, 3, 0, 3));
	// V10 = D0Z,D0Z,D2X,D2Y
	V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
	V10 = _mm_permute_ps(V10, _MM_SHUFFLE(0, 2, 3, 0));
	V01 = _mm_permute_ps(mt[0], _MM_SHUFFLE(2, 0, 3, 1));
	// V11 = D0X,D0W,D2X,D2Y
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
	V11 = _mm_permute_ps(V11, _MM_SHUFFLE(2, 1, 0, 3));
	V02 = _mm_permute_ps(mt[3], _MM_SHUFFLE(0, 3, 0, 3));
	// V12 = D1Z,D1Z,D2Z,D2W
	V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
	V12 = _mm_permute_ps(V12, _MM_SHUFFLE(0, 2, 3, 0));
	V03 = _mm_permute_ps(mt[2], _MM_SHUFFLE(2, 0, 3, 1));
	// V13 = D1X,D1W,D2Z,D2W
	V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
	V13 = _mm_permute_ps(V13, _MM_SHUFFLE(2, 1, 0, 3));

	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	V03 = _mm_mul_ps(V03, V13);
	float4 C1 = _mm_sub_ps(C0, V00);
	C0 = _mm_add_ps(C0, V00);
	float4 C3 = _mm_add_ps(C2, V01);
	C2 = _mm_sub_ps(C2, V01);
	float4 C5 = _mm_sub_ps(C4, V02);
	C4 = _mm_add_ps(C4, V02);
	float4 C7 = _mm_add_ps(C6, V03);
	C6 = _mm_sub_ps(C6, V03);

	C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
	C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
	C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
	C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
	C0 = _mm_permute_ps(C0, _MM_SHUFFLE(3, 1, 2, 0));
	C2 = _mm_permute_ps(C2, _MM_SHUFFLE(3, 1, 2, 0));
	C4 = _mm_permute_ps(C4, _MM_SHUFFLE(3, 1, 2, 0));
	C6 = _mm_permute_ps(C6, _MM_SHUFFLE(3, 1, 2, 0));
	// Get the determinate
	float4 vTemp = float4(C0 * mt[0]);
	//if (pDeterminant != nullptr)
	//	*pDeterminant = vTemp;
	vTemp = _mm_div_ps(float4(1,1,1,1), vTemp);
	out[0] = _mm_mul_ps(C0, vTemp);
	out[1] = _mm_mul_ps(C2, vTemp);
	out[2] = _mm_mul_ps(C4, vTemp);
	out[3] = _mm_mul_ps(C6, vTemp);

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
	temp[0] = _mm_hadd_ps(temp[0], temp[1]); //[a00*v0 + a01*v1 + a02*v2 + a03*v3
											 //, a10*v0 + a11*v1 + a12*v2 + a13*v3
											 //, a20*v0 + a21*v1 + a22*v2 + a23*v3
											 //, a30*v0 + a31*v1 + a32*v2 + a33*v3]
	return reinterpret_cast<float4&>(temp[0]);
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
	temp[0] = _mm_hadd_ps(temp[0], temp[1]);	//[v0*a00 + v1*a10 + v2*a20 + v3*a30	
												//,v0*a01 + v1*a11 + v2*a21 + v3*a31
												//,v0*a02 + v1*a12 + v2*a22 + v3*a32
												//,v0*a03 + v1*a13 + v2*a23 + v3*a33]
	return  reinterpret_cast<float4&>(temp[0]);
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
	float4 xyz = _mm_setr_ps(x, y, z, 0);
	float4 xyz2 = _mm_mul_ps(xyz, xyz);
	float4 mag = _mm_hadd_ps(xyz2, xyz2);
	mag = _mm_hadd_ps(mag._vector, mag._vector);

	if (mag.get<0>() <= 0.0f) return ang::move(identity());

	mag = _mm_set1_ps(sqrt(reinterpret_cast<float*>(&mag)[0]));
	xyz = _mm_div_ps(xyz, mag);
	xyz2 = _mm_div_ps(xyz2, mag);

	float s = sin(angle);
	float c = cos(angle);
	float _c = 1 - c;
	float4 xyz2_c = _mm_mul_ps(xyz2, _mm_set1_ps(_c));
	float4 xyzs = _mm_mul_ps(xyzs, _mm_set1_ps(s));
	float4 m_xyz_c = _mm_mul_ps(xyz, _mm_shuffle_ps(xyz, xyz, 0XD9));
	m_xyz_c = _mm_mul_ps(m_xyz_c, _mm_set1_ps(_c));

	m[0] = _mm_setr_ps(xyz2_c.get<0>() + c, m_xyz_c.get<0>() + xyzs.get<2>(), m_xyz_c.get<1>() - xyzs.get<1>(), 0);
	m[1] = _mm_setr_ps(m_xyz_c.get<0>() - xyzs.get<2>(), xyz2_c.get<1>() + c, m_xyz_c.get<2>() + xyzs.get<0>(), 0);
	m[2] = _mm_setr_ps(m_xyz_c.get<1>() + xyzs.get<1>(), m_xyz_c.get<2>() - xyzs.get<0>(), xyz2_c.get<2>() + c, 0);
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

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::look_at(ang::maths::vec3 eye, ang::maths::vec3 at, ang::maths::vec3 up)
{
	vec3 zaxis = norm(eye - at);
	vec3 xaxis = norm(cross(up, zaxis));
	vec3 yaxis = cross(zaxis, xaxis);

	mat4 view;
	view.set<0>(vec4(xaxis.get<0>(), yaxis.get<0>(), zaxis.get<0>(), 0));
	view.set<1>(vec4(xaxis.get<1>(), yaxis.get<1>(), zaxis.get<1>(), 0));
	view.set<2>(vec4(xaxis.get<2>(), yaxis.get<2>(), zaxis.get<2>(), 0));
	view.set<3>(vec4(-(xaxis * eye), -(yaxis * eye), -(zaxis * eye), 1));
	return ang::move(view);
}

inline ang::maths::mat4 ANG_VECCALL ang::maths::matrix::fps(ang::maths::vec3 eye, float pitch, float yaw, float roll)
{
	return translation(-eye)
		* rotation_y(yaw)
		* rotation_x(pitch)
		* rotation_z(roll);
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

#endif//__ANG_GRAPH_MATHS_INL__
