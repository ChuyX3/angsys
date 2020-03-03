#include "pch.h"
#include "angsys.h"

using namespace ang;

#define MY_TYPE ang::ibuffer_view
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::ibuffer
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE



