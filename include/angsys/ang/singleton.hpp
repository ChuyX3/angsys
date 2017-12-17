/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_SINGLETON_HPP__
#define __ANG_SINGLETON_HPP__
#ifndef __ANGSYS_HPP__
#include <angsys.hpp>
#endif//__ANGSYS_HPP__


namespace ang
{
	
	template<class T>
	class singleton
	{
	private:
		static T* instance_manager(T* ptr, bool write = false, bool del = false);

	public:
		typedef T derived_t;
		static derived_t* instance();
		static bool release_instance();
		static bool is_instanced();

		template<class...Ts>
		static derived_t* instance(Ts... args) {
			if (instance_manager(null, false, false) == null)
				new T(args...);
			return instance_manager(null, false, false);
		}

		singleton();
		virtual~singleton();
	};

	template<class T>
	class singleton<object_wrapper<T>>
	{
	private:
		static T* instance_manager(T* ptr, bool write = false, bool del = false);

	public:
		typedef T derived_t;
		static derived_t* instance();
		static bool release_instance();
		static bool is_instanced();

		template<class...Ts>
		static derived_t* instance(Ts... args) {
			if (instance_manager(null, false, false) == null)
				new T(args...);
			return instance_manager(null, false, false);
		}

		singleton();
		virtual~singleton();
	};

	
}//ang

#include <ang/inline/singleton.inl>

#endif//__ANG_SINGLETON_H__
