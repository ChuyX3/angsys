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