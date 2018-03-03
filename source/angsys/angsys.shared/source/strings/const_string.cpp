#include "pch.h"
#include "ang/system.h"

using namespace ang;
using namespace ang::strings;

typedef struct const_str_data
{
	wsize size;
	wsize buffer[1];
}*const_str_data_ptr;

pointer const_string_buffer_base::operator new(wsize sz, text::encoding_t e, raw_cstr_t str)
{
	auto encoder = text::iencoder::get_encoder(e);
	auto obj = ang_alloc_object_memory(align_up<8>(sz) + sizeof(const_str_data) + encoder->size(str.ptr(), str.encoding()), ang_buffer_memory);
	auto data = const_str_data_ptr(wsize(obj) + align_up<8>(sz));
	wsize j = 0;
	data->size = 0; 
	encoder->convert(data->buffer, data->size, str.ptr(), j, str.encoding());
	return obj;
}

void const_string_buffer_base::operator delete(pointer ptr, text::encoding_t, raw_cstr_t str)
{
	object::operator delete (ptr);
}

const_string_buffer_base::const_string_buffer_base() { }
const_string_buffer_base::~const_string_buffer_base() { }
bool const_string_buffer_base::is_constant()const { return true; }
bool const_string_buffer_base::can_realloc_buffer()const { return false; }
pointer const_string_buffer_base::buffer_ptr() { return null; }
wsize const_string_buffer_base::mem_copy(wsize, pointer, text::encoding_t) { return 0; }
ibuffer_view_t const_string_buffer_base::map_buffer(windex, wsize) { return null; }
bool const_string_buffer_base::unmap_buffer(ibuffer_view_t&, wsize) { return false; }
bool const_string_buffer_base::realloc_buffer(wsize) { return false; }

