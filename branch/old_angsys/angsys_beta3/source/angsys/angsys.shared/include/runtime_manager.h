#pragma once
#include "core_hash_table.h"

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

			bool operator == (rttiptr const& info) {
				return info->genre() == ptr->genre() &&
					info->size() == ptr->size() &&
					info->aligment() == ptr->aligment() &&
					text::ascii::compare(info->type_name().cstr(), ptr->type_name().cstr()) == 0;
			}

			bool operator != (rttiptr const& info) {
				return !operator==(info);
			}
		}rttiptr_t;
	}

	namespace collections
	{
		template<> struct hash_code_maker<runtime::rttiptr_t> {
			static ulong64 make(runtime::rttiptr_t const& value, ulong64 const TS) {
				ulong64 h = 75025;
				for (char c : value->type_name())
					h = (h << 5) + h + (byte)c + 1;
				return ulong64(h % TS);
			}
		};
	}

	namespace runtime
	{
		class runtime_type_manager {
		public:
			static runtime_type_manager* instance();

			runtime_type_manager() {
				
			}
			~runtime_type_manager() {
				set.clear([&](rttiptr_t& info)
				{
					info.ptr->~__type_info();
					deallocate(info.ptr);
					info.ptr = null;
				});
			}

		public:
			rtti_t* find_info(rtti_t* info)const {
				rttiptr_t f;
				if (set.find(info, &f))
					return f;
				return null;
			}

			rtti_t* push(rtti_t* info) {
				if (!set.insert(info))
					return null;
				return info;
			}

			rtti_t* allocate()const {
				return allocator.allocate(1);
			}
			void deallocate(rtti_t* ptr)const {
				allocator.deallocate(ptr);
			}

		private:
			mutable memory::unmanaged_allocator<rtti_t> allocator;
			collections::internal_hash_set<rttiptr_t> set;
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
			object* out = null;
			if (map.find((intptr_t)o, &out))
				return out;
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
			if (map.remove(reinterpret_cast<intptr_t>(o), &out))
				return out;
			return null;
		}

	private:
		mutable core::async::mutex_t m_mutex;
		collections::internal_hash_map<intptr_t, object*> map;
	};
#endif
}

