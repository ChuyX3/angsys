#include "pch.h"
#include "core_window.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::windows;

namespace ang
{
	namespace core
	{
		namespace async
		{
			ang_object(message_task);

			class message_task
				: public object
				, public itask<void>
			{
			private:
				friend platform::windows::core_listener;

				cond_ptr_t m_cond;
				mutex_ptr_t m_mutex;
				thread_t m_worker_thread;
				async_action_status_t m_status;

			public:
				message_task();

				ANG_DECLARE_INTERFACE();

				virtual iasync<void> then(delegates::function<void(iasync<void>)>)override;
				virtual bool wait(async_action_status_t status)const;
				virtual bool wait(async_action_status_t status, dword ms)const;
				virtual async_action_status_t status()const;
				virtual bool cancel();
				virtual void result()const;
			};

		}
	}
}


core_listener::core_listener(Windows::UI::Core::CoreDispatcher^dispatcher
	, core::delegates::function<dword(platform::events::message)> wnd_proc)
	: m_dispatcher(dispatcher)
	, m_wnd_proc(wnd_proc)
	, m_cond(make_shared<core::async::cond>())
	, m_mutex(make_shared<core::async::mutex>())
{

}

dword core_listener::send_msg(events::message msg)
{


	if (m_dispatcher->HasThreadAccess)
	{
		return m_wnd_proc(msg);
	}
	else
	{
		bool handled = false;
		m_dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler(
			[&](void)
		{
			core::async::scope_locker<core::async::mutex> lock(m_mutex);
			msg.result(m_wnd_proc(msg));
			handled = true;
			m_cond->signal();
		}));
		m_cond->waitfor(m_mutex, [&]() {return !handled; });
		return msg.result();
	}
}

core::async::iasync<dword> core_listener::post_msg(events::message msg)
{
	core::async::message_task* task = new core::async::message_task();
	core::async::task_handler<dword>* wrap = new core::async::task_handler<dword>(task);
	wrap->add_ref();
	m_dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler(
		[this, msg, wrap, task](void)
	{
		core::async::scope_locker<core::async::mutex> lock(m_mutex);
		
		if (wrap->status() & core::async::async_action_status::canceled)
		{
			wrap->done(-1);
		}
		else
		{
			task->m_status = core::async::async_action_status::running;
			wrap->done(m_wnd_proc(msg));
			task->m_status = core::async::async_action_status::completed;
		}
		m_cond->signal();
		wrap->release();
	}));

	return wrap;
}

bool core_listener::listen_to(events::event_t event)
{

}