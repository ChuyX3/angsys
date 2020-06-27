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

	platform::input::icontroller_manager::instance()->controller_connected_event +=
		new platform::events::controller_status_change_event(this, &puyo::on_controller_connected);

	platform::input::icontroller_manager::instance()->controller_disconnected_event +=
		new platform::events::controller_status_change_event(this, &puyo::on_controller_disconnected);

	core::files::input_text_file_t file;
	library()->file_system()->open("./data/library.xml"_r, &file);
	auto doc = dom::xml::xml_document_from_file(file);
	if (doc.successed()) {
		library()->load(doc->root_element());
	}
	file = null;
	doc.reset();

	m_mesh = library()->find_resource("woman"_r)->cast<graphics::resources::resource_type::mesh>();

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
	m_driver->clear(graphics::colors::gray);

	wvp.wmatrix = maths::matrix::scale(1.0f)
		* maths::matrix::rotation_x(-maths::constants::pi / 2)
		* maths::matrix::rotation_y(m_timer.total_time() / 1000000.0f)
		* maths::matrix::translation(0, 0, -5.0f);

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
		if (!var.raw_data().is_empty())
		{
			auto mat = element->material()->fields();
			var[0].cast<maths::float4>() = mat[0].cast<maths::float3>();
			var[1].cast<maths::float4>() = mat[1].cast<maths::float3>();
			var[2].cast<maths::float4>() = mat[2].cast<maths::float3>();
			var[3].cast<uint>() = mat[3].cast<uint>();
			var[4].cast<float>() = mat[4].cast<float>();
			var[5].cast<float>() = mat[5].cast<float>();

			m_shaders->unmap_ps_uniform(m_driver, var);
		}
		
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

void puyo::on_controller_connected(objptr, platform::events::icontroller_status_args_t args)
{
	if (m_controller.is_empty()) {
		m_controller = args->controller();
		m_analog_event_token = m_controller->analog_input_change_event += 
			new platform::events::controller_analog_change_event(this, &puyo::on_controller_analog_event);
		m_digital_event_token = m_controller->digital_button_change_event +=
			new platform::events::controller_button_change_event(this, &puyo::on_controller_digital_event);
	}
}

void puyo::on_controller_disconnected(objptr, platform::events::icontroller_status_args_t args)
{
	if (!m_controller.is_empty() && m_controller->controller_id() == args->controller_id()) {
		m_controller->analog_input_change_event -= m_analog_event_token;
		m_controller->digital_button_change_event -= m_digital_event_token;
		m_controller = null;
	}
}

void puyo::on_controller_analog_event(objptr, platform::events::icontroller_analog_input_args_t args)
{
	switch (args->button())
	{
	case platform::input::controller_button::left_thumb:
		break;
	default:
		break;
	}
}

void puyo::on_controller_digital_event(objptr, platform::events::icontroller_digital_input_args_t args)
{
	switch (args->button())
	{
	case platform::input::controller_button::A:
		break;

	case platform::input::controller_button::B:
		break;

	case platform::input::controller_button::X:
		break;

	case platform::input::controller_button::Y:
		break;
	default:
		break;
	}

}