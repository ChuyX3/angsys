#include "pch.h"
#include "test.h"

#if defined _DEBUG
#define new new(__FILE__,__LINE__)
#endif

using namespace ang;

#define M_PI 3.141592f

camera::camera()
{
	m_movement_speed = 8.0f;
	m_rotation_speed = 1.0f;
}

camera::~camera()
{

}

bool camera::load(graphics::size<float> screen, maths::float3 const& pos, maths::float3 const& rot)
{
	m_position = pos;
	m_rotation = rot;
	projection(0.9f, screen.width / screen.height, 0.001f, 10000.0f);
	return true;
}

void camera::update(core::time::step_timer const&)
{

}


void camera::projection(float fov, float aspect, float near_plane, float far_plane)
{
	m_projection = maths::matrix::perspective(fov, aspect, near_plane, far_plane);
}

void camera::projection(maths::float4 const& proj)
{
	m_projection = maths::matrix::perspective(proj.get<0>(), proj.get<1>(), proj.get<2>(), proj.get<3>());
}

maths::float4x4 camera::view_matrix()const
{
	return maths::matrix::translation(-(maths::float3)m_position)
		* maths::matrix::rotation_y(m_rotation.get<1>())
		* maths::matrix::rotation_x(m_rotation.get<0>())
		* maths::matrix::rotation_z(m_rotation.get<2>());
}

maths::float4x4 camera::projection_matrix()const
{
	return m_projection;
}

maths::float4x4 camera::world_matrix()const
{
	return maths::matrix::rotation_y(m_rotation.get<1>())
		* maths::matrix::rotation_x(m_rotation.get<0>());
}

void camera::move_x(float delta)
{
	m_position += world_matrix() * maths::float4{ -delta * m_movement_speed, 0, 0, 1 };
}

void camera::move_y(float delta)
{
	m_position += world_matrix() * maths::float4{ 0, delta * m_movement_speed, 0, 1 };
}

void camera::move_z(float delta)
{
	m_position += world_matrix() * maths::float4{ 0, 0, -delta * m_movement_speed, 1 };
}

void camera::rotate_pitch(float delta)
{
	const float limit = float(maths::constants::pi / 2.0); //90 degrees
	float angle = m_rotation.get<0>() + delta * m_rotation_speed;

	if (angle < -limit) //rotation is limited to 90 degress
		m_rotation.set<0>(-limit);
	else if (angle > limit) //rotation is limited to 90 degress
		m_rotation.set<0>(limit);
	else
		m_rotation.set<0>(angle);
}

void camera::rotate_roll(float delta)
{
	const float limit = float(maths::constants::pi / 2.0); //90 degrees
	float angle = m_rotation.get<2>() + delta * m_rotation_speed;

	if (angle < -limit) //rotation is limited to 90 degress
		m_rotation.set<2>(-limit);
	else if (angle > limit) //rotation is limited to 90 degress
		m_rotation.set<2>(limit);
	else
		m_rotation.set<2>(angle);
}

void camera::rotate_yaw(float delta)
{
	const float turn = float(maths::constants::pi * 2); //360 degrees
	float angle = m_rotation.get<1>() + delta * m_rotation_speed;

	while (angle > maths::constants::pi)
		angle -= turn;
	while (angle < -maths::constants::pi)
		angle += turn; 
	m_rotation.set<1>(angle);
}
