#include "pch.h"
#include "d3d11/drawing/draw_context.hpp"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

ANG_IMPLEMENT_ENUM(ang::graphics::textures, tex_stretch_mode, uint, tex_stretch_mode::none);

d3d11_draw_context::d3d11_draw_context(d3d11_driver_t driver, core::files::ifile_system_t fs)
	: _is_drawing(false)
{
	create(driver, fs);
}

d3d11_draw_context::~d3d11_draw_context()
{
	close();
}

ANG_IMPLEMENT_CLASSNAME(ang::graphics::d3d11::d3d11_draw_context);
ANG_IMPLEMENT_OBJECTNAME(ang::graphics::d3d11::d3d11_draw_context);

bool d3d11_draw_context::is_inherited_of(type_name_t name)
{
	return name == type_of<d3d11_draw_context>()
		|| object::is_inherited_of(name)
		|| drawing::idraw_context::is_inherited_of(name);
}

bool d3d11_draw_context::is_kind_of(type_name_t name)const
{
	return name == type_of<d3d11_draw_context>()
		|| object::is_kind_of(name)
		|| drawing::idraw_context::is_kind_of(name);
}

bool d3d11_draw_context::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<d3d11_draw_context>())
	{
		*out = static_cast<d3d11_draw_context*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (drawing::idraw_context::query_object(name, out))
	{
		return true;
	}
	return false;
}


bool d3d11_draw_context::create(d3d11_driver_t driver, core::files::ifile_system_t fs)
{
	if (driver.is_empty())
		return false;
	_driver = driver;
	_effect_library = driver->create_effect_library();
	effect_library()->set_file_system(fs);

	reflect::variable;
	{//creating square model
		maths::float4 vertices[] = {
			{ -1.0f,1.0f,-1.0f,1.0f },
			{ 1.0f,1.0f,-1.0f,1.0f },
			{ 1.0f,-1.0f,-1.0f,1.0f },
			{ -1.0f,-1.0f,-1.0f,1.0f },
		};

		reflect::attribute_desc input_layout[] =
		{
			{ reflect::var_type::f32, reflect::var_class::vec4 }
		};

		word indices[] = { 0,2,1,0,3,2 };

		_square.indices = driver->create_index_buffer(
			graphics::buffers::buffer_usage::def,
			graphics::reflect::var_type::u16,
			array_size(indices), collections::to_array((byte*)indices, sizeof(indices))
		);

		_square.vertices = driver->create_vertex_buffer(
			graphics::buffers::buffer_usage::def,
			input_layout,
			array_size(vertices), collections::to_array((byte*)vertices, sizeof(vertices))
		);
	}

	{//creating circle model
		//collections::vector<>;


		maths::float4 vertices[] = {
			{ -1.0f,1.0f,-1.0f,1.0f },
			{ 1.0f,1.0f,-1.0f,1.0f },
			{ 1.0f,-1.0f,-1.0f,1.0f },
			{ -1.0f,-1.0f,-1.0f,1.0f },
		};

		reflect::attribute_desc input_layout[] =
		{
			{ reflect::var_type::f32, reflect::var_class::vec4 }
		};

		word indices[] = { 0,2,1,0,3,2 };

		_circle.indices = driver->create_index_buffer(
			graphics::buffers::buffer_usage::def,
			graphics::reflect::var_type::u16,
			array_size(indices), collections::to_array((byte*)indices, sizeof(indices))
		);

		_circle.vertices = driver->create_vertex_buffer(
			graphics::buffers::buffer_usage::def,
			input_layout,
			array_size(vertices), collections::to_array((byte*)vertices, sizeof(vertices))
		);
	}
	return true;
}

bool d3d11_draw_context::close()
{
	_effect_library = null;
	//texture_loader = null;
	_driver = null;
	return true;
}


drawing::ibrush_t d3d11_draw_context::create_solid_brush(color_t diffuse, color_t additive)
{
	d3d11_solid_brush_t brush = new d3d11_solid_brush();
	if(!brush->create(this, diffuse, additive))
		return null;
	return brush.get();
}

drawing::ibrush_t d3d11_draw_context::create_linear_gradient_brush(drawing::gradient_info_t gradients, color_t diffuse, color_t additive)
{
	d3d11_linear_gradient_brush_t brush = new d3d11_linear_gradient_brush();
	if (!brush->create(this, gradients, diffuse, additive))
		return null;
	return brush.get();
}

drawing::ibrush_t d3d11_draw_context::create_texturing_brush(drawing::texturing_info_t texture, color_t diffuse, color_t additive)
{
	d3d11_texturing_brush_t brush = new d3d11_texturing_brush();
	if (!brush->create(this, texture, diffuse, additive))
		return null;
	return brush.get();
}

drawing::ibrush_t d3d11_draw_context::create_linear_gradient_texturing_brush(drawing::gradient_info_t gradient, drawing::texturing_info_t texture, color_t diffuse, color_t additive)
{
	d3d11_linear_gradient_texturing_brush_t brush = new d3d11_linear_gradient_texturing_brush();
	if (!brush->create(this, gradient, texture, diffuse, additive))
		return null;
	return brush.get();
}

void d3d11_draw_context::begin_draw(iframe_buffer_t frame)
{
	if (frame.is_empty())
		return;
	_dimentions = frame->dimentions();

	driver()->bind_frame_buffer(frame);
	_is_drawing = true;
}

void d3d11_draw_context::end_draw()
{
	if (!is_drawing())
		return;
	_is_drawing = false;
	driver()->bind_frame_buffer(null);
}

void d3d11_draw_context::clear(color_t color)
{
	if (!is_drawing())
		return;

	driver()->clear(color);
}

void d3d11_draw_context::draw_rect(drawing::ibrush_t brush, rect<float> rect)
{
	if (!is_drawing())
		return;
	maths::float2 origin = { _dimentions.width / 2 , _dimentions.height / 2 };
	maths::float2 scale = { rect.width() / _dimentions.width, rect.height() / _dimentions.height };
	maths::float2 pos = { rect.left + rect.width() / 2, rect.top + rect.height() / 2 };

	pos.set<0>(pos.get<0>() / origin.get<0>());
	pos.set<1>(-pos.get<1>() / origin.get<1>());
	pos -= {1, -1};

	maths::float4x4 trasnform = maths::matrix::scale(scale.get<0>(), scale.get<1>(), 0.01f)
		*  maths::matrix::translation(pos.get<0>(), pos.get<1>(), 0);

	static_cast<d3d11_brush*>(brush.get())->draw(driver().get(), trasnform, &_square);
}

#endif
