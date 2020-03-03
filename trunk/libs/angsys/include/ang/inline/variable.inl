#ifndef __SAMRT_PTR_H__
#error ...
#elif !defined __VARIABLE_INL__
#define __VARIABLE_INL__

namespace ang
{

	template<typename T, gender_t TYPE>
	struct intf_class_info<variable<T, TYPE>>
	{
		static inline rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= rtti_from_type<variable<T, TYPE>>::types();
				//= {	&runtime::type_of<bean>(), &runtime::type_of<ivariable>() };
			static rtti_t const& info = rtti::regist([]()->fast_astring_t {
					fast_astring_t out;
					out << "ang::variable<"_sv << rtti::type_of<T>().type_name() << ">"_sv;
					return ang::move(out);
				}()
				, gender::class_type
				, size_of<variable<T, TYPE>>()
				, align_of<variable<T, TYPE>>()
				, parents
				, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<variable<enum_class<T>, ang::gender::enum_type>>
	{
		static inline rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= rtti_from_type<variable<enum_class<T>, ang::gender::enum_type>>::types();
			static rtti_t const& info = rtti::regist([]()->fast_astring_t
				{
					fast_astring_t out;
					out << "ang::variable<"_sv << rtti::type_of<enum_class<T>>().type_name() << ">"_sv;
					return ang::move(out);
				}()
				, gender::class_type
				, size_of<variable<enum_class<T>, ang::gender::enum_type>>()
				, align_of<variable<enum_class<T>, ang::gender::enum_type>>()
				, parents
				, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<variable<flags_class<T>, ang::gender::enum_type>>
	{
		static inline rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= rtti_from_type<variable<flags_class<T>, ang::gender::enum_type>>::types();
			static rtti_t const& info = rtti::regist([]()->fast_astring_t
				{
					fast_astring_t out;
					out << "ang::variable<"_sv << rtti::type_of<flags_class<T>>().type_name() << ">"_sv;
					return ang::move(out);
				}()
				, gender::class_type
				, size_of<variable<flags_class<T>, ang::gender::enum_type>>()
				, align_of<variable<flags_class<T>, ang::gender::enum_type>>()
				, parents
				, &iintf::default_query_interface);
			return info;
		}
	};
	

	template<typename T>
	struct __variable_constructor_helper<T, true>
	{
		static void construct(variable<T>* ptr, T const& val)
		{
			ptr->set(val);
		}
		static void construct(variable<T>* ptr, value<T> const& val)
		{
			ptr->set(val);
		}
	};

	template<typename T>
	struct __variable_constructor_helper<enum_class<T>, true>
	{
		static void construct(variable<enum_class<T>>* ptr, T const& val)
		{
			ptr->set(val);
		}
		static void construct(variable<enum_class<T>>* ptr, value<T> const& val)
		{
			ptr->set(val);
		}
	};

	template<typename T>
	struct __variable_constructor_helper<flags_class<T>, true>
	{
		static void construct(variable<flags_class<T>>* ptr, T const& val)
		{
			ptr->set(val);
		}
		static void construct(variable<flags_class<T>>* ptr, value<T> const& val)
		{
			ptr->set(val);
		}
	};

	template<typename T>
	struct __variable_constructor_helper<T, false>
	{
		static void construct(variable<T>* ptr, T const& val)
		{
#ifdef WINDOWS_PLATFORM
#ifdef _DEBUG
			__debugbreak();
#endif
#endif
		}
		static void construct(variable<T>* ptr, value<T> const& val)
		{
#ifdef WINDOWS_PLATFORM
#ifdef _DEBUG
			__debugbreak();
#endif
#endif
		}
	};

	template<typename T, gender_t TYPE>
	inline variable<T, TYPE>::variable()
	{

	}

	template<typename T, gender_t TYPE>
	inline variable<T, TYPE>::variable(T const& val)
	{
		__variable_constructor_helper<T>::construct(this, val);
	}

	template<typename T, gender_t TYPE>
	inline variable<T, TYPE>::variable(variable<T, TYPE> const* val)
	{
		__variable_constructor_helper<T>::construct(this, (value<T> const&)*val);
	}

	template<typename T, gender_t TYPE>
	inline variable<T, TYPE>::~variable()
	{
	}

	template<typename T, gender_t TYPE>
	inline string variable<T, TYPE>::to_string()const
	{
		return ang::to_string<value<T, TYPE>>(*this);
	}

	template<typename T, gender_t TYPE>
	inline string variable<T, TYPE>::to_string(text::text_format_t f)const
	{
		return ang::to_string<value<T, TYPE>>(*this, f);
	}

	template<typename T, gender_t TYPE>
	inline rtti_t const& variable<T, TYPE>::value_type()const
	{
		return type_of<T>();
	}

	template<typename T, gender_t TYPE>
	inline bool variable<T, TYPE>::set_value(rtti_t const&, unknown_t)
	{
		//TODO:
		return false;
	}

	template<typename T, gender_t TYPE>
	inline bool variable<T, TYPE>::get_value(rtti_t const&, unknown_t)const
	{
		//TODO:
		return false;
	}

	template<typename T, gender_t TYPE>
	inline variant variable<T, TYPE>::clone()const
	{
		return (ivariable*)new variable<T, TYPE>(this);
	}


	///////////////////////////////////////////////////////////////////////////


	template<typename T>
	inline variable<enum_class<T>, ang::gender::enum_type>::variable()
	{

	}

	template<typename T>
	inline variable<enum_class<T>, ang::gender::enum_type>::variable(T const& val)
	{
		__variable_constructor_helper<enum_class<T>>::construct(this, val);
	}

	template<typename T>
	inline variable<enum_class<T>, ang::gender::enum_type>::variable(variable<enum_class<T>, ang::gender::enum_type> const* val)
	{
		__variable_constructor_helper<enum_class<T>>::construct(this, (value<T> const&)*val);
	}

	template<typename T>
	inline variable<enum_class<T>, ang::gender::enum_type>::~variable()
	{
	}

	template<typename T>
	inline string variable<enum_class<T>, ang::gender::enum_type>::to_string()const
	{
		return ang::to_string<enum_class<T>>(*this);
	}

	template<typename T>
	inline string variable<enum_class<T>, ang::gender::enum_type>::to_string(text::text_format_t f)const
	{
		return ang::to_string<enum_class<T>>(*this, f);
	}

	template<typename T>
	inline rtti_t const& variable<enum_class<T>, ang::gender::enum_type>::value_type()const
	{
		return type_of<T>();
	}

	template<typename T>
	inline bool variable<enum_class<T>, ang::gender::enum_type>::set_value(rtti_t const&, unknown_t)
	{
		//TODO:
		return false;
	}

	template<typename T>
	inline bool variable<enum_class<T>, ang::gender::enum_type>::get_value(rtti_t const&, unknown_t)const
	{
		//TODO:
		return false;
	}

	template<typename T>
	inline variant variable<enum_class<T>, ang::gender::enum_type>::clone()const
	{
		return (ivariable*)new variable<enum_class<T>, ang::gender::enum_type>(this);
	}


	///////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////


	template<typename T>
	inline variable<flags_class<T>, ang::gender::enum_type>::variable()
	{

	}

	template<typename T>
	inline variable<flags_class<T>, ang::gender::enum_type>::variable(T const& val)
	{
		__variable_constructor_helper<flags_class<T>>::construct(this, val);
	}

	template<typename T>
	inline variable<flags_class<T>, ang::gender::enum_type>::variable(variable<flags_class<T>, ang::gender::enum_type> const* val)
	{
		__variable_constructor_helper<flags_class<T>>::construct(this, (value<T> const&)*val);
	}

	template<typename T>
	inline variable<flags_class<T>, ang::gender::enum_type>::~variable()
	{
	}

	template<typename T>
	inline string variable<flags_class<T>, ang::gender::enum_type>::to_string()const
	{
		return ang::to_string<flags_class<T>>(*this);
	}

	template<typename T>
	inline string variable<flags_class<T>, ang::gender::enum_type>::to_string(text::text_format_t f)const
	{
		return ang::to_string<flags_class<T>>(*this, f);
	}

	template<typename T>
	inline rtti_t const& variable<flags_class<T>, ang::gender::enum_type>::value_type()const
	{
		return type_of<T>();
	}

	template<typename T>
	inline bool variable<flags_class<T>, ang::gender::enum_type>::set_value(rtti_t const&, unknown_t)
	{
		//TODO:
		return false;
	}

	template<typename T>
	inline bool variable<flags_class<T>, ang::gender::enum_type>::get_value(rtti_t const&, unknown_t)const
	{
		//TODO:
		return false;
	}

	template<typename T>
	inline variant variable<flags_class<T>, ang::gender::enum_type>::clone()const
	{
		return (ivariable*)new variable<flags_class<T>, ang::gender::enum_type>(this);
	}


	///////////////////////////////////////////////////////////////////////////

	template<typename T, gender_t TYPE>
	inline intf_wrapper<ivariable>::intf_wrapper(object_wrapper<variable<T, TYPE>> ptr)
		: intf_wrapper<ivariable>(null) {
		set(ptr.get());
	}

	template<wsize N>
	inline intf_wrapper<ivariable>::intf_wrapper(const char(&ar)[N])
		: intf_wrapper<ivariable>(null) {
		set(new text::basic_string_buffer<text::encoding::ascii>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariable>::intf_wrapper(const wchar(&ar)[N])
		: intf_wrapper<ivariable>(null) {
		set(new text::basic_string_buffer<text::encoding::unicode>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariable>::intf_wrapper(const mchar(&ar)[N])
		: intf_wrapper<ivariable>(null) {
		set(new text::basic_string_buffer<text::encoding::utf8>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariable>::intf_wrapper(const char16(&ar)[N])
		: intf_wrapper<ivariable>(null) {
		set(new text::basic_string_buffer<text::encoding::utf16>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariable>::intf_wrapper(const char32(&ar)[N])
		: intf_wrapper<ivariable>(null) {
		set(new text::basic_string_buffer<text::encoding::utf32>(ar));
	}

	template<typename T, text::encoding E>
	inline intf_wrapper<ivariable>::intf_wrapper(str_view<T, E> str)
		: intf_wrapper<ivariable>(null) {
		set(new text::basic_string_buffer<E>(str));
	}

	template<typename T, wsize N>
	inline intf_wrapper<ivariable>::intf_wrapper(T(&ar)[N])
		: intf_wrapper<ivariable>(null) {
		set(new collections::array_buffer<T>(ar));
	}

	template<text::encoding E, template<typename>class A>
	inline intf_wrapper<ivariable>::intf_wrapper(text::fast_string<E, A> const& str) 
		: intf_wrapper<ivariable>(null) {
		set(new text::basic_string_buffer<E>(str.cstr()));
	}

	template<text::encoding E>
	inline intf_wrapper<ivariable>::intf_wrapper(text::basic_string<E> const& str)
		: intf_wrapper<ivariable>(null) {
		set(str.is_empty() ? new text::basic_string_buffer<E>() : str.get());
	}

	template<typename T> 
	intf_wrapper<ivariable>& intf_wrapper<ivariable>::operator = (T* ptr) {
		static_assert(is_base_of<ivariable, T>::value, "Invalid argument for var");
		set(ptr);
		return*this;
	}

	template<typename T, gender_t TYPE> 
	intf_wrapper<ivariable>& intf_wrapper<ivariable>::operator = (object_wrapper<variable<T, TYPE>> ptr) {
		set(ptr.get());
		return*this;
	}

	template<typename T> 
	intf_wrapper<ivariable>& intf_wrapper<ivariable>::operator = (intf_wrapper<T> const& ptr) {
		ivariable* var_ = interface_cast<ivariable>(ptr.get());
		set(var_);
		return*this;
	}

	template<typename T, text::encoding E> 
	intf_wrapper<ivariable>& intf_wrapper<ivariable>::operator = (str_view<T, E> const& str) {
		text::istring *_string;
		if(_string = interface_cast<text::istring>(get()))
		{
			_string->copy(str);
		}
		else
		{
			set(new text::basic_string_buffer<E>(str));
		}
		return*this;
	}

	template<typename T, wsize N> 
	intf_wrapper<ivariable>& intf_wrapper<ivariable>::operator = (T(&ar)[N]) {
		if (interface_cast<collections::iarray<T>>(get()))
		{
			get()->set_value(ar);
		}
		else
		{
			smart_array<T> arr = ar;
			set((ivariable*)arr.get());
		}
		return*this;
	}
}


#endif//__VARIABLE_INL__