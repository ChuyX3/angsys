/*********************************************************************************************************************/
/*   File Name: angsys.hpp                                                                                             */
/*   Autor: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                    */
/*   File description: this file declare all functions and clases for dynamic type naming and types conversions.     */
/*   Also it defines useful macros for the use of this functions and clases.                                         */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_HPP__
#error ...
#elif !defined __ANG_COLLECTIONS_HPP__
#define __ANG_COLLECTIONS_HPP__

namespace ang
{
	namespace collections
	{
		const wsize ITERATOR_TYPE_DEFAULT = 0;
		const wsize ITERATOR_TYPE_REVERSE = 1;

		template<class K, class T>struct pair;
		template<typename T> class iterator;
		template<typename T> class base_iterator;
		template<typename T> class forward_iterator;
		template<typename T> class backward_iterator;

		template<typename T> using const_iterator = iterator<const T>;
		template<typename T> using const_forward_iterator = forward_iterator<const T>;
		template<typename T> using const_backward_iterator = backward_iterator<const T>;


		template<typename T> struct ienum;
		template<typename T> struct iarray;
		template<typename T> struct ilist;


		/******************************************************************/
		/* interface ang::collections::ienum :                            */
		/*  -> represents an enumeration of objects or variables which    */
		/*     can be accessed orderly                                    */
		/******************************************************************/
		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE(ienum)
			visible vcall wsize counter()const pure;

			visible vcall T& at(base_iterator<T> const&) pure;
			visible vcall bool increase(base_iterator<T>&)const pure;
			visible vcall bool increase(base_iterator<T>&, int offset)const pure;
			visible vcall bool decrease(base_iterator<T>&)const pure;
			visible vcall bool decrease(base_iterator<T>&, int offset)const pure;

			visible vcall forward_iterator<T> begin() pure;
			visible vcall forward_iterator<T> end() pure;
			visible vcall const_forward_iterator<T> begin()const pure;
			visible vcall const_forward_iterator<T> end()const pure;

			visible vcall backward_iterator<T> rbegin() pure;
			visible vcall backward_iterator<T> rend() pure;
			visible vcall const_backward_iterator<T> rbegin()const pure;
			visible vcall const_backward_iterator<T> rend()const pure;
		ANG_END_INTERFACE();

		template<class T> using ienum_t = intf_wrapper<ienum<T>>;

		/******************************************************************/
		/* interface ang::collections::iarray :							  */
		/*  -> represents a collection of objects or variables which      */
		/*     can be accessed unorderly                                  */
		/******************************************************************/
		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(iarray, public ienum<T>)
			using ienum<T>::at;
			visible vcall iterator<T> at(windex) pure;
			visible vcall const_iterator<T> at(windex)const pure;
			visible vcall void copy(const ienum<T>*) pure;
			visible vcall iterator<T> find(const T&, bool invert = false)const pure;
			visible vcall iterator<T> find(const T&, base_iterator<T> next_to, bool invert = false)const pure;
		ANG_END_INTERFACE();

		template<class T> using iarray_t = intf_wrapper<iarray<T>>;

		/******************************************************************/
		/* interface ang::collections::ilist :                            */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements randoml y                    */
		/******************************************************************/
		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(ilist, public iarray<T>)
			visible vcall void extend(const ienum<T>*) pure;
			visible vcall void push(T const&, bool last = true) pure;
			visible vcall bool insert(windex idx, T const&) pure;
			visible vcall bool insert(base_iterator<T> it, T const&) pure;
			visible vcall bool pop(bool last = true) pure;
			visible vcall bool pop(T&, bool last = true) pure;
			visible vcall bool pop_at(windex idx) pure;
			visible vcall bool pop_at(base_iterator<T> it) pure;
			visible vcall bool pop_at(windex idx, T&) pure;
			visible vcall bool pop_at(base_iterator<T> it, T&) pure;
		ANG_END_INTERFACE();

		template<class T> using ilist_t = intf_wrapper<ilist<T>>;

		/******************************************************************/
		/* interface ang::collections::isequence :                        */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements sequentially                 */
		/******************************************************************/
		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(isequence, public ienum<T>)
			visible vcall void extend(const ienum<T>*) pure;
			visible vcall void push(T const&) pure;
			visible vcall bool pop() pure;
			visible vcall bool pop(T&) pure;
		ANG_END_INTERFACE();

		template<class T> using isequence_t = intf_wrapper<isequence<T>>;

		/******************************************************************/
		/* interface ang::collections::imap :                             */
		/*  -> represents a map of objects or vareiables which can be     */
		/*     accessed by a key. Also the user can insert, find and      */
		/*     remove the elements, the items may or may not be sorted    */
		/******************************************************************/
		template<typename K, typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(imap, public ienum<pair<K, T>>)
			using ienum<T>::at;
			visible vcall iterator<T> at(K const&) pure;
			visible vcall const_iterator<T> at(K const&)const pure;
			visible vcall bool copy(const ienum<pair<K, T>>*) pure;
			visible vcall void extend(const ienum<pair<K, T>>*) pure;
			visible vcall bool insert(K, T) pure;
			visible vcall bool insert(pair<K, T>) pure;
			visible vcall bool update(K, T) pure;
			visible vcall bool update(pair<K, T>) pure;
			visible vcall bool remove(K const&) pure;
			visible vcall bool remove(K const&, T&) pure;
			visible vcall bool remove(base_iterator<pair<K, T>> it) pure;
			visible vcall bool remove(base_iterator<pair<K, T>> it, T&) pure;
			visible vcall bool has_key(const K&)const pure;
			visible vcall iterator<pair<K, T>> find(const K&) pure;
			visible vcall const_iterator<pair<K, T>> find(const K&)const pure;
		ANG_END_INTERFACE();

		template<class K, class T> using imap_t = intf_wrapper<imap<K,T>>;
		typedef pointer position_t;

	}
}

#include <ang/collections/iterators.hpp>

namespace ang
{
	namespace collections
	{
		template<typename K, typename T>
		struct pair
		{
			typedef	pair<K, T>		self_t;
			typedef T				value_t;
			typedef K				key_t;

			key_t		key;
			value_t		value;
		};

		template<typename T>
		struct linked_node {
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
		struct double_linked_node {
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


		template<typename T>
		struct linked_node<object_wrapper<T>> {
			typedef object_wrapper<T> type;
			typedef linked_node<object_wrapper<T>> sefl_t;
			typedef linked_node<object_wrapper<T>> node_t;
			typedef linked_node<object_wrapper<T>> *node_ptr_t;

			linked_node()
				: value()
				, next(null) {
			}

			linked_node(object_wrapper<T> const& val)
				: value(val.get())
				, next(null) {
			}

			object_wrapper<T> value;
			node_ptr_t next;
		};

		template<typename T>
		struct double_linked_node<object_wrapper<T>> {
			typedef object_wrapper<T> type;
			typedef double_linked_node<object_wrapper<T>> self_t;
			typedef double_linked_node<object_wrapper<T>> node_t;
			typedef double_linked_node<object_wrapper<T>> *node_ptr_t;

			double_linked_node()
				: value()
				, prev(null)
				, next(null) {
			}

			double_linked_node(object_wrapper<T> const& val)
				: value(val.get())
				, prev(null)
				, next(null) {
			}

			object_wrapper<T> value;
			node_ptr_t prev;
			node_ptr_t next;
		};

	}

	template<typename K, typename T, typename U> 
	struct comparision_operations<collections::pair<K, T>, U> {
		template<comparision_type TYPE> static bool compare(const collections::pair<K, T>& a1, const U& a2) {
			return comparision_operations<K, U>::template compare<TYPE>(a1.key, a2);
		}
	};

	namespace collections
	{
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



			template<typename K, typename T>
			wsize binary_search(K const& key, array_view<T> const& vector)
			{
				long64 first = 0, last = (long64)vector.size() - 1;
				long64 mid;
				while (first <= last)
				{
					mid = (first + last) / 2;
					if (comparision_operations<T, K>::template compare<comparision_same>(vector[mid], key)) return (wsize)mid;
					else if (comparision_operations<T, K>::template compare<comparision_major>(vector[mid], key)) last = mid - 1;
					else first = mid + 1;
				}
				return invalid_index;
			}

		}

	}

	template<typename T> class intf_wrapper<collections::ienum<T>>
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

	template<typename T> class intf_wrapper<collections::iarray<T>>
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

	template<typename T> class intf_wrapper<collections::ilist<T>>
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

	template<typename K, typename T> class intf_wrapper<collections::imap<K, T>>
	{
	public:
		typedef collections::imap<K, T> type;
	protected:
		collections::imap<K, T>* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(collections::imap<K, T>*);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();

	public:
		void clean();
		bool is_empty()const;
		collections::imap<K, T>* get(void)const;
		void set(collections::imap<K, T>*);
		collections::imap<K, T> ** addres_of(void);

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
		intf_wrapper& operator = (collections::imap<K, T>*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		collections::imap<K, T> * operator -> (void);
		collections::imap<K, T> const* operator -> (void)const;
		operator collections::imap<K, T> * (void);
		operator collections::imap<K, T> const* (void)const;
	};

}

#endif//__ANG_COLLECTIONS_HPP__
