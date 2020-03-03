#include "pch.h"

//#include "ang/core/time.h"
#include <ang/platform/win32/windows.h>
//#include <ang/platform/win32/keyboard.h>
#include "dispatcher.h"
#include "event_args.h"
#include <ang/core/timer.h>
#include <ang/platform/win32/controller.h>
#include "wnd_handle.h"

using namespace ang;
using namespace ang::core;
using namespace ang::platform;
using namespace ang::platform::windows;

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#else 
#define NEW new
#endif


app_t app::current_app()
{
	if (s_current_ui_thread == null)
		s_current_ui_thread = new app();
	return s_current_ui_thread->as<app>();
}

#if defined ANGSYS_DYNAMIC_LIBRARY
__declspec(dllimport) void ang_platform_core_app_set_instance(icore_app* instance);
__declspec(dllimport) void ang_platform_core_app_reset_instance();
__declspec(dllimport) void ang_platform_input_controller_manager_set_instance(input::icontroller_manager*);
__declspec(dllimport) void ang_platform_input_controller_manager_reset_instance();
#else
extern void ang_platform_input_controller_manager_set_instance(input::icontroller_manager*);
extern void ang_platform_input_controller_manager_reset_instance();
#endif

app::app()
	: m_main_wnd(null)
{
	ang_platform_core_app_set_instance(this);
	ang_platform_input_controller_manager_set_instance(input::controller_manager::instance());
}

app::~app()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::app, ui_thread, icore_app);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::app);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::app, ui_thread, icore_app);

void app::dispose()
{
	ang_platform_input_controller_manager_reset_instance();
	input::controller_manager::release_instance();
	ang_platform_core_app_reset_instance();
	base::dispose();
}

bool app::close()
{
	if (!m_main_wnd.is_empty())
	{
		HWND hwnd = (HWND)m_main_wnd->core_view_handle();
		if (IsWindow(hwnd))
			SendMessageW(hwnd, WM_CLOSE, 0, 0);
	}
	m_main_wnd = null;
	return ui_thread::close();
}

pointer app::core_app_handle()const
{
	return m_thread->hmodule;
}

icore_view_t app::main_core_view()
{
	return static_cast<icore_view*>(m_main_wnd.get());
}

input::ikeyboard_t app::keyboard()
{
	return null;
}

bool app::init_app(array<astring> cmdl)
{
	ui_thread::init_app(move(cmdl));

	if (!m_main_wnd.is_empty())
		close_token = m_main_wnd->closed_event += new events::closed_event(this, &app::on_main_wnd_closed);
	return true;
}

void app::update_app()
{
	ui_thread::update_app();

	if (!m_main_wnd.is_empty() && m_main_wnd->is_created)
	{
		input::controller_manager::instance()->update();
		m_main_wnd->dispatcher()->send_msg(events::message(events::core_msg::update, 0, 0));
	}
	
}

bool app::exit_app()
{
	if(!m_main_wnd.is_empty())
		m_main_wnd->closed_event -= close_token;
	return ui_thread::exit_app();
}

void app::on_main_wnd_closed(objptr, platform::events::imsg_event_args_t)
{
	PostQuitMessage(0);
}

async::iasync<dword> app::run_async(function<void(app_t)> start)
{
	app_t thread;
	core::async::thread_t core_thread = core::async::thread::create_thread();
	core_thread->run_async<void>([&](core::async::iasync<void>)
	{
		thread = new app();
	})->result();

	thread->initialize_event += new events::start_app_event([=](objptr, events::iapp_status_event_args_t)
	{
		start(thread);
	});

	return thread->run_async();
}

