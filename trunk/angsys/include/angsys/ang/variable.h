#ifndef __ANGSYS_H__
#error ...
#elif !defined __VARIABLE_H__
#define __VARIABLE_H__
namespace ang
{
	template<typename T>
	class variable
		: public object
		, public value<T>
	{

	};
}

#endif//__SAMRT_PTR_H__