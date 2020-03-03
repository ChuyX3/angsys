#include "pch.h"
#include "ang_memory.h"

#ifdef ANDROID_PLATFORM
#include <malloc.h>
#endif

using namespace ang;
using namespace ang::memory;

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
#ifdef _MEMORY_PROFILING
	: _allocator_type(hint)
#endif
{
}

allocator_internal::~allocator_internal()
{
#ifdef _MEMORY_PROFILING
	bool error = false;
	core::async::scope_locker<decltype(mutex)> lock(mutex);
	if (!memory_map.is_empty()) memory_map.iterate([&](collections::pair<pointer, memory_block_t>& info)
	{
		if (info.key)
		{
			error = true;
			//				NN_LOG("Allocator[%d]: Memory leak at %X: size: %d; file: %s: %d;\n", _allocator_type, (wsize)info.second->ptr, info.second->size, info.second->file, info.second->line);
		}


		if (info.value->file)
			ang_free_unmanaged_memory(info.value->file);
		ang_free_unmanaged_memory(info.value);
	});
	memory_map.clear();
#ifdef WINDOWS_PLATFORM
	if (error)
		__debugbreak();
#endif
#endif
}

pointer allocator_internal::memory_alloc(wsize size)
{
#ifdef _MEMORY_PROFILING
	auto block = memory_block_create(size, ang_alloc_unmanaged_memory(align_up<8, sizeof(memory_block)>() + size));
	block->line = 0;
	block->file = nullptr;
	{ //scope
		core::async::scope_locker<decltype(mutex)> lock(mutex);
		memory_map.insert(block->ptr, block);
	}
	return block->ptr;
#else
	return ang_alloc_unmanaged_memory(size);
#endif
}

void allocator_internal::memory_release(pointer ptr)
{
#ifdef _MEMORY_PROFILING
	memory_block_t block;
	{ //scope
		core::async::scope_locker<decltype(mutex)> lock(mutex);
		if (!memory_map.remove(ptr, &block))
			return;
	}

	if (block->file)ang_free_unmanaged_memory(block->file);
	block->file = nullptr;
	ang_free_unmanaged_memory(block);

#else
	return ang_free_unmanaged_memory(ptr);
#endif
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

#ifdef _MEMORY_PROFILING
	bool error = false;
	core::async::scope_locker<decltype(mutex)> lock(mutex);
	if (!memory_map.is_empty()) memory_map.iterate([&](collections::pair<pointer, memory_block_t>& info)
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

	memory_map.clear();
#ifdef WINDOWS_PLATFORM
	if (error)
		__debugbreak();
#endif
#endif
}

pointer aligned_allocator_internal::memory_alloc(wsize size) {
	return aligned_memory_alloc(size, 16u);
}

#ifdef WINDOWS_PLATFORM

void aligned_allocator_internal::memory_release(pointer ptr) {
#ifdef _MEMORY_PROFILING
	memory_block_t block;
	{ //scope
		core::async::scope_locker<decltype(mutex)> lock(mutex);
		if (!memory_map.remove(ptr, &block))
			return;
	}

	if (block->file)ang_free_unmanaged_memory(block->file);
	block->file = nullptr;
	_aligned_free(block);
#else
	_aligned_free(ptr);
#endif
}

pointer aligned_allocator_internal::aligned_memory_alloc(wsize size, const wsize ALIGMENT) {
#ifdef _MEMORY_PROFILING
	auto block = memory_block_create(size, _aligned_malloc(align_up<8, sizeof(memory_block)>() + size, ALIGMENT));
	block->line = 0;
	block->file = nullptr;
	{ //scope
		core::async::scope_locker<decltype(mutex)> lock(mutex);
		memory_map.insert(block->ptr, block);
	}
	return block->ptr;
#else
	return _aligned_malloc(size, ALIGMENT);
#endif
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

#endif
#elif defined ANDROID_PLATFORM
void aligned_allocator_internal::memory_release(pointer ptr) {
	free(ptr);
}

pointer aligned_allocator_internal::aligned_memory_alloc(wsize size, const wsize ALIGMENT) {
	return memalign(ALIGMENT, size);
}

#ifdef _DEBUG
pointer aligned_allocator_internal::memory_alloc(wsize size, const char* file, int line) {
	return memalign(16U, size);
}

pointer aligned_allocator_internal::aligned_memory_alloc(wsize size, wsize ALIGMENT, const char* file, int line) {
	return memalign(ALIGMENT, size);
}

#endif
#endif




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
	return get_raw_allocator(hint)->memory_alloc(size);
}

ANG_EXTERN void ang_free_managed_memory(pointer ptr, ang_memory_hint_t hint)
{
	return get_raw_allocator(hint)->memory_release(ptr);
}

ANG_EXTERN pointer ang_alloc_aligned_memory(wsize size, wsize ALIGMENT)
{
	return get_aligned_allocator()->aligned_memory_alloc(size, ALIGMENT);
}

ANG_EXTERN void ang_free_aligned_memory(pointer ptr)
{
	get_aligned_allocator()->memory_release(ptr);
}

#ifdef _DEBUG
ANG_EXTERN pointer ang_alloc_unmanaged_memory_debug(wsize size, const char*, int)
{
	return malloc(size);
}

ANG_EXTERN pointer ang_alloc_managed_memory_debug(wsize size, ang_memory_hint_t hint, const char* file, int line)
{
	return get_raw_allocator(hint)->memory_alloc(size, file, line);
}

ANG_EXTERN pointer ang_alloc_aligned_memory_debug(wsize size, wsize ALIGMENT, const char* file, int line)
{
	return get_aligned_allocator()->aligned_memory_alloc(size, ALIGMENT, file, line);
}
#endif
