#ifndef __ANG_GRAPHICS_H__
#elif !defined __ANG_GRAPHICS_BUFFERS_H__
#define __ANG_GRAPHICS_BUFFERS_H__

namespace ang
{
	namespace graphics
	{
		namespace buffers
		{		
			struct nvt LINK igpu_buffer
				: intf<igpu_buffer
				, iid("ang::graphics::buffers::igpu_buffer")>
			{
				virtual resources::iresource_t resource()const = 0;
				virtual buffer_type_t buffer_type()const = 0;
				virtual buffer_usage_t buffer_usage()const = 0;
				virtual buffer_bind_flag_t buffer_bind_flag()const = 0;
				virtual ibuffer_t map(idriver_t, bool writeMode) = 0;
				virtual bool unmap(idriver_t, ibuffer_t) = 0;
			};

			struct nvt LINK ivertex_buffer
				: intf<ivertex_buffer
				, iid("ang::graphics::buffers::ivertex_buffer")
				, igpu_buffer>
			{
				virtual array_view<reflect::attribute_desc> descriptor()const = 0;
				virtual wsize block_counter()const = 0;
				virtual wsize size_in_bytes()const = 0;
			};

			struct nvt LINK iindex_buffer
				: intf<iindex_buffer
				, iid("ang::graphics::buffers::iindex_buffer")
				, igpu_buffer>
			{
				virtual reflect::var_type_t data_type()const = 0;
				virtual wsize counter()const = 0;
				virtual wsize size_in_bytes()const = 0;
			};
		}

	}
}


#endif//__ANG_GRAPHICS_BUFFERS_H__