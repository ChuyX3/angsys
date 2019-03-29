#pragma once
#include <ang/graphics/graphics.h>
#include <ang/core/timer.h>
#include "resources/resources.h"
//#include "scenes/model_loader.h"

namespace ang
{
	namespace graphics
	{
		namespace scenes
		{
			ang_object(scene_object);
			ang_object(scene);
			ang_object(camera);
			ang_object(model);
		}
	}

	ANG_OBJECT_WRAPPER_DECLARATION(, graphics::scenes::scene);
	ANG_OBJECT_WRAPPER_DECLARATION(, graphics::scenes::camera);
	ANG_OBJECT_WRAPPER_DECLARATION(, graphics::scenes::model);

	namespace graphics
	{
		namespace scenes
		{
			class aligned_object
				: public smart<aligned_object>
			{
			public:
				pointer operator new(wsize);
				void operator delete(pointer);

#ifdef _DEBUG
				pointer operator new(wsize, const char*, int);
				void operator delete(pointer, const char*, int);
#endif//_DEBUG

				ANG_DECLARE_INTERFACE();

			protected:
				aligned_object();
				virtual~aligned_object();
				virtual bool auto_release() override;
			};

			class scene_object
				: public smart<scene_object, aligned_object, iscene_object>
			{
			protected:
				maths::float4 m_position;
				maths::float4 m_rotation;
				maths::float4 m_scale;
	
			protected:
				scene_object();
				virtual~scene_object();

			public:
				ANG_DECLARE_INTERFACE();

			public:
				maths::float4 position()const { return m_position; }
				maths::float4 rotation()const { return m_rotation; }
				maths::float4 scale()const { return m_scale; }

				void position(maths::float4 const& value) { m_position = value; }
				void rotation(maths::float4 const& value) { m_rotation = value; }
				void scale(maths::float4 const& value) { m_scale = value; }
			};

			class base_camera 
				: public smart<base_camera, scene_object, icamera>
			{
			protected:
				base_camera();
				virtual~base_camera();
			public:
				ANG_DECLARE_INTERFACE();

			};

			class camera 
				: public smart<camera, base_camera>
			{
			private:
				maths::matrix4 m_projection;
				maths::float4 m_proj_info;
				float m_movement_speed;
				float m_rotation_speed;

			public:
				camera();
				ANG_DECLARE_INTERFACE();
				
				bool load(iscene_t, dom::xml::xml_node_t)override;
				bool save(iscene_t, dom::xml::xml_document_t)const override;
				core::async::iasync<bool> load_async(iscene_t, dom::xml::xml_node_t)override;
				core::async::iasync<bool> save_async(iscene_t, dom::xml::xml_document_t)const override;
				void draw(iscene_t)override;
				void update(core::time::step_timer_t)override;
				void clear(void)override;

				box<float> viewport()const override;
				void viewport(box<float>) override;
				maths::float4x4 view_projection_matrix()const override;
				maths::matrix4 world_matrix()const;

				void projection(float fov, float aspect, float near_plane, float far_plane);
				void projection(const maths::float4&);
				maths::float4 projection()const;

				void move_up(float deltaTime);
				void move_down(float deltaTime);
				void move_forward(float deltaTime);
				void move_backward(float deltaTime);
				void move_left(float deltaTime);
				void move_right(float deltaTime);

				void rotate_up(float deltaTime);
				void rotate_down(float deltaTime);
				void rotate_left(float deltaTime);
				void rotate_right(float deltaTime);

			private:
				virtual~camera();
			};

			class scene 
				: public smart<scene, aligned_object, iscene>
			{
			protected:
			
				maths::float3 m_ambient_color;
				float m_saturation;
				vector<effects::light_info> m_lights;
				core::async::thread_t m_dispatcher;

				idriver_t m_driver;
				effects::ieffect_library_t m_effect_library;
				textures::itexture_loader_t m_texture_loader;
				icamera_t m_camera;
				vector<scene_object_t> m_objects;
				collections::hash_map<string, string> m_source_map;
				mutable core::async::mutex_t m_mutex;
				size<float> m_clip_size;

				bool m_loaded;
			public:
				scene();

				ANG_DECLARE_INTERFACE();

				void clear()override;

				void draw(idriver_t, iframe_buffer_t, icamera_t = nullptr)override;
				void update(core::time::step_timer_t)override;
				effects::ieffect_library_t fx_libreary()const override;
				textures::itexture_loader_t tex_loader()const override;
				icamera_t active_camera()const override;

				bool load(dom::xml::xml_node_t)override;
				bool save(dom::xml::xml_document_t)const override;
				core::async::iasync<bool> load_async(dom::xml::xml_node_t)override;
				core::async::iasync<bool> save_async(dom::xml::xml_document_t)const override;
				bool load_sources(dom::xml::xml_node_t) override;
				resources::ilibrary_t load_library(dom::xml::xml_node_t) override;
				resources::iresource_t load_resource(dom::xml::xml_node_t) override;
				core::async::iasync<resources::ilibrary_t> load_library_async(dom::xml::xml_node_t) override;
				core::async::iasync<resources::iresource_t> load_resource_async(dom::xml::xml_node_t) override;
				ifactory_t factory()const override;
				core::async::idispatcher_t dispatcher()const override;
				string find_source(cstr_t)const override;
				resources::iresource_t find_resource(cstr_t)const override;
				void clear()override;

			protected:
				bool load_sources(dom::xml::xml_node_t);
				bool load_lights(dom::xml::xml_node_t);
				bool load_object(dom::xml::xml_node_t);
	
			public: //inlines
				inline size<float>const& clip_size()const { return m_clip_size; }
				

				template<typename T> core::async::iasync<T> run_async(core::delegates::function<T(core::async::iasync<T>)> func) {
					return m_dispatcher->run_async<T>(func);
				}

				inline maths::float3 ambient_color()const { return m_ambient_color; }
				inline float ambient_saturation()const { return m_saturation; }
				array_view<effects::light_info> ligths()const { return m_lights; }
			protected:
				virtual~scene();
			};

			class model
				: public smart<model, scene_object, imodel>
			{
			public:
				class model_element
					: public smart<model_element, imodel_element>
				{
				private:
					buffers::iindex_buffer_t m_index_buffer;
					buffers::ivertex_buffer_t m_vertex_buffer;
					reflect::struct_buffer_t m_uniforms;
					vector<textures::itexture_t> m_textures;

				public:
					model_element();

					ANG_DECLARE_INTERFACE();

				public:
					string technique() const override;
					buffers::iindex_buffer_t index_buffer() const override;
					buffers::ivertex_buffer_t vertex_buffer() const override;
					reflect::varying_t uniform(cstr_t) const override;
					collections::ienum_ptr<textures::itexture_t> textures() const override;
				};

			protected:
				collections::vector<imodel_element_t> model_elements;

			public:
				model();
	
			public:
				ANG_DECLARE_INTERFACE();

				bool load(iscene_t, dom::xml::xml_node_t)override;
				bool save(iscene_t, dom::xml::xml_document_t)const override;
				core::async::iasync<bool> load_async(iscene_t, dom::xml::xml_node_t)override;
				core::async::iasync<bool> save_async(iscene_t, dom::xml::xml_document_t)const override;
				void draw(iscene_t)override;
				void update(core::time::step_timer_t)override;
				void clear(void)override;

				collections::ienum_ptr<imodel_element_t> elements()const override;
				maths::float4x4 world_matrix()const override;

			private:
				virtual~model();
			};




		}
	}

}
