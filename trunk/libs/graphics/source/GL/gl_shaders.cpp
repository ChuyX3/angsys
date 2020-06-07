#include "pch.h"
#include "gl/gl_driver.h"

#if OPENGL_FAMILY_SUPPORT

using namespace coffe;
using namespace coffe::graphics;
using namespace coffe::graphics::gl;

//none = 0,
//s8,
//u8,
//s16,
//u16,
//s32,
//u32,
//f32,

GLenum s_gl_types[] = {
	0,
	GL_BYTE,
	GL_UNSIGNED_BYTE,
	GL_SHORT,
	GL_UNSIGNED_SHORT,
	GL_INT,
	GL_UNSIGNED_INT,
	GL_FLOAT,
};

inline GLenum var_type_to_gl_type(reflect::var_type_t type) {
	return (uint)type.get() >= algorithms::array_size(s_gl_types) ? 0 : s_gl_types[(uint)type.get()];
}

/////////////////////////////////////////////////////////////////////////

gl_shaders::gl_shaders()
{
}

gl_shaders::~gl_shaders()
{
}

//COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(coffe::graphics::gl::gl_shaders);
//COFFE_IMPLEMENT_OBJECT_CLASS_INFO(coffe::graphics::gl::gl_shaders);
//COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(coffe::graphics::gl::gl_shaders, system_object, effects::ishaders, resources::iresource);

void gl_shaders::dispose()
{
	close();
}

bool gl_shaders::close()
{
	
	return true;
}

resources::iresource_t gl_shaders::resource()const { return const_cast<gl_shaders*>(this); }

resources::resource_type_t gl_shaders::resource_type()const { return resources::resource_type::shaders; }

string gl_shaders::resource_sid()const { return (cstr_t)m_resource_sid; }

void gl_shaders::resource_sid(cstr_t sid) { m_resource_sid = sid; }

intfptr gl_shaders::fast_cast(resources::resource_type_t type) {
	return type == resources::resource_type::shaders ? static_cast<effects::ishaders*>(this) : null;
}

array_view<reflect::attribute_desc> gl_shaders::input_layout()const
{
	return m_input_layout;
}

array_view<reflect::varying_desc> gl_shaders::vs_uniforms_layouts()const
{
	return m_vs_uniforms;
}

array_view<reflect::varying_desc> gl_shaders::ps_uniforms_layouts()const
{
	return m_ps_uniforms;
}

bool gl_shaders::bind_vertex_buffer(idriver_t driver, buffers::ivertex_buffer_t buffer)
{
	auto attributes = buffer->descriptor();
	reflect::attribute_desc::calculate_positions(attributes);
	auto stride = reflect::attribute_desc::get_size_in_bytes(attributes);

	driver_locker_t lock = driver;
	for (reflect::attribute_desc_t att : attributes)
	{
		try {
			auto attribloc = m_attribute_map[(cstr_t)att.var_name()];
			if (attribloc != -1) {
				glEnableVertexAttribArray(attribloc);
				glVertexAttribPointer(attribloc, (int)att.var_class().get(), var_type_to_gl_type(att.var_type()), GL_FALSE, stride, (pointer)att.position());
			}
		}
		catch (...) {
			continue;
		}
	}
	return true;
}

bool gl_shaders::bind_texture(idriver_t driver, windex idx, windex idx2)
{

}

bool gl_shaders::bind_texture(idriver_t, cstr_t, windex)
{

}

reflect::varying gl_shaders::map_vs_uniform(idriver_t, windex)
{

}

reflect::varying gl_shaders::map_vs_uniform(idriver_t, cstr_t)
{

}

reflect::varying gl_shaders::map_ps_uniform(idriver_t, windex)
{

}

reflect::varying gl_shaders::map_ps_uniform(idriver_t, cstr_t)
{

}

void gl_shaders::unmap_vs_uniform(idriver_t driver, reflect::varying&)
{

}

void gl_shaders::unmap_ps_uniform(idriver_t driver, reflect::varying&)
{

}


#endif
