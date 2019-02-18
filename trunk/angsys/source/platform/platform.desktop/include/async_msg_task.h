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
		//	class msg_dispatcher;
			class async_msg_task;

			//typedef object_wrapper<msg_dispatcher> msg_dispatcher_t;
			typedef object_wrapper<async_msg_task> async_msg_task_t;

			class async_msg_task final
				: public object
				, public core::async::itask<void>
			{
			public:
				events::message m_msg;
				imessage_listener* m_parent;
				mutable bool m_canceled;
				mutable shared_ptr<core::async::cond> m_cond;
				mutable shared_ptr<core::async::mutex> m_mutex;
				mutable core::async::async_action_status_t m_status;

			public:
				async_msg_task(imessage_listener*, core::async::cond_ptr_t cond, core::async::mutex_ptr_t, events::message);
			
			public: //Overrides
				ANG_DECLARE_INTERFACE();

				bool wait(core::async::async_action_status_t)const override;
				bool wait(core::async::async_action_status_t, dword)const override;
				core::async::async_action_status_t status()const override;
				bool cancel()override;
				void result()const override;
				void complete(dword);

			private:
				virtual~async_msg_task();
			};


	

		}
	}
}

#endif//__ASYNCMSGTASK_H__
