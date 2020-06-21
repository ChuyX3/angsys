#include "pch.h"
#include <ang/framework/framework.h>

using namespace ang;
using namespace ang::framework;

loading_state::loading_state()
{

}

loading_state::~loading_state()
{

}

game_state_t loading_state::state_id()const
{
	return game_state::loading;
}

graphics::scenes::iscene_t loading_state::scene()const
{
	return null;
}

core::async::iasync<bool> loading_state::load_async(igame_t game, dom::xml::ixml_node_t node)
{
	return game->dispatcher()->run_async<bool>([=](core::async::iasync<bool>)
	{
		m_state = game->state_manager()->create_state(node);
		m_args = node;
		return true;
	});
}

bool loading_state::update(igame_t game, core::time::step_timer const& timer)
{
	if (!m_state.is_empty() && m_task.is_empty())
		m_task = m_state->load_async(game, null);
	if (!m_task.is_empty() && m_task->status() == core::async::async_action_status::completed)
	{
		m_task->result();
		return true;
	}
	return false;
}

void loading_state::draw(igame_t game, graphics::iframe_buffer_t, graphics::scenes::icamera_t)
{

}

bool loading_state::unload(igame_t game)
{
	return false;
}
