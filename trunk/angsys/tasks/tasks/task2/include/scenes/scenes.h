#pragma once
#include <ang/graphics/graphics.h>
#include <ang/core/timer.h>
#include "scenes/model_loader.h"

namespace ang
{
	namespace graphics
	{
		namespace scenes
		{
			class scene_object;
			typedef object_wrapper<scene_object> scene_object_t;

			class scene;
			typedef object_wrapper<scene> scene_t;

			class camera;
			typedef object_wrapper<camera> camera_t;

			class model;
			typedef object_wrapper<model> model_t;

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
				: public object
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
				: public aligned_object
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

				virtual bool load(scene_t, dom::xml::xml_node_t) = 0;
				virtual void update(core::time::step_timer const&) = 0;
				virtual void draw(scene_t) = 0;
				virtual void close() = 0;

				virtual maths::matrix4 world_matrix()const;

			public:
				maths::float4 position()const { return m_position; }
				maths::float4 rotation()const { return m_rotation; }
				maths::float4 scale()const { return m_scale; }

				void position(maths::float4 const& value) { m_position = value; }
				void rotation(maths::float4 const& value) { m_rotation = value; }
				void scale(maths::float4 const& value) { m_scale = value; }
			};


			class camera
				: public scene_object
			{
			private:
				maths::matrix4 m_projection;
				maths::float4 m_proj_info;
				float m_movement_speed;
				float m_rotation_speed;

			public:
				camera();
				ANG_DECLARE_INTERFACE();
				bool load(scene_t, dom::xml::xml_node_t)override;
				bool load(maths::float3 const& position, maths::float3 const& rotation, maths::float4 const& projection);
				void update(core::time::step_timer const&)override;
				void draw(scene_t)override;
				void close()override;

				maths::matrix4 view_matrix()const;
				maths::matrix4 const& projection_matrix()const;
				maths::matrix4 world_matrix()const override;

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


			safe_enum(, light_type, uint)
			{
				directional,
				spot,
			};

			struct light_info
			{
				maths::float3 color;
				maths::float3 pos_dir;
				light_type_t type;

				bool operator == (const light_info& other)const { return false; } //dummy
				bool operator != (const light_info& other)const { return false; } //dummy
			};

			class scene 
				: public aligned_object
			{
			protected:
			
				maths::float3 m_ambient_color;
				float m_saturation;
				vector<light_info> m_lights;
				core::async::thread_t m_dispatcher;

				idriver_t m_driver;
				effects::ieffect_library_t m_effect_library;
				textures::itexture_loader_t m_texture_loader;
				camera_t m_camera;
				vector<scene_object_t> m_objects;
				collections::hash_map<string, wstring> m_source_map;
				mutable core::async::mutex_t m_mutex;
				size<float> m_clip_size;

				bool m_loaded;
			public:
				scene();

				ANG_DECLARE_INTERFACE();

				void clear()override;

				bool load(
					idriver_t driver, 
					effects::ieffect_library_t fxlibrary,
					textures::itexture_loader_t texloader,
					dom::xml::xml_node_t scene_info
				);

				core::async::iasync<bool> load_async(
					idriver_t driver,
					effects::ieffect_library_t fxlibrary,
					textures::itexture_loader_t texloader,
					dom::xml::xml_node_t scene_info
				);

				void update(core::time::step_timer const&);
				void draw(idriver_t, iframe_buffer_t);
				void close();

			protected:
				bool load_sources(dom::xml::xml_node_t);
				bool load_lights(dom::xml::xml_node_t);
				bool load_object(dom::xml::xml_node_t);
	
			public: //inlines
				inline idriver* driver()const { return m_driver.get(); }
				inline effects::ieffect_library* effect_library()const { return m_effect_library.get(); }
				inline textures::itexture_loader* texture_loader()const { return m_texture_loader.get(); }
				inline camera* camera()const { return m_camera.get(); }
				inline size<float>const& clip_size()const { return m_clip_size; }
				inline wstring find_file(text::raw_cstr_t sid)const {
					core::async::scope_locker<core::async::mutex> lock = m_mutex;
					if (m_source_map.is_empty())
						return L""_s;
					auto it = m_source_map->find(sid);
					return it.is_valid() ? it->value : L""_s;
				}

				template<typename T> core::async::iasync<T> run_async(core::delegates::function<T(core::async::iasync<T>)> func) {
					return m_dispatcher->run_async<T>(func);
				}

				inline maths::float3 ambient_color()const { return m_ambient_color; }
				inline float ambient_saturation()const { return m_saturation; }
				array_view<light_info> ligths()const { return m_lights; }
			protected:
				virtual~scene();
			};

			class model
				: public scene_object
			{
			public:
				struct model_element
				{
					effects::ishaders_t technique;
					buffers::iindex_buffer_t index_buffer;
					buffers::ivertex_buffer_t vertex_buffer;
					collections::vector<textures::itexture_t> textures;

					bool operator == (const model_element&)const { return false; }//dummy
					bool operator != (const model_element&)const { return false; }//dummy
				};

			protected:
				collections::vector<model_element> model_elements;

			public:
				model();
	
			public:
				ANG_DECLARE_INTERFACE();

				virtual bool load(scene_t, dom::xml::xml_node_t) override;
				virtual void update(core::time::step_timer const&) override;
				virtual void draw(scene_t) override;
				virtual void close() override;

				virtual bool load(idriver_t driver,
					effects::ieffect_library_t fxlibrary,
					textures::itexture_loader_t texloader, 
					core::files::input_text_file_t file
					);

				virtual bool load(idriver_t driver,
					effects::ieffect_library_t fxlibrary,
					textures::itexture_loader_t texloader,
					dom::xml::xml_node_t model_info
				);

			public:
				maths::float4 position()const { return m_position; }
				maths::float4 rotation()const { return m_rotation; }
				maths::float4 scale()const { return m_scale; }

				void position(maths::float4 const& value) { m_position = value; }
				void rotation(maths::float4 const& value) { m_rotation = value; }
				void scale(maths::float4 const& value) { m_scale = value; }

			private:
				virtual~model();
			};




		}
	}


	template<> inline ang::maths::float2 ang::dom::xml::xml_text::xml_as<ang::maths::float2>()const {
		text::raw_cstr_t cstr = xml_as<text::raw_cstr>();
		if (cstr.ptr() == null)
			return{ 0,0 };
		text::iparser_t parser = text::iparser::get_parser(cstr.encoding());
		windex i = 0;
		float temp[2];
		parser->seek(cstr, i, " [ "_s);
		temp[0] = parser->to_floating(cstr, i);
		parser->seek(cstr, i, " , "_s);
		temp[1] = parser->to_floating(cstr, i);
		//parser->seek(cstr, i, " ] "_s);
		return{ temp[0], temp[1] };
	}

	template<> inline ang::maths::float3 ang::dom::xml::xml_text::xml_as<ang::maths::float3>()const {
		text::raw_cstr_t cstr = xml_as<text::raw_cstr>();
		if (cstr.ptr() == null)
			return{ 0,0 };
		text::iparser_t parser = text::iparser::get_parser(cstr.encoding());
		windex i = 0;
		float temp[3];
		parser->seek(cstr, i, " [ "_s);
		temp[0] = parser->to_floating(cstr, i);
		parser->seek(cstr, i, " , "_s);
		temp[1] = parser->to_floating(cstr, i);
		parser->seek(cstr, i, " , "_s);
		temp[2] = parser->to_floating(cstr, i);
		//parser->seek(cstr, i, " ] "_s);
		return{ temp[0], temp[1], temp[2] };
	}

	template<> inline ang::maths::float4 ang::dom::xml::xml_text::xml_as<ang::maths::float4>()const {
		text::raw_cstr_t cstr = xml_as<text::raw_cstr>();
		if (cstr.ptr() == null)
			return{ 0,0 };
		text::iparser_t parser = text::iparser::get_parser(cstr.encoding());
		windex i = 0;
		float temp[4];
		parser->seek(cstr, i, " [ "_s);
		temp[0] = parser->to_floating(cstr, i);
		parser->seek(cstr, i, " , "_s);
		temp[1] = parser->to_floating(cstr, i);
		parser->seek(cstr, i, " , "_s);
		temp[2] = parser->to_floating(cstr, i);
		parser->seek(cstr, i, " , "_s);
		temp[3] = parser->to_floating(cstr, i);
		//parser->seek(cstr, i, " ] "_s);
		return{ temp[0], temp[1], temp[2] , temp[3] };
	}


}

ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ang::graphics::scenes::light_info);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ang::graphics::scenes::model::model_element);