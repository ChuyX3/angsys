#ifndef __ANG_DOM_XML_H__
#error ...
#elif !defined __ANG_DOM_XML_XML_TEXT_H__
#define __ANG_DOM_XML_XML_TEXT_H__

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			template<xml_encoding E, template<typename>class A>
			class xml_text
				: public strings::basic_string_buffer<E, A>
				, public ixml_text
			{
			private:
				mutable weak_ptr<ixml_node> _xml_parent;
				mutable weak_ptr<ixml_document> _xml_parent_doc;
				ixml_node* _xml_prev;
				ixml_node* _xml_next;

			public:
				xml_text();
				xml_text(xml_text const*);
				xml_text(strings::basic_string<E, A> &&);

				template<xml_encoding E2, template<typename>class A2>
				xml_text(strings::basic_string<E2, A2> const& other)
					: strings::basic_string_buffer<E, A>(other.get())
					, _xml_parent(null) {
				}

				template<typename U, xml_encoding E2>
				xml_text(str_view<U, E2> const& other)
					: strings::basic_string_buffer<E, A>(other)
					, _xml_parent(null) {
				}

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				xml_type_t xml_type()const override;
				bool xml_is_type_of(xml_type_t)const override;
				streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;
				ixml_node_t xml_clone(ixml_document_t)const override;
				ixml_node_t xml_parent()const override;
				ixml_document_t xml_parent_doc()const override;

				void xml_parent(ixml_node_t);
				void xml_parent_doc(ixml_document_t);

				text::itext_buffer_t xml_data()override;

				template<typename T> T xml_as()const {
					throw_exception(except_code::unsupported); return T();
				}

			private:
				virtual ixml_node_t xml_first_child()const override { return null; }
				virtual ixml_node_t xml_last_child()const override { return null; }
				virtual ixml_node_t xml_prev_sibling()const override { return null; }
				virtual ixml_node_t xml_next_sibling()const override { return null; }

				virtual bool xml_has_name()const override { return false; }
				virtual bool xml_has_value()const override { return true; }
				virtual bool xml_has_children()const override { return false; }
				virtual bool xml_has_attributes()const override { return false; }

				virtual ixml_text_t xml_name()const override { return null; }
				virtual ixml_text_t xml_value()const override { return this; }
				virtual ixml_collection_t xml_children()const override { return null; }
				virtual ixml_attributes_t xml_attributes()const override { return null; }

				virtual void xml_prev_sibling(ixml_node_t) override {}
				virtual void xml_next_sibling(ixml_node_t) override {}

			protected:
				virtual~xml_text();
			};


			template<xml_encoding E, template<typename>class A>
			class xml_pcdata : public xml_text<E, A>
			{
			public:
				xml_pcdata();
				xml_pcdata(xml_text const*);
				xml_pcdata(strings::basic_string<E, A> &&);

				template<xml_encoding E2, template<typename>class A2>
				xml_pcdata(strings::basic_string<E2, A2> const& other)
					: xml_text<E, A>(other.get()) {
				}

				template<typename U, xml_encoding E2>
				xml_pcdata(str_view<U, E2> const& other)
					: xml_text<E, A>(other) {
				}

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				xml_type_t xml_type()const override;
				bool xml_is_type_of(xml_type_t)const override;
				ixml_node_t xml_clone(ixml_document_t)const override;
				//streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;

			protected:
				virtual~xml_pcdata();
			};

			template<xml_encoding E, template<typename>class A>
			class xml_cdata : public xml_text<E, A>
			{
			public:
				xml_cdata();
				xml_cdata(xml_text const*);
				xml_cdata(strings::basic_string<E, A> &&);

				template<xml_encoding E2, template<typename>class A2>
				xml_cdata(strings::basic_string<E2, A2> const& other)
					: xml_text<E, A>(other.get()) {
				}

				template<typename U, xml_encoding E2>
				xml_cdata(str_view<U, E2> const& other)
					: xml_text<E, A>(other) {
				}

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				xml_type_t xml_type()const override;
				bool xml_is_type_of(xml_type_t)const override;
				ixml_node_t xml_clone(ixml_document_t)const override;
				streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;

			protected:
				virtual~xml_cdata();
			};
		}
	}
}


#endif//__ANG_DOM_XML_XML_TEXT_H__
