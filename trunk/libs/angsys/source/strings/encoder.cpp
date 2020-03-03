#include "pch.h"
#include "ang/base/base.h"
#include "inline/encoder_bridge.hpp"

using namespace ang;
using namespace ang::text;

TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, binary);
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
TO_STRING_TEMPLATE_IMPLEMENT(ang::text::encoding, auto_detect);


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

wsize text_encoder<encoding::auto_detect>::lenght(cstr_t const& cstr)
{
	return encoder_bridge::lenght(cstr);
}

wsize text_encoder<encoding::auto_detect>::count(cstr_t const& cstr)
{
	return encoder_bridge::count(cstr);
}

wsize text_encoder<encoding::auto_detect>::size(cstr_t const& from, encoding to, windex start, windex end)
{
	return encoder_bridge::size(from, to, start, end);
}

int text_encoder<encoding::auto_detect>::compare(cstr_t const& first, cstr_t const& second)
{
	return encoder_bridge::compare(first, second);
}

windex text_encoder<encoding::auto_detect>::compare_until(cstr_t const& first, cstr_t const& second)
{
	return encoder_bridge::compare_until(first, second);
}

windex text_encoder<encoding::auto_detect>::find(cstr_t const& first, cstr_t const& second, windex start)
{
	return encoder_bridge::find(first, second, start);
}

windex text_encoder<encoding::auto_detect>::find_reverse(cstr_t const& first, cstr_t const& second, windex start)
{
	return encoder_bridge::find_reverse(first, second, start);
}

windex text_encoder<encoding::auto_detect>::find_any(cstr_t const& first, windex start, array_view<const char32> chars)
{
	return encoder_bridge::find_any(first, start, chars);
}

char32_t text_encoder<encoding::auto_detect>::to_char32(cstr_t const& cstr, windex& at, bool increase)
{
	wsize i = at;
	if(increase)
		return encoder_bridge::to_char32(cstr, at);
	else
		return encoder_bridge::to_char32(cstr, i);
}

void text_encoder<encoding::auto_detect>::set_eos(str_t str, windex at)
{
	encoder_bridge::set_eos(str, at);
}

cstr_t text_encoder<encoding::auto_detect>::convert(str_t str, cstr_t const& cstr, bool eos, wsize max_out, wsize max_in)
{
	windex i = 0, j = 0;
	return encoder_bridge::convert(str, i, cstr, j, eos, max_out, max_in);
}

cstr_t text_encoder<encoding::auto_detect>::convert(str_t str, wsize& i, cstr_t const& cstr, wsize& j, bool eos, wsize max_out, wsize max_in)
{
	return encoder_bridge::convert(str, i, cstr, j, eos, max_out, max_in);
}

///////////////////////////////////////////////////////////////////////////////////////

static collections::pair<ang::text::encoding, castr_t> s_encoding_to_string[] = 
{
	{ ang::text::encoding::binary, "binary"_sv },
	{ ang::text::encoding::ascii, "ascii"_sv },
	{ ang::text::encoding::unicode, "unicode"_sv },
	{ ang::text::encoding::utf8, "utf-8"_sv },
	{ ang::text::encoding::utf16, "utf-16"_sv },
	{ ang::text::encoding::utf16_se, "utf-16-se"_sv },
	{ ang::text::encoding::utf16_le, "utf-16-le"_sv },
	{ ang::text::encoding::utf16_be, "utf-16-be"_sv },
	{ ang::text::encoding::utf32, "utf-32"_sv },
	{ ang::text::encoding::utf32_se, "utf-32-se"_sv },
	{ ang::text::encoding::utf32_le, "utf-32-le"_sv },
	{ ang::text::encoding::utf32_be, "utf-32-be"_sv },
	{ ang::text::encoding::auto_detect, "auto"_sv },
};

static collections::pair<castr_t, ang::text::encoding> s_encoding_parse[] =
{
	{ "ascii"_sv, ang::text::encoding::ascii },
	{ "auto"_sv, ang::text::encoding::auto_detect },
	{ "binary"_sv, ang::text::encoding::binary },
	{ "unicode"_sv, ang::text::encoding::unicode },
	{ "utf-16"_sv, ang::text::encoding::utf16 },
	{ "utf-16-be"_sv, ang::text::encoding::utf16_be },
	{ "utf-16-le"_sv, ang::text::encoding::utf16_le },
	{ "utf-16-se"_sv, ang::text::encoding::utf16_se },
	{ "utf-32"_sv, ang::text::encoding::utf32 },
	{ "utf-32-be"_sv, ang::text::encoding::utf32_be },
	{ "utf-32-le"_sv, ang::text::encoding::utf32_le },
	{ "utf-32-se"_sv, ang::text::encoding::utf32_se },
	{ "utf-8"_sv, ang::text::encoding::utf8 },
};

ANG_ENUM_IMPLEMENT(ang::text, encoding);

ANG_ENUM_TO_STRING_IMPLEMENT(encoding, s_encoding_to_string, "binary")
ANG_ENUM_PARSE_IMPLEMENT(encoding, s_encoding_parse, encoding::binary)

