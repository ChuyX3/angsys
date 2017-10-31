#pragma once
#include "ang/core/async.hpp"

namespace ang
{
	namespace core
	{
		namespace async
		{
			typedef object_wrapper<class async_task> async_task_t;

			class async_task
				: public task
			{
			private:
				friend task;

				mutable bool handled;
				mutable shared_ptr<cond_t> cond_;
				mutable shared_ptr<mutex_t> mutex_;
				mutable async_action_status_t status_;

				icore_thread_t worker_thread;
				delegates::function<void(icore_thread*, var_args_t)> then_callback;
				//async_task_t then_task;	

			public:
				async_task();
				async_task(async_task_t parent);


				void run(delegates::function <void(itask*)> func);
				void run(delegates::function <void(itask*, var_args_t)> func, var_args_t args);

			public: //overrides
				//ANG_DECLARE_INTERFACE();

				bool wait(async_action_status_t)const override;
				bool wait(async_action_status_t, dword)const override;
				async_action_status_t status()const override;
				bool cancel()override;
				bool join()const override;
				itask_t then(delegates::function<void(itask*)>)override;

			protected:
				virtual~async_task();
			};
			

		

		}


	}
}
