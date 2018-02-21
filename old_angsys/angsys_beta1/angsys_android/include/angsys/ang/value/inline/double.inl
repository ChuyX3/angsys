#ifndef __ANG_DOUBLE_H__
#error ...
#elif !defined __ANG_DOUBLE_HPP__
#define __ANG_DOUBLE_HPP__

inline ang::value<double>::operator double& () { return get(); }
inline ang::value<double>::operator type ()const { return get(); }
inline ang::value<double*> ang::value<double>::operator & () { return &get(); }
inline ang::value<double const*> ang::value<double>::operator & ()const { return &get(); }
	
inline ang::value<double const>::operator double()const { return get(); }
inline ang::value<double const*> ang::value<double const>::operator & ()const { return &get(); }

inline ang::value<double&>::operator double& () { return get(); }
inline ang::value<double&>::operator double()const { return get(); }
inline ang::value<double*> ang::value<double&>::operator & () { return &get(); }
inline ang::value<double const*> ang::value<double&>::operator & ()const { return &get(); }
	
inline ang::value<double const&>::operator double()const { return get(); }
inline ang::value<double const*> ang::value<double const&>::operator & ()const { return &get(); }
	
inline ang::value<double*>::operator double*& () { return get(); }
inline ang::value<double*>::operator double* ()const { return get(); }
inline ang::value<double>& ang::value<double*>::operator * () { return *reinterpret_cast<value<double>*>(get()); }
inline ang::value<double> const&  ang::value<double*>::operator * ()const { return *reinterpret_cast<value<double> const*>(get()); }

inline ang::value<double const*>::operator double const*& () { return get(); }
inline ang::value<double const*>::operator double const* ()const { return get(); }
inline ang::value<double const> & ang::value<double const*>::operator * () { return *reinterpret_cast<value<double const>*>((void*)get()); }
inline ang::value<double const> const& ang::value<double const*>::operator * ()const { return *reinterpret_cast<value<double const> const*>((void*)get()); }

#endif//__ANG_DOUBLE_HPP__
