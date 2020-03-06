#pragma once
namespace coffe
{
	namespace algorithms
	{
		struct xml_iteration
			: iteration_algorithm<dom::xml::xml_node>
		{
			using type = dom::xml::xml_node;
			using node_ptr_type = dom::xml::xml_node*;
			using iteration_callback_type = typename iteration_algorithm<dom::xml::xml_node>::iteration_callback_type;

			xml_iteration() {
				vtable = &s_vtable[0];
			}

		private:
			static node_ptr_type begin(node_ptr_type node, windex* idx);
			static node_ptr_type end(node_ptr_type node, windex* idx);
			static node_ptr_type rbegin(node_ptr_type node, windex* idx);
			static node_ptr_type rend(node_ptr_type node, windex* idx);
			static node_ptr_type increase(node_ptr_type node, windex* idx);
			static node_ptr_type decrease(node_ptr_type node, windex* idx);
			static node_ptr_type most_left(node_ptr_type node, windex* idx = null);
			static node_ptr_type most_right(node_ptr_type node, windex* idx = null);

			static constexpr iteration_callback_type s_vtable[] = {
				&begin,
				&end,
				&rbegin,
				&rend,
				&increase,
				&decrease,
				&most_left,
				&most_right
			};
		};

		template<>
		struct node_value<dom::xml::xml_node_t, dom::xml::xml_node> {
			using type = dom::xml::xml_node_t;
			using type_ptr = dom::xml::xml_node*;

			static dom::xml::xml_node_t get(dom::xml::xml_node* node, windex) {
				return node;
			}
			static type_ptr get_ptr(dom::xml::xml_node* node, windex) {
				return node;
			}
		};
	}

	namespace collections
	{
		template<>
		struct list<dom::xml::xml_node>
		{
			using element_type = dom::xml::xml_node_t;
			using element_type_ptr = dom::xml::xml_node_ptr_t;
			using node_type = dom::xml::xml_node;
			using node_type_ptr = dom::xml::xml_node*;
			using base_iterator_t = algorithms::base_iterator<node_type>;
			using iterator_t = algorithms::forward_iterator<element_type, node_type>;
			using const_iterator_t = algorithms::forward_iterator<element_type, node_type>;
			using reverse_iterator_t = algorithms::backward_iterator<element_type, node_type>;
			using reverse_const_iterator_t = algorithms::backward_iterator<element_type, node_type>;
			using iteration_type = algorithms::xml_iteration;

		protected:
			dom::xml::xml_node_t m_first;
			dom::xml::xml_node_t m_last;
			wsize m_size;

		public:
			list();
			list(coffe::nullptr_t const&);
			list(list const& other);
			list(list && other);
			~list();

		public: /*properties*/
			bool is_empty()const;
			wsize size()const;
			dom::xml::xml_node_t front()const;
			dom::xml::xml_node_t back()const;

		public: /*iterators*/
			const_iterator_t begin(algorithms::iteration_algorithm<node_type> iter = iteration_type())const;
			const_iterator_t end(algorithms::iteration_algorithm<node_type> iter = iteration_type())const;
			reverse_const_iterator_t rbegin(algorithms::iteration_algorithm<node_type> = iteration_type())const;
			reverse_const_iterator_t rend(algorithms::iteration_algorithm<node_type> iter = iteration_type())const;

		public: /*utilities*/
			void clear();
			void push_back(dom::xml::xml_node_t value);
			void push_front(dom::xml::xml_node_t value);
			bool pop_front(dom::xml::xml_node_ptr_t out = null);
			bool pop_back(dom::xml::xml_node_ptr_t out = null);
			base_iterator_t& insert(base_iterator_t& at, dom::xml::xml_node_t value);
			bool remove(base_iterator_t& at, dom::xml::xml_node_ptr_t out = null);

			void move(list& items);		
			void copy(list<dom::xml::xml_node> const& items);
			void exten(list<dom::xml::xml_node> const& items);

		public: /*operators*/
			list& operator = (list const& val);
			list& operator = (list && val);
			list& operator += (dom::xml::xml_node_t val);
		};
	}

	namespace dom
	{
		namespace xml
		{
			using xml_list = list<dom::xml::xml_node>;
		}
	}
}
