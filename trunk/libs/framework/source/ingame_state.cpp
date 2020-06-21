#include "pch.h"
#include <ang/framework/framework.h>

using namespace ang;
using namespace ang::framework;

ingame_state::ingame_state()
{

}

ingame_state::~ingame_state()
{

}

game_state_t ingame_state::state_id()const
{
	return game_state::ingame;
}

graphics::scenes::iscene_t ingame_state::scene()const
{
	return m_scene.get();
}

core::async::iasync<bool> ingame_state::load_async(igame_t game, dom::xml::ixml_node_t node)
{
	return game->dispatcher()->run_async<bool>([=](core::async::iasync<bool>)
	{
		m_scene = new graphics::scenes::scene();
		auto res = m_scene->load(game->library(), node);
		return res;
	});
}

bool ingame_state::update(igame_t game, core::time::step_timer const&)
{


	return true;
}

void ingame_state::draw(igame_t game, graphics::iframe_buffer_t, graphics::scenes::icamera_t)
{

}

bool ingame_state::unload(igame_t game)
{
	return false;
}
