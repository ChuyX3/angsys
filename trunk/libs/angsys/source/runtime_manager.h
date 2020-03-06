#pragma once
#include <angsys.h>

namespace ang
{
	namespace runtime
	{
		typedef struct rttiptr
		{
			rtti_t* ptr;
			rttiptr() : ptr(null) { }
			rttiptr(rtti_t* p) : ptr(p) { }
			rttiptr(rttiptr const& p) : ptr(p.ptr) { }
			~rttiptr() { ptr = null; }

			rttiptr& operator = (rtti_t* p) { ptr = p; return*this; }
			rttiptr& operator = (rttiptr const& p) { ptr = p.ptr; return*this; }

			rtti_t* operator ->() { return ptr; }
			rtti_t const* operator ->()const { return ptr; }

			operator rtti_t*() { return ptr; }
			operator rtti_t const*()const { return ptr; }

			bool operator == (rttiptr const& info)const {
				return info->gender() == ptr->gender() &&
					info->size() == ptr->size() &&
					info->aligment() == ptr->aligment() &&
					text::ascii::compare(info->type_name().cstr(), ptr->type_name().cstr()) == 0;
			}

			bool operator != (rttiptr const& info)const {
				return !operator==(info);
			}
		}rttiptr_t;
	}

	namespace algorithms
	{
		template<> struct hash<runtime::rttiptr_t> {
			static ulong64 make(runtime::rttiptr_t const& value) {
				ulong64 h = 75025;
				for (char c : value->type_name())
					h = (h << 5) + h + (byte)c + 1;
				return h;
			}
			ulong64 operator()(runtime::rttiptr_t const& value)const {
				return make(value);
			}
		};
	}

	namespace runtime
	{
		class runtime_type_manager {
		public:
			static runtime_type_manager* instance();
			template<typename T>using A = memory::unmanaged_allocator<T>;

			runtime_type_manager()
				: set()
				, str_set(){
				
			}
			~runtime_type_manager() {
				
			}
			void clear() {
				set.clear([&](rttiptr_t& info)
				{
					info.ptr->~__type_info();
					deallocate(info.ptr);
					info.ptr = null;
				});
				str_set.clear();
			}
		public:
			rtti_t* find_info(rtti_t* info)const {
				core::async::scope_locker<core::async::mutex> lock(m_mutex);
				if (auto it = set.find(info))
					return (*it);
				return null;
			}

			rtti_t* push(rtti_t* info) {
				core::async::scope_locker<core::async::mutex> lock(m_mutex);
				if (!set.insert(info))
					return null;
				return info;
			}

			type_name_t push_str(fast_astring_t&& str) {
				core::async::scope_locker<core::async::mutex> lock(m_mutex);
				collections::hash_set<fast_astring_t, memory::unmanaged_allocator>::iterator_t it;
				if (it = str_set.insert(ang::move(str)))
					return *it;
				return null;
			}

			rtti_t* allocate()const {
				return A<rtti_t>::allocate(1);
			}
			void deallocate(rtti_t* ptr)const {
				A<rtti_t>::deallocate(ptr);
			}

			core::async::mutex_t& main_mutex()const { return m_mutex; }

		private:
			mutable core::async::mutex_t m_mutex;
			collections::hash_set<rttiptr_t, memory::unmanaged_allocator> set;
			collections::hash_set<fast_astring_t, memory::unmanaged_allocator> str_set;
		};
	}


#if defined _DEBUG || defined _DEVELOPPER
	class object_manager {
	public:
		static object_manager* instance();

		object_manager() {
		}
		~object_manager() {
			map.clear([&](collections::pair<intptr_t, object*>& info)
			{
				__debugbreak();
			});
		}

	public:
		object* find_info(object* o)const {
			core::async::scope_locker<core::async::mutex> lock(m_mutex);
			if (auto it = map.find((intptr_t)o))
				return it->value;
			return null;
		}

		object* push(object* o) {
			core::async::scope_locker<core::async::mutex> lock(m_mutex);
			if (map.insert(reinterpret_cast<intptr_t>(o), o))
				return o;
			return null;
		}

		object* pop(object* o) {
			core::async::scope_locker<core::async::mutex> lock(m_mutex);
			object* out = null;
			if (map.remove(reinterpret_cast<intptr_t>(o), out))
				return out;
			return null;
		}

	private:
		mutable core::async::mutex_t m_mutex;
		collections::hash_map<intptr_t, object*, memory::unmanaged_allocator> map;
	};
#endif


}

