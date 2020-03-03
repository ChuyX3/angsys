#ifndef __SAMRT_PTR_H__
#error ...
#elif !defined __VARIABLE_INL__
#define __VARIABLE_INL__

namespace ang
{
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

	template<typename T>
	inline variable<T>::variable()
	{

	}

	template<typename T>
	inline variable<T>::variable(T const& val)
	{
		__variable_constructor_helper<T>::construct(this, val);
	}

	template<typename T>
	inline variable<T>::variable(variable<T> const* val)
	{
		__variable_constructor_helper<T>::construct(this, (value<T> const&)*val);
	}

	template<typename T>
	inline variable<T>::~variable()
	{
	}

	template<typename T>
	inline rtti_t const& variable<T>::class_info()
	{
		static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::variable<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
		static rtti_t const* parents[] = { &runtime::type_of<object>(), &runtime::type_of<ivariant>() };
		static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<variable<T>>(), alignof(variable<T>), parents, &intf::default_query_interface);
		return info;
	}

	template<typename T>
	inline rtti_t const& variable<T>::runtime_info()const
	{
		return class_info();
	}

	template<typename T>
	inline bool variable<T>::query_interface(rtti_t const& id, unknown_ptr_t out)
	{
		if (id.type_id() == class_info().type_id())
		{
			if (out == null) return false;
			*out = static_cast<variable<T>*>(this);
			return true;
		}
		else if (object::query_interface(id, out))
		{
			return true;
		}
		else if (id.type_id() == type_of<ivariant>().type_id()) {
			if (out == null) return false;
			*out = static_cast<ivariant*>(this);
			return true;
		}
		else if (id.type_id() == type_of<T>().type_id()) {
			if (out == null) return false;
			*out = (T*)&this->get();
			return true;
		}
		return false;
	}

	template<typename T>
	inline string variable<T>::to_string()const
	{
		return class_info().type_name();
	}

	template<typename T>
	inline string variable<T>::to_string(text::text_format_t)const
	{
		return class_info().type_name();
	}

	template<typename T>
	inline comparision_result_t variable<T>::compare(object const* obj)const 
	{
		//TODO:
		return comparision_result::diferent; 
	}

	template<typename T>
	inline rtti_t const& variable<T>::value_type()const
	{
		return type_of<T>();
	}

	template<typename T>
	inline bool variable<T>::set_value(rtti_t const&, unknown_t)
	{
		//TODO:
		return false;
	}

	template<typename T>
	inline bool variable<T>::get_value(rtti_t const&, unknown_t)const
	{
		//TODO:
		return false;
	}

	template<typename T>
	inline variant variable<T>::clone()const
	{
		return (ivariant*)new variable<T>(this);
	}


	///////////////////////////////////////////////////////////////////////////

	template<typename T>
	inline intf_wrapper<ivariant>::intf_wrapper(ang::shared_ptr<T> ptr)
		: intf_wrapper<ivariant>(null) {
		set(ptr.get());
	}

	template<wsize N>
	inline intf_wrapper<ivariant>::intf_wrapper(const char(&ar)[N])
		: intf_wrapper<ivariant>(null) {
		set(new text::basic_string_buffer<text::encoding::ascii>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariant>::intf_wrapper(const wchar(&ar)[N])
		: intf_wrapper<ivariant>(null) {
		set(new text::basic_string_buffer<text::encoding::unicode>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariant>::intf_wrapper(const mchar(&ar)[N])
		: intf_wrapper<ivariant>(null) {
		set(new text::basic_string_buffer<text::encoding::utf8>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariant>::intf_wrapper(const char16(&ar)[N])
		: intf_wrapper<ivariant>(null) {
		set(new text::basic_string_buffer<text::encoding::utf16>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariant>::intf_wrapper(const char32(&ar)[N])
		: intf_wrapper<ivariant>(null) {
		set(new text::basic_string_buffer<text::encoding::utf32>(ar));
	}

	template<typename T, text::encoding E>
	inline intf_wrapper<ivariant>::intf_wrapper(str_view<T, E> str)
		: intf_wrapper<ivariant>(null) {
		set(new text::basic_string_buffer<E>(str));
	}

	template<typename T, wsize N>
	inline intf_wrapper<ivariant>::intf_wrapper(T(&ar)[N])
		: intf_wrapper<ivariant>(null) {
		set(new collections::array_buffer<T>(ar));
	}

	template<text::encoding E, template<typename>class A>
	inline intf_wrapper<ivariant>::intf_wrapper(text::basic_string<E, A> const& str) 
		: intf_wrapper<ivariant>(null) {
		set(str.is_empty() ? null : str.get());
	}


	template<typename T> 
	intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator = (T* ptr) {
		static_assert(is_base_of<ivariant, T>::value, "Invalid argument for var");
		set(ptr);
		return*this;
	}

	template<typename T> 
	intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator = (shared_ptr<T> ptr) {
		set(ptr.get());
		return*this;
	}

	template<typename T> 
	intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator = (intf_wrapper<T> const& ptr) {
		ivariant* var_ = interface_cast<ivariant>(ptr.get());
		set(var_);
		return*this;
	}

	template<typename T, text::encoding E> 
	intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator = (str_view<T, E> const& str) {
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
	intf_wrapper<ivariant>& intf_wrapper<ivariant>::operator = (T(&ar)[N]) {
		if (interface_cast<collections::iarray<T>>(get()))
		{
			get()->set_value(ar);
		}
		else
		{
			array<T> arr = ar;
			set((ivariant*)arr.get());
		}
		return*this;
	}
}


#endif//__VARIABLE_INL__