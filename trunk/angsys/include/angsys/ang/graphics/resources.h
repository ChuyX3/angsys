#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_RESOURCES_H__
#define __ANG_GRAPHICS_RESOURCES_H__

namespace ang
{
	namespace graphics
	{
		
		namespace resources
		{
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