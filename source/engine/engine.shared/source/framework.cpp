#include "pch.h"
#include "framework.h"

using namespace ang;
using namespace ang::graphics;


framework::framework(platform::icore_view_t view)
{
	if (!initialize(view))
		throw exception_t(except_code::unknown);
}

framework::~framework()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::framework, object);

bool framework::initialize(platform::icore_view_t view)
{
	if (view.is_empty())
		return false;

	view->get_listener()->listen_to(new platform::events::destroyed_event(this, &framework::on_destroyed));
	view->get_listener()->listen_to(new platform::events::display_size_change_event(this, &framework::on_size_changed));
	view->get_listener()->listen_to(new platform::events::pointer_moved_event(this, &framework::on_pointer_moved));
	view->get_listener()->listen_to(new platform::events::pointer_pressed_event(this, &framework::on_pointer_pressed));
	view->get_listener()->listen_to(new platform::events::pointer_released_event(this, &framework::on_pointer_released));

	_driver = create_graphic_driver(graph_driver_type::DirectX11, view, &_surface);

	on_start_render_operation();
}

void framework::on_start_render_operation()
{
	async_render_operation = core::async::async_task<void>::run_async(
		[this](core::async::iasync<void>* async, var_args_t args)
	{
		core::time::timer timer;
		timer.reset();
		on_begin_rendering_event();
		while (async->status().is_active(core::async::async_action_status::running))
		{
			timer.update();
			on_update_event(timer);
			on_draw_event();
		}
		on_end_rendering_event();
	});
	async_render_operation->wait(core::async::async_action_status::running, -1);
}

bool framework::terminate()
{
	if (!async_render_operation.is_empty())
	{
		async_render_operation->cancel();
		async_render_operation->wait(core::async::async_action_status::completed, -1);
	}
	async_render_operation = null;
	return true;
}

void framework::on_destroyed(objptr, platform::events::imsg_event_args_t)
{
	framework::terminate();
}

void framework::on_begin_rendering_event()
{
	begin_rendering_event(this, _driver);
}

void framework::on_end_rendering_event()
{
	end_rendering_event(this, _driver);
}

void framework::on_draw_event()
{
	draw_event(this, _driver);
}

void framework::on_update_event(core::time::timer const& timer)
{
	update_event(this, timer);
}

void framework::on_size_changed(objptr, platform::events::idisplay_info_event_args_t)
{

}

void framework::on_pointer_moved(objptr, platform::events::ipointer_event_args_t)
{

}

void framework::on_pointer_pressed(objptr, platform::events::ipointer_event_args_t)
{

}

void framework::on_pointer_released(objptr, platform::events::ipointer_event_args_t)
{

}