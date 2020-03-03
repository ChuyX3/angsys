// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <angsys.h>
#include <string>

using namespace ang;

ANG_EXTERN ulong64 get_performance_time_us(void)
{
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
}


int main()
{
	string str = "Jesús Ángel Rocha Morales";
	str->append(" 123456"_r);

	return 0;
}
