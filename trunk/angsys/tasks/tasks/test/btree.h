#pragma once
#include <angsys.h>

namespace ang
{
	namespace collections
	{
		ang_object(set);
		template<typename T, wsize SIZE = 2> struct btree_node;
		template<typename T, wsize SIZE = 2> using btree_node_ptr = btree_node<T, SIZE>*;

	}

	namespace algorithms
	{
		template<typename T, wsize SIZE = 2>
		class btree_inorder_iteration : public iteration_algorithm<collections::btree_node<T, SIZE>>
		{
		public:
			btree_inorder_iteration() {
				iteration_algorithm<collections::btree_node<T, SIZE>>::begin = &btree_inorder_iteration::begin;
				iteration_algorithm<collections::btree_node<T, SIZE>>::end = &btree_inorder_iteration::end;
				iteration_algorithm<collections::btree_node<T, SIZE>>::rbegin = &btree_inorder_iteration::begin;
				iteration_algorithm<collections::btree_node<T, SIZE>>::rend = &btree_inorder_iteration::end;
				iteration_algorithm<collections::btree_node<T, SIZE>>::increase = &btree_inorder_iteration::increase;
				iteration_algorithm<collections::btree_node<T, SIZE>>::decrease = &btree_inorder_iteration::decrease;
			}

			inline static collections::btree_node_ptr<T, SIZE> begin(collections::btree_node_ptr<T, SIZE>, wsize&);
			inline static collections::btree_node_ptr<T, SIZE> end(collections::btree_node_ptr<T, SIZE>, wsize&);
			inline static collections::btree_node_ptr<T, SIZE> rbegin(collections::btree_node_ptr<T, SIZE>, wsize&);
			inline static collections::btree_node_ptr<T, SIZE> rend(collections::btree_node_ptr<T, SIZE>, wsize&);
			inline static collections::btree_node_ptr<T, SIZE> increase(collections::btree_node_ptr<T, SIZE>, wsize&);
			inline static collections::btree_node_ptr<T, SIZE> decrease(collections::btree_node_ptr<T, SIZE>, wsize&);
			inline static collections::btree_node_ptr<T, SIZE> most_left(collections::btree_node_ptr<T, SIZE>, wsize&);
			inline static collections::btree_node_ptr<T, SIZE> most_right(collections::btree_node_ptr<T, SIZE>, wsize&);
		};
	}
	namespace collections
	{

		template<typename T, wsize SIZE>
		struct btree_node
		{
			typedef T type;
			btree_node()
				: parent(null)
				, index(0)
				, count(0)
				, is_leaf(true) {
				for (type& value : values) {
					value = 0;
				}
				for (btree_node_ptr<T, SIZE>& child : children) {
					child = null;
				}
			}

			btree_node_ptr<T, SIZE> parent;
			wsize index;
			wsize count;
			stack_array<type, SIZE * 2 - 1> values;
			stack_array<btree_node_ptr<T, SIZE>, SIZE * 2> children;
			bool is_leaf;
		};


		class map
			: public object
			, collections::imap<ushort, string>
		{
			static const wsize SIZE = 2;

			template<typename T> using allocator = memory::default_allocator<T>;



			typedef map self_t;
			typedef ushort key;
			typedef string type;
			typedef pair<key, type> pair_t;
			typedef btree_node<pair_t, SIZE> node_t;
			typedef btree_node_ptr<pair_t, SIZE> node_ptr, node_ptr_t;
			typedef imap<key, type> imap_t;
			typedef ienum<pair_t> ienum_t;
			typedef iterator<pair_t> iterator_t;
			typedef base_iterator<pair_t> base_iterator_t;
			typedef const_iterator<pair_t> const_iterator_t;
			typedef forward_iterator<pair_t> forward_iterator_t;
			typedef backward_iterator<pair_t> backward_iterator_t;
			typedef const_forward_iterator<pair_t> const_forward_iterator_t;
			typedef const_backward_iterator<pair_t> const_backward_iterator_t;



		private:
			wsize m_count;
			node_ptr_t m_root;
			allocator<node_t> m_alloc;
			algorithms::iteration_algorithm<node_t> m_iterator;

		public:
			map();
			virtual~map();
			inline void clear();
			inline void dispose()override;
			inline wsize counter()const override;
			inline pair_t& at(base_iterator_t const& it) override;
			inline bool increase(base_iterator_t& it)const override;
			inline bool increase(base_iterator_t& it, int offset)const override;
			inline bool decrease(base_iterator_t& it)const override;
			inline bool decrease(base_iterator_t& it, int offset)const override;

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

			inline bool copy(const ienum_t*) override { return false; }
			inline void extend(const ienum_t*) override {}
			inline iterator_t insert(key, type) override { return iterator_t(); }
			inline iterator_t insert(pair<key, type>) override { return iterator_t(); }
			inline iterator_t update(key, type) override { return iterator_t(); }
			inline iterator_t update(pair<key, type>) override { return iterator_t(); }
			inline bool remove(key const&) override { return false; }
			inline bool remove(key const&, type&) override { return false; }
			inline bool remove(base_iterator_t it) override { return false; }
			inline bool remove(base_iterator_t it, type&) override { return false; }
			inline bool has_key(const key&)const override { return false; }
			inline iterator_t find(const key&) override { return iterator_t(); }
			inline const_iterator_t find(const key&)const override { return iterator_t(); }

		private:
			node_ptr_t find_node(node_ptr_t, key);
			void clear_recursive(node_ptr_t node);
		};
	}
}

///////////////////////////////////////////////////////////////////////////////////////

template<typename T, wsize SIZE>
inline ang::collections::btree_node_ptr<T, SIZE> ang::algorithms::btree_inorder_iteration<T, SIZE>::begin(ang::collections::btree_node_ptr<T, SIZE> node, wsize& i)
{
	return most_left(node, i);
}

template<typename T, wsize SIZE>
inline ang::collections::btree_node_ptr<T, SIZE> ang::algorithms::btree_inorder_iteration<T, SIZE>::end(ang::collections::btree_node_ptr<T, SIZE> node, wsize& i)
{
	i = 0;
	return null;
}

template<typename T, wsize SIZE>
inline ang::collections::btree_node_ptr<T, SIZE> ang::algorithms::btree_inorder_iteration<T, SIZE>::rbegin(ang::collections::btree_node_ptr<T, SIZE> node, wsize& i)
{
	return return most_right(node, i);
}

template<typename T, wsize SIZE>
inline ang::collections::btree_node_ptr<T, SIZE> ang::algorithms::btree_inorder_iteration<T, SIZE>::rend(ang::collections::btree_node_ptr<T, SIZE> node, wsize& i)
{
	i = 0;
	return null;
}

template<typename T, wsize SIZE>
inline ang::collections::btree_node_ptr<T, SIZE> ang::algorithms::btree_inorder_iteration<T, SIZE>::increase(ang::collections::btree_node_ptr<T, SIZE> node, wsize& i)
{
	return null;


	if (node == null)
		return null;
	if (node->is_leaf) {

		if (++i == node->count) do {
			i = node->index + 1;
			node = node->parent;
		} while (node && i == node->count);
		return node;
	}
	else {
		return most_left(node->children[i + 1], i);
	}
}

template<typename T, wsize SIZE>
inline ang::collections::btree_node_ptr<T, SIZE> ang::algorithms::btree_inorder_iteration<T, SIZE>::decrease(ang::collections::btree_node_ptr<T, SIZE> node, wsize& i)
{
	return null;
}

template<typename T, wsize SIZE>
inline ang::collections::btree_node_ptr<T, SIZE> ang::algorithms::btree_inorder_iteration<T, SIZE>::most_left(ang::collections::btree_node_ptr<T, SIZE> node, wsize& i)
{
	while (node && !node->is_leaf) {
		node = node->children[0];
	}
	i = 0;
	return node;
}

template<typename T, wsize SIZE>
inline ang::collections::btree_node_ptr<T, SIZE> ang::algorithms::btree_inorder_iteration<T, SIZE>::most_right(ang::collections::btree_node_ptr<T, SIZE> node, wsize& i)
{
	while (node && !node->is_leaf) {
		node = node->children[node->count()];
	}
	i = node->count() - 1;
	return node;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ang::collections::map::map()
	: m_count(0)
	, m_root(null)
	, m_iterator(algorithms::btree_inorder_iteration<pair_t, SIZE>()) {

}

ang::collections::map::~map() {

}


inline void ang::collections::map::dispose()
{
	clear();
}

inline void ang::collections::map::clear()
{
	clear_recursive(m_root);
	m_root = null;
}

inline wsize ang::collections::map::counter()const
{
	return m_count;
}

inline ang::collections::pair<ushort, string>& ang::collections::map::at(base_iterator_t const& it)
{
	if (it.current() != null || it.parent() != this)
		throw_exception(except_code::invalid_param);
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	return node->values[it.offset()];
}

bool ang::collections::map::increase(ang::collections::base_iterator<ang::collections::pair<ushort, string>>& it)const
{
	if (it.current() != null || it.parent() != this)
		throw_exception(except_code::invalid_param);
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	wsize idx = it.offset();
	it.current(m_iterator.increase(node, idx));
	it.offset(idx);
	return it.current() != null;
}

inline bool ang::collections::map::increase(ang::collections::base_iterator<ang::collections::pair<ushort, string>>& it, int offset)const
{
	if (it.current() != null || it.parent() != this)
		throw_exception(except_code::invalid_param);
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	wsize idx = it.offset();
	while (node != null && offset)
		node = m_iterator.increase(node, idx);
	it.current(node);
	it.offset(idx);
	return it.current() != null;
}
inline bool ang::collections::map::decrease(ang::collections::base_iterator<ang::collections::pair<ushort, string>>& it)const
{
	if (it.current() != null || it.parent() != this)
		throw_exception(except_code::invalid_param);
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	wsize idx = it.offset();
	it.current(m_iterator.decrease(node, idx));
	it.offset(idx);
	return it.current() != null;
}

inline bool ang::collections::map::decrease(ang::collections::base_iterator<ang::collections::pair<ushort, string>>& it, int offset)const
{
	if (it.current() != null || it.parent() != this)
		throw_exception(except_code::invalid_param);
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	wsize idx = it.offset();
	while (node != null && offset)
		node = m_iterator.decrease(node, idx);
	it.current(node);
	it.offset(idx);
	return it.current() != null;
}

inline ang::collections::forward_iterator<ang::collections::pair<ushort, string>> ang::collections::map::begin()
{
	wsize idx = 0;
	return forward_iterator_t(const_cast<self_t*>(this), m_iterator.begin(m_root, idx), idx);
}
inline ang::collections::forward_iterator<ang::collections::pair<ushort, string>> ang::collections::map::end()
{
	wsize idx = 0;
	return forward_iterator_t(const_cast<self_t*>(this), m_iterator.end(m_root, idx), idx);
}
inline ang::collections::const_forward_iterator<ang::collections::pair<ushort, string>> ang::collections::map::begin()const
{
	wsize idx = 0;
	return const_forward_iterator_t(const_cast<self_t*>(this), m_iterator.begin(m_root, idx), idx);
}
inline ang::collections::const_forward_iterator<ang::collections::pair<ushort, string>> ang::collections::map::end()const
{
	wsize idx = 0;
	return const_forward_iterator_t(const_cast<self_t*>(this), m_iterator.begin(m_root, idx), idx);
}
inline ang::collections::forward_iterator<ang::collections::pair<ushort, string>> ang::collections::map::last()
{
	wsize idx = 0;
	return forward_iterator_t(const_cast<self_t*>(this), m_iterator.rbegin(m_root, idx), idx);
}
inline ang::collections::const_forward_iterator<ang::collections::pair<ushort, string>> ang::collections::map::last()const
{
	wsize idx = 0;
	return const_forward_iterator_t(const_cast<self_t*>(this), m_iterator.rbegin(m_root, idx), idx);
}

inline ang::collections::backward_iterator<ang::collections::pair<ushort, string>> ang::collections::map::rbegin()
{
	wsize idx = 0;
	return backward_iterator_t(const_cast<self_t*>(this), m_iterator.rbegin(m_root, idx), idx);
}
inline ang::collections::backward_iterator<ang::collections::pair<ushort, string>> ang::collections::map::rend()
{
	wsize idx = 0;
	return backward_iterator_t(const_cast<self_t*>(this), m_iterator.rend(m_root, idx), idx);
}
inline ang::collections::const_backward_iterator<ang::collections::pair<ushort, string>> ang::collections::map::rbegin()const
{
	wsize idx = 0;
	return const_backward_iterator_t(const_cast<self_t*>(this), m_iterator.rbegin(m_root, idx), idx);
}

inline ang::collections::const_backward_iterator<ang::collections::pair<ushort, string>> ang::collections::map::rend()const
{
	wsize idx = 0;
	return const_backward_iterator_t(const_cast<self_t*>(this), m_iterator.rend(m_root, idx), idx);
}


inline ang::collections::btree_node_ptr<ang::collections::pair<ushort, ang::string>> ang::collections::map::find_node(ang::collections::btree_node_ptr<ang::collections::pair<ushort, ang::string>> node, ushort key)
{
	if (node == null)
		return null;

}

inline void ang::collections::map::clear_recursive(ang::collections::btree_node_ptr<ang::collections::pair<ushort, ang::string>> node)
{
	if (node == null)
		return;
	for (node_ptr_t child : node->children)
		clear_recursive(child);
	m_alloc.template destroy<node_t>(node);
	m_alloc.deallocate(node);
}