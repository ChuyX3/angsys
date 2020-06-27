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

	float a = mat.get<0, 0>(), b = mat.get<0, 1>(), c = mat.get<0, 2>(), d = mat.get<0, 3>();
	float e = mat.get<1, 0>(), f = mat.get<1, 1>(), g = mat.get<1, 2>(), h = mat.get<1, 3>();
	float i = mat.get<2, 0>(), j = mat.get<2, 1>(), k = mat.get<2, 2>(), l = mat.get<2, 3>();
	float m = mat.get<3, 0>(), n = mat.get<3, 1>(), o = mat.get<3, 2>(), p = mat.get<3, 3>();

	float kp_lo = k * p - l * o;
	float jp_ln = j * p - l * n;
	float jo_kn = j * o - k * n;
	float ip_lm = i * p - l * m;
	float io_km = i * o - k * m;
	float in_jm = i * n - j * m;

	float a11 = +(f * kp_lo - g * jp_ln + h * jo_kn);
	float a12 = -(e * kp_lo - g * ip_lm + h * io_km);
	float a13 = +(e * jp_ln - f * ip_lm + h * in_jm);
	float a14 = -(e * jo_kn - f * io_km + g * in_jm);

	float det = a * a11 + b * a12 + c * a13 + d * a14;

	// NaN safe
	if (!(fabs(det) >= FLT_EPSILON))
	{
		const float nan = std::numeric_limits<float>::quiet_NaN();

		out = float4x4(
			{ nan, nan, nan, nan },
			{ nan, nan, nan, nan },
			{ nan, nan, nan, nan },
			{ nan, nan, nan, nan });
		return false;
	}

	float invDet = 1.0f / det;

	out.set<0, 0>(a11 * invDet);
	out.set<1, 0>(a12 * invDet);
	out.set<2, 0>(a13 * invDet);
	out.set<3, 0>(a14 * invDet);

	out.set<0, 1>(-(b * kp_lo - c * jp_ln + d * jo_kn) * invDet);
	out.set<1, 1>(+(a * kp_lo - c * ip_lm + d * io_km) * invDet);
	out.set<2, 1>(-(a * jp_ln - b * ip_lm + d * in_jm) * invDet);
	out.set<3, 1>(+(a * jo_kn - b * io_km + c * in_jm) * invDet);

	float gp_ho = g * p - h * o;
	float fp_hn = f * p - h * n;
	float fo_gn = f * o - g * n;
	float ep_hm = e * p - h * m;
	float eo_gm = e * o - g * m;
	float en_fm = e * n - f * m;

	out.set<0, 2>(+(b * gp_ho - c * fp_hn + d * fo_gn) * invDet);
	out.set<1, 2>(-(a * gp_ho - c * ep_hm + d * eo_gm) * invDet);
	out.set<2, 2>(+(a * fp_hn - b * ep_hm + d * en_fm) * invDet);
	out.set<3, 2>(-(a * fo_gn - b * eo_gm + c * en_fm) * invDet);

	float gl_hk = g * l - h * k;
	float fl_hj = f * l - h * j;
	float fk_gj = f * k - g * j;
	float el_hi = e * l - h * i;
	float ek_gi = e * k - g * i;
	float ej_fi = e * j - f * i;

	out.set<0, 3>(-(b * gl_hk - c * fl_hj + d * fk_gj) * invDet);
	out.set<1, 3>(+(a * gl_hk - c * el_hi + d * ek_gi) * invDet);
	out.set<2, 3>(-(a * fl_hj - b * el_hi + d * ej_fi) * invDet);
	out.set<3, 3>(+(a * fk_gj - b * ek_gi + c * ej_fi) * invDet);

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