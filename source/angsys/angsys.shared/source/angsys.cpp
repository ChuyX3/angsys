/*********************************************************************************************************************/
/*   File Name: angsys.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.hpp"
#include "angc/core_hash_table.h"

#include <ang/collections/vector.hpp>
#include "thread.hpp"
#include "file_system.h"

#include <ang/xml.hpp>

//#include <ang/streams.hpp>

using namespace ang;


template<class K>
inline uint _create_hash_index(K const& k, ang_uint64_t const TS)
{
	union {
		K k;
		ulong64 val;
	}key;
	key.val = 0;
	key.k = k;
	return (uint)((2475825 + key.val + 1) % TS);
}

ANG_EXTERN ang_uint64_t ang_create_hash_index_int32(ang_int32_t key, ang_uint64_t TS) {
	return _create_hash_index<ang_int32_t>(key, TS);
}
ANG_EXTERN ang_uint64_t ang_create_hash_index_uint32(ang_uint32_t key, ang_uint64_t TS) {
	return _create_hash_index<ang_uint32_t>(key, TS);
}
ANG_EXTERN ang_uint64_t ang_create_hash_index_int64(ang_int64_t key, ang_uint64_t TS) {
	return _create_hash_index<ang_int64_t>(key, TS);
}
ANG_EXTERN ang_uint64_t ang_create_hash_index_uint64(ang_uint64_t key, ang_uint64_t TS) {
	return _create_hash_index<ang_uint64_t>(key, TS);
}
ANG_EXTERN ang_uint64_t ang_create_hash_index_voidptr(ang_void_ptr_t key, ang_uint64_t TS) {
	return _create_hash_index<ang_void_ptr_t>(key, TS);
}
ANG_EXTERN ang_uint64_t ang_create_hash_index_cstr(ang_cstr_t key, ang_uint64_t TS) {
	ulong64 h = 75025;
	for (int i = 0; key[i] != 0; i++)
		h = (h << 5) + h + key[i] + 1;
	return ang_uint64_t(h % TS);
}
ANG_EXTERN ang_uint64_t ang_create_hash_index_cwstr(ang_cwstr_t key, ang_uint64_t TS) {
	ulong64 h = 75025;
	for (int i = 0; key[i] != 0; i++)
		h = (h << 5) + h + key[i] + 1;
	return ang_uint64_t(h % TS);
}

static ang_uint32_t list[] =
{
	7u,
	13u,
	31u,
	61u,
	127u,
	251u,
	509u,
	1021u,
	2039u,
	4093u,
	8191u,
	16381u,
};

ANG_EXTERN wsize ang_hash_table_get_next_size(wsize size)
{
	wsize out = 0;
	if (size >= 16381)
	{
		out = 16381;
		while (size >= out)
			out *= 2;
		return out;
	}
	else
	{
		for (auto i = 0U; i < array_size(list); ++i)
			if (size < list[i])
				return list[i];
		return -1;
	}
}

namespace ang
{
	namespace collections
	{
		ulong64 hash_table_get_next_size(wsize size) { return ang_hash_table_get_next_size(size); }
		ulong64 create_hash_index(int value, ulong64 TS) { return _create_hash_index(value, TS); }
		ulong64 create_hash_index(uint value, ulong64 TS) { return _create_hash_index(value, TS); }
		ulong64 create_hash_index(long value, ulong64 TS) { return _create_hash_index(value, TS); }
		ulong64 create_hash_index(ulong value, ulong64 TS) { return _create_hash_index(value, TS); }
		ulong64 create_hash_index(long64 value, ulong64 TS) { return _create_hash_index(value, TS); }
		ulong64 create_hash_index(ulong64 value, ulong64 TS) { return _create_hash_index(value, TS); }
		ulong64 create_hash_index(float value, ulong64 TS) { return _create_hash_index(value, TS); }
		ulong64 create_hash_index(double value, ulong64 TS) { return _create_hash_index(value, TS); }
		ulong64 create_hash_index(pointer value, ulong64 TS) { return _create_hash_index(value, TS); }
		//uint create_hash_index(cstr_t value, wsize TS) { return ang_create_hash_index(value, TS); }
		//uint create_hash_index(cwstr_t value, wsize TS) { return ang_create_hash_index(value, TS); }
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////

namespace ang
{
	struct runtime_data_base_imp : public runtime_data_base
	{
		typedef struct node_t
		{
			string type_name;
			node_t* _next;
		}*node_ptr_t;

		node_ptr_t _head;
		node_ptr_t _tail;

		runtime_data_base_imp()
		{
			_head = null;
			_tail = null;
		}
		~runtime_data_base_imp()
		{
			node_ptr_t temp, node = _head;
			while (node)
			{
				node->type_name.clean();
				temp = node;
				node = node->_next;
				memory::unmanaged_allocator<node_t>::destruct_and_free(temp);
			}
			_head = null;
			_tail = null;
		}

		type_name_t insert(string name) {
			_tail = insert(_tail, ang::move(name));
			if (_head == null) _head = _tail;
			return _tail->type_name;
		}

		node_ptr_t insert(node_ptr_t node, string name) {
			if (node == null) {
				node = memory::unmanaged_allocator<node_t>::construct(memory::unmanaged_allocator<node_t>::alloc(1));
				node->type_name = ang::move(name);
				node->_next = null;
				return node;
			}
			return insert(node->_next, ang::move(name));
		}
	};
}

ang::type_name_t ang::runtime::runtime_data_base::regist_typename(ang::string name)
{
	static runtime_data_base_imp _runtime_data_base;
	return _runtime_data_base.insert(ang::move(name));
}

class ang_initializer
{

public:
	ang_initializer() {
		thread_manager = memory::object_allocator<core::async::core_thread_manager>::alloc(1);
		thread_manager = memory::object_allocator<core::async::core_thread_manager>::construct(thread_manager);
		ang::runtime::runtime_data_base::regist_typename(type_of<object>());
		core::async::core_thread_manager::instance();
		core::files::file_system::instance();
	}
	~ang_initializer()
	{
		core::files::file_system::release_instance();
		memory::object_allocator<core::async::core_thread_manager>::destruct_and_free(thread_manager);
		thread_manager = null;
		
	}


	core::async::core_thread_manager* thread_manager;
}initializer;


//template<>
//core::async::core_thread_manager* singleton<core::async::core_thread_manager>::instance_manager(core::async::core_thread_manager* ptr, bool write, bool del)
//{
//	return initializer.thread_manager;
//}

//////////////////////////////////////////////////////////////////////////////////////////////////

ANG_EXTERN void ang_throw_exception(dword code, const char* message)
{
#ifdef _DEBUG
#ifdef WINDOWS_PLATFORM
	OutputDebugStringA(message); OutputDebugStringA("\n");
	__debugbreak();
#endif // WINDOWS_PLATFORM

#endif // _DEBUG

}







extern "C" void ang_debug_output_verbose(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	size_t size = _vscprintf(format, argList);

	char* buffer = (char*)malloc(size + 1);
	buffer[size] = 0;

	vsprintf_s(buffer, size + 1, format, argList);
	va_end(argList);

#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_VERBOSE, "angsys", "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
#if defined _DEBUG || defined _DEVELOPPER
	printf("%s %s\n", "angsys", buffer);
	OutputDebugStringA("angsys");
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
#endif
#endif
	free(buffer);
}

extern "C" void ang_debug_output_info(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	size_t size = _vscprintf(format, argList);

	char* buffer = (char*)malloc(size + 1);
	buffer[size] = 0;

	vsprintf_s(buffer, size + 1, format, argList);
	va_end(argList);

#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_INFO, "angsys", "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
#if defined _DEBUG || defined _DEVELOPPER
	printf("%s %s\n", "angsys", buffer);
	OutputDebugStringA("angsys");
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
#endif
#endif
	free(buffer);
}

extern "C" void ang_debug_output_warning(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	size_t size = _vscprintf(format, argList);

	char* buffer = (char*)malloc(size + 1);
	buffer[size] = 0;


	vsprintf_s(buffer, size + 1, format, argList);
	va_end(argList);

#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_WARN, "angsys", "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
#if defined _DEBUG || defined _DEVELOPPER
	printf("%s %s\n", "angsys", buffer);
	//OutputDebugStringA("angsys");
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
#endif
#endif
	free(buffer);
}

extern "C" void ang_debug_output_error(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	size_t size = _vscprintf(format, argList);

	char* buffer = (char*)malloc(size + 1);
	buffer[size] = 0;


	vsprintf_s(buffer, size + 1, format, argList);
	va_end(argList);


#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_ERROR, "angsys", "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
#if defined _DEBUG || defined _DEVELOPPER
	printf("%s %s\n", "angsys", buffer);
	OutputDebugStringA("angsys");
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
#endif
#endif
	free(buffer);
}

