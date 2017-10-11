/*********************************************************************************************************************/
/*   File Name: ang/xml.h                                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes classes for the use of xml files                                         */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGXML_H__
#define __ANGXML_H__

#include <angsys.h>
//#include <ang/core/files.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace xml
	{
		class xml_node;
		class xml_items;
		class xml_tree;
		class xml_tree_iterator;

		class xml_attribute;
		class xml_attribute_list;
		class xml_header;
		class xml_comment;
		class xml_element;
		class xml_element_list;

		class xml_finder;
		class xml_builder;
		class xml_document;

		typedef object_wrapper<xml_node> xml_node_t;
		typedef object_wrapper_ptr<xml_node> xml_node_ptr_t;
		typedef object_wrapper<xml_items> xml_items_t;
		typedef object_wrapper<xml_tree> xml_tree_t;
		typedef object_wrapper<xml_tree_iterator> xml_tree_iterator_t;

		typedef object_wrapper<xml_header> xml_header_t;
		typedef object_wrapper<xml_comment> xml_comment_t;
		typedef object_wrapper<xml_element> xml_element_t;
		typedef object_wrapper<xml_attribute> xml_attribute_t;
		typedef object_wrapper<xml_element_list> xml_element_list_t;
		typedef object_wrapper<xml_attribute_list> xml_attributes_t;

		typedef object_wrapper<xml_finder> xml_finder_t;
		typedef object_wrapper<xml_builder> xml_builder_t;
		typedef object_wrapper<xml_document> xml_document_t;

		typedef collections::ienum<xml_node> ixml_items;
		typedef intf_wrapper<collections::ienum<xml_node>> ixml_items_t;
		typedef collections::base_iterator<xml_node> xml_base_iterator_t;
		typedef collections::iterator<xml_node> xml_iterator_t;
		typedef collections::const_iterator<xml_node> xml_const_iterator_t;
		typedef collections::forward_iterator<xml_node> xml_forward_iterator_t;
		typedef collections::const_forward_iterator<xml_node> xml_const_forward_iterator_t;
		typedef collections::backward_iterator<xml_node> xml_backward_iterator_t;
		typedef collections::const_backward_iterator<xml_node> xml_const_backward_iterator_t;

		typedef text::encoding xml_encoding;
		typedef text::encoding_t xml_encoding_t;

		class xml_value;
		typedef xml_value xml_value_t;

		ANG_BEGIN_ENUM(LINK, xml_type, byte)
			abstract = 0,
			value,
			node,
			store,
			tree,
			header,
			element,
			data,
			comment,
			attribute,
			element_list,
			attribute_list,
			finder,
			builder,
			document
		ANG_END_ENUM(xml_type);

		ANG_BEGIN_ENUM(LINK, xml_exception_code, uint)
			unknown,
			not_implemented = 0X200,
			parsing_error,
			unexpected_error,
		ANG_END_ENUM(xml_exception_code);

		ANG_BEGIN_FLAGS(LINK, xml_format, uint)
			none = 0X0000,
			fix_entity = 0X0100,
			wrap_text_tab = 0X0200, //has more priority that WrapTextSpace
			wrap_text_space = 0X0400,
		ANG_END_FLAGS(xml_format);


		class LINK xml_value final
			: public mstring
		{
		public:
			xml_value();
			xml_value(cstr_t);
			xml_value(cwstr_t);
			xml_value(cmstr_t);
			xml_value(strings::mstring_buffer*);
			xml_value(ang::nullptr_t const&);
			xml_value(mstring &&);
			xml_value(mstring const&);
			xml_value(string const&);
			xml_value(wstring const&);

			template<wsize N> xml_value(const wchar(&ar)[N])
				: xml_value(cwstr_t(ar, N - 1U)) { }
			template<wsize N> xml_value(const char(&ar)[N])
				: xml_value(cstr_t(ar, N - 1U)) { }

			xml_value& operator = (mstring &&);
			xml_value& operator = (mstring const&);
			xml_value& operator = (string const&);
			xml_value& operator = (wstring const&);
			xml_value& operator = (ang::nullptr_t const&);
			xml_value& operator = (strings::mstring_buffer*);

			xml_value& operator = (cstr_t);
			xml_value& operator = (cwstr_t);
			xml_value& operator = (cmstr_t);
			template<wsize N> inline xml_value& operator = (const wchar(&ar)[N]) {
				return xml_value::operator = (cwstr_t(ar, N - 1U));
			}
			template<wsize N> xml_value& operator = (const char(&ar)[N]) {
				return xml_value::operator = (cwstr_t(ar, N - 1U));
			}

			xml_value& operator += (mstring const&);
			xml_value& operator += (string const&);
			xml_value& operator += (wstring const&);
			xml_value& operator += (cstr_t);
			xml_value& operator += (cwstr_t);
			xml_value& operator += (cmstr_t);
			template<wsize N> inline xml_value& operator += (const wchar(&ar)[N]) {
				return xml_value::operator += (cwstr_t(ar, N - 1U));
			}
			template<wsize N> xml_value& operator += (const char(&ar)[N]) {
				return xml_value::operator += (cwstr_t(ar, N - 1U));
			}

			template<typename T> inline T as()const {
				//static_assert(true,
				//	"exception: unsupported type cast, user must implement the specialization of \"template<typename T> T xml_value::as()const\""_s);
				return T();
			}
			mstring& xml_print(mstring& stream, xml_format_t const& format, ushort level)const;
			//streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, xml_format_t const& format, ushort level)const;
		};


		ANG_BEGIN_INTERFACE(LINK, ixmlobject)
			visible vcall xml_type_t xml_type()const pure;
			visible vcall bool xml_is_type_of(xml_type_t)const pure;
			visible vcall mstring& xml_print(mstring& stream, const xml_format_t& flag, ushort level = 0)const pure;
			//visible vcall streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const pure;
		ANG_END_INTERFACE();
	}
}

namespace ang
{
	namespace collections
	{
		template<> class iterator<xml::xml_node> : public base_iterator<xml::xml_node>
		{
		public:
			iterator(base_iterator<xml::xml_node> const& it) : base_iterator<xml::xml_node>(it) {}
			iterator(ienum<xml::xml_node>* p = null, position_t cur = null, uint offset = 0U)
				: base_iterator<xml::xml_node>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<xml::xml_node>& it) {
				base_iterator<xml::xml_node>::operator = (it);
				return*this;
			}

			operator xml::xml_node_t()const;
			xml::xml_node* operator -> ()const;
			xml::xml_node_t operator * ()const;

			operator xml::xml_element_t()const; //can be null
			operator xml::xml_comment_t()const; //can be null
			operator xml::xml_attribute_t()const; //can be null
		};

		template<> class iterator<const xml::xml_node> : public base_iterator<xml::xml_node>
		{
		public:
			iterator(base_iterator<xml::xml_node> const& it) : base_iterator<xml::xml_node>(it) {}
			iterator(ienum<xml::xml_node>* p = null, position_t cur = null, uint offset = 0U)
				: base_iterator<xml::xml_node>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<xml::xml_node>& it) {
				base_iterator<xml::xml_node>::operator = (it);
				return*this;
			}

			operator const xml::xml_node_t()const;
			xml::xml_node const* operator -> ()const;
			const xml::xml_node_t operator * ()const;

			operator const xml::xml_element_t()const; //can be null
			operator const xml::xml_comment_t()const; //can be null
			operator const xml::xml_attribute_t()const; //can be null
		};

		template<>
		ANG_BEGIN_INTERFACE(LINK, ienum<xml::xml_node>)
			visible vcall uint counter()const pure;
			visible vcall xml::xml_node_t at(base_iterator<xml::xml_node> const&) pure;
			visible vcall bool increase(base_iterator<xml::xml_node>&)const pure;
			visible vcall bool increase(base_iterator<xml::xml_node>&, int offset)const pure;
			visible vcall bool decrease(base_iterator<xml::xml_node>&)const pure;
			visible vcall bool decrease(base_iterator<xml::xml_node>&, int offset)const pure;

			visible vcall forward_iterator<xml::xml_node> begin() pure;
			visible vcall forward_iterator<xml::xml_node> end() pure;
			visible vcall const_forward_iterator<xml::xml_node> begin()const pure;
			visible vcall const_forward_iterator<xml::xml_node> end()const pure;

			visible vcall backward_iterator<xml::xml_node> rbegin() pure;
			visible vcall backward_iterator<xml::xml_node> rend() pure;
			visible vcall const_backward_iterator<xml::xml_node> rbegin()const pure;
			visible vcall const_backward_iterator<xml::xml_node> rend()const pure;
		ANG_END_INTERFACE();

	}
}

namespace ang
{
	template<> class LINK object_wrapper<xml::xml_node>
	{
	public:
		typedef xml::xml_node type;

	private:
		xml::xml_node* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_node*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_node* get(void)const;
		void set(xml::xml_node*);
		xml::xml_node ** addres_of(void);

	public:
		object_wrapper& operator = (xml::xml_node*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<xml::xml_node> operator & (void);
		xml::xml_node * operator -> (void);
		xml::xml_node const* operator -> (void)const;

		operator xml::xml_node * (void);
		operator xml::xml_node const* (void)const;
		operator xml::xml_value_t()const;

		xml::xml_node_t operator[](cstr_t)const;
		xml::xml_node_t operator[](cwstr_t)const;
		xml::xml_node_t operator[](cmstr_t)const;
	};

	template<> class LINK object_wrapper<xml::xml_items>
	{
	public:
		typedef xml::xml_items type;

	private:
		xml::xml_items* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_items*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_items* get(void)const;
		void set(xml::xml_items*);
		xml::xml_items ** addres_of(void);

		xml::xml_forward_iterator_t begin();
		xml::xml_forward_iterator_t end();

		xml::xml_const_forward_iterator_t begin()const;
		xml::xml_const_forward_iterator_t end()const;

	public:
		object_wrapper& operator = (xml::xml_items*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (xml::xml_node_t);

		object_wrapper_ptr<xml::xml_items> operator & (void);
		xml::xml_items * operator -> (void);
		xml::xml_items const* operator -> (void)const;

		operator xml::xml_items * (void);
		operator xml::xml_items const* (void)const;

		xml::xml_node_t operator[](cstr_t)const;
		xml::xml_node_t operator[](cwstr_t)const;
		xml::xml_node_t operator[](cmstr_t)const;
	};

	template<> class LINK object_wrapper<xml::xml_attribute_list>
	{
	public:
		typedef xml::xml_attribute_list type;

	private:
		xml::xml_attribute_list* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_attribute_list*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_attribute_list* get(void)const;
		void set(xml::xml_attribute_list*);
		xml::xml_attribute_list ** addres_of(void);

		xml::xml_forward_iterator_t begin();
		xml::xml_forward_iterator_t end();

		xml::xml_const_forward_iterator_t begin()const;
		xml::xml_const_forward_iterator_t end()const;

	public:
		object_wrapper& operator = (xml::xml_attribute_list*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator += (xml::xml_node_t);

		object_wrapper_ptr<xml::xml_attribute_list> operator & (void);
		xml::xml_attribute_list * operator -> (void);
		xml::xml_attribute_list const* operator -> (void)const;

		operator xml::xml_attribute_list * (void);
		operator xml::xml_attribute_list const* (void)const;

		xml::xml_value_t operator[](cstr_t)const;
		xml::xml_value_t operator[](cwstr_t)const;
		xml::xml_value_t operator[](cmstr_t)const;
	};

	namespace xml
	{
		class LINK xml_node
			: public object
			, public ixmlobject
		{
		private:
			const xml_type_t _xml_type;
			mutable weak_ptr<xml_node> _xml_parent;
			xml_node_t _xml_prev;
			xml_node_t _xml_next;

		protected:
			xml_value_t _xml_name;
			xml_items_t _xml_attributes;
			struct {
				xml_type_t data_type;
				objptr unknown;
			} _xml_content;

		protected:
			xml_node(xml_type_t);
			virtual~xml_node();

		public:
			xml_node_t xml_parent()const;
			xml_node* xml_prev()const;
			xml_node* xml_next()const;

		protected:
			void xml_parent(xml_node*);
			void xml_prev(xml_node*);
			void xml_next(xml_node*);

		public: //Overrides
			ANG_DECLARE_INTERFACE();
			virtual void clean();
			virtual xml_node_t xml_clone()const = 0;

			xml_type_t xml_type()const override;
			bool xml_is_type_of(xml_type_t)const override;
			mstring& xml_print(mstring& stream, const xml_format_t& flag, ushort level = 0)const override;
			//streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;

			template<class _xml_type>
			object_wrapper<_xml_type> xml_as() {
				return null;
			}

			virtual bool xml_has_name()const;
			virtual bool xml_has_data()const;
			virtual bool xml_has_value()const;
			virtual bool xml_has_children()const;
			virtual bool xml_has_attributes()const;

			virtual xml_value_t xml_name()const;
			virtual xml_value_t xml_data()const;
			virtual xml_value_t xml_value()const;
			virtual xml_items_t xml_children()const;
			virtual xml_attributes_t xml_attributes()const;

			virtual bool xml_name(mstring);
			virtual bool xml_data(mstring);
			virtual bool xml_value(mstring);
			virtual bool xml_children(xml_items_t);
			virtual bool xml_attributes(xml_items_t);

			friend xml_items;
			friend xml_tree;
			friend xml_builder;
		};

		class LINK xml_items
			: public object
			, public ixmlobject
			, public collections::ienum<xml_node>
		{
		protected:
			const xml_type_t _xml_type;
			uint _count;
			mutable weak_ptr<xml_node> _xml_parent;
			xml_node_t _xml_first;
			xml_node_t _xml_last;

		public:
			typedef xml_node*						node_ptr_t;
			typedef collections::ienum<xml_node>	ienum_t;

		protected:
			xml_items(xml_type_t);
			virtual~xml_items();

		public:
			ANG_DECLARE_INTERFACE();
			virtual void clean();
			virtual xml_items_t xml_clone()const = 0;

			xml_type_t xml_type()const override;
			bool xml_is_type_of(xml_type_t)const override;
			mstring& xml_print(mstring& stream, const xml_format_t& flag, ushort level = 0)const override;
			//streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;

		public: //ienum overrides
			virtual uint counter()const override;
			virtual xml::xml_node_t at(xml_base_iterator_t const&) override;
			virtual bool increase(xml_base_iterator_t&)const override;
			virtual bool increase(xml_base_iterator_t&, int offset)const override;
			virtual bool decrease(xml_base_iterator_t&)const override;
			virtual bool decrease(xml_base_iterator_t&, int offset)const override;

			virtual xml_forward_iterator_t begin() override;
			virtual xml_forward_iterator_t end() override;
			virtual xml_const_forward_iterator_t begin()const override;
			virtual xml_const_forward_iterator_t end()const override;

			virtual xml_backward_iterator_t rbegin() override;
			virtual xml_backward_iterator_t rend() override;
			virtual xml_const_backward_iterator_t rbegin()const override;
			virtual xml_const_backward_iterator_t rend()const override;

		public: //utils
			template<class _xml_type>
			object_wrapper<_xml_type> xml_as() {
				return null;
			}

			virtual void copy(const ixml_items*);
			virtual void extend(const ixml_items*);
			virtual void push(xml_node_t node, bool last = true);
			virtual bool insert(xml_node_t node, xml_iterator_t at, bool nextTo = true);
			virtual bool insert(xml_node_t node, index at);
			virtual bool pop(xml_node_ptr_t out = null, bool last = true);
			virtual bool pop_at(xml_iterator_t it, xml_node_ptr_t out = null);

			xml_iterator_t find(xml_node_t)const;

			xml_iterator_t find_first(cstr_t name, bool invert = false)const;
			xml_iterator_t find_first(cwstr_t name, bool invert = false)const;
			xml_iterator_t find_first(cmstr_t name, bool invert = false)const;

			xml_iterator_t find_next(cstr_t name, xml_iterator_t next_to, bool invert = false)const;
			xml_iterator_t find_next(cwstr_t name, xml_iterator_t next_to, bool invert = false)const;
			xml_iterator_t find_next(cmstr_t name, xml_iterator_t next_to, bool invert = false)const;

			bool xml_parent(xml_node_t);

		protected:
			xml_node* xml_first()const;
			xml_node* xml_last()const;
			void xml_first(xml_node*);
			void xml_last(xml_node*);
		};


		template<> xml_attribute_t LINK xml_node::xml_as<xml_attribute>();
		template<> xml_header_t LINK xml_node::xml_as<xml_header>();
		template<> xml_comment_t LINK xml_node::xml_as<xml_comment>();
		template<> xml_element_t LINK xml_node::xml_as<xml_element>();
		template<> xml_attributes_t LINK xml_items::xml_as<xml_attribute_list>();
		template<> xml_element_list_t LINK xml_items::xml_as<xml_element_list>();
	}
}


////////////////////////////

namespace ang
{
	namespace xml
	{
		class LINK xml_attribute final
			: public xml_node
		{
		public:
			xml_attribute();
			xml_attribute(mstring, mstring);
			xml_attribute(xml_attribute const&);
			xml_attribute(xml_attribute const*);

		public:
			ANG_DECLARE_INTERFACE();

			xml_node_t xml_clone()const override;
			bool xml_has_name()const override;
			bool xml_has_value()const override;

		private:
			using xml_node::xml_has_children;
			using xml_node::xml_has_attributes;

			using xml_node::xml_children;
			using xml_node::xml_attributes;

			using xml_node::xml_parent;
			using xml_node::xml_prev;
			using xml_node::xml_next;

			virtual~xml_attribute();
		};

		class LINK xml_comment final
			: public xml_node
		{
		public:
			xml_comment();
			xml_comment(mstring);
			xml_comment(xml_comment const&);
			xml_comment(xml_comment const*);

		public:
			ANG_DECLARE_INTERFACE();

			xml_node_t xml_clone()const override;
			bool xml_has_value()const override;

		private:
			using xml_node::xml_has_name;
			using xml_node::xml_has_children;
			using xml_node::xml_has_attributes;

			using xml_node::xml_name;
			using xml_node::xml_children;
			using xml_node::xml_attributes;

			using xml_node::xml_parent;
			using xml_node::xml_prev;
			using xml_node::xml_next;

			virtual~xml_comment();
		};

		class LINK xml_element final
			: public xml_node
		{
		public:
			xml_element();
			xml_element(mstring, mstring);
			xml_element(mstring, xml_items_t);
			xml_element(xml_element const&);
			xml_element(xml_element const*);

		public:
			ANG_DECLARE_INTERFACE();

			xml_node_t xml_clone()const override;
			bool xml_has_name()const override;
			bool xml_has_value()const override;
			bool xml_has_children()const override;
			bool xml_has_attributes()const override;

		private:
			using xml_node::xml_parent;
			using xml_node::xml_prev;
			using xml_node::xml_next;

			virtual~xml_element();
		};

		class LINK xml_header final
			: public xml_node
		{
		public:
			xml_header();
			xml_header(const xml_header&);
			xml_header(const xml_header*);

		public: //Overrides
			ANG_DECLARE_INTERFACE();
			xml_node_t xml_clone()const override;

		public:
			mstring version()const;
			xml_encoding encoding()const;
			bool is_stand_alone()const;

			void version(mstring);
			void encoding(xml_encoding_t);
			void is_stand_alone(bool);

			void encoding(mstring);
			void is_stand_alone(mstring);

		private:
			using xml_node::xml_has_name;
			using xml_node::xml_has_value;
			using xml_node::xml_has_children;
			using xml_node::xml_has_attributes;

			using xml_node::xml_name;
			using xml_node::xml_value;
			using xml_node::xml_children;
			using xml_node::xml_attributes;

			using xml_node::xml_parent;
			using xml_node::xml_prev;
			using xml_node::xml_next;

			virtual~xml_header();
		};
	}
}


namespace ang
{
	namespace xml
	{
		class LINK xml_attribute_list final
			: public xml_items
		{
		public:
			xml_attribute_list();
			xml_attribute_list(const xml_attribute_list&);
			xml_attribute_list(const xml_attribute_list*);

		public:	//Overrides
			ANG_DECLARE_INTERFACE();
			xml_items_t xml_clone()const override;

			void push(xml_node_t node, bool last = true) override;
			bool insert(xml_node_t node, xml_iterator_t at, bool nextTo = true) override;

		private:
			virtual~xml_attribute_list();
		};

		class LINK xml_element_list final
			: public xml_items
		{
		public:
			xml_element_list();
			xml_element_list(const xml_element_list&);
			xml_element_list(const xml_element_list*);

		public:	//Overrides
			ANG_DECLARE_INTERFACE();
			xml_items_t xml_clone()const override;

			void push(xml_node_t node, bool last = true) override;
			bool insert(xml_node_t node, xml_iterator_t at, bool nextTo = true) override;
		private:
			virtual~xml_element_list();
		};
	}
}


namespace ang
{
	template<> class LINK object_wrapper<xml::xml_tree>
	{
	public:
		typedef xml::xml_tree type;

	private:
		xml::xml_tree* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_tree*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_tree* get(void)const;
		void set(xml::xml_tree*);
		xml::xml_tree ** addres_of(void);

	public:
		object_wrapper& operator = (xml::xml_tree*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<xml::xml_tree> operator & (void);
		xml::xml_tree * operator -> (void);
		xml::xml_tree const* operator -> (void)const;
		operator xml::xml_tree * (void);
		operator xml::xml_tree const* (void)const;

		//xml::xml_node_t operator[](xml::xml_value)const;
		//xml::xml_node_t operator[](string const&)const;
		//xml::xml_node_t operator[](wstring_t const&)const;
		//xml::xml_node_t operator[](cstr_t)const;
		//xml::xml_node_t operator[](cwstr_t)const;
	};

	namespace xml
	{
		class LINK xml_tree_iterator
		{
		protected:
			mutable weak_ptr<xml_tree> _xml_tree;
			xml_node_t	_xml_current;
			xml_node_t	_xml_child;

		public:
			xml_tree_iterator();
			xml_tree_iterator(xml_tree_t);
			xml_tree_iterator(const xml_tree_iterator&);
			~xml_tree_iterator();

		private:
			xml_node_t xml_current()const;
			xml_node_t xml_child()const;
			void xml_current(xml_node_t);
			void xml_child(xml_node_t);

		public: //Properties
			void xml_parent(xml_tree_t);
			xml_tree_t xml_parent()const;

			xml_iterator_t current()const;
			xml_iterator_t child()const;

		public: //Methods
			bool begin_child();
			bool end_child();
			bool next();
			bool prev();
			bool next_child();
			bool prev_Child();

			bool backward();
			bool forward();

			friend xml_tree;
			friend xml_finder;
			friend xml_builder;
		};

		class LINK xml_tree
			: public object
			, public ixmlobject
			, public collections::ienum<xml_node>
		{
		public:
			typedef xml_node*								node_ptr_t;
			typedef collections::ienum<xml_node>			ienum_t;

		protected:
			xml_type_t _xml_type;
			uint _count;
			xml_node_t _xml_head;
			xml_node_t _xml_root;
			xml_node_t _xml_tail;

		protected:
			xml_tree(xml_type_t);
			virtual~xml_tree();

		public:
			xml_tree(xml_node_t);

			ANG_DECLARE_INTERFACE();
			virtual void clean();

			xml_type_t xml_type()const override;
			bool xml_is_type_of(xml_type_t)const override;
			mstring& xml_print(mstring& stream, const xml_format_t& flag, ushort level = 0)const override;
			//streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;

		public: //ienum overrides
			virtual uint counter()const override;
			virtual xml::xml_node_t at(xml_base_iterator_t const&) override;
			virtual bool increase(xml_base_iterator_t&)const override;
			virtual bool increase(xml_base_iterator_t&, int offset)const override;
			virtual bool decrease(xml_base_iterator_t&)const override;
			virtual bool decrease(xml_base_iterator_t&, int offset)const override;

			virtual xml_forward_iterator_t begin() override;
			virtual xml_forward_iterator_t end() override;
			virtual xml_const_forward_iterator_t begin()const override;
			virtual xml_const_forward_iterator_t end()const override;

			virtual xml_backward_iterator_t rbegin() override;
			virtual xml_backward_iterator_t rend() override;
			virtual xml_const_backward_iterator_t rbegin()const override;
			virtual xml_const_backward_iterator_t rend()const override;

		public:
			template<class _xml_type>
			object_wrapper<_xml_type> xml_as() {
				return null;
			}

		public:
			xml_iterator_t find(xml_node_t)const;

			xml_iterator_t find_first(cstr_t name, bool invert = false)const;
			xml_iterator_t find_first(cwstr_t name, bool invert = false)const;
			xml_iterator_t find_first(cmstr_t name, bool invert = false)const;

			xml_iterator_t find_next(cstr_t name, xml_iterator_t next_to, bool invert = false)const;
			xml_iterator_t find_next(cwstr_t name, xml_iterator_t next_to, bool invert = false)const;
			xml_iterator_t find_next(cmstr_t name, xml_iterator_t next_to, bool invert = false)const;

		public: //Properties
			bool is_empty()const;
			xml_node* xml_head()const;
			xml_node* xml_root()const;
			xml_node* xml_tail()const;

		protected:
			void insert_next_to(xml_node*, xml_node*);
			void insert_prev_to(xml_node*, xml_node*);
			void insert_in_head(xml_node*);
			void insert_in_tail(xml_node*);
			void xml_head(xml_node_t);
			void xml_root(xml_node_t);
			void xml_tail(xml_node_t);
		};

		class LINK xml_builder final
			: public xml_tree
		{
		private:
			xml_tree_iterator _current;

		public:
			xml_builder();
			xml_builder(const xml_builder&);
			xml_builder(const xml_builder*);

		public: //Overrides
			ANG_DECLARE_INTERFACE();
			void clean() override;

			void push(xml_node_t node, bool last = true);
			bool insert(xml_node_t node, xml_iterator_t at, bool nextTo);
			bool pop(xml_node** out = null, bool last = true);
			bool pop_at(xml_iterator_t it, xml_node** out = null);

		public: //Methods
			xml_element_t xml_current()const;
			xml_iterator_t xml_current_pos()const;

			bool move_to(xml_iterator_t current);
			bool move_to_child(xml_iterator_t child);

			bool move_up();
			bool move_down();
			bool move_forward();
			bool move_backward();

			void push_header(xml_header_t);
			bool begin_element(mstring name);
			bool end_element();
			bool element(mstring name, mstring value);
			bool element(xml_element_t element);
			bool data(mstring value);
			bool value(mstring value);
			bool attribute(mstring name, mstring value);
			bool comment(mstring value);

		private:
			virtual~xml_builder();
		};
	}
}

namespace ang
{

	template<> class LINK object_wrapper<xml::xml_builder>
	{
	public:
		typedef xml::xml_builder type;

	private:
		xml::xml_builder* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_builder*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_builder* get(void)const;
		void set(xml::xml_builder*);
		xml::xml_builder ** addres_of(void);


	public:
		object_wrapper& operator = (xml::xml_builder*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<xml::xml_builder> operator & (void);
		xml::xml_builder * operator -> (void);
		xml::xml_builder const* operator -> (void)const;
		operator xml::xml_builder * (void);
		operator xml::xml_builder const* (void)const;
	};

}

namespace ang
{
	namespace xml
	{
		class LINK xml_document final
			: public object
		{

		private:
			xml_builder_t _builder;
			string _last_parsing_error;

		public:
			xml_document();
			//xml_document(core::files::input_text_file_t);
			ANG_DECLARE_INTERFACE();

		public:
			//void load(core::files::input_text_file_t file);
			//void save(core::files::output_text_file_t)const;
			//void serialize(streams::itext_output_stream_t)const;

			void parse(string code);
			void parse(mstring code);
			void parse(wstring code);

			void parse(cstr_t code);
			void parse(cwstr_t code);
			void parse(cmstr_t code);

			xml_builder_t create();
			string last_parsing_error()const;
			xml_element_t xml_root()const;
			xml_tree_t xml_tree()const;

		private:
			xml_builder_t builder()const;
			void builder(xml_builder_t);

			//bool decode_header(string& code, collections::iterator<char_t>& begin);
			//bool decode_element(string& code, collections::iterator<char_t>& begin);
			//bool decode_header(mstring& code, collections::iterator<achar_t>& begin);
			//bool decode_element(mstring& code, collections::iterator<achar_t>& begin);

			xml_encoding_t decode_header(cstr_t code, index& begin);
			bool decode_element(cstr_t code, index& begin);

			xml_encoding_t decode_header(cwstr_t code, index& begin);
			bool decode_element(cwstr_t code, index& begin);

			xml_encoding_t decode_header(cmstr_t code, index& begin);
			bool decode_element(cmstr_t code, index& begin);

		private:
			virtual~xml_document();
		};
	}
}

namespace ang
{
	template<class F>
	inline void foreach(xml::xml_items_t const& store, F func) {
		if (!store.is_empty())for (xml::xml_forward_iterator_t it = store->begin(); it.is_valid(); ++it)
			func((xml::xml_node_t)it);
	}

	template<class F>
	inline void foreach(xml::xml_attributes_t const& store, F func) {
		if (!store.is_empty())for (xml::xml_forward_iterator_t it = store->begin(); it.is_valid(); ++it)
			func((xml::xml_attribute*)((*it).get()));
	}
}


template<> bool LINK ang::xml::xml_value::as<bool>()const;
template<> short LINK ang::xml::xml_value::as<short>()const;
template<> ushort LINK ang::xml::xml_value::as<ushort>()const;
template<> int LINK ang::xml::xml_value::as<int>()const;
template<> uint LINK ang::xml::xml_value::as<uint>()const;
template<> long LINK ang::xml::xml_value::as<long>()const;
template<> ulong LINK ang::xml::xml_value::as<ulong>()const;
template<> long64 LINK ang::xml::xml_value::as<long64>()const;
template<> ulong64 LINK ang::xml::xml_value::as<ulong64>()const;
template<> float LINK ang::xml::xml_value::as<float>()const;
template<> double LINK ang::xml::xml_value::as<double>()const;

template<> ang::cstr_t LINK ang::xml::xml_value::as<ang::cstr_t>()const;
template<> ang::cmstr_t LINK ang::xml::xml_value::as<ang::cmstr_t>()const;
template<> ang::cwstr_t LINK ang::xml::xml_value::as<ang::cwstr_t>()const;
template<> ang::string LINK ang::xml::xml_value::as<ang::string>()const;
template<> ang::mstring LINK ang::xml::xml_value::as<ang::mstring>()const;
template<> ang::wstring LINK ang::xml::xml_value::as<ang::wstring>()const;
template<> ang::xml::xml_encoding_t LINK ang::xml::xml_value::as<ang::xml::xml_encoding_t>()const;

inline ang::xml::xml_node const* ang::collections::iterator<const ang::xml::xml_node>::operator -> ()const
{
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	return _parent->at(*this).get();
}

inline ang::xml::xml_node* ang::collections::iterator<ang::xml::xml_node>::operator -> ()const
{
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	return _parent->at(*this).get();
}

inline const ang::xml::xml_node_t ang::collections::iterator<const ang::xml::xml_node>::operator * ()const {
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	return _parent->at(*this);
}

inline ang::collections::iterator<const ang::xml::xml_node>::operator const ang::xml::xml_attribute_t ()const {
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	xml::xml_node_t node = _parent->at(*this);
	return !node.is_empty() ? node->xml_as<xml::xml_attribute>() : null;
}

inline ang::collections::iterator<const ang::xml::xml_node>::operator const ang::xml::xml_element_t ()const {
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	xml::xml_node_t node = _parent->at(*this);
	return !node.is_empty() ? node->xml_as<xml::xml_element>() : null;
}

inline ang::collections::iterator<const ang::xml::xml_node>::operator const ang::xml::xml_comment_t ()const {
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	xml::xml_node_t node = _parent->at(*this);
	return !node.is_empty() ? node->xml_as<xml::xml_comment>() : null;
}

inline ang::xml::xml_node_t ang::collections::iterator<ang::xml::xml_node>::operator * ()const {
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	return _parent->at(*this);
}

inline ang::collections::iterator<ang::xml::xml_node>::operator ang::xml::xml_attribute_t ()const {
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	xml::xml_node_t node = _parent->at(*this);
	return !node.is_empty() ? node->xml_as<xml::xml_attribute>() : null;
}

inline ang::collections::iterator<ang::xml::xml_node>::operator ang::xml::xml_element_t ()const {
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	xml::xml_node_t node = _parent->at(*this);
	return !node.is_empty() ? node->xml_as<xml::xml_element>() : null;
}

inline ang::collections::iterator<ang::xml::xml_node>::operator ang::xml::xml_comment_t ()const {
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	xml::xml_node_t node = _parent->at(*this);
	return !node.is_empty() ? node->xml_as<xml::xml_comment>() : null;
}

inline ang::collections::iterator<const ang::xml::xml_node>::operator const ang::xml::xml_node_t()const
{
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	return _parent->at(*this);
}

inline ang::collections::iterator<ang::xml::xml_node>::operator ang::xml::xml_node_t ()const
{
	if (_parent == null)
		throw exception(except_code::invalid_memory);
	return _parent->at(*this);
}

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANGXML_H__