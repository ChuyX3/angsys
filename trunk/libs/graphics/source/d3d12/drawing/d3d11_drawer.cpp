#include "pch.h"
#include "d3d11/drawing/d3d11_drawer.h"

#if DIRECTX_SUPPORT

using namespace coffe;
using namespace coffe::graphics;
using namespace coffe::graphics::d3d11;

d3d11_drawer::d3d11_drawer(d3d11_driver_t driver)
	: m_is_drawing(false)
{
	create(driver);
}

d3d11_drawer::~d3d11_drawer()
{

}

COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::d3d11::d3d11_drawer);
COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::d3d11::d3d11_drawer, object, drawing::idrawer);
COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::d3d11::d3d11_drawer, object, drawing::idrawer);

void d3d11_drawer::dispose()
{
	close();
}

bool d3d11_drawer::create(d3d11_driver_t driver)
{
	if (driver.is_empty())
		return false;
	m_driver = driver;

	reflect::varying;
	{//creating square model
		array<maths::float4> vertices = initializer_list<maths::float4>{
			{ -1.0f,1.0f,-1.0f,1.0f },
			{ 1.0f,1.0f,-1.0f,1.0f },
			{ 1.0f,-1.0f,-1.0f,1.0f },
			{ -1.0f,-1.0f,-1.0f,1.0f },
		};

		reflect::attribute_desc input_layout[] =
		{
			{ reflect::var_type::f32, reflect::var_class::vec4 }
		};

		array<word> indices = { 0,2,1,0,3,2 };
		
		if(auto res = driver->create_index_buffer(
			graphics::buffers::buffer_usage::def,
			graphics::reflect::var_type::u16,
			indices->counter(), indices.get()))
			m_square.indices = res.get();
	

		if (auto res = driver->create_vertex_buffer(
			graphics::buffers::buffer_usage::def,
			input_layout,
			vertices->counter(), vertices.get()))
			m_square.vertices = res.get();
	}

	{//creating circle model
		//collections::vector<>;

		array<maths::float4> vertices = initializer_list<maths::float4>{
			{ -1.0f,1.0f,-1.0f,1.0f },
			{ 1.0f,1.0f,-1.0f,1.0f },
			{ 1.0f,-1.0f,-1.0f,1.0f },
			{ -1.0f,-1.0f,-1.0f,1.0f },
		};

		reflect::attribute_desc input_layout[] =
		{
			{ reflect::var_type::f32, reflect::var_class::vec4 }
		};

		array<word> indices = { 0,2,1,0,3,2 };

		if (auto res = driver->create_index_buffer(
			graphics::buffers::buffer_usage::def,
			graphics::reflect::var_type::u16,
			indices->counter(), indices.get()))
			m_circle.indices = res.get();


		if (auto res = driver->create_vertex_buffer(
			graphics::buffers::buffer_usage::def,
			input_layout,
			vertices->counter(), vertices.get()))
			m_circle.vertices = res.get();
	}
	return true;
}

bool d3d11_drawer::close()
{
	m_driver = null;
	return true;
}


drawing::ibrush_t d3d11_drawer::create_solid_brush(color_t diffuse, color_t additive)
{
	d3d11_solid_brush_t brush = new d3d11_solid_brush();
	if(!brush->create(this, diffuse, additive))
		return null;
	return brush.get();
}

drawing::ibrush_t d3d11_drawer::create_linear_gradient_brush(drawing::gradient_info_t gradients, color_t diffuse, color_t additive)
{
	d3d11_linear_gradient_brush_t brush = new d3d11_linear_gradient_brush();
	if (!brush->create(this, gradients, diffuse, additive))
		return null;
	return brush.get();
}

drawing::ibrush_t d3d11_drawer::create_texturing_brush(drawing::texturing_info_t texture, color_t diffuse, color_t additive)
{
	d3d11_texturing_brush_t brush = new d3d11_texturing_brush();
	if (!brush->create(this, texture, diffuse, additive))
		return null;
	return brush.get();
}

drawing::ibrush_t d3d11_drawer::create_linear_gradient_texturing_brush(drawing::gradient_info_t gradient, drawing::texturing_info_t texture, color_t diffuse, color_t additive)
{
	d3d11_linear_gradient_texturing_brush_t brush = new d3d11_linear_gradient_texturing_brush();
	if (!brush->create(this, gradient, texture, diffuse, additive))
		return null;
	return brush.get();
}

void d3d11_drawer::begin_draw(iframe_buffer_t frame)
{
	if (frame.is_empty())
		return;
	m_dimentions = frame->dimentions();

	driver()->bind_frame_buffer(frame);
	m_is_drawing = true;
}

void d3d11_drawer::end_draw()
{
	if (!is_drawing())
		return;
	m_is_drawing = false;
	driver()->bind_frame_buffer(null);
}

void d3d11_drawer::clear(color_t color)
{
	if (!is_drawing())
		return;

	driver()->clear(color);
}

void d3d11_drawer::draw_rect(drawing::ibrush_t brush, rect<float> rect)
{
	if (!is_drawing())
		return;
	maths::float2 origin = { (float)m_dimentions.width / 2 , (float)m_dimentions.height / 2 };
	maths::float2 scale = { rect.width / (float)m_dimentions.width, rect.height / (float)m_dimentions.height };
	maths::float2 pos = { rect.left + rect.width / 2, rect.top + rect.height / 2 };

	pos.set<0>(pos.get<0>() / origin.get<0>());
	pos.set<1>(-pos.get<1>() / origin.get<1>());
	pos -= {1, -1};

	maths::float4x4 trasnform = maths::matrix::scale(scale.get<0>(), scale.get<1>(), 0.01f)
		*  maths::matrix::translation(pos.get<0>(), pos.get<1>(), 0);

	static_cast<d3d11_brush*>(brush.get())->draw(driver().get(), trasnform, &m_square);
}

#endif
