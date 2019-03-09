// task1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "engine.h"

using namespace ang;


int main(int argc, char*argv[])
{
	core::files::ifile_system* fs = core::files::ifile_system::fs_instance();
	fs->push_path(L"../../../third_party"_s, core::files::file_system_priority::highest);

	array<string> args(array_view<char*>(argc, argv));
	platform::windows::appptr app = new platform::windows::app();
	engine::framework_t framework = new engine::framework();

	auto token = app->start_app_event += new platform::events::start_app_event([&](objptr, platform::events::iapp_status_event_args_t args)
	{
		app->main_wnd = new platform::windows::window();

		app->main_wnd->created_event += new platform::events::created_event(bind(framework, &engine::framework::on_create_event));
		app->main_wnd->update_event += new platform::events::update_event(bind(framework, &engine::framework::on_update_event));
		app->main_wnd->destroyed_event += new platform::events::destroyed_event(bind(framework, &engine::framework::on_destroyed_event));
		
		app->main_wnd->create(new platform::windows::wnd_create_args(
			null, "Tutorial: Task 2"_s
		));
	});
	app->run(args);
	app->start_app_event -= token;
	return 0;
}

