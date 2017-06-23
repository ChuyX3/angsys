/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_SINGLETON_H__
#define __ANG_SINGLETON_H__
#ifndef __ANGSYS_H__
#include <angsys.h>
#endif//__ANGSYS_H__


namespace ang
{
	
	template<class T>
	class singleton
	{
	public:
		typedef T derived_t;
		static derived_t* instance();
		static bool release_instance();
		static bool is_instanced();

		template<class...Ts>
		static derived_t* instance(Ts... args) {
			if (_instance == null)
				new T(args...);
			return static_cast<T*>(_instance);
		}
	protected:
		static derived_t* _instance;

		singleton();
		virtual~singleton();
	};

	template<class T>
	class singleton<object_wrapper<T>>
	{
	public:
		typedef T derived_t;
		static derived_t* instance();
		static bool release_instance();
		static bool is_instanced();

		template<class...Ts>
		static derived_t* instance(Ts... args) {
			if (_instance == null)
				new T(args...);
			return static_cast<T*>(_instance);
		}
	protected:
		static derived_t* _instance;

		singleton();
		virtual~singleton();
	};

	
}//ang

#include <ang/inline/singleton.inl>

#endif//__ANG_SINGLETON_H__
