// entry_point.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ang/platform/angwin/angwin.h>
#include <ang/collections/map.h>
#include <ang/core/time.h>

using namespace ang;

namespace ang
{
	namespace script
	{
		objptr ang_object_name(object* obj, var_args_t) {
			return obj ? obj->object_name() : null;
		}
		objptr ang_is_kind_of(object* obj, var_args_t args) {
			string name = args[0]->as<string>();
			return obj ? obj->is_kind_of(name) : false;
		}
		objptr ang_query_object(object* obj, var_args_t args) {
			string name = args[0]->as<string>();
			unknown_t& out = *args[1]->as<ang::value_wrapper<pointer>>();
			return obj ? obj->query_object(name, &out) : false;
		}



		typedef core::delegates::function<objptr(var_args_t)> method;
		typedef core::delegates::function_data<objptr(var_args_t)> method_object;

		class aobject : public objptr
		{
			collections::map<string, objptr> member_fields;
			collections::map<string, method> member_methods;
		public:
			aobject() {
				member_methods += {"object_name"_s, method(get(), ang_object_name)};
				member_methods += {"is_kind_of"_s, method(get(), ang_is_kind_of)};
				member_methods += {"query_object"_s, method(get(), ang_query_object)};
			}


			~aobject();
		};
	}
}

namespace app
{
	using namespace ang;
	class aobject;
	typedef object_wrapper<aobject> aobject_t;

	class aobject : public object
	{
		collections::map<string, objptr> member_fields;
		collections::map<string, core::delegates::function<objptr(var_args_t)>> member_methods;

	public:
		aobject() {
			member_fields = new collections::map_data<string, objptr>();
			member_methods = new collections::map_data<string, core::delegates::function<objptr(var_args_t)>>();
		}

		objptr get_value(string key)const {
			auto it = member_fields->find(key);
			return it.is_valid() ? it->value() : null;
		}
		void set_value(string key, objptr value) {
			auto it = member_fields->find(key);
			if (it.is_valid())	
				it->value(ang::move(value));
		}
		objptr invoke(string key, var_args_t args) {
			auto it = member_methods->find(key);
			if (it.is_valid())
				return it->value().invoke(ang::move(args));
			return null;
		}

	private:
		objptr _as_object_name(var_args_t) {
			return object_name();
		}

	protected:
		virtual~aobject() {

		}
	};





	class mainwnd : public platform::windows::window
	{
		core::async::dispatcher_thread_t render_thread;

	public:
		mainwnd() {
			render_thread = new core::async::dispatcher_thread();
			createdEvent += new platform::events::created_event(this, &mainwnd::on_created_event);
			destroyedEvent += new platform::events::destroyed_event(this, &mainwnd::on_destroyed_event);
		}

	protected:
		virtual~mainwnd() {}

	private:
		void on_created_event(objptr sender, platform::events::icreated_event_args_t args) {


			aobject_t obj = new aobject();
			


			render_thread->start(core::async::thread_callback_t(this, &mainwnd::on_rendering), null);
		}
		void on_destroyed_event(objptr sender, platform::events::imsg_event_args_t args) {
			render_thread->cancel();
			render_thread->wait(core::async::async_action_status::completed, -1);
		}

		dword on_rendering(pointer args) {

			Sleep(1);
			return 0;
		}
	};

	typedef object_wrapper<mainwnd> mainwnd_t;

	class mainapp : public platform::windows::app
	{
		mainwnd_t _mainwnd;
	public:
		mainapp() {
			_mainwnd = new mainwnd();
		}

		virtual bool init_app(array<string> cmdl)override {
			app::init_app(cmdl);

			string wndclass = platform::windows::window::regist_wnd_class(new platform::windows::class_regist_args{
				"mainwnd_class"_s,
				platform::windows::class_style::own_dc + platform::windows::class_style::save_bits
			});

			_mainwnd->create(new platform::windows::wnd_create_args{ wndclass , "main wnd" });
			set_main_wnd(_mainwnd.get());
			_mainwnd->update_wnd();
			_mainwnd->show(platform::windows::showing_flag::show);
			return true;
		}

		virtual bool exit_app()override {
			return app::exit_app();
		}

	};

	typedef object_wrapper<mainapp> mainapp_t;
}


struct vertex
{
	maths::float4 position;
	maths::float4 normal;
	maths::float2 texcoord;

	bool operator == (const vertex&) { return false; }//dummy
	bool operator != (const vertex&) { return false; }//dummy
};

ANG_REGISTER_RUNTIME_TYPENAME(vertex);


struct model_element
{
	string material;
	collections::vector<vertex> vertices;

	bool operator == (const model_element&) { return false; }//dummy
	bool operator != (const model_element&) { return false; }//dummy
};

ANG_REGISTER_RUNTIME_TYPENAME(model_element);


collections::vector<model_element> load_model(cwstr_t path);

class ordered_model;
typedef ang::object_wrapper<ordered_model> ordered_model_t;
class ordered_model
	: public object
{
private:
	collections::vector<collections::pair<string, collections::vector<vertex>>> _elements;

public:
	ordered_model();

	//ANG_DECLARE_INTERFACE();

	bool load(core::files::input_text_file_t);
	core::async::iasync_t<bool> load_async(core::files::input_text_file_t);
	bool load(core::files::input_binary_file_t);
	bool save(core::files::output_binary_file_t);

private:
	virtual~ordered_model();
};

cstr_t xmltext = "<uniforms name = \"wvp_matrix\">"
"<var type=\"f32\" class=\"mat4\" name=\"world\" />"
"<var type=\"f32\" class=\"mat4\" name=\"view\" />"
"<var type=\"f32\" class=\"mat4\" name=\"proj\" />"
"</uniforms>";


struct wvp_data_t
{
	maths::float4x4 world;
	maths::float4x4 view;
	maths::float4x4 proj;
};

int main()
{
	//core::files::ifile_system* fs = core::files::ifile_system::get_file_system();

	//fs->register_paths(L"C:\\Users\\jesus.rocha\\Documents\\Blender\\KioMiku_blender\\"_s);

	//fs->register_paths(L"C:\\Users\\Jesus Angel\\Desktop\\Girl\\"_s);
	//fs->register_paths(L"C:\\Users\\jesus.rocha\\Desktop\\Girl\\"_s);
	////C:\Users\jesus.rocha\Documents\Blender\KioMiku_blender	
	//ang::core::time::timer time;
	//time.reset();
	//core::files::input_text_file_t file = new core::files::input_text_file(L"Miku_1_4.obj"_s);
	//ordered_model_t model = new ordered_model();
	//auto res = model->load(file);
	//time.update();
	////media::isound_engine::create_sound_engine();

	wvp_data_t data;
	xml::xml_document_t doc = new xml::xml_document();
	graphics::reflect::variable_desc desc;
	doc->parse(xmltext);
	
	desc.load(doc->xml_root().get(), 16);

	graphics::reflect::variable var(const_array<byte>((byte*)&data, sizeof(data)), desc);


	auto  var1 = var[0];
	auto  var2 = var[1];
	auto  var3 = var[2];


	app::mainapp_t _app = new app::mainapp();
    return _app->run();
}

ordered_model::ordered_model()
{

}

ordered_model::~ordered_model()
{

}

bool ordered_model::load(core::files::input_text_file_t file)
{
	_elements = null;
	try
	{
		return file->read([&](streams::itext_input_stream_t stream)->bool
		{
			ibuffer_t _buffer = interface_cast<ibuffer>(stream.get());
			cstr_t data;
			if (_buffer.is_empty())
				return false;
			data.set((char*)_buffer->buffer_ptr(), _buffer->buffer_size() - 1);

			index idx = 0;
			maths::unaligned_float3 temp1;

			cstr_t v_s = "v ";
			cstr_t vt_s = "vt ";
			cstr_t vn_s = "vn ";
			cstr_t f_s = "f ";
			cstr_t mtllib_s = "mtllib ";
			cstr_t usemtl_s = "usemtl ";

			string material;
			collections::pair<string, collections::vector<vertex>> model;
			model.value() = new collections::vector_buffer<vertex>();
			model.value()->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));

			collections::vector<maths::unaligned_float3> vertices;
			collections::vector<maths::unaligned_float3> normals;
			collections::vector<maths::unaligned_float2> texcoords;

			collections::vector<model_element> out = new collections::vector_buffer<model_element>();

			cstr_t debug_view;

			while (data.size() > idx)
			{
				//debug_view.set(&data[idx], data.size() - idx);
				if (data.find(v_s, idx, idx + v_s.size()) == idx)
				{
					stream->move_to(idx + v_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y >> temp1._vector.z;
					vertices += temp1;
				}
				else if (data.find(vn_s, idx, idx + vn_s.size()) == idx)
				{
					stream->move_to(idx + vn_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y >> temp1._vector.z;
					normals += temp1;
				}
				else if (data.find(vt_s, idx, idx + vt_s.size()) == idx)
				{
					stream->move_to(idx + vt_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y;
					texcoords += {temp1._vector.x, temp1._vector.y};
				}
				else if (data.find(f_s, idx, idx + f_s.size()) == idx)
				{
					stream->move_to(idx + f_s.size(), streams::stream_reference::begin);
					index face[3] = { 0,0,0 };
					vertex vertex;
					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 };
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 };
					}
					model.value() += vertex;

					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 };
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 };
					}
					model.value() += vertex;

					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 };
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 };
					}
					model.value() += vertex;

				}
				else if (data.find(usemtl_s, idx, idx + usemtl_s.size()) == idx)
				{
					if (!material.is_empty())
					{
						model.key_value(ang::move(material));
						_elements += ang::move(model);
						model.value() = new collections::vector_buffer<vertex>();
						model.value()->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));
					}

					auto end = data.find('\n', idx);
					if (end == invalid_index)
						break; //error

					data.sub_string(material, idx + usemtl_s.size(), end - idx - usemtl_s.size());
				}

				idx = data.find('\n', idx);
				if (idx != invalid_index)idx++;
			}
			if (!material.is_empty())
			{
				model.key_value(ang::move(material));
				_elements += ang::move(model);
				model.value() = new collections::vector_buffer<vertex>();
				model.value()->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));
			}
			return true;
		});
	}
	catch (exception_t e)
	{
		return false;
	}
}

bool ordered_model::load(core::files::input_binary_file_t file)
{
	return false;
}

bool ordered_model::save(core::files::output_binary_file_t file)
{
	return false;
}


core::async::iasync_t<bool> ordered_model::load_async(core::files::input_text_file_t file)
{
	_elements = null;
	try
	{
		return file->read_async<bool>([&](streams::itext_input_stream_t stream)->bool
		{
			ibuffer_t _buffer = interface_cast<ibuffer>(stream.get());
			cstr_t data;
			if (_buffer.is_empty())
				return false;
			data.set((char*)_buffer->buffer_ptr(), _buffer->buffer_size() - 1);

			index idx = 0;
			maths::unaligned_float3 temp1;

			cstr_t v_s = "v ";
			cstr_t vt_s = "vt ";
			cstr_t vn_s = "vn ";
			cstr_t f_s = "f ";
			cstr_t mtllib_s = "mtllib ";
			cstr_t usemtl_s = "usemtl ";

			string material;
			collections::pair<string, collections::vector<vertex>> model;
			model.value() = new collections::vector_buffer<vertex>();
			model.value()->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));

			collections::vector<maths::unaligned_float3> vertices;
			collections::vector<maths::unaligned_float3> normals;
			collections::vector<maths::unaligned_float2> texcoords;

			collections::vector<model_element> out = new collections::vector_buffer<model_element>();

			cstr_t debug_view;

			while (data.size() > idx)
			{
				//debug_view.set(&data[idx], data.size() - idx);
				if (data.find(v_s, idx, idx + v_s.size()) == idx)
				{
					stream->move_to(idx + v_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y >> temp1._vector.z;
					vertices += temp1;
				}
				else if (data.find(vn_s, idx, idx + vn_s.size()) == idx)
				{
					stream->move_to(idx + vn_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y >> temp1._vector.z;
					normals += temp1;
				}
				else if (data.find(vt_s, idx, idx + vt_s.size()) == idx)
				{
					stream->move_to(idx + vt_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y;
					texcoords += {temp1._vector.x, temp1._vector.y};
				}
				else if (data.find(f_s, idx, idx + f_s.size()) == idx)
				{
					stream->move_to(idx + f_s.size(), streams::stream_reference::begin);
					index face[3] = { 0,0,0 };
					vertex vertex;
					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 };
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 };
					}
					model.value() += vertex;

					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 };
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 };
					}
					model.value() += vertex;

					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 };
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 };
					}
					model.value() += vertex;

				}
				else if (data.find(usemtl_s, idx, idx + usemtl_s.size()) == idx)
				{
					if (!material.is_empty())
					{
						model.key_value(ang::move(material));
						_elements += ang::move(model);
						model.value() = new collections::vector_buffer<vertex>();
						model.value()->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));
					}

					auto end = data.find('\n', idx);
					if (end == invalid_index)
						break; //error

					data.sub_string(material, idx + usemtl_s.size(), end - idx - usemtl_s.size());
				}

				idx = data.find('\n', idx);
				if (idx != invalid_index)idx++;
			}
			if (!material.is_empty())
			{
				model.key_value(ang::move(material));
				_elements += ang::move(model);
				model.value() = new collections::vector_buffer<vertex>();
				model.value()->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));
			}
			return true;
		});
	}
	catch (exception_t e)
	{
		return null;
	}
}


