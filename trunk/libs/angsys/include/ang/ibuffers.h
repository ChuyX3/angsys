/*********************************************************************************************************************/
/*   File Name: ang/ibuffers.h                                                                                       */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#elif !defined __ANG_IBUFFERS_H__
#define __ANG_IBUFFERS_H__

namespace ang
{
	ang_declare_interface(ibuffer);
	ang_declare_interface(ibuffer_view);

	ang_declare_object(buffer);
	ang_declare_object(aligned_buffer);
	ang_declare_object(buffer_view);

	struct nvt LINK ibuffer_view
		: intf<ibuffer_view
		, iid("ang::ibuffer_view")>
	{
		virtual text::encoding_t encoding()const = 0;
		virtual bool is_readonly()const = 0;
		virtual pointer buffer_ptr() = 0;
		virtual const_pointer buffer_ptr()const = 0;
		virtual wsize buffer_size()const = 0;
	};

	ANG_INTF_WRAPPER_DECLARATION(LINK, ibuffer_view);

	struct nvt LINK ibuffer
		: intf<ibuffer
		, iid("ang::ibuffer")
		, ibuffer_view>
	{
		virtual wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) = 0;
		virtual ibuffer_view_t map_buffer(windex, wsize) = 0;
		virtual bool unmap_buffer(ibuffer_view_t&, wsize used) = 0;
		virtual bool can_realloc_buffer()const = 0;
		virtual bool realloc_buffer(wsize) = 0;
	};

	ANG_INTF_WRAPPER_DECLARATION(LINK, ibuffer);

}

#endif//__ANG_IBUFFERS_H__