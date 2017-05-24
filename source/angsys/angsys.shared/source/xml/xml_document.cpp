/*********************************************************************************************************************/
/*   File Name: xml_document.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/xml.h"
#include "xml_entity_values.h"

using namespace ang;
using namespace ang::xml;


////////////////////////////////////////////////////////////////////////

ang::object_wrapper<xml_builder>::object_wrapper()
	: _ptr(null)
{
}

ang::object_wrapper<xml_builder>::object_wrapper(xml_builder* ptr)
	: _ptr(null)
{
	set(ptr);
}

ang::object_wrapper<xml_builder>::object_wrapper(object_wrapper && other)
	: _ptr(null)
{
	xml_builder * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_builder>::object_wrapper(object_wrapper const& other)
	: _ptr(null)
{
	set(other._ptr);
}

ang::object_wrapper<xml_builder>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_builder>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<xml_builder>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<xml_builder>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<xml_builder>::is_empty()const
{
	return _ptr == null;
}

xml_builder* ang::object_wrapper<xml_builder>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_builder>::set(xml_builder* ptr)
{
	xml_builder * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<xml_builder>& ang::object_wrapper<xml_builder>::operator = (xml_builder* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_builder>& ang::object_wrapper<xml_builder>::operator = (ang::object_wrapper<xml_builder> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_builder>& ang::object_wrapper<xml_builder>::operator = (ang::object_wrapper<xml_builder> const& other)
{
	set(other._ptr);
	return*this;
}

xml_builder ** ang::object_wrapper<xml_builder>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<xml_builder> ang::object_wrapper<xml_builder>::operator& (void)
{
	return this;
}

xml_builder * ang::object_wrapper<xml_builder>::operator -> (void)
{
	return get();
}

xml_builder const* ang::object_wrapper<xml_builder>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_builder>::operator xml_builder * (void)
{
	return get();
}

ang::object_wrapper<xml_builder>::operator xml_builder const* (void)const
{
	return get();
}


////////////////////////////////////////////////////////////////////////////////

xml_document::xml_document()
	: _builder(null)
	, _last_parsing_error("")
{

}

xml_document::xml_document(core::files::input_text_file_t file)
	: _builder(null)
	, _last_parsing_error("")
{
	try { load(file); }
	catch (...) { }
}

xml_document::~xml_document()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_document, ang::object);

xml_builder_t xml_document::create()
{
	builder(new xml_builder());
	return builder();
}

void xml_document::load(core::files::input_text_file_t file)
{
	file->read([this](streams::itext_input_stream_t stream)->bool
	{
		ibuffer_t buff = interface_cast<ibuffer>(stream.get());
		if (buff.is_empty())
			return false;

		switch (stream->text_encoding())
		{
		case text::encoding::ascii: {
			cstr_t code((char*)buff->buffer_ptr(), buff->buffer_size());
			parse(code);
			break; }
		case text::encoding::unicode: {
			cwstr_t code((wchar*)buff->buffer_ptr(), buff->buffer_size());
			parse(code);
			break; }
		case text::encoding::utf_8: {
			cmstr_t code((mchar*)buff->buffer_ptr(), buff->buffer_size());
			parse(code);
			break; }
		default:
			break;
		}

		return true;
	});
}


void xml_document::save(core::files::output_text_file_t file)const
{
	if (!builder())
		return;

	auto header = builder()->xml_head()->xml_as<xml_header>();
	if (header == null)
	{
		header = new xml_header();
		builder()->push_header(header);
	}

	wstring text;
	xml_node_t node = header.get();
	while (node.get())
	{
		node->xml_print(text, xml_format::fix_entity + xml_format::wrap_text_space);
		text << "\n";
		node = node->xml_next();
	}
	file->format(header->encoding());
	file->write((cwstr_t)text);

	//file->format(header->encoding());
	//file->write([&](streams::itext_output_stream_t stream)->bool
	//{
	//	xml_node_t node = header;
	//	while (node.get())
	//	{
	//		node->xml_print(stream, xml_format::fix_entity + xml_format::wrap_text_space);
	//		node = node->xml_next();
	//	}	
	//	//file->write(text);
	//	return true;
	//}, 0, 128);
}

void xml_document::serialize(streams::itext_output_stream_t stream)const
{
	/*if (!builder())
	{
		auto encoding = stream->text_encoding();
		auto header = new xml_header();
		header->encoding(encoding);
		header->xml_print(stream, xml_format::fix_entity + xml_format::wrap_text_space);
		stream << "\n";
		return;
	}

	auto header = builder()->xml_head()->xml_as<xml_header>();

	if (header == null)
	{
		auto encoding = stream->text_encoding();
		header = new xml_header();
		builder()->push_header(header);
		header->encoding(encoding);
		header->xml_print(stream, xml_format::fix_entity + xml_format::wrap_text_space);
		stream << "\n";
	}
	else
	{
		auto encoding = stream->text_encoding();
		if (header->encoding() != encoding.get())
		{
			auto _header = header->xml_clone()->as<xml_header>();
			_header->encoding(encoding);
			_header->xml_print(stream, xml_format::fix_entity + xml_format::wrap_text_space);
			stream << "\n";
		}
		else
		{
			header->xml_print(stream, xml_format::fix_entity + xml_format::wrap_text_space);
			stream << "\n";
		}
	}
	xml_node_t node = header.get();
	node = node->xml_next();
	while (node != null)
	{
		node->xml_print(stream, xml_format::fix_entity + xml_format::wrap_text_space);
		stream << "\n";
		node = node->xml_next();
	}*/

}


void xml_document::parse(string code)
{
	parse((cstr_t)code);
}

void xml_document::parse(wstring code)
{
	parse((cwstr_t)code);
}

void xml_document::parse(mstring code)
{
	parse((cmstr_t)code);
}

void xml_document::parse(cstr_t code)
{
	if (code.size() < 4)
		throw(exception_t((dword)xml_exception_code::parsing_error, "invalid input"));
	mbyte bum;
	bum.bytes[0] = code[0];
	bum.bytes[1] = code[1];
	bum.bytes[2] = code[2];
	if (bum.value == mbyte::inv_mbom)
	{
		return parse(cmstr_t((mchar const*)&code.cstr()[3], code.size() - 3));
	}

	_last_parsing_error = "success"_s;
	create();
	index begin = 0;

	if (!decode_header(code,  begin))
	{
		builder()->push_header(new xml_header());
		begin = 0;
	}
	if (!decode_element(code, begin))
	{
		builder(null);
		string error = "";
		error->format("exception: ang::xml::xml_document: %s"_s, (char const*)(cstr_t)_last_parsing_error);
		throw(exception_t((dword)xml_exception_code::parsing_error, ang::move(error)));
	}
}

void xml_document::parse(cwstr_t code)
{
	if (code.size() < 4)
		throw(exception_t((dword)xml_exception_code::parsing_error, "invalid input"_s));

	_last_parsing_error = "success"_s;
	create();
	index begin = 0;
	if (!decode_header(code, begin))
	{
		builder()->push_header(new xml_header());
		begin = 0;
	}
	if (!decode_element(code, begin))
	{
		builder(null);
		string error = "";
		error->format("exception: ang::xml::xml_document: %s", (char const*)(cstr_t)_last_parsing_error);
		throw(exception_t((dword)xml_exception_code::parsing_error, ang::move(error)));
	}
}

void xml_document::parse(cmstr_t code)
{
	if (code.size() < 4)
		throw(exception_t((dword)xml_exception_code::parsing_error, "invalid input"_s));

	mbyte bum;
	bum.bytes[0] = code[0];
	bum.bytes[1] = code[1];
	bum.bytes[2] = code[2];
	if (bum.value == mbyte::inv_mbom)
	{
		code.set(&code.get()[3], code.size() - 3);
	}

	_last_parsing_error = "success"_s;
	create();
	index begin = 0;
	if (!decode_header(code, begin))
	{
		builder()->push_header(new xml_header());
		begin = 0;
	}
	if (!decode_element(code, begin))
	{
		builder(null);
		string error = "";
		error->format("exception: ang::xml::xml_document: %s"_s, (char const*)(cstr_t)_last_parsing_error);
		throw(exception_t((dword)xml_exception_code::parsing_error, ang::move(error)));
	}
}

string xml_document::last_parsing_error()const
{
	return _last_parsing_error.get();
}

xml_element_t xml_document::xml_root()const
{
	if (builder() != null && builder()->xml_root() != null)
		return builder()->xml_root()->xml_as<xml_element>();
	return null;
}

xml_tree_t xml_document::xml_tree()const
{
	return builder().get();
}

xml_builder_t xml_document::builder()const
{
	return _builder;
}

void xml_document::builder(xml_builder_t value)
{
	_builder = value;
}

bool xml_document::decode_header(cstr_t code, index& begin)
{
	wstring value;
	wstring header = "";
	collections::iterator<wchar> beg;
	collections::iterator<wchar> end;

	begin = code.find("<?xml", 0u, 6u);
	if (begin == invalid_index)
		return false;

	auto _end = code.find("?>", begin);
	if (_end == invalid_index)
		return false;

	code.sub_string(header, begin, _end);
	begin = _end + 2;

	xml_header_t _header = new xml_header();

	beg = header->find("version", header->begin());
	if (beg.is_valid())
	{
		beg = header->find("\"", beg);
		beg++;
		end = header->find(L"\"", beg);
		header->sub_string(value, beg, end);
		_header->version(ang::move(value));
	}

	beg = header->find("encoding", beg);
	if (beg.is_valid())
	{
		beg = header->find("\"", beg);
		beg++;
		end = header->find("\"", beg);
		header->sub_string(value, beg, end);
		_header->encoding(value);
	}
	builder()->push_header(_header);
	return true;
}

bool xml_document::decode_header(cwstr_t code, index& begin)
{
	wstring value;
	wstring header = "";
	collections::iterator<wchar> beg;
	collections::iterator<wchar> end;

	begin = code.find("<?xml", 0u, 6u);
	if (begin == invalid_index)
		return false;

	auto _end = code.find("?>", begin);
	if (_end == invalid_index)
		return false;

	code.sub_string(header, begin, _end);
	begin = _end + 2;

	xml_header_t _header = new xml_header();

	beg = header->find("version", header->begin());
	if (beg.is_valid())
	{
		beg = header->find("\"", beg);
		beg++;
		end = header->find(L"\"", beg);
		header->sub_string(value, beg, end);
		_header->version(ang::move(value));
	}

	beg = header->find(L"encoding", beg);
	if (beg.is_valid())
	{
		beg = header->find(L"\"", beg);
		beg++;
		end = header->find(L"\"", beg);
		header->sub_string(value, beg, end);
		_header->encoding(value);
	}
	builder()->push_header(_header);
	return true;
}

bool xml_document::decode_header(cmstr_t code, index& begin)
{
	wstring value;
	wstring header = "";
	collections::iterator<wchar> beg;
	collections::iterator<wchar> end;

	begin = code.find("<?xml", 0u, 6u);
	if (begin == invalid_index)
		return false;

	auto _end = code.find("?>", begin);
	if (_end == invalid_index)
		return false;

	code.sub_string(header, begin, _end);
	begin = _end + 2;

	xml_header_t _header = new xml_header();

	beg = header->find("version", header->begin());
	if (beg.is_valid())
	{
		beg = header->find("\"", beg);
		beg++;
		end = header->find(L"\"", beg);
		header->sub_string(value, beg, end);
		_header->version(ang::move(value));
	}

	beg = header->find(L"encoding", beg);
	if (beg.is_valid())
	{
		beg = header->find(L"\"", beg);
		beg++;
		end = header->find(L"\"", beg);
		header->sub_string(value, beg, end);
		_header->encoding(value);
	}
	builder()->push_header(_header);
	return true;
}

#if defined _DEBUG
bool xml_document::decode_element(cstr_t code, index& __begin)
#else
bool xml_document::decode_element(cstr_t code, index& begin)
#endif
{
#if defined _DEBUG
	xml_cstr_iterator<char> begin(__begin, code);
#endif

	index end;
	uint c = 0;
	wstring name = null;// = { 0 };
	wstring value = null;

	begin = code.find("<", begin);
	while (begin != invalid_index)
	{
		c = 0;
		//Check if the node is a comment
		auto find = code.find("<!--", begin, begin + 5);
		if (find == begin)
		{
			end = code.find("-->", begin);
			if (end != invalid_index)
			{
				//get comment value
				code.sub_string(value, begin + 4, end - 4 - begin);
				//insert comment
				builder()->comment(value.get());
			}
			//Finding next node start position
			begin = code.find( "<", end);
			continue;
		}

		//checking if it is a node ending
		if (code[begin + 1] == '/')
		{
			xml_element_t current = builder()->xml_current();
			if (current == null)//unkown error
			{
				_last_parsing_error = "unknown Error";
				return false;
			}
			begin += 2;

			//get element name
			bool same = true;
			wchar nC = 0;
			cwstr_t temp = current->xml_name()->cstr();
			wsize length = current->xml_name()->length();
			c = 0;
			while (same)
			{
				nC = code[begin + c];
				if (c > length)
					same = false;
				else if (c == length)
				{
					if (nC != ' ' && nC != '\t' && nC != '\r' && nC != '\n' && nC != '>')
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
				_last_parsing_error->format("bad end of element <%ls> in position %u", (wchar const*)(cwstr_t)current->xml_name(), (index)begin);
				return false;
			}//parsing error

			begin += c;

			find = code.find( ">", begin);
			if (find == invalid_index)
			{
				_last_parsing_error->format("missing character > in position %u", (index)begin + current->xml_name()->length());
				return false;
			}//parsing error
			begin = find;

			builder()->end_element();
			begin = code.find( "<", begin);
			continue;
		}

		//get element name
		name.clean();
		c = xml_get_element_name<char>(code, begin);
		code.sub_string(name, begin + 1, c - 1);

		builder()->begin_element(name.get());
		begin += c;
		c = 0;

		//parse attributes
		while (code[begin] != L'>')
		{
			begin += xml_skip_space<char>(code, begin);
			if (code[begin] == L'>')
			{
				break;
			}
			else if (code[begin] == L'/' && code[begin + 1] == L'>')
			{
				++begin;
				break;
			}

			name.clean();
			c = xml_get_attibute_name<char>(code, begin);
			code.sub_string(name, begin, c);
			begin += c;
			c = 0;
			begin = code.find( "=", begin);
			if (begin == invalid_index)
			{
				_last_parsing_error->format("unexpected end of file parsing element %ls", (wchar const*)(cwstr_t)name);
				return false;
			}

			++begin;

			begin += xml_skip_space<char>(code, begin);
			c = 0;

			if (code[begin] == L'"')
			{
				end = code.find( "\"", begin + 1);
			}
			else if (code[begin] == L'\'')
			{
				end = code.find( "\'", begin + 1);
			}
			else//parsing error
			{
				_last_parsing_error->format("unexpected character in position %u", (index)begin);
				return false;
			}
			if (end == invalid_index)//parsing error
			{
				_last_parsing_error->format("missing character \' or \" in position %u", (index)begin);
				return false;
			}

			value.clean();
			code.sub_string(value, begin + 1, end - 1 - begin);
			builder()->attribute(name.get(), value.get());
			begin = end + 1;

		}

		//if is not an empty node
		if (code[begin - 1] == L'/')
		{
			builder()->end_element();
			begin = code.find( "<", begin);
			continue;
		}
		else
		{
			++begin;

			c = xml_skip_space<char>(code, begin);

			if (code[begin + c] == '<')
			{
				//checking if it is a node ending
				if (code[begin + c + 1] == '\'')
				{
					value.clean();
					code.sub_string(value, begin, c);
					builder()->value(value.get());
					begin += c;

					xml_element* current = builder()->xml_current();
					if (current == null)//unkown error
					{
						_last_parsing_error = "unknown Error";
						return false;
					}
					begin += 2;

					//get element name
					bool same = true;
					wchar _c = 0;
					cwstr_t temp = current->xml_name();
					wsize length = current->xml_name()->length();
					c = 0;
					while (same)
					{
						_c = code[begin + c];
						if (c > length)
							same = false;
						else if (c == length)
						{
							if (_c != ' ' && _c != '\t' && _c != '\r' && _c != '\n' && _c != '>')
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
						_last_parsing_error->format("bad end of element <%ls> in position %u", (wchar const*)(cwstr_t)current->xml_name(), (index)begin);
						return false;
					}//parsing error

					begin += c;

					find = code.find( ">", begin);
					if (find == invalid_index)
					{
						_last_parsing_error->format("missing character > in position %u", (index)begin + current->xml_name()->length());
						return false;
					}//parsing error
					begin = find;
					builder()->end_element();
					begin = code.find( "<", begin);
					continue;
				}
				else
				{
					//parsing child
					begin += c;
					continue;
				}
			}
			else
			{
				end = code.find( "<", begin + c);
				if (end == invalid_index)
				{
					_last_parsing_error->format("missing character < in position %u", (index)begin + c);
					return false;
				}//parsing error
				value.clean();
				code.sub_string(value, begin, end - begin);
				builder()->value(value.get());
				begin = end;
				continue;
			}
		}
	}

	return true;
}


#if defined _DEBUG
bool xml_document::decode_element(cwstr_t code, index& __begin)
#else
bool xml_document::decode_element(cwstr_t code, index& begin)
#endif
{
#if defined _DEBUG
	xml_cstr_iterator<wchar> begin(__begin, code);
#endif

	index end;
	uint c = 0;
	wstring name = null;// = { 0 };
	wstring value = null;

	begin = code.find("<", begin);
	while (begin != invalid_index)
	{
		c = 0;
		//Check if the node is a comment
		auto find = code.find("<!--", begin, begin + 5);
		if (find == begin)
		{
			end = code.find("-->", begin);
			if (end != invalid_index)
			{
				//get comment value
				code.sub_string(value, begin + 4, end - 4 - begin);
				//insert comment
				builder()->comment(value.get());
			}
			//Finding next node start position
			begin = code.find("<", end);
			continue;
		}

		//checking if it is a node ending
		if (code[begin + 1] == '/')
		{
			xml_element_t current = builder()->xml_current();
			if (current == null)//unkown error
			{
				_last_parsing_error = "unknown Error";
				return false;
			}
			begin += 2;

			//get element name
			bool same = true;
			wchar nC = 0;
			cwstr_t temp = current->xml_name();
			wsize length = current->xml_name()->length();
			c = 0;
			while (same)
			{
				nC = code[begin + c];
				if (c > length)
					same = false;
				else if (c == length)
				{
					if (nC != ' ' && nC != '\t' && nC != '\r' && nC != '\n' && nC != '>')
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
				_last_parsing_error->format("bad end of element <%ls> in position %u", (wchar const*)(cwstr_t)current->xml_name(), (index)begin);
				return false;
			}//parsing error

			begin += c;

			find = code.find(">", begin);
			if (find == invalid_index)
			{
				_last_parsing_error->format("missing character > in position %u", (index)begin + current->xml_name()->length());
				return false;
			}//parsing error
			begin = find;

			builder()->end_element();
			begin = code.find("<", begin);
			continue;
		}

		//get element name
		name.clean();
		c = xml_get_element_name<wchar>(code, begin);
		code.sub_string(name, begin + 1, c - 1);

		builder()->begin_element(name.get());
		begin += c;
		c = 0;

		//parse attributes
		while (code[begin] != L'>')
		{
			begin += xml_skip_space<wchar>(code, begin);
			if (code[begin] == L'>')
			{
				break;
			}
			else if (code[begin] == L'/' && code[begin + 1] == L'>')
			{
				++begin;
				break;
			}

			name.clean();
			c = xml_get_attibute_name<wchar>(code, begin);
			code.sub_string(name, begin, c);
			begin += c;
			c = 0;
			begin = code.find("=", begin);
			if (begin == invalid_index)
			{
				_last_parsing_error->format("unexpected end of file parsing element %ls", (wchar const*)(cwstr_t)name);
				return false;
			}

			++begin;

			begin += xml_skip_space<wchar>(code, begin);
			c = 0;

			if (code[begin] == L'"')
			{
				end = code.find("\"", begin + 1);
			}
			else if (code[begin] == L'\'')
			{
				end = code.find("\'", begin + 1);
			}
			else//parsing error
			{
				_last_parsing_error->format("unexpected character in position %u", (index)begin);
				return false;
			}
			if (end == invalid_index)//parsing error
			{
				_last_parsing_error->format("missing character \' or \" in position %u", (index)begin);
				return false;
			}

			value.clean();
			code.sub_string(value, begin + 1, end - begin - 1);
			builder()->attribute(name.get(), value.get());
			begin = end + 1;

		}

		//if is not an empty node
		if (code[begin - 1] == L'/')
		{
			builder()->end_element();
			begin = code.find("<", begin);
			continue;
		}
		else
		{
			++begin;

			c = xml_skip_space<wchar>(code, begin);

			if (code[begin + c] == '<')
			{
				//checking if it is a node ending
				if (code[begin + c + 1] == '\'')
				{
					value.clean();
					code.sub_string(value, begin, c);
					builder()->value(value.get());
					begin += c;

					xml_element* current = builder()->xml_current();
					if (current == null)//unkown error
					{
						_last_parsing_error = "unknown Error";
						return false;
					}
					begin += 2;

					//get element name
					bool same = true;
					wchar _c = 0;
					cwstr_t temp = current->xml_name();
					wsize length = current->xml_name()->length();
					c = 0;
					while (same)
					{
						_c = code[begin + c];
						if (c > length)
							same = false;
						else if (c == length)
						{
							if (_c != ' ' && _c != '\t' && _c != '\r' && _c != '\n' && _c != '>')
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
						_last_parsing_error->format("bad end of element <%ls> in position %u", (wchar const*)(cwstr_t)current->xml_name(), (index)begin);
						return false;
					}//parsing error

					begin += c;

					find = code.find(">", begin);
					if (find == invalid_index)
					{
						_last_parsing_error->format("missing character > in position %u", (index)begin + current->xml_name()->length());
						return false;
					}//parsing error
					begin = find;
					builder()->end_element();
					begin = code.find("<", begin);
					continue;
				}
				else
				{
					//parsing child
					begin += c;
					continue;
				}
			}
			else
			{
				end = code.find("<", begin + c);
				if (end == invalid_index)
				{
					_last_parsing_error->format("missing character < in position %u", (index)begin + c);
					return false;
				}//parsing error
				value.clean();
				code.sub_string(value, begin, end - begin);
				builder()->value(value.get());
				begin = end;
				continue;
			}
		}
	}

	return true;
}


#if defined _DEBUG
bool xml_document::decode_element(cmstr_t code, index& __begin)
#else
bool xml_document::decode_element(cmstr_t code, index& begin)
#endif
{
#if defined _DEBUG
	xml_cstr_iterator<mchar> begin(__begin, code);
#endif

	index end;
	uint c = 0;
	wstring name = null;// = { 0 };
	wstring value = null;

	begin = code.find("<", begin);
	while (begin != invalid_index)
	{
		c = 0;
		//Check if the node is a comment
		auto find = code.find("<!--", begin, begin + 5);
		if (find == begin)
		{
			end = code.find("-->", begin);
			if (end != invalid_index)
			{
				//get comment value
				code.sub_string(value, begin + 4, end - 4 - begin);
				//insert comment
				builder()->comment(value.get());
			}
			//Finding next node start position
			begin = code.find("<", end);
			continue;
		}

		//checking if it is a node ending
		if (code[begin + 1] == '/')
		{
			xml_element_t current = builder()->xml_current();
			if (current == null)//unkown error
			{
				_last_parsing_error = "unknown Error";
				return false;
			}
			begin += 2;

			//get element name
			bool same = true;
			wchar nC = 0;
			cwstr_t temp = current->xml_name();
			wsize length = current->xml_name()->length();
			c = 0;
			while (same)
			{
				nC = code[begin + c];
				if (c > length)
					same = false;
				else if (c == length)
				{
					if (nC != ' ' && nC != '\t' && nC != '\r' && nC != '\n' && nC != '>')
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
				_last_parsing_error->format("bad end of element <%ls> in position %u", (wchar const*)(cwstr_t)current->xml_name(), (index)begin);
				return false;
			}//parsing error

			begin += c;

			find = code.find(">", begin);
			if (find == invalid_index)
			{
				_last_parsing_error->format("missing character > in position %u", (index)begin + current->xml_name()->length());
				return false;
			}//parsing error
			begin = find;

			builder()->end_element();
			begin = code.find("<", begin);
			continue;
		}

		//get element name
		name.clean();
		c = xml_get_element_name<mchar>(code, begin);
		code.sub_string(name, begin + 1, c - 1);

		builder()->begin_element(name.get());
		begin += c;
		c = 0;

		//parse attributes
		while (code[begin] != L'>')
		{
			begin += xml_skip_space<mchar>(code, begin);
			if (code[begin] == L'>')
			{
				break;
			}
			else if (code[begin] == L'/' && code[begin + 1] == L'>')
			{
				++begin;
				break;
			}

			name.clean();
			c = xml_get_attibute_name<mchar>(code, begin);
			code.sub_string(name, begin, c);
			begin += c;
			c = 0;
			begin = code.find("=", begin);
			if (begin == invalid_index)
			{
				_last_parsing_error->format("unexpected end of file parsing element %ls", (wchar const*)(cwstr_t)name);
				return false;
			}

			++begin;

			begin += xml_skip_space<mchar>(code, begin);
			c = 0;

			if (code[begin] == L'"')
			{
				end = code.find("\"", begin + 1);
			}
			else if (code[begin] == L'\'')
			{
				end = code.find("\'", begin + 1);
			}
			else//parsing error
			{
				_last_parsing_error->format("unexpected character in position %u", (index)begin);
				return false;
			}
			if (end == invalid_index)//parsing error
			{
				_last_parsing_error->format("missing character \' or \" in position %u", (index)begin);
				return false;
			}

			value.clean();
			code.sub_string(value, begin + 1, end - begin - 1);
			builder()->attribute(name.get(), value.get());
			begin = end + 1;

		}

		//if is not an empty node
		if (code[begin - 1] == L'/')
		{
			builder()->end_element();
			begin = code.find("<", begin);
			continue;
		}
		else
		{
			++begin;

			c = xml_skip_space<mchar>(code, begin);

			if (code[begin + c] == '<')
			{
				//checking if it is a node ending
				if (code[begin + c + 1] == '\'')
				{
					value.clean();
					code.sub_string(value, begin, c);
					builder()->value(value.get());
					begin += c;

					xml_element* current = builder()->xml_current();
					if (current == null)//unkown error
					{
						_last_parsing_error = "unknown Error";
						return false;
					}
					begin += 2;

					//get element name
					bool same = true;
					wchar _c = 0;
					cwstr_t temp = current->xml_name();
					wsize length = current->xml_name()->length();
					c = 0;
					while (same)
					{
						_c = code[begin + c];
						if (c > length)
							same = false;
						else if (c == length)
						{
							if (_c != ' ' && _c != '\t' && _c != '\r' && _c != '\n' && _c != '>')
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
						_last_parsing_error->format("bad end of element <%ls> in position %u", (wchar const*)(cwstr_t)current->xml_name(), (index)begin);
						return false;
					}//parsing error

					begin += c;

					find = code.find(">", begin);
					if (find == invalid_index)
					{
						_last_parsing_error->format("missing character > in position %u", (index)begin + current->xml_name()->length());
						return false;
					}//parsing error
					begin = find;
					builder()->end_element();
					begin = code.find("<", begin);
					continue;
				}
				else
				{
					//parsing child
					begin += c;
					continue;
				}
			}
			else
			{
				end = code.find("<", begin + c);
				if (end == invalid_index)
				{
					_last_parsing_error->format("missing character < in position %u", (index)begin + c);
					return false;
				}//parsing error
				value.clean();
				code.sub_string(value, begin, end - begin);
				builder()->value(value.get());
				begin = end;
				continue;
			}
		}
	}

	return true;
}




