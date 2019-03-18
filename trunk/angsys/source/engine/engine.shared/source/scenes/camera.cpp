#include "pch.h"
#include "scenes/scenes.h"

#if defined _DEBUG
#define new new(__FILE__,__LINE__)
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::scenes;

#define MY_TYPE camera
#include <ang/inline/object_wrapper_specialization.inl>
#undef MY_TYPE

#define M_PI 3.141592f

base_camera::base_camera()
{
}

base_camera::~base_camera()
{
}

camera::camera()
	: base()
{
	m_movement_speed = .004f;
	m_rotation_speed = 0.32f;
}

camera::~camera()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::scenes::base_camera);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::scenes::base_camera, scene_object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::scenes::base_camera, scene_object);

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::scenes::camera);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::scenes::camera, base_camera);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::scenes::camera, base_camera);

bool camera::load(scene_t scene, dom::xml::xml_node_t node)
{
	auto pos = node["position"_s];
	if (pos.is_empty())
		m_position = { 0, 0, 0, 1 };
	else 
		m_position = { pos->xml_value()->xml_as<maths::float3>() , 1 };

	auto rot = node["rotation"_s];
	if (rot.is_empty())
		m_rotation = { 0, 0, 0, 1 };
	else
		m_rotation = { rot->xml_value()->xml_as<maths::float3>() , 1 };

	auto near_plane_node = node["near_plane"_s];
	auto far_plane_node = node["far_plane"_s];
	auto fvo_node = node["fvo"_s];

	float near_plane = near_plane_node.is_empty() ? 0.001f : near_plane_node->xml_value()->xml_as<float>();
	float far_plane = far_plane_node.is_empty() ? 0.001f : far_plane_node->xml_value()->xml_as<float>();
	float fvo = fvo_node.is_empty() ? 0.001f : fvo_node->xml_value()->xml_as<float>();
	auto size = scene->clip_size();
	projection(fvo, size.width / size.height , near_plane, far_plane);

	return true;
}

bool camera::load(maths::float3 const& pos, maths::float3 const& rot, maths::float4 const& proj)
{
	m_position = pos;
	m_rotation = rot;
	projection(proj.get<0>(), proj.get<1>(), proj.get<2>(), proj.get<3>());
	return true;
}

void camera::update(core::time::step_timer const&)
{

}

void camera::draw(scene_t scene)
{
	auto size = scene->clip_size();
	float aspect = size.width / size.height;
	if (m_proj_info.get<1>() != aspect)
	{
		m_proj_info.set<1>(aspect);
		m_projection = maths::matrix::perspective(m_proj_info.get<0>(), m_proj_info.get<1>(), m_proj_info.get<2>(), m_proj_info.get<3>());
	}
}

void camera::close() {}

void camera::projection(float fov, float aspect, float near_plane, float far_plane)
{
	m_proj_info = {fov, aspect, near_plane , far_plane };
	m_projection = maths::matrix::perspective(fov, aspect, near_plane, far_plane);
}

void camera::projection(maths::float4 const& proj)
{
	m_proj_info = proj;
	m_projection = maths::matrix::perspective(proj.get<0>(), proj.get<1>(), proj.get<2>(), proj.get<3>());
}

maths::float4 camera::projection()const
{
	return m_proj_info;
}


maths::matrix4 camera::view_projection_matrix()const
{
	return maths::matrix::translation(-(maths::float3)m_position)
		* maths::matrix::rotation_y(m_rotation.get<1>())
		* maths::matrix::rotation_x(m_rotation.get<0>()) * m_projection;
}

maths::matrix4 camera::world_matrix()const
{
	return maths::matrix::rotation_y(m_rotation.get<1>())
		* maths::matrix::rotation_x(m_rotation.get<0>());
}

void camera::move_up(float delta)
{
	m_position += world_matrix() * maths::float4{ 0, delta * m_movement_speed, 0, 1 };
}

void camera::move_down(float delta)
{
	m_position += world_matrix() * maths::float4{ 0, -delta * m_movement_speed, 0, 1 };
}

void camera::move_forward(float delta)
{
	m_position += world_matrix() * maths::float4{ 0, 0, -delta * m_movement_speed, 1 };
}

void camera::move_backward(float delta)
{
	m_position += world_matrix() * maths::float4{ 0, 0, delta * m_movement_speed, 1 };
}

void camera::move_left(float delta)
{
	m_position += world_matrix() * maths::float4{ -delta * m_movement_speed, 0, 0, 1 };
}

void camera::move_right(float delta)
{
	m_position += world_matrix() * maths::float4{ delta * m_movement_speed, 0, 0, 1 };
}

void camera::rotate_up(float delta)
{
	float angle = (float)(m_rotation.get<0>() * 180.0f / M_PI);



	if (angle >= -90.0f) //rotation is limited to 90 degress
		m_rotation.set<0>(m_rotation.get<0>() + (delta * m_rotation_speed * M_PI / 180.0f));
}

void camera::rotate_down(float delta)
{
	float angle = (float)(m_rotation.get<0>() * 180.0f / M_PI);
	if (angle >= -90.0f) //rotation is limited to 90 degress
		m_rotation.set<0>(m_rotation.get<0>() + (-delta * m_rotation_speed * M_PI / 180.0f));
}

void camera::rotate_left(float delta)
{
	float angle = (float)(m_rotation.get<1>() * 180.0f / M_PI);
	//if (angle >= -90.0f) //rotation is limited to 90 degress
	m_rotation.set<1>(m_rotation.get<1>() + (delta * m_rotation_speed * M_PI / 180.0f));
}

void camera::rotate_right(float delta)
{
	float angle = (float)(m_rotation.get<1>() * 180.0f / M_PI);
	//if (angle >= -90.0f) //rotation is limited to 90 degress
	m_rotation.set<1>(m_rotation.get<1>() + (-delta * m_rotation_speed * M_PI / 180.0f));
}