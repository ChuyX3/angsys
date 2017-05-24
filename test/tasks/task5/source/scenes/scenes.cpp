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

aligned_object::aligned_object()
{

}

aligned_object::~aligned_object()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::scenes::aligned_object, object);

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

scene::scene()
{
	_camera = new scenes::camera();
}

scene::~scene()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::scenes::scene, object);

bool scene::load(idriver_t driver, effects::ieffect_library_t fxlibrary, textures::itexture_loader_t texloader, xml::xml_node_t scene_info)
{
	if (_driver == null)
		return false;
	if (scene_info.is_empty() || !scene_info->xml_has_children())
		return false;

	_effect_library = fxlibrary;
	_texture_loader = texloader;

	xml::foreach(scene_info->xml_children(), [&](xml::xml_node_t node)
	{
		auto name = node->xml_name().as<cwstr_t>();
		if (name.cstr() == null)
			return;
		if (name == "resources")
		{
			/*core::files::input_text_file_t file = new core::files::input_text_file();
			if (file->open(node->xml_value().as<cwstr_t>()))
			{
				xml::xml_document_t doc = new xml::xml_document(file);
				file->close();
				if (_resources.is_empty())
				{
					_resources = new resources::resource_manager(driver());
					_resources->load(doc->xml_root().get());
				}
				else
				{

				}
			}*/
		}
		else if (name == "camera")
		{
			_camera = new scenes::camera();
			if (!_camera->load(this, node))
				_camera = null;
		}
		/*else if (!_resources.is_empty() && name == "object")
		{
			scene_object_t obj = new character();
			if (!obj->load(this, node))
				return;
			objects += obj;
		}*/
	});
	return true;
}

void scene::update(float total, float delta)
{
	//auto obj = objects[0];
	//obj->rotation({ 0, obj->rotation().get<1>() + delta / 100000.0f,0,0 });
	foreach(objects, [&](scene_object_t& node)
	{
		node->update(total, delta);
	});
}

void scene::draw(idriver_t driver, camera_t cam)
{
	foreach(objects, [&](scene_object_t& node)
	{
		node->draw(driver, cam);
	});
}

void scene::close()
{
	foreach(objects, [&](scene_object_t& node)
	{
		node->close();
	});
	objects = null;
}