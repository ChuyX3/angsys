#ifndef __ANG_CORE_ASYNC_H__
#define __ANG_CORE_ASYNC_H__

#include <angsys.h>
#include <ang/core/listener.h>

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
#else
#define LINK
#endif


namespace ang
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

			ang_object(thread);
			template<typename T> struct itask;
			template<typename T> using itask_t = intf_wrapper<itask<T>>;
			template<typename T> using iasync = intf_wrapper<itask<T>>;

			ang_interface(ithread);
			ang_interface(idispatcher);


			/******************************************************************/
			/* falg ang::core::async::async_action_status                     */
			/*  -> represents the thread status                               */
			/******************************************************************/
			safe_flags(LINK, async_action_status, uint)
			{
				initializing = 0,
				wait_for_start = 1,
				running = 1 << 1,
				wait_for_then = 1 << 2,
				working = (uint)async_action_status::running | (uint)async_action_status::wait_for_then,
				//finishing = 1 << 3,
				completed = 1 << 3,
				canceled = 1 << 4,
				finished = (uint)async_action_status::canceled | (uint)async_action_status::completed,
				stopped = (uint)async_action_status::finished | (uint)async_action_status::wait_for_start,
				not_running = (uint)async_action_status::finished | (uint)async_action_status::wait_for_start | (uint)async_action_status::wait_for_then,
				attached = 1 << 5,
				error = 0XFF00,
			};


			/******************************************************************/
			/* interface ang::core::async::iaction< T >                    */
			/*  -> represents a asynchronous operation, it can be waited for  */
			/*     its result or any status change                            */
			/******************************************************************/
			template<typename T>
			ang_begin_interface_inline(iaction)
				visible vcall bool wait(async_action_status_t)const pure;
				visible vcall bool wait(async_action_status_t, dword)const pure;
				visible vcall async_action_status_t status()const pure;
				visible vcall bool cancel()pure;
				visible vcall T result()const pure;
			ang_end_interface();


			/******************************************************************/
			/* interface ang::core::async::itask< T >                         */
			/*  -> represents a asynchronous operation, it can be waited for  */
			/*     its result or any status change                            */
			/******************************************************************/
			template<typename T>
			ang_begin_interface_inline(itask, public iaction<T>)
				visible vcall iasync<void> then(delegates::function<void(iasync<T>)>)pure;
				template<typename U> iasync<U> then(delegates::function<U(iasync<T>)>);
			ang_end_interface();
			

			ang_begin_interface(LINK ithread)
				visible vcall iasync<void> run(core::delegates::function<void(iasync<void>)>)pure;
				visible vcall iasync<void> run(core::delegates::function<void(iasync<void>, var_args_t)>, var_args_t)pure;
				visible vcall bool is_main_thread()const pure;
				visible vcall bool is_current_thread()const pure;
				visible vcall dword thread_id()const pure;			
				visible vcall void join()const pure;
			ang_end_interface();

		}
	}
}

namespace ang
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
				inline scope_locker(mutex_ptr_t const& m) : _mutex(m.get()) {
					if(!_mutex.is_empty())_mutex->lock();
				}
				inline ~scope_locker() {
					if (!_mutex.is_empty())_mutex->unlock();
				}

				template<typename func_t>
				static auto lock(mutex_ptr_t const& m, func_t func) -> decltype(func())
				{
					scope_locker _lock = m;
					return func();
				}
			};
		}
	}
}

namespace ang
{
	namespace core
	{
		namespace async
		{
			class LINK thread
				: public object
				, public ithread
			{
			public:
				static void sleep(dword ms);
				static thread_t main_thread();
				static thread_t this_thread();
				static dword this_thread_id();
				static thread_t create_thread();

			public:
				ANG_DECLARE_INTERFACE();

			protected:
				thread();
				virtual~thread();
			};
		}
	}
}

namespace ang
{
	namespace core
	{
		namespace async
		{
			class LINK task
				: public object
				, public itask<void>
			{
			public:
				template<typename T>
				static iasync<T> run_async(delegates::function<T(iasync<T>)>);

			public:
				ANG_DECLARE_INTERFACE();

			protected:
				task();
				virtual~task();
			};

			template<>LINK iasync<void> task::run_async(delegates::function<void(iasync<void>)>);

			template<typename T>
			class task_handler final
				: public object
				, public itask<T>
			{
			private:
				T _result;
				iasync<void> _task;

			public:
				task_handler();
				task_handler(iasync<void> handle);
				ANG_DECLARE_INTERFACE();

			public:
				void attach(iasync<void> async);
				void done(T const& res);

				virtual iasync<void> then(delegates::function<void(iasync<T>)>)override;
				virtual bool wait(async_action_status_t)const override;
				virtual bool wait(async_action_status_t, dword)const override;
				virtual async_action_status_t status()const override;
				virtual bool cancel()override;
				virtual T result()const override;

			private:
				virtual~task_handler();
			};

			




			template<> LINK rtti_t const& iaction<void>::class_info();
			template<> LINK rtti_t const& iaction<char>::class_info();
			template<> LINK rtti_t const& iaction<mchar>::class_info();
			template<> LINK rtti_t const& iaction<wchar>::class_info();
			template<> LINK rtti_t const& iaction<char16_t>::class_info();
			template<> LINK rtti_t const& iaction<char32_t>::class_info();
			template<> LINK rtti_t const& iaction<short>::class_info();
			template<> LINK rtti_t const& iaction<ushort>::class_info();
			template<> LINK rtti_t const& iaction<int>::class_info();
			template<> LINK rtti_t const& iaction<uint>::class_info();
			template<> LINK rtti_t const& iaction<long>::class_info();
			template<> LINK rtti_t const& iaction<ulong>::class_info();
			template<> LINK rtti_t const& iaction<long64>::class_info();
			template<> LINK rtti_t const& iaction<ulong64>::class_info();
			template<> LINK rtti_t const& iaction<float>::class_info();
			template<> LINK rtti_t const& iaction<double>::class_info();
			template<> LINK rtti_t const& iaction<object>::class_info();
			template<> LINK rtti_t const& iaction<string>::class_info();
			template<> LINK rtti_t const& iaction<mstring>::class_info();
			template<> LINK rtti_t const& iaction<wstring>::class_info();
			template<> LINK rtti_t const& iaction<string16>::class_info();
			template<> LINK rtti_t const& iaction<string32>::class_info();
			template<> LINK rtti_t const& iaction<variant>::class_info();

			template<> LINK rtti_t const& itask<void>::class_info();
			template<> LINK rtti_t const& itask<char>::class_info();
			template<> LINK rtti_t const& itask<mchar>::class_info();
			template<> LINK rtti_t const& itask<wchar>::class_info();
			template<> LINK rtti_t const& itask<char16_t>::class_info();
			template<> LINK rtti_t const& itask<char32_t>::class_info();
			template<> LINK rtti_t const& itask<short>::class_info();
			template<> LINK rtti_t const& itask<ushort>::class_info();
			template<> LINK rtti_t const& itask<int>::class_info();
			template<> LINK rtti_t const& itask<uint>::class_info();
			template<> LINK rtti_t const& itask<long>::class_info();
			template<> LINK rtti_t const& itask<ulong>::class_info();
			template<> LINK rtti_t const& itask<long64>::class_info();
			template<> LINK rtti_t const& itask<ulong64>::class_info();
			template<> LINK rtti_t const& itask<float>::class_info();
			template<> LINK rtti_t const& itask<double>::class_info();
			template<> LINK rtti_t const& itask<object>::class_info();
			template<> LINK rtti_t const& itask<string>::class_info();
			template<> LINK rtti_t const& itask<mstring>::class_info();
			template<> LINK rtti_t const& itask<wstring>::class_info();
			template<> LINK rtti_t const& itask<string16>::class_info();
			template<> LINK rtti_t const& itask<string32>::class_info();
			template<> LINK rtti_t const& itask<variant>::class_info();
		}
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK

#include <ang/core/inline/async.inl>

#endif//__ANG_CORE_ASYNC_H__