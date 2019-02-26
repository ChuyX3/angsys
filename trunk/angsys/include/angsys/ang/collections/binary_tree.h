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

#ifndef __ANG_BINARY_TREE_H__
#define __ANG_BINARY_TREE_H__

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
		template<typename T> using binary_set_node = binary_node<T>;
		template<typename K, typename T> using binary_map_node = binary_node<pair<K, T>>;
		template<typename T, template<typename> class allocator = memory::default_allocator> class binary_set_object;
		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> class binary_map_object;
		template<typename T, template<typename> class allocator = memory::default_allocator> using binary_set = object_wrapper<binary_set_object<T, allocator>>;
		template<typename K, typename T, template<typename> class allocator = memory::default_allocator> using binary_map = object_wrapper<binary_map_object<K, T, allocator>>;
		
	}

	namespace algorithms
	{
		template<typename T>
		class binary_tree_inorder_iteration : public iteration_algorithm<collections::binary_node<T>>
		{
		public:
			binary_tree_inorder_iteration() {
				iteration_algorithm<collections::binary_node<T>>::begin = &begin;
				iteration_algorithm<collections::binary_node<T>>::end = &end;
				iteration_algorithm<collections::binary_node<T>>::rbegin = &begin;
				iteration_algorithm<collections::binary_node<T>>::rend = &end;
				iteration_algorithm<collections::binary_node<T>>::increase = &increase;
				iteration_algorithm<collections::binary_node<T>>::decrease = &decrease;
				//iteration_algorithm<collections::binary_node<T>>::most_left = &most_left;
				//iteration_algorithm<collections::binary_node<T>>::most_right = &most_right;
			}

			inline static collections::binary_node<T>* begin(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* end(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* rbegin(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* rend(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* increase(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* decrease(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* most_left(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* most_right(collections::binary_node<T>*, wsize&);
		};

		template<typename T>
		class binary_tree_preorder_iteration : public iteration_algorithm<collections::binary_node<T>>
		{
		public:
			binary_tree_preorder_iteration() {
				iteration_algorithm<collections::binary_node<T>>::begin = &begin;
				iteration_algorithm<collections::binary_node<T>>::end = &end;
				iteration_algorithm<collections::binary_node<T>>::rbegin = &begin;
				iteration_algorithm<collections::binary_node<T>>::rend = &end;
				iteration_algorithm<collections::binary_node<T>>::increase = &increase;
				iteration_algorithm<collections::binary_node<T>>::decrease = &decrease;
				//iteration_algorithm<collections::binary_node<T>>::most_left = &most_left;
				//iteration_algorithm<collections::binary_node<T>>::most_right = &most_right;
			}

			inline static collections::binary_node<T>* begin(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* end(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* rbegin(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* rend(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* increase(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* decrease(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* most_left(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* most_right(collections::binary_node<T>*, wsize&);
		};

		template<typename T>
		class binary_tree_postorder_iteration : public iteration_algorithm<collections::binary_node<T>>
		{
		public:
			binary_tree_postorder_iteration() {
				iteration_algorithm<collections::binary_node<T>>::begin = &begin;
				iteration_algorithm<collections::binary_node<T>>::end = &end;
				iteration_algorithm<collections::binary_node<T>>::rbegin = &begin;
				iteration_algorithm<collections::binary_node<T>>::rend = &end;
				iteration_algorithm<collections::binary_node<T>>::increase = &increase;
				iteration_algorithm<collections::binary_node<T>>::decrease = &decrease;
				//iteration_algorithm<collections::binary_node<T>>::most_left = &most_left;
				//iteration_algorithm<collections::binary_node<T>>::most_right = &most_right;
			}

			inline static collections::binary_node<T>* begin(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* end(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* rbegin(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* rend(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* increase(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* decrease(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* most_left(collections::binary_node<T>*, wsize&);
			inline static collections::binary_node<T>* most_right(collections::binary_node<T>*, wsize&);
		};
	}

	namespace collections
	{

		/******************************************************************/
		/* template class binary_map_object :                             */
		/*  -> implements the buffer memory of binary_map object        */
		/******************************************************************/
		template<typename T, template<typename> class allocator>
		class binary_set_object final
			: public object
			, public iset<T>
		{
		public:
			typedef T										value_t;
			typedef binary_set_object<T, allocator> 		self_t;
			typedef ienum<T>								ienum_t;
			typedef iset<T>  								imap_t;
			typedef iterator<T>								iterator_t;
			typedef const_iterator<T>						const_iterator_t;
			typedef base_iterator<T>						base_iterator_t;
			typedef forward_iterator<T>						forward_iterator_t;
			typedef const_forward_iterator<T>				const_forward_iterator_t;
			typedef backward_iterator<T>					backward_iterator_t;
			typedef const_backward_iterator<T>				const_backward_iterator_t;

		protected:
			typedef binary_set_node<T> node_t;
			typedef binary_set_node<T> *node_ptr_t;
			wsize _count;
			node_ptr_t _root;
			algorithms::iteration_algorithm<node_t> algorithm;

		public:
			inline binary_set_object();
			inline binary_set_object(ang::initializer_list<T> list);
			inline binary_set_object(const ang::nullptr_t&);
			inline binary_set_object(binary_set_object&& ar);
			inline binary_set_object(const binary_set_object& ar);
			inline binary_set_object(const binary_set_object* ar);
			inline binary_set_object(const ienum_t* store);

		private:
			virtual ~binary_set_object();

		public: //methods
			inline bool is_empty()const;
			inline void clean();
			inline bool move(binary_set_object&);
			inline void iteration_method(iteration_method_t);

		public: //ienum overrides
			inline wsize counter()const override;
			inline T& at(const base_iterator_t& it) override;
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
			inline bool insert(T) override;
			inline bool remove(T&) override;
			inline bool remove(base_iterator<T> it) override;
			inline bool remove(base_iterator<T> it, T&) override;
			inline bool has_value(const T&)const override;
			inline iterator<T> find(const T&) override;
			inline const_iterator<T> find(const T&)const override;

		public: //overrides
			ANG_DECLARE_INTERFACE();

		protected: //Memory Operations
			inline void destroy_tree(node_ptr_t)const;
			inline node_ptr_t find_node(T const&, node_ptr_t)const;
			inline node_ptr_t most_left(node_ptr_t)const;
			inline node_ptr_t most_right(node_ptr_t)const;
			inline node_ptr_t right_rotate(node_ptr_t);
			inline node_ptr_t left_rotate(node_ptr_t);
			inline node_ptr_t remove_node(node_ptr_t, T const&, bool& removed);
			inline node_ptr_t remove_node(node_ptr_t, T const&, T&, bool& removed);
		};


		/******************************************************************/
		/* template class binary_map_object :                             */
		/*  -> implements the buffer memory of binary_map object        */
		/******************************************************************/
		template<typename K, typename T, template<typename> class allocator>
		class binary_map_object final
			: public object
			, public imap<K, T>
		{
		public:
			typedef T										value_t;
			typedef K										key_t;
			typedef pair<K, T>								pair_t;
			typedef binary_map_object<K, T, allocator>		self_t;
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
			typedef binary_map_node<K, T> node_t;
			typedef binary_map_node<K, T> *node_ptr_t;
			wsize _count;
			node_ptr_t _root;
			algorithms::iteration_algorithm<node_t> algorithm;

		public:
			inline binary_map_object();
			inline binary_map_object(std::initializer_list<pair<K, T>> list);
			inline binary_map_object(const std::nullptr_t&);
			inline binary_map_object(binary_map_object&& ar);
			inline binary_map_object(const binary_map_object& ar);
			inline binary_map_object(const binary_map_object* ar);
			inline binary_map_object(const ienum_t* store);

		private:
			virtual ~binary_map_object();

		public: //methods
			inline bool is_empty()const;
			inline void clean();
			inline bool move(binary_map_object&);
			inline void iteration_method(iteration_method_t);

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
			inline bool insert(pair<K, T>) override;
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
			ANG_DECLARE_INTERFACE();

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
	/* template class ang::object_wrapper<binary_map_object> :            */
	/*  -> specialization of object_wrapper<binary_map_object> -> list  */
	/******************************************************************/

	template<typename K, typename T, template<typename> class allocator>
	class object_wrapper<collections::binary_map_object<K, T, allocator>>
	{
	public:
		typedef collections::binary_map_object<K, T, allocator> type;

	private:
		collections::binary_map_object<K, T, allocator>* m_ptr;

	public:
		object_wrapper();
		object_wrapper(std::nullptr_t const&);
		object_wrapper(collections::binary_map_object<K, T, allocator>*);
		object_wrapper(collections::ienum<collections::pair<K, T>> const* store);
		object_wrapper(std::initializer_list<collections::pair<K, T>>);
		object_wrapper(object_wrapper<collections::binary_map_object<K, T, allocator>> &&);
		object_wrapper(object_wrapper<collections::binary_map_object<K, T, allocator>> const&);
		~object_wrapper();

	public:
		void clean();
		bool is_empty()const;
		collections::binary_map_object<K, T, allocator>* get(void)const;
		void set(collections::binary_map_object<K, T, allocator>*);
		collections::binary_map_object<K, T, allocator> ** addres_of(void);

		collections::forward_iterator<object_wrapper<T>> begin() { return m_ptr ? m_ptr->begin() : collections::forward_iterator<T>(); }
		collections::forward_iterator<object_wrapper<T>> end() { return m_ptr ? m_ptr->end() : collections::forward_iterator<T>(); }
		collections::forward_iterator<const object_wrapper<T>> begin()const { return m_ptr ? ((type const*)m_ptr)->begin() : collections::forward_iterator<const object_wrapper<T>>(); }
		collections::forward_iterator<const object_wrapper<T>> end()const { return m_ptr ? ((type const*)m_ptr)->end() : collections::forward_iterator<const object_wrapper<T>>(); }

	public:
		object_wrapper& operator = (object_wrapper<collections::binary_map_object<K, T, allocator>> &&);
		object_wrapper& operator = (object_wrapper<collections::binary_map_object<K, T, allocator>> const&);
		object_wrapper& operator = (std::nullptr_t const&);
		object_wrapper& operator = (collections::binary_map_object<K, T, allocator>*);
		object_wrapper& operator = (collections::ienum<collections::pair<K, T>> const* store);

		object_wrapper& operator += (collections::pair<K, T>);

		inline operator object_t()const;

		object_wrapper_ptr<collections::binary_map_object<K, T, allocator>> operator & (void);
		collections::binary_map_object<K, T, allocator> * operator -> (void);
		collections::binary_map_object<K, T, allocator> const* operator -> (void)const;
		explicit operator collections::binary_map_object<K, T, allocator> * (void);
		explicit operator collections::binary_map_object<K, T, allocator> const* (void)const;

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
