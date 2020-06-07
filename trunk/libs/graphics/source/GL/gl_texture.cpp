#include "pch.h"
#include "gl/gl_driver.h"

#if OPENGL_FAMILY_SUPPORT

using namespace coffe;
using namespace coffe::graphics;
using namespace coffe::graphics::gl;


gl_texture::gl_texture()
{
}

//COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::gl::gl_texture);
//COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::gl::gl_texture);
//COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::gl::gl_texture, system_object, textures::itexture, buffers::igpu_buffer, resources::iresource);

void gl_texture::dispose()
{
	clear();
}

void gl_texture::clear()
{
	if (m_gl_resource != 0)
		glDeleteTextures(1, &m_gl_resource);

	m_gl_resource = 0;
	m_resource_sid = null;
	m_bind_flags = buffers::buffer_bind_flag::none;
	m_usage = buffers::buffer_usage::def;
	m_tex_type = textures::tex_type::null;
	m_tex_format = textures::tex_format::null;
	m_tex_dimentions = { 0,0,0 };
}

resources::iresource_t gl_texture::resource()const { return const_cast<gl_texture*>(this); }

resources::resource_type_t gl_texture::resource_type()const { return resources::resource_type::texture; }

string gl_texture::resource_sid()const { return (cstr_t)m_resource_sid; }

void gl_texture::resource_sid(cstr_t sid) { m_resource_sid = sid; }

intfptr gl_texture::fast_cast(resources::resource_type_t type) {
	return type == resources::resource_type::texture ? static_cast<textures::itexture*>(this) : null;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif
