#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_TEXTURES_H__
#define __ANG_GRAPHICS_TEXTURES_H__

namespace ang
{
	namespace graphics
	{
		namespace textures
		{
			struct nvt LINK itexture
				: intf<itexture
				, iid("ang::graphics::textures::itexture")
				, buffers::igpu_buffer>
			{
				virtual tex_type_t tex_type()const = 0;
				virtual tex_format_t tex_format()const = 0;
				virtual size3d<uint> tex_dimentions()const = 0;
			};

			struct nvt LINK itexture_loader
				: intf<itexture_loader
				, iid("ang::graphics::textures::itexture_loader")
				, resources::ilibrary>
			{
				virtual optional<itexture> load_texture(cstr_t sid) = 0;
				virtual bool load_texture_info(dom::xml::ixml_node_t) = 0;
				virtual optional<itexture> load_texture(dom::xml::ixml_node_t) = 0;
				virtual core::async::iasync_op<itexture> load_texture_async(cstr_t sid) = 0;
				virtual core::async::iasync<bool> load_texture_info_async(dom::xml::ixml_node_t) = 0;
				virtual core::async::iasync_op<itexture> load_texture_async(dom::xml::ixml_node_t) = 0;
				virtual itexture_t find_texture(cstr_t)const = 0;
			};
		}

	}
}

#endif//__ANG_GRAPHICS_TEXTURES_H__