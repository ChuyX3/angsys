#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_SHORT_H__
#define __ANG_SHORT_H__

namespace ang
{
	template<> struct LINK value<short>
	{
	public: /*type definitions*/
		typedef short type;

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
		value<short>& operator = (type val);
		value<short>& operator = (value<short> const& val);
		value<short>& operator = (value<short> && val);

		value<short>& operator += (type val);
		value<short>& operator += (value<short> const& val);
		value<short>& operator -= (type val);
		value<short>& operator -= (value<short> const& val);
		value<short>& operator *= (type val);
		value<short>& operator *= (value<short> const& val);
		value<short>& operator /= (type val);
		value<short>& operator /= (value<short> const& val);

		value<short>& operator ++();
		value<short> operator ++(int);
		value<short>& operator --();
		value<short> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const short>
	{
	public: /*type definitions*/
		typedef short type;

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

	template<> struct LINK value<short&>
	{
	public: /*type definitions*/
		typedef short type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<short>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<short>& operator = (type val);
		value<short>& operator = (value const& val);
		value<short>& operator = (value<short> val);

		value<short>& operator += (type val);
		value<short>& operator += (value const& val);
		value<short>& operator -= (type val);
		value<short>& operator -= (value const& val);
		value<short>& operator *= (type val);
		value<short>& operator *= (value const& val);
		value<short>& operator /= (type val);
		value<short>& operator /= (value const& val);

		value<short>& operator ++();
		value<short> operator ++(int);
		value<short>& operator --();
		value<short> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const short&>
	{
	public: /*type definitions*/
		typedef short type;

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

	template<> struct LINK value<short*>
	{
	public: /*type definitions*/
		typedef short* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(std::nullptr_t);
		value(value<short>* val);
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
		inline value<short>& operator * ();
		inline value<short> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	template<> struct LINK value<short const*>
	{
	public: /*type definitions*/
		typedef short const* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(std::nullptr_t);
		value(value<short> const* val);
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
		inline value<short const> & operator * ();
		inline value<short const> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};
}

#include<ang/value/inline/short.hpp>

#endif//__ANG_SHORT_H__

