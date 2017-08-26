/*********************************************************************************************************************/
/*   File Name: angbase.h                                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#define __ANG_BASE_H__

#include <angtypes.h>
#include <ang/utils.h>
#include <ang/interface.h>
#include <ang/enum.h>

#ifdef LINK
#undef LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

namespace ang
{
	class object;
	class safe_pointer;
	template<typename T> struct value;
	template<typename T> class intf_wrapper;
	template<typename T> class object_wrapper;
	template<typename T> class intf_wrapper_ptr;
	template<typename T> class object_wrapper_ptr;

	typedef object_wrapper<object> object_t;
	typedef object_t objptr;

	class exception_data;
	typedef object_wrapper<exception_data> exception_t;
	typedef exception_t exception;
	//typedef exception

	typedef decltype(nullptr) nullptr_t;
	template<typename T> using initializer_list_t = std::initializer_list<T>;

	windex const invalid_index = -1;
	nullptr_t const null = {};

	typedef value<pointer>			unknown_t;
	typedef value<pointer*>			unknown_ptr_t;

	typedef value<char*>			str_t;
	typedef value<wchar*>			wstr_t;
	typedef value<mchar*>			mstr_t;
	typedef value<char const*>		cstr_t;
	typedef value<wchar const*>		cwstr_t;
	typedef value<mchar const*>		cmstr_t;
	typedef value<char const*>		type_name_t;

	namespace collections
	{
		const wsize ITERATOR_TYPE_DEFAULT = 0;
		const wsize ITERATOR_TYPE_REVERSE = 1;

		template<typename T> class iterator;
		template<typename T> class base_iterator;
		template<typename T> class forward_iterator;
		template<typename T> class backward_iterator;

		template<typename T> using const_iterator = iterator<const T>;
		template<typename T> using const_forward_iterator = forward_iterator<const T>;
		template<typename T> using const_backward_iterator = backward_iterator<const T>;

		template<typename T> struct static_array;
		template<typename T> using static_const_array = struct static_array<const T>;
		template<class T> class array_buffer;
		template<class K, class T>struct pair;
		template<typename T>using array_t = object_wrapper<array_buffer<T>>;
		template<typename T>using array = array_t<T>;
	}

	using collections::array;
	using collections::static_array;
	using collections::static_const_array;


	namespace strings
	{
		class string_buffer;
		class wstring_buffer;
		class mstring_buffer;

		typedef object_wrapper<string_buffer> string, string_t;
		typedef object_wrapper<wstring_buffer> wstring, wstring_t;
		typedef object_wrapper<mstring_buffer> mstring, mstring_t;
	}

	using strings::string;
	using strings::wstring;
	using strings::mstring;

}

#include <ang/value.h>
#include <ang/stringbase.h>
#include <ang/value/mbyte.h>
#include <ang/value/char.h>
#include <ang/value/static_array.h>
#include <ang/runtime.h>
#include <ang/properties.h>

namespace ang
{
	/******************************************************************/
	/* enum ang::comparision_result_t :                                */
	/*  -> represents the result of a comparison operation            */
	/******************************************************************/
	ANG_BEGIN_ENUM(LINK, comparision_result, int)
		minor = -1,
		same = 0,
		mayor = 1,
		diferent = 20
	ANG_END_ENUM(comparision_result);

	/******************************************************************/
	/* interface ang::interface_t :                                   */
	/*  -> implements basic methods for dynamic naming and conversion */
	/******************************************************************/
	typedef struct _interface
	{
		visible virtual type_name_t object_name()const pure;
		visible virtual bool is_kind_of(type_name_t)const pure;
		visible virtual bool query_object(type_name_t, unknown_ptr_t) pure;
	}interface_t, *interface_ptr_t;

	typedef intf_wrapper<interface_t> intfptr;

	/******************************************************************/
	/* interface ang::iobject :                                       */
	/*  -> implements basic methods for smart pointer management      */
	/******************************************************************/
	ANG_BEGIN_INTERFACE(LINK, iobject)
		visible vcall dword add_ref() pure;
		visible vcall dword release() pure;
	ANG_END_INTERFACE();

#if defined _DEBUG || defined _DEVELOPPER
	namespace debug
	{
		/******************************************************************/
		/* enum ang::debug::log_level :                                   */
		/*  -> describes the log level only available for debug config.   */
		/******************************************************************/
		ANG_BEGIN_ENUM(LINK, log_level, int)
			verbose = 0,
			info = 1,
			warning = 2,
			error = 3
		ANG_END_ENUM(log_level);

		/******************************************************************/
		/* interface ang::debug::ilogger :                                */
		/*  -> interface used to print messages to debug output console   */
		/******************************************************************/
		ANG_BEGIN_INTERFACE(LINK, ilogger)
			visible vcall void print(log_level_t, cstr_t, ...)const pure;
		ANG_END_INTERFACE();
		typedef intf_wrapper<ilogger> ilogger_t;

		extern ilogger* get_platform_logger();
	}
#endif


	namespace memory
	{
		/******************************************************************/
		/* interface ang::memory::iallocator :                            */
		/*  -> implements methods for memory allocation and releasing     */
		/******************************************************************/
		ANG_BEGIN_INTERFACE(LINK, iallocator)
			visible vcall bool can_delete_from(iallocator*)const pure;
			visible vcall pointer malloc(wsize)pure;
			visible vcall void free(pointer)pure;
#if defined _DEBUG || defined _DEVELOPPER
#ifdef _DEBUG
			visible vcall pointer malloc(wsize, const char* file, int line)pure;
#endif	
			visible vcall void set_logger(debug::ilogger_t)pure;
#endif
			template<class obj_t> obj_t* alloc_object(wsize count, const char* file = NULL, int line = 0) {
				if (count == 0) return null;
#ifdef _DEBUG
				return static_cast<obj_t*>(malloc(count * sizeof(obj_t), file, line));	
#else
				return static_cast<obj_t*>(malloc(count * sizeof(obj_t)));
#endif
			}

			template<class obj_t, class... args_t> obj_t* construct(obj_t* obj, args_t... args)const {
				return new((pointer)obj)obj_t(args...);
			}
			template<class obj_t> pointer destruct(obj_t* obj)const {
				if (obj) 
					obj->~obj_t();
				return obj;
			}
		ANG_END_INTERFACE();

		/******************************************************************/
		/* interface ang::memory::ialigned_allocator :                    */
		/*  -> implements methods for aligned memory allocation and       */
		/*     releasing                                                  */
		/******************************************************************/
		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ialigned_allocator, public iallocator)
			visible vcall pointer aligned_malloc(wsize, wsize)pure;
#ifdef _DEBUG
		visible vcall pointer aligned_malloc(wsize, wsize, ang_cstr_t file, int line)pure;
#endif			
		template<class obj_t, wsize ALIGNMENT = 4> obj_t* aligned_object_alloc(wsize count) {
			if (count == 0)
				return null;
			return static_cast<obj_t*>(aligned_malloc(count * sizeof(obj_t), ALIGNMENT));
		}
		ANG_END_INTERFACE();


		/******************************************************************/
		/* static class ang::memory::allocator_manager :                  */
		/*  -> manager for obtaining the default allocators               */
		/******************************************************************/
		class LINK allocator_manager
		{
		public:
			enum : index
			{
				default_allocator,
				object_allocator,
				buffer_allocator,
				aligned_allocator,
			};

			static iallocator* get_allocator(index);
			static index regist_allocator(iallocator*);
		};

	}

	namespace collections
	{

		/******************************************************************/
		/* interface ang::collections::ienum :                            */
		/*  -> represents an enumeration of objects or variables which    */
		/*     can be accessed orderly                                    */
		/******************************************************************/
		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE(ienum)
			visible vcall uint counter()const pure;

			visible vcall T& at(base_iterator<T> const&) pure;
			visible vcall bool increase(base_iterator<T>&)const pure;
			visible vcall bool increase(base_iterator<T>&, int offset)const pure;
			visible vcall bool decrease(base_iterator<T>&)const pure;
			visible vcall bool decrease(base_iterator<T>&, int offset)const pure;

			visible vcall forward_iterator<T> begin() pure;
			visible vcall forward_iterator<T> end() pure;
			visible vcall const_forward_iterator<T> begin()const pure;
			visible vcall const_forward_iterator<T> end()const pure;

			visible vcall backward_iterator<T> rbegin() pure;
			visible vcall backward_iterator<T> rend() pure;
			visible vcall const_backward_iterator<T> rbegin()const pure;
			visible vcall const_backward_iterator<T> rend()const pure;
		ANG_END_INTERFACE();

		template<class T> using ienum_t = intf_wrapper<ienum<T>>;

		/******************************************************************/
		/* interface ang::collections::iarray :							  */
		/*  -> represents a collection of objects or variables which      */
		/*     can be accessed unorderly                                  */
		/******************************************************************/
		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(iarray, public ienum<T>)
			using ienum<T>::at;
			visible vcall iterator<T> at(index) pure;
			visible vcall const_iterator<T> at(index)const pure;
			visible vcall void copy(const ienum<T>*) pure;
			visible vcall index find(const T&, bool invert = false)const pure;
			visible vcall index find(const T&, base_iterator<T> next_to, bool invert = false)const pure;
		ANG_END_INTERFACE();

		template<class T> using iarray_t = intf_wrapper<iarray<T>>;

		/******************************************************************/
		/* interface ang::collections::ilist :                            */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements randoml y                    */
		/******************************************************************/
		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(ilist, public iarray<T>)
			visible vcall void extend(const ienum<T>*) pure;
			visible vcall void push(T const&, bool last = true) pure;
			visible vcall bool insert(index idx, T const&) pure;
			visible vcall bool insert(base_iterator<T> it, T const&) pure;
			visible vcall bool pop(bool last = true) pure;
			visible vcall bool pop(T&, bool last = true) pure;
			visible vcall bool pop_at(index idx) pure;
			visible vcall bool pop_at(base_iterator<T> it) pure;
			visible vcall bool pop_at(index idx, T&) pure;
			visible vcall bool pop_at(base_iterator<T> it, T&) pure;
		ANG_END_INTERFACE();

		template<class T> using ilist_t = intf_wrapper<ilist<T>>;

		/******************************************************************/
		/* interface ang::collections::iqueue :                           */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements sequentially                 */
		/******************************************************************/
		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(iseqlist, public ienum<T>)
			visible vcall void extend(const ienum<T>*) pure;
			visible vcall void push(T const&) pure;
			visible vcall bool pop() pure;
			visible vcall bool pop(T&) pure;
		ANG_END_INTERFACE();

		template<class T> using iseqlist_t = intf_wrapper<iseqlist<T>>;

		/******************************************************************/
		/* interface ang::collections::imap :                             */
		/*  -> represents a map of objects or vareiables which can be     */
		/*     accessed by a key. Also the user can insert, find and      */
		/*     remove the elements, the items may or may not be sorted    */
		/******************************************************************/
		template<typename K, typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(imap, public ienum<pair<K, T>>)
			using ienum<T>::at;
			visible vcall iterator<T> at(K const&) pure;
			visible vcall const_iterator<T> at(K const&)const pure;
			visible vcall bool copy(const ienum<pair<K, T>>*) pure;
			visible vcall void extend(const ienum<pair<K, T>>*) pure;
			visible vcall bool insert(K, T) pure;
			visible vcall bool insert(pair<K, T>) pure;
			visible vcall bool update(K, T) pure;
			visible vcall bool update(pair<K, T>) pure;
			visible vcall bool remove(K const&) pure;
			visible vcall bool remove(K const&, T&) pure;
			visible vcall bool remove(base_iterator<pair<K, T>> it) pure;
			visible vcall bool remove(base_iterator<pair<K, T>> it, T&) pure;
			visible vcall bool has_key(const K&)const pure;
			visible vcall iterator<pair<K, T>> find(const K&) pure;
			visible vcall const_iterator<pair<K, T>> find(const K&)const pure;
		ANG_END_INTERFACE();

		template<class K, class T> using imap_t = intf_wrapper<imap<K,T>>;
	}

}

#ifdef LINK
#undef LINK
#endif//LINK

#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
inline pointer operator new(wsize, pointer ptr) throw() { return ptr; }
inline void operator delete(pointer, pointer) throw() { return; }
#endif

#endif//__ANG_BASE_H__
