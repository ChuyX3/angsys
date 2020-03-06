#pragma once
namespace coffe
{
	namespace dom
	{
		namespace xml
		{
			template<typename EP> struct xml_text_factory_base;

			using xml_text_factory = xml_text_factory_base<value_pack_from_range<text::encoding, text::encoding::ascii, text::encoding::utf32_be>::type>;

			template<text::encoding E>
			class xml_text
				: public implement<xml_text<E>
				, iid("coffe::dom::xml_text")
				, ixml_text, text::istring>
				, public fast_string<E>
			{
			public:
				static constexpr text::encoding ENCODING = E;
				typedef text::text_encoder<ENCODING> encoder;
				typedef fast_string<E> fast_string;
				typedef typename text::char_type_by_encoding<ENCODING>::char_t type;
				typedef typename text::char_type_by_encoding<ENCODING>::str_t ptr_t;
				typedef typename text::char_type_by_encoding<ENCODING>::cstr_t cptr_t;
				typedef str_view<type, ENCODING> view_t;
				typedef str_view<const type, ENCODING> cview_t;

			public: /*constructors*/
				xml_text();
				xml_text(cstr_t);
				xml_text(xml_text &&);
				xml_text(fast_string &&);
				xml_text(xml_text const&);
				xml_text(fast_string const&);

				template<typename T, text::encoding E>
				xml_text(str_view<T, E> const& cstr) {
					fast_string::copy(cstr);
				}

				template<text::encoding E>
				xml_text(xml_text<E> const& cstr) {
					fast_string::copy(cstr.cstr());
				}

				template<text::encoding E>
				xml_text(text::fast_string<E> const& cstr) {
					fast_string::copy(cstr.cstr());
				}

			public: /*overrides*/
				virtual void dispose()override;
				virtual string to_string()const override;
				virtual string to_string(text::text_format_t)const override;

				virtual streams::itext_output_stream_t& print(streams::itext_output_stream_t& stream, const xml_format_t& flag = xml_format_t(xml_format::wrap_text_space) + xml_format::fix_entity, ushort level = 0)const override;

			public: /*istring overrides*/
				virtual text::encoding_t encoding()const override;
				virtual wsize length()const override;
				virtual pointer data()const override;
				virtual rtti_t const& char_type()const override;
				virtual str_t str(int = 0)override;
				virtual cstr_t cstr(int = 0)const override;
				virtual char32_t at(windex i, wsize* = null)const override;
				virtual int compare(cstr_t)const override;
				virtual int compare(string)const override;
				virtual windex compare_until(cstr_t)const override;
				virtual windex compare_until(string)const override;
				virtual windex find(cstr_t, windex start = 0, windex end = -1)const override;
				virtual windex find(string, windex start = 0, windex end = -1)const override;
				virtual windex find_reverse(cstr_t, windex start = -1, windex end = 0)const override;
				virtual windex find_reverse(string, windex start = -1, windex end = 0)const override;
				virtual string sub_string(windex start, windex end)const override;
				virtual vector<string> split(cstr_t)const override;
				virtual vector<string> split(string)const override;
				virtual void clear()override;
				virtual wsize copy(cstr_t)override;
				virtual wsize copy(string)override;
				virtual wsize append(cstr_t)override;
				virtual wsize append(string)override;
				virtual wsize insert(cstr_t, windex = -1)override;
				virtual wsize insert(string, windex = -1)override;
				virtual void format(cstr_t, var_args_t)override;
				virtual void format(string, var_args_t)override;
				virtual void append_format(cstr_t, var_args_t)override;
				virtual void append_format(string, var_args_t)override;

			};

			template<text::encoding... ES>
			struct xml_text_factory_base<value_pack<text::encoding, ES...>>
			{
				using create_text_type = ixml_text*(*)(cstr_t);
				using pack_type = value_pack<text::encoding, ES...>;
				static constexpr text::encoding FIRST = pack_type::get<0>();
				static constexpr wsize PACK_SIZE = value_pack_size<pack_type>::value;

				static ixml_text_t create_text(text::encoding e, cstr_t value) {
					wsize i = get_index(e);
					return i < PACK_SIZE ? s_create_text_array[i](value) : null;
				}
			private:
				static wsize get_index(text::encoding E) { return wsize(E) - wsize(FIRST); }

				template<text::encoding E>
				static ixml_text* create_text(cstr_t value) {
					return new xml_text<E>(value);
				}

				static constexpr create_text_type s_create_text_array[] = { &create_text<ES>... };
			};
		}
	}
}
