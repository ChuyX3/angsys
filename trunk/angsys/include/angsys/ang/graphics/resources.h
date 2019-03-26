#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_RESOURCES_H__
#define __ANG_GRAPHICS_RESOURCES_H__

namespace ang
{
	namespace graphics
	{
		namespace resources
		{
			safe_enum(LINK, light_type, uint)
			{
				directional,
				spot,
			};

			struct light_info
			{
				maths::float3 color;
				maths::float3 info;// position or direction
				light_type_t type;
			};

			/*See more: http://paulbourke.net/dataformats/mtl/ */
			typedef struct material
			{
				/*Most common data:
				"uint illum" statement specifies the illumination model to use in the material.
					It can be a number from 0 to 10:
				 0	Color on and Ambient off
				 1	Color on and Ambient on
				 2	Highlight on
				 3	Reflection on and Ray trace on
				 4	Transparency: Glass on
					Reflection: Ray trace on
				 5	Reflection: Fresnel on and Ray trace on
				 6	Transparency: Refraction on
					Reflection: Fresnel off and Ray trace on
				 7	Transparency: Refraction on
					Reflection: Fresnel on and Ray trace on
				 8	Reflection on and Ray trace off
				 9	Transparency: Glass on
					Reflection: Ray trace off
				 10	Casts shadows onto invisible surfaces
				"float d" specifies the dissolve for the current material
				"factor" is the amount this material dissolves into the background.  A
				factor of 1.0 is fully opaque.  This is the default when a new material
				is created.  A factor of 0.0 is fully dissolved (completely
				transparent)
				"d -halo factor" specifies that a dissolve is dependent on the surface orientation
				relative to the viewer.  For example, a sphere with the following dissolve,
				d -halo 0.0, will be fully dissolved at its center and will
				appear gradually more opaque toward its edge.
				"float Ns" exponent specifies the specular exponent for the current material.  This defines
				the focus of the specular highlight.
				"float3 Ka" statement specifies the ambient reflectivity of the current material
				"float3 Kd" statement specifies the diffuse reflectivity of the current material
				"float3 Kd" statement specifies the specular reflectivity of the current material
				"float3 Tf" statement specifies the transmission filter of the current material.
				Any light passing through the object is filtered by the transmission
				filter, which only allows the specifiec colors to pass through*/
				reflect::struct_buffer_t fields;

				/* Most common samplers
				- map_Ka: ambient color texture
				- map_Kd: diffuse color texture
				- map_Ks: specular color texture
				- map_d: alpha channel texture*/
				collections::hash_map<string, string> samplers;
			}material_t;

			struct model_element
			{
				string technique_name;	
				reflect::struct_buffer_t index_data;
				reflect::struct_buffer_t vertex_data;
				material_t material;
			};

			ang_begin_interface(LINK iresource)
				visible vcall resource_type_t resource_type()const pure
				visible vcall string resource_sid()const pure
				visible vcall void resource_sid(cstr_t) pure
				visible vcall effects::ieffect_t to_effect() pure
				visible vcall effects::ishaders_t to_shaders() pure
				visible vcall textures::itexture_t to_texture() pure
				visible vcall iframe_buffer_t to_frame_buffer() pure
				visible vcall buffers::iindex_buffer_t to_index_buffer() pure
				visible vcall buffers::ivertex_buffer_t to_vertex_buffer() pure
			ang_end_interface();

			ang_begin_interface(LINK imodel_loader)
				visible vcall void clear()pure
				visible vcall bool load(dom::xml::xml_node_t)pure
				visible vcall bool save(dom::xml::xml_document_t)const pure
				visible vcall bool load(core::files::input_text_file_t)pure
				visible vcall bool save(core::files::output_text_file_t)const pure
				visible vcall bool load(core::files::input_binary_file_t)pure
				visible vcall bool save(core::files::output_binary_file_t)const pure
				visible vcall core::async::iasync<bool> load_async(dom::xml::xml_node_t)pure
				visible vcall core::async::iasync<bool> save_async(dom::xml::xml_document_t)const pure
				visible vcall core::async::iasync<bool> load_async(core::files::input_text_file_t)pure
				visible vcall core::async::iasync<bool> save_async(core::files::output_text_file_t)const pure
				visible vcall core::async::iasync<bool> load_async(core::files::input_binary_file_t)pure
				visible vcall core::async::iasync<bool> save_async(core::files::output_binary_file_t)const pure
				visible vcall collections::ienum_ptr<model_element> elements()const pure
			ang_end_interface();

			ang_begin_interface(LINK ilibrary)
				visible vcall bool load(dom::xml::xml_node_t)pure
				visible vcall bool save(dom::xml::xml_document_t)const pure
				visible vcall core::async::iasync<bool> load_async(dom::xml::xml_node_t)pure
				visible vcall core::async::iasync<bool> save_async(dom::xml::xml_document_t)const pure
				visible vcall bool load_sources(dom::xml::xml_node_t) pure
				visible vcall ilibrary_t load_library(dom::xml::xml_node_t) pure
				visible vcall iresource_t load_resource(dom::xml::xml_node_t) pure
				visible vcall core::async::iasync<ilibrary_t> load_library_async(dom::xml::xml_node_t) pure
				visible vcall core::async::iasync<iresource_t> load_resource_async(dom::xml::xml_node_t) pure
				visible vcall ifactory_t factory()const pure
				visible vcall core::async::idispatcher_t dispatcher()const pure
				visible vcall string find_source(cstr_t)const pure
				visible vcall iresource_t find_resource(cstr_t)const pure
				visible vcall void clear()pure
			ang_end_interface();
		}
	}
}

#endif//__ANG_GRAPHICS_RESOURCES_H__