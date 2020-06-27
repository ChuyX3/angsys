#pragma once
#include <ang/framework/framework.h>

namespace ang
{
	ang_declare_object(puyo);

	class puyo
		: public implement<puyo
		, iid("ang::puyo")
		, framework::game>
	{
	public:
		static int main(array<string> args) {
			puyo_t game = new puyo();
			game->run(graphics::graph_driver_type::DirectX11, "puyopuyo"_s, "Puyo Puyo"_s);		
			return 0;
		}
	private:
		puyo();
		virtual~puyo();

		void on_initialize(objptr sender, platform::events::icreated_event_args_t args)override;
		void draw(graphics::scenes::itransform_t cam, graphics::iframe_buffer_t fbo)override;
		void on_size_changed(objptr sender, platform::events::idisplay_info_event_args_t args)override;

		void on_controller_connected(objptr, platform::events::icontroller_status_args_t args);
		void on_controller_disconnected(objptr, platform::events::icontroller_status_args_t args);

		void on_controller_analog_event(objptr, platform::events::icontroller_analog_input_args_t args);
		void on_controller_digital_event(objptr, platform::events::icontroller_digital_input_args_t args);
		

		graphics::meshes::imesh_t m_mesh;
		graphics::effects::ishaders_t m_shaders;
		graphics::buffers::iindex_buffer_t m_indices;
		graphics::buffers::ivertex_buffer_t m_vertices;
		platform::input::icontroller_t m_controller;
		platform::events::event_token_t m_analog_event_token;
		platform::events::event_token_t m_digital_event_token;
	};

}