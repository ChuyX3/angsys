#include "pch.h"
#include "engine.h"

//#include "binary_tree.h"



namespace d3d11
{
	using namespace ang;
	using namespace ang::platform;
	using namespace ang::platform::windows;

	class main_app;
	typedef object_wrapper<main_app> main_app_t;

	class main_wnd : public window
	{
	public:
		main_wnd();

		ANG_DECLARE_INTERFACE();

		virtual void window_proc(events::message_t msg)override {
			if (msg->msg() == events::win_msg_enum::EraseBkgnd
				|| msg->msg() == events::win_msg_enum::Draw)
			{
				graphics::paint_dc_t dc = new graphics::paint_dc(this);
			}
			else
			{
				return window::window_proc(msg.get());
			}
		}
		
	private:
		virtual~main_wnd();
	};

	class main_app : public app
	{
		engine_t _engine;

	public:
		main_app();

		ANG_DECLARE_INTERFACE();

		bool init_app(array<string> cmdl)override;
		bool exit_app()override;

	private:
		virtual~main_app();
	};

}


union color_bgra_t
{
	uint code;
	struct
	{
		byte blue;
		byte green;
		byte red;
		byte alpha;

	}components;
	color_bgra_t& operator = (color_bgra_t const& color) {
		code = color.code;
		return*this;
	}
};

union color_rgba_t
{
	uint code;
	struct
	{
		byte red;
		byte green;
		byte blue;
		byte alpha;
	}components;

	color_rgba_t& operator = (color_bgra_t const& color) {
		components.red = color.components.red;
		components.green = color.components.green;
		components.blue = color.components.blue;
		components.alpha = color.components.alpha;
		return*this;
	}
	color_rgba_t& operator = (color_rgba_t const& color) {
		code = color.code;
		return*this;
	}
};

#include <algorithm>

using std::swap;

//using namespace ang;

int main(int argc, char* argv[])
{
	d3d11::main_app_t app = new d3d11::main_app();
	return (int)app->run();

	/*FILE* file = fopen("C:\\Users\\jesus.rocha\\Desktop\\colors.txt", "r");
	if (file)
	{
		char buff[200];
		ang::string name;
		color_bgra_t bgra;
		color_rgba_t rgba;
		bgra.code = 0;

		ang::collections::vector<ang::collections::pair<ang::string, uint>> data;
		ang::collections::vector<ang::collections::pair<uint, ang::string>> data2;
		while (fscanf(file, "%s : #%X\n", buff, &bgra) != 0 && !feof(file))
		{
			rgba = bgra;
			data += { ang::cstr_t(buff, -1), rgba.code };
		}
		fclose(file);
		file = fopen("C:\\Users\\jesus.rocha\\Desktop\\colors.txt", "w");

		ang::foreach(data, [&](ang::collections::pair<ang::string, uint>& pair)
		{
			for (index i = 0; i < pair.key_value()->length(); ++i)
			{
				char c = pair.key_value()[i];
				if (c >= 'A' && c <= 'Z')
				{
					pair.key_value()[i] = c - 'A' + 'a';
					if (i > 0)
						pair.key_value()->insert('_', pair.key_value()->at(i));
				}
			}
			data2 += { pair.value(), pair.key_value().get()};
			printf("%s = %#.8X,\n", pair.key_value()->cstr().cstr(), pair.value());
			fprintf(file, "%s = %#.8X,\n", pair.key_value()->cstr().cstr(), pair.value());
		});
		fclose(file);
		printf("\n\n");


		file = fopen("C:\\Users\\jesus.rocha\\Desktop\\colors_name_order.txt", "w");
		ang::foreach(data2, [&](ang::collections::pair<uint, ang::string>& pair)
		{
			printf("{ \"%s\"_s, graphics::colors::%s },\n", pair.value()->cstr().cstr(), pair.value()->cstr().cstr());
			fprintf(file, "{ \"%s\"_s, graphics::colors::%s },\n", pair.value()->cstr().cstr(), pair.value()->cstr().cstr());
		});
		fclose(file);
		printf("\n\n");

		ang::static_array<ang::collections::pair<uint, ang::string>> arr = data2;
		std::sort(arr.begin(), arr.end());

		file = fopen("C:\\Users\\jesus.rocha\\Desktop\\colors_vaule_order.txt", "w");
		ang::foreach(data2, [&](ang::collections::pair<uint, ang::string>& pair)
		{
			printf("{ graphics::colors::%s, \"%s\"_s },\n", pair.value()->cstr().cstr(), pair.value()->cstr().cstr());
			fprintf(file, "{ graphics::colors::%s, \"%s\"_s },\n", pair.value()->cstr().cstr(), pair.value()->cstr().cstr());
		});
		fclose(file);
	}

	getchar();
	return true;*/
}


d3d11::main_wnd::main_wnd()
{
}

d3d11::main_wnd::~main_wnd()
{

}


ANG_IMPLEMENT_BASIC_INTERFACE(d3d11::main_wnd, window);


d3d11::main_app::main_app()
{
	_engine = new engine();
}

d3d11::main_app::~main_app()
{
	_engine = null;
}

ANG_IMPLEMENT_BASIC_INTERFACE(d3d11::main_app, app);

bool d3d11::main_app::init_app(ang::array<ang::string> cmdl)
{
	app::init_app(cmdl);

	window_t wnd = new d3d11::main_wnd();

	wnd->createdEvent += new events::created_event(_engine.get(), &engine::on_create_event);
	wnd->destroyedEvent += new events::destroyed_event(_engine.get(), &engine::on_destroy_event);
	wnd->sizeEvent += new events::display_size_change_event(_engine.get(), &engine::on_size_changed_event);
	wnd->mouseMovedEvent += new events::pointer_moved_event(_engine.get(), &engine::on_pointer_moved_event);
	wnd->mouseButtonPressedEvent += new events::pointer_moved_event(_engine.get(), &engine::on_pointer_pressed_event);
	wnd->mouseButtonReleasedEvent += new events::pointer_moved_event(_engine.get(), &engine::on_pointer_released_event);

	if (!wnd->create(new wnd_create_args(
		"d3d11_core_view_wnd", "d3d11 Task1", {100,100, 1124, 900}
	)))
		return false;
	set_main_wnd(wnd);

	wnd->update_wnd();
	wnd->show(showing_flag::show);
	return true;
}

bool d3d11::main_app::exit_app()
{
	return app::exit_app();
}