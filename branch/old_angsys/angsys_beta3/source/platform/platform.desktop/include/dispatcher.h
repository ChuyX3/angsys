#ifndef __ASYNCMSGTASK_H__
#define __ASYNCMSGTASK_H__

#include <ang/platform/win32/windows.h>
//#include <Queue.h>

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			ang_object(async_task);
			ang_object(dispatcher);

			class LINK dispatcher
				: public smart<dispatcher, imessage_listener>
			{
			private:
				core::async::thread_t m_thread;
				function<events::event_token_t(events::event_t)> m_listen_callback;
				function<dword(events::message)> m_dispatch_message;
				function<core::async::iasync<void>(core::async::iasync<void>)> m_run_async_callback;

			public:
				dispatcher(
					function<events::event_token_t(events::event_t)>,
					function<dword(events::message)>,
					function<core::async::iasync<void>(core::async::iasync<void>)>
				);

				ANG_DECLARE_INTERFACE();

				core::async::thread_t worker_thread()const;
				void worker_thread(core::async::thread_t);

				core::async::iasync<void> post_task(async_task_t);

				virtual bool has_thread_access()const override;
				virtual core::async::iasync<void> run_async(core::delegates::function<void(core::async::iasync<void>)>)override;
				virtual dword send_msg(events::message) override;
				virtual core::async::iasync<dword> post_msg(events::message) override;
				virtual events::event_token_t listen_to(events::event_t) override;

			private:
				virtual~dispatcher();
			};

			class async_task final
				: public smart<async_task, core::async::itask<void>>
			{
				friend dispatcher;
			public:
				bool m_was_canceled;
				mutable core::async::cond_t m_cond;
				mutable core::async::mutex_t m_mutex;
				mutable core::async::async_action_status_t m_status;
				weak_ptr<dispatcher> m_parent;
				function<void(core::async::iasync<void>)> m_action;
				async_task_t m_parent_task;
				async_task_t m_child_task;

			public:
				async_task(dispatcher*, function<void(core::async::iasync<void>)>);
			
			public: //Overrides
				ANG_DECLARE_INTERFACE();

				void execute(void);
				virtual core::async::iasync<void> then(core::delegates::function<void(core::async::iasync<void>)> func);
				virtual bool wait(core::async::async_action_status_t state)const;
				virtual bool wait(core::async::async_action_status_t state, dword ms)const;
				virtual core::async::async_action_status_t status()const;
				virtual bool cancel();
				virtual void result()const;

				core::async::cond& cond()const { return m_cond; }
				core::async::mutex& mutex()const { return m_mutex; }

			private:
				virtual~async_task();
			};


		}
	}
}

#endif//__ASYNCMSGTASK_H__
