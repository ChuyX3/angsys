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

#ifndef __ANGXML_HPP__
#define __ANGXML_HPP__

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

		ANG_INTERFACE(ixml_object);
		ANG_INTERFACE(ixml_node);
		ANG_INTERFACE(ixml_document);
		ANG_INTERFACE(ixml_store);

		ANG_OBJECT(xml_document);
		ANG_OBJECT(xml_node);
		//ANG_OBJECT(xml_store);
		ANG_OBJECT(xml_text);
		ANG_OBJECT(xml_cdata);
		ANG_OBJECT(xml_pcdata);
		ANG_OBJECT(xml_attribute);
		ANG_OBJECT(xml_header);
		ANG_OBJECT(xml_comment);
		ANG_OBJECT(xml_element);

		typedef class ixml_attributes ixml_attributes_t;
		typedef collections::ienum<struct ixml_node > ixml_items;
		typedef collections::iterator<struct ixml_node > xml_iterator_t;
		typedef collections::base_iterator<struct ixml_node > xml_base_iterator_t;
		typedef collections::forward_iterator<struct ixml_node > xml_forward_iterator_t;
		typedef collections::backward_iterator<struct ixml_node > xml_backward_iterator_t;
		typedef collections::const_iterator<struct ixml_node > xml_const_iterator_t;
		typedef collections::const_forward_iterator<struct ixml_node > xml_const_forward_iterator_t;
		typedef collections::const_backward_iterator<struct ixml_node > xml_const_backward_iterator_t;

		typedef text::encoding xml_encoding;
		typedef text::encoding_t xml_encoding_t;

		ANG_BEGIN_ENUM(LINK, xml_type, byte)
			abstract = 0,
			text,
			cdata,
			pcdata,
			node,
			store,
			tree,
			header,
			element,
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
	}
}

namespace ang
{
	namespace collections
	{
		template<>
		ANG_BEGIN_INTERFACE(LINK, ienum<xml::ixml_node>)
			visible vcall wsize counter()const pure;

			visible vcall xml::ixml_node_t at(base_iterator<xml::ixml_node> const&) pure;
			visible vcall bool increase(base_iterator<xml::ixml_node>&)const pure;
			visible vcall bool increase(base_iterator<xml::ixml_node>&, int offset)const pure;
			visible vcall bool decrease(base_iterator<xml::ixml_node>&)const pure;
			visible vcall bool decrease(base_iterator<xml::ixml_node>&, int offset)const pure;

			visible vcall forward_iterator<xml::ixml_node> begin() pure;
			visible vcall forward_iterator<xml::ixml_node> end() pure;
			visible vcall const_forward_iterator<xml::ixml_node> begin()const pure;
			visible vcall const_forward_iterator<xml::ixml_node> end()const pure;

			visible vcall backward_iterator<xml::ixml_node> rbegin() pure;
			visible vcall backward_iterator<xml::ixml_node> rend() pure;
			visible vcall const_backward_iterator<xml::ixml_node> rbegin()const pure;
			visible vcall const_backward_iterator<xml::ixml_node> rend()const pure;
		ANG_END_INTERFACE();


		template<> class iterator<xml::ixml_node> : public base_iterator<xml::ixml_node>
		{
		public:
			iterator(base_iterator<xml::ixml_node> const& it) : base_iterator<xml::ixml_node>(it) {}
			iterator(ienum<xml::ixml_node>* p = null, position_t cur = null, wsize offset = 0U)
				: base_iterator<xml::ixml_node>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<xml::ixml_node>& it) {
				base_iterator<xml::ixml_node>::operator = (it);
				return*this;
			}

			inline operator xml::ixml_node*()const;
			inline xml::ixml_node_t operator -> ()const;
			inline xml::ixml_node_t operator * ()const;
		};

		template<> class iterator<const xml::ixml_node> : public base_iterator<xml::ixml_node>
		{
		public:
			iterator(base_iterator<xml::ixml_node> const& it) : base_iterator<xml::ixml_node>(it) {}
			iterator(ienum<xml::ixml_node>* p = null, position_t cur = null, wsize offset = 0U)
				: base_iterator<xml::ixml_node>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<xml::ixml_node>& it) {
				base_iterator<xml::ixml_node>::operator = (it);
				return*this;
			}

			inline operator xml::ixml_node const*()const;
			inline const xml::ixml_node_t operator -> ()const;
			inline const xml::ixml_node_t operator * ()const;
		};

	}
}

namespace ang
{
	namespace xml
	{
		ANG_BEGIN_INTERFACE(LINK, ixml_object)
			visible vcall xml_type_t xml_type()const pure;
			visible vcall bool xml_is_type_of(xml_type_t)const pure;
			visible vcall wstring& xml_print(wstring& stream, const xml_format_t& flag, ushort level = 0)const pure;
			template<typename T> typename smart_ptr_type<T>::smart_ptr_t xml_as() { return null; }
		ANG_END_INTERFACE();

		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ixml_node, ixml_object)
			visible vcall ixml_node_t xml_parent()const pure;
			visible vcall ixml_document_t xml_parent_doc()const pure;
			visible vcall ixml_node_t xml_first_child()const pure;
			visible vcall ixml_node_t xml_last_child()const pure;
			visible vcall ixml_node_t xml_prev_sibling()const pure;
			visible vcall ixml_node_t xml_next_sibling()const pure;
			visible vcall ixml_node_t xml_clone(ixml_document_t)const pure;

			visible vcall bool xml_has_name()const pure;
			visible vcall bool xml_has_value()const pure;
			visible vcall bool xml_has_children()const pure;
			visible vcall bool xml_has_attributes()const pure;

			visible vcall xml_text_t xml_name()const pure;
			visible vcall xml_text_t xml_value()const pure;
			visible vcall ixml_store_t xml_children()const pure;
			visible vcall ixml_attributes_t xml_attributes()const pure;

			visible vcall void xml_parent(ixml_node_t) pure;
			visible vcall void xml_prev_sibling(ixml_node_t) pure;
			visible vcall void xml_next_sibling(ixml_node_t) pure;
			permit ixml_store;
			permit ixml_document;
		ANG_END_INTERFACE();

		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ixml_store, public ixml_object, public collections::ienum<ixml_node>)
			visible vcall ixml_node_t xml_parent()const pure;
			visible vcall ixml_document_t xml_parent_doc()const pure;
			visible vcall xml_iterator_t find(raw_str_t, bool invert = false)const pure;
			visible vcall xml_iterator_t find(raw_str_t, xml_iterator_t next_to, bool invert = false)const pure;
			template<typename T> inline xml_iterator_t find(safe_str<T> const& str, bool invert = false)const { find(raw_str(str), invert); }
			template<typename T> inline xml_iterator_t find(safe_str<T> const& str, xml_const_iterator_t next_to, bool invert = false)const { find(raw_str(str), next_to, invert); }
		ANG_END_INTERFACE();


		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ixml_document, public ixml_object, public collections::ienum<ixml_node>)
			visible vcall xml_header_t xml_header()const pure;
			visible vcall ixml_node_t xml_data_type()const pure;
			visible vcall xml_element_t xml_root_element()const pure;
			visible vcall ixml_document_t xml_clone()const pure;

			visible vcall xml_iterator_t find(raw_str_t, bool invert = false)const pure;
			visible vcall xml_iterator_t find(raw_str_t, xml_iterator_t next_to, bool invert = false)const pure;

			visible vcall xml_iterator_t xml_current()const pure;
			visible vcall xml_element_t xml_current_element()const pure;

			visible vcall bool move_to(xml_iterator_t current) pure;
			visible vcall bool move_to_child(xml_iterator_t child) pure;

			visible vcall bool move_up() pure;
			visible vcall bool move_down() pure;
			visible vcall bool move_forward() pure;
			visible vcall bool move_backward() pure;

			visible vcall void push_default_header() pure;
			visible vcall void push_header(wstring version, xml_encoding_t encoding, bool standalone) pure;
			visible vcall bool begin_element(wstring name) pure;
			visible vcall bool end_element() pure;
			visible vcall bool push_element(wstring name, wstring value) pure;
			visible vcall bool push_element(wstring element) pure;
			visible vcall bool push_data(wstring value) pure;
			visible vcall bool push_value(wstring value) pure;
			visible vcall bool push_attribute(wstring name, wstring value) pure;
			visible vcall bool push_comment(wstring value) pure;

	
			visible vcall void parse(raw_str_t text)pure;
			template<typename T> inline void parse(safe_str<T> const& text) { parse(raw_str(text)); }
			template<text::encoding_enum ENCODING> inline void parse(strings::string_base<ENCODING> const& text) { parse(text.is_empty() ? raw_str() : raw_str(text->cstr())); }

		ANG_END_INTERFACE()
	}

}

namespace ang
{

	template<> class LINK intf_wrapper<xml::ixml_node>
	{
	public:
		typedef xml::ixml_node type;

	private:
		xml::ixml_node* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(xml::ixml_node*);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		intf_wrapper(std::nullptr_t const&);
		~intf_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::ixml_node* get(void)const;
		void set(xml::ixml_node*);
		xml::ixml_node ** addres_of(void);


	public:
		intf_wrapper& operator = (xml::ixml_node*);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<xml::ixml_node> operator & (void);
		xml::ixml_node * operator -> (void);
		xml::ixml_node const* operator -> (void)const;

		operator xml::ixml_node * (void);
		operator xml::ixml_node const* (void)const;
		operator xml::xml_text_t()const;

		xml::ixml_node_t operator[](raw_str_t)const;
		template<typename T> xml::ixml_node_t operator[](safe_str<T> str)const;
	};

	template<> class LINK intf_wrapper<xml::ixml_store>
	{
	public:
		typedef xml::ixml_store type;

	private:
		xml::ixml_store* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(xml::ixml_store*);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		intf_wrapper(std::nullptr_t const&);
		~intf_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::ixml_store* get(void)const;
		void set(xml::ixml_store*);
		xml::ixml_store ** addres_of(void);

		xml::xml_forward_iterator_t begin() {
			return _ptr ? _ptr->begin() : xml::xml_forward_iterator_t(null);
		}
		xml::xml_forward_iterator_t end() {
			return _ptr ? _ptr->end() : xml::xml_forward_iterator_t(null);
		}

		xml::xml_const_forward_iterator_t begin()const {
			return _ptr ? ((xml::ixml_store const*)_ptr)->begin() : xml::xml_const_forward_iterator_t(null);
		}
		xml::xml_const_forward_iterator_t end()const {
			return _ptr ? ((xml::ixml_store const*)_ptr)->end() : xml::xml_const_forward_iterator_t(null);
		}

	public:
		intf_wrapper& operator = (xml::ixml_store*);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<xml::ixml_store> operator & (void);
		xml::ixml_store * operator -> (void);
		xml::ixml_store const* operator -> (void)const;

		operator xml::ixml_store * (void);
		operator xml::ixml_store const* (void)const;

		xml::ixml_node_t operator[](raw_str_t)const;
		template<typename T> xml::ixml_node_t operator[](safe_str<T> str)const;
	};

	template<> class LINK object_wrapper<xml::xml_text>
	{
	public:
		typedef xml::xml_text type;

	private:
		xml::xml_text* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(xml::xml_text*);
		object_wrapper(ang::nullptr_t const&);
		//template<typename T> object_wrapper(T const* str);
		template<typename T> object_wrapper(safe_str<T> const& str);
		template<text::encoding_enum OTHER_ENCODING> object_wrapper(object_wrapper<strings::string_buffer<OTHER_ENCODING>> const& str);
		template<typename T, wsize N> object_wrapper(const T(&ar)[N]);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_text* get(void)const;
		void set(xml::xml_text*);
		xml::xml_text ** addres_of(void);

	public:
		object_wrapper& operator = (ang::nullptr_t const&);
		object_wrapper& operator = (xml::xml_text*);
		object_wrapper& operator = (object_wrapper<xml::xml_text> &&);
		object_wrapper& operator = (object_wrapper<xml::xml_text> const&);
		template<typename T> object_wrapper& operator = (safe_str<T> const& str);
		template<typename T, wsize N> object_wrapper& operator = (const T(&ar)[N]);
		template<text::encoding_enum OTHER_ENCODING> object_wrapper& operator = (object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);
		template<typename T> object_wrapper& operator += (safe_str<T> const& str);
		template<typename T, wsize N> object_wrapper& operator += (const T(&ar)[N]);
		template<text::encoding_enum OTHER_ENCODING> object_wrapper& operator += (object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);
		inline operator objptr()const;
		object_wrapper_ptr<xml::xml_text> operator & (void);
		xml::xml_text * operator -> (void);
		xml::xml_text const* operator -> (void)const;
		explicit operator xml::xml_text * (void);
		explicit operator xml::xml_text const* (void)const;
		operator wstr_t(void);
		operator cwstr_t(void)const;
		operator wstring (void)const;
		wchar& operator [] (int);
		wchar operator [] (int)const;
		template<text::encoding_enum OTHER_ENCODING>

		friend object_wrapper<xml::xml_text> operator +
			(object_wrapper<xml::xml_text> const&, object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);

		template<typename T> friend object_wrapper<xml::xml_text> operator +
			(object_wrapper<xml::xml_text> const&, safe_str<T> const&);

		template<typename T> friend object_wrapper<xml::xml_text> operator +
			(safe_str<T> const&, object_wrapper<xml::xml_text> const&);

		template<text::encoding_enum OTHER_ENCODING> friend object_wrapper<xml::xml_text>& operator <<
			(object_wrapper<xml::xml_text>&, object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);

		template<typename T> friend object_wrapper<xml::xml_text>& operator <<
			(object_wrapper<xml::xml_text>&, safe_str<T> const&);
	};

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
		operator xml::xml_text_t()const;
		operator xml::ixml_node_t()const;

		xml::ixml_node_t operator[](raw_str_t)const;
		template<typename T> xml::ixml_node_t operator[](safe_str<T> str)const;
	};

	template<> class LINK intf_wrapper<xml::ixml_document>
	{
	public:
		typedef xml::ixml_document type;

	private:
		xml::ixml_document* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(xml::ixml_document*);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		intf_wrapper(std::nullptr_t const&);
		~intf_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::ixml_document* get(void)const;
		void set(xml::ixml_document*);
		xml::ixml_document ** addres_of(void);

		xml::xml_forward_iterator_t begin() {
			return _ptr ? _ptr->begin() : xml::xml_forward_iterator_t(null);
		}
		xml::xml_forward_iterator_t end() {
			return _ptr ? _ptr->end() : xml::xml_forward_iterator_t(null);
		}

		xml::xml_const_forward_iterator_t begin()const {
			return _ptr ? ((xml::ixml_document const*)_ptr)->begin() : xml::xml_const_forward_iterator_t(null);
		}
		xml::xml_const_forward_iterator_t end()const {
			return _ptr ? ((xml::ixml_document const*)_ptr)->end() : xml::xml_const_forward_iterator_t(null);
		}

	public:
		intf_wrapper& operator = (xml::ixml_document*);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<xml::ixml_document> operator & (void);
		xml::ixml_document * operator -> (void);
		xml::ixml_document const* operator -> (void)const;

		operator xml::ixml_document * (void);
		operator xml::ixml_document const* (void)const;

		xml::ixml_node_t operator[](raw_str_t)const;
		template<typename T> xml::ixml_node_t operator[](safe_str<T> str)const;
	};


	template<> class LINK object_wrapper<xml::xml_element>
	{
	public:
		typedef xml::xml_element type;

	private:
		xml::xml_element* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_element*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_element* get(void)const;
		void set(xml::xml_element*);
		xml::xml_element ** addres_of(void);


	public:
		object_wrapper& operator = (xml::xml_element*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<xml::xml_element> operator & (void);
		xml::xml_element * operator -> (void);
		xml::xml_element const* operator -> (void)const;

		operator xml::xml_element * (void);
		operator xml::xml_element const* (void)const;
		operator xml::xml_text_t()const;
		operator xml::xml_node_t()const;
		operator xml::ixml_node_t()const;

		xml::ixml_node_t operator[](raw_str_t)const;
		template<typename T> xml::ixml_node_t operator[](safe_str<T> str)const { return operator[](raw_str(str)); }
	};

	template<> class LINK object_wrapper<xml::xml_comment>
	{
	public:
		typedef xml::xml_comment type;

	private:
		xml::xml_comment* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_comment*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_comment* get(void)const;
		void set(xml::xml_comment*);
		xml::xml_comment ** addres_of(void);


	public:
		object_wrapper& operator = (xml::xml_comment*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<xml::xml_comment> operator & (void);
		xml::xml_comment * operator -> (void);
		xml::xml_comment const* operator -> (void)const;

		operator xml::xml_comment * (void);
		operator xml::xml_comment const* (void)const;
		operator xml::xml_text_t()const;
		operator xml::xml_node_t()const;
		operator xml::ixml_node_t()const;
	};

	template<> class LINK object_wrapper<xml::xml_attribute>
	{
	public:
		typedef xml::xml_attribute type;

	private:
		xml::xml_attribute* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_attribute*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_attribute* get(void)const;
		void set(xml::xml_attribute*);
		xml::xml_attribute ** addres_of(void);


	public:
		object_wrapper& operator = (xml::xml_attribute*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<xml::xml_attribute> operator & (void);
		xml::xml_attribute * operator -> (void);
		xml::xml_attribute const* operator -> (void)const;

		operator xml::xml_attribute * (void);
		operator xml::xml_attribute const* (void)const;
		operator xml::xml_text_t()const;
		operator xml::xml_node_t()const;
		operator xml::ixml_node_t()const;
	};

	template<> class LINK object_wrapper<xml::xml_header>
	{
	public:
		typedef xml::xml_header type;

	private:
		xml::xml_header* _ptr;

	public:
		object_wrapper();
		object_wrapper(xml::xml_header*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		xml::xml_header* get(void)const;
		void set(xml::xml_header*);
		xml::xml_header ** addres_of(void);


	public:
		object_wrapper& operator = (xml::xml_header*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<xml::xml_header> operator & (void);
		xml::xml_header * operator -> (void);
		xml::xml_header const* operator -> (void)const;

		operator xml::xml_header * (void);
		operator xml::xml_header const* (void)const;
		operator xml::xml_node_t()const;
		operator xml::ixml_node_t()const;

		xml::xml_text_t operator[](raw_str_t)const;
		template<typename T> xml::xml_text_t operator[](safe_str<T> str)const;
	};

	namespace xml
	{

		class LINK xml_text
			: public strings::string_buffer<text::encoding::unicode>
			, public ixml_node
		{
		private:
				mutable weak_ptr<ixml_node> _xml_parent;
				mutable weak_ptr<ixml_document> _xml_parent_doc;

		public:
			xml_text();
			xml_text(xml_text const*);
			xml_text(xml_text const&);

			xml_text(wstring &&);
			template<xml_encoding ENCODING>
			xml_text(strings::string_base<ENCODING> const& other)
				: strings::string_buffer<xml_encoding::unicode>(other.get())
				, _xml_parent(null) {
			}
			template<typename T>
			xml_text(safe_str<T> const& other)
				: strings::string_buffer<xml_encoding::unicode>(other)
				, _xml_parent(null) {
			}

		public: //Overrides
			ANG_DECLARE_INTERFACE();

			xml_type_t xml_type()const override;
			bool xml_is_type_of(xml_type_t)const override;
			wstring& xml_print(wstring& stream, const xml_format_t& flag, ushort level = 0)const override;
			ixml_node_t xml_clone(ixml_document_t)const override;
			ixml_node_t xml_parent()const override;
			ixml_document_t xml_parent_doc()const override;

			void xml_parent(ixml_node_t);
			void xml_parent_doc(ixml_document_t);

			template<typename T> T xml_as()const {
				throw_exception(except_code::unsupported); return T();
			}

		private:
			ixml_node_t xml_first_child()const override;
			ixml_node_t xml_last_child()const override;
			ixml_node_t xml_prev_sibling()const override;
			ixml_node_t xml_next_sibling()const override;

			virtual bool xml_has_name()const override;
			virtual bool xml_has_value()const override;
			virtual bool xml_has_children()const override;
			virtual bool xml_has_attributes()const override;

			virtual xml_text_t xml_name()const override;
			virtual xml_text_t xml_value()const override;
			virtual ixml_store_t xml_children()const override;
			virtual ixml_attributes_t xml_attributes()const override;

			
			virtual void xml_prev_sibling(ixml_node_t) override;
			virtual void xml_next_sibling(ixml_node_t) override;

		protected:
			virtual~xml_text();
		};

		class LINK xml_pcdata
			: public xml_text
		{
		public:
			xml_pcdata();
			xml_pcdata(xml_pcdata const*);
			xml_pcdata(xml_pcdata const&);

			xml_pcdata(wstring &&);
			xml_pcdata(wstring const&);

		public: //Overrides
			ANG_DECLARE_INTERFACE();

			xml_type_t xml_type()const override;
			bool xml_is_type_of(xml_type_t)const override;
			ixml_node_t xml_clone(ixml_document_t)const override;
			wstring& xml_print(wstring& stream, const xml_format_t& flag, ushort level = 0)const override;

		protected:
			virtual~xml_pcdata();
		};

		class LINK xml_cdata
			: public xml_text
		{
		public:
			xml_cdata();
			xml_cdata(xml_cdata const*);
			xml_cdata(xml_cdata const&);

			xml_cdata(wstring &&);
			xml_cdata(wstring const&);

		public: //Overrides
			ANG_DECLARE_INTERFACE();

			xml_type_t xml_type()const override;
			ixml_node_t xml_clone(ixml_document_t)const override;
			bool xml_is_type_of(xml_type_t)const override;

		protected:
			virtual~xml_cdata();
		};
	}
}


namespace ang
{

	namespace xml
	{
		class LINK xml_node
			: public object
			, public ixml_node
		{
		public:
			friend xml_document;

			class xml_store
				: public object
				, public ixml_store
			{
			protected:
				friend xml_node;
				const xml_type_t _xml_type;
				wsize _count;
				xml_node* _xml_first;
				xml_node* _xml_last;
				mutable weak_ptr<ixml_node> _xml_parent;

			public:
				typedef ixml_node* node_ptr_t;
				typedef collections::ienum<ixml_node> ienum_t;

			protected:
				xml_store(xml_node_t, xml_type_t);
				virtual~xml_store();

			public:
				ANG_DECLARE_INTERFACE();
				virtual void clean();

				xml_type_t xml_type()const override;
				bool xml_is_type_of(xml_type_t)const override;
				wstring& xml_print(wstring& stream, const xml_format_t& flag, ushort level = 0)const override;
				//streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;
				ixml_node_t xml_parent()const;
				ixml_document_t xml_parent_doc()const;

			public: //ienum overrides
				wsize counter()const override;

				xml::ixml_node_t at(xml_base_iterator_t const&) override;
				bool increase(xml_base_iterator_t&)const override;
				bool increase(xml_base_iterator_t&, int offset)const override;
				bool decrease(xml_base_iterator_t&)const override;
				bool decrease(xml_base_iterator_t&, int offset)const override;

				xml_forward_iterator_t begin() override;
				xml_forward_iterator_t end() override;
				xml_const_forward_iterator_t begin()const override;
				xml_const_forward_iterator_t end()const override;

				xml_backward_iterator_t rbegin() override;
				xml_backward_iterator_t rend() override;
				xml_const_backward_iterator_t rbegin()const override;
				xml_const_backward_iterator_t rend()const override;

			public: //icollection overrides
				xml_iterator_t find(raw_str_t, bool invert = false)const override;
				xml_iterator_t find(raw_str_t, xml_iterator_t next_to, bool invert = false)const override;

			public:
				virtual bool push(ixml_node_t node, bool last = true);
				virtual bool insert(ixml_node_t node, xml_iterator_t at, bool nextTo = true);
				virtual bool pop(ixml_node_ptr_t out = null, bool last = true);
				virtual bool pop_at(xml_iterator_t it, xml::ixml_node_ptr_t out = null);

			protected:
				ixml_node* xml_first()const;
				ixml_node* xml_last()const;
				void xml_first(ixml_node*);
				void xml_last(ixml_node*);
			};

		private:
			const xml_type_t _xml_type;
			mutable weak_ptr<ixml_node> _xml_parent;
			mutable weak_ptr<ixml_document> _xml_parent_doc;

			xml_node* _xml_prev;
			xml_node* _xml_next;

		protected:
			xml_text_t _xml_name;
			ixml_store_t _xml_attributes;
			ixml_object_t _xml_content;

		protected:
			xml_node(ixml_document_t, xml_type_t);
			virtual~xml_node();

		public:
			ixml_node_t xml_parent()const override;
			ixml_document_t xml_parent_doc()const override;
			ixml_node_t xml_first_child()const override;
			ixml_node_t xml_last_child()const override;
			ixml_node_t xml_prev_sibling()const override;
			ixml_node_t xml_next_sibling()const override;

		protected:
			void xml_parent(ixml_node_t) override;
			void xml_prev_sibling(ixml_node_t) override;
			void xml_next_sibling(ixml_node_t) override;

		public: //Overrides
			ANG_DECLARE_INTERFACE();
			virtual void clean();

			xml_type_t xml_type()const override;
			bool xml_is_type_of(xml_type_t)const override;
			wstring& xml_print(wstring& stream, const xml_format_t& flag, ushort level = 0)const override;

			virtual bool xml_has_name()const override;
			virtual bool xml_has_value()const override;
			virtual bool xml_has_children()const override;
			virtual bool xml_has_attributes()const override;

			virtual xml_text_t xml_name()const override;
			virtual xml_text_t xml_value()const override;
			virtual ixml_store_t xml_children()const override;
			virtual ixml_attributes_t xml_attributes()const override;

		protected:
			bool push_name(wstring value);
			bool push_data(wstring);
			bool push_value(wstring);
			xml_iterator_t push_attribute(xml_attribute_t att);
			xml_iterator_t push_attribute(xml_attribute_t att, xml_iterator_t next);
			bool push_attributes(ixml_store_t attributes);
			xml_iterator_t push_child(xml_node_t element);
			xml_iterator_t push_child(xml_node_t element, xml_iterator_t next);
			bool push_children(ixml_store_t children);

		};


		class LINK xml_attribute final
			: public xml_node
		{
		public:
			static xml_attribute_t create_new(const xml_attribute&);
			static xml_attribute_t create_new(ixml_document_t, const xml_attribute*);
			static xml_attribute_t create_new(ixml_document_t, wstring, wstring);

		protected:
			xml_attribute(ixml_document_t);

		public:
			ANG_DECLARE_INTERFACE();

			ixml_node_t xml_clone(ixml_document_t)const override;
			bool xml_has_name()const override;
			bool xml_has_value()const override;

		private:
			using xml_node::xml_has_children;
			using xml_node::xml_has_attributes;

			using xml_node::xml_children;
			using xml_node::xml_attributes;

			using xml_node::xml_parent;
			using xml_node::xml_first_child;
			using xml_node::xml_last_child;
			using xml_node::xml_prev_sibling;
			using xml_node::xml_next_sibling;

			virtual~xml_attribute();
			friend xml_document;
		};

		class LINK xml_comment final
			: public xml_node
		{
		public:
			static xml_comment_t create_new(const xml_comment&);
			static xml_comment_t create_new(ixml_document_t, const xml_comment*);
			static xml_comment_t create_new(ixml_document_t, wstring);

		protected:
			xml_comment(ixml_document_t);

		public:
			ANG_DECLARE_INTERFACE();

			ixml_node_t xml_clone(ixml_document_t)const override;
			bool xml_has_value()const override;

		private:
			using xml_node::xml_has_name;
			using xml_node::xml_has_children;
			using xml_node::xml_has_attributes;

			using xml_node::xml_name;
			using xml_node::xml_children;
			using xml_node::xml_attributes;

			using xml_node::xml_parent;
			using xml_node::xml_first_child;
			using xml_node::xml_last_child;
			using xml_node::xml_prev_sibling;
			using xml_node::xml_next_sibling;

			virtual~xml_comment();
			friend xml_document;
		};

		class LINK xml_element final
			: public xml_node
		{
		public:
			static xml_element_t create_new(const xml_element&);
			static xml_element_t create_new(ixml_document_t, const xml_element*);
			static xml_element_t create_new(ixml_document_t, wstring);
			static xml_element_t create_new(ixml_document_t, wstring, wstring);

		protected:
			xml_element(ixml_document_t);

		public:
			ANG_DECLARE_INTERFACE();

			ixml_node_t xml_clone(ixml_document_t)const override;
			bool xml_has_name()const override;
			bool xml_has_value()const override;
			bool xml_has_children()const override;
			bool xml_has_attributes()const override;

		private:
			using xml_node::xml_parent;
			using xml_node::xml_prev_sibling;
			using xml_node::xml_next_sibling;

			virtual~xml_element();
			friend xml_document;
		};

		class LINK xml_header final
			: public xml_node
		{
		public:
			static xml_header_t create_new(const xml_header&);
			static xml_header_t create_new(ixml_document_t, const xml_header*);
			static xml_header_t create_new(ixml_document_t, wstring version = "1.0"_s, xml_encoding_t encoding = xml_encoding::utf8, bool standalone = true);

		protected:
			xml_header(ixml_document_t);

		public: //Overrides
			ANG_DECLARE_INTERFACE();
			ixml_node_t xml_clone(ixml_document_t)const override;

		public:
			wstring version()const;
			xml_encoding encoding()const;
			bool is_stand_alone()const;

			void version(wstring);
			void encoding(xml_encoding_t);
			void is_stand_alone(bool);

			void encoding(wstring);
			void is_stand_alone(wstring);

		private:
			using xml_node::xml_has_name;
			using xml_node::xml_has_value;
			using xml_node::xml_has_children;
			using xml_node::xml_has_attributes;

			using xml_node::xml_name;
			using xml_node::xml_value;
			using xml_node::xml_children;

			using xml_node::xml_parent;
			using xml_node::xml_first_child;
			using xml_node::xml_last_child;
			using xml_node::xml_prev_sibling;
			using xml_node::xml_next_sibling;

			virtual~xml_header();
			friend xml_document;
		};


		class LINK ixml_attributes
		{
		public:
			typedef xml::ixml_store type;

		private:
			xml::ixml_store* _ptr;

		public:
			ixml_attributes();
			ixml_attributes(xml::ixml_store*);
			ixml_attributes(ixml_attributes_t &&);
			ixml_attributes(ixml_attributes_t const&);
			ixml_attributes(std::nullptr_t const&);
			~ixml_attributes();

		public:
			void clean();
			void clean_unsafe();
			bool is_empty()const;
			xml::ixml_store* get(void)const;
			void set(xml::ixml_store*);
			xml::ixml_store ** addres_of(void);

			xml::xml_forward_iterator_t begin() {
				return _ptr ? _ptr->begin() : xml::xml_forward_iterator_t(null);
			}
			xml::xml_forward_iterator_t end() {
				return _ptr ? _ptr->end() : xml::xml_forward_iterator_t(null);
			}

			xml::xml_const_forward_iterator_t begin()const {
				return _ptr ? ((xml::ixml_store const*)_ptr)->begin() : xml::xml_const_forward_iterator_t(null);
			}
			xml::xml_const_forward_iterator_t end()const {
				return _ptr ? ((xml::ixml_store const*)_ptr)->end() : xml::xml_const_forward_iterator_t(null);
			}

		public:
			ixml_attributes_t& operator = (xml::ixml_store*);
			ixml_attributes_t& operator = (ixml_attributes_t &&);
			ixml_attributes_t& operator = (ixml_attributes_t const&);

			//intf_wrapper_ptr<xml::ixml_store> operator & (void);
			xml::ixml_store * operator -> (void);
			xml::ixml_store const* operator -> (void)const;

			operator xml::ixml_store * (void);
			operator xml::ixml_store const* (void)const;

			xml::xml_text_t operator[](raw_str_t)const;
			template<typename T> xml::xml_text_t operator[](safe_str<T> str)const { return operator[](raw_str(str)); }
		};
	}
}




namespace ang
{
	namespace xml
	{
		class LINK xml_document
			: public object
			, public ixml_document
		{
		private:
			class LINK xml_document_iterator
			{
			protected:
				mutable weak_ptr<ixml_document> _xml_doc;
				ixml_node_t	_xml_current;
				ixml_node_t	_xml_child;

			public:
				xml_document_iterator();
				xml_document_iterator(xml_document*);
				xml_document_iterator(const xml_document_iterator&);
				~xml_document_iterator();

			private:
				ixml_node_t xml_current()const;
				ixml_node_t xml_child()const;
				void xml_current(ixml_node_t);
				void xml_child(ixml_node_t);

			public: //Properties
				void xml_parent(xml_document_t);
				ixml_document_t xml_parent()const;

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

				friend xml_document;
			};

			wsize _count;
			xml_node* _xml_first;
			xml_node* _xml_root;
			xml_node* _xml_last;
			xml_document_iterator _current;
			string _last_parsing_error;

		public:
			xml_document();
			xml_document(xml_document const*);

		protected:
		
			virtual~xml_document();

		public:
			ANG_DECLARE_INTERFACE();

			virtual xml_type_t xml_type()const override;
			virtual bool xml_is_type_of(xml_type_t)const override;
			virtual wstring& xml_print(wstring& stream, const xml_format_t& flag, ushort level = 0)const override;

			virtual wsize counter()const override;
			virtual xml::ixml_node_t at(xml_base_iterator_t const&) override;
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

			virtual xml_header_t xml_header()const override;
			virtual ixml_node_t xml_data_type()const override;
			virtual xml_element_t xml_root_element()const override;
			virtual ixml_document_t xml_clone()const override;

			virtual xml_iterator_t find(raw_str_t, bool invert = false)const override;
			virtual xml_iterator_t find(raw_str_t, xml_iterator_t next_to, bool invert = false)const override;

			virtual xml_iterator_t xml_current()const override;
			virtual xml_element_t xml_current_element()const override;

			virtual bool move_to(xml_iterator_t current) override;
			virtual bool move_to_child(xml_iterator_t child) override;

			virtual bool move_up() override;
			virtual bool move_down() override;
			virtual bool move_forward() override;
			virtual bool move_backward() override;

			virtual void push_default_header() override;
			virtual void push_header(wstring version, xml_encoding_t encoding, bool standalone) override;
			virtual bool begin_element(wstring name) override;
			virtual bool end_element() override;
			virtual bool push_element(wstring name, wstring value) override;
			virtual bool push_element(wstring element) override;
			virtual bool push_data(wstring value) override;
			virtual bool push_value(wstring value) override;
			virtual bool push_attribute(wstring name, wstring value) override;
			virtual bool push_comment(wstring value) override;


			virtual void parse(raw_str_t text)override;
			using ixml_document::parse;

		public: //Properties
			bool is_empty()const;
			void clean();

		protected:
			static bool decode_header(text::string_util& text, windex& idx, wstring& version, xml_encoding_t& encoding, bool& standalone);
			static bool decode_dtd(xml_document_t doc, text::string_util& text, windex& idx);
			static bool decode_elements(xml_document_t doc, text::string_util& text, windex& idx);

			ixml_node* xml_first()const;
			ixml_node* xml_root()const;
			ixml_node* xml_last()const;

			void insert_next_to(ixml_node*, ixml_node*);
			void insert_prev_to(ixml_node*, ixml_node*);
			void insert_first(ixml_node*);
			void insert_last(ixml_node*);
			void xml_first(ixml_node_t);
			void xml_root(ixml_node_t);
			void xml_last(ixml_node_t);
		};
	}
}

namespace ang
{

	namespace collections
	{
		inline iterator<xml::ixml_node>::operator xml::ixml_node*()const {
#ifdef DEBUG_SAFE_CODE
			if (_parent == null) throw(exception_t(except_code::invalid_memory));
#endif
			return this->_parent->at(*this).get();
		}
		inline xml::ixml_node_t iterator<xml::ixml_node>::operator -> ()const {
#ifdef DEBUG_SAFE_CODE
			if (_parent == null) throw(exception_t(except_code::invalid_memory));
#endif
			return this->_parent->at(*this);
		}
		inline xml::ixml_node_t iterator<xml::ixml_node>::operator * ()const {
#ifdef DEBUG_SAFE_CODE
			if (_parent == null) throw(exception_t(except_code::invalid_memory));
#endif
			return this->_parent->at(*this);
		}

		inline iterator<const xml::ixml_node>::operator xml::ixml_node const*()const {
#ifdef DEBUG_SAFE_CODE
			if (_parent == null) throw(exception_t(except_code::invalid_memory));
#endif
			return this->_parent->at(*this).get();
		}
		inline const xml::ixml_node_t iterator<const xml::ixml_node>::operator -> ()const {
#ifdef DEBUG_SAFE_CODE
			if (_parent == null) throw(exception_t(except_code::invalid_memory));
#endif
			return this->_parent->at(*this);
		}
		inline const xml::ixml_node_t iterator<const xml::ixml_node>::operator * ()const {
#ifdef DEBUG_SAFE_CODE
			if (_parent == null) throw(exception_t(except_code::invalid_memory));
#endif
			return this->_parent->at(*this);
		}
	}
	namespace xml
	{

		template<> typename smart_ptr_type<xml_text>::smart_ptr_t  LINK ixml_object::xml_as<xml_text>();
		template<> typename smart_ptr_type<ixml_node>::smart_ptr_t  LINK ixml_object::xml_as<ixml_node>();
		template<> typename smart_ptr_type<xml_node>::smart_ptr_t  LINK ixml_object::xml_as<xml_node>();
		template<> typename smart_ptr_type<xml_attribute>::smart_ptr_t  LINK ixml_object::xml_as<xml_attribute>();
		template<> typename smart_ptr_type<xml_header>::smart_ptr_t  LINK ixml_object::xml_as<xml_header>();
		template<> typename smart_ptr_type<xml_comment>::smart_ptr_t  LINK ixml_object::xml_as<xml_comment>();
		template<> typename smart_ptr_type<xml_element>::smart_ptr_t  LINK ixml_object::xml_as<xml_element>();
		template<> typename smart_ptr_type<ixml_store>::smart_ptr_t  LINK ixml_object::xml_as<ixml_store>();
		template<> typename smart_ptr_type<xml_node::xml_store>::smart_ptr_t  LINK ixml_object::xml_as<xml_node::xml_store>();
		template<> typename smart_ptr_type<ixml_document>::smart_ptr_t  LINK ixml_object::xml_as<ixml_document>();
		template<> typename smart_ptr_type<xml_document>::smart_ptr_t  LINK ixml_object::xml_as<xml_document>();
		//template<> typename smart_ptr_type<xml_attribute_list>::smart_ptr_t  LINK ixml_object::xml_as<xml_attribute_list>();
		//template<> typename smart_ptr_type<xml_element_list>::smart_ptr_t  LINK ixml_object::xml_as<xml_element_list>();

		template<> LINK cstr_t xml::xml_text::xml_as<cstr_t>()const;
		template<> LINK cwstr_t xml::xml_text::xml_as<cwstr_t>()const;
		template<> LINK string xml::xml_text::xml_as<string>()const;
		template<> LINK wstring xml::xml_text::xml_as<wstring>()const;
		template<> LINK mstring xml::xml_text::xml_as<mstring>()const;

		template<> LINK short xml::xml_text::xml_as<short>()const;
		template<> LINK ushort xml::xml_text::xml_as<ushort>()const;
		template<> LINK int xml::xml_text::xml_as<int>()const;
		template<> LINK uint xml::xml_text::xml_as<uint>()const;
		template<> LINK long xml::xml_text::xml_as<long>()const;
		template<> LINK ulong xml::xml_text::xml_as<ulong>()const;
		template<> LINK long64 xml::xml_text::xml_as<long64>()const;
		template<> LINK ulong64 xml::xml_text::xml_as<ulong64>()const;
		template<> LINK float xml::xml_text::xml_as<float>()const;
		template<> LINK double xml::xml_text::xml_as<double>()const;
		template<> LINK bool xml::xml_text::xml_as<bool>()const;
		template<> LINK xml::xml_encoding_t xml::xml_text::xml_as<xml::xml_encoding_t>()const;

	}

	template<xml::xml_encoding ENCODING>
	inline object_wrapper<xml::xml_text>::object_wrapper(strings::string_base<ENCODING> const& other) : _ptr(null) {
		set(new xml::xml_text(other.get()));
	}
	template<typename T>
	inline object_wrapper<xml::xml_text>::object_wrapper(safe_str<T> const& other) : _ptr(null) {
		set(new xml::xml_text(other));
	}

	template<typename T>
	inline xml::ixml_node_t intf_wrapper<xml::ixml_node>::operator[](safe_str<T> str)const {
		return operator[](raw_str_t(str));
	}

	template<typename T>
	inline xml::ixml_node_t intf_wrapper<xml::ixml_store>::operator[](safe_str<T> str)const {
		return operator[](raw_str_t(str));
	}


	template<typename T>
	inline xml::ixml_node_t intf_wrapper<xml::ixml_document>::operator[](safe_str<T> str)const {
		return operator[](raw_str_t(str));
	}

	template<typename T>
	inline xml::ixml_node_t object_wrapper<xml::xml_node>::operator[](safe_str<T> str)const {
		return operator[](raw_str_t(str));
	}

	template<typename T>
	inline xml::xml_text_t object_wrapper<xml::xml_header>::operator[](safe_str<T> str)const {
		return operator[](raw_str_t(str));
	}

}

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANGXML_HPP__