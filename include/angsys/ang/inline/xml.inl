
namespace ang
{
	namespace xml
	{
		template<> class LINK xml_value<CURRENT_ENCODING> final
			: public xml_string<CURRENT_ENCODING>
		{
		public:
			xml_value();
			xml_value(xml_string<CURRENT_ENCODING> &&);
			xml_value(xml_string<CURRENT_ENCODING> const&);
			xml_value(strings::string_buffer<CURRENT_ENCODING>*);
			xml_value(ang::nullptr_t const&);
			template<typename T> xml_value(safe_str<T> const& str);
			template<xml_encoding OTHER_ENCODING> xml_value(xml_string<OTHER_ENCODING> const& str);
			template<typename T, wsize N> xml_value(const T(&ar)[N]);

			xml_value& operator = (ang::nullptr_t const&);
			xml_value& operator = (strings::string_buffer<CURRENT_ENCODING>*);
			xml_value& operator = (xml_value<CURRENT_ENCODING> &&);
			xml_value& operator = (xml_value<CURRENT_ENCODING> const&);
			template<typename T> xml_value& operator = (safe_str<T> const& str);
			template<typename T, wsize N> xml_value& operator = (const T(&ar)[N]);
			template<xml_encoding OTHER_ENCODING> xml_value& operator = (xml_value<strings::string_buffer<OTHER_ENCODING>> const&);
			template<typename T> xml_value& operator += (safe_str<T> const& str);
			template<typename T, wsize N> xml_value& operator += (const T(&ar)[N]);
			template<xml_encoding OTHER_ENCODING> xml_value& operator += (xml_value<strings::string_buffer<OTHER_ENCODING>> const&);

			template<typename T> inline T as()const {
				throw_exception((dword)except_code::unsupported
					, "exception: unsupported type cast, user must implement the specialization of \"template<typename T> T xml_value::as()const\""_s));
				return T();
			}
			xml_string<CURRENT_ENCODING>& xml_print(xml_string<CURRENT_ENCODING>& stream, xml_format_t const& format, ushort level)const;
		};

	}

	namespace collections
	{
		template<>
		ANG_BEGIN_INTERFACE(LINK, ienum<xml::xml_node<CURRENT_ENCODING>>)
			visible vcall wsize counter()const pure;

			visible vcall xml::xml_node_ptr<CURRENT_ENCODING> at(xml::xml_base_iterator<CURRENT_ENCODING> const&) pure;
			visible vcall bool increase(xml::xml_base_iterator<CURRENT_ENCODING>&)const pure;
			visible vcall bool increase(xml::xml_base_iterator<CURRENT_ENCODING>&, int offset)const pure;
			visible vcall bool decrease(xml::xml_base_iterator<CURRENT_ENCODING>&)const pure;
			visible vcall bool decrease(xml::xml_base_iterator<CURRENT_ENCODING>&, int offset)const pure;

			visible vcall xml::xml_forward_iterator<CURRENT_ENCODING> begin() pure;
			visible vcall xml::xml_forward_iterator<CURRENT_ENCODING> end() pure;
			visible vcall xml::xml_const_forward_iterator<CURRENT_ENCODING> begin()const pure;
			visible vcall xml::xml_const_forward_iterator<CURRENT_ENCODING> end()const pure;

			visible vcall xml::xml_backward_iterator<CURRENT_ENCODING> rbegin() pure;
			visible vcall xml::xml_backward_iterator<CURRENT_ENCODING> rend() pure;
			visible vcall xml::xml_const_backward_iterator<CURRENT_ENCODING> rbegin()const pure;
			visible vcall xml::xml_const_backward_iterator<CURRENT_ENCODING> rend()const pure;
		ANG_END_INTERFACE();

		template<typename T>
		ANG_BEGIN_INLINE_INTERFACE_WITH_BASE(istore, public ienum<T>)
			using ienum<T>::at;
			visible vcall void copy(const ienum<T>*) pure;
			visible vcall iterator<T> find(const T&, bool invert = false)const pure;
			visible vcall iterator<T> find(const T&, base_iterator<T> next_to, bool invert = false)const pure;
		ANG_END_INTERFACE();
	}
}


namespace ang
{
	template<>
	class LINK object_wrapper<xml::xml_node<CURRENT_ENCODING>>
	{
	public:
		typedef xml::xml_node<CURRENT_ENCODING> type;

	private:
		xml::xml_node<CURRENT_ENCODING>* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_node<CURRENT_ENCODING>*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_node<CURRENT_ENCODING>* get(void)const;
		void set(xml::xml_node<CURRENT_ENCODING>*);
		xml::xml_node<CURRENT_ENCODING> ** addres_of(void);


	public:
		object_wrapper& operator = (xml::xml_node<CURRENT_ENCODING>*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<xml::xml_node<CURRENT_ENCODING>> operator & (void);
		xml::xml_node<CURRENT_ENCODING> * operator -> (void);
		xml::xml_node<CURRENT_ENCODING> const* operator -> (void)const;

		operator xml::xml_node<CURRENT_ENCODING> * (void);
		operator xml::xml_node<CURRENT_ENCODING> const* (void)const;
		operator xml::xml_value<CURRENT_ENCODING>()const;

		template<typename T> xml::xml_node_ptr<CURRENT_ENCODING> operator[](safe_str<T> const&)const;
	};

	template<>
	class LINK object_wrapper<xml::xml_items<CURRENT_ENCODING>>
	{
	public:
		typedef xml::xml_items<CURRENT_ENCODING> type;

	private:
		xml::xml_items<CURRENT_ENCODING>* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_items<CURRENT_ENCODING>*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_items<CURRENT_ENCODING>* get(void)const;
		void set(xml::xml_items<CURRENT_ENCODING>*);
		xml::xml_items<CURRENT_ENCODING> ** addres_of(void);

	public:
		object_wrapper& operator = (xml::xml_items<CURRENT_ENCODING>*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (xml::xml_node_ptr<CURRENT_ENCODING>);

		object_wrapper_ptr<xml::xml_items<CURRENT_ENCODING>> operator & (void);
		xml::xml_items<CURRENT_ENCODING> * operator -> (void);
		xml::xml_items<CURRENT_ENCODING> const* operator -> (void)const;

		operator xml::xml_items<CURRENT_ENCODING> * (void);
		operator xml::xml_items<CURRENT_ENCODING> const* (void)const;

		template<typename T> xml::xml_node_ptr<CURRENT_ENCODING> operator[](safe_str<T> const&)const;
	};

	template<>
	class LINK object_wrapper<xml::xml_attribute_list<CURRENT_ENCODING>>
	{
	public:
		typedef xml::xml_attribute_list<CURRENT_ENCODING> type;

	private:
		xml::xml_attribute_list<CURRENT_ENCODING>* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_attribute_list<CURRENT_ENCODING>*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_attribute_list<CURRENT_ENCODING>* get(void)const;
		void set(xml::xml_attribute_list<CURRENT_ENCODING>*);
		xml::xml_attribute_list<CURRENT_ENCODING> ** addres_of(void);

	public:
		object_wrapper& operator = (xml::xml_attribute_list<CURRENT_ENCODING>*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (xml::xml_node_ptr<CURRENT_ENCODING>);

		object_wrapper_ptr<xml::xml_attribute_list<CURRENT_ENCODING>> operator & (void);
		xml::xml_attribute_list<CURRENT_ENCODING> * operator -> (void);
		xml::xml_attribute_list<CURRENT_ENCODING> const* operator -> (void)const;

		operator xml::xml_attribute_list<CURRENT_ENCODING> * (void);
		operator xml::xml_attribute_list<CURRENT_ENCODING> const* (void)const;

		template<typename T> xml::xml_value<CURRENT_ENCODING> operator[](safe_str<T> const&)const;
	};

	namespace xml
	{
		template<>
		class LINK xml_node<CURRENT_ENCODING>
			: public object
				, public ixmlobject<CURRENT_ENCODING>
		{
		private:
			const xml_type_t _xml_type;
			safe_pointer _xml_parent;
			xml_node_ptr<CURRENT_ENCODING> _xml_prev;
			xml_node_ptr<CURRENT_ENCODING> _xml_next;

		protected:
			xml_string<CURRENT_ENCODING> _xml_name;
			xml_items_ptr<CURRENT_ENCODING> _xml_attributes;
			struct {
				xml_type_t data_type;
				objptr unknown;
			} _xml_content;

		protected:
			xml_node(xml_type_t);
			virtual~xml_node();

		public:
			xml_node<CURRENT_ENCODING>* xml_parent()const;
			xml_node<CURRENT_ENCODING>* xml_prev()const;
			xml_node<CURRENT_ENCODING>* xml_next()const;

		protected:
			void xml_parent(xml_node<CURRENT_ENCODING>*);
			void xml_prev(xml_node<CURRENT_ENCODING>*);
			void xml_next(xml_node<CURRENT_ENCODING>*);

		public: //Overrides
			ANG_DECLARE_INTERFACE();
			virtual void clean();
			virtual xml_node_ptr<CURRENT_ENCODING> xml_clone()const = 0;

			xml_type_t xml_type()const override;
			bool xml_is_type_of(xml_type_t)const override;
			xml_string<CURRENT_ENCODING>& xml_print(xml_string<CURRENT_ENCODING>& stream, const xml_format_t& flag, ushort level = 0)const override;
			//streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;

			template<xml_type_enum TYPE>
			object_wrapper<typename xml_get_type<TYPE, CURRENT_ENCODING>::type> xml_as() { return null; }

			virtual bool xml_has_name()const;
			virtual bool xml_has_value()const;
			virtual bool xml_has_children()const;
			virtual bool xml_has_attributes()const;

			virtual xml_value<CURRENT_ENCODING> xml_name()const;
			virtual xml_value<CURRENT_ENCODING> xml_value()const;
			virtual xml_items_ptr<CURRENT_ENCODING> xml_children()const;
			virtual xml_attributes_ptr<CURRENT_ENCODING> xml_attributes()const;

			virtual bool xml_name(xml_string<CURRENT_ENCODING>);
			virtual bool xml_value(xml_string<CURRENT_ENCODING>);
			virtual bool xml_children(xml_items_ptr<CURRENT_ENCODING>);
			virtual bool xml_attributes(xml_items_ptr<CURRENT_ENCODING>);

			friend xml_items<CURRENT_ENCODING>;
			friend xml_tree<CURRENT_ENCODING>;
			friend xml_builder<CURRENT_ENCODING>;
		};

		template<>
		class LINK xml_items<CURRENT_ENCODING>
			: public object
			, public ixmlobject<CURRENT_ENCODING>
			, public xml::ixml_items<CURRENT_ENCODING>
		{
		protected:
			const xml_type_t _xml_type;
			uint _count;
			xml_node_ptr<CURRENT_ENCODING> _xml_first;
			xml_node_ptr<CURRENT_ENCODING> _xml_last;
			weak_ptr<xml_node<CURRENT_ENCODING>> _xml_parent;

		public:
			typedef xml_node<CURRENT_ENCODING>*				node_ptr_t;
			typedef xml::ixml_items<CURRENT_ENCODING>		ienum_t;

		protected:
			xml_items(xml_type_t);
			virtual~xml_items();

		public:
			ANG_DECLARE_INTERFACE();
			virtual void clean();
			virtual xml_items_ptr<CURRENT_ENCODING> xml_clone()const pure;

			xml_type_t xml_type()const override;
			bool xml_is_type_of(xml_type_t)const override;
			xml_string<CURRENT_ENCODING>& xml_print(xml_string<CURRENT_ENCODING>& stream, const xml_format_t& flag, ushort level = 0)const override;
			//streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;

		public: //ienum overrides
			virtual wsize counter()const override;
			virtual xml::xml_node_ptr<CURRENT_ENCODING> at(xml::xml_base_iterator<CURRENT_ENCODING> const&) override;
			virtual bool increase(xml::xml_base_iterator<CURRENT_ENCODING>&)const override;
			virtual bool increase(xml::xml_base_iterator<CURRENT_ENCODING>&, int offset)const override;
			virtual bool decrease(xml::xml_base_iterator<CURRENT_ENCODING>&)const override;
			virtual bool decrease(xml::xml_base_iterator<CURRENT_ENCODING>&, int offset)const override;

			virtual xml::xml_forward_iterator<CURRENT_ENCODING> begin() override;
			virtual xml::xml_forward_iterator<CURRENT_ENCODING> end() override;
			virtual xml::xml_const_forward_iterator<CURRENT_ENCODING> begin()const override;
			virtual xml::xml_const_forward_iterator<CURRENT_ENCODING> end()const override;

			virtual xml::xml_backward_iterator<CURRENT_ENCODING> rbegin() override;
			virtual xml::xml_backward_iterator<CURRENT_ENCODING> rend() override;
			virtual xml::xml_const_backward_iterator<CURRENT_ENCODING> rbegin()const override;
			virtual xml::xml_const_backward_iterator<CURRENT_ENCODING> rend()const override;

		public:
			void copy(const xml_items<CURRENT_ENCODING>*);
			xml_iterator<CURRENT_ENCODING> find(xml_string_view<CURRENT_ENCODING> const&, bool invert = false)const;
			xml_iterator<CURRENT_ENCODING> find(xml_string_view<CURRENT_ENCODING> const&, xml_base_iterator<CURRENT_ENCODING> next_to, bool invert = false)const;

		public:
			template<class _xml_type>
			object_wrapper<_xml_type> xml_as() { return null; }

			virtual bool push(xml_node_ptr<CURRENT_ENCODING> node, bool last = true);
			virtual bool insert(xml_node_ptr<CURRENT_ENCODING> node, xml_base_iterator<CURRENT_ENCODING> at, int offset = 0);
			virtual bool pop(xml_node_ptr_ptr<CURRENT_ENCODING> out = null, bool last = true);
			virtual bool pop_at(xml_base_iterator<CURRENT_ENCODING> it, xml_node_ptr_ptr<CURRENT_ENCODING> out = null);

			xml_iterator<CURRENT_ENCODING> find_first(xml_string_view<CURRENT_ENCODING> name, bool invert = false)const;
			xml_iterator<CURRENT_ENCODING> find_next(xml_string_view<CURRENT_ENCODING> name, xml_iterator_t next_to, bool invert = false)const;


			bool xml_parent(xml_node_t);

		protected:
			xml_node* xml_first()const;
			xml_node* xml_last()const;
			void xml_first(xml_node*);
			void xml_last(xml_node*);
		};
		
	}
}

