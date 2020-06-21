#include "pch.h"
#include "angsys.h"

#include "inline/string_operations.hpp"

using namespace ang;
using namespace ang::text;

ang::intf_wrapper<istring>::intf_wrapper() : m_ptr(null) { }
ang::intf_wrapper<istring>::intf_wrapper(ang::nullptr_t const&) : m_ptr(null) { }
ang::intf_wrapper<istring>::intf_wrapper(istring* ptr) : m_ptr(null) {
	set(ptr);
}
ang::intf_wrapper<istring>::intf_wrapper(intf_wrapper && other) : m_ptr(null) {
	istring * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}
ang::intf_wrapper<istring>::intf_wrapper(intf_wrapper const& other) : m_ptr(null) {
	set(other.m_ptr);
}
ang::intf_wrapper<istring>::~intf_wrapper() {
	reset();
}
void ang::intf_wrapper<istring>::reset()
{
	iobject * _obj = interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}
void ang::intf_wrapper<istring>::reset_unsafe() {
	m_ptr = null;
}
bool ang::intf_wrapper<istring>::is_empty()const {
	return m_ptr == null || m_ptr->length() == 0;
}
istring* ang::intf_wrapper<istring>::get(void)const {
	return m_ptr;
}
void ang::intf_wrapper<istring>::set(istring* ptr) {
	if (ptr == m_ptr) return;
	iobject * _old = interface_cast<iobject>(m_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}
ang::intf_wrapper<istring>& ang::intf_wrapper<istring>::operator = (istring* ptr) {
	set(ptr);
	return*this;
}
ang::intf_wrapper<istring>& ang::intf_wrapper<istring>::operator = (ang::nullptr_t const&) {
	reset();
	return*this;
}
ang::intf_wrapper<istring>& ang::intf_wrapper<istring>::operator = (ang::intf_wrapper<istring> && other) {
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}
ang::intf_wrapper<istring>& ang::intf_wrapper<istring>::operator = (ang::intf_wrapper<istring> const& other) {
	set(other.m_ptr);
	return*this;
}
istring ** ang::intf_wrapper<istring>::addres_of(void) {
	return &m_ptr;
}
ang::intf_wrapper_ptr<istring> ang::intf_wrapper<istring>::operator & (void) {
	return this;
}
ang::intf_wrapper<istring>::operator ang::intfptr()const {
	return static_interface_cast(get());
}
istring* ang::intf_wrapper<istring>::operator -> (void)const {
	return get();
}
ang::intf_wrapper<istring>::operator istring* (void) {
	return get();
}
ang::intf_wrapper<istring>::operator istring const* (void)const {
	return get();
}
intf_wrapper<istring>::intf_wrapper(cstr_t cstr) : m_ptr(null) {
	auto str = create_new(cstr);
	set(str.get());
}
intf_wrapper<istring>::operator cstr_t()const {
#ifdef _DEBUG
	return is_empty() ? raw_cstr() : m_ptr->cstr();
#else
	return m_ptr->cstr();
#endif
}
char32_t intf_wrapper<istring>::operator [](windex i)const {

#ifdef _DEBUG
	return is_empty() ? 0 : m_ptr->at(i);
#else
	return m_ptr->at(i);
#endif
}

//////////////////////////////////////////////////////////////////////

string istring::create_string(cstr_t cstr)
{
	return string_operations<text::encoding::ascii>::create_string(cstr);
}

#define MY_ALLOCATOR ang::memory::buffer_allocator

#define MY_ENCODING ang::text::encoding::ascii
#include "inline/string_implement.inl"
ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(ang::text::basic_string_buffer<MY_ENCODING>, "ang::text::ascii_string"_sv);
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::unicode
#include "inline/string_implement.inl"
ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(ang::text::basic_string_buffer<MY_ENCODING>, "ang::text::unicode_string"_sv);
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf8
#include "inline/string_implement.inl"
ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(ang::text::basic_string_buffer<MY_ENCODING>, "ang::text::utf8_string"_sv);
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16
#include "inline/string_implement.inl"
ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(ang::text::basic_string_buffer<MY_ENCODING>, "ang::text::string16"_sv);
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_le
#include "inline/string_implement.inl"
ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(ang::text::basic_string_buffer<MY_ENCODING>, "ang::text::string16_le"_sv);
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_be
#include "inline/string_implement.inl"
ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(ang::text::basic_string_buffer<MY_ENCODING>, "ang::text::string16_be"_sv);
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_se
#include "inline/string_implement.inl"
ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(ang::text::basic_string_buffer<MY_ENCODING>, "ang::text::string16_se"_sv);
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32
#include "inline/string_implement.inl"
ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(ang::text::basic_string_buffer<MY_ENCODING>, "ang::text::string32"_sv);
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_le
#include "inline/string_implement.inl"
ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(ang::text::basic_string_buffer<MY_ENCODING>, "ang::text::string32_le"_sv);
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_be
#include "inline/string_implement.inl"
ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(ang::text::basic_string_buffer<MY_ENCODING>, "ang::text::string_be"_sv);
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_se
#include "inline/string_implement.inl"
ANG_IMPLEMENT_OBJECT_CLASS_INFO_OVERRIDE(ang::text::basic_string_buffer<MY_ENCODING>, "ang::text::string32_se"_sv);
#undef MY_ENCODING

#undef MY_ALLOCATOR



