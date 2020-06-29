#include "pch.h"
#include "test.h"

using namespace ang;

object3D::object3D()
	: m_position(0, 0, 0, 1)
	, m_rotation(0, 0, 0)
	, m_scale(1, 1, 1)
	, m_mesh(null)
	, m_shaders(null)
{

}

void object3D::dispose()
{
	m_shaders = null;
	m_mesh = null;
	m_scale = { 1, 1, 1 };
	m_position = { 0, 0, 0, 1 };
	m_rotation = { 0, 0, 0 };
}

bool object3D::load(graphics::gl_context_t context, string vshader, string fshader, string model, array<string> textures)
{
	graphics::gl_shaders_t shaders = new graphics::gl_shaders();
	shaders->init_async(context, vshader, fshader)->then<void>(
		[=](core::async::iasync_op<graphics::gl_shaders_t> task)
	{
		core::async::scope_locker<core::async::mutex> lock = m_mutex;
		try {
			auto result = task->result();
			if (result.successed())
				m_shaders = shaders;
			else {
				result.error();
			}
		}
		catch (...) {
		}
	});

	graphics::gl_mesh_t mesh = new graphics::gl_mesh();
	mesh->init_async(context, model)->then<void>(
		[=](core::async::iasync_op<graphics::gl_mesh_t> task)
	{
		core::async::scope_locker<core::async::mutex> lock = m_mutex;
		try {
			auto result = task->result();
			if (result.successed())
				m_mesh = mesh;
			else {
				result.error();
			}
		}
		catch (...) {
		}
	});

	for (auto const& path : textures) {
		graphics::gl_texture_t texture = new graphics::gl_texture();
		texture->init_async(context, path)->then<void>([=](core::async::iasync_op<graphics::gl_texture_t> task)
		{
			core::async::scope_locker<core::async::mutex> lock = m_mutex;
			try {
				auto result = task->result();
				if (result.successed())
					m_textures += texture;
				else {
					result.error();
				}
			}
			catch (...) {
			}
		});
	}

	return true;
}

void object3D::update(core::time::step_timer const& timer)
{
	
}

void object3D::draw(graphics::gl_context_t context, camera_t cam)
{
	core::async::scope_locker<core::async::mutex> lock = m_mutex;
	if (!m_shaders.is_empty() && !m_mesh.is_empty())
	{
		m_shaders->use_program();

		glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vertex_buffer());

		int att = -1;
		if ((att = m_shaders->attribute("position")) != -1)
		{
			glEnableVertexAttribArray(att);
			glVertexAttribPointer(att, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (pointer)offset_of(&vertex::position));
		}

		if ((att = m_shaders->attribute("normal")) != -1)
		{
			glEnableVertexAttribArray(att);
			glVertexAttribPointer(att, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (pointer)offset_of(&vertex::normal));
		}

		if ((att = m_shaders->attribute("binormal")) != -1)
		{
			glEnableVertexAttribArray(att);
			glVertexAttribPointer(att, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (pointer)offset_of(&vertex::binormal));
		}

		if ((att = m_shaders->attribute("tangent")) != -1)
		{
			glEnableVertexAttribArray(att);
			glVertexAttribPointer(att, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (pointer)offset_of(&vertex::tangent));
		}

		if ((att = m_shaders->attribute("texcoord")) != -1)
		{
			glEnableVertexAttribArray(att);
			glVertexAttribPointer(att, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (pointer)offset_of(&vertex::texcoord));
		}

		int uni = -1;
		maths::float4x4 wm = world_matrix();
		maths::float4x4 vm = cam->view_matrix();
		maths::float4x4 pm = cam->projection_matrix();
		
		if ((uni = m_shaders->uniform("wmatrix")) != -1)
		{
			glUniformMatrix4fv(uni, 1, GL_FALSE, (float*)&wm);
		}

		if ((uni = m_shaders->uniform("vmatrix")) != -1)
		{
			glUniformMatrix4fv(uni, 1, GL_FALSE, (float*)&vm);
		}

		if ((uni = m_shaders->uniform("pmatrix")) != -1)
		{
			glUniformMatrix4fv(uni, 1, GL_FALSE, (float*)&pm);
		}

		if ((uni = m_shaders->uniform("wvpmatrix")) != -1)
		{
			maths::float4x4 wvpm = wm * vm * pm;
			glUniformMatrix4fv(uni, 1, GL_FALSE, (float*)&wvpm);
		}

		uint textureUnit = 0;
		for (auto const& tex : m_textures)
		{
			// make active a texture unit
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			// bind the texture to the currently active texture unit
			glBindTexture(GL_TEXTURE_2D, tex->tex_buffer());

			switch (textureUnit)
			{
			case 0:
				glUniform1i(m_shaders->sampler("difuse_tex"_r), textureUnit);
			default:
				break;
			}
			textureUnit++;	
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh->index_buffer());
		glDrawElements(GL_TRIANGLES, m_mesh->index_count(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}


maths::float4x4 object3D::world_matrix()const
{
	return maths::matrix::scale(m_scale) *
		maths::matrix::rotation_z(m_rotation.get<0>()) *
		maths::matrix::rotation_x(m_rotation.get<0>()) *
		maths::matrix::rotation_y(m_rotation.get<0>()) *
		maths::matrix::translation((maths::float3)m_position);
}

