#include "pch.h"
#include "library.h"

using namespace ang;


ANG_EXTERN ulong64 get_performance_time_us(void)
{
	static struct PerformanceFrequency {
		ulong64 QuadPart;
		PerformanceFrequency() {
			LARGE_INTEGER _frec;
			QueryPerformanceFrequency(&_frec);
			QuadPart = (ulong64)_frec.QuadPart;
		}
	}frec;

	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return  (1000000 * count.QuadPart) / frec.QuadPart; //uS
}



ang_library_initializer::ang_library_initializer()
	: main_mutex()
	, runtime_type_manager()
//	, thread_manager(null)
//	, file_system(null)
#if defined _DEBUG || defined _DEVELOPPER
	, objects(null)
#endif //DEBUG
{
#ifdef _MEMORY_PROFILING
	m_alloc[0] = memory::unmanaged_allocator<memory::allocator_internal>::template construct<memory::allocator_internal>(
		memory::unmanaged_allocator<memory::allocator_internal>::allocate(1), ang_default_memory);
	m_alloc[1] = memory::unmanaged_allocator<memory::allocator_internal>::template construct<memory::allocator_internal>(
		memory::unmanaged_allocator<memory::allocator_internal>::allocate(1), ang_object_memory);
	m_alloc[2] = memory::unmanaged_allocator<memory::allocator_internal>::template construct<memory::allocator_internal>(
		memory::unmanaged_allocator<memory::allocator_internal>::allocate(1), ang_buffer_memory);
	m_alloc[3] = memory::unmanaged_allocator<memory::aligned_allocator_internal>::template construct<memory::aligned_allocator_internal>(
		memory::unmanaged_allocator<memory::aligned_allocator_internal>::allocate(1));
#endif//_MEMORY_PROFILING

#if defined _DEBUG || defined _DEVELOPPER
	objects = new(malloc(sizeof(object_manager))) object_manager();
#endif // _DEBUG

//	thread_manager = new(malloc(sizeof(core::async::thread_manager))) core::async::thread_manager();

}

ang_library_initializer::	~ang_library_initializer()
{
//	file_system = null;
//	thread_manager->~thread_manager();
//	free(thread_manager);
//	thread_manager = null;

#if defined _DEBUG || defined _DEVELOPPER
 	objects->~object_manager();
	free(objects);
	objects = null;
#endif // _DEBUG

	runtime_type_manager.clear();

#ifdef _MEMORY_PROFILING
	for (auto alloc : m_alloc)
	{
		alloc->~iraw_allocator();
		ang_free_unmanaged_memory(alloc);
	}
#endif//_MEMORY_PROFILING
}

//#define ANG_STATIC_INITIALIZE
#ifndef ANG_STATIC_INITIALIZE
static ang_library_initializer* s_ang_library_initializer = null;
ANG_EXTERN void ang_library_start() {
	if (s_ang_library_initializer == null) {
		s_ang_library_initializer = (ang_library_initializer*)malloc(size_of<ang_library_initializer>());
		new(s_ang_library_initializer) ang_library_initializer();
	}
}
ANG_EXTERN void ang_library_shutdown() {
	if (s_ang_library_initializer != null) {
		s_ang_library_initializer->~ang_library_initializer();
		free(s_ang_library_initializer);
		s_ang_library_initializer = null;
	}
}
ang_library_initializer* ang_library_initializer::instance() {
	//ang_library_start();
	return s_ang_library_initializer;
}
#else
static ang_library_initializer s_ang_library_initializer;
ANG_EXTERN void ang_library_start() {
}
ANG_EXTERN void ang_library_shutdown() {
}
ang_library_initializer* ang_library_initializer::instance() {
	return &s_ang_library_initializer;
}
#endif

#ifdef _MEMORY_PROFILING
memory::iraw_allocator* memory::get_raw_allocator(ang_memory_hint_t hint)
{
	return hint < 4 ? ang_library_initializer::instance()->m_alloc[(int)hint] : ang_library_initializer::instance()->m_alloc[0];
}
#endif//_MEMORY_PROFILINGc

runtime::runtime_type_manager* runtime::runtime_type_manager::instance()
{
	return &ang_library_initializer::instance()->runtime_type_manager;
}

#if defined _DEBUG || defined _DEVELOPPER
object_manager* object_manager::instance()
{
	return ang_library_initializer::instance()->objects;
}
#endif

/*
core::files::ifile_system_t core::files::ifile_system::instance() {
	auto mng = ang_library_initializer::instance();
	return mng->main_mutex.sync([&]()->core::files::ifile_system*
	{
		if (mng->file_system.is_empty())
			mng->file_system = new core::files::file_system();
		return mng->file_system.get();
	});
}
*/

static const ang_uint32_t s_list[] = {
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

namespace ang
{
	namespace algorithms
	{
		wsize hash_table_get_next_size(wsize size) {
			wsize out = 0;
			if (size >= 16381) {
				out = 16381;
				while (size >= out)
					out *= 2;
				return out;
			}
			else {
				for (auto i = 0U; i < algorithms::array_size(s_list); ++i)
					if (size < s_list[i])
						return s_list[i];
				return -1;
			}
		}
	}
}


extern "C" void ang_debug_output_info(const char* message)
{
	astring msg = "info: "_sv;
	msg.append(castr_t(message, -1));
#if defined WINDOWS_PLATFORM && defined _DEBUG
	OutputDebugStringA(msg.cstr());
#endif // WINDOWS_PLATFORM
}

extern "C" void ang_debug_output_warning(const char* message)
{
	astring msg = "warning: "_sv;
	msg.append(castr_t(message, -1));
#if defined WINDOWS_PLATFORM && defined _DEBUG
	OutputDebugStringA(msg.cstr());
#endif // WINDOWS_PLATFORM
}

extern "C" void ang_debug_output_error(const char* message)
{
	astring msg = "error: "_sv;
	msg.append(castr_t(message, -1));
#if defined WINDOWS_PLATFORM && defined _DEBUG
	OutputDebugStringA(msg.cstr());
#endif // WINDOWS_PLATFORM
}