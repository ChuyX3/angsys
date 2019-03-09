#include "pch.h"
#include "engine.h"

using namespace ang;
using namespace ang::engine;

framework::framework()
	: m_framerate(0)
	, m_fram_count(0)
{

}

framework::~framework()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::engine::framework);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::engine::framework, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::engine::framework, object);

void framework::init_graphics()
{
	m_driver = graphics::create_graphic_driver(graph_driver_type::DirectX11);
	m_surface = m_driver->create_surface(m_view);
	m_render_target = m_surface->frame_buffer();
	m_timer.fixed_time_step(true);
	m_timer.reset();
}

void framework::load_scene()
{

}

void framework::update(core::time::step_timer const& timer)
{
	if (m_fram_count >= 120)
	{
		printf("%.8f\n", timer.framerate());
		m_fram_count = 0;
	}

	m_fram_count++;
}

void framework::draw()
{
	m_driver->bind_frame_buffer(m_render_target);
	m_driver->clear(colors::blue);

	m_surface->swap_buffers(false);

}

void framework::close_graphics()
{
	m_render_target = null;
	m_surface = null;
	m_driver = null;
}

void framework::on_create_event(objptr sender, platform::events::icreated_event_args_t args)
{
	m_view = args->core_view();
	init_graphics();
	load_scene();
}

void framework::on_update_event(objptr sender, platform::events::imsg_event_args_t args)
{
	m_timer.tick([this]() 
	{
		update(m_timer);
		draw();
	});
}

void framework::on_destroyed_event(objptr sender, platform::events::imsg_event_args_t args)
{
	close_graphics();
}
