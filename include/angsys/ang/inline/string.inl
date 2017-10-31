/*********************************************************************************************************************/
/*   File Name: ang/inline/runtime.hpp                                                                               */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error Can't include string.inl, please include angsys.h inside
#elif !defined __STRING_INL__
#define __STRING_INL__

namespace ang
{

	template<text::encoding_enum ENCODING>
	inline bool text::itext_buffer<ENCODING>::is_kind_of(ang::type_name_t name)const
	{
		return name == text::itext_buffer<ENCODING>::class_name() ||
			ibuffer::is_kind_of(name);
	}

	template<text::encoding_enum ENCODING>
	inline bool text::itext_buffer<ENCODING>::is_child_of(ang::type_name_t name)
	{
		return name == text::itext_buffer<ENCODING>::class_name() ||
			ibuffer::is_child_of(name);
	}

	template<text::encoding_enum ENCODING>
	inline ang::type_name_t text::itext_buffer<ENCODING>::class_name() {
		static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::text::itext_buffer<"_o += text::encoding_t(ENCODING).to_string()) += ">"_s));
		return name;
	}

	template<text::encoding_enum ENCODING>
	inline ang::type_name_t text::itext_buffer<ENCODING>::object_name()const
	{
		return text::itext_buffer<ENCODING>::class_name();
	}

	template<text::encoding_enum ENCODING>
	inline bool text::itext_buffer<ENCODING>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
	{
		if (out == null)
			return false;
		if (name == class_name())
		{
			*out = static_cast<text::itext_buffer<ENCODING>*>(this);
			return true;
		}
		else if (ibuffer::query_object(name, out))
		{
			return true;
		}
		return false;
	}


	template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	inline bool operator == (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, object_wrapper<strings::string_buffer<ENCODING2>> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(),
			((typename strings::safe_str_by_encoding<ENCODING2>::ctype)(cstr2)).cstr()) == 0;
	}

	template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	inline bool operator != (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, object_wrapper<strings::string_buffer<ENCODING2>> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(),
			((typename strings::safe_str_by_encoding<ENCODING2>::ctype)(cstr2)).cstr()) != 0;
	}

	template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	inline bool operator >= (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, object_wrapper<strings::string_buffer<ENCODING2>> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(),
			((typename strings::safe_str_by_encoding<ENCODING2>::ctype)(cstr2)).cstr()) >= 0;
	}

	template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	inline bool operator <= (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, object_wrapper<strings::string_buffer<ENCODING2>> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(),
			((typename strings::safe_str_by_encoding<ENCODING2>::ctype)(cstr2)).cstr()) <= 0;
	}

	template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	inline bool operator > (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, object_wrapper<strings::string_buffer<ENCODING2>> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(),
			((typename strings::safe_str_by_encoding<ENCODING2>::ctype)(cstr2)).cstr()) > 0;
	}

	template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	inline bool operator < (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, object_wrapper<strings::string_buffer<ENCODING2>> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(),
			((typename strings::safe_str_by_encoding<ENCODING2>::ctype)(cstr2)).cstr()) < 0;
	}


	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator == (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, safe_str<T> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2.cstr()) == 0;
	}
	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator != (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, safe_str<T> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2.cstr()) != 0;
	}
	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator >= (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, safe_str<T> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2.cstr()) >= 0;
	}
	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator <= (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, safe_str<T> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2.cstr()) <= 0;
	}
	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator > (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, safe_str<T> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2.cstr()) > 0;
	}
	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator < (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, safe_str<T> const& cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2.cstr()) < 0;
	}


	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator == (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, T const* cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2) == 0;
	}
	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator != (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, T const* cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2) != 0;
	}
	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator >= (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, T const* cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2) >= 0;
	}
	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator <= (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, T const* cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2) <= 0;
	}
	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator > (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, T const* cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2) > 0;
	}
	template<text::encoding_enum ENCODING1, typename T>
	inline bool operator < (object_wrapper<strings::string_buffer<ENCODING1>> const& cstr1, T const* cstr2) {
		return text::get_encoder<ENCODING1>().compare(((typename strings::safe_str_by_encoding<ENCODING1>::ctype)(cstr1)).cstr(), cstr2) < 0;
	}

}

#endif//__STRING_INL__