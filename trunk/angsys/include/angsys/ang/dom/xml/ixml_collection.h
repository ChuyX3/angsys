#ifndef __ANG_DOM_XML_H__
#error ...
#elif !defined __ANG_DOM_XML_IXML_COLLECTION_H__
#define __ANG_DOM_XML_IXML_COLLECTION_H__

namespace ang
{
	namespace collections
	{
		template<>
		ang_begin_interface(LINK ienum<dom::xml::ixml_node>)
			visible vcall wsize counter()const pure;
			visible vcall dom::xml::ixml_node_t at(base_iterator<dom::xml::ixml_node> const&) pure;
			visible vcall bool increase(base_iterator<dom::xml::ixml_node>&)const pure;
			visible vcall bool increase(base_iterator<dom::xml::ixml_node>&, int offset)const pure;
			visible vcall bool decrease(base_iterator<dom::xml::ixml_node>&)const pure;
			visible vcall bool decrease(base_iterator<dom::xml::ixml_node>&, int offset)const pure;

			visible vcall forward_iterator<dom::xml::ixml_node> begin() pure;
			visible vcall forward_iterator<dom::xml::ixml_node> end() pure;
			visible vcall const_forward_iterator<dom::xml::ixml_node> begin()const pure;
			visible vcall const_forward_iterator<dom::xml::ixml_node> end()const pure;
			visible vcall forward_iterator<dom::xml::ixml_node> last() pure;
			visible vcall const_forward_iterator<dom::xml::ixml_node> last()const pure;

			visible vcall backward_iterator<dom::xml::ixml_node> rbegin() pure;
			visible vcall backward_iterator<dom::xml::ixml_node> rend() pure;
			visible vcall const_backward_iterator<dom::xml::ixml_node> rbegin()const pure;
			visible vcall const_backward_iterator<dom::xml::ixml_node> rend()const pure;
		ang_end_interface();


		template<> class iterator<dom::xml::ixml_node> : public base_iterator<dom::xml::ixml_node>
		{
		public:
			iterator(base_iterator<dom::xml::ixml_node> const& it) : base_iterator<dom::xml::ixml_node>(it) {}
			iterator(ienum<dom::xml::ixml_node>* p = null, pointer cur = null, wsize offset = 0U)
				: base_iterator<dom::xml::ixml_node>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<dom::xml::ixml_node>& it) {
				base_iterator<dom::xml::ixml_node>::operator = (it);
				return*this;
			}

			inline operator dom::xml::ixml_node*()const;
			inline dom::xml::ixml_node_t operator -> ()const;
			inline dom::xml::ixml_node_t operator * ()const;
		};

		template<> class iterator<const dom::xml::ixml_node> : public base_iterator<dom::xml::ixml_node>
		{
		public:
			iterator(base_iterator<dom::xml::ixml_node> const& it) : base_iterator<dom::xml::ixml_node>(it) {}
			iterator(ienum<dom::xml::ixml_node>* p = null, pointer cur = null, wsize offset = 0U)
				: base_iterator<dom::xml::ixml_node>(p, cur, offset) {
			}

			iterator& operator = (const base_iterator<dom::xml::ixml_node>& it) {
				base_iterator<dom::xml::ixml_node>::operator = (it);
				return*this;
			}

			inline operator dom::xml::ixml_node const*()const;
			inline const dom::xml::ixml_node_t operator -> ()const;
			inline const dom::xml::ixml_node_t operator * ()const;
		};

	}

	namespace dom
	{
		namespace xml
		{
			ang_begin_interface(LINK ixml_collection, ixml_object, collections::ienum<ixml_node>)
				visible vcall ixml_node_t xml_parent()const pure;
				visible vcall ixml_document_t xml_parent_doc()const pure;
				visible vcall xml_iterator_t find(raw_str_t, bool invert = false)const pure;
				visible vcall xml_iterator_t find(raw_str_t, xml_iterator_t next_to, bool invert = false)const pure;
				template<typename T, text::encoding E> inline xml_iterator_t find(str_view<T, E> const& str, bool invert = false)const { find(raw_str(str), invert); }
				template<typename T, text::encoding E> inline xml_iterator_t find(str_view<T, E> const& str, xml_const_iterator_t next_to, bool invert = false)const { find(raw_str(str), next_to, invert); }
			ang_end_interface();
		}
	}

	ANG_BEGIN_INTF_WRAPPER(LINK, dom::xml::ixml_collection)
		inline dom::xml::xml_forward_iterator_t begin() {
			return m_ptr ? m_ptr->begin() : dom::xml::xml_forward_iterator_t(null);
		}
		inline dom::xml::xml_forward_iterator_t end() {
			return m_ptr ? m_ptr->end() : dom::xml::xml_forward_iterator_t(null);
		}
		inline dom::xml::xml_const_forward_iterator_t begin()const {
			return m_ptr ? ((dom::xml::ixml_collection const*)m_ptr)->begin() : dom::xml::xml_const_forward_iterator_t(null);
		}
		inline dom::xml::xml_const_forward_iterator_t end()const {
			return m_ptr ? ((dom::xml::ixml_collection const*)m_ptr)->end() : dom::xml::xml_const_forward_iterator_t(null);
		}
		operator dom::xml::ixml_string_t()const;
		dom::xml::ixml_node_t operator[](raw_str_t)const;
		template<typename T, text::encoding E> dom::xml::ixml_node_t operator[](str_view<T, E> str)const;
	ANG_END_INTF_WRAPPER();

}

#endif//__ANG_DOM_XML_IXML_COLLECTION_H__
