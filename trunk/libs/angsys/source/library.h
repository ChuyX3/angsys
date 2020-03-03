#include "angsys.h"
#include "memory_manager.h"
#include "runtime_manager.h"
//#include "thread_manager.h"
//#include "file_system.h"
//#include "ang/core/time.h"

using namespace ang;

namespace ang
{
	class ang_library_initializer
	{
	public:
		core::async::mutex_t main_mutex;
		runtime::runtime_type_manager runtime_type_manager;
		//core::async::thread_manager* thread_manager;
		//core::files::file_system_t file_system;

#ifdef _MEMORY_PROFILING
		stack_array<memory::iraw_allocator*, 4> m_alloc;
#endif//_MEMORY_PROFILING

#if defined _DEBUG || defined _DEVELOPPER
		object_manager* objects;
#endif //DEBUG

		ang_library_initializer();

		~ang_library_initializer();

		static ang_library_initializer * instance();
	};
}
