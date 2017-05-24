#ifndef __ANG_VOID_PTR_H__
#error ...
#elif !defined __ANG_VOID_PTR_HPP__
#define __ANG_VOID_PTR_HPP__

inline ang::value<pointer>::operator void*& () { return get(); }
inline ang::value<pointer>::operator type ()const { return get(); }
inline ang::value<pointer*> ang::value<pointer>::operator & () { return &get(); }
inline ang::value<pointer const*> ang::value<pointer>::operator & ()const { return &get(); }

inline ang::value<pointer const>::operator void*()const { return get(); }
inline ang::value<pointer const*> ang::value<pointer const>::operator & ()const { return &get(); }

inline ang::value<pointer&>::operator void*& () { return get(); }
inline ang::value<pointer&>::operator void*()const { return get(); }
inline ang::value<pointer*> ang::value<pointer&>::operator & () { return &get(); }
inline ang::value<pointer const*> ang::value<pointer&>::operator & ()const { return &get(); }

inline ang::value<pointer const&>::operator void*()const { return get(); }
inline ang::value<pointer const*> ang::value<pointer const&>::operator & ()const { return &get(); }

inline ang::value<pointer*>::operator void**& () { return get(); }
inline ang::value<pointer*>::operator void** ()const { return get(); }
inline pointer& ang::value<pointer*>::operator * () { return *get(); }
inline pointer ang::value<pointer*>::operator * ()const { return *get(); }

inline ang::value<pointer const*>::operator void* const*& () { return get(); }
inline ang::value<pointer const*>::operator void* const* ()const { return get(); }
inline pointer ang::value<pointer const*>::operator * ()const { return *get(); }

#endif//__ANG_VOID_PTR_HPP__
