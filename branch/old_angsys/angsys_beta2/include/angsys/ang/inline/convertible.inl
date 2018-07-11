/*********************************************************************************************************************/
/*   File Name: ang/inline/convertible.hpp                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_HPP__
#error Can't include convertible.inl, please include angsys.h inside
#elif !defined __CONVERTIBLE_INL__
#define __CONVERTIBLE_INL__

//template<typename T>
//T* ang::object::as<ang::object_wrapper<T>>() {
//	return interface_cast<T>(this);
//}
//

inline ang::object_wrapper<ang::object>::object_wrapper(bool value)
	: object_wrapper() {
	set(new value_wrapper<bool>(value));
}

template<typename T>
inline ang::object_wrapper<ang::object>::object_wrapper(ang::safe_str<T> const& cstr)
	: object_wrapper() {
	set(new strings::string_base<text::encoding_by_type<T>::encoding()>(cstr));
}

template<wsize N> inline ang::object_wrapper<ang::object>::object_wrapper(const char(&ar)[N])
	: object_wrapper(ang::cstr_t(ar, N - 1U)) { }

template<wsize N> inline ang::object_wrapper<ang::object>::object_wrapper(const wchar(&ar)[N])
	: object_wrapper(ang::cwstr_t(ar, N - 1U)) { }

inline ang::object_wrapper<ang::object>::object_wrapper(int value)
	: object_wrapper() {
	set(new ang::value_wrapper<int>(value));
}

inline ang::object_wrapper<ang::object>::object_wrapper(uint value)
	: object_wrapper() {
	set(new ang::value_wrapper<uint>(value));
}

inline ang::object_wrapper<ang::object>::object_wrapper(long value)
	: object_wrapper() {
	set(new ang::value_wrapper<long>(value));
}

inline ang::object_wrapper<ang::object>::object_wrapper(ulong value)
	: object_wrapper() {
	set(new ang::value_wrapper<ulong>(value));
}

inline ang::object_wrapper<ang::object>::object_wrapper(long64 value)
	: object_wrapper() {
	set(new ang::value_wrapper<long64>(value));
}

inline ang::object_wrapper<ang::object>::object_wrapper(ulong64 value)
	: object_wrapper() {
	set(new ang::value_wrapper<ulong64>(value));
}

inline ang::object_wrapper<ang::object>::object_wrapper(float value)
	: object_wrapper() {
	set(new ang::value_wrapper<float>(value));
}

inline ang::object_wrapper<ang::object>::object_wrapper(double value)
	: object_wrapper() {
	set(new ang::value_wrapper<double>(value));
}

//template<typename T>//object convertible
//inline ang::object_wrapper<ang::object>::object_wrapper(T const& val)
//	: object_wrapper() {
//	set(new value_wrapper<T>(val));
//}

template<typename T>//object convertible
inline ang::object_wrapper<ang::object>::object_wrapper(ang::object_wrapper<T> obj)
	: object_wrapper() {
	set(obj.get());
}


template<typename T>//array convertible
inline ang::object_wrapper<ang::object>::object_wrapper(ang::initializer_list_t<T> list)
	: object_wrapper() {
	array<T> arr(list);
	set(arr.get());
}

template<>//array convertible
inline ang::object_wrapper<ang::object>::object_wrapper(ang::initializer_list_t<object_wrapper<ang::object>> list)
	: object_wrapper() {
	array<object_wrapper<ang::object>> arr(list);
	set(arr.get());
}

template<>//array convertible
inline ang::object_wrapper<ang::object>::object_wrapper(ang::initializer_list_t<ang::cstr_t> list)
	: object_wrapper() {
	array<string> arr(list);
	set(arr.get());
}

template<>//array convertible
inline ang::object_wrapper<ang::object>::object_wrapper(ang::initializer_list_t<ang::cwstr_t> list)
	: object_wrapper() {
	array<wstring> arr(list);
	set(arr.get());
}



#endif//__CONVERTIBLE_INL__