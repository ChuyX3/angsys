#pragma once
#include "angsys.h"
#include "ang_inlines.h"
#include "ang/core/async.h"

#if defined ANDROID_PLATFORM
typedef void *(*thread_start_routine_t)(void*);
//typedef struct _pthread_handler
//{
//	pthread_t _thread;
//	pthread_attr_t _attr;
//}pthread_handler,*pthread_handler_t;

#endif

namespace ang
{
	namespace core
	{
		namespace async
		{
			
			typedef struct _thread_handler
			{
				dword _id;
				pointer _handle;
				thread_t _thread;
				bool _is_main_thread;
				async_action_status_t _status;
				thread_callback_t _callback;
				pointer _args;
			}*thread_handler_t;

			class thread_manager;
			typedef object_wrapper<thread_manager> thread_manager_t;

			class thread_manager
				: public object
				, public singleton<thread_manager_t>
			{
				friend singleton<thread_manager_t>;
			private:
				ang_hash_table<uint,thread_handler_t> _thread_map;

			public:
				cond_t main_cond;
				mutex_t main_mutex;
				thread_t main_thread;

			public:
				thread_manager();
	
			public:
				static unsigned long STDCALL thread_callback_procedure(pointer args);
				static dword get_current_thread_id();
				static thread_t get_main_thread();
				static thread_handler_t create_thread(thread_t, thread_callback_t, pointer, thread_priority_t priority, detach_state_t ds);
				//static dword_t get_thread_id(handle_t);

				ANG_DECLARE_INTERFACE();
				bool register_thread(thread_handler_t);
				bool unregister_thread(thread_handler_t);
				thread_handler_t find_thread(dword id)const;
				thread_handler_t find_thread(pointer handle)const;
		
			private:
				virtual~thread_manager();

			};
			
			typedef object_wrapper<thread_manager> thread_manager_t;
		}
	}

	template<> core::async::thread_manager* ang::singleton<core::async::thread_manager_t>::instance();
	template<> bool ang::singleton<core::async::thread_manager_t>::release_instance();
}