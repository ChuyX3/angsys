#include "pch.h"
#include "angsys.h"

#include "format_parser.h"


using namespace ang;
using namespace ang::text;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::text::iparser, intf);

#define MY_TYPE ang::text::iparser
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

text_format_flags_t& format_parser<encoding::auto_detect>::bool_format() { static text_format_flags_t value(text_format::bool_); return value; }
text_format_flags_t& format_parser<encoding::auto_detect>::char_format() { static text_format_flags_t value(text_format::char_); return value; }
text_format_flags_t& format_parser<encoding::auto_detect>::string_format() { static text_format_flags_t value(text_format::text_); return value; }
text_format_flags_t& format_parser<encoding::auto_detect>::signed_format() { static text_format_flags_t value(text_format::signed_); return value; }
text_format_flags_t& format_parser<encoding::auto_detect>::unsigned_format() { static text_format_flags_t value(text_format::unsigned_); return value; }
text_format_flags_t& format_parser<encoding::auto_detect>::floating_format() { static text_format_flags_t value(text_format::float_); return value; }


template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::parser_interface<encoding::ascii>, object, iparser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::parser_interface<encoding::ascii>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::parser_interface<encoding::ascii>, object, iparser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::parser_interface<encoding::unicode>, object, iparser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::parser_interface<encoding::unicode>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::parser_interface<encoding::unicode>, object, iparser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::parser_interface<encoding::utf8>, object, iparser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::parser_interface<encoding::utf8>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::parser_interface<encoding::utf8>, object, iparser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::parser_interface<encoding::utf16>, object, iparser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::parser_interface<encoding::utf16>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::parser_interface<encoding::utf16>, object, iparser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::parser_interface<encoding::utf16_se>, object, iparser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::parser_interface<encoding::utf16_se>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::parser_interface<encoding::utf16_se>, object, iparser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::parser_interface<encoding::utf16_le>, object, iparser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::parser_interface<encoding::utf16_le>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::parser_interface<encoding::utf16_le>, object, iparser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::parser_interface<encoding::utf16_be>, object, iparser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::parser_interface<encoding::utf16_be>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::parser_interface<encoding::utf16_be>, object, iparser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::parser_interface<encoding::utf32>, object, iparser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::parser_interface<encoding::utf32>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::parser_interface<encoding::utf32>, object, iparser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::parser_interface<encoding::utf32_se>, object, iparser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::parser_interface<encoding::utf32_se>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::parser_interface<encoding::utf32_se>, object, iparser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::parser_interface<encoding::utf32_le>, object, iparser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::parser_interface<encoding::utf32_le>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::parser_interface<encoding::utf32_le>, object, iparser);

template<> ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::text::parser_interface<encoding::utf32_be>, object, iparser);
template<> ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::text::parser_interface<encoding::utf32_be>);
template<> ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::text::parser_interface<encoding::utf32_be>, object, iparser);
