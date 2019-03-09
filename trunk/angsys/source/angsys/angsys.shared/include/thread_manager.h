#include <ang/core/async.h>
#include <ang/collections/list.h>
#include "core_hash_table.h"

namespace ang
{
	namespace core
	{
		namespace async
		{
			ang_object(thread_task);
			ang_object(worker_thread);

			class thread_task
				: public smart<thread_task, task>
			{
				friend worker_thread;

			public:
				thread_task(worker_thread_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();

				virtual void clear()override;
				virtual iasync<void> then(core::delegates::function<void(iasync<void>)> func);
				virtual bool wait(async_action_status_t state)const;
				virtual bool wait(async_action_status_t state, dword ms)const;
				virtual async_action_status_t status()const;
				virtual bool cancel();
				virtual void result()const;

				void run();

				async::cond& cond()const { return m_cond; }
				async::mutex& mutex()const { return m_mutex; }
				delegates::listener<void(iasync<void>)> action;

			protected:
				bool m_was_canceled;
				mutable async_action_status_t m_status;
				mutable async::cond_t m_cond;
				mutable async::mutex_t m_mutex;

				thread_task_t m_parent_task;
				thread_task_t m_child_task;
				worker_thread_t m_thread;


			private:
				virtual~thread_task();
			};


			class worker_thread
				: public smart< worker_thread,thread>
			{
				friend class thread_manager;

			protected:
				static void entry_point(worker_thread*);

			public:
				worker_thread();

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual bool auto_release()override;
				virtual void clear()override;

				virtual iasync<void> run_async(core::delegates::function<void(iasync<void>)>)override;
				//virtual iasync<void> run_async(core::delegates::function<void(iasync<void>, var_args_t)>, var_args_t)override;

				virtual bool is_main_thread()const override;
				virtual bool has_thread_access()const override;
				virtual dword thread_id()const override;
				virtual void join()const override;

				thread_task_t post_task(thread_task_t);
				thread_task_t post_task(core::delegates::function<void(iasync<void>)>);
				void dispatch();

				bool start();
				bool attach();

			protected:
				bool m_is_main_thread;
				//bool m_auto_release;
				collections::list<thread_task_t> m_tasks;
				mutable async_action_status_t m_state;

#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
				pthread_t _thread;
#elif defined WINDOWS_PLATFORM
				dword m_id;
				pointer m_thread;
#endif


			protected:
				virtual~worker_thread();
			};


			class thread_manager
				: public singleton<thread_manager, memory::unmanaged_allocator>
			{
			private:
				collections::internal_hash_map<dword, worker_thread*> _thread_map;
				collections::vector<worker_thread_t, memory::unmanaged_allocator> _attached_threads;
				mutable cond_t _main_cond;
				mutable mutex_t _main_mutex;
				worker_thread_t _main_thread;

			public:
				thread_manager();
				virtual~thread_manager();

			public:
				static dword this_thread_id();
				worker_thread_t main_thread()const;
				inline cond_t& main_cond()const { return _main_cond; }
				inline mutex_t& main_mutex()const { return _main_mutex; }

				worker_thread_t this_thread()const;
				worker_thread_t attach_this_thread(worker_thread_t, bool ismain);
				worker_thread* regist_thread(worker_thread* thread);
				worker_thread* unregist_thread(worker_thread* thread);
			};
		}
	}
}