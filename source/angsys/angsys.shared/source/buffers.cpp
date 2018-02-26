#include "pch.h"
#include "ang/system.h"

using namespace ang;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::ibuffer_view, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::ibuffer, ibuffer_view);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::text::itext_buffer, ibuffer);

#define MY_TYPE ang::ibuffer_view
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::ibuffer
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::text::itext_buffer
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE