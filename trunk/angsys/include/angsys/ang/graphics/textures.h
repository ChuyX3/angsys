#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_TEXTURES_H__
#define __ANG_GRAPHICS_TEXTURES_H__

namespace ang
{
	namespace graphics
	{
		namespace textures
		{

			safe_enum(LINK, tex_type, uint)
			{
				null,
				tex1D,
				tex2D,
				tex3D,
				texCube,
			};

			safe_enum(LINK, tex_format, uint)
			{
				null = 0,
				R32G32B32,
				R32G32B32A32,
				R10G310B10A2,

				A8,
				R8G8B8,
				R8G8B8A8,//Default
				R5G6B5,
				R5G5B5A1,
				R4G4B4A4,
				D16,
				D24S8,
			};
			

			safe_enum(LINK, tex_wrap_mode, uint)
			{
				def = 0,
				repeate = tex_wrap_mode::def,
				clamp,
				mirrored,
			};

			ang_begin_interface(LINK itexture)
				visible vcall string resource_name()const pure;
				visible vcall tex_type_t tex_type()const pure;
				visible vcall tex_format_t tex_format()const pure;
				visible vcall graphics::size<float> tex_dimentions()const pure;
			ang_end_interface();

			ang_begin_interface(LINK itexture_loader) //TODO
			//	visible vcall bool load_sources(xml::ixml_node_t) pure;
			//	visible vcall void set_file_system(core::files::ifile_system_t) pure;
			//	visible vcall bool load_library(xml::ixml_node_t) pure;
			//	visible vcall core::async::iasync_t<itexture_loader_t> load_library_async(xml::ixml_node_t) pure;
				visible vcall itexture_t load_texture(string) pure;
			//	visible vcall itexture_t load_texture(xml::ixml_node_t) pure;
			//	visible vcall core::async::iasync_t<itexture_t> load_texture_async(string) pure;
			//	visible vcall core::async::iasync_t<itexture_t> load_texture_async(xml::ixml_node_t) pure;
				visible vcall itexture_t find_texture(cstr_t)const pure;
				visible vcall itexture_t find_texture(cwstr_t)const pure;
			ang_end_interface();
		}

	}
}

//template<> inline ang::graphics::textures::tex_type_t ang::xml::xml_text::xml_as<ang::graphics::textures::tex_type_t>()const { return ang::graphics::textures::tex_type_t::parse(xml_as<cwstr_t>()); }
//template<> inline ang::graphics::textures::tex_format_t ang::xml::xml_text::xml_as<ang::graphics::textures::tex_format_t>()const { return ang::graphics::textures::tex_format_t::parse(xml_as<cwstr_t>()); }
//template<> inline ang::graphics::textures::tex_wrap_mode_t ang::xml::xml_text::xml_as<ang::graphics::textures::tex_wrap_mode_t>()const { return ang::graphics::textures::tex_wrap_mode_t::parse(xml_as<cwstr_t>()); }


ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ang::graphics::textures::tex_type_t);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ang::graphics::textures::tex_format_t);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ang::graphics::textures::tex_wrap_mode_t);

#endif//__ANG_GRAPHICS_TEXTURES_H__