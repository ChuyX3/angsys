#include "pch.h"

#include <ang/graphics/graphics.h>
#include "ang_inlines.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::buffers;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::buffers::igpu_buffer, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::buffers::iindex_buffer, igpu_buffer);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::buffers::ivertex_buffer, igpu_buffer);


safe_enum_rrti(ang::graphics::buffers, buffer_type_t, value<buffer_type_proxy>);
safe_enum_rrti(ang::graphics::buffers, buffer_usage_t, value<buffer_usage_proxy>);
safe_enum_rrti(ang::graphics::buffers, buffer_bind_flag_t, value<buffer_bind_flag_proxy>);


#define MY_TYPE ang::graphics::buffers::igpu_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::graphics::buffers::iindex_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::graphics::buffers::ivertex_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE
