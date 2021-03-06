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

			class LINK xml_text
				: public smart<xml_text, ixml_object, text::istring>
			{
			public:
				friend xml_document;
				friend xml_node;

			private:
				mutable weak_ptr<xml_document> m_doc;

			protected:
				string m_value;
			
			public:
				xml_text(xml_document*);
				xml_text(xml_document*, string);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual void clear();

				xml_type_t xml_type()const override;
				bool xml_is_type_of(xml_type_t)const override;
				streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;

				template<typename T> T xml_as()const;

			public: //ixml_text overrides
				virtual xml_encoding_t encoding()const override;
				virtual wsize length()const override;
				virtual pointer data()const override;
				virtual rtti_t const& char_type()const override;
				virtual str_t str(int = 0)override;
				virtual cstr_t cstr(int = 0)const override;
				virtual char32_t at(windex, wsize* = null)const override;
				virtual int compare(cstr_t)const override;
				virtual windex compare_until(cstr_t)const override;
				virtual windex find(cstr_t, windex start = 0, windex end = -1)const override;
				virtual windex find_reverse(cstr_t, windex start = -1, windex end = 0)const override;
				virtual string sub_string(string_ptr_t, windex start, windex end)const override;
				virtual collections::ienum_ptr<string> split(cstr_t)const override;

				virtual void copy(cstr_t)override;
				virtual void move(string);
				void copy_cdata(cstr_t);
				void copy_pcdata(cstr_t);
			private: //ixml_text overrides
				virtual void concat(cstr_t)override;
				virtual void insert(windex, cstr_t)override;
				virtual void format(cstr_t, var_args_t)override;
				virtual void concat_format(cstr_t, var_args_t)override;
				virtual void insert_format(windex, cstr_t, var_args_t)override;

			private:
				virtual~xml_text();
			};
		}
	}

}


#endif//__ANG_DOM_XML_XML_TEXT_H__
