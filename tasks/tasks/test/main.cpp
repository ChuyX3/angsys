// test.cpp : Defines the entry point for the console application.
//

#include "pch.h"

using namespace ang;

safe_flags(, encoding, uint)
{
	binary,
	ascii,
};

int main()
{
	encoding_t e;
	e += encoding::ascii;


    return 0;
}

