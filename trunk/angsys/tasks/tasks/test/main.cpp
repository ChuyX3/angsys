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

template<typename T, typename = void>
struct __si_complete : false_type {

};

template<typename T>
struct __si_complete<T, void_t<integer_constant<bool, sizeof(T)==sizeof(T)>>> : true_type {

};

struct my_struct {

};

int main(int argc, char* argv[])
{	
	core::files::ifile_system::fs_instance()->push_path(L"..\\..\\..\\third_party\\"_s, core::files::file_system_priority::lowest);

	is_complete<my_struct>::value;


	core::files::input_text_file_t file = new core::files::input_text_file(L"resources\\girl\\girl.obj"_s);
	float a1, a2;
	var_args_t args;
	file->read_format("A18 (original)\n {f:} {f:}"_s, a1, a2);



	return 0;
}

