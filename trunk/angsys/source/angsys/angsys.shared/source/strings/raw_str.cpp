#include "pch.h"
#include "angsys.h"

using namespace ang;
using namespace ang::text;


ang::raw_str::raw_str()
	: m_value(null)
	, m_size(0)
	, m_encoding(text::encoding::none) {
}

ang::raw_str::raw_str(void* v, wsize s, text::encoding e)
	: m_value(v)
	, m_size(s)
	, m_encoding(e) {
}

ang::raw_str::raw_str(raw_str const& str)
	: m_value(str.m_value)
	, m_size(str.m_size)
	, m_encoding(str.m_encoding) {
}

void* ang::raw_str::ptr()const { return m_value; }

wsize ang::raw_str::size()const { return m_size; }

wsize ang::raw_str::count()const { return m_size / text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

wsize ang::raw_str::char_size()const { return text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

ang::text::encoding_t ang::raw_str::encoding()const { return m_encoding; }

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
	: m_value(null)
	, m_size(0)
	, m_encoding(text::encoding::none) {
}

ang::raw_cstr::raw_cstr(void const* v, wsize s, text::encoding e)
	: m_value(v)
	, m_size(s)
	, m_encoding(e) {
}

ang::raw_cstr::raw_cstr(raw_cstr const& str)
	: m_value(str.m_value)
	, m_size(str.m_size)
	, m_encoding(str.m_encoding) {
}

ang::raw_cstr::raw_cstr(raw_str const& str)
	: m_value(str.ptr())
	, m_size(str.size())
	, m_encoding(str.encoding()) {
}

void const* ang::raw_cstr::ptr()const { return m_value; }

wsize ang::raw_cstr::size()const { return m_size; }

wsize ang::raw_cstr::count()const { return m_size / text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

wsize ang::raw_cstr::char_size()const { return text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

ang::text::encoding_t ang::raw_cstr::encoding()const { return m_encoding; }


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
