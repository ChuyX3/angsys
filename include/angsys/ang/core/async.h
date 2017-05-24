/*********************************************************************************************************************/
/*   File Name: ang/async.h                                                                                          */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file exposes threading management as well as useful objects and operations for           */
/*   synchronization.                                                                                                */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_ASYNC_H__
#define __ANG_ASYNC_H__

#include <angsys.h>
#include <ang/core/delegates.h>

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
			class mutex;
			class cond;
			class thread;
			class waiter;
			struct iasync_task;
			class dispatcher_thread;
			template<typename result_t>struct iasync;
			template<typename return_t>class async_task;
			template<typename result_t> class async_task_result;

			typedef object_wrapper<mutex> mutex_t;
			typedef object_wrapper<cond> cond_t;
			typedef object_wrapper<waiter> waiter_t;
			typedef object_wrapper<thread> thread_t;
			typedef intf_wrapper<iasync_task> iasync_task_t;
			typedef object_wrapper<dispatcher_thread> dispatcher_thread_t;

			template <typename result_t>
			using iasync_t = intf_wrapper<iasync<result_t>>;

			template <typename result_t>
			using async_task_t = object_wrapper<async_task<result_t>>;

			template<typename result_t>
			using async_task_result_t = object_wrapper<async_task_result<result_t>>;

			typedef delegates::function<dword(pointer)> thread_callback_t;
			
			/******************************************************************/
			/* enum ang::core::async::thread_priority :                       */
			/*  -> represents the thread priority                             */
			/******************************************************************/
			ANG_BEGIN_ENUM(LINK, thread_priority, word)
				low,
				normal,
				high,
			ANG_END_ENUM(thread_priority);

			/******************************************************************/
			/* enum ang::core::async::detach_state                            */
			/*  -> represents a thread capability                             */
			/******************************************************************/
			ANG_BEGIN_ENUM(LINK, detach_state, word)
				joinable = 0,
				detached = 1
			ANG_END_ENUM(detach_state);

			/******************************************************************/
			/* falg ang::core::async::async_action_status                     */
			/*  -> represents the thread status                               */
			/******************************************************************/
			ANG_BEGIN_FLAGS(LINK, async_action_status, uint)
				none = 0,
				starting = 0X01,
				running = 0X02,
				suspended = 0X04,
				canceled = 0X08,
				completed = 0X10,
				finished = canceled | completed,
				stopped = finished | starting,
				not_running = stopped | suspended,
				error = 0XFF00,
			ANG_END_FLAGS(async_action_status);

			ANG_BEGIN_INTERFACE(LINK, iasync_task)
				visible vcall bool wait(async_action_status_t, dword)const pure;
				visible vcall async_action_status_t status()const pure;
				visible vcall bool cancel()pure;
				visible vcall bool suspend()pure;
				visible vcall bool resume()pure;
			ANG_END_INTERFACE();


			/******************************************************************/
			/* interface ang::core::async::iasync< result_t >                 */
			/*  -> represents a asynchronous operation, it can be waited for  */
			/*     its result or any status change                            */
			/******************************************************************/
			template<typename result_t>
			ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(iasync, public iasync_task)
				visible vcall result_t result()const pure;
				visible vcall void then(delegates::function<void(iasync<result_t>*)>)pure;
			ANG_END_INTERFACE();

			template<>
			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, iasync<void>, public iasync_task)
				visible vcall void result()const pure;
				visible vcall void then(delegates::function<void(iasync<void>*)>)pure;
			ANG_END_INTERFACE();

		}//async
	}//core
}//ang


ANG_DECLARE_INTERFACE_VECTOR_SPECIALIZATION(LINK, ang::core::async::iasync_task);
ANG_DECLARE_OBJECT_VECTOR_SPECIALIZATION(LINK, ang::core::delegates::function_data<dword(pointer)>);

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

	/******************************************************************/
	/* calss ang::core::async::mutex_t                                */
	/*  -> represents a system mutex                                  */
	/******************************************************************/
	template<> class LINK object_wrapper<core::async::mutex>
	{
	public:
		typedef core::async::mutex type;

	protected:
		core::async::mutex* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		object_wrapper(core::async::mutex*);
		~object_wrapper();

	public:
		void clean();
		bool is_empty()const;
		core::async::mutex* get(void)const;
		void set(type*);

		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (core::async::mutex*);

		operator core::async::mutex*();
		operator core::async::mutex const*()const;
		core::async::mutex* operator -> ()const;

	};

	/******************************************************************/
	/* calss ang::core::async::mutex_t                                */
	/*  -> represents a system asynchronous condition or event        */
	/******************************************************************/
	template<> class LINK object_wrapper<core::async::cond>
	{
	public:
		typedef core::async::cond type;

	protected:
		core::async::cond* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		object_wrapper(core::async::cond*);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		core::async::cond* get(void)const;
		void set(core::async::cond*);

		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (core::async::cond*);

		operator core::async::cond*();
		operator core::async::cond const*()const;
		core::async::cond* operator -> ()const;

	};

	/******************************************************************/
	/* calss ang::core::async::waiter_t                               */
	/*  -> object that can wait for multiple asinc operations         */
	/******************************************************************/
	template<> class LINK object_wrapper<core::async::waiter>
	{
	public:
		typedef core::async::waiter type;

	protected:
		core::async::waiter* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		object_wrapper(core::async::waiter*);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		core::async::waiter* get(void)const;
		void set(core::async::waiter*);

		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (core::async::waiter*);
		object_wrapper& operator += (core::async::iasync_task_t);

		operator core::async::waiter*();
		operator core::async::waiter const*()const;
		core::async::waiter* operator -> ()const;

	};


	namespace core
	{
		namespace async
		{
			/******************************************************************/
			/* calss ang::core::async::mutex                                  */
			/*  -> is the mutex_t internal implementation                     */
			/******************************************************************/
			class LINK mutex final
				: public object
			{
			private: //Non copiable
				mutex(const mutex&) = delete;
				mutex& operator = (const mutex&) = delete;

			protected:
				pointer _handle;

			public:
				mutex();
				mutex(bool lock);
				virtual~mutex();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

			public: //Properties
				pointer handle()const;
				bool lock()const;
				bool try_lock()const;
				bool unlock()const;
			};


			/******************************************************************/
			/* calss ang::core::async::scope_locker                           */
			/*  -> is a useful mutex locker limited to its cycle life         */
			/******************************************************************/
			class scope_locker final
			{
			private:
				mutex_t _mutex;

			public:
				inline scope_locker(mutex_t m) : _mutex(m) {
					if (!_mutex.is_empty()) {
						_mutex->lock();
					}
				}

				inline ~scope_locker() {
					if (_mutex) {
						_mutex->unlock();
						_mutex = null;
					}
				}

				template<typename func_t>
				static void lock(mutex_t m, func_t func)
				{
					scope_locker _lock = m;
					func();
				}
			};

			

			/******************************************************************/
			/* calss ang::core::async::cond                                   */
			/*  -> is the cond_t internal implementation                      */
			/******************************************************************/
			class LINK cond final
				: public object
			{
			private: //Non copiable
				cond(const cond&);
				cond& operator = (const cond&);

			protected:
				pointer _handle;

			public:
				cond();
				virtual~cond();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

			public: //Properties
				bool wait(mutex_t mutex)const;
				bool wait(mutex_t mutex, dword ms)const;
				bool signal()const;

				template<typename func_t>
				bool waitfor(mutex_t mutex, dword ms, func_t f)
				{
					if (f()) { wait(mutex, ms); }
					return !f();
				}
				template<typename func_t>
				void waitfor(mutex_t mutex, func_t f)
				{
					while (f()) { wait(mutex); }
				}

			};


			/******************************************************************/
			/* calss ang::core::async::waiter                                 */
			/*  -> is the waiter_t internal implementation                    */
			/******************************************************************/
			class LINK waiter final
				: public object
			{
			private: //Non copiable
				waiter(const waiter&);
				waiter& operator = (const waiter&);

			protected:
				collections::vector<iasync_task_t> _handle;

			public:
				waiter();
				virtual~waiter();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

			public: //Properties
				bool add(iasync_task_t);
				bool wait_all(async_action_status_t, dword ms)const;
				void clean();
			};
		}//async
	}//core
}//ang


namespace ang
{
	/******************************************************************/
	/* calss ang::core::async::thread_t                               */
	/*  -> represents a system worker thread                          */
	/******************************************************************/
	template<> class LINK object_wrapper<core::async::thread>
	{
	public:
		typedef core::async::thread type;

	protected:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		object_wrapper(type*);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);

		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (type*);

		operator type*();
		operator type const*()const;

		type* operator -> ();
		type const* operator -> ()const;

	};

	namespace core
	{
		namespace async
		{

			/******************************************************************/
			/* calss ang::core::async::thread                                 */
			/*  -> is the thread_t internal implementation                    */
			/******************************************************************/
			class LINK thread
				: public object
				, public iasync_task
			{
			public:
				static void sleep(dword ms);
				static thread_t current_thread(thread_t = null);
				static dword current_thread_id();
				static thread_t main_thread();

				template<typename calleable_t, typename... args_t>
				static thread_t create_worker_thread(calleable_t callback, args_t... args) {
					thread_t worker = new thread();
					if(!worker->start([=](void_args_t)->dword {
						callback(args...);
						return 0U;
					}, null)) return null;
					worker->wait(async_action_status::running, -1);
					return worker;
				}

			protected:
				pointer _handle;

			public:
				thread();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				virtual bool start(thread_callback_t callback, void_args_t args
					, thread_priority_t priority = thread_priority::normal
					, detach_state_t ds = detach_state::joinable);

				pointer handle()const;
				bool is_created()const;

			public:
				virtual bool wait(async_action_status_t, dword)const override;
				virtual async_action_status_t status()const override;
				virtual bool cancel()override;
				virtual bool suspend()override;
				virtual bool resume()override;

				bool is_main_thread()const;
				bool is_current_thread()const;

				void join();
				dword id()const;

			private: //Properties
				void dettach();

			protected:
				virtual~thread();
			};
		}//async
	}//core
}//ang


namespace ang
{
	/******************************************************************/
	/* calss ang::core::async::thread_t                               */
	/*  -> represents a system worker thread                          */
	/******************************************************************/
	template<> class LINK object_wrapper<core::async::dispatcher_thread>
	{
	public:
		typedef core::async::dispatcher_thread type;

	protected:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		object_wrapper(type*);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);

		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (type*);

		operator type*();
		operator type const*()const;

		type* operator -> ();
		type const* operator -> ()const;

	};

	template<> class LINK object_wrapper<core::delegates::function_data<void(core::async::iasync<void>*)>>
	{
	public:
		typedef core::delegates::function_data<void(core::async::iasync<void>*)> type;

	protected:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

		template<typename calleable_t>
		object_wrapper(calleable_t const& func) : object_wrapper() {
			set(new core::delegates::static_function<calleable_t, void, core::async::iasync<void>*>(func));
		}

		template<typename obj_t>
		object_wrapper(obj_t* obj, void(obj_t::*f)(void)) : object_wrapper() {
			set(new core::delegates::member_function<obj_t, void, core::async::iasync<void>*>(obj, f));
		}

	public:
		void clean();
		void clean_unsafe();
		bool_t is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

		void invoke(core::async::iasync<void>* args)const {
			if (is_empty())
				return;
			return get()->invoke(args);
		}

	public:
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (std::nullptr_t const&) {
			clean();
			return*this;
		}

		operator object_t()const {
			return _ptr;
		}

		template<typename calleable_t>
		object_wrapper& operator = (calleable_t func) {
			set(new core::delegates::static_function<calleable_t, void, core::async::iasync<void>*>(func));
			return*this;
		}

		object_wrapper_ptr<type> operator & (void);

		void operator()(core::async::iasync<void>* args)const {
			invoke(args);
		}

		friend safe_pointer;
	};

	namespace core
	{
		namespace async
		{
			template<typename T>
			class async_task_result final
				: public object
				, public iasync<T>
			{
			private:
				T _result;
				cond_t _cond;
				mutex_t _mutex;
				bool _was_canceled;
				bool _handled;
				thread_t _thread;
				async_action_status_t _status;
				delegates::function<void(iasync<T>*)> _then;

			public:
				inline async_task_result();
				inline async_task_result(thread_t, mutex_t, cond_t);

			private:
				inline virtual~async_task_result();

				async_task_result(const async_task_result<T>&) = delete;
				async_task_result& operator =(const async_task_result<T>&) = delete;

			public:
				ANG_DECLARE_INTERFACE();

				inline void complete();
				inline void complete(const T&);
				inline bool wait(async_action_status_t, dword)const override;
				inline async_action_status_t status()const override;
				inline bool cancel() override;
				inline bool suspend() override;
				inline bool resume() override;
				inline T result()const override;
				inline void then(delegates::function<void(iasync<T>*)>)override;

				friend async_task<T>;
				friend dispatcher_thread;
			};

			template<typename T>
			class async_task final
			{
			public:
				static iasync_t<T> run_async(delegates::function < T(iasync<T>*, var_args_t)>
					, var_args_t = null, thread_priority_t = thread_priority::normal);	

				static iasync_t<T> run_async(async_task_result<T>*, delegates::function < T(iasync<T>*, var_args_t)>
					, var_args_t = null, thread_priority_t = thread_priority::normal);


				static iasync_t<T> run_sync(async_task_result<T>*, delegates::function < T(iasync<T>*, var_args_t)>
					, var_args_t = null);

			//private:
			//	iasync<T>* _task;

			//public:


			};

			template<> class LINK async_task_result<void> final
				: public object
				, public iasync<void>
			{
			private:
				cond_t _cond;
				mutex_t _mutex;
				bool _was_canceled;
				bool _handled;
				thread_t _thread;
				async_action_status_t _status;
				delegates::function<void(iasync<void>*)> _then;

			public:
				inline async_task_result();

			private:
				inline virtual~async_task_result();

				async_task_result(const async_task_result<void>&) = delete;
				async_task_result& operator =(const async_task_result<void>&) = delete;

			public:
				ANG_DECLARE_INTERFACE();

				inline void complete();
				inline bool wait(async_action_status_t, dword)const override;
				inline async_action_status_t status()const override;
				inline bool cancel() override;
				inline bool suspend() override;
				inline bool resume() override;
				inline void result()const override;
				inline void then(delegates::function<void(iasync<void>*)>)override;

				friend async_task<void>;
			};

			template<> class LINK async_task<void> final
			{
			public:
				static iasync_t<void> run_async(delegates::function<void(iasync<void>*, var_args_t)>
					, var_args_t = null, thread_priority_t = thread_priority::normal);

				static iasync_t<void> run_async(async_task_result<void>*, delegates::function<void(iasync<void>*, var_args_t)>
					, var_args_t = null, thread_priority_t = thread_priority::normal);
			};


			class LINK dispatcher_thread final
				: public thread
			{
			private:
				cond_t _cond;
				mutex_t _mutex;
	
				collections::vector<thread_callback_t> _tasks;
			public:
				dispatcher_thread();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

			public:
				bool dispatch();
				bool start(thread_callback_t , void_args_t, thread_priority_t = thread_priority::normal);
			private:
				virtual bool start(thread_callback_t, void_args_t
					, thread_priority_t, detach_state_t) override;
			public:
				virtual bool wait(async_action_status_t, dword)const override;
				virtual async_action_status_t status()const override;
				virtual bool cancel()override;
				virtual bool suspend()override;
				virtual bool resume()override;

			public:
				template<class T>
				iasync_t<T> run_async(delegates::function<T(iasync<T>*, var_args_t)> callback, var_args_t args);

				template<class T, class... Ts>
				iasync_t<T> run_async(delegates::function<T(iasync<T>*, var_args_t)> callback, Ts const&...);

			//private:
				bool post_task(thread_callback_t callback);

			private:
				virtual~dispatcher_thread();
			};

		}//async
	}//core
}//ang

#ifdef LINK
#undef LINK
#endif

#include<ang/core/inlines/async.hpp>

#endif//__ANG_ASYNC_H__