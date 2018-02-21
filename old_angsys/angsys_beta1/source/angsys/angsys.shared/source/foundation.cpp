/*********************************************************************************************************************/
/*   File Name: foundation.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/foundation.h"
#include "ang_inlines.h"

using namespace ang;
using namespace ang::foundation;

#ifndef ANDROID_PLATFORM

ang::foundation::point<float>::point()
	: x(0)
	, y(0)
{
}

ang::foundation::point<float>::point(point const& p)
	: x(p.x)
	, y(p.y)
{
}

ang::foundation::point<float>::point(float x, float y)
	: x(x)
	, y(y)
{
}

point<float>& ang::foundation::point<float>::operator = (const point<float>& p)
{
	x = p.x;
	y = p.y;
	return*this;
}

point<float>& ang::foundation::point<float>::operator += (const point<float>& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

point<float>& ang::foundation::point<float>::operator -= (const point<float>& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

point<float>& ang::foundation::point<float>::operator *= (float k)
{
	x *= k;
	y *= k;
	return *this;
}

point<float>& ang::foundation::point<float>::operator /= (float k)
{
	x /= k;
	y /= k;
	return *this;
}

////////////////////////////////////////////////////////////////////////////

size<float>::size()
	: width(0)
	, height(0)
{

}

size<float>::size(size<float> const& b)
	: width(b.width)
	, height(b.height)
{

}

size<float>::size(float width, float height)
	: width(width)
	, height(height)
{

}

size<float>& size<float>::operator = (const size<float>& b)
{
	width = b.width;
	height = b.height;
	return*this;
}

size<float>& size<float>::operator += (const size<float>& b)
{
	width += b.width;
	height += b.height;
	return *this;
}

size<float>& size<float>::operator -= (const size<float>& b)
{
	width -= b.width;
	height -= b.height;
	return *this;
}

size<float>& size<float>::operator *= (float k)
{
	width *= k;
	height *= k;
	return *this;
}

size<float>& size<float>::operator /= (float k)
{
	width /= k;
	height /= k;
	return *this;
}

////////////////////////////////////////////////////////////////////

rect<float>::rect()
	: left(0)
	, top(0)
	, right(0)
	, bottom(0)
{

}

rect<float>::rect(rect<float> const& rc)
	: left(rc.left)
	, top(rc.top)
	, right(rc.right)
	, bottom(rc.bottom)
{

}

rect<float>::rect(float l, float t, float r, float b)
	: left(l)
	, top(t)
	, right(r < l ? l : r)
	, bottom(b < t ? t : b)
{

}

rect<float>::rect(point<float> lt, point<float> rb)
	: left(lt.x)
	, top(lt.y)
	, right(rb.x)
	, bottom(rb.y)
{

}

rect<float>::rect(point<float> lt, size<float> sz)
	: left(lt.x)
	, top(lt.y)
	, right(lt.x + sz.width)
	, bottom(lt.y + sz.height)
{

}

float rect<float>::width()const { return right - left; }

float rect<float>::height()const { return bottom - top; }

point<float> rect<float>::left_top()const { return{ left, top }; }

point<float> rect<float>::right_top()const { return{ right, top }; }

point<float> rect<float>::left_bottom()const { return{ left, bottom }; }

point<float> rect<float>::right_bottom()const { return{ right, bottom }; }

void rect<float>::move_to(point<float> p)
{
	right = right - left + p.x;
	bottom = bottom - top + p.y;
	left = p.x;
	top = p.y;
}

void rect<float>::move_to(float x, float y)
{
	right = right - left + x;
	bottom = bottom - top + y;
	left = x;
	top = y;
}

void rect<float>::resize(size<float> sz)
{
	right = left + sz.width;
	bottom = top + sz.height;
}

void rect<float>::resize(float cx, float cy)
{
	right = left + cx;
	bottom = top + cy;
}

void rect<float>::deflate(float l, float t, float r, float b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

void rect<float>::inflate(float l, float t, float r, float b)
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

rect<float>& rect<float>::operator = (const rect<float>& rc)
{
	left = rc.left;
	top = rc.top;
	right = rc.right;
	bottom = rc.bottom;
	return *this;
}

rect<float>& rect<float>::operator += (const rect<float>& rc)
{
	left += rc.left;
	top += rc.top;
	right += rc.right;
	bottom += rc.bottom;
	return *this;
}

rect<float>& rect<float>::operator -= (const rect<float>& rc)
{
	left -= rc.left;
	top -= rc.top;
	right -= rc.right;
	bottom -= rc.bottom;
	return *this;
}

rect<float>& rect<float>::operator += (const point<float>& pt)
{
	left += pt.x;
	top += pt.y;
	right += pt.x;
	bottom += pt.y;
	return *this;
}

rect<float>& rect<float>::operator -= (const point<float>& pt)
{
	left -= pt.x;
	top -= pt.y;
	right -= pt.x;
	bottom -= pt.y;
	return *this;
}

rect<float>& rect<float>::operator += (const size<float>& sz)
{
	right += sz.width;
	bottom += sz.height;
	return *this;
}

rect<float>& rect<float>::operator -= (const size<float>& sz)
{
	right -= sz.width;
	bottom -= sz.height;
	if (left > right) right = left;
	if (top > bottom) bottom = top;
	return *this;
}

rect<float>& rect<float>::operator *= (float k)
{
	left *= k;
	top *= k;
	right *= k;
	bottom *= k;
	return *this;
}

rect<float>& rect<float>::operator /= (float k)
{
	left /= k;
	top /= k;
	right /= k;
	bottom /= k;
	return *this;
}

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static ang_pair<graphics::colors, cstr_t> to_string_color_map[] =
{
	{ graphics::colors::null, "null"_s },
	{ graphics::colors::transparent, "transparent"_s },
	{ graphics::colors::black, "black"_s },
	{ graphics::colors::maroon, "maroon"_s },
	{ graphics::colors::dark_red, "dark_red"_s },
	{ graphics::colors::red, "red"_s },
	{ graphics::colors::orange_red, "orange_red"_s },
	{ graphics::colors::dark_green, "dark_green"_s },
	{ graphics::colors::green, "green"_s },
	{ graphics::colors::olive, "olive"_s },
	{ graphics::colors::dark_orange, "dark_orange"_s },
	{ graphics::colors::orange, "orange"_s },
	{ graphics::colors::gold, "gold"_s },
	{ graphics::colors::lawn_green, "lawn_green"_s },
	{ graphics::colors::lime, "lime"_s },
	{ graphics::colors::chartreuse, "chartreuse"_s },
	{ graphics::colors::yellow, "yellow"_s },
	{ graphics::colors::dark_goldenrod, "dark_goldenrod"_s },
	{ graphics::colors::saddle_brown, "saddle_brown"_s },
	{ graphics::colors::chocolate, "chocolate"_s },
	{ graphics::colors::goldenrod, "goldenrod"_s },
	{ graphics::colors::firebrick, "firebrick"_s },
	{ graphics::colors::forest_green, "forest_green"_s },
	{ graphics::colors::olive_drab, "olive_drab"_s },
	{ graphics::colors::brown, "brown"_s },
	{ graphics::colors::sienna, "sienna"_s },
	{ graphics::colors::dark_olive_green, "dark_olive_green"_s },
	{ graphics::colors::green_yellow, "green_yellow"_s },
	{ graphics::colors::lime_green, "lime_green"_s },
	{ graphics::colors::crimson, "crimson"_s },
	{ graphics::colors::peru, "peru"_s },
	{ graphics::colors::tomato, "tomato"_s },
	{ graphics::colors::dark_slate_gray, "dark_slate_gray"_s },
	{ graphics::colors::coral, "coral"_s },
	{ graphics::colors::sea_green, "sea_green"_s },
	{ graphics::colors::indian_red, "indian_red"_s },
	{ graphics::colors::sandy_brown, "sandy_brown"_s },
	{ graphics::colors::dim_gray, "dim_gray"_s },
	{ graphics::colors::dark_khaki, "dark_khaki"_s },
	{ graphics::colors::midnight_blue, "midnight_blue"_s },
	{ graphics::colors::medium_sea_green, "medium_sea_green"_s },
	{ graphics::colors::salmon, "salmon"_s },
	{ graphics::colors::dark_salmon, "dark_salmon"_s },
	{ graphics::colors::light_salmon, "light_salmon"_s },
	{ graphics::colors::spring_green, "spring_green"_s },
	{ graphics::colors::navy, "navy"_s },
	{ graphics::colors::purple, "purple"_s },
	{ graphics::colors::teal, "teal"_s },
	{ graphics::colors::gray, "gray"_s },
	{ graphics::colors::light_coral, "light_coral"_s },
	{ graphics::colors::indigo, "indigo"_s },
	{ graphics::colors::medium_violet_red, "medium_violet_red"_s },
	{ graphics::colors::burly_wood, "burly_wood"_s },
	{ graphics::colors::dark_blue, "dark_blue"_s },
	{ graphics::colors::dark_magenta, "dark_magenta"_s },
	{ graphics::colors::dark_slate_blue, "dark_slate_blue"_s },
	{ graphics::colors::dark_cyan, "dark_cyan"_s },
	{ graphics::colors::tan, "tan"_s },
	{ graphics::colors::khaki, "khaki"_s },
	{ graphics::colors::rosy_brown, "rosy_brown"_s },
	{ graphics::colors::dark_sea_green, "dark_sea_green"_s },
	{ graphics::colors::slate_gray, "slate_gray"_s },
	{ graphics::colors::light_green, "light_green"_s },
	{ graphics::colors::deep_pink, "deep_pink"_s },
	{ graphics::colors::pale_violet_red, "pale_violet_red"_s },
	{ graphics::colors::pale_green, "pale_green"_s },
	{ graphics::colors::light_slate_gray, "light_slate_gray"_s },
	{ graphics::colors::medium_spring_green, "medium_spring_green"_s },
	{ graphics::colors::cadet_blue, "cadet_blue"_s },
	{ graphics::colors::dark_gray, "dark_gray"_s },
	{ graphics::colors::light_sea_green, "light_sea_green"_s },
	{ graphics::colors::medium_aquamarine, "medium_aquamarine"_s },
	{ graphics::colors::pale_goldenrod, "pale_goldenrod"_s },
	{ graphics::colors::navajo_white, "navajo_white"_s },
	{ graphics::colors::wheat, "wheat"_s },
	{ graphics::colors::hot_pink, "hot_pink"_s },
	{ graphics::colors::steel_blue, "steel_blue"_s },
	{ graphics::colors::moccasin, "moccasin"_s },
	{ graphics::colors::peach_puff, "peach_puff"_s },
	{ graphics::colors::silver, "silver"_s },
	{ graphics::colors::light_pink, "light_pink"_s },
	{ graphics::colors::bisque, "bisque"_s },
	{ graphics::colors::pink, "pink"_s },
	{ graphics::colors::dark_orchid, "dark_orchid"_s },
	{ graphics::colors::medium_turquoise, "medium_turquoise"_s },
	{ graphics::colors::medium_blue, "medium_blue"_s },
	{ graphics::colors::slate_blue, "slate_blue"_s },
	{ graphics::colors::blanched_almond, "blanched_almond"_s },
	{ graphics::colors::lemon_chiffon, "lemon_chiffon"_s },
	{ graphics::colors::turquoise, "turquoise"_s },
	{ graphics::colors::dark_turquoise, "dark_turquoise"_s },
	{ graphics::colors::light_goldenrod_yellow, "light_goldenrod_yellow"_s },
	{ graphics::colors::dark_violet, "dark_violet"_s },
	{ graphics::colors::medium_orchid, "medium_orchid"_s },
	{ graphics::colors::light_gray, "light_gray"_s },
	{ graphics::colors::aquamarine, "aquamarine"_s },
	{ graphics::colors::papaya_whip, "papaya_whip"_s },
	{ graphics::colors::orchid, "orchid"_s },
	{ graphics::colors::antique_white, "antique_white"_s },
	{ graphics::colors::thistle, "thistle"_s },
	{ graphics::colors::medium_purple, "medium_purple"_s },
	{ graphics::colors::gainsboro, "gainsboro"_s },
	{ graphics::colors::beige, "beige"_s },
	{ graphics::colors::cornsilk, "cornsilk"_s },
	{ graphics::colors::plum, "plum"_s },
	{ graphics::colors::light_steel_blue, "light_steel_blue"_s },
	{ graphics::colors::light_yellow, "light_yellow"_s },
	{ graphics::colors::royal_blue, "royal_blue"_s },
	{ graphics::colors::misty_rose, "misty_rose"_s },
	{ graphics::colors::blue_violet, "blue_violet"_s },
	{ graphics::colors::light_blue, "light_blue"_s },
	{ graphics::colors::powder_blue, "powder_blue"_s },
	{ graphics::colors::linen, "linen"_s },
	{ graphics::colors::old_lace, "old_lace"_s },
	{ graphics::colors::sky_blue, "sky_blue"_s },
	{ graphics::colors::cornflower_blue, "cornflower_blue"_s },
	{ graphics::colors::medium_slate_blue, "medium_slate_blue"_s },
	{ graphics::colors::violet, "violet"_s },
	{ graphics::colors::pale_turquoise, "pale_turquoise"_s },
	{ graphics::colors::sea_shell, "sea_shell"_s },
	{ graphics::colors::floral_white, "floral_white"_s },
	{ graphics::colors::honeydew, "honeydew"_s },
	{ graphics::colors::ivory, "ivory"_s },
	{ graphics::colors::lavender_blush, "lavender_blush"_s },
	{ graphics::colors::white_smoke, "white_smoke"_s },
	{ graphics::colors::light_sky_blue, "light_sky_blue"_s },
	{ graphics::colors::lavender, "lavender"_s },
	{ graphics::colors::snow, "snow"_s },
	{ graphics::colors::mint_cream, "mint_cream"_s },
	{ graphics::colors::blue, "blue"_s },
	{ graphics::colors::fuchsia, "fuchsia"_s },
	{ graphics::colors::magenta, "magenta"_s },
	{ graphics::colors::dodger_blue, "dodger_blue"_s },
	{ graphics::colors::deep_sky_blue, "deep_sky_blue"_s },
	{ graphics::colors::alice_blue, "alice_blue"_s },
	{ graphics::colors::ghost_white, "ghost_white"_s },
	{ graphics::colors::cyan, "cyan"_s },
	{ graphics::colors::aqua, "aqua"_s },
	{ graphics::colors::light_cyan, "light_cyan"_s },
	{ graphics::colors::azure, "azure"_s },
	{ graphics::colors::white, "white"_s },
};

static ang_pair<cstr_t, graphics::colors> _parse_color_map[] =
{
	{ "alice_blue"_s, graphics::colors::alice_blue },
	{ "antique_white"_s, graphics::colors::antique_white },
	{ "aqua"_s, graphics::colors::aqua },
	{ "aquamarine"_s, graphics::colors::aquamarine },
	{ "azure"_s, graphics::colors::azure },
	{ "beige"_s, graphics::colors::beige },
	{ "bisque"_s, graphics::colors::bisque },
	{ "black"_s, graphics::colors::black },
	{ "blanched_almond"_s, graphics::colors::blanched_almond },
	{ "blue"_s, graphics::colors::blue },
	{ "blue_violet"_s, graphics::colors::blue_violet },
	{ "brown"_s, graphics::colors::brown },
	{ "burly_wood"_s, graphics::colors::burly_wood },
	{ "cadet_blue"_s, graphics::colors::cadet_blue },
	{ "chartreuse"_s, graphics::colors::chartreuse },
	{ "chocolate"_s, graphics::colors::chocolate },
	{ "coral"_s, graphics::colors::coral },
	{ "cornflower_blue"_s, graphics::colors::cornflower_blue },
	{ "cornsilk"_s, graphics::colors::cornsilk },
	{ "crimson"_s, graphics::colors::crimson },
	{ "cyan"_s, graphics::colors::cyan },
	{ "dark_blue"_s, graphics::colors::dark_blue },
	{ "dark_cyan"_s, graphics::colors::dark_cyan },
	{ "dark_goldenrod"_s, graphics::colors::dark_goldenrod },
	{ "dark_gray"_s, graphics::colors::dark_gray },
	{ "dark_green"_s, graphics::colors::dark_green },
	{ "dark_khaki"_s, graphics::colors::dark_khaki },
	{ "dark_magenta"_s, graphics::colors::dark_magenta },
	{ "dark_olive_green"_s, graphics::colors::dark_olive_green },
	{ "dark_orange"_s, graphics::colors::dark_orange },
	{ "dark_orchid"_s, graphics::colors::dark_orchid },
	{ "dark_red"_s, graphics::colors::dark_red },
	{ "dark_salmon"_s, graphics::colors::dark_salmon },
	{ "dark_sea_green"_s, graphics::colors::dark_sea_green },
	{ "dark_slate_blue"_s, graphics::colors::dark_slate_blue },
	{ "dark_slate_gray"_s, graphics::colors::dark_slate_gray },
	{ "dark_turquoise"_s, graphics::colors::dark_turquoise },
	{ "dark_violet"_s, graphics::colors::dark_violet },
	{ "deep_pink"_s, graphics::colors::deep_pink },
	{ "deep_sky_blue"_s, graphics::colors::deep_sky_blue },
	{ "dim_gray"_s, graphics::colors::dim_gray },
	{ "dodger_blue"_s, graphics::colors::dodger_blue },
	{ "firebrick"_s, graphics::colors::firebrick },
	{ "floral_white"_s, graphics::colors::floral_white },
	{ "forest_green"_s, graphics::colors::forest_green },
	{ "fuchsia"_s, graphics::colors::fuchsia },
	{ "gainsboro"_s, graphics::colors::gainsboro },
	{ "ghost_white"_s, graphics::colors::ghost_white },
	{ "gold"_s, graphics::colors::gold },
	{ "goldenrod"_s, graphics::colors::goldenrod },
	{ "gray"_s, graphics::colors::gray },
	{ "green"_s, graphics::colors::green },
	{ "green_yellow"_s, graphics::colors::green_yellow },
	{ "honeydew"_s, graphics::colors::honeydew },
	{ "hot_pink"_s, graphics::colors::hot_pink },
	{ "indian_red"_s, graphics::colors::indian_red },
	{ "indigo"_s, graphics::colors::indigo },
	{ "ivory"_s, graphics::colors::ivory },
	{ "khaki"_s, graphics::colors::khaki },
	{ "lavender"_s, graphics::colors::lavender },
	{ "lavender_blush"_s, graphics::colors::lavender_blush },
	{ "lawn_green"_s, graphics::colors::lawn_green },
	{ "lemon_chiffon"_s, graphics::colors::lemon_chiffon },
	{ "light_blue"_s, graphics::colors::light_blue },
	{ "light_coral"_s, graphics::colors::light_coral },
	{ "light_cyan"_s, graphics::colors::light_cyan },
	{ "light_goldenrod_yellow"_s, graphics::colors::light_goldenrod_yellow },
	{ "light_gray"_s, graphics::colors::light_gray },
	{ "light_green"_s, graphics::colors::light_green },
	{ "light_pink"_s, graphics::colors::light_pink },
	{ "light_salmon"_s, graphics::colors::light_salmon },
	{ "light_sea_green"_s, graphics::colors::light_sea_green },
	{ "light_sky_blue"_s, graphics::colors::light_sky_blue },
	{ "light_slate_gray"_s, graphics::colors::light_slate_gray },
	{ "light_steel_blue"_s, graphics::colors::light_steel_blue },
	{ "light_yellow"_s, graphics::colors::light_yellow },
	{ "lime"_s, graphics::colors::lime },
	{ "lime_green"_s, graphics::colors::lime_green },
	{ "linen"_s, graphics::colors::linen },
	{ "magenta"_s, graphics::colors::magenta },
	{ "maroon"_s, graphics::colors::maroon },
	{ "medium_aquamarine"_s, graphics::colors::medium_aquamarine },
	{ "medium_blue"_s, graphics::colors::medium_blue },
	{ "medium_orchid"_s, graphics::colors::medium_orchid },
	{ "medium_purple"_s, graphics::colors::medium_purple },
	{ "medium_sea_green"_s, graphics::colors::medium_sea_green },
	{ "medium_slate_blue"_s, graphics::colors::medium_slate_blue },
	{ "medium_spring_green"_s, graphics::colors::medium_spring_green },
	{ "medium_turquoise"_s, graphics::colors::medium_turquoise },
	{ "medium_violet_red"_s, graphics::colors::medium_violet_red },
	{ "midnight_blue"_s, graphics::colors::midnight_blue },
	{ "mint_cream"_s, graphics::colors::mint_cream },
	{ "misty_rose"_s, graphics::colors::misty_rose },
	{ "moccasin"_s, graphics::colors::moccasin },
	{ "navajo_white"_s, graphics::colors::navajo_white },
	{ "navy"_s, graphics::colors::navy },
	{ "null"_s, graphics::colors::null },
	{ "old_lace"_s, graphics::colors::old_lace },
	{ "olive"_s, graphics::colors::olive },
	{ "olive_drab"_s, graphics::colors::olive_drab },
	{ "orange"_s, graphics::colors::orange },
	{ "orange_red"_s, graphics::colors::orange_red },
	{ "orchid"_s, graphics::colors::orchid },
	{ "pale_goldenrod"_s, graphics::colors::pale_goldenrod },
	{ "pale_green"_s, graphics::colors::pale_green },
	{ "pale_turquoise"_s, graphics::colors::pale_turquoise },
	{ "pale_violet_red"_s, graphics::colors::pale_violet_red },
	{ "papaya_whip"_s, graphics::colors::papaya_whip },
	{ "peach_puff"_s, graphics::colors::peach_puff },
	{ "peru"_s, graphics::colors::peru },
	{ "pink"_s, graphics::colors::pink },
	{ "plum"_s, graphics::colors::plum },
	{ "powder_blue"_s, graphics::colors::powder_blue },
	{ "purple"_s, graphics::colors::purple },
	{ "red"_s, graphics::colors::red },
	{ "rosy_brown"_s, graphics::colors::rosy_brown },
	{ "royal_blue"_s, graphics::colors::royal_blue },
	{ "saddle_brown"_s, graphics::colors::saddle_brown },
	{ "salmon"_s, graphics::colors::salmon },
	{ "sandy_brown"_s, graphics::colors::sandy_brown },
	{ "sea_green"_s, graphics::colors::sea_green },
	{ "sea_shell"_s, graphics::colors::sea_shell },
	{ "sienna"_s, graphics::colors::sienna },
	{ "silver"_s, graphics::colors::silver },
	{ "sky_blue"_s, graphics::colors::sky_blue },
	{ "slate_blue"_s, graphics::colors::slate_blue },
	{ "slate_gray"_s, graphics::colors::slate_gray },
	{ "snow"_s, graphics::colors::snow },
	{ "spring_green"_s, graphics::colors::spring_green },
	{ "steel_blue"_s, graphics::colors::steel_blue },
	{ "tan"_s, graphics::colors::tan },
	{ "teal"_s, graphics::colors::teal },
	{ "thistle"_s, graphics::colors::thistle },
	{ "tomato"_s, graphics::colors::tomato },
	{ "transparent"_s, graphics::colors::transparent },
	{ "turquoise"_s, graphics::colors::turquoise },
	{ "violet"_s, graphics::colors::violet },
	{ "wheat"_s, graphics::colors::wheat },
	{ "white"_s, graphics::colors::white },
	{ "white_smoke"_s, graphics::colors::white_smoke },
	{ "yellow"_s, graphics::colors::yellow },

};

using namespace ang::graphics;

ANG_IMPLEMENT_ENUM(ang::graphics, graph_driver_type, byte, graph_driver_type::null);
ANG_IMPLEMENT_ENUM(ang::graphics, primitive, byte, primitive::point);

graphics::color::color()
{
	code = 0;
}

graphics::color::color(const color& c)
{
	code = c.code;
}

graphics::color::color(enum_t c)
{
	code = c;
}

graphics::color::color(dword rgba)
{
	code = rgba;
}

graphics::color::color(enum_t c, byte a)
{
	code = c;
	components.alpha = a;
}

graphics::color::color(dword rgb, byte a)
{
	code = rgb;
	components.alpha = a;
}
graphics::color::color(byte r, byte g, byte b, byte a)
{
	components.red = r;
	components.green = g;
	components.blue = b;
	components.alpha = a;
}

graphics::color::~color() {}

void  graphics::color::parse(cstr_t cstr)
{
	if (cstr[0] == '#')
	{
		code = interger::parse(&cstr[1], 16);
	}
	else
	{
		wsize idx = ang_binary_search<cstr_t>(cstr, _parse_color_map);
		if (idx > array_size(_parse_color_map))
			code = 0;
		else
			code = _parse_color_map[idx].value;
	}
}

void graphics::color::parse(cwstr_t cstr)
{
	if (cstr[0] == '#')
	{
		code = interger::parse(&cstr[1], 16);
	}
	else
	{
		wsize idx = ang_binary_search<cwstr_t>(cstr, _parse_color_map);
		if (idx > array_size(_parse_color_map))
			code = 0;
		else
			code = _parse_color_map[idx].value;
	}
}

string_t graphics::color::to_string()const
{
	wsize idx = ang_binary_search(code, to_string_color_map);
	if (idx > array_size(to_string_color_map))
	{
		string out = ""_s;
		out->format("#%08x"_s, code);
		return move(out);
	}
	else
	{
		return to_string_color_map[idx].value;
	}
}

graphics::color& graphics::color::operator = (const graphics::color& c)
{
	code = c.code;
	return*this;
}

graphics::color& graphics::color::operator = (graphics::colors c)
{
	code = c;
	return*this;
}

graphics::color& graphics::color::operator = (dword rgba)
{
	code = rgba;
	return*this;
}