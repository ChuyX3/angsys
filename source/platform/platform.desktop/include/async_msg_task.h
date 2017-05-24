#ifndef __ASYNCMSGTASK_H__
#define __ASYNCMSGTASK_H__

#include <ang/platform/angwin/angwin.h>
//#include <Queue.h>

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			class msg_dispatcher;
			class async_msg_task;

			typedef object_wrapper<msg_dispatcher> msg_dispatcher_t;
			typedef object_wrapper<async_msg_task> async_msg_task_t;

			class async_msg_task final
				: public object
				, public core::async::iasync<dword>
			{
			public:
				events::message_t _msg;
				imessage_reciever* _parent;
				core::async::mutex_t _mutex;
				core::async::async_action_status_t _status;
				mutable core::async::cond_t _cond;
				dword _result;
				bool _handled;
				core::async::async_task_result<void>* _then_task;
				core::delegates::function<void(core::async::iasync<dword>*)> _then;

			public:
				async_msg_task(imessage_reciever*, core::async::cond_t cond, core::async::mutex_t, events::message_t);
			
			public: //Overrides
				ANG_DECLARE_INTERFACE();

				virtual bool wait(core::async::async_action_status_t, dword)const override;
				virtual dword result()const override;
				virtual core::async::async_action_status_t status()const override;
				virtual bool cancel()override;
				virtual bool suspend()override;
				virtual bool resume()override;
				virtual void then(core::delegates::function<void(core::async::iasync<dword>*)>)override;

				void complete();

			private:
				virtual~async_msg_task();
				friend msg_dispatcher;
			};

		}
	}
}

#endif//__ASYNCMSGTASK_H__
