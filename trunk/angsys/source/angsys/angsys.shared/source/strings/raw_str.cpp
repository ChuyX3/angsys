#include "pch.h"
#include "angsys.h"

using namespace ang;
using namespace ang::text;


raw_str::raw_str()
	: m_value(null)
	, m_size(0)
	, m_encoding(text::encoding::none) {
}

raw_str::raw_str(void* v, wsize s, text::encoding e)
	: m_value(v)
	, m_size(s)
	, m_encoding(e) {
}

raw_str::raw_str(raw_str const& str)
	: m_value(str.m_value)
	, m_size(str.m_size)
	, m_encoding(str.m_encoding) {
}

void* raw_str::ptr()const { return m_value; }

wsize raw_str::size()const { return m_size; }

wsize raw_str::count()const { return m_size / text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

wsize raw_str::char_size()const { return text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

text::encoding_t raw_str::encoding()const { return m_encoding; }

bool text::operator == (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool text::operator == (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool text::operator != (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool text::operator != (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool text::operator >= (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool text::operator >= (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool text::operator <= (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool text::operator <= (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool text::operator > (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool text::operator > (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool text::operator < (raw_str_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}
bool text::operator < (raw_str_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////


raw_cstr::raw_cstr()
	: m_value(null)
	, m_size(0)
	, m_encoding(text::encoding::none) {
}

raw_cstr::raw_cstr(void const* v, wsize s, text::encoding e)
	: m_value(v)
	, m_size(s)
	, m_encoding(e) {
	if (s == invalid_index)
		m_size = iencoder::get_encoder(e)->lenght(v);
}

raw_cstr::raw_cstr(raw_cstr const& str)
	: m_value(str.m_value)
	, m_size(str.m_size)
	, m_encoding(str.m_encoding) {
}

raw_cstr::raw_cstr(raw_str const& str)
	: m_value(str.ptr())
	, m_size(str.size())
	, m_encoding(str.encoding()) {
}

void const* raw_cstr::ptr()const { return m_value; }

wsize raw_cstr::size()const { return m_size; }

wsize raw_cstr::count()const { return m_size / text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

wsize raw_cstr::char_size()const { return text::encoder<text::encoding::auto_detect>::char_size_by_encoding(m_encoding); }

text::encoding_t raw_cstr::encoding()const { return m_encoding; }


bool text::operator == (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool text::operator == (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) == 0;
}
bool text::operator != (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool text::operator != (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) != 0;
}
bool text::operator >= (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool text::operator >= (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) >= 0;
}
bool text::operator <= (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool text::operator <= (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) <= 0;
}
bool text::operator > (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool text::operator > (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) > 0;
}
bool text::operator < (raw_cstr_t const& str1, raw_str_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}
bool text::operator < (raw_cstr_t const& str1, raw_cstr_t const& str2) {
	return iencoder::get_encoder(str1.encoding())->compare(str1.ptr(), str2.ptr(), str2.encoding()) < 0;
}
