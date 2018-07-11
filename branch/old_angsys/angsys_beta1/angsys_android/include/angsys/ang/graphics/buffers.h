#ifndef __ANGRAPH_H__
#elif !defined __ANGRAPH_BUFFERS_H__
#define __ANGRAPH_BUFFERS_H__

namespace ang
{
	namespace graphics
	{
		namespace buffers
		{		
			struct igpu_buffer;
			struct iindex_buffer;
			struct ivertex_buffer;

			typedef intf_wrapper<igpu_buffer> igpu_buffer_t;
			typedef intf_wrapper<iindex_buffer> iindex_buffer_t;
			typedef intf_wrapper<ivertex_buffer> ivertex_buffer_t;

			ANG_BEGIN_ENUM(LINK, buffer_type, uint)
				none,
				texture,
				vertex_buffer,
				index_buffer,
			ANG_END_ENUM(buffer_type);

			ANG_BEGIN_ENUM(LINK, buffer_usage, uint)
				def,
				inmutable,
				dynamic,
				staging,
			ANG_END_ENUM(buffer_usage);

			ANG_BEGIN_FLAGS(LINK, buffer_bind_flag, uint)
				none = 0x0L,
				vertex_buffer = 0x1L,
				index_buffer = 0x2L,
				uniform_buffer = 0x4L,
				shader_resource = 0x8L,
				stream_output = 0x10L,
				render_target = 0x20L,
				depth_stencil = 0x40L,
			ANG_END_FLAGS(buffer_bind_flag);

			ANG_BEGIN_INTERFACE(LINK, igpu_buffer)
				visible vcall buffer_type_t buffer_type()const pure;
				visible vcall buffer_usage_t buffer_usage()const pure;
				visible vcall buffer_bind_flag_t buffer_bind_flag()const pure;
				visible vcall ibuffer_t map(idriver_t, bool writeMode) pure;
				visible vcall bool unmap(idriver_t, ibuffer_t) pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ivertex_buffer, igpu_buffer)
				visible vcall static_array<reflect::attribute_desc> descriptor()const pure;
				visible vcall wsize block_counter()const pure;
				visible vcall wsize size_in_bytes()const pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, iindex_buffer, igpu_buffer)
				visible vcall reflect::var_type_t data_type()const pure;
				visible vcall wsize counter()const pure;
				visible vcall wsize size_in_bytes()const pure;
			ANG_END_INTERFACE();
		}

	}
}


#endif//__ANGRAPH_BUFFERS_H__