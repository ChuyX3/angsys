#ifndef __COFFE_H__
#elif !defined __COFFE_COLLECTIONS_H__
#define __COFFE_COLLECTIONS_H__

namespace coffe
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

		template<typename T, smart_type TYPE = smart_ptr_type<T>::smart_type>
		struct auto_type {
			using base_type = typename smart_ptr_type<T>::type;
			using type = typename smart_ptr_type<T>::smart_ptr_t;
			using arg_type = type;
			using arg_ptr_type = typename smart_ptr_type<T>::smart_ptr_ptr_t;
			using ptr_type = base_type *;
			using const_ptr_type = base_type const*;
			using return_type = type;
			using ref_type = type;
			using const_ref_type = type const;
		};

		template<typename T>
		struct auto_type<T, smart_type::none> {
			using type = typename remove_const<typename remove_reference<T>::type>::type;
			using arg_type = type;
			using arg_ptr_type = type*;
			using ptr_type = type *;
			using const_ptr_type = type const*;
			using return_type = type*;
			using ref_type = type&;
			using const_ref_type = type const&;
		};

		template<class N> class iteration_algorithm;

		template<typename T, template<typename> class A = memory::auto_detect_buffer_allocator> class array_buffer;
		template<typename T, template<typename> class A = memory::auto_detect_buffer_allocator> class vector_buffer;
		template<typename T, template<typename> class A = memory::auto_detect_buffer_allocator> using smart_array = object_wrapper<array_buffer<T, A>>;
		template<typename T, template<typename> class A = memory::auto_detect_buffer_allocator> using smart_vector = object_wrapper<vector_buffer<T, A>>;
		template<typename T, template<typename> class A = memory::auto_detect_buffer_allocator> using smart_array_ptr = object_wrapper_ptr<array_buffer<T, A>>;
		template<typename T, template<typename> class A = memory::auto_detect_buffer_allocator> using smart_vector_ptr = object_wrapper_ptr<vector_buffer<T, A>>;

		template<typename T> struct ienum;
		template<typename T> struct iarray;
		template<typename T> struct ilist;
		template<typename T, typename K = function<bool(typename auto_type<T>::arg_type const&)>> struct ifinder;

		template<typename T> using ienum_t = intf_wrapper<ienum<T>>;
		template<typename T> using ienum_ptr = intf_wrapper<ienum<T>>;

		template<typename T, typename K> using ifinder_t = intf_wrapper<ifinder<T, K>>;
		template<typename T, typename K> using ifinder_ptr = intf_wrapper<ifinder<T, K>>;

		template<typename T> using iarray_t = intf_wrapper<iarray<T>>;
		template<typename T> using iarray_ptr = intf_wrapper<iarray<T>>;

		template<typename T> using ilist_t = intf_wrapper<ilist<T>>;
		template<typename T> using ilist_ptr = intf_wrapper<ilist<T>>;

		template<typename T, typename K = function<bool(typename auto_type<T>::arg_type const&)>> using ifinder_t = intf_wrapper<ifinder<T, K>>;
		template<typename T, typename K = function<bool(typename auto_type<T>::arg_type const&)>> using ifinder_ptr = intf_wrapper<ifinder<T, K>>;

		declare_enum(LINK, iteration_method, uint)
		{
			def,
			level,
			preorder,
			inorder,
			postorder
		};

		/******************************************************************/
		/* interface coffe::collections::ienum :                          */
		/*  -> represents an enumeration of objects or variables which    */
		/*     can be accessed orderly                                    */
		/******************************************************************/
		template<typename T>
		struct nvt ienum
			: intf<ienum<T>
			, iid("coffe::collections::ienum")>
		{
			virtual wsize counter()const = 0;
			virtual typename auto_type<T>::return_type at(base_iterator<T> const&) = 0;
			virtual bool increase(base_iterator<T>&)const = 0;
			virtual bool increase(base_iterator<T>&, int offset)const = 0;
			virtual bool decrease(base_iterator<T>&)const = 0;
			virtual bool decrease(base_iterator<T>&, int offset)const = 0;

			virtual forward_iterator<T> begin() = 0;
			virtual forward_iterator<T> end() = 0;
			virtual const_forward_iterator<T> begin()const = 0;
			virtual const_forward_iterator<T> end()const = 0;
			virtual forward_iterator<T> last() = 0;
			virtual const_forward_iterator<T> last()const = 0;

			virtual backward_iterator<T> rbegin() = 0;
			virtual backward_iterator<T> rend() = 0;
			virtual const_backward_iterator<T> rbegin()const = 0;
			virtual const_backward_iterator<T> rend()const = 0;
		};

		/******************************************************************/
		/* interface coffe::collections::ienum :                          */
		/*  -> represents a search algorithm                              */
		/******************************************************************/
		template<typename T, typename K = function<bool(typename auto_type<T>::arg_type const&)>>
		struct nvt ifinder
			: intf<ifinder<T, K>
			, iid("coffe::collections::ienum")>
		{
			virtual iterator<T> find(K, bool invert = false)const = 0;
			virtual iterator<T> find(K, base_iterator<T> next_to, bool invert = false)const = 0;
			virtual ienum_ptr<T> find_all(K)const = 0;
		};

		template<typename T>
		struct nvt ienumerable
			: intf<ienumerable<T>
			, iid("coffe::collections::ienumerable")>
		{
			virtual ienum_ptr<T> enumerate(iteration_method_t = iteration_method::def)const = 0;
		};


		/******************************************************************/
		/* interface coffe::collections::iarray :						  */
		/*  -> represents a collection of objects or variables which      */
		/*     can be accessed unorderly                                  */
		/******************************************************************/
		template<typename T>
		struct nvt iarray
			: intf<iarray<T>
			, iid("coffe::collections::iarray")
			, ifinder<T>
			, ienum<T>>
		{
			virtual iterator<T> at(windex) = 0;
			virtual const_iterator<T> at(windex)const = 0;
			virtual void copy(const ienum_ptr<T>&) = 0;
		};

		/******************************************************************/
		/* interface coffe::collections::ilist :                          */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements randomly                     */
		/******************************************************************/
		template<typename T>
		struct nvt ilist
			: intf<ilist<T>
			, iid("coffe::collections::ilist")
			, ifinder<T>
			, ienum<T>>
		{
			virtual void copy(const ienum_ptr<T>&) = 0;
			virtual void extend(const ienum_ptr<T>&) = 0;
			virtual void push_front(typename auto_type<T>::arg_type &&) = 0;
			virtual void push_front(typename auto_type<T>::arg_type const&) = 0;
			virtual void push_back(typename auto_type<T>::arg_type &&) = 0;
			virtual void push_back(typename auto_type<T>::arg_type const&) = 0;
			virtual iterator<T> insert(base_iterator<T> it, typename auto_type<T>::arg_type &&) = 0;
			virtual iterator<T> insert(base_iterator<T> it, typename auto_type<T>::arg_type const&) = 0;
			virtual bool pop_front(typename auto_type<T>::arg_ptr_type = null) = 0;
			virtual bool pop_back(typename auto_type<T>::arg_ptr_type = null) = 0;
			virtual iterator<T> remove(base_iterator<T> it, typename auto_type<T>::arg_ptr_type = null) = 0;
		};

		/******************************************************************/
		/* interface coffe::collections::isequence :                      */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements sequentialy                  */
		/******************************************************************/
		template<typename T>
		struct nvt isequence_list
			: intf<isequence_list<T>
			, iid("coffe::collections::isequence_list")
			, ienum<T>>
		{
			virtual void extend(const ienum_ptr<T>&) = 0;
			virtual void push(typename auto_type<T>::arg_type &&) = 0;
			virtual void push(typename auto_type<T>::arg_type const&) = 0;
			virtual bool pop(typename auto_type<T>::arg_ptr_type = null) = 0;
		};

		/******************************************************************/
		/* interface coffe::collections::ivector :                        */
		/*  -> represents a list of objects or variables where the user   */
		/*     can insert or remove elements randomly by index            */
		/******************************************************************/
		template<typename T>
		struct nvt ivector
			: intf<ivector<T>
			, iid("coffe::collections::ivector")
			, iarray<T>>
		{
			virtual void extend(const ienum_ptr<T>&) = 0;
			virtual void push_front(typename auto_type<T>::arg_type &&) = 0;
			virtual void push_front(typename auto_type<T>::arg_type const&) = 0;
			virtual void push_back(typename auto_type<T>::arg_type &&) = 0;
			virtual void push_back(typename auto_type<T>::arg_type const&) = 0;
			virtual iterator<T> insert(windex idx, typename auto_type<T>::arg_type &&) = 0;
			virtual iterator<T> insert(windex idx, typename auto_type<T>::arg_type const&) = 0;
			virtual iterator<T> insert(base_iterator<T> it, typename auto_type<T>::arg_type &&) = 0;
			virtual iterator<T> insert(base_iterator<T> it, typename auto_type<T>::arg_type const&) = 0;
			virtual bool pop_front(typename auto_type<T>::arg_ptr_type = null) = 0;
			virtual bool pop_back(typename auto_type<T>::arg_ptr_type = null) = 0;
			virtual iterator<T> remove(windex idx, typename auto_type<T>::arg_ptr_type = null) = 0;
			virtual iterator<T> remove(base_iterator<T> it, typename auto_type<T>::arg_ptr_type = null) = 0;
		};


		/******************************************************************/
		/* interface coffe::collections::iset :							  */
		/*  -> represents a collection of objects or variables which      */
		/*     can be accessed by a key                                   */
		/******************************************************************/
		template<typename T>
		struct nvt iset
			: intf< iset<T>
			, iid("coffe::collections::iset")
			, ienumerable<T>>
		{
			virtual void copy(const ienum_ptr<T>&) = 0;
			virtual void extend(const ienum_ptr<T>&) = 0;
			virtual bool insert(typename auto_type<T>::arg_type &&) = 0;
			virtual bool insert(typename auto_type<T>::arg_type const&) = 0;
			virtual bool remove(typename auto_type<T>::arg_type&) = 0;
			virtual bool remove(base_iterator<T> it, typename auto_type<T>::arg_ptr_type = null) = 0;
			virtual bool has_value(typename auto_type<T>::arg_type const&)const = 0;
			virtual iterator<T> find(typename auto_type<T>::arg_type const&) = 0;
			virtual const_iterator<T> find(typename auto_type<T>::arg_type const&)const = 0;
		};

		/******************************************************************/
		/* interface coffe::collections::imap :							  */
		/*  -> represents a collection of objects or variables which      */
		/*     can be accessed by a key                                   */
		/******************************************************************/
		template<typename K, typename T>
		struct nvt imap
			: intf<imap<K, T>
			, iid("coffe::collections::imap")
			, ienumerable<pair<typename auto_type<K>::type const, typename auto_type<T>::type>>>
		{
			using pair_type = pair<typename auto_type<K>::type, typename auto_type<T>::type>;
			using pair_view_type = pair<typename auto_type<K>::type const, typename auto_type<T>::type>;
			virtual void copy(const ienum_ptr<pair_view_type>&) = 0;
			virtual void extend(const ienum_ptr<pair_view_type>&) = 0;
			virtual iterator<pair_view_type> insert(typename auto_type<K>::arg_type const&, typename auto_type<T>::arg_type &&) = 0;
			virtual iterator<pair_view_type> insert(typename auto_type<K>::arg_type const&, typename auto_type<T>::arg_type const&) = 0;
			virtual iterator<pair_view_type> insert(pair_type &&) = 0;
			virtual iterator<pair_view_type> insert(pair_type const&) = 0;
			virtual iterator<pair_view_type> update(typename auto_type<K>::arg_type const&, typename auto_type<T>::arg_type &&) = 0;
			virtual iterator<pair_view_type> update(typename auto_type<K>::arg_type const&, typename auto_type<T>::arg_type const&) = 0;
			virtual iterator<pair_view_type> update(pair_type &&) = 0;
			virtual iterator<pair_view_type> update(pair_type const&) = 0;
			virtual bool remove(typename auto_type<K>::arg_type const&, typename auto_type<T>::arg_ptr_type = null) = 0;
			virtual bool remove(base_iterator<pair_view_type> it, typename auto_type<T>::arg_ptr_type = null) = 0;
			virtual bool has_key(typename auto_type<K>::arg_type const&)const = 0;
			virtual iterator<pair_view_type> find(typename auto_type<K>::arg_type const&) = 0;
			virtual const_iterator<pair_view_type> find(typename auto_type<K>::arg_type const&)const = 0;
		};
	
		template<typename T>
		struct nvt imap<string, T>
			: intf<imap<string, T>
			, iid("coffe::collections::imap")
			, ienumerable<pair<const string, typename auto_type<T>::type>>>
		{
			using pair_type = pair<const string, typename auto_type<T>::type>;
			virtual void copy(const ienum<pair_type>*) = 0;
			virtual void extend(const ienum<pair_type>*) = 0;
			virtual iterator<pair_type> insert(cstr_t const&, typename auto_type<T>::arg_type &&) = 0;
			virtual iterator<pair_type> insert(cstr_t const&, typename auto_type<T>::arg_type const&) = 0;
			virtual iterator<pair_type> insert(pair_type&&) = 0;
			virtual iterator<pair_type> insert(pair_type const&) = 0;
			virtual iterator<pair_type> update(cstr_t const, typename auto_type<T>::arg_type &&) = 0;
			virtual iterator<pair_type> update(cstr_t const, typename auto_type<T>::arg_type const&) = 0;
			virtual iterator<pair_type> update(pair_type&&) = 0;
			virtual iterator<pair_type> update(pair_type const&) = 0;
			virtual bool remove(cstr_t const&, typename auto_type<T>::arg_ptr_type = null) = 0;
			virtual bool remove(base_iterator<pair_type> it, typename auto_type<T>::arg_ptr_type = null) = 0;
			virtual bool has_key(cstr_t const&)const = 0;
			virtual iterator<pair_type> find(cstr_t const&) = 0;
			virtual const_iterator<pair_type> find(cstr_t const&)const = 0;
		};

	}

	using collections::smart_array;
	using collections::smart_vector;

	COFFE_BEGIN_INTF_WRAPPER_TEMPLATE(collections::ienum, T)
		inline collections::forward_iterator<T> begin() { return is_empty() ? collections::forward_iterator<T>() : get()->begin(); }
		inline collections::forward_iterator<T> end() { return is_empty() ? collections::forward_iterator<T>() : get()->end(); }
		inline collections::const_forward_iterator<T> begin()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->begin(); }
		inline collections::const_forward_iterator<T> end()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->end(); }

		inline collections::backward_iterator<T> rbegin() { return is_empty() ? collections::backward_iterator<T>() : get()->rbegin(); }
		inline collections::backward_iterator<T> rend() { return is_empty() ? collections::backward_iterator<T>() : get()->rend(); }
		inline collections::const_backward_iterator<T> rbegin()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rbegin(); }
		inline collections::const_backward_iterator<T> rend()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rend(); }
	COFFE_END_INTF_WRAPPER();

	COFFE_BEGIN_INTF_WRAPPER_TEMPLATE(collections::iarray, T)
		inline collections::forward_iterator<T> begin() { return is_empty() ? collections::forward_iterator<T>() : get()->begin(); }
		inline collections::forward_iterator<T> end() { return is_empty() ? collections::forward_iterator<T>() : get()->end(); }
		inline collections::const_forward_iterator<T> begin()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->begin(); }
		inline collections::const_forward_iterator<T> end()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->end(); }

		inline collections::backward_iterator<T> rbegin() { return is_empty() ? collections::backward_iterator<T>() : get()->rbegin(); }
		inline collections::backward_iterator<T> rend() { return is_empty() ? collections::backward_iterator<T>() : get()->rend(); }
		inline collections::const_backward_iterator<T> rbegin()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rbegin(); }
		inline collections::const_backward_iterator<T> rend()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rend(); }
		template<typename I> T& operator [](I idx) { return get()->at(get()->at(idx)); }
		template<typename I> T const& operator [](I idx)const { return get()->at(get()->at(idx)); }
	COFFE_END_INTF_WRAPPER();

	COFFE_BEGIN_INTF_WRAPPER_TEMPLATE(collections::ilist, T)
		inline collections::forward_iterator<T> begin() { return is_empty() ? collections::forward_iterator<T>() : get()->begin(); }
		inline collections::forward_iterator<T> end() { return is_empty() ? collections::forward_iterator<T>() : get()->end(); }
		inline collections::const_forward_iterator<T> begin()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->begin(); }
		inline collections::const_forward_iterator<T> end()const { return is_empty() ? collections::const_forward_iterator<T>() : ((type const*)get())->end(); }

		inline collections::backward_iterator<T> rbegin() { return is_empty() ? collections::backward_iterator<T>() : get()->rbegin(); }
		inline collections::backward_iterator<T> rend() { return is_empty() ? collections::backward_iterator<T>() : get()->rend(); }
		inline collections::const_backward_iterator<T> rbegin()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rbegin(); }
		inline collections::const_backward_iterator<T> rend()const { return is_empty() ? collections::const_backward_iterator<T>() : ((type const*)get())->rend(); }

		template<typename I> T& operator [](I idx) { return get()->at(get()->at(idx)); }
		template<typename I> T const& operator [](I idx)const { return get()->at(get()->at(idx)); }
	COFFE_END_INTF_WRAPPER();
}

#include <coffe/collections/iterators.h>

#endif//__COFFE_COLLECTIONS_H__
