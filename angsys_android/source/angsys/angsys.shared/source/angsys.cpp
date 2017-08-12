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

#include <angsys.h>
#include "ang_memory.h"
#include "ang_debugger.h"

#ifdef ANGSYS_DYNAMIC_LIBRARY
ANG_IMPLEMENT_MEMORYMANAGER();
#endif


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
				ang_allocator<node_t>::destruct_and_free(node);
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
				node = ang_allocator<node_t>::construct(ang_allocator<node_t>::alloc(1));
				node->type_name = ang::move(name);
				node->_next = null;
				return node;
			}
			return insert(node->_next, ang::move(name));
		}
	};

	class ang_instance_manager
	{
	private:
		friend singleton<ang_instance_manager>;
		static ang_instance_manager _main_instance;
	public:
		static ang_instance_manager* instance() { return &_main_instance; }


	private:
		ang_instance_manager()
			: _default_allocator(0)
			, _object_allocator(1)
			, _buffer_allocator(2)
			, _aligned_allocator()
			, _platform_logger()
		{
			_default_allocator.set_logger(&_platform_logger);
			_object_allocator.set_logger(&_platform_logger);
			_buffer_allocator.set_logger(&_platform_logger);
			_aligned_allocator.set_logger(&_platform_logger);
		}

		~ang_instance_manager()
		{

		}

	public:
		ang::memory::default_allocator _default_allocator;
		ang::memory::default_allocator _object_allocator;
		ang::memory::default_allocator _buffer_allocator;
		ang::memory::aligned_allocator _aligned_allocator;
		runtime_data_base_imp _runtime_data_base;

#ifdef _DEBUG
		ang::debug::platform_logger _platform_logger;
#endif
	};

	ang_instance_manager ang_instance_manager::_main_instance;
}

ang::memory::iallocator* ang::memory::allocator_manager::get_allocator(index idx)
{
	switch (idx)
	{
	case allocator_manager::object_allocator:
		return &ang_instance_manager::instance()->_object_allocator;
	case allocator_manager::buffer_allocator:
		return &ang_instance_manager::instance()->_buffer_allocator;
	case allocator_manager::aligned_allocator:
		return &ang_instance_manager::instance()->_aligned_allocator;
	default:
		return &ang_instance_manager::instance()->_default_allocator;
	}
}

ang::type_name_t ang::runtime::runtime_data_base::regist_typename(ang::string name)
{
	return ang_instance_manager::instance()->_runtime_data_base.insert(ang::move(name));
}

#ifdef _DEBUG

ang::debug::ilogger* ang::debug::get_platform_logger()
{
	return &ang_instance_manager::instance()->_platform_logger;
}

#endif // _DEBUG



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
	ang_library_initializer::instance()->logger.print(debug::log_level::info, "%s %s\n", logger, buffer);
#endif
#else
	printf("%s %s\n", "angsys", buffer);
	OutputDebugStringA("angsys");
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
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
	ang_library_initializer::instance()->logger.print(debug::log_level::info, "%s %s\n", logger, buffer);
#endif
#else
	printf("%s %s\n", "angsys", buffer);
	OutputDebugStringA("angsys");
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

#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_WARN, "angsys", "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
#if defined _DEBUG || defined _DEVELOPPER
	ang_library_initializer::instance()->logger.print(debug::log_level::warning, "%s %s\n", logger, buffer);
#endif
#else
	printf("%s %s\n", "angsys", buffer);
	OutputDebugStringA("angsys");
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


#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_ERROR, "angsys", "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
#if defined _DEBUG || defined _DEVELOPPER
	ang_library_initializer::instance()->logger.print(debug::log_level::error, "%s %s\n", logger, buffer);
#else
	printf("%s %s\n", logger, buffer);
#endif
#else
	printf("%s %s\n", "angsys", buffer);
	OutputDebugStringA("angsys");
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
#endif

	free(buffer);
}

