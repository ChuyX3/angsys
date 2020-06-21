#include "pch.h"
#include "xml/xml_impl.h"

using namespace ang;
using namespace ang::dom;
using namespace ang::dom::xml;


xml_attribs::xml_attribs()
	:  m_ptr(null)
{

}

xml_attribs::xml_attribs(xml::ixml_items* attbs)
	: m_ptr(null)
{
	set(attbs);
}

xml_attribs::xml_attribs(xml_attribs && other)
	: m_ptr(null)
{
	m_ptr = other.m_ptr;
	other.m_ptr = null;
}

xml_attribs::xml_attribs(xml_attribs const& other)
	: m_ptr(null)
{
	set(other.get());
}

xml_attribs::xml_attribs(std::nullptr_t const&)
	: m_ptr(null)
{

}

xml_attribs::~xml_attribs()
{
	reset();
}
		
void xml_attribs::reset()
{
	iobject * _obj = interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

void xml_attribs::reset_unsafe()
{
	m_ptr = null;
}

bool xml_attribs::is_empty()const
{
	return m_ptr == null;
}

xml::ixml_items* xml_attribs::get(void)const
{
	return m_ptr;
}

void xml_attribs::set(xml::ixml_items* ptr)
{
	if (ptr == m_ptr)
		return;
	iobject * _old = interface_cast<iobject>(m_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

xml::ixml_items ** xml_attribs::addres_of(void)
{
	return &m_ptr;
}

ixml_attbs_t& xml_attribs::operator = (xml::ixml_items* ptr)
{
	set(ptr);
	return*this;
}

ixml_attbs_t& xml_attribs::operator = (ang::nullptr_t const&) {
	reset();
	return*this;
}

ixml_attbs_t& xml_attribs::operator = (ixml_attbs_t && ptr)
{
	if (this != &ptr)
	{
		reset();
		m_ptr = ptr.m_ptr;
		ptr.m_ptr = null;
	}
	return*this;
}

ixml_attbs_t& xml_attribs::operator = (ixml_attbs_t const& ptr)
{
	set(ptr.get());
	return*this;
}

xml::ixml_items * xml_attribs::operator -> (void)const
{
	return m_ptr;
}

xml_attribs::operator xml::ixml_items * (void)
{
	return m_ptr;
}

xml_attribs::operator xml::ixml_items const* (void)const
{
	return m_ptr;
}

ixml_text_t xml_attribs::operator[](cstr_t name)const
{
	if (m_ptr == null)
		return null;
	auto it = m_ptr->find(name);
	return it.is_valid() ? it->value().get() : null;
}
