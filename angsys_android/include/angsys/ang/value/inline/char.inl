#ifndef __ANG_CHAR_H__
#error ...
#elif !defined __ANG_CHAR_HPP__
#define __ANG_CHAR_HPP__


inline ang::value<char>::operator char& () { return get(); }
inline ang::value<char>::operator type const& ()const { return get(); }
inline ang::value<char>::operator value<wchar>()const { return (wchar)(mchar)get(); }
inline ang::value<char*> ang::value<char>::operator & () { return{ &get(), (uint)-1}; }
inline ang::value<char const*> ang::value<char>::operator & ()const { return { &get(), (uint)-1}; }
	
inline ang::value<char const>::operator char const&()const { return get(); }
inline ang::value<char const>::operator value<wchar>()const { return (wchar)(mchar)get(); }
inline ang::value<char const*> ang::value<char const>::operator & ()const { return { &get(), (uint)-1}; }

inline ang::value<char&>::operator char& () { return get(); }
inline ang::value<char&>::operator char()const { return get(); }
inline ang::value<char&>::operator value<wchar>()const { return (wchar)(mchar)get(); }
inline ang::value<char*> ang::value<char&>::operator & () { return { &get(), (uint)-1}; }
inline ang::value<char const*> ang::value<char&>::operator & ()const { return { &get(), (uint)-1}; }
	
inline ang::value<char const&>::operator char()const { return get(); }
inline ang::value<char const&>::operator value<wchar>()const { return (wchar)(mchar)get(); }
inline ang::value<char const*> ang::value<char const&>::operator & ()const { return { &get(), (uint)-1}; }
	
inline ang::value<char*>::operator char*& () { return get(); }
inline ang::value<char*>::operator char* ()const { return get(); }
inline ang::value<char>& ang::value<char*>::operator * () { return *reinterpret_cast<value<char>*>(get()); }
inline ang::value<char> const&  ang::value<char*>::operator * ()const { return *reinterpret_cast<value<char> const*>(get()); }
inline ang::value<char>& ang::value<char*>::operator [] (ang_int32_t idx) { return *reinterpret_cast<value<char>*>(&get()[idx]); }
inline ang::value<char>& ang::value<char*>::operator [] (ang_uint32_t idx) { return *reinterpret_cast<value<char>*>(&get()[idx]); }
inline ang::value<char> const& ang::value<char*>::operator [] (ang_int32_t idx)const { return *reinterpret_cast<value<char> const*>(&get()[idx]); }
inline ang::value<char> const& ang::value<char*>::operator [] (ang_uint32_t idx)const { return *reinterpret_cast<value<char> const*>(&get()[idx]); }
	
inline ang::value<char const*>::operator char const*& () { return get(); }
inline ang::value<char const*>::operator char const* ()const { return get(); }
inline ang::value<char const> & ang::value<char const*>::operator * () { return *reinterpret_cast<value<char const>*>((void*)get()); }
inline ang::value<char const> const& ang::value<char const*>::operator * ()const { return *reinterpret_cast<value<char const> const*>((void*)get()); }
inline ang::value<char> const& ang::value<char const*>::operator [] (ang_int32_t idx)const { return *reinterpret_cast<value<char> const*>(&get()[idx]); }
inline ang::value<char> const& ang::value<char const*>::operator [] (ang_uint32_t idx)const { return *reinterpret_cast<value<char> const*>(&get()[idx]); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


inline ang::value<wchar>::operator wchar& () { return get(); }
inline ang::value<wchar>::operator type const& ()const { return get(); }
inline ang::value<wchar>::operator value<char>()const { return (char)(mchar)get(); }
inline ang::value<wchar*> ang::value<wchar>::operator & () { return { &get(), (uint)-1}; }
inline ang::value<wchar const*> ang::value<wchar>::operator & ()const { return { &get(), (uint)-1}; }

inline ang::value<wchar const>::operator wchar const&()const { return get(); }
inline ang::value<wchar const>::operator value<char>()const { return (char)(mchar)get(); }
inline ang::value<wchar const*> ang::value<wchar const>::operator & ()const { return { &get(), (uint)-1}; }

inline ang::value<wchar&>::operator wchar& () { return get(); }
inline ang::value<wchar&>::operator wchar()const { return get(); }
inline ang::value<wchar&>::operator value<char>()const { return (char)(mchar)get(); }
inline ang::value<wchar*> ang::value<wchar&>::operator & () { return { &get(), (uint)-1}; }
inline ang::value<wchar const*> ang::value<wchar&>::operator & ()const { return { &get(), (uint)-1}; }

inline ang::value<wchar const&>::operator wchar()const { return get(); }
inline ang::value<wchar const&>::operator value<char>()const { return (char)(mchar)get(); }
inline ang::value<wchar const*> ang::value<wchar const&>::operator & ()const { return { &get(), (uint)-1}; }

inline ang::value<wchar*>::operator wchar*& () { return get(); }
inline ang::value<wchar*>::operator wchar* ()const { return get(); }
inline ang::value<wchar>& ang::value<wchar*>::operator * () { return *reinterpret_cast<value<wchar>*>(get()); }
inline ang::value<wchar> const&  ang::value<wchar*>::operator * ()const { return *reinterpret_cast<value<wchar> const*>(get()); }
inline ang::value<wchar>& ang::value<wchar*>::operator [] (ang_int32_t idx) { return *reinterpret_cast<value<wchar>*>(&get()[idx]); }
inline ang::value<wchar>& ang::value<wchar*>::operator [] (ang_uint32_t idx) { return *reinterpret_cast<value<wchar>*>(&get()[idx]); }
inline ang::value<wchar> const& ang::value<wchar*>::operator [] (ang_int32_t idx)const { return *reinterpret_cast<value<wchar> const*>(&get()[idx]); }
inline ang::value<wchar> const& ang::value<wchar*>::operator [] (ang_uint32_t idx)const { return *reinterpret_cast<value<wchar> const*>(&get()[idx]); }

inline ang::value<wchar const*>::operator wchar const*& () { return get(); }
inline ang::value<wchar const*>::operator wchar const* ()const { return get(); }
inline ang::value<wchar const> & ang::value<wchar const*>::operator * () { return *reinterpret_cast<value<wchar const>*>((void*)get()); }
inline ang::value<wchar const> const& ang::value<wchar const*>::operator * ()const { return *reinterpret_cast<value<wchar const> const*>((void*)get()); }
inline ang::value<wchar> const& ang::value<wchar const*>::operator [] (ang_int32_t idx)const { return *reinterpret_cast<value<wchar> const*>(&get()[idx]); }
inline ang::value<wchar> const& ang::value<wchar const*>::operator [] (ang_uint32_t idx)const { return *reinterpret_cast<value<wchar> const*>(&get()[idx]); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


inline ang::value<mchar>::operator mchar& () { return get(); }
inline ang::value<mchar>::operator mchar const&()const { return get(); }
inline ang::value<mchar*> ang::value<mchar>::operator & () { return { &get(), (uint)-1}; }
inline ang::value<mchar const*> ang::value<mchar>::operator & ()const { return { &get(), (uint)-1}; }

inline ang::value<mchar const>::operator mchar const&()const { return get(); }
inline ang::value<mchar const*> ang::value<mchar const>::operator & ()const { return { &get(), (uint)-1}; }

inline ang::value<mchar&>::operator mchar& () { return get(); }
inline ang::value<mchar&>::operator mchar()const { return get(); }
inline ang::value<mchar*> ang::value<mchar&>::operator & () { return { &get(), (uint)-1}; }
inline ang::value<mchar const*> ang::value<mchar&>::operator & ()const { return { &get(), (uint)-1}; }

inline ang::value<mchar const&>::operator mchar()const { return get(); }
inline ang::value<mchar const*> ang::value<mchar const&>::operator & ()const { return { &get(), (uint)-1}; }

inline ang::value<mchar*>::operator mchar*& () { return get(); }
inline ang::value<mchar*>::operator mchar* ()const { return get(); }
inline ang::value<mchar>& ang::value<mchar*>::operator * () { return *reinterpret_cast<value<mchar>*>(get()); }
inline ang::value<mchar> const&  ang::value<mchar*>::operator * ()const { return *reinterpret_cast<value<mchar> const*>(get()); }
inline ang::value<mchar>& ang::value<mchar*>::operator [] (ang_int32_t idx) { return *reinterpret_cast<value<mchar>*>(&get()[idx]); }
inline ang::value<mchar>& ang::value<mchar*>::operator [] (ang_uint32_t idx) { return *reinterpret_cast<value<mchar>*>(&get()[idx]); }
inline ang::value<mchar> const& ang::value<mchar*>::operator [] (ang_int32_t idx)const { return *reinterpret_cast<value<mchar> const*>(&get()[idx]); }
inline ang::value<mchar> const& ang::value<mchar*>::operator [] (ang_uint32_t idx)const { return *reinterpret_cast<value<mchar> const*>(&get()[idx]); }

inline ang::value<mchar const*>::operator mchar const*& () { return get(); }
inline ang::value<mchar const*>::operator mchar const* ()const { return get(); }
inline ang::value<mchar const> & ang::value<mchar const*>::operator * () { return *reinterpret_cast<value<mchar const>*>((void*)get()); }
inline ang::value<mchar const> const& ang::value<mchar const*>::operator * ()const { return *reinterpret_cast<value<mchar const> const*>((void*)get()); }
inline ang::value<mchar> const& ang::value<mchar const*>::operator [] (ang_int32_t idx)const { return *reinterpret_cast<value<mchar> const*>(&get()[idx]); }
inline ang::value<mchar> const& ang::value<mchar const*>::operator [] (ang_uint32_t idx)const { return *reinterpret_cast<value<mchar> const*>(&get()[idx]); }

#endif//__ANG_CHAR_HPP__
