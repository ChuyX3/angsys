#include "pch.h"
#include "angsys.h"

using namespace ang;
using namespace ang::text;


#define MY_LINKAGE LINK
#define MY_CHAR_TYPE ang::text::char_type_by_encoding<MY_ENCODING>::char_t
#define MY_STR_TYPE ang::text::char_type_by_encoding<MY_ENCODING>::str_t
#define MY_CSTR_TYPE ang::text::char_type_by_encoding<MY_ENCODING>::cstr_t

#define MY_ENCODING ang::text::encoding::ascii
#include <ang/base/inline/str_view.inl>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::unicode
#include <ang/base/inline/str_view.inl>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf8
#include <ang/base/inline/str_view.inl>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16
#include <ang/base/inline/str_view.inl>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_se
#include <ang/base/inline/str_view.inl>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_le
#include <ang/base/inline/str_view.inl>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_be
#include <ang/base/inline/str_view.inl>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32
#include <ang/base/inline/str_view.inl>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_se
#include <ang/base/inline/str_view.inl>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_le
#include <ang/base/inline/str_view.inl>
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_be
#include <ang/base/inline/str_view.inl>
#undef MY_ENCODING

#undef MY_LINKAGE
#undef MY_CHAR_TYPE
#undef MY_STR_TYPE
#undef MY_CSTR_TYPE

str_view<void, text::encoding::auto_detect>::str_view()
	: m_value(null)
	, m_size(0)
	, m_encoding(text::encoding::none) {
}

str_view<void, text::encoding::auto_detect>::str_view(ang::nullptr_t const&)
	: m_value(null)
	, m_size(0)
	, m_encoding(text::encoding::none) {
}

str_view<void, text::encoding::auto_detect>::str_view(void* v, wsize s, text::encoding e)
	: m_value(v)
	, m_size(s)
	, m_encoding(e) {
}

str_view<void, text::encoding::auto_detect>::str_view(raw_str const& str)
	: m_value(str.m_value)
	, m_size(str.m_size)
	, m_encoding(str.m_encoding) {
}

void* str_view<void, text::encoding::auto_detect>::ptr()const { return m_value; }

wsize str_view<void, text::encoding::auto_detect>::size()const { return m_size; }

wsize str_view<void, text::encoding::auto_detect>::count()const { return m_size / text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

wsize str_view<void, text::encoding::auto_detect>::char_size()const { return text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

text::encoding str_view<void, text::encoding::auto_detect>::encoding()const { return m_encoding; }
//
//bool ang::operator == (ang::str_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
//}
//bool ang::operator == (ang::str_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
//}
//bool ang::operator != (ang::str_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
//}
//bool ang::operator != (ang::str_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
//}
//bool ang::operator >= (ang::str_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
//}
//bool ang::operator >= (ang::str_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
//}
//bool ang::operator <= (ang::str_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
//}
//bool ang::operator <= (ang::str_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
//}
//bool ang::operator > (ang::str_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
//}
//bool ang::operator > (ang::str_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
//}
//bool ang::operator < (ang::str_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
//}
//bool ang::operator < (ang::str_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
//}

/////////////////////////////////////////////////////////////////////////////////////////////////


str_view<const void, text::encoding::auto_detect>::str_view()
	: m_value(null)
	, m_size(0)
	, m_encoding(text::encoding::none) {
}

str_view<const void, text::encoding::auto_detect>::str_view(ang::nullptr_t const&)
	: m_value(null)
	, m_size(0)
	, m_encoding(text::encoding::none) {
}

str_view<const void, text::encoding::auto_detect>::str_view(void const* v, wsize s, text::encoding e)
	: m_value(v)
	, m_size(s)
	, m_encoding(e) {
//	if (s == invalid_index)
//		m_size = iencoder::get_encoder(e)->lenght(v);
}

str_view<const void, text::encoding::auto_detect>::str_view(raw_cstr const& str)
	: m_value(str.m_value)
	, m_size(str.m_size)
	, m_encoding(str.m_encoding) {
}

str_view<const void, text::encoding::auto_detect>::str_view(raw_str const& str)
	: m_value(str.ptr())
	, m_size(str.size())
	, m_encoding(str.encoding()) {
}

void const* str_view<const void, text::encoding::auto_detect>::ptr()const { return m_value; }

wsize str_view<const void, text::encoding::auto_detect>::size()const { return m_size; }

wsize str_view<const void, text::encoding::auto_detect>::count()const { return m_size / text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

wsize str_view<const void, text::encoding::auto_detect>::char_size()const { return text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

text::encoding str_view<const void, text::encoding::auto_detect>::encoding()const { return m_encoding; }


//bool ang::operator == (ang::cstr_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
//}
//bool ang::operator == (ang::cstr_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
//}
//bool ang::operator != (ang::cstr_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
//}
//bool ang::operator != (ang::cstr_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
//}
//bool ang::operator >= (ang::cstr_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
//}
//bool ang::operator >= (ang::cstr_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
//}
//bool ang::operator <= (ang::cstr_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
//}
//bool ang::operator <= (ang::cstr_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
//}
//bool ang::operator > (ang::cstr_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
//}
//bool ang::operator > (ang::cstr_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
//}
//bool ang::operator < (ang::cstr_t const& str1, ang::str_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
//}
//bool ang::operator < (ang::cstr_t const& str1, ang::cstr_t const& str2) {
//	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
//}


int str_view_compare_helper<cstr_t, cstr_t>::compare(const cstr_t& str1, const cstr_t& str2)
{
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding());
}
