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
#include "angsys.h"
#include "memory.h"
#include "debug.h"
#include "exception_code_map.h"
#include "thread_manager.h"
#include "file_system.h"

#include "ang/collections/map.h"

using namespace ang;

#ifdef ANGSYS_DYNAMIC_LIBRARY
ANG_IMPLEMENT_MEMORYMANAGER();
#endif

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif


template<class K>
inline uint _create_hash_index(K const& k, uint const TS)
{
	union {
		K k;
		ulong64 val;
	}key;
	key.val = 0;
	key.k = k;
	return (uint)((2475825 + key.val + 1) % TS);
}

ang_uint32_t ang_create_hash_index(ang_int32_t key, ang_uint32_t TS) {
	return _create_hash_index<ang_int32_t>(key, TS);
}
ang_uint32_t ang_create_hash_index(ang_uint32_t key, ang_uint32_t TS) {
	return _create_hash_index<ang_uint32_t>(key, TS);
}
ang_uint32_t ang_create_hash_index(ang_int64_t key, ang_uint32_t TS) {
	return _create_hash_index<ang_int64_t>(key, TS);
}
ang_uint32_t ang_create_hash_index(ang_uint64_t key, ang_uint32_t TS) {
	return _create_hash_index<ang_uint64_t>(key, TS);
}
ang_uint32_t ang_create_hash_index(ang_void_ptr_t key, ang_uint32_t TS) {
	return _create_hash_index<ang_void_ptr_t>(key, TS);
}
ang_uint32_t ang_create_hash_index(ang_cstr_t key, ang_uint32_t TS) {
	ulong_t h = 75025;
	for (int i = 0; key[i] != 0; i++)
		h = (h << 5) + h + key[i] + 1;
	return uint(h % TS);
}
ang_uint32_t ang_create_hash_index(ang_cwstr_t key, ang_uint32_t TS) {
	ulong_t h = 75025;
	for (int i = 0; key[i] != 0; i++)
		h = (h << 5) + h + key[i] + 1;
	return uint(h % TS);
}

namespace ang
{
	namespace collections
	{
		uint map_get_next_size(uint size) { return ang_hash_table_get_next_size(size); }
		uint create_hash_index(int value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(uint value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(long value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(ulong value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(long64 value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(ulong64 value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(float value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(double value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(pointer value, uint TS) { return _create_hash_index(value, TS); }
		uint create_hash_index(cstr_t value, uint TS) { return ang_create_hash_index(value, TS); }
		uint create_hash_index(cwstr_t value, uint TS) { return ang_create_hash_index(value, TS); }
	}
}

ang_uint32_t list[] =
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

ang_uint32_t ang_hash_table_get_next_size(ang_uint32_t size)
{
	ang_uint32_t out = 0;
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
	class ang_library_initializer : public singleton<ang_library_initializer>
	{
		static ang_library_initializer s_main_object_initializer;
		friend singleton<ang_library_initializer>;
	public:
		ang_library_initializer()
			: m_default_allocator(memory::allocator_manager::default_allocator)
			, m_object_allocator(memory::allocator_manager::object_allocator)
			, m_buffer_allocator(memory::allocator_manager::buffer_allocator)
			, m_aligned_allocator()
#if defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
			, logger("ang_debugger"_s)
#endif
		{
		//	logger.print(debug::log_level::info, "ang console: \n");
			core::async::thread_manager::instance();
			core::files::file_system::instance();
		}

		~ang_library_initializer()
		{
			core::files::file_system::release_instance();
			core::async::thread_manager::release_instance();		
		}
		debug::console_logger logger;
		memory::default_allocator m_default_allocator;
		memory::default_allocator m_object_allocator;
		memory::default_allocator m_buffer_allocator;
		memory::aligned_allocator m_aligned_allocator;
#if defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
	
#endif
	};
}

ang::ang_library_initializer ang::ang_library_initializer::s_main_object_initializer;

template<> ang_library_initializer* ang::singleton<ang_library_initializer>::instance() { 
	return &ang_library_initializer::s_main_object_initializer;
}
template<> bool ang::singleton<ang_library_initializer>::release_instance() { 
	return false; 
}

template<> core::async::thread_manager* ang::singleton<core::async::thread_manager_t>::instance() {
	if (_instance == null)
		new core::async::thread_manager();
	return static_cast<core::async::thread_manager*>(_instance);
}
template<> bool ang::singleton<core::async::thread_manager_t>::release_instance() {
	if (_instance)_instance->release();
	_instance = null;
	return true;
}
template<> core::files::file_system* ang::singleton<core::files::file_system_t>::instance() {
	if (_instance == null)
		new core::files::file_system();
	return static_cast<core::files::file_system*>(_instance);
}
template<> bool ang::singleton<core::files::file_system_t>::release_instance() {
	if (_instance)_instance->release();
	_instance = null;
	return true;
}
core::files::ifile_system_t core::files::ifile_system::get_file_system() {
	return ang::singleton<core::files::file_system_t>::instance();
}

memory::iallocator* memory::allocator_manager::get_allocator(index idx)
{
	switch (idx)
	{
	case ang::memory::allocator_manager::object_allocator:
		return &ang_library_initializer::instance()->m_object_allocator;
	case ang::memory::allocator_manager::buffer_allocator:
		return &ang_library_initializer::instance()->m_buffer_allocator;
	case ang::memory::allocator_manager::aligned_allocator:
		return &ang_library_initializer::instance()->m_aligned_allocator;
	case ang::memory::allocator_manager::default_allocator:
	default:
		return &ang_library_initializer::instance()->m_default_allocator;
	}
}


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


extern "C" void ang_debug_output_info(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	size_t size = _vscprintf(format, argList);

	char* buffer = (char*)malloc(size + 1);
	buffer[size] = 0;


	vsprintf_s(buffer, size + 1, format, argList);
	va_end(argList);

	char logger[50];
	sprintf(logger, "info:[thread:%#06x]:", (uint)core::async::thread::current_thread_id());

#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_INFO, (const char*)logger, "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
	ang_library_initializer::instance()->logger.print(debug::log_level::info, "%s %s\n", logger, buffer);
#else
	printf("%s %s\n", logger, buffer);
	OutputDebugStringA(logger);
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
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

	char logger[50];
	sprintf(logger, "warning:[thread:%#06x]:", (uint)core::async::thread::current_thread_id());


#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_WARN, (const char*)logger, "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
	ang_library_initializer::instance()->logger.print(debug::log_level::warning, "%s %s\n", logger, buffer);
#else
	printf("%s %s\n", logger, buffer);
	OutputDebugStringA(logger);
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
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

	char logger[50];

	sprintf(logger, "error:[thread:%#06x]:", (uint)core::async::thread::current_thread_id());


#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_ERROR, (const char*)logger, "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
	ang_library_initializer::instance()->logger.print(debug::log_level::error, "%s %s\n", logger, buffer);
#else
	printf("%s %s\n", logger, buffer);
	OutputDebugStringA(logger);
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
#endif

	free(buffer);
}

