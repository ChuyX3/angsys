/*********************************************************************************************************************/
/*   File Name: memory.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angbase.h"
#include "ang_memory.h"
//#include "ang/maths.h"
//ang_core/ang_btree.h"


using namespace ang;
using namespace ang::memory;

static allocator_internal& get_allocator(ang_memory_hint_t hint)
{
	static allocator_internal default_memory_allocator(ang_default_memory);
	static allocator_internal object_memory_allocator(ang_object_memory);
	static allocator_internal buffer_memory_allocator(ang_buffer_memory);

	switch (hint)
	{
	case ang_object_memory:
		return object_memory_allocator;
	case ang_buffer_memory:
		return buffer_memory_allocator;
	default:
		return default_memory_allocator;
	}
}

static aligned_allocator_internal& get_aligned_allocator() {
	static aligned_allocator_internal aligned_memory_allocator;
	return aligned_memory_allocator;
}

ANG_EXTERN pointer ang_alloc_unmanaged_memory(wsize size)
{
	return malloc(size);
}

ANG_EXTERN void ang_free_unmanaged_memory(pointer ptr)
{
	free(ptr);
}

ANG_EXTERN ang_void_ptr_t ang_alloc_managed_memory(ang_size_t size, ang_memory_hint_t hint)
{
	return get_allocator(hint).memory_alloc(size);
}

ANG_EXTERN void ang_free_managed_memory(ang_void_ptr_t ptr, ang_memory_hint_t hint)
{
	return get_allocator(hint).memory_release(ptr);
}

ANG_EXTERN ang_void_ptr_t ang_alloc_aligned_memory(ang_size_t size, wsize ALIGMENT)
{
	return get_aligned_allocator().aligned_memory_alloc(size, ALIGMENT);
}

ANG_EXTERN void ang_free_aligned_memory(ang_void_ptr_t ptr)
{
	get_aligned_allocator().memory_release(ptr);
}

#ifdef _DEBUG
ANG_EXTERN ang_void_ptr_t ang_alloc_unmanaged_memory_debug(ang_size_t size, const char*, int)
{
	return malloc(size);
}

ANG_EXTERN ang_void_ptr_t ang_alloc_managed_memory_debug(ang_size_t size, ang_memory_hint_t hint, const char* file, int line)
{
	return get_allocator(hint).memory_alloc(size, file, line);
}

ANG_EXTERN ang_void_ptr_t ang_alloc_aligned_memory_debug(ang_size_t size, wsize ALIGMENT, const char* file, int line)
{
	return get_aligned_allocator().aligned_memory_alloc(size, ALIGMENT, file, line);
}
#endif




struct _memory_block
{
	wsize size;
	pointer ptr;
	int line;
	char* file;
};

ANG_EXTERN memory_block_t memory_block_create(wsize size, pointer ptr)
{
	memory_block_t block = (memory_block_t)(ptr);
	block->size = size;
	block->ptr = pointer(wsize(ptr) + ang::align_to<8, sizeof(memory_block)>());
	return block;
}

#ifdef _MEMORY_PROFILING
void allocator_internal::hash_table_context_delete_data(ang_hash_table_pair_t pair)
{
	memory_block_t value = (memory_block_t)pair.value;
	if (value->file)
		ang_free_unmanaged_memory(value->file);

	ang_free_unmanaged_memory(value);
	
}
#endif

allocator_internal::allocator_internal(uint type)
#ifdef _MEMORY_PROFILING
	: _allocator_type(type)
	, hMutex(CreateMutexW(NULL, FALSE, NULL))
	, memory_map()
#endif
{

}

allocator_internal::~allocator_internal() {
#ifdef _MEMORY_PROFILING
	bool error = false;
	if (!memory_map.is_empty())
		memory_map.print([&](pointer ptr, memory_block_t value)
	{
		if (value)
		{
			error = true;
			ang_debug_output_warning("Allocator[%d]: Memory leak at %X: size: %d; file: %s: %d;\n", _allocator_type, (wsize)value->ptr, value->size, value->file, value->line);
		}
	});
	if (error)
		__debugbreak();

	memory_map.clean();
	CloseHandle(hMutex);
#endif // _MEMORY_PROFILING

}

pointer allocator_internal::memory_alloc(wsize size)
{
#ifdef _MEMORY_PROFILING
	auto block = memory_block_create(size, ang_alloc_unmanaged_memory(ang::align_to<8, sizeof(memory_block)>() + size));
	block->line = 0;
	block->file = null;
	WaitForSingleObjectEx(hMutex, INFINITE, TRUE);
	if (!memory_map.insert(block->ptr, block))
	{
		__debugbreak();
	}
	ReleaseMutex(hMutex);
	return block->ptr;
#else
	return ang_alloc_unmanaged_memory(size);
#endif
}

void allocator_internal::memory_release(pointer ptr)
{
#ifdef _MEMORY_PROFILING
	memory_block_t block;
	WaitForSingleObjectEx(hMutex, INFINITE, TRUE);
	if (!ptr || !memory_map.remove(ptr, &block)) {
		ReleaseMutex(hMutex);
		return;
	}
	ReleaseMutex(hMutex);
	if (block == null)
	{
		__debugbreak();
	}
	if (block->file)ang_free_unmanaged_memory(block->file);
	block->file = null;
	ang_free_unmanaged_memory(block);
#else
	ang_free_unmanaged_memory(ptr);
#endif
}

#ifdef _DEBUG
pointer allocator_internal::memory_alloc(wsize size, const char* file, int line)
{
#ifdef _MEMORY_PROFILING
	auto block = memory_block_create(size, ang_alloc_unmanaged_memory(ang::align_to<8, sizeof(memory_block)>() + size));
	block->line = line;
	block->file = (char*)ang_alloc_unmanaged_memory(strlen(file) + 1);
	strcpy(block->file, file);
	WaitForSingleObjectEx(hMutex, INFINITE, TRUE);
	if (!memory_map.insert(block->ptr, block))
	{
		__debugbreak();
	}
	ReleaseMutex(hMutex);
	return block->ptr;
#else
	return ang_alloc_unmanaged_memory(size);
#endif
}
#endif	


//////////////////////////////////////////////////////////////////////

aligned_allocator_internal::aligned_allocator_internal() {}

aligned_allocator_internal::~aligned_allocator_internal() {}

pointer aligned_allocator_internal::memory_alloc(wsize size)
{
	return _mm_malloc(size, 16);
}

pointer aligned_allocator_internal::aligned_memory_alloc(wsize size, wsize alignment)
{
	return _mm_malloc(size, alignment);
}

void aligned_allocator_internal::memory_release(pointer ptr)
{
	if (ptr)_mm_free(ptr);
}

#ifdef _DEBUG
pointer aligned_allocator_internal::memory_alloc(wsize size, const char* file, int line)
{
	return aligned_memory_alloc(size, 16);
}

pointer aligned_allocator_internal::aligned_memory_alloc(wsize size, wsize alignment, const char* file, int line)
{
	return _mm_malloc(size, alignment);
}
#endif

