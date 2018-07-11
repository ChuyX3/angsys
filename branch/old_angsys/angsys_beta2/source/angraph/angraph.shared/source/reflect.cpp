#include "pch.h"
#include <ang/graphics/angraph.hpp>
#include "ang_inlines.h"
#include "vector_specialization.inl"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::reflect;

ANG_IMPLEMENT_ENUM(ang::graphics::reflect, var_type, uint, var_type::none);
ANG_IMPLEMENT_ENUM(ang::graphics::reflect, var_class, uint, var_class::none);
ANG_IMPLEMENT_ENUM(ang::graphics::reflect, var_semantic, uint, var_semantic::none);


#define MY_ALLOCATOR ang::memory::default_allocator
#define MY_TYPE ang::graphics::reflect::variable_desc_t
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE
#define MY_TYPE ang::graphics::reflect::attribute_desc_t
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE
#undef MY_ALLOCATOR

static ang_pair<graphics::reflect::var_type, cstr_t> to_string_var_type_map[] =
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

static ang_pair<cstr_t, graphics::reflect::var_type> _parse_var_type_map[] =
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
	wsize idx = ang_binary_search<cstr_t>(cstr, _parse_var_type_map);
	if (idx > array_size(_parse_var_type_map))
		return var_type::none;
	else
		return _parse_var_type_map[idx].value;
}

var_type_t graphics::reflect::var_type_t::parse(cwstr_t cstr)
{
	wsize idx = ang_binary_search<cwstr_t>(cstr, _parse_var_type_map);
	if (idx > array_size(_parse_var_type_map))
		return var_type::none;
	else
		return _parse_var_type_map[idx].value;
}

cstr_t graphics::reflect::var_type_t::to_string()const
{
	wsize idx = ang_binary_search(_value, to_string_var_type_map);
	if (idx > array_size(to_string_var_type_map))
		return "none"_s;	
	else
		return to_string_var_type_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static ang_pair<graphics::reflect::var_class, cstr_t> to_string_var_class_map[] =
{
	{ var_class::none, "none" },
	{ var_class::scalar, "scalar" },
	{ var_class::vec2, "vec2" },
	{ var_class::vec3, "vec3" },
	{ var_class::vec4, "vec4" },
	{ var_class::mat3, "mat3" },
	{ var_class::mat4, "mat4" }
};

static ang_pair<cstr_t, graphics::reflect::var_class> _parse_var_class_map[] =
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
	wsize idx = ang_binary_search<cstr_t>(cstr, _parse_var_class_map);
	if (idx > array_size(_parse_var_class_map))
		return var_class::none;
	else
		return _parse_var_class_map[idx].value;
}

var_class_t graphics::reflect::var_class_t::parse(cwstr_t cstr)
{
	wsize idx = ang_binary_search<cwstr_t>(cstr, _parse_var_class_map);
	if (idx > array_size(_parse_var_class_map))
		return var_class::none;
	else
		return _parse_var_class_map[idx].value;
}

cstr_t graphics::reflect::var_class_t::to_string()const
{
	wsize idx = ang_binary_search(_value, to_string_var_class_map);
	if (idx > array_size(to_string_var_class_map))
		return "none"_s;
	else
		return to_string_var_class_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


static ang_pair<graphics::reflect::var_semantic, cstr_t> to_string_var_semantic_map[] =
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

static ang_pair<cstr_t, graphics::reflect::var_semantic> _parse_var_semantic_map[] =
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
	wsize idx = ang_binary_search<cstr_t>(cstr, _parse_var_semantic_map);
	if (idx > array_size(_parse_var_semantic_map))
		return var_semantic::none;
	else
		return _parse_var_semantic_map[idx].value;
}

var_semantic_t graphics::reflect::var_semantic_t::parse(cwstr_t cstr)
{
	wsize idx = ang_binary_search<cwstr_t>(cstr, _parse_var_semantic_map);
	if (idx > array_size(_parse_var_semantic_map))
		return var_semantic::none;
	else
		return _parse_var_semantic_map[idx].value;
}

cstr_t graphics::reflect::var_semantic_t::to_string()const
{
	wsize idx = ang_binary_search(_value, to_string_var_semantic_map);
	if (idx > array_size(to_string_var_semantic_map))
		return "UNKNOWN"_s;
	else
		return to_string_var_semantic_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

variable_desc::variable_desc(var_type_t _type, var_class_t _class
	, string name, uint _array, uint aligment)
{
	_var_type = _type;
	_var_class = _class;
	_var_name = name.get();
	_aligment = aligment;
	_position = 0U;
	_array_count = max(_array, 1U);
}

variable_desc::variable_desc(string name, collections::vector<variable_desc> vars, uint _array, uint aligment)
{
	_var_name = name.get();
	_aligment = aligment;
	_position = 0U;
	_array_count = max(_array, 1U);
	_fields = ang::move(vars);
	_var_type = var_type::block;
	_var_class = var_class::scalar;
	calculate_positions(true);
}

variable_desc::variable_desc(variable_desc&& value) {
	_var_name = ang::move(value._var_name);
	_aligment = ang::move(value._aligment);
	_position = ang::move(value._position);
	_array_count = ang::move(value._array_count);
	_fields = ang::move(value._fields);
	_var_type = ang::move(value._var_type);
	_var_class = ang::move(value._var_class);
}

variable_desc::variable_desc(const variable_desc& value) {
	_var_name = value._var_name;
	_aligment = value._aligment;
	_position = value._position;
	_array_count = value._array_count;
	if(!value._fields.is_empty())
		_fields = static_cast<collections::ienum<variable_desc> const*>(value._fields.get());
	_var_type = value._var_type;
	_var_class = value._var_class;
}

variable_desc::~variable_desc() {
	_fields.clean();
}


bool variable_desc::load(xml::ixml_node_t input, uint aligment)
{
	if (input.is_empty())
		return false;
	auto atts = input->xml_attributes();
	if (input->xml_has_children())
	{
		for(xml::ixml_node_t att : atts)
		{
			//xml::xml_attribute_t att = node->xml_as<xml::xml_attribute>();
			auto name = att->xml_name()->xml_as<cwstr_t>();
			if (name == "name"_s)
				_var_name = (cwstr_t)att->xml_value();
			else if (name == "array"_s)
				_array_count = att->xml_value()->xml_as<uint>();
			else if (name == "aligment"_s)
				_aligment = att->xml_value()->xml_as<uint>();
		}

		_aligment = (aligment != invalid_index) ? aligment : max(_aligment, 1u);
		_array_count = max(_array_count, 1u);
		_position = 0;
		_var_type = var_type::block;
		_var_class = var_class::scalar;
		_fields = null;

		wsize total = 0;
		wsize size = 0;
		wsize temp = 0;
		wsize res = 0;
		for(xml::ixml_node_t field : input->xml_children())
		{
			variable_desc desc;
			auto name = field->xml_name();
			if (((cwstr_t)name == L"var"_s || (cwstr_t)name == L"block"_s) && desc.load(field, _aligment))
			{
				size = desc.get_size_in_bytes();
				temp = total % _aligment;
				res = _aligment - temp;
				if (res < _aligment)
				{
					if (res > size)
						total += get_memory_size_aligned(temp, size) - temp;
					else if (res < size)
						total += res;
				}
				desc._position = total;
				total += size;
				_fields += ang::move(desc);
			}
		}
		return true;
	}
	else if (!atts.is_empty())
	{
		for(xml::ixml_node_t att : atts)
		{
			auto name = att->xml_name()->xml_as<cwstr_t>();
			if (name == "name"_s)
				_var_name = (cwstr_t)att->xml_value();
			else if (name == "type"_s)
				_var_type = att->xml_value()->xml_as<var_type_t>();
			else if (name == "class"_s)
				_var_class = att->xml_value()->xml_as<var_class_t>();
			else if (name == "array"_s)
				_array_count = att->xml_value()->xml_as<uint>();
			else if (name == "aligment"_s)
				_aligment = att->xml_value()->xml_as<uint>();
		}

		_position = 0;
		_aligment = (aligment != invalid_index) ? aligment : max(_aligment, 1u);
		_array_count = max(_array_count, 1u);
		_fields = null;
		return true;
	}
	return false;
}

bool variable_desc::save(xml::xml_document_t)const
{
	return false;
}

var_type_t variable_desc::var_type()const { return _var_type; }

var_class_t variable_desc::var_class()const { return _var_class; }

string const& variable_desc::var_name()const { return _var_name; }

uint variable_desc::array_count()const { return _array_count; }

uint variable_desc::aligment()const { return _aligment; }

uint variable_desc::position()const { return _position; }

//uniform_fields_t& variable_desc::fields() { return _fields; }

uniform_fields_t const& variable_desc::fields()const { return _fields; }

void variable_desc::var_type(var_type_t value)
{
	if (_var_type.get() == value.get())return;
	if (_var_type.get() == var_type::block)
		_var_class = var_class::scalar;
	_var_type = value;
	_fields = nullptr;
}

void variable_desc::var_class(var_class_t value)
{
	if (_var_type.get() == var_type::block)
		return; //_var_class must be var_class::scalar
	_var_class = value;
}

void variable_desc::var_name(string value)
{
	_var_name = value;
}

void variable_desc::array_count(uint value)
{
	_array_count = value;
}

void variable_desc::aligment(uint value)
{
	if (_aligment == value) return;
	_aligment = value;
	calculate_positions(true);
}

void variable_desc::position(uint value)
{
	_position = value;
}

void variable_desc::fields(uniform_fields_t value)
{
	if (value.is_empty()) return;
	_var_type = var_type::block;
	_var_class = var_class::scalar;
	_fields = ang::move(value);
	calculate_positions(true);
}

wsize variable_desc::calculate_positions(bool recursive)
{
	if (_var_type != var_type::block)
		return get_size_in_bytes();

	if (recursive)
	{
		wsize total = 0;
		wsize size = 0;
		wsize temp = 0;
		wsize res = 0;
		for(variable_desc& desc : _fields)
		{
			desc._aligment = _aligment;
			size = desc.calculate_positions(true);
			if (size == 0) {
				desc._position = invalid_index;
				continue;//next item
			}
			temp = (total % _aligment);
			res = _aligment - temp;
			if (res < _aligment)
			{
				if (res > size)
					total += get_memory_size_aligned(temp, size) - temp;
				else if(res < size)
					total += res;
			}
			desc._position = total;
			total += size;
		}
		return (_array_count == 1) ? total : get_memory_size_aligned(total, _aligment)*_array_count;
	}
	else
	{
		wsize total = 0;
		wsize size = 0;
		wsize temp = 0;
		wsize res = 0;
		for(variable_desc& desc : _fields)
		{
			size = desc.get_size_in_bytes();
			if (size == 0) {
				desc._position = invalid_index;
				continue;//next item
			}
			temp = (total % _aligment);
			res = _aligment - temp;
			if (res < _aligment)
			{
				if (res > size)
					total += get_memory_size_aligned(temp, size) - temp;
				else if (res < size)
					total += res;
			}
			desc._position = total;
			total += size;
		}
		return (_array_count == 1) ? total : get_memory_size_aligned(total, _aligment)*_array_count;
	}
}

wsize variable_desc::get_size_in_bytes()const
{
	if (_array_count == 0)
		return 0;
	switch (_var_type.get())
	{
	case var_type::s8:
	case var_type::u8:
		return (uint)_var_class.get()* _array_count;
	case var_type::s16:
	case var_type::u16:
		return 2U * (uint)_var_class.get()* _array_count;
	case var_type::s32:
	case var_type::u32:
	case var_type::f32:
		return 4U * (uint)_var_class.get()* _array_count;
	case var_type::buffer:
		return get_memory_size_aligned(_array_count, _aligment);
	case var_type::block: {
		wsize total = 0;
		wsize size = 0;
		wsize temp = 0;
		wsize res = 0;
		if (_fields.is_empty())
			return 0;
		variable_desc const& desc = *_fields->last();
		total = desc._position; //get last position for optimization, warning it can be wrong
		size = desc.get_size_in_bytes();
		if (size != 0)
		{
			temp = (total % _aligment);
			res = _aligment - temp;
			if (res < _aligment)
			{
				if (res > size)
					total += get_memory_size_aligned(temp, size) - temp;
				else if (res < size)
					total += res;
			}
			total += size;
		}
		return (_array_count == 1) ? total : get_memory_size_aligned(total, _aligment)*_array_count;
	}
	default:return 0u;
	}
}

wsize variable_desc::get_size_in_bytes(uint aligment)const
{
	if (_array_count == 0)
		return 0;
	switch (_var_type.get())
	{
	case var_type::s8:
	case var_type::u8:
		return (uint)_var_class.get()* _array_count;
	case var_type::s16:
	case var_type::u16:
		return 2U * (uint)_var_class.get()* _array_count;
	case var_type::s32:
	case var_type::u32:
	case var_type::f32:
		return 4U * (uint)_var_class.get()* _array_count;
	case var_type::buffer:
		return get_memory_size_aligned(_array_count, aligment);
	case var_type::block: {
		wsize total = 0;
		wsize size = 0;
		wsize temp = 0;
		wsize res = 0;
		for(variable_desc const& desc : _fields) {
			size = desc.get_size_in_bytes(aligment);
			if (size == 0)continue;//next item
			temp = (total % aligment);
			res = aligment - temp;
			if (res < aligment)
			{
				if (res > size)
					total += get_memory_size_aligned(temp, size) - temp;
				else if(res < size)
					total += res;
			}
			total += size;	
		}
		return (_array_count == 1) ? total : get_memory_size_aligned(total, aligment)*_array_count;
	}
	default:return 0u;
	}
}

variable_desc& variable_desc::operator = (variable_desc&& value) {
	_var_name = ang::move(value._var_name);
	_aligment = ang::move(value._aligment);
	_position = ang::move(value._position);
	_array_count = ang::move(value._array_count);
	_array_count = max(_array_count, 1u);
	_fields = ang::move(value._fields);
	_var_type = ang::move(value._var_type);
	_var_class = ang::move(value._var_class);
	return*this;
}

variable_desc& variable_desc::operator = (const variable_desc& value) {
	_var_name = value._var_name;
	_aligment = value._aligment;
	_position = value._position;
	_array_count = value._array_count;
	_fields = static_cast<collections::ienum<variable_desc> const*>(value._fields.get());
	_var_type = value._var_type;
	_var_class = value._var_class;
	return*this;
}

bool variable_desc::operator == (const variable_desc& value)const
{
	return _var_name == value._var_name
		|| _array_count == value._array_count
		|| _fields == value._fields
		|| _var_type == value._var_type
		|| _var_class == value._var_class;
}

bool variable_desc::operator != (const variable_desc& value)const
{
	return _var_name != value._var_name
		&& _array_count != value._array_count
		&& _fields != value._fields
		&& _var_type != value._var_type
		&& _var_class != value._var_class;
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
				total += get_memory_size_aligned(temp, size) - temp;
			else if (res < size)
				total += res;
		}
		desc.position(total);
		total += size;
	}
	return get_memory_size_aligned(total, aligment);
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
				total += get_memory_size_aligned(temp, size) - temp;
			else if (res < size)
				total += res;
		}		
		total += size;
	};
	return get_memory_size_aligned(total, aligment);
}

bool attribute_desc::load(xml::ixml_node_t inputs, collections::vector<attribute_desc>& out)
{
	if (inputs.is_empty() || !inputs->xml_has_children())
		return false;
	if (out.is_empty()) out = new collections::vector_buffer<attribute_desc>();
	else out->clean();
	attribute_desc_t desc;
	for(xml::ixml_node_t input : inputs->xml_children())
	{
		if(desc.load(input))
			out += desc;
	}
	return true;
}

attribute_desc::attribute_desc(var_type_t _type, var_class_t _class,
	string name, var_semantic_t sem, index idx, uint pos)
{
	_var_type = _type;
	_var_class = _class;
	_var_name = name.get();
	_semantic = sem;
	_semantic_index = idx;
	_position = pos;
}

attribute_desc::attribute_desc(const attribute_desc& value) {
	_var_name = value._var_name;
	_var_type = value._var_type;
	_var_class = value._var_class;
	_semantic = value._semantic;
	_semantic_index = value._semantic_index;
}

bool attribute_desc::load(xml::ixml_node_t input)
{
	if (input.is_empty() || !input->xml_has_attributes())
		return false;
	auto att = input->xml_attributes();
	_var_name = (cwstr_t)att["name"_s];
	_var_type = att["type"_s]->xml_as<var_type_t>();
	_var_class = att["class"_s]->xml_as<var_class_t>();
	_semantic = att["semantic"_s]->xml_as<var_semantic_t>();
	_semantic_index = att["semantic_idx"_s]->xml_as<uint>();
	return true;
}

bool attribute_desc::save(xml::xml_document_t)const
{
	return false;
}

var_type_t attribute_desc::var_type()const { return _var_type.get(); }
var_class_t attribute_desc::var_class()const { return _var_class.get(); }
string const& attribute_desc::var_name()const { return _var_name; }
var_semantic_t attribute_desc::semantic()const { return _semantic.get(); }
index attribute_desc::semantic_index()const { return _semantic_index; }
uint attribute_desc::position()const { return _position; }

void attribute_desc::var_type(var_type_t value) { _var_type = value; }
void attribute_desc::var_class(var_class_t value){ _var_class = value; }
void attribute_desc::var_name(string value){ _var_name = value; }
void attribute_desc::semantic(var_semantic_t value) { _semantic = value; }
void attribute_desc::semantic_index(index value) { _semantic_index = value; }
void attribute_desc::position(uint value) { _position = value; }

wsize attribute_desc::get_size_in_bytes()const
{
	switch (_var_type.get())
	{
	case var_type::s16:
	case var_type::u16:
		return 2u * (uint)_var_class.get();
	case var_type::s32:
	case var_type::u32:
	case var_type::f32:
		return 4u * (uint)_var_class.get();
	default:return 0u;
	}
}

attribute_desc& attribute_desc::operator = (const attribute_desc& value) {
	_var_name = value._var_name;
	_var_type = value._var_type;
	_var_class = value._var_class;
	_semantic = value._semantic;
	_semantic_index = value._semantic_index;
	return*this;
}

bool attribute_desc::operator == (const attribute_desc& value)const
{
	return _var_name == value._var_name
		|| _semantic == value._semantic
		|| _semantic_index == value._semantic_index
		|| _var_type == value._var_type
		|| _var_class == value._var_class;
}

bool attribute_desc::operator != (const attribute_desc& value)const
{
	return _var_name != value._var_name
		&& _semantic != value._semantic
		&& _semantic_index != value._semantic_index
		&& _var_type != value._var_type
		&& _var_class != value._var_class;
}

