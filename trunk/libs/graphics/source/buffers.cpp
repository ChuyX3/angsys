#include "pch.h"

#include <ang/graphics/graphics.h>

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::buffers;

//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::buffers::igpu_buffer, lattelib);
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::buffers::iindex_buffer, lattelib);
//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::buffers::ivertex_buffer, lattelib);


ANG_ENUM_IMPLEMENT(ang::graphics::buffers, buffer_type);
ANG_ENUM_IMPLEMENT(ang::graphics::buffers, buffer_usage);
ANG_FLAGS_IMPLEMENT(ang::graphics::buffers, buffer_bind_flag);


#define MY_TYPE ang::graphics::buffers::igpu_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::graphics::buffers::iindex_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::graphics::buffers::ivertex_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE
