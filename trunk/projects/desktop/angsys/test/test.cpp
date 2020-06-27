#include "pch.h"
#include "test.h"

using namespace ang;

test::test()
{
	m_camera = new camera();
}

test::~test()
{

}

void test::dispose()
{
	m_camera = null;
}

static uint vboId = 0;

void test::init()
{
	graphics::color_t color = graphics::colors::dark_blue;
	float _color[] = {
		(float)color.components.red / 255.0f,
		(float)color.components.green / 255.0f ,
		(float)color.components.blue / 255.0f ,
		(float)color.components.alpha / 255.0f
	};

	glClearColor(_color[0], _color[1], _color[2], _color[3]);
	
	m_object = new object3D();
	m_object->load(m_context, "shaders/model_vs.glsl"_s, "shaders/model_fs.glsl"_s, "models/woman1.nfg"_s, ""_s);
}

void test::update(const core::time::step_timer& timer)
{
	m_angle = timer.total_time() / 1000000.0f;
	m_object->update(timer);
}

void test::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_object->draw(m_context, m_camera);
}

void test::exit()
{
	m_object = null;;
	m_context = null;
}

error test::run(string app_name, string app_title)
{
	platform::app_t app = new platform::app();
	return app->run([&](platform::app_args_t& args)
	{
		args.fps = 60;
		args.name = app_name;
		args.title = app_title;

		args.app->created_event += new platform::events::created_event(this, &test::on_initialize);
		args.app->update_event += new platform::events::update_event(this, &test::on_update);
		args.app->destroyed_event += new platform::events::destroyed_event(this, &test::on_finalize);
		args.app->pointer_entered_event += new platform::events::pointer_entered_event(this, &test::on_pointer_entered);
		args.app->pointer_pressed_event += new platform::events::pointer_pressed_event(this, &test::on_pointer_pressed);
		args.app->pointer_moved_event += new platform::events::pointer_moved_event(this, &test::on_pointer_moved);
		args.app->pointer_released_event += new platform::events::pointer_released_event(this, &test::on_pointer_released);
		args.app->pointer_leaved_event += new platform::events::pointer_leaved_event(this, &test::on_pointer_leaved);
		args.app->key_pressed_event += new platform::events::key_pressed_event(this, &test::on_key_pressed);
		args.app->key_released_event += new platform::events::key_released_event(this, &test::on_key_released);
		args.app->size_event += new platform::events::display_size_change_event(this, &test::on_size_changed);
		args.app->size_event += new platform::events::display_invalidate_event(this, &test::on_display_changed);

		platform::input::icontroller_manager::instance()->controller_connected_event +=
			new platform::events::controller_status_change_event(this, &test::on_controller_connected);
		platform::input::icontroller_manager::instance()->controller_disconnected_event +=
			new platform::events::controller_status_change_event(this, &test::on_controller_disconnected);

	});
	m_timer.fixed_time_step(false);
}

void test::on_initialize(objptr sender, platform::events::icreated_event_args_t args)
{
	m_context = new graphics::gl_context(args->core_view());
	m_camera->load(args->core_view()->core_view_size(), { 0, 0, 5 }, { 0, 0, 0 });

	core::async::scope_locker<graphics::gl_context_t> lock = m_context;
	init();
}

void test::on_update(objptr sender, platform::events::imsg_event_args_t args)
{
	m_timer.tick([&]()
	{
		update(m_timer);

		core::async::scope_locker<graphics::gl_context_t> lock = m_context;
		draw();
		m_context->present();
	});
}

void test::on_finalize(objptr sender, platform::events::imsg_event_args_t args)
{
	exit();
}

void test::on_pointer_pressed(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void test::on_pointer_moved(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void test::on_pointer_released(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void test::on_pointer_entered(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void test::on_pointer_leaved(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void test::on_key_pressed(objptr sender, platform::events::ikey_event_args_t args)
{

}

void test::on_key_released(objptr sender, platform::events::ikey_event_args_t args)
{

}

void test::on_text_changed(objptr sender, platform::events::itext_change_event_args_t args)
{

}

void test::on_size_changed(objptr sender, platform::events::idisplay_info_event_args_t args)
{

}

void test::on_display_changed(objptr sender, platform::events::idisplay_info_event_args_t args)
{

}

void test::on_controller_connected(objptr, platform::events::icontroller_status_args_t args)
{
	if (m_controller.is_empty()) {
		m_controller = args->controller();
		m_analog_event_token = m_controller->analog_input_change_event += 
			new platform::events::controller_analog_change_event(this, &test::on_controller_analog_event);
		m_digital_event_token = m_controller->digital_button_change_event +=
			new platform::events::controller_button_change_event(this, &test::on_controller_digital_event);
	}
}

void test::on_controller_disconnected(objptr, platform::events::icontroller_status_args_t args)
{
	if (!m_controller.is_empty() && m_controller->controller_id() == args->controller_id()) {
		m_controller->analog_input_change_event -= m_analog_event_token;
		m_controller->digital_button_change_event -= m_digital_event_token;
		m_controller = null;
	}
}

void test::on_controller_analog_event(objptr, platform::events::icontroller_analog_input_args_t args)
{
	auto button = args->button();
	graphics::point<float> value;
	switch (button)
	{
	case platform::input::controller_button::left_thumb:
		value = args->state().value;
		m_camera->move_x(value.x * m_timer.elapsed_time() / 1000000.0f);
		m_camera->move_z(value.y * m_timer.elapsed_time() / 1000000.0f);
		break;
	case platform::input::controller_button::right_thumb:
		value = args->state().value;
		m_camera->rotate_yaw(value.x * m_timer.elapsed_time() / 1000000.0f);
		m_camera->rotate_pitch(value.y * m_timer.elapsed_time() / 1000000.0f);
		break;
	case platform::input::controller_button::triggers:
		value = args->state().value;
		m_camera->rotate_roll((value.x - value.y) * m_timer.elapsed_time() / 1000000.0f);
		break;
	case platform::input::controller_button::right_trigger:
		value = args->state().value;
		m_camera->rotate_roll(value.x * m_timer.elapsed_time() / 1000000.0f);
		break;
	case platform::input::controller_button::left_trigger:
		value = args->state().value;
		m_camera->rotate_roll(- value.y * m_timer.elapsed_time() / 1000000.0f);
		break;
	default:
		
		break;
	}
}

void test::on_controller_digital_event(objptr, platform::events::icontroller_digital_input_args_t args)
{
	switch (args->button())
	{
	case platform::input::controller_button::A:
		break;

	case platform::input::controller_button::B:
		break;

	case platform::input::controller_button::X:
		break;

	case platform::input::controller_button::Y:
		break;
	default:
		break;
	}

}