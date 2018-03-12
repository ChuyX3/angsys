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


ANG_EXTERN LINK ang_void_ptr_t ang_alloc_unmanaged_memory(ang_size_t);
ANG_EXTERN LINK void ang_free_unmanaged_memory(ang_void_ptr_t);

typedef enum _ang_memory_hint {
	ang_default_memory,
	ang_object_memory,
	ang_buffer_memory,
	ang_aligned_memory,
}ang_memory_hint, ang_memory_hint_t;

ANG_EXTERN LINK ang_void_ptr_t ang_alloc_managed_memory(ang_size_t, ang_memory_hint_t);
ANG_EXTERN LINK void ang_free_managed_memory(ang_void_ptr_t, ang_memory_hint_t);

ANG_EXTERN LINK ang_void_ptr_t ang_alloc_aligned_memory(ang_size_t, wsize ALIGMENT);
ANG_EXTERN LINK void ang_free_aligned_memory(ang_void_ptr_t);

#ifdef _DEBUG
ANG_EXTERN LINK ang_void_ptr_t ang_alloc_unmanaged_memory_debug(ang_size_t, const char* file, int line);
ANG_EXTERN LINK ang_void_ptr_t ang_alloc_managed_memory_debug(ang_size_t, ang_memory_hint_t, const char* file, int line);
ANG_EXTERN LINK ang_void_ptr_t ang_alloc_aligned_memory_debug(ang_size_t, wsize ALIGMENT, const char* file, int line);
#endif


namespace ang
{
	namespace memory
	{
		template<typename T, ang_memory_hint_t HINT_>
		class managed_allocator
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

			pointer address(reference _Val) const noexcept
			{	// return address of mutable _Val
				return (ang::addressof(_Val));
			}

			const_pointer address(const_reference _Val) const noexcept
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

			void deallocate(pointer _Ptr, size_type = 0)
			{	// deallocate object at _Ptr
				ang_free_managed_memory(_Ptr, HINT);
			}

			_DECLSPEC_ALLOCATOR pointer allocate(size_type _Count)
			{	// allocate array of _Count elements
				return (pointer)ang_alloc_managed_memory(_Count * sizeof(T), HINT);
			}

			_DECLSPEC_ALLOCATOR pointer allocate(size_type _Count, const void *)
			{	// allocate array of _Count elements, ignore hint
				return (allocate(_Count));
			}

#ifdef _DEBUG
			_DECLSPEC_ALLOCATOR pointer allocate(size_type _Count, const char* _File, int _Line)
			{	// allocate array of _Count elements
				return (pointer)ang_alloc_managed_memory_debug(_Count * sizeof(T), HINT, _File, _Line);
			}
#endif

			template<typename U, typename... Ts>
			void construct(U *_Ptr, Ts&&... _Args)
			{	// construct U(Ts...) at _Ptr
				::new ((void *)_Ptr) U(ang::forward<Ts>(_Args)...);
			}


			template<typename U>
			void destroy(U *_Ptr)
			{	// destroy object at _Ptr
				_Ptr->~U();
			}

			wsize max_size() const noexcept
			{	// estimate maximum array size
				return ((wsize)(-1) / sizeof(T));
			}
		};


		template<typename T, wsize ALIGNMENT_>
		class aligned_allocator
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

			pointer address(reference _Val) const noexcept
			{	// return address of mutable _Val
				return (ang::addressof(_Val));
			}

			const_pointer address(const_reference _Val) const noexcept
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

			void deallocate(pointer _Ptr, size_type = 0)
			{	// deallocate object at _Ptr
				ang_free_aligned_memory(_Ptr);
			}

			_DECLSPEC_ALLOCATOR pointer allocate(size_type _Count)
			{	// allocate array of _Count elements
				return (pointer)ang_alloc_aligned_memory(_Count * sizeof(T), ALIGNMENT);
			}

			_DECLSPEC_ALLOCATOR pointer allocate(size_type _Count, const void *)
			{	// allocate array of _Count elements, ignore hint
				return (allocate(_Count));
			}

			template<typename U, typename... Ts>
			void construct(U *_Ptr, Ts&&... _Args)
			{	// construct U(Ts...) at _Ptr
				::new ((void *)_Ptr) U(ang::forward<Ts>(_Args)...);
			}


			template<typename U>
			void destroy(U *_Ptr)
			{	// destroy object at _Ptr
				_Ptr->~U();
			}

			wsize max_size() const noexcept
			{	// estimate maximum array size
				return ((wsize)(-1) / sizeof(T));
			}
		};

		template<typename T>
		class unmanaged_allocator
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

			pointer address(reference _Val) const noexcept
			{	// return address of mutable _Val
				return (ang::addressof(_Val));
			}

			const_pointer address(const_reference _Val) const noexcept
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

			void deallocate(pointer _Ptr, size_type = 0)
			{	// deallocate object at _Ptr
				ang_free_unmanaged_memory(_Ptr);
			}

			_DECLSPEC_ALLOCATOR pointer allocate(size_type _Count)
			{	// allocate array of _Count elements
				return (pointer)ang_alloc_unmanaged_memory(_Count * sizeof(T));
			}

			_DECLSPEC_ALLOCATOR pointer allocate(size_type _Count, const void *)
			{	// allocate array of _Count elements, ignore hint
				return (allocate(_Count));
			}

#ifdef _DEBUG
			_DECLSPEC_ALLOCATOR pointer allocate(size_type _Count, const char*, int)
			{	// allocate array of _Count elements
				return ang_alloc_unmanaged_memory(_Count * sizeof(T));
			}
#endif

			template<typename U, typename... Ts>
			void construct(U *_Ptr, Ts&&... _Args)
			{	// construct U(Ts...) at _Ptr
				::new ((void *)_Ptr) U(ang::forward<Ts>(_Args)...);
			}


			template<typename U>
			void destroy(U *_Ptr)
			{	// destroy object at _Ptr
				_Ptr->~U();
			}

			wsize max_size() const noexcept
			{	// estimate maximum array size
				return ((wsize)(-1) / sizeof(T));
			}
		};

		template<typename T> using default_allocator = managed_allocator<T, ang_memory_hint::ang_default_memory>;
		template<typename T> using object_allocator = managed_allocator<T, ang_memory_hint::ang_object_memory>;
		template<typename T> using buffer_allocator = managed_allocator<T, ang_memory_hint::ang_buffer_memory>;
	}
}

#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
inline pointer operator new(wsize, pointer ptr) throw() { return ptr; }
inline void operator delete(pointer, pointer) throw() { return; }
#endif

#endif//__ANG_BASE_MEMORY_H__
