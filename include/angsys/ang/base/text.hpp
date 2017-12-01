
#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_BASE_TEXT_HPP__
#define __ANG_BASE_TEXT_HPP__


namespace ang
{
	namespace text
	{
		class text_format;

		template<typename T> struct native_encoding_selection;

		/******************************************************************/
		/* enum ang::text::encoding :                                     */
		/*  -> represents the text encoding                               */
		/******************************************************************/
		ANG_BEGIN_ENUM(LINK, encoding, uint)
			none = 0,
			binary = none,
			ascii,
			unicode,
			utf8,
			utf16_le,
			utf16_be,
			utf16,
			utf16_se,
			utf32_le,
			utf32_be,
			utf32,
			utf32_se,
			auto_detect = 0XFFFFFFFF,
			ANG_END_ENUM(encoding);

		ANG_BEGIN_ENUM(LINK, special_chars, char32_t)
			repl = 0XFFFDU,//Replcement Character
			ubom = 0XFEFFU,//Unicode Byte Order Mark
			mbom = 0XEFBBBFU,//MultiByte Order Mark
			null = 0XC080U,//modified utf-8, null character
			inv_repl = 0XFDFFU,//Inverted Replcement Character
			inv_ubom = 0XFFFEU,//Inverted Unicode Byte Order Mark
			inv_mbom = 0XBFBBEFU,//Inverted MultiByte Order Mark
			inv_null = 0X80C0U,//Inverted modified utf-8, null character
			ANG_END_ENUM(special_chars);

		template<encoding_enum ENCODING> struct char_type_by_encoding;

		template<typename T> struct encoding_by_type {
			static constexpr encoding_enum encoding() { return encoding::auto_detect; }
		};

		template<> 	struct char_type_by_encoding<encoding::ascii> {
			typedef ascii_char_t char_t;
			typedef unsafe_ascii_str_t str_t;
			typedef unsafe_ascii_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::unicode> {
			typedef unicode_char_t char_t;
			typedef unsafe_unicode_str_t str_t;
			typedef unsafe_unicode_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf8> {
			typedef utf8_char_t char_t;
			typedef unsafe_utf8_str_t str_t;
			typedef unsafe_utf8_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf16> {
			typedef utf16_char_t char_t;
			typedef unsafe_utf16_str_t str_t;
			typedef unsafe_utf16_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf16_le> {
			typedef utf16_char_t char_t;
			typedef unsafe_utf16_str_t str_t;
			typedef unsafe_utf16_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf16_be> {
			typedef utf16_char_t char_t;
			typedef unsafe_utf16_str_t str_t;
			typedef unsafe_utf16_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf16_se> {
			typedef utf16_char_t char_t;
			typedef unsafe_utf16_str_t str_t;
			typedef unsafe_utf16_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf32> {
			typedef utf32_char_t char_t;
			typedef unsafe_utf32_str_t str_t;
			typedef unsafe_utf32_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf32_le> {
			typedef utf32_char_t char_t;
			typedef unsafe_utf32_str_t str_t;
			typedef unsafe_utf32_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf32_be> {
			typedef utf32_char_t char_t;
			typedef unsafe_utf32_str_t str_t;
			typedef unsafe_utf32_cstr_t cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf32_se> {
			typedef utf32_char_t char_t;
			typedef unsafe_utf32_str_t str_t;
			typedef unsafe_utf32_cstr_t cstr_t;
		};

		template<wsize SIZE> struct encoding_by_size {
			static constexpr encoding_enum encoding() { return encoding::none; }
		};

		template<> struct encoding_by_size<1U> {
			typedef utf8_char_t char_t;
			typedef unsafe_utf8_str_t str_t;
			typedef unsafe_utf8_cstr_t cstr_t;
			static constexpr encoding_enum encoding() { return encoding::utf8; }
		};
		template<> struct encoding_by_size<2U> {
			typedef utf16_char_t char_t;
			typedef unsafe_utf16_str_t str_t;
			typedef unsafe_utf16_cstr_t cstr_t;
			static constexpr encoding_enum encoding() { return encoding::utf16; }
		};
		template<> struct encoding_by_size<4U> {
			typedef utf32_char_t char_t;
			typedef unsafe_utf32_str_t str_t;
			typedef unsafe_utf32_cstr_t cstr_t;
			static constexpr encoding_enum encoding() { return encoding::utf32; }
		};


		template<typename T> struct native_encoding_selection : public encoding_by_size<sizeof(T)> {};

		template<> struct encoding_by_type<ascii_char_t> {
			static constexpr encoding_enum encoding() { return encoding::ascii; }
		};

		template<> struct encoding_by_type<unicode_char_t> {
			static constexpr encoding_enum encoding() { return encoding::unicode; }
		};

		template<> struct encoding_by_type<utf8_char_t> {
			static constexpr encoding_enum encoding() { return encoding::utf8; }
		};

		template<> struct encoding_by_type<utf16_char_t> {
			static constexpr encoding_enum encoding() { return encoding::utf16; }
		};

		template<> struct encoding_by_type<utf32_char_t> {
			static constexpr encoding_enum encoding() { return encoding::utf32; }
		};

		template<typename T> struct encoding_by_type<T*> {
			static constexpr encoding_enum encoding() { return encoding_by_type<T>::encoding(); }
		};

		template<typename T> struct encoding_by_type<T const*> {
			static constexpr encoding_enum encoding() { return encoding_by_type<T>::encoding(); }
		};

		template<typename T> struct encoding_by_type<safe_str<T>> {
			static constexpr encoding_enum encoding() { return encoding_by_type<T>::encoding(); }
		};

		template<typename T> struct encoding_by_type<safe_str<const T>> {
			static constexpr encoding_enum encoding() { return encoding_by_type<T>::encoding(); }
		};

		template<typename T> struct char_type_by_type {
			typedef T char_t;
			typedef T* str_t;
			typedef T const* cstr_t;
		};

		template<typename T> struct char_type_by_type<const T> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<T&> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<T&&> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<T const&> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<T*> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<T const*> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<safe_str<T>> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

		template<typename T> struct char_type_by_type<safe_str<const T>> {
			typedef typename char_type_by_type<T>::char_t char_t;
			typedef typename char_type_by_type<T>::str_t str_t;
			typedef typename char_type_by_type<T>::cstr_t cstr_t;
		};

	}

	typedef struct raw_str {
		pointer _value;
		wsize _size;
		text::encoding _encoding;

		raw_str();
		raw_str(pointer v, wsize s, text::encoding e);
		raw_str(raw_str const& str);

		template<typename T> inline raw_str(safe_str<T> const& str);

		inline pointer ptr()const;
		inline wsize size()const;
		inline wsize count()const;
		inline wsize char_size()const;
		inline text::encoding_t encoding()const;

		template<typename T> inline operator safe_str<T>();
		template<typename T> inline operator safe_str<T const>()const;
		template<typename T> inline safe_str<T> to_str();
		template<typename T> inline safe_str<T const> to_cstr()const;
	}raw_str_t;
	
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
	}

	namespace text
	{
		struct encoder_interface
		{
			static LINK void initialize_interface(encoder_interface*, encoding_t);
			encoding_t(*_format)();
			wsize(*_length)(pointer);
			wsize(*_size)(pointer, encoding_t);
			char32_t(*_to_utf32)(pointer, windex&);
			int(*_compare_string)(pointer, pointer, encoding_t);
			wsize(*_compare_string_until)(pointer, pointer, encoding_t);
			wsize(*_convert_string)(pointer, wsize, pointer, encoding_t, bool);
			windex(*_find)(pointer, wsize, pointer, wsize, encoding_t, windex);
			windex(*_find_revert)(pointer, wsize, pointer, wsize, encoding_t, windex);
		};

		template<encoding_enum ENCODING>
		class encoder
			: private encoder_interface
			, rule<(ENCODING <= encoding::utf32_se)>
		{
		public:
			typedef typename char_type_by_encoding<ENCODING>::char_t char_t;
			typedef typename char_type_by_encoding<ENCODING>::str_t str_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t cstr_t;

			encoder() { initialize_interface(this, ENCODING); }

			inline text::encoding_t format()const { return _format(); }
			inline wsize length(cstr_t str)const { return _length((pointer)str); }

			template<typename T> inline wsize size(T const* str)const { return _size((pointer)str, encoding_by_type<typename char_type_by_type<T>::char_t>::encoding()); }
			template<typename T> inline wsize size(safe_str<T> str)const { return _size((pointer)str.get(), encoding_by_type<typename char_type_by_type<T>::char_t>::encoding()); }
			inline wsize size(raw_str_t str)const { return _size(str.ptr(), str.encoding()); }
			inline wsize size(pointer str, text::encoding_t format)const { return _size((pointer)str, format); }

			template<typename T> inline int compare(cstr_t cstr1, T const* cstr2)const { return _compare_string((pointer)cstr1, (pointer)cstr2, encoding_by_type<typename char_type_by_type<T>::char_t>::encoding()); }
			template<typename T> inline int compare(cstr_t cstr1, safe_str<T> cstr2)const { return _compare_string((pointer)cstr1, (pointer)cstr2.get(), encoding_by_type<typename char_type_by_type<T>::char_t>::encoding()); }
			inline int compare(cstr_t cstr1, raw_str_t cstr2)const { return _compare_string((pointer)cstr1, cstr2.ptr(), cstr2.encoding()); }
			inline int compare(cstr_t cstr1, pointer cstr2, text::encoding_t format)const { return _compare_string((pointer)cstr1, cstr2, format); }

			template<typename T> inline wsize compare_until(cstr_t cstr1, T const* cstr2)const { return _compare_string_until((pointer)cstr1, (pointer)cstr2, encoding_by_type<typename char_type_by_type<T>::char_t>::encoding()); }
			template<typename T> inline wsize compare_until(cstr_t cstr1, safe_str<T> cstr2)const { return _compare_string_until((pointer)cstr1, (pointer)cstr2.get(), encoding_by_type<typename char_type_by_type<T>::char_t>::encoding()); }
			inline wsize compare_until(cstr_t cstr1, raw_str_t cstr2)const { return _compare_string_until((pointer)cstr1, cstr2.ptr(), cstr2.encoding()); }
			inline wsize compare_until(cstr_t cstr1, pointer cstr2, text::encoding_t format)const { return _compare_string_until((pointer)cstr1, cstr2, format); }

			template<typename T> inline wsize convert(str_t dest, wsize maxsize, T const* src, bool end_of_string = true)const { return _convert_string((pointer)dest, maxsize, (pointer)src, encoding_by_type<typename char_type_by_type<T>::char_t>::encoding(), end_of_string); }
			template<typename T> inline wsize convert(str_t dest, wsize maxsize, safe_str<T> src, bool end_of_string = true)const { return _convert_string((pointer)dest, maxsize, (pointer)src.get(), encoding_by_type<typename char_type_by_type<T>::char_t>::encoding(), end_of_string); }
			inline wsize convert(str_t dest, wsize maxsize, raw_str_t src, bool end_of_string = true)const { return _convert_string((pointer)dest, maxsize, src.ptr(), src.encoding(), end_of_string); }
			inline wsize convert(str_t dest, wsize maxsize, pointer src, text::encoding_t format, bool end_of_string = true)const { return _convert_string((pointer)dest, maxsize, src, format, end_of_string); }

			template<typename T> windex find(cstr_t first, wsize s1, T const* second, wsize s2, windex start)const { return _find((pointer)first, s1, (pointer)second, s2, encoding_by_type<typename char_type_by_type<T>::char_t>::encoding(), start); }
			template<typename T> windex find(cstr_t first, wsize s1, safe_str<T> second, windex start)const { return _find((pointer)first, s1, (pointer)second.get(), second.size(), encoding_by_type<typename char_type_by_type<T>::char_t>::encoding(), start); }
			windex find(cstr_t first, wsize s1, raw_str_t second, windex start)const { return _find((pointer)first, s1, second.ptr(), second.count(), second.encoding(), start); }
			windex find(cstr_t first, wsize s1, pointer second, wsize s2, text::encoding_t format, windex start)const { return _find((pointer)first, s1, second, s2, format, start); }

			template<typename T> windex find_revert(cstr_t first, wsize s1, T const* second, wsize s2, windex start)const { return _find_revert((pointer)first, s1, (pointer)second, s2, encoding_by_type<typename char_type_by_type<T>::char_t>::encoding(), min(s1, start)); }
			template<typename T> windex find_revert(cstr_t first, wsize s1, safe_str<T> second, windex start)const { return _find_revert((pointer)first, s1, (pointer)second.get(), second.size(), encoding_by_type<typename char_type_by_type<T>::char_t>::encoding(), min(s1, start)); }
			windex find_revert(cstr_t first, wsize s1, raw_str_t second, windex start)const { return _find_revert((pointer)first, s1, second.ptr(), second.count(), second.encoding(), min(s1, start)); }
			windex find_revert(cstr_t first, wsize s1, pointer second, wsize s2, text::encoding_t format, windex start)const { return _find_revert((pointer)first, s1, second, s2, format, min(s1,start)); }
		};


		typedef encoder<encoding::ascii> ASCII_t;
		typedef encoder<encoding::unicode> UNICODE_t;
		typedef encoder<encoding::utf8> UTF8_t;
		typedef encoder<encoding::utf16> UTF16_t;
		typedef encoder<encoding::utf16_se> UTF16_SE_t;
		typedef encoder<encoding::utf16_le> UTF16_LE_t;
		typedef encoder<encoding::utf16_be> UTF16_BE_t;
		typedef encoder<encoding::utf32> UTF32_t;
		typedef encoder<encoding::utf32_se> UTF32_SE_t;
		typedef encoder<encoding::utf32_le> UTF32_LE_t;
		typedef encoder<encoding::utf32_be> UTF32_BE_t;

		template<encoding_enum ENCODING>
		inline encoder<ENCODING> const& get_encoder() {
			static encoder<ENCODING> _encoder; return _encoder;
		}

		template<typename T>
		inline encoder<encoding_by_type<T>::encoding()> const& get_encoder_by_type() {
			return get_encoder<encoding_by_type<T>::encoding()>();
		}

		inline const ASCII_t& ASCII() { return get_encoder<encoding::ascii>(); }
		inline const UNICODE_t& UNICODE() { return get_encoder<encoding::unicode>(); }
		inline const UTF8_t& UTF8() { return get_encoder<encoding::utf8>(); }
		inline const UTF16_t& UTF16() { return get_encoder<encoding::utf16>(); }
		inline const UTF16_SE_t& UTF16_SE() { return get_encoder<encoding::utf16_se>(); }
		inline const UTF16_LE_t& UTF16_LE() { return get_encoder<encoding::utf16_le>(); }
		inline const UTF16_BE_t& UTF16_BE() { return get_encoder<encoding::utf16_be>(); }
		inline const UTF32_t& UTF32() { return get_encoder<encoding::utf32>(); }
		inline const UTF32_SE_t& UTF32_SE() { return get_encoder<encoding::utf32_se>(); }
		inline const UTF32_LE_t& UTF32_LE() { return get_encoder<encoding::utf32_le>(); }
		inline const UTF32_BE_t& UTF32_BE() { return get_encoder<encoding::utf32_be>(); }

		template<>
		class encoder<encoding::auto_detect>
		{
		public:
			inline text::encoding_t format()const { return encoding::auto_detect; }
			inline wsize char_size_by_encoding(text::encoding_t encoding)const {
				switch (encoding.get())
				{
				case text::encoding::unicode:
					return sizeof(wchar_t);
				case text::encoding::utf16:
				case text::encoding::utf16_be:
				case text::encoding::utf16_le:
				case text::encoding::utf16_se:
					return sizeof(char16_t);
				case text::encoding::utf32:
				case text::encoding::utf32_be:
				case text::encoding::utf32_le:
				case text::encoding::utf32_se:
					return sizeof(char32_t);
				default:
				//case text::encoding::ascii:
				//case text::encoding::utf8:
					return sizeof(char);
				}
			}

			template<typename T>
			inline wsize length(T cstr)const { 
				return get_encoder_by_type<T>().length((typename char_type_by_type<T>::cstr_t)cstr);
			}

			template<typename T1, typename T2>
			inline wsize size(T2 cstr)const { 
				return get_encoder_by_type<T1>().size((typename char_type_by_type<T2>::cstr_t)cstr);
			}

			template<typename T1, typename T2>
			inline int compare(T1 cstr1, T2 cstr2)const { 
				return get_encoder_by_type<T1>().compare((typename char_type_by_type<T1>::cstr_t)cstr1, (typename char_type_by_type<T2>::cstr_t)cstr2);
			}

			template<typename T1, typename T2>
			inline wsize compare_until(T1 cstr1, T2 cstr2)const {
				return get_encoder_by_type<T1>().compare_until((typename char_type_by_type<T1>::cstr_t)cstr1, (typename char_type_by_type<T2>::cstr_t)cstr2);
			}

			template<typename T1, typename T2>
			inline wsize convert(T1 dest, wsize maxsize, T2 src, bool end_of_string = true)const { 
				return get_encoder_by_type<T1>().convert((typename char_type_by_type<T1>::str_t)dest, maxsize, (typename char_type_by_type<T2>::cstr_t)src, end_of_string);
			}
		};


		class string_util
		{
		private:
			raw_str_t _data;
			wsize _char_size;
			encoder_interface _encoder;
		public:
			string_util(raw_str_t data) : _data(data), _char_size(_data.char_size()){
				encoder_interface::initialize_interface(&_encoder, data.encoding());
			}

			string_util(const string_util& str) : _data(str._data), _char_size(_data.char_size()) {
				encoder_interface::initialize_interface(&_encoder, _data.encoding());
			}

			inline raw_str_t data()const { return _data; }

			template<typename T>
			windex find(safe_str<T> str, windex start = 0, windex end = invalid_index)const {
				return _encoder._find(_data.ptr(), min(_data.size() / _char_size, end),
					(pointer)str.cstr(), str.size(), encoding_by_type<typename char_type_by_type<T>::char_t>::encoding(),
					start);
			}

			template<typename T>
			windex sub_string(safe_str<T> str, windex start = 0, windex end = invalid_index)const {
				get_encoder_by_type<typename char_type_by_type<T>::char_t>().convert(str.str(), min(_data.size() / _char_size, end) - start, _data.ptr(), _data.encoding(), true);
			}

			template<encoding_enum ENCODING>
			windex sub_string(strings::string_base<ENCODING>& str, windex start = 0, windex end = invalid_index)const;

			template<typename T>char32_t operator[](T const& idx) {
				wsize _idx = min(idx, _data.size() / _char_size);
				return _encoder._to_utf32(_data.ptr(), _idx);
			}
		};

		typedef encoder<encoding::auto_detect> encoder_t;

		template< bool SWAP, typename T> inline T swap_endian(T value) { return value; }
		template<> inline char16_t  swap_endian<true, char16_t>(char16_t value) {
			return char16_t((word(value) & 0XFF00) >> 8) | ((word(value) & 0X00FF) << 8);
		}
		template<> inline char32_t swap_endian<true, char32_t>(char32_t value) {
			return char32_t((dword(value) & 0XFF000000) >> 24) | ((dword(value) & 0X00FF0000) >> 8) | ((dword(value) & 0X0000FF00) << 8) | ((dword(value) & 0X000000FF) << 24);
		}

		LINK bool is_little_endian();

		template<encoding_enum ENCODING>
		inline constexpr bool is_endian_swapped() { return false; }

		template<encoding_enum ENCODING>
		inline encoding_enum native_encoding() { return ENCODING; }

		template<> inline bool is_endian_swapped<encoding::utf16_le>() { return !is_little_endian(); }
		template<> inline bool is_endian_swapped<encoding::utf16_be>() { return is_little_endian(); }
		template<> inline bool is_endian_swapped<encoding::utf32_le>() { return !is_little_endian(); }
		template<> inline bool is_endian_swapped<encoding::utf32_be>() { return is_little_endian(); }

		template<> inline constexpr encoding_enum native_encoding<encoding::unicode>() { return encoding_by_size<sizeof(wchar)>::encoding(); }

		template<> inline encoding_enum native_encoding<encoding::utf16_le>() { return is_little_endian() ? encoding::utf16 : encoding::utf16_se; }
		template<> inline encoding_enum native_encoding<encoding::utf16_be>() { return is_little_endian() ? encoding::utf16_se : encoding::utf16; }
		template<> inline encoding_enum native_encoding<encoding::utf32_le>() { return is_little_endian() ? encoding::utf32 : encoding::utf32_se; }
		template<> inline encoding_enum native_encoding<encoding::utf32_be>() { return is_little_endian() ? encoding::utf32_se : encoding::utf32; }

		template<> inline encoding_enum native_encoding<encoding::utf16>() { return is_little_endian() ? encoding::utf16_le : encoding::utf16_be; }
		template<> inline encoding_enum native_encoding<encoding::utf16_se>() { return is_little_endian() ? encoding::utf16_be : encoding::utf16_le; }
		template<> inline encoding_enum native_encoding<encoding::utf32>() { return is_little_endian() ? encoding::utf32_le : encoding::utf32_be; }
		template<> inline encoding_enum native_encoding<encoding::utf32_se>() { return is_little_endian() ? encoding::utf32_be : encoding::utf32_le; }
	}


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

		struct algorithms
		{
			template<typename cstr_t>
			inline static windex string_length(cstr_t cstr) {
				return get_encoder_by_type<cstr_t>().length(cstr);
			}

			template<typename char_t>
			inline static windex string_length(safe_str<char_t> cstr) {
				return cstr.size();
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
				case encoding::utf16_se:return get_encoder<encoding::utf16_se>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::utf16:return get_encoder<encoding::utf16>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::utf32_le:return get_encoder<encoding::utf32_le>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::utf32_be:return get_encoder<encoding::utf32_be>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
				case encoding::utf32_se:return get_encoder<encoding::utf32_se>().size((typename char_type_by_type<cstr_t>::cstr_t)cstr);
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

}


#include <ang/base/inline/string.inl>

#endif//__ANG_BASE_TEXT_HPP__
