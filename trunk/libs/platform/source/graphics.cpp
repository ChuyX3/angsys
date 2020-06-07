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
#include <ang/platform/platform.h>

using namespace ang;
using namespace ang::graphics;

#ifndef ANDROID_PLATFORM

ang::graphics::point<float> ang::graphics::point<float>::parse(ang::cstr_t cstr)
{
	if (cstr.ptr() == null)
		return{ 0,0 };
	windex i = 0;
	float temp[2];
	text::parser::seek(cstr, i, " [ "_sv);
	temp[0] = (float)text::parser::to_floating(cstr, i);
	text::parser::seek(cstr, i, " , "_sv);
	temp[1] = (float)text::parser::to_floating(cstr, i);
	return{ temp[0], temp[1] };
}

ang::graphics::point<float>::point()
	: x(0)
	, y(0)
{
}

ang::graphics::point<float>::point(point const& p)
	: x(p.x)
	, y(p.y)
{
}

ang::graphics::point<float>::point(float x, float y)
	: x(x)
	, y(y)
{
}

point<float>& ang::graphics::point<float>::operator = (const point<float>& p)
{
	x = p.x;
	y = p.y;
	return*this;
}

point<float>& ang::graphics::point<float>::operator += (const point<float>& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

point<float>& ang::graphics::point<float>::operator -= (const point<float>& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

point<float>& ang::graphics::point<float>::operator *= (float k)
{
	x *= k;
	y *= k;
	return *this;
}

point<float>& ang::graphics::point<float>::operator /= (float k)
{
	x /= k;
	y /= k;
	return *this;
}

////////////////////////////////////////////////////////////////////////////


ang::graphics::size<float> ang::graphics::size<float>::parse(ang::cstr_t cstr)
{
	if (cstr.ptr() == null)
		return{ 0,0 };
	windex i = 0;
	float temp[2];
	text::parser::seek(cstr, i, " [ "_sv);
	temp[0] = (float)text::parser::to_floating(cstr, i);
	text::parser::seek(cstr, i, " , "_sv);
	temp[1] = (float)text::parser::to_floating(cstr, i);
	return{ temp[0], temp[1] };
}

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

ang::graphics::rect<float> ang::graphics::rect<float>::parse(ang::cstr_t cstr)
{
	if (cstr.ptr() == null)
		return{ 0,0,0,0 };
	windex i = 0;
	float temp[4];
	text::parser::seek(cstr, i, " [ "_sv);
	temp[0] = (float)text::parser::to_floating(cstr, i);
	text::parser::seek(cstr, i, " , "_sv);
	temp[1] = (float)text::parser::to_floating(cstr, i);
	text::parser::seek(cstr, i, " , "_sv);
	temp[2] = (float)text::parser::to_floating(cstr, i);
	text::parser::seek(cstr, i, " , "_sv);
	temp[3] = (float)text::parser::to_floating(cstr, i);
	return{ temp[0], temp[1], temp[2], temp[3] };
}


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


float ang::graphics::rect<float>::get_width_property(ang::base_property const*prop) {
	rect<float>* proxy = field_to_parent(&rect<float>::width, prop);
	return proxy->right - proxy->left;
}

void ang::graphics::rect<float>::set_width_property(ang::base_property *prop, float&& value) {
	rect<float>* proxy = field_to_parent(&rect<float>::width, prop);
	proxy->right = proxy->left + forward<float>(value);
}

float ang::graphics::rect<float>::get_height_property(ang::base_property const*prop) {
	rect<float>* proxy = field_to_parent(&rect<float>::height, prop);
	return proxy->bottom - proxy->top;
}

void ang::graphics::rect<float>::set_height_property(ang::base_property *prop, float&& value) {
	rect<float>* proxy = field_to_parent(&rect<float>::height, prop);
	proxy->bottom = proxy->top + forward<float>(value);
}


ang::graphics::point<float> ang::graphics::rect<float>::get_left_top_property(ang::base_property const*prop) {
	rect<float>* proxy = field_to_parent(&rect<float>::left_top, prop);
	return point<float>(proxy->left, proxy->top);
}

ang::graphics::point<float> ang::graphics::rect<float>::get_right_top_property(ang::base_property const*prop) {
	rect<float>* proxy = field_to_parent(&rect<float>::right_top, prop);
	return point<float>(proxy->right, proxy->top);
}

ang::graphics::point<float> ang::graphics::rect<float>::get_left_bottom_property(ang::base_property const*prop) {
	rect<float>* proxy = field_to_parent(&rect<float>::left_bottom, prop);
	return point<float>(proxy->left, proxy->bottom);
}

ang::graphics::point<float> ang::graphics::rect<float>::get_right_bottom_property(ang::base_property const*prop) {
	rect<float>* proxy = field_to_parent(&rect<float>::right_bottom, prop);
	return point<float>(proxy->right, proxy->bottom);
}

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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Null = 0,
//OpenGL,
//OpenGLES,
//DirectX9,
//DirectX11,
//Vulkan,//todo
//DirectX12,//todo
//DirectX11_VRX,//Virtual Reality Extension
//DirectX = DirectX11,

static collections::pair<graphics::graph_driver_type, castr_t> s_to_string_graph_driver_type_map[] =
{
	{ graph_driver_type::Null, "Null"_sv },
	{ graph_driver_type::OpenGL, "OpenGL,"_sv },
	{ graph_driver_type::OpenGLES, "OpenGLES,"_sv },
	{ graph_driver_type::DirectX9, "DirectX9,"_sv },
	{ graph_driver_type::DirectX11, "DirectX11,"_sv },
	{ graph_driver_type::Vulkan, "Vulkan,"_sv },
	{ graph_driver_type::DirectX12, "DirectX12,"_sv },
	{ graph_driver_type::DirectX11_VRX, "DirectX11_VRX,"_sv },
};

static collections::pair<castr_t, graphics::graph_driver_type> s_parse_graph_driver_type_map[] =
{	
	{ "DirectX11"_sv, graph_driver_type::DirectX11 },
	{ "DirectX11_VRX"_sv, graph_driver_type::DirectX11_VRX },
	{ "DirectX12"_sv, graph_driver_type::DirectX12 },
	{ "DirectX9"_sv, graph_driver_type::DirectX9 },
	{ "Null"_sv, graph_driver_type::Null },
	{ "OpenGL"_sv, graph_driver_type::OpenGL },
	{ "OpenGLES"_sv, graph_driver_type::OpenGLES },
	{ "Vulkan"_sv, graph_driver_type::Vulkan },
};

graph_driver_type_t graphics::graph_driver_type_t::parse(cstr_t cstr)
{
	wsize idx = algorithms::binary_search(cstr, to_array(s_parse_graph_driver_type_map));
	if (idx > algorithms::array_size(s_parse_graph_driver_type_map))
		return graph_driver_type::Null;
	else
		return s_parse_graph_driver_type_map[idx].value;
}

castr_t graphics::graph_driver_type_t::to_string()const
{
	wsize idx = algorithms::binary_search(get(), to_array(s_to_string_graph_driver_type_map));
	if (idx > algorithms::array_size(s_to_string_graph_driver_type_map))
		return "Null"_sv;
	else
		return s_to_string_graph_driver_type_map[idx].value;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static collections::pair<graphics::colors, cstr_t> s_color_to_string_map[] =
{
	{ graphics::colors::null, "null"_sv },
	{ graphics::colors::transparent, "transparent"_sv },
	{ graphics::colors::black, "black"_sv },
	{ graphics::colors::maroon, "maroon"_sv },
	{ graphics::colors::dark_red, "dark_red"_sv },
	{ graphics::colors::red, "red"_sv },
	{ graphics::colors::orange_red, "orange_red"_sv },
	{ graphics::colors::dark_green, "dark_green"_sv },
	{ graphics::colors::green, "green"_sv },
	{ graphics::colors::olive, "olive"_sv },
	{ graphics::colors::dark_orange, "dark_orange"_sv },
	{ graphics::colors::orange, "orange"_sv },
	{ graphics::colors::gold, "gold"_sv },
	{ graphics::colors::lawn_green, "lawn_green"_sv },
	{ graphics::colors::lime, "lime"_sv },
	{ graphics::colors::chartreuse, "chartreuse"_sv },
	{ graphics::colors::yellow, "yellow"_sv },
	{ graphics::colors::dark_goldenrod, "dark_goldenrod"_sv },
	{ graphics::colors::saddle_brown, "saddle_brown"_sv },
	{ graphics::colors::chocolate, "chocolate"_sv },
	{ graphics::colors::goldenrod, "goldenrod"_sv },
	{ graphics::colors::firebrick, "firebrick"_sv },
	{ graphics::colors::forest_green, "forest_green"_sv },
	{ graphics::colors::olive_drab, "olive_drab"_sv },
	{ graphics::colors::brown, "brown"_sv },
	{ graphics::colors::sienna, "sienna"_sv },
	{ graphics::colors::dark_olive_green, "dark_olive_green"_sv },
	{ graphics::colors::green_yellow, "green_yellow"_sv },
	{ graphics::colors::lime_green, "lime_green"_sv },
	{ graphics::colors::crimson, "crimson"_sv },
	{ graphics::colors::peru, "peru"_sv },
	{ graphics::colors::tomato, "tomato"_sv },
	{ graphics::colors::dark_slate_gray, "dark_slate_gray"_sv },
	{ graphics::colors::coral, "coral"_sv },
	{ graphics::colors::sea_green, "sea_green"_sv },
	{ graphics::colors::indian_red, "indian_red"_sv },
	{ graphics::colors::sandy_brown, "sandy_brown"_sv },
	{ graphics::colors::dim_gray, "dim_gray"_sv },
	{ graphics::colors::dark_khaki, "dark_khaki"_sv },
	{ graphics::colors::midnight_blue, "midnight_blue"_sv },
	{ graphics::colors::medium_sea_green, "medium_sea_green"_sv },
	{ graphics::colors::salmon, "salmon"_sv },
	{ graphics::colors::dark_salmon, "dark_salmon"_sv },
	{ graphics::colors::light_salmon, "light_salmon"_sv },
	{ graphics::colors::spring_green, "spring_green"_sv },
	{ graphics::colors::navy, "navy"_sv },
	{ graphics::colors::purple, "purple"_sv },
	{ graphics::colors::teal, "teal"_sv },
	{ graphics::colors::gray, "gray"_sv },
	{ graphics::colors::light_coral, "light_coral"_sv },
	{ graphics::colors::indigo, "indigo"_sv },
	{ graphics::colors::medium_violet_red, "medium_violet_red"_sv },
	{ graphics::colors::burly_wood, "burly_wood"_sv },
	{ graphics::colors::dark_blue, "dark_blue"_sv },
	{ graphics::colors::dark_magenta, "dark_magenta"_sv },
	{ graphics::colors::dark_slate_blue, "dark_slate_blue"_sv },
	{ graphics::colors::dark_cyan, "dark_cyan"_sv },
	{ graphics::colors::tan, "tan"_sv },
	{ graphics::colors::khaki, "khaki"_sv },
	{ graphics::colors::rosy_brown, "rosy_brown"_sv },
	{ graphics::colors::dark_sea_green, "dark_sea_green"_sv },
	{ graphics::colors::slate_gray, "slate_gray"_sv },
	{ graphics::colors::light_green, "light_green"_sv },
	{ graphics::colors::deep_pink, "deep_pink"_sv },
	{ graphics::colors::pale_violet_red, "pale_violet_red"_sv },
	{ graphics::colors::pale_green, "pale_green"_sv },
	{ graphics::colors::light_slate_gray, "light_slate_gray"_sv },
	{ graphics::colors::medium_spring_green, "medium_spring_green"_sv },
	{ graphics::colors::cadet_blue, "cadet_blue"_sv },
	{ graphics::colors::dark_gray, "dark_gray"_sv },
	{ graphics::colors::light_sea_green, "light_sea_green"_sv },
	{ graphics::colors::medium_aquamarine, "medium_aquamarine"_sv },
	{ graphics::colors::pale_goldenrod, "pale_goldenrod"_sv },
	{ graphics::colors::navajo_white, "navajo_white"_sv },
	{ graphics::colors::wheat, "wheat"_sv },
	{ graphics::colors::hot_pink, "hot_pink"_sv },
	{ graphics::colors::steel_blue, "steel_blue"_sv },
	{ graphics::colors::moccasin, "moccasin"_sv },
	{ graphics::colors::peach_puff, "peach_puff"_sv },
	{ graphics::colors::silver, "silver"_sv },
	{ graphics::colors::light_pink, "light_pink"_sv },
	{ graphics::colors::bisque, "bisque"_sv },
	{ graphics::colors::pink, "pink"_sv },
	{ graphics::colors::dark_orchid, "dark_orchid"_sv },
	{ graphics::colors::medium_turquoise, "medium_turquoise"_sv },
	{ graphics::colors::medium_blue, "medium_blue"_sv },
	{ graphics::colors::slate_blue, "slate_blue"_sv },
	{ graphics::colors::blanched_almond, "blanched_almond"_sv },
	{ graphics::colors::lemon_chiffon, "lemon_chiffon"_sv },
	{ graphics::colors::turquoise, "turquoise"_sv },
	{ graphics::colors::dark_turquoise, "dark_turquoise"_sv },
	{ graphics::colors::light_goldenrod_yellow, "light_goldenrod_yellow"_sv },
	{ graphics::colors::dark_violet, "dark_violet"_sv },
	{ graphics::colors::medium_orchid, "medium_orchid"_sv },
	{ graphics::colors::light_gray, "light_gray"_sv },
	{ graphics::colors::aquamarine, "aquamarine"_sv },
	{ graphics::colors::papaya_whip, "papaya_whip"_sv },
	{ graphics::colors::orchid, "orchid"_sv },
	{ graphics::colors::antique_white, "antique_white"_sv },
	{ graphics::colors::thistle, "thistle"_sv },
	{ graphics::colors::medium_purple, "medium_purple"_sv },
	{ graphics::colors::gainsboro, "gainsboro"_sv },
	{ graphics::colors::beige, "beige"_sv },
	{ graphics::colors::cornsilk, "cornsilk"_sv },
	{ graphics::colors::plum, "plum"_sv },
	{ graphics::colors::light_steel_blue, "light_steel_blue"_sv },
	{ graphics::colors::light_yellow, "light_yellow"_sv },
	{ graphics::colors::royal_blue, "royal_blue"_sv },
	{ graphics::colors::misty_rose, "misty_rose"_sv },
	{ graphics::colors::blue_violet, "blue_violet"_sv },
	{ graphics::colors::light_blue, "light_blue"_sv },
	{ graphics::colors::powder_blue, "powder_blue"_sv },
	{ graphics::colors::linen, "linen"_sv },
	{ graphics::colors::old_lace, "old_lace"_sv },
	{ graphics::colors::sky_blue, "sky_blue"_sv },
	{ graphics::colors::cornflower_blue, "cornflower_blue"_sv },
	{ graphics::colors::medium_slate_blue, "medium_slate_blue"_sv },
	{ graphics::colors::violet, "violet"_sv },
	{ graphics::colors::pale_turquoise, "pale_turquoise"_sv },
	{ graphics::colors::sea_shell, "sea_shell"_sv },
	{ graphics::colors::floral_white, "floral_white"_sv },
	{ graphics::colors::honeydew, "honeydew"_sv },
	{ graphics::colors::ivory, "ivory"_sv },
	{ graphics::colors::lavender_blush, "lavender_blush"_sv },
	{ graphics::colors::white_smoke, "white_smoke"_sv },
	{ graphics::colors::light_sky_blue, "light_sky_blue"_sv },
	{ graphics::colors::lavender, "lavender"_sv },
	{ graphics::colors::snow, "snow"_sv },
	{ graphics::colors::mint_cream, "mint_cream"_sv },
	{ graphics::colors::blue, "blue"_sv },
	{ graphics::colors::fuchsia, "fuchsia"_sv },
	{ graphics::colors::magenta, "magenta"_sv },
	{ graphics::colors::dodger_blue, "dodger_blue"_sv },
	{ graphics::colors::deep_sky_blue, "deep_sky_blue"_sv },
	{ graphics::colors::alice_blue, "alice_blue"_sv },
	{ graphics::colors::ghost_white, "ghost_white"_sv },
	{ graphics::colors::cyan, "cyan"_sv },
	{ graphics::colors::aqua, "aqua"_sv },
	{ graphics::colors::light_cyan, "light_cyan"_sv },
	{ graphics::colors::azure, "azure"_sv },
	{ graphics::colors::white, "white"_sv },
};

static collections::pair<castr_t, graphics::colors> s_color_parsing_map[] =
{
	{ "alice_blue"_sv, graphics::colors::alice_blue },
	{ "antique_white"_sv, graphics::colors::antique_white },
	{ "aqua"_sv, graphics::colors::aqua },
	{ "aquamarine"_sv, graphics::colors::aquamarine },
	{ "azure"_sv, graphics::colors::azure },
	{ "beige"_sv, graphics::colors::beige },
	{ "bisque"_sv, graphics::colors::bisque },
	{ "black"_sv, graphics::colors::black },
	{ "blanched_almond"_sv, graphics::colors::blanched_almond },
	{ "blue"_sv, graphics::colors::blue },
	{ "blue_violet"_sv, graphics::colors::blue_violet },
	{ "brown"_sv, graphics::colors::brown },
	{ "burly_wood"_sv, graphics::colors::burly_wood },
	{ "cadet_blue"_sv, graphics::colors::cadet_blue },
	{ "chartreuse"_sv, graphics::colors::chartreuse },
	{ "chocolate"_sv, graphics::colors::chocolate },
	{ "coral"_sv, graphics::colors::coral },
	{ "cornflower_blue"_sv, graphics::colors::cornflower_blue },
	{ "cornsilk"_sv, graphics::colors::cornsilk },
	{ "crimson"_sv, graphics::colors::crimson },
	{ "cyan"_sv, graphics::colors::cyan },
	{ "dark_blue"_sv, graphics::colors::dark_blue },
	{ "dark_cyan"_sv, graphics::colors::dark_cyan },
	{ "dark_goldenrod"_sv, graphics::colors::dark_goldenrod },
	{ "dark_gray"_sv, graphics::colors::dark_gray },
	{ "dark_green"_sv, graphics::colors::dark_green },
	{ "dark_khaki"_sv, graphics::colors::dark_khaki },
	{ "dark_magenta"_sv, graphics::colors::dark_magenta },
	{ "dark_olive_green"_sv, graphics::colors::dark_olive_green },
	{ "dark_orange"_sv, graphics::colors::dark_orange },
	{ "dark_orchid"_sv, graphics::colors::dark_orchid },
	{ "dark_red"_sv, graphics::colors::dark_red },
	{ "dark_salmon"_sv, graphics::colors::dark_salmon },
	{ "dark_sea_green"_sv, graphics::colors::dark_sea_green },
	{ "dark_slate_blue"_sv, graphics::colors::dark_slate_blue },
	{ "dark_slate_gray"_sv, graphics::colors::dark_slate_gray },
	{ "dark_turquoise"_sv, graphics::colors::dark_turquoise },
	{ "dark_violet"_sv, graphics::colors::dark_violet },
	{ "deep_pink"_sv, graphics::colors::deep_pink },
	{ "deep_sky_blue"_sv, graphics::colors::deep_sky_blue },
	{ "dim_gray"_sv, graphics::colors::dim_gray },
	{ "dodger_blue"_sv, graphics::colors::dodger_blue },
	{ "firebrick"_sv, graphics::colors::firebrick },
	{ "floral_white"_sv, graphics::colors::floral_white },
	{ "forest_green"_sv, graphics::colors::forest_green },
	{ "fuchsia"_sv, graphics::colors::fuchsia },
	{ "gainsboro"_sv, graphics::colors::gainsboro },
	{ "ghost_white"_sv, graphics::colors::ghost_white },
	{ "gold"_sv, graphics::colors::gold },
	{ "goldenrod"_sv, graphics::colors::goldenrod },
	{ "gray"_sv, graphics::colors::gray },
	{ "green"_sv, graphics::colors::green },
	{ "green_yellow"_sv, graphics::colors::green_yellow },
	{ "honeydew"_sv, graphics::colors::honeydew },
	{ "hot_pink"_sv, graphics::colors::hot_pink },
	{ "indian_red"_sv, graphics::colors::indian_red },
	{ "indigo"_sv, graphics::colors::indigo },
	{ "ivory"_sv, graphics::colors::ivory },
	{ "khaki"_sv, graphics::colors::khaki },
	{ "lavender"_sv, graphics::colors::lavender },
	{ "lavender_blush"_sv, graphics::colors::lavender_blush },
	{ "lawn_green"_sv, graphics::colors::lawn_green },
	{ "lemon_chiffon"_sv, graphics::colors::lemon_chiffon },
	{ "light_blue"_sv, graphics::colors::light_blue },
	{ "light_coral"_sv, graphics::colors::light_coral },
	{ "light_cyan"_sv, graphics::colors::light_cyan },
	{ "light_goldenrod_yellow"_sv, graphics::colors::light_goldenrod_yellow },
	{ "light_gray"_sv, graphics::colors::light_gray },
	{ "light_green"_sv, graphics::colors::light_green },
	{ "light_pink"_sv, graphics::colors::light_pink },
	{ "light_salmon"_sv, graphics::colors::light_salmon },
	{ "light_sea_green"_sv, graphics::colors::light_sea_green },
	{ "light_sky_blue"_sv, graphics::colors::light_sky_blue },
	{ "light_slate_gray"_sv, graphics::colors::light_slate_gray },
	{ "light_steel_blue"_sv, graphics::colors::light_steel_blue },
	{ "light_yellow"_sv, graphics::colors::light_yellow },
	{ "lime"_sv, graphics::colors::lime },
	{ "lime_green"_sv, graphics::colors::lime_green },
	{ "linen"_sv, graphics::colors::linen },
	{ "magenta"_sv, graphics::colors::magenta },
	{ "maroon"_sv, graphics::colors::maroon },
	{ "medium_aquamarine"_sv, graphics::colors::medium_aquamarine },
	{ "medium_blue"_sv, graphics::colors::medium_blue },
	{ "medium_orchid"_sv, graphics::colors::medium_orchid },
	{ "medium_purple"_sv, graphics::colors::medium_purple },
	{ "medium_sea_green"_sv, graphics::colors::medium_sea_green },
	{ "medium_slate_blue"_sv, graphics::colors::medium_slate_blue },
	{ "medium_spring_green"_sv, graphics::colors::medium_spring_green },
	{ "medium_turquoise"_sv, graphics::colors::medium_turquoise },
	{ "medium_violet_red"_sv, graphics::colors::medium_violet_red },
	{ "midnight_blue"_sv, graphics::colors::midnight_blue },
	{ "mint_cream"_sv, graphics::colors::mint_cream },
	{ "misty_rose"_sv, graphics::colors::misty_rose },
	{ "moccasin"_sv, graphics::colors::moccasin },
	{ "navajo_white"_sv, graphics::colors::navajo_white },
	{ "navy"_sv, graphics::colors::navy },
	{ "null"_sv, graphics::colors::null },
	{ "old_lace"_sv, graphics::colors::old_lace },
	{ "olive"_sv, graphics::colors::olive },
	{ "olive_drab"_sv, graphics::colors::olive_drab },
	{ "orange"_sv, graphics::colors::orange },
	{ "orange_red"_sv, graphics::colors::orange_red },
	{ "orchid"_sv, graphics::colors::orchid },
	{ "pale_goldenrod"_sv, graphics::colors::pale_goldenrod },
	{ "pale_green"_sv, graphics::colors::pale_green },
	{ "pale_turquoise"_sv, graphics::colors::pale_turquoise },
	{ "pale_violet_red"_sv, graphics::colors::pale_violet_red },
	{ "papaya_whip"_sv, graphics::colors::papaya_whip },
	{ "peach_puff"_sv, graphics::colors::peach_puff },
	{ "peru"_sv, graphics::colors::peru },
	{ "pink"_sv, graphics::colors::pink },
	{ "plum"_sv, graphics::colors::plum },
	{ "powder_blue"_sv, graphics::colors::powder_blue },
	{ "purple"_sv, graphics::colors::purple },
	{ "red"_sv, graphics::colors::red },
	{ "rosy_brown"_sv, graphics::colors::rosy_brown },
	{ "royal_blue"_sv, graphics::colors::royal_blue },
	{ "saddle_brown"_sv, graphics::colors::saddle_brown },
	{ "salmon"_sv, graphics::colors::salmon },
	{ "sandy_brown"_sv, graphics::colors::sandy_brown },
	{ "sea_green"_sv, graphics::colors::sea_green },
	{ "sea_shell"_sv, graphics::colors::sea_shell },
	{ "sienna"_sv, graphics::colors::sienna },
	{ "silver"_sv, graphics::colors::silver },
	{ "sky_blue"_sv, graphics::colors::sky_blue },
	{ "slate_blue"_sv, graphics::colors::slate_blue },
	{ "slate_gray"_sv, graphics::colors::slate_gray },
	{ "snow"_sv, graphics::colors::snow },
	{ "spring_green"_sv, graphics::colors::spring_green },
	{ "steel_blue"_sv, graphics::colors::steel_blue },
	{ "tan"_sv, graphics::colors::tan },
	{ "teal"_sv, graphics::colors::teal },
	{ "thistle"_sv, graphics::colors::thistle },
	{ "tomato"_sv, graphics::colors::tomato },
	{ "transparent"_sv, graphics::colors::transparent },
	{ "turquoise"_sv, graphics::colors::turquoise },
	{ "violet"_sv, graphics::colors::violet },
	{ "wheat"_sv, graphics::colors::wheat },
	{ "white"_sv, graphics::colors::white },
	{ "white_smoke"_sv, graphics::colors::white_smoke },
	{ "yellow"_sv, graphics::colors::yellow },

};

using namespace ang::graphics;

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

color_t  graphics::color::parse(ang::cstr_t cstr)
{
	wsize i = 0;
	if (text::encoder::to_char32(cstr, i) == U'#')
	{
		return (dword)text::parser::to_signed(cstr, i, true, 16);
	}
	else
	{
		wsize idx = algorithms::binary_search(cstr, collections::to_array(s_color_parsing_map));
		if (idx > algorithms::array_size(s_color_parsing_map))
			return colors::null;
		else
			return s_color_parsing_map[idx].value;
	}
}

string graphics::color::to_string()const
{
	wsize idx = algorithms::binary_search<dword, collections::pair<graphics::colors, cstr_t>>(code, s_color_to_string_map);
	if (idx > algorithms::array_size(s_color_to_string_map))
	{
		string out = ""_r;
		out->format(L"#{0i:N08,X,F0}"_r, code);
		return out.get();
	}
	else
	{
		return s_color_to_string_map[idx].value;
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

ANG_ENUM_IMPLEMENT(ang::graphics, graph_driver_type);
ANG_ENUM_IMPLEMENT(ang::graphics, primitive);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::graphics::colors);