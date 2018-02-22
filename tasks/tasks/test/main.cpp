// test.cpp : Defines the entry point for the console application.
//

#include "pch.h"

using namespace ang;

safe_enum(, encoding, uint)
{
	binary,
	ascii,
};

int main()
{
	max<1, 2>();
	max(1, 2);

    return 0;
}

