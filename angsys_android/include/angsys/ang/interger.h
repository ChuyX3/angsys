#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_INTERGER_H__
#define __ANG_INTERGER_H__

namespace ang
{
	typedef value_wrapper<int> interger;
	typedef value_wrapper<uint> uinterger;
	typedef value_wrapper<long64> interger64;
	typedef value_wrapper<ulong64> uinterger64;

	typedef object_wrapper<interger> interger_t;
	typedef object_wrapper<uinterger> uinterger_t;
	typedef object_wrapper<interger64> interger64_t;
	typedef object_wrapper<uinterger64> uinterger64_t;

	template<> class LINK value_wrapper<int> final
		: public object
		, public value<int>
	{
	public:
		static value<int> parse(cstr_t, int base = 10);
		static value<int> parse(cwstr_t, int base = 10);
		static string to_string(value<int>);
		static wstring to_wstring(value<int>);
		static string to_string(value<int>, text::text_format_t format);
		static wstring to_wstring(value<int>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(int val);
		value_wrapper(interger const& other);
		value_wrapper(value<int> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;
		string to_string(text::text_format_t)const;

	private:
		virtual~value_wrapper();
	};

	template<> class LINK value_wrapper<uint> final
		: public object
		, public value<uint>
	{
	public:
		static value<uint> parse(cstr_t, int base = 10);
		static value<uint> parse(cwstr_t, int base = 10);
		static string to_string(value<uint>);
		static wstring to_wstring(value<uint>);
		static string to_string(value<uint>, text::text_format_t format);
		static wstring to_wstring(value<uint>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(uint val);
		value_wrapper(uinterger const& other);
		value_wrapper(value<uint> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;
		string to_string(text::text_format_t)const;

	private:
		virtual~value_wrapper();
	};

	template<> class LINK value_wrapper<long64> final
		: public object
		, public value<long64>
	{
	public:
		static value<long64> parse(cstr_t, int base = 10);
		static value<long64> parse(cwstr_t, int base = 10);
		static string to_string(value<long64>);
		static wstring to_wstring(value<long64>);
		static string to_string(value<long64>, text::text_format_t format);
		static wstring to_wstring(value<long64>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(long64 val);
		value_wrapper(interger64 const& other);
		value_wrapper(value<long64> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;
		string to_string(text::text_format_t)const;

	private:
		virtual~value_wrapper();
	};

	template<> class LINK value_wrapper<ulong64> final
		: public object
		, public value<ulong64>
	{
	public:
		static value<ulong64> parse(cstr_t, int base = 10);
		static value<ulong64> parse(cwstr_t, int base = 10);
		static string to_string(value<ulong64>);
		static wstring to_wstring(value<ulong64>);
		static string to_string(value<ulong64>, text::text_format_t format);
		static wstring to_wstring(value<ulong64>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(ulong64 val);
		value_wrapper(uinterger64 const& other);
		value_wrapper(value<ulong64> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;
		string to_string(text::text_format_t)const;

	private:
		virtual~value_wrapper();
	};


}//ang

#endif//__ANG_VALUE_H__
