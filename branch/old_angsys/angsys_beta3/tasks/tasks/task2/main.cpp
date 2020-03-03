// task1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "engine.h"

using namespace ang;


int main(int argc, char*argv[])
{
	array<astring> args(array_view<char*>(argc, argv));
	engine::framework_t framework = new engine::framework();

	return platform::windows::app::run_async([&](platform::windows::app_t app) 
	{
		core::files::ifile_system* fs = core::files::ifile_system::instance();
		
		fs->push_path(L"./res"_s, core::files::path_access_type::read, "$(data)");
		fs->push_path(L"../../../third_party/res"_s, core::files::path_access_type::read, "$(data)");

		app->main_wnd = new platform::windows::window();

		app->main_wnd->initialize_event += new platform::events::initialize_event(bind(framework, &engine::framework::on_create_event));
		app->main_wnd->update_event += new platform::events::update_event(bind(framework, &engine::framework::on_update_event));
		app->main_wnd->destroyed_event += new platform::events::destroyed_event(bind(framework, &engine::framework::on_destroyed_event));
		app->main_wnd->mouse_lbutton_released_event += new platform::events::mouse_lbutton_up_event(bind(framework, &engine::framework::on_pointer_released));

		app->main_wnd->create(new platform::windows::wnd_create_args(
			null, "Tutorial: Task 2"_s
		));
	})->result();
}

