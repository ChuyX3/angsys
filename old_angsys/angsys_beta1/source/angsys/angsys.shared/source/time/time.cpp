/*********************************************************************************************************************/
/*   File Name: time.cpp                                                                                             */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/core/time.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::time;



double time::get_performance_time()
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	struct timespec ts;
	double theTick = 0;
	clock_gettime(CLOCK_REALTIME, &ts);
	theTick = (double)ts.tv_nsec / 1000000.0;
	theTick += (double)ts.tv_sec * 1000.0;
	return theTick;
#else
	static struct PerformanceFrequency {
		double QuadPart;
		PerformanceFrequency() {
			LARGE_INTEGER _frec;
			QueryPerformanceFrequency(&_frec);
			QuadPart = (double)_frec.QuadPart;
		}
	}frec;

	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return 1000.0 * (double)count.QuadPart / frec.QuadPart;
#endif
}

timer::timer()
	: _total(0)
	, _last_time(0)
	, _curr_time(0)
{
	_last_time = get_performance_time();
	_curr_time = get_performance_time();
}

timer::~timer()
{

}

//ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::time::timer, ang::object);

double timer::total()const
{
	return _total;
}

double timer::delta()const
{
	return double(_curr_time - _last_time);
}


void timer::update()
{
	_last_time = _curr_time;
	_curr_time = get_performance_time();
	_total += delta();
}

void timer::reset()
{
	_last_time = get_performance_time();
	_curr_time = get_performance_time();
}

///////////////////////////////////////////////////
