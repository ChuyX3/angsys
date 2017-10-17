#ifndef __ANG_BASE_STRING_HPP__
#error ...
#elif !defined __ANG_BASE_STRING_INL__
#define __ANG_BASE_STRING_INL__

inline ang::safe_str<char>::operator char*& () { return get(); }
inline ang::safe_str<char>::operator char* ()const { return get(); }
inline char&  ang::safe_str<char>::operator * () { return *get(); }
inline char const&  ang::safe_str<char>::operator * ()const { return *get(); }
template<typename I>inline char& ang::safe_str<char>::operator [] (I idx) { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }
template<typename I>inline char const& ang::safe_str<char>::operator [] (I idx)const { return get()[idx]; }
	
inline ang::safe_str<char const>::operator char const*& () { return get(); }
inline ang::safe_str<char const>::operator char const* ()const { return get(); }
inline char const& ang::safe_str<char const>::operator * ()const { return *get(); }
template<typename I>inline char const& ang::safe_str<char const>::operator [] (I idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }

template<typename T> wsize ang::safe_str<char const>::find(safe_str<T> const& cstr, wsize start)const { return text::get_encoder_by_type<char>().find(get(), size(), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char const>::find(safe_str<T> const& cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char>().find(get(), min(end,size()), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char const>::find(typename text::char_type_by_type<T>::char_t cstr, wsize start)const { return text::get_encoder_by_type<char>().find(get(), size(), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char const>::find(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char>().find(get(), min(end, size()), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char const>::find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const { return text::get_encoder_by_type<char>().find(get(), size(), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<char const>::find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char>().find(get(), min(end, size()), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<char const>::find_revert(safe_str<T> const& cstr, wsize start)const { return text::get_encoder_by_type<char>().find_revert(get(), size(), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char const>::find_revert(safe_str<T> const& cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char>().find_revert(get(), min(end, size()), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char const>::find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start)const { return text::get_encoder_by_type<char>().find_revert(get(), size(), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char const>::find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char>().find_revert(get(), min(end, size()), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char const>::find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const { return text::get_encoder_by_type<char>().find_revert(get(), size(), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<char const>::find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char>().find_revert(get(), min(end, size()), cstr, text::get_encoder_by_type<T>().length(cstr), start); }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline ang::safe_str<wchar>::operator wchar*& () { return get(); }
inline ang::safe_str<wchar>::operator wchar* ()const { return get(); }
inline wchar& ang::safe_str<wchar>::operator * () { return *get(); }
inline wchar const&  ang::safe_str<wchar>::operator * ()const { return *get(); }
template<typename I>inline wchar& ang::safe_str<wchar>::operator [] (I idx) { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }
template<typename I>inline wchar const& ang::safe_str<wchar>::operator [] (I idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }

inline ang::safe_str<wchar const>::operator wchar const*& () { return get(); }
inline ang::safe_str<wchar const>::operator wchar const* ()const { return get(); }
inline wchar const& ang::safe_str<wchar const>::operator * ()const { return *get(); }
template<typename I>inline wchar const& ang::safe_str<wchar const>::operator [] (I idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }

template<typename T> wsize ang::safe_str<wchar const>::find(safe_str<T> const& cstr, wsize start)const { return text::get_encoder_by_type<wchar>().find(get(), size(), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<wchar const>::find(safe_str<T> const& cstr, wsize start, wsize end)const { return text::get_encoder_by_type<wchar>().find(get(), min(end, size()), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<wchar const>::find(typename text::char_type_by_type<T>::char_t cstr, wsize start)const { return text::get_encoder_by_type<wchar>().find(get(), size(), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<wchar const>::find(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<wchar>().find(get(), min(end, size()), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<wchar const>::find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const { return text::get_encoder_by_type<wchar>().find(get(), size(), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<wchar const>::find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<wchar>().find(get(), min(end, size()), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<wchar const>::find_revert(safe_str<T> const& cstr, wsize start)const { return text::get_encoder_by_type<wchar>().find_revert(get(), size(), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<wchar const>::find_revert(safe_str<T> const& cstr, wsize start, wsize end)const { return text::get_encoder_by_type<wchar>().find_revert(get(), min(end, size()), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<wchar const>::find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start)const { return text::get_encoder_by_type<wchar>().find_revert(get(), size(), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<wchar const>::find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<wchar>().find_revert(get(), min(end, size()), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<wchar const>::find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const { return text::get_encoder_by_type<wchar>().find_revert(get(), size(), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<wchar const>::find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<wchar>().find_revert(get(), min(end, size()), cstr, text::get_encoder_by_type<T>().length(cstr), start); }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline ang::safe_str<mchar>::operator mchar*& () { return get(); }
inline ang::safe_str<mchar>::operator mchar* ()const { return get(); }
inline mchar& ang::safe_str<mchar>::operator * () { return *get(); }
inline mchar const&  ang::safe_str<mchar>::operator * ()const { return *get(); }
template<typename I>inline mchar& ang::safe_str<mchar>::operator [] (I idx) { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return *&get()[idx]; }
template<typename I>inline mchar const& ang::safe_str<mchar>::operator [] (I idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }

inline ang::safe_str<mchar const>::operator mchar const*& () { return get(); }
inline ang::safe_str<mchar const>::operator mchar const* ()const { return get(); }
inline mchar const& ang::safe_str<mchar const>::operator * ()const { return *get(); }
template<typename I>inline mchar const& ang::safe_str<mchar const>::operator [] (I idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }

template<typename T> wsize ang::safe_str<mchar const>::find(safe_str<T> const& cstr, wsize start)const { return text::get_encoder_by_type<mchar>().find(get(), size(), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<mchar const>::find(safe_str<T> const& cstr, wsize start, wsize end)const { return text::get_encoder_by_type<mchar>().find(get(), min(end, size()), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<mchar const>::find(typename text::char_type_by_type<T>::char_t cstr, wsize start)const { return text::get_encoder_by_type<mchar>().find(get(), size(), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<mchar const>::find(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<mchar>().find(get(), min(end, size()), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<mchar const>::find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const { return text::get_encoder_by_type<mchar>().find(get(), size(), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<mchar const>::find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<mchar>().find(get(), min(end, size()), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<mchar const>::find_revert(safe_str<T> const& cstr, wsize start)const { return text::get_encoder_by_type<mchar>().find_revert(get(), size(), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<mchar const>::find_revert(safe_str<T> const& cstr, wsize start, wsize end)const { return text::get_encoder_by_type<mchar>().find_revert(get(), min(end, size()), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<mchar const>::find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start)const { return text::get_encoder_by_type<mchar>().find_revert(get(), size(), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<mchar const>::find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<mchar>().find_revert(get(), min(end, size()), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<mchar const>::find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const { return text::get_encoder_by_type<mchar>().find_revert(get(), size(), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<mchar const>::find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<mchar>().find_revert(get(), min(end, size()), cstr, text::get_encoder_by_type<T>().length(cstr), start); }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline ang::safe_str<char16_t>::operator char16_t*& () { return get(); }
inline ang::safe_str<char16_t>::operator char16_t* ()const { return get(); }
inline char16_t& ang::safe_str<char16_t>::operator * () { return *get(); }
inline char16_t const&  ang::safe_str<char16_t>::operator * ()const { return *get(); }
template<typename I>inline char16_t& ang::safe_str<char16_t>::operator [] (I idx) { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }
template<typename I>inline char16_t const& ang::safe_str<char16_t>::operator [] (I idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }

inline ang::safe_str<char16_t const>::operator char16_t const*& () { return get(); }
inline ang::safe_str<char16_t const>::operator char16_t const* ()const { return get(); }
inline char16_t const& ang::safe_str<char16_t const>::operator * ()const { return *get(); }
template<typename I>inline char16_t const& ang::safe_str<char16_t const>::operator [] (I idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }

template<typename T> wsize ang::safe_str<char16_t const>::find(safe_str<T> const& cstr, wsize start)const { return text::get_encoder_by_type<char16_t>().find(get(), size(), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char16_t const>::find(safe_str<T> const& cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char16_t>().find(get(), min(end, size()), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char16_t const>::find(typename text::char_type_by_type<T>::char_t cstr, wsize start)const { return text::get_encoder_by_type<char16_t>().find(get(), size(), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char16_t const>::find(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char16_t>().find(get(), min(end, size()), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char16_t const>::find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const { return text::get_encoder_by_type<char16_t>().find(get(), size(), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<char16_t const>::find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char16_t>().find(get(), min(end, size()), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<char16_t const>::find_revert(safe_str<T> const& cstr, wsize start)const { return text::get_encoder_by_type<char16_t>().find_revert(get(), size(), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char16_t const>::find_revert(safe_str<T> const& cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char16_t>().find_revert(get(), min(end, size()), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char16_t const>::find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start)const { return text::get_encoder_by_type<char16_t>().find_revert(get(), size(), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char16_t const>::find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char16_t>().find_revert(get(), min(end, size()), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char16_t const>::find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const { return text::get_encoder_by_type<char16_t>().find_revert(get(), size(), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<char16_t const>::find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char16_t>().find_revert(get(), min(end, size()), cstr, text::get_encoder_by_type<T>().length(cstr), start); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline ang::safe_str<char32_t>::operator char32_t*& () { return get(); }
inline ang::safe_str<char32_t>::operator char32_t* ()const { return get(); }
inline char32_t& ang::safe_str<char32_t>::operator * () { return *get(); }
inline char32_t const&  ang::safe_str<char32_t>::operator * ()const { return *get(); }
template<typename I>inline char32_t& ang::safe_str<char32_t>::operator [] (I idx) { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }
template<typename I>inline char32_t const& ang::safe_str<char32_t>::operator [] (I idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }

inline ang::safe_str<char32_t const>::operator char32_t const*& () { return get(); }
inline ang::safe_str<char32_t const>::operator char32_t const* ()const { return get(); }
inline char32_t const& ang::safe_str<char32_t const>::operator * ()const { return *get(); }
template<typename I>inline char32_t const& ang::safe_str<char32_t const>::operator [] (I idx)const { static_assert(integer_value<I>::is_integer_value, "no integer value is no accepted"); return get()[idx]; }

template<typename T> wsize ang::safe_str<char32_t const>::find(safe_str<T> const& cstr, wsize start)const { return text::get_encoder_by_type<char32_t>().find(get(), size(), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char32_t const>::find(safe_str<T> const& cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char32_t>().find(get(), min(end, size()), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char32_t const>::find(typename text::char_type_by_type<T>::char_t cstr, wsize start)const { return text::get_encoder_by_type<char32_t>().find(get(), size(), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char32_t const>::find(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char32_t>().find(get(), min(end, size()), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char32_t const>::find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const { return text::get_encoder_by_type<char32_t>().find(get(), size(), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<char32_t const>::find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char32_t>().find(get(), min(end, size()), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<char32_t const>::find_revert(safe_str<T> const& cstr, wsize start)const { return text::get_encoder_by_type<char32_t>().find_revert(get(), size(), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char32_t const>::find_revert(safe_str<T> const& cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char32_t>().find_revert(get(), min(end, size()), cstr.get(), cstr.size(), start); }
template<typename T> wsize ang::safe_str<char32_t const>::find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start)const { return text::get_encoder_by_type<char32_t>().find_revert(get(), size(), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char32_t const>::find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char32_t>().find_revert(get(), min(end, size()), &cstr, 1, start); }
template<typename T> wsize ang::safe_str<char32_t const>::find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const { return text::get_encoder_by_type<char32_t>().find_revert(get(), size(), cstr, text::get_encoder_by_type<T>().length(cstr), start); }
template<typename T> wsize ang::safe_str<char32_t const>::find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const { return text::get_encoder_by_type<char32_t>().find_revert(get(), min(end, size()), cstr, text::get_encoder_by_type<T>().length(cstr), start); }


#endif//__ANG_BASE_STRING_INL__
