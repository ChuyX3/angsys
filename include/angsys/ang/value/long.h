#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_LONG_H__
#define __ANG_LONG_H__

namespace ang
{
	template<> struct LINK value<long>
	{
	public: /*type definitions*/
		typedef long type;

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
		value<long>& operator = (type val);
		value<long>& operator = (value<long> const& val);
		value<long>& operator = (value<long> && val);

		value<long>& operator += (type val);
		value<long>& operator += (value<long> const& val);
		value<long>& operator -= (type val);
		value<long>& operator -= (value<long> const& val);
		value<long>& operator *= (type val);
		value<long>& operator *= (value<long> const& val);
		value<long>& operator /= (type val);
		value<long>& operator /= (value<long> const& val);

		value<long>& operator ++();
		value<long> operator ++(int);
		value<long>& operator --();
		value<long> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const long>
	{
	public: /*type definitions*/
		typedef long type;

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

	template<> struct LINK value<long&>
	{
	public: /*type definitions*/
		typedef long type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<long>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<long>& operator = (type val);
		value<long>& operator = (value const& val);
		value<long>& operator = (value<long> val);

		value<long>& operator += (type val);
		value<long>& operator += (value const& val);
		value<long>& operator -= (type val);
		value<long>& operator -= (value const& val);
		value<long>& operator *= (type val);
		value<long>& operator *= (value const& val);
		value<long>& operator /= (type val);
		value<long>& operator /= (value const& val);

		value<long>& operator ++();
		value<long> operator ++(int);
		value<long>& operator --();
		value<long> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const long&>
	{
	public: /*type definitions*/
		typedef long type;

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

	template<> struct LINK value<long*>
	{
	public: /*type definitions*/
		typedef long* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<long>* val);
		value(value const& other);
		value(value && other);
		template<typename T2> value(value<T2*> const& other);

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
		inline value<long>& operator * ();
		inline value<long> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	template<> struct LINK value<long const*>
	{
	public: /*type definitions*/
		typedef long const* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<long> const* val);
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
		inline value<long const> & operator * ();
		inline value<long const> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};



	template<> struct LINK value<long64>
	{
	public: /*type definitions*/
		typedef long64 type;

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
		value<long64>& operator = (type val);
		value<long64>& operator = (value<long64> const& val);
		value<long64>& operator = (value<long64> && val);

		value<long64>& operator += (type val);
		value<long64>& operator += (value<long64> const& val);
		value<long64>& operator -= (type val);
		value<long64>& operator -= (value<long64> const& val);
		value<long64>& operator *= (type val);
		value<long64>& operator *= (value<long64> const& val);
		value<long64>& operator /= (type val);
		value<long64>& operator /= (value<long64> const& val);

		value<long64>& operator ++();
		value<long64> operator ++(int);
		value<long64>& operator --();
		value<long64> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const long64>
	{
	public: /*type definitions*/
		typedef long64 type;

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

	template<> struct LINK value<long64&>
	{
	public: /*type definitions*/
		typedef long64 type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<long64>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<long64>& operator = (type val);
		value<long64>& operator = (value const& val);
		value<long64>& operator = (value<long64> val);

		value<long64>& operator += (type val);
		value<long64>& operator += (value const& val);
		value<long64>& operator -= (type val);
		value<long64>& operator -= (value const& val);
		value<long64>& operator *= (type val);
		value<long64>& operator *= (value const& val);
		value<long64>& operator /= (type val);
		value<long64>& operator /= (value const& val);

		value<long64>& operator ++();
		value<long64> operator ++(int);
		value<long64>& operator --();
		value<long64> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const long64&>
	{
	public: /*type definitions*/
		typedef long64 type;

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

	template<> struct LINK value<long64*>
	{
	public: /*type definitions*/
		typedef long64* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<long64>* val);
		value(value const& other);
		value(value && other);
		template<typename T2> value(value<T2*> const& other);

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
		inline value<long64>& operator * ();
		inline value<long64> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	template<> struct LINK value<long64 const*>
	{
	public: /*type definitions*/
		typedef long64 const* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<long64> const* val);
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
		inline value<long64 const> & operator * ();
		inline value<long64 const> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};
}

#include<ang/value/inline/long.hpp>

#endif//__ANG_LONG_H__
