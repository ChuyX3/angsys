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
#include "angsys.h"
#include "ang_memory.h"
//#include "ang/maths.h"
//ang_core/ang_btree.h"


using namespace ang;
using namespace ang::memory;

ANG_IMPLEMENT_INTERFACE(ang::memory, iallocator);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::memory::ialigned_allocator, iallocator);

ANG_EXTERN pointer ang_alloc_unmanaged_memory(wsize size)
{
	return malloc(size);
}

ANG_EXTERN void ang_free_unmanaged_memory(pointer ptr)
{
	free(ptr);
}


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
	block->ptr = block + 1;
	return block;
}

index allocator_manager::regist_allocator(iallocator*)
{
	return -1;
}

pointer base_allocator::operator new(wsize size)
{
	return ang_alloc_unmanaged_memory(size);
}

void base_allocator::operator delete(pointer ptr)
{
	ang_free_unmanaged_memory(ptr);
}

//////////////////////////////////////////////////////////////////

default_allocator::default_allocator(uint type)
	: _allocator_type(type)
#ifdef _MEMORY_PROFILING
	, mutex()
	, memory_map()
#endif
{

}

default_allocator::~default_allocator() {
#ifdef _MEMORY_PROFILING
	bool error = false;
	if (!memory_map.is_empty())
		memory_map.print([&](memory_block_t value)
	{
		if (value)
		{
			error = true;
			ang_debug_output_warning("Allocator[%d]: Memory leak at %X: size: %d; file: %s: %d;\n", _allocator_type, (wsize)value->ptr, value->size, value->file, value->line);
		}
	});
	if (error)
		__debugbreak();

	memory_map.clean([](memory_block_t value)
	{
		if (value->file)
			ang_free_unmanaged_memory(value->file);

		ang_free_unmanaged_memory(value);
	});
#endif // _MEMORY_PROFILING

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::memory::default_allocator, ang::memory::iallocator);

bool default_allocator::can_delete_from(iallocator* allocator)const
{
	if (allocator && allocator->is_kind_of(type_of<default_allocator>()))
		return true;
	return false;
}

pointer default_allocator::malloc(wsize size)
{
#ifdef _MEMORY_PROFILING
	auto block = memory_block_create(size, ang_alloc_unmanaged_memory(sizeof(memory_block) + size));
	block->line = 0;
	block->file = null;
	mutex.lock();
	if (!memory_map.insert(block->ptr, block))
	{
		__debugbreak();
	}
	mutex.unlock();
	return block->ptr;
#else
	return ang_alloc_unmanaged_memory(size);
#endif
}

void default_allocator::free(pointer ptr)
{
#ifdef _MEMORY_PROFILING
	memory_block_t block;
	mutex.lock();
	if (!ptr || !memory_map.remove(ptr, block)) {
		mutex.unlock();
		return;
	}
	mutex.unlock();
	if (block == null)
	{
		__debugbreak();
	}
	ang_free_unmanaged_memory(block);
#else
	ang_free_unmanaged_memory(ptr);
#endif
}

#ifdef _DEBUG
pointer default_allocator::malloc(wsize size, const char* file, int line)
{
#ifdef _MEMORY_PROFILING
	auto block = memory_block_create(size, ang_alloc_unmanaged_memory(sizeof(memory_block) + size));
	block->line = line;
	block->file = (char*)ang_alloc_unmanaged_memory(strings::algorithms::string_length(file) + 1);
	strings::algorithms::string_copy(block->file, file);
	mutex.lock();
	if (!memory_map.insert(block->ptr, block))
	{
		__debugbreak();
	}
	mutex.unlock();
	return block->ptr;
#else
	return ang_alloc_unmanaged_memory(size);
#endif
}
#endif	

#ifdef _MEMORY_PROFILING
void default_allocator::set_logger(debug::ilogger_t log)
{
	logger = log;
}
#endif

//////////////////////////////////////////////////////////////////////

aligned_allocator::aligned_allocator() {}

aligned_allocator::~aligned_allocator() {}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::memory::aligned_allocator, ang::memory::ialigned_allocator);

bool aligned_allocator::can_delete_from(iallocator* allocator)const
{
	if (allocator && allocator->is_kind_of(type_of<aligned_allocator>()))
		return true;
	return false;
}

pointer aligned_allocator::malloc(wsize size)
{
	return _mm_malloc(size, 16);
}

pointer aligned_allocator::aligned_malloc(wsize size, wsize alignment)
{
	return _mm_malloc(size, alignment);
}

void aligned_allocator::free(pointer ptr)
{
	if (ptr)_mm_free(ptr);
}

#ifdef _DEBUG
pointer aligned_allocator::malloc(wsize size, const char* file, int line)
{
	return aligned_malloc(size, 16);
}

pointer aligned_allocator::aligned_malloc(wsize size, wsize alignment, const char* file, int line)
{
	return _mm_malloc(size, alignment);
}
#endif

#ifdef _MEMORY_PROFILING
void aligned_allocator::set_logger(debug::ilogger_t)
{
}
#endif
