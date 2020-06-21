#include "pch.h"
#include <ang/framework/framework.h>

using namespace ang;
using namespace ang::framework;

state_manager::state_manager()
{

}

state_manager::~state_manager()
{

}

istate_t state_manager::current()const
{
	return *m_states.front();
}

istate_t state_manager::create_state(dom::xml::ixml_node_t)const
{
	return null;
}

bool state_manager::load(igame_t, dom::xml::ixml_node_t node)
{
	return true;
}

void state_manager::update(igame_t game, core::time::step_timer const& timer)
{
	istate_t state = null, next = null;
	auto it = m_states.front();
	if (it.is_valid()) {
		state = *it;
		if (!state->update(game, timer))
		{
			next = state->next_state();
			m_states.pop(state);
			state->unload(game);
			m_states.push(next);
		}
	}
}

void state_manager::draw(igame_t)
{

}

bool state_manager::unload()
{
	return true;
}
