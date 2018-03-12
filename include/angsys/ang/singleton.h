#ifndef __ANGSYS_H__
#elif !defined __SINGLETON_H__
#define __SINGLETON_H__

namespace ang
{
	template<typename T, template<typename> class allocator = memory::default_allocator>
	class singleton
	{
	public:
		typedef allocator<T> allocator_t;
	private:
		static T* instance_manager(T* ptr, bool write = false, bool del = false) {
			static T* _instance = null;
			
			if (write) {
				if (_instance && del) {
					allocator<T> alloc;
					alloc.template destroy<T>(_instance);
					alloc.deallocate(_instance);
				}
				_instance = ptr;
			}
			return _instance;
		}

	public:
		static T* instance()	{
			if (!is_instanced()) {
				allocator<T> alloc;
				auto ptr = alloc.allocate(1);
				alloc.construct<T>(ptr);
			}
				
			return instance_manager(null, false);
		}
		static bool release_instance() {
			instance_manager(null, true, true);
			return true;
		}
		static bool is_instanced() {
			return (instance_manager(null, false) != null);
		}

		singleton() {
			//if (is_instanced())
			//	throw exception_t(except_code::two_singleton);
			instance_manager(static_cast<T*>(this), true, false);
		}

		virtual~singleton() {
			if (instance_manager(null, false) == this)
				instance_manager(null, true, false);
		}
	};

	template<typename T>
	class singleton<object_wrapper<T>>
	{
	private:
		static object_wrapper<T> instance_manager(T* ptr, bool write = false) {
			static object_wrapper<T> _instance = null;
			if (write) {
				_instance = ptr;
			}
			return _instance;
		}

	public:
		static object_wrapper<T> instance() {
			if (!is_instanced()) {
				new T();
			}
			return instance_manager(null, false);
		}
		static bool release_instance() {
			instance_manager(null, true);
			return true;
		}
		static bool is_instanced() {
			return !instance_manager(null, false).is_empty();
		}

		singleton() {
			instance_manager(static_cast<T*>(this), true);
		}

		virtual~singleton() {
			instance_manager(null, true);
		}
	};
}

#endif//__SINGLETON_H__