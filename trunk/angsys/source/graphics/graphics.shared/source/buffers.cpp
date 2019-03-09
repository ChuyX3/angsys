#include "pch.h"

#include <ang/graphics/graphics.h>

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::buffers;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::buffers::igpu_buffer, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::buffers::iindex_buffer, igpu_buffer);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::buffers::ivertex_buffer, igpu_buffer);


safe_enum_rrti2(ang::graphics::buffers, buffer_type);
safe_enum_rrti2(ang::graphics::buffers, buffer_usage);
safe_enum_rrti2(ang::graphics::buffers, buffer_bind_flag);


#define MY_TYPE ang::graphics::buffers::igpu_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::graphics::buffers::iindex_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::graphics::buffers::ivertex_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE
