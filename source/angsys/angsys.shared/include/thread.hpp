#pragma once
#include "ang/core/async.hpp"
#include "angc/core_hash_table.h"

namespace ang
{
	namespace core
	{
		namespace async
		{
			
			typedef object_wrapper<class core_thread> core_thread_t;

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
				virtual var_args_t user_args()const override;
				virtual dword thread_id()const override;
				virtual async_action_status_t thread_state()const override;
				virtual bool start(thread_start_routine_t callback, var_args_t args) override;
				virtual bool then(thread_then_routine_t callback, var_args_t args) override;
				virtual bool wait(async_action_status_t) override;
				virtual bool wait(async_action_status_t, dword ms) override;
				virtual bool cancel() override;
				virtual bool join() override;

			private:
				bool _is_main;
				bool _then_request;
				ibuffer_view_t _tle_data;
				tle_deleting_event_t tle_notify_callback;
				async_action_status_t _state;
				var_args_t _user_args;
				thread_start_routine_t _start_routine;
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
		}


	}
}
