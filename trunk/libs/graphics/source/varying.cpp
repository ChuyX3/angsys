#include "pch.h"
#include <ang/graphics/graphics.h>
//#include "ang_inlines.h"
//#include "vector_specialization.inl"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::reflect;


algorithms::varying_iteration::varying_iteration()
{
	vtable = s_vtable;
}

typename algorithms::varying_iteration::node_ptr_type algorithms::varying_iteration::begin(node_ptr_type node, windex* idx)
{
	if (idx != null)
		*idx = 0;
	return node;
}

typename algorithms::varying_iteration::node_ptr_type algorithms::varying_iteration::end(node_ptr_type node, windex* idx)
{
	if (idx != null)
		*idx = invalid_index;
	return node;
}

typename algorithms::varying_iteration::node_ptr_type algorithms::varying_iteration::rbegin(node_ptr_type node, windex* idx)
{
	if (idx != null)
		*idx = node->counter() - 1;
	return node;
}

typename algorithms::varying_iteration::node_ptr_type algorithms::varying_iteration::rend(node_ptr_type node, windex* idx)
{
	if (idx != null)
		*idx = invalid_index;
	return node;
}

typename algorithms::varying_iteration::node_ptr_type algorithms::varying_iteration::increase(node_ptr_type node, windex* idx)
{
	wsize count = node->counter();
#ifdef DEBUG_SAFE_CODE
	if (node == null || idx == null)
		throw_exception(error_code::array_overflow);
	if (*idx >= count)
		throw_exception(error_code::array_overflow);
#endif
	(*idx)++;
	if (*idx >= count)
		*idx = invalid_handle;
	return node;
}

typename algorithms::varying_iteration::node_ptr_type algorithms::varying_iteration::decrease(node_ptr_type node, windex* idx)
{
	wsize count = node->counter();
#ifdef DEBUG_SAFE_CODE
	if (node == null || idx == null)
		throw_exception(error_code::array_overflow);
	if (*idx >= count)
		throw_exception(error_code::array_overflow);
#endif
	(*idx)--;
	if (*idx >= count)
		*idx = invalid_handle;
	return node;
}

typename algorithms::varying_iteration::node_ptr_type algorithms::varying_iteration::most_left(node_ptr_type node, windex* idx)
{
	if (idx != null)
		*idx = 0;
	return node;
}

typename algorithms::varying_iteration::node_ptr_type algorithms::varying_iteration::most_right(node_ptr_type node, windex* idx)
{
	if (idx != null)
		*idx = node->counter() - 1;
	return node;
}

///////////////////////////////////////////////////////////////////////////////

varying::varying()
	: m_raw_data()
	, m_descriptor()
{
}

varying::varying(varying && other)
	: m_raw_data(ang::move(other.m_raw_data))
	, m_descriptor(ang::move(other.m_descriptor))
{
}

varying::varying(varying const& other)
	: m_raw_data(other.m_raw_data)
	, m_descriptor(other.m_descriptor)
{
}

varying::varying(array_view<byte> bytes, varying_desc desc, wsize aligment)
	: m_raw_data(ang::move(bytes))
	, m_descriptor(ang::move(desc))
{
	if(aligment != invalid_index)
		m_descriptor.aligment(aligment);
	auto size = m_descriptor.get_size_in_bytes();
	if (bytes.size() < size)
		throw(exception(error_code::invalid_param));
}

varying::varying(array_view<byte> bytes, array_view<attribute_desc_t>const& desc, wsize aligment)
	: m_raw_data(ang::move(bytes))
	, m_descriptor()
{
	m_descriptor.var_type(var_type::block);
	m_descriptor.var_class(var_class::scalar);

	for(auto att : desc)
		m_descriptor.push_field(varying_desc(att.var_type(), att.var_class(), att.var_name(), 1));

	if (aligment != invalid_index)
		m_descriptor.aligment(aligment);
	auto size = m_descriptor.get_size_in_bytes();
	if (bytes.size() < size)
		throw(exception(error_code::invalid_param));
	m_descriptor.array_count(bytes.size() / size);
}

ang::rtti_t const& varying::runtime_info()const {
	return class_info();
}

bool varying::query_interface(ang::rtti_t const& id, unknown_ptr_t) {
	//TODO:
	return false;
}

wsize varying::aligment()const { return m_descriptor.aligment(); }

array_view<byte> varying::raw_data()const { return m_raw_data; }

varying_desc const& varying::descriptor()const { return m_descriptor; }

varying varying::field(windex idx) 
{
	if (m_descriptor.array_count() > 1)
	{
		if(idx >= m_descriptor.array_count())
			return varying();

		wsize sz = m_descriptor.get_size_in_bytes() / m_descriptor.array_count();
		varying_desc desc = m_descriptor;
		desc.array_count(1);
		return varying(to_array(&m_raw_data[sz * idx], &m_raw_data[sz * idx] + sz), desc, m_descriptor.aligment());
	}
	else
	{
		if (m_descriptor.var_type() != var_type::block)
			return varying();
		uniform_fields_t const& fields = m_descriptor.fields();
		if (fields.is_empty() || fields.size() <= idx)
			return varying();

		varying va;
		va.m_descriptor = fields[idx];
		va.m_raw_data.set(&m_raw_data[va.m_descriptor.position()], &m_raw_data[va.m_descriptor.position()] + va.m_descriptor.get_size_in_bytes());
		return ang::move(va);
	}
}

varying varying::field(cstr_t name)
{
	if(m_descriptor.array_count() > 1)
		return varying();

	auto idx = find_field(name);
	if (invalid_index == idx)
		return varying();

	varying va;
	va.m_descriptor = m_descriptor.fields()[idx];
	va.m_raw_data.set(&m_raw_data[va.m_descriptor.position()], &m_raw_data[va.m_descriptor.position()] + va.m_descriptor.get_size_in_bytes());
	return ang::move(va);
}

windex varying::find_field(cstr_t name)const
{
	if (m_descriptor.var_type() != var_type::block)
		return (windex)invalid_index;
	uniform_fields_t const& fields = m_descriptor.fields();
	if (fields.is_empty())
		return (windex)invalid_index;
	windex i = 0;
	for (auto const& field : fields) {
		if ((castr_t)field.var_name() == name)
			return i;
		i++;
	}	
	return (windex)invalid_index;
}

varying varying::operator [](windex idx)
{
	auto count = m_descriptor.array_count();
	if (count > 1 && idx < count)
	{
		auto size = m_descriptor.get_size_in_bytes() / count;
		varying_desc desc = m_descriptor;
		desc.array_count(1);
		return varying(collections::to_array(&m_raw_data.get()[idx * size], &m_raw_data.get()[idx * size] + size), desc);
	}
	else
		return field(idx);
}

varying varying::operator [](cstr_t name)
{
	return field(name);
}

collections::vector<varying> varying::fragment()
{
	if (m_descriptor.array_count() > 1)
		return ang::move(array_cast<varying>());
	if (m_descriptor.var_type() != var_type::block)
		return initializer_list<varying>{*this};
	collections::vector<varying> vars;
	for (auto const& field : m_descriptor.fields())
		vars += varying{collections::to_array(&m_raw_data.get()[field.position()], &m_raw_data.get()[field.position()] + field.get_size_in_bytes()), field};
	return ang::move(vars);
}

collections::vector<varying> varying::varying_cast<array_view<varying>>::force_cast(varying* va)
{
	auto count = va->descriptor().array_count();
	auto size = va->descriptor().get_size_in_bytes() / count;
	varying_desc desc = va->descriptor();
	desc.array_count(1);

	collections::vector<varying> vars;
	for (windex i = 0U; i < count; ++i)
		vars += varying( collections::to_array(&va->m_raw_data.get()[i * size], &va->m_raw_data.get()[i * size] + size), desc);
	return ang::move(vars);
}

collections::vector<varying> varying::varying_cast<array_view<varying>>::cast(varying* va)
{
	auto count = va->descriptor().array_count();
	auto size = va->descriptor().get_size_in_bytes() / count;
	varying_desc desc = va->descriptor();
	desc.array_count(1);

	collections::vector<varying> out;
	for (windex i = 0U; i < count; ++i)
		out += varying(collections::to_array(&va->m_raw_data.get()[i * size], &va->m_raw_data.get()[i * size] + size), desc);
	return out;
}

varying& varying::operator = (varying && other) {
	m_raw_data = ang::move(other.m_raw_data);
	m_descriptor = ang::move(other.m_descriptor);
	return*this;
}

varying& varying::operator = (varying const& other) {
	m_raw_data = other.m_raw_data;
	m_descriptor =other.m_descriptor;
	return*this;
}

bool varying::operator == (const varying& other)const
{
	return m_descriptor == other.m_descriptor;
}

bool varying::operator != (const varying& other)const
{
	return m_descriptor != other.m_descriptor;
}

wsize varying::counter()const
{
	return m_descriptor.array_count() > 1 ?
		m_descriptor.array_count() :
		!m_descriptor.fields().is_empty() ?
		m_descriptor.fields().size() :
		1;
}

bool varying::try_copy(varying const& value) {
	if (value.descriptor() == descriptor())
		return false;
	//temporal fix
	memcpy(m_raw_data.data(), value.m_raw_data.data(), min(m_raw_data.size(), value.m_raw_data.size()));
	return true;
}

//varying_t varying::at(varying::base_iterator_t const& it)
//{
//#ifdef DEBUG_SAFE_CODE
//	if (this != it.parent())
//		throw_exception(error_code::invalid_param);
//	if (it.offset() >= counter())
//		throw_exception(error_code::array_overflow);
//#endif
//	return field(it.offset());
//}

varying::iterator_t varying::begin(algorithms::iteration_algorithm<varying> iter)
{
	wsize idx = 0;
	return iterator_t(iter, iter.begin(const_cast<varying*>(this), &idx), idx);
}

varying::iterator_t varying::end(algorithms::iteration_algorithm<varying> iter)
{
	wsize idx = 0;
	return iterator_t(iter, iter.end(const_cast<varying*>(this), &idx), idx);
}

varying::const_iterator_t varying::begin(algorithms::iteration_algorithm<varying> iter)const
{
	wsize idx = 0;
	return const_iterator_t(iter, iter.begin(const_cast<varying*>(this), &idx), idx);
}

varying::const_iterator_t varying::end(algorithms::iteration_algorithm<varying> iter)const
{
	wsize idx = 0;
	return const_iterator_t(iter, iter.end(const_cast<varying*>(this), &idx), idx);
}

varying::reverse_iterator_t varying::rbegin(algorithms::iteration_algorithm<varying> iter)
{
	wsize idx = 0;
	return reverse_iterator_t(iter, iter.rbegin(const_cast<varying*>(this), &idx), idx);
}

varying::reverse_iterator_t varying::rend(algorithms::iteration_algorithm<varying> iter)
{
	wsize idx = 0;
	return reverse_iterator_t(iter, iter.rend(const_cast<varying*>(this), &idx), idx);
}

varying::reverse_const_iterator_t varying::rbegin(algorithms::iteration_algorithm<varying> iter)const
{
	wsize idx = 0;
	return reverse_const_iterator_t(iter, iter.rbegin(const_cast<varying*>(this), &idx), idx);
}

varying::reverse_const_iterator_t varying::rend(algorithms::iteration_algorithm<varying> iter)const
{
	wsize idx = 0;
	return reverse_const_iterator_t(iter, iter.rend(const_cast<varying*>(this), &idx), idx);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


vertex_buffer::vertex_buffer()
{

}

vertex_buffer::vertex_buffer(vertex_buffer const* buff)
{

}

vertex_buffer::vertex_buffer(collections::array_view<attribute_desc> const& desc, windex idx)
{

}

vertex_buffer::~vertex_buffer()
{
	dispose();
}

void vertex_buffer::dispose()
{
	clear();
}

bool vertex_buffer::is_readonly()const
{
	return true;
}

text::encoding_t vertex_buffer::encoding()const
{
	return text::encoding::binary;
}

pointer vertex_buffer::buffer_ptr()
{
	return m_aligned_data.data();
}

const_pointer vertex_buffer::buffer_ptr()const
{
	return m_aligned_data.data();
}

wsize vertex_buffer::buffer_size()const
{
	return m_stride * m_array_count;
}

wsize vertex_buffer::mem_copy(wsize sz, pointer ptr, text::encoding_t e)
{
	if (e != text::encoding::binary)
		return 0;
	memcpy(m_aligned_data.data(), ptr, min(m_stride * m_array_count, sz));
	return  min(m_stride * m_array_count, sz);
}

ibuffer_view_t vertex_buffer::map_buffer(windex, wsize)
{
	return null;
}

bool vertex_buffer::unmap_buffer(ibuffer_view_t&, wsize)
{
	return false;
}

bool vertex_buffer::can_realloc_buffer()const
{
	return false;
}

bool vertex_buffer::realloc_buffer(wsize sz)
{
	m_aligned_data.capacity(sz / size_of<long64>(), true);
	m_aligned_data.size(sz / size_of<long64>());
	//m_raw_data = to_array((byte*)&m_aligned_data[0], (byte*)&m_aligned_data[0] + sz);
	return true;
}

void vertex_buffer::clear()
{
	m_aligned_data.clear();
	m_descriptor.clear();
}

bool vertex_buffer::load(dom::xml::ixml_node_t node)
{
	if (node.is_empty() || !node->has_children())
		return false;

	cstr_t data;
	wsize size_in_bytes = 0;
	m_name = node->attributes()["name"]->as<cstr_t>();
	for (dom::xml::ixml_node_t node : node->children())
	{
		auto name = node->name()->as<cstr_t>();
		if (name == "data_layout"_sv && node->has_children())
		{
			for (dom::xml::ixml_node_t att : node->children())
			{
				attribute_desc_t desc;
				desc.load(att);
				m_descriptor += move(desc);
			}
			m_array_count = node->attributes()["array"_sv]->as<uint>();
		}
		else if (name == "data"_sv)
		{
			data = node->value();			
		}
	}

	attribute_desc::calculate_positions(*reinterpret_cast<array_view<attribute_desc_t>*>(&m_descriptor));
	m_stride = attribute_desc::get_size_in_bytes(m_descriptor);
	realloc_buffer(m_stride * m_array_count);
	memset(m_aligned_data.data(), 0, m_aligned_data.size());

	text_data_loader_context_t parser;
	text_data_loader::create_context(m_descriptor, parser);
	parser.array_count = m_array_count;

	text_data_loader_input_context_t input = {
		//text::iparser::get_parser(data.encoding()),
		data,
		0
	};
	text_data_loader_output_context_t output = {
		(byte*)m_aligned_data.data(),
		0
	};
	parser.load_data(parser, input, output);
	return true;
}

bool vertex_buffer::save(dom::xml::ixml_document_t doc)
{
	return false;
}

wsize vertex_buffer::array_count()const
{
	return m_array_count;
}

collections::array_view<attribute_desc> vertex_buffer::descriptor()const
{
	return m_descriptor;
}

varying vertex_buffer::make(collections::array_view<attribute_desc> const& desc, wsize count)
{
	clear();
	m_descriptor = desc;
	m_array_count = max(count, 1);
	attribute_desc::calculate_positions(*reinterpret_cast<array_view<attribute_desc_t>*>(&m_descriptor));
	m_stride = attribute_desc::get_size_in_bytes(m_descriptor);
	realloc_buffer(m_stride * m_array_count);

	memset(m_aligned_data.data(), 0, m_stride * m_array_count);

	return varying(to_array<byte>((byte*)(&m_aligned_data[0]), (byte*)(&m_aligned_data[0] + (m_stride * m_array_count))), desc, 16);
}

varying vertex_buffer::data()
{
	return varying(to_array<byte>((byte*)(&m_aligned_data[0]), (byte*)(&m_aligned_data[0] + (m_stride * m_array_count))), m_descriptor, 16);
}

varying vertex_buffer::block(windex idx)
{
	if (idx >= m_array_count)
		return varying();
	auto data = to_array((byte*)(&m_aligned_data[0] + (m_stride * idx)), (byte*)(&m_aligned_data[0] + (m_stride * (idx + 1))));
	return varying(data, m_descriptor, 16);
}

vertex_buffer::iterator_t vertex_buffer::begin(algorithms::iteration_algorithm<varying> it)
{
	return const_cast<vertex_buffer*>(this)->data().begin(it);
}

vertex_buffer::iterator_t vertex_buffer::end(algorithms::iteration_algorithm<varying> it)
{
	return const_cast<vertex_buffer*>(this)->data().end(it);
}

vertex_buffer::const_iterator_t vertex_buffer::begin(algorithms::iteration_algorithm<varying> it)const
{
	return const_cast<vertex_buffer*>(this)->data().begin(it);
}

vertex_buffer::const_iterator_t vertex_buffer::end(algorithms::iteration_algorithm<varying> it)const
{
	return const_cast<vertex_buffer*>(this)->data().end(it);
}

vertex_buffer::reverse_iterator_t vertex_buffer::rbegin(algorithms::iteration_algorithm<varying> it)
{
	return const_cast<vertex_buffer*>(this)->data().rbegin(it);
}

vertex_buffer::reverse_iterator_t vertex_buffer::rend(algorithms::iteration_algorithm<varying> it)
{
	return const_cast<vertex_buffer*>(this)->data().rend(it);
}

vertex_buffer::reverse_const_iterator_t vertex_buffer::rbegin(algorithms::iteration_algorithm<varying> it)const
{
	return const_cast<vertex_buffer*>(this)->data().rbegin(it);
}

vertex_buffer::reverse_const_iterator_t vertex_buffer::rend(algorithms::iteration_algorithm<varying> it)const
{
	return const_cast<vertex_buffer*>(this)->data().rend(it);
}

varying object_wrapper<vertex_buffer>::operator[](windex idx)
{
	return m_ptr ? move(m_ptr->block(idx)) : varying();
}

vertex_buffer::iterator_t object_wrapper<vertex_buffer>::begin()
{
	return m_ptr ? m_ptr->begin() : vertex_buffer::iterator_t();
}

vertex_buffer::iterator_t object_wrapper<vertex_buffer>::end()
{
	return m_ptr ? m_ptr->end() : vertex_buffer::iterator_t();
}

vertex_buffer::const_iterator_t object_wrapper<vertex_buffer>::begin()const
{
	return m_ptr ? m_ptr->begin() : vertex_buffer::const_iterator_t();
}

vertex_buffer::const_iterator_t object_wrapper<vertex_buffer>::end()const
{
	return m_ptr ? m_ptr->end() : vertex_buffer::const_iterator_t();
}

#define MY_TYPE vertex_buffer
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct_buffer::struct_buffer()
{

}

struct_buffer::struct_buffer(struct_buffer const* buff)
{
	if (buff)
	{
		copy(*buff);
	}
}

struct_buffer::struct_buffer(varying_t const& va)
{
	copy(va);
}

struct_buffer::struct_buffer(varying_desc desc, wsize aligment)
{
	m_descriptor = ang::move(desc);
	if (aligment != invalid_index)
		m_descriptor.aligment(aligment);
	auto size = m_descriptor.get_size_in_bytes();
	realloc_buffer(size);
}

struct_buffer::~struct_buffer()
{
	dispose();
}

void struct_buffer::dispose()
{
	clear();
	m_aligned_data = null;
}

bool struct_buffer::is_readonly()const
{
	return true;
}

text::encoding_t struct_buffer::encoding()const 
{
	return text::encoding::binary;
}

pointer struct_buffer::buffer_ptr()
{
	return m_raw_data.data();
}

const_pointer struct_buffer::buffer_ptr()const
{
	return m_raw_data.data();
}

wsize struct_buffer::buffer_size()const
{
	return m_raw_data.size();
}

wsize struct_buffer::mem_copy(wsize sz, pointer ptr, text::encoding_t e)
{
	if (e != text::encoding::binary)
		return 0;
	memcpy(m_raw_data.data(), ptr, min(m_raw_data.size(), sz));
	return  min(m_raw_data.size(), sz);
}

ibuffer_view_t struct_buffer::map_buffer(windex, wsize)
{
	return null;
}

bool struct_buffer::unmap_buffer(ibuffer_view_t&, wsize)
{
	return false;
}

bool struct_buffer::can_realloc_buffer()const
{
	return false;
}

bool struct_buffer::realloc_buffer(wsize sz)
{
	m_aligned_data.capacity(sz / size_of<long long>(), true);
	m_raw_data = to_array((byte*)&m_aligned_data[0], (byte*)&m_aligned_data[0] + sz);
	return true;
}

void struct_buffer::clear()
{
	m_aligned_data.clear();
	m_raw_data = null;
	m_descriptor = varying_desc();
}

bool struct_buffer::load(dom::xml::ixml_node_t node)
{
	if (node.is_empty() || !node->has_children())
		return false;

	cstr_t data;
	m_name = node->attributes()["name"]->as<cstr_t>();
	for (dom::xml::ixml_node_t node : node->children())
	{
		auto name = node->name()->as<cstr_t>();
		if (name == "data_layout"_sv)
		{
			if (!m_descriptor.load(node))
				return false;
		}
		else if (name == "data"_sv)
		{
			data = node->value();
		}
	}

	//m_descriptor.aligment(16);//for default
	//m_descriptor.calculate_positions(true);

	realloc_buffer(m_descriptor.get_size_in_bytes());
	memset(m_raw_data.data(), 0, m_raw_data.size());

	text_data_loader_context_t parser;
	text_data_loader::create_context(m_descriptor, parser);

	text_data_loader_input_context_t input = {
		//text::iparser::get_parser(data.encoding()),
		data
	};
	text_data_loader_output_context_t output = {
		m_raw_data.data(),
		0
	};
	
	parser.load_data(parser, input, output);
	return true;
}

bool struct_buffer::copy(varying_t const& va)
{
	m_descriptor = va.descriptor();
	wsize sz = va.raw_data().size();
	realloc_buffer(sz);
	mem_copy(sz, va.raw_data().data());
	return true;
}

void struct_buffer::push_var(varying_t var)
{
	if (m_descriptor.var_type() != var_type::block)
		return;

	varying_desc_t desc = ang::move(var.descriptor());
	desc.aligment(m_descriptor.aligment());
	desc.calculate_positions(true);

	wsize my_sz = m_raw_data.size() / m_descriptor.array_count();
	m_descriptor.push_field(ang::move(desc));
	wsize new_sz = m_descriptor.get_size_in_bytes() / m_descriptor.array_count();
	wsize sz = new_sz - my_sz;
	array<byte> temp(my_sz);

	realloc_buffer((new_sz)* m_descriptor.array_count());

	//array_view<byte> data;
	wsize c = m_descriptor.array_count();
	for (wsize i = 1; i < c; i++) {
		memcpy(&temp[0], &m_raw_data[(c - i) * my_sz], my_sz); //copying data to temporal buffer
		memcpy(&m_raw_data[(c - i) * (new_sz)], &temp[0], my_sz);//reinserting data in the correct position
		memcpy(&m_raw_data[(c - i) * (new_sz) + my_sz], &var.raw_data()[0], sz);
	}

}

varying_t struct_buffer::push_var(varying_desc_t desc)
{
	if (m_descriptor.var_type() != var_type::block)
		return varying();

	desc.aligment(m_descriptor.aligment());
	desc.calculate_positions(true);
	wsize my_sz = m_raw_data.size() / m_descriptor.array_count();
	m_descriptor.push_field(ang::move(desc));
	wsize new_sz = m_descriptor.get_size_in_bytes() / m_descriptor.array_count();
	wsize sz = new_sz - my_sz;
	array<byte> temp(my_sz);

	realloc_buffer((new_sz) * m_descriptor.array_count());
	
	wsize c = m_descriptor.array_count();
	for (wsize i = 1; i < c; i++) {
		memcpy(&temp[0], &m_raw_data[(c - i) * my_sz], my_sz); //copying data to temporal buffer
		memcpy(&m_raw_data[(c - i) * (new_sz)], &temp[0], my_sz);//reinserting data in the correct position
	}
	return varying(to_array(&m_raw_data[my_sz], &m_raw_data[my_sz] + sz), desc, desc.aligment());
}

bool struct_buffer::push_var(varying_desc_t desc, vector<varying>* out)
{
	if (m_descriptor.var_type() != var_type::block)
		return false;

	desc.aligment(m_descriptor.aligment());
	desc.calculate_positions(true);
	wsize my_sz = m_raw_data.size() / m_descriptor.array_count();
	m_descriptor.push_field(ang::move(desc));
	wsize new_sz = m_descriptor.get_size_in_bytes() / m_descriptor.array_count();
	wsize sz = new_sz - my_sz;
	array<byte> temp(my_sz);

	realloc_buffer((new_sz)* m_descriptor.array_count());
	vector<varying_t> vars;

	array_view<byte> data;
	wsize c = m_descriptor.array_count();
	for (wsize i = 1; i < c; i++) {
		data.set(&m_raw_data[i * my_sz + sz * (i - 1)], &m_raw_data[i * my_sz + sz * (i - 1)] + sz);
		vars += varying(data, desc, desc.aligment());
		memcpy(&temp[0], &m_raw_data[(c - i) * my_sz], my_sz); //copying data to temporal buffer
		memcpy(&m_raw_data[(c - i) * (new_sz)], &temp[0], my_sz);//reinserting data in the correct position
	}

	data.set(&m_raw_data[c * my_sz + sz * (c - 1)], &m_raw_data[c * my_sz + sz * (c - 1)] + sz);
	vars += varying(data, desc, desc.aligment());
	if (out != null)
		*out = vars;

	return true;
}

varying_t struct_buffer::make_var(varying_desc_t const& desc, wsize ALIGMENT)
{
	clear();
	m_descriptor = desc;
	m_descriptor.aligment(ALIGMENT);
	m_descriptor.array_count(1);
	m_descriptor.calculate_positions(true);
	realloc_buffer(m_descriptor.get_size_in_bytes());
	return (varying const&)*this;
}

varying_t struct_buffer::make_struct(wsize ALIGMENT)
{
	clear();
	m_descriptor.aligment(ALIGMENT);
	m_descriptor.var_type(var_type::block);
	m_descriptor.var_class(var_class::scalar);
	m_descriptor.array_count(1);
	realloc_buffer(0);
	return (varying const&)*this;
}

varying_t struct_buffer::make_struct(array_view<varying_desc> const& desc, wsize ALIGMENT)
{
	clear();
	m_descriptor.aligment(ALIGMENT);
	m_descriptor.var_type(var_type::block);
	m_descriptor.var_class(var_class::scalar);
	m_descriptor.fields(desc);
	m_descriptor.array_count(1);
	m_descriptor.calculate_positions(true);
	realloc_buffer(m_descriptor.get_size_in_bytes());
	return (varying const&)*this;
}

vector<varying> struct_buffer::make_array(varying_desc_t const& desc, wsize count, wsize ALIGMENT)
{
	clear();
	m_descriptor = desc;
	m_descriptor.aligment(ALIGMENT);
	m_descriptor.array_count(count ? count : 1);
	m_descriptor.calculate_positions(true);
	realloc_buffer(m_descriptor.get_size_in_bytes());
	return array_cast<varying>();
}

vector<varying> struct_buffer::make_struct_array(array_view<varying_desc> const& desc, wsize count, wsize ALIGMENT)
{
	clear();
	m_descriptor.aligment(ALIGMENT);
	m_descriptor.var_type(var_type::block);
	m_descriptor.var_class(var_class::scalar);
	m_descriptor.array_count(count ? count : 1);
	m_descriptor.fields(desc);
	m_descriptor.calculate_positions(true);
	realloc_buffer(m_descriptor.get_size_in_bytes());
	return array_cast<varying>();
}

#define MY_TYPE struct_buffer
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

graphics::reflect::varying_t struct_buffer_t::operator [](windex idx) { return get() ? get()->field(idx) : graphics::reflect::varying(); }
graphics::reflect::varying_t struct_buffer_t::operator [](cstr_t name) { return get() ? get()->field(name) : graphics::reflect::varying(); }
graphics::reflect::varying::iterator_t struct_buffer_t::begin() { return get() ? get()->begin() : graphics::reflect::varying::iterator_t(); }
graphics::reflect::varying::iterator_t struct_buffer_t::end() { return get() ? get()->end() : graphics::reflect::varying::iterator_t(); }
graphics::reflect::varying::const_iterator_t struct_buffer_t::begin()const { return get() ? ((varying const*) get())->begin() : graphics::reflect::varying::const_iterator_t(); }
graphics::reflect::varying::const_iterator_t struct_buffer_t::end()const { return get() ? ((varying const*)get())->end() : graphics::reflect::varying::const_iterator_t(); }

/////////////////////////////////////////////////////////////////////////////////

varying ang::graphics::reflect::varying::varying_cast<varying>::cast(ang::graphics::reflect::varying* va) { return *va; }
varying ang::graphics::reflect::varying::varying_cast<varying>::force_cast(ang::graphics::reflect::varying* va) {	return *va;  } 
bool ang::graphics::reflect::varying::varying_cast<varying>::is_type_of(ang::graphics::reflect::varying* va) { return true; }

#define ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(_TYPE, _RETURNTYPE, _VAR_CLASS, _VAR_TYPE, _ALIGNMENT) \
_RETURNTYPE ang::graphics::reflect::varying::varying_cast<_TYPE>::cast(ang::graphics::reflect::varying* va) { \
	if (!is_type_of(va)) \
		throw(ang::exception(ang::error_code::invalid_access)); \
	return *(_TYPE*)va->m_raw_data.get(); \
 } \
_RETURNTYPE ang::graphics::reflect::varying::varying_cast<_TYPE>::force_cast(ang::graphics::reflect::varying* va) { \
	return *(_TYPE*)va->m_raw_data.get(); \
 } \
bool ang::graphics::reflect::varying::varying_cast<_TYPE>::is_type_of(ang::graphics::reflect::varying* va) { \
	return va->m_raw_data.get() != nullptr \
		&& va->m_descriptor.aligment() >= _ALIGNMENT  \
		&& (va->m_descriptor.var_class().get() == _VAR_CLASS && va->m_descriptor.var_type().get() == _VAR_TYPE); \
 } \
template<> varying_desc reflect::type_desc<_TYPE>(cstr_t name) { \
	return varying_desc(_VAR_TYPE, _VAR_CLASS, name, 1U, _ALIGNMENT); \
}


ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(float, float&, ang::graphics::reflect::var_class::scalar, ang::graphics::reflect::var_type::f32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::ul_float2, ang::maths::ul_float2&, ang::graphics::reflect::var_class::vec2, ang::graphics::reflect::var_type::f32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::ul_float3, ang::maths::ul_float3&, ang::graphics::reflect::var_class::vec3, ang::graphics::reflect::var_type::f32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::ul_float4, ang::maths::ul_float4&, ang::graphics::reflect::var_class::vec4, ang::graphics::reflect::var_type::f32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::ul_float4x4, ang::maths::ul_float4x4&, ang::graphics::reflect::var_class::mat4, ang::graphics::reflect::var_type::f32, 4U);

ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::float2, ang::maths::float2&, ang::graphics::reflect::var_class::vec2, ang::graphics::reflect::var_type::f32, 8U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::float3, ang::maths::float3&, ang::graphics::reflect::var_class::vec3, ang::graphics::reflect::var_type::f32, 16U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::float4, ang::maths::float4&, ang::graphics::reflect::var_class::vec4, ang::graphics::reflect::var_type::f32, 16U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::float4x4, ang::maths::float4x4&, ang::graphics::reflect::var_class::mat4, ang::graphics::reflect::var_type::f32, 16U);

ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(char, char&, ang::graphics::reflect::var_class::scalar, ang::graphics::reflect::var_type::s8, 1U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(byte, byte&, ang::graphics::reflect::var_class::scalar, ang::graphics::reflect::var_type::u8, 1U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(short, short&, ang::graphics::reflect::var_class::scalar, ang::graphics::reflect::var_type::s16, 2U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ushort, ushort&, ang::graphics::reflect::var_class::scalar, ang::graphics::reflect::var_type::u16, 2U);

ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(int, int&, ang::graphics::reflect::var_class::scalar, ang::graphics::reflect::var_type::s32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::int2, ang::maths::int2&, ang::graphics::reflect::var_class::vec2, ang::graphics::reflect::var_type::s32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::int3, ang::maths::int3&, ang::graphics::reflect::var_class::vec3, ang::graphics::reflect::var_type::s32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::int4, ang::maths::int4&, ang::graphics::reflect::var_class::vec4, ang::graphics::reflect::var_type::s32, 4U);

ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(uint, uint&, ang::graphics::reflect::var_class::scalar, ang::graphics::reflect::var_type::u32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::uint2, ang::maths::uint2&, ang::graphics::reflect::var_class::vec2, ang::graphics::reflect::var_type::u32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::uint3, ang::maths::uint3&, ang::graphics::reflect::var_class::vec3, ang::graphics::reflect::var_type::u32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::uint4, ang::maths::uint4&, ang::graphics::reflect::var_class::vec4, ang::graphics::reflect::var_type::u32, 4U);