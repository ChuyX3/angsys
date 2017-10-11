#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_FLOATING_H__
#define __ANG_FLOATING_H__

namespace ang
{
	typedef value_wrapper<float> floating;
	typedef value_wrapper<double> floating64;
	typedef object_wrapper<floating> floating_t;
	typedef object_wrapper<floating64> floating64_t;

	template<> class LINK value_wrapper<float> final
		: public object
		, public value<float>
	{
	public:
		static value<float> parse(cstr_t);
		static value<float> parse(cwstr_t);
		static string to_string(value<float>);
		static wstring to_wstring(value<float>);
		static string to_string(value<float>, text::text_format_t format);
		static wstring to_wstring(value<float>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(float val);
		value_wrapper(floating const& other);
		value_wrapper(value<float> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;
		string to_string(text::text_format_t)const;

	private:
		virtual~value_wrapper();
	};

	
	template<> class LINK value_wrapper<double> final
		: public object
		, public value<double>
	{
	public:
		static value<double> parse(cstr_t);
		static value<double> parse(cwstr_t);
		static string to_string(value<double>);
		static wstring to_wstring(value<double>);
		static string to_string(value<double>, text::text_format_t format);
		static wstring to_wstring(value<double>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(double val);
		value_wrapper(floating64 const& other);
		value_wrapper(value<double> const& other);
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

#endif//__ANG_FLOATING_H__
