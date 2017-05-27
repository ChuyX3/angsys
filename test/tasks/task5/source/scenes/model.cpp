#include "pch.h"
#include "scenes/scenes.h"

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

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


model::model()
{

}

model::~model()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::scenes::model, scene_object);

bool model::load(scene_t scene, xml::xml_node_t node)
{
	if (scene.is_empty() || node.is_empty() || !node->xml_has_children())
		return false;

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

	auto scale = node["scale"];
	if (scale.is_empty())
		_scale = { 0, 0, 0, 1 };
	else
		_scale = { scale->xml_value().as<maths::float3>() , 1 };

	auto source = node["source"];
	if (!source.is_empty())
	{
		wstring filename = source->xml_value();
		auto _filename = scene->find_file(filename);
		core::files::input_text_file_t file;
		if (_filename.is_empty()) file = new core::files::input_text_file(filename);
		else file = new core::files::input_text_file(_filename);
		if (!file->is_valid())
			return false;

		auto type = source->xml_attributes()["type"].as<cwstr_t>();

		if (type == "indexed")
		{
			try {
				indexed_model_t _loader = new indexed_model();
				xml::xml_document_t doc = new xml::xml_document(file);
				_loader->load_async(doc->xml_root().get()).then<bool>([this, _loader, scene, file](core::async::iasync<collections::vector<indexed_model::model_element>>* task)->bool
				{
					textures::itexture_loader_t texloader = scene->texture_loader();
					effects::ieffect_library_t fxlibrary = scene->effect_library();
					idriver_t driver = scene->driver();

					collections::vector<indexed_model::model_element> data;
					try
					{
						data = ang::move(task->result());
					}
					catch (exception_t)
					{
						return false;
					}

					model_element element;
					reflect::attribute_desc desc[3] =
					{
						{ reflect::var_type::f32, reflect::var_class::vec4, "position"_s, reflect::var_semantic::position, 0 },
						{ reflect::var_type::f32, reflect::var_class::vec4, "normal"_s, reflect::var_semantic::normal, 0 },
						{ reflect::var_type::f32, reflect::var_class::vec2, "texcoord"_s, reflect::var_semantic::tex_coord, 0 }
					};

					foreach(data, [&](indexed_model::model_element& _element)
					{
						if (_element.vertex_data.is_empty() || _element.vertex_data->buffer_size() == 0)
							return;
						//if (_element.material == "_ea80e0f_dds"_s)
						//	return;
						auto stride = reflect::attribute_desc::get_size_in_bytes(_element.vertex_desc);
						element.vertex_buffer = driver->create_vertex_buffer(
							graphics::buffers::buffer_usage::dynamic,
							_element.vertex_desc,
							_element.vertex_data->buffer_size() / stride,
							static_array<byte>((byte*)_element.vertex_data->buffer_ptr(), _element.vertex_data->buffer_size()));

						element.index_buffer = driver->create_index_buffer(
							graphics::buffers::buffer_usage::dynamic,
							_element.index_type,
							_element.index_data->buffer_size() / reflect::variable_desc(_element.index_type, reflect::var_class::scalar).get_size_in_bytes(),
							static_array<byte>((byte*)_element.index_data->buffer_ptr(), _element.index_data->buffer_size()));

						element.technique = fxlibrary->find_technique(_element.technique_name);

						foreach(_element.textures, [&](string& texture)
						{
							auto res = texloader->load_texture(texture);
							driver->execute_on_thread_safe([&]()
							{
								if (!res.is_empty()) element.textures += res;
							});

						});
				
						driver->execute_on_thread_safe([&]()
						{
							model_elements += element;
						});

						//index idx = model_elements->counter() - 1;
						/*texloader->load_texture_async(_element.material).then<bool>([=](core::async::iasync<graphics::textures::itexture_t>* result)
						{
						driver->execute_on_thread_safe([&]()
						{
						try {
						auto res = result->result();
						if (!res.is_empty())
						model_elements[idx].textures += res;
						}
						catch (exception_t)
						{

						}
						});
						return true;
						});*/
					});
					return true;
				});
				return true;
			}
			catch (exception_t e)
			{
				printf("%s\n", cstr_t(e->what()).cstr());
			}
		}
		
		ordered_model_t _loader = new ordered_model();
		_loader->load_async(file).then<bool>([this, _loader, scene, file](core::async::iasync<collections::vector<ordered_model::model_element>>* task)->bool
		{
			textures::itexture_loader_t texloader = scene->texture_loader();
			effects::ieffect_library_t fxlibrary = scene->effect_library();
			idriver_t driver = scene->driver();

			collections::vector<ordered_model::model_element> data;
			try
			{
				data = ang::move(task->result());
			}
			catch (exception_t)
			{
				return false;
			}

			model_element element;
			reflect::attribute_desc desc[3] =
			{
				{ reflect::var_type::f32, reflect::var_class::vec4, "position"_s, reflect::var_semantic::position, 0 },
				{ reflect::var_type::f32, reflect::var_class::vec4, "normal"_s, reflect::var_semantic::normal, 0 },
				{ reflect::var_type::f32, reflect::var_class::vec2, "texcoord"_s, reflect::var_semantic::tex_coord, 0 }
			};

			foreach(data, [&](ordered_model::model_element& _element)
			{
				if (_element.vertices.is_empty() || _element.vertices->counter() == 0)
					return;

				element.vertex_buffer = driver->create_vertex_buffer(
					graphics::buffers::buffer_usage::dynamic,
					static_array<reflect::attribute_desc>(desc, 3),
					_element.vertices->counter(),
					static_array<byte>((byte*)_element.vertices->data(), _element.vertices->counter() * sizeof(ordered_model::vertex)));

				element.technique = fxlibrary->find_technique("character_lighting_fx"_s);

				driver->execute_on_thread_safe([&]()
				{
					model_elements += element;
				});

				index idx = model_elements->counter() - 1;
				texloader->load_texture_async(_element.material).then<bool>([=](core::async::iasync<graphics::textures::itexture_t>* result)
				{
					driver->execute_on_thread_safe([&]()
					{
						try {
							auto res = result->result();
							if (!res.is_empty())
								model_elements[idx].textures = { res };
						}
						catch (exception_t)
						{

						}
					});
					return true;
				});
			});
			return true;
		});

		
	}
	return true;
}

bool model::load(idriver_t driver, effects::ieffect_library_t fxlibrary, textures::itexture_loader_t _texloader, core::files::input_text_file_t file)
{
	_position = { 0,0,-3,0 };
	_rotation = { 0,0,0,0 };
	_scale = { .30f,.30f,.30f,.30f };
	ordered_model_t _loader = new ordered_model();
	//_loader->add_ref();
	_loader->load_async(file).then<bool>([this, _loader, driver, fxlibrary, _texloader, file](core::async::iasync<collections::vector<ordered_model::model_element>>* task)->bool
	{
		textures::itexture_loader_t texloader = _texloader.get();
		collections::vector<ordered_model::model_element> data;
		try
		{
			data = ang::move(task->result());
		}
		catch (exception_t)
		{
			return false;
		}

		model_element element;
		reflect::attribute_desc desc[3] = 
		{
			{ reflect::var_type::f32, reflect::var_class::vec4, "position"_s, reflect::var_semantic::position, 0 },
			{ reflect::var_type::f32, reflect::var_class::vec4, "normal"_s, reflect::var_semantic::normal, 0 },
			{ reflect::var_type::f32, reflect::var_class::vec2, "texcoord"_s, reflect::var_semantic::tex_coord, 0 }
		};
		
		foreach(data, [&](ordered_model::model_element& _element)
		{
			if (_element.vertices.is_empty() || _element.vertices->counter() == 0)
				return;
			//if (_element.material == "_ea80e0f_dds"_s)
			//	return;

			element.vertex_buffer = driver->create_vertex_buffer(
				graphics::buffers::buffer_usage::dynamic,
				static_array<reflect::attribute_desc>(desc, 3),
				_element.vertices->counter(),
				static_array<byte>((byte*)_element.vertices->data(), _element.vertices->counter() * sizeof(ordered_model::vertex)));

			element.technique = fxlibrary->find_technique("character_lighting_fx"_s);

			driver->execute_on_thread_safe([&]()
			{
				model_elements += element;
			});
			
			index idx = model_elements->counter() - 1;
			texloader->load_texture_async(_element.material).then<bool>([=](core::async::iasync<graphics::textures::itexture_t>* result)
			{		
				driver->execute_on_thread_safe([&]()
				{
					try {
						auto res = result->result();
						if (!res.is_empty())
							model_elements[idx].textures += res;
					}
					catch (exception_t)
					{

					}
				});
				return true;
			});
		});

	
		return true;
	});

	return true;
}

bool model::load(idriver_t driver, effects::ieffect_library_t fxlibrary, textures::itexture_loader_t texloader, xml::xml_node_t model_info)
{

	return true;
}

inline float XMConvertToRadians(float fDegrees) { return fDegrees * (3.141592f / 180.0f); }
inline float XMConvertToDegrees(float fRadians) { return fRadians * (180.0f / 3.141592f); }

void model::update(float total, float delta)
{
	float radiansPerSecond = XMConvertToRadians(45);
	double totalRotation = total * radiansPerSecond / 1000.0f;
	float radians = static_cast<float>(fmod(totalRotation, 3.141592f * 2));

//	_rotation.set<1>(radians);

}


struct light_data_global
{
	maths::float4 ambient_color;
	uint light_count;
	float specular_power;
	struct {
		maths::float4 color;
		maths::unaligned_float3 position;
		uint type;
	}light_data[10];
} _light_data;


void model::draw(scene_t scene) {
	idriver_t driver = scene->driver();
	camera_t cam = scene->camera();
	driver->execute_on_thread_safe([&]()
	{
		effects::ishaders_t shaders = null;
		foreach(model_elements, [&](model_element& _element)
		{
			if (shaders.get() != _element.technique.get())
			{
				shaders = _element.technique;
				auto uniform = shaders->map_vs_uniform(driver, 0 /*"wvp"*/);
				uniform[0 /*"world"*/].cast<maths::float4x4>() = ang::maths::matrix::transpose(world_matrix());
				uniform[1 /*"view"*/].cast<maths::float4x4>() = ang::maths::matrix::transpose(cam->view_matrix());
				uniform[2 /*"proj"*/].cast<maths::float4x4>() = ang::maths::matrix::transpose(cam->projection_matrix());
				shaders->unmap_vs_uniform(driver, uniform);

				auto lights_info = shaders->map_ps_uniform(driver, 0 /*"lights"*/);
				if (lights_info.raw_data().get() != null)
				{
					auto scene_lights = scene->ligths();

					lights_info[0 /*"ambient"*/].cast<maths::float4>() = { scene->ambient_color(), 0.5f };
					lights_info[1 /*"lights_count"*/].cast<uint>() = scene_lights.size();
					lights_info[2 /*"specular_power"*/].cast<float>() = 10;
					auto lights = lights_info[3 /*"lights"*/].cast<static_array<reflect::variable>>();
					for (index i = 0; i < scene_lights.size(); ++i)
					{
						lights[i][0 /*"color"*/].cast<maths::float4>() = maths::float4(scene_lights[i].color, 1.0f);
						lights[i][1 /*"position"*/].cast<maths::float3>() = scene_lights[i].pos_dir;
						lights[i][2 /*"type"*/].cast<uint>() = scene_lights[i].type.get();
					}
					shaders->unmap_ps_uniform(driver, lights_info);

				}

				driver->bind_shaders(shaders);
			}

			index i = 0;
			foreach(_element.textures, [&](textures::itexture_t& tex) 
			{
				driver->bind_texture(tex, i++);
			});

			driver->bind_vertex_buffer(_element.vertex_buffer);
			if (_element.index_buffer.is_empty())
			{
				driver->bind_index_buffer(null);
				driver->draw(_element.vertex_buffer->block_counter(), primitive::triangle);
			}
			else
			{
				driver->bind_index_buffer(_element.index_buffer);
				driver->draw_indexed(_element.index_buffer->counter(), primitive::triangle);
			}

			for(index i = 0, c = _element.textures.is_empty()? 1 : _element.textures->counter(); i < c; ++i)
				driver->bind_texture(null, i);
		});
		driver->bind_vertex_buffer(null);
		driver->bind_shaders(null);
	});
}

void model::close() {
	model_elements = null;

}

