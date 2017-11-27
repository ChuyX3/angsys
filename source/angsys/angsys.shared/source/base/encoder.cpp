#include "pch.h"
#include "angsys.h"
#include "encoder.h"

#include "inline/encoder.inl"
#include "inline/encoder_length.inl"
#include "inline/encoder_size.inl"
#include "inline/encoder_convert.inl"
#include "inline/encoder_compare.inl"
#include "inline/encoder_find.inl"

using namespace ang;
using namespace ang::text;

ANG_IMPLEMENT_ENUM(ang::text, encoding, uint, encoding::auto_detect);

ang::collections::pair<encoding_enum, cstr_t> encoding_to_string[] = {
	{encoding::binary, "binary"_s },
	{ encoding::ascii, "ascii"_s },
	{ encoding::unicode, "unicode"_s },
	{ encoding::utf8, "utf8"_s },
	{ encoding::utf16_le, "utf16_le"_s },
	{ encoding::utf16_be, "utf16_be"_s },
	{ encoding::utf16, "utf16"_s },
	{ encoding::utf16_se, "utf16_se"_s },
	{ encoding::utf32_le, "utf32_le"_s },
	{ encoding::utf32_be, "utf32_be"_s },
	{ encoding::utf32, "utf32"_s },
	{ encoding::utf32_se, "utf32_se"_s },
	{ encoding::auto_detect, "auto_detect"_s },
};

const int i = 'A';

ang::collections::pair<cstr_t, encoding_enum> encoding_parse[] = {
	{ "ASCII"_s, encoding::ascii },
	{ "AUTO"_s, encoding::auto_detect },
	{ "AUTO_DETECT"_s, encoding::auto_detect },
	{ "BINARY"_s, encoding::binary },
	{ "UNICODE"_s, encoding::unicode },
	{ "UTF-16"_s, encoding::utf16 },
	{ "UTF-16-BE"_s, encoding::utf16_be },
	{ "UTF-16-LE"_s, encoding::utf16_le },
	{ "UTF-32"_s, encoding::utf32 },
	{ "UTF-32-BE"_s, encoding::utf32_be },
	{ "UTF-32-LE"_s, encoding::utf32_le },
	{ "UTF-8"_s, encoding::utf8 },

	{ "ascii"_s, encoding::ascii },
	{ "auto"_s, encoding::auto_detect },
	{ "auto_detect"_s, encoding::auto_detect },
	{ "binary"_s, encoding::binary },
	{ "unicode"_s, encoding::unicode },
	{ "utf-16"_s, encoding::utf16 },
	{ "utf-16-be"_s, encoding::utf16_be },
	{ "utf-16-le"_s, encoding::utf16_le },
	{ "utf-32"_s, encoding::utf32 },
	{ "utf-32-be"_s, encoding::utf32_be },
	{ "utf-32-le"_s, encoding::utf32_le },
	{ "utf-8"_s, encoding::utf8 },
};




cstr_t encoding_t::to_string()const
{
	wsize idx = collections::algorithms::binary_search<encoding_enum>(_value, to_array(encoding_to_string));
	if (idx > array_size(encoding_to_string))
		return "none"_s;
	else
		return encoding_to_string[idx].value;
}

encoding_t encoding_t::parse(cstr_t code)
{
	wsize idx = collections::algorithms::binary_search(code, to_array(encoding_parse));
	if (idx > array_size(encoding_parse))
		return encoding::auto_detect;
	else
		return encoding_parse[idx].value;
}

encoding_t encoding_t::parse(cmstr_t code)
{
	wsize idx = collections::algorithms::binary_search(code, to_array(encoding_parse));
	if (idx > array_size(encoding_parse))
		return encoding::auto_detect;
	else
		return encoding_parse[idx].value;
}

encoding_t encoding_t::parse(cwstr_t code)
{
	wsize idx = collections::algorithms::binary_search(code, to_array(encoding_parse));
	if (idx > array_size(encoding_parse))
		return encoding::auto_detect;
	else
		return encoding_parse[idx].value;
}

static union { int sdat; char cdat[4]; } __is_little_endian_impl = { 0x01 };

ANG_EXTERN ang_bool_t ang_is_little_endian_platform() {
	return __is_little_endian_impl.cdat[0] == 0X01;
}

bool ang::text::is_little_endian() { return __is_little_endian_impl.cdat[0] == 0X01; }


template<ang::text::encoding_enum ENCODING>
void ang_text_encoder_interface_initialize_interface(encoder_interface* encoder)
{
	

	encoder->_format = &ang_text_encoder<ENCODING>::format;
	encoder->_length = [](pointer ptr)->wsize { return ang_text_encoder<ENCODING>::length((typename ang::text::char_type_by_encoding<ENCODING>::cstr_t)ptr); };
	encoder->_size = [](pointer ptr, encoding_t format)->wsize { return ang_text_encoder<ENCODING>::size(ptr, format); };
	encoder->_to_utf32 = is_endian_swapped<ENCODING>()?
		function_cast<char32_t, pointer, windex&>([](pointer ptr, windex& idx)->char32_t { return converter<char32_t, typename text::char_type_by_encoding<ENCODING>::char_t>::template convert<true>((typename text::char_type_by_encoding<ENCODING>::char_t const*)ptr, idx); }) :
		function_cast<char32_t, pointer, windex&>([](pointer ptr, windex& idx)->char32_t { return converter<char32_t, typename text::char_type_by_encoding<ENCODING>::char_t>::template convert<false>((typename text::char_type_by_encoding<ENCODING>::char_t const*)ptr, idx); });

	encoder->_convert_string = [](pointer dest, wsize sz, pointer src, encoding_t format, bool eos)->wsize {
		return ang_text_encoder<ENCODING>::convert((typename ang::text::char_type_by_encoding<ENCODING>::str_t)dest, sz, src, format, eos);
	};
	encoder->_compare_string = [](pointer dest, pointer src, encoding_t format)->int {
		return ang_text_encoder<ENCODING>::compare((typename ang::text::char_type_by_encoding<ENCODING>::cstr_t)dest, src, format);
	};
	encoder->_compare_string_until = [](pointer dest, pointer src, encoding_t format)->wsize {
		return ang_text_encoder<ENCODING>::compare_until((typename ang::text::char_type_by_encoding<ENCODING>::cstr_t)dest, src, format);
	};
	encoder->_find = [](pointer first, wsize s1, pointer second, wsize s2, text::encoding_t format, windex start)->windex {
		return ang_text_encoder<ENCODING>::find((typename ang::text::char_type_by_encoding<ENCODING>::cstr_t)first, s1, second, s2, format, start);
	};
	encoder->_find_revert = [](pointer first, wsize s1, pointer second, wsize s2, text::encoding_t format, windex start)->windex {
		return ang_text_encoder<ENCODING>::find_revert((typename ang::text::char_type_by_encoding<ENCODING>::cstr_t)first, s1, second, s2, format, start);
	};
}

void ang::text::encoder_interface::initialize_interface(encoder_interface* encoder, encoding_t format)
{
	if (encoder)
	{
		switch (format)
		{
		case encoding::ascii: ang_text_encoder_interface_initialize_interface<encoding::ascii>(encoder); break;
		case encoding::unicode: ang_text_encoder_interface_initialize_interface<encoding::unicode>(encoder); break;		
		case encoding::utf8: ang_text_encoder_interface_initialize_interface<encoding::utf8>(encoder); break;
		case encoding::utf16:ang_text_encoder_interface_initialize_interface<encoding::utf16>(encoder); break;
		case encoding::utf16_se:ang_text_encoder_interface_initialize_interface<encoding::utf16_se>(encoder); break;
		case encoding::utf16_le:
			is_little_endian() ? ang_text_encoder_interface_initialize_interface<encoding::utf16>(encoder)
				: ang_text_encoder_interface_initialize_interface<encoding::utf16_se>(encoder);
			break;			
		case encoding::utf16_be:
			is_little_endian() ? ang_text_encoder_interface_initialize_interface<encoding::utf16_se>(encoder)
				: ang_text_encoder_interface_initialize_interface<encoding::utf16>(encoder);
			break;
		case encoding::utf32:ang_text_encoder_interface_initialize_interface<encoding::utf32>(encoder); break;
		case encoding::utf32_se:ang_text_encoder_interface_initialize_interface<encoding::utf32_se>(encoder); break;
		case encoding::utf32_le:
			is_little_endian() ? ang_text_encoder_interface_initialize_interface<encoding::utf32>(encoder)
				: ang_text_encoder_interface_initialize_interface<encoding::utf32_se>(encoder);
			break;
		case encoding::utf32_be:
			is_little_endian() ? ang_text_encoder_interface_initialize_interface<encoding::utf32_se>(encoder)
				: ang_text_encoder_interface_initialize_interface<encoding::utf32>(encoder);
			break;


		default:break;
		}


	}
}