#include "pch.h"

#include "ang/platform/angwin/angwin.h"
#include "async_msg_task.h"
#include <ang/collections/hash_map.h>
#include <ang/core/timer.h>

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

			struct process::_hprocess
			{
				pointer hmodule = null;
				core::async::thread_t main_thread = null;
				collections::hash_map<string, var> properties;

			};
			process* s_current_process = null;
		}
	}
}

ANG_EXTERN ulong64 get_performance_time_us(void);

process_t process::current_process()
{
	if (s_current_process == null)
		new process();
	return s_current_process;
}

process::process()
	: m_process(null)
	, start_app_event(this, [](events::core_msg_t code) { return code == events::win_msg_enum::start_app; })
	, exit_app_event(this, [](events::core_msg_t code) { return code == events::win_msg_enum::exit_app; })
{
	if (s_current_process != null)
		throw(exception_t(except_code::two_singleton));

	s_current_process = this;
	
	m_process = new _hprocess();

	m_process->hmodule = GetModuleHandle(NULL);
	m_process->main_thread = core::async::thread::main_thread();

	m_process->properties["cmd_args"] = null;
	m_process->properties["main_cond"] = make_shared<core::async::cond_t>();
	m_process->properties["main_mutex"] = make_shared<core::async::mutex_t>();
	m_process->properties["step_timer"] = make_shared<core::time::step_timer>();
}

process::~process()
{
	close();
	s_current_process = null;
	static_cast<events::event_trigger<process>&>(start_app_event).empty();
	static_cast<events::event_trigger<process>&>(exit_app_event).empty();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::process);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::process, object, ang::platform::imessage_listener);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::process, object, ang::platform::imessage_listener)

pointer process::handle()const
{
	return m_process;
}

bool process::close()
{
	auto handle = m_process;
	m_process = null;
	delete handle;
	return true;
}

bool process::is_worker_thread()const {
	return m_process->main_thread->is_current_thread();
}

core::async::mutex_ptr_t process::main_mutex()const {
	return m_process->properties["main_mutex"].as<core::async::mutex_t>();
}

core::async::cond_ptr_t process::main_cond()const {
	return m_process->properties["main_cond"].as<core::async::cond_t>();
}

core::async::thread_t process::main_worker_thread()const {
	return m_process->main_thread;
}

array<string> process::command_line_args()const {
	return m_process->properties["cmd_args"].as<array<string>>();
}

var process::property(cstr_t key)const {
	return m_process->properties[key];
}

void process::property(cstr_t key, var obj) {
	m_process->properties[key] = obj;
}

events::event_trigger<process>const& process::trigger(events::event_listener const& listener)const
{
	return static_cast<events::event_trigger<process>const&>(listener);
}

dword process::run()
{
	if (!init_app(command_line_args()))
	{
		close();
		return -1;
	}
	main_loop();
	exit_app();
	return 0;
}

dword process::run(array<string> args)
{
	if (!init_app(command_line_args()))
	{
		close();
		return -1;
	}

	int res = main_loop();

	exit_app();

	return res;
}

int process::main_loop()
{
	MSG msg;
	while (true)
	{
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
	
			if (msg.hwnd == NULL)
			{
				dispatch_msg({ msg.message, msg.wParam, msg.lParam });
			}

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			update_app();
		}
	}
	return msg.wParam;
}

bool process::init_app(array<string> cmd)
{
	platform::events::message m = platform::events::message(events::win_msg_enum::start_app, 0, 0);
	try { trigger(start_app_event).invoke(new platform::events::app_status_event_args(m, null)); }
	catch (...) {}
	m_process->properties["step_timer"].as<core::time::step_timer>()->reset();
	return true;
}

void process::update_app()
{
	m_process->properties["step_timer"].as<core::time::step_timer>()->update();
}

bool process::exit_app()
{
	platform::events::message m = platform::events::message(events::win_msg_enum::start_app, 0, 0);
	try { trigger(exit_app_event).invoke(new platform::events::app_status_event_args(m, null)); }
	catch (...) {}

	return close();
}

bool process::listen_to(events::event_t)
{
	return false;
}

dword process::send_msg(events::message msg)
{
	if (m_process->main_thread->is_current_thread())
	{
		return dispatch_msg(msg);
	}
	else
	{
		PostMessageW(NULL, msg.msg(), msg.wparam(), msg.lparam());
		return -1; //TODO
	}
}

core::async::iasync<dword> process::post_msg(events::message msg)
{
	PostMessageW(NULL, msg.msg(), msg.wparam(), msg.lparam());
	return null; //TODO
}

dword process::dispatch_msg(events::message msg)
{
	return 0;
}
