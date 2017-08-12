#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_FLOAT_H__
#define __ANG_FLOAT_H__

namespace ang
{
	template<> struct LINK value<float>
	{
	public: /*type definitions*/
		typedef float type;

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
		value<float>& operator = (type val);
		value<float>& operator = (value<float> const& val);
		value<float>& operator = (value<float> && val);

		value<float>& operator += (type val);
		value<float>& operator += (value<float> const& val);
		value<float>& operator -= (type val);
		value<float>& operator -= (value<float> const& val);
		value<float>& operator *= (type val);
		value<float>& operator *= (value<float> const& val);
		value<float>& operator /= (type val);
		value<float>& operator /= (value<float> const& val);

		value<float>& operator ++();
		value<float> operator ++(int);
		value<float>& operator --();
		value<float> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const float>
	{
	public: /*type definitions*/
		typedef float type;

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

	template<> struct LINK value<float&>
	{
	public: /*type definitions*/
		typedef float type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<float>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<float>& operator = (type val);
		value<float>& operator = (value const& val);
		value<float>& operator = (value<float> val);

		value<float>& operator += (type val);
		value<float>& operator += (value const& val);
		value<float>& operator -= (type val);
		value<float>& operator -= (value const& val);
		value<float>& operator *= (type val);
		value<float>& operator *= (value const& val);
		value<float>& operator /= (type val);
		value<float>& operator /= (value const& val);

		value<float>& operator ++();
		value<float> operator ++(int);
		value<float>& operator --();
		value<float> operator --(int);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const float&>
	{
	public: /*type definitions*/
		typedef float type;

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

	template<> struct LINK value<float*>
	{
	public: /*type definitions*/
		typedef float* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<float>* val);
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
		inline value<float>& operator * ();
		inline value<float> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	template<> struct LINK value<float const*>
	{
	public: /*type definitions*/
		typedef float const* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<float> const* val);
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
		inline value<float const> & operator * ();
		inline value<float const> const& operator * ()const;
		inline explicit operator pointer()const { return (void*)get(); }
	};

	inline value<float> operator ""_f(long double val) { return value<float>(static_cast<float>(val)); }
	inline value<float> operator ""_f(unsigned long long val) { return value<float>(static_cast<float>(val)); }
}

#include <ang/value/inline/float.inl>

#endif//__ANG_FLOAT_H__
