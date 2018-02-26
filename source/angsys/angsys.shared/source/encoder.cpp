#include "pch.h"
#include "ang/system.h"
#include "encoder_interface.h"

using namespace ang;
using namespace ang::text;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::text::iencoder, interface);

#define MY_TYPE ang::text::iencoder
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::encoder_interface<encoding::ascii>, object, iencoder);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::encoder_interface<encoding::ascii>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::encoder_interface<encoding::ascii>, object, iencoder);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::encoder_interface<encoding::unicode>, object, iencoder);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::encoder_interface<encoding::unicode>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::encoder_interface<encoding::unicode>, object, iencoder);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::encoder_interface<encoding::utf8>, object, iencoder);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::encoder_interface<encoding::utf8>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::encoder_interface<encoding::utf8>, object, iencoder);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::encoder_interface<encoding::utf16>, object, iencoder);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::encoder_interface<encoding::utf16>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::encoder_interface<encoding::utf16>, object, iencoder);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::encoder_interface<encoding::utf16_se>, object, iencoder);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::encoder_interface<encoding::utf16_se>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::encoder_interface<encoding::utf16_se>, object, iencoder);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::encoder_interface<encoding::utf16_le>, object, iencoder);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::encoder_interface<encoding::utf16_le>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::encoder_interface<encoding::utf16_le>, object, iencoder);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::encoder_interface<encoding::utf16_be>, object, iencoder);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::encoder_interface<encoding::utf16_be>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::encoder_interface<encoding::utf16_be>, object, iencoder);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::encoder_interface<encoding::utf32>, object, iencoder);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::encoder_interface<encoding::utf32>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::encoder_interface<encoding::utf32>, object, iencoder);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::encoder_interface<encoding::utf32_se>, object, iencoder);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::encoder_interface<encoding::utf32_se>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::encoder_interface<encoding::utf32_se>, object, iencoder);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::encoder_interface<encoding::utf32_le>, object, iencoder);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::encoder_interface<encoding::utf32_le>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::encoder_interface<encoding::utf32_le>, object, iencoder);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::encoder_interface<encoding::utf32_be>, object, iencoder);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::encoder_interface<encoding::utf32_be>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::encoder_interface<encoding::utf32_be>, object, iencoder);


ang::raw_str::raw_str()
	: _value(null)
	, _size(0)
	, _encoding(text::encoding::none) {
}

ang::raw_str::raw_str(void* v, wsize s, text::encoding e)
	: _value(v)
	, _size(s)
	, _encoding(e) {
}

ang::raw_str::raw_str(raw_str const& str)
	: _value(str._value)
	, _size(str._size)
	, _encoding(str._encoding) {
}

void* ang::raw_str::ptr()const { return _value; }

wsize ang::raw_str::size()const { return _size; }

wsize ang::raw_str::count()const { return _size / text::encoder<text::encoding::auto_detect>::char_size_by_encoding(_encoding); }

wsize ang::raw_str::char_size()const { return text::encoder<text::encoding::auto_detect>::char_size_by_encoding(_encoding); }

ang::text::encoding ang::raw_str::encoding()const { return _encoding; }

/////////////////////////////////////////////////////////////////////////////////////////////////


ang::raw_cstr::raw_cstr()
	: _value(null)
	, _size(0)
	, _encoding(text::encoding::none) {
}

ang::raw_cstr::raw_cstr(void const* v, wsize s, text::encoding e)
	: _value(v)
	, _size(s)
	, _encoding(e) {
}

ang::raw_cstr::raw_cstr(raw_cstr const& str)
	: _value(str._value)
	, _size(str._size)
	, _encoding(str._encoding) {
}

void const* ang::raw_cstr::ptr()const { return _value; }

wsize ang::raw_cstr::size()const { return _size; }

wsize ang::raw_cstr::count()const { return _size / text::encoder<text::encoding::auto_detect>::char_size_by_encoding(_encoding); }

wsize ang::raw_cstr::char_size()const { return text::encoder<text::encoding::auto_detect>::char_size_by_encoding(_encoding); }

ang::text::encoding ang::raw_cstr::encoding()const { return _encoding; }


