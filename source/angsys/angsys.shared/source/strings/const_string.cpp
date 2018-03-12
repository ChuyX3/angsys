#include "pch.h"
#include "ang/system.h"

using namespace ang;
using namespace ang::strings;

typedef struct const_str_data
{
	wsize size;
	wsize buffer[1];
}*const_str_data_ptr;

pointer basic_const_string_buffer_base::operator new(wsize sz, text::encoding_t e, raw_cstr_t str)
{
	auto encoder = text::iencoder::get_encoder(e);
	auto obj = ang_alloc_object_memory(align_up<8>(sz) + sizeof(const_str_data) + encoder->size(str.ptr(), str.encoding()), ang_buffer_memory);
	auto data = const_str_data_ptr(wsize(obj) + align_up<8>(sz));
	wsize j = 0;
	data->size = 0; 
	encoder->convert(data->buffer, data->size, str.ptr(), j, str.encoding());
	return obj;
}

void basic_const_string_buffer_base::operator delete(pointer ptr, text::encoding_t, raw_cstr_t str)
{
	object::operator delete (ptr);
}

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::strings::basic_const_string_buffer_base, object, itext_buffer);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::strings::basic_const_string_buffer_base);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::strings::basic_const_string_buffer_base, object, itext_buffer, ibuffer, ibuffer_view);

basic_const_string_buffer_base::basic_const_string_buffer_base() { }
basic_const_string_buffer_base::~basic_const_string_buffer_base() { }
bool basic_const_string_buffer_base::is_readonly()const { return true; }
bool basic_const_string_buffer_base::can_realloc_buffer()const { return false; }
pointer basic_const_string_buffer_base::buffer_ptr() { return null; }
wsize basic_const_string_buffer_base::mem_copy(wsize, pointer, text::encoding_t) { return 0; }
ibuffer_view_t basic_const_string_buffer_base::map_buffer(windex, wsize) { return null; }
bool basic_const_string_buffer_base::unmap_buffer(ibuffer_view_t&, wsize) { return false; }
bool basic_const_string_buffer_base::realloc_buffer(wsize) { return false; }

#define MY_ENCODING ang::text::encoding::ascii
#include "inline/const_string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::unicode
#include "inline/const_string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf8
#include "inline/const_string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16
#include "inline/const_string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_se
#include "inline/const_string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_le
#include "inline/const_string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_be
#include "inline/const_string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32
#include "inline/const_string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_se
#include "inline/const_string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_le
#include "inline/const_string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_be
#include "inline/const_string.inl"
#undef MY_ENCODING
