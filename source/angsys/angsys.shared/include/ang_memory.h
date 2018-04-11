#ifndef __ANG_MEMORY_H__
#define __ANG_MEMORY_H__

#include <ang/base/base.h>
#include "core_hash_table.h"

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
			: public iraw_allocator
		{
		protected:
#ifdef _MEMORY_PROFILING
			ang_memory_hint_t _allocator_type;
			core::async::mutex_t mutex;
			collections::internal_hash_map<pointer, memory_block_t> memory_map;
#endif
		public:
			allocator_internal(ang_memory_hint_t);
			~allocator_internal();

			pointer memory_alloc(wsize) override;
			void memory_release(pointer) override;
			pointer aligned_memory_alloc(wsize size, wsize)override { return memory_alloc(size); }
#ifdef _DEBUG
			pointer memory_alloc(wsize, const char* file, int line) override;
			pointer aligned_memory_alloc(wsize size, wsize, const char* file, int line)override { return memory_alloc(size, file, line); }
#endif
		};

		class aligned_allocator_internal : public allocator_internal
		{
		public:
			aligned_allocator_internal();
			~aligned_allocator_internal();

			pointer memory_alloc(wsize) override;
			void memory_release(pointer) override;
			pointer aligned_memory_alloc(wsize, wsize) override;
#ifdef _DEBUG
			pointer memory_alloc(wsize, const char* file, int line) override;
			pointer aligned_memory_alloc(wsize, wsize, const char* file, int line) override;
#endif

		};

		aligned_allocator_internal* get_aligned_allocator();
	}
}

#endif//__ANG_MEMORY_H__