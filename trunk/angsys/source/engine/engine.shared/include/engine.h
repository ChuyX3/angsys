#pragma once

#include <ang/core/timer.h>
#include <ang/graphics/graphics.h>

namespace ang
{
	namespace engine
	{
		using namespace graphics;
		ang_object(framework);
	}

	namespace dom
	{
		namespace xml
		{
			template<> maths::float2 xml_text::xml_as<maths::float2>()const;
			template<> maths::float3 xml_text::xml_as<maths::float3>()const;
			template<> maths::float4 xml_text::xml_as<maths::float4>()const;
			template<> maths::float4 xml_text::xml_as<maths::float4>()const;
		}
	}

}


#include "resources/resources.h"
#include "scenes/scenes.h"

namespace ang
{
	namespace engine
	{
		

		class framework
			: public object
		{
			platform::icore_view_t m_view;
			idriver_t m_driver;
			isurface_t m_surface;
			//iframe_buffer_t m_render_target;
			core::time::step_timer m_timer;

			resources::resource_manager_t m_rm;

			scenes::scene_t m_scene;

			long m_framerate;
			long m_fram_count;

			//Windows::Graphics::Holographic::HolographicSpace^ m_holographicSpace;

		public:
			framework();

			ANG_DECLARE_INTERFACE();

		public:
			void init_graphics();
			void init_holographics();
			void load_scene();
			void update(core::time::step_timer const&);
			void draw();
			void close_graphics();


		public: //event handlers
			void on_create_event(objptr sender, platform::events::icreated_event_args_t args);
			void on_update_event(objptr sender, platform::events::imsg_event_args_t args);
			void on_destroyed_event(objptr sender, platform::events::imsg_event_args_t args);

			void on_pointer_released(objptr, platform::events::ipointer_event_args_t args);

		private:
			virtual~framework();
		};

	}
}