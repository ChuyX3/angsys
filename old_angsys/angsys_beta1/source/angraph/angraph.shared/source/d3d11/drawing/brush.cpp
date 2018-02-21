#include "pch.h"
#include "d3d11/drawing/draw_context.h"


#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

static xml::xml_node_t create_tecnique_template(cwstr_t name, cwstr_t vertex_shader, cwstr_t pixel_shader)
{
	xml::xml_builder_t builder = new xml::xml_builder();
	builder->begin_element("technique"_s);
	{
		builder->attribute("name"_s, name);
		builder->begin_element("vertex_shader"_s);
		{
			builder->begin_element("uniforms"_s);
			{
				builder->attribute("name"_s, "world_data"_s);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "mat4"_s);
					builder->attribute("name"_s, "world"_s);
				}builder->end_element(/*var*/);
			}builder->end_element(/*uniforms*/);
			builder->begin_element("input_layout"_s);
			{
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec3"_s);
					builder->attribute("name"_s, "position"_s);
					builder->attribute("semantic"_s, "POSITION"_s);
					builder->attribute("semantic_idx"_s, "0"_s);
				}builder->end_element(/*var*/);
			}builder->end_element(/*input_layout*/);
			builder->element("code"_s, vertex_shader);
		}builder->end_element(/*vertex_shader*/);
		builder->begin_element("pixel_shader"_s);
		{
			builder->begin_element("uniforms"_s);
			{
				builder->attribute("name"_s, "color_info"_s);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec4"_s);
					builder->attribute("name"_s, "additive_color"_s);
				}builder->end_element(/*var*/);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec4"_s);
					builder->attribute("name"_s, "diffuse_color"_s);
				}builder->end_element(/*var*/);
			}builder->end_element(/*uniforms*/);

			builder->begin_element("uniforms"_s);
			{
				builder->attribute("name"_s, "gradient_info"_s);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec2"_s);
					builder->attribute("name"_s, "gradient_start_point"_s);
				}builder->end_element(/*var*/);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec2"_s);
					builder->attribute("name"_s, "gradient_end_point"_s);
				}builder->end_element(/*var*/);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "s32"_s);
					builder->attribute("class"_s, "scalar"_s);
					builder->attribute("name"_s, "gradient_colors_count"_s);
				}builder->end_element(/*var*/);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec4"_s);
					builder->attribute("name"_s, "color_factor"_s);
					builder->attribute("array"_s, "10"_s);
				}builder->end_element(/*var*/);
			}builder->end_element(/*uniforms*/);
			builder->element("code"_s, pixel_shader);
		}builder->end_element(/*pixel_shader*/);
	}builder->end_element(/*technique*/);
	return builder->xml_root();
}

static xml::xml_node_t create_tecnique_texturing_template(cwstr_t name, textures::tex_wrap_mode_t wrap, cwstr_t vertex_shader, cwstr_t pixel_shader)
{
	xml::xml_builder_t builder = new xml::xml_builder();
	builder->begin_element("technique"_s);
	{
		builder->attribute("name"_s, name);
		builder->begin_element("vertex_shader"_s);
		{
			builder->begin_element("uniforms"_s);
			{
				builder->attribute("name"_s, "world_data"_s);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "mat4"_s);
					builder->attribute("name"_s, "world"_s);
				}builder->end_element(/*var*/);
			}builder->end_element(/*uniforms*/);
			builder->begin_element("input_layout"_s);
			{
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec3"_s);
					builder->attribute("name"_s, "position"_s);
					builder->attribute("semantic"_s, "POSITION"_s);
					builder->attribute("semantic_idx"_s, "0"_s);
				}builder->end_element(/*var*/);
			}builder->end_element(/*input_layout*/);
			builder->element("code"_s, vertex_shader);
		}builder->end_element(/*vertex_shader*/);
		builder->begin_element("pixel_shader"_s);
		{
			builder->begin_element("uniforms"_s);
			{
				builder->attribute("name"_s, "color_info"_s);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec4"_s);
					builder->attribute("name"_s, "additive_color"_s);
				}builder->end_element(/*var*/);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec4"_s);
					builder->attribute("name"_s, "diffuse_color"_s);
				}builder->end_element(/*var*/);
			}builder->end_element(/*uniforms*/);

			builder->begin_element("uniforms"_s);
			{
				builder->attribute("name"_s, "gradient_info"_s);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec2"_s);
					builder->attribute("name"_s, "gradient_start_point"_s);
				}builder->end_element(/*var*/);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec2"_s);
					builder->attribute("name"_s, "gradient_end_point"_s);
				}builder->end_element(/*var*/);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "s32"_s);
					builder->attribute("class"_s, "scalar"_s);
					builder->attribute("name"_s, "gradient_colors_count"_s);
				}builder->end_element(/*var*/);
				builder->begin_element("var"_s);
				{
					builder->attribute("type"_s, "f32"_s);
					builder->attribute("class"_s, "vec4"_s);
					builder->attribute("name"_s, "color_factor"_s);
					builder->attribute("array"_s, "10"_s);
				}builder->end_element(/*var*/);
			}builder->end_element(/*uniforms*/);
			builder->begin_element("samplers"_s);
			{
				builder->attribute("name"_s, "color_info"_s);
				builder->begin_element("sampler"_s);
				{
					builder->attribute("type"_s, "linear"_s);
					builder->attribute("wrap"_s, wrap.to_string());
				}builder->end_element(/*sampler*/);
			}builder->end_element(/*samplers*/);
			builder->element("code"_s, pixel_shader);
		}builder->end_element(/*pixel_shader*/);
	}builder->end_element(/*technique*/);
	return builder->xml_root();
}

d3d11_brush::d3d11_brush()
{

}

d3d11_brush::~d3d11_brush()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::graphics::d3d11::d3d11_brush);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::d3d11::d3d11_brush);

bool d3d11_brush::is_child_of(type_name_t name)
{
	return name == type_name<d3d11_brush>()
		|| object::is_child_of(name)
		|| drawing::ibrush::is_child_of(name);
}

bool d3d11_brush::is_kind_of(type_name_t name)const
{
	return name == type_name<d3d11_brush>()
		|| object::is_kind_of(name)
		|| drawing::ibrush::is_kind_of(name);
}

bool d3d11_brush::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<d3d11_brush>())
	{
		*out = static_cast<d3d11_brush*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (drawing::ibrush::query_object(name, out))
	{
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static wchar _solid_color_vertex_shader[] =
L"cbuffer world_data : register(b0) {\n"
L"	float4x4 world;\n"
L"}\n"
L"float4 main(float4 position : POSITION) : SV_Position {\n"
L"	return mul(float4(position.xyz, 1), world);\n"
L"}\n";

static wchar _solid_color_pixel_shader[] =
L"cbuffer color_info : register(b0) {\n"
L"	float4 additive_color;\n"
L"	float4 diffuse_color;\n"
L"}\n"
L"float4 main(float4 position : SV_Position) : SV_Target {\n"
L"	 return float4(diffuse_color.rgb * (1 - additive_color.a) + additive_color.rgb * additive_color.a, diffuse_color.a);\n"
L"}\n";

d3d11_solid_brush::d3d11_solid_brush()
{
}

d3d11_solid_brush::~d3d11_solid_brush()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::d3d11::d3d11_solid_brush, d3d11_brush);

bool d3d11_solid_brush::create(d3d11_draw_context_t context, color_t color, color_t additive)
{
	_diffuse_color = color;
	_additive_color = additive;
	_technique = context->effect_library->find_technique("solid_color_fx"_s);
	if (_technique.is_empty())	
		_technique = context->effect_library->load_technique(create_tecnique_template(L"solid_color_fx"_s, _solid_color_vertex_shader, _solid_color_pixel_shader));
	return !_technique.is_empty();
}

void d3d11_solid_brush::draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t model)
{
	driver->bind_shaders(_technique);
	auto world = _technique->map_vs_uniform(driver.get(), 0);
	world[0].cast<maths::float4x4>() = maths::matrix::transpose(tranform);
	_technique->unmap_vs_uniform(driver.get(), world);

	auto colors = _technique->map_ps_uniform(driver.get(), 0);
	colors[0].cast<maths::float4>() = color_to_vector(graphics::colors::black, 0);
	colors[1].cast<maths::float4>() = color_to_vector(_diffuse_color);
	_technique->unmap_ps_uniform(driver.get(), colors);

	driver->bind_index_buffer(model->indices);
	driver->bind_vertex_buffer(model->vertices);
	driver->draw_indexed(model->indices->counter(), graphics::primitive::triangle);
}



//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static wchar _linear_gradient_vertex_shader[] =
L"cbuffer world_data : register(b0) {\n"
L"	float4x4 world;\n"
L"}\n"
L"struct vs_output {\n"
L"	float4 position : SV_Position;\n"
L"	float2 screencoord : TEXCOORD9;\n"
L"};\n"
L"vs_output main(float4 position : POSITION) {\n"
L"	vs_output output;\n"
L"	output.position = mul(float4(position.xyz, 1), world);\n"
L"	output.screencoord = output.position.xy;\n"
L"	return output;\n"
L"}\n"
L"\n";

static wchar _linear_gradient_pixel_shader[] =
L"cbuffer color_info : register(b0) {\n"
L"	float4 additive_color;\n"
L"	float4 diffuse_color;\n"
L"}\n"
L"cbuffer gradient_info : register(b1) {\n"
L"	float2 gradient_start_point : packoffset(c0.x);\n"
L"	float2 gradient_end_point : packoffset(c0.z);\n"
L"	int gradient_colors_count : packoffset(c1.x);\n"
L"	float4 color_factor[10] : packoffset(c2);\n"
L"}\n"
L"float calc_factor(float2 A, float2 B, float2 C)\n"
L"{\n"
L"	if (A.x == B.x)\n"
L"	{\n"
L"		return (C.y - A.y) / (B.y - A.y);\n"
L"	}\n"
L"	else if (A.y == B.y)\n"
L"	{\n"
L"		return (C.x - A.x) / (B.x - A.x);\n"
L"	}\n"
L"	else\n"
L"	{\n"
L"		float2 BA = B - A;\n"
L"		float m1 = BA.y / BA.x;\n"
L"		float m2 = -1 / m1;\n"
L"		float2 D = { 0, 0 };\n"
L"		D.x = (m2 * C.x - m1 * A.x + A.y - C.y) / (m2 - m1);\n"
L"		D.y = m2 * D.x - m2 * C.x + C.y;\n"
L"		float factor = length(D - A) / length(BA);\n"
L"		return (A.x < B.x && D.x < A.x) || (A.x > B.x && D.x > A.x) ? - factor : factor;\n"
L"	}\n"
L"}\n"
L"float4 main(float4 position : SV_Position, float2 screencoord : TEXCOORD9) : SV_Target {\n"
L"	float3 diffuse = diffuse_color.rgb;\n"
L"	float factor = calc_factor(gradient_start_point, gradient_end_point, screencoord);\n"
L"	factor = max(min(factor, 1), 0);\n"
L"	for(int i = 1; i < gradient_colors_count; ++i) {\n"
L"		if(color_factor[i - 1].a <= factor && factor <= color_factor[i].a) { \n"
L"			diffuse *= lerp(color_factor[i - 1].rgb, color_factor[i].rgb, (factor - color_factor[i - 1].a) / (color_factor[i].a - color_factor[i - 1].a));"
L"			break; } \n"
L"	}\n"
L"	return float4(diffuse * (1 - additive_color.a) + additive_color.rgb * additive_color.a, diffuse_color.a);\n"
L"}\n";

d3d11_linear_gradient_brush::d3d11_linear_gradient_brush()
{
}

d3d11_linear_gradient_brush::~d3d11_linear_gradient_brush()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::d3d11::d3d11_linear_gradient_brush, d3d11_brush);

bool d3d11_linear_gradient_brush::create(d3d11_draw_context_t context, drawing::gradient_info gradients, color_t diffuse, color_t additive)
{
	_diffuse_color = diffuse;
	_additive_color = additive;
	_start_point = gradients.start_point;
	_end_point = gradients.end_point;
	_gradients = new collections::array_buffer<drawing::gradient_info::stop_color_info_t>(gradients.stop_colors.size(), gradients.stop_colors.data());
	_technique = context->effect_library->find_technique("linear_gradient_fx"_s);
	if (_technique.is_empty())
		_technique = context->effect_library->load_technique(create_tecnique_template(L"linear_gradient_fx"_s, _linear_gradient_vertex_shader, _linear_gradient_pixel_shader));
	return !_technique.is_empty();
}

void d3d11_linear_gradient_brush::draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t model)
{
	driver->bind_shaders(_technique);
	auto world = _technique->map_vs_uniform(driver.get(), 0);
	world[0].cast<maths::float4x4>() = maths::matrix::transpose(tranform);
	_technique->unmap_vs_uniform(driver.get(), world);

	auto colors = _technique->map_ps_uniform(driver.get(), 0);
	colors[0].cast<maths::float4>() = color_to_vector(_additive_color);
	colors[1].cast<maths::float4>() = color_to_vector(_diffuse_color);
	_technique->unmap_ps_uniform(driver.get(), colors);

	auto gradient_info = _technique->map_ps_uniform(driver.get(), 1);
	auto pos = maths::float4{ _start_point.x,_start_point.y,0,1 } *tranform;
	gradient_info[0].cast<maths::float2>() = { pos.get<0>(), pos.get<1>() };
	pos = maths::float4{ _end_point.x,_end_point.y,0,1 } *tranform;
	gradient_info[1].cast<maths::float2>() = { pos.get<0>(), pos.get<1>() };

	gradient_info[2].cast<int>() = _gradients->counter();
	auto gradients = gradient_info[3].cast<static_array<graphics::reflect::variable>>();
	for (index i = 0; i <_gradients->counter(); ++i)
	{
		drawing::gradient_info::stop_color_info_t& gradient = _gradients[i];
		gradients[i].cast<maths::float4>() = color_to_vector(gradient.stop_color, gradient.stop_factor);
	}
	_technique->unmap_ps_uniform(driver.get(), gradient_info);

	driver->bind_index_buffer(model->indices);
	driver->bind_vertex_buffer(model->vertices);
	driver->draw_indexed(model->indices->counter(), graphics::primitive::triangle);
}



//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static wchar _texturing_vertex_shader[] =
L"cbuffer world_data : register(b0) {\n"
L"	float4x4 world;\n"
L"}\n"
L"struct vs_output {\n"
L"	float4 position : SV_Position;\n"
L"	float2 texcoord : TEXCOORD0;\n"
L"};\n"
L"vs_output main(float4 position : POSITION) {\n"
L"	vs_output output;\n"
L"	output.position = mul(float4(position.xyz, 1), world);\n"
L"	output.texcoord = float2((1 + position.x) / 2, (1 - position.y) / 2);\n"
L"	return output;\n"
L"}\n"
L"\n";

static wchar _texturing_pixel_shader[] =
L"cbuffer color_info : register(b0) {\n"
L"	float4 additive_color;\n"
L"	float4 diffuse_color;\n"
L"}\n"
L"Texture2D difusse_tex : register(t0);\n"
L"SamplerState sam_linear : register(s0);\n"
L"float4 main(float4 position : SV_Position, float2 texcoord : TEXCOORD0) : SV_Target {\n"
L"	float4 diffuse = difusse_tex.Sample(sam_linear, texcoord);\n"
L"	return float4(diffuse.rgb * diffuse_color.rgb * (1 - additive_color.a) + additive_color.rgb * additive_color.a, diffuse_color.a * diffuse.a);\n"
L"}\n";

d3d11_texturing_brush::d3d11_texturing_brush()
{
}

d3d11_texturing_brush::~d3d11_texturing_brush()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::d3d11::d3d11_texturing_brush, d3d11_brush);

bool d3d11_texturing_brush::create(d3d11_draw_context_t context, drawing::texturing_info_t info, color_t diffuse, color_t additive)
{
	_diffuse_color = diffuse;
	_additive_color = additive;
	_texture = info.texture;
	_tilling = info.tiling_factor;
	_stretch = info.stretch_mode;
	_technique = context->effect_library->find_technique("texturing_fx"_s);
	if (_technique.is_empty())
		_technique = context->effect_library->load_technique(create_tecnique_texturing_template(L"texturing_fx"_s, info.wrap_mode, _texturing_vertex_shader, _texturing_pixel_shader));
	return !_technique.is_empty();
}

void d3d11_texturing_brush::draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t model)
{
	driver->bind_shaders(_technique);
	driver->bind_texture(_texture, 0);
	auto world = _technique->map_vs_uniform(driver.get(), 0);
	world[0].cast<maths::float4x4>() = maths::matrix::transpose(tranform);
	_technique->unmap_vs_uniform(driver.get(), world);

	auto colors = _technique->map_ps_uniform(driver.get(), 0);
	colors[0].cast<maths::float4>() = color_to_vector(_additive_color);
	colors[1].cast<maths::float4>() = color_to_vector(_diffuse_color);
	_technique->unmap_ps_uniform(driver.get(), colors);

	driver->bind_index_buffer(model->indices);
	driver->bind_vertex_buffer(model->vertices);
	driver->bind_texture(_texture, 0);
	driver->draw_indexed(model->indices->counter(), graphics::primitive::triangle);
	driver->bind_texture(null, 0);
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static wchar _linear_gradient_texturing_vertex_shader[] =
L"cbuffer world_data : register(b0) {\n"
L"	float4x4 world;\n"
L"}\n"
L"struct vs_output {\n"
L"	float4 position : SV_Position;\n"
L"	float2 texcoord : TEXCOORD0;\n"
L"	float2 screencoord : TEXCOORD9;\n"
L"};\n"
L"vs_output main(float4 position : POSITION) {\n"
L"	vs_output output;\n"
L"	output.position = mul(float4(position.xyz, 1), world);\n"
L"	output.screencoord = output.position.xy;\n"
L"	output.texcoord = float2((1 + position.x) / 2, (1 - position.y) / 2);\n"
L"	return output;\n"
L"}\n"
L"\n";

static wchar _linear_gradient_texturing_pixel_shader[] =
L"cbuffer color_info : register(b0) {\n"
L"	float4 additive_color;\n"
L"	float4 diffuse_color;\n"
L"}\n"
L"cbuffer gradient_info : register(b1) {\n"
L"	float2 gradient_start_point : packoffset(c0.x);\n"
L"	float2 gradient_end_point : packoffset(c0.z);\n"
L"	int gradient_colors_count : packoffset(c1.x);\n"
L"	float4 color_factor[10] : packoffset(c2);\n"
L"}\n"
L"Texture2D difusse_tex : register(t0);\n"
L"SamplerState sam_linear : register(s0);\n"
L"float calc_factor(float2 A, float2 B, float2 C)\n"
L"{\n"
L"	if (A.x == B.x)\n"
L"	{\n"
L"		return (C.y - A.y) / (B.y - A.y);\n"
L"	}\n"
L"	else if (A.y == B.y)\n"
L"	{\n"
L"		return (C.x - A.x) / (B.x - A.x);\n"
L"	}\n"
L"	else\n"
L"	{\n"
L"		float2 BA = B - A;\n"
L"		float m1 = BA.y / BA.x;\n"
L"		float m2 = -1 / m1;\n"
L"		float2 D = { 0, 0 };\n"
L"		D.x = (m2 * C.x - m1 * A.x + A.y - C.y) / (m2 - m1);\n"
L"		D.y = m2 * D.x - m2 * C.x + C.y;\n"
L"		float factor = length(D - A) / length(BA);\n"
L"		return D.x < A.x ? -factor : factor;\n"
L"	}\n"
L"}\n"
L"float4 main(float4 position : SV_Position, float2 texcoord : TEXCOORD0, float2 screencoord : TEXCOORD9) : SV_Target {\n"
L"	float4 texture_color = difusse_tex.Sample(sam_linear, texcoord);\n"
L"	float3 diffuse = diffuse_color.rgb;\n"
L"	float factor = calc_factor(gradient_start_point, gradient_end_point, screencoord);\n"
L"	factor = max(min(factor, 1), 0);\n"
L"	for(int i = 1; i < gradient_colors_count; ++i) {\n"
L"		if(color_factor[i - 1].a <= factor && factor <= color_factor[i].a) { \n"
L"			diffuse *= lerp(color_factor[i - 1].rgb, color_factor[i].rgb, (factor - color_factor[i - 1].a) / (color_factor[i].a - color_factor[i - 1].a));"
L"			break; } \n"
L"	}\n"
L"	return float4(texture_color.rgb * diffuse * (1 - additive_color.a) + additive_color.rgb * additive_color.a, diffuse_color.a * texture_color.a);\n"
L"}\n";

d3d11_linear_gradient_texturing_brush::d3d11_linear_gradient_texturing_brush()
{
}

d3d11_linear_gradient_texturing_brush::~d3d11_linear_gradient_texturing_brush()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::d3d11::d3d11_linear_gradient_texturing_brush, d3d11_brush);

bool d3d11_linear_gradient_texturing_brush::create(d3d11_draw_context_t context, drawing::gradient_info gradients, drawing::texturing_info_t texture, color_t diffuse, color_t additive)
{
	_diffuse_color = diffuse;
	_additive_color = additive;
	_start_point = gradients.start_point;
	_end_point = gradients.end_point;
	_texture = texture.texture;
	_tilling = texture.tiling_factor;
	_stretch = texture.stretch_mode;
	_gradients = new collections::array_buffer<drawing::gradient_info::stop_color_info_t>(gradients.stop_colors.size(), gradients.stop_colors.data());
	_technique = context->effect_library->find_technique("linear_gradient_texturing_fx"_s);
	if (_technique.is_empty())
		_technique = context->effect_library->load_technique(create_tecnique_texturing_template(L"linear_gradient_fx"_s, texture.wrap_mode, _linear_gradient_texturing_vertex_shader, _linear_gradient_texturing_pixel_shader));
	return !_technique.is_empty();
}

void d3d11_linear_gradient_texturing_brush::draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t model)
{
	driver->bind_shaders(_technique);
	auto world = _technique->map_vs_uniform(driver.get(), 0);
	world[0].cast<maths::float4x4>() = maths::matrix::transpose(tranform);
	_technique->unmap_vs_uniform(driver.get(), world);

	auto colors = _technique->map_ps_uniform(driver.get(), 0);
	colors[0].cast<maths::float4>() = color_to_vector(_additive_color);
	colors[1].cast<maths::float4>() = color_to_vector(_diffuse_color);
	_technique->unmap_ps_uniform(driver.get(), colors);

	auto gradient_info = _technique->map_ps_uniform(driver.get(), 1);
	auto pos = maths::float4{ _start_point.x,_start_point.y,0,1 } *tranform;
	gradient_info[0].cast<maths::float2>() = { pos.get<0>(), pos.get<1>() };
	pos = maths::float4{ _end_point.x,_end_point.y,0,1 } *tranform;
	gradient_info[1].cast<maths::float2>() = { pos.get<0>(), pos.get<1>() };

	gradient_info[2].cast<int>() = _gradients->counter();
	auto gradients = gradient_info[3].cast<static_array<graphics::reflect::variable>>();
	for (index i = 0; i <_gradients->counter(); ++i)
	{
		drawing::gradient_info::stop_color_info_t& gradient = _gradients[i];
		gradients[i].cast<maths::float4>() = color_to_vector(gradient.stop_color, gradient.stop_factor);
	}
	_technique->unmap_ps_uniform(driver.get(), gradient_info);

	driver->bind_index_buffer(model->indices);
	driver->bind_vertex_buffer(model->vertices);
	driver->draw_indexed(model->indices->counter(), graphics::primitive::triangle);
}


#endif//DIRECTX_SUPPORT