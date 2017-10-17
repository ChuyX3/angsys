/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_ASYNC_HPP__
#define __ANG_ASYNC_HPP__

#include <angsys.h>
#include<ang/core/delegates.hpp>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace core
	{
		namespace async
		{
			struct mutex;
			struct cond;
			class waiter;
			class thread;
			struct iasync_task;
			template<typename result_t>struct iasync;
			//template<typename return_t>class async_task;
			template<typename result_t> class async_task_result;

			typedef mutex mutex_t;
			typedef cond cond_t;
			//typedef object_wrapper<thread> thread_t;
			typedef intf_wrapper<iasync_task> iasync_task_t;
			typedef object_wrapper<thread> thread_t;

			ANG_INTERFACE(icore_thread);
			ANG_INTERFACE(idispatcher);

			typedef pointer handle;

			//typedef object_wrapper<dispatcher_thread> dispatcher_thread_t;

			template <typename result_t>
			using iasync_t = intf_wrapper<iasync<result_t>>;

			//template <typename result_t>
			//using async_task_t = object_wrapper<async_task<result_t>>;

			template<typename result_t>
			using async_task_result_t = object_wrapper<async_task_result<result_t>>;

			typedef delegates::function<dword(thread_t, var_args_t)> thread_start_routine_t;
			typedef delegates::function<dword(thread_t, var_args_t)> thread_then_routine_t;
			typedef delegates::function<void(thread_t, ibuffer_view_t)> tle_deleting_event_t;
			//typedef delegates::listener<void(icore_thread_t, wsize ,pointer)> tle_deleting_event_handler_t;


			/******************************************************************/
			/* falg ang::core::async::async_action_status                     */
			/*  -> represents the thread status                               */
			/******************************************************************/
			ANG_BEGIN_FLAGS(LINK, async_action_status, uint)
				initializing = 0,
				wait_for_start = 1,
				running = 1 << 1,
				wait_for_then = 1 << 2,
				working = running | wait_for_then,
				//finishing = 1 << 3,
				completed = 1 << 3,
				canceled = 1 << 4,
				finished = canceled | completed,
				stopped = finished | wait_for_start,
				not_running = finished | wait_for_start | wait_for_then,
				attached = 1 << 5,
				error = 0XFF00,
			ANG_END_FLAGS(async_action_status);


			ANG_BEGIN_INTERFACE(LINK, icore_thread)
				visible vcall bool is_main_thread()const pure;
				visible vcall bool is_current_thread()const pure;
				visible vcall ibuffer_view_t tle_buffer()const pure;		
				visible vcall void set_tle_data(ibuffer_view_t, bool alloc = false) pure;
				visible vcall void set_tle_notify(tle_deleting_event_t callback) pure;
				visible vcall var_args_t user_args()const pure;
				visible vcall dword thread_id()const pure;
				visible vcall async_action_status_t thread_state()const pure;
				visible vcall bool start(thread_start_routine_t callback, var_args_t args) pure;
				visible vcall bool then(thread_then_routine_t callback, var_args_t args) pure;
				visible vcall bool wait(async_action_status_t) pure;
				visible vcall bool wait(async_action_status_t, dword ms) pure;
				visible vcall bool cancel() pure;
				visible vcall bool join() pure;
				template<typename T> T* tle_data()const { return (!tle_buffer().is_empty() && tle_buffer()->buffer_size() == sizeof(T)) ? (T*)tle_buffer()->buffer_ptr() : NULL; }
			ANG_END_INTERFACE();


			ANG_BEGIN_INTERFACE(LINK, iasync_task)
				visible vcall bool wait(async_action_status_t, dword)const pure;
				visible vcall async_action_status_t status()const pure;
				visible vcall bool cancel()pure;
				visible vcall iasync_task_t then(delegates::function<void(iasync_task*)>)pure;
				//visible vcall bool suspend()pure;
				//visible vcall bool resume()pure;
			ANG_END_INTERFACE();


			/******************************************************************/
			/* interface ang::core::async::iasync< result_t >                 */
			/*  -> represents a asynchronous operation, it can be waited for  */
			/*     its result or any status change                            */
			/******************************************************************/
			template<typename result_t>
			ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(iasync, public iasync_task)
				visible vcall void complete(result_t) pure;
				visible vcall result_t result()const pure;
				visible vcall void then(delegates::function<void(iasync<result_t>*)>)pure;
				inherit using iasync_task::then;
			ANG_END_INTERFACE();


			template<>
			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, iasync<void>, public iasync_task)
				visible vcall void complete()pure;
				visible vcall void completed()const pure;
				visible vcall iasync_task_t then(delegates::function<void(iasync<void>*)>, iasync_task_t = null)pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE(LINK, idispatcher)
				visible vcall icore_thread_t worker_thread()const pure;
				visible vcall bool post_task(delegates::function<void(void)>()) pure;
				visible template<typename T> iasync_t<T> run_async(delegates::function<T(iasync<T>*, var_args_t)> callback, var_args_t args);
				visible template<typename T, typename... Ts> iasync_t<T> run_async(delegates::function<T(iasync<T>*, var_args_t)> callback, Ts... args);
				visible	delegates::listener<void(idispatcher_t)> start_event;
				visible delegates::listener<void(idispatcher_t)> end_event;
			ANG_END_INTERFACE();

		}//async
	}//core
}//ang


namespace ang
{
	template <typename result_t>
	class intf_wrapper<core::async::iasync<result_t>>
	{
	public:
		typedef core::async::iasync<result_t> type;

	protected:
		type* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(type*);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();

	public:
		void clean();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

		template<class then_result_t>
		core::async::iasync_t<then_result_t> then(core::delegates::function<then_result_t(core::async::iasync<result_t>*)> func);

		inline bool wait(core::async::async_action_status_t status, dword ms = -1)const {
			return is_empty() ? false : _ptr->wait(status, ms);
		}

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;
	};

	namespace core
	{
		namespace async
		{
			/******************************************************************/
			/* calss ang::core::async::mutex                                  */
			/*  -> represents a system asynchronous condition or event        */
			/******************************************************************/
			struct LINK mutex final
			{
			private: //Non copiable
				mutex(const mutex&) = delete;
				mutex& operator = (const mutex&) = delete;

			protected:
				handle _handle;

			public:
				mutex();
				mutex(bool lock);
				~mutex();

			public: //Properties
				bool lock()const;
				bool trylock()const;
				bool unlock()const;

				friend cond;
			};

			/******************************************************************/
			/* calss ang::core::async::scope_locker                           */
			/*  -> is a useful mutex locker limited to its cycle life         */
			/******************************************************************/
			struct scope_locker final
			{
			private:
				mutex& _mutex;

			public:
				inline scope_locker(mutex& m) : _mutex(m) {
					_mutex.lock();
				}
				inline ~scope_locker() {
					_mutex.unlock();
				}

				template<typename func_t>
				static auto lock(mutex_t& m, func_t func) -> decltype(func())
				{
					scope_locker _lock = m;
					return func();
				}
			};

			/******************************************************************/
			/* calss ang::core::async::cond                                   */
			/*  -> represents a system condition                              */
			/******************************************************************/
			struct LINK cond final
			{
			private: //Non copiable
				cond(const cond&);
				cond& operator = (const cond&);

			protected:
				handle _handle;

			public:
				cond();
				~cond();

			public: //Properties
				bool wait(mutex_t& mutex)const;
				bool wait(mutex_t& mutex, dword ms)const;
				bool signal()const;

				template<typename func_t>
				bool waitfor(mutex_t& mutex, dword ms, func_t f) {
					if (f()) { wait(mutex, ms); }
					return !f();
				}
				template<typename func_t>
				void waitfor(mutex_t& mutex, func_t f) {
					while (f()) { wait(mutex); }
				}
			};
		}
	}


	namespace core
	{
		namespace async
		{
			class LINK thread
				: public object
				, public icore_thread
			{
			public:
				static thread_t create_thread_suspended(uint flags, ibuffer_view_t, bool alloc);
				static thread_t create_thread(thread_start_routine_t routine, var_args_t args, uint flags, ibuffer_view_t, bool alloc);
				static void sleep(dword ms);
				static thread_t main_thread();
				static thread_t this_thread();
				static dword this_thread_id();

			protected:
				thread();
				virtual~thread();

			public:
				ANG_DECLARE_INTERFACE();

			};
		}//async
	}//core


	namespace core
	{
		namespace async
		{
			template<typename T> class async_task_result;
			template<typename T> using async_task_result_t = object_wrapper<async_task_result<T>>;

			class LINK async_task 
				: public object
				, public iasync_task
			{
			public:
				template<typename T> static iasync_t<T> run_async(delegates::function < T(iasync<T>*, var_args_t)>, var_args_t = null);
				template<typename T> static iasync_t<T> run_async(async_task_result<T>*, delegates::function < T(iasync<T>*, var_args_t)>, var_args_t = null);
				template<typename T> static iasync_t<T> run_sync(async_task_result<T>*, delegates::function < T(iasync<T>*, var_args_t)>, var_args_t = null);

			protected:
				mutable cond_t _cond;
				mutable mutex_t _mutex;
				icore_thread_t _thread;
				async_action_status_t _status;

			public:
				async_task();

			public:
				ANG_DECLARE_INTERFACE();
				virtual bool wait(async_action_status_t, dword)const override;
				virtual async_action_status_t status()const override;
				virtual bool cancel()override;
				virtual void then(delegates::function<void(iasync_task*)>)override;

			public:
				cond_t& cond()const;
				mutex_t& mutex()const;
				icore_thread_t thread()const;
				async_action_status_t status()const;

			protected:
				virtual~async_task();
			};

			template<typename T>
			class async_task_result
				: public object
				, public iasync<T>
			{
			protected:
				T _result;
				object_wrapper<async_task> _task;

			public:
				inline async_task_result(async_task* task);
				//inline async_task_result(thread_t, mutex_t, cond_t);

			protected:
				inline virtual~async_task_result();

				async_task_result(const async_task_result<T>&) = delete;
				async_task_result& operator =(const async_task_result<T>&) = delete;

			public:
				ANG_DECLARE_INTERFACE();

				inline bool wait(async_action_status_t, dword)const override;
				inline async_action_status_t status()const override;
				inline bool cancel() override;
				//inline bool suspend() override;
				//inline bool resume() override;
				inline void complete(T) override;
				inline T result()const override;
				inline iasync_task_t then(delegates::function<void(iasync<T>*)>, iasync_task_t = null)override;

				friend async_task<T>;
			};

		}
	}
}

#include <ang/core/inlines/async.inl>

#endif//__ANG_ASYNC_HPP__