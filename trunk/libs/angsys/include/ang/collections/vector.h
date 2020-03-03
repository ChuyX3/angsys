#ifndef __COFFE_COLLECTIONS_H__
#elif !defined __COFFE_COLLECTIONS_VECTOR_H__
#define __COFFE_COLLECTIONS_VECTOR_H__

namespace coffe
{

	template<typename T, template<typename>class A>
	struct smart_ptr_type<vector<T, A>, false, false> {
		static constexpr coffe::smart_type smart_type = coffe::smart_type::none;
		using smart_ptr_t = smart_vector<T>;
		using type = collections::vector_buffer<T, A>;
	};

	namespace collections
	{
		/******************************************************************/
		/* template class vector_buffer :                                 */
		/*  -> implements the buffer memory of vector object              */
		/******************************************************************/
		template<typename T, template<typename> class A>
		class vector_buffer final
			: public implement<vector_buffer<T,A>
			, iid("coffe::collections::vector")
			, ivariable
			, ibuffer
			, ibuffer_view
			, ivector<T>
			, iarray<T>
			, ifinder<T>
			, ienum<T>>
			, public vector<typename auto_type<T>::type, A>
		{
		public:
			static_assert(!is_const<T>::value, "T is constant type");
			using element_type = typename auto_type<T>::type;
			using type = element_type *;
			using const_type = element_type const*;
			using ilist_t = ilist<T>;
			using iarray_t = iarray<T>;
			using ienum_type = ienum<T>;
			using base_iterator_t = collections::base_iterator<T>;
			using iterator_t = collections::iterator<T>;
			using const_iterator_t = collections::const_iterator<T>;
			using forward_iterator_t = collections::forward_iterator<T>;
			using const_forward_iterator_t = collections::const_forward_iterator<T>;
			using backward_iterator_t = collections::backward_iterator<T>;
			using const_backward_iterator_t = collections::const_backward_iterator<T>;
			using vector_t = vector<typename auto_type<T>::type, A>;

		public:
			vector_buffer();
			vector_buffer(const coffe::nullptr_t&);
			vector_buffer(wsize sz);
			vector_buffer(vector<typename auto_type<T>::type, A> && ar);
			vector_buffer(vector<typename auto_type<T>::type, A> const& ar);
			vector_buffer(vector_buffer&& ar);
			vector_buffer(const vector_buffer& ar);
			vector_buffer(const ienum_ptr<T>& store);
			template<typename T2>
			vector_buffer(coffe::initializer_list<T2> list);
			template<typename T2>
			vector_buffer(array_view<T2> const&);
			template<typename IT>
			vector_buffer(IT first, IT last);
		private:
			virtual ~vector_buffer();

		public: /*methods*/
			using vector::copy;
			using vector::extend;
			template<typename T2> 
			void copy(ienum_ptr<T2> const&);

		protected: /*overrides*/
			void dispose()override;

		private: /*ivariable overrides*/
			rtti_t const& value_type()const override;
			bool set_value(rtti_t const&, unknown_t)override;
			bool get_value(rtti_t const&, unknown_t)const override;
			variant clone()const override;

		public: /*ivariable overrides*/
			string to_string()const override;
			string to_string(text::text_format_t)const override;

		private: /*ibuffer overrides*/
			bool is_readonly()const override;
			text::encoding_t encoding()const override;
			pointer buffer_ptr() override;
			const_pointer buffer_ptr()const override;
			wsize buffer_size()const override;
			wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::binary) override;
			ibuffer_view_t map_buffer(windex, wsize) override;
			bool unmap_buffer(ibuffer_view_t&, wsize) override;
			bool can_realloc_buffer()const override;
			bool realloc_buffer(wsize) override;

		public: /*ienum overrides*/
			wsize counter()const override;
			typename auto_type<T>::return_type at(base_iterator_t const&) override;
			bool increase(base_iterator_t&)const override;
			bool increase(base_iterator_t&, int offset)const override;
			bool decrease(base_iterator_t&)const override;
			bool decrease(base_iterator_t&, int offset)const override;

			forward_iterator_t begin() override;
			forward_iterator_t end() override;
			const_forward_iterator_t begin()const override;
			const_forward_iterator_t end()const override;
			forward_iterator_t last() override;
			const_forward_iterator_t last()const override;

			backward_iterator_t rbegin() override;
			backward_iterator_t rend() override;
			const_backward_iterator_t rbegin()const override;
			const_backward_iterator_t rend()const override;

			iterator_t find(function<bool(typename auto_type<T>::arg_type const&)>, bool invert = false)const override;
			iterator_t find(function<bool(typename auto_type<T>::arg_type const&)>, base_iterator_t next_to, bool invert = false)const override;
			ienum_ptr<T> find_all(function<bool(typename auto_type<T>::arg_type const&)>)const override;

		public: /*iarray overrides*/
			iterator_t at(windex) override;
			const_iterator_t at(windex)const override;
			void copy(const ienum_ptr<T>&) override;		

		public: /*ilist overrides*/
			void extend(const ienum_ptr<T>&) override;
			void push_front(typename auto_type<T>::arg_type &&) override;
			void push_front(typename auto_type<T>::arg_type const&) override;
			void push_back(typename auto_type<T>::arg_type &&) override;
			void push_back(typename auto_type<T>::arg_type const&) override;
			iterator_t insert(windex idx, typename auto_type<T>::arg_type &&) override;
			iterator_t insert(windex idx, typename auto_type<T>::arg_type const&) override;
			iterator_t insert(base_iterator_t it, typename auto_type<T>::arg_type &&) override;
			iterator_t insert(base_iterator_t it, typename auto_type<T>::arg_type const&) override;
			bool pop_front(typename auto_type<T>::arg_ptr_type = null) override;
			bool pop_back(typename auto_type<T>::arg_ptr_type = null) override;
			iterator_t remove(windex idx, typename auto_type<T>::arg_ptr_type = null) override;
			iterator_t remove(base_iterator_t it, typename auto_type<T>::arg_ptr_type = null) override;
		};

	}//collections


	/******************************************************************/
	/* template class coffe::object_wrapper<vector_buffer> :          */
	/*  -> specialization of object_wrapper<vector_buffer> -> vector  */
	/******************************************************************/
	template<typename T, template<typename> class A>
	class object_wrapper<collections::vector_buffer<T, A>>
	{
	public:
		typedef collections::vector_buffer<T, A> type;
		typedef typename collections::vector_buffer<T, A>::type data_type;
		typedef typename collections::vector_buffer<T, A>::element_type element_type;

	private:
		collections::vector_buffer<T, A>* m_ptr;

	public:
		object_wrapper();
		object_wrapper(type* ar);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(const coffe::nullptr_t&);
		object_wrapper(data_type arr, wsize sz);
		object_wrapper(wsize sz);
		object_wrapper(vector<T, A> && ar);
		object_wrapper(vector<T, A> const& ar);
		object_wrapper(const collections::ienum_ptr<T>& store);

		template<typename T2>
		object_wrapper(coffe::initializer_list<T2> list)
			: m_ptr(null) {
			set(new type(coffe::forward<coffe::initializer_list<T2>>(list)));
		}
		template<typename T2>
		object_wrapper(array_view<T2> const& ar)
			: m_ptr(null) {
			set(new type(ar));
		}
		template<typename T2, wsize N>
		object_wrapper(T2(&ar)[N])
			: m_ptr(null) {
			set(new type(to_array(ar, N)));
		}
		~object_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		collections::vector_buffer<T, A>* get(void)const;
		void set(collections::vector_buffer<T, A>*);
		collections::vector_buffer<T, A>** addres_of(void);

		collections::forward_iterator<T> begin() { return m_ptr ? m_ptr->begin() : collections::iterator<T>(); }
		collections::forward_iterator<T> end() { return m_ptr ? m_ptr->end() : collections::iterator<T>(); }
		collections::forward_iterator<const T> begin()const { return m_ptr ? m_ptr->begin() : collections::iterator<const T>(); }
		collections::forward_iterator<const T> end()const { return m_ptr ? m_ptr->end() : collections::iterator<const T>(); }

	public:
		object_wrapper& operator = (collections::vector_buffer<T, A>*);
		object_wrapper& operator = (const coffe::nullptr_t&);

		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		template<wsize N>
		object_wrapper& operator = (typename collections::auto_type<T>::type(&ar)[N]) {
			if (is_empty())
				set(new collections::vector_buffer<T, A>(collections::to_array(ar, N)));
			else
				get()->copy(to_array(ar, N));
			return *this;
		}
		template<typename T2>
		object_wrapper& operator = (collections::ienum_ptr<T2> const items) {
			if (is_empty())
				set(new collections::vector_buffer<T, A>());
			get()->copy(items);
			return *this;
		}
		object_wrapper& operator += (typename collections::auto_type<T>::arg_type && item) {
			if (is_empty())
				set(new collections::vector_buffer<T, A>());
			get()->push_back(coffe::forward<typename collections::auto_type<T>::arg_type>(item));
			return *this;
		}
		object_wrapper& operator += (typename collections::auto_type<T>::arg_type const& item) {
			if (is_empty())
				set(new collections::vector_buffer<T, A>());
			get()->push_back(item);
			return *this;
		}
		object_wrapper_ptr<collections::vector_buffer<T, A>> operator & (void);
		collections::vector_buffer<T, A> * operator -> (void);
		collections::vector_buffer<T, A> const* operator -> (void)const;
		explicit operator collections::vector_buffer<T, A> * (void);
		explicit operator collections::vector_buffer<T, A> const* (void)const;
		operator array_view<typename collections::auto_type<T>::type>()const { return m_ptr ? to_array(m_ptr->data(), m_ptr->size()) : array_view<typename collections::auto_type<T>::type>(); }
		operator array_view<const typename collections::auto_type<T>::type>()const { return m_ptr ? to_array((typename collections::auto_type<T>::type const*)m_ptr->data(), m_ptr->size()) : array_view<typename collections::auto_type<T>::type>(); }
		template<typename I> typename collections::auto_type<T>::type& operator[](I const& idx);
		template<typename I> typename collections::auto_type<T>::type const& operator[](I const& idx)const;
	};


	template<typename T, template<typename>class A>
	struct property_helper<smart_vector<T, A>, smart_type::none> {
		using type = smart_vector<T, A>;
		using ret_type = type & ;
		using ptr_type = collections::vector_buffer<T, A>*;
		using arg_type = array_view<T>;
		using property_class = base_property;
		using getter_type = ret_type(*)(property_class const*);
		using setter_type = void(*)(property_class const*, arg_type);
	};

	template<typename T, template<typename>class A>
	struct property_helper<const smart_vector<T, A>, smart_type::none> {
		using type = smart_vector<T, A>;
		using ret_type = array_view<const T>;
		using ptr_type = collections::vector_buffer<T, A>*;
		using arg_type = array_view<T>;
		using property_class = base_property;
		using getter_type = ret_type(*)(property_class const*);
		using setter_type = void(*)(property_class const*, arg_type);
	};

}//coffe

#endif //__COFFE_COLLECTIONS_VECTOR_H__
