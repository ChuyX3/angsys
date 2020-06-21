// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "puyo.h"

using namespace ang;

struct light_data
{
	maths::float4 ambient_color;
	uint light_count;
	float specular_power;
	struct {
		maths::float4 color;
		maths::float3 position;
		uint type;
	}light_data[10];
};

int main(int argc, char* argv[])
{
	return entrypoint<puyo>::run(argc, argv);
}