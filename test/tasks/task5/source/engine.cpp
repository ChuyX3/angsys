#include "pch.h"
#include "engine.h"
#include <ang/core/time.h>

using namespace d3d11;

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace d3d11;


struct vertex_data_t
{
	ang::maths::float3 position;
	ang::maths::float4 color;
};

//////////////////////////////////////////////////////////

void engine::init()
{
	effect_library = d3d_driver->create_effect_library();
	texture_loader = d3d_driver->create_texture_loader();
	
	core::files::input_text_file_t file = new core::files::input_text_file();
	file->open(L"resources/fx_library.xml"_s);
	xml::xml_document_t doc = new xml::xml_document(file);
	file->close();
	effect_library->load_library(doc->xml_root().get());

	file->open(L"resources/texture_apex.xml"_s);
	doc = new xml::xml_document(file);
	file->close();
	texture_loader->load_library(doc->xml_root().get());

	core::files::ifile_system* fs = core::files::ifile_system::get_file_system();
	fs->register_paths(L"..\\..\\..\\third_party\\resources\\"_s);

	core::files::input_text_file_t model_file = new core::files::input_text_file(L"Girl\\girl.obj"_s);
	//core::files::input_text_file_t model_file = new core::files::input_text_file(L"Blonde_nude\\Blonde_nude.obj"_s);

	model = new graphics::scenes::model();

	model->load(d3d_driver.get(), effect_library, texture_loader, model_file);

}

inline float XMConvertToRadians(float fDegrees) { return fDegrees * (3.141592f / 180.0f); }
inline float XMConvertToDegrees(float fRadians) { return fRadians * (180.0f / 3.141592f); }

static maths::matrix4 world;

void engine::update(StepTimer const& timer)
{
	model->update(timer.GetTotalSeconds(), timer.GetElapsedSeconds());
}

void engine::draw()
{
	float color[4] = { 0.1f,0.2f,.4f,1.0f };
	
	d3d_driver->clear(maths::float4{ 0.1f,0.2f,.4f,1.0f });

	model->draw(d3d_driver.get(), camera);

	d3d_surface->swap_buffers();
}

void engine::exit()
{
	d3d_driver->bind_shaders(null);
	d3d_driver->bind_frame_buffer(null);
	d3d_surface = null;
	effect_library = null;
	d3d_driver = null;
}

void engine::on_size_changed_event(objptr sender, platform::events::idisplay_info_event_args_t args)
{
	auto info = args->display_info();

	foundation::size<float> size = {
		info.display_resolution->width * info.display_scale_factor->width ,
		info.display_resolution->height * info.display_scale_factor->height };
	size.height = max(size.height, 10.0f);
	size.width = max(size.width, 10.0f);

	camera->projection(0.8f, size.width / size.height, 0.01f, 10000.0f);
}

bool pressed = false;
void engine::on_pointer_moved_event(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void engine::on_pointer_pressed_event(objptr sender, platform::events::ipointer_event_args_t args)
{

}

void engine::on_pointer_released_event(objptr sender, platform::events::ipointer_event_args_t args)
{
	

}

//////////////////////////////////////////////////////////

engine::engine()
{
	_is_running = false;
	cond = new core::async::cond();
	mutex = new core::async::mutex();
	camera = new graphics::scenes::camera();
}

engine::~engine()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(d3d11::engine, ang::object);

void engine::on_create_event(objptr caller, platform::events::icreated_event_args_t args)
{
	core_view = args->core_view();
	on_render_operation(caller, args);
}

void engine::on_destroy_event(objptr, platform::events::imsg_event_args_t)
{
	if (!async_render_operation.is_empty())
	{
		async_render_operation->cancel();
		async_render_operation->wait(core::async::async_action_status::completed, -1);
	}
	async_render_operation = null;
}

void engine::on_render_operation(objptr, platform::events::icreated_event_args_t)
{
	
	async_render_operation = core::async::async_task<void>::run_async(
		[this](core::async::iasync<void>* async, var_args_t args)
	{
		{//scope
			core::async::scope_locker lock = mutex;
			if (!init_driver())
				return;
			_is_running = true;
		}

		init();
		double last = core::time::get_performance_time();
		while (async->status().is_active(core::async::async_action_status::running))
		{
			_timer.Tick([&]()
			{
				update(_timer);
			});
			
			draw();
		}
		exit();

		{//scope
			core::async::scope_locker lock = mutex;
			close_driver();
		}
	});
	async_render_operation->wait(core::async::async_action_status::running, -1);
}


bool engine::init_driver()
{
	d3d_driver = new graphics::d3d11::d3d11_driver();
	foundation::size<float> size = core_view->get_core_view_size();

	auto _surface = d3d_driver->create_surface(core_view);
	d3d_surface = interface_cast<graphics::d3d11::d3d11_surface>(_surface.get());

	d3d_driver->bind_frame_buffer(d3d_surface->frame_buffer());

	camera->load({ 0,1.1f,0 }, { 0,0,0 }, { 0.8f, size.width / size.height, 0.01f, 10000.0f });

	return true;
}

void engine::close_driver()
{
	_is_running = false;
	d3d_surface = null;
	d3d_driver = null;
}