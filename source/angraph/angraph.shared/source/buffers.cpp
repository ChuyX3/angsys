#include "pch.h"

#include <ang/graphics/angraph.hpp>
#include "ang_inlines.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::buffers;

ANG_IMPLEMENT_INTERFACE(ang::graphics::buffers, igpu_buffer)
ANG_IMPLEMENT_INTERFACE(ang::graphics::buffers, iindex_buffer)
ANG_IMPLEMENT_INTERFACE(ang::graphics::buffers, ivertex_buffer)

ANG_IMPLEMENT_ENUM(ang::graphics::buffers, buffer_type, uint, buffers::buffer_type::none);
ANG_IMPLEMENT_ENUM(ang::graphics::buffers, buffer_usage, uint, buffers::buffer_usage::def);
ANG_IMPLEMENT_FLAGS(ang::graphics::buffers, buffer_bind_flag, uint);


#define MY_TYPE ang::graphics::buffers::igpu_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::graphics::buffers::iindex_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::graphics::buffers::ivertex_buffer
#include <ang/inline/intf_wrapper_specialization.inl>
#undef MY_TYPE
