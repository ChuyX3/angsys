/*********************************************************************************************************************/
/*   File Name: ang/maths/maths_matrix.inl                                                                           */
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

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::transpose(ang::maths::float4x4 const& mat) {
	float4x4 out;
	out.set<0>({mat.get<0,0>(), mat.get<1,0>(), mat.get<2,0>(), mat.get<3,0>()});
	out.set<1>({ mat.get<0,1>(), mat.get<1,1>(), mat.get<2,1>(), mat.get<3,1>() });
	out.set<2>({ mat.get<0,2>(), mat.get<1,2>(), mat.get<2,2>(), mat.get<3,2>() });
	out.set<3>({ mat.get<0,3>(), mat.get<1,3>(), mat.get<2,3>(), mat.get<3,3>() });
	ang::move(out);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::invert(ang::maths::float4x4 const& mat) {
	float4x4 out;
	out.set<0>({ -mat.get<0,0>(), -mat.get<0,1>(), -mat.get<0,2>(), -mat.get<0,3>() });
	out.set<1>({ -mat.get<1,0>(), -mat.get<1,1>(), -mat.get<1,2>(), -mat.get<1,3>() });
	out.set<2>({ -mat.get<2,0>(), -mat.get<2,1>(), -mat.get<2,2>(), -mat.get<2,3>() });
	out.set<3>({ -mat.get<3,0>(), -mat.get<3,1>(), -mat.get<3,2>(), -mat.get<3,3>() });
	ang::move(out);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::add(ang::maths::float4x4 const& mat1, ang::maths::float4x4 const& mat2) {
	float4x4 mat;

	mat.set<0, 0>(mat1.get<0, 0>() + mat2.get<0, 0>()); mat.set<0, 1>(mat1.get<0, 1>() + mat2.get<0, 1>());	mat.set<0, 2>(mat1.get<0, 2>() + mat2.get<0, 2>()); mat.set<0, 3>(mat1.get<0, 3>() + mat2.get<0, 3>());
	mat.set<1, 0>(mat1.get<1, 0>() + mat2.get<1, 0>()); mat.set<1, 1>(mat1.get<1, 1>() + mat2.get<1, 1>());	mat.set<1, 2>(mat1.get<1, 2>() + mat2.get<1, 2>()); mat.set<1, 3>(mat1.get<1, 3>() + mat2.get<1, 3>());
	mat.set<2, 0>(mat1.get<2, 0>() + mat2.get<2, 0>()); mat.set<2, 1>(mat1.get<2, 1>() + mat2.get<2, 1>());	mat.set<2, 2>(mat1.get<2, 2>() + mat2.get<2, 2>()); mat.set<2, 3>(mat1.get<2, 3>() + mat2.get<2, 3>());
	mat.set<3, 0>(mat1.get<3, 0>() + mat2.get<3, 0>()); mat.set<3, 1>(mat1.get<3, 1>() + mat2.get<3, 1>());	mat.set<3, 2>(mat1.get<3, 2>() + mat2.get<3, 2>()); mat.set<3, 3>(mat1.get<3, 3>() + mat2.get<3, 3>());

	return ang::move(mat);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::sub(ang::maths::float4x4 const& mat1, ang::maths::float4x4 const& mat2) {
	float4x4 mat;
	mat.set<0, 0>(mat1.get<0, 0>() - mat2.get<0, 0>()); mat.set<0, 1>(mat1.get<0, 1>() - mat2.get<0, 1>());	mat.set<0, 2>(mat1.get<0, 2>() - mat2.get<0, 2>()); mat.set<0, 3>(mat1.get<0, 3>()  -mat2.get<0, 3>());
	mat.set<1, 0>(mat1.get<1, 0>() - mat2.get<1, 0>()); mat.set<1, 1>(mat1.get<1, 1>() - mat2.get<1, 1>());	mat.set<1, 2>(mat1.get<1, 2>() - mat2.get<1, 2>()); mat.set<1, 3>(mat1.get<1, 3>() - mat2.get<1, 3>());
	mat.set<2, 0>(mat1.get<2, 0>() - mat2.get<2, 0>()); mat.set<2, 1>(mat1.get<2, 1>() - mat2.get<2, 1>());	mat.set<2, 2>(mat1.get<2, 2>() - mat2.get<2, 2>()); mat.set<2, 3>(mat1.get<2, 3>()  -mat2.get<2, 3>());
	mat.set<3, 0>(mat1.get<3, 0>() - mat2.get<3, 0>()); mat.set<3, 1>(mat1.get<3, 1>() - mat2.get<3, 1>());	mat.set<3, 2>(mat1.get<3, 2>() - mat2.get<3, 2>()); mat.set<3, 3>(mat1.get<3, 3>() - mat2.get<3, 3>());

	return ang::move(mat);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::mul(ang::maths::float4x4 const& mat1, ang::maths::float4x4 const& mat2) {
	float4x4 mat;
	mat.set<0>({
		mat1.get<0,0>() * mat2.get<0,0>() + mat1.get<0,1>() * mat2.get<1,0>() + mat1.get<0,2>() * mat2.get<2,0>() + mat1.get<0,3>() * mat2.get<3,0>(),
		mat1.get<0,0>() * mat2.get<0,1>() + mat1.get<0,1>() * mat2.get<1,1>() + mat1.get<0,2>() * mat2.get<2,1>() + mat1.get<0,3>() * mat2.get<3,1>(),
		mat1.get<0,0>() * mat2.get<0,2>() + mat1.get<0,1>() * mat2.get<1,2>() + mat1.get<0,2>() * mat2.get<2,2>() + mat1.get<0,3>() * mat2.get<3,2>(),
		mat1.get<0,0>() * mat2.get<0,3>() + mat1.get<0,1>() * mat2.get<1,3>() + mat1.get<0,2>() * mat2.get<2,3>() + mat1.get<0,3>() * mat2.get<3,3>()
	});

	mat.set<1>({
		mat1.get<1,0>() * mat2.get<0,0>() + mat1.get<1,1>() * mat2.get<1,0>() + mat1.get<1,2>() * mat2.get<2,0>() + mat1.get<1,3>() * mat2.get<3,0>(),
		mat1.get<1,0>() * mat2.get<0,1>() + mat1.get<1,1>() * mat2.get<1,1>() + mat1.get<1,2>() * mat2.get<2,1>() + mat1.get<1,3>() * mat2.get<3,1>(),
		mat1.get<1,0>() * mat2.get<0,2>() + mat1.get<1,1>() * mat2.get<1,2>() + mat1.get<1,2>() * mat2.get<2,2>() + mat1.get<1,3>() * mat2.get<3,2>(),
		mat1.get<1,0>() * mat2.get<0,3>() + mat1.get<1,1>() * mat2.get<1,3>() + mat1.get<1,2>() * mat2.get<2,3>() + mat1.get<1,3>() * mat2.get<3,3>()
	});

	mat.set<2>({
		mat1.get<2,0>() * mat2.get<0,0>() + mat1.get<2,1>() * mat2.get<1,0>() + mat1.get<2,2>() * mat2.get<2,0>() + mat1.get<2,3>() * mat2.get<3,0>(),
		mat1.get<2,0>() * mat2.get<0,1>() + mat1.get<2,1>() * mat2.get<1,1>() + mat1.get<2,2>() * mat2.get<2,1>() + mat1.get<2,3>() * mat2.get<3,1>(),
		mat1.get<2,0>() * mat2.get<0,2>() + mat1.get<2,1>() * mat2.get<1,2>() + mat1.get<2,2>() * mat2.get<2,2>() + mat1.get<2,3>() * mat2.get<3,2>(),
		mat1.get<2,0>() * mat2.get<0,3>() + mat1.get<2,1>() * mat2.get<1,3>() + mat1.get<2,2>() * mat2.get<2,3>() + mat1.get<2,3>() * mat2.get<3,3>()
	});

	mat.set<3>({
		mat1.get<3,0>() * mat2.get<0,0>() + mat1.get<3,1>() * mat2.get<1,0>() + mat1.get<3,2>() * mat2.get<2,0>() + mat1.get<3,3>() * mat2.get<3,0>(),
		mat1.get<3,0>() * mat2.get<0,1>() + mat1.get<3,1>() * mat2.get<1,1>() + mat1.get<3,2>() * mat2.get<2,1>() + mat1.get<3,3>() * mat2.get<3,1>(),
		mat1.get<3,0>() * mat2.get<0,2>() + mat1.get<3,1>() * mat2.get<1,2>() + mat1.get<3,2>() * mat2.get<2,2>() + mat1.get<3,3>() * mat2.get<3,2>(),
		mat1.get<3,0>() * mat2.get<0,3>() + mat1.get<3,1>() * mat2.get<1,3>() + mat1.get<3,2>() * mat2.get<2,3>() + mat1.get<3,3>() * mat2.get<3,3>()
	});

	return ang::move(mat);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::mul(ang::maths::float4x4 const& mat1, float k) {
	float4x4 mat;
	mat.set<0>(mat1.get<0>() * k);
	mat.set<1>(mat1.get<1>() * k);
	mat.set<2>(mat1.get<2>() * k);
	mat.set<3>(mat1.get<3>() * k);
	return ang::move(mat);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::mul(float k, ang::maths::float4x4 const& mat1) {
	float4x4 mat;
	mat.set<0>(mat1.get<0>() * k);
	mat.set<1>(mat1.get<1>() * k);
	mat.set<2>(mat1.get<2>() * k);
	mat.set<3>(mat1.get<3>() * k);
	return ang::move(mat);
}

inline ang::maths::float4 ANG_VECCALL ang::maths::matrix::mul(ang::maths::float4x4 const& mat, ang::maths::float4 const& vec) {
	ang::maths::float4 res;
	res._vector.x = mat.get<0>() * vec;
	res._vector.y = mat.get<1>() * vec;
	res._vector.z = mat.get<2>() * vec;
	res._vector.w = mat.get<3>() * vec;
	return ang::move(res);
}

inline ang::maths::float4 ANG_VECCALL ang::maths::matrix::mul(ang::maths::float4 const& vec, ang::maths::float4x4 const& mat)
{
	float4 res;
	float4x4 temp = ang::move(transpose(mat));
	res._vector.x = temp.get<0>() * vec;
	res._vector.y = temp.get<1>() * vec;
	res._vector.z = temp.get<2>() * vec;
	res._vector.w = temp.get<3>() * vec;
	return ang::move(res);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::div(ang::maths::float4x4 const& mat1, float k) {
	float4x4 mat;
	mat.set<0>(mat1.get<0>() / k);
	mat.set<1>(mat1.get<1>() / k);
	mat.set<2>(mat1.get<2>() / k);
	mat.set<3>(mat1.get<3>() / k);
	return ang::move(mat);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::rotation_x(float angle) {
	float s = sin(angle);
	float c = cos(angle);
	float4x4 m;
	m.set<0>({ 1.0f, 0.0f, 0.0f, 0.0f});
	m.set<1>({ 0.0f, c, s, 0.0f });
	m.set<2>({ 0.0f, -s, c, 0.0f });
	m.set<3>({ 0.0f, 0.0f, 0.0f, 1.0f });
	return ang::move(m);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::rotation_y(float angle) {
	float s = sin(angle);
	float c = cos(angle);
	float4x4 m;
	m.set<0>({ c, 0.0f, -s, 0.0f });
	m.set<1>({ 0.0f, 1.0f, 0.0f, 0.0f });
	m.set<2>({ s, 0.0f, c, 0.0f });
	m.set<3>({ 0.0f, 0.0f, 0.0f, 1.0f });
	return ang::move(m);
}
inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::rotation_z(float angle) {
	float s = sin(angle);
	float c = cos(angle);
	float4x4 m;
	m.set<0>({ c, s, 0.0f, 0.0f });
	m.set<1>({ -s, c, 0.0f, 0.0f });
	m.set<2>({ 0.0f, 0.0f, 1.0f, 0.0f });
	m.set<3>({ 0.0f, 0.0f, 0.0f, 1.0f });
	return ang::move(m);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::rotation_axis(float angle, float x, float y, float z) {
	float mag = sum_cuad(x, y, z);

	if (mag <= 0.0f)
		return identity();
	
	float4x4 m;
	x /= mag;
	y /= mag;
	z /= mag;

	float s = sin(angle);
	float c = cos(angle);
	float _c = 1.0f - c, xs = x*s, ys = y*s, zs = z*s;
	float	xx = x*x, xy = x*y, xz = x*z, yy = y*y, yz = y*z, zz = z*z;

	m.set<0>({ (_c * xx) + c, (_c * xy) + zs, (_c * xz) - ys, 0.0f });
	m.set<1>({ (_c * xy) - zs, (_c * yy) + c, (_c * yz) + xs, 0.0f});
	m.set<2>({ (_c * xz) + ys, (_c * yz) - xs, (_c * zz) + c, 0.0f });
	m.set<3>({ 0.0f, 0.0f, 0.0f, 1.0f });
	return ang::move(m);	
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::scale(float scale) {
	float4x4 m;
	m.set<0>({ 1.0f, 0.0f, 0.0f, 0.0f });
	m.set<1>({ 0.0f, 1.0f, 0.0f, 0.0f });
	m.set<2>({ 0.0f, 0.0f, 1.0f, 0.0f });
	m.set<3>({ 0.0f, 0.0f, 0.0f, 1.0f });
	return ang::move(m);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::scale(ang::maths::float3  const& vec) {
	float4x4 m;
	m.set<0>({ 1.0f, 0.0f, 0.0f, 0.0f });
	m.set<1>({ 0.0f, 1.0f, 0.0f, 0.0f });
	m.set<2>({ 0.0f, 0.0f, 1.0f, 0.0f });
	m.set<3>({ 0.0f, 0.0f, 0.0f, 1.0f });
	return ang::move(m);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::scale(float x, float y, float z) {
	float4x4 m;
	m.set<0>({ 1.0f, 0.0f, 0.0f, 0.0f });
	m.set<1>({ 0.0f, 1.0f, 0.0f, 0.0f });
	m.set<2>({ 0.0f, 0.0f, 1.0f, 0.0f });
	m.set<3>({ 0.0f, 0.0f, 0.0f, 1.0f });
	return ang::move(m);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::translation(ang::maths::float3  const& vec) {
	float4x4 m;
	m.set<0>({ 1.0f, 0.0f, 0.0f, 0.0f });
	m.set<1>({ 0.0f, 1.0f, 0.0f, 0.0f });
	m.set<2>({ 0.0f, 0.0f, 1.0f, 0.0f });
	m.set<3>({ 0.0f, 0.0f, 0.0f, 1.0f });
	return ang::move(m);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::translation(float x, float y, float z) {
	float4x4 m;
	m.set<0>({ 1.0f, 0.0f, 0.0f, 0.0f });
	m.set<1>({ 0.0f, 1.0f, 0.0f, 0.0f });
	m.set<2>({ 0.0f, 0.0f, 1.0f, 0.0f });
	m.set<3>({ 0.0f, 0.0f, 0.0f, 1.0f });
	return ang::move(m);
}

inline ang::maths::float4x4 ANG_VECCALL ang::maths::matrix::perspective(float fovY, float aspect, float near_plane, float far_plane) {
	float4x4 m;
	m.set<0>({ 1.0f, 0.0f, 0.0f, 0.0f });
	m.set<1>({ 0.0f, 1.0f, 0.0f, 0.0f });
	m.set<2>({ 0.0f, 0.0f, 1.0f, 0.0f });
	m.set<3>({ 0.0f, 0.0f, 0.0f, 1.0f });
	return ang::move(m);
}

#endif//__ANG_MATHS_MATRIX_INL__