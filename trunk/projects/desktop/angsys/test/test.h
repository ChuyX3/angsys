#pragma once

namespace ang
{
	ang_declare_object(test);
	ang_declare_object(camera);
	ang_declare_object(object3D);
	ang_declare_object(library);

	namespace graphics
	{
		ang_declare_object(gl_context);
		ang_declare_object(gl_resource);
		ang_declare_object(gl_shaders);
		ang_declare_object(gl_mesh);
		ang_declare_object(gl_texture);
	}

	struct ANG_ALIGN(16) vertex
	{
		maths::float3 position;
		maths::float3 normal;
		maths::float3 binormal;
		maths::float3 tangent;
		maths::float2 texcoord;
	};

	struct ANG_ALIGN(16) vertex2
	{
		maths::float3 position;
		maths::float4 color;
	};

}

#include "context.h"
#include "shaders.h"
#include "mesh.h"
#include "texture.h"

namespace ang
{
	class camera
		: public implement<camera
		, iid("ang::camera")
		, framework::aligned>
	{
	private:
		maths::float4x4 m_projection;
		
		maths::float4 m_position;
		maths::float4 m_rotation;
		float m_movement_speed;
		float m_rotation_speed;

	public:
		camera();

		bool load(graphics::size<float> screen, maths::float3 const& position = { 0,0,0 }, maths::float3 const& rotation = { 0,0,0 });
		void update(core::time::step_timer const&);

		maths::float4x4 view_matrix()const;
		maths::float4x4 projection_matrix()const;
		maths::float4x4 world_matrix()const;

		void projection(float fov, float aspect, float near_plane, float far_plane);
		void projection(const maths::float4&);

		void move_x(float deltaTime);
		void move_y(float deltaTime);
		void move_z(float deltaTime);

		void rotate_yaw(float deltaTime);
		void rotate_pitch(float deltaTime);
		void rotate_roll(float deltaTime);

	private:
		virtual~camera();
	};

	class object3D
		: public implement<object3D
		, iid("ang::object3D")
		, framework::aligned>
	{
	private:
		maths::float4 m_position;
		maths::float3 m_rotation;
		maths::float3 m_scale;

		core::async::mutex_t m_mutex;
		graphics::gl_mesh_t m_mesh;
		graphics::gl_shaders_t m_shaders;
		vector<graphics::gl_texture_t> m_textures;

	public:
		object3D();

		virtual bool load(graphics::gl_context_t, string vshader, string fshader, string model, array<string> textures);
		virtual void update(core::time::step_timer const&);
		virtual void draw(graphics::gl_context_t, camera_t);
		virtual void dispose()override;

		maths::float4x4 world_matrix()const;
	};

	class library
		: public implement<library
		, iid("ang::resource_manager")>
	{
	private:
		collections::hash_map<string, string> m_sources;
		collections::hash_map<string, graphics::gl_mesh_t> m_meshes;
		collections::hash_map<string, graphics::gl_shaders_t> m_shaders;
		collections::hash_map<string, graphics::gl_texture_t> m_textures;

	public:
		library();

		void load(dom::xml::ixml_node_t);
		void load_sources(dom::xml::ixml_node_t);
		void load_resources(dom::xml::ixml_node_t);
		
		core::async::iasync<library_t> load_async(dom::xml::ixml_node_t);
		void load_resources(dom::xml::ixml_node_t);
	};

	class test
		: public implement<test
		, iid("ang::test")>
	{
	public:
		static int main(array<string> args) {
			test_t game = new test();
			game->run("puyopuyo"_s, "Puyo Puyo"_s);		
			return 0;
		}
	private:
		test();
		virtual~test();

		virtual void dispose()override;

		error run(string app_name, string app_title);

		void init();
		void update(const core::time::step_timer&);
		void draw();
		void exit();

		virtual void on_initialize(objptr sender, platform::events::icreated_event_args_t args);
		virtual void on_update(objptr sender, platform::events::imsg_event_args_t args);
		virtual void on_finalize(objptr sender, platform::events::imsg_event_args_t args);

		virtual void on_pointer_pressed(objptr sender, platform::events::ipointer_event_args_t args);
		virtual void on_pointer_moved(objptr sender, platform::events::ipointer_event_args_t args);
		virtual void on_pointer_released(objptr sender, platform::events::ipointer_event_args_t args);
		virtual void on_pointer_entered(objptr sender, platform::events::ipointer_event_args_t args);
		virtual void on_pointer_leaved(objptr sender, platform::events::ipointer_event_args_t args);

		virtual void on_key_pressed(objptr sender, platform::events::ikey_event_args_t args);
		virtual void on_key_released(objptr sender, platform::events::ikey_event_args_t args);
		virtual void on_text_changed(objptr sender, platform::events::itext_change_event_args_t args);

		virtual void on_size_changed(objptr sender, platform::events::idisplay_info_event_args_t args);
		virtual void on_display_changed(objptr sender, platform::events::idisplay_info_event_args_t args);

		virtual void on_controller_connected(objptr, platform::events::icontroller_status_args_t args);
		virtual void on_controller_disconnected(objptr, platform::events::icontroller_status_args_t args);

		virtual void on_controller_analog_event(objptr, platform::events::icontroller_analog_input_args_t args);
		virtual void on_controller_digital_event(objptr, platform::events::icontroller_digital_input_args_t args);
		
		graphics::gl_context_t m_context;
		object3D_t m_object;

		float m_angle;
		camera_t m_camera;

		core::time::step_timer m_timer;
		platform::input::icontroller_t m_controller;
		platform::events::event_token_t m_analog_event_token;
		platform::events::event_token_t m_digital_event_token;

		
	};

}