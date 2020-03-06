/*********************************************************************************************************************/
/*   File Name: ang/core/time.h                                                                                      */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_CORE_TIME_H__
#define __ANG_CORE_TIME_H__

#include <angsys.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANG_DYNAMIC_LIBRARY

#ifdef ANG_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_EXPORTS
#else//#elif defined ANG_STATIC_LIBRARY
#define LINK
#endif//ANG_DYNAMIC_LIBRARY
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
			ulong64 LINK get_performance_time_us();

			class step_timer
			{
			public:
				step_timer() :
					m_elapsed_time(0),
					m_total_time(0),
					m_left_over_time(0),
					m_frame_count(0),
					m_total_frame_count(0),
					m_frames_per_second(0),
					m_time_counter(0),
					m_is_fixed_time_step(false),
					m_target_elapsed_time(1000000 / 60)
				{
					m_max_delta = 100000;
				}

				// Get elapsed time since the previous Update call.
				ulong64 elapsed_time() const { return m_elapsed_time; }

				// Get total time since the start of the program.
				ulong64 total_time() const { return m_total_time; }

				// Get total number of updates since start of the program.
				uint frame_count() const { return m_total_frame_count; }

				// Get the current framerate.
				double framerate() const { return m_frames_per_second; }

				// Set whether to use fixed or variable timestep mode.
				void fixed_time_step(bool value) { m_is_fixed_time_step = value; }

				// Set how often to call Update when in fixed timestep mode.
				void frames_per_second(ulong64 targetElapsed) { m_target_elapsed_time = 1000000 / targetElapsed; }

				void reset()
				{
					m_last_time = get_performance_time_us();
					m_left_over_time = 0;
					m_frames_per_second = 0;
					m_frame_count = 0;
				}

				void update()
				{
					ulong64 current_time = get_performance_time_us();
					ulong64 time_delta = current_time - m_last_time;

					m_last_time = current_time;
					m_time_counter += time_delta;

					if (time_delta > m_max_delta)
						time_delta = m_max_delta;

					m_elapsed_time = time_delta;
					m_total_time += time_delta;
					m_left_over_time = 0;
					m_total_frame_count++;
					m_frame_count++;
					if (m_frame_count >= m_target_elapsed_time / 1000)
					{
						m_frames_per_second = 1000000.0 * (double)m_frame_count / (double)m_time_counter;
						m_time_counter = 0;
						m_frame_count = 0;
					}
				}

				template<typename F>
				void tick(const F& update)
				{
					ulong64 current_time = get_performance_time_us();
					ulong64 time_delta = current_time - m_last_time;

					m_last_time = current_time;
					m_time_counter += time_delta;

					// Clamp excessively large time deltas (e.g. after paused in the debugger).
					if (time_delta > m_max_delta)
					{
						time_delta = m_max_delta;
					}

					uint last_frame_count = m_total_frame_count;

					if (m_is_fixed_time_step)
					{
						if (abs(static_cast<long64>(time_delta - m_target_elapsed_time)) < 1250)
						{
							time_delta = m_target_elapsed_time;
						}

						m_left_over_time += time_delta;

						while (m_left_over_time >= m_target_elapsed_time)
						{
							m_elapsed_time = m_target_elapsed_time;
							m_total_time += m_target_elapsed_time;
							m_left_over_time -= m_target_elapsed_time;
							m_total_frame_count++;

							update();
						}
					}
					else
					{
						// Variable timestep update logic.
						m_elapsed_time = time_delta;
						m_total_time += time_delta;
						m_left_over_time = 0;
						m_total_frame_count++;

						update();
					}

					// Track the current framerate.
					m_frame_count += m_total_frame_count - last_frame_count;

					if (m_frame_count >= m_target_elapsed_time / 1000)
					{
						m_frames_per_second = 1000000.0 * (double)m_frame_count / (double)m_time_counter;
						m_time_counter = 0;
						m_frame_count = 0;
					}
				}

			private:
				ulong64 m_max_delta;

				// Derived timing data uses a canonical tick format.
				ulong64 m_last_time;

				ulong64 m_elapsed_time;
				ulong64 m_total_time;
				ulong64 m_left_over_time;

				// Members for tracking the framerate.
				uint m_frame_count;
				uint m_total_frame_count;
				double m_frames_per_second;
				ulong64 m_time_counter;

				// Members for configuring fixed timestep mode.
				ulong64 m_target_elapsed_time;
				bool m_is_fixed_time_step;
			};
		}
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_CORE_ASYNC_H__
