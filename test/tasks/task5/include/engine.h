#pragma once

#include "d3d11/driver.h"

#include "scenes/scenes.h"

namespace d3d11
{
	using namespace ang;

	//class ordered_model;
	//typedef ang::object_wrapper<ordered_model> ordered_model_t;
	//class ordered_model
	//	: public object
	//{
	//public:
	//	struct vertex
	//	{
	//		maths::float4 position;
	//		maths::float4 normal;
	//		maths::float2 texcoord;

	//		bool operator == (const vertex&) { return false; }//dummy
	//		bool operator != (const vertex&) { return false; }//dummy
	//	};

	//private:
	//	collections::vector<collections::pair<string, collections::vector<vertex>>> _elements;

	//public:
	//	ordered_model();

	//	//ANG_DECLARE_INTERFACE();

	//	bool load(core::files::input_text_file_t);
	//	core::async::iasync_t<bool> load_async(core::files::input_text_file_t);
	//	bool load(core::files::input_binary_file_t);
	//	bool save(core::files::output_binary_file_t);

	//private:
	//	virtual~ordered_model();
	//};

	class StepTimer
	{
	public:
		StepTimer() :
			m_elapsedTicks(0),
			m_totalTicks(0),
			m_leftOverTicks(0),
			m_frameCount(0),
			m_framesPerSecond(0),
			m_framesThisSecond(0),
			m_qpcSecondCounter(0),
			m_isFixedTimeStep(false),
			m_targetElapsedTicks(TicksPerSecond / 60)
		{
			if (!QueryPerformanceFrequency(&m_qpcFrequency))
			{
				throw  exception_t(except_code::unknown);;
			}

			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw  exception_t(except_code::unknown);;
			}

			// Initialize max delta to 1/10 of a second.
			m_qpcMaxDelta = m_qpcFrequency.QuadPart / 10;
		}

		// Get elapsed time since the previous Update call.
		ulong64 GetElapsedTicks() const { return m_elapsedTicks; }
		double GetElapsedSeconds() const { return TicksToSeconds(m_elapsedTicks); }

		// Get total time since the start of the program.
		ulong64 GetTotalTicks() const { return m_totalTicks; }
		double GetTotalSeconds() const { return TicksToSeconds(m_totalTicks); }

		// Get total number of updates since start of the program.
		uint GetFrameCount() const { return m_frameCount; }

		// Get the current framerate.
		uint GetFramesPerSecond() const { return m_framesPerSecond; }

		// Set whether to use fixed or variable timestep mode.
		void SetFixedTimeStep(bool isFixedTimestep) { m_isFixedTimeStep = isFixedTimestep; }

		// Set how often to call Update when in fixed timestep mode.
		void SetTargetElapsedTicks(ulong64 targetElapsed) { m_targetElapsedTicks = targetElapsed; }
		void SetTargetElapsedSeconds(double targetElapsed) { m_targetElapsedTicks = SecondsToTicks(targetElapsed); }

		// Integer format represents time using 10,000,000 ticks per second.
		static const ulong64 TicksPerSecond = 1000000;

		static double TicksToSeconds(ulong64 ticks) { return static_cast<double>(ticks) / TicksPerSecond; }
		static ulong64 SecondsToTicks(double seconds) { return static_cast<ulong64>(seconds * TicksPerSecond); }

		// After an intentional timing discontinuity (for instance a blocking IO operation)
		// call this to avoid having the fixed timestep logic attempt a set of catch-up 
		// Update calls.

		void ResetElapsedTime()
		{
			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw  exception_t(except_code::unknown);;
			}

			m_leftOverTicks = 0;
			m_framesPerSecond = 0;
			m_framesThisSecond = 0;
			m_qpcSecondCounter = 0;
		}

		// Update timer state, calling the specified Update function the appropriate number of times.
		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			// Query the current time.
			LARGE_INTEGER currentTime;

			if (!QueryPerformanceCounter(&currentTime))
			{
				throw  exception_t(except_code::unknown);;
			}

			ulong64 timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;

			m_qpcLastTime = currentTime;
			m_qpcSecondCounter += timeDelta;

			// Clamp excessively large time deltas (e.g. after paused in the debugger).
			if (timeDelta > m_qpcMaxDelta)
			{
				timeDelta = m_qpcMaxDelta;
			}

			// Convert QPC units into a canonical tick format. This cannot overflow due to the previous clamp.
			timeDelta *= TicksPerSecond;
			timeDelta /= m_qpcFrequency.QuadPart;

			uint lastFrameCount = m_frameCount;

			if (m_isFixedTimeStep)
			{
				// Fixed timestep update logic

				// If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp
				// the clock to exactly match the target value. This prevents tiny and irrelevant errors
				// from accumulating over time. Without this clamping, a game that requested a 60 fps
				// fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
				// accumulate enough tiny errors that it would drop a frame. It is better to just round 
				// small deviations down to zero to leave things running smoothly.

				if (abs(static_cast<long64>(timeDelta - m_targetElapsedTicks)) < TicksPerSecond / 4000)
				{
					timeDelta = m_targetElapsedTicks;
				}

				m_leftOverTicks += timeDelta;

				while (m_leftOverTicks >= m_targetElapsedTicks)
				{
					m_elapsedTicks = m_targetElapsedTicks;
					m_totalTicks += m_targetElapsedTicks;
					m_leftOverTicks -= m_targetElapsedTicks;
					m_frameCount++;

					update();
				}
			}
			else
			{
				// Variable timestep update logic.
				m_elapsedTicks = timeDelta;
				m_totalTicks += timeDelta;
				m_leftOverTicks = 0;
				m_frameCount++;

				update();
			}

			// Track the current framerate.
			if (m_frameCount != lastFrameCount)
			{
				m_framesThisSecond++;
			}

			if (m_qpcSecondCounter >= static_cast<ulong64>(m_qpcFrequency.QuadPart))
			{
				m_framesPerSecond = m_framesThisSecond;
				m_framesThisSecond = 0;
				m_qpcSecondCounter %= m_qpcFrequency.QuadPart;
			}
		}

	private:
		// Source timing data uses QPC units.
		LARGE_INTEGER m_qpcFrequency;
		LARGE_INTEGER m_qpcLastTime;
		ulong64 m_qpcMaxDelta;

		// Derived timing data uses a canonical tick format.
		ulong64 m_elapsedTicks;
		ulong64 m_totalTicks;
		ulong64 m_leftOverTicks;

		// Members for tracking the framerate.
		uint m_frameCount;
		uint m_framesPerSecond;
		uint m_framesThisSecond;
		ulong64 m_qpcSecondCounter;

		// Members for configuring fixed timestep mode.
		bool m_isFixedTimeStep;
		ulong64 m_targetElapsedTicks;
	};

	class engine;
	typedef object_wrapper<engine> engine_t;

	template<typename... Ts>
	void debug_print(cstr_t format, Ts... args)
	{
		string text = "";
		text->format(format, args...);
		OutputDebugStringA(text->cstr());
	}

	class engine final
		: public object
	{
	private:
		StepTimer _timer;
		bool _is_running;
		core::async::cond_t cond;
		core::async::mutex_t mutex;
		
		platform::icore_view_t core_view;
		graphics::d3d11::d3d11_driver_t d3d_driver;
		graphics::d3d11::d3d11_surface_t d3d_surface;
		graphics::effects::ieffect_library_t effect_library;
		graphics::textures::itexture_loader_t texture_loader;

		core::async::iasync_t<void> async_render_operation;

		graphics::buffers::iindex_buffer_t index_buffer;
		graphics::buffers::ivertex_buffer_t vertex_buffer;

		graphics::scenes::camera_t camera;

		graphics::scenes::model_t model;

	public:
		engine();
		ANG_DECLARE_INTERFACE();

		void on_create_event(objptr, platform::events::icreated_event_args_t);
		void on_destroy_event(objptr, platform::events::imsg_event_args_t);
		void on_render_operation(objptr, platform::events::icreated_event_args_t);
		void on_size_changed_event(objptr, platform::events::idisplay_info_event_args_t);
		void on_pointer_moved_event(objptr, platform::events::ipointer_event_args_t);
		void on_pointer_pressed_event(objptr, platform::events::ipointer_event_args_t);
		void on_pointer_released_event(objptr, platform::events::ipointer_event_args_t);

	private:
		bool init_driver();
		void close_driver();

		void init();
		void update(StepTimer const&);
		void update_controller(float delta);
		void draw();
		void exit();



	private:
		virtual~engine();
	};
}
