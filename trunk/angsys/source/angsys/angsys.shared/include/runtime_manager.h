#pragma once
namespace ang
{
	namespace runtime
	{
		class runtime_type_manager {
		public:
			static runtime_type_manager* instance();

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

