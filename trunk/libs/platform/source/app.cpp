#include "pch.h"
#include <ang/platform/platform.h>

#include "core_app.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;
using namespace ang::platform::display;
using namespace ang::platform::input;

error app::run(function<void(app_args_t&)> setup)
{
#ifdef WINDOWS_PLATFORM
	windows::core_app_t coreapp = new windows::core_app();
#endif // WINDOWS_PLATFORM
	app_args_t args;
	args.app = this;
	args.name = "agsys_app";
	args.title = "angsys";
	args.fps = 60;
	return coreapp->run([&](icore_app_t app)->error
	{	
		if (!init_app())
			return error_code::operation_canceled;
		setup(args);
		return error_code::success;
	}, args);
}

app::app()
	: m_created_event(this)
	, m_destroyed_event(this)
	, m_draw_event(this)
	, m_update_event(this)
	, m_orientation_event(this)
	, m_activate_event(this)
	, m_size_event(this)
	, m_display_change_event(this)
	, m_key_pressed_event(this)
	, m_key_released_event(this)
	, m_text_changed_event(this)
	, m_pointer_entered_event(this)
	, m_pointer_leaved_event(this)
	, m_pointer_moved_event(this)
	, m_pointer_pressed_event(this)
	, m_pointer_released_event(this)
	, m_mouse_moved_event(this)
	, m_mouse_lbutton_pressed_event(this)
	, m_mouse_lbutton_released_event(this)
	, m_mouse_rbutton_pressed_event(this)
	, m_mouse_rbutton_released_event(this)
{

}

app::~app()
{

}


ang_platform_implement_event_handler(app, created_event);
ang_platform_implement_event_handler(app, destroyed_event);

ang_platform_implement_event_handler(app, draw_event);
ang_platform_implement_event_handler(app, update_event);

ang_platform_implement_event_handler(app, orientation_event);
ang_platform_implement_event_handler(app, activate_event);
ang_platform_implement_event_handler(app, size_event);
ang_platform_implement_event_handler(app, display_change_event);

ang_platform_implement_event_handler(app, key_pressed_event);
ang_platform_implement_event_handler(app, key_released_event);
ang_platform_implement_event_handler(app, text_changed_event);

ang_platform_implement_event_handler(app, pointer_entered_event);
ang_platform_implement_event_handler(app, pointer_pressed_event);
ang_platform_implement_event_handler(app, pointer_moved_event);
ang_platform_implement_event_handler(app, pointer_released_event);
ang_platform_implement_event_handler(app, pointer_leaved_event);

ang_platform_implement_event_handler(app, mouse_moved_event);
ang_platform_implement_event_handler(app, mouse_lbutton_pressed_event);
ang_platform_implement_event_handler(app, mouse_rbutton_pressed_event);
ang_platform_implement_event_handler(app, mouse_lbutton_released_event);
ang_platform_implement_event_handler(app, mouse_rbutton_released_event);

icore_app* app::get_core_app_property(base_property const* prop)
{
	return icore_app::current_app();
}

icore_view* app::get_core_view_property(base_property const* prop)
{
	return icore_app::current_app()->core_view();
}

imessage_listener* app::get_dispatcher_property(base_property const* prop)
{
	return icore_app::current_app()->core_view()->dispatcher();
}


bool app::init_app()
{
	icore_view_t view = core_view;
	view->dispatcher()->notify(new events::created_event(this, &app::on_created_event));
	view->dispatcher()->notify(new events::destroyed_event(this, &app::on_destroyed_event));

	view->dispatcher()->notify(new events::draw_event(this, &app::on_draw_event));
	view->dispatcher()->notify(new events::update_event(this, &app::on_update_event));

	view->dispatcher()->notify(new events::display_orientation_change_event(this, &app::on_orientation_event));
	view->dispatcher()->notify(new events::activate_event(this, &app::on_activate_event));
	view->dispatcher()->notify(new events::display_size_change_event(this, &app::on_size_event));
	view->dispatcher()->notify(new events::display_invalidate_event(this, &app::on_display_change_event));

	view->dispatcher()->notify(new events::key_pressed_event(this, &app::on_key_pressed_event));
	view->dispatcher()->notify(new events::key_released_event(this, &app::on_key_released_event));
	view->dispatcher()->notify(new events::text_changed_event(this, &app::on_text_changed_event));
	
	view->dispatcher()->notify(new events::pointer_entered_event(this, &app::on_pointer_entered_event));
	view->dispatcher()->notify(new events::pointer_pressed_event(this, &app::on_pointer_pressed_event));
	view->dispatcher()->notify(new events::pointer_moved_event(this, &app::on_pointer_moved_event));
	view->dispatcher()->notify(new events::pointer_released_event(this, &app::on_pointer_released_event));
	view->dispatcher()->notify(new events::pointer_leaved_event(this, &app::on_pointer_leaved_event));

	view->dispatcher()->notify(new events::mouse_moved_event(this, &app::on_mouse_moved_event));
	view->dispatcher()->notify(new events::mouse_lbutton_down_event(this, &app::on_mouse_lbutton_pressed_event));
	view->dispatcher()->notify(new events::mouse_rbutton_down_event(this, &app::on_mouse_rbutton_pressed_event));
	view->dispatcher()->notify(new events::mouse_lbutton_up_event(this, &app::on_mouse_lbutton_released_event));
	view->dispatcher()->notify(new events::mouse_rbutton_up_event(this, &app::on_mouse_rbutton_released_event));

	return true;
}

void app::update_app()
{

}

bool app::exit_app()
{
	return true;
}

void app::on_created_event(objptr, events::icreated_event_args_t args)
{	 
	if(m_created_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_destroyed_event(objptr, events::imsg_event_args_t args)
{
	if (m_destroyed_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_draw_event(objptr, events::idraw_event_args_t args)
{
	if (m_draw_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_update_event(objptr, events::imsg_event_args_t args)
{
	if (m_update_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_orientation_event(objptr, events::idisplay_info_event_args_t args)
{
	if (m_orientation_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_activate_event(objptr, events::iactivate_event_args_t args)
{
	if (m_activate_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_size_event(objptr, events::idisplay_info_event_args_t args)
{
	if (m_size_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_display_change_event(objptr, events::idisplay_info_event_args_t args)
{
	if (m_display_change_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_key_pressed_event(objptr, events::ikey_event_args_t args)
{
	if (m_key_pressed_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_key_released_event(objptr, events::ikey_event_args_t args)
{
	if (m_key_released_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_text_changed_event(objptr, events::itext_change_event_args_t args)
{
	if (m_text_changed_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_pointer_entered_event(objptr, events::ipointer_event_args_t args)
{
	if (m_pointer_entered_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_pointer_pressed_event(objptr, events::ipointer_event_args_t args)
{
	if (m_pointer_pressed_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_pointer_moved_event(objptr, events::ipointer_event_args_t args)
{
	if (m_pointer_moved_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_pointer_released_event(objptr, events::ipointer_event_args_t args)
{
	if (m_pointer_released_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_pointer_leaved_event(objptr, events::ipointer_event_args_t args)
{
	if (m_pointer_leaved_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_mouse_moved_event(objptr, events::ipointer_event_args_t args)
{
	if (m_mouse_moved_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_mouse_lbutton_pressed_event(objptr, events::ipointer_event_args_t args)
{
	if (m_mouse_lbutton_pressed_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_mouse_rbutton_pressed_event(objptr, events::ipointer_event_args_t args)
{
	if (m_mouse_rbutton_pressed_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_mouse_lbutton_released_event(objptr, events::ipointer_event_args_t args)
{
	if (m_mouse_lbutton_released_event.invoke(args.get()) > 0)
		args->handled(true);
}

void app::on_mouse_rbutton_released_event(objptr, events::ipointer_event_args_t args)
{
	if (m_mouse_rbutton_released_event.invoke(args.get()) > 0)
		args->handled(true);
}

