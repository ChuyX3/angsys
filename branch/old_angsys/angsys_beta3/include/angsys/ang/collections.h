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


		template<class N> class iteration_algorithm;

		/******************************************************************/
		/* interface ang::collections::ienum :                            */
		/*  -> represents an enumeration of objects or variables which    */
		/*     can be accessed orderly                                    */
		/******************************************************************/
		template<typename T>
		ang_begin_interface_inline(ienum)
			visible vcall wsize counter()const pure
			visible vcall T& at(base_iterator<T> const&) pure
			visible vcall bool increase(base_iterator<T>&)const pure
			visible vcall bool increase(base_iterator<T>&, int offset)const pure
			visible vcall bool decrease(base_iterator<T>&)const pure
			visible vcall bool decrease(base_iterator<T>&, int offset)const pure

			visible vcall forward_iterator<T> begin() pure
			visible vcall forward_iterator<T> end() pure
			visible vcall const_forward_iterator<T> begin()const pure
			visible vcall const_forward_iterator<T> end()const pure
			visible vcall forward_iterator<T> last() pure
			visible vcall const_forward_iterator<T> last()const pure

			visible vcall backward_iterator<T> rbegin() pure
			visible vcall backward_iterator<T> rend() pure
			visible vcall const_backward_iterator<T> rbegin()const pure
			visible vcall const_backward_iterator<T> rend()const pure
		ang_end_interface();


		/******************************************************************/
		/* interface ang::collections::iarray :							  */
		/*  -> represents a collection of objects or variables which      */
		/*     can be accessed unorderly                                  */
		/******************************************************************/
		template<typename T>
		ang_begin_interface_inline(iarray, ienum<T>)
			using ienum<T>::at;
			visible vcall iterator<T> at(windex) pure
			visible vcall const_iterator<T> at(windex)const pure
			visible vcall void copy(const ienum<T>*) pure
			visible vcall iterator<T> find(core::delegates::function<bool(T const&)>, bool invert = false)const pure
			visible vcall iterator<T> find(core::delegates::function<bool(T const&)>, base_iterator<T> next_to, bool invert = false)const pure
			visible vcall ienum_ptr<T> find_all(core::delegates::function<bool(T const&)>)const pure
		ang_end_interface();

		/******************************************************************/
		/* interface ang::collections::ilist :                            */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements randoml y                    */
		/******************************************************************/
		template<typename T>
		ang_begin_interface_inline(ilist, iarray<T>)
			visible vcall void extend(const ienum<T>*) pure
			visible vcall void push(T const&, bool last = true) pure
			visible vcall bool insert(windex idx, T const&) pure
			visible vcall bool insert(base_iterator<T> it, T const&) pure
			visible vcall bool pop(bool last = true) pure
			visible vcall bool pop(T&, bool last = true) pure
			visible vcall bool pop_at(windex idx) pure
			visible vcall bool pop_at(base_iterator<T> it) pure
			visible vcall bool pop_at(windex idx, T&) pure
			visible vcall bool pop_at(base_iterator<T> it, T&) pure
		ang_end_interface();

		/******************************************************************/
		/* interface ang::collections::isequence :                        */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements sequentially                 */
		/******************************************************************/
		template<typename T>
		ang_begin_interface_inline(isequence, ienum<T>)
			visible vcall void extend(const ienum<T>*) pure
			visible vcall void push(T const&) pure
			visible vcall bool pop() pure
			visible vcall bool pop(T&) pure
		ang_end_interface();


		/******************************************************************/
		/* interface ang::collections::iset :							  */
		/*  -> represents a collection of objects or variables which      */
		/*     can be accessed by a key                                   */
		/******************************************************************/
		template<typename T>
		ang_begin_interface_inline(iset, ienum<T>)
			visible vcall bool copy(const ienum<T>*) pure
			visible vcall void extend(const ienum<T>*) pure
			visible vcall bool insert(T) pure
			visible vcall bool remove(T&) pure
			visible vcall bool remove(base_iterator<T> it) pure
			visible vcall bool remove(base_iterator<T> it, T&) pure
			visible vcall bool has_value(const T&)const pure
			visible vcall iterator<T> find(const T&) pure
			visible vcall const_iterator<T> find(const T&)const pure
		ang_end_interface();

		/******************************************************************/
		/* interface ang::collections::imap :							  */
		/*  -> represents a collection of objects or variables which      */
		/*     can be accessed by a key                                   */
		/******************************************************************/
		template<typename K, typename T>
		ang_begin_interface_inline(imap, ienum<pair<K COMA T>>)
			visible vcall bool copy(const ienum<pair<K, T>>*) pure
			visible vcall void extend(const ienum<pair<K, T>>*) pure
			visible vcall iterator<pair<K, T>> insert(K, T) pure
			visible vcall iterator<pair<K, T>> insert(pair<K, T>) pure
			visible vcall iterator<pair<K, T>> update(K, T) pure
			visible vcall iterator<pair<K, T>> update(pair<K, T>) pure
			visible vcall bool remove(K const&) pure
			visible vcall bool remove(K const&, T&) pure
			visible vcall bool remove(base_iterator<pair<K, T>> it) pure
			visible vcall bool remove(base_iterator<pair<K, T>> it, T&) pure
			visible vcall bool has_key(const K&)const pure
			visible vcall iterator<pair<K, T>> find(const K&) pure
			visible vcall const_iterator<pair<K, T>> find(const K&)const pure
		ang_end_interface();
	
		template<text::encoding E, template<typename>class A, typename T>
		struct _DECLSPEC_NOVTABLE imap<text::basic_string<E, A>, T>
			: ienum<pair<text::basic_string<E, A>, T>> {
			inline ANG_DECLARE_CLASS_INFO();
			inline ANG_DECLARE_RUNTIME_INFO();
			inline ANG_DECLARE_QUERY_INTERFACE();
			visible vcall bool copy(const ienum<pair<text::basic_string<E, A>, T>>*) pure
			visible vcall void extend(const ienum<pair<text::basic_string<E, A>, T>>*) pure
			visible vcall iterator<pair<text::basic_string<E, A>, T>> insert(cstr_t, T) pure
			visible vcall iterator<pair<text::basic_string<E, A>, T>> insert(pair<text::basic_string<E, A>, T>) pure
			visible vcall iterator<pair<text::basic_string<E, A>, T>> update(cstr_t, T) pure
			visible vcall iterator<pair<text::basic_string<E, A>, T>> update(pair<text::basic_string<E, A>, T>) pure
			visible vcall bool remove(cstr_t) pure
			visible vcall bool remove(cstr_t, T&) pure
			visible vcall bool remove(base_iterator<pair<text::basic_string<E, A>, T>> it) pure
			visible vcall bool remove(base_iterator<pair<text::basic_string<E, A>, T>> it, T&) pure
			visible vcall bool has_key(cstr_t)const pure
			visible vcall iterator<pair<text::basic_string<E, A>, T>> find(cstr_t) pure
			visible vcall const_iterator<pair<text::basic_string<E, A>, T>> find(cstr_t)const pure
		};

		template<typename T>
		struct _DECLSPEC_NOVTABLE imap<string, T>
			: ienum<pair<string, T>> {
			inline ANG_DECLARE_CLASS_INFO();
			inline ANG_DECLARE_RUNTIME_INFO();
			inline ANG_DECLARE_QUERY_INTERFACE();
			visible vcall bool copy(const ienum<pair<string, T>>*) pure
			visible vcall void extend(const ienum<pair<string, T>>*) pure
			visible vcall iterator<pair<string, T>> insert(cstr_t, T) pure
			visible vcall iterator<pair<string, T>> insert(pair<string, T>) pure
			visible vcall iterator<pair<string, T>> update(cstr_t, T) pure
			visible vcall iterator<pair<string, T>> update(pair<string, T>) pure
			visible vcall bool remove(cstr_t) pure
			visible vcall bool remove(cstr_t, T&) pure
			visible vcall bool remove(base_iterator<pair<string, T>> it) pure
			visible vcall bool remove(base_iterator<pair<string, T>> it, T&) pure
			visible vcall bool has_key(cstr_t)const pure
			visible vcall iterator<pair<string, T>> find(cstr_t) pure
			visible vcall const_iterator<pair<string, T>> find(cstr_t)const pure
		};

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

		template<typename T>
		struct binary_node {
			//typedef K key_t;
			typedef T type;
			//typedef collections::pair<K, T> pair_t;
			typedef binary_node<T> self_t, *self_ptr_t;

			binary_node(T key) { _data = ang::move(key); }

			inline type const& key()const {
				return _data;
			}

			inline type& value() {
				return _data;
			}
			inline type const& value()const {
				return _data;
			}
			inline void value(type val) {
				_data = ang::move(val);
			}

			inline self_ptr_t& parent() {
				return _parent;
			}
			inline self_ptr_t const& parent()const {
				return _parent;
			}
			inline void parent(self_ptr_t n) {
				_parent = ang::move(n);
				if (_parent == null)_node_type = 0;
			}

			inline self_ptr_t left()const {
				return _left;
			}
			inline void left(self_ptr_t n) {
				_left = ang::move(n);
				if (_left) {
					_left->_node_type = 1;
					_left->parent(this);
				}
			}

			inline self_ptr_t right()const {
				return _right;
			}
			inline void right(self_ptr_t n) {
				_right = ang::move(n);
				if (_right) {
					_right->_node_type = 2;
					_right->parent(this);
				}
			}

			inline int node_type()const { return this ? _node_type : 0; }
			inline int height()const { return this ? _height : 0; }
			inline void height(int n) { _height = ang::move(n); }
			inline int balance()const { return this ? left()->height() - right()->height() : 0; }

			type _data;
			int _node_type = 0;
			int _height = 1;
			self_ptr_t _parent = null;
			self_ptr_t _left = null;
			self_ptr_t _right = null;
		};

		template<typename K, typename T>
		struct binary_node<pair<K,T>> {
			typedef K key_t;
			typedef T type, value_t;
			typedef collections::pair<K, T> pair_t;
			typedef binary_node<T> self_t, *self_ptr_t;

			binary_node(K key) { _data.key = ang::move(key); }
			binary_node(K key, T value) { _data.key = ang::move(key); _data.value = ang::move(value); }
			binary_node(pair_t pair) { _data = ang::move(pair); }

			inline K& key() {
				return _data.key;
			}
			inline K const& key()const {
				return _data.key;
			}
			inline void key(K val) {
				_data.key = ang::move(val);
			}
			inline type& value() {
				return _data.value;
			}
			inline type const& value()const {
				return _data.value;
			}
			inline void value(type val) {
				_data.value = ang::move(val);
			}

			inline self_ptr_t& parent() {
				return _parent;
			}
			inline self_ptr_t const& parent()const {
				return _parent;
			}
			inline void parent(self_ptr_t n) {
				_parent = ang::move(n);
				if (_parent == null)_node_type = 0;
			}

			inline self_ptr_t left()const {
				return _left;
			}
			inline void left(self_ptr_t n) {
				_left = ang::move(n);
				if (_left) {
					_left->_node_type = 1;
					_left->parent(this);
				}
			}

			inline self_ptr_t right()const {
				return _right;
			}
			inline void right(self_ptr_t n) {
				_right = ang::move(n);
				if (_right) {
					_right->_node_type = 2;
					_right->parent(this);
				}
			}

			inline int node_type()const { return this ? _node_type : 0; }
			inline int height()const { return this ? _height : 0; }
			inline void height(int n) { _height = ang::move(n); }
			inline int balance()const { return this ? left()->height() - right()->height() : 0; }

			pair_t _data;
			int _node_type = 0;
			int _height = 1;
			self_ptr_t _parent = null;
			self_ptr_t _left = null;
			self_ptr_t _right = null;
		};
	}


	ANG_BEGIN_INTF_WRAPPER_TEMPLATE(collections::ienum, T)
		inline collections::forward_iterator<T> begin() { return is_empty() ? collections::forward_iterator<T>() : get()->begin(); }
		inline collections::forward_iterator<T> end() { return is_empty() ? collections::forward_iterator<T>() : get()->end(); }
		inline collections::const_forward_iterator<T> begin()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->begin(); }
		inline collections::const_forward_iterator<T> end()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->end(); }

		inline collections::backward_iterator<T> rbegin() { return is_empty() ? collections::backward_iterator<T>() : get()->rbegin(); }
		inline collections::backward_iterator<T> rend() { return is_empty() ? collections::backward_iterator<T>() : get()->rend(); }
		inline collections::const_backward_iterator<T> rbegin()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rbegin(); }
		inline collections::const_backward_iterator<T> rend()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rend(); }
	ANG_END_INTF_WRAPPER();

	ANG_BEGIN_INTF_WRAPPER_TEMPLATE(collections::iarray, T)
		inline collections::forward_iterator<T> begin() { return is_empty() ? collections::forward_iterator<T>() : get()->begin(); }
		inline collections::forward_iterator<T> end() { return is_empty() ? collections::forward_iterator<T>() : get()->end(); }
		inline collections::const_forward_iterator<T> begin()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->begin(); }
		inline collections::const_forward_iterator<T> end()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->end(); }

		inline collections::backward_iterator<T> rbegin() { return is_empty() ? collections::backward_iterator<T>() : get()->rbegin(); }
		inline collections::backward_iterator<T> rend() { return is_empty() ? collections::backward_iterator<T>() : get()->rend(); }
		inline collections::const_backward_iterator<T> rbegin()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rbegin(); }
		inline collections::const_backward_iterator<T> rend()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rend(); }
	ANG_END_INTF_WRAPPER();

	ANG_BEGIN_INTF_WRAPPER_TEMPLATE(collections::ilist, T)
		inline collections::forward_iterator<T> begin() { return is_empty() ? collections::forward_iterator<T>() : get()->begin(); }
		inline collections::forward_iterator<T> end() { return is_empty() ? collections::forward_iterator<T>() : get()->end(); }
		inline collections::const_forward_iterator<T> begin()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->begin(); }
		inline collections::const_forward_iterator<T> end()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->end(); }

		inline collections::backward_iterator<T> rbegin() { return is_empty() ? collections::backward_iterator<T>() : get()->rbegin(); }
		inline collections::backward_iterator<T> rend() { return is_empty() ? collections::backward_iterator<T>() : get()->rend(); }
		inline collections::const_backward_iterator<T> rbegin()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rbegin(); }
		inline collections::const_backward_iterator<T> rend()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rend(); }
	ANG_END_INTF_WRAPPER();
}

#include <ang/collections/iterators.h>

namespace ang
{
	namespace algorithms
	{

		template<class N>
		class iteration_algorithm
		{
		public:
			typedef N* node_ptr_t;
			typedef node_ptr_t(*iteration_callback_t)(node_ptr_t, wsize&);

		public: //overrides
			iteration_callback_t begin;
			iteration_callback_t end;
			iteration_callback_t rbegin;
			iteration_callback_t rend;
			iteration_callback_t increase;
			iteration_callback_t decrease;
			//iteration_callback_t most_left;
			//iteration_callback_t most_right;

			iteration_algorithm(iteration_algorithm const& algorithm) {
				begin = algorithm.begin;
				end = algorithm.end;
				rbegin = algorithm.rbegin;
				rend = algorithm.rend;
				increase = algorithm.increase;
				decrease = algorithm.decrease;
				//most_left = algorithm.most_left;
				//most_right = algorithm.most_right;
			}

			iteration_algorithm& operator = (iteration_algorithm const& algorithm) {
				begin = algorithm.begin;
				end = algorithm.end;
				rbegin = algorithm.rbegin;
				rend = algorithm.rend;
				increase = algorithm.increase;
				decrease = algorithm.decrease;
				//most_left = algorithm.most_left;
				//most_right = algorithm.most_right;
				return*this;
			}

		protected:
			iteration_algorithm() {
				begin = null;
				end = null;
				rbegin = null;
				rend = null;
				increase = null;
				decrease = null;
				//most_left = null;
				//most_right = null;
			}
		};
	}
}

#endif//__ANG_COLLECTIONS_H__
