/*********************************************************************************************************************/
/*   File Name: ang/singleton.h                                                                                      */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#elif !defined __ANG_SINGLETON_H__
#define __ANG_SINGLETON_H__

namespace ang
{
	template<typename T, template<typename> class A = memory::default_allocator>
	class singleton
	{
	private:
		static T* instance_manager(T* ptr, bool write = false, bool del = false) {
			static T* s_instance = null;
			
			if (write) {
				if (s_instance && del) {
					A<T>::template destroy<T>(s_instance);
					A<T>::deallocate(s_instance);
				}
				s_instance = ptr;
			}
			return s_instance;
		}

	public:
		static T* instance()	{
			if (!is_instanced()) {
				auto ptr = A<T>::allocate(1);
				A<T>::template construct<T>(ptr);
			}
				
			return instance_manager(null, false);
		}
		static bool release_instance() {
			instance_manager(null, true, true);
			return true;
		}
		static bool is_instanced() {
			return (instance_manager(null, false) != null);
		}

		singleton() {
			//if (is_instanced())
			//	throw exception_t(error_code::two_singleton);
			instance_manager(static_cast<T*>(this), true, false);
		}

		virtual~singleton() {
			if (instance_manager(null, false) == this)
				instance_manager(null, true, false);
		}
	};

	template<typename T>
	class singleton<object_wrapper<T>>
	{
	private:
		static object_wrapper<T> instance_manager(T* ptr, bool write = false) {
			static object_wrapper<T> s_instance = null;
			if (write) {
				s_instance = ptr;
			}
			return s_instance;
		}

	public:
		static object_wrapper<T> instance() {
			if (!is_instanced()) {
				new T();
			}
			return instance_manager(null, false);
		}
		template<typename... Ts>
		static object_wrapper<T> instance(Ts... args) {
			if (!is_instanced()) {
				new T(ang::forward<Ts>(args)...);
			}
			return instance_manager(null, false);
		}
		static bool release_instance() {
			instance_manager(null, true);
			return true;
		}
		static bool is_instanced() {
			return !instance_manager(null, false).is_empty();
		}

		singleton() {
			instance_manager(static_cast<T*>(this), true);
		}

		virtual~singleton() {
			instance_manager(null, true);
		}
	};
}

#endif//__ANG_SINGLETON_H__