#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_STATIC_ARRAY_H__
#define __ANG_STATIC_ARRAY_H__

namespace ang
{
	namespace collections
	{
		template<typename T> struct static_array
		{
		public: /*type definitions*/
			typedef T* type;

		private: /*members*/
			type _value;
			uint _size;

		public: /*constructors*/
			static_array() : _value(null), _size(0) {}
			static_array(type val, uint size) : _value(val), _size(size) {}
			static_array(ang::nullptr_t, uint size = 0) : _value(null), _size(size) {}
			static_array(value<T*> val, uint size) : _value(val.get()), _size(size) {}
			static_array(static_array const& other) : _value(other._value), _size(other._size) {}
			static_array(static_array && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
			template<wsize N>
			static_array(T(&ar)[N])
				: _value(ar), _size(N) { }
			~static_array() {}

		public: /*getters and setters*/
			type & get() { return _value; }
			type const& get()const { return _value; }
			void set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
			type data()const { return _value; }
			uint size()const { return _size; }

		public: /*operators*/
			static_array& operator = (type val) { set(ang::move(val), 1u); return*this; }
			static_array& operator = (value<T*> const& val) { set(val.get(), 1u); return*this; }
			static_array& operator = (static_array const& val) { set(val._value, val._size); return*this; }
			static_array& operator = (static_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
			template<wsize N> static_array& operator = (T(&ar)[N]) { set(ar, N); return*this; }

			operator type& () { return get(); }
			operator type ()const { return get(); }

			T& operator [](int idx) { return get()[idx]; }
			T const& operator [](int idx)const { return get()[idx]; }

			value<T**> operator & () { return &get(); }
			//static_array<T *const*> operator & ()const { return &get(); }
		};

		template<typename T> struct static_const_array
		{
		public: /*type definitions*/
			typedef T const* type;

		private: /*members*/
			type _value;
			uint _size;

		public: /*constructors*/
			static_const_array() : _value(null), _size(0) {}
			static_const_array(type val, uint size) : _value(val), _size(size) {}
			static_const_array(ang::nullptr_t, uint size = 0) : _value(null), _size(0) {}
			static_const_array(value<T const*> val, uint size) : _value(val.get()), _size(size) {}
			static_const_array(static_const_array<T> const* val, uint size) : _value(&val->get()), _size(size) {}
			static_const_array(static_const_array const& other) : _value(other._value), _size(other._size) {}
			static_const_array(static_const_array && other) : _value(ang::move(other._value)), _value(ang::move(other._size)) {}
			template<wsize N>
			static_const_array(const T(&ar)[N])
				: _value(ar), _size(N) { }
			~static_const_array() {}

		public: /*getters and setters*/
			type & get() { return _value; }
			type const& get()const { return _value; }
			void set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
			type data()const { return _value; }
			uint size()const { return _size; }

		public: /*operators*/
			static_const_array& operator = (type val) { set(ang::move(val), 1); return*this; }
			static_const_array& operator = (value<T const*> const& val) { set(val.get(), 1); return*this; }
			static_const_array& operator = (static_const_array const& val) { set(val._value, al._size); return*this; }
			static_const_array& operator = (static_const_array && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
			template<wsize N> static_const_array& operator = (const T(&ar)[N]) { set(ar, N); return*this; }

			operator type& () { return get(); }
			operator type ()const { return get(); }

			T const& operator [](int idx)const { return get()[idx]; }

		};

		template<> struct LINK static_array<char>
		{
		public: /*type definitions*/
			typedef char* type;

		private: /*members*/
			type _value;
			uint _size;

		public: /*constructors*/
			static_array();
			static_array(type val, uint size);
			static_array(ang::nullptr_t, uint size = 0);
			static_array(value<char*> val, uint size);
			static_array(static_array const& other);
			static_array(static_array && other);
			template<wsize N>
			static_array(char(&ar)[N])
				: _value(ar), _size(N - 1U) { }
			~static_array();

		public: /*getters and setters*/
			type & get();
			type const& get()const;
			void set(type val, uint size);
			type data()const;
			uint size()const;

		public: /*operators*/
			static_array& operator = (type val);
			static_array& operator = (value<char*> const& val);
			static_array& operator = (static_array const& val);
			static_array& operator = (static_array && val);
			template<wsize N> static_array& operator = (char(&ar)[N]);

			inline operator type& () { return get(); }
			inline operator type ()const { return get(); }
			inline char& operator [](int idx) { return get()[idx]; }
			inline char const& operator [](int idx)const { return get()[idx]; }
			inline value<char**> operator & () { return &get(); }
		};

		template<> struct LINK static_const_array<char>
		{
		public: /*type definitions*/
			typedef char const* type;

		private: /*members*/
			type _value;
			uint _size;

		public: /*constructors*/
			static_const_array();
			static_const_array(type val, uint size);
			static_const_array(ang::nullptr_t, uint size = 0);
			static_const_array(value<char const*> val, uint size);
			static_const_array(static_const_array<char> const* val, uint size);
			static_const_array(static_const_array const& other);
			static_const_array(static_const_array && other);
			template<wsize N>
			static_const_array(const char(&ar)[N])
				: _value(ar), _size(N - 1U) { }
			~static_const_array();

		public: /*getters and setters*/
			type & get();
			type const& get()const;
			void set(type val, uint size);
			type data()const;
			uint size()const;

		public: /*operators*/
			static_const_array& operator = (type val);
			static_const_array& operator = (value<char const*> const& val);
			static_const_array& operator = (static_const_array const& val);
			static_const_array& operator = (static_const_array && val);
			template<wsize N> static_const_array& operator = (const char(&ar)[N]) { set(ar, N - 1U); return*this; }

			inline operator type& () { return get(); }
			inline operator type ()const { return get(); }
			inline char const& operator [](int idx)const { return get()[idx]; }

		};

		template<> struct LINK static_array<byte>
		{
		public: /*type definitions*/
			typedef byte* type;

		private: /*members*/
			type _value;
			uint _size;

		public: /*constructors*/
			static_array();
			static_array(type val, uint size);
			static_array(ang::nullptr_t, uint size = 0);
			static_array(value<byte*> val, uint size);
			static_array(static_array const& other);
			static_array(static_array && other);
			template<wsize N>
			static_array(byte(&ar)[N])
				: _value(ar), _size(N - 1U) { }
			~static_array();

		public: /*getters and setters*/
			type & get();
			type const& get()const;
			void set(type val, uint size);
			type data()const;
			uint size()const;

		public: /*operators*/
			static_array& operator = (type val);
			static_array& operator = (value<byte*> const& val);
			static_array& operator = (static_array const& val);
			static_array& operator = (static_array && val);
			template<wsize N> static_array& operator = (byte(&ar)[N]);

			inline operator type& () { return get(); }
			inline operator type ()const { return get(); }
			inline byte& operator [](int idx) { return get()[idx]; }
			inline byte const& operator [](int idx)const { return get()[idx]; }
			inline value<byte**> operator & () { return &get(); }
		};

		template<> struct LINK static_const_array<byte>
		{
		public: /*type definitions*/
			typedef byte const* type;

		private: /*members*/
			type _value;
			uint _size;

		public: /*constructors*/
			static_const_array();
			static_const_array(type val, uint size);
			static_const_array(ang::nullptr_t, uint size = 0);
			static_const_array(value<byte const*> val, uint size);
			static_const_array(static_const_array<byte> const* val, uint size);
			static_const_array(static_const_array const& other);
			static_const_array(static_const_array && other);
			template<wsize N>
			static_const_array(const byte(&ar)[N])
				: _value(ar), _size(N - 1U) { }
			~static_const_array();

		public: /*getters and setters*/
			type & get();
			type const& get()const;
			void set(type val, uint size);
			type data()const;
			uint size()const;

		public: /*operators*/
			static_const_array& operator = (type val);
			static_const_array& operator = (value<byte const*> const& val);
			static_const_array& operator = (static_const_array const& val);
			static_const_array& operator = (static_const_array && val);
			template<wsize N> static_const_array& operator = (const byte(&ar)[N]) { set(ar, N - 1U); return*this; }

			inline operator type& () { return get(); }
			inline operator type ()const { return get(); }
			inline byte const& operator [](int idx)const { return get()[idx]; }

		};

		template<> struct LINK static_array<wchar>
		{
		public: /*type definitions*/
			typedef wchar* type;

		private: /*members*/
			type _value;
			uint _size;

		public: /*constructors*/
			static_array();
			static_array(type val, uint size);
			static_array(ang::nullptr_t, uint size = 0);
			static_array(value<wchar*> val, uint size);
			static_array(static_array const& other);
			static_array(static_array && other);
			template<wsize N>
			static_array(wchar(&ar)[N])
				: _value(ar), _size(N - 1U) { }
			~static_array();

		public: /*getters and setters*/
			type & get();
			type const& get()const;
			void set(type val, uint size);
			type data()const;
			uint size()const;

		public: /*operators*/
			static_array& operator = (type val);
			static_array& operator = (value<wchar*> const& val);
			static_array& operator = (static_array const& val);
			static_array& operator = (static_array && val);
			template<wsize N> static_array& operator = (wchar(&ar)[N]);

			inline operator type& () { return get(); }
			inline operator type ()const { return get(); }
			inline wchar& operator [](int idx) { return get()[idx]; }
			inline wchar const& operator [](int idx)const { return get()[idx]; }
			inline value<wchar**> operator & () { return &get(); }
		};

		template<> struct LINK static_const_array<wchar>
		{
		public: /*type definitions*/
			typedef wchar const* type;

		private: /*members*/
			type _value;
			uint _size;

		public: /*constructors*/
			static_const_array();
			static_const_array(type val, uint size);
			static_const_array(ang::nullptr_t, uint size = 0);
			static_const_array(value<wchar const*> val, uint size);
			static_const_array(static_const_array<wchar> const* val, uint size);
			static_const_array(static_const_array const& other);
			static_const_array(static_const_array && other);
			template<wsize N>
			static_const_array(const wchar(&ar)[N])
				: _value(ar), _size(N - 1U) { }
			~static_const_array();

		public: /*getters and setters*/
			type & get();
			type const& get()const;
			void set(type val, uint size);
			type data()const;
			uint size()const;

		public: /*operators*/
			static_const_array& operator = (type val);
			static_const_array& operator = (value<wchar const*> const& val);
			static_const_array& operator = (static_const_array const& val);
			static_const_array& operator = (static_const_array && val);
			template<wsize N> static_const_array& operator = (const wchar(&ar)[N]) { set(ar, N - 1U); return*this; }

			inline operator type& () { return get(); }
			inline operator type ()const { return get(); }
			inline wchar const& operator [](int idx)const { return get()[idx]; }

		};

	}
}

#endif//__ANG_STATIC_ARRAY_H__