#include "pch.h"
#include "scenes/scenes.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::scenes;

#define M_PI 3.141592f

camera::camera()
{
	_movement_speed = .004f;
	_rotation_speed = 0.32f;
}

camera::~camera()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::scenes::camera, scene_object);

bool camera::load(scene_t scene, xml::xml_node_t node)
{
	auto pos = node["position"];
	if (pos.is_empty())
		_position = { 0, 0, 0, 1 };
	else
		_position = { pos->xml_value().as<maths::float3>() , 1 };

	auto rot = node["rotation"];
	if (rot.is_empty())
		_rotation = { 0, 0, 0, 1 };
	else
		_rotation = { rot->xml_value().as<maths::float3>() , 1 };
	auto proj = node["projection"];

	//auto size = scene->driver()->bound();

	if (proj.is_empty())
		projection(0.8f, 800.0f / 600.0f, 0.01f, 1000.0f);
	else {
		auto att = proj->xml_attributes();
		projection(att["fvo"].as<float>(), 800.0f / 600.0f, att["near"].as<float>(), att["far"].as<float>());
	}

	return true;
}

bool camera::load(maths::float3 const& pos, maths::float3 const& rot, maths::float4 const& proj)
{
	_position = pos;
	_rotation = rot;
	projection(proj.get<0>(), proj.get<1>(), proj.get<2>(), proj.get<3>());
	return true;
}

void camera::update(float total, float delta)
{

}

void camera::draw(idriver_t, camera_t) { }

void camera::close() {}

void camera::projection(float fov, float aspect, float near_plane, float far_plane)
{
	_proj_info = {fov, aspect, near_plane , far_plane };
	_projection = maths::matrix::perspective(fov, aspect, near_plane, far_plane);
}

maths::matrix4 camera::view_matrix()const
{
	return maths::matrix::translation(-(maths::float3)_position)
		* maths::matrix::rotation_y(_rotation.get<1>())
		* maths::matrix::rotation_x(_rotation.get<0>());
}

maths::matrix4 const& camera::projection_matrix()const
{
	return _projection;
}

maths::matrix4 camera::world_matrix()const
{
	return maths::matrix::rotation_y(_rotation.get<1>())
		* maths::matrix::rotation_x(_rotation.get<0>());
}

void camera::move_up(float delta)
{
	_position += world_matrix() * maths::float4{ 0, delta * _movement_speed, 0, 1 };
}

void camera::move_down(float delta)
{
	_position += world_matrix() * maths::float4{ 0, -delta * _movement_speed, 0, 1 };
}

void camera::move_forward(float delta)
{
	_position += world_matrix() * maths::float4{ 0, 0, -delta * _movement_speed, 1 };
}

void camera::move_backward(float delta)
{
	_position += world_matrix() * maths::float4{ 0, 0, delta * _movement_speed, 1 };
}

void camera::move_left(float delta)
{
	_position += world_matrix() * maths::float4{ -delta * _movement_speed, 0, 0, 1 };
}

void camera::move_right(float delta)
{
	_position += world_matrix() * maths::float4{ delta * _movement_speed, 0, 0, 1 };
}

void camera::rotate_up(float delta)
{
	float angle = (float)(_rotation.get<0>() * 180.0f / M_PI);



	if (angle >= -90.0f) //rotation is limited to 90 degress
		_rotation.set<0>(_rotation.get<0>() + (delta * _rotation_speed * M_PI / 180.0f));
}

void camera::rotate_down(float delta)
{
	float angle = (float)(_rotation.get<0>() * 180.0f / M_PI);
	if (angle >= -90.0f) //rotation is limited to 90 degress
		_rotation.set<0>(_rotation.get<0>() + (-delta * _rotation_speed * M_PI / 180.0f));
}

void camera::rotate_left(float delta)
{
	float angle = (float)(_rotation.get<1>() * 180.0f / M_PI);
	//if (angle >= -90.0f) //rotation is limited to 90 degress
	_rotation.set<1>(_rotation.get<1>() + (delta * _rotation_speed * M_PI / 180.0f));
}

void camera::rotate_right(float delta)
{
	float angle = (float)(_rotation.get<1>() * 180.0f / M_PI);
	//if (angle >= -90.0f) //rotation is limited to 90 degress
	_rotation.set<1>(_rotation.get<1>() + (-delta * _rotation_speed * M_PI / 180.0f));
}