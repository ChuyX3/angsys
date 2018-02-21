#include "pch.h"
#include "engine.h"

using namespace d3d11;

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace d3d11;

//////////////////////////////////////////////////////////

void engine::init()
{
	core::files::ifile_system* fs = core::files::ifile_system::get_file_system();
	fs->register_paths(L"../../../third_party/"_s);

	effect_library = driver->create_effect_library();
	texture_loader = driver->create_texture_loader();
	
	core::files::input_text_file_t file = new core::files::input_text_file();
	file->open(L"resources/fx_library.xml"_s);
	xml::xml_document_t doc = new xml::xml_document(file);
	file->close();
	effect_library->load_library(doc->xml_root().get());

	file->open(L"resources/texture_apex.xml"_s);
	doc = new xml::xml_document(file);
	file->close();
	texture_loader->load_library(doc->xml_root().get());

	file->open(L"resources/scenes/scenes.xml"_s);
	doc = new xml::xml_document(file);
	file->close();
	scene = new graphics::scenes::scene();
	scene->load(driver.get(), effect_library, texture_loader, doc->xml_root().get());
	camera = scene->camera();

	graphics::drawing::gradient_info::stop_color_info_t stop_colors[] = {
		{ 0, graphics::colors::yellow },
		{ 0.8f, graphics::colors::antique_white },
		{ 1, graphics::colors::antique_white },
	};

	solid_brush = draw_context->create_linear_gradient_brush({
		{ 0,1 },
		{ 0,-1 },
		stop_colors
	});
}

void engine::update(core::time::timer const& timer)
{
	update_controller(timer.total());
	scene->update(timer.total(), timer.delta());
}


void engine::draw()
{
	auto frame_buffer = surface->frame_buffer();
	auto size = frame_buffer->dimentions();
	scene->draw(driver.get(), frame_buffer);

	draw_context->begin_draw(frame_buffer);
	draw_context->draw_rect(solid_brush, { graphics::point<float>(0,0), graphics::size<float>(size.width,300) });
	draw_context->end_draw();

	surface->swap_buffers();
}

void engine::exit()
{
	solid_brush = null;
	driver->bind_shaders(null);
	driver->bind_frame_buffer(null);
	surface = null;
	effect_library = null;
	driver = null;

}

void engine::on_size_changed_event(objptr sender, platform::events::idisplay_info_event_args_t args)
{
	auto info = args->display_info();

	foundation::size<float> size = {
		info.display_resolution->width * info.display_scale_factor->width ,
		info.display_resolution->height * info.display_scale_factor->height };
	size.height = max(size.height, 10.0f);
	size.width = max(size.width, 10.0f);		
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
	cond = new core::async::cond();
	mutex = new core::async::mutex();
}

engine::~engine()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(d3d11::engine, ang::object);

void engine::on_create_event(objptr caller, platform::events::icreated_event_args_t args)
{
	core_view = args->core_view();
	on_render_operation(caller, args);
}

void engine::on_destroy_event(objptr, platform::events::imsg_event_args_t)
{
	if (!async_render_operation.is_empty())
	{
		async_render_operation->cancel();
		async_render_operation->wait(core::async::async_action_status::completed, -1);
	}
	async_render_operation = null;
}

void engine::on_render_operation(objptr, platform::events::icreated_event_args_t)
{
	
	async_render_operation = core::async::async_task<void>::run_async(
		[this](core::async::iasync<void>* async, var_args_t args)
	{
		{//scope
			core::async::scope_locker lock = mutex;
			if (!init_driver())
				return;
			_is_running = true;
		}

		init();
		_timer.reset();
		while (async->status().is_active(core::async::async_action_status::running))
		{		
			_timer.update();
			update(_timer);
			draw();
		}
		exit();

		{//scope
			core::async::scope_locker lock = mutex;
			close_driver();
		}
	});
	async_render_operation->wait(core::async::async_action_status::running, -1);
}


bool engine::init_driver()
{
	driver = graphics::create_graphic_driver(graphics::graph_driver_type::DirectX11);
	foundation::size<float> size = core_view->get_core_view_size();
	surface = driver->create_surface(core_view);

	draw_context = graphics::drawing::create_drawing_context(driver);

	return true;
}

void engine::close_driver()
{
	_is_running = false;
	draw_context = null;
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
		float magnitude = sqrt(LX*LX + LY*LY);

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