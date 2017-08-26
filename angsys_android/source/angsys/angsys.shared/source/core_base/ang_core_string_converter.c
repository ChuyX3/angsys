#include <angtypes.h>
#include "ang_core_string.h"

#define new(_TYPE) (_TYPE*)ang_alloc_unmanaged_memory(sizeof(_TYPE))
#define delete(_PTR) ang_free_unmanaged_memory(_PTR)


#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
#elif defined WINDOWS_PLATFORM
#endif
