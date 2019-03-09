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

		template<typename T> intf_wrapper(shared_ptr<T> ptr);
		template<wsize N> intf_wrapper(const char(&ar)[N]);
		template<wsize N> intf_wrapper(const wchar(&ar)[N]);
		template<wsize N> intf_wrapper(const mchar(&ar)[N]);
		template<wsize N> intf_wrapper(const char16(&ar)[N]);
		template<wsize N> intf_wrapper(const char32(&ar)[N]);
		template<typename T, wsize N> intf_wrapper(T(&ar)[N]);
		//template<typename T> intf_wrapper(array_view<T> const& ar);
		template<typename T, text::encoding E> intf_wrapper(str_view<T, E>);
		template<text::encoding E, template<typename>class A> intf_wrapper(text::basic_string<E,A> const&);

		intf_wrapper(bool); //bool convertible
		intf_wrapper(int); //int convertible
		intf_wrapper(uint); //uint convertible
		intf_wrapper(long); //long convertible
		intf_wrapper(ulong); //ulong convertible
		intf_wrapper(long64); //long convertible
		intf_wrapper(ulong64); //ulong convertible
		intf_wrapper(float); //float convertible
		intf_wrapper(double); //double convertible

		template<typename T> intf_wrapper(T const& val);
		template<typename T> intf_wrapper(variable<T>* val);

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
			return interface_cast<typename smart_ptr_type<T>::type>(m_ptr);
		}

		template<typename T> explicit operator T& () {
			return (T&)as<T>();
		}

		template<typename T> explicit operator T const& ()const {
			return (T const&)const_cast<var*>(this)->as<T>();
		}

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);
		template<typename T> intf_wrapper& operator = (T*);
		template<typename T> intf_wrapper& operator = (shared_ptr<T> ptr);
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
		ivariant* m_ptr;

	};

	template<typename T, bool = std::is_copy_constructible<T>::value>
	struct __variable_constructor_helper;

	template<typename T>
	class variable final
		: public smart<variable<T>, ivariant, ang::value<T>>
	{
	public:
		friend __variable_constructor_helper<T>;

	public:
		variable();
		variable(T const& val);
		variable(ang::value<T> const& val)
			: variable(val.get()) {
		}
		variable(variable<T> const* val);

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


	template<typename T>
	class object_wrapper<variable<T>>
	{
	public:
		typedef variable<T> type;
		typedef variable<T>* type_ptr;
		typedef variable<T>& type_ref;
		typedef variable<T> const* ctype_ptr;
		typedef variable<T> const& ctype_ref;

	public:
		object_wrapper()
			: m_ptr(null) {
		}

		object_wrapper(type* ptr)
			: object_wrapper() {
			set(ptr);
		}

		object_wrapper(object_wrapper && ptr)
			: object_wrapper() {
			T * temp = ptr.m_ptr;
			ptr.m_ptr = null;
			m_ptr = temp;
		}

		object_wrapper(object_wrapper const& ptr)
			: object_wrapper() {
			set(ptr.get());
		}

		object_wrapper(ang::nullptr_t const&)
			: object_wrapper() {
		}

		~object_wrapper() {
			reset();
		}

	public: //properties

		void reset() {
			if (m_ptr)m_ptr->release();
			m_ptr = null;
		}

		void reset_unsafe() {
			m_ptr = null;
		}

		bool is_empty()const {
			return m_ptr == null;
		}

		type* get(void)const {
			return m_ptr;
		}

		void set(type* ptr) {
			type * temp = m_ptr;
			if (ptr == m_ptr)
				return;
			m_ptr = ptr;
			if (m_ptr)m_ptr->add_ref();
			if (temp)temp->release();
		}

		void move(object_wrapper& ptr) {
			if ((pointer)this == (pointer)&reinterpret_cast<uint&>(ptr))
				return;
			reset();
			m_ptr = ptr.m_ptr;
			ptr.m_ptr = null;
		}

		type ** addres_of(void) {
			return&m_ptr;
		}

		type ** addres_for_init(void) {
			reset();
			return&m_ptr;
		}

	public: //operators

		object_wrapper& operator = (type* ptr) {
			set(ptr);
			return*this;
		}

		object_wrapper& operator = (ang::nullptr_t const&) {
			reset();
			return*this;
		}
		object_wrapper& operator = (object_wrapper && ptr) {
			move(ptr);
			return*this;
		}

		object_wrapper& operator = (object_wrapper const& ptr) {
			set(ptr.get());
			return*this;
		}

		object_wrapper_ptr<T> operator & (void);

		type* operator -> (void) {
			return get();
		}

		type const* operator -> (void)const {
			return get();
		}

		operator type* (void) {
			return get();
		}

		operator type const* (void)const {
			return get();
		}

		operator T& () {
			return get()->get();
		}

		operator T const& ()const {
			return get()->get();
		}

	private:
		type* m_ptr;
	};


	template<typename T, typename...Ts>
	inline shared_ptr<T> make_shared(Ts... args) {
		return new variable<T>(ang::forward<Ts>(args)...);
	}
}


namespace ang
{
	template<> 
	class LINK variable<bool> final
		: public smart<variable<bool>, ivariant, ang::value<bool>>
	{
	public:
		static value<bool> parse(text::raw_cstr_t);
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
	class LINK variable<int> final
		: public smart<variable<int>, ivariant, ang::value<int>>
	{
	public:
		template<typename T, text::encoding E> static inline value<int> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (int)str_to_signed(str, i, b);
		}
		static value<int> parse(text::raw_cstr_t, windex i = 0, int b = 10);
		static wstring to_string(value<int>, text::text_format = text::default_text_format<int>::format());

	public:
		variable();
		variable(int const&);
		variable(value<int> const& val);
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
	class LINK variable<uint> final
		: public smart<variable<uint>, ivariant, ang::value<uint>>
	{
	public:
		template<typename T, text::encoding E> static inline value<uint> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (uint)str_to_unsigned(str, i, b);
		}
		static value<uint> parse(text::raw_cstr_t, windex i = 0, int b = 10);
		static wstring to_string(value<uint>, text::text_format = text::default_text_format<uint>::format());

	public:
		variable();
		variable(uint const&);
		variable(value<uint> const& val);
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
	class LINK variable<long> final
		: public smart<variable<long>, ivariant, ang::value<long>>
	{
	public:
		template<typename T, text::encoding E> static inline value<long> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (long)str_to_signed(str, i, b);
		}
		static value<long> parse(text::raw_cstr_t, windex i = 0, int b = 10);
		static wstring to_string(value<long>, text::text_format = text::default_text_format<long>::format());

	public:
		variable();
		variable(long const&);
		variable(value<long> const& val);
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
	class LINK variable<ulong> final
		: public smart<variable<ulong>, ivariant, ang::value<ulong>>
	{
	public:
		template<typename T, text::encoding E> static inline value<ulong> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (ulong)str_to_unsigned(str, i, b);
		}
		static value<ulong> parse(text::raw_cstr_t, windex i = 0, int b = 10);
		static wstring to_string(value<ulong>, text::text_format = text::default_text_format<ulong>::format());

	public:
		variable();
		variable(ulong const&);
		variable(value<ulong> const& val);
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
	class LINK variable<long64> final
		: public smart<variable<long64>, ivariant, ang::value<long64>>
	{
	public:
		template<typename T, text::encoding E> static inline value<long64> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (long64)str_to_signed(str, i, b);
		}
		static value<long64> parse(text::raw_cstr_t, windex i = 0, int b = 10);
		static wstring to_string(value<long64>, text::text_format = text::default_text_format<long64>::format());

	public:
		variable();
		variable(long64 const&);
		variable(value<long64> const& val);
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
	class LINK variable<ulong64> final
		: public smart<variable<ulong64>, ivariant, ang::value<ulong64>>
	{
	public:
		template<typename T, text::encoding E> static inline value<ulong64> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (ulong64)str_to_unsigned(str, i, b);
		}
		static value<ulong64> parse(text::raw_cstr_t, windex i = 0, int b = 10);
		static wstring to_string(value<ulong64>, text::text_format = text::default_text_format<ulong64>::format());

	public:
		variable();
		variable(ulong64 const&);
		variable(value<ulong64> const& val);
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
	class LINK variable<float> final
		: public smart<variable<float>, ivariant, ang::value<float>>
	{
	public:
		template<typename T, text::encoding E> static inline value<float> parse(str_view<T, E> str, windex i = 0, bool exp = false) {
			return (float)str_to_floating(str, i, exp);
		}
		static value<float> parse(text::raw_cstr_t, windex i = 0, bool exp = false);
		static wstring to_string(value<float>, text::text_format = text::default_text_format<float>::format());

	public:
		variable();
		variable(float const&);
		variable(value<float> const& val);
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
	class LINK variable<double> final
		: public smart<variable<double>, ivariant, ang::value<double>>
	{
	public:
		template<typename T, text::encoding E> static inline value<double> parse(str_view<T, E> str, windex i = 0, bool exp = false) {
			return str_to_floating(str, i, exp);
		}
		static value<double> parse(text::raw_cstr_t, windex i = 0, bool exp = false);
		static wstring to_string(value<double>, text::text_format = text::default_text_format<double>::format());

	public:
		variable();
		variable(double const&);
		variable(value<double> const& val);
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


	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<bool>)
		object_wrapper(bool const& value) : m_ptr(null) { set(new variable<bool>(value)); }
		object_wrapper<variable<bool>>& operator = (const bool& value){
			if (is_empty())
				set(new variable<bool>(value));
			else
				m_ptr->set(value);
			return*this;
		}
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<int>)
		object_wrapper(int const& value) : m_ptr(null) { set(new variable<int>(value)); }
		object_wrapper<variable<int>>& operator = (const int& value){
			if (is_empty())
				set(new variable<int>(value));
			else
				m_ptr->set(value);
			return*this;
		}
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<uint>)
		object_wrapper(uint const& value) : m_ptr(null) { set(new variable<uint>(value)); }
		object_wrapper<variable<uint>>& operator = (const int& value){
			if (is_empty())
				set(new variable<uint>(value));
			else
				m_ptr->set(value);
			return*this;
		}
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<long>)
		object_wrapper(long const& value) : m_ptr(null) { set(new variable<long>(value)); }
		object_wrapper<variable<long>>& operator = (const long& value){
			if (is_empty())
				set(new variable<long>(value));
			else
				m_ptr->set(value);
			return*this;
		}
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<ulong>)
		object_wrapper(ulong const& value) : m_ptr(null) { set(new variable<ulong>(value)); }
		object_wrapper<variable<ulong>>& operator = (const ulong& value){
			if (is_empty())
				set(new variable<ulong>(value));
			else
				m_ptr->set(value);
			return*this;
		}
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<long64>)
		object_wrapper(long64 const& value) : m_ptr(null) { set(new variable<long64>(value)); }
		object_wrapper<variable<long64>>& operator = (const long64& value){
			if (is_empty())
				set(new variable<long64>(value));
			else
				m_ptr->set(value);
			return*this;
		}
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<ulong64>)
		object_wrapper(ulong64 const& value) : m_ptr(null) { set(new variable<ulong64>(value)); }
		object_wrapper<variable<ulong64>>& operator = (const ulong64& value){
			if (is_empty())
				set(new variable<ulong64>(value));
			else
				m_ptr->set(value);
			return*this;
		}
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<float>)
		object_wrapper(float const& value) : m_ptr(null) { set(new variable<float>(value)); }
		object_wrapper<variable<float>>& operator = (const float& value){
			if (is_empty())
				set(new variable<float>(value));
			else
				m_ptr->set(value);
			return*this;
		}
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<double>)
		object_wrapper(double const& value) : m_ptr(null) { set(new variable<double>(value)); }
		object_wrapper<variable<double>>& operator = (const double& value){
			if (is_empty())
				set(new variable<double>(value));
			else
				m_ptr->set(value);
			return*this;
		}
	ANG_END_OBJECT_WRAPPER();

	template<typename T>
	struct ivariant_setter {
		static bool set(ivariant* var, T const& value) {
			return var->set_value(type_of<T>(), (void*)&value);
		}
	};

	template<typename T>
	struct ivariant_setter<object_wrapper<T>> {
		static bool set(ivariant* var, object_wrapper<T> const& value) {
			return var->set_value(type_of<T>(), (void*)value.get());
		}
	};

	template<typename T>
	struct ivariant_setter<intf_wrapper<T>> {
		static bool set(ivariant* var, intf_wrapper<T> const& value) {
			return var->set_value(type_of<T>(), (void*)value.get());
		}
	};

	template<typename T, wsize N>
	struct ivariant_setter<T const(&)[N]> {
		static bool set(ivariant* var, T const(&ar)[N]) {
			array_view<T> value = collections::to_array((T*)ar, N);
			return var->set_value(type_of<array_view<T>>(), (void*)&value);
		}
	};

	template<typename T>
	inline bool ivariant::set_value(T const& value) {
		return ivariant_setter<T>::set(this, value);
	}


	template<typename T>
	struct ivariant_getter {
		static bool get(ivariant const* var, T& value) {
			return var->get_value(type_of<T>(), (void*)&value);
		}
	};

	template<typename T>
	struct ivariant_getter<intf_wrapper<T>> {
		static bool get(ivariant const* var, intf_wrapper<T>& value) {
			if (value.is_empty())return false;
			return var->get_value(type_of<T>(), (void*)value.get());
		}
	};

	template<typename T>
	struct ivariant_getter<object_wrapper<T>> {
		static bool get(ivariant const* var, object_wrapper<T>& value) {
			if (value.is_empty())return false;
			return var->get_value(type_of<T>(), (void*)value.get());
		}
	};

	template<typename T>
	struct ivariant_getter<array_view<T>> {
		static bool get(ivariant const* var, array_view<T>& value) {
			return var->get_value(type_of<array_view<T>>(), (void*)&value);
		}
	};

	template<typename T>
	inline bool ivariant::get_value(T& value)const {
		return ivariant_getter<T>::get(this, value);
	}

	template<typename T>
	inline intf_wrapper<ivariant>::intf_wrapper(T const& val)
		: intf_wrapper((ivariant*)new variable<T>(val)) {
	}

	template<typename T>
	inline intf_wrapper<ivariant>::intf_wrapper(variable<T>* val)
		: intf_wrapper((ivariant*)val) {
	}
}
#endif//__SAMRT_PTR_H__