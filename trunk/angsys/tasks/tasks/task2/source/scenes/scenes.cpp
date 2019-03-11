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

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::scenes::aligned_object);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::scenes::aligned_object, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::scenes::aligned_object, object);

safe_enum_rrti2(ang::graphics::scenes, light_type);

/////////////////////////////////////////////////////////////////

scene_object::scene_object()
{

}

scene_object::~scene_object()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::scenes::scene_object);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::scenes::scene_object, aligned_object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::scenes::scene_object, aligned_object);


maths::matrix4 scene_object::world_matrix()const
{
	return maths::matrix::scale((maths::float3)m_scale)
		* maths::matrix::rotation_y(m_rotation.get<1>())
		* maths::matrix::rotation_x(m_rotation.get<0>())
		* maths::matrix::rotation_z(m_rotation.get<2>())
		* maths::matrix::translation((maths::float3)m_position);
}


/////////////////////////////////////////////////////////////////////

#define MY_TYPE scene
#include <ang/inline/object_wrapper_specialization.inl>
#undef MY_TYPE

scene::scene()
	: m_saturation(1)
	, m_loaded(false)
{
	m_camera = new scenes::camera();
	m_clip_size = { 800,600 };

	m_dispatcher = core::async::thread::create_thread();
	//m_dispatcher->resume();
}

scene::~scene()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::scenes::scene);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::scenes::scene, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::scenes::scene, object);

void scene::clear()
{
	close();

	m_lights.reset();
	if(m_dispatcher)
		m_dispatcher->exit();
	m_dispatcher = null;

	m_driver = null;
	m_effect_library = null;
	m_texture_loader = null;
	m_camera = null;
	m_source_map = null;
}

bool scene::load(idriver_t driver, effects::ieffect_library_t fxlibrary, textures::itexture_loader_t texloader, dom::xml::xml_node_t scene_info)
{
	if (driver == null)
		return false;
	if (scene_info.is_empty() || !scene_info->xml_has_children())
		return false;

	m_driver = driver;
	m_effect_library = fxlibrary;
	m_texture_loader = texloader;

	for(dom::xml::xml_node_t node : scene_info->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		if (name.ptr() == null)
			continue;
		if (name == "sources"_s)
		{
			load_sources(node);
		}
		//else if (name == "camera"_s)
		//{
		//	if(m_camera == null)m_camera = new scenes::camera();
		//	if (!m_camera->load(this, node))
		//		m_camera = null;
		//}
		else if (name == "lights"_s)
		{
			load_lights(node);
		}
		else if (name == "object"_s)
		{
			auto type = node->xml_attributes()["type"_s]->xml_as<text::raw_cstr>();
			if (type == "camera"_s)
			{
				if (m_camera == null)m_camera = new scenes::camera();
				if (!m_camera->load(this, node))
					m_camera = null;
			}
			else if (type == "skybox"_s)
			{
				load_object(node);
			}
			else if (type == "character"_s)
			{
				load_object(node);
			}
			
		}
	}
	m_loaded = true;
	return true;
}

core::async::iasync<bool> scene::load_async(idriver_t driver, effects::ieffect_library_t fxlibrary, textures::itexture_loader_t texloader, dom::xml::xml_node_t scene_info) {
	scene_t auto_save = this;
	return run_async<bool>([=](core::async::iasync<bool>) 
	{
		return load(driver, fxlibrary, texloader, scene_info);
	});
}

bool scene::load_sources(dom::xml::xml_node_t sources_info)
{
	if (sources_info.is_empty() || !sources_info->xml_has_children())
		return false;
	for(dom::xml::xml_node_t file : sources_info->xml_children())
	{
		auto name = file->xml_name()->xml_as<text::raw_cstr>();
		if (name != "file"_s)
			continue;
		m_mutex.lock();
		m_source_map += {file->xml_attributes()["sid"_s]->xml_as<text::raw_cstr>(), file->xml_value()->xml_as<text::raw_cstr>()};
		m_mutex.unlock();
	}
	return true;
}

bool scene::load_lights(dom::xml::xml_node_t lights_info)
{
	if (lights_info.is_empty() || !lights_info->xml_has_children())
		return false;
	streams::string_input_stream_t wrapper = new streams::string_input_stream(null);
	streams::itext_input_stream_t stream = wrapper.get();
	for (dom::xml::xml_node_t node : lights_info->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		if (name == "ambient"_s)
		{
			m_saturation = 1;
			auto color = node["color"_s];
			if (!color.is_empty() && color->xml_has_value())
			{
				graphics::color_t c = color->xml_value()->xml_as<graphics::color>();
				if (c == colors::null) {
					wrapper->attach(color->xml_value().get());
					stream >> m_ambient_color.get<0>() >> ","_s >> m_ambient_color.get<1>() >> ","_s >> m_ambient_color.get<2>();
				}
				else {
					m_ambient_color = maths::float3(c.components.red, c.components.green, c.components.blue) / 255.0f;
				}
			}
			auto sat = node["saturation"_s];
			if (!sat.is_empty() && sat->xml_has_value())
			{
				wrapper->attach(sat->xml_value().get());
				stream >> m_saturation;
			}
		}
		else if (name == "light"_s)
		{
			light_info info;
			auto color = node["color"_s];
			if (!color.is_empty() && color->xml_has_value())
			{
				wrapper->attach(color->xml_value().get());
				stream >> info.color.get<0>() >> ","_s >> info.color.get<1>() >> ","_s >> info.color.get<2>();
			}
			auto position = node["position"_s];
			if (!position.is_empty() && position->xml_has_value())
			{
				info.type = light_type::spot;
				wrapper->attach(position->xml_value().get());
				stream >> info.pos_dir.get<0>() >> ","_s >> info.pos_dir.get<1>() >> ","_s >> info.pos_dir.get<2>();
			}

			auto direction = node["direction"_s];
			if (!direction.is_empty() && direction->xml_has_value())
			{
				info.type = light_type::directional;
				wrapper->attach(position->xml_value().get());
				stream >> info.pos_dir.get<0>() >> ","_s >> info.pos_dir.get<1>() >> ","_s >> info.pos_dir.get<2>();
			}
			m_lights += info;
		}
	}
	return true;
}

bool scene::load_object(dom::xml::xml_node_t object_info)
{
	if (object_info.is_empty())
		return false;
	model_t mod = new model();
	if (!mod->load(this, object_info))
		return null;
	m_mutex.lock();
	m_objects += mod.get();
	m_mutex.unlock();
	return true;
}


void scene::update(core::time::step_timer const& timer)
{
	if (!m_loaded)
		return;

	//auto obj = m_objects[0];
	//obj->rotation({ 0, obj->rotation().get<1>() + 0.1f * timer.elapsed_time() / 100000.0f,0,0 });

	for(scene_object_t& node : m_objects)
		node->update(timer);
}

void scene::draw(idriver_t driver, iframe_buffer_t frame)
{
	if (!m_loaded)
		return;

	m_clip_size = frame->dimentions();
	maths::float4 clear_color = maths::float4(m_saturation * m_ambient_color / 7.5f, 1);
	driver->bind_frame_buffer(frame);
	driver->clear(clear_color);
	m_camera->draw(this);
	for(scene_object_t& node : m_objects)
		node->draw(this);	
}

void scene::close()
{
	for(scene_object_t& node : m_objects)
		node->close();
	m_objects = null;
}