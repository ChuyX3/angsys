/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_COLLECTIONS_H__
#define __ANG_COLLECTIONS_H__

#include <ang/collections/iterators.h>

namespace ang
{
	namespace collections
	{
		typedef pointer position_t;

		template<class K, class T>
		struct pair
		{
			typedef	pair<K, T>		self_t;
			typedef T				value_t;
			typedef K				key_t;

			key_t		key;
			value_t		value;

			friend inline bool operator == (const self_t& value1, const self_t& value2) { return value1.key == value2.key; }
			friend inline bool operator == (const self_t& value1, const key_t& value2) { return value1.key == value2; }
			friend inline bool operator == (const key_t& value1, const self_t& value2) { return value1 == value2.key; }
			friend inline bool operator != (const self_t& value1, const self_t& value2) { return value1.key != value2.key; }
			friend inline bool operator != (const self_t& value1, const key_t& value2) { return value1.key != value2; }
			friend inline bool operator != (const key_t& value1, const self_t& value2) { return value1 != value2.key; }
			friend inline bool operator >= (const self_t& value1, const self_t& value2) { return value1.key >= value2.key; }
			friend inline bool operator >= (const self_t& value1, const key_t& value2) { return value1.key >= value2; }
			friend inline bool operator >= (const key_t& value1, const self_t& value2) { return value1 >= value2.key; }
			friend inline bool operator <= (const self_t& value1, const self_t& value2) { return value1.key <= value2.key; }
			friend inline bool operator <= (const self_t& value1, const key_t& value2) { return value1.key <= value2; }
			friend inline bool operator <= (const key_t& value1, const self_t& value2) { return value1 <= value2.key; }
			friend inline bool operator > (const self_t& value1, const self_t& value2) { return value1.key > value2.key; }
			friend inline bool operator > (const self_t& value1, const key_t& value2) { return value1.key > value2; }
			friend inline bool operator > (const key_t& value1, const self_t& value2) { return value1 > value2.key; }
			friend inline bool operator < (const self_t& value1, const self_t& value2) { return value1.key < value2.key; }
			friend inline bool operator < (const self_t& value1, const key_t& value2) { return value1.key < value2; }
			friend inline bool operator < (const key_t& value1, const self_t& value2) { return value1 < value2.key; }
		};


		template<typename T>
		struct linked_node
		{
			typedef T type;
			typedef linked_node<T> sefl_t;
			typedef linked_node<T> node_t;
			typedef linked_node<T> *node_ptr_t;

			linked_node()
				: value()
				, next(null) {
			}

			linked_node(T val)
				: value(ang::move(val))
				, next(null) {
			}

			T value;
			node_ptr_t next;
		};


		template<typename T>
		struct double_linked_node
		{
			typedef T type;
			typedef double_linked_node<T> self_t;
			typedef double_linked_node<T> node_t;
			typedef double_linked_node<T> *node_ptr_t;

			double_linked_node()
				: value()
				, prev(null)
				, next(null) {
			}

			double_linked_node(T val)
				: value(ang::move(val))
				, prev(null)
				, next(null) {
			}

			T value;
			node_ptr_t prev;
			node_ptr_t next;
		};

		namespace algorithms
		{
			ANG_BEGIN_ENUM(LINK, iteration_method, byte)
				by_levels,
				preorder,
				inorder,
				postorder,
			ANG_END_ENUM(iteration_method);

			template<class _node>
			class iteration_algorithm
			{
			public:
				typedef _node* node_ptr_t;
				typedef node_ptr_t(*iteration_callback_t)(node_ptr_t, index&);

			public: //overrides
				iteration_callback_t begin;
				iteration_callback_t end;
				iteration_callback_t next;
				iteration_callback_t prev;
				iteration_callback_t most_left;
				iteration_callback_t most_right;

				iteration_algorithm(iteration_algorithm const& algorithm) {
					begin = algorithm.begin;
					end = algorithm.end;
					next = algorithm.next;
					prev = algorithm.prev;
					most_left = algorithm.most_left;
					most_right = algorithm.most_right;
				}

				iteration_algorithm& operator = (iteration_algorithm const& algorithm) {
					begin = algorithm.begin;
					end = algorithm.end;
					next = algorithm.next;
					prev = algorithm.prev;
					most_left = algorithm.most_left;
					most_right = algorithm.most_right;
					return*this;
				}

			protected:
				iteration_algorithm() {
					begin = null;
					end = null;
					next = null;
					prev = null;
					most_left = null;
					most_right = null;
				}
			};
		}

	}//collections


	template<class T> class intf_wrapper<collections::ienum<T>>
	{
	public:
		typedef collections::ienum<T> type;
	protected:
		collections::ienum<T>* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(collections::ienum<T>*);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();

	public:
		void clean();
		bool is_empty()const;
		collections::ienum<T>* get(void)const;
		void set(collections::ienum<T>*);
		collections::ienum<T> ** addres_of(void);

		collections::forward_iterator<T> begin() {
			return _ptr ? _ptr ->begin() : collections::forward_iterator<T>(null);
		}
		collections::forward_iterator<T> end() {
			return _ptr ? _ptr->end() : collections::forward_iterator<T>(null);
		}

		collections::const_forward_iterator<T> begin()const {
			return _ptr ? _ptr->begin() : collections::const_forward_iterator<T>(null);
		}
		collections::const_forward_iterator<T> end()const {
			return _ptr ? _ptr->end() : collections::const_forward_iterator<T>(null);
		}

	public:
		intf_wrapper& operator = (collections::ienum<T>*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		collections::ienum<T> * operator -> (void);
		collections::ienum<T> const* operator -> (void)const;
		operator collections::ienum<T> * (void);
		operator collections::ienum<T> const* (void)const;
	};


	template<class T> class intf_wrapper<collections::iarray<T>>
	{
	public:
		typedef collections::iarray<T> type;
	protected:
		collections::iarray<T>* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(collections::iarray<T>*);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();

	public:
		void clean();
		bool is_empty()const;
		collections::iarray<T>* get(void)const;
		void set(collections::iarray<T>*);
		collections::iarray<T> ** addres_of(void);

		collections::forward_iterator<T> begin() {
			return _ptr ? _ptr->begin() : collections::forward_iterator<T>(null);
		}
		collections::forward_iterator<T> end() {
			return _ptr ? _ptr->end() : collections::forward_iterator<T>(null);
		}

		collections::const_forward_iterator<T> begin()const {
			return _ptr ? _ptr->begin() : collections::const_forward_iterator<T>(null);
		}
		collections::const_forward_iterator<T> end()const {
			return _ptr ? _ptr->end() : collections::const_forward_iterator<T>(null);
		}

	public:
		intf_wrapper& operator = (collections::iarray<T>*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		collections::iarray<T> * operator -> (void);
		collections::iarray<T> const* operator -> (void)const;
		operator collections::iarray<T> * (void);
		operator collections::iarray<T> const* (void)const;
	};


	template<class T> class intf_wrapper<collections::ilist<T>>
	{
	public:
		typedef collections::ilist<T> type;
	protected:
		collections::ilist<T>* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(collections::ilist<T>*);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();

	public:
		void clean();
		bool is_empty()const;
		collections::ilist<T>* get(void)const;
		void set(collections::ilist<T>*);
		collections::ilist<T> ** addres_of(void);

		collections::forward_iterator<T> begin() {
			return _ptr ? _ptr->begin() : collections::forward_iterator<T>(null);
		}
		collections::forward_iterator<T> end() {
			return _ptr ? _ptr->end() : collections::forward_iterator<T>(null);
		}

		collections::const_forward_iterator<T> begin()const {
			return _ptr ? _ptr->begin() : collections::const_forward_iterator<T>(null);
		}
		collections::const_forward_iterator<T> end()const {
			return _ptr ? _ptr->end() : collections::const_forward_iterator<T>(null);
		}

	public:
		intf_wrapper& operator = (collections::ilist<T>*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		collections::ilist<T> * operator -> (void);
		collections::ilist<T> const* operator -> (void)const;
		operator collections::ilist<T> * (void);
		operator collections::ilist<T> const* (void)const;
	};

	template<class K, class T> class intf_wrapper<collections::imap<K,T>>
	{
	public:
		typedef collections::imap<K,T> type;
	protected:
		collections::imap<K,T>* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(collections::imap<K,T>*);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();

	public:
		void clean();
		bool is_empty()const;
		collections::imap<K,T>* get(void)const;
		void set(collections::imap<K,T>*);
		collections::imap<K,T> ** addres_of(void);

		collections::forward_iterator<T> begin() {
			return _ptr ? _ptr->begin() : collections::forward_iterator<T>(null);
		}
		collections::forward_iterator<T> end() {
			return _ptr ? _ptr->end() : collections::forward_iterator<T>(null);
		}

		collections::const_forward_iterator<T> begin()const {
			return _ptr ? _ptr->begin() : collections::const_forward_iterator<T>(null);
		}
		collections::const_forward_iterator<T> end()const {
			return _ptr ? _ptr->end() : collections::const_forward_iterator<T>(null);
		}

	public:
		intf_wrapper& operator = (collections::imap<K,T>*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		collections::imap<K,T> * operator -> (void);
		collections::imap<K,T> const* operator -> (void)const;
		operator collections::imap<K,T> * (void);
		operator collections::imap<K,T> const* (void)const;
	};

}//ang


#endif//__ANG_COLLECTIONS_H__
