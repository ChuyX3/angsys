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
	namespace algorithms
	{
		template<class _node>
		class iteration_algorithm
		{
		public:
			typedef _node* node_ptr_t;
			typedef node_ptr_t(*iteration_callback_t)(node_ptr_t, index_t&);

		public: //overrides
			iteration_callback_t begin;
			iteration_callback_t end;
			iteration_callback_t next;
			iteration_callback_t prev;
			iteration_callback_t most_left;
			iteration_callback_t most_right;
		};
	}

	namespace collections
	{
		template<typename _pair, uint_t min_degree> struct btree_node;
		template<typename K, typename T, uint_t min_degree = 8> class btree_data;
		template<typename K, typename T, uint_t min_degree = 8> using btree = object_wrapper<btree_data<K,T, min_degree>>;
	}

	namespace collections
	{
		template<typename _pair, uint_t min_degree> struct btree_node
		{
			typedef _pair pair_t;
			typedef btree_node<_pair, min_degree> self_t;

			inline static type_name_t class_name();

			inline uint_t count()const { return _count; }
			inline void count(uint_t c) { _count = c; }
			inline void increase_count() { ++_count; }
			inline void decrease_count() { --_count; }
			inline bool_t is_leaf()const { return _is_leaf; }
			inline pair_t& pair(index_t idx) {
				if (idx >= (2 * min_degree - 1))
					throw(exception_t(except_code::array_overflow));
				return _pairs[idx];
			}
			inline pair_t const& pair(index_t idx)const {
				if (idx >= (2 * min_degree - 1))
					throw(exception_t(except_code::array_overflow));
				return _pairs[idx];
			}
			inline void pair(index_t idx, pair_t value) {
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
			index_t current_index()const { return _current_index; };
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
			inline self_t* child(index_t idx) { return (idx <= _count) ? _children[idx] : null; }
			inline void child(index_t idx, self_t* node) {
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
			uint_t _count = 0;
			index_t _current_index = 0;
			bool_t _is_leaf = true;
		};


		template<class U, uint_t min_degree>
		class btree_inorder_algorithm
			: public algorithms::iteration_algorithm<btree_node<U, min_degree>>
		{
		public:
			static inline btree_node<U, min_degree>* begin(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* end(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* next(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* prev(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* most_left(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* most_right(btree_node<U, min_degree>* pNode, index_t& idx);

			btree_inorder_algorithm() {
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::begin = &begin;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::end = &end;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::next = &next;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::prev = &prev;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_left = &most_left;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_right = &most_right;
			}

		};

		template<class U, uint_t min_degree >
		class btree_postorder_algorithm
			: public algorithms::iteration_algorithm<btree_node<U, min_degree>>
		{
		public:
			static inline btree_node<U, min_degree>* begin(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* end(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* next(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* prev(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* most_left(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* most_right(btree_node<U, min_degree>* pNode, index_t& idx);
			btree_postorder_algorithm() {
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::begin = &begin;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::end = &end;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::next = &next;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::prev = &prev;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_left = &most_left;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_right = &most_right;
			}
		};

		template<class U, uint_t min_degree >
		class btree_preorder_algorithm
			: public algorithms::iteration_algorithm<btree_node<U, min_degree>>
		{
		public:
			static inline btree_node<U, min_degree>* begin(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* end(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* next(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* prev(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* most_left(btree_node<U, min_degree>* pNode, index_t& idx);
			static inline btree_node<U, min_degree>* most_right(btree_node<U, min_degree>* pNode, index_t& idx);
			btree_preorder_algorithm() {
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::begin = &begin;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::end = &end;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::next = &next;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::prev = &prev;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_left = &most_left;
				algorithms::iteration_algorithm<btree_node<U, min_degree>>::most_right = &most_right;
			}
		};


		template<typename K, typename T, uint_t _min_degree>
		class btree_data final
			: public object
			, public imap<K,T>
			, public memory::iallocator_client
		{
		public:
			static const uint_t min_degree = _min_degree;
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
			uint_t _count;
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
			inline bool_t is_empty()const;
			inline uint_t size()const;
			inline void clean();
			inline bool_t move(btree_data&);
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
			inline bool_t next(iterator_t& it)const override;
			inline bool_t next(iterator_t& it, int_t offset)const override;
			inline bool_t prev(iterator_t& it)const override;
			inline bool_t prev(iterator_t& it, int_t offset)const override;
			inline uint_t counter()const override;

		public: //icollection overrides
			inline bool_t has_items()const override;
			inline bool_t copy(const ienum_t*) override;
			inline index_t index_of(iterator_t it)const override;
			inline iterator_t find_index(index_t idx)const override;
			inline iterator_t find(const pair_t&, bool_t invert = false)const override;
			inline iterator_t find(const pair_t&, iterator_t nextTo, bool_t invert = false)const override;

		public:
			inline iterator_t find(const key_t&, bool_t invert = false)const;
			inline iterator_t find(const key_t&, iterator_t nextTo, bool_t invert = false)const;

			inline void extend(const ienum_t*);
			inline void append(pair_t);
			inline void append(key_t, type);
			inline void modify(pair_t);
			inline void modify(key_t, type);
			inline bool_t pop(pair_t& inout);
			inline bool_t pop(const key_t& in);
			inline bool_t pop(const key_t& in, type& out);
			inline bool_t pop_at(iterator_t it);
			inline bool_t pop_at(iterator_t it, type&);
			inline bool_t pop_at(iterator_t it, pair_t&);

		public: //overrides
			inline static type_name_t class_name();
			inline type_name_t object_name()const override;
			inline bool_t is_kind_of(type_name_t)const override;
			inline bool_t query_object(type_name_t, unknown_ptr_t) override;

		private:
			index_t find_key_in_node(node_ptr_t node, const key_t& key)const;
			index_t find_key_in_node(node_ptr_t node, pair_t const& key)const;
			bool_t split_child_node(node_ptr_t node, index_t index);
			bool_t remove_node(node_ptr_t node, const key_t& key, pair_t& out);
			bool_t lend_to_next(node_ptr_t node, index_t index, uint_t count);
			bool_t borrow_from_next(node_ptr_t node, index_t index, uint_t count);
			bool_t merge_node(node_ptr_t node, index_t index);
			bool_t balance_node(node_ptr_t node, index_t index);
			void remove_all_recursively(node_ptr_t node);
		};




	}//collections
}//ang

#include<ang/collections/inline/btree.hpp>

#endif//__ANG_BTREE_H__