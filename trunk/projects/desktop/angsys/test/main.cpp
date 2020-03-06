// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <angsys.h>
#include <ang/core/async.h>

using namespace ang;

int main()
{
	string str;
	auto task = core::async::create_task<string>([](core::async::iasync<string> task)->string
	{
		return u8"Jesús Ángel"_svm;
	});
	str = task->result();
	str->append("..."_r);
	return 0;
}
