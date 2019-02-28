#ifndef __XML_COLLECTION_INL__
#define __XML_COLLECTION_INL__
#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

namespace ang{namespace dom{namespace xml{

template<xml_encoding E>
/*inline*/ xml_node<E>::xml_collection::xml_collection(xml_node_ptr<E> parent, xml_type_t type)
	: m_type(type)
	, m_count(0)
	, m_first(null)
	, m_last(null)
	, m_parent(parent)
{

}

template<xml_encoding E>
/*inline*/ xml_node<E>::xml_collection::~xml_collection()
{
	clear();
}


template<xml_encoding E>
/*inline*/ ang::rtti_t const& xml_node<E>::xml_collection::class_info()
{
	static const cstr_view<char> name = strings::string_pool::instance()->save_string((string("ang::dom::xml::xml_collection<"_s) += xml_encoding_t(E).to_string()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<object>(), &runtime::type_of<ixml_collection>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<xml_node<E>::xml_collection>(), align_of<xml_node<E>::xml_collection>(), parents, &default_query_interface);
	return info;
}

template<xml_encoding E>
/*inline*/ ang::rtti_t const& xml_node<E>::xml_collection::runtime_info()const
{
	return class_info();
}


template<xml_encoding E>
/*inline*/ bool xml_node<E>::xml_collection::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == class_info().type_id())
	{
		if (out == null) return false;
		*out = static_cast<xml_node<E>::xml_collection*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == type_of<ixml_collection>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ixml_collection*>(this);
		return true;
	}
	else if (id.type_id() == type_of<ixml_object>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ixml_object*>(this);
		return true;
	}
	else if (id.type_id() == type_of<collections::ienum<ixml_node>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<collections::ienum<ixml_node>*>(this);
		return true;
	}
	return false;
}

template<xml_encoding E>
/*inline*/ void xml_node<E>::xml_collection::clear() {
	while (pop());
}

template<xml_encoding E>
/*inline*/ xml_type_t xml_node<E>::xml_collection::xml_type()const {
	return m_type;
}

template<xml_encoding E>
/*inline*/ bool xml_node<E>::xml_collection::xml_is_type_of(xml_type_t type)const {
	return m_type == type || xml_type::store == type;
}

template<xml_encoding E>
/*inline*/ streams::itext_output_stream_t& xml_node<E>::xml_collection::xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const {

	switch (m_type)
	{
	case xml_type::attribute_list:
		for (ixml_node_t node : *this)
			node->xml_print(stream, flag, level) << " "_s;
		break;
	case xml_type::element_list:
		for (ixml_node_t node : *this)
		{
			if ((flag & xml_format::wrap_text_tab)
				|| (flag & xml_format::wrap_text_space))
				stream << L"\n"_s;
			node->xml_print(stream, flag, level);
		}
		break;
	}
	return stream;
}

template<xml_encoding E>
/*inline*/ ixml_node_t xml_node<E>::xml_collection::xml_parent()const {
	return m_parent.lock();
}

template<xml_encoding E>
/*inline*/ ixml_document_t xml_node<E>::xml_collection::xml_parent_doc()const {
	ixml_node_t parent = xml_parent();
	return parent.is_empty() ? null : parent->xml_parent_doc();
}

template<xml_encoding E>
/*inline*/ wsize xml_node<E>::xml_collection::counter()const {
	return m_count;
}

template<xml_encoding E>
/*inline*/ xml::ixml_node_t xml_node<E>::xml_collection::at(xml_base_iterator_t const& it) {
	if (!counter()) throw(ang::exception_t(except_code::invalid_memory));
	if (it.parent() != this)throw(ang::exception_t(except_code::invalid_param));
	return reinterpret_cast<ixml_node*>(it.current());
}

template<xml_encoding E>
/*inline*/ bool xml_node<E>::xml_collection::increase(xml_base_iterator_t& it)const {
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	it.current(reinterpret_cast<ixml_node*>(it.current())->xml_next_sibling());
	if (it.current() == null)
		return false;
	return true;
}

template<xml_encoding E>
/*inline*/ bool xml_node<E>::xml_collection::increase(xml_base_iterator_t& it, int offset)const {
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	auto node = reinterpret_cast<ixml_node*>(it.current());
	int c = 0;
	while (node && c < offset) {
		node = node->xml_next_sibling();
		c++;
	}
	it.current(node);
	if (it.current() == null)
		return false;
	return true;
}

template<xml_encoding E>
/*inline*/ bool xml_node<E>::xml_collection::decrease(xml_base_iterator_t& it)const {
	if (it.parent() != this) throw(ang::exception_t(except_code::invalid_memory));
	it.current(reinterpret_cast<ixml_node*>(it.current())->xml_prev_sibling());
	if (it.current() == null)
		return false;
	return true;
}

template<xml_encoding E>
/*inline*/ bool xml_node<E>::xml_collection::decrease(xml_base_iterator_t& it, int offset)const {

	if (it.parent() != this) throw(ang::exception_t(except_code::invalid_memory));
	auto node = reinterpret_cast<ixml_node*>(it.current());
	int c = 0;
	while (node && c < offset) {
		node = node->xml_prev_sibling();
		c++;
	}
	it.current(node);
	if (it.current() == null)
		return false;
	return true;
}


template<xml_encoding E>
/*inline*/ xml_forward_iterator_t xml_node<E>::xml_collection::begin() {
	return xml_iterator_t(const_cast<self_t*>(this), (pointer)(ixml_node*)xml_first());
}

template<xml_encoding E>
/*inline*/ xml_forward_iterator_t xml_node<E>::xml_collection::end() {
	return xml_iterator_t(const_cast<self_t*>(this), null);
}

template<xml_encoding E>
/*inline*/ xml_const_forward_iterator_t xml_node<E>::xml_collection::begin()const {
	return xml_iterator_t(const_cast<self_t*>(this), (pointer)(ixml_node*)xml_first());
}

template<xml_encoding E>
/*inline*/ xml_const_forward_iterator_t xml_node<E>::xml_collection::end()const {
	return xml_iterator_t(const_cast<self_t*>(this), null);
}

template<xml_encoding E>
/*inline*/ xml_forward_iterator_t xml_node<E>::xml_collection::last() {
	return xml_iterator_t(const_cast<self_t*>(this), (pointer)(ixml_node*)xml_last());
}

template<xml_encoding E>
/*inline*/ xml_const_forward_iterator_t xml_node<E>::xml_collection::last()const {
	return xml_iterator_t(const_cast<self_t*>(this), (pointer)(ixml_node*)xml_last());
}

template<xml_encoding E>
/*inline*/ xml_backward_iterator_t xml_node<E>::xml_collection::rbegin() {
	return xml_iterator_t(const_cast<self_t*>(this), (pointer)(ixml_node*)xml_last());
}

template<xml_encoding E>
/*inline*/ xml_backward_iterator_t xml_node<E>::xml_collection::rend() {
	return xml_iterator_t(const_cast<self_t*>(this), null);
}

template<xml_encoding E>
/*inline*/ xml_const_backward_iterator_t xml_node<E>::xml_collection::rbegin()const {
	return xml_iterator_t(const_cast<self_t*>(this), (pointer)(ixml_node*)xml_last());
}

template<xml_encoding E>
/*inline*/ xml_const_backward_iterator_t xml_node<E>::xml_collection::rend()const {
	return xml_iterator_t(const_cast<self_t*>(this), null);
}


template<xml_encoding E>
/*inline*/ xml_iterator_t xml_node<E>::xml_collection::find(raw_str_t value, bool invert)const {
	ixml_node_t node;
	if (invert)
	{
		for (node = xml_last();
			(node != null);
			node = node->xml_prev_sibling()) {
			ixml_string_t name = node->xml_name();
			if (!name.is_empty() && name->compare(value) == 0)
				break;
		}
	}
	else
	{
		for (node = xml_first();
			(node != null);
			node = node->xml_next_sibling()) {
			ixml_string_t name = node->xml_name();
			if (!name.is_empty() && name->compare(value) == 0)
				break;
		}
	}
	return xml_iterator_t(const_cast<self_t*>(this), (pointer)node.get());
}

template<xml_encoding E>
/*inline*/ xml_iterator_t xml_node<E>::xml_collection::find(raw_str_t value, xml_iterator_t next_to, bool invert)const {
	ixml_node_t node = null;
	if (next_to.is_valid() && next_to.parent() == this)
		node = reinterpret_cast<xml_node*>(next_to.current());
	if (invert)
	{
		if (node == null)
			node = xml_last();
		for (; (node != null);
			node = node->xml_prev_sibling()) {
			ixml_string_t name = node->xml_name();
			if (!name.is_empty() && name->compare(value) == 0)
				break;
		}
	}
	else
	{
		if (node == null)
			node = xml_first();
		for (; (node != null);
			node = node->xml_next_sibling()) {
			ixml_string_t name = node->xml_name();
			if (!name.is_empty() && name->compare(value) == 0)
				break;
		}
	}
	return xml_iterator_t(const_cast<self_t*>(this), (pointer)node.get());
}

template<xml_encoding E>
/*inline*/ bool xml_node<E>::xml_collection::push(ixml_node_t node, bool last) {
	if (node.is_empty()
		|| (xml_is_type_of(xml_type::attribute_list) && !node->xml_is_type_of(xml_type::attribute))
		|| (xml_is_type_of(xml_type::element_list) && !node->xml_is_type_of(xml_type::element) && !node->xml_is_type_of(xml_type::comment))
		) return false;

	node->xml_parent(xml_parent());
	if (last)
	{
		if (xml_first() == null)
		{
			xml_first(node);
			xml_last(node);
		}
		else
		{
			xml_last()->xml_next_sibling(node);
			node->xml_prev_sibling(xml_last());
			xml_last(node);
		}
	}
	else
	{
		if (xml_first() == null)
		{
			xml_first(node);
			xml_last(node);
		}
		else
		{
			xml_first()->xml_prev_sibling(node);
			node->xml_next_sibling(xml_first());
			xml_first(node);
		}
	}
	m_count++;
	return true;
}

template<xml_encoding E>
/*inline*/ bool xml_node<E>::xml_collection::insert(ixml_node_t node, xml_iterator_t at, bool next_to) {
	if (node.is_empty()
		|| (xml_is_type_of(xml_type::attribute_list) && !node->xml_is_type_of(xml_type::attribute))
		|| (xml_is_type_of(xml_type::element_list) && !node->xml_is_type_of(xml_type::element) && !node->xml_is_type_of(xml_type::comment))
		) return false;

	if (node == null)
		return false;

	if (!at.is_valid() || at.parent() != this)
	{
		return push(node, next_to);
	}

	if (next_to && *at == xml_last())
		return push(node, true);

	if (!next_to && *at == xml_first())
		return push(node, false);

	node->xml_parent(xml_parent());
	if (next_to)
	{
		at->xml_next_sibling(node);
		node->xml_prev_sibling(*at);
	}
	else
	{
		at->xml_prev_sibling(node);
		node->xml_next_sibling(*at);
	}
	m_count++;
	return true;
}

template<xml_encoding E>
/*inline*/ bool xml_node<E>::xml_collection::pop(ixml_node_ptr_t out, bool last) {
	if (m_count == 0)
		return false;
	if (m_count == 1)
	{
		if (!out.is_empty())
		{
			*out = xml_first();
		}
		xml_first(null);
		xml_last(null);
	}
	else
	{
		ixml_node_t temp = null;
		if (last)
		{
			temp = xml_last();
			if (!out.is_empty())
			{
				*out = temp;
			}
			temp->xml_prev_sibling()->xml_next_sibling(null);
			xml_last(temp->xml_prev_sibling());
			temp->xml_prev_sibling(null);
		}
		else
		{
			temp = xml_first();
			if (!out.is_empty())
			{
				*out = temp;
			}
			temp->xml_next_sibling()->xml_prev_sibling(null);
			xml_first(temp->xml_prev_sibling());
			temp->xml_next_sibling(null);
		}
	}
	m_count--;
	return true;
}

template<xml_encoding E>
/*inline*/ bool xml_node<E>::xml_collection::pop_at(xml_iterator_t it, xml::ixml_node_ptr_t out) {
	if (m_count == 0)
		return false;

	if (it.parent() != this)
		return false;
	if (it.current() == null)
		return false;

	ixml_node_t temp = *it;
	if (!out.is_empty())
	{
		*out = temp;
	}

	if (temp->xml_prev_sibling() != null)
		temp->xml_prev_sibling()->xml_next_sibling(temp->xml_next_sibling());

	if (temp->xml_next_sibling() != null)
		temp->xml_next_sibling()->xml_prev_sibling(temp->xml_prev_sibling());

	if (temp == xml_first())
		xml_first(temp->xml_next_sibling());
	if (temp == xml_last())
		xml_last(temp->xml_prev_sibling());

	temp->xml_next_sibling(null);
	temp->xml_prev_sibling(null);

	m_count--;
	return true;
}

template<xml_encoding E>
/*inline*/ ixml_node* xml_node<E>::xml_collection::xml_first()const {
	return m_first;
}

template<xml_encoding E>
/*inline*/ ixml_node* xml_node<E>::xml_collection::xml_last()const {
	return m_last;
}

template<xml_encoding E>
/*inline*/ void xml_node<E>::xml_collection::xml_first(ixml_node* node) {
	if (m_first == node) return;
	auto old = m_first;
	m_first = (node) ? node->xml_as<xml_node<E>>().get() : null;
	if (m_first)m_first->add_ref();
	if (old)old->release();
}

template<xml_encoding E>
/*inline*/ void xml_node<E>::xml_collection::xml_last(ixml_node* node) {
	if (m_last == node) return;
	auto old = m_last;
	m_last = (node) ? node->xml_as<xml_node<E>>().get() : null;
	if (m_last)m_last->add_ref();
	if (old)old->release();
}


}}}
#undef NEW
#undef NEW_ARGS
#endif//__XML_COLLECTION_INL__