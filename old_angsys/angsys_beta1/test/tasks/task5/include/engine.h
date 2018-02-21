#pragma once

#include "scenes/scenes.h"


namespace d3d11
{
	using namespace ang;


	class engine;
	typedef object_wrapper<engine> engine_t;

	template<typename... Ts>
	void debug_print(cstr_t format, Ts... args)
	{
		string text = "";
		text->format(format, args...);
		OutputDebugStringA(text->cstr());
	}

	class engine final
		: public object
	{
	private:
		core::time::timer _timer;
		bool _is_running;
		core::async::cond_t cond;
		core::async::mutex_t mutex;
		
		platform::icore_view_t core_view;
		graphics::idriver_t driver;
		graphics::isurface_t surface;
		graphics::effects::ieffect_library_t effect_library;
		graphics::textures::itexture_loader_t texture_loader;

		graphics::drawing::idraw_context_t draw_context;
		graphics::drawing::ibrush_t solid_brush;

		core::async::iasync_t<void> async_render_operation;

		graphics::scenes::camera_t camera;
		graphics::scenes::scene_t scene;

	public:
		engine();
		ANG_DECLARE_INTERFACE();

		void on_create_event(objptr, platform::events::icreated_event_args_t);
		void on_destroy_event(objptr, platform::events::imsg_event_args_t);
		void on_render_operation(objptr, platform::events::icreated_event_args_t);
		void on_size_changed_event(objptr, platform::events::idisplay_info_event_args_t);
		void on_pointer_moved_event(objptr, platform::events::ipointer_event_args_t);
		void on_pointer_pressed_event(objptr, platform::events::ipointer_event_args_t);
		void on_pointer_released_event(objptr, platform::events::ipointer_event_args_t);

	private:
		bool init_driver();
		void close_driver();

		void init();
		void update(core::time::timer const&);
		void update_controller(float delta);
		void draw();
		void exit();



	private:
		virtual~engine();
	};
}
