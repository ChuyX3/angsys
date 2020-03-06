/*********************************************************************************************************************/
/*   File Name: ang/variable.h                                                                                       */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __VARIABLE_H__
#define __VARIABLE_H__

namespace ang
{
	template<typename T, bool = std::is_copy_constructible<T>::value>
	struct __variable_constructor_helper;

	template<typename T, gender_t TYPE>
	class variable final
		: public implement<variable<T, TYPE>
		, iid("ang::variable")
		, ivariable>
		, public ang::value<T>
	{
	public:
		static constexpr auto alloc = memory::auto_detect_allocator<T, ang_object_memory>::value;
		typedef typename ang::value<T> base_type;
		typedef typename ang::value<T>::type type;
		static constexpr gender_t gender_id = TYPE;

		friend __variable_constructor_helper<T>;

		pointer operator new(wsize sz)
		{
			return ang_alloc_object_memory(sz, alloc, (ushort)align_of<T>());
		}

		void operator delete(pointer ptr)
		{
			object::operator delete(ptr);
		}

#ifdef _DEBUG
		pointer operator new(wsize sz, const char* file, int line)
		{
			return ang_alloc_object_memory_debug(sz, file, line, alloc, align_of<T>());
		}
#endif

	public:
		variable();
		variable(T const& val);
		variable(ang::value<T> const& val)
			: variable(val.get()) {
		}
		variable(variable<T, TYPE> const* val);

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};

	template<typename T>
	class variable<enum_class<T>, gender::enum_type> final
		: public implement<variable<enum_class<T>, gender::enum_type>
		, iid("ang::variable")
		, ivariable>
		, public enum_class<T>
	{
	public:
		static constexpr auto alloc = memory::auto_detect_allocator<enum_class<T>, ang_object_memory>::value;
		typedef enum_class<T> base_type;
		typedef typename enum_class<T>::type type;
		static constexpr gender_t gender_id = gender::enum_type;

		friend __variable_constructor_helper<T>;

		pointer operator new(wsize sz)
		{
			return ang_alloc_object_memory(sz, alloc, align_of<T>());
		}

		void operator delete(pointer ptr)
		{
			object::operator delete(ptr);
		}

#ifdef _DEBUG
		pointer operator new(wsize sz, const char* file, int line)
		{
			return ang_alloc_object_memory(sz, file, line, alloc, align_of<T>());
		}
#endif

	public:
		variable();
		variable(T const& val);
		variable(ang::value<T> const& val)
			: variable(val.get()) {
		}
		variable(enum_class<T> const& val)
			: variable(val.get()) {
		}
		variable(variable<enum_class<T>, gender::enum_type> const* val);

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};

	template<typename T>
	class variable<flags_class<T>, gender::enum_type> final
		: public implement<variable<flags_class<T>, gender::enum_type>
		, iid("ang::variable")
		, ivariable>
		, public flags_class<T>
	{
	public:
		static constexpr auto alloc = memory::auto_detect_allocator<flags_class<T>, ang_object_memory>::value;
		typedef flags_class<T> base_type;
		typedef typename flags_class<T>::type type;
		static constexpr gender_t gender_id = gender::enum_type;

		friend __variable_constructor_helper<T>;

		pointer operator new(wsize sz)
		{
			return ang_alloc_object_memory(sz, alloc, align_of<T>());
		}

		void operator delete(pointer ptr)
		{
			object::operator delete(ptr);
		}

#ifdef _DEBUG
		pointer operator new(wsize sz, const char* file, int line)
		{
			return ang_alloc_object_memory(sz, file, line, alloc, align_of<T>());
		}
#endif

	public:
		variable();
		variable(T const& val);
		variable(ang::value<T> const& val)
			: variable(val.get()) {
		}
		variable(flags_class<T> const& val)
			: variable(val.get()) {
		}
		variable(variable<flags_class<T>, gender::enum_type> const* val);

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};


	template<typename T, gender_t TYPE>
	class object_wrapper<variable<T, TYPE>>
	{
	public:
		typedef typename variable<T, TYPE>::base_type base_type;
		typedef typename variable<T, TYPE>::type data_type;
		typedef variable<T, TYPE> type;
		typedef variable<T, TYPE>* type_ptr;
		typedef variable<T, TYPE>& type_ref;
		typedef variable<T, TYPE> const* ctype_ptr;
		typedef variable<T, TYPE> const& ctype_ref;

	public:
		object_wrapper()
			: m_ptr(null) {
		}

		object_wrapper(data_type const& val)
			: object_wrapper() {
			set(new type(val));
		}

		object_wrapper(base_type const& val)
			: object_wrapper() {
			set(new type(val));
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
		object_wrapper& operator = (data_type const& val) {
			if (is_empty())
				set(new type(val));
			else
				get()->set(val);
			return*this;
		}

		object_wrapper& operator = (base_type const& val) {
			if (is_empty())
				set(new type(val));
			else
				get()->set(val);
			return*this;
		}

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

		type* operator -> (void)const {
			return get();
		}

		operator type* (void) {
			return get();
		}

		operator type const* (void)const {
			return get();
		}

		operator data_type& () {
			return get()->get();
		}

		operator data_type const& ()const {
			return get()->get();
		}

	private:
		type* m_ptr;
	};

	template<typename T>
	class object_wrapper<variable<T, gender::class_type>>
	{
	public:
		typedef typename variable<T, gender::class_type>::type data_type;
		typedef variable<T, gender::class_type> type;
		typedef variable<T, gender::class_type>* type_ptr;
		typedef variable<T, gender::class_type>& type_ref;
		typedef variable<T, gender::class_type> const* ctype_ptr;
		typedef variable<T, gender::class_type> const& ctype_ref;

	public:
		object_wrapper()
			: m_ptr(null) {
		}

		object_wrapper(T const& val)
			: object_wrapper() {
			set(new type(val));
		}

		object_wrapper(value<T> const& val)
			: object_wrapper() {
			set(new type(val));
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

		data_type* operator -> (void)const {
			return &get()->get();
		}

		operator type* (void) {
			return get();
		}

		operator type const* (void)const {
			return get();
		}

		operator data_type& () {
			return get()->get();
		}

		operator data_type const& ()const {
			return get()->get();
		}

	private:
		type* m_ptr;
	};

	template<typename T>
	class object_wrapper<variable<T, gender::union_type>>
	{
	public:
		typedef typename variable<T, gender::union_type>::type data_type;
		typedef variable<T, gender::union_type> type;
		typedef variable<T, gender::union_type>* type_ptr;
		typedef variable<T, gender::union_type>& type_ref;
		typedef variable<T, gender::union_type> const* ctype_ptr;
		typedef variable<T, gender::union_type> const& ctype_ref;

	public:
		object_wrapper()
			: m_ptr(null) {
		}

		object_wrapper(T const& val)
			: object_wrapper() {
			set(new type(val));
		}

		object_wrapper(value<T> const& val)
			: object_wrapper() {
			set(new type(val));
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

		data_type* operator -> (void)const {
			return &get()->get();
		}

		operator type* (void) {
			return get();
		}

		operator type const* (void)const {
			return get();
		}

		operator data_type& () {
			return get()->get();
		}

		operator data_type const& ()const {
			return get()->get();
		}

	private:
		type* m_ptr;
	};


	template<typename T, typename...Ts>
	inline shared_ptr<T> make_shared(Ts&&... args) {
		return new typename smart_ptr_type<T>::type(ang::forward<Ts>(args)...);
	}

	template<typename T, typename U>
	inline shared_ptr<T> make_shared(ang::initializer_list<U>&& args) {
		return new typename smart_ptr_type<T>::type(ang::forward<ang::initializer_list<U>>(args));
	}
}

namespace ang
{
	template<>
	class LINK variable<bool, gender::value_type> final
		: public implement<variable<bool, gender::value_type>
		, iid("ang::boolean")
		, ivariable>
		, public ang::value<bool>
	{
	public:
		typedef value<bool> base_type;
		typedef typename base_type::type type;
		
		static value<bool> parse(cstr_t);
		static string to_string(value<bool>, text::text_format = text::default_text_format<bool>::format());

	public:
		variable();
		variable(bool const&);
		variable(value<bool> const& val);
		variable(variable const*);
		template<typename T> variable(value<T> const& other)
			: value<T>(other.get() ? true : false) {}

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<int, gender::value_type> final
		: public implement<variable<int, gender::value_type>
		, iid("ang::integer")
		, ivariable>
		, public ang::value<int>
	{
	public:
		typedef value<int> base_type;
		typedef typename base_type::type type;

		template<typename T, text::encoding E> static inline value<int> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (int)str_to_signed(str, i, b);
		}
		static value<int> parse(cstr_t, windex i = 0, int b = 10);
		static string to_string(value<int> val, text::text_format f = text::default_text_format<int>::format()) {
			return ang::to_string<int>(val, f);
		}

	public:
		variable();
		variable(int const&);
		variable(value<int> const& val);
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<uint, gender::value_type> final
		: public implement<variable<uint, gender::value_type>
		, iid("ang::uinteger")
		, ivariable>
		, public ang::value<uint>
	{
	public:
		typedef value<uint> base_type;
		typedef typename base_type::type type;

		template<typename T, text::encoding E> static inline value<uint> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (uint)str_to_unsigned(str, i, b);
		}
		static value<uint> parse(cstr_t, windex i = 0, int b = 10);
		static string to_string(value<uint> val, text::text_format f = text::default_text_format<uint>::format()) {
			return ang::to_string<uint>(val, f);
		}

	public:
		variable();
		variable(uint const&);
		variable(value<uint> const& val);
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<long, gender::value_type> final
		: public implement<variable<long, gender::value_type>
		, iid("ang::linteger")
		, ivariable>
		, public ang::value<long>
	{
	public:
		typedef value<long> base_type;
		typedef typename base_type::type type;

		template<typename T, text::encoding E> static inline value<long> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (long)str_to_signed(str, i, b);
		}
		static value<long> parse(cstr_t, windex i = 0, int b = 10);
		static string to_string(value<long> val, text::text_format f = text::default_text_format<long>::format()) {
			return ang::to_string<long>(val, f);
		}

	public:
		variable();
		variable(long const&);
		variable(value<long> const& val);
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<ulong, gender::value_type> final
		: public implement<variable<ulong, gender::value_type>
		, iid("ang::ulinteger")
		, ivariable>
		, public ang::value<ulong>
	{
	public:
		typedef value<ulong> base_type;
		typedef typename base_type::type type;

		template<typename T, text::encoding E> static inline value<ulong> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (ulong)str_to_unsigned(str, i, b);
		}
		static value<ulong> parse(cstr_t, windex i = 0, int b = 10);
		static string to_string(value<ulong> val, text::text_format f = text::default_text_format<ulong>::format()) {
			return ang::to_string<ulong>(val, f);
		}

	public:
		variable();
		variable(ulong const&);
		variable(value<ulong> const& val);
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<long64, gender::value_type> final
		: public implement<variable<long64, gender::value_type>
		, iid("ang::integer64")
		, ivariable>
		, public ang::value<long64>
	{
	public:
		typedef value<long64> base_type;
		typedef typename base_type::type type;

		template<typename T, text::encoding E> static inline value<long64> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (long64)str_to_signed(str, i, b);
		}
		static value<long64> parse(cstr_t, windex i = 0, int b = 10);
		static string to_string(value<long64> val, text::text_format f = text::default_text_format<long64>::format()) {
			return ang::to_string<long64>(val, f);
		}

	public:
		variable();
		variable(long64 const&);
		variable(value<long64> const& val);
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<ulong64, gender::value_type> final
		: public implement<variable<ulong64, gender::value_type>
		, iid("ang::uinteger64")
		, ivariable>
		, public ang::value<ulong64>
	{
	public:
		typedef value<ulong64> base_type;
		typedef typename base_type::type type;

		template<typename T, text::encoding E> static inline value<ulong64> parse(str_view<T, E> str, windex i = 0, int b = 10) {
			return (ulong64)str_to_unsigned(str, i, b);
		}
		static value<ulong64> parse(cstr_t, windex i = 0, int b = 10);
		static string to_string(value<ulong64> val, text::text_format f = text::default_text_format<ulong64>::format()) {
			return ang::to_string<ulong64>(val, f);
		}

	public:
		variable();
		variable(ulong64 const&);
		variable(value<ulong64> const& val);
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<float, gender::value_type> final
		: public implement<variable<float, gender::value_type>
		, iid("ang::floating32")
		, ivariable>
		, public ang::value<float>
	{
	public:
		typedef value<float> base_type;
		typedef typename base_type::type type;

		template<typename T, text::encoding E> static inline value<float> parse(str_view<T, E> str, windex i = 0, bool exp = false) {
			return (float)str_to_floating(str, i, exp);
		}
		static value<float> parse(cstr_t, windex i = 0, bool exp = false);
		static string to_string(value<float> val, text::text_format f = text::default_text_format<float>::format()) {
			return ang::to_string<float>(val, f);
		}

	public:
		variable();
		variable(float const&);
		variable(value<float> const& val);
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};

	template<>
	class LINK variable<double, gender::value_type> final
		: public implement<variable<double, gender::value_type>
		, iid("ang::floating64")
		, ivariable>
		, public ang::value<double>
	{
	public:
		typedef value<double> base_type;
		typedef typename base_type::type type;

		template<typename T, text::encoding E> static inline value<double> parse(str_view<T, E> str, windex i = 0, bool exp = false) {
			return str_to_floating(str, i, exp);
		}
		static value<double> parse(cstr_t, windex i = 0, bool exp = false);
		static string to_string(value<double> val, text::text_format f = text::default_text_format<double>::format()) {
			return ang::to_string<double>(val, f);
		}

	public:
		variable();
		variable(double const&);
		variable(value<double> const& val);
		variable(variable const*);
		template<typename T>
		variable(value<T> const& other) : value(other.get()) {}

	public: //overrides
		virtual string to_string()const override;
		virtual string to_string(text::text_format_t)const override;

		virtual rtti_t const& value_type()const override;
		virtual bool set_value(rtti_t const&, unknown_t) override;
		virtual bool get_value(rtti_t const&, unknown_t)const override;
		virtual variant clone()const override;

		using ivariable::set_value;
		using ivariable::get_value;

	protected:
		virtual~variable();
	};


	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<bool>)
		typedef typename type::base_type base_type;
		typedef typename type::type data_type;
		object_wrapper(bool const& value) : m_ptr(null) { set(new variable<bool>(value)); }
		object_wrapper<variable<bool>>& operator = (const bool& value) {
			if (is_empty())
				set(new variable<bool>(value));
			else
				m_ptr->set(value);
			return*this;
		}
		operator data_type& () { return get()->get(); }
		operator data_type const& ()const { return get()->get(); }
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<int>)
		typedef typename type::base_type base_type;
		typedef typename type::type data_type;
		object_wrapper(int const& value) : m_ptr(null) { set(new variable<int>(value)); }
		object_wrapper<variable<int>>& operator = (const int& value) {
			if (is_empty())
				set(new variable<int>(value));
			else
				m_ptr->set(value);
			return*this;
		}
		operator data_type& () { return get()->get(); }
		operator data_type const& ()const { return get()->get(); }
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<uint>)
		typedef typename type::base_type base_type;
		typedef typename type::type data_type;
		object_wrapper(uint const& value) : m_ptr(null) { set(new variable<uint>(value)); }
		object_wrapper<variable<uint>>& operator = (const int& value) {
			if (is_empty())
				set(new variable<uint>(value));
			else
				m_ptr->set(value);
			return*this;
		}
		operator data_type& () { return get()->get(); }
		operator data_type const& ()const { return get()->get(); }
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<long>)
		typedef typename type::base_type base_type;
		typedef typename type::type data_type;
		object_wrapper(long const& value) : m_ptr(null) { set(new variable<long>(value)); }
		object_wrapper<variable<long>>& operator = (const long& value) {
			if (is_empty())
				set(new variable<long>(value));
			else
				m_ptr->set(value);
			return*this;
		}
		operator data_type& () { return get()->get(); }
		operator data_type const& ()const { return get()->get(); }
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<ulong>)
		typedef typename type::base_type base_type;
		typedef typename type::type data_type;
		object_wrapper(ulong const& value) : m_ptr(null) { set(new variable<ulong>(value)); }
		object_wrapper<variable<ulong>>& operator = (const ulong& value) {
			if (is_empty())
				set(new variable<ulong>(value));
			else
				m_ptr->set(value);
			return*this;
		}
		operator data_type& () { return get()->get(); }
		operator data_type const& ()const { return get()->get(); }
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<long64>)
		typedef typename type::base_type base_type;
		typedef typename type::type data_type;
		object_wrapper(long64 const& value) : m_ptr(null) { set(new variable<long64>(value)); }
		object_wrapper<variable<long64>>& operator = (const long64& value) {
			if (is_empty())
				set(new variable<long64>(value));
			else
				m_ptr->set(value);
			return*this;
		}
		operator data_type& () { return get()->get(); }
		operator data_type const& ()const { return get()->get(); }
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<ulong64>)
		typedef typename type::base_type base_type;
		typedef typename type::type data_type;
		object_wrapper(ulong64 const& value) : m_ptr(null) { set(new variable<ulong64>(value)); }
		object_wrapper<variable<ulong64>>& operator = (const ulong64& value) {
			if (is_empty())
				set(new variable<ulong64>(value));
			else
				m_ptr->set(value);
			return*this;
		}
		operator data_type& () { return get()->get(); }
		operator data_type const& ()const { return get()->get(); }
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<float>)
		typedef typename type::base_type base_type;
		typedef typename type::type data_type;
		object_wrapper(float const& value) : m_ptr(null) { set(new variable<float>(value)); }
		object_wrapper<variable<float>>& operator = (const float& value) {
			if (is_empty())
				set(new variable<float>(value));
			else
				m_ptr->set(value);
			return*this;
		}
		operator data_type& () { return get()->get(); }
		operator data_type const& ()const { return get()->get(); }
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, variable<double>)
		typedef typename type::base_type base_type;
		typedef typename type::type data_type;
		object_wrapper(double const& value) : m_ptr(null) { set(new variable<double>(value)); }
		object_wrapper<variable<double>>& operator = (const double& value) {
			if (is_empty())
				set(new variable<double>(value));
			else
				m_ptr->set(value);
			return*this;
		}
		operator data_type& () { return get()->get(); }
		operator data_type const& ()const { return get()->get(); }
	ANG_END_OBJECT_WRAPPER();


	template<typename T>
	inline intf_wrapper<ivariable>::intf_wrapper(T const& val)
		: intf_wrapper((ivariable*)new variable<T>(val)) {
		//static_assert(false);
	}

	template<typename T>
	inline intf_wrapper<ivariable>::intf_wrapper(variable<T>* val)
		: intf_wrapper((ivariable*)val) {
		//static_assert(false);
	}
}

#endif/*__VARIABLE_H__*/