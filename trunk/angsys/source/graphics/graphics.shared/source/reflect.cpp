#include "pch.h"
#include <ang/graphics/graphics.h>
//#include "vector_specialization.inl"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::reflect;

safe_enum_rrti2(ang::graphics::reflect, var_type);
safe_enum_rrti2(ang::graphics::reflect, var_class);
safe_enum_rrti2(ang::graphics::reflect, var_semantic);


//#define MY_ALLOCATOR ang::memory::default_allocator
//#define MY_TYPE ang::graphics::reflect::varying_desc_t
//#include <ang/collections/inline/vector_object_specialization.inl>
//#undef MY_TYPE
//#define MY_TYPE ang::graphics::reflect::attribute_desc_t
//#include <ang/collections/inline/vector_object_specialization.inl>
//#undef MY_TYPE
//#undef MY_ALLOCATOR

static collections::pair<graphics::reflect::var_type, castr_t> s_to_string_var_type_map[] =
{
	{ var_type::none, "none" },
	{ var_type::s8, "s8," },
	{ var_type::u8, "u8" },
	{ var_type::s16, "s16" },
	{ var_type::u16, "u16" },
	{ var_type::s32, "s32" },
	{ var_type::u32, "u32" },
	{ var_type::f32, "f32" },
	{ var_type::tex1D, "tex1D" },
	{ var_type::tex2D, "tex2D" },
	{ var_type::tex3D, "tex3D" },
	{ var_type::texCube, "texCube" },
	{ var_type::buffer, "buffer" },
	{ var_type::block, "structure" }
};

static collections::pair<castr_t, graphics::reflect::var_type> s_parse_var_type_map[] =
{
	{ "block", var_type::block },
	{ "buffer", var_type::buffer },
	{ "f32", var_type::f32 },
	{ "none", var_type::none },
	{ "s16", var_type::s16 },
	{ "s32", var_type::s32 },
	{ "s8", var_type::s8 },
	{ "tex1D", var_type::tex1D },
	{ "tex2D", var_type::tex2D },
	{ "tex3D", var_type::tex3D },
	{ "texCube", var_type::texCube },
	{ "u16", var_type::u16 },
	{ "u32", var_type::u32 },
	{ "u8", var_type::u8 }
};

var_type_t graphics::reflect::var_type_t::parse(cstr_t cstr)
{
	wsize idx = algorithms::binary_search(cstr, collections::to_array(s_parse_var_type_map));
	if (idx > algorithms::array_size(s_parse_var_type_map))
		return var_type::none;
	else
		return s_parse_var_type_map[idx].value;
}

cstr_t graphics::reflect::var_type_t::to_string()const
{
	wsize idx = algorithms::binary_search(get(), collections::to_array(s_to_string_var_type_map));
	if (idx > algorithms::array_size(s_to_string_var_type_map))
		return "none"_s;	
	else
		return s_to_string_var_type_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static collections::pair<graphics::reflect::var_class, castr_t> s_to_string_var_class_map[] =
{
	{ var_class::none, "none" },
	{ var_class::scalar, "scalar" },
	{ var_class::vec2, "vec2" },
	{ var_class::vec3, "vec3" },
	{ var_class::vec4, "vec4" },
	{ var_class::mat3, "mat3" },
	{ var_class::mat4, "mat4" }
};

static collections::pair<cstr_t, graphics::reflect::var_class> s_parse_var_class_map[] =
{
	{ "mat2", var_class::mat2 },
	{ "mat3", var_class::mat3 },
	{ "mat4", var_class::mat4 },
	{ "none", var_class::none },
	{ "scalar", var_class::scalar },
	{ "vec2", var_class::vec2 },
	{ "vec3", var_class::vec3 },
	{ "vec4", var_class::vec4 }
};

var_class_t graphics::reflect::var_class_t::parse(cstr_t cstr)
{
	wsize idx = algorithms::binary_search(cstr, collections::to_array(s_parse_var_class_map));
	if (idx > algorithms::array_size(s_parse_var_class_map))
		return var_class::none;
	else
		return s_parse_var_class_map[idx].value;
}


cstr_t graphics::reflect::var_class_t::to_string()const
{
	wsize idx = algorithms::binary_search(get(), collections::to_array(s_to_string_var_class_map));
	if (idx > algorithms::array_size(s_to_string_var_class_map))
		return "none"_s;
	else
		return s_to_string_var_class_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


static collections::pair<graphics::reflect::var_semantic, cstr_t> s_to_string_var_semantic_map[] =
{
	{ var_semantic::none, "UNKNOWN" },
	{ var_semantic::position, "POSITION" },
	{ var_semantic::color, "COLOR" },
	{ var_semantic::tex_coord, "TEXCOORD" },
	{ var_semantic::normal, "NORMAL" },
	{ var_semantic::binormal, "BINORMAL" },
	{ var_semantic::tangent, "TANGENT" },
	{ var_semantic::fog, "FOG" }
};

static collections::pair<castr_t, graphics::reflect::var_semantic> s_parse_var_semantic_map[] =
{
	{ "BINORMAL" , var_semantic::binormal },
	{ "COLOR" , var_semantic::color },
	{ "FOG" , var_semantic::fog },
	{ "NORMAL" , var_semantic::normal },
	{ "POSITION" , var_semantic::position },
	{ "TANGENT" , var_semantic::tangent },
	{ "TEXCOORD" , var_semantic::tex_coord },
	{ "UNKNOWN" , var_semantic::none }
};

var_semantic_t graphics::reflect::var_semantic_t::parse(cstr_t cstr)
{
	wsize idx = algorithms::binary_search(cstr, collections::to_array(s_parse_var_semantic_map));
	if (idx > algorithms::array_size(s_parse_var_semantic_map))
		return var_semantic::none;
	else
		return s_parse_var_semantic_map[idx].value;
}

cstr_t graphics::reflect::var_semantic_t::to_string()const
{
	wsize idx = algorithms::binary_search(get(), collections::to_array(s_to_string_var_semantic_map));
	if (idx > algorithms::array_size(s_to_string_var_semantic_map))
		return "UNKNOWN"_s;
	else
		return s_to_string_var_semantic_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

varying_desc::varying_desc(var_type_t _type, var_class_t _class
	, astring name, uint _array, uint aligment)
{
	m_var_type = _type;
	m_var_class = _class;
	m_var_name = name.get();
	m_aligment = aligment;
	m_position = 0U;
	m_array_count = max(_array, 1U);
}

varying_desc::varying_desc(astring name, collections::vector<varying_desc> vars, uint _array, uint aligment)
{
	m_var_name = name.get();
	m_aligment = aligment;
	m_position = 0U;
	m_array_count = max(_array, 1U);
	m_fields = ang::move(vars);
	m_var_type = var_type::block;
	m_var_class = var_class::scalar;
	calculate_positions(true);
}

varying_desc::varying_desc(varying_desc&& value) {
	m_var_name = ang::move(value.m_var_name);
	m_aligment = ang::move(value.m_aligment);
	m_position = ang::move(value.m_position);
	m_array_count = ang::move(value.m_array_count);
	m_fields = ang::move(value.m_fields);
	m_var_type = ang::move(value.m_var_type);
	m_var_class = ang::move(value.m_var_class);
}

varying_desc::varying_desc(const varying_desc& value) {
	m_var_name = value.m_var_name;
	m_aligment = value.m_aligment;
	m_position = value.m_position;
	m_array_count = value.m_array_count;
	if(!value.m_fields.is_empty())
		m_fields = static_cast<collections::ienum<varying_desc> const*>(value.m_fields.get());
	m_var_type = value.m_var_type;
	m_var_class = value.m_var_class;
}

varying_desc::~varying_desc() {
	m_fields.reset();
}


bool varying_desc::load(dom::xml::xml_node_t input, uint aligment)
{
	if (input.is_empty())
		return false;
	auto atts = input->xml_attributes();
	if (input->xml_has_children())
	{
		for(dom::xml::xml_node_t att : atts)
		{
			//xml::xml_attribute_t att = node->xml_as<xml::xml_attribute>();
			auto name = att->xml_name()->xml_as<cstr_t>();
			if (name == "name"_s)
				m_var_name = (cstr_t)att->xml_value();
			else if (name == "array"_s)
				m_array_count = att->xml_value()->xml_as<uint>();
			else if (name == "aligment"_s)
				m_aligment = att->xml_value()->xml_as<uint>();
		}

		m_aligment = (aligment != invalid_index) ? aligment : max(m_aligment, 1u);
		m_array_count = max(m_array_count, 1u);
		m_position = 0;
		m_var_type = var_type::block;
		m_var_class = var_class::scalar;
		m_fields = null;

		wsize total = 0;
		wsize size = 0;
		wsize temp = 0;
		wsize res = 0;
		for(dom::xml::xml_node_t field : input->xml_children())
		{
			varying_desc desc;
			auto name = field->xml_name();
			if (((cstr_t)name == L"var"_s || (cstr_t)name == L"block"_s) && desc.load(field, m_aligment))
			{
				size = desc.get_size_in_bytes();
				temp = total % m_aligment;
				res = m_aligment - temp;
				if (res < m_aligment)
				{
					if (res > size)
						total += align_up(size, temp) - temp;
					else if (res < size)
						total += res;
				}
				desc.m_position = total;
				total += size;
				m_fields += ang::move(desc);
			}
		}
		return true;
	}
	else if (!atts.is_empty())
	{
		for(dom::xml::xml_node_t att : atts)
		{
			auto name = att->xml_name()->xml_as<cstr_t>();
			if (name == "name"_s)
				m_var_name = (cstr_t)att->xml_value();
			else if (name == "type"_s)
				m_var_type = att->xml_value()->xml_as<var_type_t>();
			else if (name == "class"_s)
				m_var_class = att->xml_value()->xml_as<var_class_t>();
			else if (name == "array"_s)
				m_array_count = att->xml_value()->xml_as<uint>();
			else if (name == "aligment"_s)
				m_aligment = att->xml_value()->xml_as<uint>();
		}

		m_position = 0;
		m_aligment = (aligment != invalid_index) ? aligment : max(m_aligment, 1u);
		m_array_count = max(m_array_count, 1u);
		m_fields = null;
		return true;
	}
	return false;
}

bool varying_desc::save(dom::xml::xml_document_t)const
{
	return false;
}

var_type_t varying_desc::var_type()const { return m_var_type; }

var_class_t varying_desc::var_class()const { return m_var_class; }

astring const& varying_desc::var_name()const { return m_var_name; }

uint varying_desc::array_count()const { return m_array_count; }

uint varying_desc::aligment()const { return m_aligment; }

uint varying_desc::position()const { return m_position; }

//uniform_fields_t& varying_desc::fields() { return m_fields; }

uniform_fields_t const& varying_desc::fields()const { return m_fields; }

void varying_desc::var_type(var_type_t value)
{
	if (m_var_type.get() == value.get())return;
	if (m_var_type.get() == var_type::block)
		m_var_class = var_class::scalar;
	m_var_type = value;
	m_fields = nullptr;
}

void varying_desc::var_class(var_class_t value)
{
	if (m_var_type.get() == var_type::block)
		return; //m_var_class must be var_class::scalar
	m_var_class = value;
}

void varying_desc::var_name(astring value)
{
	m_var_name = value;
}

void varying_desc::array_count(uint value)
{
	m_array_count = value;
}

void varying_desc::aligment(uint value)
{
	if (m_aligment == value) return;
	m_aligment = value;
	calculate_positions(true);
}

void varying_desc::position(uint value)
{
	m_position = value;
}

void varying_desc::fields(uniform_fields_t value, bool calc_pos)
{
	if (value.is_empty()) return;
	m_var_type = var_type::block;
	m_var_class = var_class::scalar;
	m_fields = ang::move(value);
	if(calc_pos)
		calculate_positions(true);
}

wsize varying_desc::calculate_positions(bool recursive)
{
	if (m_var_type != var_type::block)
		return get_size_in_bytes();

	if (recursive)
	{
		wsize total = 0;
		wsize size = 0;
		wsize temp = 0;
		wsize res = 0;
		for(varying_desc& desc : m_fields)
		{
			desc.m_aligment = m_aligment;
			size = desc.calculate_positions(true);
			if (size == 0) {
				desc.m_position = invalid_index;
				continue;//next item
			}
			temp = (total % m_aligment);
			res = m_aligment - temp;
			if (res < m_aligment)
			{
				if (res > size)
					total += align_up(size, temp) - temp;
				else if(res < size)
					total += res;
			}
			desc.m_position = total;
			total += size;
		}
		return (m_array_count == 1) ? total : align_up(m_aligment, total)*m_array_count;
	}
	else
	{
		wsize total = 0;
		wsize size = 0;
		wsize temp = 0;
		wsize res = 0;
		for(varying_desc& desc : m_fields)
		{
			size = desc.get_size_in_bytes();
			if (size == 0) {
				desc.m_position = invalid_index;
				continue;//next item
			}
			temp = (total % m_aligment);
			res = m_aligment - temp;
			if (res < m_aligment)
			{
				if (res > size)
					total += align_up(size, temp) - temp;
				else if (res < size)
					total += res;
			}
			desc.m_position = total;
			total += size;
		}
		return (m_array_count == 1) ? total : align_up(m_aligment, total)*m_array_count;
	}
}

wsize varying_desc::get_size_in_bytes()const
{
	if (m_array_count == 0)
		return 0;
	switch (m_var_type.get())
	{
	case var_type::s8:
	case var_type::u8:
		return (uint)m_var_class.get() * m_array_count;
	case var_type::s16:
	case var_type::u16:
		return 2U * (uint)m_var_class.get() * m_array_count;
	case var_type::s32:
	case var_type::u32:
	case var_type::f32:
		return 4U * (uint)m_var_class.get() * m_array_count;
	case var_type::buffer:
		return align_up(m_aligment, m_array_count);
	case var_type::block: {
		wsize total = 0;
		wsize size = 0;
		wsize temp = 0;
		wsize res = 0;
		if (m_fields.is_empty())
			return 0;
		varying_desc const& desc = *m_fields->last();
		total = desc.m_position; //get last position for optimization, warning it can be wrong
		size = desc.get_size_in_bytes();
		if (size != 0)
		{
			temp = (total % m_aligment);
			res = m_aligment - temp;
			if (res < m_aligment)
			{
				if (res > size)
					total += align_up(size, temp) - temp;
				else if (res < size)
					total += res;
			}
			total += size;
		}
		return (m_array_count == 1) ? total : align_up(m_aligment, total)*m_array_count;
	}
	default:return 0u;
	}
}

wsize varying_desc::get_size_in_bytes(uint aligment)const
{
	if (m_array_count == 0)
		return 0;
	switch (m_var_type.get())
	{
	case var_type::s8:
	case var_type::u8:
		return (uint)m_var_class.get()* m_array_count;
	case var_type::s16:
	case var_type::u16:
		return 2U * (uint)m_var_class.get()* m_array_count;
	case var_type::s32:
	case var_type::u32:
	case var_type::f32:
		return 4U * (uint)m_var_class.get()* m_array_count;
	case var_type::buffer:
		return align_up(aligment, m_array_count);
	case var_type::block: {
		wsize total = 0;
		wsize size = 0;
		wsize temp = 0;
		wsize res = 0;
		for(varying_desc const& desc : m_fields) {
			size = desc.get_size_in_bytes(aligment);
			if (size == 0)continue;//next item
			temp = (total % aligment);
			res = aligment - temp;
			if (res < aligment)
			{
				if (res > size)
					total += align_up(size, temp) - temp;
				else if(res < size)
					total += res;
			}
			total += size;	
		}
		return (m_array_count == 1) ? total : align_up(aligment, total) * m_array_count;
	}
	default:return 0u;
	}
}

varying_desc& varying_desc::operator = (varying_desc&& value) {
	m_var_name = ang::move(value.m_var_name);
	m_aligment = ang::move(value.m_aligment);
	m_position = ang::move(value.m_position);
	m_array_count = ang::move(value.m_array_count);
	m_array_count = max(m_array_count, 1u);
	m_fields = ang::move(value.m_fields);
	m_var_type = ang::move(value.m_var_type);
	m_var_class = ang::move(value.m_var_class);
	return*this;
}

varying_desc& varying_desc::operator = (const varying_desc& value) {
	m_var_name = value.m_var_name;
	m_aligment = value.m_aligment;
	m_position = value.m_position;
	m_array_count = value.m_array_count;
	m_fields = static_cast<collections::ienum<varying_desc> const*>(value.m_fields.get());
	m_var_type = value.m_var_type;
	m_var_class = value.m_var_class;
	return*this;
}

bool varying_desc::operator == (const varying_desc& value)const
{
	return m_var_name == value.m_var_name
		|| m_array_count == value.m_array_count
		|| m_fields == value.m_fields
		|| m_var_type == value.m_var_type
		|| m_var_class == value.m_var_class;
}

bool varying_desc::operator != (const varying_desc& value)const
{
	return m_var_name != value.m_var_name
		&& m_array_count != value.m_array_count
		&& m_fields != value.m_fields
		&& m_var_type != value.m_var_type
		&& m_var_class != value.m_var_class;
}


////////////////////////////////////////////////////////////////

wsize attribute_desc::calculate_positions(array_view<attribute_desc>& attributes)
{
	static const wsize aligment = 16U;
	wsize total = 0;
	wsize size = 0;
	wsize temp = 0;
	wsize res = 0;
	
	for(attribute_desc& desc : attributes)
	{
		size = desc.get_size_in_bytes();
		if (size == 0)continue;//next item
		temp = (total % aligment);
		res = aligment - temp;
		if (res < aligment)
		{
			if (res > size)
				total += align_up(size, temp) - temp;
			else if (res < size)
				total += res;
		}
		desc.position(total);
		total += size;
	}
	return align_up(aligment, total);
}

wsize attribute_desc::get_size_in_bytes(array_view<attribute_desc> attributes, wsize aligment, uint from, uint to)
{
	if (attributes.size() <= from || from >= to)
		return 0;

	wsize total = 0;
	wsize size = 0;
	wsize temp = 0;
	wsize res = 0;
	to = min(attributes.size(), to);
	for (index i = from; i < to; ++i)
	{
		attribute_desc const& desc = attributes[i];
		size = desc.get_size_in_bytes();
		if (size == 0)continue;//next item
		temp = (total % aligment);
		res = aligment - temp;
		if (res < aligment)
		{
			if (res > size)
				total += align_up(size, temp) - temp;
			else if (res < size)
				total += res;
		}		
		total += size;
	};
	return align_up(aligment, total);
}

bool attribute_desc::load(dom::xml::xml_node_t inputs, collections::vector<attribute_desc>& out)
{
	if (inputs.is_empty() || !inputs->xml_has_children())
		return false;
	if (out.is_empty()) out = new collections::vector_buffer<attribute_desc>();
	else out->clear();
	attribute_desc_t desc;
	for(dom::xml::xml_node_t input : inputs->xml_children())
	{
		if(desc.load(input))
			out += desc;
	}
	return true;
}

attribute_desc::attribute_desc(var_type_t _type, var_class_t _class,
	astring name, var_semantic_t sem, index idx, uint pos)
{
	m_var_type = _type;
	m_var_class = _class;
	m_var_name = name.get();
	m_semantic = sem;
	m_semantic_index = idx;
	m_position = pos;
}

attribute_desc::attribute_desc(const attribute_desc& value) {
	m_var_name = value.m_var_name;
	m_var_type = value.m_var_type;
	m_var_class = value.m_var_class;
	m_semantic = value.m_semantic;
	m_semantic_index = value.m_semantic_index;
	m_position = value.m_position;
}

bool attribute_desc::load(dom::xml::xml_node_t input)
{
	if (input.is_empty() || !input->xml_has_attributes())
		return false;
	auto att = input->xml_attributes();
	m_var_name = (cstr_t)att["name"_s];
	m_var_type = att["type"_s]->xml_as<var_type_t>();
	m_var_class = att["class"_s]->xml_as<var_class_t>();
	m_semantic = att["semantic"_s]->xml_as<var_semantic_t>();
	m_semantic_index = att["semantic_idx"_s]->xml_as<uint>();
	return true;
}

bool attribute_desc::save(dom::xml::xml_document_t)const
{
	return false;
}

var_type_t attribute_desc::var_type()const { return m_var_type.get(); }
var_class_t attribute_desc::var_class()const { return m_var_class.get(); }
astring const& attribute_desc::var_name()const { return m_var_name; }
var_semantic_t attribute_desc::semantic()const { return m_semantic.get(); }
index attribute_desc::semantic_index()const { return m_semantic_index; }
uint attribute_desc::position()const { return m_position; }

void attribute_desc::var_type(var_type_t value) { m_var_type = value; }
void attribute_desc::var_class(var_class_t value){ m_var_class = value; }
void attribute_desc::var_name(astring value){ m_var_name = value; }
void attribute_desc::semantic(var_semantic_t value) { m_semantic = value; }
void attribute_desc::semantic_index(index value) { m_semantic_index = value; }
void attribute_desc::position(uint value) { m_position = value; }

wsize attribute_desc::get_size_in_bytes()const
{
	switch (m_var_type.get())
	{
	case var_type::s16:
	case var_type::u16:
		return 2u * (uint)m_var_class.get();
	case var_type::s32:
	case var_type::u32:
	case var_type::f32:
		return 4u * (uint)m_var_class.get();
	default:return 0u;
	}
}

attribute_desc& attribute_desc::operator = (const attribute_desc& value) {
	m_var_name = value.m_var_name;
	m_var_type = value.m_var_type;
	m_var_class = value.m_var_class;
	m_semantic = value.m_semantic;
	m_semantic_index = value.m_semantic_index;
	return*this;
}

bool attribute_desc::operator == (const attribute_desc& value)const
{
	return m_var_name == value.m_var_name
		|| m_semantic == value.m_semantic
		|| m_semantic_index == value.m_semantic_index
		|| m_var_type == value.m_var_type
		|| m_var_class == value.m_var_class;
}

bool attribute_desc::operator != (const attribute_desc& value)const
{
	return m_var_name != value.m_var_name
		&& m_semantic != value.m_semantic
		&& m_semantic_index != value.m_semantic_index
		&& m_var_type != value.m_var_type
		&& m_var_class != value.m_var_class;
}

