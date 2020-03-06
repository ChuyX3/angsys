#include "pch.h"
#include "xml/xml_impl.h"

namespace coffe
{
	namespace algorithms
	{
		xml_iteration::node_ptr_type xml_iteration::begin(node_ptr_type node, windex* idx)
		{
			return most_left(node, idx);
		}

		xml_iteration::node_ptr_type xml_iteration::end(node_ptr_type node, windex* idx)
		{
			if (idx)*idx = 0;
			return null;
		}

		xml_iteration::node_ptr_type xml_iteration::rbegin(node_ptr_type node, windex* idx)
		{
			return most_right(node, idx);
		}

		xml_iteration::node_ptr_type xml_iteration::rend(node_ptr_type node, windex* idx)
		{
			if (idx)*idx = 0;
			return null;
		}

		xml_iteration::node_ptr_type xml_iteration::increase(node_ptr_type node, windex* idx)
		{
			return node ? node->next_sibling().get() : null;
		}

		xml_iteration::node_ptr_type xml_iteration::decrease(node_ptr_type node, windex* idx)
		{
			return node ? node->prev_sibling().get() : null;
		}

		xml_iteration::node_ptr_type xml_iteration::most_left(node_ptr_type node, windex* idx)
		{
			if (idx)*idx = 0;
			if (node == null)
				return null;
			auto parent = node->to_intf<dom::xml::xml_node_base>()->parent();
			return parent ? parent->first_child().get() : node;
		}

		xml_iteration::node_ptr_type xml_iteration::most_right(node_ptr_type node, windex* idx)
		{
			if (idx)*idx = 0;
			if (node == null)
				return null;
			auto parent = node->to_intf<dom::xml::xml_node_base>()->parent();
			return parent ? parent->last_child().get() : node;
		}
	}

	namespace collections
	{
		list<dom::xml::xml_node>::list()
			: m_first(null)
			, m_last(null)
			, m_size(0)
		{
		}
		list<dom::xml::xml_node>::list(coffe::nullptr_t const&)
			: list()
		{
		}

		list<dom::xml::xml_node>::list(list const& other)
			: list()
		{
			for (auto node : other) {
				push_back(node);
			}
		}

		list<dom::xml::xml_node>::list(list && other)
			: list()
		{
			m_first = other.m_first;
			m_last = other.m_last;
			m_size = other.m_size;
			other.m_first = null;
			other.m_last = null;
			other.m_size = 0;
		}

		list<dom::xml::xml_node>::~list()
		{
			clear();
		}

		bool list<dom::xml::xml_node>::is_empty()const
		{
			return m_size == 0;
		}

		wsize list<dom::xml::xml_node>::size()const
		{
			return m_size;
		}

		dom::xml::xml_node_t list<dom::xml::xml_node>::front()const
		{
			return m_first;
		}

		dom::xml::xml_node_t list<dom::xml::xml_node>::back()const
		{
			return m_last ;
		}

		list<dom::xml::xml_node>::const_iterator_t list<dom::xml::xml_node>::begin(algorithms::iteration_algorithm<node_type> iter)const
		{
			return const_iterator_t(iter, m_first.get(), 0);
		}

		list<dom::xml::xml_node>::const_iterator_t list<dom::xml::xml_node>::end(algorithms::iteration_algorithm<node_type> iter)const
		{
			return const_iterator_t(iter, null, 0);
		}

		list<dom::xml::xml_node>::reverse_const_iterator_t list<dom::xml::xml_node>::rbegin(algorithms::iteration_algorithm<node_type> iter)const
		{
			return const_iterator_t(iter, m_last.get(), 0);
		}

		list<dom::xml::xml_node>::reverse_const_iterator_t list<dom::xml::xml_node>::rend(algorithms::iteration_algorithm<node_type> iter)const
		{
			return const_iterator_t(iter, null, 0);
		}

		void list<dom::xml::xml_node>::clear()
		{
			dom::xml::xml_node_t to_del, node = m_first;
			while (node != null)
			{
				to_del = node;
				node = node->next_sibling();
				to_del->dispose();
				to_del = null;
			}
			m_first = null;
			m_last = null;
			m_size = 0;
		}

		void list<dom::xml::xml_node>::push_back(dom::xml::xml_node_t node)
		{
			if (node.is_empty())
				return;
			if (m_first == null) {
				m_first = m_last = node;
			}
			else {
				m_last->next_sibling(node);	
				node->prev_sibling(m_last);
				m_last = node;
			}
			m_size++;
		}

		void list<dom::xml::xml_node>::push_front(dom::xml::xml_node_t node)
		{
			if (node.is_empty())
				return;
			if (m_first == null) {
				m_first = m_last = node;
			}
			else {
				m_first->prev_sibling(node);
				node->next_sibling(m_first);
				m_first = node;
			}
			m_size++;
		}

		bool list<dom::xml::xml_node>::pop_front(dom::xml::xml_node_ptr_t out)
		{
			if (m_first == null)
				return false;
			if (m_first == m_last) {
				if (!out.is_empty())
					*out = coffe::move(m_first);
				m_first = m_last = null;
			}
			else {
				node_type_ptr node = m_first;
				m_first = m_first->next_sibling();
				m_first->prev_sibling(null);
				node->next_sibling(null);
				if (!out.is_empty())
					*out = coffe::move(node);
				node = null;
			}
			m_size--;
			return true;
		}

		bool list<dom::xml::xml_node>::pop_back(dom::xml::xml_node_ptr_t out)
		{
			if (m_first == null)
				return false;
			if (m_first == m_last) {
				if (!out.is_empty())
					*out = coffe::move(m_first);
				m_first = m_last = null;
			}
			else {
				node_type_ptr node = m_last;
				m_last = m_last->prev_sibling();
				m_last->next_sibling(null);
				node->prev_sibling(null);
				if (!out.is_empty())
					*out = coffe::move(node);
				node = null;
			}
			m_size--;
			return true;
		}

		list<dom::xml::xml_node>::base_iterator_t& list<dom::xml::xml_node>::insert(base_iterator_t& at, dom::xml::xml_node_t node)
		{
			if (node.is_empty())
			{
				at = end();
			}		
			else if (at.get_node() == m_first) {
				push_front(coffe::forward<element_type>(node));
				at = base_iterator_t(iteration_type(), m_first.get(), 0);
			}
			else if (at.get_node() == null) {
				push_back(coffe::forward<element_type>(node));
				at = base_iterator_t(iteration_type(), m_last.get(), 0);
			}
			else {
				node->prev_sibling(at.get_node()->prev_sibling());
				at.get_node()->prev_sibling()->next_sibling(node);
				node->next_sibling(at.get_node());
				at.get_node()->prev_sibling(node);
				at = base_iterator_t(iteration_type(), node.get(), 0);
				m_size++;
			}
			at = base_iterator_t(at.algorithm(), node.get(), 0);
			return at;
		}

		bool list<dom::xml::xml_node>::remove(base_iterator_t& at, dom::xml::xml_node_ptr_t out)
		{
			if (at.get_node() == null)
				return false;

			else if (at.get_node() == m_first) {
				if (!pop_front(out))
					return false;
				at = base_iterator_t(iteration_type(), m_first.get(), 0);
			}
			else if (at.get_node() == m_last) {
				if (!pop_back(out))
					return false;
				at = base_iterator_t(iteration_type(), null, 0);
			}
			else {
				node_type_ptr node = at.get_node();
				at = base_iterator_t(iteration_type(), node->next_sibling().get(), 0);
				node->prev_sibling()->next_sibling(node->next_sibling());
				node->next_sibling()->prev_sibling(node->prev_sibling());
				node->next_sibling(null);
				node->prev_sibling(null);
				if (!out.is_empty())
					*out = coffe::move(node);
				node = null;
				m_size--;
			}
			return true;
		}

		void list<dom::xml::xml_node>::move(list& list)
		{
			if (&list == this)
				return;
			clear();
			m_size = list.m_size;
			m_first = list.m_first;
			m_last = list.m_last;
			list.m_size = 0;
			list.m_first = null;
			list.m_last = null;
		}

		void list<dom::xml::xml_node>::copy(list const& list)
		{
			clear();
			for (auto node : list) {
				push_back(node);
			}
		}

		list<dom::xml::xml_node>& list<dom::xml::xml_node>::operator = (list const& val)
		{
			copy(val);
			return*this;
		}

		list<dom::xml::xml_node>& list<dom::xml::xml_node>::operator = (list && val)
		{
			move(val);
			return*this;
		}

		list<dom::xml::xml_node>& list<dom::xml::xml_node>::operator += (dom::xml::xml_node_t val)
		{
			push_back(forward<element_type>(val));
			return*this;
		}

	}
}
