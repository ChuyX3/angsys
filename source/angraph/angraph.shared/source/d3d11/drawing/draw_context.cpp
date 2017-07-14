#include "pch.h"
#include "d3d11/drawing/draw_context.h"

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;

static cwstr_t library = L".\\drawing\\effects\\drawing_eefetc_library.xml"_s;









d3d11_draw_context::d3d11_draw_context(d3d11_driver_t driver, core::files::ifile_system_t fs)
	: is_drawing(false)
{
	create(driver, fs);
}

d3d11_draw_context::~d3d11_draw_context()
{
	close();
}


bool d3d11_draw_context::create(d3d11_driver_t driver, core::files::ifile_system_t fs)
{
	if (driver.is_empty())
		return false;
	this->driver = driver;
	effect_library = driver->create_effect_library();
	//texture_loader = driver->create_texture_loader();

	effect_library->set_file_system(fs);
	//texture_loader->set_file_system(fs);

	core::files::input_text_file_t file;
	fs->open(L".\\drawing\\effects\\drawing_eefetc_library.xml"_s, file);
	xml::xml_document_t doc;
	try{
		doc = new xml::xml_document(file);
	}
	catch (...)
	{
		close();
		return false;
	}

	if (!effect_library->load_library(doc->xml_root().get()))
	{
		close();
		return false;
	}

	return true;
}

bool d3d11_draw_context::close()
{
	effect_library = null;
	//texture_loader = null;
	driver = null;
	return true;
}


drawing::ibrush_t d3d11_draw_context::create_solid_brush(color_t)
{
	return null;
}

drawing::ibrush_t d3d11_draw_context::create_texturing_brush(textures::tex_wrap_mode_t, textures::itexture_t)
{
	return null;
}

void d3d11_draw_context::begin_draw(iframe_buffer_t frame)
{
	if (frame.is_empty())
		return;
	driver->bind_frame_buffer(frame);
	is_drawing = true;
}

void d3d11_draw_context::end_draw()
{
	if (!is_drawing)
		return;
	is_drawing = false;
	driver->bind_frame_buffer(null);
}

void d3d11_draw_context::clear(color_t color)
{
	if (!is_drawing)
		return;

	driver->clear(color);
}

#endif
