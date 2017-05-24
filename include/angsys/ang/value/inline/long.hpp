#ifndef __ANG_LONG_H__
#error ...
#elif !defined __ANG_LONG_HPP__
#define __ANG_LONG_HPP__

inline ang::value<long>::operator long& () { return get(); }
inline ang::value<long>::operator type ()const { return get(); }
inline ang::value<long*> ang::value<long>::operator & () { return &get(); }
inline ang::value<long const*> ang::value<long>::operator & ()const { return &get(); }

inline ang::value<long const>::operator long()const { return get(); }
inline ang::value<long const*> ang::value<long const>::operator & ()const { return &get(); }

inline ang::value<long&>::operator long& () { return get(); }
inline ang::value<long&>::operator long()const { return get(); }
inline ang::value<long*> ang::value<long&>::operator & () { return &get(); }
inline ang::value<long const*> ang::value<long&>::operator & ()const { return &get(); }

inline ang::value<long const&>::operator long()const { return get(); }
inline ang::value<long const*> ang::value<long const&>::operator & ()const { return &get(); }

inline ang::value<long*>::operator long*& () { return get(); }
inline ang::value<long*>::operator long* ()const { return get(); }
inline ang::value<long>& ang::value<long*>::operator * () { return *reinterpret_cast<value<long>*>(get()); }
inline ang::value<long> const&  ang::value<long*>::operator * ()const { return *reinterpret_cast<value<long> const*>(get()); }

inline ang::value<long const*>::operator long const*& () { return get(); }
inline ang::value<long const*>::operator long const* ()const { return get(); }
inline ang::value<long const> & ang::value<long const*>::operator * () { return *reinterpret_cast<value<long const>*>((void*)get()); }
inline ang::value<long const> const& ang::value<long const*>::operator * ()const { return *reinterpret_cast<value<long const> const*>((void*)get()); }

inline ang::value<long64>::operator long64& () { return get(); }
inline ang::value<long64>::operator type ()const { return get(); }
inline ang::value<long64*> ang::value<long64>::operator & () { return &get(); }
inline ang::value<long64 const*> ang::value<long64>::operator & ()const { return &get(); }

inline ang::value<long64 const>::operator long64()const { return get(); }
inline ang::value<long64 const*> ang::value<long64 const>::operator & ()const { return &get(); }

inline ang::value<long64&>::operator long64& () { return get(); }
inline ang::value<long64&>::operator long64()const { return get(); }
inline ang::value<long64*> ang::value<long64&>::operator & () { return &get(); }
inline ang::value<long64 const*> ang::value<long64&>::operator & ()const { return &get(); }

inline ang::value<long64 const&>::operator long64()const { return get(); }
inline ang::value<long64 const*> ang::value<long64 const&>::operator & ()const { return &get(); }

inline ang::value<long64*>::operator long64*& () { return get(); }
inline ang::value<long64*>::operator long64* ()const { return get(); }
inline ang::value<long64>& ang::value<long64*>::operator * () { return *reinterpret_cast<value<long64>*>(get()); }
inline ang::value<long64> const&  ang::value<long64*>::operator * ()const { return *reinterpret_cast<value<long64> const*>(get()); }

inline ang::value<long64 const*>::operator long64 const*& () { return get(); }
inline ang::value<long64 const*>::operator long64 const* ()const { return get(); }
inline ang::value<long64 const> & ang::value<long64 const*>::operator * () { return *reinterpret_cast<value<long64 const>*>((void*)get()); }
inline ang::value<long64 const> const& ang::value<long64 const*>::operator * ()const { return *reinterpret_cast<value<long64 const> const*>((void*)get()); }

#endif//__ANG_LONG_HPP__
