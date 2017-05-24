#ifndef __ANG_SHORT_H__
#error ...
#elif !defined __ANG_SHORT_HPP__
#define __ANG_SHORT_HPP__

inline ang::value<short>::operator short& () { return get(); }
inline ang::value<short>::operator type ()const { return get(); }
inline ang::value<short*> ang::value<short>::operator & () { return &get(); }
inline ang::value<short const*> ang::value<short>::operator & ()const { return &get(); }

inline ang::value<short const>::operator short()const { return get(); }
inline ang::value<short const*> ang::value<short const>::operator & ()const { return &get(); }

inline ang::value<short&>::operator short& () { return get(); }
inline ang::value<short&>::operator short()const { return get(); }
inline ang::value<short*> ang::value<short&>::operator & () { return &get(); }
inline ang::value<short const*> ang::value<short&>::operator & ()const { return &get(); }

inline ang::value<short const&>::operator short()const { return get(); }
inline ang::value<short const*> ang::value<short const&>::operator & ()const { return &get(); }

inline ang::value<short*>::operator short*& () { return get(); }
inline ang::value<short*>::operator short* ()const { return get(); }
inline ang::value<short>& ang::value<short*>::operator * () { return *reinterpret_cast<value<short>*>(get()); }
inline ang::value<short> const&  ang::value<short*>::operator * ()const { return *reinterpret_cast<value<short> const*>(get()); }

inline ang::value<short const*>::operator short const*& () { return get(); }
inline ang::value<short const*>::operator short const* ()const { return get(); }
inline ang::value<short const> & ang::value<short const*>::operator * () { return *reinterpret_cast<value<short const>*>((void*)get()); }
inline ang::value<short const> const& ang::value<short const*>::operator * ()const { return *reinterpret_cast<value<short const> const*>((void*)get()); }

#endif//__ANG_SHORT_HPP__
