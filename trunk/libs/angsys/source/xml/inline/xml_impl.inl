#pragma once

template<typename T, ang::dom::xml::xml_encoding E>
inline int ang::dom::xml::xml_utils_impl<T,E>::xml_skip_space(ang::str_view<T, E> const& code, windex begin) {
	windex n = begin, c = begin;
	char32_t at;
	while (((at = text::to_char32<false, text::is_endian_swapped<E>::value>(code, c)) == U' ')
		|| (at == U'\n')
		|| (at == U'\t')
		|| (at == U'\r'))
		n = c;
	return int(n - begin);
}

template<typename T, ang::dom::xml::xml_encoding E>
inline int ang::dom::xml::xml_utils_impl<T, E>::xml_get_element_name(ang::str_view<T, E> const& code, windex begin) {
	windex n = begin, c = begin;
	char32_t at;
	while (((at = text::to_char32<false, text::is_endian_swapped<E>::value>(code, c)) != U' ')
		&& (at != U'\n')
		&& (at != U':')
		&& (at != U'\t')
		&& (at != U'\r')
		&& (at != U'/')
		&& (at != U'>')) n = c;
	return int(n - begin);
}

template<typename T, ang::dom::xml::xml_encoding E>
inline int ang::dom::xml::xml_utils_impl<T, E>::xml_get_attibute_name(ang::str_view<T, E> const& code, windex begin) {
	windex n = begin, c = begin;
	char32_t at;
	while (((at = text::to_char32<false, text::is_endian_swapped<E>::value>(code, c)) != U' ')
		&& (at != U'\n')
		&& (at != U'\t')
		&& (at != U'\r')
		&& (at != U'=')) n = c;
	return int(n - begin);
}

template<typename T, ang::dom::xml::xml_encoding E>
inline ang::error ang::dom::xml::xml_utils_impl<T, E>::xml_parse(ang::dom::xml::ixml_document_builder_t doc, ang::str_view<T, E> const& code_, windex& beg)
{
	fast_string<E> code, value, name, header;
	code.attach(code_);

	windex end;
	uint c = 0;

	xml_encoding_t encoding = E;
	fast_string<E> version ="1.0";

	do {
		windex idx;
		fast_string<E> header;
		
		idx = code.find("<?xml"_sv, beg, beg + 5);
		if (idx == invalid_index)
			break;
		
		end = code.find("?>"_sv, idx);
		if (end == invalid_index)
			break;
		
		code.sub_string(header, idx, end + 2);
		beg = end + 2;

		idx = header.find("version"_sv);
		if (idx != invalid_index)
		{
			idx = header.find("\""_sv, idx);
			idx++;
			end = header.find("\""_sv, idx);
			header.sub_string(version, idx, end);
		}

		idx = header.find("encoding"_sv, idx);
		if (idx != invalid_index)
		{
			idx = header.find("\""_sv, idx);
			idx++;
			end = header.find("\""_sv, idx);
			header.sub_string(value, idx, end);
			encoding = xml_encoding_t::parse(value);
		}

	} while (false);
	
	doc->push_header(encoding, version);
	
	//string name = null;// = { 0 };
	//string value = null;

	beg += xml_skip_space(code, beg);
	if (code[beg] != '<')
		return error(0,"xml parsing error: bad string format"_sv);
	//beg = code.find("<"_sv, beg);
	while (beg != invalid_index)
	{
		c = 0;
		//Check if the node is a comment
		wsize find = code.find("<!--"_sv, beg, beg + 5);
		if (find == beg)
		{
			end = code.find("-->"_sv, beg);
			if (end != invalid_index)
			{
				//get comment value
				code.sub_string(value, beg + 4, end - 4);

				//insert comment
				doc->push_comment(value);
			}
			//Finding next node start position
			beg = code.find("<"_sv, end);
			continue;
		}

		//Check if the node is a CDATA
		find = code.find("<![CDATA["_sv, beg, beg + 10);
		if (find == beg)
		{
			end = code.find("]]>"_sv, beg);
			if (end != invalid_index)
			{
				//get CDATA value
				code.sub_string(value, beg + 9, end - 4);

				//insert comment
				doc->push_data(value);
			}
			//Finding next node start position
			beg = code.find("<"_sv, end);
			continue;
		}

		//checking if it is a node ending
		if (code[beg + 1] == U'/')
		{
			ixml_node_t current = doc->current_element();
			if (current.is_empty())//unkown error
			{
				//doc->m_last_parsing_error = "unknown Error"_sv;
				return error(0, "xml parsing error: unknown error"_sv);
			}
			beg += 2;

			//get element name
			bool same = true;
			char32 nC = 0;
			string temp = current->name();
			wsize length = current->name()->length();
			c = 0;
			while (same)
			{
				nC = code[beg + c];
				if (c > length)
					same = false;
				else if (c == length)
				{
					if (nC != U' ' && nC != U'\t' && nC != U'\r' && nC != U'\n' && nC != U'>')
						same = false;
					else
						break;
				}
				else if (nC != temp[c])
				{
					same = false;
				}
				c++;
			}

			if (!same)
			{
				astring parsing_error;
				parsing_error.format("xml parsing error : bad end of element <{0}> in position {1}"_sv, (astring)(cstr_t)current->name(), (ulong64)beg);
				return error(0, parsing_error);
			}//parsing error

			beg += c;

			find = code.find(">"_sv, beg);
			if (find == invalid_index)
			{
				astring parsing_error;
				parsing_error.format("xml parsing error : missing character > in position {0}"_sv, (ulong64)beg + current->name()->length());
				return error(0, parsing_error);
			}//parsing error
			beg = find;

			doc->end_element();
			beg = code.find("<"_sv, beg);
			continue;
		}

		//get element name
		c = xml_get_element_name(code, beg);
		code.sub_string(name, beg + 1, beg + c);

		doc->begin_element(name);
		beg += c;
		c = 0;

		//parse attributes
		while (code[beg] != U'>')
		{
			beg += xml_skip_space(code, beg);
			if (code[beg] == U'>')
			{
				break;
			}
			else if (code[beg] == U'/' && code[beg + 1] == U'>')
			{
				++beg;
				break;
			}

			c = xml_get_attibute_name(code, beg);
			code.sub_string(name, beg, beg + c);
			beg += c;
			c = 0;
			beg = code.find("="_sv, beg);
			if (beg == invalid_index)
			{
				astring parsing_error;
				parsing_error.format("xml parsing error : unexpected end of file parsing element {0}"_sv, name);
				return error(0, parsing_error);
			}

			++beg;

			beg += xml_skip_space(code, beg);
			c = 0;

			if (code[beg] == U'"')
			{
				end = code.find("\""_sv, beg + 1);
			}
			else if (code[beg] == U'\'')
			{
				end = code.find("\'"_sv, beg + 1);
			}
			else//parsing error
			{
				astring parsing_error;
				parsing_error.format("xml parsing error : unexpected character in position {0}"_sv, (ulong64)beg);
				return error(0, parsing_error);
			}
			if (end == invalid_index)//parsing error
			{
				astring parsing_error;
				parsing_error.format("xml parsing error : missing character \' or \" in position {0}"_sv, (ulong64)beg);
				return error(0, parsing_error);
			}

			code.sub_string(value, beg + 1, end);
			doc->push_attribute(name, value);
			beg = end + 1;

		}

		//if is not an empty node
		if (code[beg - 1] == U'/')
		{
			doc->end_element();
			beg = code.find("<"_sv, beg);
			continue;
		}
		else
		{
			++beg;

			c = xml_skip_space(code, beg);

			if (code[beg + c] == '<')
			{
				//checking if it is a node ending
				if (code[beg + c + 1] == '\'')
				{
					code.sub_string(value, beg, beg + c);
					doc->push_value(value);
					beg += c;

					ixml_node_t current = doc->current_element();
					if (current.is_empty())//unkown error
					{
						return error(0, "xml parsing error : unknown error"_sv);
					}
					beg += 2;

					//get element name
					bool same = true;
					char32_t _c = 0;
					string temp = current->name();
					wsize length = temp->length();
					c = 0;
					while (same)
					{
						_c = code[beg + c];
						if (c > length)
							same = false;
						else if (c == length)
						{
							if (_c != ' ' && _c != U'\t' && _c != U'\r' && _c != U'\n' && _c != U'>')
								same = false;
							else
								break;
						}
						else if (_c != temp[c])
						{
							same = false;
						}
						c++;
					}

					if (!same)
					{
						astring parsing_error;
						parsing_error.format("xml parsing error : bad end of element <{0}> in position {1}"_sv, (astring)(cstr_t)current->name(), (ulong64)beg);
						return error(0, parsing_error);
					}//parsing error

					beg += c;

					find = code.find(">"_sv, beg);
					if (find == invalid_index)
					{
						astring parsing_error;
						parsing_error.format("xml parsing error : missing character > in position {0}"_sv, (ulong64)beg + current->name()->length());
						return error(0, parsing_error);
					}//parsing error
					beg = find;
					doc->end_element();
					beg = code.find("<"_sv, beg);
					continue;
				}
				else
				{
					//parsing child
					beg += c;
					continue;
				}
			}
			else
			{
				end = code.find("<"_sv, beg + c);
				if (end == invalid_index)
				{
					astring parsing_error;
					parsing_error.format("xml parsing error : missing character < in position {0}"_sv, (ulong64)beg + c);
					return error(0, parsing_error);
				}//parsing error

				code.sub_string(value, beg, end);
				doc->push_value(value);
				beg = end;
				continue;
			}
		}
	}
	
	return ang::error_code::success;
}