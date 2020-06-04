#pragma once

#include "inline/string_operations.hpp"

template<ang::text::encoding E>
inline ang::dom::xml::xml_text<E>::xml_text()
{

}

template<ang::text::encoding E>
inline ang::dom::xml::xml_text<E>::xml_text(ang::cstr_t cstr)
{
	copy(cstr);
}

template<ang::text::encoding E>
inline ang::dom::xml::xml_text<E>::xml_text(ang::dom::xml::xml_text<E> && str)
{
	fast_string::move(str);
}

template<ang::text::encoding E>
inline ang::dom::xml::xml_text<E>::xml_text(ang::fast_string<E> && str)
{
	fast_string::move(str);
}

template<ang::text::encoding E>
inline ang::dom::xml::xml_text<E>::xml_text(ang::dom::xml::xml_text<E> const& cstr)
{
	fast_string::copy((ang::fast_string<E> const&)cstr);
}

template<ang::text::encoding E>
inline ang::dom::xml::xml_text<E>::xml_text(ang::fast_string<E> const& cstr)
{
	fast_string::copy(cstr);
}

template<ang::text::encoding E>
inline void ang::dom::xml::xml_text<E>::dispose()
{
	clear();
}

template<ang::text::encoding E>
inline ang::streams::itext_output_stream_t& ang::dom::xml::xml_text<E>::print(ang::streams::itext_output_stream_t& stream, const ang::dom::xml::xml_format_t& flag, word level)const
{
	return stream;
}

template<ang::text::encoding E>
inline ang::string ang::dom::xml::xml_text<E>::to_string()const
{
	return const_cast<xml_text<E>*>(this);
}

template<ang::text::encoding E>
inline ang::string ang::dom::xml::xml_text<E>::to_string(ang::text::text_format_t format)const
{
	return const_cast<xml_text<E>*>(this);
}

template<ang::text::encoding E>
inline ang::text::encoding_t ang::dom::xml::xml_text<E>::encoding()const
{
	return E;
}

template<ang::text::encoding E>
inline wsize ang::dom::xml::xml_text<E>::length()const
{
	return fast_string::length();
}

template<ang::text::encoding E>
inline pointer ang::dom::xml::xml_text<E>::data()const
{
	return (pointer)fast_string::cstr().cstr();
}

template<ang::text::encoding E>
inline ang::rtti_t const& ang::dom::xml::xml_text<E>::char_type()const
{
	return type_of<typename text::char_type_by_encoding<E>::char_type>();
}

template<ang::text::encoding E>
inline ang::str_t ang::dom::xml::xml_text<E>::str(int)
{
	return fast_string::str();
}

template<ang::text::encoding E>
inline ang::cstr_t ang::dom::xml::xml_text<E>::cstr(int)const
{
	return fast_string::cstr();
}

template<ang::text::encoding E>
inline char32_t ang::dom::xml::xml_text<E>::at(windex i, wsize* sz)const
{
	windex j = i;
	auto c = text::to_char32<false, text::is_endian_swapped<E>::value>(fast_string::cstr(), j);
	if (sz)*sz = j - i;
	return c;
}

template<ang::text::encoding E>
inline int ang::dom::xml::xml_text<E>::compare(ang::cstr_t val)const
{
	return text::string_operations<E>::compare(*this, val);
}

template<ang::text::encoding E>
inline int ang::dom::xml::xml_text<E>::compare(ang::string val)const
{
	return text::string_operations<E>::compare(*this, (cstr_t)val);
}

template<ang::text::encoding E>
inline windex ang::dom::xml::xml_text<E>::compare_until(ang::cstr_t val)const
{
	return text::string_operations<E>::compare_until(*this, val);
}

template<ang::text::encoding E>
inline windex ang::dom::xml::xml_text<E>::compare_until(ang::string val)const
{
	return text::string_operations<E>::compare_until(*this, (cstr_t)val);
}

template<ang::text::encoding E>
inline windex ang::dom::xml::xml_text<E>::find(ang::cstr_t val, windex start, windex end)const
{
	return text::string_operations<E>::find(*this, val, start, end);
}

template<ang::text::encoding E>
inline windex ang::dom::xml::xml_text<E>::find(ang::string val, windex start, windex end)const
{
	return text::string_operations<E>::find(*this, (cstr_t)val, start, end);
}

template<ang::text::encoding E>
inline windex ang::dom::xml::xml_text<E>::find_reverse(ang::cstr_t val, windex start, windex end)const
{
	return text::string_operations<E>::find_reverse(*this, (cstr_t)val, start, end);
}

template<ang::text::encoding E>
inline windex ang::dom::xml::xml_text<E>::find_reverse(ang::string val, windex start, windex end)const
{
	return text::string_operations<E>::find_reverse(*this, (cstr_t)val, start, end);
}

template<ang::text::encoding E>
inline ang::string ang::dom::xml::xml_text<E>::sub_string(windex start, windex end)const
{
	object_wrapper<xml_text<E>> out = new xml_text<E>();
	fast_string::sub_string(*out, start, end);
	return out.get();
}

template<ang::text::encoding E>
inline ang::vector<ang::string> ang::dom::xml::xml_text<E>::split(ang::cstr_t val)const
{
	return text::string_operations<E>::split(*this, val);
}

template<ang::text::encoding E>
inline ang::vector<ang::string> ang::dom::xml::xml_text<E>::split(ang::string val)const
{
	return text::string_operations<E>::split(*this, (cstr_t)val);
}

template<ang::text::encoding E>
inline void ang::dom::xml::xml_text<E>::clear()
{
	fast_string::clear();
}

template<ang::text::encoding E>
inline wsize ang::dom::xml::xml_text<E>::copy(ang::cstr_t val)
{
	return text::string_operations<E>::copy(*this, val);
}

template<ang::text::encoding E>
inline wsize ang::dom::xml::xml_text<E>::copy(ang::string val)
{
	return text::string_operations<E>::copy(*this, (cstr_t)val);
}

template<ang::text::encoding E>
inline wsize ang::dom::xml::xml_text<E>::append(ang::cstr_t) { return 0; }

template<ang::text::encoding E>
inline wsize ang::dom::xml::xml_text<E>::append(ang::string) { return 0; }

template<ang::text::encoding E>
inline wsize ang::dom::xml::xml_text<E>::insert(ang::cstr_t, windex) { return 0; }

template<ang::text::encoding E>
inline wsize ang::dom::xml::xml_text<E>::insert(ang::string, windex) { return 0; }

template<ang::text::encoding E>
inline void ang::dom::xml::xml_text<E>::format(ang::cstr_t, ang::var_args_t){ }

template<ang::text::encoding E>
inline void ang::dom::xml::xml_text<E>::format(ang::string, ang::var_args_t){ }

template<ang::text::encoding E>
inline void ang::dom::xml::xml_text<E>::append_format(ang::cstr_t, ang::var_args_t){ }

template<ang::text::encoding E>
inline void ang::dom::xml::xml_text<E>::append_format(ang::string, ang::var_args_t){ }
