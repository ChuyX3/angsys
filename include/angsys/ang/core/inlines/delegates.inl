#ifndef __ANG_DELEGATES_HPP__
#error Can't include delegate.inl, please include delegates.hpp inside
#elif !defined __ANG_DELEGATES_INL__
#define __ANG_DELEGATES_INL__


template<typename return_t, typename... args_t>
inline ang::type_name_t ang::core::delegates::ifunction<return_t(args_t...)>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move("ang::core::delegates::ifunction<"_o + type_of<return_t(args_t...)>() + ")>"_s));
	return name;
}

template<typename... args_t>
inline ang::type_name_t ang::core::delegates::ifunction<void(args_t...)>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move("ang::core::delegates::ifunction<"_o + type_of<void(args_t...)>() + ")>"_s));
	return name;
}

template<typename return_t, typename... args_t>
inline ang::type_name_t ang::core::delegates::ifunction<return_t(args_t...)>::object_name()const
{
	return class_name();
}

template<typename... args_t>
inline ang::type_name_t ang::core::delegates::ifunction<void(args_t...)>::object_name()const
{
	return class_name();
}

template<typename return_t, typename... args_t>
inline bool ang::core::delegates::ifunction<return_t(args_t...)>::is_kind_of(ang::type_name_t name)const
{
	return name == class_name();
}

template<typename... args_t>
inline bool ang::core::delegates::ifunction<void(args_t...)>::is_kind_of(ang::type_name_t name)const
{
	return name == class_name();
}

template<typename return_t, typename... args_t>
inline bool ang::core::delegates::ifunction<return_t(args_t...)>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (name == class_name())
	{
		*out = this;
		return true;
	}
	return false;
}

template<typename... args_t>
inline bool ang::core::delegates::ifunction<void(args_t...)>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (name == class_name())
	{
		*out = this;
		return true;
	}
	return false;
}

template<typename return_t, typename... args_t>
inline ang::type_name_t ang::core::delegates::function_data<return_t(args_t...)>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::core::delegates::function<"_o += type_of<return_t(args_t...)>()) += ")>"_s));
	return name;
}

template<typename... args_t>
inline ang::type_name_t ang::core::delegates::function_data<void(args_t...)>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::core::delegates::function<"_o += type_of<void(args_t...)>()) += ")>"_s));
	return name;
}

template<typename return_t, typename... args_t>
inline ang::type_name_t ang::core::delegates::function_data<return_t(args_t...)>::object_name()const
{
	return class_name();
}

template<typename... args_t>
inline ang::type_name_t ang::core::delegates::function_data<void(args_t...)>::object_name()const
{
	return class_name();
}

template<typename return_t, typename... args_t>
inline bool ang::core::delegates::function_data<return_t(args_t...)>::is_kind_of(ang::type_name_t name)const
{
	return (name == class_name()
		|| ifunction<return_t(args_t...)>::is_kind_of(name));
}


template<typename... args_t>
inline bool ang::core::delegates::function_data<void(args_t...)>::is_kind_of(ang::type_name_t name)const
{
	return (name == class_name()
		|| ifunction<void(args_t...)>::is_kind_of(name));
}


template<typename return_t, typename... args_t>
inline bool ang::core::delegates::function_data<return_t(args_t...)>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (name == class_name())
	{
		*out = this;
		return true;
	}
	else if (ifunction<return_t(args_t...)>::query_object(name, out))
		return true;
	
	return false;
}

template<typename... args_t>
inline bool ang::core::delegates::function_data<void(args_t...)>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (name == class_name())
	{
		*out = this;
		return true;
	}
	else if (ifunction<void(args_t...)>::query_object(name, out))
		return true;

	return false;
}


template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::object_wrapper()
	: _ptr(null)
{

}

template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	core::delegates::function_data<return_t(args_t...)> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper()
{

}

template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::~object_wrapper()
{
	clean();
}

template<typename return_t, typename... args_t>
void ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename return_t, typename... args_t>
inline void ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::clean_unsafe()
{
	_ptr = null;
}

template<typename return_t, typename... args_t>
inline bool ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::is_empty()const
{
	return _ptr == null;
}

template<typename return_t, typename... args_t>
inline ang::core::delegates::function_data<return_t(args_t...)>* ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::get(void)const
{
	return _ptr;
}

template<typename return_t, typename... args_t>
inline void ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::set(ang::core::delegates::function_data<return_t(args_t...)>* ptr)
{
	core::delegates::function_data<return_t(args_t...)> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename return_t, typename... args_t>
ang::core::delegates::function_data<return_t(args_t...)>** ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::addres_of(void)
{
	return &_ptr;
}


template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>& ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>& ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename return_t, typename... args_t>
ang::object_wrapper_ptr<ang::core::delegates::function_data<return_t(args_t...)>> ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::operator & (void)
{
	return this;
}


///////////////////////////////////////////////////////////////////////////


template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::object_wrapper()
	: _ptr(null)
{

}

template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	core::delegates::function_data<void(args_t...)> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper()
{

}

template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::~object_wrapper()
{
	clean();
}

template<typename... args_t>
void ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename... args_t>
inline void ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::clean_unsafe()
{
	_ptr = null;
}

template<typename... args_t>
inline bool ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::is_empty()const
{
	return _ptr == null;
}

template<typename... args_t>
inline ang::core::delegates::function_data<void(args_t...)>* ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::get(void)const
{
	return _ptr;
}

template<typename... args_t>
inline void ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::set(ang::core::delegates::function_data<void(args_t...)>* ptr)
{
	core::delegates::function_data<void(args_t...)> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename... args_t>
ang::core::delegates::function_data<void(args_t...)>** ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::addres_of(void)
{
	return &_ptr;
}


template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>& ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>& ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename... args_t>
ang::object_wrapper_ptr<ang::core::delegates::function_data<void(args_t...)>> ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::operator & (void)
{
	return this;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>::listener()
{
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>::listener(listener&& other)
	: functions(ang::move(other.functions))
{	
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>::listener(listener const& other)
	: functions(other.functions)
{
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>::~listener()
{
	empty();
}

template<typename return_t, typename... args_t>
void ang::core::delegates::listener<return_t(args_t...)>::empty()
{
	functions.clean();
}

template<typename return_t, typename... args_t>
bool ang::core::delegates::listener<return_t(args_t...)>::is_empty()const
{
	return functions.is_empty() || functions->is_empty();
}

template<typename return_t, typename... args_t>
index ang::core::delegates::listener<return_t(args_t...)>::append(ang::core::delegates::function<return_t(args_t...)> func)
{
	functions += ang::move(func);
	return functions.get()->size() - 1;
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>& ang::core::delegates::listener<return_t(args_t...)>::operator = (listener&& other)
{
	functions = ang::move(other.functions);
	return *this;
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>& ang::core::delegates::listener<return_t(args_t...)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>& ang::core::delegates::listener<return_t(args_t...)>::operator += (ang::core::delegates::function<return_t(args_t...)> func)
{
	append(ang::move(func));
	return *this;
}

template<typename return_t, typename... args_t>
ang::collections::ienum_t<return_t> ang::core::delegates::listener<return_t(args_t...)>::operator() (args_t... args)const
{
	collections::vector<return_t> result;
	if (!is_empty())
	{
		for (function<return_t(args_t...)>func : functions)	
			result += func(args...);
	}
	return result.get();
}

template<typename return_t, typename... args_t>
ang::collections::ienum_t<return_t> ang::core::delegates::listener<return_t(args_t...)>::invoke(args_t... args)const
{
	collections::vector<return_t> result;
	if (!is_empty())
	{
		for (function<return_t(args_t...)>func : functions)
			result += func(args...);
	}
	return result.get();
}





template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>::listener()
{
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>::listener(listener&& other)
	: functions(ang::move(other.functions))
{
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>::listener(listener const& other)
	: functions(other.functions)
{
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>::~listener()
{
	empty();
}

template<typename... args_t>
void ang::core::delegates::listener<void(args_t...)>::empty()
{
	functions.clean();
}

template<typename... args_t>
bool ang::core::delegates::listener<void(args_t...)>::is_empty()const
{
	return functions.is_empty() || functions->is_empty();
}

template<typename... args_t>
index ang::core::delegates::listener<void(args_t...)>::append(ang::core::delegates::function<void(args_t...)> func)
{
	functions += ang::move(func);
	return functions.get()->size() - 1;
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>& ang::core::delegates::listener<void(args_t...)>::operator = (listener&& other)
{
	functions = ang::move(other.functions);
	return *this;
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>& ang::core::delegates::listener<void(args_t...)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>& ang::core::delegates::listener<void(args_t...)>::operator += (ang::core::delegates::function<void(args_t...)> func)
{
	append(ang::move(func));
	return *this;
}


template<typename... args_t>
void ang::core::delegates::listener<void(args_t...)>::invoke(args_t... args)const
{
	if (!is_empty())
	{
		for(function<void(args_t...)> const & func : functions) {
			func(args...);
		}
	}
}

template<typename... args_t>
void ang::core::delegates::listener<void(args_t...)>::operator() (args_t... args)const
{
	if (!is_empty())
	{
		for(function<void(args_t...)> const & func : functions) {
			func(args...);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////


template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::intf_wrapper() : _ptr(null) {

}

template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {

}

template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::intf_wrapper(ang::core::delegates::ifunction<return_t(args_t...)>* ptr) : _ptr(null) {
	set(ptr);
}
template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ang::core::delegates::ifunction<return_t(args_t...)> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}
template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::~intf_wrapper() {
	clean();
}

template<typename return_t, typename... args_t>
inline void ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

template<typename return_t, typename... args_t>
inline bool ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::is_empty()const
{
	return _ptr == null;
}

template<typename return_t, typename... args_t>
inline ang::core::delegates::ifunction<return_t(args_t...)>* ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::get(void)const
{
	return _ptr;
}

template<typename return_t, typename... args_t>
inline void ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::set(ang::core::delegates::ifunction<return_t(args_t...)>* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>& ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::operator = (ang::core::delegates::ifunction<return_t(args_t...)>* ptr)
{
	set(ptr);
	return*this;
}

template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>& ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>& ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::operator = (ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>& ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::operator = (ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename return_t, typename... args_t>
inline ang::core::delegates::ifunction<return_t(args_t...)> ** ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::addres_of(void)
{
	return &_ptr;
}

template<typename return_t, typename... args_t>
inline ang::intf_wrapper_ptr<ang::core::delegates::ifunction<return_t(args_t...)>> ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::operator & (void)
{
	return this;
}

template<typename return_t, typename... args_t>
inline ang::core::delegates::ifunction<return_t(args_t...)> * ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::operator -> (void)
{
	return get();
}

template<typename return_t, typename... args_t>
inline ang::core::delegates::ifunction<return_t(args_t...)> const* ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::operator -> (void)const
{
	return get();
}

template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::operator ang::core::delegates::ifunction<return_t(args_t...)> * (void)
{
	return get();
}

template<typename return_t, typename... args_t>
inline ang::intf_wrapper<ang::core::delegates::ifunction<return_t(args_t...)>>::operator ang::core::delegates::ifunction<return_t(args_t...)> const* (void)const
{
	return get();
}

#endif//__ANG_DELEGATES_INL__
