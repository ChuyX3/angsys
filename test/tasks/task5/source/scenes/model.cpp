#include "pch.h"
#include "scenes/scenes.h"

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::scenes;

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
	return false;
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
	double totalRotation = total * radiansPerSecond;
	float radians = static_cast<float>(fmod(totalRotation, 3.141592f * 2));

	_rotation.set<1>(radians);

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


void model::draw(idriver_t driver, camera_t cam) {
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
					lights_info[0 /*"ambient"*/].cast<maths::float4>() = { 1,1,1,.8f };
					lights_info[1 /*"lights_count"*/].cast<uint>() = 1;
					lights_info[2 /*"specular_power"*/].cast<float>() = 0.8;
					auto lights = lights_info[3 /*"lights"*/].cast<static_array<reflect::variable>>();
					lights[0][0 /*"color"*/].cast<maths::float4>() = { .9f,.9f,.9f,1 };
					lights[0][1 /*"position"*/].cast<maths::unaligned_float3>() = { 21.75f, 5,10 };
					lights[0][2 /*"type"*/].cast<uint>() = 0;
					shaders->unmap_ps_uniform(driver, lights_info);
				}

				driver->bind_shaders(shaders);
			}

			index i = 0;
			foreach(_element.textures, [&](textures::itexture_t tex) 
			{
				driver->bind_texture(tex, i);
				i++;
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
		});
		driver->bind_vertex_buffer(null);
		driver->bind_shaders(null);
	});
}

void model::close() {
	model_elements = null;

}

