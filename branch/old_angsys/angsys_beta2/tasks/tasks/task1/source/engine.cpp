#include "pch.h"
#include "engine.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

using namespace d3d11;

double a1 = 0.0;
double a2 = 2.0 * M_PI / 3.0;
double a3 = 4.0 * M_PI / 3.0;


void engine::init()
{

}

void engine::update(shared_ptr<core::time::step_timer> timer)
{
	a1 += timer->delta() / 1000000.0;
	a2 += timer->delta() / 1000000.0;
	a3 += timer->delta() / 1000000.0;
}

void engine::draw()
{
	maths::float4 color = { (float)max(0, sin(a1)), (float)max(0, sin(a2)), (float)max(0, sin(a3)), 1.0f };

	driver->bind_frame_buffer(surface->frame_buffer());
	driver->clear(color);

	surface->swap_buffers();
}

void engine::exit()
{
	
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
