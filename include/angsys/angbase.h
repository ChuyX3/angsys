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
#include <ang/interface.h>
#include <ang/utils.h>
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

	namespace strings
	{
		struct algorithms;
		class string_buffer;
		class wstring_buffer;
		class mstring_buffer;
	}

	class exception;
	typedef object_wrapper<exception> exception_t;

	namespace collections
	{
		template<typename T>class iterator;
		template<class T> class array_buffer;
		template<class K, class T>class pair;
		template<typename T>using array_t = object_wrapper<array_buffer<T>>;
		template<typename T>using array = array_t<T>;

		template<typename T> struct static_array;
		template<typename T> struct static_const_array;
	}

	using collections::array;
	using collections::static_array;
	using collections::static_const_array;

	namespace strings
	{

		typedef object_wrapper<strings::string_buffer> string_t;
		typedef object_wrapper<strings::wstring_buffer> wstring_t;
		typedef object_wrapper<strings::mstring_buffer> mstring_t;
		typedef string_t string;
		typedef wstring_t wstring;
		typedef mstring_t mstring;
	}

	using strings::string;
	using strings::wstring;
	using strings::mstring;

	using strings::string_t;
	using strings::wstring_t;
	using strings::mstring_t;


	typedef decltype(nullptr) nullptr_t;
	template<typename T> using initializer_list_t = std::initializer_list<T>;

	uint const invalid_index = -1;
	nullptr_t const null = {};

	struct mbyte;
	typedef bool							bool_t;
	typedef value<char>						char_t;
	typedef value<wchar_t>					uchar_t;
	typedef value<unsigned char>			mchar_t;
	typedef value<mbyte>					mbyte_t;
	typedef value<short>					short_t;
	typedef value<unsigned short>			ushort_t;
	typedef value<int>						int_t;
	typedef value<unsigned int>				uint_t;
	typedef value<long>						long_t;
	typedef value<unsigned long>			ulong_t;
	typedef value<long long>				long64_t;
	typedef value<unsigned long long>		ulong64_t;
	typedef value<float>					float_t;
	typedef value<double>					double_t;

#if defined WINDOWS_PLATFORM
#if defined _WIN64
	typedef unsigned __int64	wsize_t;
	typedef unsigned __int64	windex_t;
	typedef unsigned __int64	int_ptr_t;
#elif defined _ARM_
	typedef unsigned int	wsize_t;
	typedef unsigned int	windex_t;
	typedef unsigned int	int_ptr_t;
#elif defined _WIN32 
	typedef __w64 unsigned int	wsize_t;
	typedef __w64 unsigned int	windex_t;
	typedef __w64 unsigned int	int_ptr_t;
#endif
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
	typedef unsigned int		wsize_t;
	typedef unsigned int		windex_t;
	typedef unsigned int*		wint_ptr_t;
#endif

	typedef value<pointer> 					void_ptr_t;
	typedef value<pointer>					handle_t;
	typedef value<pointer>					unknown_t;
	typedef value<pointer*>					unknown_ptr_t;

	typedef value<char*>					str_t;
	typedef value<wchar_t*>					wstr_t;
	typedef value<mchar*>					mstr_t;
	typedef value<char const*>				cstr_t;
	typedef value<wchar const*>				cwstr_t;
	typedef value<mchar const*>				cmstr_t;

	typedef cstr_t							type_name_t;

	typedef value<byte>			byte_t;
	typedef value<word>			word_t;
	typedef value<dword>		dword_t;
	typedef value<qword>		qword_t;

	typedef uint_t							index_t;
	typedef void_ptr_t						void_args_t;

}

#include <ang/value.h>
#include <ang/properties.h>
#include <ang/stringbase.h>

#include <ang/value/char.h>
#include <ang/value/mbyte.h>
#include <ang/value/void_ptr.h>
#include <ang/value/short.h>
#include <ang/value/ushort.h>
#include <ang/value/int.h>
#include <ang/value/uint.h>
#include <ang/value/long.h>
#include <ang/value/ulong.h>
#include <ang/value/float.h>
#include <ang/value/double.h>

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
			none = 0,
			info = 1,
			warning = 2,
			error = 3
		ANG_END_ENUM(log_level);

		/******************************************************************/
		/* interface ang::debug::ilogger :                                */
		/*  -> interface used to print messages to debug output console   */
		/******************************************************************/
		ANG_BEGIN_INTERFACE(LINK, ilogger)
			visible vcall void print(log_level_t ,cstr_t, ...)const pure;
		ANG_END_INTERFACE();
		typedef intf_wrapper<ilogger> ilogger_t;

	}
#endif

	namespace text
	{
		class text_format;
		struct itext_buffer;


		/******************************************************************/
		/* enum ang::text::encoding :                                     */
		/*  -> represents the text encoding                               */
		/******************************************************************/
		ANG_BEGIN_ENUM(LINK, encoding, uint)
			unknown = 0,
			ascii,
			iso_8859_1 = ascii,
			utf_8,
			unicode,
			iso_10646 = unicode,
			utf_16,
		ANG_END_ENUM(encoding);
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
			visible vcall T& at(iterator<T> const&)const pure;
			visible vcall bool next(iterator<T>&)const pure;
			visible vcall bool next(iterator<T>&, int offset)const pure;
			visible vcall bool prev(iterator<T>&)const pure;
			visible vcall bool prev(iterator<T>&, int offset)const pure;
			visible vcall iterator<T> begin()const pure;
			visible vcall iterator<T> end()const pure;
		ANG_END_INTERFACE();

		/******************************************************************/
		/* interface ang::collections::icollection :                      */
		/*  -> represents a collection of objects or variables which      */
		/*     can be accessed unorderly                                  */
		/******************************************************************/
		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(icollection, public ienum<T>)
			visible vcall bool has_items()const pure;
			visible vcall bool copy(const ienum<T>*) pure;
			visible vcall iterator<T> find_index(index idx)const pure;
			visible vcall index index_of(iterator<T> it)const pure;
			visible vcall iterator<T> find(const T&, bool invert = false)const pure;
			visible vcall iterator<T> find(const T&, iterator<T> next_to, bool invert = false)const pure;
		ANG_END_INTERFACE();

		/******************************************************************/
		/* interface ang::collections::ilist :                            */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements                              */
		/******************************************************************/
		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(ilist, public icollection<T>)
			visible vcall void extend(const ienum<T>*) pure;
			visible vcall void append(T const&, bool last = true) pure;
			visible vcall bool insert(index idx, T const&) pure;
			visible vcall bool insert(iterator<T> it, T const&) pure;
			visible vcall bool pop(bool last = true) pure;
			visible vcall bool pop(T&, bool last = true) pure;
			visible vcall bool pop_at(index idx) pure;
			visible vcall bool pop_at(iterator<T> it) pure;
			visible vcall bool pop_at(index idx, T&) pure;
			visible vcall bool pop_at(iterator<T> it, T&) pure;
		ANG_END_INTERFACE();

		/******************************************************************/
		/* interface ang::collections::imap :                             */
		/*  -> represents a map of objects or vareiables which can be     */
		/*     accessed by a key. Also the user can insert, find and      */
		/*     remove the elements                                        */
		/******************************************************************/
		template<typename K, typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(imap, public icollection<pair<K, T>>)
			visible vcall void extend(const ienum<pair<K,T>>*) pure;
			visible vcall bool insert(K, T) pure;
			visible vcall bool insert(pair<K, T>) pure;
			visible vcall bool update(K, T) pure;
			visible vcall bool update(pair<K, T>) pure;
			visible vcall bool remove(K const&) pure;
			visible vcall bool remove(K const&, T&) pure;
			visible vcall bool remove(iterator<pair<K, T>> it) pure;
			visible vcall bool remove(iterator<pair<K, T>> it, T&) pure;
			visible vcall bool has_key(const K&)const pure;
			visible vcall iterator<pair<K, T>> find(const K&)const pure;
			inherit vcall iterator<pair<K, T>> find_index(index idx)const override pure;
			inherit vcall index index_of(iterator<pair<K, T>> it)const override pure;
			inherit vcall iterator<pair<K, T>> find(const pair<K, T>&, bool)const override pure;
			inherit vcall iterator<pair<K, T>> find(const pair<K, T>&, iterator<pair<K, T>>, bool)const override pure;
		ANG_END_INTERFACE();

		template<typename T>
		struct imap<string, T>: public icollection<pair<string, T>>
		{
			inline ANG_DECLARE_CLASSNAME();
			inline ANG_DECLARE_ISCHILDOF();
			inline ANG_DECLARE_OBJECTNAME();
			inline ANG_DECLARE_ISKINDOF();
			inline ANG_DECLARE_QUERYOBJECT();
			visible vcall void extend(const ienum<pair<string,T>>*) pure;
			visible vcall bool insert(cstr_t, T) pure;
			visible vcall bool insert(cwstr_t, T) pure;
			visible vcall bool insert(pair<string, T>) pure;
			visible vcall bool update(cstr_t, T) pure;
			visible vcall bool update(cwstr_t, T) pure;
			visible vcall bool update(pair<string, T>) pure;
			visible vcall bool remove(cstr_t) pure;
			visible vcall bool remove(cwstr_t) pure;
			visible vcall bool remove(cstr_t, T&) pure;
			visible vcall bool remove(cwstr_t, T&) pure;
			visible vcall bool remove(iterator<pair<string, T>> it) pure;
			visible vcall bool remove(iterator<pair<string, T>> it, T&) pure;
			visible vcall bool has_key(cstr_t)const pure;
			visible vcall bool has_key(cwstr_t)const pure;
			visible vcall iterator<pair<string, T>> find(cstr_t)const pure;
			visible vcall iterator<pair<string, T>> find(cwstr_t)const pure;
			inherit vcall iterator<pair<string, T>> find_index(index idx)const override pure;
			inherit vcall index index_of(iterator<pair<string, T>> it)const override pure;
			inherit vcall iterator<pair<string, T>> find(const pair<string, T>&, bool)const override pure;
			inherit vcall iterator<pair<string, T>> find(const pair<string, T>&, iterator<pair<string, T>>, bool)const override pure;
		};

		template<typename T>
		struct imap<wstring, T> : public icollection<pair<wstring, T>>
		{
			inline ANG_DECLARE_CLASSNAME();
			inline ANG_DECLARE_ISCHILDOF();
			inline ANG_DECLARE_OBJECTNAME();
			inline ANG_DECLARE_ISKINDOF();
			inline ANG_DECLARE_QUERYOBJECT();
			visible vcall void extend(const ienum<pair<wstring, T>>*) pure;
			visible vcall bool insert(cstr_t, T) pure;
			visible vcall bool insert(cwstr_t, T) pure;
			visible vcall bool insert(pair<wstring, T>) pure;
			visible vcall bool update(cstr_t, T) pure;
			visible vcall bool update(cwstr_t, T) pure;
			visible vcall bool update(pair<wstring, T>) pure;
			visible vcall bool remove(cstr_t) pure;
			visible vcall bool remove(cwstr_t) pure;
			visible vcall bool remove(cstr_t, T&) pure;
			visible vcall bool remove(cwstr_t, T&) pure;
			visible vcall bool remove(iterator<pair<wstring, T>> it) pure;
			visible vcall bool remove(iterator<pair<wstring, T>> it, T&) pure;
			visible vcall bool has_key(cstr_t)const pure;
			visible vcall bool has_key(cwstr_t)const pure;
			visible vcall iterator<pair<wstring, T>> find(cstr_t)const pure;
			visible vcall iterator<pair<wstring, T>> find(cwstr_t)const pure;
			inherit vcall iterator<pair<wstring, T>> find_index(index idx)const override pure;
			inherit vcall index index_of(iterator<pair<wstring, T>> it)const override pure;
			inherit vcall iterator<pair<wstring, T>> find(const pair<wstring, T>&, bool)const override pure;
			inherit vcall iterator<pair<wstring, T>> find(const pair<wstring, T>&, iterator<pair<wstring, T>>, bool)const override pure;
		};



		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(iset, public icollection<T>)
			visible vcall void extend(const ienum<T>*) pure;
			visible vcall bool insert(T) pure;
			visible vcall bool remove(const T&) pure;
			visible vcall bool remove(iterator<T> it) pure;
			visible vcall bool has_value(const T&)const pure;
			visible vcall iterator<T> find(const T&)const pure;
			inherit vcall iterator<T> find_index(index idx)const override pure;
			inherit vcall index index_of(iterator<T> it)const override pure;
			inherit vcall iterator<T> find(const T&, bool)const override pure;
			inherit vcall iterator<T> find(const T&, iterator<T>, bool)const override pure;
		ANG_END_INTERFACE();

		template<>
		struct iset<wstring> : public icollection<wstring>
		{
			inline ANG_DECLARE_CLASSNAME();
			inline ANG_DECLARE_ISCHILDOF();
			inline ANG_DECLARE_OBJECTNAME();
			inline ANG_DECLARE_ISKINDOF();
			inline ANG_DECLARE_QUERYOBJECT();
			visible vcall void extend(const ienum<wstring>*) pure;
			visible vcall bool insert(cstr_t) pure;
			visible vcall bool insert(cwstr_t) pure;
			visible vcall bool remove(cstr_t) pure;
			visible vcall bool remove(cwstr_t) pure;
			visible vcall bool remove(iterator<wstring> it) pure;
			visible vcall bool has_value(cstr_t)const pure;
			visible vcall bool has_value(cwstr_t)const pure;
			visible vcall iterator<wstring> find(cstr_t)const pure;
			visible vcall iterator<wstring> find(cwstr_t)const pure;
			inherit vcall iterator<wstring> find_index(index idx)const override pure;
			inherit vcall index index_of(iterator<wstring> it)const override pure;
			inherit vcall iterator<wstring> find(const wstring&, bool)const override pure;
			inherit vcall iterator<wstring> find(const wstring&, iterator<wstring>, bool)const override pure;
		};


		template<>
		struct iset<string> : public icollection<string>
		{
			inline ANG_DECLARE_CLASSNAME();
			inline ANG_DECLARE_ISCHILDOF();
			inline ANG_DECLARE_OBJECTNAME();
			inline ANG_DECLARE_ISKINDOF();
			inline ANG_DECLARE_QUERYOBJECT();
			visible vcall void extend(const ienum<string>*) pure;
			visible vcall bool insert(cstr_t) pure;
			visible vcall bool insert(cwstr_t) pure;
			visible vcall bool remove(cstr_t) pure;
			visible vcall bool remove(cwstr_t) pure;
			visible vcall bool remove(iterator<string> it) pure;
			visible vcall bool has_value(cstr_t)const pure;
			visible vcall bool has_value(cwstr_t)const pure;
			visible vcall iterator<string> find(cstr_t)const pure;
			visible vcall iterator<string> find(cwstr_t)const pure;
			inherit vcall iterator<string> find_index(index idx)const override pure;
			inherit vcall index index_of(iterator<string> it)const override pure;
			inherit vcall iterator<string> find(const string&, bool)const override pure;
			inherit vcall iterator<string> find(const string&, iterator<string>, bool)const override pure;
		};
	}

	namespace memory
	{
		/******************************************************************/
		/* interface ang::memory::iallocator :                            */
		/*  -> implements methods for memory allocation and releasing     */
		/******************************************************************/
		ANG_BEGIN_INTERFACE(LINK, iallocator)
			visible vcall void auto_release()pure;
			visible vcall bool can_delete_from(iallocator*)const pure;
			visible vcall pointer memory_alloc(wsize)pure;
			visible vcall void memory_release(pointer)pure;
#ifdef _DEBUG
			visible vcall pointer memory_alloc(wsize, ang_cstr_t file, int line)pure;
#endif	

#if defined _DEBUG || defined _DEVELOPPER
			visible vcall void set_logger(debug::ilogger_t)pure;
#endif

			template<class obj_t> obj_t* object_alloc(wsize count) {
				if (count == 0)
					return null;
				return static_cast<obj_t*>(memory_alloc(count * sizeof(obj_t)));
			}
			template<class obj_t, class... args_t> obj_t* construct(obj_t* obj, args_t... args)const {
				return new((pointer)obj)obj_t(args...);
			}
			template<class obj_t> pointer destruct(obj_t* obj)const {
				if (obj) {
					obj->~obj_t();
				}
				return obj;
			}
		ANG_END_INTERFACE();

		/******************************************************************/
		/* interface ang::memory::ialigned_allocator :                    */
		/*  -> implements methods for aligned memory allocation and       */
		/*     releasing                                                  */
		/******************************************************************/
		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ialigned_allocator, public iallocator)
			visible vcall pointer aligned_memory_alloc(wsize, wsize)pure;
#ifdef _DEBUG
			visible vcall pointer aligned_memory_alloc(wsize, wsize, ang_cstr_t file, int line)pure;
#endif			
			template<class obj_t, wsize ALIGNMENT = 4> obj_t* aligned_object_alloc(wsize count) {
				if (count == 0)
					return null;
				return static_cast<obj_t*>(aligned_memory_alloc(count * sizeof(obj_t), ALIGNMENT));
			}
		ANG_END_INTERFACE();

		/******************************************************************/
		/* interface ang::memory::iallocator_client :                     */
		/*  -> reprecents a client of memory allocator                    */
		/******************************************************************/
		ANG_BEGIN_INTERFACE(LINK, iallocator_client)
			visible vcall void set_allocator(iallocator*)pure;
			visible vcall iallocator* get_allocator()const pure;
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
}

#include <ang/value/static_array.h>

#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
inline pointer operator new(wsize, pointer ptr) throw()
{
	return ptr;
}

inline void operator delete(pointer, pointer) throw()
{
	return;
}
#endif


#if defined _DEBUG
#define ANG_NEW_ARGS(...) new(__VA_ARGS__)
#define ANG_DEBUG_NEW() new(__FILE__, __LINE__)
#define ANG_IMPLEMENT_MEMORYMANAGER()	\
pointer operator new (wsize size){\
	return ang::memory::allocator_manager::get_allocator(0)->memory_alloc(size);\
}\
void operator delete (pointer buffer){\
	ang::memory::allocator_manager::get_allocator(0)->memory_release(buffer);\
}\
pointer operator new[](wsize size){\
	return ang::memory::allocator_manager::get_allocator(0)->memory_alloc(size);\
}\
void operator delete[](pointer buffer){\
	ang::memory::allocator_manager::get_allocator(0)->memory_release(buffer);\
}\
pointer operator new (wsize size, ang_cstr_t file, int line){ \
return ang::memory::allocator_manager::get_allocator(0)->memory_alloc(size, file, line); \
}\
pointer operator new[] (wsize size, ang_cstr_t file, int line){ \
return ang::memory::allocator_manager::get_allocator(0)->memory_alloc(size, file, line); \
} \
void operator delete(pointer buffer, ang_cstr_t, int){\
	ang::memory::allocator_manager::get_allocator(0)->memory_release(buffer);\
} \
void operator delete[](pointer buffer, ang_cstr_t, int){\
	ang::memory::allocator_manager::get_allocator(0)->memory_release(buffer);\
}

#define ANG_DECLARE_MEMORYMANAGER()\
extern pointer operator new (wsize size);\
extern void operator delete (pointer buffer);\
extern pointer operator new[](wsize size);\
extern void operator delete[](pointer buffer);\
extern pointer operator new (wsize size, ang_cstr_t file, int line);\
extern pointer operator new[] (wsize size, ang_cstr_t file, int line);\
extern void operator delete (pointer buffer, ang_cstr_t file, int line);\
extern void operator delete[] (pointer buffer, ang_cstr_t file, int line);

#else
#define ANG_IMPLEMENT_MEMORYMANAGER()	\
pointer operator new (wsize size){\
	return ang::memory::allocator_manager::get_allocator(0)->memory_alloc(size);\
}\
void operator delete (pointer buffer){\
	ang::memory::allocator_manager::get_allocator(0)->memory_release(buffer);\
}\
pointer operator new[](wsize size){\
	return ang::memory::allocator_manager::get_allocator(0)->memory_alloc(size);\
}\
void operator delete[](pointer buffer){\
	ang::memory::allocator_manager::get_allocator(0)->memory_release(buffer);\
}

#define ANG_DECLARE_MEMORYMANAGER()\
extern pointer operator new (wsize size);\
extern void operator delete (pointer buffer);\
extern pointer operator new[](wsize size);\
extern void operator delete[](pointer buffer);

#endif


#ifdef LINK
#undef LINK
#endif//LINK

#endif//__ANG_BASE_H__