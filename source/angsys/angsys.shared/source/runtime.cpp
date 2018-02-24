#include "pch.h"
#include "ang/base/base.h"

#include <list>

using namespace ang;

ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(char, "ang::char_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(byte, "ang::byte_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(wchar, "ang::wchar_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(short, "ang::short_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ushort, "ang::ushort_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(int, "ang::int_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(uint, "ang::uint_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(long, "ang::long_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ulong, "ang::ulong_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(long64, "ang::long64_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ulong64, "ang::ulong64_t");


namespace ang
{
	namespace runtime
	{
		class runtime_type_manager {
			runtime_type_manager() { 
				head = null;
				tail = null;
			}
			~runtime_type_manager() { 
				rtti_node* node = head, *temp;
				while (node) {
					temp = node;
					node = node->next;
					temp->info->~__type_info();
					allocator.deallocate(temp->info);
					free(temp);
				}
			}

		public:
			static runtime_type_manager* instance() {
				static runtime_type_manager s_instance;
				return &s_instance;
			}
	
			rtti_t* find_info(rtti_t* info)const {
				rtti_node* node = head;
				while (node) {
					if (info->genre() == node->info->genre() &&
						info->size() == node->info->size() &&
						info->aligment() == node->info->aligment() &&
						text::ascii::compare(info->type_name().cstr(), node->info->type_name().cstr()) == 0)
						return node->info;

					node = node->next;
				}
				return null;
			}

			rtti_t* push(rtti_t* info) {
				if (tail) {
					tail->next = (rtti_node*)malloc(sizeof(rtti_node));
					tail->next->info = info;
					tail->next->next = null;
					tail = tail->next;
				}
				else {
					tail = head = (rtti_node*)malloc(sizeof(rtti_node));
					tail->info = info;
					tail->next = null;
				}
				return tail->info;
			}

			rtti_t* allocate()const { 
				return allocator.allocate(1);
			}
			void deallocate(rtti_t* ptr)const {
				allocator.deallocate(ptr);
			}

		private:
			mutable memory::unmanaged_allocator<rtti_t> allocator;
			struct rtti_node {
				rtti_t* info;
				rtti_node* next;
			};
			rtti_node* head;
			rtti_node* tail;
		};
	}
}


inline auto find_rtti(rtti_t* info, std::list<rtti_t*>& list)-> decltype(list.begin()) {
	for (auto it = list.begin(); it != list.end(); ++it) {
		if (info->genre() == (*it)->genre() &&
			info->size() == (*it)->size() &&
			info->aligment() == (*it)->aligment() &&
			text::ascii::compare(info->type_name().cstr(), (*it)->type_name().cstr()) == 0)
			return it;
	}
	return list.end();
}

bool rtti::default_dyn_cast(const rtti_t& src_id, unknown_t src, const rtti_t& out_id, unknown_ptr_t out)
{
	if (out && src_id.type_id() == out_id.type_id()) {
		*out = src;
		return true;
	}
	else {
		for (const rtti_t* info : src_id._parents) {
			if (info->dyn_cast(src, out_id, out))
				return true;
		}
	}
	return false;
}

rtti_t const& rtti::regist(type_name_t name, genre_t g, wsize sz, wsize a)
{
	runtime_type_manager* instance = runtime_type_manager::instance();

	rtti_t* info = instance->allocate();
	info = new(info)rtti(name, g, sz, a);
	auto it = instance->find_info(info);
	if (it != null) {
		info->~__type_info();
		instance->deallocate(info);
		info = it;
	}
	else {
		instance->push(info);
	}
	return*info;
}

rtti_t const& rtti::regist(type_name_t name, genre_t g, wsize sz, wsize a, array_view<rtti_t const*> parents, dynamic_cast_proc dyn)
{
	runtime_type_manager* instance = runtime_type_manager::instance();

	rtti_t* info = instance->allocate();
	info = new(info)rtti(name, g, sz, a, parents, dyn);
	auto it = instance->find_info(info);
	if (it != null) {
		info->~__type_info();
		instance->deallocate(info);
		info = it;
	}
	else {
		instance->push(info);
	}
	return*info;
}

