#ifndef __ANG_BTREE_H__
#define __ANG_BTREE_H__

#include <angsys.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace collections
	{
		template<typename _pair, uint min_degree> struct btree_node;
		template<typename K, typename T, uint min_degree = 8> class btree_data;
		template<typename K, typename T, uint min_degree = 8> using btree = object_wrapper<btree_data<K,T, min_degree>>;
	}

	namespace collections
	{
		template<typename _pair, uint min_degree> struct btree_node
		{
			typedef _pair pair_t;
			typedef btree_node<_pair, min_degree> self_t;

			inline static type_name_t class_name();

			inline uint count()const { return _count; }
			inline void count(uint c) { _count = c; }
			inline void increase_count() { ++_count; }
			inline void decrease_count() { --_count; }
			inline bool is_leaf()const { return _is_leaf; }
			inline pair_t& pair(index idx) {
				if (idx >= (2 * min_degree - 1))
					throw(exception_t(except_code::array_overflow));
				return _pairs[idx];
			}
			inline pair_t const& pair(index idx)const {
				if (idx >= (2 * min_degree - 1))
					throw(exception_t(except_code::array_overflow));
				return _pairs[idx];
			}
			inline void pair(index idx, pair_t value) {
				if (idx >= (2 * min_degree - 1))
					throw(exception_t(except_code::array_overflow));
				_pairs[idx] = ang::move(value);
			}
			inline pair_t const& first_pair()const { return _pairs[0]; }
			inline pair_t const& last_pair()const { return _pairs[_count - 1]; }
			self_t* parent()const { return _parent; };
			void parent(self_t* node) {
				_parent = node;
				if (node == null)
					_current_index = -1;
			};
			index current_index()const { return _current_index; };
			inline self_t* first_child()const { return _children[0]; }
			inline self_t* last_child()const { return _children[_count]; }
			inline void first_child(self_t* node)const {
				_children[0] = node;
				if (node) {
					node->_current_index = 0;
					node->parent(this);
				}
			}
			inline void last_child(self_t* node)const {
				if (_count >= (2 * min_degree))
					throw(exception_t(except_code::array_overflow));
				_children[_count] = node;
				if (node) {
					node->_current_index = _count;
					node->parent(this);
				}
			}
			inline self_t* child(index idx) { return (idx <= _count) ? _children[idx] : null; }
			inline void child(index idx, self_t* node) {
				if (idx >= (2 * min_degree))
					throw(exception_t(except_code::array_overflow));
				if (idx < (2 * min_degree)) {
					_children[idx] = node;
					if (node) {
						node->_current_index = idx;
						node->parent(this);
					}
				}
			}

			pair_t _pairs[2 * min_degree - 1];
			self_t* _children[2 * min_degree] = { null };
			self_t* _parent = null;
			uint _count = 0;
			index _current_index = 0;
			bool _is_leaf = true;
		};


		template<class U, uint min_degree>
		class btree_inorder_algorithm
			: public algorithms::iteration_algorithm<btree_node<U, min_degree>>
		{
		public:
			static inline btree_node<U, min_degree>* begin(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* end(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* next(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* prev(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* most_left(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* most_right(btree_node<U, min_degree>* pNode, index& idx);

			btree_inorder_algorithm() {
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::begin = &begin;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::end = &end;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::next = &next;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::prev = &prev;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_left = &most_left;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_right = &most_right;
			}

		};

		template<class U, uint min_degree >
		class btree_postorder_algorithm
			: public algorithms::iteration_algorithm<btree_node<U, min_degree>>
		{
		public:
			static inline btree_node<U, min_degree>* begin(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* end(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* next(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* prev(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* most_left(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* most_right(btree_node<U, min_degree>* pNode, index& idx);
			btree_postorder_algorithm() {
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::begin = &begin;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::end = &end;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::next = &next;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::prev = &prev;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_left = &most_left;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_right = &most_right;
			}
		};

		template<class U, uint min_degree >
		class btree_preorder_algorithm
			: public algorithms::iteration_algorithm<btree_node<U, min_degree>>
		{
		public:
			static inline btree_node<U, min_degree>* begin(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* end(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* next(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* prev(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* most_left(btree_node<U, min_degree>* pNode, index& idx);
			static inline btree_node<U, min_degree>* most_right(btree_node<U, min_degree>* pNode, index& idx);
			btree_preorder_algorithm() {
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::begin = &begin;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::end = &end;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::next = &next;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::prev = &prev;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_left = &most_left;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_right = &most_right;
			}
		};


		template<typename K, typename T, uint _min_degree>
		class btree_data final
			: public object
			, public imap<K,T>
			, public memory::iallocator_client
		{
		public:
			static const uint min_degree = _min_degree;
			typedef K											key_t;
			typedef T											type;
			typedef pair<K, T>									pair_t;
			typedef btree_data<K,T, _min_degree>				self_t;
			typedef icollection<pair_t>							icollection_t;
			typedef ienum<pair_t>								ienum_t;
			typedef ilist<pair_t>								ilist_t;
			typedef iterator<pair_t>							iterator_t;
			typedef btree_inorder_algorithm<pair_t, min_degree> btree_inorder_algorithm_t;
			typedef btree_node<pair_t, min_degree> node_t;
			typedef btree_node<pair_t, min_degree>* node_ptr_t;

		private:
			uint _count;
			node_ptr_t _root;
			memory::iallocator* allocator;
			algorithms::iteration_algorithm<node_t> iteration_algorithm;

		public:
			inline btree_data();
			inline btree_data(std::initializer_list<pair_t> list);
			inline btree_data(const std::nullptr_t&);
			inline btree_data(const btree_data& ar);
			inline btree_data(const btree_data* ar);
			inline btree_data(const ienum_t& store);

		private:
			virtual ~btree_data();

		public: //methods
			inline bool is_empty()const;
			inline uint size()const;
			inline void clean();
			inline bool move(btree_data&);
			inline node_ptr_t root()const;

		private:
			void root(node_ptr_t);

		public: //iallocator_client overrides
			inline void set_allocator(memory::iallocator*)override;
			inline memory::iallocator* get_allocator()const override;

		public: //ienum overrides
			inline pair_t& at(const iterator_t& it)const override;
			inline iterator_t begin()const override;
			inline iterator_t end()const override;
			inline bool next(iterator_t& it)const override;
			inline bool next(iterator_t& it, int_t offset)const override;
			inline bool prev(iterator_t& it)const override;
			inline bool prev(iterator_t& it, int_t offset)const override;
			inline uint counter()const override;

		public: //icollection overrides
			inline bool has_items()const override;
			inline bool copy(const ienum_t*) override;
			inline index index_of(iterator_t it)const override;
			inline iterator_t find_index(index idx)const override;
			inline iterator_t find(const pair_t&, bool invert = false)const override;
			inline iterator_t find(const pair_t&, iterator_t nextTo, bool invert = false)const override;

		public:
			inline iterator_t find(const key_t&, bool invert = false)const;
			inline iterator_t find(const key_t&, iterator_t nextTo, bool invert = false)const;

			inline void extend(const ienum_t*);
			inline void append(pair_t);
			inline void append(key_t, type);
			inline void modify(pair_t);
			inline void modify(key_t, type);
			inline bool pop(pair_t& inout);
			inline bool pop(const key_t& in);
			inline bool pop(const key_t& in, type& out);
			inline bool pop_at(iterator_t it);
			inline bool pop_at(iterator_t it, type&);
			inline bool pop_at(iterator_t it, pair_t&);

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		private:
			index find_key_in_node(node_ptr_t node, const key_t& key)const;
			index find_key_in_node(node_ptr_t node, pair_t const& key)const;
			bool split_child_node(node_ptr_t node, index idx);
			bool remove_node(node_ptr_t node, const key_t& key, pair_t& out);
			bool lend_to_next(node_ptr_t node, index idx, uint count);
			bool borrow_from_next(node_ptr_t node, index idx, uint count);
			bool merge_node(node_ptr_t node, index idx);
			bool balance_node(node_ptr_t node, index idx);
			void remove_all_recursively(node_ptr_t node);
		};




	}//collections
}//ang

#include<ang/collections/inline/btree.hpp>

#endif//__ANG_BTREE_H__