/*********************************************************************************************************************/
/*   File Name: ang/base/async.h                                                                                     */
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
			template<typename T> struct scope_locker;

			/******************************************************************/
			/* class ang::core::async::mutex                                  */
			/*  -> represents a system asynchronous condition or event        */
			/******************************************************************/
			struct LINK mutex
			{
			private: //Non copiable
				mutex(const mutex&) = delete;
				mutex& operator = (const mutex&) = delete;

			protected:
				pointer m_handle;

			public:
				mutex();
				mutex(bool lock);
				~mutex();

			public: //Properties
				bool lock()const;
				bool trylock()const;
				bool unlock()const;

				scope_locker<mutex> scope()const;
				template<typename FN> auto sync(FN&& func)const -> decltype(static_cast<FN&&>(func)());
				friend cond;
			};

			/******************************************************************/
			/* class ang::core::async::scope_locker                         */
			/*  -> is a useful mutex locker limited to its cycle life         */
			/******************************************************************/
			template<> struct scope_locker<mutex> final
			{
			private:
				mutex const* _mutex;

			public:
				inline scope_locker(mutex const& m) : _mutex(&m) {
					_mutex->lock();
				}
				inline scope_locker(scope_locker && m) : _mutex(null) {
					_mutex = m._mutex;
					m._mutex = null;
				}
				inline ~scope_locker() {
					if(_mutex)_mutex->unlock();
					_mutex = null;
				}

				template<typename func_t>
				static auto lock(mutex_t& m, func_t func) -> decltype(func())
				{
					scope_locker _lock = m;
					return func();
				}
			};

			inline scope_locker<mutex> mutex::scope()const {
				return scope_locker<mutex>(*this);
			}

			template<typename FN>inline auto mutex::sync(FN&& func)const -> decltype(static_cast<FN&&>(func)()) {
				auto scope = this->scope();
				return static_cast<FN&&>(func)();
			}

			/******************************************************************/
			/* class ang::core::async::cond                                   */
			/*  -> represents a system condition                              */
			/******************************************************************/
			struct LINK cond
			{
			private: //Non copiable
				cond(const cond&);
				cond& operator = (const cond&);

			protected:
				pointer m_handle;

			public:
				cond();
				~cond();

			public: //Properties
				bool wait(mutex_t& mutex)const;
				bool wait(mutex_t& mutex, dword ms)const;
				bool signal()const;

				template<typename func>
				void waitfor(mutex_t& mutex, func f) {
					while (f()) { wait(mutex); }
				}

				template<typename func>
				bool waitfor(mutex_t& mutex, dword ms, func f) {
					if (f()) { wait(mutex, ms); }
					return !f();
				}

			};

		}
	}

}

#endif//__ANG_BASE_INTERFACE_H__
