#include "pch.h"
#include "gl/gl_driver.h"

#if OPENGL_FAMILY_SUPPORT

using namespace coffe;
using namespace coffe::graphics;
using namespace coffe::graphics::gl;

gl_frame_buffer::gl_frame_buffer(gl_driver* driver)
	: m_parent_driver(driver)
{

}

//COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::gl::gl_frame_buffer);
//COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::gl::gl_frame_buffer);
//COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::gl::gl_frame_buffer, system_object, iframe_buffer, resources::iresource);

void gl_frame_buffer::dispose()
{
	close();
}

resources::iresource_t gl_frame_buffer::resource()const { return const_cast<gl_frame_buffer*>(this); }

resources::resource_type_t gl_frame_buffer::resource_type()const { return resources::resource_type::frame_buffer; }

string gl_frame_buffer::resource_sid()const { return (cstr_t)m_resource_sid; }

void gl_frame_buffer::resource_sid(cstr_t sid) { m_resource_sid = sid; }

intfptr gl_frame_buffer::fast_cast(resources::resource_type_t type) {
	return type == resources::resource_type::frame_buffer ? static_cast<iframe_buffer*>(this) : null;
}

bool gl_frame_buffer::create(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<uint> dimentions, string sid)
{
	if (color_format.size() == 0)
		return false;

	gl_driver_t driver = m_parent_driver.lock();

	if (driver.is_empty())
		return false;

	return true;
}

#endif
