#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_TEXTURES_H__
#define __ANG_GRAPHICS_TEXTURES_H__

namespace ang
{
	namespace graphics
	{
		namespace textures
		{
	
			ang_begin_interface(LINK itexture, buffers::igpu_buffer)
				visible vcall tex_type_t tex_type()const pure;
				visible vcall tex_format_t tex_format()const pure;
				visible vcall size3d<uint> tex_dimentions()const pure;
			ang_end_interface();

			ang_begin_interface(LINK itexture_loader, resources::ilibrary)
				visible vcall itexture_t load_texture(cstr_t sid) pure;
				visible vcall itexture_t load_texture(dom::xml::xml_node_t) pure;
				visible vcall core::async::iasync<itexture_t> load_texture_async(cstr_t sid) pure;
				visible vcall core::async::iasync<itexture_t> load_texture_async(dom::xml::xml_node_t) pure;
				visible vcall itexture_t find_texture(cstr_t)const pure;
			ang_end_interface();
		}

	}
}

#endif//__ANG_GRAPHICS_TEXTURES_H__