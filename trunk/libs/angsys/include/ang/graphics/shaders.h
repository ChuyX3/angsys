#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_SHADERS_H__
#define __ANG_GRAPHICS_SHADERS_H__

namespace ang
{
	namespace graphics
	{
		namespace effects
		{
			typedef struct shader_info
			{
				string file;
				astring code;
				astring entry;
				astring compile_config;
			}shader_info_t;

			struct nvt LINK ishaders
				: intf<ishaders
				, iid("ang::graphics::effects::ishaders")>
			{
				virtual resources::iresource_t resource()const = 0;
				virtual array_view<reflect::attribute_desc>const& input_layout()const = 0;
				virtual array_view<reflect::varying_desc>const& vs_uniforms_layouts()const = 0;
				virtual array_view<reflect::varying_desc>const& ps_uniforms_layouts()const = 0;
				virtual bool bind_vertex_buffer(idriver_t, buffers::ivertex_buffer_t) = 0;
				virtual bool bind_texture(idriver_t, windex, windex) = 0;
				virtual bool bind_texture(idriver_t, cstr_t, windex) = 0;
				virtual reflect::varying map_vs_uniform(idriver_t, windex) = 0; //only one per time
				virtual reflect::varying map_vs_uniform(idriver_t, cstr_t) = 0; //only one per time
				virtual reflect::varying map_ps_uniform(idriver_t, windex) = 0; //only one per time
				virtual reflect::varying map_ps_uniform(idriver_t, cstr_t) = 0; //only one per time
				virtual void unmap_vs_uniform(idriver_t, reflect::varying&) = 0;
				virtual void unmap_ps_uniform(idriver_t, reflect::varying&) = 0;
			};

			struct nvt LINK ipass
				: intf<ipass
				, iid("ang::graphics::effects::ipass")>
			{
				virtual void draw(idriver_t, scenes::iscene_t, meshes::igeometry_t) = 0;
				virtual ishaders_t shaders()const = 0;
				virtual string frame_buffer()const = 0;
				virtual string geometry()const = 0;
				virtual array<string> resources()const = 0;
			};

			struct nvt LINK ieffect
				: intf<ieffect
				, iid("ang::graphics::effects::ieffect")>
			{
				virtual resources::iresource_t resource()const = 0;
				virtual void draw(idriver_t, iframe_buffer_t, scenes::iscene_t, scenes::inode_t) = 0;
				virtual wsize pass_count()const = 0;
				virtual ipass_t pass(windex)const = 0;
				virtual iframe_buffer_t frame_buffer(cstr_t)const = 0;
				virtual iframe_buffer_t frame_buffer(cstr_t, size<uint> resize)const = 0;
			};

			struct nvt LINK ieffect_library
				: intf<ieffect_library
				, iid("ang::graphics::effects::ieffect_library")
				, resources::ilibrary>
			{
				virtual optional<ieffect> load_effect(dom::xml::ixml_node_t) = 0;;
				virtual core::async::iasync_op<ieffect> load_effect_async(dom::xml::ixml_node_t) = 0;;
				virtual optional<ishaders> load_technique(dom::xml::ixml_node_t) = 0;;
				virtual core::async::iasync_op<ishaders> load_technique_async(dom::xml::ixml_node_t) = 0;;
				virtual ieffect_t find_effect(cstr_t)const = 0;
				virtual ishaders_t find_technique(cstr_t)const = 0;
			};
		}
	}
}

#endif//__ANG_GRAPHICS_SHADERS_H__