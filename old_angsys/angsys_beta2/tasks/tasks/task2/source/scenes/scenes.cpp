#include "pch.h"
#include "scenes/scenes.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::scenes;

pointer aligned_object::operator new(wsize size)
{
	return object::operator new(size, 16u);
}

void aligned_object::operator delete(pointer ptr)
{
	return object::operator delete(ptr, 16u);
}

#ifdef _DEBUG
pointer aligned_object::operator new(wsize size, const char*, int)
{
	return object::operator new(size, 16u);
}

void aligned_object::operator delete(pointer ptr, const char*, int)
{
	return object::operator delete(ptr, 16u);
}
#endif

#if defined _DEBUG
#define new new(__FILE__,__LINE__)
#endif

template<> inline ang::maths::float2 ang::xml::xml_text::xml_as<ang::maths::float2>()const
{
	if (this == null || cstr() == null)
		return{ 0,0 };
	streams::itext_input_stream_t stream = new streams::text_buffer_input_stream(const_cast<xml::xml_text*>(this));
	float temp[2];
	stream >> "[" >> temp[0] >> "," >> temp[1];
	return{ temp[0], temp[1] };
}

template<> inline ang::maths::float3 ang::xml::xml_text::xml_as<ang::maths::float3>()const
{
	if (this == null || cstr() == null)
		return{ 0,0,0 };
	streams::itext_input_stream_t stream = new streams::text_buffer_input_stream(const_cast<xml::xml_text*>(this));
	float temp[3];
	stream >> "[" >> temp[0] >> "," >> temp[1] >> "," >> temp[2];
	return{ temp[0], temp[1], temp[2] };
}

template<> inline ang::maths::float4 ang::xml::xml_text::xml_as<ang::maths::float4>()const
{
	if (this == null || cstr() == null)
		return{ 0,0,0,0 };
	streams::itext_input_stream_t stream = new streams::text_buffer_input_stream(const_cast<xml::xml_text*>(this));
	float temp[4];
	stream >> "[" >> temp[0] >> "," >> temp[1] >> "," >> temp[2] >> "," >> temp[2];
	return{ temp[0], temp[1], temp[2], temp[3] };
}



aligned_object::aligned_object()
{
	
}

aligned_object::~aligned_object()
{

}

bool aligned_object::auto_release()
{
	return object::auto_release(16U);
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::scenes::aligned_object, object);


ANG_IMPLEMENT_ENUM(ang::graphics::scenes, light_type, uint, light_type::directional);

/////////////////////////////////////////////////////////////////

scene_object::scene_object()
{

}

scene_object::~scene_object()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::scenes::scene_object, aligned_object);



maths::matrix4 scene_object::world_matrix()const
{
	return maths::matrix::scale((maths::float3)_scale)
		* maths::matrix::rotation_y(_rotation.get<1>())
		* maths::matrix::rotation_x(_rotation.get<0>())
		* maths::matrix::rotation_z(_rotation.get<2>())
		* maths::matrix::translation((maths::float3)_position);
}


/////////////////////////////////////////////////////////////////////

#define MY_TYPE scene
#include <ang/inline/object_wrapper_specialization.inl>
#undef MY_TYPE

scene::scene()
{
	_camera = new scenes::camera();
	_current_size = { 800,600 };
}

scene::~scene()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::scenes::scene, object);

bool scene::load(idriver_t driver, effects::ieffect_library_t fxlibrary, textures::itexture_loader_t texloader, xml::ixml_node_t scene_info)
{
	if (driver == null)
		return false;
	if (scene_info.is_empty() || !scene_info->xml_has_children())
		return false;

	_driver = driver;
	_effect_library = fxlibrary;
	_texture_loader = texloader;

	for(xml::ixml_node_t node : scene_info->xml_children())
	{
		auto name = node->xml_name()->xml_as<cwstr_t>();
		if (name.cstr() == null)
			continue;
		if (name == "sources"_s)
		{
			load_sources(node);
		}
		else if (name == "camera"_s)
		{
			_camera = new scenes::camera();
			if (!_camera->load(this, node))
				_camera = null;
		}
		else if (name == "lights"_s)
		{
			load_lights(node);
		}
		else if (name == "object"_s)
		{
			load_object(node);
		}
	}
	return true;
}

bool scene::load_sources(xml::ixml_node_t sources_info)
{
	if (sources_info.is_empty() || !sources_info->xml_has_children())
		return false;
	for(xml::ixml_node_t file : sources_info->xml_children())
	{
		auto name = file->xml_name()->xml_as<cwstr_t>();
		if (name != "file"_s)
			continue;
		main_mutex.lock();
		_source_map += {file->xml_attributes()["sid"_s]->xml_as<cwstr_t>(), file->xml_value()->xml_as<cwstr_t>()};
		main_mutex.unlock();
	}
	return true;
}

bool scene::load_lights(xml::ixml_node_t lights_info)
{
	if (lights_info.is_empty() || !lights_info->xml_has_children())
		return false;
	streams::text_buffer_input_stream_t stream_buffer = new streams::text_buffer_input_stream();
	streams::itext_input_stream_t stream = stream_buffer.get();
	for (xml::ixml_node_t node : lights_info->xml_children())
	{
		auto name = node->xml_name()->xml_as<cwstr_t>();
		if (name == "ambient"_s)
		{
			auto color = node["color"_s];
			if (!color.is_empty() && color->xml_has_value())
			{
				stream_buffer->attach(color->xml_value().get());
				stream >> _ambient_color._vector.x >> ","_s >> _ambient_color._vector.y >> ","_s >> _ambient_color._vector.z;
			}
		}
		else if (name == "light"_s)
		{
			light_info info;
			auto color = node["color"_s];
			if (!color.is_empty() && color->xml_has_value())
			{
				stream_buffer->attach(color->xml_value().get());
				stream >> info.color._vector.x >> ","_s >> info.color._vector.y >> ","_s >> info.color._vector.z;
			}
			auto position = node["position"_s];
			if (!position.is_empty() && position->xml_has_value())
			{
				info.type = light_type::spot;
				stream_buffer->attach(position->xml_value().get());
				stream >> info.pos_dir._vector.x >> ","_s >> info.pos_dir._vector.y >> ","_s >> info.pos_dir._vector.z;
			}

			auto direction = node["direction"_s];
			if (!direction.is_empty() && direction->xml_has_value())
			{
				info.type = light_type::directional;
				stream_buffer->attach(position->xml_value().get());
				stream >> info.pos_dir._vector.x >> ","_s >> info.pos_dir._vector.y >> ","_s >> info.pos_dir._vector.z;
			}
			_lights += info;
		}
	}
	return true;
}

bool scene::load_object(xml::ixml_node_t object_info)
{
	if (object_info.is_empty())
		return false;
	model_t mod = new model();
	if (!mod->load(this, object_info))
		return null;
	main_mutex.lock();
	_objects += mod.get();
	main_mutex.unlock();
	return true;
}


void scene::update(float total, float delta)
{
	//auto obj = objects[0];
	//obj->rotation({ 0, obj->rotation().get<1>() + delta / 100000.0f,0,0 });

	for(scene_object_t& node : _objects)
		node->update(total, delta);
	
}

void scene::draw(idriver_t driver, iframe_buffer_t frame)
{
	_current_size = frame->dimentions();
	maths::float4 clear_color = maths::float4(_ambient_color / 5.0f, 1);
	driver->bind_frame_buffer(frame);
	driver->clear(clear_color);
	_camera->draw(this);
	for(scene_object_t& node : _objects)
		node->draw(this);	
}

void scene::close()
{
	for(scene_object_t& node : _objects)
		node->close();
	_objects = null;
}