#pragma once

#include "angsys.h"
#include "ang_inlines.h"

#if defined _DEBUG || defined _DEVELOPPER
#define _MEMORY_PROFILING
#endif

typedef struct _memory_block memory_block, *memory_block_t;
ANG_EXTERN memory_block_t memory_block_create(ang_size_t, ang_void_ptr_t);

namespace ang
{
	namespace memory
	{
		class base_allocator
		{
		public:
			ang_void_ptr_t operator new(ang_size_t);
			void operator delete(ang_void_ptr_t);
		};

		class default_allocator
			: public base_allocator
			, public iallocator
			//, public singleton<default_allocator>	
		{
		private:
			uint _allocator_type;
#ifdef _MEMORY_PROFILING
			ang_core_mutex_ptr_t mutex;
			debug::ilogger_t logger;
			ang_hash_table<pointer, memory_block_t> memory_map;
#endif
		public:
			default_allocator(uint);
			virtual~default_allocator();

			ANG_DECLARE_INTERFACE();

			virtual bool can_delete_from(iallocator*)const override;
			virtual pointer malloc(wsize)override;
			virtual void free(pointer)override;
#ifdef _DEBUG
			virtual pointer malloc(wsize, const char* file, int line)override;

#endif

#ifdef _MEMORY_PROFILING
			virtual void set_logger(debug::ilogger_t)override;
			template<class... Ts>
			void debug_info(cstr_t format, Ts... args) {
				if (!logger.is_empty()) logger->print(debug::log_level::info, format, args...);
			}
			template<class... Ts>
			void debug_warning(cstr_t format, Ts... args) {
				if (!logger.is_empty()) logger->print(debug::log_level::warning, format, args...);
			}
			template<class... Ts>
			void debug_error(cstr_t format, Ts... args) {
				if (!logger.is_empty()) logger->print(debug::log_level::error, format, args...);
			}
#endif
		};

		class aligned_allocator
			: public base_allocator
			, public ialigned_allocator
			//, public singleton<aligned_allocator>
		{
		public:
			aligned_allocator();
			virtual~aligned_allocator();

			ANG_DECLARE_INTERFACE();

			virtual bool can_delete_from(iallocator*)const override;
			virtual pointer malloc(wsize)override;
			virtual void free(pointer)override;
			virtual pointer aligned_malloc(wsize, wsize)override;

#ifdef _DEBUG
			virtual pointer malloc(wsize, const char* file, int line)override;
			virtual pointer aligned_malloc(wsize, wsize, const char* file, int line)override;
#endif

#ifdef _MEMORY_PROFILING
			virtual void set_logger(debug::ilogger_t)override;
#endif

		};
	}
}