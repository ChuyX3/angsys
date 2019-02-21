#include "pch.h"
#include "engine.h"
#include "scenes/model_loader.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

using namespace d3d11;

double a1 = 0.0;
double a2 = 2.0 * M_PI / 3.0;
double a3 = 4.0 * M_PI / 3.0;

maths::float4x4 world_matrix;
maths::float4x4 view_matrix;
maths::float4x4 projection_matrix;

struct vertex_t
{
	maths::float3 position;
	maths::float4 color;
};

void engine::init()
{
	effect_library = driver->create_effect_library();
	texture_loader = driver->create_texture_loader();

	core::files::input_text_file_t file = new core::files::input_text_file();
	file->open(L"resources/fx_library.xml"_s);
	xml::xml_document_t doc = xml::xml_document::from_file(file);
	file->close();
	effect_library->load_library(doc->xml_root_element());

	file->open(L"resources/texture_apex.xml"_s);
	doc = xml::xml_document::from_file(file);
	file->close();
	texture_loader->load_library(doc->xml_root_element());


	auto size = core_view->get_core_view_size();

	file->open(L"resources/scenes/scenes.xml"_s);
	doc = xml::xml_document::from_file(file);
	file->close();

	scene->load(driver, effect_library, texture_loader, doc->xml_root_element());

	driver->blend_mode(graphics::blend_mode::enable);
}

void engine::update(shared_ptr<core::time::step_timer> timer)
{
	a1 += timer->delta() / 1000000.0;
	a2 += timer->delta() / 1000000.0;
	a3 += timer->delta() / 1000000.0;
	scene->update(timer->delta(), timer->total());
	update_controller(timer->delta());
}

void engine::draw()
{
	//maths::float4 color = { (float)max(0, sin(a1)), (float)max(0, sin(a2)), (float)max(0, sin(a3)), 1.0f };

	scene->draw(driver, surface->frame_buffer());

	surface->swap_buffers();
}

void engine::exit()
{
	driver->bind_shaders(null);
	driver->bind_frame_buffer(null);
	scene = null;
	texture_loader = null;
	effect_library = null;

}

void engine::on_size_changed_event(objptr sender, platform::events::idisplay_info_event_args_t args)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = mutex;
}

void engine::on_pointer_moved_event(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void engine::on_pointer_pressed_event(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void engine::on_pointer_released_event(objptr sender, platform::events::ipointer_event_args_t args)
{


}

//////////////////////////////////////////////////////////

engine::engine()
{
	_is_running = false;
	cond = make_shared<core::async::cond>();
	mutex = make_shared<core::async::mutex>();
	timer = make_shared<core::time::step_timer>();
	//camera = new graphics::scenes::camera();

	scene = new graphics::scenes::scene();
	camera = scene->camera();
}

engine::~engine()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(d3d11::engine, ang::object);

void engine::on_create_event(objptr caller, platform::events::icreated_event_args_t args)
{
	core_view = args->core_view();
	dispatcher = core::async::thread::create_dispatcher();

	dispatcher->start_event += [this](core::async::idispatcher_t dispatcher) {
		core::async::scope_locker<core::async::mutex_ptr_t> lock = mutex;
		if (!init_driver())
		{
			dispatcher->stop();
			return;
		}
		timer->reset();
		_is_running = true;
		init();
	};

	dispatcher->update_event += [this](core::async::idispatcher_t) {
		timer->update();
		update(timer);
		draw();
	};

	dispatcher->end_event += [this](core::async::idispatcher_t) {
		exit();
		_is_running = false;
		close_driver();
	};

	dispatcher->resume();
}

void engine::on_destroy_event(objptr, platform::events::imsg_event_args_t)
{
	auto thread = dispatcher->worker_thread();
	dispatcher->stop();
	thread->wait(core::async::async_action_status::completed);
	dispatcher = null;
}

bool engine::init_driver()
{
	driver = graphics::create_graphic_driver(graphics::graph_driver_type::DirectX11);
	graphics::size<float> size = core_view->get_core_view_size();
	surface = driver->create_surface(core_view);

	return true;
}

void engine::close_driver()
{
	_is_running = false;
	surface = null;
	driver = null;
}


#define INPUT_DEADZONE 10000

void engine::update_controller(float delta)
{
	delta /= 1000.0f;

	if (camera.is_empty())
		return;

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{

		float LX = state.Gamepad.sThumbLX;
		float LY = state.Gamepad.sThumbLY;

		//determine how far the controller is pushed
		float magnitude = sqrt(LX*LX + LY * LY);

		//determine the direction the controller is pushed
		float normalizedLX = LX / magnitude;
		float normalizedLY = LY / magnitude;

		float normalizedMagnitude = 0;

		//check if the controller is outside a circular dead zone
		if (magnitude > INPUT_DEADZONE)
		{
			//clip the magnitude at its expected maximum value
			if (magnitude > 32767) magnitude = 32767;

			//adjust magnitude relative to the end of the dead zone
			magnitude -= INPUT_DEADZONE;

			//optionally normalize the magnitude with respect to its expected range
			//giving a magnitude value of 0.0 to 1.0
			normalizedMagnitude = magnitude / (32767 - INPUT_DEADZONE);
		}
		else //if the controller is in the deadzone zero out the magnitude
		{
			magnitude = 0.0;
			normalizedMagnitude = 0.0;
		}


		if (normalizedMagnitude > 0)
		{
			camera->rotate_left(delta / 3.0f * normalizedMagnitude * normalizedLX);
			camera->rotate_up(-delta / 3.0f * normalizedMagnitude * normalizedLY);
		}

		float speed = (25.5f + float(state.Gamepad.bRightTrigger))
			/ (25.5f + float(state.Gamepad.bLeftTrigger));

		if ((state.Gamepad.wButtons & 0x4000) == 0x4000)
		{
			camera->move_forward(delta * speed);
		}

		if ((state.Gamepad.wButtons & 0x1000) == 0x1000)
		{
			camera->move_backward(delta * speed);
		}

		if ((state.Gamepad.wButtons & 0x100) == 0x100)
		{
			camera->move_left(delta * speed);
		}

		if ((state.Gamepad.wButtons & 0x200) == 0x200)
		{
			camera->move_right(delta * speed);
		}
	}
}