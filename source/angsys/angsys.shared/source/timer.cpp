#include "pch.h"
#include <angsys.hpp>
#include <ang/core/time.hpp>

using namespace ang;
using namespace ang::core::time;

extern "C" ulong64 ang_get_performance_time()
{
	{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		struct timespec ts;
		ulong64 theTick = 0;
		clock_gettime(CLOCK_REALTIME, &ts);
		theTick = (ulong64)ts.tv_nsec / 1000.0;
		theTick += (ulong64)ts.tv_sec * 1000000.0;
		return theTick;
#else
		static struct PerformanceFrequency {
			ulong64 QuadPart;
			PerformanceFrequency() {
				LARGE_INTEGER _frec;
				QueryPerformanceFrequency(&_frec);
				QuadPart = (ulong64)_frec.QuadPart;
			}
		}frec;

		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return  (1000000 * count.QuadPart) / frec.QuadPart; //uS
#endif
	}
}

double ang::core::time::get_performance_time()
{
	{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		struct timespec ts;
		double theTick = 0;
		clock_gettime(CLOCK_REALTIME, &ts);
		theTick = (double)ts.tv_nsec / 1000.0;
		theTick += (double)ts.tv_sec * 1000000.0;
		return theTick;
#else
		static struct PerformanceFrequency {
			ulong64 QuadPart;
			PerformanceFrequency() {
				LARGE_INTEGER _frec;
				QueryPerformanceFrequency(&_frec);
				QuadPart = (ulong64)_frec.QuadPart;
			}
		}frec;

		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return  (1000000.0 * (double)count.QuadPart) / (double)frec.QuadPart; //uS
#endif
	}
}


step_timer::step_timer()
	: _total(0)
	, _last_time(0)
	, _curr_time(0) {
}
step_timer::~step_timer() { }
double step_timer::total()const { return _total; }
double step_timer::delta()const { return _curr_time - _last_time; }
void step_timer::update() {
	_last_time = _curr_time;
	_curr_time = get_performance_time();
	_total += delta();
}
void step_timer::reset() {
	_curr_time = _last_time = get_performance_time();
}
