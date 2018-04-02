#ifndef __ANGSYS_H__
#elif !defined __ANG_COLLECTIONS_H__
#define __ANG_COLLECTIONS_H__

namespace ang
{
	namespace collections
	{
		const wsize ITERATOR_TYPE_DEFAULT = 0;
		const wsize ITERATOR_TYPE_REVERSE = 1;

		template<typename K, typename T> struct pair;
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

		template<class T> using ienum_t = intf_wrapper<ienum<T>>;
		template<class T> using ienum_ptr = intf_wrapper<ienum<T>>;

		template<class T> using iarray_t = intf_wrapper<iarray<T>>;
		template<class T> using iarray_ptr = intf_wrapper<iarray<T>>;

		template<class T> using ilist_t = intf_wrapper<ilist<T>>;
		template<class T> using ilist_ptr = intf_wrapper<ilist<T>>;

		/******************************************************************/
/* interface ang::collections::ienum :                            */
/*  -> represents an enumeration of objects or variables which    */
/*     can be accessed orderly                                    */
/******************************************************************/
		template<typename T>
		ang_begin_interface_inline(ienum)
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
			visible vcall forward_iterator<T> last() pure;
			visible vcall const_forward_iterator<T> last()const pure;

			visible vcall backward_iterator<T> rbegin() pure;
			visible vcall backward_iterator<T> rend() pure;
			visible vcall const_backward_iterator<T> rbegin()const pure;
			visible vcall const_backward_iterator<T> rend()const pure;
		ang_end_interface();


		/******************************************************************/
		/* interface ang::collections::iarray :							  */
		/*  -> represents a collection of objects or variables which      */
		/*     can be accessed unorderly                                  */
		/******************************************************************/
		template<typename T>
		ang_begin_interface_inline(iarray, public ienum<T>)
			using ienum<T>::at;
			visible vcall iterator<T> at(windex) pure;
			visible vcall const_iterator<T> at(windex)const pure;
			visible vcall void copy(const ienum<T>*) pure;
			visible vcall iterator<T> find(core::delegates::function<bool(T const&)>, bool invert = false)const pure;
			visible vcall iterator<T> find(core::delegates::function<bool(T const&)>, base_iterator<T> next_to, bool invert = false)const pure;
			visible vcall ienum_ptr<T> find_all(core::delegates::function<bool(T const&)>)const pure;
		ang_end_interface();

		/******************************************************************/
		/* interface ang::collections::ilist :                            */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements randoml y                    */
		/******************************************************************/
		template<typename T>
		ang_begin_interface_inline(ilist, public iarray<T>)
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
		ang_end_interface();

		/******************************************************************/
		/* interface ang::collections::isequence :                        */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements sequentially                 */
		/******************************************************************/
		template<typename T>
		ang_begin_interface_inline(isequence, public ienum<T>)
			visible vcall void extend(const ienum<T>*) pure;
			visible vcall void push(T const&) pure;
			visible vcall bool pop() pure;
			visible vcall bool pop(T&) pure;
		ang_end_interface();


		/******************************************************************/
		/* interface ang::collections::imap :							  */
		/*  -> represents a collection of objects or variables which      */
		/*     can be accessed by a key                                   */
		/******************************************************************/
		template<typename K, typename T>
		ang_begin_interface_inline(imap, public ienum<pair<K, T>>)
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
		ang_end_interface();


		template<typename T>
		struct linked_node {
			typedef T type;
			typedef linked_node<T> sefl_t;
			typedef linked_node<T> node_t;
			typedef linked_node<T> *node_ptr_t;

			linked_node()
				: data()
				, next(null) {
			}

			linked_node(T val)
				: data(ang::move(val))
				, next(null) {
			}

			T data;
			node_ptr_t next;
		};

		template<typename K, typename T>
		struct linked_node<pair<K, T>> {
			typedef pair<K, T> type;
			typedef linked_node<type> sefl_t;
			typedef linked_node<type> node_t;
			typedef linked_node<type> *node_ptr_t;

			linked_node()
				: data()
				, next(null) {
			}

			linked_node(type val)
				: data(ang::move(val))
				, next(null) {
			}

			linked_node(K key, T val)
				: data(ang::move(key), ang::move(val))
				, next(null) {
			}

			type data;
			node_ptr_t next;
		};

		template<typename T>
		struct double_linked_node {
			typedef T type;
			typedef double_linked_node<T> self_t;
			typedef double_linked_node<T> node_t;
			typedef double_linked_node<T> *node_ptr_t;

			double_linked_node()
				: data()
				, prev(null)
				, next(null) {
			}

			double_linked_node(T val)
				: data(ang::move(val))
				, prev(null)
				, next(null) {
			}

			T data;
			node_ptr_t prev;
			node_ptr_t next;
		};

		template<typename K, typename T>
		struct double_linked_node<pair<K, T>> {
			typedef pair<K, T> type;
			typedef double_linked_node<type> self_t;
			typedef double_linked_node<type> node_t;
			typedef double_linked_node<type> *node_ptr_t;

			double_linked_node()
				: data()
				, prev(null)
				, next(null) {
			}

			double_linked_node(type val)
				: data(ang::move(val))
				, prev(null)
				, next(null) {
			}

			double_linked_node(K key, T val)
				: data(ang::move(key), ang::move(val))
				, prev(null)
				, next(null) {
			}

			type data;
			node_ptr_t prev;
			node_ptr_t next;
		};

	}
}

#include <ang/collections/iterators.h>

namespace ang
{

}

#endif//__ANG_COLLECTIONS_H__
