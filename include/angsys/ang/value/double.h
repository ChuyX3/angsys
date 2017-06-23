#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_DOUBLE_H__
#define __ANG_DOUBLE_H__

namespace ang
{
	template<> struct LINK value<double>
	{
	public: /*type definitions*/
		typedef double type;

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
		value<double>& operator = (type val);
		value<double>& operator = (value<double> const& val);
		value<double>& operator = (value<double> && val);

		value<double>& operator += (type val);
		value<double>& operator += (value<double> const& val);
		value<double>& operator -= (type val);
		value<double>& operator -= (value<double> const& val);
		value<double>& operator *= (type val);
		value<double>& operator *= (value<double> const& val);
		value<double>& operator /= (type val);
		value<double>& operator /= (value<double> const& val);

		value<double>& operator ++();
		value<double> operator ++(int);
		value<double>& operator --();
		value<double> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const double>
	{
	public: /*type definitions*/
		typedef double type;

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

	template<> struct LINK value<double&>
	{
	public: /*type definitions*/
		typedef double type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<double>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<double>& operator = (type val);
		value<double>& operator = (value const& val);
		value<double>& operator = (value<double> val);

		value<double>& operator += (type val);
		value<double>& operator += (value const& val);
		value<double>& operator -= (type val);
		value<double>& operator -= (value const& val);
		value<double>& operator *= (type val);
		value<double>& operator *= (value const& val);
		value<double>& operator /= (type val);
		value<double>& operator /= (value const& val);

		value<double>& operator ++();
		value<double> operator ++(int);
		value<double>& operator --();
		value<double> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const double&>
	{
	public: /*type definitions*/
		typedef double type;

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

	template<> struct LINK value<double*>
	{
	public: /*type definitions*/
		typedef double* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<double>* val);
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
		inline value<double>& operator * ();
		inline value<double> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	template<> struct LINK value<double const*>
	{
	public: /*type definitions*/
		typedef double const* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<double> const* val);
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
		inline value<double const> & operator * ();
		inline value<double const> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	inline value<double> operator ""_lf(long double val) { return value<double>(static_cast<double>(val)); }
	inline value<double> operator ""_lf(unsigned long long val) { return value<double>(static_cast<double>(val)); }
}

#include <ang/value/inline/double.inl>

#endif//__ANG_DOUBLE_H__
