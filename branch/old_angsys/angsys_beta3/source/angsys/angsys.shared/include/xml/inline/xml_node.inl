#ifndef __XML_NODE_INL__
#define __XML_NODE_INL__
#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

namespace ang { namespace dom { namespace xml {

template<xml_encoding E>
xml_node<E>::xml_node(ixml_document_t doc, xml_type_t type)
	: m_type(type)
	, m_parent(null)
	, m_parent_doc(doc)
	, m_prev(null)
	, m_next(null)

{

}

template<xml_encoding E>
xml_node<E>::~xml_node() {

}

}}}
#undef NEW
#undef NEW_ARGS
#endif//__XML_NODE_INL__