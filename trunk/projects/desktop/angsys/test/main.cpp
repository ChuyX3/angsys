// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"


using namespace ang;
using namespace ang::platform;


ang_declare_object(engine);


int main()
{
	framework::game_t game = new framework::game();
	game->run(graphics::graph_driver_type::DirectX11, "puyopuyo"_s, "Puyo Puyo"_s);
	return 0;
}