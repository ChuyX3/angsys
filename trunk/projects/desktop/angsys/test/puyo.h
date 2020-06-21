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

		graphics::meshes::imesh_t m_mesh;
		graphics::effects::ishaders_t m_shaders;
		graphics::buffers::iindex_buffer_t m_indices;
		graphics::buffers::ivertex_buffer_t m_vertices;
	};

}