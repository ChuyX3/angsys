/*********************************************************************************************************************/
/*   File Name: ang/collections/binary_tree.h                                                                             */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: Implements list object                                                                      */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BINARY_TREE_HPP__
#define __ANG_BINARY_TREE_HPP__

#include <angsys.hpp>


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
		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> class binary_tree_object;
		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> using binary_tree = object_wrapper<binary_tree_object<K, T, allocator>>;
	}

	namespace collections
	{
		template<typename K, typename T>
		struct binary_tree_node
		{
			typedef K key_t;
			typedef T value_t;
			typedef collections::pair<K, T> pair_t;
			typedef binary_tree_node<K, T> self_t, *self_ptr_t;

			binary_tree_node(K key) { _pair.key = ang::move(key); }
			binary_tree_node(K key, value_t val) {
				_pair.key = ang::move(key);
				_pair.value = ang::move(val);
			}
			binary_tree_node(pair_t p) : _pair(ang::move(p)) {}

			inline key_t& key() {
				return _pair.key;
			}
			inline key_t const& key()const {
				return _pair.key;
			}
			inline void key(key_t p) {
				_pair.key = ang::move(p);
			}

			inline value_t& value() {
				return _pair.value;
			}
			inline value_t const& value()const {
				return _pair.value;
			}
			inline void value(value_t val) {
				_pair.value = ang::move(val);
			}

			inline self_ptr_t& parent() {
				return _parent;
			}
			inline self_ptr_t const& parent()const {
				return _parent;
			}
			inline void parent(self_ptr_t n) {
				_parent = ang::move(n);
				if (_parent == null)_type = 0;
			}

			inline self_ptr_t left()const {
				return _left;
			}
			inline void left(self_ptr_t n) {
				_left = ang::move(n);
				if (_left) {
					_left->_type = 1;
					_left->parent(this);
				}
			}

			inline self_ptr_t right()const {
				return _right;
			}
			inline void right(self_ptr_t n) {
				_right = ang::move(n);
				if (_right) {
					_right->_type = 2;
					_right->parent(this);
				}
			}

			inline int type()const { return this ? _type : 0; }
			inline int height()const { return this ? _height : 0; }
			inline void height(int n) { _height = ang::move(n); }
			inline int balance()const { return this ? left()->height() - right()->height() : 0; }

			int _type = 0;
			int _height = 1;
			pair_t _pair;
			self_ptr_t _parent = null;
			self_ptr_t _left = null;
			self_ptr_t _right = null;
		};

		namespace algorithms
		{
			template<typename K, typename T>
			class binary_tree_inorder_iteration : public iteration_algorithm<binary_tree_node<K, T>>
			{
			public:
				binary_tree_inorder_iteration() {
					iteration_algorithm<binary_tree_node<K, T>>::begin = &begin;
					iteration_algorithm<binary_tree_node<K, T>>::end = &end;
					iteration_algorithm<binary_tree_node<K, T>>::rbegin = &begin;
					iteration_algorithm<binary_tree_node<K, T>>::rend = &end;
					iteration_algorithm<binary_tree_node<K, T>>::increase = &increase;
					iteration_algorithm<binary_tree_node<K, T>>::decrease = &decrease;
					iteration_algorithm<binary_tree_node<K, T>>::most_left = &most_left;
					iteration_algorithm<binary_tree_node<K, T>>::most_right = &most_right;
				}

				inline static binary_tree_node<K, T>* begin(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* end(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* rbegin(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* rend(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* increase(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* decrease(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* most_left(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* most_right(binary_tree_node<K, T>*, index&);
			};

			template<typename K, typename T>
			class binary_tree_preorder_iteration : public iteration_algorithm<binary_tree_node<K, T>>
			{
			public:
				binary_tree_preorder_iteration() {
					iteration_algorithm<binary_tree_node<K, T>>::begin = &begin;
					iteration_algorithm<binary_tree_node<K, T>>::end = &end;
					iteration_algorithm<binary_tree_node<K, T>>::rbegin = &begin;
					iteration_algorithm<binary_tree_node<K, T>>::rend = &end;
					iteration_algorithm<binary_tree_node<K, T>>::increase = &increase;
					iteration_algorithm<binary_tree_node<K, T>>::decrease = &decrease;
					iteration_algorithm<binary_tree_node<K, T>>::most_left = &most_left;
					iteration_algorithm<binary_tree_node<K, T>>::most_right = &most_right;
				}

				inline static binary_tree_node<K, T>* begin(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* end(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* rbegin(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* rend(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* increase(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* decrease(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* most_left(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* most_right(binary_tree_node<K, T>*, index&);
			};

			template<typename K, typename T>
			class binary_tree_postorder_iteration : public iteration_algorithm<binary_tree_node<K, T>>
			{
			public:
				binary_tree_postorder_iteration() {
					iteration_algorithm<binary_tree_node<K, T>>::begin = &begin;
					iteration_algorithm<binary_tree_node<K, T>>::end = &end;
					iteration_algorithm<binary_tree_node<K, T>>::rbegin = &begin;
					iteration_algorithm<binary_tree_node<K, T>>::rend = &end;
					iteration_algorithm<binary_tree_node<K, T>>::increase = &increase;
					iteration_algorithm<binary_tree_node<K, T>>::decrease = &decrease;
					iteration_algorithm<binary_tree_node<K, T>>::most_left = &most_left;
					iteration_algorithm<binary_tree_node<K, T>>::most_right = &most_right;
				}

				inline static binary_tree_node<K, T>* begin(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* end(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* rbegin(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* rend(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* increase(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* decrease(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* most_left(binary_tree_node<K, T>*, index&);
				inline static binary_tree_node<K, T>* most_right(binary_tree_node<K, T>*, index&);
			};
		}


		/******************************************************************/
		/* template class binary_tree_object :                             */
		/*  -> implements the buffer memory of binary_tree object        */
		/******************************************************************/
		template<typename K, typename T, template<typename> class allocator>
		class binary_tree_object final
			: public object
			, public imap<K, T>
		{
		public:
			typedef T										value_t;
			typedef K										key_t;
			typedef pair<K, T>								pair_t;
			typedef binary_tree_object<K, T, allocator>		self_t;
			typedef ienum<pair<K, T>>						ienum_t;
			typedef imap<K, T>								imap_t;
			typedef iterator<pair<K, T >>					iterator_t;
			typedef const_iterator<pair<K, T >>				const_iterator_t;
			typedef base_iterator<pair<K, T >>				base_iterator_t;
			typedef forward_iterator<pair<K, T >>			forward_iterator_t;
			typedef const_forward_iterator<pair<K, T >>		const_forward_iterator_t;
			typedef backward_iterator<pair<K, T >>			backward_iterator_t;
			typedef const_backward_iterator<pair<K, T >>	const_backward_iterator_t;


		protected:
			typedef binary_tree_node<K, T> node_t;
			typedef binary_tree_node<K, T> *node_ptr_t;
			wsize _count;
			node_ptr_t _root;
			algorithms::iteration_algorithm<node_t> algorithm;

		public:
			inline binary_tree_object();
			inline binary_tree_object(std::initializer_list<pair<K, T>> list);
			inline binary_tree_object(const std::nullptr_t&);
			inline binary_tree_object(binary_tree_object&& ar);
			inline binary_tree_object(const binary_tree_object& ar);
			inline binary_tree_object(const binary_tree_object* ar);
			inline binary_tree_object(const ienum_t* store);

		private:
			virtual ~binary_tree_object();

		public: //methods
			inline bool is_empty()const;
			inline void clean();
			inline bool move(binary_tree_object&);
			inline void iteration_method(algorithms::iteration_method_t);

		public: //ienum overrides
			inline wsize counter()const override;
			inline pair_t& at(const base_iterator_t& it) override;
			inline bool increase(base_iterator_t&)const override;
			inline bool increase(base_iterator_t&, int offset)const override;
			inline bool decrease(base_iterator_t&)const override;
			inline bool decrease(base_iterator_t&, int offset)const override;

			inline forward_iterator_t begin() override;
			inline forward_iterator_t end() override;
			inline const_forward_iterator_t begin()const override;
			inline const_forward_iterator_t end()const override;
			inline forward_iterator_t last() override;
			inline const_forward_iterator_t last()const override;

			inline backward_iterator_t rbegin() override;
			inline backward_iterator_t rend() override;
			inline const_backward_iterator_t rbegin()const override;
			inline const_backward_iterator_t rend()const override;

		public: //imap overrides
			inline bool copy(const ienum_t*) override;
			inline void extend(const ienum_t*) override;
			inline bool insert(K, T) override;
			inline bool insert(pair_t) override;
			inline bool update(K, T) override;
			inline bool update(pair<K, T>) override;
			inline bool remove(K const&) override;
			inline bool remove(K const&, T&) override;
			inline bool remove(base_iterator<pair<K, T>> it) override;
			inline bool remove(base_iterator<pair<K, T>> it, T&) override;
			inline bool has_key(const K&)const override;
			inline iterator<pair<K, T>> find(const K&) override;
			inline const_iterator<pair<K, T>> find(const K&)const override;

		public: //overrides
			inline static type_name_t class_name();
			inline static bool is_inherited_of(type_name_t);
			inline type_name_t object_name()const override;
			inline bool is_kind_of(type_name_t)const override;
			inline bool query_object(type_name_t, unknown_ptr_t) override;

		protected: //Memory Operations
			inline void destroy_tree(node_ptr_t)const;
			inline node_ptr_t find_node(key_t const&, node_ptr_t)const;
			inline node_ptr_t most_left(node_ptr_t)const;
			inline node_ptr_t most_right(node_ptr_t)const;
			inline node_ptr_t right_rotate(node_ptr_t);
			inline node_ptr_t left_rotate(node_ptr_t);
			inline node_ptr_t remove_node(node_ptr_t, key_t const&, bool& removed);
			inline node_ptr_t remove_node(node_ptr_t, key_t const&, T& out, bool& removed);
		};

	}

	/******************************************************************/
	/* template class ang::object_wrapper<binary_tree_object> :            */
	/*  -> specialization of object_wrapper<binary_tree_object> -> list  */
	/******************************************************************/

	template<typename K, typename T, template<typename> class allocator>
	class object_wrapper<collections::binary_tree_object<K, T, allocator>>
	{
	public:
		typedef collections::binary_tree_object<K, T, allocator> type;

	private:
		collections::binary_tree_object<K, T, allocator>* _ptr;

	public:
		object_wrapper();
		object_wrapper(std::nullptr_t const&);
		object_wrapper(collections::binary_tree_object<K, T, allocator>*);
		object_wrapper(collections::ienum<collections::pair<K, T>> const* store);
		object_wrapper(std::initializer_list<collections::pair<K, T>>);
		object_wrapper(object_wrapper<collections::binary_tree_object<K, T, allocator>> &&);
		object_wrapper(object_wrapper<collections::binary_tree_object<K, T, allocator>> const&);
		~object_wrapper();

	public:
		void clean();
		bool is_empty()const;
		collections::binary_tree_object<K, T, allocator>* get(void)const;
		void set(collections::binary_tree_object<K, T, allocator>*);
		collections::binary_tree_object<K, T, allocator> ** addres_of(void);

		collections::forward_iterator<object_wrapper<T>> begin() { return _ptr ? _ptr->begin() : collections::forward_iterator<T>(); }
		collections::forward_iterator<object_wrapper<T>> end() { return _ptr ? _ptr->end() : collections::forward_iterator<T>(); }
		collections::forward_iterator<const object_wrapper<T>> begin()const { return _ptr ? ((type const*)_ptr)->begin() : collections::forward_iterator<const object_wrapper<T>>(); }
		collections::forward_iterator<const object_wrapper<T>> end()const { return _ptr ? ((type const*)_ptr)->end() : collections::forward_iterator<const object_wrapper<T>>(); }

	public:
		object_wrapper& operator = (object_wrapper<collections::binary_tree_object<K, T, allocator>> &&);
		object_wrapper& operator = (object_wrapper<collections::binary_tree_object<K, T, allocator>> const&);
		object_wrapper& operator = (std::nullptr_t const&);
		object_wrapper& operator = (collections::binary_tree_object<K, T, allocator>*);
		object_wrapper& operator = (collections::ienum<collections::pair<K, T>> const* store);

		object_wrapper& operator += (collections::pair<K, T>);

		inline operator object_t()const;

		object_wrapper_ptr<collections::binary_tree_object<K, T, allocator>> operator & (void);
		collections::binary_tree_object<K, T, allocator> * operator -> (void);
		collections::binary_tree_object<K, T, allocator> const* operator -> (void)const;
		explicit operator collections::binary_tree_object<K, T, allocator> * (void);
		explicit operator collections::binary_tree_object<K, T, allocator> const* (void)const;

		T& operator [] (K const&);
		T operator [] (K const&)const;
	};

}

//#include<ang/collections/binary_tree_specialization.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#include<ang/collections/inline/binary_tree.inl>

#endif//__ANG_BINARY_TREE_H__
