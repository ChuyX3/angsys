/*********************************************************************************************************************/
/*   File Name: angsys.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/base/base.h"
#include <assert.h>

ANG_EXTERN ang_void_ptr_t ang_alloc_unmanaged_memory(ang_size_t sz) { return malloc(sz); }
ANG_EXTERN void ang_free_unmanaged_memory(ang_void_ptr_t ptr) { free(ptr); }

using namespace ang;

bool interface::default_query_interface(rtti_t const& src_id, unknown_t src, rtti_t const& out_id, unknown_ptr_t out)
{
	assert(src && src_id.is_type_of(interface::class_info()));

	return ((interface*)(src))->query_interface(out_id, out);
}

rtti_t const& interface::class_info() {
	static const char name[] = "ang::interface";
	return rtti::regist(name, genre::class_type, sizeof(interface), alignof(wsize), null, &default_query_interface);
}
