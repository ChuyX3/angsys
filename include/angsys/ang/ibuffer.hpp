/*********************************************************************************************************************/
/*   File Name: angsys.h                                                                                             */
/*   Autor: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                    */
/*   File description: this file declare all functions and clases for dynamic type naming and types conversions.     */
/*   Also it defines useful macros for the use of this functions and clases.                                         */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/


#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_IBUFFER_HPP__
#define __ANG_IBUFFER_HPP__

namespace ang
{
	ANG_INTERFACE(ibuffer);
	ANG_INTERFACE(ibuffer_view);

	ANG_BEGIN_INTERFACE(LINK, ibuffer_view)
		visible vcall pointer buffer_ptr()const pure;
		visible vcall wsize buffer_size()const pure;
	ANG_END_INTERFACE();

	ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ibuffer, ibuffer_view)
		visible static wsize serialize(ibuffer_t, streams::ibinary_output_stream_t stream);
		visible static wsize serialize(ibuffer_t, streams::itext_output_stream_t stream);
		visible vcall text::encoding_t encoding()const pure;
		visible vcall wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) pure;
		visible vcall ibuffer_view_t map_buffer(windex, wsize) pure;
		visible vcall bool unmap_buffer(ibuffer_view_t&, wsize used) pure;
		visible vcall bool can_realloc_buffer()const pure;
		visible vcall bool realloc_buffer(wsize) pure;
	ANG_END_INTERFACE();

}


#endif//__ANG_IBUFFER_HPP__