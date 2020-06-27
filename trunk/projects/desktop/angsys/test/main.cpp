// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "test.h"

using namespace ang;

int main(int argc, char* argv[])
{
	core::files::ifile_system::instance()->push_path("D:\\ja_ro\\Documents\\GitHub\\data\\tasks\\data"_r, core::files::path_access_type::read, "$(Data)"_r);
	return entrypoint<test>::run(argc, argv);
}