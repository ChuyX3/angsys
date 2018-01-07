#pragma once
#include "ang/core/async.hpp"
#include "angc/core_hash_table.h"

#include "ang/collections/queue.hpp"

namespace ang
{
	namespace core
	{
		namespace async
		{
			
			typedef object_wrapper<class core_thread> core_thread_t;
			typedef object_wrapper<class dispatcher_thread> dispatcher_thread_t;
			typedef object_wrapper<class async_task> async_task_t;

			class core_thread
				: public thread
			{
				friend class core_thread_manager;
				static dword core_thread_start_routine(pointer);
			public:
				core_thread();
				core_thread(wsize flags, ibuffer_view_t data, bool alloc);

			public: //overrides
				//ANG_DECLARE_INTERFACE();

				virtual bool is_main_thread()const override;
				virtual bool is_current_thread()const override;
				virtual ibuffer_view_t tle_buffer()const override;
				virtual void set_tle_data(ibuffer_view_t, bool alloc = false) override;
				virtual void set_tle_notify(tle_deleting_event_t callback) override;
				virtual dword thread_id()const override;
				virtual async_action_status_t status()const override;
				virtual bool start(thread_routine_t callback, var_args_t args) override;
				virtual bool then(thread_routine_t callback, var_args_t args) override;
				virtual bool wait(async_action_status_t)const override;
				virtual bool wait(async_action_status_t, dword ms)const override;
				virtual bool cancel() override;
				virtual bool join()const override;

			protected:
				bool _is_main;
				mutable bool _join_request;
				ibuffer_view_t _tle_data;
				tle_deleting_event_t tle_notify_callback;
				mutable async_action_status_t _state;
				var_args_t _user_args;
				thread_routine_t _start_routine;
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
				pthread_t _thread;
#elif defined WINDOWS_PLATFORM
				DWORD _id;
				HANDLE _hthread;
#endif
			protected:
				virtual~core_thread();
			};

			class core_thread_manager
				: public singleton<core_thread_manager>
			{
			public:
				friend singleton<core_thread_manager>;
				static ulong64 hash_table_context_create_hash_number(dword key, ulong64 size);
				static void hash_table_context_delete_data(ang_hash_table_pair_t pair);

			private:
				ang_unorder_map<dword, core_thread*> _thread_map;
				mutable cond_t _main_cond;
				mutable mutex_t _main_mutex;
				core_thread_t _main_thread;

			public:
				core_thread_manager();
				virtual ~core_thread_manager();

			public:
				static dword this_thread_id();
				core_thread_t main_thread()const;
				cond_t& main_cond()const { return _main_cond; }
				mutex_t& main_mutex()const { return _main_mutex; }

				core_thread_t this_thread()const;
				core_thread_t attach_this_thread(core_thread_t, bool is_main, ibuffer_view_t data, bool alloc);
				core_thread_t regist_thread(core_thread_t thread);
				core_thread_t unregist_thread(core_thread_t thread);
			};

			class dispatcher_thread
				: public core_thread
				, public idispatcher
			{
				friend async_task;
				friend class core_thread_manager;
				static dword core_thread_start_routine(pointer);

			public:
				dispatcher_thread();

			public: //overrides
				//ANG_DECLARE_INTERFACE();
				virtual icore_thread_t worker_thread()const override;
				virtual bool resume()override;
				virtual bool pause()override;
				virtual bool stop()override;
				virtual bool wait(async_action_status_t)const override;
				virtual bool wait(async_action_status_t, dword ms)const override;
				virtual bool cancel() override;
				virtual bool join()const override;

				virtual itask_t post_task(delegates::function <void(itask*)>) override;
				virtual itask_t post_task(delegates::function <void(itask*, var_args_t)>, var_args_t) override;

				virtual bool start(thread_routine_t callback, var_args_t args) override;
				virtual bool then(thread_routine_t callback, var_args_t args) override;

				void start_event(icore_thread*, var_args_t);
				void end_event(icore_thread*, var_args_t);

				virtual dword release()override;

			protected:
				mutable cond_ptr_t cond_;
				mutable mutex_ptr_t mutex_;
				collections::queue<thread_routine_t> task_queue_;

				
			protected:
				virtual~dispatcher_thread();
			};

			class async_task
				: public task
			{
			private:
				friend task;

				bool dispatched;
				mutable bool handled;
				mutable shared_ptr<cond_t> cond_;
				mutable shared_ptr<mutex_t> mutex_;
				mutable async_action_status_t status_;

				icore_thread_t worker_thread;
				delegates::function<void(icore_thread*, var_args_t)> then_callback;
				//async_task_t then_task;	

			public:
				async_task();
				async_task(dispatcher_thread_t dispatcher);
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
