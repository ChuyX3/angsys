#include "pch.h"
#include "puyo.h"

using namespace ang;

static struct {
	maths::float4x4 wmatrix;
	maths::float4x4 vmatrix;
	maths::float4x4 pmatrix;
}wvp = {
	maths::matrix::identity(),
	maths::matrix::identity(),
	maths::matrix::identity()
};


puyo::puyo()
{

}

puyo::~puyo()
{

}

void puyo::on_initialize(objptr sender, platform::events::icreated_event_args_t args)
{
	game::on_initialize(sender, args);

	core::files::input_text_file_t file;
	library()->file_system()->open("./data/library.xml"_r, &file);
	auto doc = dom::xml::xml_document_from_file(file);
	if (doc.successed()) {
		library()->load(doc->root_element());
	}
	file = null;
	doc.reset();

	m_mesh = library()->find_resource("woman"_r)->cast<graphics::resources::resource_type::mesh>();

	//smart<float[]> vertices = {
	//	-0.1f, -0.1f, -0.1f, 1.0f,  0.0f, 0.0f, 0.0f, 1.0f,
	//	-0.1f, -0.1f,  0.1f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,
	//	-0.1f,  0.1f, -0.1f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,
	//	-0.1f,  0.1f,  0.1f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,
	//	 0.1f, -0.1f, -0.1f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	//	 0.1f, -0.1f,  0.1f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	//	 0.1f,  0.1f, -0.1f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
	//	 0.1f,  0.1f,  0.1f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
	//};

	//graphics::reflect::attribute_desc input_layout[] =
	//{
	//	{ graphics::reflect::var_type::f32, graphics::reflect::var_class::vec4 },
	//	{ graphics::reflect::var_type::f32, graphics::reflect::var_class::vec4 }
	//};

	//smart<ushort[]> indices = {
	//		2,1,0, // -x
	//		2,3,1,

	//		6,4,5, // +x
	//		6,5,7,

	//		0,1,5, // -y
	//		0,5,4,

	//		2,6,7, // +y
	//		2,7,3,

	//		0,4,6, // -z
	//		0,6,2,

	//		1,3,7, // +z
	//		1,7,5,
	//};

	//if (auto res = factory()->create_index_buffer(
	//	graphics::buffers::buffer_usage::def, indices)) {
	//	m_indices = res.get();
	//}

	//if (auto res = factory()->create_vertex_buffer(
	//	graphics::buffers::buffer_usage::def,
	//	input_layout,
	//	8, vertices.get())) {
	//	m_vertices = res.get();
	//}

	//m_driver->bind_index_buffer(m_indices);
	//m_driver->bind_vertex_buffer(m_vertices);

	auto res = library()->find_resource("character_fx:difuse"_r);
	m_shaders = res->cast<graphics::resources::resource_type::shaders>();
	auto size = args->core_view()->core_view_size();
	wvp.pmatrix = maths::matrix::perspective(0.9f, size.width / size.height, 0.001f, 1000.0f);

	m_driver->cull_mode(graphics::cull_mode::back);
	m_driver->blend_mode(graphics::blend_mode::enable);

}


void puyo::draw(graphics::scenes::itransform_t cam, graphics::iframe_buffer_t fbo)
{
	m_driver->bind_frame_buffer(fbo);
	m_driver->clear(graphics::colors::dark_gray);

	wvp.wmatrix = maths::matrix::scale(0.01f)
		//* maths::matrix::rotation_y(m_timer.total_time() / 1000000.0f)
		* maths::matrix::translation(0, 0.0f, -2.0f);

	m_driver->bind_shaders(m_shaders);
	if (cam == null)
	{
		auto var = m_shaders->map_vs_uniform(m_driver, 0);
		var[0].cast<maths::float4x4>() = maths::matrix::transpose(wvp.wmatrix);
		var[1][0].cast<maths::float4x4>() = maths::matrix::transpose(wvp.vmatrix * wvp.pmatrix);
		m_shaders->unmap_vs_uniform(m_driver, var);
	}
	else if(cam->is_stereo())
	{
		m_driver->viewport(cam->viewport());
		auto var = m_shaders->map_vs_uniform(m_driver, 0);
		var[0].cast<maths::float4x4>() = maths::matrix::transpose(wvp.wmatrix);
		var[1][0].cast<maths::float4x4>() = maths::matrix::transpose(cam->view_matrix(0) * cam->projection_matrix(0));
		var[1][1].cast<maths::float4x4>() = maths::matrix::transpose(cam->view_matrix(1) * cam->projection_matrix(1));
		m_shaders->unmap_vs_uniform(m_driver, var);
	}
	else
	{
		m_driver->viewport(cam->viewport());
		auto var = m_shaders->map_vs_uniform(m_driver, 0);
		var[0].cast<maths::float4x4>() = maths::matrix::transpose(wvp.wmatrix);
		var[1][0].cast<maths::float4x4>() = maths::matrix::transpose(cam->view_matrix(0) * cam->projection_matrix(0));
		m_shaders->unmap_vs_uniform(m_driver, var);
	}

	for (auto element : m_mesh->elements()) {
		
		windex c = 0;
		for (graphics::textures::itexture_t tex : element->material()->textures())
		{
			m_driver->bind_texture(tex, c);
			c++;
		}

		auto var = m_shaders->map_ps_uniform(m_driver, 0);
		
		m_shaders->unmap_ps_uniform(m_driver, var);

		if (element->index_buffer())
		{
			m_driver->bind_index_buffer(element->index_buffer());
			m_driver->bind_vertex_buffer(element->vertex_buffer());
			m_driver->draw_indexed(element->index_buffer()->counter(), graphics::primitive::triangle);
		}
		else
		{
			m_driver->bind_vertex_buffer(element->vertex_buffer());
			m_driver->draw(element->vertex_buffer()->block_counter(), graphics::primitive::triangle);
		}

		m_driver->bind_index_buffer(null);
		m_driver->bind_vertex_buffer(null);

		for (wsize i = 0; i < c; i++)
		{
			m_driver->bind_texture(null, i);
		}
	}

	m_driver->bind_frame_buffer(null);
}

void puyo::on_size_changed(objptr sender, platform::events::idisplay_info_event_args_t args)
{
	game::on_size_changed(sender, args);
	auto size = args->display_info().display_resolution;
	wvp.pmatrix = maths::matrix::perspective(0.9f, size.width / size.height, 0.001f, 1000.0f);
}