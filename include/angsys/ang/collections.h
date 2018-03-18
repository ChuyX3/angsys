#ifndef __ANGSYS_H__
#elif !defined __ANG_COLLECTIONS_H__
#define __ANG_COLLECTIONS_H__

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

	}
}

#include <ang/collections/iterators.h>

namespace ang
{
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
		void clear();
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
		void clear();
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
		void clear();
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

}

#endif//__ANG_COLLECTIONS_H__
