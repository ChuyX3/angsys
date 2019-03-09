#include "pch.h"

#include "ang/platform/win32/windows.h"
#include "dispatcher.h"
#include "event_args.h"
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
				dispatcher_t dispatcher;
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
	, m_start_app_event(this, [](events::core_msg_t code) { return (events::win_msg)code == events::win_msg::start_app; })
	, m_exit_app_event(this, [](events::core_msg_t code) { return (events::win_msg)code == events::win_msg::exit_app; })
{
	if (s_current_process != null)
		throw(exception_t(except_code::two_singleton));

	s_current_process = this;
	
	m_process = new _hprocess();

	m_process->hmodule = GetModuleHandle(NULL);
	m_process->main_thread = core::async::thread::main_thread();

	m_process->properties["cmd_args"] = null;
	m_process->properties["main_cond"] = make_shared<core::async::cond>();
	m_process->properties["main_mutex"] = make_shared<core::async::mutex>();
	//m_process->properties["step_timer"] = make_shared<core::time::step_timer>();
	m_process->dispatcher = new windows::dispatcher(
		bind(this, &process::listen_to),
		bind(this, &process::send_msg),
		bind(this, &process::post_task));
}

process::~process()
{
	close();
	s_current_process = null;
	m_start_app_event.empty();
	m_exit_app_event.empty();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::process);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::process, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::process, object)

ang_platform_implement_event_handler(process, start_app_event);
ang_platform_implement_event_handler(process, exit_app_event);

pointer process::handle()const
{
	return m_process;
}

bool process::close()
{
	if (m_process)
	{
		auto handle = m_process;
		m_process = null;
		handle->dispatcher = null;
		handle->main_thread = null;
		delete handle;
	}
	return true;
}

bool process::is_worker_thread()const {
	return m_process->main_thread->has_thread_access();
}

core::async::mutex_ptr_t process::main_mutex()const {
	return m_process->properties["main_mutex"].as<core::async::mutex>();
}

core::async::cond_ptr_t process::main_cond()const {
	return m_process->properties["main_cond"].as<core::async::cond>();
}

core::async::thread_t process::main_worker_thread()const {
	return m_process->main_thread;
}

array<string> process::command_line_args()const {
	return m_process->properties["cmd_args"].as<array<string>>();
}

var process::settings(cstr_t key)const {
	return m_process->properties[key];
}

void process::settings(cstr_t key, var obj) {
	m_process->properties[key] = obj;
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
	long64 last_time = core::time::get_performance_time_us();	
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

		long64 curr_time = core::time::get_performance_time_us();
		if((curr_time - last_time) > 8333)
		{
			last_time = curr_time;
			update_app();
		}
	}
	return msg.wParam;
}

bool process::init_app(array<string> cmd)
{
	m_process->dispatcher->worker_thread(m_process->main_thread);
	platform::events::message m = platform::events::message((events::core_msg_t)events::win_msg::start_app, 0, 0);
	try { m_start_app_event(new platform::events::app_status_event_args(m, null)); }
	catch (...) {}
	//m_process->properties["step_timer"].as<core::time::step_timer>()->reset();
	
	return true;
}

void process::update_app()
{
	//m_process->properties["step_timer"].as<core::time::step_timer>()->update();
}

bool process::exit_app()
{
	platform::events::message m = platform::events::message((events::core_msg_t)events::win_msg::start_app, 0, 0);
	try { m_exit_app_event(new platform::events::app_status_event_args(m, null)); }
	catch (...) {}

	return close();
}


events::event_token_t process::listen_to(events::event_t)
{
	return events::event_token();
}

dword process::send_msg(events::message msg)
{
	return dispatch_msg(msg);
}

core::async::iasync<void> process::post_task(core::async::iasync<void> async)
{
	auto task = interface_cast<async_task>(async.get());
	if (task == null)
		return null;
	task->add_ref();
	if (PostThreadMessageW(m_process->main_thread->thread_id(), (events::core_msg_t)events::win_msg::system_reserved_event, 0, reinterpret_cast<LPARAM>(task)) == 0)
	{
		task->release();
		return null;
	}
	return async;
}

dword process::dispatch_msg(events::message m)
{
	switch ((events::win_msg)m.msg())
	{
	case events::win_msg::system_reserved_event: {
			auto task = reinterpret_cast<async_task*>(m.lparam());
			task->execute();
			task->release();
			m.result(0);
		} break;
	}
	return 0;
}
