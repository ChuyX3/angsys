#include "pch.h"
#include "console.h"
#include <conio.h>

using namespace ang;
using namespace ang::platform::console;


menu::menu()
{

}

menu::~menu()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::console::menu);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::console::menu, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::console::menu, object);

struct hide_cursor
{
	bool vis;
	uint size;
	output_t output;

	hide_cursor(output_t out) : output(out) {
		vis = output->cursor_visible();
		size = output->cursor_size();
		output->cursor_info(size, false);
	}
	~hide_cursor() {
		output->cursor_info(size, vis);
	}
};

int menu::show(output_t output)const
{
	wsize w = 0;
	wsize h = items().size();
	hide_cursor hc(output);

	for (string text : items())
		w = max(w, text->length());

	coord_t xy = coord() + coord_t{ 1, 0 };
	rect_t rc = coord() + bound(w + 2, h);
	output->draw_solid_rect(rc, normal_text_property(), ' ');

	for (string text : items())
	{
		output->write(xy, text);
		xy.y++;
	}
	
	int idx = 0;

	output->draw_solid_rect(coord_t(coord().x, coord().y + idx) + bound_t(w + 2, 1), highlight_text_property());
	output->write(coord_t(coord().x, coord().y + idx), ">");
	output->write(coord_t(coord().x + w + 1, coord().y + idx), "<");
	do {
		int c = _getch();

		if (c == 0 || c == 0XE0)
		{
			c = _getch();
			if (c == 0X48)
			{
				output->draw_solid_rect(coord_t(coord().x, coord().y + idx) + bound_t(w + 2, 1), normal_text_property());
				output->write(coord_t(coord().x, coord().y + idx), " ");
				output->write(coord_t(coord().x + w + 1, coord().y + idx), " ");
				if (idx > 0)idx--;
				else idx = h - 1;
			}
			else if(c == 0X50)
			{
				output->write(coord_t(coord().x, coord().y + idx), " ");
				output->write(coord_t(coord().x + w + 1, coord().y + idx), " ");
				output->draw_solid_rect(coord_t(coord().x, coord().y + idx) + bound_t(w + 2, 1), normal_text_property());
				if (idx < (h - 1))idx++;
				else idx = 0;
			}

			output->write(coord_t(coord().x, coord().y + idx), ">");
			output->write(coord_t(coord().x + w + 1, coord().y + idx), "<");
			output->draw_solid_rect(coord_t(coord().x, coord().y + idx) + bound_t(w + 2, 1), highlight_text_property());
		}
		else if (c == 13)
		{
			return idx;
		}
		else if (c == 27)
		{
			return -1;
		}

	} while (true);
}