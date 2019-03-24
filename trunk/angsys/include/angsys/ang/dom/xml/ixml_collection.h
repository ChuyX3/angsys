#ifndef __ANG_DOM_XML_H__
#error ...
#elif !defined __ANG_DOM_XML_IXML_COLLECTION_H__
#define __ANG_DOM_XML_IXML_COLLECTION_H__

namespace ang
{
	namespace collections
	{
		template<>
		ang_begin_interface(LINK ienum<dom::xml::xml_node>)
			visible vcall wsize counter()const pure
			visible vcall dom::xml::xml_node_t at(base_iterator<dom::xml::xml_node> const&) pure
			visible vcall bool increase(base_iterator<dom::xml::xml_node>&)const pure
			visible vcall bool increase(base_iterator<dom::xml::xml_node>&, int offset)const pure
			visible vcall bool decrease(base_iterator<dom::xml::xml_node>&)const pure
			visible vcall bool decrease(base_iterator<dom::xml::xml_node>&, int offset)const pure

			visible vcall forward_iterator<dom::xml::xml_node> begin() pure
			visible vcall forward_iterator<dom::xml::xml_node> end() pure
			visible vcall const_forward_iterator<dom::xml::xml_node> begin()const pure
			visible vcall const_forward_iterator<dom::xml::xml_node> end()const pure
			visible vcall forward_iterator<dom::xml::xml_node> last() pure
			visible vcall const_forward_iterator<dom::xml::xml_node> last()const pure

			visible vcall backward_iterator<dom::xml::xml_node> rbegin() pure
			visible vcall backward_iterator<dom::xml::xml_node> rend() pure
			visible vcall const_backward_iterator<dom::xml::xml_node> rbegin()const pure
			visible vcall const_backward_iterator<dom::xml::xml_node> rend()const pure
		ang_end_interface();


		template<> class iterator<dom::xml::xml_node> : public base_iterator<dom::xml::xml_node>
		{
		public:
			iterator(base_iterator<dom::xml::xml_node> const& it) : base_iterator<dom::xml::xml_node>(it) {}
			iterator(ienum<dom::xml::xml_node>* p = null, pointer cur = null, wsize offset = 0U)
				: base_iterator<dom::xml::xml_node>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<dom::xml::xml_node>& it) {
				base_iterator<dom::xml::xml_node>::operator = (it);
				return*this;
			}

			inline operator dom::xml::xml_node*()const;
			inline dom::xml::xml_node_t operator -> ()const;
			inline dom::xml::xml_node_t operator * ()const;
		};

		template<> class iterator<const dom::xml::xml_node> : public base_iterator<dom::xml::xml_node>
		{
		public:
			iterator(base_iterator<dom::xml::xml_node> const& it) : base_iterator<dom::xml::xml_node>(it) {}
			iterator(ienum<dom::xml::xml_node>* p = null, pointer cur = null, wsize offset = 0U)
				: base_iterator<dom::xml::xml_node>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<dom::xml::xml_node>& it) {
				base_iterator<dom::xml::xml_node>::operator = (it);
				return*this;
			}

			inline operator dom::xml::xml_node const*()const;
			inline const dom::xml::xml_node_t operator -> ()const;
			inline const dom::xml::xml_node_t operator * ()const;
		};

	}

	namespace dom
	{
		namespace xml
		{
			ang_begin_interface(LINK ixml_collection, ixml_object, collections::ienum<xml_node>)
				visible vcall xml_node_t xml_parent()const pure
				visible vcall xml_document_t xml_parent_doc()const pure
				visible vcall xml_iterator_t find(cstr_t, bool invert = false)const pure
				visible vcall xml_iterator_t find(cstr_t, xml_iterator_t next_to, bool invert = false)const pure
			ang_end_interface();
		
		
			class LINK xml_attributes
			{
			public:
				typedef xml::ixml_collection type;

			private:
				xml::ixml_collection* m_ptr;

			public:
				xml_attributes();
				xml_attributes(xml::ixml_collection*);
				xml_attributes(xml_attributes_t &&);
				xml_attributes(xml_attributes_t const&);
				xml_attributes(std::nullptr_t const&);
				~xml_attributes();

			public:
				void reset();
				void reset_unsafe();
				bool is_empty()const;
				xml::ixml_collection* get(void)const;
				void set(xml::ixml_collection*);
				xml::ixml_collection ** addres_of(void);

				xml::xml_forward_iterator_t begin() {
					return m_ptr ? m_ptr->begin() : xml::xml_forward_iterator_t(null);
				}
				xml::xml_forward_iterator_t end() {
					return m_ptr ? m_ptr->end() : xml::xml_forward_iterator_t(null);
				}

				xml::xml_const_forward_iterator_t begin()const {
					return m_ptr ? ((xml::ixml_collection const*)m_ptr)->begin() : xml::xml_const_forward_iterator_t(null);
				}
				xml::xml_const_forward_iterator_t end()const {
					return m_ptr ? ((xml::ixml_collection const*)m_ptr)->end() : xml::xml_const_forward_iterator_t(null);
				}

				//xml_attribute_t xml_attribute(raw_str_t);
				//xml_namespace_t xml_namespace(raw_str_t);

				//template<typename T, xml_encoding E> xml_attribute_t xml_attribute(str_view<T, E> str)const { return xml_attribute(raw_str(str)); }
				//template<typename T, xml_encoding E> xml_namespace_t xml_namespace(str_view<T, E> str)const { return xml_namespace(raw_str(str)); }

			public:
				xml_attributes_t& operator = (xml::ixml_collection*);
				xml_attributes_t& operator = (xml_attributes_t &&);
				xml_attributes_t& operator = (xml_attributes_t const&);

				//intf_wrapper_ptr<xml::ixml_collection> operator & (void);
				xml::ixml_collection * operator -> (void);
				xml::ixml_collection const* operator -> (void)const;

				operator xml::ixml_collection * (void);
				operator xml::ixml_collection const* (void)const;

				xml_text_t operator[](cstr_t)const;
			};
		
		}
	}

	ANG_BEGIN_INTF_WRAPPER(LINK, dom::xml::ixml_collection)
		dom::xml::xml_forward_iterator_t begin();
		dom::xml::xml_forward_iterator_t end();
		dom::xml::xml_const_forward_iterator_t begin()const;
		dom::xml::xml_const_forward_iterator_t end()const;

		dom::xml::xml_backward_iterator_t rbegin();
		dom::xml::xml_backward_iterator_t rend();
		dom::xml::xml_const_backward_iterator_t rbegin()const;
		dom::xml::xml_const_backward_iterator_t rend()const;
		dom::xml::xml_node_t operator[](cstr_t cstr)const;
	ANG_END_INTF_WRAPPER();

}



#endif//__ANG_DOM_XML_IXML_COLLECTION_H__
