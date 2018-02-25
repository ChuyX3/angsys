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
		struct iraw_allocator
		{
			virtual pointer memory_alloc(wsize size) = 0;
			virtual void memory_release(pointer ptr) = 0;
#ifdef _DEBUG
			virtual pointer memory_alloc(wsize size, const char* file, int line) = 0;
#endif
		};

		class allocator_internal 
			: public iraw_allocator
		{
		protected:
#ifdef _MEMORY_PROFILING
			ang_memory_hint_t _allocator_type;
			core::async::mutex_t mutex;
			collections::hash_table<pointer, memory_block_t> memory_map;
#endif
		public:
			allocator_internal(ang_memory_hint_t);
			~allocator_internal();

			pointer memory_alloc(wsize);
			void memory_release(pointer);
#ifdef _DEBUG
			pointer memory_alloc(wsize, const char* file, int line);
#endif
		};

		class aligned_allocator_internal : public allocator_internal
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

		aligned_allocator_internal* get_aligned_allocator();
		iraw_allocator* get_raw_allocator(ang_memory_hint_t hint);
	}
}

#endif//__ANG_MEMORY_H__