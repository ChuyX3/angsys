#ifndef __ANG_UINT_H__
#error ...
#elif !defined __ANG_UINT_HPP__
#define __ANG_UINT_HPP__

inline ang::value<uint>::operator uint& () { return get(); }
inline ang::value<uint>::operator type ()const { return get(); }
inline ang::value<uint*> ang::value<uint>::operator & () { return &get(); }
inline ang::value<uint const*> ang::value<uint>::operator & ()const { return &get(); }

inline ang::value<uint const>::operator uint()const { return get(); }
inline ang::value<uint const*> ang::value<uint const>::operator & ()const { return &get(); }

inline ang::value<uint&>::operator uint& () { return get(); }
inline ang::value<uint&>::operator uint()const { return get(); }
inline ang::value<uint*> ang::value<uint&>::operator & () { return &get(); }
inline ang::value<uint const*> ang::value<uint&>::operator & ()const { return &get(); }

inline ang::value<uint const&>::operator uint()const { return get(); }
inline ang::value<uint const*> ang::value<uint const&>::operator & ()const { return &get(); }

inline ang::value<uint*>::operator uint*& () { return get(); }
inline ang::value<uint*>::operator uint* ()const { return get(); }
inline ang::value<uint>& ang::value<uint*>::operator * () { return *reinterpret_cast<value<uint>*>(get()); }
inline ang::value<uint> const&  ang::value<uint*>::operator * ()const { return *reinterpret_cast<value<uint> const*>(get()); }

inline ang::value<uint const*>::operator uint const*& () { return get(); }
inline ang::value<uint const*>::operator uint const* ()const { return get(); }
inline ang::value<uint const> & ang::value<uint const*>::operator * () { return *reinterpret_cast<value<uint const>*>((void*)get()); }
inline ang::value<uint const> const& ang::value<uint const*>::operator * ()const { return *reinterpret_cast<value<uint const> const*>((void*)get()); }

#endif//__ANG_UINT_HPP__
