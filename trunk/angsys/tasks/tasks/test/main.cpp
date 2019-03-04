// test.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ang/streams.h>
#include <ang/maths/vectors.h>
#include <ang/dom/xml.h>
#include <ang/collections/list.h>

using namespace ang;
using namespace ang::dom;

platform::windows::appptr s_app = null;

void on_init_instance(objptr, platform::events::iapp_status_event_args_t args);
void on_create_event(objptr sender, platform::events::icreated_event_args_t args);
void on_update_event(objptr sender, platform::events::iupdate_event_args_t args);
void on_draw_event(objptr sender, platform::events::idraw_event_args_t args);
void on_destroyed_event(objptr sender, platform::events::imsg_event_args_t args);

int main(int argc, char* argv[])
{	
	array<string> args(array_view<char*>(argc,argv));
	s_app = new platform::windows::app();
	s_app->start_app_event += new platform::events::start_app_event(&on_init_instance);
	s_app->run(args);
	return 0;
}

void on_init_instance(objptr, platform::events::iapp_status_event_args_t args)
{
	platform::windows::wndptr wnd = new platform::windows::window();

	wnd->created_event += new platform::events::created_event(&on_create_event);
	wnd->update_event += new platform::events::update_event(&on_update_event);
	wnd->draw_event += new platform::events::draw_event(&on_draw_event);
	wnd->destroyed_event += new platform::events::destroyed_event(&on_destroyed_event);

	s_app->main_wnd(wnd);
	s_app->main_wnd()->create(new platform::windows::wnd_create_args(
		null, "Test"_s
	));
}


void on_create_event(objptr sender, platform::events::icreated_event_args_t args)
{

}

void on_update_event(objptr sender, platform::events::iupdate_event_args_t args)
{

}

void on_draw_event(objptr sender, platform::events::idraw_event_args_t args)
{

}

void on_destroyed_event(objptr sender, platform::events::imsg_event_args_t args)
{

}


