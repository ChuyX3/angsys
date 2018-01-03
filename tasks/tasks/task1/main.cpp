// task1.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ang/streams.hpp>
#include <ang/xml.hpp>
#include <ang/maths/vectors.hpp>
#include <ang/collections/map.hpp>

#include <type_traits>

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::windows;

double a1 = 0.0;
double a2 = 2.0 * M_PI / 3.0;
double a3 = 4.0 * M_PI / 3.0;

void on_start_app_event(objptr caller, events::iapp_status_event_args_t args);
void on_create_event(objptr caller, events::icreated_event_args_t args);
void on_draw_event(objptr caller, events::idraw_event_args_t);
void on_update_event(objptr caller, events::iupdate_event_args_t);
void on_close_event(objptr caller, events::imsg_event_args_t);

core::async::idispatcher_t dispatcher;


void on_start_app_event(objptr caller, platform::events::iapp_status_event_args_t args)
{
	window_t wnd = new window();

	wnd->created_event += new events::created_event(&on_create_event);
	wnd->draw_event += new events::draw_event(&on_draw_event);
	wnd->update_event += new events::update_event(&on_update_event);
	wnd->destroyed_event += new events::destroyed_event(&on_close_event);

	wnd->create(new wnd_create_args("test"_s, "test"_s));
	
	platform::windows::app::current_app()->set_main_wnd(wnd);
	wnd->show(showing_flag::show);
	wnd->update_wnd();

}

void on_create_event(objptr caller, events::icreated_event_args_t args)
{
	dispatcher = core::async::thread::create_dispatcher();
	dispatcher->resume();
}

void on_draw_event(objptr, events::idraw_event_args_t args)
{
	auto size = args->canvas_size();
	HDC hdc = (HDC)args->core_context()->get_core_context_handle();
	dword color = RGB(max(0,255 * sin(a1)), max(0,255 * sin(a2)), max(0,255 * sin(a3)));
	HBRUSH hbr = CreateSolidBrush(color);
	auto oldbr = SelectObject(hdc, hbr);

	Rectangle(hdc, 0, 0, (int)size.width, (int)size.height);
	SelectObject(hdc, oldbr);
	DeleteObject(hbr);
}

void on_update_event(objptr, events::iupdate_event_args_t args)
{
	auto timer = args->step_timer();
	a1 += timer->delta() / 1000000.0;
	a2 += timer->delta() / 1000000.0;
	a3 += timer->delta() / 1000000.0;
}

void on_close_event(objptr, events::imsg_event_args_t)
{
	dispatcher->stop();
	dispatcher->worker_thread()->wait(core::async::async_action_status::completed);
	dispatcher = null;
}


int main(int argc, char* argv[])
{
	platform::windows::app_t app = new platform::windows::app();
	app->start_app_event += new platform::events::start_app_event(&on_start_app_event);
	return app->run();

}

