/*********************************************************************************************************************/
/*   File Name: ang/base/property.h                                                                                     */
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
#elif !defined __ANG_BASE_PROPERTY_H__
#define __ANG_BASE_PROPERTY_H__

namespace ang //constants
{
	typedef enum class property_style : uint 
	{
		def,
		alertable,
		no_storage
	}property_style_t;


	template<typename T, class owner, property_style TYPE = property_style::def> class property;

	template<typename T, class owner, property_style TYPE>
	class property {
	public:
		property(T val) : m_value(val) {}
		const T& get()const { return m_value; }
		void set(const T& val) { m_value = val; }
		operator T &() { return m_value; }
		operator T const&()const { return m_value; }
		property& operator = (property const& val) { set(val.get()); return*this; }
		property& operator = (T const& val) { set(val); return*this; }
		T* operator ->()const { return const_cast<T*>(&m_value); }

	protected:
		typename ang::remove_constant<T>::type m_value;
		friend owner;
	};

	template<typename T, class owner, property_style TYPE>
	class property<const T, owner, TYPE> {
	public:
		property(T val) : m_value(val) {}
		const T& get()const { return m_value; }
		operator T const&()const { return m_value; }
		T* operator ->()const { return const_cast<T*>(&m_value); }

	protected:
		void set(const T& val) { m_value = val; }
		property& operator = (property const& val) { set(val.get()); return*this; }
		property& operator = (T const& val) { set(val); return*this; }

	private:
		typename ang::remove_constant<T>::type m_value;
		friend owner;
	};


	template<typename T, class owner, property_style TYPE>
	class property<T*, owner, TYPE> {
	public:
		property(T val) : m_value(val) {}
		const T& get()const { return m_value; }
		void set(const T& val) { m_value = val; }
		operator T &() { return m_value; }
		operator T const&()const { return m_value; }
		property& operator = (property const& val) { set(val.get()); return*this; }
		property& operator = (T const& val) { set(val); return*this; }
		T* operator ->()const { return const_cast<T*>(&m_value); }

	protected:
		typename ang::remove_constant<T>::type m_value;
		friend owner;
	};

}

#endif//__ANG_BASE_PROPERTY_H__
