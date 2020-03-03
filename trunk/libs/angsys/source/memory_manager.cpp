#include "pch.h"
#include "memory_manager.h"

#ifdef ANDROID_PLATFORM
#include <malloc.h>
#endif

using namespace ang;
using namespace ang::memory;

#ifdef _MEMORY_PROFILING

typedef struct _memory_block
{
	wsize size;
	pointer ptr;
	int line;
	char* file;
} memory_block, *memory_block_t;


ANG_EXTERN memory_block_t memory_block_create(wsize size, pointer ptr)
{
	memory_block_t block = (memory_block_t)(ptr);
	block->size = size;
	block->ptr = pointer(wsize(ptr) + align_up<8, sizeof(memory_block)>());
	return block;
}

allocator_internal::allocator_internal(ang_memory_hint_t hint)
	: _allocator_type(hint)
{
}

allocator_internal::~allocator_internal()
{
	bool error = false;
	core::async::scope_locker<decltype(mutex)> lock(mutex);
	if (!memory_map.is_empty()) memory_map.clear([&](collections::pair<pointer, memory_block_t>& info)
	{
		if (info.key)
		{
			error = true;
		}

		if (info.value->file)
			ang_free_unmanaged_memory(info.value->file);
		ang_free_unmanaged_memory(info.value);
	});
	//memory_map.clear();
#ifdef WINDOWS_PLATFORM
	if (error)
		__debugbreak();
#endif
}

pointer allocator_internal::memory_alloc(wsize size)
{
	auto block = memory_block_create(size, ang_alloc_unmanaged_memory(align_up<8, sizeof(memory_block)>() + size));
	block->line = 0;
	block->file = nullptr;
	{ //scope
		core::async::scope_locker<decltype(mutex)> lock(mutex);
		memory_map.insert(block->ptr, block);
	}
	return block->ptr;
}

void allocator_internal::memory_release(pointer ptr)
{
	memory_block_t block;
	{ //scope
		core::async::scope_locker<decltype(mutex)> lock(mutex);
		if (!memory_map.remove(ptr, block))
			return;
	}

	if (block->file)ang_free_unmanaged_memory(block->file);
	block->file = nullptr;
	ang_free_unmanaged_memory(block);
}

#ifdef _DEBUG
pointer allocator_internal::memory_alloc(wsize size, const char* file, int line)
{
	auto block = memory_block_create(size, ang_alloc_unmanaged_memory(align_up<8, sizeof(memory_block)>() + size));
	block->line = line;
	block->file = (char*)ang_alloc_unmanaged_memory(strlen(file) + 1);
	strcpy(block->file, file);
	{ //scope
		core::async::scope_locker<decltype(mutex)> lock(mutex);
		memory_map.insert(block->ptr, block);
	}
	return block->ptr;
}
#endif


////////////////////////////////////////////////////////////////////////////

aligned_allocator_internal::aligned_allocator_internal()
	: allocator_internal(ang_aligned_memory) {
}
aligned_allocator_internal::~aligned_allocator_internal() {

	bool error = false;
	core::async::scope_locker<decltype(mutex)> lock(mutex);
	if (!memory_map.is_empty()) memory_map.clear([&](collections::pair<pointer, memory_block_t>& info)
	{
		if (info.key)
		{
			error = true;
			//				NN_LOG("Allocator[%d]: Memory leak at %X: size: %d; file: %s: %d;\n", _allocator_type, (wsize)info.second->ptr, info.second->size, info.second->file, info.second->line);
		}

		if (info.value->file)
			ang_free_unmanaged_memory(info.value->file);
		_aligned_free(info.value);
	});
	//memory_map.clear();
#ifdef WINDOWS_PLATFORM
	if (error)
		__debugbreak();
#endif
}

pointer aligned_allocator_internal::memory_alloc(wsize size) {
	return aligned_memory_alloc(size, 16u);
}


void aligned_allocator_internal::memory_release(pointer ptr) {
	memory_block_t block;
	{ //scope
		core::async::scope_locker<decltype(mutex)> lock(mutex);
		if (!memory_map.remove(ptr, block))
			return;
	}

	if (block->file)ang_free_unmanaged_memory(block->file);
	block->file = nullptr;
#ifdef WINDOWS_PLATFORM
	_aligned_free(block);
#elif defined ANDROID_PLATFORM
	free(ptr);
#endif
}

pointer aligned_allocator_internal::aligned_memory_alloc(wsize size, const wsize ALIGMENT) {
#ifdef WINDOWS_PLATFORM
	auto block = memory_block_create(size, _aligned_malloc(align_up<8, sizeof(memory_block)>() + size, ALIGMENT));
#elif defined ANDROID_PLATFORM
	auto block = memory_block_create(size, memalign(ALIGMENT, align_up<8, sizeof(memory_block)>() + size));
#endif
	
	block->line = 0;
	block->file = nullptr;
	{ //scope
		core::async::scope_locker<decltype(mutex)> lock(mutex);
		memory_map.insert(block->ptr, block);
	}
	return block->ptr;
}

#ifdef _DEBUG
pointer aligned_allocator_internal::memory_alloc(wsize size, const char* file, int line) {
	return aligned_memory_alloc(size, 16U, file, line);
}

pointer aligned_allocator_internal::aligned_memory_alloc(wsize size, wsize ALIGMENT, const char* file, int line) {
	auto block = memory_block_create(size, _aligned_malloc(align_up<8, sizeof(memory_block)>() + size, ALIGMENT));
	block->line = line;
	block->file = (char*)ang_alloc_unmanaged_memory(strlen(file) + 1);
	strcpy(block->file, file);
	{ //scope
		core::async::scope_locker<decltype(mutex)> lock(mutex);
		memory_map.insert(block->ptr, block);
	}
	return block->ptr;
}
#endif//_DEBUG
#endif//_MEMORY_PROFILING

///////////////////////////////////////////////////////////////////////////////////////////////////////


ANG_EXTERN pointer ang_alloc_unmanaged_memory(wsize size)
{
	return malloc(size);
}

ANG_EXTERN void ang_free_unmanaged_memory(pointer ptr)
{
	free(ptr);
}

ANG_EXTERN pointer ang_alloc_managed_memory(wsize size, ang_memory_hint_t hint)
{
#ifdef _MEMORY_PROFILING
	return get_raw_allocator(hint)->memory_alloc(size);
#else//_MEMORY_PROFILING
	return malloc(size);
#endif//_MEMORY_PROFILING
}

ANG_EXTERN void ang_free_managed_memory(pointer ptr, ang_memory_hint_t hint)
{
#ifdef _MEMORY_PROFILING
	return get_raw_allocator(hint)->memory_release(ptr);
#else//_MEMORY_PROFILING
	if (hint == ang_aligned_memory)
		ang_free_aligned_memory(ptr);
	else
		free(ptr);
#endif//_MEMORY_PROFILING
}

ANG_EXTERN pointer ang_alloc_aligned_memory(wsize size, wsize ALIGMENT)
{
#ifdef _MEMORY_PROFILING
	return get_raw_allocator(ang_aligned_memory)->aligned_memory_alloc(size, ALIGMENT);
#else//_MEMORY_PROFILING
#ifdef WINDOWS_PLATFORM
	return _aligned_malloc(size, ALIGMENT);
#elif defined ANDROID_PLATFORM
	return memalign(ALIGMENT, size);
#endif
#endif//_MEMORY_PROFILING
}

ANG_EXTERN void ang_free_aligned_memory(pointer ptr)
{
	
#ifdef _MEMORY_PROFILING
	get_raw_allocator(ang_aligned_memory)->memory_release(ptr);
#else//_MEMORY_PROFILING

#ifdef WINDOWS_PLATFORM
	_aligned_free(ptr);
#elif defined ANDROID_PLATFORM
	free(ptr);
#endif
#endif//_MEMORY_PROFILING
}

#ifdef _DEBUG
ANG_EXTERN pointer ang_alloc_unmanaged_memory_debug(wsize size, const char*, int)
{
	return malloc(size);
}

ANG_EXTERN pointer ang_alloc_managed_memory_debug(wsize size, ang_memory_hint_t hint, const char* file, int line)
{
#ifdef _MEMORY_PROFILING
	return get_raw_allocator(hint)->memory_alloc(size, file, line);
#else//_MEMORY_PROFILING
	return malloc(size);
#endif//_MEMORY_PROFILING
}

ANG_EXTERN pointer ang_alloc_aligned_memory_debug(wsize size, wsize ALIGMENT, const char* file, int line)
{
#ifdef _MEMORY_PROFILING
	return get_raw_allocator(ang_aligned_memory)->aligned_memory_alloc(size, ALIGMENT, file, line);
#else//_MEMORY_PROFILING
#ifdef WINDOWS_PLATFORM
	return _aligned_malloc(size, ALIGMENT);
#elif defined ANDROID_PLATFORM
	return memalign(ALIGMENT, size);
#endif
#endif//_MEMORY_PROFILING
}
#endif
