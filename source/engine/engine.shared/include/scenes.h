#ifndef __ANG_FRAMEWORK_H__
#error ...
#elif !defined __ANG_SCENES_H__
#define __ANG_SCENES_H__

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
			};


			class scene_object
				: public aligned_object
			{
			protected:
				maths::float4 _position;
				maths::float4 _rotation;
				maths::float4 _scale;

			protected:
				scene_object();
				virtual~scene_object();

			public:
				ANG_DECLARE_INTERFACE();

				virtual bool load(scene_t, xml::xml_node_t) pure;
				virtual void update(float, float) pure;
				virtual void draw(scene_t) pure;
				virtual void close() pure;

				virtual maths::matrix4 world_matrix()const;

			public:
				maths::float4 position()const { return _position; }
				maths::float4 rotation()const { return _rotation; }
				maths::float4 scale()const { return _scale; }

				void position(maths::float4 const& value) { _position = value; }
				void rotation(maths::float4 const& value) { _rotation = value; }
				void scale(maths::float4 const& value) { _scale = value; }
			};


			class camera
				: public scene_object
			{
			private:
				maths::matrix4 _projection;
				maths::float4 _proj_info;
				float _movement_speed;
				float _rotation_speed;

			public:
				camera();
				ANG_DECLARE_INTERFACE();
				bool load(scene_t, xml::xml_node_t)override;
				bool load(maths::float3 const& position, maths::float3 const& rotation, maths::float4 const& projection);
				void update(float, float)override;
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


			ANG_BEGIN_ENUM(, light_type, uint)
				directional,
				spot,
				ANG_END_ENUM(light_type);

			struct light_info
			{
				maths::float3 color;
				maths::float3 pos_dir;
				light_type_t type;

				bool operator == (const light_info& other)const { return false; }
				bool operator != (const light_info& other)const { return false; }
			};

			class scene
				: public aligned_object
			{
			protected:
				maths::float3 _ambient_color;

				idriver_t _driver;
				effects::ieffect_library_t _effect_library;
				textures::itexture_loader_t _texture_loader;

				camera_t _camera;
				collections::vector<light_info> _lights;
				collections::vector<scene_object_t> _objects;
				collections::map<string, wstring> _source_map;
				core::async::mutex_t main_mutex;
				foundation::size<float> _current_size;

			public:
				scene();

				ANG_DECLARE_INTERFACE();

				bool load(
					idriver_t driver,
					effects::ieffect_library_t fxlibrary,
					textures::itexture_loader_t texloader,
					xml::xml_node_t scene_info
				);

				void update(float, float);
				void draw(idriver_t, iframe_buffer_t);
				void close();

			protected:
				bool load_sources(xml::xml_node_t);
				bool load_lights(xml::xml_node_t);
				bool load_object(xml::xml_node_t);

			public: //inlines
				inline idriver* driver()const { return _driver.get(); }
				inline effects::ieffect_library* effect_library()const { return _effect_library.get(); }
				inline textures::itexture_loader* texture_loader()const { return _texture_loader.get(); }
				inline camera* camera()const { return _camera.get(); }
				inline foundation::size<float>const& current_size()const { return _current_size; }
				inline wstring find_file(cstr_t sid)const {
					core::async::scope_locker lock = main_mutex;
					if (_source_map.is_empty())
						return "";
					auto it = _source_map->find(sid);
					return it.is_valid() ? it->value() : L"";
				}
				inline wstring find_file(cwstr_t sid)const {
					core::async::scope_locker lock = main_mutex;
					if (_source_map.is_empty())
						return "";
					auto it = _source_map->find(sid);
					return it.is_valid() ? it->value() : L"";
				}

				inline maths::float3 ambient_color()const { return _ambient_color; }
				static_array<light_info> ligths()const { return _lights; }
			protected:
				virtual~scene();
			};

			class model;
			typedef object_wrapper<model> model_t;

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

				virtual bool load(scene_t, xml::xml_node_t) override;
				virtual void update(float, float) override;
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
					xml::xml_node_t model_info
				);

			public:
				maths::float4 position()const { return _position; }
				maths::float4 rotation()const { return _rotation; }
				maths::float4 scale()const { return _scale; }

				void position(maths::float4 const& value) { _position = value; }
				void rotation(maths::float4 const& value) { _rotation = value; }
				void scale(maths::float4 const& value) { _scale = value; }

			private:
				virtual~model();
			};
		}
	}
}

ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::scenes::light_info)
ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::scenes::model::model_element)

#endif//__ANG_SCENES_H__
