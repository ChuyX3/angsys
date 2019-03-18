#include "pch.h"

#include <ang/core/timer.h>
#include <ang/collections/hash_map.h>
#include <ang/platform/win32/windows.h>
#include "wnd_handle.h"

#include "dispatcher.h"
#include "event_args.h"

using namespace ang;
using namespace ang::core;
using namespace ang::platform;
using namespace ang::platform::windows;


thread_local ui_thread* windows::s_current_ui_thread = null;

ANG_EXTERN ulong64 get_performance_time_us(void);

ui_thread_t ui_thread::create_ui_thread()
{
	ui_thread_t thread;
	core::async::thread_t core_thread = core::async::thread::create_thread();
	core_thread->run_async<void>([&](core::async::iasync<void>)
	{
		thread = new ui_thread();
	})->result();
	return thread;
}

ui_thread_t ui_thread::current_thread()
{
//	if (s_current_ui_thread == null)
//		new ui_thread();
	return s_current_ui_thread;
}

ui_thread::ui_thread()
	: m_thread(null)
	, m_initialize_event(this, [](events::core_msg_t code) { return code == (events::core_msg)events::win_msg::start_app; })
	, m_finalize_event(this, [](events::core_msg_t code) { return code == (events::core_msg)events::win_msg::exit_app; })
{
	if (s_current_ui_thread != null)
		throw(exception_t(except_code::two_singleton));

	s_current_ui_thread = this;
	
	m_thread = new _hthread();

	m_thread->hmodule = GetModuleHandle(NULL);
	m_thread->core_thread = core::async::thread::this_thread();

	m_thread->properties["cmd_args"] = null;
	m_thread->properties["main_cond"] = make_shared<core::async::cond>();
	m_thread->properties["main_mutex"] = make_shared<core::async::mutex>();
	//m_thread->properties["step_timer"] = make_shared<core::time::step_timer>();
	m_thread->dispatcher = new windows::dispatcher(
		bind(this, &ui_thread::listen_to),
		bind(this, &ui_thread::send_msg),
		bind(this, &ui_thread::post_task));
}

ui_thread::~ui_thread()
{
	
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::ui_thread);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::ui_thread, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::ui_thread, object)

ang_platform_implement_event_handler(ui_thread, initialize_event);
ang_platform_implement_event_handler(ui_thread, finalize_event);

void ui_thread::clear() {
	close();
	s_current_ui_thread = null;
}

pointer ui_thread::handle()const
{
	return m_thread;
}

bool ui_thread::close()
{
	if (m_thread)
	{
		auto handle = m_thread;
		m_thread = null;
		handle->dispatcher = null;
		handle->core_thread->exit();
		handle->core_thread = null;
		delete handle;
	}
	m_initialize_event.empty();
	m_finalize_event.empty();
	return true;
}

bool ui_thread::is_current_thread()const {
	return m_thread->core_thread->has_thread_access();
}

core::async::mutex_ptr_t ui_thread::main_mutex()const {
	return m_thread->properties["main_mutex"].as<core::async::mutex>();
}

core::async::cond_ptr_t ui_thread::main_cond()const {
	return m_thread->properties["main_cond"].as<core::async::cond>();
}

core::async::thread_t ui_thread::main_worker_thread()const {
	return m_thread->core_thread;
}

array<astring> ui_thread::command_line_args()const {
	return m_thread->properties["cmd_args"].as<array<astring>>();
}

var ui_thread::settings(castr_t key)const {
	return m_thread->properties[key];
}

void ui_thread::settings(castr_t key, var obj) {
	m_thread->properties[key] = obj;
}

core::async::iasync<dword> ui_thread::run_async()
{
	if (m_thread->core_thread->status().is_active<core::async::async_action_status::attached>())
		throw_exception(except_code::unsupported);
	ui_thread_t auto_save = this;
	return m_thread->core_thread->run_async<dword>([=](core::async::iasync<dword>)->dword
	{
		return auto_save.get()->run();
	});
}

dword ui_thread::run()
{
	ui_thread_t auto_save = this;
	if (!is_current_thread())
		throw_exception(except_code::invalid_access);

	if (!init_app(command_line_args()))
	{
		close();
		return -1;
	}
	int res = 0;
	res = on_run();
	exit_app();
	return res;
}

dword ui_thread::run(array<astring> args)
{
	ui_thread_t auto_save = this;
	if (!is_current_thread())
		throw_exception(except_code::invalid_access);

	if (!init_app(command_line_args()))
	{
		close();
		return -1;
	}
	int res = 0;
	res = on_run();
	exit_app();
	return res;
}

int ui_thread::on_run()
{
	MSG msg;
	long64 last_time = core::time::get_performance_time_us();	
	while (true)
	{
		core::async::async_action_status_t status = m_thread->core_thread->status();

		if (status & core::async::async_action_status::canceled)
			PostQuitMessage(0);

		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
	
			if (msg.hwnd == NULL)
			{
				dispatch_msg({ (events::core_msg)msg.message, msg.wParam, msg.lParam });
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

bool ui_thread::init_app(array<astring> cmd)
{
	m_thread->dispatcher->worker_thread(m_thread->core_thread);
	platform::events::message m = platform::events::message((events::core_msg)events::win_msg::start_app, 0, 0);
	try { m_initialize_event(new platform::events::app_status_event_args(m, null)); }
	catch (...) {}
	return true;
}

void ui_thread::update_app()
{
	//m_thread->properties["step_timer"].as<core::time::step_timer>()->update();
}

bool ui_thread::exit_app()
{
	platform::events::message m = platform::events::message((events::core_msg)events::win_msg::exit_app, 0, 0);
	try { m_finalize_event(new platform::events::app_status_event_args(m, null)); }
	catch (...) {}

	return close();
}


events::event_token_t ui_thread::listen_to(events::event_t)
{
	return events::event_token();
}

dword ui_thread::send_msg(events::message msg)
{
	return dispatch_msg(msg);
}

core::async::iasync<void> ui_thread::post_task(core::async::iasync<void> async)
{
	auto task = interface_cast<async_task>(async.get());
	if (task == null)
		return null;
	task->add_ref();
	if (PostThreadMessageW(m_thread->core_thread->thread_id(), (uint)events::win_msg::system_reserved_event, 0, reinterpret_cast<LPARAM>(task)) == 0)
	{
		task->release();
		return null;
	}
	return async;
}

dword ui_thread::dispatch_msg(events::message m)
{
	switch ((events::win_msg)(events::core_msg)m.msg())
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
