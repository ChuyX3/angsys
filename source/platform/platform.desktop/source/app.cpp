#include "pch.h"

#include "ang/core/time.hpp"
#include "ang/platform/angwin/angwin.hpp"
#include "async_msg_task.h"

using namespace ang;
using namespace ang::core;
using namespace ang::platform;
using namespace ang::platform::windows;

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

namespace ang {
	namespace platform {
		namespace windows {
			typedef struct _hprocess
			{
				pointer hmodule;
				bool close_request;
				events::message_t nonqueue_msg;
				core::async::thread_t main_thread;
			}*hprocess_t;
			extern process* _current_process;
		}
	}
}

app_t app::current_app()
{
	if (_current_process == null)
		_current_process = new app();
	return _current_process->as<app>();
}

icore_app_t icore_app::get_core_app()
{
	return app::current_app().get();
}

app::app()
	: _enable_update(true)
	, _main_wnd(null)
	, main_wnd_created_event(this, [](events::core_msg_t code) { return events::win_msg_enum::created == code; })
	, main_wnd_destroyed_event(this, [](events::core_msg_t code) { return events::win_msg_enum::destroyed == code; })
{

}

app::~app()
{
	static_cast<events::event_trigger<app>&>(main_wnd_created_event).empty();
	static_cast<events::event_trigger<app>&>(main_wnd_destroyed_event).empty();
}

ANG_IMPLEMENT_CLASSNAME(ang::platform::windows::app);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::windows::app);
bool app::is_inherited_of(type_name_t name)
{
	return name == type_of<app>()
		|| process::is_inherited_of(name)
		|| icore_app::is_inherited_of(name);
}

bool app::is_kind_of(type_name_t name)const
{
	if (name == type_of<app>()
		|| process::is_kind_of(name)
		|| name == type_of<icore_app>())
		return true;
	return false;
}

bool app::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<app>())
	{
		*out = static_cast<app*>(this);
		return true;
	}
	else if (process::query_object(name, out))
	{
		return true;
	}
	else if (icore_app::query_object(name, out))
	{
		return true;
	}
	return false;
}


void app::set_main_wnd(window_t mainView)
{
	if (!_main_wnd.is_empty() || mainView == null)
		return;
	_main_wnd = mainView;
}

pointer app::get_core_app_handle()const
{
	if (is_created())
		return hprocess_t(_process)->hmodule;
	return null;
}

icore_view_t app::get_main_core_view()
{
	return static_cast<icore_view*>(_main_wnd);
}

//imessage_reciever_t app::get_listener()const
//{
//	return const_cast<app*>(this);
//}

input::ikeyboard_t app::get_keyboard()
{
	return null;
}

window_t app::main_wnd()const
{
	return _main_wnd;
}

bool app::init_app(array<string> cmdl)
{
	return process::init_app(move(cmdl));
}

void app::update_app()
{
	process::update_app();

	if (_enable_update && !_main_wnd.is_empty())
	{
		_main_wnd->send_msg(new events::message(events::core_msg_enum::update, 0, (LPARAM)(pointer)property("main_step_timer").get()));
		_main_wnd->send_msg(new events::message(events::core_msg_enum::draw));
	}
	
}

bool app::exit_app()
{
	return process::exit_app();
}

async::iasync_t<dword> app::run_async(window_t wnd, wnd_create_args_t args)
{
	using namespace async;

	auto ret = async::task::run_async<dword>([this](iasync<dword>* async, var_args_t args)->dword
	{
		window_t wnd = args[1]->as<window>();
		wnd_create_args_t wndArgs = args[2]->as<wnd_create_args>();

		if (!wnd->create(wndArgs))
			return -1;	

		wnd->update_wnd();
		wnd->show(showing_flag::show);
		return on_run_async(async, wnd);
	}, var_args_t{ this, wnd.get(), args.get() });
	ret->wait(async_action_status::running, -1);
	return ret;
}

dword app::on_run_async(core::async::iasync_t<dword> action, window_t wnd)
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
			::SendMessageW((HWND)wnd->get_core_view_handle(), WM_CLOSE, 0, 0);
			return -1;
		}

		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			switch (msg.message)
			{
			case events::win_msg_enum::system_reserved_event:
				if (msg.wParam != 0 && msg.lParam != 0)
				{
					dword result = 0;
					auto task = reinterpret_cast<async_msg_task*>(msg.wParam);
					auto reciever = reinterpret_cast<imessage_reciever*>(msg.lParam);
					
					core::async::scope_locker<core::async::mutex_ptr_t> locker(main_mutex());
					if (task->status_ != core::async::async_action_status::canceled)
					{
						task->status_ = core::async::async_action_status::running;
						task->cond_->signal();
						{//Executing command
							mutex->unlock();
							reciever->send_msg(task->msg_);
							result = task->msg_->result();
							mutex->lock();
						}
						task->status_ = core::async::async_action_status::completed;
					}
					task->complete(result);
					task->release();
				}
				break;
			default:
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
				break;
			}
		}

		if (_enable_update) {
			timer->update();
			long64 lprm = (LPARAM)(void*)timer.get();
			wnd->send_msg(new events::message(events::win_msg_enum::update, 0, lprm));
			wnd->send_msg(new events::message(events::win_msg_enum::draw));
		}
		
	}
	return 0;
}
