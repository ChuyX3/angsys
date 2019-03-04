#ifndef __ANG_CORE_TIMER_H__
#define __ANG_CORE_TIMER_H__

#include <angsys.h>

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
		namespace time
		{
			LINK ulong64 get_performance_time_us();

			class step_timer
			{
			private:
				ulong64 m_firtTime;
				ulong64 m_last_time;
				ulong64 m_curr_time;

			public:
				step_timer()
					: m_firtTime(0)
					, m_last_time(0)
					, m_curr_time(0)
				{
					m_firtTime = m_last_time = m_curr_time = get_performance_time_us();
				}
				~step_timer() {

				}

			public: //Properties
				ulong64 total()const {
					return m_curr_time - m_firtTime;
				}
				ulong64 delta()const {
					return m_curr_time - m_last_time;
				}

			public: //Methods
				void update() {
					m_last_time = m_curr_time;
					m_curr_time = get_performance_time_us();
				}
				void reset() {
					m_firtTime = m_last_time = m_curr_time = get_performance_time_us();
				}

				template<typename F>
				inline auto tick(F f) {
					update();
					return f();
				}
			};

			typedef shared_ptr<step_timer> step_timer_t;
		}
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK


#endif//__ANG_CORE_ASYNC_H__