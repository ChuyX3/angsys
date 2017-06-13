#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_INT_H__
#define __ANG_INT_H__

namespace ang
{
	template<> struct LINK value<int>
	{
	public: /*type definitions*/
		typedef int type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(value const& other);
		value(value && other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<int>& operator = (type val);
		value<int>& operator = (value<int> const& val);
		value<int>& operator = (value<int> && val);

		value<int>& operator += (type val);
		value<int>& operator += (value<int> const& val);
		value<int>& operator -= (type val);
		value<int>& operator -= (value<int> const& val);
		value<int>& operator *= (type val);
		value<int>& operator *= (value<int> const& val);
		value<int>& operator /= (type val);
		value<int>& operator /= (value<int> const& val);

		value<int>& operator ++();
		value<int> operator ++(int);
		value<int>& operator --();
		value<int> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const int>
	{
	public: /*type definitions*/
		typedef int type;

	private: /*members*/
		const type _value;

	public: /*constructors*/
		value(type val);
		value(value const& other);

	public: /*getters and setters*/
		type const& get()const;

	public: /*operators*/
		inline operator type ()const;
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<int&>
	{
	public: /*type definitions*/
		typedef int type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<int>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<int>& operator = (type val);
		value<int>& operator = (value const& val);
		value<int>& operator = (value<int> val);

		value<int>& operator += (type val);
		value<int>& operator += (value const& val);
		value<int>& operator -= (type val);
		value<int>& operator -= (value const& val);
		value<int>& operator *= (type val);
		value<int>& operator *= (value const& val);
		value<int>& operator /= (type val);
		value<int>& operator /= (value const& val);

		value<int>& operator ++();
		value<int> operator ++(int);
		value<int>& operator --();
		value<int> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const int&>
	{
	public: /*type definitions*/
		typedef int type;

	private: /*members*/
		const type& _value;

	public: /*constructors*/
		value(type const& val);
		value(value const& other);

	public: /*getters and setters*/
		type const& get()const;

	public: /*operators*/
		inline operator type ()const;
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<int*>
	{
	public: /*type definitions*/
		typedef int* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<int>* val);
		value(value const& other);
		value(value && other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value& operator = (type val);
		value& operator = (value const& val);
		value& operator = (value && val);

		inline operator type& ();
		inline operator type ()const;
		inline value<int>& operator * ();
		inline value<int> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	template<> struct LINK value<int const*>
	{
	public: /*type definitions*/
		typedef int const* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<int> const* val);
		value(value const& other);
		value(value && other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value& operator = (type val);
		value& operator = (value const& val);
		value& operator = (value && val);

		inline operator type& ();
		inline operator type ()const;
		inline value<int const> & operator * ();
		inline value<int const> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	inline value<int> operator ""_i (unsigned long long val) { return value<int>(static_cast<int>(val)); }
}

#include<ang/value/inline/int.hpp>

#endif//__ANG_INT_H__
