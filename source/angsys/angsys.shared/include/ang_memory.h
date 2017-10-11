#ifndef __ANG_MEMORY_H__
#define __ANG_MEMORY_H__

#include <angplatform.h>
#include <angtypes.h>
#include "angc/core_hash_table.h"

#if defined _DEBUG || defined _DEVELOPPER
#define _MEMORY_PROFILING
#endif

typedef struct _memory_block memory_block, *memory_block_t;
ANG_EXTERN memory_block_t memory_block_create(ang_size_t, ang_void_ptr_t);

namespace ang
{
	namespace memory
	{
		class allocator_internal
		{
#ifdef _MEMORY_PROFILING
		public:
			static ulong64 hash_table_context_create_hash_number(pointer key, ulong64 size) { return ulong64(key) % size; }
			static void hash_table_context_delete_data(ang_hash_table_pair_t pair);
#endif
		private:
#ifdef _MEMORY_PROFILING
			uint _allocator_type;
			HANDLE hMutex;
			ang_unorder_map<pointer, memory_block_t, allocator_internal> memory_map;
#endif
		public:
			allocator_internal(uint);
			~allocator_internal();

			pointer memory_alloc(wsize);
			void memory_release(pointer);
#ifdef _DEBUG
			pointer memory_alloc(wsize, const char* file, int line);
#endif
		};

		class aligned_allocator_internal
		{
		public:
			aligned_allocator_internal();
			~aligned_allocator_internal();

			pointer memory_alloc(wsize);
			void memory_release(pointer);
			pointer aligned_memory_alloc(wsize, wsize);

#ifdef _DEBUG
			pointer memory_alloc(wsize, const char* file, int line);
			pointer aligned_memory_alloc(wsize, wsize, const char* file, int line);
#endif

		};
	}
}

#endif//__ANG_MEMORY_H__