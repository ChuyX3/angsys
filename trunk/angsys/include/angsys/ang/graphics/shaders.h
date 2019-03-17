#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_SHADERS_H__
#define __ANG_GRAPHICS_SHADERS_H__

namespace ang
{
	namespace graphics
	{
		namespace effects
		{
			typedef struct shader_info : auto_self<shader_info>
			{
				string file;
				astring code;
				astring entry;
				astring compile_config;
			}shader_info_t;

			ang_begin_interface(LINK ishaders)
				visible vcall resources::iresource_t resource()const pure;
				visible vcall array_view<reflect::attribute_desc> input_layout()const pure;
				visible vcall array_view<reflect::varying_desc> vs_uniforms_layouts()const pure;
				visible vcall array_view<reflect::varying_desc> ps_uniforms_layouts()const pure;
				visible vcall array_view<string> textures()const pure;
				visible vcall reflect::varying map_vs_uniform(idriver_t, index) pure; //only one per time
				visible vcall reflect::varying map_vs_uniform(idriver_t, cstr_t) pure; //only one per time
				visible vcall reflect::varying map_ps_uniform(idriver_t, index) pure; //only one per time
				visible vcall reflect::varying map_ps_uniform(idriver_t, cstr_t) pure; //only one per time
				visible vcall void unmap_vs_uniform(idriver_t, reflect::varying&) pure;
				visible vcall void unmap_ps_uniform(idriver_t, reflect::varying&) pure;
			ang_end_interface();

			ang_begin_interface(LINK ipass)
				//visible vcall void draw(iframe_buffer_t)pure;
				//visible vcall ishaders_t technique()const pure;
			ang_end_interface();

			ang_begin_interface(LINK ieffect)
				//visible vcall void draw(iframe_buffer_t)pure;
				//visible vcall ipass_t pass(index)const pure;
			ang_end_interface();
		}
	}
}

#endif//__ANG_GRAPHICS_SHADERS_H__