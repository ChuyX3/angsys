#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_BUFFERS_H__
#define __ANG_GRAPHICS_BUFFERS_H__

namespace ang
{
	namespace graphics
	{
		namespace buffers
		{		

			safe_enum(LINK, buffer_type, uint)
			{
				none,
				texture,
				vertex_buffer,
				index_buffer,
			};

			safe_enum(LINK, buffer_usage, uint)
			{
				def,
				inmutable,
				dynamic,
				staging,
			};

			safe_flags(LINK, buffer_bind_flag, uint)
			{
				none = 0x0L,
				vertex_buffer = 0x1L,
				index_buffer = 0x2L,
				uniform_buffer = 0x4L,
				shader_resource = 0x8L,
				stream_output = 0x10L,
				render_target = 0x20L,
				depth_stencil = 0x40L,
			};

			ang_begin_interface(LINK igpu_buffer)
				visible vcall resources::iresource_t resource()const pure;
				visible vcall buffer_type_t buffer_type()const pure;
				visible vcall buffer_usage_t buffer_usage()const pure;
				visible vcall buffer_bind_flag_t buffer_bind_flag()const pure;
				visible vcall ibuffer_t map(idriver_t, bool writeMode) pure;
				visible vcall bool unmap(idriver_t, ibuffer_t) pure;
			ang_end_interface();

			ang_begin_interface(LINK ivertex_buffer, igpu_buffer)
				visible vcall array_view<reflect::attribute_desc> descriptor()const pure;
				visible vcall wsize block_counter()const pure;
				visible vcall wsize size_in_bytes()const pure;
			ang_end_interface();

			ang_begin_interface(LINK iindex_buffer, igpu_buffer)
				visible vcall reflect::var_type_t data_type()const pure;
				visible vcall wsize counter()const pure;
				visible vcall wsize size_in_bytes()const pure;
			ang_end_interface();
		}

	}
}


#endif//__ANG_GRAPHICS_BUFFERS_H__