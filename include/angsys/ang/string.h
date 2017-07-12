/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_STRINGS_H__
#define __ANG_STRINGS_H__


namespace ang
{

	template<>
	class LINK object_wrapper<strings::wstring_buffer>
	{
	public:
		typedef strings::wstring_buffer type;

	private:
		strings::wstring_buffer* _ptr;

	public:
		object_wrapper();

		object_wrapper(cstr_t);
		object_wrapper(cwstr_t);
		object_wrapper(cmstr_t);

		object_wrapper(strings::wstring_buffer*);
		object_wrapper(ang::nullptr_t const&);

		object_wrapper(object_wrapper<strings::wstring_buffer> &&);
		object_wrapper(object_wrapper<strings::wstring_buffer> const&);
		object_wrapper(object_wrapper<strings::string_buffer> const&);
		object_wrapper(object_wrapper<strings::mstring_buffer> const&);

		template<wsize N> object_wrapper(const wchar(&ar)[N]);
		template<wsize N> object_wrapper(const char(&ar)[N]);

		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		strings::wstring_buffer* get(void)const;
		void set(strings::wstring_buffer*);
		strings::wstring_buffer ** addres_of(void);

	public:
		object_wrapper& operator = (object_wrapper<strings::wstring_buffer> &&);
		object_wrapper& operator = (object_wrapper<strings::wstring_buffer> const&);
		object_wrapper& operator = (object_wrapper<strings::string_buffer> const&);
		object_wrapper& operator = (object_wrapper<strings::mstring_buffer> const&);
		object_wrapper& operator = (ang::nullptr_t const&);
		object_wrapper& operator = (strings::wstring_buffer*);

		object_wrapper& operator = (cstr_t);
		object_wrapper& operator = (cwstr_t);
		object_wrapper& operator = (cmstr_t);
		template<wsize N> inline object_wrapper& operator = (const wchar(&ar)[N]);
		template<wsize N> object_wrapper& operator = (const char(&ar)[N]);

		object_wrapper& operator += (object_wrapper<strings::wstring_buffer> const&);

		object_wrapper& operator += (cstr_t);
		object_wrapper& operator += (cwstr_t);
		object_wrapper& operator += (cmstr_t);
		template<wsize N> inline object_wrapper& operator += (const wchar(&ar)[N]);
		template<wsize N> object_wrapper& operator += (const char(&ar)[N]);

		inline operator object_t()const;

		object_wrapper_ptr<strings::wstring_buffer> operator & (void);
		strings::wstring_buffer * operator -> (void);
		strings::wstring_buffer const* operator -> (void)const;
		explicit operator strings::wstring_buffer * (void);
		explicit operator strings::wstring_buffer const* (void)const;

		operator wstr_t (void);
		operator cwstr_t (void)const;

		wchar& operator [] (int);
		wchar operator [] (int)const;

		friend LINK wstring operator + (wstring const&, wstring const&);
		friend LINK wstring operator + (wstring const&, string const&);
		friend LINK wstring operator + (wstring const&, mstring const&);
		friend LINK wstring operator + (wstring const&, cwstr_t);
		friend LINK wstring operator + (cwstr_t, wstring const&);
		friend LINK wstring operator + (wstring const&, cstr_t);
		friend LINK wstring operator + (cstr_t, wstring const&);

		friend LINK wstring& operator << (wstring&, wstring const&);
		friend LINK wstring& operator << (wstring&, string const&);
		friend LINK wstring& operator << (wstring&, cwstr_t);
		friend LINK wstring& operator << (wstring&, cstr_t);
		friend LINK wstring& operator << (wstring&, wchar);
		friend LINK wstring& operator << (wstring&, char);

		friend LINK bool operator == (wstring const& first, wstring const& second);
		friend LINK bool operator != (wstring const& first, wstring const& second);
		friend LINK bool operator >= (wstring const& first, wstring const& second);
		friend LINK bool operator <= (wstring const& first, wstring const& second);
		friend LINK bool operator > (wstring const& first, wstring const& second);
		friend LINK bool operator < (wstring const& first, wstring const& second);

		friend LINK bool operator == (wstring const& first, string const& second);
		friend LINK bool operator != (wstring const& first, string const& second);
		friend LINK bool operator >= (wstring const& first, string const& second);
		friend LINK bool operator <= (wstring const& first, string const& second);
		friend LINK bool operator > (wstring const& first, string const& second);
		friend LINK bool operator < (wstring const& first, string const& second);

		friend LINK bool operator == (wstring const& first, mstring const& second);
		friend LINK bool operator != (wstring const& first, mstring const& second);
		friend LINK bool operator >= (wstring const& first, mstring const& second);
		friend LINK bool operator <= (wstring const& first, mstring const& second);
		friend LINK bool operator > (wstring const& first, mstring const& second);
		friend LINK bool operator < (wstring const& first, mstring const& second);

		friend LINK bool operator == (wstring const& first, cwstr_t second);
		friend LINK bool operator != (wstring const& first, cwstr_t second);
		friend LINK bool operator >= (wstring const& first, cwstr_t second);
		friend LINK bool operator <= (wstring const& first, cwstr_t second);
		friend LINK bool operator > (wstring const& first, cwstr_t second);
		friend LINK bool operator < (wstring const& first, cwstr_t second);

		friend LINK bool operator == (cwstr_t first, wstring const& second);
		friend LINK bool operator != (cwstr_t first, wstring const& second);
		friend LINK bool operator >= (cwstr_t first, wstring const& second);
		friend LINK bool operator <= (cwstr_t first, wstring const& second);
		friend LINK bool operator > (cwstr_t first, wstring const& second);
		friend LINK bool operator < (cwstr_t first, wstring const& second);

		friend LINK bool operator == (wstring const& first, cstr_t second);
		friend LINK bool operator != (wstring const& first, cstr_t second);
		friend LINK bool operator >= (wstring const& first, cstr_t second);
		friend LINK bool operator <= (wstring const& first, cstr_t second);
		friend LINK bool operator > (wstring const& first, cstr_t second);
		friend LINK bool operator < (wstring const& first, cstr_t second);

		friend LINK bool operator == (cstr_t first, wstring const& second);
		friend LINK bool operator != (cstr_t first, wstring const& second);
		friend LINK bool operator >= (cstr_t first, wstring const& second);
		friend LINK bool operator <= (cstr_t first, wstring const& second);
		friend LINK bool operator > (cstr_t first, wstring const& second);
		friend LINK bool operator < (cstr_t first, wstring const& second);

		friend LINK bool operator == (wstring const& first, cmstr_t second);
		friend LINK bool operator != (wstring const& first, cmstr_t second);
		friend LINK bool operator >= (wstring const& first, cmstr_t second);
		friend LINK bool operator <= (wstring const& first, cmstr_t second);
		friend LINK bool operator > (wstring const& first, cmstr_t second);
		friend LINK bool operator < (wstring const& first, cmstr_t second);

		friend LINK bool operator == (cmstr_t first, wstring const& second);
		friend LINK bool operator != (cmstr_t first, wstring const& second);
		friend LINK bool operator >= (cmstr_t first, wstring const& second);
		friend LINK bool operator <= (cmstr_t first, wstring const& second);
		friend LINK bool operator > (cmstr_t first, wstring const& second);
		friend LINK bool operator < (cmstr_t first, wstring const& second);
	};

	template<>
	class LINK object_wrapper<strings::string_buffer>
	{
	public:
		typedef strings::string_buffer type;

	private:
		strings::string_buffer* _ptr;

	public:
		object_wrapper();
		object_wrapper(cstr_t);
		object_wrapper(cwstr_t);
		object_wrapper(cmstr_t);

		object_wrapper(strings::string_buffer*);
		object_wrapper(ang::nullptr_t const&);

		object_wrapper(object_wrapper<strings::string_buffer> &&);
		object_wrapper(object_wrapper<strings::string_buffer> const&);
		object_wrapper(object_wrapper<strings::wstring_buffer> const&);
		object_wrapper(object_wrapper<strings::mstring_buffer> const&);

		template<wsize N> object_wrapper(const wchar(&ar)[N]);
		template<wsize N> object_wrapper(const char(&ar)[N]);

		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		strings::string_buffer* get(void)const;
		void set(strings::string_buffer*);
		strings::string_buffer ** addres_of(void);

	public:
		object_wrapper& operator = (object_wrapper<strings::string_buffer> &&);
		object_wrapper& operator = (object_wrapper<strings::string_buffer> const&);
		object_wrapper& operator = (object_wrapper<strings::wstring_buffer> const&);
		object_wrapper& operator = (object_wrapper<strings::mstring_buffer> const&);
		object_wrapper& operator = (ang::nullptr_t const&);
		object_wrapper& operator = (strings::string_buffer*);

		object_wrapper& operator = (cstr_t);
		object_wrapper& operator = (cwstr_t);
		object_wrapper& operator = (cmstr_t);
		template<wsize N> inline object_wrapper& operator = (const wchar(&ar)[N]);
		template<wsize N> object_wrapper& operator = (const char(&ar)[N]);

		object_wrapper& operator += (object_wrapper<strings::string_buffer> const&);

		object_wrapper& operator += (cstr_t);
		object_wrapper& operator += (cwstr_t);
		object_wrapper& operator += (cmstr_t);
		template<wsize N> inline object_wrapper& operator += (const wchar(&ar)[N]);
		template<wsize N> object_wrapper& operator += (const char(&ar)[N]);

		inline operator object_t()const;

		object_wrapper_ptr<strings::string_buffer> operator & (void);
		strings::string_buffer * operator -> (void);
		strings::string_buffer const* operator -> (void)const;
		explicit operator strings::string_buffer * (void);
		explicit operator strings::string_buffer const* (void)const;

		operator str_t (void);
		operator cstr_t (void)const;

		char& operator [] (int);
		char operator [] (int)const;

		friend LINK string operator + (string const&, string const&);
		friend LINK string operator + (string const&, wstring const&);
		friend LINK string operator + (string const&, mstring const&);
		friend LINK string operator + (string const&, cstr_t);
		friend LINK string operator + (cstr_t, string const&);
		friend LINK string operator + (string const&, cwstr_t);
		friend LINK string operator + (cwstr_t, string const&);

		friend LINK string& operator << (string&, wstring const&);
		friend LINK string& operator << (string&, string const&);
		friend LINK string& operator << (string&, cwstr_t);
		friend LINK string& operator << (string&, cstr_t);
		friend LINK string& operator << (string&, wchar);
		friend LINK string& operator << (string&, char);

		friend LINK bool operator == (string const& first, string const& second);
		friend LINK bool operator != (string const& first, string const& second);
		friend LINK bool operator >= (string const& first, string const& second);
		friend LINK bool operator <= (string const& first, string const& second);
		friend LINK bool operator > (string const& first, string const& second);
		friend LINK bool operator < (string const& first, string const& second);

		friend LINK bool operator == (string const& first, wstring const& second);
		friend LINK bool operator != (string const& first, wstring const& second);
		friend LINK bool operator >= (string const& first, wstring const& second);
		friend LINK bool operator <= (string const& first, wstring const& second);
		friend LINK bool operator > (string const& first, wstring const& second);
		friend LINK bool operator < (string const& first, wstring const& second);

		friend LINK bool operator == (string const& first, mstring const& second);
		friend LINK bool operator != (string const& first, mstring const& second);
		friend LINK bool operator >= (string const& first, mstring const& second);
		friend LINK bool operator <= (string const& first, mstring const& second);
		friend LINK bool operator > (string const& first, mstring const& second);
		friend LINK bool operator < (string const& first, mstring const& second);

		friend LINK bool operator == (string const& first, cwstr_t second);
		friend LINK bool operator != (string const& first, cwstr_t second);
		friend LINK bool operator >= (string const& first, cwstr_t second);
		friend LINK bool operator <= (string const& first, cwstr_t second);
		friend LINK bool operator > (string const& first, cwstr_t second);
		friend LINK bool operator < (string const& first, cwstr_t second);

		friend LINK bool operator == (cwstr_t first, string const& second);
		friend LINK bool operator != (cwstr_t first, string const& second);
		friend LINK bool operator >= (cwstr_t first, string const& second);
		friend LINK bool operator <= (cwstr_t first, string const& second);
		friend LINK bool operator > (cwstr_t first, string const& second);
		friend LINK bool operator < (cwstr_t first, string const& second);

		friend LINK bool operator == (string const& first, cstr_t second);
		friend LINK bool operator != (string const& first, cstr_t second);
		friend LINK bool operator >= (string const& first, cstr_t second);
		friend LINK bool operator <= (string const& first, cstr_t second);
		friend LINK bool operator > (string const& first, cstr_t second);
		friend LINK bool operator < (string const& first, cstr_t second);

		friend LINK bool operator == (cstr_t first, string const& second);
		friend LINK bool operator != (cstr_t first, string const& second);
		friend LINK bool operator >= (cstr_t first, string const& second);
		friend LINK bool operator <= (cstr_t first, string const& second);
		friend LINK bool operator > (cstr_t first, string const& second);
		friend LINK bool operator < (cstr_t first, string const& second);

		friend LINK bool operator == (string const& first, cmstr_t second);
		friend LINK bool operator != (string const& first, cmstr_t second);
		friend LINK bool operator >= (string const& first, cmstr_t second);
		friend LINK bool operator <= (string const& first, cmstr_t second);
		friend LINK bool operator > (string const& first, cmstr_t second);
		friend LINK bool operator < (string const& first, cmstr_t second);

		friend LINK bool operator == (cmstr_t first, string const& second);
		friend LINK bool operator != (cmstr_t first, string const& second);
		friend LINK bool operator >= (cmstr_t first, string const& second);
		friend LINK bool operator <= (cmstr_t first, string const& second);
		friend LINK bool operator > (cmstr_t first, string const& second);
		friend LINK bool operator < (cmstr_t first, string const& second);
	};

	template<>
	class LINK object_wrapper<strings::mstring_buffer>
	{
	public:
		typedef strings::mstring_buffer type;

	private:
		strings::mstring_buffer* _ptr;

	public:
		object_wrapper();
		object_wrapper(strings::mstring_buffer*);
		object_wrapper(ang::nullptr_t const&);
		object_wrapper(cmstr_t);
		object_wrapper(cwstr_t);
		object_wrapper(cstr_t);
		object_wrapper(object_wrapper<strings::mstring_buffer> &&);
		object_wrapper(object_wrapper<strings::mstring_buffer> const&);
		object_wrapper(object_wrapper<strings::wstring_buffer> const&);
		object_wrapper(object_wrapper<strings::string_buffer> const&);

		template<wsize N> object_wrapper(const wchar(&ar)[N]);
		template<wsize N> object_wrapper(const char(&ar)[N]);

		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		strings::mstring_buffer* get(void)const;
		void set(strings::mstring_buffer*);
		strings::mstring_buffer ** addres_of(void);

	public:
		object_wrapper& operator = (object_wrapper<strings::mstring_buffer> &&);
		object_wrapper& operator = (object_wrapper<strings::mstring_buffer> const&);
		object_wrapper& operator = (object_wrapper<strings::wstring_buffer> const&);
		object_wrapper& operator = (object_wrapper<strings::string_buffer> const&);
		object_wrapper& operator = (ang::nullptr_t const&);
		object_wrapper& operator = (strings::mstring_buffer*);
		object_wrapper& operator = (cmstr_t);
		object_wrapper& operator = (cwstr_t);
		object_wrapper& operator = (cstr_t);

		template<wsize N> inline object_wrapper& operator = (const wchar(&ar)[N]);
		template<wsize N> object_wrapper& operator = (const char(&ar)[N]);

		object_wrapper& operator += (object_wrapper<strings::mstring_buffer> const&);
		object_wrapper& operator += (cmstr_t);
		object_wrapper& operator += (cwstr_t);
		object_wrapper& operator += (cstr_t);

		template<wsize N> inline object_wrapper& operator += (const wchar(&ar)[N]);
		template<wsize N> object_wrapper& operator += (const char(&ar)[N]);

		inline operator object_t()const;

		object_wrapper_ptr<strings::mstring_buffer> operator & (void);
		strings::mstring_buffer * operator -> (void);
		strings::mstring_buffer const* operator -> (void)const;
		explicit operator strings::mstring_buffer * (void);
		explicit operator strings::mstring_buffer const* (void)const;
		operator mstr_t (void);
		operator cmstr_t (void)const;



		friend LINK mstring operator + (mstring const&, mstring const&);
		friend LINK mstring operator + (mstring const&, wstring const&);
		friend LINK mstring operator + (mstring const&, string const&);
		friend LINK mstring operator + (mstring const&, cwstr_t);
		friend LINK mstring operator + (cwstr_t, mstring const&);
		friend LINK mstring operator + (mstring const&, cstr_t);
		friend LINK mstring operator + (cstr_t, mstring const&);

		friend LINK mstring& operator << (mstring&, wstring const&);
		friend LINK mstring& operator << (mstring&, string const&);
		friend LINK mstring& operator << (mstring&, cwstr_t);
		friend LINK mstring& operator << (mstring&, cstr_t);
		friend LINK mstring& operator << (mstring&, wchar);
		friend LINK mstring& operator << (mstring&, char);

		friend LINK bool operator == (mstring const& first, mstring const& second);
		friend LINK bool operator != (mstring const& first, mstring const& second);
		friend LINK bool operator >= (mstring const& first, mstring const& second);
		friend LINK bool operator <= (mstring const& first, mstring const& second);
		friend LINK bool operator > (mstring const& first, mstring const& second);
		friend LINK bool operator < (mstring const& first, mstring const& second);

		friend LINK bool operator == (mstring const& first, wstring const& second);
		friend LINK bool operator != (mstring const& first, wstring const& second);
		friend LINK bool operator >= (mstring const& first, wstring const& second);
		friend LINK bool operator <= (mstring const& first, wstring const& second);
		friend LINK bool operator > (mstring const& first, wstring const& second);
		friend LINK bool operator < (mstring const& first, wstring const& second);

		friend LINK bool operator == (mstring const& first, string const& second);
		friend LINK bool operator != (mstring const& first, string const& second);
		friend LINK bool operator >= (mstring const& first, string const& second);
		friend LINK bool operator <= (mstring const& first, string const& second);
		friend LINK bool operator > (mstring const& first, string const& second);
		friend LINK bool operator < (mstring const& first, string const& second);

		friend LINK bool operator == (mstring const& first, cwstr_t second);
		friend LINK bool operator != (mstring const& first, cwstr_t second);
		friend LINK bool operator >= (mstring const& first, cwstr_t second);
		friend LINK bool operator <= (mstring const& first, cwstr_t second);
		friend LINK bool operator > (mstring const& first, cwstr_t second);
		friend LINK bool operator < (mstring const& first, cwstr_t second);

		friend LINK bool operator == (cwstr_t first, mstring const& second);
		friend LINK bool operator != (cwstr_t first, mstring const& second);
		friend LINK bool operator >= (cwstr_t first, mstring const& second);
		friend LINK bool operator <= (cwstr_t first, mstring const& second);
		friend LINK bool operator > (cwstr_t first, mstring const& second);
		friend LINK bool operator < (cwstr_t first, mstring const& second);

		friend LINK bool operator == (mstring const& first, cstr_t second);
		friend LINK bool operator != (mstring const& first, cstr_t second);
		friend LINK bool operator >= (mstring const& first, cstr_t second);
		friend LINK bool operator <= (mstring const& first, cstr_t second);
		friend LINK bool operator > (mstring const& first, cstr_t second);
		friend LINK bool operator < (mstring const& first, cstr_t second);

		friend LINK bool operator == (cstr_t first, mstring const& second);
		friend LINK bool operator != (cstr_t first, mstring const& second);
		friend LINK bool operator >= (cstr_t first, mstring const& second);
		friend LINK bool operator <= (cstr_t first, mstring const& second);
		friend LINK bool operator > (cstr_t first, mstring const& second);
		friend LINK bool operator < (cstr_t first, mstring const& second);

		friend LINK bool operator == (mstring const& first, cmstr_t second);
		friend LINK bool operator != (mstring const& first, cmstr_t second);
		friend LINK bool operator >= (mstring const& first, cmstr_t second);
		friend LINK bool operator <= (mstring const& first, cmstr_t second);
		friend LINK bool operator > (mstring const& first, cmstr_t second);
		friend LINK bool operator < (mstring const& first, cmstr_t second);

		friend LINK bool operator == (cmstr_t first, mstring const& second);
		friend LINK bool operator != (cmstr_t first, mstring const& second);
		friend LINK bool operator >= (cmstr_t first, mstring const& second);
		friend LINK bool operator <= (cmstr_t first, mstring const& second);
		friend LINK bool operator > (cmstr_t first, mstring const& second);
		friend LINK bool operator < (cmstr_t first, mstring const& second);
	};

	namespace text
	{
		////text_format class////
		// {char:[u|U]|[l|L][N|n]}
		//	u = uppercase
		//	l = lowercase
		//  n = default, optional
		// {text:[u|U]|[l|L]}
		//	u = uppercase
		//	l = lowercase
		//  n = default, optional
		// {signed:[n|N]xx,(F|f)c,[[x|X]|[b|B]],[S|s]}
		//	N = Max Number of digits (xx = number),
		//	F = Fill character (c)
		//	X = Uppercase Hexadecimal format
		//	x = Lowercase Hexadecimal format
		//	B = Binary
		//	S = +|-
		// {unsigned:[n|N]xx,(F|f)c,[[x|X]|[b|B]]}
		//	N = Max Number of digits (xx = number),
		//	X = Hexadecimal format
		//	F = Fill character (c)
		//	B = Binary
		// {floating:[n|N]xx,(F|f)c,[[x|X]|[b|B]],[E],[S|s]}
		//	N = Max Number of decimals (xx = number)
		//	X = Hexadecimal format
		//	F = Fill character (c)
		//	B = Binary
		//	S = +|-
		//	E = Cientific Notation
		class LINK text_format
		{
		protected:
			qword flags;

		public:
			enum target : byte
			{
				none = 0,
				character,
				text,
				signed_interger,
				usigned_interger,
				floating,
			};

		public:
			text_format(cstr_t format);
			text_format(const text_format&);
			virtual~text_format();

		public:
			target format_target()const;
			void format(cstr_t format);
			string format()const;
			qword format_flags()const;
			text_format& operator = (const text_format&);
		};

		typedef text_format text_format_t;

		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, itext_buffer, ibuffer)
			visible vcall encoding_t encoding()const pure;
		ANG_END_INTERFACE();
	}

	namespace strings
	{
		class LINK wstring_buffer
			: public object
			, public text::itext_buffer
			, public memory::iallocator_client
			, public collections::ienum<wchar>
			, public algorithms
		{
		protected:
			friend string_buffer;
			friend mstring_buffer;

			memory::iallocator* allocator;
			union
			{
				word _flag_is_local_data;
				struct
				{
					word _unused;
					uint _buffer_size_used;
					uint _buffer_capacity;
					wchar* _buffer_ptr;
				};
				struct //32 bytes
				{
					word _local_size;
					wchar _local_buffer[30];
				};
			}_data;

		public:
			wstring_buffer();
			explicit wstring_buffer(uint reserv);
			wstring_buffer(cstr_t cstr);
			wstring_buffer(cwstr_t cstr);
			wstring_buffer(cmstr_t cstr);
			wstring_buffer(const wstring_buffer& cstr);
			wstring_buffer(const wstring_buffer* cstr);
			wstring_buffer(const string_buffer* cstr);
			wstring_buffer(const mstring_buffer* cstr);

		private:
			virtual~wstring_buffer();

		public:
			ANG_DECLARE_DYNAMIC_INTERFACE();

			bool is_local_data()const;
			bool realloc(uint new_size, bool save = true);
			void length(uint len);

		protected:
			bool realloc(memory::iallocator*);

		public:
			void clean();
			bool is_empty()const;
			wstr_t str();
			cwstr_t cstr() const;
			uint length() const;
			uint capacity() const;

			uint copy(cwstr_t cstr);
			uint copy(cstr_t cstr);
			uint copy(cmstr_t cstr);
			uint copy(wstring_buffer const* cstr);
			uint copy(string_buffer const* cstr);
			uint copy(mstring_buffer const* cstr);

			uint concat(cwstr_t cstr);
			uint concat(cstr_t cstr);
			uint concat(cmstr_t cstr);
			uint concat(wstring_buffer const* cstr);
			uint concat(string_buffer const* cstr);
			uint concat(mstring_buffer const* cstr);

			void move(wstring_buffer & str);
			void move(wstring_buffer * str);

			int compare(cwstr_t cstr)const;
			int compare(cstr_t cstr)const;
			int compare(cmstr_t cstr)const;

			wstr_t map(uint);

		public: //overrides
			virtual string to_string()const override;

			virtual void set_allocator(memory::iallocator*)override;
			virtual memory::iallocator* get_allocator()const override;

			virtual pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual wsize mem_copy(wsize, pointer) override;
			virtual pointer map_buffer(windex, wsize) override;
			virtual void unmap_buffer(pointer, wsize) override;
			virtual bool can_realloc_buffer()const override;
			virtual bool realloc_buffer(wsize) override;
			virtual text::encoding_t encoding()const override;

			virtual wchar& at(const collections::iterator<wchar>& it)const override;
			virtual collections::iterator<wchar> begin()const override;
			virtual collections::iterator<wchar> end()const override;
			virtual bool next(collections::iterator<wchar>& it)const override;
			virtual bool next(collections::iterator<wchar>& it, int offset)const override;
			virtual bool prev(collections::iterator<wchar>& it)const override;
			virtual bool prev(collections::iterator<wchar>& it, int offset)const override;
			virtual uint counter()const override;

		public: //utils
			collections::iterator<wchar> at(index idx)const;

			uint insert(cwstr_t cstr, collections::iterator<wchar> pos);
			uint insert(wchar c, collections::iterator<wchar> pos);

			uint insert(cstr_t cstr, collections::iterator<wchar> pos);
			uint insert(char c, collections::iterator<wchar> pos);

			uint replace(cwstr_t cstr, collections::iterator<wchar> beg, collections::iterator<wchar> end);
			uint replace(wchar c, collections::iterator<wchar> beg, collections::iterator<wchar> end);

			uint replace(cstr_t cstr, collections::iterator<wchar> beg, collections::iterator<wchar> end);
			uint replace(char c, collections::iterator<wchar> beg, collections::iterator<wchar> end);

			//int replace_all(cwstr_t from, cwstr_t to);

			collections::iterator<wchar> find(cwstr_t cstr, collections::iterator<wchar> start, bool rev = false)const;
			collections::iterator<wchar> find(cwstr_t cstr, collections::iterator<wchar> start, collections::iterator<wchar> end, bool rev = false)const;

			collections::iterator<wchar> find(cstr_t cstr, collections::iterator<wchar> start, bool rev = false)const;
			collections::iterator<wchar> find(cstr_t cstr, collections::iterator<wchar> start, collections::iterator<wchar> end, bool rev = false)const;

			uint sub_string(wstring& out, collections::iterator<wchar> start, uint count = -1)const;
			uint sub_string(wstr_t out, collections::iterator<wchar> start, uint count = -1)const;

			uint sub_string(wstring& out, collections::iterator<wchar> start, collections::iterator<wchar> end)const;
			uint sub_string(wstr_t out, collections::iterator<wchar> start, collections::iterator<wchar> end)const;

			uint sub_string(string& out, collections::iterator<wchar> start, uint count = -1)const;
			uint sub_string(str_t out, collections::iterator<wchar> start, uint count = -1)const;

			uint sub_string(string& out, collections::iterator<wchar> start, collections::iterator<wchar> end)const;
			uint sub_string(str_t out, collections::iterator<wchar> start, collections::iterator<wchar> end)const;

			void format(cwstr_t format, ...);
			void concat_format(cwstr_t format, ...);

			array<wstring> split(wchar val = ' ')const;
			array<wstring> split(array<wchar> arr)const;
			array<wstring> split(cwstr_t val)const;

			void invert();
			void invert(collections::iterator<wchar> beg, collections::iterator<wchar> end);
			void uppercase();
			void lowercase();

			wstring_buffer& operator = (cwstr_t cstr);
			wstring_buffer& operator = (cstr_t cstr);
			wstring_buffer& operator = (cmstr_t cstr);
			wstring_buffer& operator = (const wstring_buffer& cstr);
			wstring_buffer& operator = (const wstring_buffer* cstr);
			wstring_buffer& operator = (const string_buffer* cstr);
			wstring_buffer& operator = (const mstring_buffer* cstr);
		};

		class LINK string_buffer
			: public object
			, public text::itext_buffer
			, public memory::iallocator_client
			, public collections::ienum<char>
			, public algorithms
		{
		protected:
			friend wstring_buffer;
			friend mstring_buffer;

			memory::iallocator* allocator;
			union
			{
				word _flag_is_local_data;
				struct
				{
					word _unused;
					uint _buffer_size_used;
					uint _buffer_capacity;
					char* _buffer_ptr;
				};
				struct //32 bytes
				{
					word _local_size;
					char _local_buffer[30];
				};
			}_data;

		public:
			string_buffer();
			string_buffer(uint reserv);
			string_buffer(cwstr_t cstr);
			string_buffer(cstr_t cstr);
			string_buffer(cmstr_t cstr);
			string_buffer(const string_buffer& cstr);
			string_buffer(const string_buffer* cstr);
			string_buffer(const wstring_buffer* cstr);
			string_buffer(const mstring_buffer* cstr);

		private:
			virtual~string_buffer();

		public:
			ANG_DECLARE_DYNAMIC_INTERFACE();

			bool is_local_data()const;
			bool realloc(uint new_size, bool save = true);
			void length(uint len);

		protected:
			bool realloc(memory::iallocator*);

		public:
			void clean();
			bool is_empty()const;
			str_t str();
			cstr_t cstr() const;
			uint length() const;
			uint capacity() const;

			uint copy(cwstr_t cstr);
			uint copy(cstr_t cstr);
			uint copy(cmstr_t cstr);
			uint copy(string_buffer const* cstr);
			uint copy(wstring_buffer const* cstr);
			uint copy(mstring_buffer const* cstr);

			uint concat(cwstr_t cstr);
			uint concat(cstr_t cstr);
			uint concat(cmstr_t cstr);
			uint concat(string_buffer const* cstr);
			uint concat(wstring_buffer const* cstr);
			uint concat(mstring_buffer const* cstr);

			void move(string_buffer & str);
			void move(string_buffer * str);

			int compare(cstr_t cstr)const;
			int compare(cwstr_t cstr)const;
			int compare(cmstr_t cstr)const;

			str_t map(uint);

		public: //overrides
			virtual string to_string()const override;

			virtual void set_allocator(memory::iallocator*)override;
			virtual memory::iallocator* get_allocator()const override;

			virtual pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual wsize mem_copy(wsize, pointer) override;
			virtual pointer map_buffer(windex, wsize) override;
			virtual void unmap_buffer(pointer, wsize) override;
			virtual bool can_realloc_buffer()const override;
			virtual bool realloc_buffer(wsize) override;
			virtual text::encoding_t encoding()const override;

			virtual char& at(const collections::iterator<char>& it)const override;
			virtual collections::iterator<char> begin()const override;
			virtual collections::iterator<char> end()const override;
			virtual bool next(collections::iterator<char>& it)const override;
			virtual bool next(collections::iterator<char>& it, int offset)const override;
			virtual bool prev(collections::iterator<char>& it)const override;
			virtual bool prev(collections::iterator<char>& it, int offset)const override;
			virtual uint counter()const override;

		public: //utils
			collections::iterator<char> at(index idx)const;

			uint insert(cstr_t cstr, collections::iterator<char> pos);
			uint insert(char c, collections::iterator<char> pos);

			uint insert(cwstr_t cstr, collections::iterator<char> pos);
			uint insert(wchar c, collections::iterator<char> pos);

			uint replace(cstr_t cstr, collections::iterator<char> beg, collections::iterator<char> end);
			uint replace(char c, collections::iterator<char> beg, collections::iterator<char> end);

			uint replace(cwstr_t cstr, collections::iterator<char> beg, collections::iterator<char> end);
			uint replace(wchar c, collections::iterator<char> beg, collections::iterator<char> end);

			//int replace_all(cwstr_t from, cwstr_t to);

			collections::iterator<char> find(cstr_t cstr, collections::iterator<char> start, bool rev = false)const;
			collections::iterator<char> find(cstr_t cstr, collections::iterator<char> start, collections::iterator<char> end, bool rev = false)const;

			collections::iterator<char> find(cwstr_t cstr, collections::iterator<char> start, bool rev = false)const;
			collections::iterator<char> find(cwstr_t cstr, collections::iterator<char> start, collections::iterator<char> end, bool rev = false)const;

			uint sub_string(string& out, collections::iterator<char> start, uint count = -1)const;
			uint sub_string(str_t out, collections::iterator<char> start, uint count = -1)const;

			uint sub_string(string& out, collections::iterator<char> start, collections::iterator<char> end)const;
			uint sub_string(str_t out, collections::iterator<char> start, collections::iterator<char> end)const;

			uint sub_string(wstring& out, collections::iterator<char> start, uint count = -1)const;
			uint sub_string(wstr_t out, collections::iterator<char> start, uint count = -1)const;

			uint sub_string(wstring& out, collections::iterator<char> start, collections::iterator<char> end)const;
			uint sub_string(wstr_t out, collections::iterator<char> start, collections::iterator<char> end)const;

			void format(cstr_t format, ...);
			void concat_format(cstr_t format, ...);

			array<string> split(char val = ' ')const;
			array<string> split(array<char> arr)const;
			array<string> split(cstr_t val)const;

			void invert();
			void invert(collections::iterator<char> beg, collections::iterator<char> end);
			void uppercase();
			void lowercase();
		};

		class LINK mstring_buffer
			: public object
			, public text::itext_buffer
			, public memory::iallocator_client
			, public algorithms
		{
		protected:
			friend wstring_buffer;
			friend string_buffer;

			memory::iallocator* allocator;
			union
			{
				word _flag_is_local_data;
				struct
				{
					word _unused;
					uint _buffer_size_used;
					uint _buffer_capacity;
					unsigned char* _buffer_ptr;
				};
				struct //32 bytes
				{
					word _local_size;
					unsigned char _local_buffer[30];
				};
			}_data;

		public:
			mstring_buffer();
			mstring_buffer(uint reserv);
			mstring_buffer(cwstr_t cstr);
			mstring_buffer(cstr_t cstr);
			mstring_buffer(cmstr_t cstr);
			mstring_buffer(const mstring_buffer& cstr);
			mstring_buffer(const mstring_buffer* cstr);
			mstring_buffer(const wstring_buffer* cstr);
			mstring_buffer(const string_buffer* cstr);

		private:
			virtual~mstring_buffer();

		public:
			ANG_DECLARE_DYNAMIC_INTERFACE();

			bool is_local_data()const;
			bool realloc(uint new_size, bool save = true);
			void length(uint len);

		protected:
			bool realloc(memory::iallocator*);

		public:
			virtual string to_string()const override;

			virtual void set_allocator(memory::iallocator*)override;
			virtual memory::iallocator* get_allocator()const override;

			virtual pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual wsize mem_copy(wsize, pointer) override;
			virtual pointer map_buffer(windex, wsize) override;
			virtual void unmap_buffer(pointer, wsize) override;
			virtual bool can_realloc_buffer()const override;
			virtual bool realloc_buffer(wsize) override;
			virtual text::encoding_t encoding()const override;

			void clean();
			bool is_empty()const;
			mstr_t str();
			cmstr_t cstr() const;
			uint length() const;
			uint size() const;
			uint capacity() const;

			uint copy(cmstr_t cstr);
			uint copy(cwstr_t cstr);
			uint copy(cstr_t cstr);
			uint copy(mstring_buffer const* cstr);
			uint copy(wstring_buffer const* cstr);
			uint copy(string_buffer const* cstr);

			uint concat(cmstr_t cstr);
			uint concat(cwstr_t cstr);
			uint concat(cstr_t cstr);
			uint concat(mstring_buffer const* cstr);
			uint concat(wstring_buffer const* cstr);
			uint concat(string_buffer const* cstr);

			void move(mstring_buffer & str);
			void move(mstring_buffer * str);

			int compare(cmstr_t cstr)const;
			int compare(cwstr_t cstr)const;
			int compare(cstr_t cstr)const;
		};
	
	}

	inline string operator "" _o(const char* str, wsize sz) { return string(cstr_t(str, sz)); }
	inline wstring operator "" _o(const wchar* str, wsize sz) { return wstring(cwstr_t(str, sz)); }

	template<wsize N>inline string::object_wrapper(const wchar(&ar)[N])
		: object_wrapper(cwstr_t(ar, N-1)){
	}
	template<wsize N>inline string::object_wrapper(const char(&ar)[N])
		: object_wrapper(cstr_t(ar, N - 1)) {
	}

	template<wsize N>inline wstring::object_wrapper(const wchar(&ar)[N])
		: object_wrapper(cwstr_t(ar, N - 1)) {
	}
	template<wsize N>inline wstring::object_wrapper(const char(&ar)[N])
		: object_wrapper(cstr_t(ar, N - 1)) {
	}

	template<wsize N>inline mstring::object_wrapper(const wchar(&ar)[N])
		: object_wrapper(cwstr_t(ar, N - 1)) {
	}
	template<wsize N>inline mstring::object_wrapper(const char(&ar)[N])
		: object_wrapper(cstr_t(ar, N - 1)) {
	}


	template<wsize N>inline string& string::operator=(const wchar(&ar)[N]) { return string::operator=(cwstr_t(ar, N - 1)); }
	template<wsize N>inline string& string::operator=(const char(&ar)[N]) { return string::operator=(cstr_t(ar, N - 1)); }
	template<wsize N>inline string& string::operator+=(const wchar(&ar)[N]) { return string::operator+=(cwstr_t(ar, N - 1)); }
	template<wsize N>inline string& string::operator+=(const char(&ar)[N]) { return string::operator+=(cstr_t(ar, N - 1)); }

	template<wsize N>inline wstring& wstring::operator=(const wchar(&ar)[N]) { return wstring::operator=(cwstr_t(ar, N - 1)); }
	template<wsize N>inline wstring& wstring::operator=(const char(&ar)[N]) { return wstring::operator=(cstr_t(ar, N - 1)); }
	template<wsize N>inline wstring& wstring::operator+=(const wchar(&ar)[N]) { return wstring::operator+=(cwstr_t(ar, N - 1)); }
	template<wsize N>inline wstring& wstring::operator+=(const char(&ar)[N]) { return wstring::operator+=(cstr_t(ar, N - 1)); }

	template<wsize N>inline mstring& mstring::operator=(const wchar(&ar)[N]) { return mstring::operator=(cwstr_t(ar, N - 1)); }
	template<wsize N>inline mstring& mstring::operator=(const char(&ar)[N]) { return mstring::operator=(cstr_t(ar, N - 1)); }
	template<wsize N>inline mstring& mstring::operator+=(const wchar(&ar)[N]) { return mstring::operator+=(cwstr_t(ar, N - 1)); }
	template<wsize N>inline mstring& mstring::operator+=(const char(&ar)[N]) { return mstring::operator+=(cstr_t(ar, N - 1)); }


	template<wsize N>
	inline wstring operator + (wstring const& a1, const wchar(&a2)[N]) {
		return operator +(a1, cwstr_t(a2, N - 1));
	}
	template<wsize N>
	inline wstring operator + (wstring const& a1, const char(&a2)[N]) {
		return operator +(a1, cstr_t(a2, N - 1));
	}

	template<wsize N>
	inline wstring operator + (const wchar(&a1)[N], wstring const& a2) {
		return operator +(cwstr_t(a1, N - 1), a2);
	}
	template<wsize N>
	inline wstring operator + (const char(&a1)[N], wstring const& a2) {
		return operator +(cstr_t(a1, N - 1), a2);
	}

	template<wsize N>
	inline string operator + (string const& a1, const wchar(&a2)[N]) {
		return operator +(a1, cwstr_t(a2, N - 1));
	}
	template<wsize N>
	inline string operator + (string const& a1, const char(&a2)[N]) {
		return operator +(a1, cstr_t(a2, N - 1));
	}

	template<wsize N>
	inline string operator + (const wchar(&a1)[N], string const& a2) {
		return operator +(cwstr_t(a1, N - 1), a2);
	}
	template<wsize N>
	inline string operator + (const char(&a1)[N], string const& a2) {
		return operator +(cstr_t(a1, N - 1), a2);
	}

	template<wsize N>
	inline mstring operator + (mstring const& a1, const wchar(&a2)[N]) {
		return operator +(a1, cwstr_t(a2, N - 1));
	}
	template<wsize N>
	inline mstring operator + (mstring const& a1, const char(&a2)[N]) {
		return operator +(a1, cstr_t(a2, N - 1));
	}

	template<wsize N>
	inline mstring operator + (const wchar(&a1)[N], mstring const& a2) {
		return operator +(cwstr_t(a1, N - 1), a2);
	}
	template<wsize N>
	inline mstring operator + (const char(&a1)[N], mstring const& a2) {
		return operator +(cstr_t(a1, N - 1), a2);
	}

	template<wsize N>
	inline wstring& operator << (wstring& out, const wchar(&a1)[N]) {
		return operator <<(out, cwstr_t(a1, N - 1));
	}
	template<wsize N>
	inline wstring& operator << (wstring& out, const char(&a1)[N]) {
		return operator <<(out, cstr_t(a1, N - 1));
	}

	template<wsize N>
	inline string& operator << (string& out, const wchar(&a1)[N]) {
		return operator <<(out, cwstr_t(a1, N - 1));
	}
	template<wsize N>
	inline string& operator << (string& out, const char(&a1)[N]) {
		return operator <<(out, cstr_t(a1, N - 1));
	}

	template<wsize N>
	inline mstring& operator << (mstring& out, const wchar(&a1)[N]) {
		return operator <<(out, cwstr_t(a1, N - 1));
	}
	template<wsize N>
	inline mstring& operator << (mstring& out, const char(&a1)[N]) {
		return operator <<(out, cstr_t(a1, N - 1));
	}

}

inline ang::object_wrapper<ang::strings::wstring_buffer>::operator ang::object_t()const { return get(); }
inline ang::object_wrapper<ang::strings::string_buffer>::operator ang::object_t()const { return get(); }
inline ang::object_wrapper<ang::strings::mstring_buffer>::operator ang::object_t()const { return get(); }


#endif//__ANG_STRINGS_H__