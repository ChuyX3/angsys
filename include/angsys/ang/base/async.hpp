/*********************************************************************************************************************/
/*   File Name: ang/async.hpp                                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_ASYNC_HPP__
#define __ANG_BASE_ASYNC_HPP__

namespace ang
{
	namespace core
	{
		namespace async
		{
			struct mutex;
			struct cond;
			typedef mutex mutex_t;
			typedef cond cond_t;

#ifdef WINDOWS_PLATFORM
			typedef pointer handle;
#elif defined ANDROID_PLATFORM
			typedef dword handle;
#endif // PLATFORM

			ANG_INTERFACE(icore_thread);
			//typedef struct core_thread core_thread_t, *core_thread_ptr_t;
			typedef dword(STDCALL*core_thread_start_routine_t)(icore_thread_t, pointer);
			typedef dword(STDCALL*core_thread_then_routine_t)(icore_thread_t, pointer, dword);
			typedef void(*core_thread_tle_deleting_notify_callback_t)(icore_thread_t, wsize, pointer);

			/******************************************************************/
			/* falg ang::core::async::async_action_status                     */
			/*  -> represents the thread status                               */
			/******************************************************************/
			ANG_BEGIN_ENUM(LINK, async_action_status, uint)
				initializing = 0,
				wait_for_start = 1,
				running = 1 << 1,
				wait_for_finish = 1 << 2,
				finishing = 1 << 3,
				completed = 1 << 4,
				canceled = 1 << 5,
				finished = canceled | completed,
				stopped = finished | wait_for_start,
				not_running = finished | wait_for_start | wait_for_finish,
				attached = 1 << 6,
				error = 0XFF00,
			ANG_END_ENUM(async_action_status);


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
				bool waitfor(mutex_t mutex, dword ms, func_t f) {
					if (f()) { wait(mutex, ms); }
					return !f();
				}
				template<typename func_t>
				void waitfor(mutex_t mutex, func_t f) {
					while (f()) { wait(mutex); }
				}
			};

			ANG_BEGIN_INTERFACE(LINK, icore_thread)
				visible static icore_thread_t create_thread_suspended(uint flags, wsize sz, pointer tle, bool alloc);
				visible static icore_thread_t create_thread(core_thread_start_routine_t routine, pointer args, uint flags, wsize sz, pointer tle, bool alloc);
				//visible static void destroy_thread(icore_thread_t);
				visible static icore_thread_t main_thread();
				visible static icore_thread_t this_thread();
				visible static dword this_thread_id();
				visible vcall bool is_main_thread()const pure;
				visible vcall bool is_current_thread()const pure;
				visible vcall wsize tle_size()const pure;
				visible vcall pointer tle_buffer()const pure;		
				visible vcall void set_tle_data(pointer ptr, wsize sz, bool alloc = false) pure;
				visible vcall void set_tle_notify(core_thread_tle_deleting_notify_callback_t callback) pure;
				visible vcall pointer user_args()const pure;
				visible vcall dword thread_id()const pure;
				visible vcall uint thread_state()const pure;
				visible vcall void sleep(dword ms) pure;
				visible vcall bool start(core_thread_start_routine_t callback, pointer args) pure;
				visible vcall bool then(core_thread_then_routine_t callback, pointer args) pure;
				visible vcall bool wait() pure;
				visible vcall bool wait(dword ms) pure;
				visible vcall bool cancel() pure;
				visible vcall bool join() pure;
				template<typename T> T* tle_data()const { return tle_size() == sizeof(T) ? (T*)tle_buffer() : NULL; }
			ANG_END_INTERFACE();


		}
	}

}//ang


#endif//__ANG_BASE_ASYNC_HPP__
