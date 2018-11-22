#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_EFFECTS_H__
#define __ANG_GRAPHICS_EFFECTS_H__

namespace ang
{
	namespace graphics
	{
		namespace effects
		{

			ang_begin_interface(LINK ishaders)
				visible vcall string resource_name()const pure;
				visible vcall array_view<reflect::attribute_desc> input_layout()const pure;
				visible vcall array_view<reflect::variable_desc> vs_uniforms_layouts()const pure;
				visible vcall array_view<reflect::variable_desc> ps_uniforms_layouts()const pure;
				visible vcall reflect::variable map_vs_uniform(idriver_t, index) pure; //only one per time
				visible vcall reflect::variable map_vs_uniform(idriver_t, cstr_t) pure; //only one per time
				visible vcall reflect::variable map_vs_uniform(idriver_t, cwstr_t) pure; //only one per time
				visible vcall reflect::variable map_ps_uniform(idriver_t, index) pure; //only one per time
				visible vcall reflect::variable map_ps_uniform(idriver_t, cstr_t) pure; //only one per time
				visible vcall reflect::variable map_ps_uniform(idriver_t, cwstr_t) pure; //only one per time
				visible vcall void unmap_vs_uniform(idriver_t, reflect::variable&) pure;
				visible vcall void unmap_ps_uniform(idriver_t, reflect::variable&) pure;
			ang_end_interface();

			ang_begin_interface(LINK ieffect)
				visible vcall void draw(iframe_buffer_t)pure;
				visible vcall ishaders_t pass(index)const pure;
			ang_end_interface();

			ang_begin_interface(LINK ieffect_library)
			//	visible vcall bool load_sources(xml::ixml_node_t) pure;
			//	visible vcall void set_file_system(core::files::ifile_system_t) pure;
			//	visible vcall bool load_library(xml::ixml_node_t) pure;
			//	visible vcall core::async::iasync_t<ieffect_library_t> load_library_async(xml::ixml_node_t) pure;
			//	visible vcall ieffect_t load_effect(xml::ixml_node_t) pure;
			//	visible vcall core::async::iasync_t<ieffect_t> load_effect_async(xml::ixml_node_t) pure;	
			//	visible vcall ishaders_t load_shaders(xml::ixml_node_t) pure;
			//	visible vcall core::async::iasync_t<ishaders_t> load_shaders_async(xml::ixml_node_t) pure;
				visible vcall ieffect_t find_effect(cstr_t)const pure;
				visible vcall ieffect_t find_effect(cwstr_t)const pure;
				visible vcall ishaders_t find_shaders(cstr_t)const pure;
				visible vcall ishaders_t find_shaders(cwstr_t)const pure;
			ang_end_interface();
		}
	}
}

#endif//__ANG_GRAPHICS_EFFECTS_H__