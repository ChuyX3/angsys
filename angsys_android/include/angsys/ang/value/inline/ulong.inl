#ifndef __ANG_ULONG_H__
#error ...
#elif !defined __ANG_ULONG_HPP__
#define __ANG_ULONG_HPP__

inline ang::value<ulong>::operator ulong& () { return get(); }
inline ang::value<ulong>::operator type ()const { return get(); }
inline ang::value<ulong*> ang::value<ulong>::operator & () { return &get(); }
inline ang::value<ulong const*> ang::value<ulong>::operator & ()const { return &get(); }

inline ang::value<ulong const>::operator ulong()const { return get(); }
inline ang::value<ulong const*> ang::value<ulong const>::operator & ()const { return &get(); }

inline ang::value<ulong&>::operator ulong& () { return get(); }
inline ang::value<ulong&>::operator ulong()const { return get(); }
inline ang::value<ulong*> ang::value<ulong&>::operator & () { return &get(); }
inline ang::value<ulong const*> ang::value<ulong&>::operator & ()const { return &get(); }

inline ang::value<ulong const&>::operator ulong()const { return get(); }
inline ang::value<ulong const*> ang::value<ulong const&>::operator & ()const { return &get(); }

inline ang::value<ulong*>::operator ulong*& () { return get(); }
inline ang::value<ulong*>::operator ulong* ()const { return get(); }
inline ang::value<ulong>& ang::value<ulong*>::operator * () { return *reinterpret_cast<value<ulong>*>(get()); }
inline ang::value<ulong> const&  ang::value<ulong*>::operator * ()const { return *reinterpret_cast<value<ulong> const*>(get()); }

inline ang::value<ulong const*>::operator ulong const*& () { return get(); }
inline ang::value<ulong const*>::operator ulong const* ()const { return get(); }
inline ang::value<ulong const> & ang::value<ulong const*>::operator * () { return *reinterpret_cast<value<ulong const>*>((void*)get()); }
inline ang::value<ulong const> const& ang::value<ulong const*>::operator * ()const { return *reinterpret_cast<value<ulong const> const*>((void*)get()); }


inline ang::value<ulong64>::operator ulong64& () { return get(); }
inline ang::value<ulong64>::operator type ()const { return get(); }
inline ang::value<ulong64*> ang::value<ulong64>::operator & () { return &get(); }
inline ang::value<ulong64 const*> ang::value<ulong64>::operator & ()const { return &get(); }

inline ang::value<ulong64 const>::operator ulong64()const { return get(); }
inline ang::value<ulong64 const*> ang::value<ulong64 const>::operator & ()const { return &get(); }

inline ang::value<ulong64&>::operator ulong64& () { return get(); }
inline ang::value<ulong64&>::operator ulong64()const { return get(); }
inline ang::value<ulong64*> ang::value<ulong64&>::operator & () { return &get(); }
inline ang::value<ulong64 const*> ang::value<ulong64&>::operator & ()const { return &get(); }

inline ang::value<ulong64 const&>::operator ulong64()const { return get(); }
inline ang::value<ulong64 const*> ang::value<ulong64 const&>::operator & ()const { return &get(); }

inline ang::value<ulong64*>::operator ulong64*& () { return get(); }
inline ang::value<ulong64*>::operator ulong64* ()const { return get(); }
inline ang::value<ulong64>& ang::value<ulong64*>::operator * () { return *reinterpret_cast<value<ulong64>*>(get()); }
inline ang::value<ulong64> const&  ang::value<ulong64*>::operator * ()const { return *reinterpret_cast<value<ulong64> const*>(get()); }

inline ang::value<ulong64 const*>::operator ulong64 const*& () { return get(); }
inline ang::value<ulong64 const*>::operator ulong64 const* ()const { return get(); }
inline ang::value<ulong64 const> & ang::value<ulong64 const*>::operator * () { return *reinterpret_cast<value<ulong64 const>*>((void*)get()); }
inline ang::value<ulong64 const> const& ang::value<ulong64 const*>::operator * ()const { return *reinterpret_cast<value<ulong64 const> const*>((void*)get()); }

#endif//__ANG_ULONG_HPP__
