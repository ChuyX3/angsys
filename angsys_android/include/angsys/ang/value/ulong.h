#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_ULONG_H__
#define __ANG_ULONG_H__

namespace ang
{
	template<> struct LINK value<ulong>
	{
	public: /*type definitions*/
		typedef ulong type;

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
		value<ulong>& operator = (type val);
		value<ulong>& operator = (value<ulong> const& val);
		value<ulong>& operator = (value<ulong> && val);

		value<ulong>& operator += (type val);
		value<ulong>& operator += (value<ulong> const& val);
		value<ulong>& operator -= (type val);
		value<ulong>& operator -= (value<ulong> const& val);
		value<ulong>& operator *= (type val);
		value<ulong>& operator *= (value<ulong> const& val);
		value<ulong>& operator /= (type val);
		value<ulong>& operator /= (value<ulong> const& val);

		value<ulong>& operator ++();
		value<ulong> operator ++(int);
		value<ulong>& operator --();
		value<ulong> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const ulong>
	{
	public: /*type definitions*/
		typedef ulong type;

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

	template<> struct LINK value<ulong&>
	{
	public: /*type definitions*/
		typedef ulong type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<ulong>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<ulong>& operator = (type val);
		value<ulong>& operator = (value const& val);
		value<ulong>& operator = (value<ulong> val);

		value<ulong>& operator += (type val);
		value<ulong>& operator += (value const& val);
		value<ulong>& operator -= (type val);
		value<ulong>& operator -= (value const& val);
		value<ulong>& operator *= (type val);
		value<ulong>& operator *= (value const& val);
		value<ulong>& operator /= (type val);
		value<ulong>& operator /= (value const& val);

		value<ulong>& operator ++();
		value<ulong> operator ++(int);
		value<ulong>& operator --();
		value<ulong> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const ulong&>
	{
	public: /*type definitions*/
		typedef ulong type;

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

	template<> struct LINK value<ulong*>
	{
	public: /*type definitions*/
		typedef ulong* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<ulong>* val);
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
		inline value<ulong>& operator * ();
		inline value<ulong> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	template<> struct LINK value<ulong const*>
	{
	public: /*type definitions*/
		typedef ulong const* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<ulong> const* val);
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
		inline value<ulong const> & operator * ();
		inline value<ulong const> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};


	template<> struct LINK value<ulong64>
	{
	public: /*type definitions*/
		typedef ulong64 type;

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
		value<ulong64>& operator = (type val);
		value<ulong64>& operator = (value<ulong64> const& val);
		value<ulong64>& operator = (value<ulong64> && val);

		value<ulong64>& operator += (type val);
		value<ulong64>& operator += (value<ulong64> const& val);
		value<ulong64>& operator -= (type val);
		value<ulong64>& operator -= (value<ulong64> const& val);
		value<ulong64>& operator *= (type val);
		value<ulong64>& operator *= (value<ulong64> const& val);
		value<ulong64>& operator /= (type val);
		value<ulong64>& operator /= (value<ulong64> const& val);

		value<ulong64>& operator ++();
		value<ulong64> operator ++(int);
		value<ulong64>& operator --();
		value<ulong64> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const ulong64>
	{
	public: /*type definitions*/
		typedef ulong64 type;

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

	template<> struct LINK value<ulong64&>
	{
	public: /*type definitions*/
		typedef ulong64 type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<ulong64>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<ulong64>& operator = (type val);
		value<ulong64>& operator = (value const& val);
		value<ulong64>& operator = (value<ulong64> val);

		value<ulong64>& operator += (type val);
		value<ulong64>& operator += (value const& val);
		value<ulong64>& operator -= (type val);
		value<ulong64>& operator -= (value const& val);
		value<ulong64>& operator *= (type val);
		value<ulong64>& operator *= (value const& val);
		value<ulong64>& operator /= (type val);
		value<ulong64>& operator /= (value const& val);

		value<ulong64>& operator ++();
		value<ulong64> operator ++(int);
		value<ulong64>& operator --();
		value<ulong64> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const ulong64&>
	{
	public: /*type definitions*/
		typedef ulong64 type;

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

	template<> struct LINK value<ulong64*>
	{
	public: /*type definitions*/
		typedef ulong64* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<ulong64>* val);
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
		inline value<ulong64>& operator * ();
		inline value<ulong64> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	template<> struct LINK value<ulong64 const*>
	{
	public: /*type definitions*/
		typedef ulong64 const* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<ulong64> const* val);
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
		inline value<ulong64 const> & operator * ();
		inline value<ulong64 const> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	inline value<ulong> operator ""_ul(unsigned long long val) { return value<ulong>(static_cast<ulong>(val)); }
	inline value<ulong> operator ""_uli(unsigned long long val) { return value<ulong>(static_cast<ulong>(val)); }
	inline value<ulong64> operator ""_ull(unsigned long long val) { return value<ulong64>(static_cast<ulong>(val)); }
	inline value<ulong64> operator ""_ulli(unsigned long long val) { return value<ulong64>(static_cast<ulong>(val)); }
}

#include<ang/value/inline/ulong.inl>

#endif//__ANG_ULONG_H__

