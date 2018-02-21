#include "pch.h"
#include <ang/platform/platform.h>

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::display;
using namespace ang::platform::input;

ANG_IMPLEMENT_ENUM(ang::platform, activate_status, uint, activate_status::deactivated);
ANG_IMPLEMENT_ENUM(ang::platform, display_invalidate_reason, uint, display_invalidate_reason::none);
ANG_IMPLEMENT_ENUM(ang::platform::display, orientation, uint, orientation::none);

ANG_IMPLEMENT_FLAGS(ang::platform::input, key_modifiers, uint);
ANG_IMPLEMENT_ENUM(ang::platform::input, pointer_hardware_type, uint, pointer_hardware_type::touch);
ANG_IMPLEMENT_ENUM(ang::platform::input, keyboard_type, uint, keyboard_type::def);
ANG_IMPLEMENT_ENUM(ang::platform::input, virtual_key, uint, virtual_key::no_key);

ANG_IMPLEMENT_INTERFACE(ang::platform::input, ikeyboard);


ANG_IMPLEMENT_INTERFACE(ang::platform, imessage_reciever);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::icore_msg_dispatcher, imessage_reciever);
ANG_IMPLEMENT_INTERFACE(ang::platform, icore_context);
ANG_IMPLEMENT_INTERFACE(ang::platform, icore_view);
ANG_IMPLEMENT_INTERFACE(ang::platform, icore_app);




#define ANG_IMPLEMENT_OBJECT_VECTOR_SPECIALIZATION(_TYPE)	 \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper() \
	: _ptr(null) { } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper(ang::collections::vector_buffer<object_wrapper<_TYPE>>* ptr) \
	: object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>() { set(ptr); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper(ang::initializer_list_t<ang::object_wrapper<_TYPE>> list) \
	: object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>() { set(new collections::vector_buffer<ang::object_wrapper<_TYPE>>(ang::move(list))); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper(const ang::collections::ienum<ang::object_wrapper<_TYPE>>* store) \
	: object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>() { set(new collections::vector_buffer<ang::object_wrapper<_TYPE>>(store)); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>> && other) \
	: object_wrapper<collections::vector_buffer<ang::object_wrapper<_TYPE>>>() {  \
	collections::vector_buffer<ang::object_wrapper<_TYPE>> * temp = other._ptr; \
	other._ptr = null; \
	_ptr = temp; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>> const& other) \
	: object_wrapper<collections::vector_buffer<ang::object_wrapper<_TYPE>>>() { set(other.get()); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::~object_wrapper() { clean(); } \
void ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::clean() { \
	if (_ptr)_ptr->release(); \
	_ptr = null; \
} \
void ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::clean_unsafe() { _ptr = null; } \
bool ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::is_empty()const { return _ptr == null; } \
ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>* ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::get(void)const { return _ptr; } \
void ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::set(ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>* ptr) { \
	ang::collections::vector_buffer<ang::object_wrapper<_TYPE>> * temp = _ptr; \
	if (ptr == _ptr) return; \
	_ptr = ptr; \
	if (_ptr)_ptr->add_ref(); \
	if (temp)temp->release(); \
} \
ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>** ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::addres_of(void) { return &_ptr; } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator = (ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>* ptr) { \
	set(ptr); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator = (const ang::nullptr_t&) { \
	clean(); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator = (ang::collections::ienum<ang::object_wrapper<_TYPE>> const* items) { \
	if (_ptr == null) set(new collections::vector_buffer<ang::object_wrapper<_TYPE>>(items)); \
	else _ptr->copy(items); \
	return *this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>> && other) { \
	if (this == &other) return *this; \
	clean(); \
	_ptr = other._ptr; \
	other._ptr = null; \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>> const& other) { \
	set(other._ptr); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator += (ang::object_wrapper<_TYPE> value) { \
	if (is_empty()) set(new ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>()); \
	get()->append(ang::move(value)); \
	return*this; \
} \
ang::object_wrapper_ptr<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>> ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator & (void) { return this; } \
ang::collections::vector_buffer<ang::object_wrapper<_TYPE>> * ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator -> (void) { return get(); } \
ang::collections::vector_buffer<ang::object_wrapper<_TYPE>> const* ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator -> (void)const { return get(); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator ang::collections::vector_buffer<ang::object_wrapper<_TYPE>> * (void) { return get(); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator ang::collections::vector_buffer<ang::object_wrapper<_TYPE>> const* (void)const { return get(); } \
ang::object_wrapper<_TYPE> const& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator[](int idx)const { return _ptr->data()[idx]; } \
ang::object_wrapper<_TYPE> & ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator[](int idx) { return _ptr->data()[idx]; } 


ANG_IMPLEMENT_OBJECT_VECTOR_SPECIALIZATION(ang::platform::events::event)