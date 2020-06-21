#include "pch.h"
#include <ang/framework/framework.h>

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::scenes;

scene::scene()
{

}

scene::~scene()
{

}

string scene::sid()const
{
	return m_sid;
}

node_type_t scene::type()const
{
	return node_type::scene;
}


void scene::draw(idriver_t driver, iframe_buffer_t fbo, icamera_t cam)
{
	for (inode_t node : children()) {
		m_effect->draw(driver, fbo, this, node);
		//node->draw(driver, this, fbo, null);
	}
}

void scene::draw(idriver_t driver, iscene_t parent, iframe_buffer_t fbo, effects::ipass_t pass)
{
	draw(driver, fbo, active_camera());
}

void scene::update(core::time::step_timer const& timer)
{
	for (inode_t node : children()) {
		node->update(timer);
	}
}

inode_t scene::parent()const
{
	return null;
}

void scene::parent(inode_t)
{
}

collections::ienum_ptr<inode> scene::children()const
{
	return const_cast<scene*>(this);
}

collections::ilist_ptr<inode> scene::children()
{
	return const_cast<scene*>(this);
}

bool scene::load(resources::ilibrary_t lib, dom::xml::ixml_node_t node)
{
	m_library = new resources::library(lib);



	return true;
}

core::async::iasync<bool> scene::load_async(resources::ilibrary_t lib, dom::xml::ixml_node_t node)
{
	return m_library->dispatcher()->run_async<bool>([=](core::async::iasync<bool>)->bool
	{
		return load(lib, node);
	});
}

resources::ilibrary_t scene::library()const
{
	return m_library.get();
}

size<uint> scene::clip_size()const
{
	return size<uint>();
}

icamera_t scene::active_camera()const
{
	return null;
}

maths::float4 scene::ambient_color()const
{
	return maths::float4(0);
}

array<resources::light_info> scene::ligths()const
{
	return null;
}

wsize scene::counter()const
{
	return m_children.size();
}

inode_t scene::at(collections::base_iterator<inode> const& it)
{
#ifdef DEBUG_SAFE_CODE
	if (m_children.is_empty())
		throw_exception(error_code::invalid_memory);
	if ((inode**)&m_children[0] != it.current())
		throw_exception(error_code::invalid_param);
	if (it.offset() >= m_children.size())
		throw_exception(error_code::array_overflow);
#endif
	return m_children[it.offset()];
}

ang::collections::ienum_ptr<inode> scene::items()const
{
	return const_cast<scene*>(this);
}

void scene::copy(const ang::collections::ienum_ptr<inode>& items)
{
	m_children.clear();
	for (auto const& item : items) {
		m_children += item;
		item->parent(this);
	}
}

void scene::extend(const collections::ienum_ptr<inode>& items)
{

	for (auto const& item : items) {
		m_children += item;
		item->parent(this);
	}
}

void scene::push_front(inode_t&& node)
{
	m_children.push_front(forward<inode_t>(node));
}

void scene::push_front(inode_t const& node)
{
	m_children.push_front(node);
}

void scene::push_back(inode_t&& node)
{
	m_children.push_back(forward<inode_t>(node));
}

void scene::push_back(inode_t const& node)
{
	m_children.push_back(node);
}

collections::iterator<inode> scene::insert(collections::base_iterator<inode> it, inode_t&& node)
{
	auto it_ = m_children.insert(it.offset(), forward<inode_t>(node));
	return collections::iterator<inode>(this, (inode**)&m_children[0], it_.get_index());
}

collections::iterator<inode> scene::insert(collections::base_iterator<inode> it, inode_t const& node)
{
	auto it_ = m_children.insert(it.offset(), node);
	return collections::iterator<inode>(this, (inode**)&m_children[0], it_.get_index());
}

bool scene::pop_front(inode_ptr_t node)
{
	return node.is_empty() ? m_children.pop_front() : m_children.pop_front(*node);
}

bool scene::pop_back(inode_ptr_t node)
{
	return node.is_empty() ? m_children.pop_back() : m_children.pop_back(*node);
}

collections::iterator<inode> scene::remove(collections::base_iterator<inode> it, inode_ptr_t node)
{
	vector<inode_t>::iterator_t it_(vector<inode_t>::iterator_t().algorithm(), reinterpret_cast<collections::array_node<inode_t>*>(&m_children), windex(((inode*)(it.current())) + it.offset()));
	it_ = node.is_empty() ? m_children.remove(it_) : m_children.remove(it_, *node);
	return collections::iterator<inode>(this, (inode**)&m_children[0], it_.get_index());
}

collections::forward_iterator<inode> scene::begin()
{
	return collections::base_iterator(const_cast<scene*>(this), pointer((inode**)&m_children[0]));
}

collections::forward_iterator<inode> scene::end()
{
	return collections::base_iterator(const_cast<scene*>(this), pointer((inode**)&m_children[0]), m_children.size());
}

collections::const_forward_iterator<inode> scene::begin()const
{
	return collections::base_iterator(const_cast<scene*>(this), pointer((inode**)&m_children[0]));
}

collections::const_forward_iterator<inode> scene::end()const
{
	return collections::base_iterator(const_cast<scene*>(this), pointer((inode**)&m_children[0]), m_children.size());
}

collections::forward_iterator<inode> scene::last()
{
	return m_children.size() ? collections::base_iterator(const_cast<scene*>(this), pointer((inode**)&m_children[0]), m_children.size() - 1) : end();
}

collections::const_forward_iterator<inode> scene::last()const
{
	return m_children.size() ? collections::base_iterator(const_cast<scene*>(this), pointer((inode**)&m_children[0]), m_children.size() - 1) : end();
}

collections::backward_iterator<inode> scene::rbegin()
{
	return collections::base_iterator(const_cast<scene*>(this), pointer((inode**)&m_children[0]), m_children.size() - 1);
}

collections::backward_iterator<inode> scene::rend()
{
	return collections::base_iterator(const_cast<scene*>(this), pointer((inode**)&m_children[0]), (wsize)invalid_index);
}

collections::const_backward_iterator<inode> scene::rbegin()const
{
	return collections::base_iterator(const_cast<scene*>(this), pointer((inode**)&m_children[0]), m_children.size() - 1);
}

collections::const_backward_iterator<inode> scene::rend()const
{
	return collections::base_iterator(const_cast<scene*>(this), pointer((inode**)&m_children[0]), (wsize)invalid_index);
}

bool scene::increase(collections::base_iterator<inode>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != (inode**)&m_children[0])
		throw_exception(error_code::invalid_param);
	if (it.offset() >= m_children.size())
		throw_exception(error_code::array_overflow);
#endif
	it.offset(it.offset() + 1);
	if (it.offset() > m_children.size()) it.offset(m_children.size());
	return true;
}

bool scene::increase(collections::base_iterator<inode>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != (inode**)&m_children[0])
		throw_exception(error_code::invalid_param);
	if (it.offset() >= m_children.size())
		throw_exception(error_code::array_overflow);
#endif
	it.offset(it.offset() + offset);
	if (it.offset() >= m_children.size())
		it.offset(m_children.size());
	return true;
}

bool scene::decrease(collections::base_iterator<inode>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != (inode**)&m_children[0])
		throw_exception(error_code::invalid_param);
	if (it.offset() >= m_children.size())
		throw_exception(error_code::array_overflow);
#endif
	it.offset(it.offset() - 1);
	if ((int)it.offset() < -1)
		it.offset((wsize)invalid_index);
	return true;
}

bool scene::decrease(collections::base_iterator<inode>& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() != (inode**)&m_children[0])
		throw_exception(error_code::invalid_param);
	if (it.offset() >= m_children.size())
		throw_exception(error_code::array_overflow);
#endif
	it.offset(it.offset() - offset);
	if ((int)it.offset() < -1)
		it.offset((wsize)invalid_index);
	return true;
}
