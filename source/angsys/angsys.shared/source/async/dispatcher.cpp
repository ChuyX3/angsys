#include "pch.h"
#include "ang/core/async.hpp"
#include "thread.hpp"

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

extern "C" ulong64 ang_get_performance_time();

ANG_IMPLEMENT_INTERFACE(ang::core::async, idispatcher);

