// task1.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ang/core/async.hpp>

#include <type_traits>

using namespace ang;

//template<typename T>

struct MyStruct
{
	int a;
};
 
class MyClass : public object
{
	

};

ANG_INTERFACE(imy_intercface);
ANG_BEGIN_INTERFACE(,imy_intercface)
ANG_END_INTERFACE();

int main(int argc, char* argv[])
{

	intfptr ptr = new collections::array_buffer<int>();

	array<int> arr = ptr.as<array<int>>();

	core::async::thread_t thread = core::async::thread::create_thread([](core::async::thread_t thread, var_args_t args)->dword
	{
		printf("core::async::icore_thread do... \n");
		core::async::thread::sleep(1000);
		printf("core::async::icore_thread done. \n");
		return 0;
	}, null, 0, null, false);

	thread->then([](core::async::thread_t thread, var_args_t args)->dword
	{
		printf("core::async::icore_thread then do... \n");
		core::async::thread::sleep(1000);
		printf("core::async::icore_thread done. \n");
		return  0;
	}, null);

	thread->join();
	printf("bye \n");
    return 0;
}

