#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_CHAR_H__
#define __ANG_CHAR_H__

namespace ang
{
	template<> struct LINK value<char>
	{
	public: /*type definitions*/
		typedef char type;

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
		value& operator = (type val);
		value& operator = (value const& val);
		value& operator = (value && val);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator value<wchar>()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const char>
	{
	public: /*type definitions*/
		typedef char type;

	private: /*members*/
		const type _value;

	public: /*constructors*/
		value(type val);
		value(value const& other);

	public: /*getters and setters*/
		type const& get()const;

	public: /*operators*/
		inline operator type ()const;
		inline explicit operator value<wchar>()const;
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<char&>
	{
	public: /*type definitions*/
		typedef char type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<char>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<char>& operator = (type val);
		value<char>& operator = (value const& val);
		value<char>& operator = (value<char> val);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator value<wchar>()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const char&>
	{
	public: /*type definitions*/
		typedef char type;

	private: /*members*/
		const type& _value;

	public: /*constructors*/
		value(type const& val);
		value(value const& other);

	public: /*getters and setters*/
		type const& get()const;

	public: /*operators*/
		inline operator type ()const;
		inline explicit operator value<wchar>()const;
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<char*>
	{
	public: /*type definitions*/
		typedef char* type;

	private: /*members*/
		type _value;
		uint _size;

	public: /*constructors*/
		value();
		value(type val, uint /*= -1*/);
		value(value<char>* val, uint /*= -1*/);
		value(value const& other);
		value(value && other);
		value(ang::nullptr_t);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, uint /*= -1*/);
		inline uint size()const { return _size; }
		inline char* data()const { return _value; }
		inline char* str() { return _value; }
		inline const char* cstr()const { return _value; }

	public: /*operators*/
		value& operator = (type val);
		value& operator = (value const& val);
		value& operator = (value && val);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline value<char>& operator * ();
		inline value<char> const& operator * ()const;
		inline value<char>& operator [] (int idx);
		inline value<char>& operator [] (uint idx);
		inline value<char> const& operator [] (int idx)const;
		inline value<char> const& operator [] (uint idx)const;
	};

	template<> struct LINK value<char const*>
	{
	public: /*type definitions*/
		typedef char const* type;

	private: /*members*/
		type _value;
		uint _size;

	public: /*constructors*/
		value();
		value(type val, uint /*= -1*/);
		value(value<char> const* val, uint /*= -1*/);
		value(value const& other);
		value(value && other);
		value(std::nullptr_t);
		template<wsize N>
		value(const char(&ar)[N])
			: value(ar, N - 1U) { }

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, uint /*= -1*/);
		inline uint size()const { return _size; }
		inline char const* data()const { return _value; }
		inline char const* cstr()const { return _value; }

	public: /*string operations*/
		uint find(cstr_t cstr, uint start)const;
		uint find(cstr_t cstr, uint start, uint end)const;
		uint find(cwstr_t cstr, uint start)const;
		uint find(cwstr_t cstr, uint start, uint end)const;
		uint find(char c, uint start)const;
		uint find(char c, uint start, uint end)const;

		uint find_rev(cstr_t cstr, uint start)const;
		uint find_rev(cstr_t cstr, uint start, uint end)const;
		uint find_rev(cwstr_t cstr, uint start)const;
		uint find_rev(cwstr_t cstr, uint start, uint end)const;
		uint find_rev(char c, uint start)const;
		uint find_rev(char c, uint start, uint end)const;

		uint sub_string(string& out, uint start, uint count)const;
		uint sub_string(char* out, uint start, uint count)const;

		uint sub_string(wstring& out, uint start, uint count)const;
		uint sub_string(wchar* out, uint start, uint count)const;

	public: /*operators*/
		value& operator = (type val);
		value& operator = (value const& val);
		value& operator = (value && val);
		template<wsize N>
		value& operator = (const char(&ar)[N]) {
			_value = ar;
			_size = N - 1;
			return*this;
		}

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline value<char const> & operator * ();
		inline value<char const> const& operator * ()const;
		inline value<char> const& operator [] (int idx)const;
		inline value<char> const& operator [] (uint idx)const;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<> struct LINK value<wchar>
	{
	public: /*type definitions*/
		typedef wchar type;

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
		value& operator = (type val);
		value& operator = (value const& val);
		value& operator = (value && val);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator value<char> ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const wchar>
	{
	public: /*type definitions*/
		typedef wchar type;

	private: /*members*/
		const type _value;

	public: /*constructors*/
		value(type val);
		value(value const& other);

	public: /*getters and setters*/
		type const& get()const;

	public: /*operators*/
		inline operator type ()const;
		inline explicit operator value<char>()const;
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<wchar&>
	{
	public: /*type definitions*/
		typedef wchar type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<wchar>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<wchar>& operator = (type val);
		value<wchar>& operator = (value const& val);
		value<wchar>& operator = (value<wchar> val);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator value<char>()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const wchar&>
	{
	public: /*type definitions*/
		typedef wchar type;

	private: /*members*/
		const type& _value;

	public: /*constructors*/
		value(type const& val);
		value(value const& other);

	public: /*getters and setters*/
		type const& get()const;

	public: /*operators*/
		inline operator type ()const;
		inline explicit operator value<char>()const;
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<wchar*>
	{
	public: /*type definitions*/
		typedef wchar* type;

	private: /*members*/
		type _value;
		uint _size;

	public: /*constructors*/
		value();
		value(type val, uint /*= -1*/);
		value(value<wchar>* val, uint /*= -1*/);
		value(value const& other);
		value(value && other);
		value(std::nullptr_t);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, uint /*= -1*/);
		inline uint size()const { return _size; }
		inline wchar* data()const { return _value; }
		inline wchar* str() { return _value; }
		inline wchar const* cstr()const { return _value; }

	public: /*operators*/
		value& operator = (type val);
		value& operator = (value const& val);
		value& operator = (value && val);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline value<wchar>& operator * ();
		inline value<wchar> const& operator * ()const;
		inline value<wchar>& operator [] (int idx);
		inline value<wchar>& operator [] (uint idx);
		inline value<wchar> const& operator [] (int idx)const;
		inline value<wchar> const& operator [] (uint idx)const;
	};

	template<> struct LINK value<wchar const*>
	{
	public: /*type definitions*/
		typedef wchar const* type;

	private: /*members*/
		type _value;
		uint _size;

	public: /*constructors*/
		value();
		value(type val, uint /*= -1*/);
		value(value<wchar> const* val, uint /*= -1*/);
		value(value const& other);
		value(value && other);
		value(std::nullptr_t);
		template<wsize N>
		value(const wchar(&ar)[N])
			: value(ar, N - 1U) { }

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, uint /*= -1*/);
		inline uint size()const { return _size; }
		inline wchar const* data()const { return _value; }
		inline wchar const* cstr()const { return _value; }

	public: /*string operations*/

		uint find(cstr_t str, uint start)const;
		uint find(cstr_t str, uint start, uint end)const;
		uint find(cwstr_t str, uint start)const;
		uint find(cwstr_t str, uint start, uint end)const;
		uint find(wchar c, uint start)const;
		uint find(wchar c, uint start, uint end)const;

		uint find_rev(cstr_t cstr, uint start)const;
		uint find_rev(cstr_t cstr, uint start, uint end)const;
		uint find_rev(cwstr_t cstr, uint start)const;
		uint find_rev(cwstr_t cstr, uint start, uint end)const;
		uint find_rev(char c, uint start)const;
		uint find_rev(char c, uint start, uint end)const;

		uint sub_string(wstring& out, uint start, uint count)const;
		uint sub_string(wchar* out, uint start, uint count)const;

		uint sub_string(string& out, uint start, uint count)const;
		uint sub_string(char* out, uint start, uint count)const;

	public: /*operators*/
		value& operator = (type val);
		value& operator = (value const& val);
		value& operator = (value && val);
		template<wsize N>
		value& operator = (const wchar(&ar)[N]) {
			_value = ar;
			_size = N - 1;
			return*this;
		}

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline value<wchar const> & operator * ();
		inline value<wchar const> const& operator * ()const;
		inline value<wchar> const& operator [] (int idx)const;
		inline value<wchar> const& operator [] (uint idx)const;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<> struct LINK value<mchar>
	{
	public: /*type definitions*/
		typedef mchar type;

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
		value& operator = (type val);
		value& operator = (value const& val);
		value& operator = (value && val);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const mchar>
	{
	public: /*type definitions*/
		typedef mchar type;

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

	template<> struct LINK value<mchar&>
	{
	public: /*type definitions*/
		typedef mchar type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val);
		value(value& other);
		value(value<mchar>& other);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val);

	public: /*operators*/
		value<mchar>& operator = (type val);
		value<mchar>& operator = (value const& val);
		value<mchar>& operator = (value<mchar> val);

		inline operator type& ();
		inline operator type ()const;
		inline value<type*> operator & ();
		inline value<type const*> operator & ()const;
	};

	template<> struct LINK value<const mchar&>
	{
	public: /*type definitions*/
		typedef mchar type;

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

	template<> struct LINK value<mchar*>
	{
	public: /*type definitions*/
		typedef mchar* type;

	private: /*members*/
		type _value;
		uint _size;

	public: /*constructors*/
		value();
		value(type val, uint /*= -1*/);
		value(value<mchar>* val, uint /*= -1*/);
		value(value const& other);
		value(value && other);
		value(std::nullptr_t);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, uint /*= -1*/);
		inline uint size()const { return _size; }
		inline mchar* str() { return _value; }
		inline mchar* data()const { return _value; }
		inline mchar const* cstr()const { return _value; }

	public: /*operators*/
		value& operator = (type val);
		value& operator = (value const& val);
		value& operator = (value && val);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline value<mchar>& operator * ();
		inline value<mchar> const& operator * ()const;
		inline value<mchar>& operator [] (int idx);
		inline value<mchar>& operator [] (uint idx);
		inline value<mchar> const& operator [] (int idx)const;
		inline value<mchar> const& operator [] (uint idx)const;
	};

	template<> struct LINK value<mchar const*>
	{
	public: /*type definitions*/
		typedef mchar const* type;

	private: /*members*/
		type _value;
		uint _size;

	public: /*constructors*/
		value();
		value(type val, uint /*= -1*/);
		value(value<mchar> const* val, uint /*= -1*/);
		value(value const& other);
		value(value && other);
		value(std::nullptr_t);
		template<wsize N>
		explicit value(const char(&ar)[N])
			: value((type)ar, N - 1U) { }

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, uint /*= -1*/);
		inline uint size()const { return _size; }
		inline mchar const* data()const { return _value; }
		inline mchar const* cstr()const { return _value; }

	public: /*string operations*/
		template<wsize N>
		uint find(const char(&ar)[N], uint start, uint* plenght = null) {
			return find(cstr_t(ar), start, plenght);
		}
		template<wsize N>
		uint find(const char(&ar)[N], uint start, uint end, uint* plenght = null) {
			return find(cstr_t(ar), start, end, plenght);
		}
		uint find(cmstr_t cstr, uint start)const;
		uint find(cmstr_t cstr, uint start, uint end)const;
		uint find(cstr_t cstr, uint start, uint* plenght = null)const;
		uint find(cstr_t cstr, uint start, uint end, uint* plenght = null)const;
		uint find(cwstr_t cstr, uint start, uint* plenght = null)const;
		uint find(cwstr_t cstr, uint start, uint end, uint* plenght = null)const;

		uint sub_string(wstring& out, uint start, uint count)const;
		uint sub_string(wchar* out, uint start, uint count)const;

		uint sub_string(string& out, uint start, uint count)const;
		uint sub_string(char* out, uint start, uint count)const;

	public: /*operators*/
		value& operator = (type val);
		value& operator = (value const& val);
		value& operator = (value && val);
		template<wsize N>
		value& operator = (const mchar(&ar)[N]) {
			_value = ar;
			_size = N - 1;
			return*this;
		}

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline value<mchar const> & operator * ();
		inline value<mchar const> const& operator * ()const;
		inline value<mchar> const& operator [] (int idx)const;
		inline value<mchar> const& operator [] (uint idx)const;
	};

	inline cstr_t operator "" _s(const char* str, wsize sz) { return cstr_t(str, sz); }
	inline cwstr_t operator "" _s(const wchar* str, wsize sz) { return cwstr_t(str, sz); }
	inline cmstr_t operator "" _sm(const char* str, wsize sz) { return cmstr_t((mchar const*)str, sz); }

	inline value<char> operator ""_ac(unsigned long long val) { return value<char>(static_cast<char>(val)); }
	inline value<byte> operator ""_mc(unsigned long long val) { return value<byte>(static_cast<byte>(val)); }
	inline value<mchar> operator ""_b(unsigned long long val) { return value<mchar>(static_cast<mchar>(val)); }
	inline value<wchar> operator ""_wc(unsigned long long val) { return value<wchar>(static_cast<wchar>(val)); }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#include <ang/value/inline/char.inl>

#endif//__ANG_CHAR_H__
