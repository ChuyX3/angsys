#pragma once

#include <ang/core/timer.h>
#include <ang/graphics/graphics.h>


#include <scenes/scenes.h>

namespace ang
{
	namespace engine
	{
		using namespace graphics;

		ang_object(framework);

		class framework
			: public object
		{
			platform::icore_view_t m_view;
			idriver_t m_driver;
			isurface_t m_surface;
			iframe_buffer_t m_render_target;
			core::time::step_timer m_timer;

			effects::ieffect_library_t m_effect_library;
			textures::itexture_loader_t m_texture_loader;

			scenes::scene_t m_scene;

			long m_framerate;
			long m_fram_count;

		public:
			framework();

			ANG_DECLARE_INTERFACE();

		public:
			void init_graphics();
			void load_scene();
			void update(core::time::step_timer const&);
			void draw();
			void close_graphics();


		public: //event handlers
			void on_init_instance(objptr, platform::events::iapp_status_event_args_t args);
			void on_create_event(objptr sender, platform::events::icreated_event_args_t args);
			void on_update_event(objptr sender, platform::events::imsg_event_args_t args);
			void on_destroyed_event(objptr sender, platform::events::imsg_event_args_t args);

		private:
			virtual~framework();
		};

	}
}