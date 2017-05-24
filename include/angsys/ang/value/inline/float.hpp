#ifndef __ANG_FLOAT_H__
#error ...
#elif !defined __ANG_FLOAT_HPP__
#define __ANG_FLOAT_HPP__

inline ang::value<float>::operator float& () { return get(); }
inline ang::value<float>::operator type ()const { return get(); }
inline ang::value<float*> ang::value<float>::operator & () { return &get(); }
inline ang::value<float const*> ang::value<float>::operator & ()const { return &get(); }
	
inline ang::value<float const>::operator float()const { return get(); }
inline ang::value<float const*> ang::value<float const>::operator & ()const { return &get(); }

inline ang::value<float&>::operator float& () { return get(); }
inline ang::value<float&>::operator float()const { return get(); }
inline ang::value<float*> ang::value<float&>::operator & () { return &get(); }
inline ang::value<float const*> ang::value<float&>::operator & ()const { return &get(); }
	
inline ang::value<float const&>::operator float()const { return get(); }
inline ang::value<float const*> ang::value<float const&>::operator & ()const { return &get(); }
	
inline ang::value<float*>::operator float*& () { return get(); }
inline ang::value<float*>::operator float* ()const { return get(); }
inline ang::value<float>& ang::value<float*>::operator * () { return *reinterpret_cast<value<float>*>(get()); }
inline ang::value<float> const&  ang::value<float*>::operator * ()const { return *reinterpret_cast<value<float> const*>(get()); }

inline ang::value<float const*>::operator float const*& () { return get(); }
inline ang::value<float const*>::operator float const* ()const { return get(); }
inline ang::value<float const> & ang::value<float const*>::operator * () { return *reinterpret_cast<value<float const>*>((void*)get()); }
inline ang::value<float const> const& ang::value<float const*>::operator * ()const { return *reinterpret_cast<value<float const> const*>((void*)get()); }

#endif//__ANG_FLOAT_HPP__
