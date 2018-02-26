#ifndef __ANGSYS_H__
#elif !defined __BUFFERS_H__
#define __BUFFERS_H__

namespace ang
{
	ang_interface(ibuffer);
	ang_interface(ibuffer_view);

	ang_begin_interface(LINK ibuffer_view)
		visible vcall pointer buffer_ptr()const pure;
		visible vcall wsize buffer_size()const pure;
	ang_end_interface();

	ANG_INTF_WRAPPER_DECLARATION(LINK, ibuffer_view);

	ang_begin_interface(LINK ibuffer, ibuffer_view)
		visible vcall text::encoding_t encoding()const pure;
		visible vcall wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) pure;
		visible vcall ibuffer_view_t map_buffer(windex, wsize) pure;
		visible vcall bool unmap_buffer(ibuffer_view_t&, wsize used) pure;
		visible vcall bool can_realloc_buffer()const pure;
		visible vcall bool realloc_buffer(wsize) pure;
	ang_end_interface();

	ANG_INTF_WRAPPER_DECLARATION(LINK, ibuffer);

}

#endif//__BUFFERS_H__