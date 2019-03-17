#include "pch.h"
#include "angsys.h"

using namespace ang;
using namespace ang::text;


str_view<void, text::encoding::auto_detect>::str_view()
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

bool ang::operator == (ang::str_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool ang::operator == (ang::str_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool ang::operator != (ang::str_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool ang::operator != (ang::str_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool ang::operator >= (ang::str_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool ang::operator >= (ang::str_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool ang::operator <= (ang::str_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool ang::operator <= (ang::str_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool ang::operator > (ang::str_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool ang::operator > (ang::str_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool ang::operator < (ang::str_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}
bool ang::operator < (ang::str_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////


str_view<const void, text::encoding::auto_detect>::str_view()
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


bool ang::operator == (ang::cstr_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool ang::operator == (ang::cstr_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool ang::operator != (ang::cstr_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool ang::operator != (ang::cstr_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool ang::operator >= (ang::cstr_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool ang::operator >= (ang::cstr_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool ang::operator <= (ang::cstr_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool ang::operator <= (ang::cstr_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool ang::operator > (ang::cstr_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool ang::operator > (ang::cstr_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool ang::operator < (ang::cstr_t const& str1, ang::str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}
bool ang::operator < (ang::cstr_t const& str1, ang::cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}
