#include "pch.h"
#include <ang/framework/framework.h>

using namespace ang;
using namespace ang::framework;

game::game()
{

}

game::~game()
{
}

graphics::idriver_t game::driver()const
{
	return m_driver;
}

graphics::isurface_t game::surface()const
{
	return m_surface;
}

graphics::ifactory_t game::factory()const
{
	return m_driver->get_factory();
}

graphics::resources::ilibrary_t game::library()const
{
	return null;
}

istate_manager_t game::state_manager()const
{
	return null;
}

error game::run(graphics::graph_driver_type_t type, string app_name, string app_title)
{
	m_driver_type = type;
	platform::app_t app = new platform::app();
	return app->run([&](platform::app_args_t & args)
	{
		args.fps = 60;
		args.name = app_name;
		args.title = app_title;

		args.app->created_event += new platform::events::created_event(this, &game::on_initialize);
		args.app->update_event += new platform::events::update_event(this, &game::on_update);
		args.app->destroyed_event += new platform::events::destroyed_event(this, &game::on_finalize);
		args.app->pointer_entered_event += new platform::events::pointer_entered_event(this, &game::on_pointer_entered);
		args.app->pointer_pressed_event += new platform::events::pointer_pressed_event(this, &game::on_pointer_pressed);
		args.app->pointer_moved_event += new platform::events::pointer_moved_event(this, &game::on_pointer_moved);
		args.app->pointer_released_event += new platform::events::pointer_released_event(this, &game::on_pointer_released);
		args.app->pointer_leaved_event += new platform::events::pointer_leaved_event(this, &game::on_pointer_leaved);
		args.app->key_pressed_event += new platform::events::key_pressed_event(this, &game::on_key_pressed);
		args.app->key_released_event += new platform::events::key_released_event(this, &game::on_key_released);
		args.app->size_event += new platform::events::display_size_change_event(this, &game::on_size_changed);
		args.app->size_event += new platform::events::display_invalidate_event(this, &game::on_display_changed);

	});
}

void game::update(double delta, double total)
{

}

void game::draw(graphics::scenes::itransform_t cam, graphics::iframe_buffer_t fbo)
{
	m_driver->bind_frame_buffer(fbo);
	m_driver->clear(graphics::colors::black);
	m_driver->bind_frame_buffer(null);
}

void game::on_initialize(objptr sender, platform::events::icreated_event_args_t args)
{
	m_driver = graphics::create_graphic_driver(m_driver_type, 0, args->core_view(), &m_surface);
}

void game::on_update(objptr sender, platform::events::imsg_event_args_t args)
{
	update(args->msg().wparam() / 1000.0, args->msg().lparam() / 1000.0);

	auto cams = m_surface->update();
	if (!cams.is_empty()) for (auto const& cam : cams)
		draw(cam.get<0>(), cam.get<1>());
	else
		draw(null, m_surface->frame_buffer());
	m_surface->swap_buffers(false);
}

void game::on_finalize(objptr sender, platform::events::imsg_event_args_t args)
{
	m_surface = null;
	m_driver = null;
}

void game::on_pointer_pressed(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void game::on_pointer_moved(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void game::on_pointer_released(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void game::on_pointer_entered(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void game::on_pointer_leaved(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void game::on_key_pressed(objptr sender, platform::events::ikey_event_args_t args)
{

}

void game::on_key_released(objptr sender, platform::events::ikey_event_args_t args)
{

}

void game::on_text_changed(objptr sender, platform::events::itext_change_event_args_t args)
{

}

void game::on_size_changed(objptr sender, platform::events::idisplay_info_event_args_t args)
{

}

void game::on_display_changed(objptr sender, platform::events::idisplay_info_event_args_t args)
{

}

