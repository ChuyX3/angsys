#pragma once

#include <coffe/core/async.h>

namespace coffe
{
	namespace core
	{
		namespace async
		{
			coffe_declare_object(sync_task);
			coffe_declare_object(thread_task);
			coffe_declare_object(dispatcher);
			coffe_declare_object(core_thread);
			coffe_declare_object(worker_thread);

			class sync_task
				: public implement<sync_task
				, iid("coffe::core::asyc::sync_task")
				, sync>
			{
			private: /*fields*/
				mutable cond_t m_cond;
				mutable mutex_t m_mutex;
				mutable mutex_t m_shared_mutex;
				collections::hash_set<iasync<void>> m_task_set;

			public:
				sync_task();

			public: /*overrides*/
				void run(function<void(isync_t)>);
				virtual void run_sync(function<void(isync_t)>) override;
				virtual mutex_t& shared_mutex()const override;
			
			protected:		
				virtual~sync_task();
			};

			class thread_task
				: public implement<thread_task
				, iid("coffe::core::asyc::thread_task")
				, task>
			{
			public:
				thread_task(core_thread_t);

			protected: //overrides
				virtual void dispose()override;

			public: //overrides
				virtual iasync<void> then(function<void(iasync<void>)> func);
				virtual bool wait(async_action_status_t state)const;
				virtual bool wait(async_action_status_t state, dword ms)const;
				virtual async_action_status_t status()const;
				virtual bool cancel();
				virtual void result()const;

				void run();
				async::cond& cond()const { return m_cond; }
				async::mutex& mutex()const { return m_mutex; }
				function<void(iasync<void>)> action;

				bool m_was_canceled;
				mutable async_action_status_t m_status;
				mutable async::cond_t m_cond;
				mutable async::mutex_t m_mutex;

				thread_task_t m_parent_task;
				thread_task_t m_child_task;
				core_thread_t m_thread;

			private:
				virtual~thread_task();
			};

			class core_thread
				: public implement<core_thread
				, iid("coffe::core::asyc::core_thread")
				, thread>
			{
				friend class thread_manager;
			protected:
				static int entry_point(core_thread*);

			public:
				core_thread();

			protected: /*overrides*/
				virtual void dispose()override;

			public: /*overrides*/
				virtual bool is_this_thread()const override;
				virtual bool is_main_thread()const override;
				virtual dword thread_id()const override;
				virtual async_action_status_t status()const override;
				virtual void exit()override;
				virtual void join()override;
				virtual bool attach();
				virtual bool detach();

				virtual bool start() { return false; }
				virtual thread_task_t post_task(thread_task_t) { return null; }
				virtual thread_task_t post_task(function<void(iasync<void>)>) { return null; }
				virtual int dispatch() { return 0; }

			protected:
				bool m_is_main_thread;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
				pthread_t m_thread;
#elif defined WINDOWS_PLATFORM
				dword m_id;
				pointer m_thread;
#endif

			protected:
				virtual~core_thread();
			};

			class worker_thread
				: public implement<worker_thread
				, iid("coffe::core::asyc::worker_thread")
				, core_thread>
			{
				friend class thread_manager;

			public:
				worker_thread();

			protected: //overrides
				virtual void dispose()override;
				virtual bool auto_release()override;

			public: //overrides
				virtual async_action_status_t status()const override;
				virtual void exit()override;
				virtual void join()override;
				virtual bool attach()override;
				virtual bool detach()override;

				virtual thread_task_t post_task(thread_task_t)override;
				virtual thread_task_t post_task(function<void(iasync<void>)>)override;
				virtual int dispatch()override;
				virtual bool start()override;

			protected:
				thread_task_t m_task; //only one task at a time
				mutable bool m_was_joined;
				mutable async_action_status_t m_state;
				mutable cond_t m_cond;
				mutable mutex_t m_mutex;

			protected:
				virtual~worker_thread();
			};

			class dispatcher
				: public implement<dispatcher
				, iid("coffe::core::asyc::dispatcher")
				, core_thread
				, idispatcher>
			{
				friend class thread_manager;

			public:
				dispatcher();

			protected: //overrides
				virtual void dispose()override;
				virtual bool auto_release()override;

			public: //overrides
				virtual bool has_thread_access()const override;
				virtual async_action_status_t status()const override;
				virtual void exit()override;
				virtual void join()override;
				virtual listen_token<void(time::step_timer const&)> add_idle_event(function<void(time::step_timer const&)>)override;
				virtual bool remove_idle_event(listen_token<void(time::step_timer const&)>)override;
				virtual iasync<void> run_async(function<void(iasync<void>)>)override;
				virtual thread_task_t post_task(thread_task_t)override;
				virtual thread_task_t post_task(function<void(iasync<void>)>)override;
				virtual int dispatch()override;
				virtual bool start()override;
				virtual bool attach()override;
				virtual bool detach()override;

			protected:
				time::step_timer m_timer;
				listener<void(time::step_timer const&)> m_idle_event;
				queue<thread_task_t> m_tasks;
				mutable bool m_was_joined;
				mutable async_action_status_t m_state;
				mutable cond_t m_cond;
				mutable mutex_t m_mutex;

			protected:
				virtual~dispatcher();
			};

			class thread_manager
				: public singleton<thread_manager, memory::unmanaged_allocator>
			{
			private:
				collections::hash_map<dword, core_thread*, memory::unmanaged_allocator> m_thread_map;
				vector<core_thread_t, memory::unmanaged_allocator> m_attached_threads;
				mutable cond_t m_main_cond;
				mutable mutex_t m_main_mutex;
				core_thread_t m_main_thread;

			public:
				thread_manager();
				virtual~thread_manager();

			public:
				static dword this_thread_id();
				core_thread_t main_thread()const;
				inline cond_t& main_cond()const { return m_main_cond; }
				inline mutex_t& main_mutex()const { return m_main_mutex; }

				core_thread_t this_thread()const;
				core_thread_t attach_this_thread(core_thread_t, bool ismain);
				core_thread* regist_thread(core_thread* thread);
				core_thread* unregist_thread(core_thread* thread);
			};
		}
	}
}