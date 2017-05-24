#pragma once
#include <ang/graphics/angraph.h>
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

				virtual bool load(scene_t, xml::xml_node_t) = 0;
				virtual void update(float, float) = 0;
				virtual void draw(idriver_t, camera_t) = 0;
				virtual void close() = 0;

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
				void draw(idriver_t, camera_t)override;
				void close()override;

				maths::matrix4 view_matrix()const;
				maths::matrix4 const& projection_matrix()const;
				maths::matrix4 world_matrix()const override;

				void projection(float fov, float aspect, float near_plane, float far_plane);

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

			class scene : public object
			{
			protected:
				idriver_t _driver;
				effects::ieffect_library_t _effect_library;
				textures::itexture_loader_t _texture_loader;
				camera_t _camera;
				collections::vector<scene_object_t> objects;

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
				void draw(idriver_t, camera_t);
				void close();
	
			public: //inlines
				inline idriver* driver()const { return _driver.get(); }
				inline effects::ieffect_library* effect_library()const { return _effect_library.get(); }
				inline textures::itexture_loader* texture_loader()const { return _texture_loader.get(); }
				inline camera* camera()const { return _camera.get(); }

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

					bool operator == (const model_element&) { return false; }//dummy
					bool operator != (const model_element&) { return false; }//dummy
				};

			protected:
				collections::vector<model_element> model_elements;

			public:
				model();
	
			public:
				ANG_DECLARE_INTERFACE();

				virtual bool load(scene_t, xml::xml_node_t) override;
				virtual void update(float, float) override;
				virtual void draw(idriver_t, camera_t) override;
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

ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::scenes::model::model_element);