#ifndef __ANG_USHORT_H__
#error ...
#elif !defined __ANG_USHORT_HPP__
#define __ANG_USHORT_HPP__

inline ang::value<ushort>::operator ushort& () { return get(); }
inline ang::value<ushort>::operator type ()const { return get(); }
inline ang::value<ushort*> ang::value<ushort>::operator & () { return &get(); }
inline ang::value<ushort const*> ang::value<ushort>::operator & ()const { return &get(); }

inline ang::value<ushort const>::operator ushort()const { return get(); }
inline ang::value<ushort const*> ang::value<ushort const>::operator & ()const { return &get(); }

inline ang::value<ushort&>::operator ushort& () { return get(); }
inline ang::value<ushort&>::operator ushort()const { return get(); }
inline ang::value<ushort*> ang::value<ushort&>::operator & () { return &get(); }
inline ang::value<ushort const*> ang::value<ushort&>::operator & ()const { return &get(); }

inline ang::value<ushort const&>::operator ushort()const { return get(); }
inline ang::value<ushort const*> ang::value<ushort const&>::operator & ()const { return &get(); }

inline ang::value<ushort*>::operator ushort*& () { return get(); }
inline ang::value<ushort*>::operator ushort* ()const { return get(); }
inline ang::value<ushort>& ang::value<ushort*>::operator * () { return *reinterpret_cast<value<ushort>*>(get()); }
inline ang::value<ushort> const&  ang::value<ushort*>::operator * ()const { return *reinterpret_cast<value<ushort> const*>(get()); }

inline ang::value<ushort const*>::operator ushort const*& () { return get(); }
inline ang::value<ushort const*>::operator ushort const* ()const { return get(); }
inline ang::value<ushort const> & ang::value<ushort const*>::operator * () { return *reinterpret_cast<value<ushort const>*>((void*)get()); }
inline ang::value<ushort const> const& ang::value<ushort const*>::operator * ()const { return *reinterpret_cast<value<ushort const> const*>((void*)get()); }

#endif//__ANG_USHORT_HPP__
