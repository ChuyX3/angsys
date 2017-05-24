#include "pch.h"

#include "ang/core/time.h"
#include "ang/platform/angwin/angwin.h"
#include "async_msg_task.h"

using namespace ang;
using namespace ang::core;
using namespace ang::platform;
using namespace ang::platform::windows;

//#if defined _DEBUG
//#define new ANG_DEBUG_NEW()
//#endif

namespace ang {
	namespace platform {
		namespace windows {
			typedef struct _hprocess
			{
				pointer hmodule;
				bool_t close_request;
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

app::app()
	: _main_wnd(null)
	, mainWndCreatedEvent(this, [](events::core_msg_t code) { return events::win_msg_enum::Created == code; })
	, mainWndDestroyedEvent(this, [](events::core_msg_t code) { return events::win_msg_enum::Destroyed == code; })
{

}

app::~app()
{
}

ANG_IMPLEMENT_CLASSNAME(ang::platform::windows::app);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::windows::app);
bool_t app::is_child_of(type_name_t className)
{
	if (className == type_name<app>()
		|| process::is_child_of(className)
		|| className == type_name<icore_app>())
		return true;
	return false;
}

bool_t app::is_kind_of(type_name_t className)const
{
	if (className == type_name<app>()
		|| process::is_kind_of(className)
		|| className == type_name<icore_app>())
		return true;
	return false;
}

bool_t app::query_object(type_name_t className, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == type_name<app>())
	{
		*out = static_cast<app*>(this);
		return true;
	}
	else if (process::query_object(className, out))
	{
		return true;
	}
	else if (className == type_name<icore_app>())
	{
		*out = static_cast<ang::platform::icore_app*>(this);
		return true;
	}
	return false;
}


void app::set_main_wnd(window_t mainView)
{
	//ObjectPtr::Set(dynamic_cast<Window*>(mainView));

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

imessage_reciever_t app::get_listener()const
{
	return const_cast<app*>(this);
}

input::ikeyboard_t app::get_keyboard()
{
	return null;
}

window_t app::main_wnd()const
{
	return _main_wnd;
}

async::iasync_t<dword> app::run_async(window_t wnd, wnd_create_args_t args)
{
	using namespace async;

	auto ret = async::async_task<dword>::run_async([this](iasync<dword>* async, var_args_t args)->dword
	{
		window_t wnd = args[1]->as<window>();
		wnd_create_args_t wndArgs = args[2]->as<wnd_create_args>();

		if (!wnd->create(wndArgs))
			return -1;	

		wnd->update_wnd();
		wnd->show(showing_flag::show);
		return on_run_async(async, wnd);
	}, { this, wnd.get(), args.get() }, async::thread_priority::normal);
	ret->wait(async_action_status::running, -1);
	return ret;
}

dword app::on_run_async(core::async::iasync_t<dword> action, window_t wnd)
{
	if (wnd == null || !wnd->is_created())
		return -1;
	MSG msg;

	core::time::timer timer;
	timer.reset();
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
			case events::win_msg_enum::SystemReservedEvent:
				if (msg.wParam != 0 && msg.lParam != 0)
				{
					auto task = reinterpret_cast<async_msg_task*>(msg.wParam);
					auto reciever = reinterpret_cast<imessage_reciever*>(msg.lParam);
					core::async::scope_locker locker(mutex);
					if (task->_status != core::async::async_action_status::canceled)
					{
						task->_status = core::async::async_action_status::running;
						task->_cond->signal();
						{//Executing command
							mutex->unlock();
							reciever->send_msg(task->_msg);
							mutex->lock();
						}
						task->_status = core::async::async_action_status::completed;
					}
					task->complete();
					task->release();
				}
				break;
			default:
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
				break;
			}
		}

		timer.update();
		events::message_t msg = new events::message(events::win_msg_enum::Update, &timer);
		wnd->send_msg(msg);
		wnd->send_msg(new events::message(events::win_msg_enum::Draw));
	}
	return 0;
}
