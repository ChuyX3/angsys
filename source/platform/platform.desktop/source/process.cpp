#include "pch.h"

#include "ang/core/time.hpp"
#include "ang/platform/angwin/angwin.hpp"
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
				bool close_request;
				events::message_t nonqueue_msg;
				core::async::thread_t main_thread;
			}*hprocess_t;

			process* _current_process = null;
		}
	}
}

process_t process::current_process()
{
	if (_current_process == null)
		new process();
	return _current_process;
}

process::process()
	: _process(null)
	, mutex(make_shared<core::async::mutex>())
	, cond(make_shared<core::async::cond>())
	, startAppEvent(this, [](events::core_msg_t code) { return code == events::win_msg_enum::start_app; })
	, exitAppEvent(this, [](events::core_msg_t code) { return code == events::win_msg_enum::exit_app; })
{
	if (_current_process != null)
		throw(exception_t(except_code::two_singleton));
	_current_process = this;
//	ang::instance_manager::regist_instance(class_name(), this);
}

process::~process()
{
	close();
	_current_process = null;
}

ANG_IMPLEMENT_CLASSNAME(ang::platform::windows::process);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::windows::process);

bool process::is_inherited_of(type_name_t name)
{
	return name == type_of<process>()
		|| object::is_inherited_of(name)
		|| imessage_reciever::is_inherited_of(name);
}

bool process::is_kind_of(type_name_t name)const
{
	return name == type_of<process>()
		|| object::is_kind_of(name)
		|| imessage_reciever::is_kind_of(name);
}

bool process::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<process>())
	{
		*out = static_cast<process*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (imessage_reciever::query_object(name, out))
	{
		return true;
	}
	return false;
}

hprocces_t process::handle()const
{
	return _process;
}

bool process::is_created()const
{
	return bool(_process != null);
}

bool process::create(pointer a)
{
	if (a != null)
		cmd_args = reinterpret_cast<collections::array_buffer<string>*>(a);

	auto handle = new _hprocess();
	handle->close_request = false;
	handle->nonqueue_msg = null;
	handle->main_thread = core::async::thread::main_thread();
	handle->hmodule = GetModuleHandle(NULL);

	attach(handle);

	return true;
}

bool process::close()
{
	auto handle = reinterpret_cast<hprocess_t>(_process);
	_process = null;
	delete handle;
	return true;
}

void process::attach(hprocces_t h)
{
	_process = h;
}

void process::detach()
{
	_process = null;
}

bool process::is_worker_thread()const
{
	if (!is_created())
		return false;
	return hprocess_t(_process)->main_thread->is_current_thread();
}

core::async::mutex_ptr_t process::main_mutex()const
{
	return mutex;
}

core::async::cond_ptr_t process::main_cond()const
{
	return cond;
}

core::async::thread_t process::main_worker_thread()const
{
	if (!is_created())
		return null;
	return hprocess_t(_process)->main_thread;
}

array<string> process::command_line_args()const
{
	return cmd_args;
}

dword process::run()
{
	if (!create(null))
		return false;
	if (!init_app(cmd_args))
	{
		close();
		return -1;
	}
	while (update());
	exit_app();
	return 0;
}

dword process::run(array<string> args)
{
	if (!create(args.get()))
		return false;
	if (!init_app(cmd_args))
	{
		close();
		return -1;
	}
	while (update());
	exit_app();
	return 0;
}

bool process::update()
{
	try {
		dispatch_msg();
		update_app();
	}
	catch (exception_t e)
	{
		ang_debug_output_error((cstr_t)e->what());
		hprocess_t(_process)->close_request = true;
	}
	return !hprocess_t(_process)->close_request;
}

bool process::init_app(array<string>)
{
	return true;
}

void process::update_app()
{

}

bool process::exit_app()
{
	return close();
}

dword process::on_message_dispatcher(events::message_t msg)
{
	switch (msg->msg())
	{
	case WM_QUIT:
		hprocess_t(_process)->close_request = true;
		return (dword)msg->wparam();
	case events::win_msg_enum::system_reserved_event:
		if (msg->wparam() != 0)
		{
			dword res = 0;
			auto task = reinterpret_cast<async_msg_task*>(msg->wparam());
			auto reciever = reinterpret_cast<imessage_reciever*>(msg->lparam());
			core::async::scope_locker<core::async::mutex_ptr_t> locker(mutex);
			if (task->status_ != core::async::async_action_status::canceled)
			{
				task->status_ = core::async::async_action_status::running;
				task->cond_->signal();
				{//Executing command
					mutex->unlock();
					if (reciever)
						reciever->send_msg(task->msg_);
					else send_msg(task->msg_);
					res = task->msg_->result();
					mutex->lock();
				}
				task->status_ = core::async::async_action_status::completed;
			}
			task->complete(res);
			task->release();
		}
		break;
	case events::win_msg_enum::interprocess_command:
		if (msg->wparam() != 0)
		{
			HANDLE hEvent = reinterpret_cast<HANDLE>(msg->wparam());
			uint command = uint(msg->lparam());
			core::async::scope_locker<core::async::mutex_ptr_t> locker(mutex);
			SetEvent(hEvent);
		}
		break;
	}
	return 0;
}

bool process::listen_to(events::event_t)
{
	return false;
}

void process::send_msg(events::message_t msg)
{
	if (!is_created())
		return;

	if (is_worker_thread())
	{
		msg->result(on_message_dispatcher(msg));
		return;
	}

	core::async::scope_locker<core::async::mutex_ptr_t> locker(mutex);
	//Waiting for dispatch previous message
	cond->waitfor(mutex, [&]()->bool
	{
		return !hprocess_t(_process)->nonqueue_msg.is_empty();
	});

	hprocess_t(_process)->nonqueue_msg = msg.get();
	//Waiting for dispatch my message
	cond->waitfor(mutex, [&]()->bool
	{
		return (hprocess_t(_process)->nonqueue_msg.get() == msg.get());
	});
}

core::async::ioperation_t<dword> process::post_msg(events::message_t msg)
{
	if (!is_created())
		return null;

	auto task = new async_msg_task(this, cond, mutex, msg);
	task->add_ref();
	if (PostThreadMessageW(hprocess_t(_process)->main_thread->thread_id()
		, events::win_msg_enum::system_reserved_event, (WPARAM)task
		, (LPARAM)static_cast<imessage_reciever*>(this)) == 0)
	{
		task->release();
		return null;
	}
	return task;
}

bool process::dispatch_msg()
{
	if (!is_created())
		return false;
	bool result = false;

	if (hprocess_t(_process)->nonqueue_msg != null)
	{
		hprocess_t(_process)->nonqueue_msg->result(on_message_dispatcher(hprocess_t(_process)->nonqueue_msg));
		core::async::scope_locker<core::async::mutex_ptr_t> locker(mutex);
		hprocess_t(_process)->nonqueue_msg = null;
		cond->signal();
		result = true;
	}

	MSG msg;
	if (PeekMessageW(&msg, null, 0, 0, PM_REMOVE) != FALSE)
	{
		result = true;
		if (msg.hwnd == null)
		{
			events::message_t m = new events::message(msg.message, msg.wParam, msg.lParam);
			m->result(on_message_dispatcher(m));
		}
		else switch (msg.message)
		{
		case WM_QUIT:
			hprocess_t(_process)->close_request = true;
			break;
		default:
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			break;
		}
	}
	return result;
}
