#include "pch.h"

#include <ang/graphics/graphics.h>

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::textures;


ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::textures::itexture, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::textures::itexture_loader, interface);


safe_enum_rrti2(ang::graphics::textures, tex_type);
safe_enum_rrti2(ang::graphics::textures, tex_format);
safe_enum_rrti2(ang::graphics::textures, tex_wrap_mode);

#define MY_TYPE ang::graphics::textures::itexture
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::graphics::textures::itexture_loader
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static collections::pair<graphics::textures::tex_type, cstr_t> s_to_string_tex_type_map[] =
{
	{ tex_type::null, "null"_s },
	{ tex_type::tex1D, "tex1D,"_s },
	{ tex_type::tex2D, "tex2D,"_s },
	{ tex_type::tex3D, "tex3D,"_s },
	{ tex_type::texCube, "texCube,"_s },
};

static collections::pair<cstr_t, graphics::textures::tex_type> s_parse_tex_type_map[] =
{
	{ "null"_s, tex_type::null },
	{ "tex1D"_s, tex_type::tex1D },
	{ "tex2D"_s, tex_type::tex2D },
	{ "tex3D"_s, tex_type::tex3D },
	{ "texCube"_s, tex_type::texCube },
};


tex_type_t graphics::textures::tex_type_t::parse(text::raw_cstr_t cstr)
{
	wsize idx = algorithms::binary_search(cstr, to_array(s_parse_tex_type_map));
	if (idx > algorithms::array_size(s_parse_tex_type_map))
		return tex_type::null;
	else
		return s_parse_tex_type_map[idx].value;
}

cstr_t graphics::textures::tex_type_t::to_string()const
{
	wsize idx = algorithms::binary_search(get(), to_array(s_to_string_tex_type_map));
	if (idx > algorithms::array_size(s_to_string_tex_type_map))
		return "null"_s;
	else
		return s_to_string_tex_type_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static collections::pair<graphics::textures::tex_format, cstr_t> s_to_string_tex_format_map[] =
{
	{ tex_format::null, "null"_s },
	{ tex_format::R32G32B32, "R32G32B32,"_s },
	{ tex_format::R32G32B32A32, "R32G32B32A32,"_s },
	{ tex_format::R10G310B10A2, "R10G310B10A2,"_s },
	{ tex_format::A8, "A8,"_s },
	{ tex_format::R8G8B8, "R8G8B8,"_s },
	{ tex_format::R8G8B8A8, "R8G8B8A8,"_s },
	{ tex_format::R5G6B5, "R5G6B5,"_s },
	{ tex_format::R5G5B5A1, "R5G5B5A1,"_s },
	{ tex_format::R4G4B4A4, "R4G4B4A4,"_s },
	{ tex_format::D16, "D16,"_s },
	{ tex_format::D24S8, "D24S8,"_s }
};

static collections::pair<cstr_t, graphics::textures::tex_format> s_parse_tex_format_map[] =
{
	{ "A8"_s, tex_format::A8 },
	{ "D16"_s, tex_format::D16 },
	{ "D24S8"_s, tex_format::D24S8 },
	{ "R10G310B10A2"_s, tex_format::R10G310B10A2 },
	{ "R32G32B32"_s, tex_format::R32G32B32 },
	{ "R32G32B32A32"_s, tex_format::R32G32B32A32 },
	{ "R4G4B4A4"_s, tex_format::R4G4B4A4 },
	{ "R5G6B5"_s, tex_format::R5G6B5 },
	{ "R5G5B5A1"_s, tex_format::R5G5B5A1 },
	{ "R8G8B8"_s, tex_format::R8G8B8 },
	{ "R8G8B8A8"_s, tex_format::R8G8B8A8 },
	{ "null"_s, tex_format::null },
};

tex_format_t graphics::textures::tex_format_t::parse(text::raw_cstr_t cstr)
{
	wsize idx = algorithms::binary_search(cstr, to_array(s_parse_tex_format_map));
	if (idx > algorithms::array_size(s_parse_tex_format_map))
		return tex_format::null;
	else
		return s_parse_tex_format_map[idx].value;
}

cstr_t graphics::textures::tex_format_t::to_string()const
{
	wsize idx = algorithms::binary_search(get(), to_array(s_to_string_tex_format_map));
	if (idx > algorithms::array_size(s_to_string_tex_format_map))
		return "null"_s;
	else
		return s_to_string_tex_format_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static collections::pair<graphics::textures::tex_wrap_mode, cstr_t> s_to_string_tex_wrap_mode_map[] =
{
	{ tex_wrap_mode::def, "def"_s },
	{ tex_wrap_mode::repeate, "repeate,"_s },
	{ tex_wrap_mode::clamp, "clamp,"_s },
	{ tex_wrap_mode::mirrored, "mirrored,"_s }
};

static collections::pair<cstr_t, graphics::textures::tex_wrap_mode> s_parse_tex_wrap_mode_map[] =
{
	{ "clamp"_s, tex_wrap_mode::clamp },
	{ "def"_s, tex_wrap_mode::def },
	{ "mirrored"_s, tex_wrap_mode::mirrored },
	{ "repeate"_s, tex_wrap_mode::repeate }
};


tex_wrap_mode_t graphics::textures::tex_wrap_mode_t::parse(text::raw_cstr_t cstr)
{
	wsize idx = algorithms::binary_search(cstr, to_array(s_parse_tex_wrap_mode_map));
	if (idx > algorithms::array_size(s_parse_tex_wrap_mode_map))
		return tex_wrap_mode::def;
	else
		return s_parse_tex_wrap_mode_map[idx].value;
}


cstr_t graphics::textures::tex_wrap_mode_t::to_string()const
{
	wsize idx = algorithms::binary_search(get(), to_array(s_to_string_tex_wrap_mode_map));
	if (idx > algorithms::array_size(s_to_string_tex_wrap_mode_map))
		return "def"_s;
	else
		return s_to_string_tex_wrap_mode_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////