// test.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ang/streams.h>
#include <ang/maths/vectors.h>
#include <ang/core/files.h>

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
	wstring str;
	core::files::input_text_file_t file = new core::files::input_text_file(L"C:\\Users\\Jesus Rocha\\Desktop\\airfoil_2312_core.txt"_s);
	
	file->read_line(str, 100);

	return 0;
}

