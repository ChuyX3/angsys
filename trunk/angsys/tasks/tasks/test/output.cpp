#include "pch.h"
#include "console.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::console;

const char frame1[] = { '\xB3', '\xC4', '\xDA', '\xBF', '\xD9', '\xC0' , 0 };

output::output()
{
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
}

output::~output()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::console::output);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::console::output, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::console::output, object);

bound_t output::size()const {
	CONSOLE_SCREEN_BUFFER_INFO cb;
	GetConsoleScreenBufferInfo(handle, &cb);
	return{ cb.dwSize.X, cb.dwSize.Y };
}

void output::size(bound_t sz)const {
	SetConsoleScreenBufferSize(handle, {sz.width, sz.height });
}

uint output::cursor_size()const
{
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(handle, &cci);
	return cci.dwSize;
}

bool output::cursor_visible()const
{
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(handle, &cci);
	return cci.bVisible ? true : false;
}

void output::cursor_info(uint size, bool vis)
{
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = vis ? TRUE : FALSE;
	cci.dwSize = size;
	SetConsoleCursorInfo(handle, &cci);
}

coord_t output::cursor()const
{
	coord_t xy;
	CONSOLE_SCREEN_BUFFER_INFO cb;

	GetConsoleScreenBufferInfo(handle, &cb);
	xy.x = cb.dwCursorPosition.X;
	xy.y = cb.dwCursorPosition.Y;
	return xy;
}

void output::cursor(coord_t xy)
{
//	coord_t old;
	CONSOLE_SCREEN_BUFFER_INFO cb;
	GetConsoleScreenBufferInfo(handle, &cb);
//	old.x = cb.dwCursorPosition.X;
//	old.y = cb.dwCursorPosition.Y;

	if (xy.x >= cb.dwSize.X)
		xy.x = cb.dwSize.X - 1;

	if (xy.y >= cb.dwSize.Y)
		xy.y = cb.dwSize.Y - 1;

	COORD c = { xy.x, xy.y };
	SetConsoleCursorPosition(handle, reinterpret_cast<COORD&>(xy));
}

text_property_t output::text_property()const {
	text_property_t c;
	CONSOLE_SCREEN_BUFFER_INFO cb;
	GetConsoleScreenBufferInfo(handle, &cb);
	c.code(cb.wAttributes);
	return c;
}

void output::text_property(text_property_t color) {
	SetConsoleTextAttribute(handle, color.code());
}

void output::clrscr() {
	DWORD result;
	CONSOLE_SCREEN_BUFFER_INFO cb;
	GetConsoleScreenBufferInfo(handle, &cb);
	FillConsoleOutputCharacterA(handle, ' '
		, cb.dwSize.X * cb.dwSize.Y
		, {0,0}, &result);
	FillConsoleOutputAttribute(handle, cb.wAttributes
		, cb.dwSize.X * cb.dwSize.Y
		, { 0,0 }, &result);
	SetConsoleCursorPosition(handle, { 0,0 });
}

void output::draw_solid_rect(rect_t rc, text_property_t prop, char fill)
{
	dword result;
	CONSOLE_SCREEN_BUFFER_INFO cb;
	GetConsoleScreenBufferInfo(handle, &cb);
	short left = min(max(0, rc.left), cb.dwSize.X - 1);
	short width = min(rc.width(), cb.dwSize.X - left);

	if (fill != 0)
	{
		for (short j = max(0, rc.top), c = min(cb.dwSize.Y, rc.bottom); j < c; j++)
		{
			FillConsoleOutputAttribute(handle, prop.code(), width, { left,j }, &result);
			FillConsoleOutputCharacterA(handle, fill, width, { left,j }, &result);
		}
	}
	else
	{
		for (short j = max(0, rc.top), c = min(cb.dwSize.Y, rc.bottom); j < c; j++)
		{
			FillConsoleOutputAttribute(handle, prop.code(), width, { left,j }, &result);
		}
	}
}


void output::draw_frame_rect(rect_t rc, text_property_t prop, bool thick, bool fill)
{
	dword result;
	CONSOLE_SCREEN_BUFFER_INFO cb;
	GetConsoleScreenBufferInfo(handle, &cb);
	short left = min(max(0, rc.left), cb.dwSize.X - 1);
	short width = min(rc.width(), cb.dwSize.X - left);

	SetConsoleTextAttribute(handle, prop.code());

	const char* frm = thick ? frame1 : frame1;
	
	if (fill)
	{
		for (short j = max(1, rc.top + 1), c = min(cb.dwSize.Y - 1, rc.bottom - 1); j < c; j++)
		{
			FillConsoleOutputAttribute(handle, cb.wAttributes, width - 2, { left - 1,j }, &result);
			FillConsoleOutputCharacterA(handle, ' ', width - 2, { left - 1,j }, &result);
		}
	}


	SetConsoleTextAttribute(handle, cb.wAttributes);

}

void output::print(cstr_t text)
{
	dword written;
	WriteConsoleA(handle, text.cstr(), text.size(), &written, null);
}

void output::print(cstr_t format, collections::vector<variant> vars)
{
	string out;
	dword written;
	out->format(format, vars);
	WriteConsoleA(handle, out->cstr().cstr(), out->length(), &written, null);

	//WriteConsoleOutputCharacterA
}

void output::write(coord_t xy, cstr_t text)
{
	dword written;
	WriteConsoleOutputCharacterA(handle, text.cstr(), text.size(), reinterpret_cast<COORD&>(xy), &written);
}

void output::write(coord_t xy, cstr_t format, collections::vector<variant> vars)
{
	string out;
	dword written;
	out->format(format, vars);
	WriteConsoleOutputCharacterA(handle, out->cstr().cstr(), out->length(), reinterpret_cast<COORD&>(xy), &written);
}


