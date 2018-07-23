#ifndef __ANGSYS_H__
#error ...
#elif !defined __VARIABLE_H__
#define __VARIABLE_H__
namespace ang
{
	ang_begin_interface(LINK ivariant)
		visible vcall rtti_t const& value_type()const pure;
		visible vcall bool set_value(rtti_t const&, unknown_t)pure;
		visible vcall bool get_value(rtti_t const&, unknown_t)const pure;
		visible vcall variant clone()const pure;
		visible vcall wstring to_string()const pure;
		visible vcall wstring to_string(text::text_format_t)const pure;
		template<typename T> bool set_value(T const& val);
		template<typename T> bool get_value(T& val)const;
	ang_end_interface();


	template<> class LINK intf_wrapper<ivariant>
	{
	public:
		typedef ivariant type;
		typedef ivariant* type_ptr;
		typedef ivariant& type_ref;
		typedef ivariant const* ctype_ptr;
		typedef ivariant const& ctype_ref;

	public:
		intf_wrapper();
		intf_wrapper(type*);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);

		template<typename T, text::encoding E> intf_wrapper(str_view<T,E> const&); //string convertible
		template<typename T, wsize N> intf_wrapper(T(&ar)[N]); //array convertible

		intf_wrapper(bool); //bool convertible
		intf_wrapper(int); //int convertible
		intf_wrapper(uint); //uint convertible
		intf_wrapper(long); //long convertible
		intf_wrapper(ulong); //ulong convertible
		intf_wrapper(long64); //long convertible
		intf_wrapper(ulong64); //ulong convertible
		intf_wrapper(float); //float convertible
		intf_wrapper(double); //double convertible

		~intf_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);
		type ** addres_for_init(void);

		template<typename T> typename smart_ptr_type<T>::smart_ptr_t as() {
			return interface_cast<typename smart_ptr_type<T>::type>(_ptr);
		}

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);
		template<typename T> intf_wrapper& operator = (T*);
		template<typename T> intf_wrapper& operator = (intf_wrapper<T> const&);

		template<typename T, text::encoding E> intf_wrapper& operator = (str_view<T, E> const&); //string convertible
		template<typename T, wsize N> intf_wrapper& operator = (T(&ar)[N]); //array convertible

		intf_wrapper& operator = (bool); //bool convertible
		intf_wrapper& operator = (int); //int convertible
		intf_wrapper& operator = (uint); //uint convertible
		intf_wrapper& operator = (long); //long convertible
		intf_wrapper& operator = (ulong); //ulong convertible
		intf_wrapper& operator = (long64); //long convertible
		intf_wrapper& operator = (ulong64); //ulong convertible
		intf_wrapper& operator = (float); //float convertible
		intf_wrapper& operator = (double); //double convertible

		intf_wrapper_ptr<type> operator & (void);
		operator ang::intfptr()const;
		type* operator -> (void);
		type const* operator -> (void)const;
		operator type* (void);
		operator type const* (void)const;

	private:
		ivariant* _ptr;

	};

	template<typename T>
	class variable
		: public object
		, public ivariant
		, public value<T>
	{
	public:
		variable();
		variable(T const&);
		variable(value<T> const& val) : value<T>(val) {	}
		variable(variable const*);

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual wstring to_string()const override;
		virtual wstring to_string(text::text_format_t)const override;
		comparision_result_t compare(object const* obj)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariant::set_value;
		using ivariant::get_value;

	protected:
		virtual~variable();
	};
}


namespace ang
{
	template<> 
	class LINK variable<bool>
		: public object
		, public ivariant
		, public value<bool>
	{
	public:
		static wstring to_string(value<bool>, text::text_format = text::default_text_format<bool>::format());

	public:
		variable();
		variable(bool const&);
		variable(value<bool> const& val);
		variable(variable const*);
		template<typename T> variable(value<T> const& other)
			: value<T>(other.get() ? true : false) {}

	public: //overrides
		ANG_DECLARE_INTERFACE();

		comparision_result_t compare(object const* obj)const override;

		virtual wstring to_string()const override;
		virtual wstring to_string(text::text_format_t)const override;
	
		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariant::set_value;
		using ivariant::get_value;

	protected:
		virtual~variable();
	};

	template<> 
	class LINK variable<int>
		: public object
		, public ivariant
		, public value<int>
	{
	public:
		template<typename T, text::encoding E> static inline value<int> parse(str_view<T, E> str, int b = 10) {
			windex i = 0;
			return (int)str_to_signed(str, i, b);
		}
		template<typename T, text::encoding E> static inline value<int> Parse(str_view<T, E> str, windex i, int b = 10) {
			return (int)str_to_signed(str, i, b);
		}
		static wstring to_string(value<int>, text::text_format = text::default_text_format<int>::format());

	public:
		variable();
		variable(int const&);
		variable(value<int> const& val) : value<int>(val) {	}
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual wstring to_string()const override;
		virtual wstring to_string(text::text_format_t)const override;
		comparision_result_t compare(object const* obj)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariant::set_value;
		using ivariant::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<uint>
		: public object
		, public ivariant
		, public value<uint>
	{
	public:
		template<typename T, text::encoding E> static inline value<uint> parse(str_view<T, E> str, int b = 10) {
			windex i = 0;
			return (uint)str_to_unsigned(str, i, b);
		}
		template<typename T, text::encoding E> static inline value<uint> Parse(str_view<T, E> str, windex i, int b = 10) {
			return (uint)str_to_unsigned(str, i, b);
		}
		static wstring to_string(value<uint>, text::text_format = text::default_text_format<uint>::format());

	public:
		variable();
		variable(uint const&);
		variable(value<uint> const& val) : value<uint>(val) {	}
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual wstring to_string()const override;
		virtual wstring to_string(text::text_format_t)const override;
		comparision_result_t compare(object const* obj)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariant::set_value;
		using ivariant::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<long64>
		: public object
		, public ivariant
		, public value<long64>
	{
	public:
		template<typename T, text::encoding E> static inline value<long64> parse(str_view<T, E> str, int b = 10) {
			windex i = 0;
			return (long64)str_to_signed(str, i, b);
		}
		template<typename T, text::encoding E> static inline value<long64> Parse(str_view<T, E> str, windex i, int b = 10) {
			return (long64)str_to_signed(str, i, b);
		}
		static wstring to_string(value<long64>, text::text_format = text::default_text_format<long64>::format());

	public:
		variable();
		variable(long64 const&);
		variable(value<long64> const& val) : value<long64>(val) {	}
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual wstring to_string()const override;
		virtual wstring to_string(text::text_format_t)const override;
		comparision_result_t compare(object const* obj)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariant::set_value;
		using ivariant::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<ulong64>
		: public object
		, public ivariant
		, public value<ulong64>
	{
	public:
		template<typename T, text::encoding E> static inline value<ulong64> parse(str_view<T, E> str, int b = 10) {
			windex i = 0;
			return (ulong64)str_to_unsigned(str, i, b);
		}
		template<typename T, text::encoding E> static inline value<ulong64> Parse(str_view<T, E> str, windex i, int b = 10) {
			return (ulong64)str_to_unsigned(str, i, b);
		}
		static wstring to_string(value<ulong64>, text::text_format = text::default_text_format<ulong64>::format());

	public:
		variable();
		variable(ulong64 const&);
		variable(value<ulong64> const& val) : value<ulong64>(val) {	}
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual wstring to_string()const override;
		virtual wstring to_string(text::text_format_t)const override;
		comparision_result_t compare(object const* obj)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariant::set_value;
		using ivariant::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<float>
		: public object
		, public ivariant
		, public value<float>
	{
	public:
		template<typename T, text::encoding E> static inline value<float> parse(str_view<T, E> str, bool exp = false) {
			windex i = 0;
			return (float)str_to_floating(str, i, exp);
		}
		template<typename T, text::encoding E> static inline value<float> Parse(str_view<T, E> str, windex i, bool exp = false) {
			return (float)str_to_floating(str, i, exp);
		}
		static wstring to_string(value<float>, text::text_format = text::default_text_format<float>::format());

	public:
		variable();
		variable(float const&);
		variable(value<float> const& val) : value<float>(val) {	}
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual wstring to_string()const override;
		virtual wstring to_string(text::text_format_t)const override;
		comparision_result_t compare(object const* obj)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariant::set_value;
		using ivariant::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<double>
		: public object
		, public ivariant
		, public value<double>
	{
	public:
		template<typename T, text::encoding E> static inline value<double> parse(str_view<T, E> str, bool exp = false) {
			windex i = 0;
			return str_to_floating(str, i, exp);
		}
		template<typename T, text::encoding E> static inline value<double> Parse(str_view<T, E> str, windex i, bool exp = false) {
			return str_to_floating(str, i, exp);
		}
		static wstring to_string(value<double>, text::text_format = text::default_text_format<double>::format());

	public:
		variable();
		variable(double const&);
		variable(value<double> const& val) : value<double>(val) {	}
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		ANG_DECLARE_INTERFACE();

		virtual wstring to_string()const override;
		virtual wstring to_string(text::text_format_t)const override;
		comparision_result_t compare(object const* obj)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariant::set_value;
		using ivariant::get_value;

	protected:
		virtual~variable();
	};
}
#endif//__SAMRT_PTR_H__