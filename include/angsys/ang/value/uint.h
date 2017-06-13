#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_UINT_H__
#define __ANG_UINT_H__

namespace ang
{
	template<> struct LINK value<uint>
	{
	public: /*type definitions*/
		typedef uint type;

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
		value<uint>& operator = (type val);
		value<uint>& operator = (value<uint> const& val);
		value<uint>& operator = (value<uint> && val);

		value<uint>& operator += (type val);
		value<uint>& operator += (value<uint> const& val);
		value<uint>& operator -= (type val);
		value<uint>& operator -= (value<uint> const& val);
		value<uint>& operator *= (type val);
		value<uint>& operator *= (value<uint> const& val);
		value<uint>& operator /= (type val);
		value<uint>& operator /= (value<uint> const& val);

		value<uint>& operator ++();
		value<uint> operator ++(int);
		value<uint>& operator --();
		value<uint> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const uint>
	{
	public: /*type definitions*/
		typedef uint type;

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

	template<> struct LINK value<uint&>
	{
	public: /*type definitions*/
		typedef uint type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<uint>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<uint>& operator = (type val);
		value<uint>& operator = (value const& val);
		value<uint>& operator = (value<uint> val);

		value<uint>& operator += (type val);
		value<uint>& operator += (value const& val);
		value<uint>& operator -= (type val);
		value<uint>& operator -= (value const& val);
		value<uint>& operator *= (type val);
		value<uint>& operator *= (value const& val);
		value<uint>& operator /= (type val);
		value<uint>& operator /= (value const& val);

		value<uint>& operator ++();
		value<uint> operator ++(int);
		value<uint>& operator --();
		value<uint> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const uint&>
	{
	public: /*type definitions*/
		typedef uint type;

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

	template<> struct LINK value<uint*>
	{
	public: /*type definitions*/
		typedef uint* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<uint>* val);
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
		inline value<uint>& operator * ();
		inline value<uint> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	template<> struct LINK value<uint const*>
	{
	public: /*type definitions*/
		typedef uint const* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<uint> const* val);
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
		inline value<uint const> & operator * ();
		inline value<uint const> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	inline value<uint> operator ""_u(unsigned long long val) { return value<uint>(static_cast<uint>(val)); }
	inline value<uint> operator ""_ui(unsigned long long val) { return value<uint>(static_cast<uint>(val)); }
}

#include<ang/value/inline/uint.hpp>

#endif//__ANG_UINT_H__
