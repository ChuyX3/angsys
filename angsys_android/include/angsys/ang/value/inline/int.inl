#ifndef __ANG_INT_H__
#error ...
#elif !defined __ANG_INT_HPP__
#define __ANG_INT_HPP__

inline ang::value<int>::operator int& () { return get(); }
inline ang::value<int>::operator int()const { return get(); }
inline ang::value<int*> ang::value<int>::operator & () { return &get(); }
inline ang::value<int const*> ang::value<int>::operator & ()const { return &get(); }
	
inline ang::value<int const>::operator int()const { return get(); }
inline ang::value<int const*> ang::value<int const>::operator & ()const { return &get(); }

inline ang::value<int&>::operator int& () { return get(); }
inline ang::value<int&>::operator int()const { return get(); }
inline ang::value<int*> ang::value<int&>::operator & () { return &get(); }
inline ang::value<int const*> ang::value<int&>::operator & ()const { return &get(); }
	
inline ang::value<int const&>::operator int()const { return get(); }
inline ang::value<int const*> ang::value<int const&>::operator & ()const { return &get(); }
	
inline ang::value<int*>::operator int*& () { return get(); }
inline ang::value<int*>::operator int* ()const { return get(); }
inline ang::value<int>& ang::value<int*>::operator * () { return *reinterpret_cast<value<int>*>(get()); }
inline ang::value<int> const&  ang::value<int*>::operator * ()const { return *reinterpret_cast<value<int> const*>(get()); }

inline ang::value<int const*>::operator int const*& () { return get(); }
inline ang::value<int const*>::operator int const* ()const { return get(); }
inline ang::value<int const> & ang::value<int const*>::operator * () { return *reinterpret_cast<value<int const>*>((void*)get()); }
inline ang::value<int const> const& ang::value<int const*>::operator * ()const { return *reinterpret_cast<value<int const> const*>((void*)get()); }


#endif//__ANG_INT_HPP__
