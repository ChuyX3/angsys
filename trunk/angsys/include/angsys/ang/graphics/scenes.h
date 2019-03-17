#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_SCENES_H__
#define __ANG_GRAPHICS_SCENES_H__

namespace ang
{
	namespace graphics
	{
		namespace scenes
		{
			ang_begin_interface(LINK icamera)
				/*Sets the Current Viewport*/
				visible vcall void viewport(box<float>) pure;
				/*Gets the Current Viewport*/
				visible vcall box<float> viewport()const pure;
				/*Gets the View Projection Matrix*/
				visible vcall maths::float4x4 view_projection_matrix()const pure;
			ang_end_interface();


			ang_begin_interface(LINK imodel_element)
				visible vcall string technique() const pure;
				visible vcall buffers::iindex_buffer_t index_buffer() const pure;
				visible vcall buffers::ivertex_buffer_t vertex_buffer() const pure;
				visible vcall reflect::varying_t uniform(cstr_t) const pure;
				//visible vcall buffers::ivertex_buffer_t vertex_buffer() const pure;
				visible vcall array_view<textures::itexture_t> textures() const pure;
			ang_end_interface();

			ang_begin_interface(LINK imodel)
				visible vcall collections::ienum_ptr<imodel_element_t> elements()const pure;
				visible vcall maths::float4x4 world_matrix()const pure;
			ang_end_interface();
		}
	}
}

#endif//__ANG_GRAPHICS_SCENES_H__