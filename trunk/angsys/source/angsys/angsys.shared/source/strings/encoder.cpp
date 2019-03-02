#include "pch.h"
#include "angsys.h"
#include "encoder_interface.h"

using namespace ang;
using namespace ang::text;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::text::iencoder, interface);


TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, ascii);
TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, unicode);
TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, utf8);
TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, utf16);
TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, utf16_se);
TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, utf16_le);
TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, utf16_be);
TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, utf32);
TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, utf32_se);
TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, utf32_le);
TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, utf32_be);


/////////////////////////////////////////////////////////////////////////////


template<> wsize text::load_bom<text::encoding::utf8>(pointer ptr) {
	alignas(4) static byte utf8_bom[4] = { 0xef, 0xbb, 0xbf, 0x0 };
	return (text::utf8().compare_until((mchar const*)ptr, (mchar const*)utf8_bom) == 3) ? 3 : 0;
}

template<> wsize text::load_bom<text::encoding::utf16_le>(pointer ptr) {
	alignas(4) static byte utf16_le_bom[4] = { 0xff, 0xfe, 0x0, 0x0 };
	return (text::utf16_le().compare_until((char16_t const*)ptr, (char16_t const*)utf16_le_bom) == 1) ? 2 : 0;
}

template<> wsize text::load_bom<text::encoding::utf16_be>(pointer ptr) {
	alignas(4) static byte utf16_be_bom[4] = { 0xfe, 0xff, 0x0, 0x0 };
	return (text::utf16_be().compare_until((char16_t const*)ptr, (char16_t const*)utf16_be_bom) == 1) ? 2 : 0;
}

template<> wsize text::load_bom<text::encoding::utf32_le>(pointer ptr) {
	alignas(4) static byte utf32_le_bom[8] = { 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	return (text::utf32_le().compare_until((char32_t const*)ptr, (char32_t const*)utf32_le_bom) == 1) ? 4 : 0;
}

template<> wsize text::load_bom<text::encoding::utf32_be>(pointer ptr) {
	alignas(4) static byte utf32_be_bom[8] = { 0x0, 0x0, 0xfe, 0xff, 0x0, 0x0, 0x0, 0x0 };
	return (text::utf32_be().compare_until((char32_t const*)ptr, (char32_t const*)utf32_be_bom) == 1) ? 4 : 0;
}

template<> wsize text::load_bom<text::encoding::utf16>(pointer ptr) {
	return load_bom<native_encoding<text::encoding::utf16>::value>(ptr);
}

template<> wsize text::load_bom<text::encoding::utf16_se>(pointer ptr) {
	return load_bom<native_inverse_encoding<text::encoding::utf16>::value>(ptr);
}

template<> wsize text::load_bom<text::encoding::utf32>(pointer ptr) {
	return load_bom<native_encoding<text::encoding::utf16>::value>(ptr);
}

template<> wsize text::load_bom<text::encoding::utf32_se>(pointer ptr) {
	return load_bom<native_inverse_encoding<text::encoding::utf16>::value>(ptr);
}

template<> wsize text::load_bom<text::encoding::unicode>(pointer ptr) {
	return load_bom<native_encoding<text::encoding::unicode>::value>(ptr);
}

////////////////////////////////////////////////////////////////////////////


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

///////////////////////////////////////////////////////////////////////////////////////

static collections::pair<ang::text::encoding, cstr_t> s_encoding_to_string[] = 
{
	{ ang::text::encoding::binary, "binary"_s },
	{ ang::text::encoding::ascii, "ascii"_s },
	{ ang::text::encoding::unicode, "unicode"_s },
	{ ang::text::encoding::utf8, "utf-8"_s },
	{ ang::text::encoding::utf16, "utf-16"_s },
	{ ang::text::encoding::utf16_se, "utf-16-se"_s },
	{ ang::text::encoding::utf16_le, "utf-16-le"_s },
	{ ang::text::encoding::utf16_be, "utf-16-be"_s },
	{ ang::text::encoding::utf32, "utf-32"_s },
	{ ang::text::encoding::utf32_se, "utf-32-se"_s },
	{ ang::text::encoding::utf32_le, "utf-32-le"_s },
	{ ang::text::encoding::utf32_be, "utf-32-be"_s },
};

static collections::pair<cstr_t, ang::text::encoding> s_encoding_parse[] =
{
	{ "ascii"_s, ang::text::encoding::ascii },
	{ "binary"_s, ang::text::encoding::binary },
	{ "unicode"_s, ang::text::encoding::unicode },
	{ "utf-16"_s, ang::text::encoding::utf16 },
	{ "utf-16-be"_s, ang::text::encoding::utf16_be },
	{ "utf-16-le"_s, ang::text::encoding::utf16_le },
	{ "utf-16-se"_s, ang::text::encoding::utf16_se },
	{ "utf-32"_s, ang::text::encoding::utf32 },
	{ "utf-32-be"_s, ang::text::encoding::utf32_be },
	{ "utf-32-le"_s, ang::text::encoding::utf32_le },
	{ "utf-32-se"_s, ang::text::encoding::utf32_se },
	{ "utf-8"_s, ang::text::encoding::utf8 },
};

safe_enum_rrti2(ang::text, encoding);

cstr_t encoding_t::to_string()const
{
	auto idx = algorithms::binary_search(get(), collections::to_array(s_encoding_to_string));
	if (idx >= algorithms::array_size(s_encoding_to_string))
		return "binary"_s;
	else
		return s_encoding_to_string[idx].value;
}

encoding_t encoding_t::parse(raw_cstr_t str)
{
	auto idx = algorithms::binary_search(str, collections::to_array(s_encoding_parse));
	if (idx >= algorithms::array_size(s_encoding_parse))
		return encoding::binary;
	else
		return s_encoding_parse[idx].value;
}
