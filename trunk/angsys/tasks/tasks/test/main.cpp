// test.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ang/streams.h>
#include <ang/maths/vectors.h>


#include "console.h"

using namespace ang;

ANG_EXTERN ulong64 get_performance_time_us()
{
	{
#if defined ANDROID_PLATFORM
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

int main(int argc, char* argv[])
{	
	var_args_t args;
	for (int i = 0; i < argc; i++)
		args += cstr_t(argv[i]);

	auto time = get_performance_time_us();
	var s = string::format("{4f} {3f} {2f} {1f} {0f}"_s, 1.2345, 2.3456, 3.4567, 4.5678, 5.6789);
	time = get_performance_time_us() - time;

	return 0;
}

