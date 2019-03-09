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
				visible vcall text::istring_view_t resource_name()const pure;
				visible vcall array_view<reflect::attribute_desc> input_layout()const pure;
				visible vcall array_view<reflect::varying_desc> vs_uniforms_layouts()const pure;
				visible vcall array_view<reflect::varying_desc> ps_uniforms_layouts()const pure;
				visible vcall reflect::varying map_vs_uniform(idriver_t, index) pure; //only one per time
				visible vcall reflect::varying map_vs_uniform(idriver_t, text::raw_cstr_t) pure; //only one per time
				visible vcall reflect::varying map_ps_uniform(idriver_t, index) pure; //only one per time
				visible vcall reflect::varying map_ps_uniform(idriver_t, text::raw_cstr_t) pure; //only one per time
				visible vcall void unmap_vs_uniform(idriver_t, reflect::varying&) pure;
				visible vcall void unmap_ps_uniform(idriver_t, reflect::varying&) pure;
			ang_end_interface();

			ang_begin_interface(LINK ieffect)
				visible vcall void draw(iframe_buffer_t)pure;
				visible vcall ishaders_t pass(index)const pure;
			ang_end_interface();

			ang_begin_interface(LINK ieffect_library)
				visible vcall bool load_sources(dom::xml::xml_node_t) pure;
				visible vcall void set_file_system(core::files::ifile_system_t) pure;
				visible vcall bool load_library(dom::xml::xml_node_t) pure;
				visible vcall core::async::iasync<ieffect_library_t> load_library_async(dom::xml::xml_node_t) pure;
				visible vcall ieffect_t load_effect(dom::xml::xml_node_t) pure;
				visible vcall core::async::iasync<ieffect_t> load_effect_async(dom::xml::xml_node_t) pure;
				visible vcall ishaders_t load_shaders(dom::xml::xml_node_t) pure;
				visible vcall core::async::iasync<ishaders_t> load_shaders_async(dom::xml::xml_node_t) pure;
				visible vcall ieffect_t find_effect(text::raw_cstr_t)const pure;
				visible vcall ishaders_t find_shaders(text::raw_cstr_t)const pure;
			ang_end_interface();
		}
	}
}

#endif//__ANG_GRAPHICS_EFFECTS_H__