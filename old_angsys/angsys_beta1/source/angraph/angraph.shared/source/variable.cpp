#include "pch.h"
#include <ang/graphics/angraph.h>
#include "ang_inlines.h"
#include "vector_specialization.inl"

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::reflect;

variable::variable()
	: _raw_data()
	, _descriptor()
{
}

variable::variable(variable && other)
	: _raw_data(ang::move(other._raw_data))
	, _descriptor(ang::move(other._descriptor))
{
}

variable::variable(variable const& other)
	: _raw_data(other._raw_data)
	, _descriptor(other._descriptor)
{
}

variable::variable(static_array<byte> bytes, variable_desc desc, uint aligment)
	: _raw_data(ang::move(bytes))
	, _descriptor(ang::move(desc))
{
	if(aligment != invalid_index)
		_descriptor.aligment(aligment);
	auto size = _descriptor.get_size_in_bytes();
	if (bytes.size() < size)
		throw(exception_t(except_code::invalid_param));

}

//variable& variable::operator = (variable && other)
//{
//
//}
//
//variable& variable::operator = (variable const& other)
//{
//
//}

uint variable::aligment()const { return _descriptor.aligment(); }

static_array<byte> variable::raw_data()const { return _raw_data; }

variable_desc const& variable::descriptor()const { return _descriptor; }

variable variable::field(index idx) 
{
	if (_descriptor.var_type() != var_type::block)
		variable();
	uniform_fields_t const& fields = _descriptor.fields();
	if(fields.is_empty() || fields->size() <= idx)
		variable();

	variable var;
	var._descriptor = fields[idx];
	var._raw_data.set(&_raw_data[var._descriptor.position()], var._descriptor.get_size_in_bytes());
	return ang::move(var);
}

variable variable::field(cstr_t name)
{
	auto idx = find_field(name);
	if (invalid_index == idx)
		variable();

	variable var;
	var._descriptor = _descriptor.fields()[idx];
	var._raw_data.set(&_raw_data[var._descriptor.position()], var._descriptor.get_size_in_bytes());
	return ang::move(var);
}

variable variable::field(cwstr_t name)
{
	auto idx = find_field(name);
	if (invalid_index == idx)
		variable();
	variable var;
	var._descriptor = _descriptor.fields()[idx];
	var._raw_data.set(&_raw_data[var._descriptor.position()], var._descriptor.get_size_in_bytes());
	return ang::move(var);
}

index variable::find_field(cstr_t name)const
{
	if (_descriptor.var_type() != var_type::block)
		return invalid_index;
	uniform_fields_t const& fields = _descriptor.fields();
	if (fields.is_empty())
		return invalid_index;
	index i = 0;
	for (auto it = fields->begin(); it.is_valid(); ++it, ++i) {
		cstr_t _name = it->var_name();
		if (_name == name)
			return i;
	}
	
	return invalid_index;
}

index variable::find_field(cwstr_t name)const
{
	if (_descriptor.var_type() != var_type::block)
		return -1;
	uniform_fields_t const& fields = _descriptor.fields();
	if (fields.is_empty())
		return -1;
	index i = 0;
	for (auto it = fields->begin(); it.is_valid(); ++it, ++i)
		if (it->var_name() == name)
			return i;
	return -1;
}

variable variable::operator [](index idx)
{
	auto count = _descriptor.array_count();
	if (count > 1 && idx < count)
	{
		auto size = _descriptor.get_size_in_bytes() / count;
		variable_desc desc = _descriptor;
		desc.array_count(1);
		return variable(static_array<byte>(&_raw_data.get()[idx * size], size), desc);
	}
	else
		return field(idx);
}

variable variable::operator [](cstr_t name)
{
	return field(name);
}

variable variable::operator [](cwstr_t name)
{
	return field(name);
}


collections::vector<variable> variable::fragment()
{
	if (_descriptor.array_count() > 1)
		return ang::move(array_cast<variable>());
	if (_descriptor.var_type() != var_type::block)
		return collections::vector<variable>{*this};
	collections::vector<variable> vars;
	for (auto it = _descriptor.fields()->begin(); it.is_valid(); ++it)
		vars += {static_array<byte>(&_raw_data.get()[it->position()], it->get_size_in_bytes()), *it};
	return ang::move(vars);
}

collections::vector<variable> variable::variable_cast<static_array<variable>>::force_cast(variable* var)
{
	auto count = var->descriptor().array_count();
	auto size = var->descriptor().get_size_in_bytes() / count;
	variable_desc desc = var->descriptor();
	desc.array_count(1);

	collections::vector<variable> vars;
	for (index i = 0U; i < count; ++i)
		vars += variable( static_array<byte>(&var->_raw_data.get()[i * size], size), desc);
	return ang::move(vars);
}

collections::vector<variable> variable::variable_cast<static_array<variable>>::cast(variable* var)
{
	auto count = var->descriptor().array_count();
	auto size = var->descriptor().get_size_in_bytes() / count;
	variable_desc desc = var->descriptor();
	desc.array_count(1);

	collections::vector<variable> out;
	for (index i = 0U; i < count; ++i)
		out += variable(static_array<byte>(&var->_raw_data.get()[i * size], size), desc);
	return out;
}

variable& variable::operator = (variable && other) {
	_raw_data = ang::move(other._raw_data);
	_descriptor = ang::move(other._descriptor);
	return*this;
}

variable& variable::operator = (variable const& other) {
	_raw_data = other._raw_data;
	_descriptor =other._descriptor;
	return*this;
}

bool variable::operator == (const variable& other)const
{
	return _descriptor == other._descriptor;
}

bool variable::operator != (const variable& other)const
{
	return _descriptor != other._descriptor;
}

variable ang::graphics::reflect::variable::variable_cast<variable>::cast(ang::graphics::reflect::variable* var) { return *var; }
variable ang::graphics::reflect::variable::variable_cast<variable>::force_cast(ang::graphics::reflect::variable* var) {	return *var;  } 
bool ang::graphics::reflect::variable::variable_cast<variable>::is_type_of(ang::graphics::reflect::variable* var) { return true; }

#define ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(_TYPE, _RETURNTYPE, _VAR_CLASS, _VAR_TYPE, _ALIGNMENT) \
_RETURNTYPE ang::graphics::reflect::variable::variable_cast<_TYPE>::cast(ang::graphics::reflect::variable* var) { \
	if (!is_type_of(var)) \
		throw(ang::exception_t(ang::except_code::invalid_access)); \
	return *(_TYPE*)var->_raw_data.get(); \
 } \
_RETURNTYPE ang::graphics::reflect::variable::variable_cast<_TYPE>::force_cast(ang::graphics::reflect::variable* var) { \
	return *(_TYPE*)var->_raw_data.get(); \
 } \
bool ang::graphics::reflect::variable::variable_cast<_TYPE>::is_type_of(ang::graphics::reflect::variable* var) { \
	return var->_raw_data.get() != nullptr \
		&& var->_descriptor.aligment() >= _ALIGNMENT  \
		&& (var->_descriptor.var_class().get() == _VAR_CLASS && var->_descriptor.var_type().get() == _VAR_TYPE); \
 } \
template<> variable_desc reflect::type_desc<_TYPE>(cstr_t name) { \
	return variable_desc(_VAR_TYPE, _VAR_CLASS, name, 1U, _ALIGNMENT); \
}


ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(float, float&, ang::graphics::reflect::var_class::scalar, ang::graphics::reflect::var_type::f32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::unaligned_float2, ang::maths::unaligned_float2&, ang::graphics::reflect::var_class::vec2, ang::graphics::reflect::var_type::f32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::unaligned_float3, ang::maths::unaligned_float3&, ang::graphics::reflect::var_class::vec3, ang::graphics::reflect::var_type::f32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::unaligned_float4, ang::maths::unaligned_float4&, ang::graphics::reflect::var_class::vec4, ang::graphics::reflect::var_type::f32, 4U);
ANG_GRAPHICS_REFLECT_IMPLEMENT_TEMPLATE_VARIABLE_CAST(ang::maths::unaligned_float4x4, ang::maths::unaligned_float4x4&, ang::graphics::reflect::var_class::mat4, ang::graphics::reflect::var_type::f32, 4U);

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