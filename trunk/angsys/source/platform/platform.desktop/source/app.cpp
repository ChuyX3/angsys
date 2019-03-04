#include "pch.h"

//#include "ang/core/time.h"
#include "ang/platform/win32/windows.h"
#include "dispatcher.h"
#include <ang/core/timer.h>

using namespace ang;
using namespace ang::core;
using namespace ang::platform;
using namespace ang::platform::windows;

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#else 
#define NEW new
#endif

namespace ang {
	namespace platform {
		namespace windows {

			struct process::_hprocess
			{
				pointer hmodule = null;
				core::async::thread_t main_thread = null;
				dispatcher_t dispatcher;
				collections::hash_map<string, var> properties;
			};
			extern process* s_current_process;
		}
	}
}

app_t app::current_app()
{
	if (s_current_process == null)
		s_current_process = new app();
	return s_current_process->as<app>();
}

//icore_app_t icore_app::core_app()
//{
//	return app::current_app().get();
//}

app::app()
	: m_main_wnd(null)
{

}

app::~app()
{
}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::app, process, icore_app);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::app);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::app, process, icore_app);

void app::set_main_wnd(wndptr mainView)
{
	if (!m_main_wnd.is_empty() || mainView == null)
		return;
	m_main_wnd = mainView;
}

pointer app::core_app_handle()const
{
	return m_process->hmodule;
}

icore_view_t app::main_core_view()
{
	return static_cast<icore_view*>(m_main_wnd.get());
}

//imessage_reciever_t app::get_listener()const
//{
//	return const_cast<app*>(this);
//}

input::ikeyboard_t app::keyboard()
{
	return null;
}

wndptr app::main_wnd()const
{
	return m_main_wnd;
}

void app::main_wnd(wndptr wnd)
{
	if (m_main_wnd.is_empty())
		m_main_wnd = wnd;
}

bool app::init_app(array<string> cmdl)
{
	process::init_app(move(cmdl));

	if (!m_main_wnd.is_empty())
		m_main_wnd->closed_event += new events::closed_event(this, &app::on_main_wnd_closed);
	return true;
}

void app::update_app()
{
	process::update_app();

	if (!m_main_wnd.is_empty() && m_main_wnd->is_created())
	{
		auto timer = m_process->properties["step_timer"].as<core::time::step_timer>();
		timer->delta();
		//pointer(currentTime - lastTime), pointer(currentTime - startTime)
		m_main_wnd->listener()->send_msg(events::message((events::core_msg_t)events::core_msg_enum::update, timer->delta(), timer->total()));
		m_main_wnd->listener()->send_msg(events::message((events::core_msg_t)events::core_msg_enum::draw));
	}
	
}

bool app::exit_app()
{
	return process::exit_app();
}

void app::on_main_wnd_closed(objptr, platform::events::imsg_event_args_t)
{
	PostQuitMessage(0);
}

async::iasync<dword> app::run_async(wndptr w, wnd_create_args_t a)
{
	using namespace async;
	app_t s_app = this;
	auto ret = async::task::run_async<dword>([this, s_app, w, a](iasync<dword> async)->dword
	{
		wndptr wnd = w.get();
		wnd_create_args_t args = a.get();

		if (!wnd->create(args))
			return -1;	

		wnd->update_wnd();
		wnd->show(showing_flag::show);
		return on_run_async(async, wnd);
	});
	ret->wait(async_action_status::running, -1);
	return ret;
}

dword app::on_run_async(core::async::iasync<dword> action, wndptr wnd)
{
	if (wnd == null || !wnd->is_created())
		return -1;
	MSG msg;
	auto mutex = main_mutex();
	auto timer = make_shared<core::time::step_timer>();
	timer->reset();
	while (wnd->is_created())
	{
		if (action->status() == core::async::async_action_status::canceled)
		{
			::SendMessageW((HWND)wnd->core_view_handle(), WM_CLOSE, 0, 0);
			return -1;
		}

		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			switch ((events::win_msg)msg.message)
			{
			case events::win_msg::system_reserved_event: {
					auto task = reinterpret_cast<async_task*>(msg.lParam);
					task->execute();
					task->release();
				} break;
			default:
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
				break;
			}
		}
		else {
			timer->update();
			wnd->listener()->send_msg(events::message((events::core_msg_t)events::win_msg::update, timer->delta(), timer->total()));
			wnd->listener()->send_msg(events::message((events::core_msg_t)events::win_msg::draw));
		}
		
	}
	return 0;
}
