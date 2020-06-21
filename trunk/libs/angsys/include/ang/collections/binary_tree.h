/*********************************************************************************************************************/
/*   File Name: coffe/collections/binary_tree.h                                                                             */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: Implements list object                                                                      */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Copyright (C) coffe sys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __COFFE_BINARY_TREE_H__
#define __COFFE_BINARY_TREE_H__

#include <coffe.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined COFFE_DYNAMIC_LIBRARY

#ifdef COFFE_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//COFFE_EXPORTS
#else//#elif defined COFFE_STATIC_LIBRARY
#define LINK
#endif//COFFE_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace coffe
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
		class binary_tree_inorder_iteratior : public iteration_algorithm<collections::binary_node<T>>
		{
		public:
			binary_tree_inorder_iteratior() {
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
		class binary_tree_preorder_iteratior : public iteration_algorithm<collections::binary_node<T>>
		{
		public:
			binary_tree_preorder_iteratior() {
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
		class binary_tree_postorder_iteratior : public iteration_algorithm<collections::binary_node<T>>
		{
		public:
			binary_tree_postorder_iteratior() {
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
			: public smart<binary_set_object<T, allocator>, iset<T>>
		{
		public:
			typedef T										value_type;
			typedef binary_set_object<T, allocator> 		self_type;
			typedef ienum<T>								ienum_type;
			typedef iset<T>  								iset_type;
			typedef iterator<T>								iterator_type;
			typedef const_iterator<T>						const_iterator_type;
			typedef base_iterator<T>						base_iterator_type;
			typedef forward_iterator<T>						forward_iterator_type;
			typedef const_forward_iterator<T>				const_forward_iterator_type;
			typedef backward_iterator<T>					backward_iterator_type;
			typedef const_backward_iterator<T>				const_backward_iterator_type;

		protected:
			typedef binary_set_node<T> node_type;
			typedef binary_set_node<T> *node_ptr_type;
			wsize m_count;
			node_ptr_type m_root;
			allocator<node_type> m_alloc;
			algorithms::iteration_algorithm<node_type> m_algorithm;

		public:
			inline binary_set_object();
			inline binary_set_object(coffe::initializer_list<T> list);
			inline binary_set_object(const coffe::nullptr_t&);
			inline binary_set_object(binary_set_object&& ar);
			inline binary_set_object(const binary_set_object& ar);
			inline binary_set_object(const binary_set_object* ar);
			inline binary_set_object(const ienum_type* store);

		private:
			virtual ~binary_set_object();

		public: //methods
			inline bool is_empty()const;
			inline void clean();
			inline bool move(binary_set_object&);
			inline void iteration_method(iteration_method_t);

		public: //ienum overrides
			inline wsize counter()const override;
			inline T& at(const base_iterator_type& it) override;
			inline bool increase(base_iterator_type&)const override;
			inline bool increase(base_iterator_type&, int offset)const override;
			inline bool decrease(base_iterator_type&)const override;
			inline bool decrease(base_iterator_type&, int offset)const override;

			inline forward_iterator_type begin() override;
			inline forward_iterator_type end() override;
			inline const_forward_iterator_type begin()const override;
			inline const_forward_iterator_type end()const override;
			inline forward_iterator_type last() override;
			inline const_forward_iterator_type last()const override;

			inline backward_iterator_type rbegin() override;
			inline backward_iterator_type rend() override;
			inline const_backward_iterator_type rbegin()const override;
			inline const_backward_iterator_type rend()const override;

		public: //imap overrides
			inline bool copy(const ienum_type*) override;
			inline void extend(const ienum_type*) override;
			inline bool insert(T) override;
			inline bool remove(T&) override;
			inline bool remove(base_iterator<T> it) override;
			inline bool remove(base_iterator<T> it, T&) override;
			inline bool has_value(const T&)const override;
			inline iterator<T> find(const T&) override;
			inline const_iterator<T> find(const T&)const override;

		public: //overrides
			COFFE_DECLARE_INTERFACE();

		protected: //Memory Operations
			inline void destroy_tree(node_ptr_type)const;
			inline node_ptr_type find_node(T const&, node_ptr_type)const;
			inline node_ptr_type most_left(node_ptr_type)const;
			inline node_ptr_type most_right(node_ptr_type)const;
			inline node_ptr_type right_rotate(node_ptr_type);
			inline node_ptr_type left_rotate(node_ptr_type);
			inline node_ptr_type remove_node(node_ptr_type, T const&, bool& removed);
			inline node_ptr_type remove_node(node_ptr_type, T const&, T&, bool& removed);
		};



		/******************************************************************/
		/* template class binary_map_object :                             */
		/*  -> implements the buffer memory of binary_map object        */
		/******************************************************************/
		template<typename K, typename T, template<typename> class allocator>
		class binary_map_enum final
			: public smart<binary_map_enum<K, T, allocator>, ienum<pair<K, T>>>
		{
		public:
			typedef T										value_type;
			typedef K										key_type;
			typedef pair<K, T>								pair_type;
			typedef binary_map_enum<K, T, allocator>		self_type;
			typedef ienum<pair<K, T>>						ienum_type;
			typedef iterator<pair<K, T >>					iterator_type;
			typedef const_iterator<pair<K, T >>				const_iterator_type;
			typedef base_iterator<pair<K, T >>				base_iterator_type;
			typedef forward_iterator<pair<K, T >>			forward_iterator_type;
			typedef const_forward_iterator<pair<K, T >>		const_forward_iterator_type;
			typedef backward_iterator<pair<K, T >>			backward_iterator_type;
			typedef const_backward_iterator<pair<K, T >>	const_backward_iterator_type;

		protected:
			typedef binary_map_node<K, T> node_type;
			typedef binary_map_node<K, T> *node_ptr_type;
			mutable binary_map<K, T, allocator> m_map;
			algorithms::iteration_algorithm<node_type> m_algorithm;

		public:
			inline binary_map_enum(binary_map<K, T, allocator>, iteration_method_t);

		private:
			virtual ~binary_map_enum();

		protected:
			inline void dispose()override;

		public: //ienum overrides
			inline wsize counter()const override;
			inline pair_type& at(const base_iterator_type& it) override;
			inline bool increase(base_iterator_type&)const override;
			inline bool increase(base_iterator_type&, int offset)const override;
			inline bool decrease(base_iterator_type&)const override;
			inline bool decrease(base_iterator_type&, int offset)const override;

			inline forward_iterator_type begin() override;
			inline forward_iterator_type end() override;
			inline const_forward_iterator_type begin()const override;
			inline const_forward_iterator_type end()const override;
			inline forward_iterator_type last() override;
			inline const_forward_iterator_type last()const override;

			inline backward_iterator_type rbegin() override;
			inline backward_iterator_type rend() override;
			inline const_backward_iterator_type rbegin()const override;
			inline const_backward_iterator_type rend()const override;
		
		public: //overrides
			COFFE_DECLARE_INTERFACE();
		};


		/******************************************************************/
		/* template class binary_map_object :                             */
		/*  -> implements the buffer memory of binary_map object        */
		/******************************************************************/
		template<typename K, typename T, template<typename> class allocator>
		class binary_map_object final
			: public smart<binary_map_object<K, T, allocator>, imap<K, T>, ienum<pair<K, T>>>
		{
		public:
			typedef T										value_type;
			typedef K										key_type;
			typedef pair<K, T>								pair_type;
			typedef binary_map_object<K, T, allocator>		self_type;
			typedef ienum<pair<K, T>>						ienum_type;
			typedef imap<K, T>								imap_type;
			typedef iterator<pair<K, T >>					iterator_type;
			typedef const_iterator<pair<K, T >>				const_iterator_type;
			typedef base_iterator<pair<K, T >>				base_iterator_type;
			typedef forward_iterator<pair<K, T >>			forward_iterator_type;
			typedef const_forward_iterator<pair<K, T >>		const_forward_iterator_type;
			typedef backward_iterator<pair<K, T >>			backward_iterator_type;
			typedef const_backward_iterator<pair<K, T >>	const_backward_iterator_type;
			typedef algorithms::binary_tree_inorder_iteratior<pair_type> three_iterator;

		protected:
			friend binary_map_enum<K, T, allocator>;
			
			typedef binary_map_node<K, T> node_type;
			typedef binary_map_node<K, T> *node_ptr_type;
			wsize m_count;
			node_ptr_type m_root;
			allocator<node_type> m_alloc;

		public:
			inline binary_map_object();
			inline binary_map_object(std::initializer_list<pair<K, T>> list);
			inline binary_map_object(const std::nullptr_t&);
			inline binary_map_object(binary_map_object&& ar);
			inline binary_map_object(const binary_map_object& ar);
			inline binary_map_object(const binary_map_object* ar);
			inline binary_map_object(const ienum_type* store);

		private:
			virtual ~binary_map_object();

		protected:
			inline void dispose()override;

		public: //methods
			inline bool is_empty()const;
			inline void clear();
			inline bool move(binary_map_object&);

		public: //ienum overrides
			inline wsize counter()const override;
			inline pair_type& at(const base_iterator_type& it) override;
			inline bool increase(base_iterator_type&)const override;
			inline bool increase(base_iterator_type&, int offset)const override;
			inline bool decrease(base_iterator_type&)const override;
			inline bool decrease(base_iterator_type&, int offset)const override;

			inline forward_iterator_type begin() override;
			inline forward_iterator_type end() override;
			inline const_forward_iterator_type begin()const override;
			inline const_forward_iterator_type end()const override;
			inline forward_iterator_type last() override;
			inline const_forward_iterator_type last()const override;

			inline backward_iterator_type rbegin() override;
			inline backward_iterator_type rend() override;
			inline const_backward_iterator_type rbegin()const override;
			inline const_backward_iterator_type rend()const override;

		public: //imap overrides
			inline ienum_ptr<pair_type> enumerate(iteration_method_t)const override;
			inline bool copy(const ienum_type*) override;
			inline void extend(const ienum_type*) override;
			inline iterator_type insert(K, T) override;
			inline iterator_type insert(pair<K, T>) override;
			inline iterator_type update(K, T) override;
			inline iterator_type update(pair<K, T>) override;
			inline bool remove(K const&) override;
			inline bool remove(K const&, T&) override;
			inline bool remove(base_iterator_type it) override;
			inline bool remove(base_iterator_type it, T&) override;
			inline bool has_key(const K&)const override;
			inline iterator_type find(const K&) override;
			inline const_iterator_type find(const K&)const override;

		public: //overrides
			COFFE_DECLARE_INTERFACE();

		protected: //Memory Operations
			inline void destroy_tree(node_ptr_type);
			inline node_ptr_type find_node(key_type const&, node_ptr_type)const;
			inline node_ptr_type most_left(node_ptr_type)const;
			inline node_ptr_type most_right(node_ptr_type)const;
			inline node_ptr_type right_rotate(node_ptr_type);
			inline node_ptr_type left_rotate(node_ptr_type);
			inline node_ptr_type remove_node(node_ptr_type, key_type const&, bool& removed);
			inline node_ptr_type remove_node(node_ptr_type, key_type const&, T& out, bool& removed);
		};


		/******************************************************************/
		/* template class binary_map_object :                             */
		/*  -> implements the buffer memory of binary_map object        */
		/******************************************************************/
		template<text::encoding E, template<typename>class A,typename T, template<typename> class allocator>
		class binary_map_object<text::basic_string<E, A>, T, allocator> final
			: public smart<binary_map_object<text::basic_string<E, A>, T, allocator>, imap<text::basic_string<E, A>, T>>
		{
		public:
			typedef T																value_type;
			typedef text::basic_string<E, A>										key_type;
			typedef pair<text::basic_string<E, A>, T>								pair_type;
			typedef binary_map_object<text::basic_string<E, A>, T, allocator>		self_type;
			typedef ienum<pair<text::basic_string<E, A>, T>>						ienum_type;
			typedef imap<text::basic_string<E, A>, T>								imap_type;
			typedef iterator<pair<text::basic_string<E, A>, T >>					iterator_type;
			typedef const_iterator<pair<text::basic_string<E, A>, T >>				const_iterator_type;
			typedef base_iterator<pair<text::basic_string<E, A>, T >>				base_iterator_type;
			typedef forward_iterator<pair<text::basic_string<E, A>, T >>			forward_iterator_type;
			typedef const_forward_iterator<pair<text::basic_string<E, A>, T >>		const_forward_iterator_type;
			typedef backward_iterator<pair<text::basic_string<E, A>, T >>			backward_iterator_type;
			typedef const_backward_iterator<pair<text::basic_string<E, A>, T >>		const_backward_iterator_type;


		protected:
			typedef binary_map_node<text::basic_string<E, A>, T> node_type;
			typedef binary_map_node<text::basic_string<E, A>, T> *node_ptr_type;
			wsize m_count;
			node_ptr_type m_root;
			allocator<node_type> m_alloc;
			algorithms::iteration_algorithm<node_type> m_algorithm;

		public:
			inline binary_map_object();
			inline binary_map_object(std::initializer_list<pair<text::basic_string<E, A>, T>> list);
			inline binary_map_object(const std::nullptr_t&);
			inline binary_map_object(binary_map_object&& ar);
			inline binary_map_object(const binary_map_object& ar);
			inline binary_map_object(const binary_map_object* ar);
			inline binary_map_object(const ienum_type* store);

		private:
			virtual ~binary_map_object();

		public: //methods
			inline bool is_empty()const;
			inline void clean();
			inline bool move(binary_map_object&);
			inline void iteration_method(iteration_method_t);

		public: //ienum overrides
			inline wsize counter()const override;
			inline pair_type& at(const base_iterator_type& it) override;
			inline bool increase(base_iterator_type&)const override;
			inline bool increase(base_iterator_type&, int offset)const override;
			inline bool decrease(base_iterator_type&)const override;
			inline bool decrease(base_iterator_type&, int offset)const override;

			inline forward_iterator_type begin() override;
			inline forward_iterator_type end() override;
			inline const_forward_iterator_type begin()const override;
			inline const_forward_iterator_type end()const override;
			inline forward_iterator_type last() override;
			inline const_forward_iterator_type last()const override;

			inline backward_iterator_type rbegin() override;
			inline backward_iterator_type rend() override;
			inline const_backward_iterator_type rbegin()const override;
			inline const_backward_iterator_type rend()const override;

		public: //imap overrides
			inline bool copy(const ienum_type*) override;
			inline void extend(const ienum_type*) override;
			inline iterator_type insert(cstr_t, T) override;
			inline iterator_type insert(pair<text::basic_string<E, A>, T>) override;
			inline iterator_type update(cstr_t, T) override;
			inline iterator_type update(pair<text::basic_string<E, A>, T>) override;
			inline bool remove(cstr_t) override;
			inline bool remove(cstr_t, T&) override;
			inline bool remove(base_iterator_type it) override;
			inline bool remove(base_iterator_type it, T&) override;
			inline bool has_key(cstr_t)const override;
			inline iterator_type find(cstr_t) override;
			inline const_iterator_type find(cstr_t)const override;

		public: //overrides
			COFFE_DECLARE_INTERFACE();

		protected: //Memory Operations
			inline void destroy_tree(node_ptr_type);
			inline node_ptr_type find_node(cstr_t, node_ptr_type)const;
			inline node_ptr_type most_left(node_ptr_type)const;
			inline node_ptr_type most_right(node_ptr_type)const;
			inline node_ptr_type right_rotate(node_ptr_type);
			inline node_ptr_type left_rotate(node_ptr_type);
			inline node_ptr_type remove_node(node_ptr_type, cstr_t, bool& removed);
			inline node_ptr_type remove_node(node_ptr_type, cstr_t, T& out, bool& removed);
		};



		/******************************************************************/
		/* template class binary_map_object :                             */
		/*  -> implements the buffer memory of binary_map object        */
		/******************************************************************/
		template<typename T, template<typename> class allocator>
		class binary_map_object<string, T, allocator> final
			: public smart<binary_map_object<string, T, allocator>, imap<string, T>>
		{
		public:
			typedef T											value_type;
			typedef string										key_type;
			typedef pair<string, T>								pair_type;
			typedef binary_map_object<string, T, allocator>		self_type;
			typedef ienum<pair<string, T>>						ienum_type;
			typedef imap<string, T>								imap_type;
			typedef iterator<pair<string, T >>					iterator_type;
			typedef const_iterator<pair<string, T >>			const_iterator_type;
			typedef base_iterator<pair<string, T >>				base_iterator_type;
			typedef forward_iterator<pair<string, T >>			forward_iterator_type;
			typedef const_forward_iterator<pair<string, T >>	const_forward_iterator_type;
			typedef backward_iterator<pair<string, T >>			backward_iterator_type;
			typedef const_backward_iterator<pair<string, T >>	const_backward_iterator_type;


		protected:
			typedef binary_map_node<string, T> node_type;
			typedef binary_map_node<string, T> *node_ptr_type;
			wsize m_count;
			node_ptr_type m_root;
			allocator<node_type> m_alloc;
			algorithms::iteration_algorithm<node_type> m_algorithm;

		public:
			inline binary_map_object();
			inline binary_map_object(std::initializer_list<pair<string, T>> list);
			inline binary_map_object(const std::nullptr_t&);
			inline binary_map_object(binary_map_object&& ar);
			inline binary_map_object(const binary_map_object& ar);
			inline binary_map_object(const binary_map_object* ar);
			inline binary_map_object(const ienum_type* store);

		private:
			virtual ~binary_map_object();

		public: //methods
			inline bool is_empty()const;
			inline void clean();
			inline bool move(binary_map_object&);
			inline void iteration_method(iteration_method_t);

		public: //ienum overrides
			inline wsize counter()const override;
			inline pair_type& at(const base_iterator_type& it) override;
			inline bool increase(base_iterator_type&)const override;
			inline bool increase(base_iterator_type&, int offset)const override;
			inline bool decrease(base_iterator_type&)const override;
			inline bool decrease(base_iterator_type&, int offset)const override;

			inline forward_iterator_type begin() override;
			inline forward_iterator_type end() override;
			inline const_forward_iterator_type begin()const override;
			inline const_forward_iterator_type end()const override;
			inline forward_iterator_type last() override;
			inline const_forward_iterator_type last()const override;

			inline backward_iterator_type rbegin() override;
			inline backward_iterator_type rend() override;
			inline const_backward_iterator_type rbegin()const override;
			inline const_backward_iterator_type rend()const override;

		public: //imap overrides
			inline bool copy(const ienum_type*) override;
			inline void extend(const ienum_type*) override;
			inline iterator_type insert(cstr_t, T) override;
			inline iterator_type insert(pair<string, T>) override;
			inline iterator_type update(cstr_t, T) override;
			inline iterator_type update(pair<string, T>) override;
			inline bool remove(cstr_t) override;
			inline bool remove(cstr_t, T&) override;
			inline bool remove(base_iterator_type it) override;
			inline bool remove(base_iterator_type it, T&) override;
			inline bool has_key(cstr_t)const override;
			inline iterator_type find(cstr_t) override;
			inline const_iterator_type find(cstr_t)const override;

		public: //overrides
			COFFE_DECLARE_INTERFACE();

		protected: //Memory Operations
			inline void destroy_tree(node_ptr_type);
			inline node_ptr_type find_node(key_type const&, node_ptr_type)const;
			inline node_ptr_type most_left(node_ptr_type)const;
			inline node_ptr_type most_right(node_ptr_type)const;
			inline node_ptr_type right_rotate(node_ptr_type);
			inline node_ptr_type left_rotate(node_ptr_type);
			inline node_ptr_type remove_node(node_ptr_type, key_type const&, bool& removed);
			inline node_ptr_type remove_node(node_ptr_type, key_type const&, T& out, bool& removed);
		};
	}

	/******************************************************************/
	/* template class coffe::object_wrapper<binary_map_object> :            */
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
		void reset();
		bool is_empty()const;
		collections::binary_map_object<K, T, allocator>* get(void)const;
		void set(collections::binary_map_object<K, T, allocator>*);
		collections::binary_map_object<K, T, allocator> ** addres_of(void);

		auto begin() { return m_ptr ? m_ptr->begin() : decltype(m_ptr->begin())(); }
		auto end() { return m_ptr ? m_ptr->end() : decltype(m_ptr->end())(); }
		auto begin()const { return m_ptr ? ((type const*)m_ptr)->begin() : decltype(((type const*)m_ptr)->begin())(); }
		auto end()const { return m_ptr ? ((type const*)m_ptr)->end() : decltype(((type const*)m_ptr)->end())();
		}

	public:
		object_wrapper& operator = (object_wrapper<collections::binary_map_object<K, T, allocator>> &&);
		object_wrapper& operator = (object_wrapper<collections::binary_map_object<K, T, allocator>> const&);
		object_wrapper& operator = (std::nullptr_t const&);
		object_wrapper& operator = (collections::binary_map_object<K, T, allocator>*);
		object_wrapper& operator = (collections::ienum<collections::pair<K, T>> const* store);

		object_wrapper& operator += (collections::pair<K, T>);

		inline operator object_t()const;

		object_wrapper_ptr<collections::binary_map_object<K, T, allocator>> operator & (void);
		collections::binary_map_object<K, T, allocator> * operator -> (void)const;
		explicit operator collections::binary_map_object<K, T, allocator> * (void);
		explicit operator collections::binary_map_object<K, T, allocator> const* (void)const;

		T& operator [] (K const&);
		T operator [] (K const&)const;
	};

}

//#include<coffe/collections/binary_tree_specialization.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#include<coffe/collections/inline/binary_tree.inl>

#endif//__COFFE_BINARY_TREE_H__
