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
				repeate = def,
				clamp,
				mirrored,
			};

			ang_begin_interface(LINK itexture)
				visible vcall text::istring_view_t resource_name()const pure;
				visible vcall tex_type_t tex_type()const pure;
				visible vcall tex_format_t tex_format()const pure;
				visible vcall graphics::size<float> tex_dimentions()const pure;
			ang_end_interface();

			ang_begin_interface(LINK itexture_loader)
				visible vcall bool load_sources(dom::xml::xml_node_t) pure;
				visible vcall void set_file_system(core::files::ifile_system_t) pure;
				visible vcall bool load_library(dom::xml::xml_node_t) pure;
				visible vcall core::async::iasync<itexture_loader_t> load_library_async(dom::xml::xml_node_t) pure;
				visible vcall itexture_t load_texture(text::raw_cstr_t) pure;
				visible vcall itexture_t load_texture(dom::xml::xml_node_t) pure;
				visible vcall core::async::iasync<itexture_t> load_texture_async(text::raw_cstr_t) pure;
				visible vcall core::async::iasync<itexture_t> load_texture_async(dom::xml::xml_node_t) pure;
				visible vcall itexture_t find_texture(text::raw_cstr_t)const pure;
			ang_end_interface();
		}

	}
}

#endif//__ANG_GRAPHICS_TEXTURES_H__