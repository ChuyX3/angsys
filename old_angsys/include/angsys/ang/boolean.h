/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_BOOLEAN_H__
#define __ANG_BOOLEAN_H__

namespace ang
{
	typedef value_wrapper<bool> boolean;

	typedef object_wrapper<boolean> boolean_t;

	template<> class LINK value_wrapper<bool> final
		: public object
		, public value<bool>
	{
	public:
		static value<bool> parse(cstr_t);
		static value<bool> parse(cwstr_t);
		static string to_string(value<bool>);
		static wstring to_wstring(value<bool>);

	public:
		value_wrapper();
		value_wrapper(bool val);
		value_wrapper(boolean const& other);
		value_wrapper(value<bool> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;

	private:
		virtual~value_wrapper();
	};

}//ang

#endif//__ANG_BOOLEAN_H__
