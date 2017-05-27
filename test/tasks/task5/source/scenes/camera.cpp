#include "pch.h"
#include "scenes/scenes.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::scenes;

template<> inline ang::maths::float2 ang::xml::xml_value::as<ang::maths::float2>()const
{
	if (get() == null)
		return{ 0,0 };
	streams::itext_input_stream_t stream = new streams::text_buffer_input_stream(get());
	float temp[2];
	stream >> "[" >> temp[0] >> "," >> temp[1];
	return{ temp[0], temp[1] };
}

template<> inline ang::maths::float3 ang::xml::xml_value::as<ang::maths::float3>()const
{
	if (get() == null)
		return{ 0,0,0 };
	streams::itext_input_stream_t stream = new streams::text_buffer_input_stream(get());
	float temp[3];
	stream >> "[" >> temp[0] >> "," >> temp[1] >> "," >> temp[2];
	return{ temp[0], temp[1], temp[2] };
}

template<> inline ang::maths::float4 ang::xml::xml_value::as<ang::maths::float4>()const
{
	if (get() == null)
		return{ 0,0,0,0 };
	streams::itext_input_stream_t stream = new streams::text_buffer_input_stream(get());
	float temp[4];
	stream >> "[" >> temp[0] >> "," >> temp[1] >> "," >> temp[2] >> "," >> temp[2];
	return{ temp[0], temp[1], temp[2], temp[3] };
}


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

	auto near_plane_node = node["near_plane"];
	auto far_plane_node = node["far_plane"];
	auto fvo_node = node["fvo"];

	float near_plane = near_plane_node.is_empty() ? 0.001f : near_plane_node->xml_value().as<float>();
	float far_plane = far_plane_node.is_empty() ? 0.001f : far_plane_node->xml_value().as<float>();
	float fvo = fvo_node.is_empty() ? 0.001f : fvo_node->xml_value().as<float>();
	auto size = scene->current_size();
	projection(fvo, size.width / size.height , near_plane, far_plane);

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

void camera::draw(scene_t scene)
{
	auto size = scene->current_size();
	float aspect = size.width / size.height;
	if (_proj_info.get<1>() != aspect)
	{
		_proj_info.set<1>(aspect);
		_projection = maths::matrix::perspective(_proj_info.get<0>(), _proj_info.get<1>(), _proj_info.get<2>(), _proj_info.get<3>());
	}
}

void camera::close() {}

void camera::projection(float fov, float aspect, float near_plane, float far_plane)
{
	_proj_info = {fov, aspect, near_plane , far_plane };
	_projection = maths::matrix::perspective(fov, aspect, near_plane, far_plane);
}

void camera::projection(maths::float4 const& proj)
{
	_proj_info = proj;
	_projection = maths::matrix::perspective(proj.get<0>(), proj.get<1>(), proj.get<2>(), proj.get<3>());
}

maths::float4 camera::projection()const
{
	return _proj_info;
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