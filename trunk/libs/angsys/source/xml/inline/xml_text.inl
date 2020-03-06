#pragma once

#include "inline/string_operations.hpp"

template<coffe::text::encoding E>
inline coffe::dom::xml::xml_text<E>::xml_text()
{

}

template<coffe::text::encoding E>
inline coffe::dom::xml::xml_text<E>::xml_text(coffe::cstr_t cstr)
{
	copy(cstr);
}

template<coffe::text::encoding E>
inline coffe::dom::xml::xml_text<E>::xml_text(coffe::dom::xml::xml_text<E> && str)
{
	fast_string::move(str);
}

template<coffe::text::encoding E>
inline coffe::dom::xml::xml_text<E>::xml_text(coffe::fast_string<E> && str)
{
	fast_string::move(str);
}

template<coffe::text::encoding E>
inline coffe::dom::xml::xml_text<E>::xml_text(coffe::dom::xml::xml_text<E> const& cstr)
{
	fast_string::copy((coffe::fast_string<E> const&)cstr);
}

template<coffe::text::encoding E>
inline coffe::dom::xml::xml_text<E>::xml_text(coffe::fast_string<E> const& cstr)
{
	fast_string::copy(cstr);
}

template<coffe::text::encoding E>
inline void coffe::dom::xml::xml_text<E>::dispose()
{
	clear();
}

template<coffe::text::encoding E>
inline coffe::streams::itext_output_stream_t& coffe::dom::xml::xml_text<E>::print(coffe::streams::itext_output_stream_t& stream, const coffe::dom::xml::xml_format_t& flag, word level)const
{
	return stream;
}

template<coffe::text::encoding E>
inline coffe::string coffe::dom::xml::xml_text<E>::to_string()const
{
	return const_cast<xml_text<E>*>(this);
}

template<coffe::text::encoding E>
inline coffe::string coffe::dom::xml::xml_text<E>::to_string(coffe::text::text_format_t format)const
{
	return const_cast<xml_text<E>*>(this);
}

template<coffe::text::encoding E>
inline coffe::text::encoding_t coffe::dom::xml::xml_text<E>::encoding()const
{
	return E;
}

template<coffe::text::encoding E>
inline wsize coffe::dom::xml::xml_text<E>::length()const
{
	return fast_string::length();
}

template<coffe::text::encoding E>
inline pointer coffe::dom::xml::xml_text<E>::data()const
{
	return (pointer)fast_string::cstr().cstr();
}

template<coffe::text::encoding E>
inline coffe::rtti_t const& coffe::dom::xml::xml_text<E>::char_type()const
{
	return type_of<typename text::char_type_by_encoding<E>::char_t>();
}

template<coffe::text::encoding E>
inline coffe::str_t coffe::dom::xml::xml_text<E>::str(int)
{
	return fast_string::str();
}

template<coffe::text::encoding E>
inline coffe::cstr_t coffe::dom::xml::xml_text<E>::cstr(int)const
{
	return fast_string::cstr();
}

template<coffe::text::encoding E>
inline char32_t coffe::dom::xml::xml_text<E>::at(windex i, wsize* sz)const
{
	windex j = i;
	auto c = text::to_char32<false, text::is_endian_swapped<E>::value>(fast_string::cstr(), j);
	if (sz)*sz = j - i;
	return c;
}

template<coffe::text::encoding E>
inline int coffe::dom::xml::xml_text<E>::compare(coffe::cstr_t val)const
{
	return text::string_operations<E>::compare(*this, val);
}

template<coffe::text::encoding E>
inline int coffe::dom::xml::xml_text<E>::compare(coffe::string val)const
{
	return text::string_operations<E>::compare(*this, (cstr_t)val);
}

template<coffe::text::encoding E>
inline windex coffe::dom::xml::xml_text<E>::compare_until(coffe::cstr_t val)const
{
	return text::string_operations<E>::compare_until(*this, val);
}

template<coffe::text::encoding E>
inline windex coffe::dom::xml::xml_text<E>::compare_until(coffe::string val)const
{
	return text::string_operations<E>::compare_until(*this, (cstr_t)val);
}

template<coffe::text::encoding E>
inline windex coffe::dom::xml::xml_text<E>::find(coffe::cstr_t val, windex start, windex end)const
{
	return text::string_operations<E>::find(*this, val, start, end);
}

template<coffe::text::encoding E>
inline windex coffe::dom::xml::xml_text<E>::find(coffe::string val, windex start, windex end)const
{
	return text::string_operations<E>::find(*this, (cstr_t)val, start, end);
}

template<coffe::text::encoding E>
inline windex coffe::dom::xml::xml_text<E>::find_reverse(coffe::cstr_t val, windex start, windex end)const
{
	return text::string_operations<E>::find_reverse(*this, (cstr_t)val, start, end);
}

template<coffe::text::encoding E>
inline windex coffe::dom::xml::xml_text<E>::find_reverse(coffe::string val, windex start, windex end)const
{
	return text::string_operations<E>::find_reverse(*this, (cstr_t)val, start, end);
}

template<coffe::text::encoding E>
inline coffe::string coffe::dom::xml::xml_text<E>::sub_string(windex start, windex end)const
{
	object_wrapper<xml_text<E>> out = new xml_text<E>();
	fast_string::sub_string(*out, start, end);
	return out.get();
}

template<coffe::text::encoding E>
inline coffe::vector<coffe::string> coffe::dom::xml::xml_text<E>::split(coffe::cstr_t val)const
{
	return text::string_operations<E>::split(*this, val);
}

template<coffe::text::encoding E>
inline coffe::vector<coffe::string> coffe::dom::xml::xml_text<E>::split(coffe::string val)const
{
	return text::string_operations<E>::split(*this, (cstr_t)val);
}

template<coffe::text::encoding E>
inline void coffe::dom::xml::xml_text<E>::clear()
{
	fast_string::clear();
}

template<coffe::text::encoding E>
inline wsize coffe::dom::xml::xml_text<E>::copy(coffe::cstr_t val)
{
	return text::string_operations<E>::copy(*this, val);
}

template<coffe::text::encoding E>
inline wsize coffe::dom::xml::xml_text<E>::copy(coffe::string val)
{
	return text::string_operations<E>::copy(*this, (cstr_t)val);
}

template<coffe::text::encoding E>
inline wsize coffe::dom::xml::xml_text<E>::append(coffe::cstr_t) { return 0; }

template<coffe::text::encoding E>
inline wsize coffe::dom::xml::xml_text<E>::append(coffe::string) { return 0; }

template<coffe::text::encoding E>
inline wsize coffe::dom::xml::xml_text<E>::insert(coffe::cstr_t, windex) { return 0; }

template<coffe::text::encoding E>
inline wsize coffe::dom::xml::xml_text<E>::insert(coffe::string, windex) { return 0; }

template<coffe::text::encoding E>
inline void coffe::dom::xml::xml_text<E>::format(coffe::cstr_t, coffe::var_args_t){ }

template<coffe::text::encoding E>
inline void coffe::dom::xml::xml_text<E>::format(coffe::string, coffe::var_args_t){ }

template<coffe::text::encoding E>
inline void coffe::dom::xml::xml_text<E>::append_format(coffe::cstr_t, coffe::var_args_t){ }

template<coffe::text::encoding E>
inline void coffe::dom::xml::xml_text<E>::append_format(coffe::string, coffe::var_args_t){ }
