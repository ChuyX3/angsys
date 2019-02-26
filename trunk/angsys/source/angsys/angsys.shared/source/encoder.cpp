#include "pch.h"
#include "angsys.h"

#include "format_parser.h"
#include "encoder_interface.h"

using namespace ang;
using namespace ang::text;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::text::iencoder, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::text::iformat_parser, interface);

#define MY_TYPE ang::text::iformat_parser
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

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



template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::format_parser_interface<encoding::ascii>, object, iformat_parser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::format_parser_interface<encoding::ascii>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::format_parser_interface<encoding::ascii>, object, iformat_parser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::format_parser_interface<encoding::unicode>, object, iformat_parser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::format_parser_interface<encoding::unicode>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::format_parser_interface<encoding::unicode>, object, iformat_parser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::format_parser_interface<encoding::utf8>, object, iformat_parser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::format_parser_interface<encoding::utf8>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::format_parser_interface<encoding::utf8>, object, iformat_parser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::format_parser_interface<encoding::utf16>, object, iformat_parser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::format_parser_interface<encoding::utf16>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::format_parser_interface<encoding::utf16>, object, iformat_parser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::format_parser_interface<encoding::utf16_se>, object, iformat_parser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::format_parser_interface<encoding::utf16_se>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::format_parser_interface<encoding::utf16_se>, object, iformat_parser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::format_parser_interface<encoding::utf16_le>, object, iformat_parser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::format_parser_interface<encoding::utf16_le>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::format_parser_interface<encoding::utf16_le>, object, iformat_parser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::format_parser_interface<encoding::utf16_be>, object, iformat_parser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::format_parser_interface<encoding::utf16_be>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::format_parser_interface<encoding::utf16_be>, object, iformat_parser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::format_parser_interface<encoding::utf32>, object, iformat_parser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::format_parser_interface<encoding::utf32>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::format_parser_interface<encoding::utf32>, object, iformat_parser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::format_parser_interface<encoding::utf32_se>, object, iformat_parser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::format_parser_interface<encoding::utf32_se>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::format_parser_interface<encoding::utf32_se>, object, iformat_parser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::format_parser_interface<encoding::utf32_le>, object, iformat_parser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::format_parser_interface<encoding::utf32_le>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::format_parser_interface<encoding::utf32_le>, object, iformat_parser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::format_parser_interface<encoding::utf32_be>, object, iformat_parser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::format_parser_interface<encoding::utf32_be>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::format_parser_interface<encoding::utf32_be>, object, iformat_parser);


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

bool ang::operator == (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool ang::operator == (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool ang::operator != (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool ang::operator != (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool ang::operator >= (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool ang::operator >= (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool ang::operator <= (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool ang::operator <= (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool ang::operator > (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool ang::operator > (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool ang::operator < (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}
bool ang::operator < (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}

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

ang::raw_cstr::raw_cstr(raw_str const& str)
	: _value(str.ptr())
	, _size(str.size())
	, _encoding(str.encoding()) {
}

void const* ang::raw_cstr::ptr()const { return _value; }

wsize ang::raw_cstr::size()const { return _size; }

wsize ang::raw_cstr::count()const { return _size / text::encoder<text::encoding::auto_detect>::char_size_by_encoding(_encoding); }

wsize ang::raw_cstr::char_size()const { return text::encoder<text::encoding::auto_detect>::char_size_by_encoding(_encoding); }

ang::text::encoding ang::raw_cstr::encoding()const { return _encoding; }


bool ang::operator == (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool ang::operator == (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool ang::operator != (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool ang::operator != (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool ang::operator >= (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool ang::operator >= (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool ang::operator <= (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool ang::operator <= (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool ang::operator > (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool ang::operator > (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool ang::operator < (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}
bool ang::operator < (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}

///////////////////////////////////////////////////////////////////////////////////////

static collections::pair<ang::text::encoding, cstr_t> s_encoding_to_string[] = 
{
	{ ang::text::encoding::binary, "binary"_s },
	{ ang::text::encoding::ascii, "ascii"_s },
	{ ang::text::encoding::unicode, "unicode"_s },
	{ ang::text::encoding::utf8, "utf8"_s },
	{ ang::text::encoding::utf16, "utf16"_s },
	{ ang::text::encoding::utf16_se, "utf16_se"_s },
	{ ang::text::encoding::utf16_le, "utf16_le"_s },
	{ ang::text::encoding::utf16_be, "utf16_be"_s },
	{ ang::text::encoding::utf32, "utf32"_s },
	{ ang::text::encoding::utf32_se, "utf32_se"_s },
	{ ang::text::encoding::utf32_le, "utf32_le"_s },
	{ ang::text::encoding::utf32_be, "utf32_be"_s },
};

static collections::pair<cstr_t, ang::text::encoding> s_encoding_parse[] =
{
	{ "ascii"_s, ang::text::encoding::ascii },
	{ "binary"_s, ang::text::encoding::binary },
	{ "unicode"_s, ang::text::encoding::unicode },
	{ "utf16"_s, ang::text::encoding::utf16 },
	{ "utf16_be"_s, ang::text::encoding::utf16_be },
	{ "utf16_le"_s, ang::text::encoding::utf16_le },
	{ "utf16_se"_s, ang::text::encoding::utf16_se },
	{ "utf32"_s, ang::text::encoding::utf32 },
	{ "utf32_be"_s, ang::text::encoding::utf32_be },
	{ "utf32_le"_s, ang::text::encoding::utf32_le },
	{ "utf32_se"_s, ang::text::encoding::utf32_se },
};

safe_enum_rrti2(ang::text, encoding);

wstring encoding_t::to_string()const
{
	auto idx = algorithms::binary_search(get(), collections::to_array(s_encoding_to_string));
	if (idx >= algorithms::array_size(s_encoding_to_string))
		return "binary"_s;
	else
		return s_encoding_to_string[idx].value;
}

encoding_t encoding_t::parse(cstr_t str)
{
	auto idx = algorithms::binary_search(str, collections::to_array(s_encoding_parse));
	if (idx >= algorithms::array_size(s_encoding_parse))
		return encoding::binary;
	else
		return s_encoding_parse[idx].value;
}

encoding_t encoding_t::parse(cwstr_t str)
{
	auto idx = algorithms::binary_search(str, collections::to_array(s_encoding_parse));
	if (idx >= algorithms::array_size(s_encoding_parse))
		return encoding::binary;
	else
		return s_encoding_parse[idx].value;
}

encoding_t encoding_t::parse(cmstr_t str)
{
	auto idx = algorithms::binary_search(str, collections::to_array(s_encoding_parse));
	if (idx >= algorithms::array_size(s_encoding_parse))
		return encoding::binary;
	else
		return s_encoding_parse[idx].value;
}