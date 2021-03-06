#include "pch.h"
#include "engine.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

namespace d3d11
{
	using namespace ang;
	using namespace ang::platform;
	using namespace ang::platform::windows;

	class main_app;
	typedef object_wrapper<main_app> main_app_t;

	class main_wnd : public window
	{
	public:
		main_wnd();

		ANG_DECLARE_INTERFACE();

		virtual void window_proc(events::message_t msg)override {
			if (msg->msg() == events::win_msg_enum::erase_bkgnd
				|| msg->msg() == events::win_msg_enum::draw)
			{
				graphics::paint_dc_t dc = new graphics::paint_dc(this);
			}
			else
			{
				return window::window_proc(msg.get());
			}
		}

	private:
		virtual~main_wnd();
	};

	class main_app : public app
	{
		engine_t _engine;

	public:
		main_app();

		ANG_DECLARE_INTERFACE();

		bool init_app(array<string> cmdl)override;
		bool exit_app()override;

	private:
		virtual~main_app();
	};

}

int main(int argc, char* argv[])
{
	d3d11::main_app_t app = new d3d11::main_app();
	return (int)app->run();
}


d3d11::main_wnd::main_wnd()
{
}

d3d11::main_wnd::~main_wnd()
{

}


ANG_IMPLEMENT_BASIC_INTERFACE(d3d11::main_wnd, window);


d3d11::main_app::main_app()
{
	_engine = new engine();
}

d3d11::main_app::~main_app()
{
	_engine = null;
}

ANG_IMPLEMENT_BASIC_INTERFACE(d3d11::main_app, app);

bool d3d11::main_app::init_app(ang::array<ang::string> cmdl)
{
	app::init_app(cmdl);

	window_t wnd = new d3d11::main_wnd();

	wnd->created_event += new events::created_event(_engine, &engine::on_create_event);
	wnd->destroyed_event += new events::destroyed_event(_engine, &engine::on_destroy_event);
	wnd->size_event += new events::display_size_change_event(_engine, &engine::on_size_changed_event);
	wnd->mouse_moved_event += new events::pointer_moved_event(_engine, &engine::on_pointer_moved_event);
	wnd->mouse_button_pressed_event += new events::pointer_moved_event(_engine, &engine::on_pointer_pressed_event);
	wnd->mouse_button_released_event += new events::pointer_moved_event(_engine, &engine::on_pointer_released_event);

	if (!wnd->create(new wnd_create_args(
		"d3d11_core_view_wnd", "d3d11 Task3", { 100,100, 1124, 900 }
	)))
		return false;
	set_main_wnd(wnd);

	wnd->update_wnd();
	wnd->show(showing_flag::show);
	return true;
}

bool d3d11::main_app::exit_app()
{
	return app::exit_app();
}