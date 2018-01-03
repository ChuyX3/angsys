#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_BASE_ALLOCATOR_HPP__
#define __ANG_BASE_ALLOCATOR_HPP__

namespace ang
{
	namespace memory
	{

		template<typename T, ang_memory_hint_t HINT>
		struct managed_allocator
		{
			static T* alloc(wsize count) {
				return reinterpret_cast<T*>(ang_alloc_managed_memory(count * sizeof(T), HINT));
			}
#ifdef _DEBUG
			static T* alloc(wsize count, const char* file, int line) {
				return reinterpret_cast<T*>(ang_alloc_managed_memory_debug(count * sizeof(T), HINT, file, line));
			}
#endif // _DEBUG

			static void free(T* ptr) {
				ang_free_managed_memory(ptr, HINT);
			}

			static void destruct_and_free(T* ptr) {
				ptr->~T();
				ang_free_managed_memory(ptr, HINT);
			}

			template<typename... args_t>
			static T* construct(T* obj, args_t... args) {
				return new(obj) T(ang::forward<args_t>(args)...);
			}

			template<typename... args_t>
			static T* alloc_and_construct(args_t... args) {
				return construct<args_t...>(reinterpret_cast<T*>(ang_alloc_managed_memory(sizeof(T), HINT)), ang::forward<args_t>(args)...);
			}

			static void destruct(T* obj) {
				obj->~T();
			}
		};


		template<ang_memory_hint_t HINT>
		struct managed_allocator<void, HINT >
		{
			static void* alloc(wsize sz) {
				return ang_alloc_managed_memory(sz, HINT);
			}
#ifdef _DEBUG
			static void* alloc(wsize sz, const char* file, int line) {
				return ang_alloc_managed_memory_debug(sz, HINT, file, line);
			}
#endif // _DEBUG

			static void free(void* ptr) {
				ang_free_managed_memory(ptr, HINT);
			}
		};


		template<typename T, wsize ALIGNMENT>
		struct aligned_allocator
		{
			static T* alloc(wsize count) {
				return reinterpret_cast<T*>(ang_alloc_aligned_memory(count * sizeof(T), ALIGNMENT));
			}
#ifdef _DEBUG
			static T* alloc(wsize count, const char* file, int line) {
				return reinterpret_cast<T*>(ang_alloc_aligned_memory_debug(count * sizeof(T), ALIGNMENT, file, line));
			}
#endif // _DEBUG

			static void free(T* ptr) {
				ang_free_aligned_memory(ptr);
			}

			static void destruct_and_free(T* ptr) {
				ptr->~T();
				ang_free_aligned_memory(ptr);
			}

			template<typename... args_t>
			static T* construct(T* obj, args_t... args) {
				return new(obj) T(args...);
			}

			template<typename... args_t>
			static T* alloc_and_construct(args_t... args) {
				return construct<args_t...>(reinterpret_cast<T*>(ang_alloc_aligned_memory(sizeof(T), ALIGNMENT)), args...);
			}

			static void destruct(T* obj) {
				obj->~T();
			}
		};

		template<typename T>
		struct unmanaged_allocator
		{
			static T* alloc(wsize count) {
				return reinterpret_cast<T*>(ang_alloc_unmanaged_memory(count * sizeof(T)));
			}
#ifdef _DEBUG
			static T* alloc(wsize count, const char* file, int line) {
				return reinterpret_cast<T*>(ang_alloc_unmanaged_memory_debug(count * sizeof(T), file, line));
			}
#endif // _DEBUG

			static void free(T* ptr) {
				ang_free_unmanaged_memory(ptr);
			}

			static void destruct_and_free(T* ptr) {
				ptr->~T();
				ang_free_unmanaged_memory(ptr);
			}

			template<typename... args_t>
			static T* construct(T* obj, args_t... args) {
				return new(obj) T(args...);
			}

			template<typename... args_t>
			static T* alloc_and_construct(args_t... args) {
				return construct<args_t...>(reinterpret_cast<T*>(ang_alloc_unmanaged_memory(sizeof(T))), args...);
			}

			static void destruct(T* obj) {
				obj->~T();
			}
		};
	}
}

#endif//__ANG_BASE_ALLOCATOR_HPP__