/*********************************************************************************************************************/
/*   File Name: ang/base/async.h                                                                                 */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_ASYNC_H__
#define __ANG_BASE_INTERFACE_H__

namespace ang //constants
{
	namespace core
	{
		namespace async
		{
			struct mutex;
			struct cond;

			typedef mutex mutex_t;
			typedef cond cond_t;


			/******************************************************************/
			/* calss ang::core::async::mutex                                  */
			/*  -> represents a system asynchronous condition or event        */
			/******************************************************************/
			struct LINK mutex
			{
			private: //Non copiable
				mutex(const mutex&) = delete;
				mutex& operator = (const mutex&) = delete;

			protected:
				pointer _handle;

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
			template<typename T> struct scope_locker;

			template<> struct scope_locker<mutex> final
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
			struct LINK cond
			{
			private: //Non copiable
				cond(const cond&);
				cond& operator = (const cond&);

			protected:
				pointer _handle;

			public:
				cond();
				~cond();

			public: //Properties
				bool wait(mutex_t& mutex)const;
				bool wait(mutex_t& mutex, dword ms)const;
				bool signal()const;

				template<typename func_t>
				void waitfor(mutex_t& mutex, func_t f) {
					while (f()) { wait(mutex); }
				}

				template<typename func_t>
				bool waitfor(mutex_t& mutex, dword ms, func_t f) {
					if (f()) { wait(mutex, ms); }
					return !f();
				}

			};

		}
	}

}

#endif//__ANG_BASE_INTERFACE_H__