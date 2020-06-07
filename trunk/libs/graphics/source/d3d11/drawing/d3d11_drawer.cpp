#include "pch.h"
#include "d3d11/drawing/d3d11_drawer.h"

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

d3d11_drawer::d3d11_drawer(d3d11_driver_t driver)
	: m_is_drawing(false)
{
	create(driver);
}

d3d11_drawer::~d3d11_drawer()
{

}

void d3d11_drawer::dispose()
{
	close();
}

bool d3d11_drawer::create(d3d11_driver_t driver)
{
	if (driver.is_empty())
		return false;
	m_driver = driver;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampDesc.MaxAnisotropy = 1;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	driver->D3D11Device()->CreateSamplerState(&sampDesc, &m_samperStates[0]);

	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	driver->D3D11Device()->CreateSamplerState(&sampDesc, &m_samperStates[1]);

	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	driver->D3D11Device()->CreateSamplerState(&sampDesc, &m_samperStates[2]);

	reflect::varying;
	{//creating square model
		smart<maths::float4[]> vertices = {
			maths::float4{ -1.0f,1.0f,0.0f,1.0f },
			maths::float4{ 1.0f,1.0f,0.0f,1.0f },
			maths::float4{ 1.0f,-1.0f,0.0f,1.0f },
			maths::float4{ -1.0f,-1.0f,0.0f,1.0f },
		};

		reflect::attribute_desc input_layout[] =
		{
			{ reflect::var_type::f32, reflect::var_class::vec4 }
		};

		smart<ushort[]> indices = { 0,2,1,0,3,2 };
	
		if(auto res = driver->create_index_buffer(
			graphics::buffers::buffer_usage::def, indices))
			m_square.indices = res.get();
	

		if (auto res = driver->create_vertex_buffer(
			graphics::buffers::buffer_usage::def,
			input_layout,
			vertices->size(), vertices.get()))
			m_square.vertices = res.get();
	}

	{//creating circle model
		//collections::vector<>;

		smart<maths::float4[]> vertices = {
			maths::float4{ -1.0f,1.0f,0.0f,1.0f },
			maths::float4{ 1.0f,1.0f,0.0f,1.0f },
			maths::float4{ 1.0f,-1.0f,0.0f,1.0f },
			maths::float4{ -1.0f,-1.0f,0.0f,1.0f },
		};

		reflect::attribute_desc input_layout[] =
		{
			{ reflect::var_type::f32, reflect::var_class::vec4 }
		};

		smart<ushort[]> indices = { 0,2,1,0,3,2 };

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
	m_samperStates[0] = null;
	m_samperStates[1] = null;
	m_samperStates[2] = null;
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

drawing::ilinear_gradient_brush_t d3d11_drawer::create_linear_gradient_brush(drawing::linear_gradient_info_t gradients, color_t diffuse, color_t additive)
{
	d3d11_linear_gradient_brush_t brush = new d3d11_linear_gradient_brush();
	if (!brush->create(this, gradients, diffuse, additive))
		return null;
	return brush.get();
}

drawing::iradial_gradient_brush_t d3d11_drawer::create_radial_gradient_brush(drawing::radial_gradient_info_t gradients, color_t diffuse, color_t additive)
{
	return null;
	/*
	d3d11_radial_gradient_brush_t brush = new d3d11_radial_gradient_brush();
	if (!brush->create(this, gradients, diffuse, additive))
		return null;
	return brush.get();*/
}

drawing::itexturing_brush_t d3d11_drawer::create_texturing_brush(drawing::texturing_info_t texture, color_t diffuse, color_t additive)
{
	d3d11_texturing_brush_t brush = new d3d11_texturing_brush();
	if (!brush->create(this, texture, diffuse, additive))
		return null;
	return brush.get();
}

void d3d11_drawer::begin_draw(iframe_buffer_t frame)
{
	if (is_drawing() || frame.is_empty())
		return;
	m_dimentions = frame->dimentions();

	m_blend = driver()->blend_mode();
	driver()->blend_mode(blend_mode::enable);
	driver()->bind_frame_buffer(frame);
	m_is_drawing = true;
}

void d3d11_drawer::end_draw()
{
	if (!is_drawing())
		return;
	m_is_drawing = false;
	driver()->blend_mode(m_blend);
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
	d3d11_brush_t br;
	if(is_drawing() && (br = brush.as<d3d11_brush>()))
	{
		maths::float2 origin = { (float)m_dimentions.width / 2 , (float)m_dimentions.height / 2 };
		maths::float2 scale = { rect.width / (float)m_dimentions.width, rect.height / (float)m_dimentions.height };
		maths::float2 pos = { rect.left + rect.width / 2, rect.top + rect.height / 2 };

		pos.set<0>(pos.get<0>() / origin.get<0>());
		pos.set<1>(-pos.get<1>() / origin.get<1>());
		pos -= {1, -1};

		maths::float4x4 trasnform = maths::matrix::scale(1,1, 0.01f)
			*  maths::matrix::translation(0, 0, -0.1f);

		br->draw(this, trasnform, &m_square);
	}
}

#endif
