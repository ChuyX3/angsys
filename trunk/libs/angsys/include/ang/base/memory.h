/*********************************************************************************************************************/
/*   File Name: ang/base/memory.h                                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_MEMORY_H__
#define __ANG_BASE_MEMORY_H__


namespace ang
{
	namespace memory
	{
		template<typename T> struct unmanaged_allocator;
		template<typename T, ang_memory_hint_t HINT_> struct managed_allocator;
		template<typename T, wsize ALIGNMENT_> struct aligned_allocator;

		template<typename T> using default_allocator = managed_allocator<T, ang_memory_hint::ang_default_memory>;
		template<typename T> using object_allocator = managed_allocator<T, ang_memory_hint::ang_object_memory>;
		template<typename T> using buffer_allocator = managed_allocator<T, ang_memory_hint::ang_buffer_memory>;
		template<typename T> using aligned16_allocator = aligned_allocator<T, 16>;

		template<typename T, ang_memory_hint_t HINT = ang_default_memory, bool ALIGMENT = (align_of<T>() > 4U)>
		struct auto_detect_allocator {
			static constexpr ang_memory_hint_t value = HINT;
			using type = managed_allocator<T, HINT>;
			template<typename U> using rebind = managed_allocator<U, HINT>;
		};

		template<typename T, ang_memory_hint_t HINT>
		struct auto_detect_allocator<T, HINT, true> {
			static constexpr ang_memory_hint_t value = ang_aligned_memory;
			using type = aligned_allocator<T, align_of<T>()>;
			template<typename U> using rebind = aligned_allocator<U, align_of<T>()>;
		};

		template<typename T> using auto_detect_default_allocator = typename auto_detect_allocator<T, ang_default_memory>::type;
		template<typename T> using auto_detect_buffer_allocator = typename auto_detect_allocator<T, ang_buffer_memory>::type;

		template<typename T, ang_memory_hint_t HINT_>
		struct managed_allocator
		{
		public:
			static const ang_memory_hint_t HINT = HINT_;
			typedef void _Not_user_specialized;

			typedef T value_type;

			typedef value_type *pointer;
			typedef const value_type *const_pointer;

			typedef value_type& reference;
			typedef const value_type& const_reference;

			typedef wsize size_type;
			typedef ptrdiff_t difference_type;

			typedef ang::true_type propagate_on_container_move_assignment;
			typedef ang::true_type is_always_equal;

			template<typename _Other>
			struct rebind
			{	// convert this type to allocator<_Other>
				typedef managed_allocator<_Other, HINT> other;
			};

			static pointer address(reference _Val) noexcept
			{	// return address of mutable _Val
				return (ang::addressof(_Val));
			}

			static const_pointer address(const_reference _Val) noexcept
			{	// return address of nonmutable _Val
				return (ang::addressof(_Val));
			}

			managed_allocator() noexcept
			{	// construct default allocator (do nothing)
			}

			managed_allocator(const managed_allocator<T, HINT>&) noexcept
			{	// construct by copying (do nothing)
			}

			template<typename _Other>
			managed_allocator(const managed_allocator<_Other, HINT>&) noexcept
			{	// construct from a related allocator (do nothing)
			}

			template<typename _Other>
			managed_allocator<T, HINT>& operator=(const managed_allocator<_Other, HINT>&)
			{	// assign from a related allocator (do nothing)
				return (*this);
			}

			static void deallocate(pointer _Ptr, size_type = 0)
			{	// deallocate object at _Ptr
				ang_free_managed_memory(_Ptr, HINT);
			}

			static _DECLSPEC_ALLOCATOR pointer allocate(size_type _Count)
			{	// allocate array of _Count elements
				return (pointer)ang_alloc_managed_memory(_Count * sizeof(T), HINT);
			}

			static _DECLSPEC_ALLOCATOR pointer allocate(size_type _Count, const void *)
			{	// allocate array of _Count elements, ignore hint
				return (allocate(_Count));
			}

#ifdef _DEBUG
			static _DECLSPEC_ALLOCATOR pointer allocate(size_type _Count, const char* _File, int _Line)
			{	// allocate array of _Count elements
				return (pointer)ang_alloc_managed_memory_debug(_Count * sizeof(T), HINT, _File, _Line);
			}
#endif

			template<typename U, typename... Ts>
			static U * construct(U *_Ptr, Ts&&... _Args)
			{	// construct U(Ts...) at _Ptr
				::new ((void *)_Ptr) U(ang::forward<Ts>(_Args)...);
				return _Ptr;
			}


			template<typename U>
			static void destroy(U *_Ptr)
			{	// destroy object at _Ptr
				_Ptr->~U();
			}

			static wsize max_size() noexcept
			{	// estimate maximum array size
				return ((wsize)(-1) / sizeof(T));
			}
		};

		template<typename T, wsize ALIGNMENT_>
		struct aligned_allocator
		{
		public:
			static const wsize ALIGNMENT = ALIGNMENT_;
			typedef void _Not_user_specialized;

			typedef T value_type;

			typedef value_type *pointer;
			typedef const value_type *const_pointer;

			typedef value_type& reference;
			typedef const value_type& const_reference;

			typedef wsize size_type;
			typedef ptrdiff_t difference_type;

			typedef ang::true_type propagate_on_container_move_assignment;
			typedef ang::true_type is_always_equal;

			template<typename _Other>
			struct rebind
			{	// convert this type to allocator<_Other>
				typedef aligned_allocator<_Other, ALIGNMENT> other;
			};

			static pointer address(reference _Val) noexcept
			{	// return address of mutable _Val
				return (ang::addressof(_Val));
			}

			static const_pointer address(const_reference _Val) noexcept
			{	// return address of nonmutable _Val
				return (ang::addressof(_Val));
			}

			aligned_allocator() noexcept
			{	// construct default allocator (do nothing)
			}

			aligned_allocator(const aligned_allocator<T, ALIGNMENT>&) noexcept
			{	// construct by copying (do nothing)
			}

			template<typename _Other>
			aligned_allocator(const aligned_allocator<_Other, ALIGNMENT>&) noexcept
			{	// construct from a related allocator (do nothing)
			}

			template<typename _Other>
			aligned_allocator<T, ALIGNMENT>& operator=(const aligned_allocator<_Other, ALIGNMENT>&)
			{	// assign from a related allocator (do nothing)
				return (*this);
			}

			static void deallocate(pointer _Ptr, size_type = 0)
			{	// deallocate object at _Ptr
				ang_free_aligned_memory(_Ptr);
			}

			static _DECLSPEC_ALLOCATOR pointer allocate(size_type _Count)
			{	// allocate array of _Count elements
				return (pointer)ang_alloc_aligned_memory(_Count * sizeof(T), ALIGNMENT);
			}

			static _DECLSPEC_ALLOCATOR pointer allocate(size_type _Count, const void *)
			{	// allocate array of _Count elements, ignore hint
				return (allocate(_Count));
			}

			template<typename U, typename... Ts>
			static U * construct(U *_Ptr, Ts&&... _Args)
			{	// construct U(Ts...) at _Ptr
				::new ((void *)_Ptr) U(ang::forward<Ts>(_Args)...);
				return _Ptr;
			}


			template<typename U>
			static void destroy(U *_Ptr)
			{	// destroy object at _Ptr
				_Ptr->~U();
			}

			static wsize max_size() noexcept
			{	// estimate maximum array size
				return ((wsize)(-1) / sizeof(T));
			}
		};

		template<typename T>
		struct unmanaged_allocator
		{
		public:
			typedef void _Not_user_specialized;

			typedef T value_type;

			typedef value_type *pointer;
			typedef const value_type *const_pointer;

			typedef value_type& reference;
			typedef const value_type& const_reference;

			typedef wsize size_type;
			typedef ptrdiff_t difference_type;

			typedef ang::true_type propagate_on_container_move_assignment;
			typedef ang::true_type is_always_equal;

			template<typename _Other>
			struct rebind
			{	// convert this type to allocator<_Other>
				typedef unmanaged_allocator<_Other> other;
			};

			static 	pointer address(reference _Val) noexcept
			{	// return address of mutable _Val
				return (ang::addressof(_Val));
			}

			static const_pointer address(const_reference _Val) noexcept
			{	// return address of nonmutable _Val
				return (ang::addressof(_Val));
			}

			unmanaged_allocator() noexcept
			{	// construct default allocator (do nothing)
			}

			unmanaged_allocator(const unmanaged_allocator<T>&) noexcept
			{	// construct by copying (do nothing)
			}

			template<typename _Other>
			unmanaged_allocator(const unmanaged_allocator<_Other>&) noexcept
			{	// construct from a related allocator (do nothing)
			}

			template<typename _Other>
			unmanaged_allocator<T>& operator=(const unmanaged_allocator<_Other>&)
			{	// assign from a related allocator (do nothing)
				return (*this);
			}

			static void deallocate(pointer _Ptr, size_type = 0)
			{	// deallocate object at _Ptr
				ang_free_unmanaged_memory(_Ptr);
			}

			static _DECLSPEC_ALLOCATOR pointer allocate(size_type _Count)
			{	// allocate array of _Count elements
				return (pointer)ang_alloc_unmanaged_memory(_Count * sizeof(T));
			}

			static _DECLSPEC_ALLOCATOR pointer allocate(size_type _Count, const void *)
			{	// allocate array of _Count elements, ignore hint
				return (allocate(_Count));
			}

#ifdef _DEBUG
			static _DECLSPEC_ALLOCATOR pointer allocate(size_type _Count, const char*, int)
			{	// allocate array of _Count elements
				return ang_alloc_unmanaged_memory(_Count * sizeof(T));
			}
#endif

			template<typename U, typename... Ts>
			static U * construct(U *_Ptr, Ts&&... _Args)
			{	// construct U(Ts...) at _Ptr
				::new ((void *)_Ptr) U(ang::forward<Ts>(_Args)...);
				return _Ptr;
			}


			template<typename U>
			static void destroy(U *_Ptr)
			{	// destroy object at _Ptr
				_Ptr->~U();
			}

			static wsize max_size() noexcept
			{	// estimate maximum array size
				return ((wsize)(-1) / sizeof(T));
			}
		};

	}
}

#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
inline pointer operator new(wsize, pointer ptr) throw() { return ptr; }
inline void operator delete(pointer, pointer) throw() { return; }
#endif

#endif//__ANG_BASE_MEMORY_H__
