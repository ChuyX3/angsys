#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_VOID_PTR_H__
#define __ANG_VOID_PTR_H__

namespace ang
{
	template<> struct LINK value<pointer>
	{
	public: /*type definitions*/
		typedef pointer type;

	private: /*members*/
		type _value;
		uint _size;

	public: /*constructors*/
		value();
		value(type val, uint = -1);
		value(value const& other);
		value(value && other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, uint = -1);
		uint size()const;

	public: /*operators*/
		value<pointer>& operator = (type val);
		value<pointer>& operator = (value<pointer> const& val);
		value<pointer>& operator = (value<pointer> && val);

		value<pointer>& operator += (type val);
		value<pointer>& operator += (value<pointer> const& val);
		value<pointer>& operator -= (type val);
		value<pointer>& operator -= (value<pointer> const& val);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
		template<class U> inline explicit operator value<U*>() { return (U*)get(); }
		template<class U> inline explicit operator value<U const*>()const { return (U const*)get(); }
	};

	template<> struct LINK value<const pointer>
	{
	public: /*type definitions*/
		typedef pointer type;

	private: /*members*/
		const type _value;
		uint _size;

	public: /*constructors*/
		value(type val, uint = -1);
		value(value const& other);
		template<typename T2> value(value<T2> const& other);

	public: /*getters and setters*/
		type const& get()const;
		uint size()const;

	public: /*operators*/
		inline operator type ()const;
		inline value<type const*> operator & ()const;
		template<class U> inline explicit operator value<U const*>()const { return (U const*)get(); }

	};

	template<> struct LINK value<pointer&>
	{
	public: /*type definitions*/
		typedef pointer type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<pointer>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<pointer>& operator = (type val);
		value<pointer>& operator = (value const& val);
		value<pointer>& operator = (value<pointer> val);

		value<pointer>& operator += (type val);
		value<pointer>& operator += (value const& val);
		value<pointer>& operator -= (type val);
		value<pointer>& operator -= (value const& val);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
		template<class U> inline explicit operator value<U*>() { return (U*)get(); }
		template<class U> inline explicit operator value<U const*>()const { return (U const*)get(); }
	};

	template<> struct LINK value<const pointer&>
	{
	public: /*type definitions*/
		typedef pointer type;

	private: /*members*/
		const type& _value;

	public: /*constructors*/
		value(type const& val);
		value(value const& other);
		template<typename T2> value(value<T2> const& other) : _value(other.get()) {}

	public: /*getters and setters*/
		type const& get()const;

	public: /*operators*/
		inline operator type ()const;
		inline value<type const*> operator & ()const;
		template<class U> inline explicit operator value<U const*>()const { return (U const*)get(); }

	};

	template<> struct LINK value<pointer*>
	{
	public: /*type definitions*/
		typedef pointer* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<pointer>* val);
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
		inline pointer& operator * ();
		inline pointer operator * ()const;
	};

	template<> struct LINK value<pointer const*>
	{
	public: /*type definitions*/
		typedef pointer const* type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(type val);
		value(ang::nullptr_t);
		value(value<pointer> const* val);
		value(value const& other);
		value(value && other);
		template<typename T2> value(value<T2*> other) : _value(other.get()) {}
		template<typename T2> value(value<T2 const*> other) : _value(other->get()) {}

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
		inline pointer operator * ()const;
	};
}

#include<ang/value/inline/void_ptr.hpp>

#endif//__ANG_VOID_PTR_H__

