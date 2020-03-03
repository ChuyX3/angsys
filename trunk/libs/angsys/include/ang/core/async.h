#ifndef __COFFE_CORE_ASYNC_H__
#define __COFFE_CORE_ASYNC_H__

#include <coffe.h>
#include <coffe/core/time.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined COFFE_DYNAMIC_LIBRARY

#ifdef COFFE_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//COFFE_EXPORTS
#else//#elif defined COFFE_STATIC_LIBRARY
#define LINK
#endif//COFFE_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif


namespace coffe
{
	namespace core
	{
		namespace async
		{
			template<typename T> struct itask;
			template<typename T> struct iaction;
			template<typename T> class task_handler;

			typedef shared_ptr<cond> cond_ptr_t;
			typedef shared_ptr<mutex> mutex_ptr_t;

			template<typename T> struct itask;
			template<typename T> using itask_t = intf_wrapper<itask<T>>;
			template<typename T> using iasync = intf_wrapper<itask<T>>;
			template<typename T> using task_handler_ptr = object_wrapper<task_handler<T>>;

			template<typename T> using iasync_op = intf_wrapper<itask<optional<T>>>;

			coffe_declare_interface(isync);
			coffe_declare_interface(ithread);
			coffe_declare_interface(idispatcher);

			coffe_declare_object(sync);
			coffe_declare_object(thread);

			/******************************************************************/
			/* falg coffe::core::async::async_action_status                   */
			/*  -> represents the thread status                               */
			/******************************************************************/
			declare_flags(LINK, async_action_status, uint)
			{
				initializing = 0,
				wait_for_start = 1,
				running = 1 << 1,
				wait_for_then = 1 << 2,
				working = running | wait_for_then,
				completed = 1 << 3,
				canceled = 1 << 4,
				finished = canceled | completed,
				stopped = finished | wait_for_start,
				not_running = finished | wait_for_start | wait_for_then,
				attached = 1 << 5,
				error = 0XFF00,
			};


			/******************************************************************/
			/* interface coffe::core::async::iaction< T >                     */
			/*  -> represents a asynchronous operation, it can be waited for  */
			/*     its result or any status change                            */
			/******************************************************************/
			template<typename T>
			struct nvt iaction
				: intf<iaction<T>
				, iid("coffe::core::async::iaction")>
			{
				virtual bool wait(async_action_status_t)const = 0;
				virtual bool wait(async_action_status_t, dword)const = 0;
				virtual async_action_status_t status()const = 0;
				virtual bool cancel()= 0;
				virtual T result()const = 0;
			};

			/******************************************************************/
			/* interface coffe::core::async::itask< T >                       */
			/*  -> represents a asynchronous operation, it can be waited for  */
			/*     its result or any status change                            */
			/******************************************************************/
			template<typename T>
			struct nvt itask 
				: intf<itask<T>
				, iid("coffe::core::async::itask")
				, iaction<T>>
			{
				virtual iasync<void> then(delegates::function<void(iasync<T>)>)= 0;
				template<typename U> iasync<U> then(delegates::function<U(iasync<T>)>);
			};
			
			struct nvt LINK idispatcher
				: intf<idispatcher
				, iid("coffe::core::async::idispatcher")>
			{
				virtual bool has_thread_access()const = 0;
				virtual async_action_status_t status()const = 0;
				virtual void exit() = 0;
				virtual void join() = 0;
				virtual listen_token<void(time::step_timer const&)> add_idle_event(function<void(time::step_timer const&)>)= 0;
				virtual bool remove_idle_event(listen_token<void(time::step_timer const&)>)= 0;
				virtual iasync<void> run_async(function<void(iasync<void>)>)= 0;
				template<typename T> iasync<T> run_async(delegates::function<T(iasync<T>)>);
			};

			struct nvt LINK ithread
				: intf<ithread
				, iid("coffe::core::async::ithread")>
			{
				virtual bool is_this_thread()const = 0;
				virtual bool is_main_thread()const = 0;
				virtual dword thread_id()const = 0;
				virtual async_action_status_t status()const = 0;
				virtual void exit() = 0;
				virtual void join() = 0;
			};

			struct nvt LINK isync
				: intf<isync
				, iid("coffe::core::async::isync")>
			{
				virtual void run_sync(function<void(isync_t)>) = 0;
				virtual mutex_t& shared_mutex()const = 0;
			};

		}
	}

	COFFE_BEGIN_INTF_WRAPPER_TEMPLATE(core::async::iaction, T)
		inline operator nullable<T>()const;
	COFFE_END_INTF_WRAPPER();

	COFFE_BEGIN_INTF_WRAPPER_TEMPLATE(core::async::itask, T)
		inline operator nullable<T>()const;
	COFFE_END_INTF_WRAPPER();
}

namespace coffe
{
	namespace core
	{
		namespace async
		{
			template<> struct scope_locker<mutex_ptr_t> final
			{
			private:
				mutex_ptr_t _mutex;

			public:
				inline scope_locker(mutex_ptr_t m) : _mutex(m.get()) {
					if(!_mutex.is_empty())_mutex->lock();
				}
				inline ~scope_locker() {
					if (!_mutex.is_empty())_mutex->unlock();
				}

				template<typename func_t>
				static auto lock(mutex_ptr_t m, func_t func) -> decltype(func())
				{
					scope_locker _lock = m;
					return func();
				}
			};
		}
	}
}

namespace coffe
{
	namespace core
	{
		namespace async
		{
			class LINK thread
				: public implement<thread
				, iid("coffe::core::async::thread")
				, ithread>
			{
			public:
				static void sleep(dword ms);
				static thread_t main_thread();
				static thread_t this_thread();
				static dword this_thread_id();
				static iasync<void> create_worker_thread(function<void(iasync<void>)>&&);
				static idispatcher_t create_dispatcher_thread();

			protected:
				thread();
				virtual~thread();
			};
		}
	}
}

namespace coffe
{
	namespace core
	{
		namespace async
		{
			class LINK sync
				: public implement<sync
				, iid("coffe::core::async::sync")
				, isync>
			{
			public:
				static void synchronize(function<void(isync_t)>&&);

			protected:
				sync();
				virtual~sync();
			};
		}
	}
}

namespace coffe
{
	namespace core
	{
		namespace async
		{
			class LINK task
				: public implement<task
				, iid("coffe::core::async::task")
				, itask<void>
				, iaction<void>>
			{
			public:
				template<typename T>
				static iasync<T> run_async(delegates::function<T(iasync<T>)>);

			protected:
				task();
				virtual~task();
			};

			template<>LINK iasync<void> task::run_async(delegates::function<void(iasync<void>)>);

			template<typename T>
			inline iasync<T> create_task(function<T(iasync<T>)>&& action) {
				return task::run_async<T>(coffe::forward<function<T(iasync<T>)>>(action));
			}

			template<typename T>
			class task_handler final
				: public implement<task_handler<T>
				, iid("coffe::core::async::task_handler")
				, itask<T>
				, iaction<T>>
			{
			private:
				mutable T m_result;
				iasync<void> m_task;

			public:
				task_handler();
				task_handler(iasync<void> handle);

			public:
				void attach(iasync<void> async);
				void done(T&& res)const;

				virtual iasync<void> then(delegates::function<void(iasync<T>)>)override;
				virtual bool wait(async_action_status_t)const override;
				virtual bool wait(async_action_status_t, dword)const override;
				virtual async_action_status_t status()const override;
				virtual bool cancel()override;
				virtual T result()const override;

			private:
				virtual~task_handler();
			};

		}
	}
}


COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<void>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<char>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<mchar>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<wchar>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<char16_t>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<char32_t>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<short>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<ushort>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<int>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<uint>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<long>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<ulong>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<long64>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<ulong64>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<float>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<double>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<coffe::objptr>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<coffe::string>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::iaction<coffe::variant>);

COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<void>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<char>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<mchar>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<wchar>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<char16_t>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<char32_t>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<short>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<ushort>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<int>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<uint>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<long>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<ulong>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<long64>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<ulong64>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<float>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<double>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<coffe::objptr>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<coffe::string>);
COFFE_DECLARE_CLASS_INFO_OVERRIDE(LINK, coffe::core::async::itask<coffe::variant>);

COFFE_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, coffe::core::async::async_action_status);

#ifdef  LINK
#undef  LINK
#endif//LINK

#include <coffe/core/inline/async.inl>

#endif//__COFFE_CORE_ASYNC_H__
