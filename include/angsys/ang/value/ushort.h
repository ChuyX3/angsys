#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_USHORT_H__
#define __ANG_USHORT_H__

namespace ang
{
	template<> struct LINK value<ushort>
	{
	public: /*type definitions*/
		typedef ushort type;

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
		value<ushort>& operator = (type val);
		value<ushort>& operator = (value<ushort> const& val);
		value<ushort>& operator = (value<ushort> && val);

		value<ushort>& operator += (type val);
		value<ushort>& operator += (value<ushort> const& val);
		value<ushort>& operator -= (type val);
		value<ushort>& operator -= (value<ushort> const& val);
		value<ushort>& operator *= (type val);
		value<ushort>& operator *= (value<ushort> const& val);
		value<ushort>& operator /= (type val);
		value<ushort>& operator /= (value<ushort> const& val);

		value<ushort>& operator ++();
		value<ushort> operator ++(int);
		value<ushort>& operator --();
		value<ushort> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const ushort>
	{
	public: /*type definitions*/
		typedef ushort type;

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

	template<> struct LINK value<ushort&>
	{
	public: /*type definitions*/
		typedef ushort type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<ushort>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<ushort>& operator = (type val);
		value<ushort>& operator = (value const& val);
		value<ushort>& operator = (value<ushort> val);

		value<ushort>& operator += (type val);
		value<ushort>& operator += (value const& val);
		value<ushort>& operator -= (type val);
		value<ushort>& operator -= (value const& val);
		value<ushort>& operator *= (type val);
		value<ushort>& operator *= (value const& val);
		value<ushort>& operator /= (type val);
		value<ushort>& operator /= (value const& val);

		value<ushort>& operator ++();
		value<ushort> operator ++(int);
		value<ushort>& operator --();
		value<ushort> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const ushort&>
	{
	public: /*type definitions*/
		typedef ushort type;

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

	template<> struct LINK value<ushort*>
	{
	public: /*type definitions*/
		typedef ushort* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<ushort>* val);
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
		inline value<ushort>& operator * ();
		inline value<ushort> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	template<> struct LINK value<ushort const*>
	{
	public: /*type definitions*/
		typedef ushort const* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<ushort> const* val);
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
		inline value<ushort const> & operator * ();
		inline value<ushort const> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	inline value<ushort> operator ""_us(unsigned long long val) { return value<ushort>(static_cast<ushort>(val)); }
	inline value<ushort> operator ""_usi(unsigned long long val) { return value<ushort>(static_cast<ushort>(val)); }
}

#include<ang/value/inline/ushort.hpp>

#endif//__ANG_USHORT_H__

