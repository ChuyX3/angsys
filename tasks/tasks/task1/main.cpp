// task1.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ang/core/async.hpp>

#include <type_traits>

using namespace ang;


int main(int argc, char* argv[])
{
	cmstr_t text = u8"Jesús Ángel Rocha Morales"; 
	auto idx = text.find_revert(L"Ángels"_s, -1);

	auto res = core::async::task::run_async<string>([=](core::async::itask* task) ->string
	{
		return "hello world";
	}).then<string>([=](core::async::iasync<string>* task)->string 
	{
		return task->result() + " hello world"_s;
	})->result();
	
	printf("%s \n bye \n", res->cstr().cstr());
    return 0;
}

