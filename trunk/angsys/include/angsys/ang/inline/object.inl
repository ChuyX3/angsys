#ifndef __SAMRT_PTR_H__
#error ...
#elif !defined __OBJECT_INL__
#define __OBJECT_INL__

namespace ang
{
	template<wsize N> 
	inline object_wrapper<object>::object_wrapper(const char(&ar)[N]) 
		: object_wrapper<object>(null) {
		set(new strings::basic_string_buffer<text::encoding::ascii>(ar));
	}

	template<wsize N> 
	inline object_wrapper<object>::object_wrapper(const wchar(&ar)[N])
		: object_wrapper<object>(null) {
		set(new strings::basic_string_buffer<text::encoding::unicode>(ar));
	}

	template<wsize N>
	inline object_wrapper<object>::object_wrapper(const mchar(&ar)[N])
		: object_wrapper<object>(null) {
		set(new strings::basic_string_buffer<text::encoding::utf8>(ar));
	}

	template<wsize N>
	inline object_wrapper<object>::object_wrapper(const char16(&ar)[N])
		: object_wrapper<object>(null) {
		set(new strings::basic_string_buffer<text::encoding::utf16>(ar));
	}

	template<wsize N> 
	inline object_wrapper<object>::object_wrapper(const char32(&ar)[N])
		: object_wrapper<object>(null) {
		set(new strings::basic_string_buffer<text::encoding::utf32>(ar));
	}
	
	template<typename T, text::encoding E>
	inline object_wrapper<object>::object_wrapper(str_view<T, E> str)
		: object_wrapper<object>(null) {
		set(new strings::basic_string_buffer<E>(str));
	}

	template<typename T, wsize N>
	inline object_wrapper<object>::object_wrapper(T(&ar)[N])
		: object_wrapper<object>(null) {
		set(new collections::array_buffer<T>(ar));
	}

	inline object_wrapper<object>::object_wrapper(bool val)
		: object_wrapper<object>(null)
	{
		set(new variable<bool>(val));
	}

	inline object_wrapper<object>::object_wrapper(int val)
		: object_wrapper<object>(null)
	{
		set(new variable<int>(val));
	}

	inline object_wrapper<object>::object_wrapper(uint val)
		: object_wrapper<object>(null)
	{
		set(new variable<uint>(val));
	}

	inline object_wrapper<object>::object_wrapper(long val)
		: object_wrapper<object>(null)
	{
		set(new variable<long>(val));
	}

	inline object_wrapper<object>::object_wrapper(ulong val)
		: object_wrapper<object>(null)
	{
		set(new variable<ulong>(val));
	}

	inline object_wrapper<object>::object_wrapper(long64 val)
		: object_wrapper<object>(null)
	{
		set(new variable<long64>(val));
	}

	inline object_wrapper<object>::object_wrapper(ulong64 val)
		: object_wrapper<object>(null)
	{
		set(new variable<ulong64>(val));
	}

	inline object_wrapper<object>::object_wrapper(float val)
		: object_wrapper<object>(null)
	{
		set(new variable<float>(val));
	}

	inline object_wrapper<object>::object_wrapper(double val)
		: object_wrapper<object>(null)
	{
		set(new variable<double>(val));
	}

	template<typename T>//object convertible
	inline object_wrapper<object>::object_wrapper(object_wrapper<T> obj)
		: object_wrapper<object>(null) {
		set(obj.get());
	}

	template<typename T>//array convertible
	inline object_wrapper<object>::object_wrapper(initializer_list<T> args) 
		: object_wrapper<object>(null) {
		set(new collections::vector_buffer<T>(args));
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
		set(new strings::basic_string_buffer<text::encoding::ascii>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariant>::intf_wrapper(const wchar(&ar)[N])
		: intf_wrapper<ivariant>(null) {
		set(new strings::basic_string_buffer<text::encoding::unicode>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariant>::intf_wrapper(const mchar(&ar)[N])
		: intf_wrapper<ivariant>(null) {
		set(new strings::basic_string_buffer<text::encoding::utf8>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariant>::intf_wrapper(const char16(&ar)[N])
		: intf_wrapper<ivariant>(null) {
		set(new strings::basic_string_buffer<text::encoding::utf16>(ar));
	}

	template<wsize N>
	inline intf_wrapper<ivariant>::intf_wrapper(const char32(&ar)[N])
		: intf_wrapper<ivariant>(null) {
		set(new strings::basic_string_buffer<text::encoding::utf32>(ar));
	}

	template<typename T, text::encoding E>
	inline intf_wrapper<ivariant>::intf_wrapper(str_view<T, E> str)
		: intf_wrapper<ivariant>(null) {
		set(new strings::basic_string_buffer<E>(str));
	}

	template<typename T, wsize N>
	inline intf_wrapper<ivariant>::intf_wrapper(T(&ar)[N])
		: intf_wrapper<ivariant>(null) {
		set(new collections::array_buffer<T>(ar));
	}

	template<text::encoding E, template<typename>class A>
	inline intf_wrapper<ivariant>::intf_wrapper(strings::basic_string<E, A> const& str) 
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
		text::itext_buffer *buffer;
		if(buffer = interface_cast<text::itext_buffer>(get()))
		{
			buffer->mem_copy(str.size(), (pointer)str.cstr(), E);
		}
		else
		{
			strings::basic_string<E> ptr = str;
			set(ptr.get());
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
			set((ivariant*)arr.get())
		}
		return*this;
	}
}


#endif//__OBJECT_INL__