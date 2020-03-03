#ifndef __ANG_MEMORY_H__
#define __ANG_MEMORY_H__

#include <ang/base/base.h>

#if defined _DEBUG || defined _DEVELOPPER
#define _MEMORY_PROFILING
#endif

#ifdef _MEMORY_PROFILING

typedef struct _memory_block memory_block, *memory_block_t;
ANG_EXTERN memory_block_t memory_block_create(ang_size_t, ang_void_ptr_t);

namespace ang
{
	namespace memory
	{
		struct iraw_allocator
		{
			virtual~iraw_allocator() {}
			virtual pointer memory_alloc(wsize size) = 0;
			virtual void memory_release(pointer ptr) = 0;
			virtual pointer aligned_memory_alloc(wsize, wsize) = 0;// only for aligned allocator
#ifdef _DEBUG
			virtual pointer memory_alloc(wsize size, const char* file, int line) = 0;
			virtual pointer aligned_memory_alloc(wsize, wsize, const char* file, int line) = 0;// only for aligned allocator
#endif
		};

		iraw_allocator* get_raw_allocator(ang_memory_hint_t);


		class allocator_internal : public iraw_allocator
		{
		protected:
			ang_memory_hint_t _allocator_type;
			core::async::mutex_t mutex;
			collections::hash_map<pointer, memory_block_t, memory::unmanaged_allocator> memory_map;
		public:
			allocator_internal(ang_memory_hint_t);
			virtual~allocator_internal();

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
			virtual~aligned_allocator_internal();

			pointer memory_alloc(wsize) override;
			void memory_release(pointer) override;
			pointer aligned_memory_alloc(wsize, wsize) override;
#ifdef _DEBUG
			pointer memory_alloc(wsize, const char* file, int line) override;
			pointer aligned_memory_alloc(wsize, wsize, const char* file, int line) override;
#endif

		};
	}
}

#endif//_MEMORY_PROFILING
#endif//__ANG_MEMORY_H__
