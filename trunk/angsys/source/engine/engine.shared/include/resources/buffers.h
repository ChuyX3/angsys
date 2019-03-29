#pragma once

namespace ang
{
	namespace graphics
	{
		namespace buffers
		{
			using namespace resources;

			class index_buffer
				: public resource
			{
			public:
				inline index_buffer()
					: resource() {
				}

				inline iindex_buffer_t handle()const {
					return const_cast<index_buffer*>(this)->to_index_buffer();
				}

				bool load(ilibrary_t lib, dom::xml::xml_node_t);

			private: //override
				using resource::resource_type;
				using resource::resource_sid;
				using resource::to_effect;
				using resource::to_shaders;
				using resource::to_texture;
				using resource::to_frame_buffer;
				using resource::to_index_buffer;
				using resource::to_vertex_buffer;
			};

			class vertex_buffer
				: public resource
			{
			public:
				inline vertex_buffer()
					: resource() {
				}

				inline ivertex_buffer_t handle()const {
					return const_cast<vertex_buffer*>(this)->to_vertex_buffer();
				}

				bool load(ilibrary_t lib, dom::xml::xml_node_t);

			private: //override
				using resource::resource_type;
				using resource::resource_sid;
				using resource::to_effect;
				using resource::to_shaders;
				using resource::to_texture;
				using resource::to_frame_buffer;
				using resource::to_index_buffer;
				using resource::to_vertex_buffer;
			};

			class frame_buffer
				: public resource
			{
			public:
				inline frame_buffer()
					: resource() {
				}

				inline iframe_buffer_t handle()const {
					return const_cast<frame_buffer*>(this)->to_frame_buffer();
				}

				bool load(ilibrary_t lib, dom::xml::xml_node_t);

			private: //override
				using resource::resource_type;
				using resource::resource_sid;
				using resource::to_effect;
				using resource::to_shaders;
				using resource::to_texture;
				using resource::to_frame_buffer;
				using resource::to_index_buffer;
				using resource::to_vertex_buffer;
			};
		}
	}
}