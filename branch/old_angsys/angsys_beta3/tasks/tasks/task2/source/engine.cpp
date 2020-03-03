#include "pch.h"
#include "engine.h"

using namespace ang;
using namespace ang::engine;

framework::framework()
	: m_framerate(0)
	, m_fram_count(0)
{
	m_scene = new scenes::scene();
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
	m_timer.frames_per_second(60);
	m_timer.reset();
}

void framework::load_scene()
{
	m_effect_library = m_driver->create_effect_library();
	m_texture_loader = m_driver->create_texture_loader();

	core::files::input_text_file_t file = new core::files::input_text_file();
	file->open(L"res/fx_library.xml"_s);
	dom::xml::xml_document_t doc = dom::xml::xml_document::from_file(file);
	file->clear();
	m_effect_library->load_library(doc->xml_root_element());

	file->open(L"res/texture_apex.xml"_s);
	doc = dom::xml::xml_document::from_file(file);
	file->clear();
	m_texture_loader->load_library(doc->xml_root_element());

	file->open(L"res/scenes/scenes.xml"_s);
	doc = dom::xml::xml_document::from_file(file);
	file->clear();

	m_scene->load(m_driver, m_effect_library, m_texture_loader, doc->xml_root_element());

	m_driver->blend_mode(blend_mode::enable);
}

void framework::update(core::time::step_timer const& timer)
{
	if (m_fram_count >= 120)
	{
		printf("%.8f\n", timer.framerate());
		m_fram_count = 0;
	}

	m_fram_count++;
	m_scene->update(timer);
}

void framework::draw()
{
	m_driver->execute_on_thread_safe([&] 
	{
		m_scene->draw(m_driver, m_render_target);

		m_surface->swap_buffers(false);
	});
}

void framework::close_graphics()
{
	m_scene->clear();
	m_effect_library = null;
	m_texture_loader = null;
	m_scene = null;
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

void framework::on_pointer_released(objptr, platform::events::ipointer_event_args_t args)
{
	auto keyboard = platform::icore_app::current_app()->keyboard();

	//keyboard->visibility() ? keyboard->hide_touch_keyboard() : keyboard->show_touch_keyboard();
}



