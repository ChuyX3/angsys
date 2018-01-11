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
		bool _is_running;
		core::async::cond_ptr_t cond;
		core::async::mutex_ptr_t mutex;

		platform::icore_view_t core_view;
		graphics::idriver_t driver;
		graphics::isurface_t surface;

		graphics::effects::ishaders_t shader;
		graphics::buffers::iindex_buffer_t indices;
		graphics::buffers::ivertex_buffer_t vertices;

		graphics::effects::ieffect_library_t effect_library;
		graphics::textures::itexture_loader_t texture_loader;

		graphics::scenes::camera_t camera;

		shared_ptr<core::time::step_timer> timer;
		core::async::idispatcher_t dispatcher;

	public:
		engine();
		ANG_DECLARE_INTERFACE();

		void on_create_event(objptr, platform::events::icreated_event_args_t);
		void on_destroy_event(objptr, platform::events::imsg_event_args_t);
		void on_size_changed_event(objptr, platform::events::idisplay_info_event_args_t);
		void on_pointer_moved_event(objptr, platform::events::ipointer_event_args_t);
		void on_pointer_pressed_event(objptr, platform::events::ipointer_event_args_t);
		void on_pointer_released_event(objptr, platform::events::ipointer_event_args_t);

	private:
		bool init_driver();
		void close_driver();

		void init();
		void update(shared_ptr<core::time::step_timer>);
		void update_controller(float delta);
		void draw();
		void exit();



	private:
		virtual~engine();
	};
}
