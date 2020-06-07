#include "pch.h"

#include <ang/graphics/graphics.h>
#include <ang/graphics/drawing.h>

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::textures;


//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::textures::itexture, buffers::igpu_buffer);
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::textures::itexture_loader, resources::ilibrary);

ANG_ENUM_IMPLEMENT(ang::graphics::textures, tex_type);
ANG_ENUM_IMPLEMENT(ang::graphics::textures, tex_format);
ANG_ENUM_IMPLEMENT(ang::graphics::textures, tex_wrap_mode);
ANG_ENUM_IMPLEMENT(ang::graphics::textures, tex_stretch_mode);

#define MY_TYPE ang::graphics::textures::itexture
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static collections::pair<graphics::textures::tex_type, castr_t> s_to_string_tex_type_map[] =
{
	{ tex_type::null, "null"_sv },
	{ tex_type::tex1D, "tex1D,"_sv },
	{ tex_type::tex2D, "tex2D,"_sv },
	{ tex_type::tex3D, "tex3D,"_sv },
	{ tex_type::texCube, "texCube,"_sv },
};

static collections::pair<castr_t, graphics::textures::tex_type> s_parse_tex_type_map[] =
{
	{ "null"_sv, tex_type::null },
	{ "tex1D"_sv, tex_type::tex1D },
	{ "tex2D"_sv, tex_type::tex2D },
	{ "tex3D"_sv, tex_type::tex3D },
	{ "texCube"_sv, tex_type::texCube },
};

tex_type_t graphics::textures::tex_type_t::parse(cstr_t cstr)
{
	wsize idx = algorithms::binary_search(cstr, to_array(s_parse_tex_type_map));
	if (idx > algorithms::array_size(s_parse_tex_type_map))
		return tex_type::null;
	else
		return s_parse_tex_type_map[idx].value;
}

castr_t graphics::textures::tex_type_t::to_string()const
{
	wsize idx = algorithms::binary_search(get(), to_array(s_to_string_tex_type_map));
	if (idx > algorithms::array_size(s_to_string_tex_type_map))
		return "null"_sv;
	else
		return s_to_string_tex_type_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static collections::pair<graphics::textures::tex_format, castr_t> s_to_string_tex_format_map[] =
{
	{ tex_format::null, "null"_sv },
	{ tex_format::R32G32B32, "R32G32B32,"_sv },
	{ tex_format::R32G32B32A32, "R32G32B32A32,"_sv },
	{ tex_format::R10G310B10A2, "R10G310B10A2,"_sv },
	{ tex_format::A8, "A8,"_sv },
	{ tex_format::R8G8B8, "R8G8B8,"_sv },
	{ tex_format::R8G8B8A8, "R8G8B8A8,"_sv },
	{ tex_format::R5G6B5, "R5G6B5,"_sv },
	{ tex_format::R5G5B5A1, "R5G5B5A1,"_sv },
	{ tex_format::R4G4B4A4, "R4G4B4A4,"_sv },
	{ tex_format::D16, "D16,"_sv },
	{ tex_format::D24S8, "D24S8,"_sv }
};

static collections::pair<castr_t, graphics::textures::tex_format> s_parse_tex_format_map[] =
{
	{ "A8"_sv, tex_format::A8 },
	{ "D16"_sv, tex_format::D16 },
	{ "D24S8"_sv, tex_format::D24S8 },
	{ "R10G310B10A2"_sv, tex_format::R10G310B10A2 },
	{ "R32G32B32"_sv, tex_format::R32G32B32 },
	{ "R32G32B32A32"_sv, tex_format::R32G32B32A32 },
	{ "R4G4B4A4"_sv, tex_format::R4G4B4A4 },
	{ "R5G6B5"_sv, tex_format::R5G6B5 },
	{ "R5G5B5A1"_sv, tex_format::R5G5B5A1 },
	{ "R8G8B8"_sv, tex_format::R8G8B8 },
	{ "R8G8B8A8"_sv, tex_format::R8G8B8A8 },
	{ "null"_sv, tex_format::null },
};

tex_format_t graphics::textures::tex_format_t::parse(cstr_t cstr)
{
	wsize idx = algorithms::binary_search(cstr, to_array(s_parse_tex_format_map));
	if (idx > algorithms::array_size(s_parse_tex_format_map))
		return tex_format::null;
	else
		return s_parse_tex_format_map[idx].value;
}

castr_t graphics::textures::tex_format_t::to_string()const
{
	wsize idx = algorithms::binary_search(get(), to_array(s_to_string_tex_format_map));
	if (idx > algorithms::array_size(s_to_string_tex_format_map))
		return "null"_sv;
	else
		return s_to_string_tex_format_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static collections::pair<graphics::textures::tex_wrap_mode, castr_t> s_to_string_tex_wrap_mode_map[] =
{
	{ tex_wrap_mode::def, "def"_sv },
	{ tex_wrap_mode::repeate, "repeate,"_sv },
	{ tex_wrap_mode::clamp, "clamp,"_sv },
	{ tex_wrap_mode::mirrored, "mirrored,"_sv }
};

static collections::pair<castr_t, graphics::textures::tex_wrap_mode> s_parse_tex_wrap_mode_map[] =
{
	{ "clamp"_sv, tex_wrap_mode::clamp },
	{ "def"_sv, tex_wrap_mode::def },
	{ "mirrored"_sv, tex_wrap_mode::mirrored },
	{ "repeate"_sv, tex_wrap_mode::repeate }
};

tex_wrap_mode_t graphics::textures::tex_wrap_mode_t::parse(cstr_t cstr)
{
	wsize idx = algorithms::binary_search(cstr, to_array(s_parse_tex_wrap_mode_map));
	if (idx > algorithms::array_size(s_parse_tex_wrap_mode_map))
		return tex_wrap_mode::def;
	else
		return s_parse_tex_wrap_mode_map[idx].value;
}

castr_t graphics::textures::tex_wrap_mode_t::to_string()const
{
	wsize idx = algorithms::binary_search(get(), to_array(s_to_string_tex_wrap_mode_map));
	if (idx > algorithms::array_size(s_to_string_tex_wrap_mode_map))
		return "def"_sv;
	else
		return s_to_string_tex_wrap_mode_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

static collections::pair<graphics::textures::tex_stretch_mode, castr_t> s_to_string_tex_stretch_mode_map[] =
{
	{ tex_stretch_mode::none, "none"_sv },
	{ tex_stretch_mode::fill, "fill,"_sv },
	{ tex_stretch_mode::uniform, "uniform,"_sv },
	{ tex_stretch_mode::uniform_to_fill, "uniform_to_fill,"_sv }
};

static collections::pair<castr_t, graphics::textures::tex_stretch_mode> s_parse_tex_stretch_mode_map[] =
{
	{ "fill"_sv, tex_stretch_mode::fill },
	{ "none"_sv, tex_stretch_mode::none },
	{ "uniform"_sv, tex_stretch_mode::uniform },
	{ "uniform_to_fill"_sv, tex_stretch_mode::uniform_to_fill }
};

tex_stretch_mode_t graphics::textures::tex_stretch_mode_t::parse(cstr_t cstr)
{
	wsize idx = algorithms::binary_search(cstr, to_array(s_parse_tex_stretch_mode_map));
	if (idx > algorithms::array_size(s_parse_tex_stretch_mode_map))
		return tex_stretch_mode::none;
	else
		return s_parse_tex_stretch_mode_map[idx].value;
}

castr_t graphics::textures::tex_stretch_mode_t::to_string()const
{
	wsize idx = algorithms::binary_search(get(), to_array(s_to_string_tex_stretch_mode_map));
	if (idx > algorithms::array_size(s_to_string_tex_stretch_mode_map))
		return "none"_sv;
	else
		return s_to_string_tex_stretch_mode_map[idx].value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////