#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_BASE_STRING_HPP__
#define __ANG_BASE_STRING_HPP__

namespace ang
{
	template<typename T> struct safe_str : public rule<char_value<T>::value> {};

	template<> struct LINK safe_str<char>
	{
	public: /*type definitions*/
		typedef char* type;
		typedef char const* ctype;

	private: /*members*/
		type _value;
		wsize _size;

	public: /*constructors*/
		safe_str();
		safe_str(type val, wsize);
		safe_str(safe_str const& other);
		safe_str(safe_str && other);
		safe_str(ang::nullptr_t);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, wsize);
		inline wsize size()const { return _size; }
		inline char* data()const { return _value; }
		inline char* str() { return _value; }
		inline const char* cstr()const { return _value; }

	public: /*operators*/
		safe_str& operator = (type val);
		safe_str& operator = (safe_str const& val);
		safe_str& operator = (safe_str && val);

		safe_str& operator ++ (void);
		safe_str operator ++ (int);
		safe_str& operator -- (void);
		safe_str operator -- (int);

		inline operator type& ();
		inline operator type ()const;
		inline operator ctype ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline char& operator * ();
		inline char const& operator * ()const;
		template<typename I>inline char& operator [] (I idx);
		template<typename I>inline char const& operator [] (I idx)const;
	};

	template<> struct LINK safe_str<char const>
	{
	public: /*type definitions*/
		typedef char const* type;

	private: /*members*/
		type _value;
		wsize _size;

	public: /*constructors*/
		safe_str();
		safe_str(type val, wsize = -1);
		safe_str(safe_str const& other);
		safe_str(safe_str && other);
		safe_str(std::nullptr_t);
		template<wsize N>
		safe_str(const char(&ar)[N])
			: safe_str(ar, N - 1U) { }

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, wsize = -1);
		inline wsize size()const { return _size; }
		inline char const* data()const { return _value; }
		inline char const* cstr()const { return _value; }

	public: /*string operations*/
		template<typename T> wsize find(safe_str<T> const& cstr, wsize start)const;
		template<typename T> wsize find(safe_str<T> const& cstr, wsize start, wsize end)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::char_t cstr, wsize start)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const;

		template<typename T> wsize find_revert(safe_str<T> const& cstr, wsize start)const;
		template<typename T> wsize find_revert(safe_str<T> const& cstr, wsize start, wsize end)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const;


		//wsize sub_string(string& out, wsize start, wsize count)const;
		wsize sub_string(char* out, wsize start, wsize count)const;

		//wsize sub_string(mstring& out, wsize start, wsize count)const;
		wsize sub_string(mchar* out, wsize start, wsize count)const;

		//wsize sub_string(wstring& out, wsize start, wsize count)const;
		wsize sub_string(wchar* out, wsize start, wsize count)const;

	public: /*operators*/
		safe_str& operator = (type val);
		safe_str& operator = (safe_str const& val);
		safe_str& operator = (safe_str && val);
		template<wsize N>
		safe_str& operator = (const char(&ar)[N]) {
			_value = ar;
			_size = N - 1;
			return*this;
		}

		safe_str& operator ++ (void);
		safe_str operator ++ (int);
		safe_str& operator -- (void);
		safe_str operator -- (int);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline char const& operator * ()const;
		template<typename I>inline char const& operator [] (I idx)const;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<> struct LINK safe_str<wchar>
	{
	public: /*type definitions*/
		typedef wchar* type;
		typedef wchar const* ctype;

	private: /*members*/
		type _value;
		wsize _size;

	public: /*constructors*/
		safe_str();
		safe_str(type val, wsize);
		safe_str(safe_str const& other);
		safe_str(safe_str && other);
		safe_str(std::nullptr_t);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, wsize);
		inline wsize size()const { return _size; }
		inline wchar* data()const { return _value; }
		inline wchar* str() { return _value; }
		inline wchar const* cstr()const { return _value; }

	public: /*operators*/
		safe_str& operator = (type val);
		safe_str& operator = (safe_str const& val);
		safe_str& operator = (safe_str && val);

		safe_str& operator ++ (void);
		safe_str operator ++ (int);
		safe_str& operator -- (void);
		safe_str operator -- (int);

		inline operator type& ();
		inline operator type ()const;
		inline operator ctype ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline wchar& operator * ();
		inline wchar const& operator * ()const;
		template<typename I>inline wchar& operator [] (I idx);
		template<typename I>inline wchar const& operator [] (I idx)const;
	};

	template<> struct LINK safe_str<wchar const>
	{
	public: /*type definitions*/
		typedef wchar const* type;

	private: /*members*/
		type _value;
		wsize _size;

	public: /*constructors*/
		safe_str();
		safe_str(type val, wsize = -1);
		safe_str(safe_str const& other);
		safe_str(safe_str && other);
		safe_str(std::nullptr_t);
		template<wsize N>
		safe_str(const wchar(&ar)[N])
			: safe_str(ar, N - 1U) { }

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, wsize = -1);
		inline wsize size()const { return _size; }
		inline wchar const* data()const { return _value; }
		inline wchar const* cstr()const { return _value; }

	public: /*string operations*/
		template<typename T> wsize find(safe_str<T> const& cstr, wsize start)const;
		template<typename T> wsize find(safe_str<T> const& cstr, wsize start, wsize end)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::char_t cstr, wsize start)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const;

		template<typename T> wsize find_revert(safe_str<T> const& cstr, wsize start)const;
		template<typename T> wsize find_revert(safe_str<T> const& cstr, wsize start, wsize end)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const;

		//wsize sub_string(wstring& out, wsize start, wsize count)const;
		wsize sub_string(wchar* out, wsize start, wsize count)const;

		//wsize sub_string(mstring& out, wsize start, wsize count)const;
		wsize sub_string(mchar* out, wsize start, wsize count)const;

		//wsize sub_string(string& out, wsize start, wsize count)const;
		wsize sub_string(char* out, wsize start, wsize count)const;

	public: /*operators*/
		safe_str& operator = (type val);
		safe_str& operator = (safe_str const& val);
		safe_str& operator = (safe_str && val);
		template<wsize N>
		safe_str& operator = (const wchar(&ar)[N]) {
			_value = ar;
			_size = N - 1;
			return*this;
		}

		safe_str& operator ++ (void);
		safe_str operator ++ (int);
		safe_str& operator -- (void);
		safe_str operator -- (int);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline wchar const& operator * ()const;
		template<typename I>inline wchar const& operator [] (I idx)const;
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<> struct LINK safe_str<mchar>
	{
	public: /*type definitions*/
		typedef mchar* type;
		typedef mchar const* ctype;

	private: /*members*/
		type _value;
		wsize _size;

	public: /*constructors*/
		safe_str();
		safe_str(type val, wsize);
		safe_str(safe_str const& other);
		safe_str(safe_str && other);
		safe_str(std::nullptr_t);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, wsize);
		inline wsize size()const { return _size; }
		inline mchar* str() { return _value; }
		inline mchar* data()const { return _value; }
		inline mchar const* cstr()const { return _value; }

	public: /*operators*/
		safe_str& operator = (type val);
		safe_str& operator = (safe_str const& val);
		safe_str& operator = (safe_str && val);

		safe_str& operator ++ (void);
		safe_str operator ++ (int);
		safe_str& operator -- (void);
		safe_str operator -- (int);

		inline operator type& ();
		inline operator type ()const;
		inline operator ctype ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline mchar& operator * ();
		inline mchar const& operator * ()const;
		template<typename I>inline mchar& operator [] (I idx);
		template<typename I>inline mchar const& operator [] (I idx)const;
	};

	template<> struct LINK safe_str<mchar const>
	{
	public: /*type definitions*/
		typedef mchar const* type;

	private: /*members*/
		type _value;
		wsize _size;

	public: /*constructors*/
		safe_str();
		safe_str(type val, wsize = -1);
		safe_str(safe_str const& other);
		safe_str(safe_str && other);
		safe_str(std::nullptr_t);
		template<wsize N>
		safe_str(const char(&ar)[N])
			: safe_str((type)ar, N - 1U) { }

		template<wsize N>
		safe_str(const mchar(&ar)[N])
			: safe_str(ar, N - 1U) { }

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, wsize = -1);
		inline wsize size()const { return _size; }
		inline mchar const* data()const { return _value; }
		inline mchar const* cstr()const { return _value; }

	public: /*string operations*/
		template<typename T> wsize find(safe_str<T> const& cstr, wsize start)const;
		template<typename T> wsize find(safe_str<T> const& cstr, wsize start, wsize end)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::char_t cstr, wsize start)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const;

		template<typename T> wsize find_revert(safe_str<T> const& cstr, wsize start)const;
		template<typename T> wsize find_revert(safe_str<T> const& cstr, wsize start, wsize end)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const;

		//wsize sub_string(mstring& out, wsize start, wsize count)const;
		wsize sub_string(mchar* out, wsize start, wsize count)const;

		//wsize sub_string(wstring& out, wsize start, wsize count)const;
		wsize sub_string(wchar* out, wsize start, wsize count)const;

		//wsize sub_string(string& out, wsize start, wsize count)const;
		wsize sub_string(char* out, wsize start, wsize count)const;

	public: /*operators*/
		safe_str& operator = (type val);
		safe_str& operator = (safe_str const& val);
		safe_str& operator = (safe_str && val);
		template<wsize N>
		safe_str& operator = (const mchar(&ar)[N]) {
			_value = ar;
			_size = N - 1;
			return*this;
		}

		safe_str& operator ++ (void);
		safe_str operator ++ (int);
		safe_str& operator -- (void);
		safe_str operator -- (int);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline mchar const& operator * ()const;
		template<typename I>inline mchar const& operator [] (I idx)const;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<> struct LINK safe_str<char16_t>
	{
	public: /*type definitions*/
		typedef char16_t* type;
		typedef char16_t const* ctype;

	private: /*members*/
		type _value;
		wsize _size;

	public: /*constructors*/
		safe_str();
		safe_str(type val, wsize);
		safe_str(safe_str const& other);
		safe_str(safe_str && other);
		safe_str(std::nullptr_t);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, wsize);
		inline wsize size()const { return _size; }
		inline char16_t* data()const { return _value; }
		inline char16_t* str() { return _value; }
		inline char16_t const* cstr()const { return _value; }

	public: /*operators*/
		safe_str& operator = (type val);
		safe_str& operator = (safe_str const& val);
		safe_str& operator = (safe_str && val);

		safe_str& operator ++ (void);
		safe_str operator ++ (int);
		safe_str& operator -- (void);
		safe_str operator -- (int);

		inline operator type& ();
		inline operator type ()const;
		inline operator ctype ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline char16_t& operator * ();
		inline char16_t const& operator * ()const;
		template<typename I>inline char16_t& operator [] (I idx);
		template<typename I>inline char16_t const& operator [] (I idx)const;
	};

	template<> struct LINK safe_str<char16_t const>
	{
	public: /*type definitions*/
		typedef char16_t const* type;

	private: /*members*/
		type _value;
		wsize _size;

	public: /*constructors*/
		safe_str();
		safe_str(type val, wsize = -1);
		safe_str(safe_str const& other);
		safe_str(safe_str && other);
		safe_str(std::nullptr_t);
		template<wsize N>
		safe_str(const char16_t(&ar)[N])
			: safe_str(ar, N - 1U) { }

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, wsize = -1);
		inline wsize size()const { return _size; }
		inline char16_t const* data()const { return _value; }
		inline char16_t const* cstr()const { return _value; }

	public: /*string operations*/
		template<typename T> wsize find(safe_str<T> const& cstr, wsize start)const;
		template<typename T> wsize find(safe_str<T> const& cstr, wsize start, wsize end)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::char_t cstr, wsize start)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const;

		template<typename T> wsize find_revert(safe_str<T> const& cstr, wsize start)const;
		template<typename T> wsize find_revert(safe_str<T> const& cstr, wsize start, wsize end)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const;

		//wsize sub_string(wstring& out, wsize start, wsize count)const;
		wsize sub_string(char16_t* out, wsize start, wsize count)const;

		//wsize sub_string(mstring& out, wsize start, wsize count)const;
		wsize sub_string(mchar* out, wsize start, wsize count)const;

		//wsize sub_string(string& out, wsize start, wsize count)const;
		wsize sub_string(char* out, wsize start, wsize count)const;

	public: /*operators*/
		safe_str& operator = (type val);
		safe_str& operator = (safe_str const& val);
		safe_str& operator = (safe_str && val);
		template<wsize N>
		safe_str& operator = (const char16_t(&ar)[N]) {
			_value = ar;
			_size = N - 1;
			return*this;
		}

		safe_str& operator ++ (void);
		safe_str operator ++ (int);
		safe_str& operator -- (void);
		safe_str operator -- (int);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline char16_t const& operator * ()const;
		template<typename I>inline char16_t const& operator [] (I idx)const;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<> struct LINK safe_str<char32_t>
	{
	public: /*type definitions*/
		typedef char32_t* type;
		typedef char32_t const* ctype;

	private: /*members*/
		type _value;
		wsize _size;

	public: /*constructors*/
		safe_str();
		safe_str(type val, wsize);
		safe_str(safe_str const& other);
		safe_str(safe_str && other);
		safe_str(std::nullptr_t);

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, wsize);
		inline wsize size()const { return _size; }
		inline char32_t* data()const { return _value; }
		inline char32_t* str() { return _value; }
		inline char32_t const* cstr()const { return _value; }

	public: /*operators*/
		safe_str& operator = (type val);
		safe_str& operator = (safe_str const& val);
		safe_str& operator = (safe_str && val);

		safe_str& operator ++ (void);
		safe_str operator ++ (int);
		safe_str& operator -- (void);
		safe_str operator -- (int);

		inline operator type& ();
		inline operator type ()const;
		inline operator ctype ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline char32_t& operator * ();
		inline char32_t const& operator * ()const;
		template<typename I>inline char32_t& operator [] (I idx);
		template<typename I>inline char32_t const& operator [] (I idx)const;
	};

	template<> struct LINK safe_str<char32_t const>
	{
	public: /*type definitions*/
		typedef char32_t const* type;

	private: /*members*/
		type _value;
		wsize _size;

	public: /*constructors*/
		safe_str();
		safe_str(type val, wsize = -1);
		safe_str(safe_str const& other);
		safe_str(safe_str && other);
		safe_str(std::nullptr_t);
		template<wsize N>
		safe_str(const char32_t(&ar)[N])
			: safe_str(ar, N - 1U) { }

	public: /*getters and setters*/
		type & get();
		type const& get()const;
		void set(type val, wsize = -1);
		inline wsize size()const { return _size; }
		inline char32_t const* data()const { return _value; }
		inline char32_t const* cstr()const { return _value; }

	public: /*string operations*/
		template<typename T> wsize find(safe_str<T> const& cstr, wsize start)const;
		template<typename T> wsize find(safe_str<T> const& cstr, wsize start, wsize end)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::char_t cstr, wsize start)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const;
		template<typename T> wsize find(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const;

		template<typename T> wsize find_revert(safe_str<T> const& cstr, wsize start)const;
		template<typename T> wsize find_revert(safe_str<T> const& cstr, wsize start, wsize end)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::char_t cstr, wsize start, wsize end)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start)const;
		template<typename T> wsize find_revert(typename text::char_type_by_type<T>::cstr_t cstr, wsize start, wsize end)const;

		//wsize sub_string(wstring& out, wsize start, wsize count)const;
		wsize sub_string(char32_t* out, wsize start, wsize count)const;

		//wsize sub_string(mstring& out, wsize start, wsize count)const;
		wsize sub_string(mchar* out, wsize start, wsize count)const;

		//wsize sub_string(string& out, wsize start, wsize count)const;
		wsize sub_string(char* out, wsize start, wsize count)const;

	public: /*operators*/
		safe_str& operator = (type val);
		safe_str& operator = (safe_str const& val);
		safe_str& operator = (safe_str && val);
		template<wsize N>
		safe_str& operator = (const char32_t(&ar)[N]) {
			_value = ar;
			_size = N - 1;
			return*this;
		}

		safe_str& operator ++ (void);
		safe_str operator ++ (int);
		safe_str& operator -- (void);
		safe_str operator -- (int);

		inline operator type& ();
		inline operator type ()const;
		inline explicit operator pointer()const { return (void*)get(); }
		inline char32_t const& operator * ()const;
		template<typename I>inline char32_t const& operator [] (I idx)const;
	};

	template<typename T1, typename T2> inline bool operator == (safe_str<T1> const& cstr1, safe_str<T2> const& cstr2) { return ang::text::encoder_t().compare(cstr1.cstr(), cstr2.cstr()) == 0; }
	template<typename T1, typename T2> inline bool operator != (safe_str<T1> const& cstr1, safe_str<T2> const& cstr2) { return ang::text::encoder_t().compare(cstr1.cstr(), cstr2.cstr()) != 0; }
	template<typename T1, typename T2> inline bool operator >= (safe_str<T1> const& cstr1, safe_str<T2> const& cstr2) { return ang::text::encoder_t().compare(cstr1.cstr(), cstr2.cstr()) >= 0; }
	template<typename T1, typename T2> inline bool operator <= (safe_str<T1> const& cstr1, safe_str<T2> const& cstr2) { return ang::text::encoder_t().compare(cstr1.cstr(), cstr2.cstr()) <= 0; }
	template<typename T1, typename T2> inline bool operator > (safe_str<T1> const& cstr1, safe_str<T2> const& cstr2) { return ang::text::encoder_t().compare(cstr1.cstr(), cstr2.cstr()) > 0; }
	template<typename T1, typename T2> inline bool operator < (safe_str<T1> const& cstr1, safe_str<T2> const& cstr2) { return ang::text::encoder_t().compare(cstr1.cstr(), cstr2.cstr()) < 0; }

	inline cstr_t operator "" _s(const char* str, wsize sz) { return cstr_t(str, sz); }
	inline cwstr_t operator "" _s(const wchar_t* str, wsize sz) { return cwstr_t(str, sz); }
	inline cmstr_t operator "" _sm(const char* str, wsize sz) { return cmstr_t((mchar const*)str, sz); }

	namespace strings
	{
		using namespace ang::text;

		template<encoding_enum ENCODING> class string_buffer;

		typedef string_buffer<encoding::ascii> astring_buffer, ascii_string_buffer;
		typedef string_buffer<encoding::unicode> wstring_buffer, unicode_string_buffer;
		typedef string_buffer<encoding::utf8> mstring_buffer, utf8_string_buffer;

		typedef object_wrapper<string_buffer<encoding::ascii>> string_t, string;
		typedef object_wrapper<string_buffer<encoding::unicode>> wstring_t, wstring;
		typedef object_wrapper<string_buffer<encoding::utf8>> mstring_t, mstring;
		typedef object_wrapper<string_buffer<encoding::utf16>> u16string_t, u16string;
		typedef object_wrapper<string_buffer<encoding::utf32>> u32string_t, u32string;
		template<encoding_enum ENCODING> using string_base = object_wrapper<string_buffer<ENCODING>>;

		template<encoding_enum ENCODING>
		struct safe_str_by_encoding {
			typedef safe_str<typename char_type_by_encoding<ENCODING>::char_t> type;
			typedef safe_str<typename char_type_by_encoding<ENCODING>::char_t const> ctype;
		};

		struct algorithms
		{
			template<typename cstr_t>
			inline static windex string_length(cstr_t cstr) {
				return get_encoder_by_type<cstr_t>().length(cstr);
			}

			template<typename char_t>
			inline static windex string_length(safe_str<char_t> cstr) {
				return cstr.seze();
			}

			template<typename char_t>
			inline static windex string_length(safe_str<const char_t> cstr) {
				return cstr.size();
			}

			template<encoding_enum ENCODING, typename cstr_t>
			inline static windex string_size(cstr_t cstr) {
				return get_encoder<ENCODING>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
			}

			template<typename cstr_t>
			inline static windex string_size(cstr_t cstr, encoding_t encoding)
			{
				switch (encoding.get())
				{
				case encoding::ascii: return get_encoder<encoding::ascii>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::unicode:return get_encoder<encoding::unicode>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::utf8:return get_encoder<encoding::utf8>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::utf16_le:return get_encoder<encoding::utf16_le>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::utf16_be:return get_encoder<encoding::utf16_be>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::utf16:return get_encoder<encoding::utf16>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::utf32_le:return get_encoder<encoding::utf32_le>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::utf32_be:return get_encoder<encoding::utf32_be>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::utf32:return get_encoder<encoding::utf32>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				default: return 0;
				}
			}

			template<typename str_t, typename cstr_t>
			inline static windex string_copy(str_t str, cstr_t cstr, wsize count, bool eos = true) {
				return get_encoder_by_type<str_t>().convert(
					(typename char_type_by_type<str_t>::str_t)str, count,
					(typename char_type_by_type<cstr_t>::cstr_t)cstr, eos);
			}

			template<typename cstr1_t, typename cstr2_t>
			inline static int string_compare(cstr1_t cstr1, cstr2_t cstr2) {
				return get_encoder_by_type<cstr1_t>().compare(
					(typename char_type_by_type<cstr1_t>::cstr_t)cstr1,
					(typename char_type_by_type<cstr2_t>::cstr_t)cstr2
				);
			}

			template<typename cstr1_t, typename cstr2_t>
			inline static windex string_compare_until(cstr1_t cstr1, cstr2_t cstr2) {
				return get_encoder_by_type<cstr1_t>().compare_until(
					(typename char_type_by_type<cstr1_t>::cstr_t)cstr1,
					(typename char_type_by_type<cstr2_t>::cstr_t)cstr2
				);
			}
		};

	}

	namespace text
	{
		template<encoding_enum ENCODING> struct char_type_by_type<strings::string_base<ENCODING>> {
			typedef typename char_type_by_encoding<ENCODING>::char_t char_t;
			typedef typename char_type_by_encoding<ENCODING>::str_t str_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t cstr_t;
		};

		template<encoding_enum ENCODING> struct char_type_by_type<strings::string_base<ENCODING>&> {
			typedef typename char_type_by_encoding<ENCODING>::char_t char_t;
			typedef typename char_type_by_encoding<ENCODING>::str_t str_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t cstr_t;
		};

		template<encoding_enum ENCODING> struct char_type_by_type<strings::string_base<ENCODING> const&> {
			typedef typename char_type_by_encoding<ENCODING>::char_t char_t;
			typedef typename char_type_by_encoding<ENCODING>::str_t str_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t cstr_t;
		};
	}

	using strings::string;
	using strings::wstring;
	using strings::mstring;
	using strings::u16string;
	using strings::u32string;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#include <ang/base/inline/string.inl>

#endif//__ANG_BASE_SAFE_STR_HPP__
