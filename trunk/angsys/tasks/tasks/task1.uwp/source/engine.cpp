#include "pch.h"
#include "engine.h"

using namespace ang;
using namespace ang::graphics;

engine::engine()
{

}

engine::~engine()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(engine);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::engine, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(engine, object);


void engine::init(holographic_space_t hs)
{
	m_space = hs;
}

void engine::resume()
{

}

void engine::suspend()
{

}

void engine::update(core::time::step_timer_t time)
{

}

void engine::render()
{

}

void engine::exit()
{

}

