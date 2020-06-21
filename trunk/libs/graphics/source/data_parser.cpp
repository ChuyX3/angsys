#include "pch.h"
#include <ang/graphics/graphics.h>
//#include "ang_inlines.h"
//#include "vector_specialization.inl"


using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::reflect;

template<typename T>
wsize text_data_value_loader_load_data(text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output)
{
	windex idx = output.idx + context.position;
	for (windex i = 0; i < context.array_count; i++) 
	{
		*((T*)(output.data + idx)) = text::parser::to_value<T>(input.data, input.idx);
		text::parser::seek(input.data, input.idx, " , "_r);
		idx += align_up(context.aligment, size_of<T>());
	}
	return idx;
}

wsize text_data_value_loader<var_type::s8, var_class::scalar>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output) 
{
	return text_data_value_loader_load_data<char>(context, input, output);
}

wsize text_data_value_loader<var_type::u8, var_class::scalar>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output)
{
	return text_data_value_loader_load_data<uchar>(context, input, output);
}

wsize text_data_value_loader<var_type::s16, var_class::scalar>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output)
{
	return text_data_value_loader_load_data<short>(context, input, output);
}

wsize text_data_value_loader<var_type::u16, var_class::scalar>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output)
{
	return text_data_value_loader_load_data<ushort>(context, input, output);
}

wsize text_data_value_loader<var_type::s32, var_class::scalar>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output)
{
	return text_data_value_loader_load_data<int>(context, input, output);
}

wsize text_data_value_loader<var_type::u32, var_class::scalar>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output)
{
	return text_data_value_loader_load_data<uint>(context, input, output);
}

wsize text_data_value_loader<var_type::f32, var_class::scalar>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output)
{
	return text_data_value_loader_load_data<float>(context, input, output);
}

wsize text_data_value_loader<var_type::f32, var_class::vec2>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output) 
{
	text_data_loader_context_t float_context = {
		context.position,
		4, //ALIGMENT
		2 //ARRAY_COUNT
	};

	for (windex i = 0; i < context.array_count; i++)
	{
		text::parser::seek(input.data, input.idx, " { "_r);
		text_data_value_loader<var_type::f32, var_class::scalar>::load_data(float_context, input, output);
		text::parser::seek(input.data, input.idx, " } , "_r);
		float_context.position = ang::align_up(align_of<maths::float2>(), float_context.position + size_of<maths::float2>());
	}
	return float_context.position;
}

wsize text_data_value_loader<var_type::f32, var_class::vec3>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output)
{
	text_data_loader_context_t float_context = {
		context.position,
		4, //ALIGMENT
		3 //ARRAY_COUNT
	};

	for (windex i = 0; i < context.array_count; i++) 
	{
		text::parser::seek(input.data, input.idx, " { "_r);
		text_data_value_loader<var_type::f32, var_class::scalar>::load_data(float_context, input, output);
		text::parser::seek(input.data, input.idx, " } , "_r);
		float_context.position = align_up(align_of<maths::float3>(), float_context.position + size_of<maths::float3>());
	}
	return float_context.position;
}

wsize text_data_value_loader<var_type::f32, var_class::vec4>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output)
{
	text_data_loader_context_t float_context = {
		context.position,
		4, //ALIGMENT
		4 //ARRAY_COUNT
	};

	for (windex i = 0; i < context.array_count; i++)
	{
		text::parser::seek(input.data, input.idx, " { "_r);
		text_data_value_loader<var_type::f32, var_class::scalar>::load_data(float_context, input, output);
		text::parser::seek(input.data, input.idx, " } , "_r);
		float_context.position = align_up(align_of<maths::float4>(), float_context.position + size_of<maths::float4>());
	}
	return float_context.position;
}

wsize text_data_value_loader<var_type::f32, var_class::mat4>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output)
{
	static text_data_loader_context_t float4_context = {
		context.position,
		align_of<maths::float4x4>(), //ALIGMENT
		4 //ARRAY_COUNT
	};

	for (windex i = 0; i < context.array_count; i++) 
	{
		text::parser::seek(input.data, input.idx, " { "_r);
		text_data_value_loader<var_type::f32, var_class::vec4>::load_data(float4_context, input, output);
		text::parser::seek(input.data, input.idx, " } , "_r);
		float4_context.position = align_up(align_of<maths::float4x4>(), float4_context.position + size_of<maths::float4x4>());
	}

	return float4_context.position;
}


wsize text_data_value_loader<var_type::block, var_class::scalar>::load_data(
	text_data_loader_context_t const& context,
	text_data_loader_input_context_t& input,
	text_data_loader_output_context_t& output) {

	wsize pos = 0;
	text_data_loader_output_context_t _output = {
		output.data,
		output.idx
	};

	for (windex i = 0; i < context.array_count; i++) {
		
		for (text_data_loader_context_t const& child : context.children)
			pos = child.load_data(child, input, _output);

		_output.idx += align_up(context.aligment, pos);
	}
	return _output.idx;
}

bool text_data_value_loader<var_type::block, var_class::scalar>::create_context(varying_desc_t const& desc, text_data_loader_context_t& context)
{
	switch (desc.var_type())
	{
	case var_type::s8:
		context.position = desc.position();
		context.aligment = align_of<char>();
		context.array_count = (uint)desc.var_class().get() * desc.array_count();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::s8, var_class::scalar>::load_data;
		return true;
	case var_type::u8:
		context.position = desc.position();
		context.aligment = align_of<uchar>();
		context.array_count = (uint)desc.var_class().get() * desc.array_count();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::u8, var_class::scalar>::load_data;
		return true;
	case var_type::s16:
		context.position = desc.position();
		context.aligment = align_of<short>();
		context.array_count = (uint)desc.var_class().get() * desc.array_count();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::s16, var_class::scalar>::load_data;
		return true;
	case var_type::u16:
		context.position = desc.position();
		context.aligment = align_of<ushort>();
		context.array_count = (uint)desc.var_class().get() * desc.array_count();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::u16, var_class::scalar>::load_data;
		return true;
	case var_type::s32:
		context.position = desc.position();
		context.aligment = align_of<int>();
		context.array_count = (uint)desc.var_class().get() * desc.array_count();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::s32, var_class::scalar>::load_data;
		return true;
	case var_type::u32:
		context.position = desc.position();
		context.aligment = align_of<uint>();
		context.array_count = (uint)desc.var_class().get() * desc.array_count();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::u32, var_class::scalar>::load_data;
		return true;
	case var_type::f32:
		switch (desc.var_class())
		{
		case var_class::scalar:
			context.position = desc.position();
			context.aligment = align_of<float>();
			context.array_count = desc.array_count();
			context.children = null;
			context.load_data = &text_data_value_loader<var_type::f32, var_class::scalar>::load_data;
			break;
		case var_class::vec2:
			context.position = desc.position();
			context.aligment = align_of<maths::float2>();
			context.array_count = desc.array_count();
			context.children = null;
			context.load_data = &text_data_value_loader<var_type::f32, var_class::vec2>::load_data;
			break;
		case var_class::vec3:
			context.position = desc.position();
			context.aligment = align_of<maths::float3>();
			context.array_count = desc.array_count();
			context.children = null;
			context.load_data = &text_data_value_loader<var_type::f32, var_class::vec3>::load_data;
			break;
		case var_class::vec4:
			context.position = desc.position();
			context.aligment = align_of<maths::float4>();
			context.array_count = desc.array_count();
			context.children = null;
			context.load_data = &text_data_value_loader<var_type::f32, var_class::vec4>::load_data;
			break;
		case var_class::mat4:
			context.position = desc.position();
			context.aligment = align_of<maths::float4x4>();
			context.array_count = desc.array_count();
			context.children = null;
			context.load_data = &text_data_value_loader<var_type::f32, var_class::mat4>::load_data;
			break;
		default:
			break;
		}
		return true;
	case var_type::block:
	{
		text_data_loader_context_t child;
		context.position = desc.position();
		context.aligment = desc.aligment();
		context.array_count = desc.array_count();
		context.load_data = &text_data_value_loader<var_type::block, var_class::scalar>::load_data;
		for (varying_desc_t const& child_desc : desc.fields())
		{
			create_context(child_desc, child);
			context.children += ang::move(child);
		}	
		return true;
	}
	default: 
		return false;
	}
}

bool text_data_value_loader<var_type::block, var_class::scalar>::create_context(attribute_desc_t const& desc, text_data_loader_context_t& context)
{
	switch (desc.var_type())
	{
	case var_type::s8:
		context.position = desc.position();
		context.aligment = align_of<char>();
		context.array_count = (uint)desc.var_class().get();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::s8, var_class::scalar>::load_data;
		return true;
	case var_type::u8:
		context.position = desc.position();
		context.aligment = align_of<uchar>();
		context.array_count = (uint)desc.var_class().get();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::u8, var_class::scalar>::load_data;
		return true;
	case var_type::s16:
		context.position = desc.position();
		context.aligment = align_of<short>();
		context.array_count = (uint)desc.var_class().get();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::s16, var_class::scalar>::load_data;
		return true;
	case var_type::u16:
		context.position = desc.position();
		context.aligment = align_of<ushort>();
		context.array_count = (uint)desc.var_class().get();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::u16, var_class::scalar>::load_data;
		return true;
	case var_type::s32:
		context.position = desc.position();
		context.aligment = align_of<int>();
		context.array_count = (uint)desc.var_class().get();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::s32, var_class::scalar>::load_data;
		return true;
	case var_type::u32:
		context.position = desc.position();
		context.aligment = align_of<uint>();
		context.array_count = (uint)desc.var_class().get();
		context.children = null;
		context.load_data = &text_data_value_loader<var_type::u32, var_class::scalar>::load_data;
		return true;
	case var_type::f32:
		switch (desc.var_class())
		{
		case var_class::scalar:
			context.position = desc.position();
			context.aligment = align_of<float>();
			context.array_count = 1;
			context.children = null;
			context.load_data = &text_data_value_loader<var_type::f32, var_class::scalar>::load_data;
			break;
		case var_class::vec2:
			context.position = desc.position();
			context.aligment = align_of<maths::float2>();
			context.array_count = 1;
			context.children = null;
			context.load_data = &text_data_value_loader<var_type::f32, var_class::vec2>::load_data;
			break;
		case var_class::vec3:
			context.position = desc.position();
			context.aligment = align_of<maths::float3>();
			context.array_count = 1;
			context.children = null;
			context.load_data = &text_data_value_loader<var_type::f32, var_class::vec3>::load_data;
			break;
		case var_class::vec4:
			context.position = desc.position();
			context.aligment = align_of<maths::float4>();
			context.array_count = 1;
			context.children = null;
			context.load_data = &text_data_value_loader<var_type::f32, var_class::vec4>::load_data;
			break;
		case var_class::mat4:
			context.position = desc.position();
			context.aligment = align_of<maths::float4x4>();
			context.array_count = 1;
			context.children = null;
			context.load_data = &text_data_value_loader<var_type::f32, var_class::mat4>::load_data;
			break;
		default:
			break;
		}
		return true;
	default:
		return false;
	}
}


bool text_data_value_loader<var_type::block, var_class::scalar>::create_context(array_view<attribute_desc_t> const& desc, text_data_loader_context_t& context)
{
	text_data_loader_context_t child;
	context.position = 0;
	context.aligment = 16;
	context.array_count = 1;
	context.load_data = &text_data_value_loader<var_type::block, var_class::scalar>::load_data;
	for (attribute_desc_t const& vars : desc)
	{
		create_context(vars, child);
		context.children += ang::move(child);
	}
	return true;
}


text_data_loader_context::text_data_loader_context(
	wsize position,
	wsize aligment,
	wsize array_count,
	vector<text_data_loader_context> children,
	load_data_func_t load_data
)
	: position(position)
	, aligment(aligment)
	, array_count(array_count)
	, children(children)
	, load_data(load_data)
{
}

text_data_loader_context::text_data_loader_context(text_data_loader_context&& other)
	: position(ang::move(other.position))
	, aligment(ang::move(other.aligment))
	, array_count(ang::move(other.array_count))
	, children(ang::move(other.children))
	, load_data(ang::move(other.load_data))
{

}

text_data_loader_context::text_data_loader_context(text_data_loader_context const& other)
	: position(other.position)
	, aligment(other.aligment)
	, array_count(other.array_count)
	, children(other.children)
	, load_data(other.load_data)
{

}

text_data_loader_context& text_data_loader_context::operator = (text_data_loader_context&& other) {
	if (&other != this)
	{
		position = ang::move(other.position);
		aligment = ang::move(other.aligment);
		array_count = ang::move(other.array_count);
		children = ang::move(other.children);
		load_data = ang::move(other.load_data);
	}
	return*this;
}

text_data_loader_context& text_data_loader_context::operator = (const text_data_loader_context& other) {

//	if (children.is_empty())
//		children = new collections::vector_buffer<text_data_loader_context>();

	position = other.position;
	aligment = other.aligment;
	array_count = other.array_count;
	children.copy(other.children);
	load_data = other.load_data;
	return*this;
}


