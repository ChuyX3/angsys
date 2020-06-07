#include "pch.h"
#include "gl/gl_driver.h"

#if OPENGL_FAMILY_SUPPORT

using namespace coffe;
using namespace coffe::graphics;
using namespace coffe::graphics::gl;

gl_index_buffer::gl_index_buffer()
	: m_index_count(0)
{

}

//COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::gl::gl_index_buffer);
//COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::gl::gl_index_buffer);
//COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::gl::gl_index_buffer, system_object, buffers::iindex_buffer, buffers::igpu_buffer, resources::iresource);

void gl_index_buffer::dispose()
{
	close();
}

resources::iresource_t gl_index_buffer::resource()const { return const_cast<gl_index_buffer*>(this); }

resources::resource_type_t gl_index_buffer::resource_type()const { return resources::resource_type::index_buffer; }

string gl_index_buffer::resource_sid()const { return (cstr_t)m_resource_sid; }

void gl_index_buffer::resource_sid(cstr_t sid) { m_resource_sid = sid; }

intfptr gl_index_buffer::fast_cast(resources::resource_type_t type) {
	return type == resources::resource_type::index_buffer ? static_cast<buffers::iindex_buffer*>(this) : null;
}

buffers::buffer_type_t gl_index_buffer::buffer_type()const { return buffers::buffer_type::index_buffer; }

buffers::buffer_usage_t gl_index_buffer::buffer_usage()const { return m_usage; }

buffers::buffer_bind_flag_t gl_index_buffer::buffer_bind_flag()const { return buffers::buffer_bind_flag::index_buffer; }

ibuffer_t gl_index_buffer::map(idriver_t, bool writeMode)
{
	return null;
}

bool gl_index_buffer::unmap(idriver_t, ibuffer_t)
{
	return false;
}

reflect::var_type_t gl_index_buffer::data_type()const { return m_index_type; }

wsize gl_index_buffer::counter()const { return m_index_count; }

wsize gl_index_buffer::size_in_bytes()const
{
	reflect::varying_desc desc(m_index_type, reflect::var_class::scalar, null, m_index_count);
	return desc.get_size_in_bytes();
}

bool gl_index_buffer::create(gl_driver_t driver, buffers::buffer_usage_t usage, reflect::var_type_t type, wsize count, array_view<byte> init_data, string sid)
{
	if ((int)type.get() < 3 || (int)type.get() > 6)
		return false;

	m_index_type = type;
	m_index_count = count;
	m_usage = usage;// buffers::buffer_usage::dynamic;

	reflect::varying_desc desc(type, reflect::var_class::scalar, null, count);

	auto byte_width = desc.get_size_in_bytes();
	
	if (init_data.size() < byte_width)
		return false;

	glGenBuffers(1, &m_index_buffer);
	if (m_index_buffer == 0U)
		return false;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

	GLenum glUsage;
	switch (usage)
	{
	case buffers::buffer_usage::dynamic:
		glUsage = GL_DYNAMIC_DRAW;
		break;
	case buffers::buffer_usage::inmutable:
		glUsage = GL_STREAM_DRAW;
		break;
	default:
		glUsage = GL_STATIC_DRAW;
		break;
	}

#if OPENGL_SUPPORT
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_width, init_data.data(), glUsage);
#else
	if (driver->max_version() > 2)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_width, init_data.data(), glUsage);
	else
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_width, init_data.data(), GL_STATIC_DRAW);
#endif

	
	m_resource_sid = sid;
	return true;
}

bool gl_index_buffer::close()
{
	m_index_count = 0;
	m_usage = buffers::buffer_usage::def;
	m_index_type = reflect::var_type::none;
	if (m_index_buffer != 0U)
		glDeleteBuffers(1, &m_index_buffer);
	m_index_buffer = 0;
	return true;
}

void gl_index_buffer::use_buffer(gl_driver_t)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);	
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


gl_vertex_buffer::gl_vertex_buffer()
	: m_vertex_count(0)
	, m_stride(0)
{

}

//COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::gl::gl_vertex_buffer);
//COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::gl::gl_vertex_buffer);
//COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::gl::gl_vertex_buffer, system_object, buffers::ivertex_buffer, buffers::igpu_buffer, resources::iresource);

void gl_vertex_buffer::dispose()
{
	close();
}

resources::iresource_t gl_vertex_buffer::resource()const { return const_cast<gl_vertex_buffer*>(this); }

resources::resource_type_t gl_vertex_buffer::resource_type()const { return resources::resource_type::vertex_buffer; }

string gl_vertex_buffer::resource_sid()const { return (cstr_t)m_resource_sid; }

void gl_vertex_buffer::resource_sid(cstr_t sid) { m_resource_sid = sid; }

intfptr gl_vertex_buffer::fast_cast(resources::resource_type_t type) {
	return type == resources::resource_type::vertex_buffer ? static_cast<buffers::ivertex_buffer*>(this) : null;
}

buffers::buffer_type_t gl_vertex_buffer::buffer_type()const { return buffers::buffer_type::index_buffer; }

buffers::buffer_usage_t gl_vertex_buffer::buffer_usage()const { return m_usage; }

buffers::buffer_bind_flag_t gl_vertex_buffer::buffer_bind_flag()const { return buffers::buffer_bind_flag::index_buffer; }

ibuffer_t gl_vertex_buffer::map(idriver_t, bool writeMode)
{
	return null;
}

bool gl_vertex_buffer::unmap(idriver_t, ibuffer_t)
{
	return false;
}

array_view<reflect::attribute_desc> gl_vertex_buffer::descriptor()const
{
	if (m_vertex_desc.is_empty())
		return array_view<reflect::attribute_desc>(null);
	return collections::to_array(m_vertex_desc->data(), m_vertex_desc->size());
}

wsize gl_vertex_buffer::block_counter()const { return m_vertex_count; }

wsize gl_vertex_buffer::size_in_bytes()const { return m_stride * m_vertex_count; }

bool gl_vertex_buffer::create(gl_driver_t driver, buffers::buffer_usage_t usage, array_view<reflect::attribute_desc> vertex_desc, wsize count, array_view<byte> init_data, string sid)
{
	if (vertex_desc.size() == 0)
		return false;

	m_vertex_desc = vertex_desc;
	m_vertex_count = count;
	m_usage = usage;// buffers::buffer_usage::dynamic;

	auto byte_width = reflect::attribute_desc::get_size_in_bytes(m_vertex_desc) * m_vertex_count;

	if (init_data.size() < byte_width)
		return false;

	glGenBuffers(1, &m_vertex_buffer);
	if (m_vertex_buffer == 0U)
		return false;

	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

	GLenum glUsage;
	switch (usage)
	{
	case buffers::buffer_usage::dynamic:
		glUsage = GL_DYNAMIC_DRAW;
		break;
	case buffers::buffer_usage::inmutable:
		glUsage = GL_STREAM_DRAW;
		break;
	default:
		glUsage = GL_STATIC_DRAW;
		break;
	}


#if OPENGL_SUPPORT
	glBufferData(GL_ARRAY_BUFFER, byte_width, init_data.data(), glUsage);
#else
	if (driver->max_version() > 2)
		glBufferData(GL_ARRAY_BUFFER, byte_width, init_data.data(), glUsage);
	else
		glBufferData(GL_ARRAY_BUFFER, byte_width, init_data.data(), GL_STATIC_DRAW);
#endif


	m_resource_sid = sid;
	return true;
}

bool gl_vertex_buffer::close()
{
	m_resource_sid = null;
	m_vertex_count = 0;
	m_stride = 0;
	m_usage = buffers::buffer_usage::def;
	m_vertex_desc = null;
	if (m_vertex_buffer != 0U)
		glDeleteBuffers(1, &m_vertex_buffer);
	m_vertex_buffer = 0;
	return true;
}

void gl_vertex_buffer::use_buffer(gl_driver_t)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertex_buffer);
}

#endif
