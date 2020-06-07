#include "pch.h"
#include "d3d11/drawing/d3d11_drawer.h"

#if DIRECTX_SUPPORT

using namespace coffe;
using namespace coffe::dom;
using namespace coffe::graphics;
using namespace coffe::graphics::d3d11;

//static xml::xml_node_t create_tecnique_template(cwstr_t name, cwstr_t vertex_shader, cwstr_t pixel_shader)
//{
//	xml::xml_document_t document = new xml::xml_document();
//	document->begin_element("technique"_s);
//	{
//		document->push_attribute("name"_s, name);
//		document->begin_element("vertex_shader"_s);
//		{
//			document->begin_element("uniforms"_s);
//			{
//				document->push_attribute("name"_s, "world_data"_s);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "mat4"_s);
//					document->push_attribute("name"_s, "world"_s);
//				}document->end_element(/*var*/);
//			}document->end_element(/*uniforms*/);
//			document->begin_element("input_layout"_s);
//			{
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec3"_s);
//					document->push_attribute("name"_s, "position"_s);
//					document->push_attribute("semantic"_s, "POSITION"_s);
//					document->push_attribute("semantic_idx"_s, "0"_s);
//				}document->end_element(/*var*/);
//			}document->end_element(/*input_layout*/);
//			document->push_element("code"_s, vertex_shader);
//		}document->end_element(/*vertex_shader*/);
//		document->begin_element("pixel_shader"_s);
//		{
//			document->begin_element("uniforms"_s);
//			{
//				document->push_attribute("name"_s, "color_info"_s);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec4"_s);
//					document->push_attribute("name"_s, "additive_color"_s);
//				}document->end_element(/*var*/);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec4"_s);
//					document->push_attribute("name"_s, "diffuse_color"_s);
//				}document->end_element(/*var*/);
//			}document->end_element(/*uniforms*/);
//
//			document->begin_element("uniforms"_s);
//			{
//				document->push_attribute("name"_s, "gradient_info"_s);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec2"_s);
//					document->push_attribute("name"_s, "gradient_start_point"_s);
//				}document->end_element(/*var*/);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec2"_s);
//					document->push_attribute("name"_s, "gradient_end_point"_s);
//				}document->end_element(/*var*/);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "s32"_s);
//					document->push_attribute("class"_s, "scalar"_s);
//					document->push_attribute("name"_s, "gradient_colors_count"_s);
//				}document->end_element(/*var*/);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec4"_s);
//					document->push_attribute("name"_s, "color_factor"_s);
//					document->push_attribute("array"_s, "10"_s);
//				}document->end_element(/*var*/);
//			}document->end_element(/*uniforms*/);
//			document->push_element("code"_s, pixel_shader);
//		}document->end_element(/*pixel_shader*/);
//	}document->end_element(/*technique*/);
//	return document->xml_root_element();
//}
//
//static xml::xml_node_t create_tecnique_texturing_template(cwstr_t name, textures::tex_wrap_mode_t wrap, cwstr_t vertex_shader, cwstr_t pixel_shader)
//{
//	xml::xml_document_t document = new xml::xml_document();
//	document->begin_element("technique"_s);
//	{
//		document->push_attribute("name"_s, name);
//		document->begin_element("vertex_shader"_s);
//		{
//			document->begin_element("uniforms"_s);
//			{
//				document->push_attribute("name"_s, "world_data"_s);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "mat4"_s);
//					document->push_attribute("name"_s, "world"_s);
//				}document->end_element(/*var*/);
//			}document->end_element(/*uniforms*/);
//			document->begin_element("input_layout"_s);
//			{
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec3"_s);
//					document->push_attribute("name"_s, "position"_s);
//					document->push_attribute("semantic"_s, "POSITION"_s);
//					document->push_attribute("semantic_idx"_s, "0"_s);
//				}document->end_element(/*var*/);
//			}document->end_element(/*input_layout*/);
//			document->push_element("code"_s, vertex_shader);
//		}document->end_element(/*vertex_shader*/);
//		document->begin_element("pixel_shader"_s);
//		{
//			document->begin_element("uniforms"_s);
//			{
//				document->push_attribute("name"_s, "color_info"_s);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec4"_s);
//					document->push_attribute("name"_s, "additive_color"_s);
//				}document->end_element(/*var*/);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec4"_s);
//					document->push_attribute("name"_s, "diffuse_color"_s);
//				}document->end_element(/*var*/);
//			}document->end_element(/*uniforms*/);
//
//			document->begin_element("uniforms"_s);
//			{
//				document->push_attribute("name"_s, "gradient_info"_s);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec2"_s);
//					document->push_attribute("name"_s, "gradient_start_point"_s);
//				}document->end_element(/*var*/);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec2"_s);
//					document->push_attribute("name"_s, "gradient_end_point"_s);
//				}document->end_element(/*var*/);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "s32"_s);
//					document->push_attribute("class"_s, "scalar"_s);
//					document->push_attribute("name"_s, "gradient_colors_count"_s);
//				}document->end_element(/*var*/);
//				document->begin_element("var"_s);
//				{
//					document->push_attribute("type"_s, "f32"_s);
//					document->push_attribute("class"_s, "vec4"_s);
//					document->push_attribute("name"_s, "color_factor"_s);
//					document->push_attribute("array"_s, "10"_s);
//				}document->end_element(/*var*/);
//			}document->end_element(/*uniforms*/);
//			document->begin_element("samplers"_s);
//			{
//				document->push_attribute("name"_s, "color_info"_s);
//				document->begin_element("sampler"_s);
//				{
//					document->push_attribute("type"_s, "linear"_s);
//					document->push_attribute("wrap"_s, wrap.to_string());
//				}document->end_element(/*sampler*/);
//			}document->end_element(/*samplers*/);
//			document->push_element("code"_s, pixel_shader);
//		}document->end_element(/*pixel_shader*/);
//	}document->end_element(/*technique*/);
//	return document->xml_root_element();
//}

d3d11_brush::d3d11_brush()
{

}

d3d11_brush::~d3d11_brush()
{

}

COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::d3d11::d3d11_brush);
COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::d3d11::d3d11_brush, object, drawing::ibrush);
COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::d3d11::d3d11_brush, object, drawing::ibrush);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



static castr_t _solid_color_vertex_shader =
"cbuffer wvp : register(b0) {\n"
"	float4x4 world;\n"
"}\n"
"float4 main(float4 position : POSITION) : SV_Position {\n"
"	return mul(float4(position.xyz, 1), world);\n"
"}\n";

static castr_t _solid_color_pixel_shader =
"cbuffer color_info : register(b0) {\n"
"	float4 additive_color;\n"
"	float4 diffuse_color;\n"
"}\n"
"float4 main(float4 position : SV_Position) : SV_Target {\n"
"	 return float4(diffuse_color.rgb * (1 - additive_color.a) + additive_color.rgb * additive_color.a, diffuse_color.a);\n"
"}\n";

d3d11_solid_brush::d3d11_solid_brush()
{
}

d3d11_solid_brush::~d3d11_solid_brush()
{
}


COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::d3d11::d3d11_solid_brush);
COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::d3d11::d3d11_solid_brush, d3d11_brush);
COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::d3d11::d3d11_solid_brush, d3d11_brush);

bool d3d11_solid_brush::create(d3d11_drawer_t context, color_t color, color_t additive)
{
	m_diffuse_color = color;
	m_additive_color = additive;
	m_technique = context->find_technique("solid_color_fx"_s);
	if (m_technique.is_empty()) {
		effects::shader_info_t vertex;
		effects::shader_info_t pixel;

		vertex.code = _solid_color_vertex_shader;
		vertex.compile_config = "-DDIRECTX11";
		vertex.entry = "main";
		pixel.code = _solid_color_pixel_shader;
		pixel.compile_config = "-DDIRECTX11";
		pixel.entry = "main";
		
		m_technique = context->load_technique(vertex, pixel, "solid_color_fx"_s);
	}
		
	return !m_technique.is_empty();
}

void d3d11_solid_brush::draw(d3d11_driver_t driver, maths::mat4 const& tranform, model_ptr_t model)
{
	driver->bind_shaders(m_technique);
	auto wvp = m_technique->map_vs_uniform(driver.get(), 0);
	auto world = wvp[0];
	world.cast<maths::float4x4>() = maths::matrix::transpose(tranform);
	m_technique->unmap_vs_uniform(driver.get(), wvp);

	auto colors = m_technique->map_ps_uniform(driver.get(), 0);
	colors[0].cast<maths::float4>() = color_to_vector(graphics::colors::black, 0);
	colors[1].cast<maths::float4>() = color_to_vector(m_diffuse_color);
	m_technique->unmap_ps_uniform(driver.get(), colors);

	driver->bind_index_buffer(model->indices);
	driver->bind_vertex_buffer(model->vertices);
	driver->draw_indexed(model->indices->counter(), graphics::primitive::triangle);
}



//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static castr_t _linear_gradient_vertex_shader =
"cbuffer world_data : register(b0) {\n"
"	float4x4 world;\n"
"}\n"
"struct vs_output {\n"
"	float4 position : SV_Position;\n"
"	float2 screencoord : TEXCOORD9;\n"
"};\n"
"vs_output main(float4 position : POSITION) {\n"
"	vs_output output;\n"
"	output.position = mul(float4(position.xyz, 1), world);\n"
"	output.screencoord = output.position.xy;\n"
"	return output;\n"
"}\n"
"\n";

static castr_t _linear_gradient_pixel_shader =
"cbuffer color_info : register(b0) {\n"
"	float4 additive_color;\n"
"	float4 diffuse_color;\n"
"}\n"
"cbuffer gradient_info : register(b1) {\n"
"	float2 gradient_start_point : packoffset(c0.x);\n"
"	float2 gradient_end_point : packoffset(c0.z);\n"
"	int gradient_colors_count : packoffset(c1.x);\n"
"	float4 color_factor[10] : packoffset(c2);\n"
"}\n"
"float calc_factor(float2 A, float2 B, float2 C)\n"
"{\n"
"	if (A.x == B.x)\n"
"	{\n"
"		return (C.y - A.y) / (B.y - A.y);\n"
"	}\n"
"	else if (A.y == B.y)\n"
"	{\n"
"		return (C.x - A.x) / (B.x - A.x);\n"
"	}\n"
"	else\n"
"	{\n"
"		float2 BA = B - A;\n"
"		float m1 = BA.y / BA.x;\n"
"		float m2 = -1 / m1;\n"
"		float2 D = { 0, 0 };\n"
"		D.x = (m2 * C.x - m1 * A.x + A.y - C.y) / (m2 - m1);\n"
"		D.y = m2 * D.x - m2 * C.x + C.y;\n"
"		float factor = length(D - A) / length(BA);\n"
"		return (A.x < B.x && D.x < A.x) || (A.x > B.x && D.x > A.x) ? - factor : factor;\n"
"	}\n"
"}\n"
"float4 main(float4 position : SV_Position, float2 screencoord : TEXCOORD9) : SV_Target {\n"
"	float3 diffuse = diffuse_color.rgb;\n"
"	float factor = calc_factor(gradient_start_point, gradient_end_point, screencoord);\n"
"	factor = max(min(factor, 1), 0);\n"
"	for(int i = 1; i < gradient_colors_count; ++i) {\n"
"		if(color_factor[i - 1].a <= factor && factor <= color_factor[i].a) { \n"
"			diffuse *= lerp(color_factor[i - 1].rgb, color_factor[i].rgb, (factor - color_factor[i - 1].a) / (color_factor[i].a - color_factor[i - 1].a));"
"			break; } \n"
"	}\n"
"	return float4(diffuse * (1 - additive_color.a) + additive_color.rgb * additive_color.a, diffuse_color.a);\n"
"}\n";

d3d11_linear_gradient_brush::d3d11_linear_gradient_brush()
{
}

d3d11_linear_gradient_brush::~d3d11_linear_gradient_brush()
{
}

COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::d3d11::d3d11_linear_gradient_brush);
COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::d3d11::d3d11_linear_gradient_brush, d3d11_brush);
COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::d3d11::d3d11_linear_gradient_brush, d3d11_brush);

bool d3d11_linear_gradient_brush::create(d3d11_drawer_t context, drawing::gradient_info gradients, color_t diffuse, color_t additive)
{
	m_diffuse_color = diffuse;
	m_additive_color = additive;
	m_start_point = gradients.start_point;
	m_end_point = gradients.end_point;
	m_gradients = to_array(gradients.stop_colors.data(), gradients.stop_colors.size());
	
	m_technique = context->find_technique("linear_gradient_fx"_s);
	if (m_technique.is_empty()) {
		effects::shader_info_t vertex;
		effects::shader_info_t pixel;

		vertex.code = _linear_gradient_vertex_shader;
		vertex.compile_config = "-DDIRECTX11";
		vertex.entry = "main";
		pixel.code = _linear_gradient_pixel_shader;
		pixel.compile_config = "-DDIRECTX11";
		pixel.entry = "main";

		m_technique = context->load_technique(vertex, pixel, "linear_gradient_fx"_s);
	}
	
	return !m_technique.is_empty();
}

void d3d11_linear_gradient_brush::draw(d3d11_driver_t driver, maths::mat4 const& tranform, model_ptr_t model)
{
	driver->bind_shaders(m_technique);
	auto world = m_technique->map_vs_uniform(driver.get(), 0);
	world[0].cast<maths::float4x4>() = maths::matrix::transpose(tranform);
	m_technique->unmap_vs_uniform(driver.get(), world);

	auto colors = m_technique->map_ps_uniform(driver.get(), 0);
	colors[0].cast<maths::float4>() = color_to_vector(m_additive_color);
	colors[1].cast<maths::float4>() = color_to_vector(m_diffuse_color);
	m_technique->unmap_ps_uniform(driver.get(), colors);

	auto gradient_info = m_technique->map_ps_uniform(driver.get(), 1);
	auto pos = maths::float4{ m_start_point.x,m_start_point.y,0,1 } *tranform;
	gradient_info[0].cast<maths::float2>() = { pos.get<0>(), pos.get<1>() };
	pos = maths::float4{ m_end_point.x,m_end_point.y,0,1 } *tranform;
	gradient_info[1].cast<maths::float2>() = { pos.get<0>(), pos.get<1>() };

	gradient_info[2].cast<int>() = m_gradients->counter();
	auto gradients = gradient_info[3].cast<array_view<graphics::reflect::varying>>();
	for (windex i = 0; i <m_gradients->counter(); ++i)
	{
		drawing::gradient_info::stop_color_info_t& gradient = m_gradients[i];
		gradients[i].cast<maths::float4>() = color_to_vector(gradient.stop_color, gradient.stop_factor);
	}
	m_technique->unmap_ps_uniform(driver.get(), gradient_info);

	driver->bind_index_buffer(model->indices);
	driver->bind_vertex_buffer(model->vertices);
	driver->draw_indexed(model->indices->counter(), graphics::primitive::triangle);
}



//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static castr_t _texturing_vertex_shader =
"cbuffer world_data : register(b0) {\n"
"	float4x4 world;\n"
"}\n"
"struct vs_output {\n"
"	float4 position : SV_Position;\n"
"	float2 texcoord : TEXCOORD0;\n"
"};\n"
"vs_output main(float4 position : POSITION) {\n"
"	vs_output output;\n"
"	output.position = mul(float4(position.xyz, 1), world);\n"
"	output.texcoord = float2((1 + position.x) / 2, (1 - position.y) / 2);\n"
"	return output;\n"
"}\n"
"\n";

static castr_t _texturing_pixel_shader =
"cbuffer color_info : register(b0) {\n"
"	float4 additive_color;\n"
"	float4 diffuse_color;\n"
"}\n"
"Texture2D difusse_tex : register(t0);\n"
"SamplerState sam_linear : register(s0);\n"
"float4 main(float4 position : SV_Position, float2 texcoord : TEXCOORD0) : SV_Target {\n"
"	float4 diffuse = difusse_tex.Sample(sam_linear, texcoord);\n"
"	return float4(diffuse.rgb * diffuse_color.rgb * (1 - additive_color.a) + additive_color.rgb * additive_color.a, diffuse_color.a * diffuse.a);\n"
"}\n";

d3d11_texturing_brush::d3d11_texturing_brush()
{
}

d3d11_texturing_brush::~d3d11_texturing_brush()
{
}

COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::d3d11::d3d11_texturing_brush);
COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::d3d11::d3d11_texturing_brush, d3d11_brush);
COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::d3d11::d3d11_texturing_brush, d3d11_brush);

bool d3d11_texturing_brush::create(d3d11_drawer_t context, drawing::texturing_info_t info, color_t diffuse, color_t additive)
{
	m_diffuse_color = diffuse;
	m_additive_color = additive;
	m_texture = info.texture;
	m_tilling = info.tiling_factor;
	m_stretch = info.stretch_mode;
	m_technique = context->find_technique("texturing_fx"_s);
	if (m_technique.is_empty()) {
		effects::shader_info_t vertex;
		effects::shader_info_t pixel;

		vertex.code = _texturing_vertex_shader;
		vertex.compile_config = "-DDIRECTX11";
		vertex.entry = "main";
		pixel.code = _texturing_pixel_shader;
		pixel.compile_config = "-DDIRECTX11";
		pixel.entry = "main";

		m_technique = context->load_technique(vertex, pixel, "texturing_fx"_s);
	}
	return !m_technique.is_empty();
}

void d3d11_texturing_brush::draw(d3d11_driver_t driver, maths::mat4 const& tranform, model_ptr_t model)
{
	driver->bind_shaders(m_technique);
	driver->bind_texture(m_texture, 0);
	auto world = m_technique->map_vs_uniform(driver.get(), 0);
	world[0].cast<maths::float4x4>() = maths::matrix::transpose(tranform);
	m_technique->unmap_vs_uniform(driver.get(), world);

	auto colors = m_technique->map_ps_uniform(driver.get(), 0);
	colors[0].cast<maths::float4>() = color_to_vector(m_additive_color);
	colors[1].cast<maths::float4>() = color_to_vector(m_diffuse_color);
	m_technique->unmap_ps_uniform(driver.get(), colors);

	driver->bind_index_buffer(model->indices);
	driver->bind_vertex_buffer(model->vertices);
	driver->bind_texture(m_texture, 0);
	driver->draw_indexed(model->indices->counter(), graphics::primitive::triangle);
	driver->bind_texture(null, 0);
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

static castr_t _linear_gradient_texturing_vertex_shader =
"cbuffer world_data : register(b0) {\n"
"	float4x4 world;\n"
"}\n"
"struct vs_output {\n"
"	float4 position : SV_Position;\n"
"	float2 texcoord : TEXCOORD0;\n"
"	float2 screencoord : TEXCOORD9;\n"
"};\n"
"vs_output main(float4 position : POSITION) {\n"
"	vs_output output;\n"
"	output.position = mul(float4(position.xyz, 1), world);\n"
"	output.screencoord = output.position.xy;\n"
"	output.texcoord = float2((1 + position.x) / 2, (1 - position.y) / 2);\n"
"	return output;\n"
"}\n"
"\n";

static castr_t _linear_gradient_texturing_pixel_shader =
"cbuffer color_info : register(b0) {\n"
"	float4 additive_color;\n"
"	float4 diffuse_color;\n"
"}\n"
"cbuffer gradient_info : register(b1) {\n"
"	float2 gradient_start_point : packoffset(c0.x);\n"
"	float2 gradient_end_point : packoffset(c0.z);\n"
"	int gradient_colors_count : packoffset(c1.x);\n"
"	float4 color_factor[10] : packoffset(c2);\n"
"}\n"
"Texture2D difusse_tex : register(t0);\n"
"SamplerState sam_linear : register(s0);\n"
"float calc_factor(float2 A, float2 B, float2 C)\n"
"{\n"
"	if (A.x == B.x)\n"
"	{\n"
"		return (C.y - A.y) / (B.y - A.y);\n"
"	}\n"
"	else if (A.y == B.y)\n"
"	{\n"
"		return (C.x - A.x) / (B.x - A.x);\n"
"	}\n"
"	else\n"
"	{\n"
"		float2 BA = B - A;\n"
"		float m1 = BA.y / BA.x;\n"
"		float m2 = -1 / m1;\n"
"		float2 D = { 0, 0 };\n"
"		D.x = (m2 * C.x - m1 * A.x + A.y - C.y) / (m2 - m1);\n"
"		D.y = m2 * D.x - m2 * C.x + C.y;\n"
"		float factor = length(D - A) / length(BA);\n"
"		return D.x < A.x ? -factor : factor;\n"
"	}\n"
"}\n"
"float4 main(float4 position : SV_Position, float2 texcoord : TEXCOORD0, float2 screencoord : TEXCOORD9) : SV_Target {\n"
"	float4 texture_color = difusse_tex.Sample(sam_linear, texcoord);\n"
"	float3 diffuse = diffuse_color.rgb;\n"
"	float factor = calc_factor(gradient_start_point, gradient_end_point, screencoord);\n"
"	factor = max(min(factor, 1), 0);\n"
"	for(int i = 1; i < gradient_colors_count; ++i) {\n"
"		if(color_factor[i - 1].a <= factor && factor <= color_factor[i].a) { \n"
"			diffuse *= lerp(color_factor[i - 1].rgb, color_factor[i].rgb, (factor - color_factor[i - 1].a) / (color_factor[i].a - color_factor[i - 1].a));"
"			break; } \n"
"	}\n"
"	return float4(texture_color.rgb * diffuse * (1 - additive_color.a) + additive_color.rgb * additive_color.a, diffuse_color.a * texture_color.a);\n"
"}\n";

d3d11_linear_gradient_texturing_brush::d3d11_linear_gradient_texturing_brush()
{
}

d3d11_linear_gradient_texturing_brush::~d3d11_linear_gradient_texturing_brush()
{
}

COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::d3d11::d3d11_linear_gradient_texturing_brush);
COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::d3d11::d3d11_linear_gradient_texturing_brush, d3d11_brush);
COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::d3d11::d3d11_linear_gradient_texturing_brush, d3d11_brush);

bool d3d11_linear_gradient_texturing_brush::create(d3d11_drawer_t context, drawing::gradient_info gradients, drawing::texturing_info_t texture, color_t diffuse, color_t additive)
{
	m_diffuse_color = diffuse;
	m_additive_color = additive;
	m_start_point = gradients.start_point;
	m_end_point = gradients.end_point;
	m_texture = texture.texture;
	m_tilling = texture.tiling_factor;
	m_stretch = texture.stretch_mode;
	m_gradients = to_array(gradients.stop_colors.data(), gradients.stop_colors.size());
	
	m_technique = context->find_technique("linear_gradient_texturing_fx"_s);
	if (m_technique.is_empty()) {
		effects::shader_info_t vertex;
		effects::shader_info_t pixel;

		vertex.code = _texturing_vertex_shader;
		vertex.compile_config = "-DDIRECTX11";
		vertex.entry = "main";
		pixel.code = _texturing_pixel_shader;
		pixel.compile_config = "-DDIRECTX11";
		pixel.entry = "main";

		m_technique = context->load_technique(vertex, pixel, "linear_gradient_texturing_fx"_s);
	}
	return !m_technique.is_empty();
}

void d3d11_linear_gradient_texturing_brush::draw(d3d11_driver_t driver, maths::mat4 const& tranform, model_ptr_t model)
{
	driver->bind_shaders(m_technique);
	auto world = m_technique->map_vs_uniform(driver.get(), 0);
	world[0].cast<maths::float4x4>() = maths::matrix::transpose(tranform);
	m_technique->unmap_vs_uniform(driver.get(), world);

	auto colors = m_technique->map_ps_uniform(driver.get(), 0);
	colors[0].cast<maths::float4>() = color_to_vector(m_additive_color);
	colors[1].cast<maths::float4>() = color_to_vector(m_diffuse_color);
	m_technique->unmap_ps_uniform(driver.get(), colors);

	auto gradient_info = m_technique->map_ps_uniform(driver.get(), 1);
	auto pos = maths::float4{ m_start_point.x,m_start_point.y,0,1 } *tranform;
	gradient_info[0].cast<maths::float2>() = { pos.get<0>(), pos.get<1>() };
	pos = maths::float4{ m_end_point.x,m_end_point.y,0,1 } *tranform;
	gradient_info[1].cast<maths::float2>() = { pos.get<0>(), pos.get<1>() };

	gradient_info[2].cast<int>() = m_gradients->counter();
	auto gradients = gradient_info[3].cast<array_view<graphics::reflect::varying>>();
	for (windex i = 0; i <m_gradients->counter(); ++i)
	{
		drawing::gradient_info::stop_color_info_t& gradient = m_gradients[i];
		gradients[i].cast<maths::float4>() = color_to_vector(gradient.stop_color, gradient.stop_factor);
	}
	m_technique->unmap_ps_uniform(driver.get(), gradient_info);

	driver->bind_index_buffer(model->indices);
	driver->bind_vertex_buffer(model->vertices);
	driver->draw_indexed(model->indices->counter(), graphics::primitive::triangle);
}


#endif//DIRECTX_SUPPORT